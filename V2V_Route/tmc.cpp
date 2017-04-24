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
#include"context.h"
#include"gtt.h"
#include"tmc.h"
#include"vue.h"
#include"vue_physics.h"
#include"config.h"
#include"route_tcp.h"
#include"route_udp.h"

using namespace std;

void tmc::statistic() {
	context* __context = context::get_context();
	ofstream success_route_event;
	ofstream failed_route_event;
	if (context::get_context()->get_global_control_config()->get_platform() == Windows) {
		success_route_event.open("log\\success_event.txt");
		failed_route_event.open("log\\failed_event.txt");
	}
	else {
		success_route_event.open("log/success_event.txt");
		failed_route_event.open("log/failed_event.txt");
	}

	if (context::get_context()->get_global_control_config()->get_route_mode() == TCP) {
		route_tcp* __route = (route_tcp*)(__context->get_route());
		success_route_event << "total success event: " << __route->get_successful_event_vec().size() << endl;
		failed_route_event << "total failed event: " << __route->get_failed_event_vec().size() << endl;

		for (route_tcp_route_event* tcp_event : __route->get_successful_event_vec()) {
			success_route_event << tcp_event->to_string();
		}
	}
	else {
		route_udp* __route = (route_udp*)(__context->get_route());
		success_route_event << "total success event: " << __route->get_successful_route_event_vec().size() << endl;
		failed_route_event << "total failed event: " << __route->get_failed_route_event_vec().size() << endl;
		for (route_udp_route_event* udp_event : __route->get_successful_route_event_vec()) {
			success_route_event << udp_event->to_string();
		}
	}
	
	success_route_event << endl;
}