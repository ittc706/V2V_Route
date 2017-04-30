#pragma once
#include"enumeration.h"
#include"gtt.h"

enum route_transimit_state {
	SUCCESS,
	FAILURE
};

struct event_trigger_dto {
	int source_vue_id;
	int destination_vue_id;
};

class route:public object {
	/*------------------��Ԫ����------------------*/
	/*
	* ��context��Ϊ��Ԫ������ҪΪ��ע��������
	*/
	friend class context;

	/*--------------------��̬--------------------*/
	/*
	* ����gttģʽ������gtt�������
	*/
	static route* route_bind_by_mode(route_mode t_mode);

public:
	virtual void set_gtt(object* t_gtt) = 0;
	
	virtual gtt* get_gtt() = 0;
	/*
	* ��ʼ��
	*/
	virtual void initialize() = 0;

	/*
	* ��������������״̬���£����Ⱪ¶�Ľӿڣ�ÿ��TTI����һ�μ���
	*/
	virtual void process_per_tti() = 0;

	/*
	* �泵���˶��������ڽ��б�����ˢ��ʱ���ü���
	*/
	virtual void update_route_table_from_physics_level() = 0;
};