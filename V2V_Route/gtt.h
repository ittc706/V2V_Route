#pragma once
#include<string>
#include"enumeration.h"
#include"reflect/object.h"

class gtt_config;
class vue;

class gtt:public object {

	/*--------------------静态--------------------*/
public:
	/*
	* 根据gtt模式来生成gtt组件对象
	*/
	static gtt* gtt_bind_by_mode(gtt_mode t_mode);

	/*--------------------接口--------------------*/
public:
	/*
	* 设置配置文件,不同的实现需要不同的配置文件，因此设为虚函数
	*/
	virtual void set_config(object* t_config) = 0;

	/*
	* 获取车辆数组指针
	*/
	virtual vue* get_vue_array() = 0;

	/*
	* 做一些初始化工作
	*/
	virtual void initialize() = 0;

	/*
	* 获取车辆数量
	*/
	virtual int get_vue_num() = 0;

	/*
	* 用于更新车辆位置
	*/
	virtual void fresh_location() = 0;

	/*
	* 用于计算指定信道响应矩阵
	*/
	virtual void calculate_pl(int t_vue_id1, int t_vue_id2) = 0;
};