#include <iostream>
#include "Matrix.h"
using namespace std;
int main()
{
	int L = 1;
	int M = 2;
	int N = 3;
	Matrix** A = new Matrix* [L];
	for (int i = 0; i < L; i++)
	{
		A[i] = new Matrix[M];
		for (int j = 0; j < M; j++)
		{
			Matrix a(8, 8);
			a.init();
			A[i][j] = a;
		}
	}

	Matrix** B = new Matrix * [M];
	for (int i = 0; i < M; i++)
	{
		B[i] = new Matrix[N];
		for (int j = 0; j < N; j++)
		{
			Matrix b(8, 8);
			b.init();
			B[i][j] = b;
		}
	}

	Matrix** C = new Matrix * [L];
	for (int i = 0; i < L; i++)
	{
		C[i] = new Matrix[N];
		for (int j = 0; j < N; j++)
		{
			Matrix c(8, 8);
			C[i][j] = c;
		}
	}
//#pragma loop( no_vector )
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < M; k++)
			{
				Matrix result = A[i][k] * B[k][j];
				C[i][k] = result + C[i][k];
			}
		}
	}
	return 0;
}