#include <iostream>
#include "Matrix.h"
#include <chrono>
#include <ctime>
using namespace std::chrono;
using namespace std;
int main()
{
	int L = 300;
	int M = 200;
	int N = 300;
	setlocale(LC_ALL, "Russian");
	high_resolution_clock::time_point t1, t2;
	duration<double> time_span;
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

	Matrix** B = new Matrix* [M];
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
	cout << "С автоматической векторизацией ";
	t1 = high_resolution_clock::now();
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < M; k++)
			{
				for (int l = 0; l < 8; l++)
				{
					for (int n = 0; n < 8; n++)
					{
						C[i][j].getArr()[l][n] +=
							A[i][k].getArr()[l][0] * B[k][j].getArr()[0][n] +
							A[i][k].getArr()[l][1] * B[k][j].getArr()[1][n] +
							A[i][k].getArr()[l][2] * B[k][j].getArr()[2][n] +
							A[i][k].getArr()[l][3] * B[k][j].getArr()[3][n] +
							A[i][k].getArr()[l][4] * B[k][j].getArr()[4][n] +
							A[i][k].getArr()[l][5] * B[k][j].getArr()[5][n] +
							A[i][k].getArr()[l][6] * B[k][j].getArr()[6][n] +
							A[i][k].getArr()[l][7] * B[k][j].getArr()[7][n];
					}
				}


				/*Matrix result = A[i][k] * B[k][j];
				C[i][j] = result + C[i][j];*/
			}
		}
	}
	t2 = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(t2 - t1);
	cout << time_span.count() << endl;

	Matrix** D = new Matrix * [L];
	for (int i = 0; i < L; i++)
	{
		D[i] = new Matrix[N];
		for (int j = 0; j < N; j++)
		{
			Matrix c(8, 8);
			D[i][j] = c;
		}
	}
	cout << "С ручной векторизацией ";
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < M; j++)
		{
			for (int k = 0; k < N; k++)
			{
				Matrix result = Matrix::multiply(A[i][j], B[j][k]);
				D[i][k] = result + D[i][k];
			}
		}
	}
	t2 = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(t2 - t1);
	cout << time_span.count() << endl;
	bool flag;
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < N; j++)
		{

			if (C[i][j] == D[i][j]) flag = true;
			else
			{
				flag = false;
				break;
			}

		}
		if (!flag) break;
	}
	if (flag) cout << "Good";
	else cout << "Bad";
	return 0;
}