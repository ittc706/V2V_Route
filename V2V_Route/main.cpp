#include<iostream>
#include"vue.h"
#include"system_control.h"
#include"reflect\context.h"
#include"gtt_highspeed.h"
#include"gtt_urban.h"
#include"route_tcp.h"
using namespace std;

int main() {
	context::create_context("beans.xml");
	context *__context = context::get_context();

	object* ooo = __context->get_bean("gtt");
	cout << ooo->get_class_id() << endl;

	system_control *__system = (system_control *)__context->get_bean("system_control");
	
	cout << "tcp:" << route_tcp::class_id << endl;
	cout << "gtt:" << gtt_urban::class_id << endl;
	//__system->process();
	system("pause");
}