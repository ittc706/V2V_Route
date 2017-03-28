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
#include"vue_link.h"
#include"vue_network.h"
#include"event.h"
#include"config.h"

using namespace std;

tmc::tmc() {

}

tmc::~tmc() {

}

void tmc::set_config(tmc_config* t_config) {
	m_config = t_config;
}

tmc_config* tmc::get_config() {
	return m_config;
}

void tmc::initialize() {
	context* __context = context::get_context();

	__context->set_tti_event_list();//初始化tti_event_list成员

	default_random_engine e;
	for (int vue_id = 0; vue_id < __context->get_gtt()->get_vue_num(); vue_id++) {
		//初始化车辆类的period_event_next_trigger_tti字段
		__context->get_vue_array()[vue_id].get_network_level()->set_periodic_event_next_trigger_tti(__context->get_tmc_config()->get_congestion_level_num());
		
		//为每辆车配置初始化事件触发时刻
		for (int congestion_level = 0; congestion_level < __context->get_tmc_config()->get_congestion_level_num(); congestion_level++) {
			uniform_int_distribution<int> u(0, get_config()->get_periodic_event_period_per_congestion_level()[congestion_level]);
			__context->get_vue_array()[vue_id].get_network_level()->m_periodic_event_next_trigger_tti[congestion_level] = u(e);
		}	
	}

}

void tmc::event_trigger() {
	context* __context = context::get_context();
	for (int vue_id = 0; vue_id < __context->get_gtt()->get_vue_num(); vue_id++) {
		int tti = __context->get_tti();
		
		int congestion_level = __context->get_vue_array()[vue_id].get_physics_level()->get_congestion_level();
		/*
		* 为什么要用大于，因为，如果用等于来判断，若拥塞等级改变前后，车辆永远不触发事件了
		* 例如之前处于一个拥塞状态，预计触发时间为2000TTI，但是1900时刻，拥塞等级刷新为较低的值，其对应的触发时间为1800TTI，那么在1900TTI以后不可能与1800TTI相等了
		*/
		if (__context->get_vue_array()[vue_id].get_network_level()->get_periodic_event_next_trigger_tti()[congestion_level] > tti) continue;
		
		sender_event* __sender_event = new sender_event();
		__sender_event->set_sender_vue(&__context->get_vue_array()[vue_id]);
		__sender_event->set_slot_time_idx(__context->get_vue_array()[vue_id].get_physics_level()->get_slot_time_idx());

		__context->get_event_array().push_back(__sender_event);
		__context->get_tti_event_list()[tti].push_back(__sender_event);

		//将事件添加到该车辆的发送列表中
		__context->get_vue_array()[vue_id].get_network_level()->add_sender_event(__sender_event);

		for (int congestion_level = 0; congestion_level < __context->get_tmc_config()->get_congestion_level_num(); congestion_level++) {
			__context->get_vue_array()[vue_id].get_network_level()->m_periodic_event_next_trigger_tti[congestion_level] = tti + __context->get_tmc_config()->get_periodic_event_period_per_congestion_level()[congestion_level];
		}
	}
}


void tmc::statistic() {
	ofstream loss_package_distance;
	ofstream distance;
	if (context::get_context()->get_global_control_config()->get_platform() == Windows) {
		loss_package_distance.open("log\\loss_package_distance.txt");
		distance.open("log\\distance.txt");
	}
	else {
		loss_package_distance.open("log/loss_package_distance.txt");
		distance.open("log/distance.txt");
	}
	context* __context = context::get_context();
	for (int vue_id = 0; vue_id < __context->get_gtt()->get_vue_num(); vue_id++) {

		//以事件为最小粒度统计丢包率
		/*for (receiver_event* __receiver_event : __context->get_vue_array()[vue_id].get_link_level()->get_loss_event_list()) {
		loss_package_distance << __receiver_event->get_distance() << " ";
		distance << __receiver_event->get_distance() << " ";
		}

		for (receiver_event* __receiver_event : __context->get_vue_array()[vue_id].get_link_level()->get_success_event_list()) {
		distance << __receiver_event->get_distance() << " ";
		}*/

		//以包为最小粒度统计丢包率
		for (receiver_event* __receiver_event : __context->get_vue_array()[vue_id].get_link_level()->get_loss_event_list()) {
			for (int package_idx = 0; package_idx < __context->get_tmc_config()->get_package_num(); package_idx++) {
				if (__receiver_event->get_package_loss()[package_idx]) {
					loss_package_distance << __receiver_event->get_distance() << " ";
				}
				distance << __receiver_event->get_distance() << " ";
			}
		}

		for (receiver_event* __receiver_event : __context->get_vue_array()[vue_id].get_link_level()->get_success_event_list()) {
			for (int package_idx = 0; package_idx < __context->get_tmc_config()->get_package_num(); package_idx++) {
				distance << __receiver_event->get_distance() << " ";
			}
		}
	}
}