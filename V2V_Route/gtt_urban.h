#pragma once
#include"reflect\object.h"
#include"gtt.h"

class gtt_urban_config;

class gtt_urban :public object, public gtt {
	REGISTE_MEMBER_HEAD(gtt_urban)
	/*--------------------�ӿ�--------------------*/
public:
	void initialize() override;

	int get_vue_num() override;

	void fresh_location() override;

	void calculate_pl(int t_vue_id1, int t_vue_id2) override;

	gtt_urban_config* get_precise_config();
}; 
