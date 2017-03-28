#pragma once
#include<vector>
#include<random>
#include<string>
#include<utility>
#include<iostream>
#include"complex.h"

class row_vector {
	/*------------------域------------------*/
private:
	/*
	* 底层容器，设为私有禁止直接访问
	*/
	complex* m_row_vector;
public:
	/*
	* 向量长度，由于是行向量，这里称为列数
	*/
	int m_col;


	/*------------------方法------------------*/
private:
	/*
	* 资源清理，即释放堆内存
	*/
	void free();
public:
	/*
	* 析构函数，用于释放rowVector
	*/
	~row_vector();

	/*
	* 默认构造函数
	* 向量长度为0，指针为空
	*/
	row_vector();

	/*
	* 接受一个向量维度的参数
	* 初始化后，向量维度等于指定的t_Col，元素初始化微为数值(0,0)
	*/
	explicit row_vector(int t_col);

	/*
	* 拷贝构造函数
	* 注意有指针成员时，拷贝语意的实现
	*/
	row_vector(const row_vector& t_row_vector);

	/*
	* 移动构造函数
	* 注意移动后源对象指针成员的置空
	* 声明为不抛出异常的
	*/
	row_vector(row_vector&& t_row_vector) noexcept;

	/*
	* 列表构造函数
	*/
	row_vector(const std::initializer_list<complex> t_il);


	/*
	* 拷贝赋值运算符
	*/
	row_vector& operator=(const row_vector& t_row_vector);

	/*
	* 移动赋值运算符
	*/
	row_vector& operator=(row_vector&& t_row_vector) noexcept;

	/*
	* 下标运算符
	*/
	complex& operator[](int t_pos);

	/*
	* 常量版本的下标运算符
	*/
	const complex& operator[](int t_pos) const;

	/*
	* 重置大小
	* 重置后若维度变大，那么补(0,0)
	* 重置后若维度变小，那么删除多余元素
	*/
	void resize(int t_size);

	/*
	* 生成向量的格式化字符串
	*/
	std::string toString();

	/*
	* 将向量打印到指定输出流
	*/
	void print(std::ostream& t_out = std::cout);

};

/*
* 向量单目取反运算符
*/
row_vector operator-(const row_vector& t_row_vector);

/*
* 向量加法、减法运算
*/
row_vector operator+(const row_vector& t_row_vector1, const row_vector& t_row_vector2);
row_vector operator-(const row_vector& t_row_vector1, const row_vector& t_row_vector2);

/*
* 向量与复数(复数与向量)的加减乘除运算符
*/
row_vector operator+(const row_vector& t_row_vector, const complex&t_complex);
row_vector operator+(const complex&t_complex, const row_vector& t_row_vector);

row_vector operator-(const row_vector& t_row_vector, const complex&t_complex);
row_vector operator-(const complex&t_complex, const row_vector& t_row_vector);

row_vector operator*(const row_vector& t_row_vector, const complex&t_complex);
row_vector operator*(const complex&t_complex, const row_vector& t_row_vector);

row_vector operator/(const row_vector& t_row_vector, const complex&t_complex);
row_vector operator/(const complex&t_complex, const row_vector& t_row_vector);

/*
* 向量与向量的对应元素乘法与除法
*/
row_vector element_product(const row_vector& t_row_vector1, const row_vector& t_row_vector2);
row_vector element_divide(const row_vector& t_row_vector1, const row_vector& t_row_vector2);


class matrix {
	/*------------------静态------------------*/
public:
	/*
	* 随机数引擎，该类共享
	*/
	static std::default_random_engine s_engine;

	/*
	* 矩阵的左右合并，因此行维度必须相同
	*/
	static matrix horizon_merge(const matrix& t_matrix1, const matrix& t_matrix2);

	/*
	* 矩阵的水平分裂
	*/
	static std::pair<matrix, matrix> horizon_split(const matrix& t_matrix, int t_left_col, int t_right_col);

	/*
	* 矩阵的垂直分裂
	*/
	static std::pair<matrix, matrix> vertical_split(const matrix& t_matrix, int t_up_row, int t_down_row);

	/*
	* 生成指定维度的单位矩阵
	*/
	static matrix eye(const int t_dim);
	/*------------------域------------------*/
private:
	/*
	* 底层向量容器，设为私有禁止直接访问
	*/
	row_vector* m_matrix;
public:
	/*
	* 行数
	*/
	int m_row;

	/*
	* 列数
	*/
	int m_col;

	/*------------------方法------------------*/
private:
	/*
	* 清理堆内存
	*/
	void free();
public:
	/*
	* 析构函数
	*/
	~matrix();

	/*
	* 默认构造函数
	* 行列均为0，指针为空
	*/
	matrix();

	/*
	* 指定维度的构造函数
	*/
	matrix(int t_row, int t_col);

	/*
	* 拷贝构造函数，注意有指针时拷贝语意的实现
	*/
	matrix(const matrix& t_matrix);

	/*
	* 移动构造函数，注意有指针时移动语意的实现
	*/
	matrix(matrix&& t_matrix) noexcept;

	/*
	* 列表构造函数
	* 列表中每行的元素数目可以不同
	* 以最长的一行的维度作为列数
	* 不足的补上(0,0)
	*/
	matrix(const std::initializer_list<row_vector>& t_il);

	/*---矩阵功能函数---*/
public:
	/*
	* 随机填充均匀分布的数值
	* 实部的范围[t_RealLeft,t_RealRight]
	* 虚部的范围[t_ImagLeft,t_ImagRight]
	*/
	void random_fill(double t_real_left, double t_real_right, double t_imag_left, double t_imag_right);

	/*
	* 求共轭
	*/
	matrix conjugate();

	/*
	* 求转置
	*/
	matrix transpose();

	/*
	* 共轭转置
	*/
	matrix hermitian();

	/*
	* 求逆矩阵
	* 当布尔值为true时，当矩阵无法求逆，会尝试求伪逆(广义逆矩阵)
	*/
	matrix inverse(bool t_try_pseudo_inverse = false);

	/*
	* 求对角线构成的矩阵(行向量)
	*/
	matrix diag();

	/*
	* 求满秩分解
	*/
	std::pair<matrix, matrix> full_rank_decomposition();

	/*
	* 求广义逆矩阵
	*/
	matrix pseudo_inverse();


	/*---成员运算符重载---*/
public:
	/*
	* 拷贝赋值运算符
	* 注意，有指针成员时，也需要检查自赋值的正确性
	* 注意原指针析构的时机
	*/
	matrix& operator=(const matrix& t_matrix);

	/*
	* 移动赋值运算符
	* 需要检查自赋值的正确性
	* 注意原指针析构的时机
	* 声明为不会抛出异常
	*/
	matrix& operator=(matrix&& t_matrix) noexcept;

	/*
	* 下标运算符(非常量版本)
	*/
	row_vector& operator[](int t_pos);

	/*
	* 下标运算符(常量版本)
	*/
	const row_vector& operator[](int t_pos) const;


	/*---其他功能函数---*/
public:
	/*
	* 生成格式化字符串
	*/
	std::string to_string();

	/*
	* 打印到指定输出流
	*/
	void print(std::ostream& t_out = std::cout, int t_num_enter = 0);


private:
	/*
	* 用于计算当维度小于3时的矩阵求逆运算
	* 同理，当布尔值为true时，无法求逆便会求伪逆(广义逆矩阵)
	*/
	matrix inverse_when_dim_lower_than3(bool t_try_pseudo_inverse);
};

/*
* 单目取反运算符
*/
matrix operator-(const matrix& t_matrix);


/*
* 矩阵间的运算，即加、减、乘
*/
matrix operator+(const matrix& t_matrix1, const matrix& t_matrix2);
matrix operator-(const matrix& t_matrix1, const matrix& t_matrix2);
matrix operator*(const matrix& t_matrix1, const matrix& t_matrix2);

/*
* 矩阵与复数的运算，即加、减、乘、除
*/
matrix operator+(const matrix& t_matrix, const complex& t_complex);
matrix operator+(const complex& t_complex, const matrix& t_matrix);

matrix operator-(const matrix& t_matrix, const complex& t_complex);
matrix operator-(const complex& t_complex, const matrix& t_matrix);

matrix operator*(const matrix& t_matrix, const complex& t_complex);
matrix operator*(const complex& t_complex, const matrix& t_matrix);

matrix operator/(const matrix& t_matrix, const complex& t_complex);
matrix operator/(const complex& t_complex, const matrix& t_matrix);


/*
* 对应位置元素计算
*/
matrix element_product(const matrix& t_matrix1, const matrix& t_matrix2);
matrix element_divide(const matrix& t_matrix1, const matrix& t_matrix2);
