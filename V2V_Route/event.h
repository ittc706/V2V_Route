#pragma once

#include<vector>

class receiver_event;

class sender_event {
	/*------------------��Ԫ����------------------*/
	/*
	* ��tmc��Ϊ��Ԫ���¼���ص��ֶ���Ҫͨ��tmc����������
	*/
	friend class tmc;
	friend class vue;
	/*------------------��̬��Ա------------------*/
private:
	static int s_event_count;

	/*----------------�������Ƴ�Ա----------------*/
public:
	/*
	* Ĭ�Ϲ��캯��
	*/
	sender_event();

	/*
	* ��������������������Դ
	*/
	~sender_event();

	/*
	* ���������캯������Ϊɾ��
	*/
	sender_event(const sender_event& t_sender_event) = delete;

	/*
	* ���ƶ����캯������Ϊɾ��
	*/
	sender_event(sender_event&& t_sender_event) = delete;

	/*
	* ��������ֵ���������Ϊɾ��
	*/
	sender_event& operator=(const sender_event& t_sender_event) = delete;

	/*
	* ���ƶ���ֵ���������Ϊɾ��
	*/
	sender_event& operator=(sender_event&& t_sender_event) = delete;
	
	/*--------------------�ֶ�--------------------*/
	/*
	* �����¼�id
	*/
private:
	int m_event_id = s_event_count++;
public:
	int get_event_id();

	/*
	* ��÷����¼����������н����¼�
	*/
private:
	std::vector<receiver_event*> m_receiver_event_vec;
public:
	void add_receiver_event_vec(receiver_event* t_receiver_event);
	const std::vector<receiver_event*>& get_receiver_event_vec();

	/*
	* ����
	*/
private:
	vue* m_sender_vue;
	void set_sender_vue(vue* t_sender_vue);
public:
	vue* get_sender_vue();
	int get_sender_vue_id();

	/*
	* ռ��Ƶ��
	*/
private:
	int m_pattern_idx;
public:
	void set_pattern_idx(int t_pattern_idx);
	int get_pattern_idx();

	/*
	* ʱ϶���
	*/
private:
	int m_slot_time_idx;
	void set_slot_time_idx(int t_slot_time_idx);
public:
	int get_slot_time_idx();


	/*
	* ���ݰ�����
	*/
private:
	int m_package_num;

	/*
	* ÿ����ʣ�ഫ��ʱ��
	*/
private:
	std::vector<int> m_remaining_transmission_time_per_package;

	/*
	* ��ǵ�ǰʱ�̴���İ����
	*/
private:
	int m_package_idx = 0;
public:
	int get_package_idx();

	/*
	* ����Ƿ������(�����Ƿ�������)
	*/
private:
	bool m_is_finished = false;
public:
	bool get_is_finished();

	/*--------------------�ӿ�--------------------*/
public:
	/*
	* �����¼��Ĵ���
	*/
	void transimit();

	/*
	* �жϵ�ǰʱ�̸��¼��Ƿ��ڿɷ���ʱ϶
	*/
	bool is_transmit_time_slot(int t_tti);

private:
	void update();
};


class receiver_event {
	/*----------------�������Ƴ�Ա----------------*/
public:
	/*
	* Ĭ�Ϲ��캯��
	*/
	receiver_event() = delete;

	/*
	* ���캯��
	*/
	receiver_event(sender_event* t_sender_event, vue* t_receiver_vue);

	/*
	* ��������������������Դ
	*/
	~receiver_event();

	/*
	* ���������캯������Ϊɾ��
	*/
	receiver_event(const receiver_event& t_receiver_event) = delete;

	/*
	* ���ƶ����캯������Ϊɾ��
	*/
	receiver_event(receiver_event&& t_receiver_event) = delete;

	/*
	* ��������ֵ���������Ϊɾ��
	*/
	receiver_event& operator=(const receiver_event& t_receiver_event) = delete;

	/*
	* ���ƶ���ֵ���������Ϊɾ��
	*/
	receiver_event& operator=(receiver_event&& t_receiver_event) = delete;

	
	/*------------------��̬��Ա------------------*/
private:
	static int s_event_count;

	/*--------------------�ֶ�--------------------*/

	/*
	* �����¼�id
	*/
private:
	int m_event_id = s_event_count++;
public:
	int get_event_id();

	/*
	* �ý����¼������ķ����¼�
	*/
private:
	sender_event *m_sender_event;
	void set_sender_event(sender_event * t_sender_event);
public:
	sender_event* get_sender_event();


	/*
	* ���ͳ���
	*/
private:
	vue *m_sender_vue;
	void set_sender_vue(vue *t_sender_vue);
public:
	vue* get_sender_vue();
	int get_sendr_vue_id();

	/*
	* ���ճ���
	*/
private:
	vue *m_receiver_vue;
	void set_receiver_vue(vue *t_receiver_vue);
public:
	vue* get_receiver_vue();
	int get_receiver_vue_id();

	/*
	* �շ�����֮��ľ���
	*/
private:
	double m_distance;
	void set_distance(double t_distance);
public:
	double get_distance();

	/*
	* ռ��Ƶ��
	*/
private:
	int m_pattern_idx;
public:
	void set_pattern_idx(int t_pattern_idx);
	int get_pattern_idx();

	/*
	* ����Ƿ�������
	*/
private:
	std::vector<bool> m_package_loss;
	bool m_is_loss = false;
public:
	void set_package_loss(int t_package_loss);
	const std::vector<bool>& get_package_loss();
	bool get_is_loss();

	/*--------------------�ӿ�--------------------*/
public:
	/*
	* ���д���
	*/
	void receive(int t_package_idx, bool t_is_finished);
};