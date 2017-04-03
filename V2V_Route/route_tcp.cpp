#include<iostream>
#include"route_tcp.h"
#include"context.h"
#include"config.h"
#include"gtt.h"
using namespace std;

int route_tcp_event::s_event_count = 0;

void route_tcp_event::transimit() {
	//<Warn>:һ������ָ��Ƶ�δ��䣬���ٷֳɶ�����ֱ�ѡ��Ƶ�δ�����
	
	if (++m_package_idx == m_package_num) {
		m_is_curlink_finished = true;
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
	clone_event->m_package_num = m_package_num;
	clone_event->m_through_node_vec = m_through_node_vec;
	m_type = RELAY;
	return clone_event;
}

int route_tcp_node::s_node_count = 0;

default_random_engine route_tcp_node::s_engine;

std::vector<std::set<route_tcp_node*>> route_tcp_node::s_node_per_pattern;

void route_tcp_node::transfer_event_from_relay_queue_to_source_queue() {
	add_source_event(m_relay_event_queue.front());
	m_relay_event_queue.pop();
}

void route_tcp_node::update_state() {
	route_tcp_node_state max_state = IDLE;

	if (m_next_posible_state_set.find(RELAY_SEND_ACK) == m_next_posible_state_set.end()) {
		if (m_next_posible_state_set.size() != 1) {
			throw logic_error("error");
		}
	}

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

	set_cur_state(max_state);
	m_next_posible_state_set.clear();
}

int route_tcp_node::source_select_relay_node() {
	int res = -1;

	//<Warn>������

	m_source_syn_node = res;//ά����״̬����ɾ
	return res;
}

std::pair<int, std::vector<int>> route_tcp_node::relay_response_ack() {
	if (get_cur_state() != RELAY_SEND_ACK) {
		return pair<int, vector<int>>(-1, vector<int>());
	}

	uniform_int_distribution<int> u(0, m_relay_syn_node_vec.size()-1);

	int select_node_id = m_relay_syn_node_vec[u(s_engine)];

	vector<int> nonselect_node_id_vec;
	for (int node_id : m_relay_syn_node_vec) {
		if (select_node_id != node_id) {
			nonselect_node_id_vec.push_back(node_id);
		}
	}
	return std::pair<int, std::vector<int>>(select_node_id, nonselect_node_id_vec);
}

default_random_engine route_tcp::s_engine;

route_tcp::route_tcp() {

}

void route_tcp::initialize() {
	//<Warn>:��ʼ���ڵ�
	context* __context = context::get_context();
	int vue_num = __context->get_gtt()->get_vue_num();
	m_node_array = new route_tcp_node[vue_num];
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
	//<Warn>:���������ȡ�ڽ�����
}

void route_tcp::event_trigger() {
	context* __context = context::get_context();
	double trigger_rate = __context->get_tmc_config()->get_trigger_rate();

	uniform_real_distribution<double> u_rate(0, 1);
	cout << route_tcp_node::s_node_count << endl;
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
		}
	}
}

void route_tcp::process_syn_connection() {
	//����SYN�����ڱ���tti�ڣ����SYN�ķ����Լ�����
	for (int source_node_id = 0; source_node_id < route_tcp_node::s_node_count; source_node_id++) {
		if (get_node_array()[source_node_id].get_cur_state() == SOURCE_SEND_SYN) {
			int relay_node_id = get_node_array()[source_node_id].source_select_relay_node();
			get_node_array()[relay_node_id].add_relay_syn_node(source_node_id);

			//����source-relay�ڵ����һ�̿���״̬
			get_node_array()[source_node_id].add_next_posible_state(SOURCE_RECEIVE_ACK);
			get_node_array()[relay_node_id].add_next_posible_state(RELAY_SEND_ACK);
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
			get_node_array()[relay_node_id].add_next_posible_state(RELAY_RECEIVING);
		}

		for (int reject_node_id : ack_response.second) {
			get_node_array()[reject_node_id].set_source_ack_state(REJECT);

			get_node_array()[reject_node_id].add_next_posible_state(SOURCE_SEND_SYN);
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
			route_tcp_event* cur_event = get_node_array()[relay_node_id].get_relay_event_queue().front();
			cur_event->transimit();
			int source_node_id = cur_event->get_source_node_id();
			if (cur_event->is_curlink_finished()) {
				get_node_array()[source_node_id].add_next_posible_state(SOURCE_LINK_RESPONSE);
				get_node_array()[relay_node_id].add_next_posible_state(RELAY_LINK_RESPONSE);
			}
			else {
				get_node_array()[source_node_id].add_next_posible_state(SOURCE_SENDING);
				get_node_array()[relay_node_id].add_next_posible_state(RELAY_RECEIVING);
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
				add_failed_event(cur_event);//��ӵ�ʧ���¼��б�
				get_node_array()[relay_node_id].remove_relay_event();//����

				get_node_array()[source_node_id].add_next_posible_state(SOURCE_SEND_SYN);
				get_node_array()[relay_node_id].add_next_posible_state(IDLE);
			}
			else {
				get_node_array()[source_node_id].set_source_link_response_state(SUCCESS);
				get_node_array()[source_node_id].add_next_posible_state(IDLE);

				if (cur_event->is_all_finished()) {
					add_successful_event(cur_event);//��ӵ��ɹ��¼��б�
					get_node_array()[relay_node_id].remove_relay_event();//����
					get_node_array()[relay_node_id].add_next_posible_state(IDLE);
				}
				else {
					get_node_array()[relay_node_id].transfer_event_from_relay_queue_to_source_queue();
					get_node_array()[relay_node_id].add_next_posible_state(SOURCE_SEND_SYN);
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