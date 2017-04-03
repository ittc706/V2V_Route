#pragma once
#include<vector>
#include<list>
#include<queue>
#include<set>
#include<utility>
#include<random>
#include"route.h"

enum route_event_type {
	SOURCE,
	RELAY
};

enum route_response_state{
	ACCEPT,
	REJECT
};

enum route_transimit_state {
	SUCCESS,
	FAILURE
};

enum route_tcp_node_state {
	IDLE = 0,//空闲状态

	SOURCE_SEND_SYN = 11,//source节点发送转发请求给relay节点
	SOURCE_RECEIVE_ACK = 12,//source节点接收relay节点的应答信号
	SOURCE_SENDING = 13,//source节点正在转发数据包
	SOURCE_LINK_RESPONSE = 14,//source节点接收relay节点的传输是否成功的响应

	RELAY_SEND_ACK = 2,//relay节点发送应答信号
	RELAY_RECEIVING = 3,//relay节点正在接收数据包
	RELAY_LINK_RESPONSE = 4//relay节点发送传输是否成功

};

/*
* 对于路由层，不涉及车辆，将车辆抽象为node
* 对于一个节点，收发矛盾。即同一时刻只能收，或者只能发
* 当一个节点处于收发状态时，回绝一切请求
* 当节点处于收状态时，该节点的作为源节点的信息也将在收完毕后再进行发送，即排在转发消息之后
* 若节点处于空闲状态，且同时收到两个或多个转发请求，随机应答一个，拒绝其他(可以扩展为优先级应答)
*/

class route_tcp_node;

class route_tcp_event {
private:
	static int s_event_count;

	/*
	* 事件为发送事件或接收事件
	* 接收事件多一个字段，m_source_node_id
	*/
private:
	route_event_type m_type;
	void set_type(route_event_type t_type) {
		m_type = t_type;
	}
public:
	route_event_type get_type() {
		return m_type;
	}
	
	/*
	* m_type为RELAY时,才有效
	* 发送节点id
	*/
private:
	int m_source_node_id;
	void set_source_node_id(int t_source_node_id) {
		m_source_node_id = t_source_node_id;
	}
public:
	int get_source_node_id() {
		return m_source_node_id;
	}

	/*
	* 源节点
	*/
private:
	route_tcp_node* const m_origin_source_node;
public:
	route_tcp_node* get_origin_source_node() { return m_origin_source_node; }

	/*
	* 目的节点
	*/
private:
	route_tcp_node* const m_final_destination_node;
public:
	route_tcp_node* get_final_destination_node() { return m_final_destination_node; }

	/*
	* 当前节点
	* 可能尚未传递给当前节点，如果传输成功，将其添加到m_through_node_vec中
	* 事件传递时会clone，分别关联到两个node上，cur_node指向其关联的node
	* 当事件传输成功，那么pre节点关联的事件将被删除
	* 当事件传输失败，那么next节点关联的事件将被删除
	*/
private:
	route_tcp_node* m_current_node = nullptr;
public:
	void set_current_node(route_tcp_node* t_current_node) { m_current_node = t_current_node; }
	route_tcp_node* get_current_node() { return m_current_node; }

	/*
	* 经历的节点列表(只包含成功传输的)
	*/
private:
	std::vector<route_tcp_node*> m_through_node_vec;
public:
	void add_through_node(route_tcp_node* t_through_node) {
		m_through_node_vec.push_back(t_through_node);
	}
	const std::vector<route_tcp_node*>& get_through_node_vec() {
		return m_through_node_vec;
	}


	/*
	* 整个从源节点到目的节点是否传递成功
	*/
	bool is_all_finished() { 
		return m_current_node == m_final_destination_node; 
	}

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
	bool m_is_curlink_finished = false;
public:
	bool is_curlink_finished() { return m_is_curlink_finished; }

	/*
	* 标记本跳是否发生丢包
	*/
private:
	bool m_is_curlink_loss = false;
public:
	bool get_is_curlink_loss() { return m_is_curlink_loss; }

private:
	route_tcp_event(const route_tcp_event& t_route_tcp_event):
		m_event_id(t_route_tcp_event.m_event_id),
		m_origin_source_node(t_route_tcp_event.m_origin_source_node), 
		m_final_destination_node(t_route_tcp_event.m_final_destination_node){
		m_package_num = -1;//<Warn>
	}
public:
	/*
	* 构造函数，提供给事件触发模块调用
	*/
	route_tcp_event(route_tcp_node* t_source_node, route_tcp_node* t_destination_node) :
		m_event_id(s_event_count++), 
		m_origin_source_node(t_source_node),
		m_final_destination_node(t_destination_node) {
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
	* 拷贝当前事件，拷贝前后event_id不变
	* 当某一跳成功传输，但是标记成功与否的信令传输失败，则会造成多条链路，此时才会调用clone
	*/
	route_tcp_event* clone();
};

class route_tcp_node {
	friend class route_tcp;

private:
	static int s_node_count;
	static std::default_random_engine s_engine;
	/*
	* 正在发送(强调一下:发状态的节点)的node节点
	* 外层下标为pattern编号
	*/
	static std::vector<std::set<route_tcp_node*>> s_node_per_pattern;

private:
	const int m_id = s_node_count++;
public:
	int get_id() {
		return m_id;
	}

	/*
	* 邻接列表
	*/
	std::list<route_tcp_node*> m_adjacent_list;

private:
	/*
	* 节点当前状态状态
	*/
	route_tcp_node_state m_cur_state;
	void set_cur_state(route_tcp_node_state t_cur_state) { m_cur_state = t_cur_state; }
public:
	route_tcp_node_state get_cur_state() { return m_cur_state; }

	/*
	* 节点下一刻可能的状态集合
	*/
	std::set<route_tcp_node_state> m_next_posible_state_set;
	void add_next_posible_state(route_tcp_node_state t_next_state) { 
		m_next_posible_state_set.insert(t_next_state);
	}

	/*
	* 节点收到的ACK响应
	*/
private:
	route_response_state m_source_ack_state;
	void set_source_ack_state(route_response_state t_source_ack_state) {
		m_source_ack_state = t_source_ack_state;
	}
public:
	route_response_state get_source_ack_state() {
		return m_source_ack_state;
	}

	/*
	* 节点收到的link response响应
	*/
private:
	route_transimit_state m_source_link_response_state;
	void set_source_link_response_state(route_transimit_state t_source_link_response_state) {
		m_source_link_response_state = t_source_link_response_state;
	}
public:
	route_transimit_state get_source_link_response_state() {
		return m_source_link_response_state;
	}

private:
	/*
	* 当前节点作为source节点的事件队列，当且仅当：当前节点处于转发状态，并且触发信的事件，此时队列长度为2
	* 其他任何时候队列中只有触发事件或者转发事件
	*/
	std::queue<route_tcp_event*> m_source_event_queue;
	void add_source_event(route_tcp_event* t_event) {
		m_source_event_queue.push(t_event);
	}
	void remove_source_event() {
		m_source_event_queue.pop();
	}
	const std::queue<route_tcp_event*>& get_source_event_queue() {
		return m_source_event_queue;
	}
	/*
	* 当前节点作为relay节点的事件队列
	*/
	std::queue<route_tcp_event*> m_relay_event_queue;
	void add_relay_event(route_tcp_event* t_event) {
		m_relay_event_queue.push(t_event);
	}
	void remove_relay_event() {
		m_relay_event_queue.pop();
	}
	const std::queue<route_tcp_event*>& get_relay_event_queue() {
		return m_relay_event_queue;
	}
	void transfer_event_from_relay_queue_to_source_queue();

	/*
	* 当前节点(作为source节点)请求syn的节点(即relay节点)
	* 该状态由select_relay_node()方法进行维护
	*/
private:
	int m_source_syn_node;
public:
	int get_source_syn_node() {
		return m_source_syn_node;
	}

	/*
	* 当前节点(作为relay节点)接收来自其他车辆的syn请求的列表
	*/
private:
	std::vector<int> m_relay_syn_node_vec;
public:
	void add_relay_syn_node(int t_node_id) {
		m_relay_syn_node_vec.push_back(t_node_id);
	}

public:
	/*
	* 更新节点状态
	*/
	void update_state();

	/*
	* 选择请求转发的车辆
	*/
	int source_select_relay_node();

	/*
	* 对所有syn请求，返回相应的ack
	* 返回结果若为res,则向res.first节点发送ack=接收，向res.second节点发送ack=不接受
	* 若无法响应任何节点，那么res.first返回-1
	*/
	std::pair<int,std::vector<int>> relay_response_ack();
};

class route_tcp :public route {
	/*
	* 让context容器提供依赖注入
	*/
	friend class context;

private:
	route_tcp_node* m_node_array;
	void set_node_array(route_tcp_node* t_node_array) { m_node_array = t_node_array ; }
public:
	route_tcp_node* get_node_array() { return m_node_array; }

private:
	/*
	* 成功/失败传输的事件
	*/
	std::vector<route_tcp_event*> m_successful_event_vec;
	std::vector<route_tcp_event*> m_failed_event_vec;
	void add_successful_event(route_tcp_event* t_successful_event_vec) {
		m_successful_event_vec.push_back(t_successful_event_vec);
	}
	void add_failed_event(route_tcp_event* t_failed_event_vec) {
		m_failed_event_vec.push_back(t_failed_event_vec);
	}
	const std::vector<route_tcp_event*>& get_successful_event_vec() {
		return m_successful_event_vec;
	}
	const std::vector<route_tcp_event*>& get_failed_event_vec(){
		return m_failed_event_vec;
	}

public:
	/*
	* 构造函数
	*/
	route_tcp();

	/*
	* 初始化
	*/
	void initialize()override;

	/*
	* 对整个网络层进行状态更新，对外暴露的接口，每个TTI调用一次即可
	*/
	void process_per_tti()override;

	/*
	* 随车辆运动而更新邻接列表，车辆刷新时调用即可
	*/
	void update_route_table_from_physics_level()override;

private:
	/*
	* 随机触发事件
	*/
	void event_trigger();

	/*
	* 处理请求连接
	*/
	void process_syn_connection();

	/*
	* 处理求情响应
	*/
	void process_ack_connection();

	/*
	* 处理转发数据包
	*/
	void process_transimit_connection();

	/*
	* 返回成功传输信息
	*/
	void process_response_connection();

	/*
	* 更新节点状态
	*/
	void update_node_state();
};



