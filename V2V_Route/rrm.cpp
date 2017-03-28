/*
* =====================================================================================
*
*       Filename:  rrm.cpp
*
*    Description:  无限资源管理类实现
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

#include"context.h"
#include"config.h"
#include"event.h"
#include"gtt.h"
#include"rrm.h"
#include"vue.h"
#include"vue_link.h"
#include"vue_network.h"

using namespace std;

rrm::rrm() {

}

rrm::~rrm() {

}

void rrm::set_config(rrm_config* t_config) {
	m_config = t_config;
}

rrm_config* rrm::get_config() {
	return m_config;
}

void rrm::initialize() {
	vue_network::s_sender_event_per_pattern.assign(m_config->get_pattern_num(),set<sender_event*>());
	vue_network::s_sender_event_per_pattern_finished.assign(m_config->get_pattern_num(), set<sender_event*>());
}

void rrm::schedule() {
	context* __context = context::get_context();

	//首先，将当前时刻触发的事件，建立与接收车辆的关联
	for (int vue_id = 0; vue_id < __context->get_gtt()->get_vue_num(); vue_id++) {
		__context->get_vue_array()[vue_id].get_network_level()->send_connection();
	}

	//进行传输
	for (int pattern_idx = 0; pattern_idx < __context->get_rrm_config()->get_pattern_num(); pattern_idx++) {
		for (sender_event *__sender_event : vue_network::s_sender_event_per_pattern[pattern_idx]) {
			__sender_event->transimit();
		}
		for (sender_event *__finished_sender_event : vue_network::s_sender_event_per_pattern_finished[pattern_idx]) {
			vue_network::s_sender_event_per_pattern[pattern_idx].erase(__finished_sender_event);
		}
		vue_network::s_sender_event_per_pattern_finished[pattern_idx].clear();
	}
}