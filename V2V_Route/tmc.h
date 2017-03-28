#pragma once


class tmc {
	/*------------------��Ԫ����------------------*/
	/*
	* ��context��Ϊ��Ԫ������ҪΪ��ע��������
	*/
	friend class context;
	/*------------------��̬��Ա------------------*/


	/*----------------�������Ƴ�Ա----------------*/
public:
	/*
	* Ĭ�Ϲ��캯��
	*/
	tmc();

	/*
	* ��������������������Դ
	*/
	~tmc();

	/*
	* ���������캯������Ϊɾ��
	*/
	tmc(const tmc& t_tmc) = delete;

	/*
	* ���ƶ����캯������Ϊɾ��
	*/
	tmc(tmc&& t_tmc) = delete;

	/*
	* ��������ֵ���������Ϊɾ��
	*/
	tmc& operator=(const tmc& t_tmc) = delete;

	/*
	* ���ƶ���ֵ���������Ϊɾ��
	*/
	tmc& operator=(tmc&& t_tmc) = delete;

	/*--------------------�ֶ�--------------------*/
	/*
	* �������ò�������
	*/
private:
	tmc_config* m_config;
	void set_config(tmc_config* t_config);
public:
	tmc_config* get_config();

	/*--------------------����--------------------*/
public:
	/*
	* ��ʼ��������Ϊÿһ��������ʵ�����ò���
	*/
	void initialize();

	/*
	* �¼��������������ÿ������ͬ����������ά������context�е��¼���Ա
	*/
	void event_trigger();

	/*
	* ͳ�Ƹ�����Ϣ
	*/
	void statistic();
};