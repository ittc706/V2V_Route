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

static ofstream LOGGER("log\\route_log.txt");

static string state_to_string(route_tcp_node_state state) {
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

static void log(int tti, int source_node_id, int relay_node_id, route_event_type event_type, route_tcp_node_state cur_state, route_tcp_node_state next_state,string description) {
	int cur_node_id = event_type == SOURCE ? source_node_id : relay_node_id;
	LOGGER << "TTI[" << left << setw(3) << context::get_context()->get_tti()
		<< "] - link[" << left << setw(3) << source_node_id << ", " << left << setw(3) << relay_node_id << "] - "
		<< "node[" << left << setw(3) << cur_node_id << "] - "
		<< "cur_state[" << state_to_string(cur_state) << "] - next_state[" << state_to_string(next_state) << "] - "
		<< description << endl;
}




int route_tcp_event::s_event_count = 0;

void route_tcp_event::transimit() {
	//<Warn>:一跳就在指定频段传输，不再分成多个包分别选择频段传输了
	
	if (++m_package_idx == m_package_num) {
		m_is_curlink_finished = true;
		if (m_type != RELAY) throw logic_error("error");
		m_through_node_vec.push_back(m_current_node);
	}
	double sinr = -1;
	//<Warn>:sinr计算待补充，以及是否丢包字段的维护

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
	for (int i = m_through_node_vec.size() - 1; i >= 0; i--) {
		ss << "node[" << m_through_node_vec[i]->get_id() << "], ";
	}
	ss << endl;
	return ss.str();
}

int route_tcp_node::s_node_count = 0;

default_random_engine route_tcp_node::s_engine;

std::vector<std::set<route_tcp_node*>> route_tcp_node::s_node_per_pattern;

void route_tcp_node::transfer_event_from_relay_queue_to_source_queue() {
	m_relay_event_queue.front()->set_type(SOURCE);
	add_source_event(m_relay_event_queue.front());
	m_relay_event_queue.pop();
}

void route_tcp_node::update_state() {
	route_tcp_node_state max_state = IDLE;

	//if (m_next_posible_state_set.size()>1) {
	//	cout << "node["<<get_id()<<"] possible list: ";
	//	for (int i : m_next_posible_state_set)
	//		cout << i << ",";
	//	cout << endl;
	//}


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

	m_source_syn_node = res;//维护该状态，勿删
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

	m_relay_syn_node_vec.clear();

	return std::pair<int, std::vector<int>>(select_node_id, nonselect_node_id_vec);
}

default_random_engine route_tcp::s_engine;

route_tcp::route_tcp() {

}

void route_tcp::initialize() {
	//<Warn>:初始化节点
	context* __context = context::get_context();
	int vue_num = __context->get_gtt()->get_vue_num();
	m_node_array = new route_tcp_node[vue_num];
}

void route_tcp::process_per_tti() {
	//事件触发
	event_trigger();

	//更新节点状态
	update_node_state();

	//处理那些处于syn状态的节点
	process_syn_connection();

	//处理那些需要回复ack信号的节点
	process_ack_connection();

	//处理那些正在传输状态的节点
	process_transimit_connection();

	//处理那些正在回复传输标志的节点
	process_response_connection();
}

void route_tcp::update_route_table_from_physics_level() {
	//清除之前的邻接表
	for (int node_id = 0; node_id < route_tcp_node::s_node_count; node_id++) {
		get_node_array()[node_id].m_adjacent_list.clear();
	}

	//<Warn>:暂时改为根据距离确定邻接表
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
			LOGGER << "TTI: " << left << setw(3) << context::get_context()->get_tti() << ",  trigger[" << left << setw(3) << origin_source_node_id << ", " << left << setw(3) << final_destination_node_id << "]" << endl;

		}
	}
}

void route_tcp::process_syn_connection() {
	//处理SYN请求，在本次tti内，完成SYN的发送以及接收
	for (int source_node_id = 0; source_node_id < route_tcp_node::s_node_count; source_node_id++) {
		if (get_node_array()[source_node_id].get_cur_state() == SOURCE_SEND_SYN) {
			int relay_node_id = get_node_array()[source_node_id].source_select_relay_node();
			if (relay_node_id == -1) {
				get_node_array()[source_node_id].add_next_posible_state(SOURCE_SEND_SYN);
				continue;
			}
			get_node_array()[relay_node_id].add_relay_syn_node(source_node_id);

			//设置source-relay节点的下一刻可能状态
			get_node_array()[source_node_id].add_next_posible_state(SOURCE_RECEIVE_ACK);
			log(context::get_context()->get_tti(),
				source_node_id, relay_node_id, SOURCE,
				get_node_array()[source_node_id].get_cur_state(), SOURCE_RECEIVE_ACK,
				"process_syn_connection：发送syn请求，准等待ack");

			get_node_array()[relay_node_id].add_next_posible_state(RELAY_SEND_ACK);
			log(context::get_context()->get_tti(),
				source_node_id, relay_node_id, RELAY,
				get_node_array()[relay_node_id].get_cur_state(), RELAY_SEND_ACK,
				"process_syn_connection：未知状态，准备发送ack");
		}
	}
}

void route_tcp::process_ack_connection() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		pair<int, vector<int>> ack_response = get_node_array()[relay_node_id].relay_response_ack();

		int accept_node_id = ack_response.first;
		if (accept_node_id == -1) {
			/*此时relay节点可能正处于发送状态，不为其添加下一刻任何可能的状态,下一刻状态由传输函数保证*/
			/*此时relay节点可能处于空闲状态，因为没有进行判断(任意状态下都能进行ack应答)*/
		}
		else {
			get_node_array()[accept_node_id].set_source_ack_state(ACCEPT);

			get_node_array()[accept_node_id].add_next_posible_state(SOURCE_SENDING);
			log(context::get_context()->get_tti(),
				accept_node_id, relay_node_id, SOURCE,
				get_node_array()[accept_node_id].get_cur_state(), SOURCE_SENDING,
				"process_ack_connection：收到接收ack，准备发送数据");
			get_node_array()[relay_node_id].add_next_posible_state(RELAY_RECEIVING);
			log(context::get_context()->get_tti(),
				accept_node_id, relay_node_id, RELAY,
				get_node_array()[relay_node_id].get_cur_state(), RELAY_RECEIVING,
				"process_ack_connection：发送ack，准备接收数");
		}

		for (int reject_node_id : ack_response.second) {
			get_node_array()[reject_node_id].set_source_ack_state(REJECT);

			get_node_array()[reject_node_id].add_next_posible_state(SOURCE_SEND_SYN);
			log(context::get_context()->get_tti(),
				reject_node_id, relay_node_id, SOURCE,
				get_node_array()[reject_node_id].get_cur_state(), SOURCE_SEND_SYN,
				"process_ack_connection：收到拒收ack，准备再次发送syn");
		}
	}

	//上下两个循环在一个TTI内完成，但是有先后顺序，发送ack在前，接收ack在后
	//对ack状态为RECEIVE的node，建立事件逻辑层面的连接
	for (int source_node_id = 0; source_node_id < route_tcp_node::s_node_count; source_node_id++) {
		if (get_node_array()[source_node_id].get_cur_state() == SOURCE_RECEIVE_ACK) {
			if (get_node_array()[source_node_id].get_source_ack_state() == ACCEPT) {
				//获取队列第一个元素(不出队)，进行拷贝，与relay节点关联
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
				log(context::get_context()->get_tti(),
					source_node_id, relay_node_id, SOURCE,
					get_node_array()[source_node_id].get_cur_state(), SOURCE_LINK_RESPONSE,
					"process_transimit_connection：发送完毕，准备等待response");
				get_node_array()[relay_node_id].add_next_posible_state(RELAY_LINK_RESPONSE);
				log(context::get_context()->get_tti(),
					source_node_id, relay_node_id, RELAY,
					get_node_array()[relay_node_id].get_cur_state(), RELAY_LINK_RESPONSE,
					"process_transimit_connection：接收完毕，准备发送response");
			}
			else {
				get_node_array()[source_node_id].add_next_posible_state(SOURCE_SENDING);
				log(context::get_context()->get_tti(),
					source_node_id, relay_node_id, SOURCE,
					get_node_array()[source_node_id].get_cur_state(), SOURCE_SENDING,
					"process_transimit_connection：尚未传完，准备继续发送");
				get_node_array()[relay_node_id].add_next_posible_state(RELAY_RECEIVING);
				log(context::get_context()->get_tti(),
					source_node_id, relay_node_id, RELAY,
					get_node_array()[relay_node_id].get_cur_state(), RELAY_RECEIVING,
					"process_transimit_connection：尚未传完，准备继续接收");
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
				LOGGER << "TTI: " << left << setw(3) << context::get_context()->get_tti() << ",  link[" << left << setw(3) << source_node_id << ", " << left << setw(3) << relay_node_id << "]: { FAILED }" << endl;

				add_failed_event(cur_event);//添加到失败事件列表
				get_node_array()[relay_node_id].remove_relay_event();//弹出

				get_node_array()[source_node_id].add_next_posible_state(SOURCE_SEND_SYN);
				log(context::get_context()->get_tti(),
					source_node_id, relay_node_id, SOURCE,
					get_node_array()[source_node_id].get_cur_state(), SOURCE_SEND_SYN,
					"process_response_connection：丢包，准备重传");
				get_node_array()[relay_node_id].add_next_posible_state(IDLE);
				log(context::get_context()->get_tti(),
					source_node_id, relay_node_id, RELAY,
					get_node_array()[relay_node_id].get_cur_state(), IDLE,
					"process_response_connection：丢包，准备进入空闲状态");
			}
			else {
				get_node_array()[source_node_id].set_source_link_response_state(SUCCESS);
				LOGGER << "TTI: " << left << setw(3) << context::get_context()->get_tti() << ",  link[" << left << setw(3) << source_node_id << ", " << left << setw(3) << relay_node_id << "]: { SUCCEED }" << endl;

				get_node_array()[source_node_id].add_next_posible_state(IDLE);
				get_node_array()[source_node_id].remove_source_event();
				log(context::get_context()->get_tti(),
					source_node_id, relay_node_id, SOURCE,
					get_node_array()[source_node_id].get_cur_state(), IDLE,
					"process_response_connection：成功发送，准备进入空闲状态");
				if (cur_event->is_all_finished()) {
					add_successful_event(cur_event);//添加到成功事件列表
					get_node_array()[relay_node_id].remove_relay_event();//弹出
					get_node_array()[relay_node_id].add_next_posible_state(IDLE);
					log(context::get_context()->get_tti(),
						source_node_id, relay_node_id, RELAY,
						get_node_array()[relay_node_id].get_cur_state(), IDLE,
						"process_response_connection：整条链路完毕，准备进入空闲状态");
				}
				else {
					get_node_array()[relay_node_id].transfer_event_from_relay_queue_to_source_queue();
					get_node_array()[relay_node_id].add_next_posible_state(SOURCE_SEND_SYN);
					log(context::get_context()->get_tti(),
						source_node_id, relay_node_id, RELAY,
						get_node_array()[relay_node_id].get_cur_state(), SOURCE_SEND_SYN,
						"process_response_connection：尚未传输完毕，从接收节点转为发送节点");
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