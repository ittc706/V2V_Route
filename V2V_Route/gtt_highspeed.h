#pragma once
#include"reflect\object.h"
#include"gtt.h"

class gtt_highspeed_config;

class gtt_highspeed:public object,public gtt{
	REGISTE_MEMBER_HEAD(gtt_highspeed)
	/*--------------------½Ó¿Ú--------------------*/
public:
	void initialize() override;

	int get_vue_num() override;

	void fresh_location() override;

	void calculate_pl(int t_vue_id1, int t_vue_id2) override;

	gtt_highspeed_config* get_precise_config();
};