#include"route_tcp.h"

using namespace std;

int route_tcp_event::s_event_count = 0;

void route_tcp_event::transimit() {
	//<Warn>:一跳就在指定频段传输，不再分成多个包分别选择频段传输了
	
	if (++m_package_idx == m_package_num) {
		m_is_finished = true;
	}
	double sinr = -1;
	//<Warn>:sinr计算待补充，以及是否丢包字段的维护

}

void route_tcp_event::reset() {
	//<Warn>
	m_package_idx = 0;
	m_is_finished = false;
	m_is_loss = false;
}

route_tcp_event* route_tcp_event::clone() {
	return nullptr;
}

void route_tcp::update_state() {

}

void route_tcp::update_route_table() {

}

void route_tcp::event_trigger(){

}

void route_tcp::process_request_connection() {

}

void route_tcp::process_queue_delay_connection() {

}

void route_tcp::process_decision_connection() {

}