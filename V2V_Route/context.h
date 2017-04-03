#pragma once
#include<vector>
#include<list>

class system_control;
class config_loader;
class global_control_config;
class gtt_config;
class rrm_config;
class tmc_config;
class gtt;
class tmc;
class wt;
class vue;
class sender_event;
class route;

class context {
	/*------------------��Ԫ����------------------*/
	/*
	* ��system_control��Ϊcontext����Ԫ���ṩ�乹��������Ψһʵ����Ȩ��
	*/
	friend class system_control;
	friend class gtt_highspeed;
	friend class gtt_urban;
	/*------------------��̬��Ա�ֶ�------------------*/
private:
	/*
	* ����ģʽ�£�Ψһʵ���ָ��
	*/
	static context* s_singleton_context;

public:
	/*
	* ����ģʽ�£���ȡΨһʵ���ָ��
	*/
	static context* get_context();

	/*----------------�������Ƴ�Ա----------------*/
private:
	/*
	* Ĭ�Ϲ��캯��
	*/
	context();

	/*
	* ������������
	*/
	void singleton_initialize();

	/*
	* ����ע��
	*/
	void dependcy_inject();

	/*
	* �������ɣ��Լ�����ע���Ժ�ִ�еĶ���ĳ�ʼ������
	*/
	void post_processor();

public:
	/*
	* ��������������������Դ
	*/
	~context();

	/*
	* ���������캯������Ϊɾ��
	*/
	context(const context& t_context) = delete;

	/*
	* ���ƶ����캯������Ϊɾ��
	*/
	context(context&& t_context) = delete;

	/*
	* ��������ֵ���������Ϊɾ��
	*/
	context& operator=(const context& t_context) = delete;

	/*
	* ���ƶ���ֵ���������Ϊɾ��
	*/
	context& operator=(context&& t_context) = delete;

	/*------------------������Ա------------------*/
	/*
	* ϵͳ���������󡢱༭����������
	*/
private:
	system_control* m_system_control = nullptr;
	void set_system_control(system_control* t_system_control) {
		m_system_control = t_system_control;
	}
public:
	system_control* get_system_control() {
		return m_system_control;
	}

	/*
	* �����ļ����ض��󡢱༭����������
	*/
private:
	config_loader* m_config_loader = nullptr;
	void set_config_loader(config_loader* t_config_loader) {
		m_config_loader = t_config_loader;
	}
public:
	config_loader* get_config_loader() {
		return m_config_loader;
	}

	/*
	* global_control���ò�������
	*/
private:
	global_control_config* m_global_control_config = nullptr;
	void set_global_control_config(global_control_config* t_global_control_config) {
		m_global_control_config = t_global_control_config;
	}
public:
	global_control_config* get_global_control_config() {
		return m_global_control_config;
	}

	/*
	* gtt���ò�������
	*/
private:
	gtt_config* m_gtt_config = nullptr;
	void set_gtt_config(gtt_config* t_gtt_config) {
		m_gtt_config = t_gtt_config;
	}
public:
	gtt_config* get_gtt_config() {
		return m_gtt_config;
	}

	/*
	* rrm���ò�������
	*/
private:
	rrm_config* m_rrm_config = nullptr;
	void set_rrm_config(rrm_config* t_rrm_config) {
		m_rrm_config = t_rrm_config;
	}
public:
	rrm_config* get_rrm_config() {
		return m_rrm_config;
	}

	/*
	* tmc���ò�������
	*/
private:
	tmc_config* m_tmc_config = nullptr;
	void set_tmc_config(tmc_config* t_tmc_config) {
		m_tmc_config = t_tmc_config;
	}
public:
	tmc_config* get_tmc_config() {
		return m_tmc_config;
	}

	/*
	* tmc���ò�������
	*/
private:
	route_config*

	/*
	* tti,����ʱ��
	*/
private:
	int m_tti = 0;
public:
	void increase_tti() {
		++m_tti;
	}
	int get_tti() {
		return m_tti;
	}

	/*
	* gttʵ��ָ��
	*/
private:
	gtt* m_gtt = nullptr;
	void set_gtt(gtt* t_gtt) {
		m_gtt = t_gtt;
	}
public:
	gtt* get_gtt() {
		return m_gtt;
	}

	/*
	* tmcʵ��ָ��
	*/
private:
	tmc* m_tmc = nullptr;
	void set_tmc(tmc* t_tmc) {
		m_tmc = t_tmc;
	}
public:
	tmc* get_tmc() {
		return m_tmc;
	}

	/*
	* wt����Ϊ�ǵ���ģʽ��������������wt���͵Ķ���
	*/
private:
	wt* m_wt = nullptr;
	void set_wt(wt* t_wt) {
		m_wt = t_wt;
	}
public:
	wt* get_wt() {
		return m_wt;
	}

	/*
	* ����������ָ��
	*/
private:
	vue* m_vue_array = nullptr;
	void set_vue_array(vue* t_vue_array) {
		m_vue_array = t_vue_array;
	}
public:
	vue* get_vue_array() {
		return m_vue_array;
	}

	/*
	* route
	*/
private:
	route* m_route;
	void set_route(route* t_route) {
		m_route = t_route;
	}
public:
	route* get_route() {
		return m_route;
	}
};

