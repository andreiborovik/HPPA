#include <iostream>
#include <random>
#include <chrono>
#include <ctime>
#include <immintrin.h>
using namespace std;
using namespace std::chrono;

int L = 300;
int M = 400;
int N = 300;

double**** init(int rows, int columns, bool flag)
{
	double**** ptr;
	ptr = new double***[rows];
	for (int i = 0; i < rows; i++)
	{
		ptr[i] = new double** [columns];
		for (int j = 0; j < columns; j++)
		{
			ptr[i][j] = new double* [8];
				for (int k = 0; k < 8; k++)
				{
					ptr[i][j][k] = new double[8];
					for (int m = 0; m < 8; m++)
					{
						if (flag)
						{
							random_device rd;
							default_random_engine eng(rd());
							uniform_real_distribution<double> distr(1, 10);
							ptr[i][j][k][m] = distr(eng);
						}
						else ptr[i][j][k][m] = 0;
					}
				}

		}
	}
	return ptr;
}
void my_multiply(double**** A, double**** B, double**** C)
{
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
						C[i][j][l][n] +=
							A[i][k][l][0] * B[k][j][0][n] +
							A[i][k][l][1] * B[k][j][1][n] +
							A[i][k][l][2] * B[k][j][2][n] +
							A[i][k][l][3] * B[k][j][3][n] +
							A[i][k][l][4] * B[k][j][4][n] +
							A[i][k][l][5] * B[k][j][5][n] +
							A[i][k][l][6] * B[k][j][6][n] +
							A[i][k][l][7] * B[k][j][7][n];
					}
				}
			}
		}
	}
}
void my_multiply_without_optimization(double**** A, double**** B, double**** C)
{
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < M; k++)
			{
				for (int l = 0; l < 8; l++)
				{
#pragma loop(no_vector)
					for (int n = 0; n < 8; n++)
					{
						C[i][j][l][n] +=
							A[i][k][l][0] * B[k][j][0][n] +
							A[i][k][l][1] * B[k][j][1][n] +
							A[i][k][l][2] * B[k][j][2][n] +
							A[i][k][l][3] * B[k][j][3][n] +
							A[i][k][l][4] * B[k][j][4][n] +
							A[i][k][l][5] * B[k][j][5][n] +
							A[i][k][l][6] * B[k][j][6][n] +
							A[i][k][l][7] * B[k][j][7][n];
					}
				}
			}
		}
	}
}

void multiply(double** a, double** b, double** c)
{
	__m256d a1, b1, c1;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			a1 = _mm256_set1_pd(a[i][j]);
			for (int k = 0; k < 8; k += 4)
			{
				c1 = _mm256_load_pd(&c[i][k]);
				b1 = _mm256_load_pd(&b[j][k]);
				c1 = _mm256_fmadd_pd(a1, b1, c1);
				_mm256_store_pd(&c[i][k], c1);
			}
		}
	}
}

bool my_equal(double**** A, double**** B)
{
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				for (int m = 0; m < 8; m++)
				{
					if ((A[i][j][k][m] - B[i][j][k][m]) > 0.1) return false;
				}
			}
			
		}
	}
	return true;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	high_resolution_clock::time_point t1, t2;
	duration<double> time_span;

	double**** A;
	double**** B;
	double**** C;
	double**** D;
	double**** E;

	A = init(L, M, true);
	B = init(M, N, true);
	C = init(L, N, false);
	D = init(L, N, false);
	E = init(L, N, false);

	cout << "С автоматической векторизацией ";
	t1 = high_resolution_clock::now();
	my_multiply(A, B, C);
	t2 = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(t2 - t1);
	cout << time_span.count() << endl;

	cout << "Без векторизацией ";
	t1 = high_resolution_clock::now();
	my_multiply_without_optimization(A, B, E);
	t2 = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(t2 - t1);
	cout << time_span.count() << endl;

	cout << "С ручной векторизацией ";
	t1 = high_resolution_clock::now();
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < M; k++) {
				multiply(A[i][k], B[k][j], D[i][j]);
			}
		}
	}
	t2 = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(t2 - t1);
	cout << time_span.count() << endl;

	if (my_equal(C,D)) cout << "Good";
	else cout << "Bad";
	if (my_equal(C, E)) cout << "Good";
	else cout << "Bad";
	if (my_equal(E, D)) cout << "Good";
	else cout << "Bad";
	return 0;
}