/*
* =====================================================================================
*
*       Filename:  tmc.cpp
*
*    Description:  业务模型与控制类实现
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  HCF
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<random>
#include<fstream>
#include"gtt.h"
#include"tmc.h"
#include"vue.h"
#include"vue_physics.h"
#include"config.h"
#include"route_tcp.h"
#include"route_udp.h"
#include"reflect/context.h"


using namespace std;

void tmc::statistic() {
	context* __context = context::get_context();
	ofstream success_route_event;
	ofstream event_count;
	ofstream route_count;
	ofstream total_delay;

	success_route_event.open("log/success_event.txt");
	event_count.open("log/event_count.txt");
	route_count.open("log/route_count.txt");
	total_delay.open("log/total_delay.txt");

	object* __object = context::get_context()->get_bean("route");

	if (__object->get_class_id()==route_tcp::class_id) {
		route_tcp* __route_tcp = (route_tcp*)__object;
		for (route_tcp_route_event* tcp_event : __route_tcp->get_successful_event_vec()) {
			success_route_event << tcp_event->to_string();
			route_count << tcp_event->get_through_node_vec().size() << endl;
			total_delay << (tcp_event->get_finished_tti() - tcp_event->get_trigger_tti() + 1) << endl;
		}

		event_count << __route_tcp->get_successful_event_vec().size() << endl;
		event_count << __route_tcp->get_failed_event_vec().size() << endl;
	}
	else {

		route_udp* __route_udp = (route_udp*)__object;
		
		for (route_udp_route_event* udp_event : __route_udp->get_successful_route_event_vec()) {
			success_route_event << udp_event->to_string();
			route_count << udp_event->get_through_node_vec().size() << endl;
			total_delay << (udp_event->get_finished_tti() - udp_event->get_trigger_tti() + 1) << endl;
		}
	}
	
	success_route_event << endl;

	ofstream last_location;
	last_location.open("log/last_location.txt");
	for (int i = 0; i < vue_physics::get_vue_num(); i++) {
		gtt* __gtt = (gtt*)context::get_context()->get_bean("gtt");
		last_location << __gtt->get_vue_array()[i].get_physics_level()->get_absx() << " ";
		last_location << __gtt->get_vue_array()[i].get_physics_level()->get_absy() << " ";
		last_location << endl;
	}

}