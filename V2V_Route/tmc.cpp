/*
* =====================================================================================
*
*       Filename:  tmc.cpp
*
*    Description:  ҵ��ģ���������ʵ��
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

	__context->set_tti_event_list();//��ʼ��tti_event_list��Ա

}

void tmc::event_trigger() {
	
}


void tmc::statistic() {
	
		
}