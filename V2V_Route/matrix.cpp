/*
* =====================================================================================
*
*       Filename:  matrix.cpp
*
*    Description:  ������ʵ��
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


#include<iostream>
#include<iomanip>
#include<sstream>
#include<cmath>
#include<string.h>
#include<stdexcept>
#include"matrix.h"
#include"function.h"

using namespace std;

void row_vector::free() {
	memory_clean::safe_delete(m_row_vector, true);
}

row_vector::~row_vector() {
	free();
}


row_vector::row_vector() :
	m_col(0) {
	m_row_vector = nullptr;
}


row_vector::row_vector(int t_col) :
	m_col(t_col) {
	if (m_col < 0) throw logic_error("������ά�ȱ����ǷǸ���");
	m_row_vector = new complex[m_col]();
}


row_vector::row_vector(const row_vector& t_row_vector) :
	m_col(t_row_vector.m_col) {
	m_row_vector = new complex[m_col]();
	memcpy(m_row_vector, t_row_vector.m_row_vector, m_col * sizeof(complex));
}


row_vector::row_vector(row_vector&& t_row_vector) noexcept
	:m_col(t_row_vector.m_col), m_row_vector(t_row_vector.m_row_vector) {
	//�ڳ�ʼ���б�ӹ���Դ
	t_row_vector.m_row_vector = nullptr;//�ÿ�ָ��
}


row_vector::row_vector(const initializer_list<complex> t_il) {
	m_col = static_cast<int>(t_il.size());
	m_row_vector = new complex[m_col]();
	int iter = 0;
	for (const complex&c : t_il)
		m_row_vector[iter++] = c;
}


row_vector& row_vector::operator=(const row_vector& t_row_vector) {
	if (m_row_vector != t_row_vector.m_row_vector) {//����Ը�ֵ����ȷ��
		free();
		m_col = t_row_vector.m_col;
		m_row_vector = new complex[m_col]();
		memcpy(m_row_vector, t_row_vector.m_row_vector, m_col * sizeof(complex));
	}
	return *this;
}


row_vector& row_vector::operator=(row_vector&& t_row_vector) noexcept {
	if (m_row_vector != t_row_vector.m_row_vector) {//����Ը�ֵ����ȷ��
		free();//������Դ
		m_col = t_row_vector.m_col;
		m_row_vector = t_row_vector.m_row_vector;//�ӹ���Դ
		t_row_vector.m_row_vector = nullptr;//�ÿ�ָ��
	}
	return *this;
}


complex& row_vector::operator[](int t_pos) {
	return m_row_vector[t_pos];
}


const complex& row_vector::operator[](int t_pos) const {
	return m_row_vector[t_pos];
}


void row_vector::resize(int t_Size) {
	if (t_Size < 0) throw logic_error("������ά�ȱ����ǷǸ���");
	int preCol = m_col;
	complex* preRowVector = m_row_vector;
	m_col = t_Size;
	m_row_vector = new complex[m_col]();
	int copyCol = m_col < preCol ? m_col : preCol;
	memcpy(m_row_vector, preRowVector, copyCol * sizeof(complex));
	/*-----------------------ATTENTION-----------------------
	* ���ﲻ�ܵ���free()������ԭ������Դ
	* free()���ͷ�thisָ��Ķ����rowVectorָ��
	* ����ʱ����rowVectorָ���Ѿ�ָ�����µ�����
	* �����ֶ�ɾ��֮ǰ��ָ��
	*------------------------ATTENTION-----------------------*/
	memory_clean::safe_delete(preRowVector, true);
}


string row_vector::toString() {
	ostringstream ss;
	for (int c = 0; c < m_col; c++) {
		ss << this->operator[](c).to_string() << "  ";
	}
	return ss.str();
}


void row_vector::print(ostream& t_out) {
	t_out << toString();
}

//��������Ŀȡ�������
row_vector operator-(const row_vector& t_row_vector) {
	row_vector res(t_row_vector.m_col);
	for (int c = 0; c < t_row_vector.m_col; c++) {
		res[c] = -t_row_vector[c];
	}
	return res;
}


//��������������������
row_vector operator+(const row_vector& t_row_vector1, const row_vector& t_row_vector2) {
	if (t_row_vector1.m_col != t_row_vector2.m_col) throw logic_error("����ά�Ȳ�ͬ���޷����");
	row_vector res(t_row_vector1.m_col);
	for (int c = 0; c < t_row_vector1.m_col; c++) {
		res[c] = t_row_vector1[c] + t_row_vector2[c];
	}
	return res;
}


//��������������������
row_vector operator-(const row_vector& t_row_vector1, const row_vector& t_row_vector2) {
	if (t_row_vector1.m_col != t_row_vector2.m_col) throw logic_error("����ά�Ȳ�ͬ���޷����");
	row_vector res(t_row_vector1.m_col);
	for (int c = 0; c < t_row_vector1.m_col; c++) {
		res[c] = t_row_vector1[c] - t_row_vector2[c];
	}
	return res;
}


//�������븴��������
row_vector operator+(const row_vector& t_row_vector, const complex&t_complex) {
	row_vector res(t_row_vector.m_col);
	for (int c = 0; c < t_row_vector.m_col; c++) {
		res[c] = t_row_vector[c] + t_complex;
	}
	return res;
}
row_vector operator+(const complex&t_complex, const row_vector& t_row_vector) {
	return t_row_vector + t_complex;
}


row_vector operator-(const row_vector& t_row_vector, const complex&t_complex) {
	row_vector res(t_row_vector.m_col);
	for (int c = 0; c < t_row_vector.m_col; c++) {
		res[c] = t_row_vector[c] - t_complex;
	}
	return res;
}
row_vector operator-(const complex&t_complex, const row_vector& t_row_vector) {
	return -t_row_vector + t_complex;
}


row_vector operator*(const row_vector& t_row_vector, const complex&t_complex) {
	row_vector res(t_row_vector.m_col);
	for (int c = 0; c < t_row_vector.m_col; c++) {
		res[c] = t_row_vector[c] * t_complex;
	}
	return res;
}
row_vector operator*(const complex&t_complex, const row_vector& t_row_vector) {
	return t_row_vector*t_complex;
}


row_vector operator/(const row_vector& t_row_vector, const complex&t_complex) {
	if (complex::abs(t_complex) == 0) throw logic_error("����ģֵ����Ϊ0");
	row_vector res(t_row_vector.m_col);
	for (int c = 0; c < t_row_vector.m_col; c++) {
		res[c] = t_row_vector[c] / t_complex;
	}
	return res;
}
row_vector operator/(const complex&t_complex, const row_vector& t_row_vector) {
	row_vector res(t_row_vector.m_col);
	for (int c = 0; c < t_row_vector.m_col; c++) {
		if (complex::abs(t_row_vector[c]) == 0) throw logic_error("�����к���ģֵΪ0��Ԫ�أ�������Ϊ��ĸ");
		res[c] = t_complex / t_row_vector[c];
	}
	return res;
}


row_vector element_product(const row_vector& t_row_vector1, const row_vector& t_row_vector2) {
	if (t_row_vector1.m_col != t_row_vector2.m_col) throw logic_error("����ά�Ȳ�ƥ��");
	row_vector res(t_row_vector1.m_col);
	for (int c = 0; c < t_row_vector1.m_col; c++) {
		res[c] = t_row_vector1[c] * t_row_vector2[c];
	}
	return res;
}
row_vector element_divide(const row_vector& t_row_vector1, const row_vector& t_row_vector2) {
	if (t_row_vector1.m_col != t_row_vector2.m_col) throw logic_error("����ά�Ȳ�ƥ��");
	row_vector res(t_row_vector1.m_col);
	for (int c = 0; c < t_row_vector1.m_col; c++) {
		if (complex::abs(t_row_vector2[c]) == 0) throw logic_error("Ԫ��ģֵΪ0��������Ϊ����");
		res[c] = t_row_vector1[c] / t_row_vector2[c];
	}
	return res;
}


default_random_engine matrix::s_engine((unsigned)time(NULL));


void matrix::free() {
	memory_clean::safe_delete(m_matrix, true);
}


matrix:: ~matrix() {
	free();
}


matrix::matrix() :
	m_row(0), m_col(0) {
	m_matrix = nullptr;
}



matrix::matrix(int t_row, int t_col) :
	m_row(t_row), m_col(t_col) {
	if (t_row < 0 || t_col < 0) throw logic_error("�����ά�ȱ����ǷǸ���");
	m_matrix = new row_vector[m_row];
	for (int iter = 0; iter < m_row; iter++) {
		m_matrix[iter] = row_vector(m_col);
	}
}


matrix::matrix(const matrix& t_matrix) :
	m_row(t_matrix.m_row), m_col(t_matrix.m_col) {
	m_matrix = new row_vector[m_row];
	for (int iter = 0; iter < m_row; iter++) {
		/*-----------------------ATTENTION-----------------------
		* ����RowVector�Ŀ�����ֵ�����
		* ���ﲻ�ܵ���memcpy������matrix��Ա����ΪҪʵ�ֿ�����ֵ������
		* memcpy�Ὣt_Matrix.matrix�ڴ������ֵ��ȫ��ֵһ�ݸ�this->matrix
		* ���Ǹ��ڴ�������RowVector���󣬶��ö��������ָ��
		* �����ᵼ�¸���ǰ��RowVector�е�ָ��һ��
		*------------------------ATTENTION-----------------------*/
		m_matrix[iter] = t_matrix[iter];
	}
}


matrix::matrix(matrix&& t_matrix) noexcept
	:m_row(t_matrix.m_row), m_col(t_matrix.m_col), m_matrix(t_matrix.m_matrix) {
	//�ڳ�ʼ���б��нӹ���Դ
	t_matrix.m_matrix = nullptr;//�ÿ�ָ��
}


matrix::matrix(const initializer_list<row_vector>& t_il) {
	if (t_il.size() == 0) {
		m_row = 0;
		m_col = 0;
		m_matrix = nullptr;
	}
	else {
		m_col = 0;
		m_row = static_cast<int>(t_il.size());
		m_matrix = new row_vector[m_row];
		for (const row_vector& rv : t_il) {
			if (m_col < rv.m_col) m_col = rv.m_col;
		}
		int iter = 0;
		for (const row_vector& rv : t_il) {
			row_vector rvTmp(rv);
			rvTmp.resize(m_col);
			m_matrix[iter++] = std::move(rvTmp);
		}
	}
}


void matrix::random_fill(double t_real_left, double t_real_right, double t_imag_left, double t_imag_right) {
	uniform_real_distribution<double> urdReal(t_real_left, t_real_right);
	uniform_real_distribution<double> urdImag(t_imag_left, t_imag_right);
	for (int r = 0; r < m_row; r++) {
		for (int c = 0; c < m_col; c++) {
			m_matrix[r][c] = complex(urdReal(matrix::s_engine), urdImag(matrix::s_engine));
		}
	}
}


matrix matrix::conjugate() {
	matrix res(m_row, m_col);
	for (int r = 0; r < m_row; r++) {
		for (int c = 0; c < m_col; c++) {
			res[r][c] = m_matrix[r][c].conjugate();
		}
	}
	return res;
}


matrix matrix::transpose() {
	matrix res(m_col, m_row);
	for (int r = 0; r < m_col; r++) {
		for (int c = 0; c < m_row; c++) {
			res[r][c] = m_matrix[c][r];
		}
	}
	return res;
}


matrix matrix::hermitian() {
	matrix res = conjugate();
	res = res.transpose();
	return res;
}


matrix matrix::inverse(bool t_try_pseudo_inverse) {
	if (m_row <= 0 || m_col <= 0 || m_row != m_col) throw logic_error("�þ����޷�����");

	if (m_row < 3) return inverse_when_dim_lower_than3(t_try_pseudo_inverse);

	matrix mergeMatrix = matrix::horizon_merge(*this, matrix::eye(m_row));

	//�ȱ任�������Ǿ���
	complex zero(0, 0);
	int tmpRow = 0;
	row_vector tmpRV;
	for (int r = 0; r < m_row; r++) {
		if (mergeMatrix[r][r] == zero) {//����ǰ�еĶԽ��߲���Ϊ0�����벻Ϊ0�����л���
			tmpRow = r + 1;
			while (tmpRow < m_row&&mergeMatrix[tmpRow][r] == zero)
				tmpRow++;
			if (tmpRow == m_row) {
				if (t_try_pseudo_inverse) {
					return this->pseudo_inverse();
				}
				else {
					throw logic_error("�þ������ȣ��޷��������");
				}
			}

			//����r��tmp����
			tmpRV = mergeMatrix[r];
			mergeMatrix[r] = mergeMatrix[tmpRow];
			mergeMatrix[tmpRow] = tmpRV;
		}
		mergeMatrix[r] = mergeMatrix[r] / mergeMatrix[r][r];//���Խ��߲�����1

															//�����е��°벿����0
		for (tmpRow = r + 1; tmpRow < m_row; tmpRow++) {
			if (mergeMatrix[tmpRow][r] == zero)continue;
			complex factor = -mergeMatrix[tmpRow][r];
			mergeMatrix[tmpRow] = mergeMatrix[r] * factor + mergeMatrix[tmpRow];
		}
	}


	//���ұ߼����任Ϊ������
	for (int r = m_row - 1; r >= 0; r--) {
		for (tmpRow = r - 1; tmpRow >= 0; tmpRow--) {
			if (mergeMatrix[tmpRow][r] == zero)continue;
			complex factor = -mergeMatrix[tmpRow][r];
			mergeMatrix[tmpRow] = mergeMatrix[r] * factor + mergeMatrix[tmpRow];
		}
	}
	return matrix::horizon_split(mergeMatrix, m_col, m_col).second;
}


matrix matrix::inverse_when_dim_lower_than3(bool t_try_pseudo_inverse) {
	if (m_row == 1) {
		if (complex::abs(this->operator[](0)[0]) == 0) {
			if (t_try_pseudo_inverse)
				return matrix{ { 0,0 } };
			else
				throw logic_error("�þ����޷�����");
		}
		return matrix{ { 1 / this->operator[](0)[0] } };
	}
	else {
		matrix res(m_row, m_col);
		complex denominator = this->operator[](0)[0] * this->operator[](1)[1] - this->operator[](0)[1] * this->operator[](1)[0];
		if (denominator == 0) {
			if (t_try_pseudo_inverse) return pseudo_inverse();
			else throw logic_error("�þ����޷�����");
		}
		res[0][0] = this->operator[](1)[1] / denominator;
		res[0][1] = -(this->operator[](0)[1]) / denominator;
		res[1][0] = -(this->operator[](1)[0]) / denominator;
		res[1][1] = this->operator[](0)[0] / denominator;
		return res;
	}
}


matrix matrix::diag() {
	if (m_row != m_col) throw logic_error("�ú���ֻ֧�ַ���");
	matrix res(1, m_row);
	for (int r = 0; r < m_row; r++) {
		res[0][r] = this->operator[](r)[r];
	}
	return res;
}


pair<matrix, matrix>  matrix::full_rank_decomposition() {
	//������ά��Ϊm*n����Ϊr��r����Ϊ1
	matrix mergeMatrix = matrix::horizon_merge(*this, matrix::eye(m_row));

	//���Ƚ�����ת��Ϊ��׼�ͣ���������r*n�������Ⱦ���������(m-r)*n�������
	complex zero(0, 0);
	int tmpRow = 0;
	row_vector tmpRV;
	int iterCol = 0;
	int rank = 0;
	for (int r = 0; r < m_row; r++) {
		if (iterCol == m_col) break;
		if (mergeMatrix[r][iterCol] == zero) {//����ǰ�еĶԽ��߲���Ϊ0�����벻Ϊ0�����л���
			tmpRow = r + 1;
			while (tmpRow < m_row&&mergeMatrix[tmpRow][iterCol] == zero)
				tmpRow++;
			if (tmpRow == m_row) { //������û�з���Ԫ�أ���������
				iterCol++;
				r--;
				continue;//�����ڵ�ǰ�е���һ��Ѱ�ҷ���Ԫ��
			}
			else {
				//����r��tmp����
				tmpRV = mergeMatrix[r];
				mergeMatrix[r] = mergeMatrix[tmpRow];
				mergeMatrix[tmpRow] = tmpRV;
			}
		}
		mergeMatrix[r] = mergeMatrix[r] / mergeMatrix[r][iterCol];//���Խ��߲�����1

																  //�����е��°벿����0
		for (tmpRow = r + 1; tmpRow < m_row; tmpRow++) {
			if (mergeMatrix[tmpRow][iterCol] == zero)continue;
			complex factor = -mergeMatrix[tmpRow][iterCol];
			mergeMatrix[tmpRow] = mergeMatrix[iterCol] * factor + mergeMatrix[tmpRow];
		}
		iterCol++;
		rank++;
	}

	if (rank == 0) throw logic_error("�þ�����Ϊ0�����������ȷֽ������");

	pair<matrix, matrix> splitRes = horizon_split(mergeMatrix, mergeMatrix.m_col - m_row, m_row);
	matrix B = splitRes.first;
	matrix P = splitRes.second;

	matrix F = horizon_split(P.inverse(), rank, P.m_col - rank).first;
	matrix G = vertical_split(B, rank, B.m_row - rank).first;

	return pair<matrix, matrix>(F, G);
}


matrix matrix::pseudo_inverse() {
	pair<matrix, matrix> fullRankDecompositionRes = full_rank_decomposition();
	matrix F = fullRankDecompositionRes.first;
	matrix G = fullRankDecompositionRes.second;
	return G.hermitian()*(G*G.hermitian()).inverse()*(F.hermitian()*F).inverse()*F.hermitian();
}



matrix& matrix::operator=(const matrix& t_matrix) {
	if (m_matrix != t_matrix.m_matrix) {//����Ը�ֵ����ȷ��
		free();
		m_row = t_matrix.m_row;
		m_col = t_matrix.m_col;
		m_matrix = new row_vector[m_row];
		for (int r = 0; r < m_row; r++) {
			/*-----------------------ATTENTION-----------------------
			* ����RowVector�Ŀ�����ֵ�����
			* ���ﲻ�ܵ���memcpy������matrix��Ա����ΪҪʵ�ֿ�����ֵ������
			* memcpy�Ὣt_Matrix.matrix�ڴ������ֵ��ȫ��ֵһ�ݸ�this->matrix
			* ���Ǹ��ڴ�������RowVector���󣬶��ö��������ָ��
			* �����ᵼ�¸���ǰ��RowVector�е�ָ��һ��
			*------------------------ATTENTION-----------------------*/
			m_matrix[r].operator=(t_matrix.m_matrix[r]);
		}
	}
	return *this;
}


matrix& matrix::operator=(matrix&& t_matrix) noexcept {
	if (m_matrix != t_matrix.m_matrix) {//����Ը�ֵ����ȷ��
		free();//������Դ
		m_row = t_matrix.m_row;
		m_col = t_matrix.m_col;
		m_matrix = t_matrix.m_matrix;//�ӹ���Դ
		t_matrix.m_matrix = nullptr;//�ÿ�ָ��
	}
	return *this;
}


row_vector& matrix::operator[](int t_pos) {
	return m_matrix[t_pos];
}


const row_vector& matrix::operator[](int t_pos) const {
	return  m_matrix[t_pos];
}


string matrix::to_string() {
	ostringstream ss;
	for (int r = 0; r < m_row; r++) {
		ss << this->operator[](r).toString() << endl;
	}
	return ss.str();
}


void matrix::print(ostream&t_out, int t_num_enter) {
	t_out << to_string();
	for (int i = 0; i < t_num_enter; i++)
		t_out << endl;
}


matrix matrix::horizon_merge(const matrix& t_matrix1, const matrix& t_matrix2) {
	if (t_matrix1.m_row != t_matrix2.m_row) throw logic_error("�����������޷��ϲ�");
	matrix res(t_matrix1.m_row, t_matrix1.m_col + t_matrix2.m_col);
	for (int r = 0; r < t_matrix1.m_row; r++) {
		for (int c = 0; c < t_matrix1.m_col; c++) {
			res[r][c] = t_matrix1[r][c];
		}
	}
	for (int r = 0; r < t_matrix2.m_row; r++) {
		for (int c = 0; c < t_matrix2.m_col; c++) {
			res[r][c + t_matrix1.m_col] = t_matrix2[r][c];
		}
	}
	return res;
}


pair<matrix, matrix> matrix::horizon_split(const matrix& t_matrix, int t_left_col, int t_right_col) {
	if (t_matrix.m_col != t_left_col + t_right_col) throw logic_error("�þ����޷����ѳ�ָ��ά��");
	matrix left(t_matrix.m_row, t_left_col), right(t_matrix.m_row, t_right_col);
	for (int r = 0; r < left.m_row; r++) {
		for (int c = 0; c < left.m_col; c++) {
			left[r][c] = t_matrix[r][c];
		}
	}

	for (int r = 0; r < right.m_row; r++) {
		for (int c = 0; c < right.m_col; c++) {
			right[r][c] = t_matrix[r][c + left.m_col];
		}
	}
	return pair<matrix, matrix>(left, right);
}


pair<matrix, matrix> matrix::vertical_split(const matrix& t_matrix, int t_up_row, int t_down_row) {
	if (t_matrix.m_row != t_up_row + t_down_row) throw logic_error("�þ����޷����ѳ�ָ��ά��");
	matrix up(t_up_row, t_matrix.m_col), down(t_down_row, t_matrix.m_col);
	for (int r = 0; r < up.m_row; r++) {
		for (int c = 0; c < up.m_col; c++) {
			up[r][c] = t_matrix[r][c];
		}
	}

	for (int r = 0; r < down.m_row; r++) {
		for (int c = 0; c < down.m_col; c++) {
			down[r][c] = t_matrix[r + up.m_row][c];
		}
	}
	return pair<matrix, matrix>(up, down);
}

matrix matrix::eye(const int t_dim) {
	if (t_dim < 1) throw logic_error("��λ��ά������Ϊ1");
	matrix res(t_dim, t_dim);
	for (int i = 0; i < t_dim; i++) {
		res[i][i] = complex(1, 0);
	}
	return res;
}


//��Ŀȡ�������
matrix operator-(const matrix& t_matrix) {
	matrix res(t_matrix.m_row, t_matrix.m_col);
	for (int r = 0; r < t_matrix.m_row; r++) {
		res[r] = -t_matrix[r];
	}
	return res;
}


//����������
matrix operator+(const matrix& t_matrix1, const matrix& t_matrix2) {
	if (t_matrix1.m_row != t_matrix2.m_row || t_matrix1.m_col != t_matrix2.m_col) throw logic_error("����ά�Ȳ�ͬ���޷����");
	matrix res(t_matrix1.m_row, t_matrix1.m_col);
	for (int r = 0; r < t_matrix1.m_row; r++) {
		for (int c = 0; c < t_matrix1.m_col; c++) {
			res[r][c] = t_matrix1[r][c] + t_matrix2[r][c];
		}
	}
	return res;
}


matrix operator-(const matrix& t_matrix1, const matrix& t_matrix2) {
	if (t_matrix1.m_row != t_matrix2.m_row || t_matrix1.m_col != t_matrix2.m_col) throw logic_error("����ά�Ȳ�ͬ���޷����");
	matrix res(t_matrix1.m_row, t_matrix1.m_col);
	for (int r = 0; r < t_matrix1.m_row; r++) {
		for (int c = 0; c < t_matrix1.m_col; c++) {
			res[r][c] = t_matrix1[r][c] - t_matrix2[r][c];
		}
	}
	return res;
}


matrix operator*(const matrix& t_matrix1, const matrix& t_matrix2) {
	if (t_matrix1.m_col != t_matrix2.m_row) throw logic_error("����ά�Ȳ�ƥ�䣬�޷����");

	matrix res(t_matrix1.m_row, t_matrix2.m_col);

	for (int r = 0; r < t_matrix1.m_row; r++) {
		for (int c = 0; c < t_matrix2.m_col; c++) {
			//����ÿһ��
			complex tmp(0, 0);
			for (int idx = 0; idx < t_matrix1.m_col; idx++) {
				tmp += t_matrix1[r][idx] * t_matrix2[idx][c];
			}
			res[r][c] = tmp;
		}
	}
	return res;
}


//�����븴��������
matrix operator+(const matrix& t_matrix, const complex& t_complex) {
	matrix res(t_matrix.m_row, t_matrix.m_col);
	for (int r = 0; r < t_matrix.m_row; r++) {
		res[r] = t_matrix[r] + t_complex;
	}
	return res;
}
matrix operator+(const complex& t_complex, const matrix& t_matrix) {
	return t_matrix + t_complex;
}


matrix operator-(const matrix& t_matrix, const complex& t_complex) {
	matrix res(t_matrix.m_row, t_matrix.m_col);
	for (int r = 0; r < t_matrix.m_row; r++) {
		res[r] = t_matrix[r] - t_complex;
	}
	return res;
}
matrix operator-(const complex& t_complex, const matrix& t_matrix) {
	return -t_matrix + t_complex;
}


matrix operator*(const matrix& t_matrix, const complex& t_complex) {
	matrix res(t_matrix.m_row, t_matrix.m_col);
	for (int r = 0; r < t_matrix.m_row; r++) {
		res[r] = t_matrix[r] * t_complex;
	}
	return res;
}
matrix operator*(const complex& t_complex, const matrix& t_matrix) {
	return t_matrix*t_complex;
}


matrix operator/(const matrix& t_matrix, const complex& t_complex) {
	if (complex::abs(t_complex) == 0) throw logic_error("����ģֵΪ0���Ƿ���");
	matrix res(t_matrix.m_row, t_matrix.m_col);
	for (int r = 0; r < t_matrix.m_row; r++) {
		res[r] = t_matrix[r] / t_complex;
	}
	return res;
}
matrix operator/(const complex& t_complex, const matrix& t_matrix) {
	matrix res(t_matrix.m_row, t_matrix.m_col);
	for (int r = 0; r < t_matrix.m_row; r++) {
		res[r] = t_complex / t_matrix[r];
	}
	return res;
}


matrix element_product(const matrix& t_matrix1, const matrix& t_matrix2) {
	if (t_matrix1.m_row != t_matrix2.m_row ||
		t_matrix1.m_col != t_matrix2.m_col) throw logic_error("����ά�Ȳ�ƥ��");
	matrix res(t_matrix1.m_row, t_matrix1.m_col);
	for (int r = 0; r < t_matrix1.m_row; r++) {
		res[r] = element_product(t_matrix1[r], t_matrix2[r]);
	}
	return res;
}
matrix element_divide(const matrix& t_matrix1, const matrix& t_matrix2) {
	if (t_matrix1.m_row != t_matrix2.m_row ||
		t_matrix1.m_col != t_matrix2.m_col) throw logic_error("����ά�Ȳ�ƥ��");
	matrix res(t_matrix1.m_row, t_matrix1.m_col);
	for (int r = 0; r < t_matrix1.m_row; r++) {
		res[r] = element_divide(t_matrix1[r], t_matrix2[r]);
	}
	return res;
}