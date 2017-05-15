#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include"route_udp.h"
#include"config.h"
#include"gtt.h"
#include"wt.h"
#include"vue.h"
#include"vue_physics.h"
#include"function.h"
#include"reflect/context.h"
#include"time_stamp.h"

using namespace std;

int route_udp_route_event::s_event_count = 0;

default_random_engine route_udp::s_engine;

std::string route_udp_route_event::to_string() {
	stringstream ss;
	for (int i = 0; i < m_through_node_id_vec.size(); i++) {
		ss << "node[" << left << setw(3) << m_through_node_id_vec[i] << "]";
		if (i < m_through_node_id_vec.size() - 1)ss << " -> ";
	}
	ss << endl;
	return ss.str();
}

void route_udp_link_event::transimit() {
	//<Warn>:һ������ָ��Ƶ�δ��䣬���ٷֳɶ�����ֱ�ѡ��Ƶ�δ�����

	if (++m_tti_idx == m_tti_num) {
		m_is_finished = true;
	}

	if (get_pattern_idx() < 0 || get_pattern_idx() > 5) throw logic_error("error");
	double sinr = ((wt*)context::get_context()->get_bean("wt"))->calculate_sinr(
		get_source_node_id(),
		get_destination_node_id(),
		get_pattern_idx(),
		route_udp_node::get_node_id_set(get_pattern_idx()));
	sinr_per_tti.push_back(sinr);

	if (sinr < ((rrm_config*)context::get_context()->get_bean("rrm_config"))->get_drop_sinr_boundary()){
		m_is_loss = true;
		m_loss_reason = LOW_SINR;
	}
}

int route_udp_node::s_node_count = 0;

default_random_engine route_udp_node::s_engine;

std::vector<std::set<int>> route_udp_node::s_node_id_per_pattern;

const std::set<int>& route_udp_node::get_node_id_set(int t_pattern_idx) {
	return s_node_id_per_pattern[t_pattern_idx];
}

route_udp_node::route_udp_node() {
	m_pattern_state = vector<pair<route_udp_pattern_state,int>>(
		((rrm_config*)context::get_context()->get_bean("rrm_config"))->get_pattern_num()+1,
		pair<route_udp_pattern_state,int>(IDLE_UDP,0)
		);

	context* __context = context::get_context();

	//��ʼ�����η���hello����ʱ�䣬Ŀ��Ϊ���ڹ涨��TTI����ɳ������������֤ÿ�������������Լ����ڽӱ�
	int interval = ((route_config*)__context->get_bean("route_config"))->get_t_interval();
	uniform_int_distribution<int> u_tti_hello_start(0, interval);
	m_tti_next_hello = u_tti_hello_start(s_engine);
}

pair<int, int> route_udp_node::select_relay_information() {
	pair<int, int> res = make_pair<int, int>(-1, -1);

	//����ѡ·�ɳ���id
	//<Warn>�������������㷨
	int final_destination_node_id = peek_send_event_queue()->get_final_destination_node_id();
	if (final_destination_node_id != -1) {//�ж��Ƿ�ΪHello���¼��������������ѡ��һ��

		double min_distance = (numeric_limits<double>::max)();
		for (auto p : m_adjacent_list) {
			int near_node_id = p.first;
			double cur_distance = vue_physics::get_distance(near_node_id, final_destination_node_id);
			if (cur_distance < min_distance) {
				min_distance = cur_distance;
				res.first = near_node_id;
			}
		}

		//GPSR�������ǰ�ڵ���ڽӵ㵽Ŀ�Ľڵ�ľ��붼������Ŀ�Ľڵ�ľ���Զ�����ж϶���
		//<Warn>����߽�ת��ģʽ
		if (min_distance > vue_physics::get_distance(get_id(), peek_send_event_queue()->get_final_destination_node_id())) {
			res.first = -1;
		}
	}
	
	//��ѡƵ�Σ�����Ƿ���Hello������������ר��pattern�Ϸ���(�����һ��pattern������Ƿ������ݰ���δռ�õ�Ƶ�������ѡ��
	if (final_destination_node_id == -1) {
		res.second = m_pattern_state.size()-1;
	}
	else if (res.first != -1) {
		vector<int> candidate;
		for (int pattern_idx = 0; pattern_idx < m_pattern_state.size()-1; pattern_idx++) {
			if (m_pattern_state[pattern_idx].first == IDLE_UDP) {
				candidate.push_back(pattern_idx);
			}
		}

		if (candidate.size() != 0) {
			//��δռ�õ�Ƶ���������ѡһ��
			//<Warn>�������������㷨
			uniform_int_distribution<int> u(0, static_cast<int>(candidate.size()) - 1);
			res.second = candidate[u(s_engine)];
		}
	}

	return res;
}

ofstream route_udp::s_logger_pattern;
ofstream route_udp::s_logger_link;
ofstream route_udp::s_logger_event;
ofstream route_udp::s_logger_link_pdr_distance;
ofstream route_udp::s_logger_delay;

void route_udp::log_node_pattern(int t_source_node_id,
	int t_relay_node_id,
	int t_cur_node_id,
	int t_pattern_idx,
	route_udp_pattern_state t_from_pattern_state,
	route_udp_pattern_state t_to_pattern_state,
	string t_description) {
	v2x_time* __time = (v2x_time*)context::get_context()->get_bean("time");
	s_logger_pattern << "TTI[" << left << setw(3) << __time->get_tti() << "] - ";
	s_logger_pattern << "link[" << left << setw(3) << t_source_node_id << ", ";
	s_logger_pattern << left << setw(3) << t_relay_node_id << "] - ";
	s_logger_pattern << "node[" << left << setw(3) << t_cur_node_id << "] - ";
	s_logger_pattern << "pattern[" << t_pattern_idx << "] - ";
	s_logger_pattern << "[" << left << setw(15) << pattern_state_to_string(t_from_pattern_state) << " -> " << left << setw(15) << pattern_state_to_string(t_to_pattern_state) << "] - ";
	s_logger_pattern << t_description << endl;
}

string route_udp::pattern_state_to_string(route_udp_pattern_state t_pattern_state) {
	switch (t_pattern_state) {
	case IDLE_UDP:
		return "IDLE";
	case SENDING_UDP:
		return "SENDING";
	case RECEIVING_UDP:
		return "RECEIVING";
	default:
		throw logic_error("error");
	}
}

void route_udp::log_event(int t_origin_node_id, int t_fianl_destination_node_id) {
	v2x_time* __time = (v2x_time*)context::get_context()->get_bean("time");
	s_logger_event << "TTI[" << left << setw(3) << __time->get_tti() << "] - ";
	s_logger_event << "trigger[" << left << setw(3) << t_origin_node_id << ", ";
	s_logger_event << left << setw(3) << t_fianl_destination_node_id << "]" << endl;

}

void route_udp::log_link(int t_source_node_id, int t_relay_node_id, std::string t_description,std::string t_loss_reason,adjacent_message last_time,adjacent_message current_time) {
	string lost_reason1 = "����";
	string lost_reason2 = "�����Բ���";
	double last_sinr = 0;
	
	v2x_time* __time = (v2x_time*)context::get_context()->get_bean("time");
	vue* vue_array = ((gtt*)context::get_context()->get_bean("gtt"))->get_vue_array();

	s_logger_link << "TTI[" << left << setw(3) << __time->get_tti() << "] - ";
	s_logger_link << "link[" << left << setw(3) << t_source_node_id << ", ";
	s_logger_link << left << setw(3) << t_relay_node_id << "] - ";
	s_logger_link << "{" << t_description << "}";
	s_logger_link << "{" << t_loss_reason << "}" << endl;
	context* __context = context::get_context();
	{
		s_logger_link << "Pattern Id:" << last_time.pattern_id << ";" << "Infer_Set:{";
		set<int>::iterator it = last_time.infer_node_id.begin();
		while (it != last_time.infer_node_id.end())
		{
			s_logger_link << *it << "(" << vue_array[*it].get_physics_level()->m_absx << "," << vue_array[*it].get_physics_level()->m_absy << "),";
			it++;
		}
		s_logger_link << "};";
		s_logger_link << "Send:��" << last_time.send_node_x << "," << last_time.send_node_y << "��,";
		s_logger_link << "Receive:��" << last_time.receive_node_x << "," << last_time.receive_node_y << "��;";
		s_logger_link << "Sinr:";
		vector<double>::iterator it1 = last_time.sinr.begin();
		while (it1 != last_time.sinr.end()) {
			s_logger_link << *it1 << ",";
			last_sinr = *it1;
			it1++;
		}
		s_logger_link << "PL:" << -10*log10(last_time.pl);
		s_logger_link << endl;
	}
	
	{
		s_logger_link << "Pattern Id:" << current_time.pattern_id << ";" << "Infer_Set:{";
		set<int>::iterator __it = current_time.infer_node_id.begin();
		while (__it != current_time.infer_node_id.end())
		{
			s_logger_link << *__it << "(" << vue_array[*__it].get_physics_level()->m_absx << "," << vue_array[*__it].get_physics_level()->m_absy << "),";
			s_logger_link << "PL:" << -10*log10(vue_physics::get_pl(*__it, t_relay_node_id)) << ",";
			__it++;
		}
		s_logger_link << "};";
		s_logger_link << "Send:��" << current_time.send_node_x << "," << current_time.send_node_y << "��,";
		s_logger_link << "Receive:��" << current_time.receive_node_x << "," << current_time.receive_node_y << "��";
		s_logger_link << "Sinr:";
		vector<double>::iterator __it1 = current_time.sinr.begin();
		while (__it1 != current_time.sinr.end()) {
			s_logger_link << *__it1 << ",";
			__it1++;
		}
		s_logger_link << "PL:" << -10*log10(current_time.pl);
		s_logger_link << endl;
	}
	if (abs(last_time.send_node_x - current_time.send_node_x) > 50 || abs(last_time.send_node_y - current_time.send_node_y) > 50 ||
		abs(last_time.receive_node_x - current_time.receive_node_x) > 50 || abs(last_time.receive_node_y - current_time.receive_node_y)>50)
	{
		s_logger_link << "�������ƣ����˶�����Χ" << endl << endl;
	}
	else {
		if ((last_sinr - (-10 * log10(current_time.pl) - (-10 * log10(last_time.pl))))>((rrm_config*)context::get_context()->get_bean("rrm_config"))->get_drop_sinr_boundary()) s_logger_link << lost_reason1;
		else s_logger_link << lost_reason2;
		s_logger_link << endl;
	}
}

void route_udp::initialize() {
	context* __context = context::get_context();
	int vue_num = get_gtt()->get_vue_num();
	m_node_array = new route_udp_node[vue_num];

	s_logger_pattern.open("log/route_udp_pattern_log.txt");
	s_logger_link.open("log/route_udp_link_log.txt");
	s_logger_event.open("log/route_udp_event_log.txt");
	s_logger_link_pdr_distance.open("log/route_udp_link_pdr_distance.txt");
	s_logger_delay.open("log/route_udp_delay.txt");

	route_udp_node::s_node_id_per_pattern = vector<set<int>>(get_rrm_config()->get_pattern_num()+1);
}

void route_udp::process_per_tti() {
	//ɾ����ʱ�ڵ�
	update_adjacent_list();

	//�¼�����
	event_trigger();

    //����Ҫ��ʼ���͵��¼�
	start_sending_data();

	//���䵱ǰTTI���ڵ��¼�
	transmit_data();
}

//�򵥵ĸ��ݾ���ά���ڽӱ�
void route_udp::update_route_table_from_physics_level() {
	//���֮ǰ���ڽӱ�
	for (int node_id = 0; node_id < route_udp_node::s_node_count; node_id++) {
		get_node_array()[node_id].m_adjacent_list.clear();
	}

	//<Warn>:��ʱ��Ϊ���ݾ���ȷ���ڽӱ�
	context* __context = context::get_context();
	int vue_num = get_gtt()->get_vue_num();
	for (int vue_id_i = 0; vue_id_i < vue_num; vue_id_i++) {
		for (int vue_id_j = 0; vue_id_j < vue_num; vue_id_j++) {
			if (vue_id_i == vue_id_j)continue;
			if (vue_physics::get_distance(vue_id_i, vue_id_j) < 100) {
				//get_node_array()[vue_id_i].add_to_adjacent_list(vue_id_j);
				//get_node_array()[vue_id_j].add_to_adjacent_list(vue_id_i);
			}
		}
	}
}

//ɾ����ʱ�ڵ�
void route_udp::update_adjacent_list() {
	for (int source_node_id = 0; source_node_id < route_udp_node::s_node_count; source_node_id++) {
		route_udp_node& source_node = get_node_array()[source_node_id];

		context* __context = context::get_context();
		int current_tti = get_time()->get_tti();
		int interval = 1.5*get_route_config()->get_t_interval();
		vector<pair<int, adjacent_message>>::iterator it= source_node.m_adjacent_list.begin();
		while (it != source_node.m_adjacent_list.end()) {
			if ((current_tti - it->second.life_time) > interval) {
				it = source_node.m_adjacent_list.erase(it);
			}
			else {
				it++;
			}
		}
	}
}

void route_udp::event_trigger() {
	context* __context = context::get_context();
	double trigger_rate = get_tmc_config()->get_trigger_rate();

	uniform_real_distribution<double> u_rate(0, 1);
	uniform_int_distribution<int> u_node_id(0, route_udp_node::s_node_count - 1);

	//ͨ�������ļ�������hello���Ĵ���ƽ�����
	int interval = get_route_config()->get_t_interval();
	uniform_int_distribution<int> u_tti_hello_between(0.5*interval, 1.5*interval);

	//����Hello��
	for (int origin_source_node_id = 0; origin_source_node_id < route_udp_node::s_node_count; origin_source_node_id++) {
		route_udp_node& source_node = get_node_array()[origin_source_node_id];

		//�ж��Ƿ񵽸÷���hello����ʱ��
		if (source_node.m_tti_next_hello == get_time()->get_tti()) {
			get_node_array()[origin_source_node_id].offer_send_event_queue(
				new route_udp_route_event(origin_source_node_id, -1, HEllO,get_time()->get_tti())
			);

			//������һ�η���hello����ʱ��
			source_node.m_tti_next_hello += u_tti_hello_between(s_engine);
		}
	}

	//�ڳ�ʼ��ʱ����󣬴������ݴ����¼�
	if(get_time()->get_tti()>interval){
		for (int origin_source_node_id = 0; origin_source_node_id < route_udp_node::s_node_count; origin_source_node_id++) {
			if (u_rate(s_engine) < trigger_rate) {

				int final_destination_node_id = origin_source_node_id;
				while (final_destination_node_id == origin_source_node_id) {
					final_destination_node_id = u_node_id(s_engine);
				}
				get_node_array()[origin_source_node_id].offer_send_event_queue(
					new route_udp_route_event(origin_source_node_id, final_destination_node_id, DATA,get_time()->get_tti())
				);
				log_event(origin_source_node_id, final_destination_node_id);
			}
		}
	}
}

void route_udp::start_sending_data() {
	//����ͬһʱ�̷���Ϣ����������Ϣ��ԭ�����з���Ϣ���¼��ڴ���ǰ��ѡ����Ƶ�β�����ռ��
	for (int source_node_id = 0; source_node_id < route_udp_node::s_node_count; source_node_id++) {
		route_udp_node& source_node = get_node_array()[source_node_id];

		if (source_node.sending_link_event.size() == 0) {//��ǰ�ڵ㲢�����ڴ���
			//��ǰ�����������¼��б�Ϊ�գ���������
			if (source_node.is_send_event_queue_empty()) continue;

			if (source_node.m_send_event_queue.front()->get_route_event_type() == DATA) {//������¼������ݴ����¼�
				//ѡ���м̳����Լ�Ƶ��
				pair<int, int> select_res = source_node.select_relay_information();

				//����Ҳ�����һ�������¼route_event����
				while (select_res.first == -1) {
					//<Warn>��¼route_event����
					//��route_eventɾ��
					if (source_node.m_send_event_queue.empty()) throw logic_error("error");

					route_udp_route_event* temp = source_node.m_send_event_queue.front();
					source_node.m_send_event_queue.pop();
					delete temp;

					//���û���¼���Ҫ��������������ѭ��
					if (source_node.m_send_event_queue.empty()) break;

					//������в�Ϊ��,�������������е���һ��route_event
					select_res = source_node.select_relay_information();
				}

				//���û���¼���Ҫ��������,�����������һ��node
				if (source_node.m_send_event_queue.empty()) continue;

				//�����ʱû�п���Ƶ�Σ��Ƴٵ���һ��TTI�ٳ��Է���
				if (select_res.second == -1) {
					continue;
				}

				//��ѡ��Ҫ����Ŀ���Ƶ���Ƚ���ռ��
				source_node.m_pattern_state[select_res.second].first = SENDING_UDP;
				if (source_node.m_pattern_state[select_res.second].second != 0) throw logic_error("error");

				//ά�������б�
				if (route_udp_node::s_node_id_per_pattern[select_res.second].find(source_node_id) != route_udp_node::s_node_id_per_pattern[select_res.second].end()) throw logic_error("error");
				if (select_res.second < 0 || select_res.second > 4) throw logic_error("error");
				route_udp_node::s_node_id_per_pattern[select_res.second].insert(source_node_id);

				//������·�¼�
				if (select_res.second < 0 || select_res.second > 4) throw logic_error("error");
				source_node.sending_link_event.push_back(new route_udp_link_event(
					source_node_id, select_res.first, select_res.second, source_node.peek_send_event_queue()->get_tti_num()));
			}

			else {//������¼���Hello�������¼�
				//ѡ��Ƶ��
				pair<int, int> select_res = source_node.select_relay_information();

				//��ѡ��Ҫ����Ŀ���Ƶ���Ƚ���ռ��
				source_node.m_pattern_state[select_res.second].first = SENDING_UDP;
				if (source_node.m_pattern_state[select_res.second].second != 0) throw logic_error("error");

				//ά�������б�
				if (route_udp_node::s_node_id_per_pattern[select_res.second].find(source_node_id) != route_udp_node::s_node_id_per_pattern[select_res.second].end()) throw logic_error("error");
				if (select_res.second < 0 || select_res.second > 5) throw logic_error("error");
				route_udp_node::s_node_id_per_pattern[select_res.second].insert(source_node_id);

				//�Գ��˸ýڵ�����������ڵ㴴����·�¼�
				for (int dst_id = 0; dst_id < route_udp_node::s_node_count; dst_id++) {
					if (dst_id == source_node_id) continue;
					source_node.sending_link_event.push_back(new route_udp_link_event(
						source_node_id, dst_id, select_res.second, source_node.peek_send_event_queue()->get_hello_tti_num()));
				}
			}
		}
	}
}

void route_udp::transmit_data() {
	//������link_event���е�һ�������Ŀ��1�����������¼���Ŀ��2��ά�����սڵ㴫��pattern��״̬
	for (int source_node_id = 0; source_node_id < route_udp_node::s_node_count; source_node_id++) {
		route_udp_node& source_node = get_node_array()[source_node_id];
		if (source_node.sending_link_event.size() == 0) continue;

		//�Ե�ǰ��������link_event���б�������
		vector<route_udp_link_event*>::iterator it;
		for (it = source_node.sending_link_event.begin(); it != source_node.sending_link_event.end(); it++) {

			route_udp_node& destination_node = get_node_array()[(*it)->get_destination_node_id()];

			//�¼�����
			(*it)->transimit();

			//ȡ����ǰ�¼���ռ�õ�pattern���
			int pattern_idx = (*it)->m_pattern_idx;

			if (pattern_idx < 0 || pattern_idx > 5) throw logic_error("error");

			//�����������Ƶ���������սڵ����ڷ������¼�����ʧ��
			if (destination_node.m_pattern_state[pattern_idx].first == SENDING_UDP) {
				(*it)->m_is_loss = true;
				(*it)->m_loss_reason = DST_IS_SENDING;
			}

			//���û�б�ռ���Ҵ��ڿ���״̬���޸ĵ�ǰpattern״̬Ϊ���ڽ��ղ��ҽ���pattern�Ͻ������ݰ��ĸ�����0��Ϊ1
			else if (destination_node.m_pattern_state[pattern_idx].first == IDLE_UDP) {
				//��Ŀ�Ľڵ��patternһֱ����sending״̬
				(*it)->dst_maintain_sending = false;

				destination_node.m_pattern_state[pattern_idx].first = RECEIVING_UDP;
				if (destination_node.m_pattern_state[pattern_idx].second != 0) throw logic_error("error");
				destination_node.m_pattern_state[pattern_idx].second++;
			}

			//����Ѿ�����RECEVING״̬�Ҹ�link_event�տ�ʼ���ͣ��򽫸�pattern�Ͻ������ݰ�������1
			else {
				//������¼��տ�ʼ�����һ��TTI����������Ƶ����Ŀ�Ľڵ�������ݰ�����Ŀ
				if ((*it)->get_tti_idx() == 1) {
					destination_node.m_pattern_state[pattern_idx].second++;
				}

				//������¼�֮ǰһֱ������ΪĿ�Ľڵ㵱ǰƵ�δ���sending״̬�����ܾ���ȴ�ڵ�ǰTTI���µ��Է��ͣ���������Ƶ����Ŀ�Ľڵ�������ݰ�����Ŀ
				else if ((*it)->dst_maintain_sending == true) {
					destination_node.m_pattern_state[pattern_idx].second++;
				}

				//��Ŀ�Ľڵ��patternһֱ����sending״̬
				(*it)->dst_maintain_sending = false;
			}
		}
	}

	//������link_event���еڶ�����������Ѿ�������ϵ��¼����в�����Ŀ��1��ͳ���¼�����ɹ����Ƕ�ʧ��Ŀ��2���޸ķ��ͽڵ�ͽ��սڵ㵱ǰpattern�ϵ�״̬,ά�������б�Ŀ��3������link_event������route_event
	for (int source_node_id = 0; source_node_id < route_udp_node::s_node_count; source_node_id++) {
		route_udp_node& source_node = get_node_array()[source_node_id];
		if (source_node.sending_link_event.size() == 0) continue;

		//�Ե�ǰ��������link_event���б���ά��
		vector<route_udp_link_event*>::iterator it;

		bool all_link_event_finished = false;//�����ж�����link_event�Ƿ�����ϣ���ɾ��link_event
		for (it = source_node.sending_link_event.begin(); it != source_node.sending_link_event.end(); it++) {

			if ((*it)->is_finished()) {
				all_link_event_finished = true;

				int pattern_idx = (*it)->m_pattern_idx;

				//����link_event������Ϻ�ά�����ͽڵ㵱ǰpattern״̬
				if (it == source_node.sending_link_event.end() - 1) {
					source_node.m_pattern_state[pattern_idx].first = IDLE_UDP;
					if (source_node.m_pattern_state[pattern_idx].second != 0) throw logic_error("error");
				}

				//���սڵ㵱ǰpattern״̬ά��

				route_udp_node& destination_node = get_node_array()[(*it)->get_destination_node_id()];

				if (destination_node.m_pattern_state[pattern_idx].second != 0) {
					destination_node.m_pattern_state[pattern_idx].second--;
					if (destination_node.m_pattern_state[pattern_idx].second == 0) {
						destination_node.m_pattern_state[pattern_idx].first = IDLE_UDP;
					}
				}

				//����link_event������Ϻ�ά�������б�
				if (it == source_node.sending_link_event.end() - 1){
					if (route_udp_node::s_node_id_per_pattern[pattern_idx].find(source_node_id) == route_udp_node::s_node_id_per_pattern[pattern_idx].end()) throw logic_error("error");
					if (pattern_idx < 0 || pattern_idx > 5) throw logic_error("error");
					route_udp_node::s_node_id_per_pattern[pattern_idx].erase(source_node_id);
				}

				//�ж��Ƿ񶪰�
				if ((*it)->get_is_loss()) {
					if (source_node.m_send_event_queue.front()->get_route_event_type() == DATA) {//����������¼������շ�����С��500m�����¼��link_event
						if (vue_physics::get_distance(source_node.m_send_event_queue.front()->get_origin_source_node_id(), source_node.m_send_event_queue.front()->get_final_destination_node_id()) < 500) {//ֻ��¼�շ����С��500m���¼�
							string loss_reason;
							if ((*it)->m_loss_reason == UNKNOW) loss_reason = "UNKNOW";
							else if ((*it)->m_loss_reason == LOW_SINR) loss_reason = "LOW_SINR";
							else loss_reason = "DST_IS_SENDING";
							int count = 0;
							while (source_node.m_adjacent_list[count].first != (*it)->get_destination_node_id())
							{
								count++;
							}
							context* __context = context::get_context();
							adjacent_message temp;
							temp.pattern_id = pattern_idx;
							temp.sinr = (*it)->sinr_per_tti;
							temp.infer_node_id = route_udp_node::s_node_id_per_pattern[pattern_idx];
							temp.send_node_x = get_gtt()->get_vue_array()[destination_node.get_id()].get_physics_level()->m_absx;
							temp.send_node_y = get_gtt()->get_vue_array()[destination_node.get_id()].get_physics_level()->m_absy;
							temp.receive_node_x = get_gtt()->get_vue_array()[source_node_id].get_physics_level()->m_absx;
							temp.receive_node_y = get_gtt()->get_vue_array()[source_node_id].get_physics_level()->m_absy;
							temp.pl = vue_physics::get_pl(source_node_id, destination_node.get_id());

							log_link(source_node_id, (*it)->get_destination_node_id(), "FAILED", loss_reason, source_node.m_adjacent_list[count].second, temp);

							s_logger_link_pdr_distance << 0 << "," << vue_physics::get_distance(source_node_id, destination_node.get_id()) << endl;

							if (abs(source_node.m_adjacent_list[count].second.send_node_x - temp.send_node_x) < 50 && abs(source_node.m_adjacent_list[count].second.send_node_y - temp.send_node_y) < 50 &&
								abs(source_node.m_adjacent_list[count].second.receive_node_x - temp.receive_node_x) < 50 && abs(source_node.m_adjacent_list[count].second.receive_node_y - temp.receive_node_y) < 50)//�����Ƶĳ����ų�����
								//add_failed_route_event(source_node.peek_send_event_queue());
								m_failed_route_event_num++;
						}
					}

					if (source_node.m_send_event_queue.empty()) throw logic_error("error");

					//����link_event������Ϻ�ɾ��route_event
					if (it == source_node.sending_link_event.end() - 1) {
						route_udp_route_event* temp = source_node.m_send_event_queue.front();

						//ɾ��route_event
						source_node.m_send_event_queue.pop();
						//if (source_node.m_send_event_queue.front()->get_route_event_type() == HEllO) delete temp;
						delete temp;
					}
				}

				else {
					if (source_node.m_send_event_queue.front()->get_route_event_type() == DATA) {
						string loss_reason;
						if ((*it)->m_loss_reason == UNKNOW) loss_reason = "UNKNOW";
						else if ((*it)->m_loss_reason == LOW_SINR) loss_reason = "LOW_SINR";
						else loss_reason = "DST_IS_SENDING";
						//log_link(source_node_id, (*it)->get_destination_node_id(), "SUCCESSFUL",loss_reason);
					}
					if (source_node.m_send_event_queue.empty()) throw logic_error("error");

					//����������¼�������µ�ǰ�ڵ�id�����ж��Ƿ���ճɹ�
					if (source_node.m_send_event_queue.front()->get_route_event_type() == DATA) {
						if (source_node.sending_link_event.size() != 1) throw logic_error("error");

						source_node.m_send_event_queue.front()->set_current_node_id((*it)->get_destination_node_id());

						s_logger_link_pdr_distance << 1 << "," << vue_physics::get_distance(source_node_id, destination_node.get_id()) << endl;

						if (source_node.m_send_event_queue.front()->get_final_destination_node_id() == destination_node.get_id()) {

							//OP1:��¼route_event���ͳɹ�,�������ñ��浽�б���

							int now_tti = get_time()->get_tti();

							int delay = get_time()->get_tti() - source_node.m_send_event_queue.front()->get_start_tti();
							s_logger_delay << delay << endl;
							if (vue_physics::get_distance(source_node.m_send_event_queue.front()->get_origin_source_node_id(), destination_node.get_id()) < 500) {
								//add_successful_route_event(source_node.poll_send_event_queue());
								m_success_route_event_num++;
							}

							//OP2��ֻ������Ҫ�����ݣ�Ȼ��route_eventɾ�����ͷſռ�
							route_udp_route_event* temp = source_node.m_send_event_queue.front();
							source_node.m_send_event_queue.pop();
							delete temp;
						}

						else {
							//�����û�е�����Ŀ�Ľڵ���route_event���ݸ���һ���ڵ�
							destination_node.offer_send_event_queue(source_node.poll_send_event_queue());
						}
					}

					//�����Hello�����򽫽��յ�����Դ��ַ���뵱ǰ�����ڽӱ�
					else {
						context* __context = context::get_context();
						adjacent_message temp;
						temp.pattern_id = pattern_idx;
						temp.life_time = get_time()->get_tti();
						temp.infer_node_id = route_udp_node::s_node_id_per_pattern[pattern_idx];
						temp.sinr = (*it)->sinr_per_tti;
						temp.receive_node_x = get_gtt()->get_vue_array()[destination_node.get_id()].get_physics_level()->m_absx;
						temp.receive_node_y = get_gtt()->get_vue_array()[destination_node.get_id()].get_physics_level()->m_absy;
						temp.send_node_x = get_gtt()->get_vue_array()[source_node_id].get_physics_level()->m_absx;
						temp.send_node_y = get_gtt()->get_vue_array()[source_node_id].get_physics_level()->m_absy;
						temp.pl = vue_physics::get_pl(destination_node.get_id(), source_node_id);

						destination_node.add_to_adjacent_list(source_node.get_id(),temp);

						//����link_event������Ϻ�ɾ��route_event
						if (it == source_node.sending_link_event.end() - 1) {
							route_udp_route_event* temp = source_node.m_send_event_queue.front();
							source_node.m_send_event_queue.pop();
							delete temp;
						}
					}
				}
			}
		}

		//������ɵ�link_event ������Ϻ�ɾ������link_event
		if (all_link_event_finished == true) {
			vector<route_udp_link_event*>::iterator it = source_node.sending_link_event.begin();
			while (it != source_node.sending_link_event.end()) {
				delete *it;
				it++;
			}
			source_node.sending_link_event.clear();
		}
	}
}

