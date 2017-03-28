#pragma once
#include<string>
#include<initializer_list>

class complex {
	/*------------------静态------------------*/
public:
	/*
	* 用于计算复数的模值
	*/
	static double abs(const complex&t_c);

	/*------------------域------------------*/
public:
	/*
	* 实部
	*/
	double m_real;

	/*
	* 虚部
	*/
	double m_imag;

	/*------------------方法------------------*/
	/*
	* 默认构造函数
	* 初始化为(0,0)
	*/
	complex();

	/*
	* 接受一个实参的构造函数
	* 该double类型的作为实部，虚部为0
	* 接受该类型的隐式转换
	*/
	complex(double t_real);

	/*
	* 接受两个实参的构造函数
	* 分别为实部和虚部
	*/
	complex(double t_real, double t_imag);

	/*
	* 拷贝构造函数
	*/
	complex(const complex& t_c);

	/*
	* 列表初始化
	* 列表长度可以是0,1,2,
	*		0的时候，初始化为(0,0)
	*		1的时候，用于初始化实部
	*		2的时候，用于初始化实部和虚部
	*/
	complex(const std::initializer_list<double> t_il);


	/*
	* 拷贝赋值运算符
	*/
	complex& operator=(const complex& t_c);

	/*
	* 复合赋值运算符
	*/
	complex& operator+=(const complex& t_c);

	/*
	* 复合赋值运算符
	*/
	complex& operator-=(const complex& t_c);


	/*
	* 求共轭
	*/
	complex conjugate();

	/*
	* 生成格式化字符串
	*/
	std::string to_string();

	/*
	* 用于打印到指定输出流
	* 默认的输出流为std::out
	*/
	void print(std::ostream& t_out = std::cout);
};

/*
* 取反运算符
*/
complex operator-(const complex& t_c);

/*
* 加法运算符
*/
complex operator+(const complex& t_c1, const complex& t_c2);

/*
* 减法运算符
*/
complex operator-(const complex& t_c1, const complex& t_c2);

/*
* 乘法运算符
*/
complex operator*(const complex& t_c1, const complex& t_c2);

/*
* 除法运算符
*/
complex operator/(const complex& t_c1, const complex& t_c2);

/*
* 相等运算符
*/
bool operator==(const complex& t_c1, const complex& t_c2);

/*
* 不等运算符
*/
bool operator!=(const complex& t_c1, const complex& t_c2);
