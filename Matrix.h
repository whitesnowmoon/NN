#pragma once
#ifndef _Matrix_h_
#define _Matrix_h_

#include<vector>
#include <iomanip>
#include <iostream>

template<typename Type_ = double>
class Matrix
{
public:
	Matrix();											
	Matrix(int Matrix_row, int Matrix_column);
	Matrix(int Matrix_row, int Matrix_column, Type_* const* const Matrix_data);
	Matrix(const Matrix& old);
	Matrix(Matrix&& old);
	Matrix(std::vector<Type_>& vec_n);
	~Matrix();
	void print();
	Matrix& operator=(Matrix&& old);
	Matrix& operator=(const Matrix& old);
	Matrix& operator=(std::vector<Type_>& vec_n);
	Matrix operator*(Matrix& mutipul);
	Matrix operator*(Type_ num);
	Matrix operator-(Matrix& sub);
	Matrix operator+(Matrix& add);
	void fun_handle(Type_(*fun)(Type_));
	void rotate();																	
	Matrix<Type_> hconcat(const Matrix& t);
	Matrix<Type_> vconcat(const Matrix& t);
	Matrix<Type_> Diagonal();
	template<typename Type_ = double> friend std::ostream& operator<<(std::ostream& out, const Matrix<Type_>& t);
	Type_* const* const Get_Matrix_data(void);
	int Get_Matrix_row(void);
	int Get_Matrix_column(void);
private:
	Type_** Matrix_data;
	int Matrix_row;
	int Matrix_column;
};

template<typename Type_>
inline Matrix<Type_>::Matrix() {
	this->Matrix_row = 0;
	this->Matrix_column = 0;
	this->Matrix_data = nullptr;
}

template<typename Type_>
Matrix<Type_>::Matrix(int Matrix_row, int Matrix_column) {
	srand(time(0));
	this->Matrix_row = Matrix_row;
	this->Matrix_column = Matrix_column;
	Matrix_data = new Type_ * [Matrix_row];
	for (int i = 0; i < Matrix_row; i++)
	{
		Matrix_data[i] = new Type_[Matrix_column];
	}
	for (int i = 0; i < Matrix_row; i++)
	{
		for (int j = 0; j < Matrix_column; j++)
		{
			Matrix_data[i][j] = ((Type_)rand()) / RAND_MAX;
		}
	}
}

template<typename Type_>
inline Matrix<Type_>::Matrix(int Matrix_row, int Matrix_column, Type_* const* const Matrix_data) {
	this->Matrix_row = Matrix_row;
	this->Matrix_column = Matrix_column;
	this->Matrix_data = (Type_**)Matrix_data;
}

template<typename Type_>
Matrix<Type_>::Matrix(const Matrix& old) :Matrix_row(old.Matrix_row), Matrix_column(old.Matrix_column), Matrix_data(new double* [old.Matrix_row]) {
	for (int i = 0; i < this->Matrix_row; i++)
	{
		this->Matrix_data[i] = new double[this->Matrix_column];
		memcpy(this->Matrix_data[i], old.Matrix_data[i], old.Matrix_column * sizeof(double));
	}
}

template<typename Type_>
Matrix<Type_>::Matrix(Matrix&& old):Matrix_row(old.Matrix_row), Matrix_column(old.Matrix_column), Matrix_data(old.Matrix_data) {
		old.Matrix_data = nullptr;
}

template<typename Type_>
Matrix<Type_>::Matrix(std::vector<Type_>& vec_n) :Matrix_row(1), Matrix_column(vec_n.size()) {
	Matrix_data = new Type_ * [Matrix_row];
	for (int i = 0; i < Matrix_row; i++)
	{
		Matrix_data[i] = new Type_[Matrix_column];
		memcpy(Matrix_data[i], vec_n.data(), sizeof(Type_) * vec_n.size());
	}
}

template<typename Type_>
Matrix<Type_>::~Matrix() {
	if (Matrix_data != nullptr) {
		for (int i = 0; i < Matrix_row; i++)
		{
			delete Matrix_data[i];
			Matrix_data[i] = nullptr;
		}
		delete Matrix_data;
		Matrix_data = nullptr;
	}
}

template<typename Type_>
void Matrix<Type_>::print() {
	for (int i = 0; i < Matrix_row; i++)
	{
		for (int j = 0; j < Matrix_column; j++)
		{
			printf("%3.4lf ", Matrix_data[i][j]);
		}
		printf("\n");
	}
}

template<typename Type_>
Matrix<Type_>& Matrix<Type_>::operator=(Matrix<Type_>&& old) {
	if (this->Matrix_data != nullptr) {
		for (int i = 0; i < Matrix_row; i++)
		{
			delete this->Matrix_data[i];
			this->Matrix_data[i] = nullptr;
		}
		delete Matrix_data;
		this->Matrix_data = nullptr;
	}
	this->Matrix_column = old.Matrix_column;
	this->Matrix_data = old.Matrix_data;
	this->Matrix_row = old.Matrix_row;
	old.Matrix_data = nullptr;
	return *this;
}

template<typename Type_>
Matrix<Type_>& Matrix<Type_>::operator=(const Matrix<Type_>& old) {
	if (this->Matrix_data != nullptr) {
		for (int i = 0; i < this->Matrix_row; i++)
		{
			delete this->Matrix_data[i];
			this->Matrix_data[i] = nullptr;
		}
		delete Matrix_data;
		this->Matrix_data = nullptr;
	}
	this->Matrix_column = old.Matrix_column;
	this->Matrix_row = old.Matrix_row;
	this->Matrix_data = new double* [this->Matrix_row];
	for (int i = 0; i < this->Matrix_row; i++)
	{
		Matrix_data[i] = new double[this->Matrix_column];
		memcpy(Matrix_data[i], old.Matrix_data[i], old.Matrix_column * sizeof(Type_));
	}
	return *this;
}

template<typename Type_>
Matrix<Type_>& Matrix<Type_>::operator=(std::vector<Type_>& vec_n) {
	this->Matrix_row = 1;
	this->Matrix_column = vec_n.size();
	Matrix_data = new Type_ * [Matrix_row];
	for (int i = 0; i < Matrix_row; i++)
	{
		Matrix_data[i] = new Type_[Matrix_column];
		memcpy(Matrix_data[i], vec_n.data(), sizeof(Type_) * vec_n.size());
	}
	return *this;
}

template<typename Type_>
Matrix<Type_> Matrix<Type_>::operator*(Matrix& mutipul) {

	Matrix putn(this->Matrix_row, mutipul.Matrix_column);
	for (int i = 0; i < this->Matrix_row; i++)
	{
		for (int j = 0; j < mutipul.Matrix_column; j++)
		{
			double in = 0;
			for (int ti = 0; ti < this->Matrix_column; ti++)
			{
				in += this->Matrix_data[i][ti] * mutipul.Matrix_data[ti][j];
			}
			putn.Matrix_data[i][j] = in;
		}
	}
	return putn;
}

template<typename Type_>
Matrix<Type_> Matrix<Type_>::operator*(Type_ num) {
	for (int i = 0; i < this->Matrix_row; i++)
	{
		for (int j = 0; j < this->Matrix_column; j++)
		{

			this->Matrix_data[i][j] *= num;
		}
	}
	return *this;
}

template<typename Type_>
Matrix<Type_> Matrix<Type_>::operator-(Matrix<Type_>& sub) {
	Matrix putn(sub.Matrix_row, sub.Matrix_column);
	for (int i = 0; i < sub.Matrix_row; i++)
	{
		for (int j = 0; j < sub.Matrix_column; j++)
		{
			putn.Matrix_data[i][j] = this->Matrix_data[i][j] - sub.Matrix_data[i][j];
		}
	}
	return putn;
}

template<typename Type_>
Matrix<Type_> Matrix<Type_>::operator+(Matrix<Type_>& add) {
	Matrix putn(add.Matrix_row, add.Matrix_column);
	for (int i = 0; i < add.Matrix_row; i++)
	{
		for (int j = 0; j < add.Matrix_column; j++)
		{
			putn.Matrix_data[i][j] = this->Matrix_data[i][j] + add.Matrix_data[i][j];
		}
	}
	return putn;
}

template<typename Type_>
void Matrix<Type_>::fun_handle(Type_(*fun)(Type_)) {
	for (int i = 0; i < Matrix_row; i++)
	{
		for (int j = 0; j < Matrix_column; j++)
		{
			Matrix_data[i][j] = fun(Matrix_data[i][j]);
		}
	}
}

template<typename Type_>
void Matrix<Type_>::rotate() {
	Type_** nn;
	nn = new Type_ * [Matrix_column];
	for (int i = 0; i < Matrix_column; i++)
	{
		nn[i] = new Type_[Matrix_row];
	}
	for (int i = 0; i < this->Matrix_row; i++)
	{
		for (int j = 0; j < this->Matrix_column; j++)
		{
			nn[j][i] = this->Matrix_data[i][j];
		}
	}
	for (int i = 0; i < Matrix_row; i++)
	{
		delete Matrix_data[i];
	}
	delete Matrix_data;
	Matrix_data = nn;
	int t = Matrix_column;
	Matrix_column = Matrix_row; Matrix_row = t;
}

template<typename Type_>
Matrix<Type_> Matrix<Type_>::hconcat(const Matrix& t) {
	Type_** nn;
	nn = new Type_ * [Matrix_row];
	for (int i = 0; i < Matrix_row; i++)
	{
		nn[i] = new Type_[Matrix_column+t.Matrix_column];
		memcpy(nn[i], Matrix_data[i], Matrix_column *sizeof(Type_));
		memcpy((Type_*)&nn[i][Matrix_column], t.Matrix_data[i], t.Matrix_column * sizeof(Type_));
	
	}
	Matrix<Type_> new_one(Matrix_row, Matrix_column + t.Matrix_column, nn);
	return new_one;
}

template<typename Type_>
Matrix<Type_> Matrix<Type_>::vconcat(const Matrix& t) {
	Type_** nn;
	nn = new Type_ * [Matrix_row+t.Matrix_row];
	for (int i = 0; i < Matrix_row; i++)
	{
		nn[i] = new Type_[Matrix_column];
		memcpy(nn[i], Matrix_data[i], Matrix_column * sizeof(Type_));
	}
	for (int i = 0; i < t.Matrix_row; i++)
	{
		nn[Matrix_row+i] = new Type_[t.Matrix_column];
		memcpy(nn[Matrix_row+i], t.Matrix_data[i], t.Matrix_column * sizeof(Type_));
	}
	Matrix<Type_> new_one(Matrix_row + t.Matrix_row, Matrix_column, nn);
	return new_one;
}

template<typename Type_>
inline Matrix<Type_> Matrix<Type_>::Diagonal() {
	Matrix<Type_> new_one;
	Type_** nn;
	if (this->Matrix_row == 1) {
		nn = new Type_ * [Matrix_column];
		for (int i = 0; i < Matrix_column; i++)
		{
			nn[i] = new Type_[Matrix_column];
			memset(nn[i], 0, Matrix_column * sizeof(Type_));
		}
		for (int i = 0; i < Matrix_column; i++)
		{
			nn[i][i] = this->Matrix_data[0][i];
		}
		new_one.Matrix_column = Matrix_column;
		new_one.Matrix_row = Matrix_column;
		new_one.Matrix_data = nn;
	}
	if (this->Matrix_column == 1) {
		nn = new Type_ * [Matrix_row];
		for (int i = 0; i < Matrix_row; i++)
		{
			nn[i] = new Type_[Matrix_row];
			memset(nn[i], 0, Matrix_row * sizeof(Type_));
		}
		for (int i = 0; i < Matrix_row; i++)
		{
			nn[i][i] = this->Matrix_data[i][0];
		}
		new_one.Matrix_column = Matrix_row;
		new_one.Matrix_row = Matrix_row;
		new_one.Matrix_data = nn;
	}
	return new_one;
}

template<typename Type_>
Type_* const* const  Matrix<Type_>::Get_Matrix_data(void) {
	return this->Matrix_data;
}

template<typename Type_>
int Matrix<Type_>::Get_Matrix_row(void) {
	return this->Matrix_row;
}

template<typename Type_>
int Matrix<Type_>::Get_Matrix_column(void) {
	return this->Matrix_column;
}

template<typename Type_>
std::ostream& operator<<(std::ostream& out, const Matrix<Type_>& t) {
	for (int i = 0; i < t.Matrix_row; i++)
	{
		for (int j = 0; j < t.Matrix_column; j++)
		{
			out << std::fixed << std::setprecision(5) << t.Matrix_data[i][j] << " ";
		}
		out << std::endl;
	}
	return out;
}


#endif 