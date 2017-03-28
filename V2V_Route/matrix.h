#pragma once
#include<vector>
#include<random>
#include<string>
#include<utility>
#include<iostream>
#include"complex.h"

class row_vector {
	/*------------------��------------------*/
private:
	/*
	* �ײ���������Ϊ˽�н�ֱֹ�ӷ���
	*/
	complex* m_row_vector;
public:
	/*
	* �������ȣ��������������������Ϊ����
	*/
	int m_col;


	/*------------------����------------------*/
private:
	/*
	* ��Դ�������ͷŶ��ڴ�
	*/
	void free();
public:
	/*
	* ���������������ͷ�rowVector
	*/
	~row_vector();

	/*
	* Ĭ�Ϲ��캯��
	* ��������Ϊ0��ָ��Ϊ��
	*/
	row_vector();

	/*
	* ����һ������ά�ȵĲ���
	* ��ʼ��������ά�ȵ���ָ����t_Col��Ԫ�س�ʼ��΢Ϊ��ֵ(0,0)
	*/
	explicit row_vector(int t_col);

	/*
	* �������캯��
	* ע����ָ���Աʱ�����������ʵ��
	*/
	row_vector(const row_vector& t_row_vector);

	/*
	* �ƶ����캯��
	* ע���ƶ���Դ����ָ���Ա���ÿ�
	* ����Ϊ���׳��쳣��
	*/
	row_vector(row_vector&& t_row_vector) noexcept;

	/*
	* �б��캯��
	*/
	row_vector(const std::initializer_list<complex> t_il);


	/*
	* ������ֵ�����
	*/
	row_vector& operator=(const row_vector& t_row_vector);

	/*
	* �ƶ���ֵ�����
	*/
	row_vector& operator=(row_vector&& t_row_vector) noexcept;

	/*
	* �±������
	*/
	complex& operator[](int t_pos);

	/*
	* �����汾���±������
	*/
	const complex& operator[](int t_pos) const;

	/*
	* ���ô�С
	* ���ú���ά�ȱ����ô��(0,0)
	* ���ú���ά�ȱ�С����ôɾ������Ԫ��
	*/
	void resize(int t_size);

	/*
	* ���������ĸ�ʽ���ַ���
	*/
	std::string toString();

	/*
	* ��������ӡ��ָ�������
	*/
	void print(std::ostream& t_out = std::cout);

};

/*
* ������Ŀȡ�������
*/
row_vector operator-(const row_vector& t_row_vector);

/*
* �����ӷ�����������
*/
row_vector operator+(const row_vector& t_row_vector1, const row_vector& t_row_vector2);
row_vector operator-(const row_vector& t_row_vector1, const row_vector& t_row_vector2);

/*
* �����븴��(����������)�ļӼ��˳������
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
* �����������Ķ�ӦԪ�س˷������
*/
row_vector element_product(const row_vector& t_row_vector1, const row_vector& t_row_vector2);
row_vector element_divide(const row_vector& t_row_vector1, const row_vector& t_row_vector2);


class matrix {
	/*------------------��̬------------------*/
public:
	/*
	* ��������棬���๲��
	*/
	static std::default_random_engine s_engine;

	/*
	* ��������Һϲ��������ά�ȱ�����ͬ
	*/
	static matrix horizon_merge(const matrix& t_matrix1, const matrix& t_matrix2);

	/*
	* �����ˮƽ����
	*/
	static std::pair<matrix, matrix> horizon_split(const matrix& t_matrix, int t_left_col, int t_right_col);

	/*
	* ����Ĵ�ֱ����
	*/
	static std::pair<matrix, matrix> vertical_split(const matrix& t_matrix, int t_up_row, int t_down_row);

	/*
	* ����ָ��ά�ȵĵ�λ����
	*/
	static matrix eye(const int t_dim);
	/*------------------��------------------*/
private:
	/*
	* �ײ�������������Ϊ˽�н�ֱֹ�ӷ���
	*/
	row_vector* m_matrix;
public:
	/*
	* ����
	*/
	int m_row;

	/*
	* ����
	*/
	int m_col;

	/*------------------����------------------*/
private:
	/*
	* ������ڴ�
	*/
	void free();
public:
	/*
	* ��������
	*/
	~matrix();

	/*
	* Ĭ�Ϲ��캯��
	* ���о�Ϊ0��ָ��Ϊ��
	*/
	matrix();

	/*
	* ָ��ά�ȵĹ��캯��
	*/
	matrix(int t_row, int t_col);

	/*
	* �������캯����ע����ָ��ʱ���������ʵ��
	*/
	matrix(const matrix& t_matrix);

	/*
	* �ƶ����캯����ע����ָ��ʱ�ƶ������ʵ��
	*/
	matrix(matrix&& t_matrix) noexcept;

	/*
	* �б��캯��
	* �б���ÿ�е�Ԫ����Ŀ���Բ�ͬ
	* �����һ�е�ά����Ϊ����
	* ����Ĳ���(0,0)
	*/
	matrix(const std::initializer_list<row_vector>& t_il);

	/*---�����ܺ���---*/
public:
	/*
	* ��������ȷֲ�����ֵ
	* ʵ���ķ�Χ[t_RealLeft,t_RealRight]
	* �鲿�ķ�Χ[t_ImagLeft,t_ImagRight]
	*/
	void random_fill(double t_real_left, double t_real_right, double t_imag_left, double t_imag_right);

	/*
	* ����
	*/
	matrix conjugate();

	/*
	* ��ת��
	*/
	matrix transpose();

	/*
	* ����ת��
	*/
	matrix hermitian();

	/*
	* �������
	* ������ֵΪtrueʱ���������޷����棬�᳢����α��(���������)
	*/
	matrix inverse(bool t_try_pseudo_inverse = false);

	/*
	* ��Խ��߹��ɵľ���(������)
	*/
	matrix diag();

	/*
	* �����ȷֽ�
	*/
	std::pair<matrix, matrix> full_rank_decomposition();

	/*
	* ����������
	*/
	matrix pseudo_inverse();


	/*---��Ա���������---*/
public:
	/*
	* ������ֵ�����
	* ע�⣬��ָ���Աʱ��Ҳ��Ҫ����Ը�ֵ����ȷ��
	* ע��ԭָ��������ʱ��
	*/
	matrix& operator=(const matrix& t_matrix);

	/*
	* �ƶ���ֵ�����
	* ��Ҫ����Ը�ֵ����ȷ��
	* ע��ԭָ��������ʱ��
	* ����Ϊ�����׳��쳣
	*/
	matrix& operator=(matrix&& t_matrix) noexcept;

	/*
	* �±������(�ǳ����汾)
	*/
	row_vector& operator[](int t_pos);

	/*
	* �±������(�����汾)
	*/
	const row_vector& operator[](int t_pos) const;


	/*---�������ܺ���---*/
public:
	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string to_string();

	/*
	* ��ӡ��ָ�������
	*/
	void print(std::ostream& t_out = std::cout, int t_num_enter = 0);


private:
	/*
	* ���ڼ��㵱ά��С��3ʱ�ľ�����������
	* ͬ��������ֵΪtrueʱ���޷���������α��(���������)
	*/
	matrix inverse_when_dim_lower_than3(bool t_try_pseudo_inverse);
};

/*
* ��Ŀȡ�������
*/
matrix operator-(const matrix& t_matrix);


/*
* ���������㣬���ӡ�������
*/
matrix operator+(const matrix& t_matrix1, const matrix& t_matrix2);
matrix operator-(const matrix& t_matrix1, const matrix& t_matrix2);
matrix operator*(const matrix& t_matrix1, const matrix& t_matrix2);

/*
* �����븴�������㣬���ӡ������ˡ���
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
* ��Ӧλ��Ԫ�ؼ���
*/
matrix element_product(const matrix& t_matrix1, const matrix& t_matrix2);
matrix element_divide(const matrix& t_matrix1, const matrix& t_matrix2);
