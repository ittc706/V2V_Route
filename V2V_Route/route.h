#pragma once

struct event_trigger_dto {
	int source_vue_id;
	int destination_vue_id;
};

class route {
public:
	/*
	* 对整个网络层进行状态更新，对外暴露的接口，每个TTI调用一次即可
	*/
	virtual void update_state() = 0;

	/*
	* 随车辆运动而更新邻接列表，车辆刷新时调用即可
	*/
	virtual void update_route_table() = 0;
};