#pragma once

#include<vector>

class receiver_event;

class sender_event {
	/*------------------友元声明------------------*/
	/*
	* 将tmc设为友元，事件相关的字段需要通过tmc对象来配置
	*/
	friend class tmc;
	friend class vue;
	/*------------------静态成员------------------*/
private:
	static int s_event_count;

	/*----------------拷贝控制成员----------------*/
public:
	/*
	* 默认构造函数
	*/
	sender_event();

	/*
	* 析构函数，负责清理资源
	*/
	~sender_event();

	/*
	* 将拷贝构造函数定义为删除
	*/
	sender_event(const sender_event& t_sender_event) = delete;

	/*
	* 将移动构造函数定义为删除
	*/
	sender_event(sender_event&& t_sender_event) = delete;

	/*
	* 将拷贝赋值运算符定义为删除
	*/
	sender_event& operator=(const sender_event& t_sender_event) = delete;

	/*
	* 将移动赋值运算符定义为删除
	*/
	sender_event& operator=(sender_event&& t_sender_event) = delete;
	
	/*--------------------字段--------------------*/
	/*
	* 发送事件id
	*/
private:
	int m_event_id = s_event_count++;
public:
	int get_event_id();

	/*
	* 与该发送事件关联的所有接收事件
	*/
private:
	std::vector<receiver_event*> m_receiver_event_vec;
public:
	void add_receiver_event_vec(receiver_event* t_receiver_event);
	const std::vector<receiver_event*>& get_receiver_event_vec();

	/*
	* 车辆
	*/
private:
	vue* m_sender_vue;
	void set_sender_vue(vue* t_sender_vue);
public:
	vue* get_sender_vue();
	int get_sender_vue_id();

	/*
	* 占用频段
	*/
private:
	int m_pattern_idx;
public:
	void set_pattern_idx(int t_pattern_idx);
	int get_pattern_idx();

	/*
	* 时隙编号
	*/
private:
	int m_slot_time_idx;
	void set_slot_time_idx(int t_slot_time_idx);
public:
	int get_slot_time_idx();


	/*
	* 数据包总数
	*/
private:
	int m_package_num;

	/*
	* 每个包剩余传输时间
	*/
private:
	std::vector<int> m_remaining_transmission_time_per_package;

	/*
	* 标记当前时刻传输的包编号
	*/
private:
	int m_package_idx = 0;
public:
	int get_package_idx();

	/*
	* 标记是否传输完毕(无论是否发生丢包)
	*/
private:
	bool m_is_finished = false;
public:
	bool get_is_finished();

	/*--------------------接口--------------------*/
public:
	/*
	* 进行事件的传输
	*/
	void transimit();

	/*
	* 判断当前时刻该事件是否处于可发送时隙
	*/
	bool is_transmit_time_slot(int t_tti);

private:
	void update();
};


class receiver_event {
	/*----------------拷贝控制成员----------------*/
public:
	/*
	* 默认构造函数
	*/
	receiver_event() = delete;

	/*
	* 构造函数
	*/
	receiver_event(sender_event* t_sender_event, vue* t_receiver_vue);

	/*
	* 析构函数，负责清理资源
	*/
	~receiver_event();

	/*
	* 将拷贝构造函数定义为删除
	*/
	receiver_event(const receiver_event& t_receiver_event) = delete;

	/*
	* 将移动构造函数定义为删除
	*/
	receiver_event(receiver_event&& t_receiver_event) = delete;

	/*
	* 将拷贝赋值运算符定义为删除
	*/
	receiver_event& operator=(const receiver_event& t_receiver_event) = delete;

	/*
	* 将移动赋值运算符定义为删除
	*/
	receiver_event& operator=(receiver_event&& t_receiver_event) = delete;

	
	/*------------------静态成员------------------*/
private:
	static int s_event_count;

	/*--------------------字段--------------------*/

	/*
	* 接收事件id
	*/
private:
	int m_event_id = s_event_count++;
public:
	int get_event_id();

	/*
	* 该接收事件关联的发送事件
	*/
private:
	sender_event *m_sender_event;
	void set_sender_event(sender_event * t_sender_event);
public:
	sender_event* get_sender_event();


	/*
	* 发送车辆
	*/
private:
	vue *m_sender_vue;
	void set_sender_vue(vue *t_sender_vue);
public:
	vue* get_sender_vue();
	int get_sendr_vue_id();

	/*
	* 接收车辆
	*/
private:
	vue *m_receiver_vue;
	void set_receiver_vue(vue *t_receiver_vue);
public:
	vue* get_receiver_vue();
	int get_receiver_vue_id();

	/*
	* 收发车辆之间的距离
	*/
private:
	double m_distance;
	void set_distance(double t_distance);
public:
	double get_distance();

	/*
	* 占用频段
	*/
private:
	int m_pattern_idx;
public:
	void set_pattern_idx(int t_pattern_idx);
	int get_pattern_idx();

	/*
	* 标记是否发生丢包
	*/
private:
	std::vector<bool> m_package_loss;
	bool m_is_loss = false;
public:
	void set_package_loss(int t_package_loss);
	const std::vector<bool>& get_package_loss();
	bool get_is_loss();

	/*--------------------接口--------------------*/
public:
	/*
	* 进行传输
	*/
	void receive(int t_package_idx, bool t_is_finished);
};