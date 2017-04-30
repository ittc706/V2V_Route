#pragma once
#include<vector>
#include<string>
#include<stdexcept>
#include"reflect/object.h"
#include"enumeration.h"

#define INVALID -1

class global_control_config :public object{
	REGISTE_MEMBER_HEAD(global_control_config)
	/*
	* ƽ̨
	*/
private:
	platform m_platform;
public:
	void set_platform(std::string t_platform) {
		if (t_platform == "Windows") {
			m_platform = Windows;
		}
		else if(t_platform == "Linux") {
			m_platform = Linux;
		}
		else {
			throw std::logic_error("error");
		}
	}
	platform get_platform() {
		return m_platform;
	}

	/*
	* ����ʱ��
	*/
private:
	int m_ntti;
public:
	void set_ntti(std::string t_ntti) {
		m_ntti = stoi(t_ntti);
	}
	int get_ntti() {
		return m_ntti;
	}

	/*
	* gttģʽѡ��
	*/
private:
	gtt_mode m_gtt_mode;
public:
	void set_gtt_mode(std::string t_gtt_mode) {
		if (t_gtt_mode == "URBAN") {
			m_gtt_mode = URBAN;
		}
		else if (t_gtt_mode == "HIGHSPEED") {
			m_gtt_mode = HIGHSPEED;
		}
		else {
			throw std::logic_error("error");
		}
	}
	gtt_mode get_gtt_mode() {
		return m_gtt_mode;
	}

	/*
	* routeģʽѡ��
	*/
private:
	route_mode m_route_mode;
public:
	void set_route_mode(std::string t_route_mode) {
		if (t_route_mode == "TCP") {
			m_route_mode = TCP;
		}
		else if(t_route_mode == "UDP"){
			m_route_mode = UDP;
		}
		else {
			throw std::logic_error("error");
		}
	}
	route_mode get_route_mode() {
		return m_route_mode;
	}

};


class gtt_highspeed_config :public object {
	REGISTE_MEMBER_HEAD(gtt_highspeed_config)
	/*
	* ��·����
	*/
private:
	const int m_road_num = 6;
public:
	int get_road_num() {
		return m_road_num;
	}

	/*
	* ·��,��λm
	*/
private:
	double m_road_length = INVALID;
public:
	void set_road_length(std::string t_road_length) {
		m_road_length = stod(t_road_length);
	}
	double get_road_length() {
		return m_road_length;
	}

	/*
	* ·����λm
	*/
private:
	double m_road_width = INVALID;
public:
	void set_road_width(std::string t_road_width) {
		m_road_width = stod(t_road_width);
	}
	double get_road_width() {
		return m_road_width;
	}

	/*
	* ����,km/h
	*/
private:
	double m_speed = INVALID;
public:
	void set_speed(std::string t_speed) {
		m_speed = stod(t_speed);
	}
	double get_speed() {
		return m_speed;
	}

	/*
	* ��·����λ��
	*/
private:
	const double m_road_topo_ratio[6 * 2]{
		0.0f, -2.5f,
		0.0f, -1.5f,
		0.0f, -0.5f,
		0.0f, 0.5f,
		0.0f, 1.5f,
		0.0f, 2.5f,
	};
public:
	const double* get_road_topo_ratio() {
		return m_road_topo_ratio;
	}

	/*
	* ����λ��ˢ��ʱ��,��λs
	*/
private:
	int m_freshtime = INVALID;
public:
	void set_freshtime(std::string t_freshtime) {
		m_freshtime = stoi(t_freshtime);
	}
	int get_freshtime() {
		return m_freshtime;
	}
};


class gtt_urban_config :public object {
	REGISTE_MEMBER_HEAD(gtt_urban_config)

	/*
	* ��������
	*/
private:
	const int m_road_num = 14;
public:
	int get_road_num() {
		return m_road_num;
	}

	/*
	* ·������Ϊ��������ϱ���,��λm
	*/
private:
	double m_road_length_ew = INVALID;
	double m_road_length_sn = INVALID;
public:
	void set_road_length_ew(std::string t_road_length_ew) {
		m_road_length_ew = stod(t_road_length_ew);
	}
	void set_road_length_sn(std::string t_road_length_sn) {
		m_road_length_sn = stod(t_road_length_sn);
	}
	double get_road_length_ew() {
		return m_road_length_ew;
	}
	double get_road_length_sn() {
		return m_road_length_sn;
	}

	/*
	* ·����λm
	*/
private:
	double m_road_width = INVALID;
public:
	void set_road_width(std::string t_road_width) {
		m_road_width = stod(t_road_width);
	}
	double get_road_width() {
		return m_road_width;
	}

	/*
	* ����,km/h
	*/
private:
	double m_speed = INVALID;
public:
	void set_speed(std::string t_speed) {
		m_speed = stod(t_speed);
	}
	double get_speed() {
		return m_speed;
	}

	/*
	* ��·����λ��
	*/
private:
	const double m_road_topo_ratio[14 * 2] = {
		-1.5f, 1.0f,
		-0.5f, 1.0f,
		0.5f, 1.0f,
		1.5f, 1.0f,
		-2.5f, 0.0f,
		-1.5f, 0.0f,
		-0.5f, 0.0f,
		0.5f, 0.0f,
		1.5f, 0.0f,
		2.5f, 0.0f,
		-1.5f,-1.0f,
		-0.5f,-1.0f,
		0.5f,-1.0f,
		1.5f,-1.0f
	};

private:
	const int m_wrap_around_road[14][9] = {
		{ 0,1,6,5,4,13,8,9,10 },
		{ 1,2,7,6,5,0,9,10,11 },
		{ 2,3,8,7,6,1,10,11,12 },
		{ 3,4,9,8,7,2,11,12,13 },
		{ 4,5,10,9,8,3,12,13,0 },
		{ 5,6,11,10,9,4,13,0,1 },
		{ 6,7,12,11,10,5,0,1,2 },
		{ 7,8,13,12,11,6,1,2,3 },
		{ 8,9,0,13,12,7,2,3,4 },
		{ 9,10,1,0,13,8,3,4,5 },
		{ 10,11,2,1,0,9,4,5,6 },
		{ 11,12,3,2,1,10,5,6,7 },
		{ 12,13,4,3,2,11,6,7,8 },
		{ 13,0,5,4,3,12,7,8,9 }
	};

public:
	const double* get_road_topo_ratio() {
		return m_road_topo_ratio;
	}
	const int(*get_wrap_around_road())[9]{
		return m_wrap_around_road;
	}
	/*
	* ����λ��ˢ��ʱ��,��λs
	*/
private:
	int m_freshtime = INVALID;
public:
	void set_freshtime(std::string t_freshtime) {
		m_freshtime = stoi(t_freshtime);
	}
	int get_freshtime() {
		return m_freshtime;
	}

};


class rrm_config :public object {
	REGISTE_MEMBER_HEAD(rrm_config)

	/*------------------��̬��Ա------------------*/
public:
	/*
	* ÿ��RB�Ĵ���(Hz)
	*/
	static const int s_BANDWIDTH_OF_RB = 12 * 1000 * 15;

	/*
	* ��λ(��),����RB����Ϊ180kHz��TTIΪ1ms����˵�λTTI��λRB����ı�����Ϊ180k*1ms=180
	*/
	static const int s_BIT_NUM_PER_RB = 180;

	/*--------------------�ֶ�--------------------*/

	/*
	* �ܴ���
	*/
private:
	int m_total_bandwidth;
public:
	void set_total_bandwidth(std::string t_total_bandwidth) {
		m_total_bandwidth = stoi(t_total_bandwidth);
	}
	int get_total_bandwidth() {
		return m_total_bandwidth;
	}

	/*
	* һ��������Դ���rb����
	*/
private:
	int m_rb_num_per_pattern;
public:
	void set_rb_num_per_pattern(std::string t_rb_num_per_pattern) {
		m_rb_num_per_pattern = stoi(t_rb_num_per_pattern);
	}
	int get_rb_num_per_pattern() {
		return m_rb_num_per_pattern;
	}

	/*
	* pattern����
	*/
private:
	int m_pattern_num;
public:
	void set_pattern_num() {
		m_pattern_num = get_total_bandwidth() / s_BANDWIDTH_OF_RB / get_rb_num_per_pattern();
	}
	int get_pattern_num() {
		return m_pattern_num;
	}

	/*
	* �����ٽ�sinr
	*/
private:
	double m_drop_sinr_boundary;
public:
	void set_drop_sinr_boundary(std::string t_drop_sinr_boundary) {
		m_drop_sinr_boundary = stod(t_drop_sinr_boundary);
	}
	double get_drop_sinr_boundary() {
		return m_drop_sinr_boundary;
	}

	/*
	* ѡ���㷨
	*/
private:
	int m_select_altorithm;
public:
	void set_select_altorithm(std::string t_select_altorithm) {
		m_select_altorithm = stoi(t_select_altorithm);
	}
	int get_select_altorithm() {
		return m_select_altorithm;
	}

	/*
	* ʱ�����ȴ�С
	*/
private:
	int m_time_division_granularity;
public:
	void set_time_division_granularity(std::string t_time_division_granularity) {
		m_time_division_granularity = stoi(t_time_division_granularity);
	}
	int get_time_division_granularity() {
		return m_time_division_granularity;
	}
};


class tmc_config :public object {
	REGISTE_MEMBER_HEAD(tmc_config)

	/*
	* �¼�����
	*/
private:
	int m_package_num;
public:
	void set_package_num(std::string t_package_num) {
		m_package_num = stoi(t_package_num);
	}
	int get_package_num() {
		return m_package_num;
	}

	/*
	* Hello����
	*/
private:
	int m_hello_tti;
public:
	void set_hello_tti(std::string t_m_hello_tti) {
		m_hello_tti = stoi(t_m_hello_tti);
	}
	int get_hello_tti() {
		return m_hello_tti;
	}

	/*
	*  �¼���������
	*/
private:
	double m_trigger_rate;
public:
	void set_trigger_rate(std::string t_trigger_rate) {
		m_trigger_rate = stod(t_trigger_rate);
	}
	double get_trigger_rate() {
		return m_trigger_rate;
	}
};


class route_config :public object {
	REGISTE_MEMBER_HEAD(route_config)

	/*
	* Hello������ƽ������
	*/
private:
	int m_interval;
public:
	void set_interval(std::string t_interval) {
		m_interval = stoi(t_interval);
	}
	int get_t_interval() {
		return m_interval;
	}
};