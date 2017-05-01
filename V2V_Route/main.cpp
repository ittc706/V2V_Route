#include<iostream>
#include<fstream>
#include"vue.h"
#include"system_control.h"
#include"reflect/context.h"
#include"gtt_highspeed.h"
#include"gtt_urban.h"
#include"route_tcp.h"
#include"wt.h"
#include"tmc.h"
#include"route_udp.h"
#include"enumeration.h"
using namespace std;

class A{
public:
	static void f() {}
};

int main() {
	/*cout <<"gtt_urban: "<< gtt_urban::class_id << endl;
	cout << "gtt_highspeed: " << gtt_highspeed::class_id << endl;
	cout << "wt: " << wt::class_id << endl;
	cout << "tmc: " << tmc::class_id << endl;
	cout << "global_control_config: " << global_control_config::class_id << endl;
	cout << "gtt_highspeed_config: " << gtt_highspeed_config::class_id << endl;
	cout << "gtt_urban_config: " << gtt_urban_config::class_id << endl;
	cout << "rrm_config: " << rrm_config::class_id << endl;
	cout << "tmc_config: " << tmc_config::class_id << endl;
	cout << "route_config: " << route_config::class_id << endl;
	cout << "system_control: " << system_control::class_id << endl;
	cout << "route_tcp: " << route_tcp::class_id << endl;
	cout << "route_udp: " << route_udp::class_id << endl;*/

	/*object* obj1 = route_tcp::new_instance();
	cout << obj1->get_class_id() << endl;
	object* obj2 = route_udp::new_instance();
	cout << obj2->get_class_id() << endl;
	object* obj3 = rrm_config::new_instance();
	cout << obj3->get_class_id() << endl;
	object* obj4 = gtt_highspeed::new_instance();
	cout << obj4->get_class_id() << endl;*/


	string path1 = "config/beans.xml";
	string path2 = "config\\beans.xml";
	ifstream in1(path1);
	ifstream in2(path2);
	string path;
	if (in1.is_open()) {
		path = path1;
	}
	else if (in2.is_open()) {
		path = path2;
	}
	else {
		cout << "配置文件路径有误，默认为:config->beans.xml" << endl;
		system("pause");
		exit(0);
	}

	context::create_context(path);
	context *__context = context::get_context();

	system_control *__system = (system_control *)__context->get_bean("system_control");

	
	__system->process();
	system("pause");
}