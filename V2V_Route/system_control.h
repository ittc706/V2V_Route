#pragma once

class context;

class system_control {
	/*------------------��Ԫ����------------------*/
	/*
	* ����������Ϊ��Ԫ�����ṩ����ע���Ȩ��
	*/
	friend class context;

	/*------------------˽���ֶ�------------------*/
private:
	/*
	* ����������ָ��
	*/
	context* m_context = nullptr;
	void set_context(context* t_context);
public:
	context* get_context();


	/*----------------�������Ƴ�Ա----------------*/
public:
	/*
	* Ĭ�Ϲ��캯��
	*/
	system_control();

	/*
	* ��������������������Դ
	*/
	~system_control();

	/*
	* ���������캯������Ϊɾ��
	*/
	system_control(const system_control& t_system_control) = delete;

	/*
	* ���ƶ����캯������Ϊɾ��
	*/
	system_control(system_control&& t_system_control) = delete;

	/*
	* ��������ֵ���������Ϊɾ��
	*/
	system_control& operator=(const system_control& t_system_control) = delete;

	/*
	* ���ƶ���ֵ���������Ϊɾ��
	*/
	system_control& operator=(system_control&& t_system_control) = delete;


	/*--------------------�ӿ�--------------------*/
public:
	/*
	* ϵͳ����Ψһ�ӿ�
	*/
	void process();

	/*--------------------ʵ��--------------------*/
private:
	/*
	* ƽ̨��ʼ��
	*/
	void initialize();
};