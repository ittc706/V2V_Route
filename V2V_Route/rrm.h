#pragma once

class rrm_config;

class rrm {
	/*------------------友元声明------------------*/
	/*
	* 将context设为友元，容器要为其注入依赖项
	*/
	friend class context;

	/*------------------静态成员------------------*/


	/*----------------拷贝控制成员----------------*/
public:
	/*
	* 默认构造函数
	*/
	rrm();

	/*
	* 析构函数，负责清理资源
	*/
	~rrm();

	/*
	* 将拷贝构造函数定义为删除
	*/
	rrm(const rrm& t_rrm) = delete;

	/*
	* 将移动构造函数定义为删除
	*/
	rrm(rrm&& t_rrm) = delete;

	/*
	* 将拷贝赋值运算符定义为删除
	*/
	rrm& operator=(const rrm& t_rrm) = delete;

	/*
	* 将移动赋值运算符定义为删除
	*/
	rrm& operator=(rrm&& t_rrm) = delete;

	/*--------------------字段--------------------*/
	/*
	* 场景配置参数对象
	*/
private:
	rrm_config* m_config;
	void set_config(rrm_config* t_config);
public:
	rrm_config* get_config();

	/*--------------------方法--------------------*/
public:
	/*
	* 初始化工作
	*/
	void initialize();

	/*
	* 运行
	*/
	void schedule();
};