#include "Matrix.h"
#include <cfloat>
#include <stdlib.h>
#include <iostream>
#include <immintrin.h>
#include <random>
using namespace std;

Matrix::Matrix(int rows, int columns)
{
		this->rows = rows;
		this->columns = columns;
		arr = new double* [rows];
		for (int i = 0; i < rows; i++)
		{
			arr[i] = new double [columns];
			for (int j = 0; j < columns; j++)
			{
				arr[i][j] = 0;
			}
		}
}
Matrix::Matrix(const Matrix& A) {
	this->columns = A.columns;
	this->rows = A.rows;
	arr = new double* [A.rows];
	for (int i = 0; i < A.rows; i++) {
		arr[i] = new double[A.columns];
		for (int j = 0; j < A.columns; j++) {
			arr[i][j] = A.getArr()[i][j];
		}
	}
}

Matrix::~Matrix()
{
	for (int i = 0; i < rows; i++)
	{
		delete[] arr[i];
	}
	delete arr;
}

void Matrix::init()
{
	int k = 2;
	for (int i = 0; i < rows; i++)
	{
		//if (i % 2 == 0) k = 1;
		for (int j = 0; j < columns; j++)
		{
			random_device rd;
			default_random_engine eng(rd());
			uniform_real_distribution<double> distr(1, 10);
			arr[i][j] = distr(eng);
			//k++;
		}
	}
}

void Matrix::print()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

void Matrix::add(Matrix& m)
{
	auto data1 = m.getArr();
	for (int i = 0; i < 8; ++i) {
		auto row1 = data1[i];
#pragma loop(no_vector) 
		for (int j = 0; j < 8; ++j) {
			arr[i][j] += row1[j];
		}
	}
}

Matrix Matrix::multiply(Matrix& A, Matrix& B)
{
	__m256d a, b, c;
	Matrix C(8, 8);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			a = _mm256_set1_pd(A.arr[i][j]);
			for (int k = 0; k < 8; k+=4)
			{
				c = _mm256_load_pd(&C.arr[i][k]);
				b = _mm256_load_pd(&B.arr[j][k]);
				c = _mm256_fmadd_pd(a, b, c);
				_mm256_store_pd(&C.arr[i][k], c);
				//C.arr[i][j] += A.arr[i][k] * this->arr[k][j];
			}
		}
	}
	return C;
}

Matrix Matrix::operator*(Matrix& A)
{
	Matrix C(8, 8);
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			
				C.arr[i][j] += A.arr[i][0] * this->arr[0][j]
					+ A.arr[i][1] * this->arr[1][j]
					+ A.arr[i][2] * this->arr[2][j]
					+ A.arr[i][3] * this->arr[3][j]
					+ A.arr[i][4] * this->arr[4][j]
					+ A.arr[i][5] * this->arr[5][j]
					+ A.arr[i][6] * this->arr[6][j]
					+ A.arr[i][7] * this->arr[7][j];
		}
	}
	return C;
}

Matrix Matrix::operator+(Matrix& A)
{
	double** resultData = new double* [this->rows];
		for (int i = 0; i < this->rows; i++)
		{
			resultData[i] = new double[this->columns];
			for (int j = 0; j < this->columns; j++)
			{
				resultData[i][j] = 0;
			}
		}
	double** matrix1 = A.getArr();
	double** matrix2 = arr;
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->columns; j++) {
			resultData[i][j] = matrix1[i][j] + matrix2[i][j];
		}
	}

	Matrix result(this->rows, this->columns);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			result.arr[i][j] = resultData[i][j];
		}
	}
	return result;
}

Matrix& Matrix::operator=(const Matrix& A)
{
	this->columns = A.columns;
	this->rows = A.rows;
	this->arr = new double* [rows];
	for (int i = 0; i < rows; i++) {
		this->arr[i] = new double[columns];
		for (int j = 0; j < columns; j++) {
			this->arr[i][j] = A.getArr()[i][j];
		}
	}
	return *this;
}

bool Matrix::operator==(Matrix& A)
{
	for (int i = 0; i < A.rows; i++)
	{
		for (int j = 0; j < A.columns; j++)
		{
			if ((this->arr[i][j] - A.arr[i][j]) > 0.1) return false;
		}
	}
	return true;
}
