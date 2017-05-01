#pragma once
#include"reflect/object.h"

class context;
class gtt;
class route;
class wt;
class tmc;

class system_control:public object {
	REGISTE_MEMBER_HEAD(system_control)

	/*------------------私有字段------------------*/

private:
	/*
	* gtt模块控制器
	*/
	gtt* m_gtt;
	void set_gtt(object* t_gtt) {
		m_gtt = (gtt*)t_gtt;
	}
public:
	gtt* get_gtt() {
		return m_gtt;
	}

	/*
	* route模块控制器
	*/
private:
	route* m_route;
	void set_route(object* t_route) {
		m_route = (route*)t_route;
	}
public:
	route* get_route() {
		return m_route;
	}

	/*
	* tmc模块控制器
	*/
private:
	tmc* m_tmc;
	void set_tmc(object* t_tmc) {
		m_tmc = (tmc*)t_tmc;
	}
public:
	tmc* get_tmc() {
		return m_tmc;
	}

	/*
	* wt模块控制器
	*/
private:
	wt* m_wt;
	void set_wt(object* t_wt) {
		m_wt = (wt*)t_wt;
	}
public:
	wt* get_wt() {
		return m_wt;
	}

	/*----------------拷贝控制成员----------------*/
public:
	/*
	* 默认构造函数
	*/
	system_control();

	/*
	* 析构函数，负责清理资源
	*/
	~system_control();

	/*
	* 将拷贝构造函数定义为删除
	*/
	system_control(const system_control& t_system_control) = delete;

	/*
	* 将移动构造函数定义为删除
	*/
	system_control(system_control&& t_system_control) = delete;

	/*
	* 将拷贝赋值运算符定义为删除
	*/
	system_control& operator=(const system_control& t_system_control) = delete;

	/*
	* 将移动赋值运算符定义为删除
	*/
	system_control& operator=(system_control&& t_system_control) = delete;


	/*--------------------接口--------------------*/
public:
	/*
	* 系统运行唯一接口
	*/
	void process();

};