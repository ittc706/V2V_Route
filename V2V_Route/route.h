#pragma once

struct event_trigger_dto {
	int source_vue_id;
	int destination_vue_id;
};

class route {
public:
	/*
	* ��������������״̬����
	*/
	virtual void update_state() = 0;

	/*
	* �泵���˶��������ڽ��б�
	*/
	virtual void update_route_table() = 0;
};