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
using namespace std;

enum route_udp_pattern_state {
	IDLE_UDP = -3,//����״̬
	SENDING_UDP = 1,//����״̬
	RECEIVING_UDP = 2,//����״̬
};

enum route_udp_route_event_type {
	HEllO,//����ά���ڽӱ��hello��
	DATA//���ڴ������ݵ����ݰ�
};

enum route_udp_link_event_loss_reason {
	UNKNOW,//����
	LOW_SINR,//SINR������ֵ
	DST_IS_SENDING//���սڵ����ڷ����޷�����
};

struct adjacent_message {
	int pattern_id;
	double sinr;
	std::set<int> infer_node_id;
	int life_time;
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

private:
	/*
	* �������ͣ������ڰ�ͷ�����ã�������ȷ��ͬ���õ����ݰ�
	*/
	const route_udp_route_event_type m_route_event_type;
public:
	route_udp_route_event_type get_route_event_type() {
		return m_route_event_type;
	}

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
	* ���ݰ���С��ӳ���TTI
	*/
private:
	const int m_tti_num;
public:
	int get_tti_num() {
		return m_tti_num;
	}

	/*
	* Hello����С��ӳ��ΪTTI
	*/
private:
	const int m_hello_tti_num;
public:
	int get_hello_tti_num() {
		return m_hello_tti_num;
	}

public:
	/*
	* ���캯�����ṩ���¼�����ģ�����
	*/
	route_udp_route_event(int t_source_node, int t_destination_node,route_udp_route_event_type t_route_event_type) :
		m_route_event_type(t_route_event_type),
		m_event_id(s_event_count++),
		m_origin_source_node_id(t_source_node),
		m_final_destination_node_id(t_destination_node),
		m_hello_tti_num(context::get_context()->get_tmc_config()->get_hello_tti()),
		m_tti_num(context::get_context()->get_tmc_config()->get_package_num()) {
		set_current_node_id(t_source_node);
	}

	/*
	* תΪ�ַ���
	*/
	std::string to_string();
};

class route_udp_link_event {
	friend class route_udp_node;
	friend class route_udp;


private:

	/*
	* ��link_evnent����ʧ�ܵ�ԭ��
	*/
	route_udp_link_event_loss_reason m_loss_reason = UNKNOW;

private:
	/*
	* ��link_evnent��Ŀ�Ľڵ������ݰ������Ƶ�����Ƿ���¼�������ʼ����ǰTTI֮ǰ��һֱ����SENDING״̬
	*/
	bool dst_maintain_sending = true;

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
	const int m_tti_num;

	/*
	* ռ�õ�pattern���
	*/
private:
	int m_pattern_idx;
	void set_pattern_idx(int t_pattern_idx) {
		m_pattern_idx = t_pattern_idx;
	}
public:
	int get_pattern_idx() {
		return m_pattern_idx;
	}

	/*
	* ��Ǳ�����ǰʱ�̴��䵽�׼���TTI
	*/
private:
	int m_tti_idx = 0;
public:
	int get_tti_idx() { return m_tti_idx; }

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
	route_udp_link_event(int t_source_node_id, int t_destination_node_id, int t_pattern_idx, int t_package_num) :
		m_source_node_id(t_source_node_id),
		m_destination_node_id(t_destination_node_id),
		m_pattern_idx(t_pattern_idx),
		m_tti_num(t_package_num) {}

	/*
	* �������ݰ��ķ���
	*/
	void transimit();
};

class route_udp_node {
	friend class route_udp;

private:

	/*
	* ��һ�η���Hello����ʱ��
	*/
	int m_tti_next_hello;

	/*
	* ���ڷ��͵�link_eventָ�룬��Ϊ�������ݰ���һ���ڵ�ͬʱֻ����һ������Ϊ����Hello����Ϊһ���������Լ��������нڵ������
	*/
	std::vector<route_udp_link_event*> sending_link_event;

	/*
	* �ڵ���
	*/
	static int s_node_count;

	/*
	* ���������
	*/
	static std::default_random_engine s_engine;

	/*
	* ���ڷ���(ǿ��һ��:��״̬�Ľڵ�)��node�ڵ�
	* ����±�Ϊpattern���
	*/
	static std::vector<std::set<int>> s_node_id_per_pattern;
public:
	static const std::set<int>& get_node_id_set(int t_pattern_idx);

	/*
	* ��ǰ�ڵ�����ͳ�������
	*/
private:
	std::queue<route_udp_route_event*> m_send_event_queue;
public:
	void offer_send_event_queue(route_udp_route_event* t_event) {
		m_send_event_queue.push(t_event);
	}
	route_udp_route_event* poll_send_event_queue() {
		route_udp_route_event* temp = m_send_event_queue.front();
		m_send_event_queue.pop();
		return temp;
	}
	route_udp_route_event* peek_send_event_queue() {
		return m_send_event_queue.front();
	}
	bool is_send_event_queue_empty() {
		return m_send_event_queue.empty();
	}

private:

	/*
	* ��ǰ�ڵ㣬��ǰʱ�̣�ÿ��pattern��ʹ�����
	* ���Ϊpattern��ţ��ڲ�pair��firstΪ��ǰpattern��״̬����Ϊ���У�IDLE�����ͣ�SENDING�����գ�RECEVING��
    * �ڲ�pair��secondΪ����ǰpattern���ڽ��գ�RECEVING��״̬ʱ����pattern�ϴ��ڵ���ýڵ㷢�͵����ݰ�����
	*/
private:
	std::vector<std::pair<route_udp_pattern_state, int>> m_pattern_state;

private:
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

	//�ڽӱ�pair�ĵ�һ��Ϊ����id���ڶ���Ϊ�ϴν��յ��ó�hello����ʱ�䣨TTI��
	std::vector<pair<int, adjacent_message>> m_adjacent_list;

public:
	//�����ڽӱ�
	void add_to_adjacent_list(int t_node_id, adjacent_message config) {
		context* __context = context::get_context();
		pair<int, adjacent_message> temp(t_node_id, config);
		vector<pair<int, adjacent_message>>::iterator it = m_adjacent_list.begin();
		while (it < m_adjacent_list.end() && it->first != t_node_id) {
			++it;
		}
		//������������ڽӱ�������ڽӱ��м���ó���
		if (it == m_adjacent_list.end()) {
			m_adjacent_list.push_back(temp);
		}
		//��������Ѿ����ڽӱ����ˢ�¸ó����ĸ���ʱ��Ϊ��ǰʱ��
		else {
			m_adjacent_list.erase(it);
			m_adjacent_list.push_back(temp);
		}
	}

	//<Warn>
	const std::vector<pair<int, adjacent_message>> get_adjacent_list() {
		return m_adjacent_list;
	}


public:
	/*
	* ���캯��
	*/
	route_udp_node();

public:
	/*
	* ѡ������ת���ĳ����Լ���Ӧ��Ƶ��
	* first�ֶ�Ϊ����id
	* second�ֶ�ΪƵ�α��
	* ����һ���ֶ�Ϊ-1���ʹ���ѡ��ʧ��
	*/
	std::pair<int, int> select_relay_information();
};

class route_udp :public route {
	/*
	* ��context�����ṩ����ע��
	*/
	friend class context;

private:
	/*
	* ���������
	*/
	static std::default_random_engine s_engine;

	/*
	* ��־�����
	*/
	static std::ofstream s_logger_pattern;
	static std::ofstream s_logger_link;
	static std::ofstream s_logger_event;

	/*
	* ��¼��־
	*/
	static void log_node_pattern(int t_source_node_id,
		int t_relay_node_id,
		int t_cur_node_id,
		int t_pattern_idx,
		route_udp_pattern_state t_from_pattern_state,
		route_udp_pattern_state t_to_pattern_state,
		std::string t_description);

	static std::string pattern_state_to_string(route_udp_pattern_state t_pattern_state);

	static void log_event(int t_origin_node_id, int t_fianl_destination_node_id);

	static void log_link(int t_source_node_id, int t_relay_node_id, std::string t_description,std::string t_loss_reason, int last_time_pattern_id, int current_time_pattern_id);
private:
	/*
	* �ڵ�����
	*/
	route_udp_node* m_node_array;
public:
	route_udp_node* get_node_array() {
		return m_node_array;
	}

private:
	/*
	* �ɹ�/ʧ�ܴ�����¼�
	*/
	std::vector<route_udp_route_event*> m_successful_route_event_vec;
	std::vector<route_udp_route_event*> m_failed_route_event_vec;
	std::vector<route_udp_link_event*> m_failed_link_event_vec;
	void add_successful_route_event(route_udp_route_event* t_successful_route_event_vec) {
		m_successful_route_event_vec.push_back(t_successful_route_event_vec);
	}
	void add_failed_route_event(route_udp_route_event* t_failed_route_event_vec) {
		m_failed_route_event_vec.push_back(t_failed_route_event_vec);
	}
	void add_failed_link_event(route_udp_link_event* t_failed_link_event_vec) {
		m_failed_link_event_vec.push_back(t_failed_link_event_vec);
	}

public:
	const std::vector<route_udp_route_event*>& get_successful_route_event_vec() {
		return m_successful_route_event_vec;
	}
	const std::vector<route_udp_link_event*>& get_failed_link_event_vec() {
		return m_failed_link_event_vec;
	}
	const std::vector<route_udp_route_event*>& get_failed_route_event_vec() {
		return m_failed_route_event_vec;
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
	* ά���ڽӱ���Ҫɾ����ʱ�Ľڵ�
	*/
	void update_adjacent_list();

	/*
	* ����Ҫ��ʼ���͵��¼�
	*/
	void start_sending_data();

	/*
	* ���䵱ǰTTI���ڵ��¼�
	*/
	void transmit_data();

};
