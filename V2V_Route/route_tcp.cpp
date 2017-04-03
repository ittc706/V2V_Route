#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include"route_tcp.h"
#include"context.h"
#include"config.h"
#include"gtt.h"
#include"vue.h"
#include"vue_physics.h"
#include"function.h"
using namespace std;

int route_tcp_event::s_event_count = 0;

void route_tcp_event::transimit() {
	//<Warn>:һ������ָ��Ƶ�δ��䣬���ٷֳɶ�����ֱ�ѡ��Ƶ�δ�����
	
	if (++m_package_idx == m_package_num) {
		m_is_curlink_finished = true;
		if (m_type != RELAY) throw logic_error("error");
		m_through_node_vec.push_back(m_current_node);
	}
	double sinr = -1;
	//<Warn>:sinr��������䣬�Լ��Ƿ񶪰��ֶε�ά��

}

void route_tcp_event::reset() {
	m_package_idx = 0;
	m_is_curlink_finished = false;
	m_is_curlink_loss = false;
}

route_tcp_event* route_tcp_event::clone() {
	route_tcp_event* clone_event = new route_tcp_event(get_origin_source_node(),get_final_destination_node());
	clone_event->m_through_node_vec = m_through_node_vec;
	clone_event->m_type = RELAY;
	clone_event->m_source_node_id = this->get_current_node()->get_id();
	return clone_event;
}

std::string route_tcp_event::to_string() {
	stringstream ss;
	for (int i = 0; i < m_through_node_vec.size();i++) {
		ss << "node[" << left << setw(3) << m_through_node_vec[i]->get_id() << "]";
		if (i < m_through_node_vec.size() - 1)ss << " -> ";
	}
	ss << endl;
	return ss.str();
}

int route_tcp_node::s_node_count = 0;

default_random_engine route_tcp_node::s_engine;

ofstream route_tcp_node::s_logger;

void route_tcp_node::log(route_tcp_node* t_node) {
	s_logger << "TTI[" << left << setw(3) << context::get_context()->get_tti() << "] - ";
	s_logger << "node[" << left << setw(3) << t_node->get_id() << "] - ";
	s_logger << "curr_state[" << left << setw(20) << route_tcp::state_to_string(t_node->get_cur_state()) << "] - ";
	s_logger << "next_state[";
	for (route_tcp_node_state state : t_node->m_next_posible_state_set) {
		s_logger << left << setw(20) << route_tcp::state_to_string(state);
	}
	s_logger << "]" << endl;
}

std::vector<std::set<route_tcp_node*>> route_tcp_node::s_node_per_pattern;

void route_tcp_node::transfer_event_from_relay_queue_to_source_queue() {
	m_relay_event_queue.front()->set_type(SOURCE);
	add_source_event(m_relay_event_queue.front());
	m_relay_event_queue.pop();
}

void route_tcp_node::update_state() {
	route_tcp_node_state max_state = IDLE;

	log(this);

	check_state();

	for (route_tcp_node_state state : m_next_posible_state_set) {
		if (state > max_state) {
			max_state = state;
		}
	}



	if (max_state == IDLE) {
		if (get_source_event_queue().size() != 0) {
			max_state = SOURCE_SEND_SYN;
		}
	}

	if (max_state == SOURCE_SEND_SYN) {
		if (get_source_event_queue().size() == 0) {
			cout << "node[" << get_id() << "] possible list: ";
			for (int i : m_next_posible_state_set)
				cout << i << ",";
			cout << endl;
		}
	}

	set_cur_state(max_state);
	m_next_posible_state_set.clear();
}

int route_tcp_node::source_select_relay_node() {
	int res = -1;

	int final_destination_node_id = get_source_event_queue().front()->get_final_destination_node()->get_id();

	double min_distance = (numeric_limits<double>::max)();
	for (int near_node_id : m_adjacent_list) {
		int cur_distance = vue_physics::get_distance(near_node_id, final_destination_node_id);
		if (cur_distance< min_distance) {
			min_distance = cur_distance;
			res = near_node_id;
		}
	}

	m_source_syn_node = res;//ά����״̬����ɾ
	return res;
}

std::pair<int, std::vector<int>> route_tcp_node::relay_response_ack() {
	/*
	* ���۽ڵ㴦�ں���״̬�������봦�����syn�����б����Ҵ��������һʱ�̵������б�
	*/
	int select_node_id = -1;
	vector<int> reject_vec;

	if (get_cur_state() != RELAY_SEND_ACK) {
		reject_vec = m_relay_pre_syn_node_vec;
	}
	else {
		uniform_int_distribution<int> u(0, m_relay_pre_syn_node_vec.size() - 1);

		select_node_id = m_relay_pre_syn_node_vec[u(s_engine)];

		for (int node_id : m_relay_pre_syn_node_vec) {
			if (select_node_id != node_id) {
				reject_vec.push_back(node_id);
			}
		}
	}

	m_relay_pre_syn_node_vec = m_relay_syn_node_vec;//����ǰtti�յ���syn�б����ת�ƣ�������һ��tti�Żᴦ��
	m_relay_syn_node_vec.clear();//����ת���ˣ������ռ���

	return std::pair<int, std::vector<int>>(select_node_id, reject_vec);
}

void route_tcp_node::check_state() {
	if (get_cur_state() == IDLE) {
		if (m_next_posible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_SENDING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_RECEIVING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_LINK_RESPONSE) != m_next_posible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == SOURCE_SEND_SYN) {
		if (m_next_posible_state_set.find(IDLE) != m_next_posible_state_set.end() ||
			//m_next_posible_state_set.find(SOURCE_SEND_SYN) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_SENDING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_RECEIVING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_LINK_RESPONSE) != m_next_posible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == SOURCE_RECEIVE_ACK) {
		if (m_next_posible_state_set.find(IDLE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_RECEIVING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_LINK_RESPONSE) != m_next_posible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == SOURCE_SENDING) {
		if (m_next_posible_state_set.find(IDLE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_SEND_SYN) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_RECEIVING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_LINK_RESPONSE) != m_next_posible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == SOURCE_LINK_RESPONSE) {
		if (m_next_posible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_SENDING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_RECEIVING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_LINK_RESPONSE) != m_next_posible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == RELAY_SEND_ACK) {
		if (m_next_posible_state_set.find(IDLE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_SEND_SYN) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_SENDING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_LINK_RESPONSE) != m_next_posible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == RELAY_RECEIVING) {
		if (m_next_posible_state_set.find(IDLE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_SEND_SYN) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_SENDING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_posible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == RELAY_LINK_RESPONSE) {
		if (m_next_posible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_SENDING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_RECEIVING) != m_next_posible_state_set.end() ||
			m_next_posible_state_set.find(RELAY_LINK_RESPONSE) != m_next_posible_state_set.end())
			throw logic_error("state_error");
	}
}

default_random_engine route_tcp::s_engine;

ofstream route_tcp::s_logger;

string route_tcp::state_to_string(route_tcp_node_state state) {
	switch (state) {
	case IDLE:
		return "IDLE";
	case SOURCE_SEND_SYN:
		return "SOURCE_SEND_SYN";
	case SOURCE_RECEIVE_ACK:
		return "SOURCE_RECEIVE_ACK";
	case SOURCE_SENDING:
		return "SOURCE_SENDING";
	case SOURCE_LINK_RESPONSE:
		return "SOURCE_LINK_RESPONSE";
	case RELAY_SEND_ACK:
		return "RELAY_SEND_ACK";
	case RELAY_RECEIVING:
		return "RELAY_RECEIVING";
	case RELAY_LINK_RESPONSE:
		return "RELAY_LINK_RESPONSE";
	}
}

void route_tcp::log(int source_node_id, int relay_node_id, route_event_type event_type, route_tcp_node_state cur_state, route_tcp_node_state next_state, string description) {
	int cur_node_id = event_type == SOURCE ? source_node_id : relay_node_id;
	s_logger << "TTI[" << left << setw(3) << context::get_context()->get_tti() << "] - ";
	s_logger << "link[" << left << setw(3) << source_node_id << ", ";
	s_logger << left << setw(3) << relay_node_id << "] - ";
	s_logger << "node[" << left << setw(3) << cur_node_id << "] - ";
	s_logger << "curr_state[" << left << setw(20) << state_to_string(cur_state) << "] - ";
	s_logger << "next_state[" << left << setw(20) << state_to_string(next_state) << "] - ";
	s_logger << description << endl;
}

route_tcp::route_tcp() {

}

void route_tcp::initialize() {
	context* __context = context::get_context();
	int vue_num = __context->get_gtt()->get_vue_num();
	m_node_array = new route_tcp_node[vue_num];

	if (__context->get_global_control_config()->get_platform() == Windows) {
		s_logger.open("log\\route_tcp_log.txt");
		route_tcp_node::s_logger.open("log\\route_tcp_node_state_log.txt");
	}
	else {
		s_logger.open("log/route_tcp_log.txt");
		route_tcp_node::s_logger.open("log/route_tcp_node_state_log.txt");
	}
}

void route_tcp::process_per_tti() {
	//�¼�����
	event_trigger();

	//���½ڵ�״̬
	update_node_state();

	//������Щ����syn״̬�Ľڵ�
	process_syn_connection();

	//������Щ��Ҫ�ظ�ack�źŵĽڵ�
	process_ack_connection();

	//������Щ���ڴ���״̬�Ľڵ�
	process_transimit_connection();

	//������Щ���ڻظ������־�Ľڵ�
	process_response_connection();
}

void route_tcp::update_route_table_from_physics_level() {
	//���֮ǰ���ڽӱ�
	for (int node_id = 0; node_id < route_tcp_node::s_node_count; node_id++) {
		get_node_array()[node_id].m_adjacent_list.clear();
	}

	//<Warn>:��ʱ��Ϊ���ݾ���ȷ���ڽӱ�
	context* __context = context::get_context();
	vue* vue_ary = __context->get_vue_array();
	int vue_num = __context->get_gtt()->get_vue_num();
	for (int vue_id_i = 0; vue_id_i < vue_num; vue_id_i++) {
		for (int vue_id_j = 0; vue_id_j < vue_num; vue_id_j++) {
			if (vue_id_i == vue_id_j)continue;
			if (vue_physics::get_distance(vue_id_i, vue_id_j) < 500) {
				get_node_array()[vue_id_i].add_to_adjacent_list(vue_id_j);
				get_node_array()[vue_id_j].add_to_adjacent_list(vue_id_i);
			}
		}
	}
}

void route_tcp::event_trigger() {
	context* __context = context::get_context();
	double trigger_rate = __context->get_tmc_config()->get_trigger_rate();

	uniform_real_distribution<double> u_rate(0, 1);
	uniform_int_distribution<int> u_node_id(0, route_tcp_node::s_node_count - 1);

	for (int origin_source_node_id = 0; origin_source_node_id < route_tcp_node::s_node_count; origin_source_node_id++) {
		if (u_rate(s_engine) < trigger_rate) {
			route_tcp_node* origin_source_node = &get_node_array()[origin_source_node_id];

			int final_destination_node_id = origin_source_node_id;
			while (final_destination_node_id == origin_source_node_id) {
				final_destination_node_id = u_node_id(s_engine);
			}

			route_tcp_node* final_destination_node= &get_node_array()[final_destination_node_id];

			get_node_array()[origin_source_node_id].add_source_event(
				new route_tcp_event(origin_source_node, final_destination_node)
				);
			s_logger << "TTI: " << left << setw(3) << context::get_context()->get_tti() << ",  trigger[" << left << setw(3) << origin_source_node_id << ", " << left << setw(3) << final_destination_node_id << "]" << endl;

		}
	}
}

void route_tcp::process_syn_connection() {
	//����SYN�����ڱ���tti�ڣ����SYN�ķ����Լ�����
	for (int source_node_id = 0; source_node_id < route_tcp_node::s_node_count; source_node_id++) {
		if (get_node_array()[source_node_id].get_cur_state() == SOURCE_SEND_SYN) {
			int relay_node_id = get_node_array()[source_node_id].source_select_relay_node();
			if (relay_node_id == -1) {
				get_node_array()[source_node_id].add_next_posible_state(SOURCE_SEND_SYN);
				continue;
			}
			get_node_array()[relay_node_id].add_relay_syn_node(source_node_id);

			//����source-relay�ڵ����һ�̿���״̬
			get_node_array()[source_node_id].add_next_posible_state(SOURCE_RECEIVE_ACK);
			log(source_node_id, relay_node_id, SOURCE,
				get_node_array()[source_node_id].get_cur_state(), SOURCE_RECEIVE_ACK,
				"process_syn_connection������syn����׼�ȴ�ack");

			get_node_array()[relay_node_id].add_next_posible_state(RELAY_SEND_ACK);
			log(source_node_id, relay_node_id, RELAY,
				get_node_array()[relay_node_id].get_cur_state(), RELAY_SEND_ACK,
				"process_syn_connection��δ֪״̬��׼������ack");
		}
	}
}

void route_tcp::process_ack_connection() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		pair<int, vector<int>> ack_response = get_node_array()[relay_node_id].relay_response_ack();

		int accept_node_id = ack_response.first;
		if (accept_node_id == -1) {
			/*��ʱrelay�ڵ���������ڷ���״̬����Ϊ�������һ���κο��ܵ�״̬,��һ��״̬�ɴ��亯����֤*/
			/*��ʱrelay�ڵ���ܴ��ڿ���״̬����Ϊû�н����ж�(����״̬�¶��ܽ���ackӦ��)*/
		}
		else {
			get_node_array()[accept_node_id].set_source_ack_state(ACCEPT);

			get_node_array()[accept_node_id].add_next_posible_state(SOURCE_SENDING);
			log(accept_node_id, relay_node_id, SOURCE,
				get_node_array()[accept_node_id].get_cur_state(), SOURCE_SENDING,
				"process_ack_connection���յ�����ack��׼����������");
			get_node_array()[relay_node_id].add_next_posible_state(RELAY_RECEIVING);
			log(accept_node_id, relay_node_id, RELAY,
				get_node_array()[relay_node_id].get_cur_state(), RELAY_RECEIVING,
				"process_ack_connection������ack��׼��������");
		}

		for (int reject_node_id : ack_response.second) {
			get_node_array()[reject_node_id].set_source_ack_state(REJECT);

			get_node_array()[reject_node_id].add_next_posible_state(SOURCE_SEND_SYN);
			log(reject_node_id, relay_node_id, SOURCE,
				get_node_array()[reject_node_id].get_cur_state(), SOURCE_SEND_SYN,
				"process_ack_connection���յ�����ack��׼���ٴη���syn");
		}
	}

	//��������ѭ����һ��TTI����ɣ��������Ⱥ�˳�򣬷���ack��ǰ������ack�ں�
	//��ack״̬ΪRECEIVE��node�������¼��߼����������
	for (int source_node_id = 0; source_node_id < route_tcp_node::s_node_count; source_node_id++) {
		if (get_node_array()[source_node_id].get_cur_state() == SOURCE_RECEIVE_ACK) {
			if (get_node_array()[source_node_id].get_source_ack_state() == ACCEPT) {
				//��ȡ���е�һ��Ԫ��(������)�����п�������relay�ڵ����
				route_tcp_event* cur_event = get_node_array()[source_node_id].get_source_event_queue().front()->clone();
				int relay_node_id = get_node_array()[source_node_id].get_source_syn_node();
				cur_event->set_current_node(&(get_node_array()[relay_node_id]));
				get_node_array()[relay_node_id].add_relay_event(cur_event);
				cur_event->reset();
			}
		}
	}
}

void route_tcp::process_transimit_connection() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		if (get_node_array()[relay_node_id].get_cur_state() == RELAY_RECEIVING) {
			if (get_node_array()[relay_node_id].get_relay_event_queue().size() == 0)
				cout << "relay_id: " << relay_node_id << endl;
			route_tcp_event* cur_event = get_node_array()[relay_node_id].get_relay_event_queue().front();
			cur_event->transimit();
			int source_node_id = cur_event->get_source_node_id();

			if (cur_event->is_curlink_finished()) {
				get_node_array()[source_node_id].add_next_posible_state(SOURCE_LINK_RESPONSE);
				log(source_node_id, relay_node_id, SOURCE,
					get_node_array()[source_node_id].get_cur_state(), SOURCE_LINK_RESPONSE,
					"process_transimit_connection��������ϣ�׼���ȴ�response");
				get_node_array()[relay_node_id].add_next_posible_state(RELAY_LINK_RESPONSE);
				log(source_node_id, relay_node_id, RELAY,
					get_node_array()[relay_node_id].get_cur_state(), RELAY_LINK_RESPONSE,
					"process_transimit_connection��������ϣ�׼������response");
			}
			else {
				get_node_array()[source_node_id].add_next_posible_state(SOURCE_SENDING);
				log(source_node_id, relay_node_id, SOURCE,
					get_node_array()[source_node_id].get_cur_state(), SOURCE_SENDING,
					"process_transimit_connection����δ���꣬׼����������");
				get_node_array()[relay_node_id].add_next_posible_state(RELAY_RECEIVING);
				log(source_node_id, relay_node_id, RELAY,
					get_node_array()[relay_node_id].get_cur_state(), RELAY_RECEIVING,
					"process_transimit_connection����δ���꣬׼����������");
			}
		}
	}
}

void route_tcp::process_response_connection() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		if (get_node_array()[relay_node_id].get_cur_state() == RELAY_LINK_RESPONSE) {
			route_tcp_event* cur_event = get_node_array()[relay_node_id].get_relay_event_queue().front();
			if (!cur_event->is_curlink_finished())throw logic_error("error");
			int source_node_id = cur_event->get_source_node_id();

			if (cur_event->get_is_curlink_loss()) {
				get_node_array()[source_node_id].set_source_link_response_state(FAILURE);
				s_logger << "TTI: " << left << setw(3) << context::get_context()->get_tti() << ",  link[" << left << setw(3) << source_node_id << ", " << left << setw(3) << relay_node_id << "]: { FAILED }" << endl;

				add_failed_event(cur_event);//��ӵ�ʧ���¼��б�
				get_node_array()[relay_node_id].remove_relay_event();//����

				get_node_array()[source_node_id].add_next_posible_state(SOURCE_SEND_SYN);
				log(source_node_id, relay_node_id, SOURCE,
					get_node_array()[source_node_id].get_cur_state(), SOURCE_SEND_SYN,
					"process_response_connection��������׼���ش�");
				get_node_array()[relay_node_id].add_next_posible_state(IDLE);
				log(source_node_id, relay_node_id, RELAY,
					get_node_array()[relay_node_id].get_cur_state(), IDLE,
					"process_response_connection��������׼���������״̬");
			}
			else {
				get_node_array()[source_node_id].set_source_link_response_state(SUCCESS);
				s_logger << "TTI: " << left << setw(3) << context::get_context()->get_tti() << ",  link[" << left << setw(3) << source_node_id << ", " << left << setw(3) << relay_node_id << "]: { SUCCEED }" << endl;

				get_node_array()[source_node_id].add_next_posible_state(IDLE);
				get_node_array()[source_node_id].remove_source_event();
				log(source_node_id, relay_node_id, SOURCE,
					get_node_array()[source_node_id].get_cur_state(), IDLE,
					"process_response_connection���ɹ����ͣ�׼���������״̬");
				if (cur_event->is_all_finished()) {
					add_successful_event(cur_event);//��ӵ��ɹ��¼��б�
					get_node_array()[relay_node_id].remove_relay_event();//����
					get_node_array()[relay_node_id].add_next_posible_state(IDLE);
					log(source_node_id, relay_node_id, RELAY,
						get_node_array()[relay_node_id].get_cur_state(), IDLE,
						"process_response_connection��������·��ϣ�׼���������״̬");
				}
				else {
					get_node_array()[relay_node_id].transfer_event_from_relay_queue_to_source_queue();
					get_node_array()[relay_node_id].add_next_posible_state(SOURCE_SEND_SYN);
					log(source_node_id, relay_node_id, RELAY,
						get_node_array()[relay_node_id].get_cur_state(), SOURCE_SEND_SYN,
						"process_response_connection����δ������ϣ��ӽ��սڵ�תΪ���ͽڵ�");
				}
			}
		}
	}
}

void route_tcp::update_node_state() {
	for (int node_id = 0; node_id < route_tcp_node::s_node_count; node_id++) {
		get_node_array()[node_id].update_state();
	}
}