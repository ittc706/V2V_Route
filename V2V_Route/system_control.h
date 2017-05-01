#pragma once
#include"reflect/object.h"

class context;
class gtt;
class route;
class wt;
class tmc;

class system_control:public object {
	REGISTE_MEMBER_HEAD(system_control)

	/*------------------˽���ֶ�------------------*/

private:
	/*
	* gttģ�������
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
	* routeģ�������
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
	* tmcģ�������
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
	* wtģ�������
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

	/*----------------�������Ƴ�Ա----------------*/
public:
	/*
	* Ĭ�Ϲ��캯��
	*/
	system_control();

	/*
	* ��������������������Դ
	*/
	~system_control();

	/*
	* ���������캯������Ϊɾ��
	*/
	system_control(const system_control& t_system_control) = delete;

	/*
	* ���ƶ����캯������Ϊɾ��
	*/
	system_control(system_control&& t_system_control) = delete;

	/*
	* ��������ֵ���������Ϊɾ��
	*/
	system_control& operator=(const system_control& t_system_control) = delete;

	/*
	* ���ƶ���ֵ���������Ϊɾ��
	*/
	system_control& operator=(system_control&& t_system_control) = delete;


	/*--------------------�ӿ�--------------------*/
public:
	/*
	* ϵͳ����Ψһ�ӿ�
	*/
	void process();

};