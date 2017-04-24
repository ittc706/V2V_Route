#pragma once

#include<vector>
#include"enumeration.h"

#define INVALID -1

class config_loader;

class global_control_config {
	/*------------------��Ԫ����------------------*/
	/*
	* ��context��Ϊ��Ԫ������ҪΪ��ע��������
	*/
	friend class context;

	/*--------------------�ֶ�--------------------*/
	/*
	* �����������
	*/
private:
	config_loader* m_config_loader;
	void set_config_loader(config_loader* t_config_loader) {
		m_config_loader = t_config_loader;
	}
public:
	config_loader* get_config_loader() {
		return m_config_loader;
	}

	/*
	* ƽ̨
	*/
private:
	platform m_platform;
	void set_platform(platform t_platform) {
		m_platform = t_platform;
	}
public:
	platform get_platform() {
		return m_platform;
	}

	/*
	* ����ʱ��
	*/
private:
	int m_ntti;
	void set_ntti(int t_ntti) {
		m_ntti = t_ntti;
	}
public:
	int get_ntti() {
		return m_ntti;
	}

	/*
	* gttģʽѡ��
	*/
private:
	gtt_mode m_gtt_mode;
	void set_gtt_mode(gtt_mode t_gtt_mode) {
		m_gtt_mode = t_gtt_mode;
	}
public:
	gtt_mode get_gtt_mode() {
		return m_gtt_mode;
	}

	/*
	* routeģʽѡ��
	*/
private:
	route_mode m_route_mode;
	void set_route_mode(route_mode t_route_mode) {
		m_route_mode = t_route_mode;
	}
public:
	route_mode get_route_mode() {
		return m_route_mode;
	}

	/*--------------------�ӿ�--------------------*/
public:
	void load();
};


class gtt_config {
	/*------------------��Ԫ����------------------*/
	/*
	* ��context��Ϊ��Ԫ������ҪΪ��ע��������
	*/
	friend class context;

	/*--------------------�ֶ�--------------------*/
	/*
	* ����gttģʽ������gtt_config�����ļ�����
	*/
	static gtt_config* gtt_config_bind_by_mode(gtt_mode t_mode);

	/*--------------------�ֶ�--------------------*/
	/*
	* �����������
	*/
private:
	config_loader* m_config_loader;
	void set_config_loader(config_loader* t_config_loader) {
		m_config_loader = t_config_loader;
	}
public:
	config_loader* get_config_loader() {
		return m_config_loader;
	}

	/*--------------------�ӿ�--------------------*/
public:
	virtual void load() = 0;
};


class gtt_highspeed_config :public gtt_config {
	/*--------------------�ֶ�--------------------*/
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
	void set_road_length(double t_road_length) {
		m_road_length = t_road_length;
	}
public:
	double get_road_length() {
		return m_road_length;
	}

	/*
	* ·����λm
	*/
private:
	double m_road_width = INVALID;
	void set_road_width(double t_road_width) {
		m_road_width = t_road_width;
	}
public:
	double get_road_width() {
		return m_road_width;
	}

	/*
	* ����,km/h
	*/
private:
	double m_speed = INVALID;
	void set_speed(double t_speed) {
		m_speed = t_speed;
	}
public:
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
	void set_freshtime(int t_freshtime) {
		m_freshtime = t_freshtime;
	}
public:
	int get_freshtime() {
		return m_freshtime;
	}

	/*--------------------�ӿ�--------------------*/
public:
	void load() override;

};


class gtt_urban_config :public gtt_config {
	/*--------------------�ӿ�--------------------*/
public:
	void load() override;
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
	void set_road_length_ew(double t_road_length_ew) {
		m_road_length_ew = t_road_length_ew;
	}
	void set_road_length_sn(double t_road_length_sn) {
		m_road_length_sn = t_road_length_sn;
	}
public:
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
	void set_road_width(double t_road_width) {
		m_road_width = t_road_width;
	}
public:
	double get_road_width() {
		return m_road_width;
	}

	/*
	* ����,km/h
	*/
private:
	double m_speed = INVALID;
	void set_speed(double t_speed) {
		m_speed = t_speed;
	}
public:
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
	void set_freshtime(int t_freshtime) {
		m_freshtime = t_freshtime;
	}
public:
	int get_freshtime() {
		return m_freshtime;
	}

};


class rrm_config {
	/*------------------��Ԫ����------------------*/
	/*
	* ��context��Ϊ��Ԫ������ҪΪ��ע��������
	*/
	friend class context;

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
	* �����������
	*/
private:
	config_loader* m_config_loader;
	void set_config_loader(config_loader* t_config_loader) {
		m_config_loader = t_config_loader;
	}
public:
	config_loader* get_config_loader() {
		return m_config_loader;
	}

	/*
	* �ܴ���
	*/
private:
	int m_total_bandwidth;
	void set_total_bandwidth(int t_total_bandwidth) {
		m_total_bandwidth = t_total_bandwidth;
	}
public:
	int get_total_bandwidth() {
		return m_total_bandwidth;
	}

	/*
	* һ��������Դ���rb����
	*/
private:
	int m_rb_num_per_pattern;
	void set_rb_num_per_pattern(int t_rb_num_per_pattern) {
		m_rb_num_per_pattern = t_rb_num_per_pattern;
	}
public:
	int get_rb_num_per_pattern() {
		return m_rb_num_per_pattern;
	}

	/*
	* pattern����
	*/
private:
	int m_pattern_num;
	void set_pattern_num() {
		m_pattern_num = get_total_bandwidth() / s_BANDWIDTH_OF_RB / get_rb_num_per_pattern();
	}
public:
	int get_pattern_num() {
		return m_pattern_num;
	}

	/*
	* �����ٽ�sinr
	*/
private:
	double m_drop_sinr_boundary;
	void set_drop_sinr_boundary(double t_drop_sinr_boundary) {
		m_drop_sinr_boundary = t_drop_sinr_boundary;
	}
public:
	double get_drop_sinr_boundary() {
		return m_drop_sinr_boundary;
	}

	/*
	* ѡ���㷨
	*/
private:
	int m_select_altorithm;
	void set_select_altorithm(int t_select_altorithm) {
		m_select_altorithm = t_select_altorithm;
	}
public:
	int get_select_altorithm() {
		return m_select_altorithm;
	}

	/*
	* ʱ�����ȴ�С
	*/
private:
	int m_time_division_granularity;
	void set_time_division_granularity(int t_time_division_granularity) {
		m_time_division_granularity = t_time_division_granularity;
	}
public:
	int get_time_division_granularity() {
		return m_time_division_granularity;
	}
	/*--------------------�ӿ�--------------------*/
public:
	void load();
};


class tmc_config {
	/*------------------��Ԫ����------------------*/
	/*
	* ��context��Ϊ��Ԫ������ҪΪ��ע��������
	*/
	friend class context;

	/*--------------------�ֶ�--------------------*/
	/*
	* �����������
	*/
private:
	config_loader* m_config_loader;
	void set_config_loader(config_loader* t_config_loader) {
		m_config_loader = t_config_loader;
	}
public:
	config_loader* get_config_loader() {
		return m_config_loader;
	}

	/*
	* �¼�����
	*/
private:
	int m_package_num;
	void set_package_num(int t_package_num) {
		m_package_num = t_package_num;
	}
public:
	int get_package_num() {
		return m_package_num;
	}

	/*
	* Hello����
	*/
private:
	int m_hello_tti;
	void set_hello_tti(int t_m_hello_tti) {
		m_hello_tti = t_m_hello_tti;
	}
public:
	int get_hello_tti() {
		return m_hello_tti;
	}

	/*
	*  �¼���������
	*/
private:
	double m_trigger_rate;
	void set_trigger_rate(double t_trigger_rate) {
		m_trigger_rate = t_trigger_rate;
	}
public:
	double get_trigger_rate() {
		return m_trigger_rate;
	}

	/*--------------------�ӿ�--------------------*/
public:
	void load();
};


class route_config {
	/*------------------��Ԫ����------------------*/
	/*
	* ��context��Ϊ��Ԫ������ҪΪ��ע��������
	*/
	friend class context;

	/*--------------------�ֶ�--------------------*/
	/*
	* �����������
	*/
private:
	config_loader* m_config_loader;
	void set_config_loader(config_loader* t_config_loader) {
		m_config_loader = t_config_loader;
	}
public:
	config_loader* get_config_loader() {
		return m_config_loader;
	}
private:
	/*
	* Hello������ƽ������
	*/

	int m_interval;
	void set_interval(int t_interval) {
		m_interval = t_interval;
	}
public:
	int get_t_interval() {
		return m_interval;
	}

	/*--------------------�ӿ�--------------------*/
public:
	void load();
};