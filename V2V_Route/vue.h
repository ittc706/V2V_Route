#pragma once


/*
* ǰ������
*/
class vue_physics;
class vue_link;
class vue_network;


class vue {
	/*------------------˽���ֶ�------------------*/
private:
	/*
	* ָ������������ʵ��
	*/
	vue_physics* m_physics_level = nullptr;

	/*
	* ָ��������·���ʵ��
	*/
	vue_link* m_link_level = nullptr;

	/*
	* ָ������������ʵ��
	*/
	vue_network* m_network_level = nullptr;

	/*----------------�������Ƴ�Ա----------------*/
public: 
	/*
	* Ĭ�Ϲ��캯������������ʵ��ָ��ĳ�ʼ��
	*/
	vue();

	/*
	* ��������������������Դ
	*/
	~vue();

	/*
	* ���������캯������Ϊɾ��
	*/
	vue(const vue& t_vue) = delete;

	/*
	* ���ƶ����캯������Ϊɾ��
	*/
	vue(vue&& t_vue) = delete;

	/*
	* ��������ֵ���������Ϊɾ��
	*/
	vue& operator=(const vue& t_vue) = delete;

	/*
	* ���ƶ���ֵ���������Ϊɾ��
	*/
	vue& operator=(vue&& t_vue) = delete;

	/*----------------������༭��----------------*/
public:
	/*
	* ��������㲿�ַ�����
	*/
	vue_physics* get_physics_level();

	/*
	* ������·�㲿�ַ�����
	*/
	vue_link* get_link_level();

	/*
	* ��������㲿�ַ�����
	*/
	vue_network* get_network_level();
};