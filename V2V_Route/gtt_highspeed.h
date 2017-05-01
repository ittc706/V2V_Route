#pragma once
#include"reflect/object.h"
#include"gtt.h"

class gtt_highspeed_config;
class v2x_time;

class gtt_highspeed :public gtt {
	REGISTE_MEMBER_HEAD(gtt_highspeed)
private:
	gtt_highspeed_config* m_config;
	vue* m_vue_array;
	v2x_time* m_time;
	void set_config(object* t_config) {
		m_config = (gtt_highspeed_config*)t_config;
	}
	void set_time(object* t_time) {
		m_time = (v2x_time*)t_time;
	}
public:
	gtt_highspeed_config* get_config() {
		return m_config;
	}

	/*--------------------½Ó¿Ú--------------------*/
public:
	v2x_time* get_time()override {
		return m_time;
	}

	vue* get_vue_array() override{
		return m_vue_array;
	}

	void initialize() override;

	int get_vue_num() override;

	void fresh_location() override;

	void calculate_pl(int t_vue_id1, int t_vue_id2) override;
};