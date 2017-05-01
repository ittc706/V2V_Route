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
#include"config.h"
#include"gtt.h"
#include"gtt_highspeed.h"
#include"tmc.h"
#include"vue.h"
#include"route.h"
#include"reflect/context.h"
#include"non_bean_id.h"

using namespace std;

system_control::system_control() {

}

system_control::~system_control() {
	
}

void system_control::process() {
	int *ptti = new int(0);
	context::get_context()->add_non_bean(TTI, ptti);

	while ((*(int*)context::get_context()->get_non_bean(TTI)) < ((global_control_config*)context::get_context()->get_bean("global_control_config"))->get_ntti()) {
		cout << "TTI: " << (*(int*)context::get_context()->get_non_bean(TTI)) << endl;

		//车辆运动
		get_gtt()->fresh_location();

		//路由层更新
		get_route()->process_per_tti();

		++(*(int*)context::get_context()->get_non_bean(TTI));
	}

	get_tmc()->statistic();

	delete ptti;
}

