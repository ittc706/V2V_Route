#pragma once
#include<string>
#include"enumeration.h"
#include"reflect/object.h"

class gtt_config;
class vue;

class gtt:public object {

	/*--------------------��̬--------------------*/
public:
	/*
	* ����gttģʽ������gtt�������
	*/
	static gtt* gtt_bind_by_mode(gtt_mode t_mode);

	/*--------------------�ӿ�--------------------*/
public:
	/*
	* ���������ļ�,��ͬ��ʵ����Ҫ��ͬ�������ļ��������Ϊ�麯��
	*/
	virtual void set_config(object* t_config) = 0;

	/*
	* ��ȡ��������ָ��
	*/
	virtual vue* get_vue_array() = 0;

	/*
	* ��һЩ��ʼ������
	*/
	virtual void initialize() = 0;

	/*
	* ��ȡ��������
	*/
	virtual int get_vue_num() = 0;

	/*
	* ���ڸ��³���λ��
	*/
	virtual void fresh_location() = 0;

	/*
	* ���ڼ���ָ���ŵ���Ӧ����
	*/
	virtual void calculate_pl(int t_vue_id1, int t_vue_id2) = 0;
};