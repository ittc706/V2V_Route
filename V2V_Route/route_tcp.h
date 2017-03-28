#pragma once
#include<vector>
#include<list>
#include"route.h"

enum route_tcp_state {
	SOURCE_SEND_SYN = 1,//source节点发送转发请求给relay节点
	SOURCE_RECEIVE_ACK = 2,//source节点接收relay节点的应答信号
	SOURCE_SENDING = 4,//source节点正在转发数据包


	RELAY_RECEIVE_SYN = 11,//relay节点接收source的转发请求
	RELAY_SEND_ACK = 12,//relay节点发送应答信号
	RELAY_RECEIVE_DECISION = 13,//relay节点接收source的判决信号
	RELAY_RECEIVING = 14,//relay节点正在接收数据包
};

/*
* 对于一个节点，收发矛盾。即同一时刻只能收，或者只能发
* 当一个节点处于收发状态时，回绝一切请求
* 当节点处于收状态时，该节点的作为源节点的信息也将在收完毕后再进行发送，即排在转发消息之后
* 若节点处于空闲状态，且同时收到两个或多个转发请求，随机应答一个，拒绝其他(可以扩展为优先级应答)
*/

class route_tcp_node;
class route_tcp_event {
private:
	static int s_event_count;
private:
	/*
	* 源节点
	*/
	route_tcp_node* const m_source_node;
public:
	route_tcp_node* get_source_node() { return m_source_node; }

	/*
	* 目的节点
	*/
	route_tcp_node* const m_destination_node;
public:
	route_tcp_node* get_destination_node() { return m_destination_node; }

	/*
	* 当前节点
	*/
	route_tcp_node* m_current_node;
public:
	void set_current_node(route_tcp_node* t_route_tcp_node) { m_current_node = t_route_tcp_node; }
	route_tcp_node* get_current_node() { return m_current_node; }

	/*
	* 整个从源节点到目的节点是否传递成功
	*/
	bool is_successful() { return m_current_node == m_destination_node; }

	/*
	* 事件id
	*/
private:
	const int m_event_id;
public:
	int get_event_id() { return m_event_id; }

	/*
	* 数据包总数
	* 即要发送的数据封装成IP数据包的数量，这些IP数据包丢失任意一个，那么整个数据包就算丢失
	*/
private:
	int m_package_num;

	/*
	* 标记本跳当前时刻传输的包编号
	*/
private:
	int m_package_idx = 0;
public:
	int get_package_idx() { return m_package_idx; }

	/*
	* 标记本跳是否传输完毕(无论是否发生丢包)
	*/
private:
	bool m_is_finished = false;
public:
	bool get_is_finished() { return m_is_finished; }

	/*
	* 标记本跳是否发生丢包
	*/
private:
	bool m_is_loss = false;
public:
	bool get_is_loss() { return m_is_loss; }

private:
	route_tcp_event(const route_tcp_event& t_route_tcp_event):
		m_event_id(t_route_tcp_event.m_event_id),
		m_source_node(t_route_tcp_event.m_source_node), 
		m_destination_node(t_route_tcp_event.m_destination_node){
		m_package_num = -1;//<Warn>
	}
public:
	/*
	* 构造函数，提供给事件触发模块调用
	*/
	route_tcp_event(route_tcp_node* t_source_node, route_tcp_node* t_destination_node) :
		m_event_id(s_event_count++), 
		m_source_node(t_source_node),
		m_destination_node(t_destination_node) {
		m_package_num = -1;//<Warn>
	}

	/*
	* 进行数据包的发送
	*/
	void transimit();

	/*
	* 当一跳发送完毕后需要重置其状态
	*/
	void reset();

	/*
	* 拷贝当前事件，考虑一下event_id拷贝前后是否改变<Warn>
	*/
	route_tcp_event* clone();
};

class route_tcp_node {
private:
	/*
	* 邻接列表
	*/
	std::list<route_tcp_node*> m_adjacent_list;

private:
	/*
	* 节点状态
	*/
	route_tcp_state m_state;
	void set_state(route_tcp_state t_state) { m_state = t_state; }
public:
	route_tcp_state get_state() { return m_state; }

};

class route_tcp :public route {
public:
	/*
	* 对整个网络层进行状态更新
	*/
	void update_state();

	/*
	* 随车辆运动而更新邻接列表
	*/
	void update_route_table();

private:
	/*
	* 随机触发事件
	*/
	void event_trigger();

	/*
	* 处理发送请求
	*/
	void process_request_connection();

	/*
	* 处理预期时延信息
	*/
	void process_queue_delay_connection();

	/*
	* 处理判决信息
	*/
	void process_decision_connection();
};



