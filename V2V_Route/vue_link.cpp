/*
* =====================================================================================
*
*       Filename:  vue_link.cpp
*
*    Description:  车辆类的链路层部分实现
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
#include"wt.h"
#include"vue.h"
#include"vue_physics.h"
#include"vue_link.h"
#include"vue_network.h"
#include"event.h"

using namespace std;

vue_link::vue_link() {

}

vue_link::~vue_link() {

}

void vue_link::set_superior_level(vue* t_superior_level) {
	m_superior_level = t_superior_level;
}

vue* vue_link::get_superior_level() {
	return m_superior_level;
}

const std::list<receiver_event*>& vue_link::get_success_event_list() {
	return m_success_event_list;
}

const std::list<receiver_event*>& vue_link::get_loss_event_list() {
	return m_loss_event_list;
}

void vue_link::receive_connection(sender_event* t_sender_event) {
	receiver_event* __receiver_event = new receiver_event(t_sender_event, this->get_superior_level());
	t_sender_event->add_receiver_event_vec(__receiver_event);
}
