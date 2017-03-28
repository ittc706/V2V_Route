/*
* =====================================================================================
*
*       Filename:  容器类.cpp
*
*    Description:  系统类实现
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
#include"function.h"
#include"system_control.h"
#include"config_loader.h"
#include"config.h"
#include"gtt_highspeed.h"
#include"gtt_urban.h"
#include"rrm.h"
#include"tmc.h"
#include"wt.h"
#include"vue.h"

using namespace std;

context* context::s_singleton_context = nullptr;

context* context::get_context() {
	if (s_singleton_context == nullptr) {
		context_build();
	}
	return s_singleton_context;
}

void context::set_context(context* t_singleton_context) {
	memory_clean::safe_delete(s_singleton_context);
	s_singleton_context = t_singleton_context;
}

void context::context_build() {
	context* __context = new context();
	set_context(__context);

	__context->initialize();
}

context::context() {

}

void context::initialize() {
	set_system_control(new system_control());
	get_system_control()->set_context(this);

	set_config_loader(new config_loader());

	set_global_control_config(new global_control_config());
	get_global_control_config()->set_config_loader(get_config_loader());
	get_global_control_config()->load();

	set_gtt_config(gtt_config::gtt_config_bind_by_mode(get_global_control_config()->get_gtt_mode()));
	get_gtt_config()->set_config_loader(get_config_loader());
	get_gtt_config()->load();

	set_rrm_config(new rrm_config());
	get_rrm_config()->set_config_loader(get_config_loader());
	get_rrm_config()->load();

	set_tmc_config(new tmc_config());
	get_tmc_config()->set_config_loader(get_config_loader());
	get_tmc_config()->load();

	set_gtt(gtt::gtt_bind_by_mode(get_global_control_config()->get_gtt_mode()));
	get_gtt()->set_config(get_gtt_config());

	set_rrm(new rrm());
	get_rrm()->set_config(get_rrm_config());

	set_tmc(new tmc());
	get_tmc()->set_config(get_tmc_config());

	set_wt(new wt());

	set_event_array();

	set_tti_event_list();
}

void context::dependcy_inject() {
	
}

context::~context() {
	memory_clean::safe_delete(m_vue_array, true);
}

void context::set_system_control(system_control* t_system_control) {
	m_system_control = t_system_control;
}

system_control* context::get_system_control() {
	return m_system_control;
}

void context::set_config_loader(config_loader* t_config_loader) {
	m_config_loader = t_config_loader;
}

config_loader* context::get_config_loader() {
	return m_config_loader;
}

void context::set_global_control_config(global_control_config* t_global_control_config) {
	m_global_control_config = t_global_control_config;
}

global_control_config* context::get_global_control_config() {
	return m_global_control_config;
}

void context::set_gtt_config(gtt_config* t_gtt_config) {
	m_gtt_config = t_gtt_config;
}

gtt_config* context::get_gtt_config() {
	return m_gtt_config;
}

void context::set_rrm_config(rrm_config* t_rrm_config) {
	m_rrm_config = t_rrm_config;
}

rrm_config* context::get_rrm_config() {
	return m_rrm_config;
}

void context::set_tmc_config(tmc_config* t_tmc_config) {
	m_tmc_config = t_tmc_config;
}

tmc_config* context::get_tmc_config() {
	return m_tmc_config;
}

void context::increase_tti() {
	++m_tti;
}

int context::get_tti() {
	return m_tti;
}

void context::set_vue_array(vue* t_vue_array) {
	m_vue_array = t_vue_array;
}

vue* context::get_vue_array() {
	return m_vue_array;
}

void context::set_gtt(gtt* t_gtt) {
	m_gtt = t_gtt;
}

gtt* context::get_gtt() {
	return m_gtt;
}

void context::set_rrm(rrm* t_rrm) {
	m_rrm = t_rrm;
}

rrm* context::get_rrm() {
	return m_rrm;
}

void context::set_tmc(tmc* t_tmc) {
	m_tmc = t_tmc;
}

tmc* context::get_tmc() {
	return m_tmc;
}

void context::set_wt(wt* t_wt) {
	wt::set_resource();
	m_wt = t_wt;
}

wt* context::get_wt() {
	return m_wt;
}

void context::set_event_array() {
	m_event_array = vector<sender_event*>(0);
}

vector<sender_event*>& context::get_event_array() {
	return m_event_array;
}

void context::set_tti_event_list() {
	m_tti_event_list = vector<list<sender_event*>>(get_global_control_config()->get_ntti());
}

vector<std::list<sender_event*>>& context::get_tti_event_list() {
	return m_tti_event_list;
}