#pragma once
#include<vector>
#include<list>
#include"route.h"

enum route_tcp_state {
	SOURCE_SEND_SYN = 1,//source�ڵ㷢��ת�������relay�ڵ�
	SOURCE_RECEIVE_ACK = 2,//source�ڵ����relay�ڵ��Ӧ���ź�
	SOURCE_SENDING = 4,//source�ڵ�����ת�����ݰ�


	RELAY_RECEIVE_SYN = 11,//relay�ڵ����source��ת������
	RELAY_SEND_ACK = 12,//relay�ڵ㷢��Ӧ���ź�
	RELAY_RECEIVE_DECISION = 13,//relay�ڵ����source���о��ź�
	RELAY_RECEIVING = 14,//relay�ڵ����ڽ������ݰ�
};

/*
* ����һ���ڵ㣬�շ�ì�ܡ���ͬһʱ��ֻ���գ�����ֻ�ܷ�
* ��һ���ڵ㴦���շ�״̬ʱ���ؾ�һ������
* ���ڵ㴦����״̬ʱ���ýڵ����ΪԴ�ڵ����ϢҲ��������Ϻ��ٽ��з��ͣ�������ת����Ϣ֮��
* ���ڵ㴦�ڿ���״̬����ͬʱ�յ���������ת���������Ӧ��һ�����ܾ�����(������չΪ���ȼ�Ӧ��)
*/

class route_tcp_node;
class route_tcp_event {
private:
	static int s_event_count;
private:
	/*
	* Դ�ڵ�
	*/
	route_tcp_node* const m_source_node;
public:
	route_tcp_node* get_source_node() { return m_source_node; }

	/*
	* Ŀ�Ľڵ�
	*/
	route_tcp_node* const m_destination_node;
public:
	route_tcp_node* get_destination_node() { return m_destination_node; }

	/*
	* ��ǰ�ڵ�
	*/
	route_tcp_node* m_current_node;
public:
	void set_current_node(route_tcp_node* t_route_tcp_node) { m_current_node = t_route_tcp_node; }
	route_tcp_node* get_current_node() { return m_current_node; }

	/*
	* ������Դ�ڵ㵽Ŀ�Ľڵ��Ƿ񴫵ݳɹ�
	*/
	bool is_successful() { return m_current_node == m_destination_node; }

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
	int m_package_num;

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
	bool get_is_finished() { return m_is_finished; }

	/*
	* ��Ǳ����Ƿ�������
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
	* ���캯�����ṩ���¼�����ģ�����
	*/
	route_tcp_event(route_tcp_node* t_source_node, route_tcp_node* t_destination_node) :
		m_event_id(s_event_count++), 
		m_source_node(t_source_node),
		m_destination_node(t_destination_node) {
		m_package_num = -1;//<Warn>
	}

	/*
	* �������ݰ��ķ���
	*/
	void transimit();

	/*
	* ��һ��������Ϻ���Ҫ������״̬
	*/
	void reset();

	/*
	* ������ǰ�¼�������һ��event_id����ǰ���Ƿ�ı�<Warn>
	*/
	route_tcp_event* clone();
};

class route_tcp_node {
private:
	/*
	* �ڽ��б�
	*/
	std::list<route_tcp_node*> m_adjacent_list;

private:
	/*
	* �ڵ�״̬
	*/
	route_tcp_state m_state;
	void set_state(route_tcp_state t_state) { m_state = t_state; }
public:
	route_tcp_state get_state() { return m_state; }

};

class route_tcp :public route {
public:
	/*
	* ��������������״̬����
	*/
	void update_state();

	/*
	* �泵���˶��������ڽ��б�
	*/
	void update_route_table();

private:
	/*
	* ��������¼�
	*/
	void event_trigger();

	/*
	* ����������
	*/
	void process_request_connection();

	/*
	* ����Ԥ��ʱ����Ϣ
	*/
	void process_queue_delay_connection();

	/*
	* �����о���Ϣ
	*/
	void process_decision_connection();
};



