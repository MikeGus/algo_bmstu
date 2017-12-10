#include <iostream>
#include <exception>
#include <cmath>

const double acc = 1e-4;
const unsigned size = 4;

double gauss(unsigned m, double** A) {
	double result = 1;
	for (unsigned i = 0; i < m; ++i) {
//		обмен строк, если ячейка A[i][i] нулевая на ту, где ячейка на той же позиции ненулевая
		if (fabs(A[i][i]) < acc) {
			for (unsigned j = i + 1; j < m; ++j) {
				if (fabs(A[j][i]) > acc) {
					double* buffer = A[i];
					A[i] = A[j];
					A[j] = buffer;
				}
			}
		}
//		если обмен не произошел - определитель равен нулю
		if (fabs(A[i][i]) < acc) {
			return 0;
		}
//		вычисляем новую итерацию определителя
		result *= A[i][i];
//		приводим к треугольному виду
		for (unsigned j = i + 1; j < m; ++j) {
			if (fabs(A[j][i]) >= acc) {
				double q = A[j][i] / A[i][i];
				for (unsigned k = i; k < m; ++k) {
					A[j][k] -= A[i][k] * q;
				}
			}
		}
	}
	return result;
}


int main() {
	double** A = new double*[size];
	for (unsigned i = 0; i < size; ++i) {
		A[i] = new double[size];
	}

	double newA[][4] = {{0, 2, 4, 3},
					{1, 3, 7, 8},
					{5, 6, 3, 2},
					{0, 1, 1, 3}};

	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			A[i][j] = newA[i][j];
		}
	}

	std::cout << gauss(size, A);

	for (unsigned i = 0; i < size; ++i) {
		delete[] A[i];
	}
	delete[] A;

	return 0;
}
