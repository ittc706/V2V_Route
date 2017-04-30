#pragma once
#include"reflect/object.h"
#include"gtt.h"

class gtt_urban_config;

class gtt_urban :public gtt {
	REGISTE_MEMBER_HEAD(gtt_urban)
private:
	gtt_urban_config* m_config;
	/*--------------------½Ó¿Ú--------------------*/
public:
	void set_config(object* t_config) override;

	gtt_urban_config* get_config() {
		return m_config;
	}

	void initialize() override;

	int get_vue_num() override;

	void fresh_location() override;

	void calculate_pl(int t_vue_id1, int t_vue_id2) override;
}; 
