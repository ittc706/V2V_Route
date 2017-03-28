#pragma once

#include<random>
#include<vector>
#include<list>
#include<set>

class sender_event;

class vue_network {
	/*------------------��Ԫ����------------------*/
	/*
	* ��vue��Ϊ��Ԫ������vue_network�Ĺ��캯����Ϊ˽�У�������vue������
	*/
	friend class vue;

	/*
	* ��tmc��Ϊ��Ԫ���¼���ص��ֶ���Ҫͨ��tmc����������
	*/
	friend class tmc;

	/*
	* ��rrm��Ϊ��Ԫ���¼���ص��ֶ���Ҫͨ��rrm����������
	*/
	friend class rrm;

	/*
	* ��vue_link��Ϊ��Ԫ
	*/
	friend class vue_link;

	/*
	* ��sender/receiver_event��Ϊ��Ԫ
	*/
	friend class sender_event;
	friend class receiver_event;

	/*------------------��̬��Ա------------------*/
private:
	/*
	* ���������
	*/
	static std::default_random_engine s_engine;

	/*
	* ���ڴ���ķ����¼�
	* ����±�Ϊpattern���
	*/
	static std::vector<std::set<sender_event*>> s_sender_event_per_pattern;

	/*
	* ĳһ����������ϵķ����¼�
	* ����±�Ϊpattern���
	*/
	static std::vector<std::set<sender_event*>> s_sender_event_per_pattern_finished;

	/*----------------�������Ƴ�Ա----------------*/
private:
	/*
	* Ĭ�Ϲ��캯��
	* ��Ϊ˽�У����ʼ����ȫ����vue����ɣ���ֹ���ɸ����͵�ʵ��
	*/
	vue_network();

public:
	/*
	* ��������������������Դ
	*/
	~vue_network();

	/*
	* ���������캯������Ϊɾ��
	*/
	vue_network(const vue_network& t_vue_network) = delete;

	/*
	* ���ƶ����캯������Ϊɾ��
	*/
	vue_network(vue_network&& t_vue_network) = delete;

	/*
	* ��������ֵ���������Ϊɾ��
	*/
	vue_network& operator=(const vue_network& t_vue_network) = delete;

	/*
	* ���ƶ���ֵ���������Ϊɾ��
	*/
	vue_network& operator=(vue_network&& t_vue_network) = delete;

	/*--------------------�ֶ�--------------------*/
	/*
	* ָ���ϲ��ָ��
	*/
private:
	vue* m_superior_level;
	void set_superior_level(vue* t_superior_level);
public:
	vue* get_superior_level();

	/*
	* �����¼���һ�δ���ʱ��
	*/
private:
	std::vector<int> m_periodic_event_next_trigger_tti;
	void set_periodic_event_next_trigger_tti(int t_congestion_level_num);
public:
	const std::vector<int>& get_periodic_event_next_trigger_tti();

private:
	/*
	* �����¼��б�
	*/
	std::list<sender_event*> m_sender_event_list;
	void add_sender_event(sender_event* t_sender_event);

	/*--------------------�ӿ�--------------------*/
public:
	/*
	* �շ�������������(���������ϵ�����)
	*/
	void send_connection();

	/*--------------------ʵ��--------------------*/
private:
	/*
	* ѡ��ռ�õ���Դ����
	*/
	int select_pattern();

	/*
	* ѡ��ռ�õ���Դ����
	*/
	int select_pattern_base();

	/*
	* ��ǿ�����ѡ��(0-1)ʽ
	*/
	int select_pattern_based_on_sensing();

	/*
	* ��ǿ�����ѡ��(�ŵ�ʽ)
	*/
	int select_pattern_based_on_sensing_classical();

	/*
	* �����Ƿ���յ��ź����жϸ�Ƶ���Ƿ�ռ��
	*/
	int select_sensing();

};