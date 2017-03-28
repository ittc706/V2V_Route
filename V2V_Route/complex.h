#pragma once
#include<string>
#include<initializer_list>

class complex {
	/*------------------��̬------------------*/
public:
	/*
	* ���ڼ��㸴����ģֵ
	*/
	static double abs(const complex&t_c);

	/*------------------��------------------*/
public:
	/*
	* ʵ��
	*/
	double m_real;

	/*
	* �鲿
	*/
	double m_imag;

	/*------------------����------------------*/
	/*
	* Ĭ�Ϲ��캯��
	* ��ʼ��Ϊ(0,0)
	*/
	complex();

	/*
	* ����һ��ʵ�εĹ��캯��
	* ��double���͵���Ϊʵ�����鲿Ϊ0
	* ���ܸ����͵���ʽת��
	*/
	complex(double t_real);

	/*
	* ��������ʵ�εĹ��캯��
	* �ֱ�Ϊʵ�����鲿
	*/
	complex(double t_real, double t_imag);

	/*
	* �������캯��
	*/
	complex(const complex& t_c);

	/*
	* �б��ʼ��
	* �б��ȿ�����0,1,2,
	*		0��ʱ�򣬳�ʼ��Ϊ(0,0)
	*		1��ʱ�����ڳ�ʼ��ʵ��
	*		2��ʱ�����ڳ�ʼ��ʵ�����鲿
	*/
	complex(const std::initializer_list<double> t_il);


	/*
	* ������ֵ�����
	*/
	complex& operator=(const complex& t_c);

	/*
	* ���ϸ�ֵ�����
	*/
	complex& operator+=(const complex& t_c);

	/*
	* ���ϸ�ֵ�����
	*/
	complex& operator-=(const complex& t_c);


	/*
	* ����
	*/
	complex conjugate();

	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string to_string();

	/*
	* ���ڴ�ӡ��ָ�������
	* Ĭ�ϵ������Ϊstd::out
	*/
	void print(std::ostream& t_out = std::cout);
};

/*
* ȡ�������
*/
complex operator-(const complex& t_c);

/*
* �ӷ������
*/
complex operator+(const complex& t_c1, const complex& t_c2);

/*
* ���������
*/
complex operator-(const complex& t_c1, const complex& t_c2);

/*
* �˷������
*/
complex operator*(const complex& t_c1, const complex& t_c2);

/*
* ���������
*/
complex operator/(const complex& t_c1, const complex& t_c2);

/*
* ��������
*/
bool operator==(const complex& t_c1, const complex& t_c2);

/*
* ���������
*/
bool operator!=(const complex& t_c1, const complex& t_c2);
