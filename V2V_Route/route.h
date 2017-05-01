#pragma once
#include"enumeration.h"
#include"reflect/object.h"

enum route_transimit_state {
	SUCCESS,
	FAILURE
};

struct event_trigger_dto {
	int source_vue_id;
	int destination_vue_id;
};

class gtt;
class v2x_time;

class route:public object {
public:
	/*
	* ��ȡʱ�����
	*/
	virtual v2x_time* get_time() = 0;

	/*
	* ��ȡgtt������
	*/

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