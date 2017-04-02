/*
* =====================================================================================
*
*       Filename:  system_control.cpp
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

#include"system_control.h"
#include"function.h"
#include"context.h"
#include"config.h"
#include"gtt.h"
#include"gtt_highspeed.h"
#include"tmc.h"
#include"vue.h"

using namespace std;

void system_control::set_context(context* t_context) {
	m_context = t_context;
}

context* system_control::get_context() {
	return m_context;
}

system_control::system_control() {

}

system_control::~system_control() {
	memory_clean::safe_delete(m_context);
}

void system_control::process() {
	initialize();//初始化

	while (m_context->get_tti() < m_context->get_global_control_config()->get_ntti()) {
		cout << "TTI: " << m_context->get_tti() << endl;

		//车辆运动
		m_context->get_gtt()->fresh_location();

		m_context->increase_tti();
	}

	m_context->get_tmc()->statistic();
}


void system_control::initialize() {
	//gtt单元初始化工作
	m_context->get_gtt()->initialize();
}

