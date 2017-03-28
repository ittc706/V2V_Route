#pragma once

#include<vector>
#include<list>
#include<utility>

class sender_event;
class receiver_event;
class vue;

class vue_link {
	/*------------------友元声明------------------*/
	/*
	* 将vue设为友元，由于vue_link的构造函数设为私有，但可由vue来调用
	*/
	friend class vue;

	/*
	* 将tmc设为友元，事件相关的字段需要通过tmc对象来配置
	*/
	friend class tmc;

	/*
	* 将receive_event设为友元
	*/
	friend class receiver_event;
	/*----------------拷贝控制成员----------------*/
private:
	/*
	* 默认构造函数
	* 设为私有，其初始化完全交给vue来完成，禁止生成该类型的实例
	*/
	vue_link();

public:
	/*
	* 析构函数，负责清理资源
	*/
	~vue_link();

	/*
	* 将拷贝构造函数定义为删除
	*/
	vue_link(const vue_link& t_vue_link) = delete;

	/*
	* 将移动构造函数定义为删除
	*/
	vue_link(vue_link&& t_vue_link) = delete;

	/*
	* 将拷贝赋值运算符定义为删除
	*/
	vue_link& operator=(const vue_link& t_vue_link) = delete;

	/*
	* 将移动赋值运算符定义为删除
	*/
	vue_link& operator=(vue_link&& t_vue_link) = delete;

	/*--------------------字段--------------------*/
	/*
	* 指向上层的指针
	*/
private:
	vue* m_superior_level;
	void set_superior_level(vue* t_superior_level);
public:
	vue* get_superior_level();

	/*
	* 成功传输事件链表
	*/
private:
	std::list<receiver_event*> m_success_event_list;
public:
	const std::list<receiver_event*>& get_success_event_list();

	/*
	* 传输失败事件链表(发生了丢包)
	* 一个消息有N个包，任意一个丢包就算丢包
	*/
private:
	std::list<receiver_event*> m_loss_event_list;
public:
	const std::list<receiver_event*>& get_loss_event_list();
	
	/*--------------------接口--------------------*/
public:
	/*
	* 接收连接
	* 维护接受事件列表
	*/
	void receive_connection(sender_event* t_sender_event);

	/*--------------------实现--------------------*/
private:
};