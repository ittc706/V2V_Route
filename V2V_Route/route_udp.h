#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<set>
#include<utility>
#include<random>
#include<string>
#include"route.h"
#include"context.h"
#include"config.h"

enum route_response_state {
	ACCEPT,
	REJECT
};

enum route_transimit_state {
	SUCCESS,
	FAILURE
};

enum route_udp_node_state {
	/*
	* ����״̬��ֵ������ƹ��ģ��벻Ҫ�������
	*/

	IDLE = 0,//����״̬

	SOURCE_FORWARDING = 1,//source�ڵ�����ת�����ݰ�

	SOURCE_SENDING = 2,//source�ڵ����ڷ������ݰ�



};

/*
* ����·�ɲ㣬���漰����������������Ϊnode
* ����һ���ڵ㣬�շ�ì�ܡ���ͬһʱ��ֻ���գ�����ֻ�ܷ�
* ��һ���ڵ㴦���շ�״̬ʱ���ؾ�һ������
* ���ڵ㴦����״̬ʱ���ýڵ����ΪԴ�ڵ����ϢҲ��������Ϻ��ٽ��з��ͣ�������ת����Ϣ֮��
* ���ڵ㴦�ڿ���״̬����ͬʱ�յ���������ת���������Ӧ��һ�����ܾ�����(������չΪ���ȼ�Ӧ��)
*/

class route_udp_node;

class route_udp_route_event {
private:
	static int s_event_count;

	/*
	* Դ�ڵ�
	*/
private:
	const int m_origin_source_node_id;
public:
	int get_origin_source_node_id() {
		return m_origin_source_node_id;
	}

	/*
	* Ŀ�Ľڵ�
	*/
private:
	const int m_final_destination_node_id;
public:
	int get_final_destination_node_id() {
		return m_final_destination_node_id;
	}

	/*
	* ��ǰ�ڵ�(����ȷ���䵽��ǰ�ڵ㣬����ǰ�ڵ�Ҳ��m_through_node_vec֮��)
	*/
private:
	int m_current_node_id = -1;
public:
	void set_current_node_id(int t_current_node_id) {
		m_current_node_id = t_current_node_id;
		m_through_node_id_vec.push_back(m_current_node_id);
	}
	int get_current_node_id() {
		return m_current_node_id;
	}

	/*
	* �����Ľڵ��б�(ֻ�����ɹ������)
	*/
private:
	std::vector<int> m_through_node_id_vec;
public:
	const std::vector<int>& get_through_node_vec() {
		return m_through_node_id_vec;
	}


	/*
	* ������Դ�ڵ㵽Ŀ�Ľڵ��Ƿ񴫵ݳɹ�
	*/
	bool is_finished() {
		return m_current_node_id == m_final_destination_node_id;
	}

	/*
	* �¼�id
	*/
private:
	const int m_event_id;
public:
	int get_event_id() { return m_event_id; }

	/*
	* ���ݰ�����
	* ��Ҫ���͵����ݷ�װ��IP���ݰ�����������ЩIP���ݰ���ʧ����һ������ô�������ݰ����㶪ʧ
	*/
private:
	const int m_package_num;
public:
	int get_package_num() {
		return m_package_num;
	}
public:
	/*
	* ���캯�����ṩ���¼�����ģ�����
	*/
	route_udp_route_event(int t_source_node, int t_destination_node) :
		m_event_id(s_event_count++),
		m_origin_source_node_id(t_source_node),
		m_final_destination_node_id(t_destination_node),
		m_package_num(context::get_context()->get_tmc_config()->get_package_num()) {
		set_current_node_id(t_source_node);
	}

	/*
	* ������ǰ�¼�������ǰ��event_id����
	* ��ĳһ���ɹ����䣬���Ǳ�ǳɹ����������ʧ�ܣ������ɶ�����·����ʱ�Ż����clone
	*/
	route_udp_route_event* clone();

	/*
	* ������ǰ�¼�������ǰ��event_id����
	* ��ĳһ���ɹ����䣬���Ǳ�ǳɹ����������ʧ�ܣ������ɶ�����·����ʱ�Ż����clone
	*/
	void transfer_to(int t_node_id) {
		set_current_node_id(t_node_id);
	}

	/*
	* תΪ�ַ���
	*/
	std::string to_string();
};

class route_udp_link_event {
private:
	/*
	* ��ǰ��·Դ�ڵ�id
	*/
	const int m_source_node_id;
public:
	int get_source_node_id() {
		return m_source_node_id;
	}

	/*
	* ��ǰ��·Ŀ�Ľڵ�id
	*/
private:
	const int m_destination_node_id;
public:
	int get_destination_node_id() {
		return m_destination_node_id;
	}

	/*
	* ���ݰ�����
	* ��Ҫ���͵����ݷ�װ��IP���ݰ�����������ЩIP���ݰ���ʧ����һ������ô�������ݰ����㶪ʧ
	*/
private:
	const int m_package_num;

	/*
	* ��Ǳ�����ǰʱ�̴���İ����
	*/
private:
	int m_package_idx = 0;
public:
	int get_package_idx() { return m_package_idx; }

	/*
	* ��Ǳ����Ƿ������(�����Ƿ�������)
	*/
private:
	bool m_is_finished = false;
public:
	bool is_finished() { return m_is_finished; }

	/*
	* ��Ǳ����Ƿ�������
	*/
private:
	bool m_is_loss = false;
public:
	bool get_is_loss() { return m_is_loss; }

public:
	route_udp_link_event(int t_source_node_id, int t_destination_node_id, int t_package_num) :
		m_source_node_id(t_source_node_id),
		m_destination_node_id(t_destination_node_id),
		m_package_num(t_package_num) {}

	/*
	* �������ݰ��ķ���
	*/
	void transimit();
};

class route_udp_source_node {
	friend class route_udp_node;
private:
	route_udp_source_node() {}
	/*
	* ��·��ת���¼��б�
	* ��ǰ�ڵ���Ϊsource�ڵ���¼����У����ҽ�������ǰ�ڵ㴦��ת��״̬�����Ҵ����µ��¼�����ʱ���г���Ϊ2
	* �����κ�ʱ�������ֻ�д����¼�����ת���¼�
	*/
private:
	std::queue<route_udp_route_event*> m_event_queue;
public:
	void offer_event(route_udp_route_event* t_event) {
		m_event_queue.push(t_event);
	}
	route_udp_route_event* poll_event() {
		route_udp_route_event* temp = m_event_queue.front();
		m_event_queue.pop();
		return temp;
	}
	route_udp_route_event* peek_event() {
		return m_event_queue.front();
	}
};

class route_udp_relay_node {
	friend class route_udp_node;
private:
	route_udp_relay_node() {}
	/*
	* ��ǰ�ڵ���Ϊrelay�ڵ���¼�����
	*/
private:
	std::queue<route_udp_link_event*> m_event_queue;
public:
	void offer_event(route_udp_link_event* t_event) {
		m_event_queue.push(t_event);
	}
	route_udp_link_event* poll_event() {
		route_udp_link_event* temp = m_event_queue.front();
		m_event_queue.pop();
		return temp;
	}
	route_udp_link_event* peek_event() {
		return m_event_queue.front();
	}
};

class route_udp_node {
	friend class route_udp;

private:
	static int s_node_count;
	static std::default_random_engine s_engine;

	static std::ofstream s_logger;

	static void log_state_update(route_udp_node* t_node);
	/*
	* ���ڷ���(ǿ��һ��:��״̬�Ľڵ�)��node�ڵ�
	* ����±�Ϊpattern���
	*/
	static std::vector<std::set<route_udp_node*>> s_node_per_pattern;

private:
	/*
	*  ָ�����ýڵ�source״̬�Ķ���
	*/
	route_udp_source_node* m_source_node;
	route_udp_source_node* get_source_node() {
		return m_source_node;
	}

	/*
	*  ָ�����ýڵ�relay״̬�Ķ���
	*/
	route_udp_relay_node* m_relay_node;
	route_udp_relay_node* get_relay_node() {
		return m_relay_node;
	}

	/*
	* �ڵ�id
	*/
	const int m_id = s_node_count++;
public:
	int get_id() {
		return m_id;
	}

	/*
	* �ڽ��б�
	*/
private:
	std::vector<int> m_adjacent_list;
public:
	void add_to_adjacent_list(int t_node_id) {
		m_adjacent_list.push_back(t_node_id);
	}
	const std::vector<int>& get_adjacent_list() {
		return m_adjacent_list;
	}

	/*
	* �ڵ㵱ǰ״̬״̬
	*/
private:
	route_udp_node_state m_cur_state = IDLE;
	void set_cur_state(route_udp_node_state t_cur_state) {
		m_cur_state = t_cur_state;
	}
public:
	route_udp_node();

	/*
	* ���ؽڵ㵱ǰʱ�̵�״̬
	*/
	route_udp_node_state get_cur_state() {
		return m_cur_state;
	}

	/*
	* �ڵ���һ�̿��ܵ�״̬����
	*/
private:
	std::set<route_udp_node_state> m_next_possible_state_set;
	void add_next_possible_state(route_udp_node_state t_next_state) {
		m_next_possible_state_set.insert(t_next_state);
	}

public:
	/*
	* ���½ڵ�״̬
	*/
	void update_state();

	/*
	* ѡ������ת���ĳ���
	*/
	int source_select_relay_node();

private:
	void check_state();
};

class route_udp :public route {
	/*
	* ��context�����ṩ����ע��
	*/
	friend class context;

private:
	static std::default_random_engine s_engine;

	static std::ofstream s_logger;
public:
	static std::string state_to_string(route_udp_node_state state);
private:
	static void log_possible_state(int source_node_id, int relay_node_id,
		int cur_node_id, route_udp_node_state cur_state,
		route_udp_node_state next_state, std::string description);

	static void log_event_trigger(int t_origin_node_id, int t_fianl_destination_node_id);

	static void log_link_event_state(int t_source_node_id, int t_relay_node_id, std::string t_description);
private:
	route_udp_node* m_node_array;
public:
	route_udp_node* get_node_array() {
		return m_node_array;
	}

private:
	/*
	* �ɹ�/ʧ�ܴ�����¼�
	*/
	std::vector<route_udp_route_event*> m_successful_event_vec;
	std::vector<route_udp_link_event*> m_failed_event_vec;
	void add_successful_event(route_udp_route_event* t_successful_event_vec) {
		m_successful_event_vec.push_back(t_successful_event_vec);
	}
	void add_failed_event(route_udp_link_event* t_failed_event_vec) {
		m_failed_event_vec.push_back(t_failed_event_vec);
	}
public:
	const std::vector<route_udp_route_event*>& get_successful_event_vec() {
		return m_successful_event_vec;
	}
	const std::vector<route_udp_link_event*>& get_failed_event_vec() {
		return m_failed_event_vec;
	}

public:
	/*
	* ���캯��
	*/
	route_udp();

	/*
	* ��ʼ��
	*/
	void initialize()override;

	/*
	* ��������������״̬���£����Ⱪ¶�Ľӿڣ�ÿ��TTI����һ�μ���
	*/
	void process_per_tti()override;

	/*
	* �泵���˶��������ڽ��б�����ˢ��ʱ���ü���
	*/
	void update_route_table_from_physics_level()override;

private:
	/*
	* ��������¼�
	*/
	void event_trigger();

	/*
	* ������������
	*/
	void process_syn_connection();

	/*
	* ����������Ӧ
	*/
	void process_ack_connection();

	/*
	* ����ת�����ݰ�
	*/
	void process_transimit_connection();

	/*
	* ���سɹ�������Ϣ
	*/
	void process_response_connection();

	/*
	* ���½ڵ�״̬
	*/
	void update_node_state();
};

