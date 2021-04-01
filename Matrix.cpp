#include "Matrix.h"
#include <cfloat>
#include <stdlib.h>
#include <iostream>
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
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			arr[i][j] = (double)rand()/RAND_MAX * 100.;
		}
	}
}

void Matrix::print()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cout << arr[i][j] << endl;
		}
	}
}

Matrix Matrix::operator*(Matrix& A)
{
	Matrix C(A.rows, this->columns);
	for (int i = 0; i < A.rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			for (int k = 0; k < A.columns; k++)
			{
				C.arr[i][j] += A.arr[i][k] * this->arr[k][j];
			}
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