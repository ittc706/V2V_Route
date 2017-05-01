#pragma once
#include"reflect/object.h"
#include"gtt.h"

class gtt_highspeed_config;

class gtt_highspeed :public gtt {
	REGISTE_MEMBER_HEAD(gtt_highspeed)
private:
	gtt_highspeed_config* m_config;
	vue* m_vue_array;
	/*--------------------½Ó¿Ú--------------------*/
public:
	void set_config(object* t_config) override;

	gtt_highspeed_config* get_config() {
		return m_config;
	}

	vue* get_vue_array() override{
		return m_vue_array;
	}

	void initialize() override;

	int get_vue_num() override;

	void fresh_location() override;

	void calculate_pl(int t_vue_id1, int t_vue_id2) override;
};