#include"route_tcp.h"
#include"context.h"
using namespace std;

int route_tcp_event::s_event_count = 0;

void route_tcp_event::transimit() {
	//<Warn>:一跳就在指定频段传输，不再分成多个包分别选择频段传输了
	
	if (++m_package_idx == m_package_num) {
		m_is_curlink_finished = true;
	}
	double sinr = -1;
	//<Warn>:sinr计算待补充，以及是否丢包字段的维护

}

void route_tcp_event::reset() {
	//<Warn>
	m_package_idx = 0;
	m_is_curlink_finished = false;
	m_is_curlink_loss = false;
}

route_tcp_event* route_tcp_event::clone() {
	//<Warn>
	return nullptr;
}

int route_tcp_node::s_node_count = 0;

std::vector<std::set<route_tcp_node*>> route_tcp_node::s_node_per_pattern;

route_tcp_node_state route_tcp_node::update_next_state() {
	//<Warn>:根据节点状态优先级的大小，选择下一刻的状态
	return INVALID;
}

void route_tcp_node::transfer_event_from_relay_queue_to_source_queue() {
	add_source_event(m_relay_event_queue.front());
	m_relay_event_queue.pop();
}

int route_tcp_node::source_select_relay_node() {
	int res = -1;

	//<Warn>待补充

	m_source_syn_node = res;//维护改状态
	return res;
}

std::pair<int, std::vector<int>> route_tcp_node::relay_response_syn() {
	//<Warn>待补充
	return std::pair<int, std::vector<int>>();
}

route_tcp::route_tcp() {
	//<Warn>:根据配置文件初始化s_node_per_pattern

}

void route_tcp::process_per_tti() {
	
}

void route_tcp::update_route_table() {

}

void route_tcp::event_trigger(){

}

void route_tcp::process_syn_connection() {
	//处理SYN请求，在本次tti内，完成SYN的发送以及接收
	for (int source_node_id = 0; source_node_id < route_tcp_node::s_node_count; source_node_id++) {
		if (get_node_array()[source_node_id].get_cur_state() == SOURCE_SEND_SYN) {
			int relay_node_id = get_node_array()[source_node_id].source_select_relay_node();
			get_node_array()[relay_node_id].add_relay_syn_node(source_node_id);
			get_node_array()[source_node_id].add_next_posible_state(SOURCE_RECEIVE_ACK);
			get_node_array()[relay_node_id].add_next_posible_state(RELAY_SEND_ACK);
		}
	}
}

void route_tcp::process_ack_connection() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		if (get_node_array()[relay_node_id].get_cur_state() == RELAY_SEND_ACK) {
			pair<int, vector<int>> ack_response = get_node_array()[relay_node_id].relay_response_syn();
			
			int accept_node_id = ack_response.first;
			if (accept_node_id == -1) {//当前节点无法响应任何请求节点
				//此时relay节点可能正处于发送状态，不为其添加下一刻任何可能的状态
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
	}

	//上下两个循环在一个TTI内完成，但是有先后顺序，发送ack在前，接收ack在后
	//对ack状态为RECEIVE的node，建立事件逻辑层面的连接
	for (int source_node_id = 0; source_node_id < route_tcp_node::s_node_count; source_node_id++) {
		if (get_node_array()[source_node_id].get_cur_state() == SOURCE_SEND_SYN) {
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
			route_tcp_event* cur_event = get_node_array()[relay_node_id].get_relay_event_queue().front();
			cur_event->transimit();
			if (cur_event->get_is_curlink_finished()) {
				if (cur_event->get_is_curlink_loss()) {
					int source_node_id = cur_event->get_through_node_vec().back()->get_id();
					get_node_array()[source_node_id].set_source_link_response_state(FAILURE);
					add_failed_event(cur_event);//添加到失败事件列表
					get_node_array()[relay_node_id].remove_relay_event();//弹出
				}
				else {
					int source_node_id = cur_event->get_through_node_vec().back()->get_id();
					get_node_array()[source_node_id].set_source_link_response_state(SUCCESS);
					if (cur_event->is_all_finished()) {
						add_successful_event(cur_event);//添加到成功事件列表
						get_node_array()[relay_node_id].remove_relay_event();//弹出
					}
					else {
						get_node_array()[relay_node_id].transfer_event_from_relay_queue_to_source_queue();
					}
				}
			}
		}
	}
}

void route_tcp::process_response_connection() {

}

void route_tcp::update_node_state() {

}