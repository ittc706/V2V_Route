#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include"route_udp.h"
#include"context.h"
#include"config.h"
#include"gtt.h"
#include"vue.h"
#include"vue_physics.h"
#include"function.h"
using namespace std;

int route_udp_route_event::s_event_count = 0;

route_udp_route_event* route_udp_route_event::clone() {
	route_udp_route_event* clone_event = new route_udp_route_event(get_origin_source_node_id(), get_final_destination_node_id());
	clone_event->m_through_node_id_vec = this->m_through_node_id_vec;
	return clone_event;
}

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
	//<Warn>:一跳就在指定频段传输，不再分成多个包分别选择频段传输了

	if (++m_package_idx == m_package_num) {
		m_is_finished = true;
	}
	double sinr = -1;
	//<Warn>:sinr计算待补充，以及是否丢包字段的维护
}

int route_udp_node::s_node_count = 0;

default_random_engine route_udp_node::s_engine;

ofstream route_udp_node::s_logger;

void route_udp_node::log_state_update(route_udp_node* t_node) {
	s_logger << "TTI[" << left << setw(3) << context::get_context()->get_tti() << "] - ";
	s_logger << "node[" << left << setw(3) << t_node->get_id() << "] - ";
	s_logger << "curr_state[" << left << setw(20) << route_udp::state_to_string(t_node->get_cur_state()) << "] - ";
	s_logger << "next_state[";
	for (route_udp_node_state state : t_node->m_next_possible_state_set) {
		s_logger << left << setw(20) << route_udp::state_to_string(state);
	}
	s_logger << "]" << endl;
}

std::vector<std::set<route_udp_node*>> route_udp_node::s_node_per_pattern;

route_udp_node::route_udp_node() {
	m_source_node = new route_udp_source_node();
	m_relay_node = new route_udp_relay_node();
}

void route_udp_node::update_state() {
	route_udp_node_state max_state = IDLE;

	log_state_update(this);

	for (route_udp_node_state state : m_next_possible_state_set) {
		if (state > max_state) {
			max_state = state;
		}
	}

	if (max_state == IDLE) {
		if (get_source_node()->m_event_queue.size() != 0) {
			max_state = ;//是自己触发的还是转发的
		}
	}

	set_cur_state(max_state);
	m_next_possible_state_set.clear();
}

int route_udp_node::source_select_relay_node() {
	int res = -1;

	int final_destination_node_id = get_source_node()->peek_event()->get_final_destination_node_id();

	double min_distance = (numeric_limits<double>::max)();
	for (int near_node_id : m_adjacent_list) {
		double cur_distance = vue_physics::get_distance(near_node_id, final_destination_node_id);
		if (cur_distance< min_distance) {
			min_distance = cur_distance;
			res = near_node_id;
		}
	}

	return res;
}

default_random_engine route_udp::s_engine;

ofstream route_udp::s_logger;

string route_udp::state_to_string(route_udp_node_state state) {
	switch (state) {
	case IDLE:
		return "IDLE";
	case SOURCE_SENDING:
		return "SOURCE_SENDING";
	case SOURCE_RECEIVING:
		return "SOURCE_RECEIVING";
	case SOURCE_FORWARDING:
		return "SOURCE_FORWARDING";
	default:
		throw logic_error("error");
	}
}
