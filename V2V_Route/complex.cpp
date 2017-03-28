/*
* =====================================================================================
*
*       Filename:  complex.cpp
*
*    Description:  复数类实现
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  HCF
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<cmath>
#include<sstream>
#include<iomanip>
#include<iostream>
#include<stdexcept>
#include"complex.h"

using namespace std;

double complex::abs(const complex&t_c) {
	double res = 0;
	res += sqrt(t_c.m_real*t_c.m_real + t_c.m_imag*t_c.m_imag);
	return res;
}


complex::complex() :
	m_real(0), m_imag(0) {}


complex::complex(double t_real) :
	m_real(t_real), m_imag(0) {}


complex::complex(double t_real, double t_imag) :
	m_real(t_real), m_imag(t_imag) {}


complex::complex(const complex& t_c) :
	m_real(t_c.m_real), m_imag(t_c.m_imag) {}


complex::complex(const initializer_list<double> t_il) {
	if (t_il.size() > 2) throw logic_error("Complex初始化列表元素数目错误：应该为两个double类型的元素");
	initializer_list<double>::iterator it = t_il.begin();
	switch (t_il.size()) {
	case 0:
		m_real = 0;
		m_imag = 0;
		break;
	case 1:
		m_real = *it;
		m_imag = 0;
		break;
	case 2:
		m_real = *it++;
		m_imag = *it;
	}
}


complex& complex::operator =(const complex& t_c) {
	this->m_real = t_c.m_real;
	this->m_imag = t_c.m_imag;
	return *this;
}
complex& complex::operator +=(const complex& t_c) {
	this->m_real += t_c.m_real;
	this->m_imag += t_c.m_imag;
	return *this;
}
complex& complex::operator -=(const complex& t_c) {
	this->m_real -= t_c.m_real;
	this->m_imag -= t_c.m_imag;
	return *this;
}


complex complex::conjugate() {
	complex res;
	res.m_real = m_real;
	res.m_imag = -m_imag;
	return res;
}


string complex::to_string() {
	ostringstream ss;
	ss.precision(2);
	//ss << fixed;//避免用科学计数法，因为可能会存在极小的数值
	ss << "[" << left << setw(2) << m_real <<"+ j"<< left << setw(2) << m_imag << "]";
	return ss.str();
}

void complex::print(ostream&t_out) {
	t_out << to_string() << endl;
}


complex operator-(const complex& t_c) {
	complex res;
	res.m_real = -t_c.m_real;
	res.m_imag = -t_c.m_imag;
	return res;
}

complex operator+(const complex& t_c1, const complex& t_c2) {
	return complex(t_c1.m_real + t_c2.m_real, t_c1.m_imag + t_c2.m_imag);
}


complex operator-(const complex& t_c1, const complex& t_c2) {
	return complex(t_c1.m_real - t_c2.m_real, t_c1.m_imag - t_c2.m_imag);
}


complex operator*(const complex& t_c1, const complex& t_c2) {
	double realPart = 0;
	double imagPart = 0;
	realPart = t_c1.m_real*t_c2.m_real - t_c1.m_imag*t_c2.m_imag;
	imagPart = t_c1.m_real*t_c2.m_imag + t_c2.m_real*t_c1.m_imag;
	return complex(realPart, imagPart);
}


complex operator/(const complex& t_c1, const complex& t_c2) {
	double tmp = complex::abs(t_c2);
	double denominator = tmp*tmp;//分母
	if (denominator == 0) throw logic_error("Complex.cpp->Complex operator/(const Complex& c1, const Complex& c2)");

	double realPart = 0;
	double imagPart = 0;
	realPart = (t_c1.m_real*t_c2.m_real + t_c1.m_imag*t_c2.m_imag) / denominator;
	imagPart = (-t_c1.m_real*t_c2.m_imag + t_c2.m_real*t_c1.m_imag) / denominator;
	return complex(realPart, imagPart);
}


bool operator==(const complex& t_c1, const complex& t_c2) {
	return t_c1.m_real == t_c2.m_real&&t_c1.m_imag == t_c2.m_imag;
}


bool operator!=(const complex& t_c1, const complex& t_c2) {
	return !(t_c1 == t_c2);
}