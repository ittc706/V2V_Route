/*
* =====================================================================================
*
*       Filename:  config.cpp
*
*    Description:  配置文件类
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
#include<fstream>
#include<sstream>
#include"config.h"
#include"context.h"
#include"config_loader.h"
#include"function.h"

using namespace std;

void global_control_config::set_config_loader(config_loader* t_config_loader) {
	m_config_loader = t_config_loader;
}

config_loader* global_control_config::get_config_loader() {
	return m_config_loader;
}

void global_control_config::set_platform(platform t_platform) {
	m_platform = t_platform;
}

platform global_control_config::get_platform() {
	return m_platform;
}

void global_control_config::set_ntti(int t_ntti) {
	m_ntti = t_ntti;
}

int global_control_config::get_ntti() {
	return m_ntti;
}

void global_control_config::set_gtt_mode(gtt_mode t_gtt_mode) {
	m_gtt_mode = t_gtt_mode;
}

gtt_mode global_control_config::get_gtt_mode() {
	return m_gtt_mode;
}

void global_control_config::load() {
	//首先先判断当前的平台，利用路径的表示在两个平台下的差异来判断
	ifstream inPlatformWindows("config\\global_control_config.xml"),
		inPlatformLinux("config/global_control_config.xml");

	if (inPlatformWindows.is_open()) {
		set_platform(Windows);
		cout << "您当前的平台为：Windows" << endl;
	}
	else if (inPlatformLinux.is_open()) {
		set_platform(Linux);
		cout << "您当前的平台为：Linux" << endl;
	}
	else
		throw logic_error("PlatformError");


	//开始解析配置文件
	switch (get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\global_control_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/global_control_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("ntti")) != nullString) {
		set_ntti(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("gtt_mode")) != nullString) {
		if (temp == "HIGHSPEED")
			set_gtt_mode(HIGHSPEED);
		else if (temp == "URBAN")
			set_gtt_mode(URBAN);
		else
			throw logic_error("ConfigLoaderError");
	}
	else
		throw logic_error("ConfigLoaderError");

	cout << "ntti: " << get_ntti() << endl;
	cout << "gtt_mode: " << (get_gtt_mode() == URBAN ? "URBAN" : "HIGHSPEED") << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

gtt_config* gtt_config::gtt_config_bind_by_mode(gtt_mode t_mode) {
	if (t_mode == HIGHSPEED) {
		return new gtt_highspeed_config();
	}
	else {
		return new gtt_urban_config();
	}
}

void gtt_config::set_config_loader(config_loader* t_config_loader) {
	m_config_loader = t_config_loader;
}

config_loader* gtt_config::get_config_loader() {
	return m_config_loader;
}

int gtt_highspeed_config::get_road_num() {
	return m_road_num;
}

void gtt_highspeed_config::set_road_length(double t_road_length) {
	m_road_length = t_road_length;
}

double gtt_highspeed_config::get_road_length() {
	return m_road_length;
}

void gtt_highspeed_config::set_road_width(double t_road_width) {
	m_road_width = t_road_width;
}

double gtt_highspeed_config::get_road_width() {
	return m_road_width;
}

void gtt_highspeed_config::set_speed(double t_speed) {
	m_speed = t_speed;
}

double gtt_highspeed_config::get_speed() {
	return m_speed;
}

const double* gtt_highspeed_config::get_road_topo_ratio() {
	return m_road_topo_ratio;
}

void gtt_highspeed_config::set_freshtime(int t_freshtime) {
	m_freshtime = t_freshtime;
}

int gtt_highspeed_config::get_freshtime() {
	return m_freshtime;
}

void gtt_highspeed_config::load() {

	//开始解析系统配置文件
	switch (context::get_context()->get_global_control_config()->get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\gtt_highspeed_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/gtt_highspeed_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("road_length")) != nullString) {
		set_road_length(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");


	if ((temp = get_config_loader()->get_param("road_width")) != nullString) {
		set_road_width(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("speed")) != nullString) {
		set_speed(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("freshtime")) != nullString) {
		set_freshtime(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	cout << "road_length: " << get_road_length() << endl;
	cout << "road_width: " << get_road_width() << endl;
	cout << "speed: " << get_speed() << endl;
	cout << "freshtime: " << get_freshtime() << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

void gtt_urban_config::load() {
	//开始解析系统配置文件
	switch (context::get_context()->get_global_control_config()->get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\gtt_urban_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/gtt_urban_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("road_length_ew")) != nullString) {
		set_road_length_ew(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("road_length_sn")) != nullString) {
		set_road_length_sn(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");


	if ((temp = get_config_loader()->get_param("road_width")) != nullString) {
		set_road_width(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("speed")) != nullString) {
		set_speed(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("freshtime")) != nullString) {
		set_freshtime(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	cout << "road_length_ew: " << get_road_length_ew() << endl;
	cout << "road_length_sn: " << get_road_length_sn() << endl;
	cout << "road_width: " << get_road_width() << endl;
	cout << "speed: " << get_speed() << endl;
	cout << "freshtime: " << get_freshtime() << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

int gtt_urban_config::get_road_num() {
	return m_road_num;
}

void gtt_urban_config::set_road_length_ew(double t_road_length_ew) {
	m_road_length_ew = t_road_length_ew;
}

double gtt_urban_config::get_road_length_ew() {
	return m_road_length_ew;
}

void gtt_urban_config::set_road_length_sn(double t_road_length_sn) {
	m_road_length_sn = t_road_length_sn;
}

double gtt_urban_config::get_road_length_sn() {
	return m_road_length_sn;
}
void gtt_urban_config::set_road_width(double t_road_width) {
	m_road_width = t_road_width;
}

double gtt_urban_config::get_road_width() {
	return m_road_width;
}

void gtt_urban_config::set_speed(double t_speed) {
	m_speed = t_speed;
}

double gtt_urban_config::get_speed() {
	return m_speed;
}

const double* gtt_urban_config::get_road_topo_ratio() {
	return m_road_topo_ratio;
}

const int(*gtt_urban_config::get_wrap_around_road())[9] {
	return m_wrap_around_road;
}

void gtt_urban_config::set_freshtime(int t_freshtime) {
	m_freshtime = t_freshtime;
}

int gtt_urban_config::get_freshtime() {
	return m_freshtime;
}


void rrm_config::set_config_loader(config_loader* t_config_loader) {
	m_config_loader = t_config_loader;
}

config_loader* rrm_config::get_config_loader() {
	return m_config_loader;
}

void rrm_config::set_total_bandwidth(int t_total_bandwidth) {
	m_total_bandwidth = t_total_bandwidth;
}

int rrm_config::get_total_bandwidth() {
	return m_total_bandwidth;
}

void rrm_config::set_rb_num_per_pattern(int t_rb_num_per_pattern) {
	m_rb_num_per_pattern = t_rb_num_per_pattern;
}

int rrm_config::get_rb_num_per_pattern() {
	return m_rb_num_per_pattern;
}

void rrm_config::set_pattern_num() {
	m_pattern_num = get_total_bandwidth() / s_BANDWIDTH_OF_RB / get_rb_num_per_pattern();
}

int rrm_config::get_pattern_num() {
	return m_pattern_num;
}

void rrm_config::set_drop_sinr_boundary(double t_drop_sinr_boundary) {
	m_drop_sinr_boundary = t_drop_sinr_boundary;
}

double rrm_config::get_drop_sinr_boundary() {
	return m_drop_sinr_boundary;
}

void rrm_config::set_select_altorithm(int t_select_altorithm) {
	m_select_altorithm = t_select_altorithm;
}

int rrm_config::get_select_altorithm() {
	return m_select_altorithm;
}

void rrm_config::set_time_division_granularity(int t_time_division_granularity) {
	m_time_division_granularity = t_time_division_granularity;
}

int rrm_config::get_time_division_granularity() {
	return m_time_division_granularity;
}

void rrm_config::load() {
	//开始解析系统配置文件
	switch (context::get_context()->get_global_control_config()->get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\rrm_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/rrm_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("total_bandwidth")) != nullString) {
		int t_total_bandwidth = stoi(temp);
		t_total_bandwidth *= 1000 * 1000;
		set_total_bandwidth(t_total_bandwidth);
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("rb_num_per_pattern")) != nullString) {
		set_rb_num_per_pattern(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("drop_sinr_boundary")) != nullString) {
		set_drop_sinr_boundary(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("select_altorithm")) != nullString) {
		set_select_altorithm(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("time_division_granularity")) != nullString) {
		set_time_division_granularity(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	//通过获取的配置信息，计算pattern数量
	set_pattern_num();

	cout << "total_bandwidth: " << get_total_bandwidth() << endl;
	cout << "rb_num_per_pattern: " << get_rb_num_per_pattern() << endl;
	cout << "pattern_num: " << get_pattern_num() << endl;
	cout << "drop_sinr_boundary: " << get_drop_sinr_boundary() << endl;
	cout << "select_altorithm: " << get_select_altorithm() << endl;
	cout << "time_division_granularity: " << get_time_division_granularity() << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

void tmc_config::set_config_loader(config_loader* t_config_loader) {
	m_config_loader = t_config_loader;
}

config_loader* tmc_config::get_config_loader() {
	return m_config_loader;
}

void tmc_config::set_congestion_level_num(int t_congestion_level_num) {
	m_congestion_level_num = t_congestion_level_num;
}

int tmc_config::get_congestion_level_num() {
	return m_congestion_level_num;
}

const std::vector<int>& tmc_config::get_periodic_event_period_per_congestion_level() {
	return m_periodic_event_period_per_congestion_level;
}

void tmc_config::set_package_num(int t_package_num) {
	m_package_num = t_package_num;
}

int tmc_config::get_package_num() {
	return m_package_num;
}

const std::vector<int>& tmc_config::get_tti_per_package() {
	return m_tti_per_package;
}

void tmc_config::load() {
	//开始解析系统配置文件
	switch (context::get_context()->get_global_control_config()->get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\tmc_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/tmc_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("congestion_level_num")) != nullString) {
		set_congestion_level_num(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("periodic_event_period")) != nullString) {
		stringstream ss;
		ss << temp;
		string temp2;
		while (ss >> temp2) {
			m_periodic_event_period_per_congestion_level.push_back(stoi(temp));
		}
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("package_num")) != nullString) {
		set_package_num(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("tti_per_package")) != nullString) {
		stringstream ss;
		ss << temp;
		string temp2;
		while (ss >> temp2) {
			m_tti_per_package.push_back(stoi(temp2));
		}
	}
	else
		throw logic_error("ConfigLoaderError");

	cout << "congestion_level_num: " << get_congestion_level_num() << endl;
	cout << "periodic_event_period_per_congestion_level: "; array_print::print_vector_dim1(get_periodic_event_period_per_congestion_level());
	cout << "package_num: " << get_package_num() << endl;
	cout << "tti_per_package: "; array_print::print_vector_dim1(get_tti_per_package());
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}