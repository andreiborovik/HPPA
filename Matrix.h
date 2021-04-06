#pragma once
class Matrix
{	
private:
	int rows;
	int columns;
	double** arr;

public:
	Matrix() = default;
	Matrix(int rows, int columns);
	Matrix(const Matrix&);
	~Matrix();


	void init();
	double** getArr() const
	{
		return this->arr;
	}
	void print();
	static Matrix multiply(Matrix& A, Matrix& B);
	Matrix operator * (Matrix& A);
	Matrix operator+ (Matrix& A);
	Matrix& operator=(const Matrix& A);
	bool operator ==(Matrix& A);
};