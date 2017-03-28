#pragma once

#include<vector>
#include<list>
#include<utility>

class sender_event;
class receiver_event;
class vue;

class vue_link {
	/*------------------��Ԫ����------------------*/
	/*
	* ��vue��Ϊ��Ԫ������vue_link�Ĺ��캯����Ϊ˽�У�������vue������
	*/
	friend class vue;

	/*
	* ��tmc��Ϊ��Ԫ���¼���ص��ֶ���Ҫͨ��tmc����������
	*/
	friend class tmc;

	/*
	* ��receive_event��Ϊ��Ԫ
	*/
	friend class receiver_event;
	/*----------------�������Ƴ�Ա----------------*/
private:
	/*
	* Ĭ�Ϲ��캯��
	* ��Ϊ˽�У����ʼ����ȫ����vue����ɣ���ֹ���ɸ����͵�ʵ��
	*/
	vue_link();

public:
	/*
	* ��������������������Դ
	*/
	~vue_link();

	/*
	* ���������캯������Ϊɾ��
	*/
	vue_link(const vue_link& t_vue_link) = delete;

	/*
	* ���ƶ����캯������Ϊɾ��
	*/
	vue_link(vue_link&& t_vue_link) = delete;

	/*
	* ��������ֵ���������Ϊɾ��
	*/
	vue_link& operator=(const vue_link& t_vue_link) = delete;

	/*
	* ���ƶ���ֵ���������Ϊɾ��
	*/
	vue_link& operator=(vue_link&& t_vue_link) = delete;

	/*--------------------�ֶ�--------------------*/
	/*
	* ָ���ϲ��ָ��
	*/
private:
	vue* m_superior_level;
	void set_superior_level(vue* t_superior_level);
public:
	vue* get_superior_level();

	/*
	* �ɹ������¼�����
	*/
private:
	std::list<receiver_event*> m_success_event_list;
public:
	const std::list<receiver_event*>& get_success_event_list();

	/*
	* ����ʧ���¼�����(�����˶���)
	* һ����Ϣ��N����������һ���������㶪��
	*/
private:
	std::list<receiver_event*> m_loss_event_list;
public:
	const std::list<receiver_event*>& get_loss_event_list();
	
	/*--------------------�ӿ�--------------------*/
public:
	/*
	* ��������
	* ά�������¼��б�
	*/
	void receive_connection(sender_event* t_sender_event);

	/*--------------------ʵ��--------------------*/
private:
};