/*
* =====================================================================================
*
*       Filename:  vue_network.cpp
*
*    Description:  �����������㲿��ʵ��
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

#include<iostream>
#include"context.h"
#include"config.h"
#include"gtt.h"
#include"vue.h"
#include"vue_physics.h"
#include"vue_link.h"
#include"vue_network.h"
#include"event.h"
#include<memory.h>

using namespace std;

default_random_engine vue_network::s_engine(0);

vector<set<sender_event*>> vue_network::s_sender_event_per_pattern;
vector<set<sender_event*>> vue_network::s_sender_event_per_pattern_finished;

vue_network::vue_network() {

}

vue_network::~vue_network() {

}

void vue_network::set_superior_level(vue* t_superior_level) {
	m_superior_level = t_superior_level;
}

vue* vue_network::get_superior_level() {
	return m_superior_level;
}

void vue_network::set_periodic_event_next_trigger_tti(int t_congestion_level_num) {
	m_periodic_event_next_trigger_tti.assign(t_congestion_level_num, 0);
}

const std::vector<int>& vue_network::get_periodic_event_next_trigger_tti() {
	return m_periodic_event_next_trigger_tti;
}

void vue_network::add_sender_event(sender_event* t_sender_event) {
	m_sender_event_list.push_back(t_sender_event);
}

void vue_network::send_connection() {
	context* __context = context::get_context();
	vue* vue_ary = __context->get_vue_array();

	list<sender_event*>::iterator it = m_sender_event_list.begin();
	while (it != m_sender_event_list.end()) {
		sender_event* __sender_event = *it;

		//ѡ����Ƶ��
		int pattern_idx = select_pattern();

		//<Warn>:��û�п�ѡPatternʱ�������˱ܣ���һTTI����ѡ�񣬿����޸�Ϊ�˱�һ��ʱ�䣬��Ҫ��sender_event������˱ܴ���С�Ȳ��������Ҹ�����˱�ʱ��(�������趨ֵ����ֱ���ж�����)
		if (pattern_idx == -1) {
			++it;
			continue;
		}

		__sender_event->set_pattern_idx(pattern_idx);
		
		s_sender_event_per_pattern[pattern_idx].insert(__sender_event);

		if (__sender_event->get_package_idx() == 0) {
			if (__sender_event->get_receiver_event_vec().size() != 0) throw logic_error("logic error");
			//���������г�����������
			for (int vue_id = 0; vue_id < __context->get_gtt()->get_vue_num(); vue_id++) {
				if (vue_id == __sender_event->get_sender_vue_id()) continue;
				vue_ary[vue_id].get_link_level()->receive_connection(__sender_event);
			}
		}
		it = m_sender_event_list.erase(it);
	}
}

int vue_network::select_pattern() {
	int altorithm = context::get_context()->get_rrm_config()->get_select_altorithm();
	switch (altorithm) {
	case 1:
		return select_pattern_base();
		break;
	case 2:
		return select_pattern_based_on_sensing();
		break;
	case 3:
		return select_pattern_based_on_sensing_classical();
		break;
	case 4:
		return select_sensing();
	default:
		throw logic_error("altorithm config error");
	}
}

int vue_network::select_pattern_base() {
	uniform_int_distribution<int> u(0, context::get_context()->get_rrm_config()->get_pattern_num() - 1);
	return u(s_engine);
}

int vue_network::select_sensing() {
	context* __context = context::get_context();
	int pattern_num = __context->get_rrm_config()->get_pattern_num();
	int vue_id = get_superior_level()->get_physics_level()->get_vue_id();
	auto p = __context->get_vue_array()[vue_id].get_physics_level();
	//��ѡpattern����
	vector<int> candidate_pattern;

	for (int pattern_idx = 0; pattern_idx < pattern_num; pattern_idx++) {
		if (p->m_pattern_occupied[pattern_idx] == false){
			candidate_pattern.push_back(pattern_idx);
		}
	}

	//ѡ����Ϻ����patternʹ���������
	memset(p->m_pattern_occupied, false, sizeof(p->m_pattern_occupied));

	//����޿�ѡpattern������-1
	if (candidate_pattern.size() == 0) return -1;

	uniform_int_distribution<int> u(0, static_cast<int>(candidate_pattern.size()) - 1);

	return candidate_pattern[u(s_engine)];
}

int vue_network::select_pattern_based_on_sensing() {
	context* __context = context::get_context();
	int pattern_num = __context->get_rrm_config()->get_pattern_num();
	vector<double> pattern_cumulative_power(pattern_num,0);

	double noise_power = pow(10, -17.4);
	int subcarrier_num = context::get_context()->get_rrm_config()->get_rb_num_per_pattern() * 12;
	double send_power = pow(10, (23 - 10 * log10(subcarrier_num * 15 * 1000)) / 10);
	int vue_id = get_superior_level()->get_physics_level()->get_vue_id();

	//����ÿ��Pattern�ϵ��ۼƹ���
	for (int pattern_idx = 0; pattern_idx < pattern_num; pattern_idx++) {
		for (sender_event* __sender_event : s_sender_event_per_pattern[pattern_idx]) {
			int inter_vue_id = __sender_event->get_sender_vue_id();
			pattern_cumulative_power[pattern_idx] += noise_power + vue_physics::get_pl(vue_id, inter_vue_id)*send_power;
		}
	}

	//���������ֵ	
	double power_boundary = send_power * 1e-13;
	//�����ѡpattern
	vector<int> candidate_pattern;
	for (int pattern_idx = 0; pattern_idx < pattern_num; pattern_idx++) {
		if (pattern_cumulative_power[pattern_idx] < power_boundary) {
			candidate_pattern.push_back(pattern_idx);
		}
	}

	//����޿�ѡpattern������-1
	if (candidate_pattern.size() == 0) return -1;

	uniform_int_distribution<int> u(0, static_cast<int>(candidate_pattern.size()) - 1);

	return candidate_pattern[u(s_engine)];

}


int vue_network::select_pattern_based_on_sensing_classical() {
	context* __context = context::get_context();
	int pattern_num = __context->get_rrm_config()->get_pattern_num();
	
	double noise_power = pow(10, -17.4);
	//��ʼ��Ϊ�������ʣ�����Ϊ0
	vector<double> pattern_cumulative_power(pattern_num, noise_power);

	int subcarrier_num = context::get_context()->get_rrm_config()->get_rb_num_per_pattern() * 12;
	double send_power = pow(10, (23 - 10 * log10(subcarrier_num * 15 * 1000)) / 10);
	int vue_id = get_superior_level()->get_physics_level()->get_vue_id();

	//����ÿ��Pattern�ϵ��ۼƹ���
	for (int pattern_idx = 0; pattern_idx < pattern_num; pattern_idx++) {
		for (sender_event* __sender_event : s_sender_event_per_pattern[pattern_idx]) {
			int inter_vue_id = __sender_event->get_sender_vue_id();
			pattern_cumulative_power[pattern_idx] += noise_power + vue_physics::get_pl(vue_id, inter_vue_id)*send_power;
		}
	}

	//������ת��Ϊ����
	double total = 0;
	for (int pattern_idx = 0; pattern_idx < pattern_num; pattern_idx++) {
		pattern_cumulative_power[pattern_idx] = 1 / pattern_cumulative_power[pattern_idx];
		total += pattern_cumulative_power[pattern_idx];
		if (pattern_idx > 0) {//תΪ���ʵ������ۻ�ֵ
			pattern_cumulative_power[pattern_idx] += pattern_cumulative_power[pattern_idx - 1];
		}
	}

	//��һ��
	for (int pattern_idx = 0; pattern_idx < pattern_num; pattern_idx++) {
		pattern_cumulative_power[pattern_idx] /= total;
	}
	
	uniform_real_distribution<double> u(0, 1);
	double p = u(s_engine);
	for (int pattern_idx = 0; pattern_idx < pattern_num; pattern_idx++) {
		if (p < pattern_cumulative_power[pattern_idx]) return pattern_idx;
	}

	throw new logic_error("select_error");
}