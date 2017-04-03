#pragma once
#include"enumeration.h"

struct event_trigger_dto {
	int source_vue_id;
	int destination_vue_id;
};

class route {
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