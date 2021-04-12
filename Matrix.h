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
	double** getData() 
	{
		return this->arr;
	}
	void print();
	void add(Matrix& m);
	//static Matrix multiply(Matrix& A, Matrix& B);
	static void multiply(double** a, double** b, double** c);
	Matrix operator * (Matrix& A);
	Matrix operator+ (Matrix& A);
	Matrix& operator=(const Matrix& A);
	bool operator ==(Matrix& A);
};