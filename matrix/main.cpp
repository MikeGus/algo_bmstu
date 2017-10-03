#include <iostream>
#include <exception>
#include <ctime>
#include <random>
#include <stdint.h>

class Matrix {
	public:
		Matrix(unsigned rows = 1, unsigned columns = 1);
		Matrix(int randMax, unsigned rows = 1, unsigned columns = 1);
		Matrix(const Matrix& other);
		~Matrix();

		Matrix classicMult(const Matrix& other, unsigned long long& time);
		Matrix vinogradMult(const Matrix& other, unsigned long long& time);
//		Matrix vinogradImprMult(const Matrix& other, unsigned long long& time);

	private:
		unsigned rows;
		unsigned columns;
		unsigned** data;
};

Matrix::Matrix(int randMax, unsigned rows, unsigned columns): rows(rows), columns(columns) {
	if (rows == 0 || columns == 0) {
		throw std::logic_error("Wrong parameters passed to constuctor!");
	}

	data = new unsigned*[rows];
	for (unsigned i = 0; i < rows; ++i) {
		data[i] = new unsigned[columns];
	}
	srand(time(nullptr));

	for (unsigned i = 0; i < rows; ++i) {
		for (unsigned j = 0; j < columns; ++j) {
			data[i][j] = rand() % randMax;
		}
	}
}

Matrix::Matrix(unsigned rows, unsigned columns): rows(rows), columns(columns) {
	if (rows == 0 || columns == 0) {
		throw std::logic_error("Wrong parameters passed to constuctor!");
	}

	data = new unsigned*[rows];
	for (unsigned i = 0; i < rows; ++i) {
		data[i] = new unsigned[columns];
	}
}

Matrix::Matrix(const Matrix& other): rows(other.rows), columns(other.columns) {

	data = new unsigned*[rows];
	for (unsigned i = 0; i < rows; ++i) {
		data[i] = new unsigned[columns];
	}

	for (unsigned i = 0; i < rows; ++i) {
		for (unsigned j = 0; j < rows; ++j) {
			data[i][j] = other.data[i][j];
		}
	}
}

Matrix::~Matrix() {

	for (unsigned i = 0; i < rows; ++i) {
		delete[] data[i];
	}
	delete[] data;
}

Matrix Matrix::classicMult(const Matrix &other, unsigned long long &time) {
	if (columns != other.rows) {
		throw std::logic_error("Sizes don't match!");
	}
	Matrix result(rows, other.columns);

	unsigned long long tickStart = __rdtsc();
	for (unsigned i = 0; i < rows; ++i) {
		for (unsigned j = 0; j < columns; ++j) {
			unsigned buffer = 0;
			for (unsigned index = 0; index < columns; ++index) {
				buffer += data[i][index] * other.data[index][j];
			}
			result.data[i][j] = buffer;
		}
	}
	time = __rdtsc() - tickStart;

	return result;
}

Matrix Matrix::vinogradMult(const Matrix &other, unsigned long long &time) {
	if (columns != other.rows) {
		throw std::logic_error("Sizes don't match!");
	}
	Matrix result(rows, other.columns);
	
	unsigned* rowFactor = new unsigned[rows];
	unsigned* columnFactor = new unsigned[columns];

	unsigned long long tickStart = __rdtsc();

	for (unsigned i = 0; i < rows; ++i) {
		rowFactor[i] = data[i][0] * data[i][1];
		unsigned buffer = rowFactor[i];
		for (unsigned j = 2; j < columns - 1; j += 2) {
			buffer += data[i][j] * data[i][j + 1];
		}
		rowFactor[i] += buffer;
	}

	for (unsigned i = 0; i < other.columns; ++i) {
		columnFactor[i] = data[0][i] * data[1][i];
		unsigned buffer = columnFactor[i];
		for (unsigned j = 2; j < other.rows - 1; j += 2) {
			buffer += data[j][i] * data[j + 1][i];
		}
		columnFactor[i] = buffer;
	}

	for (unsigned i = 0; i < rows; ++i) {
		for (unsigned j = 0; j < other.columns; ++j) {
			unsigned buffer = 0;
			for (unsigned k = 0; k < columns - 1; k += 2) {
				buffer += (data[i][k] + other.data[k + 1][j]) *
						(data[i][k + 1] + other.data[k][j]);
			}
			result.data[i][j] = buffer;
			result.data[i][j] -= (rowFactor[i] + columnFactor[j]);
		}
	}

	if (columns % 2 != 0) {
		unsigned columnsDiv2 = columns / 2;
		for (unsigned i = 0; i < rows; ++i) {
			for (unsigned j = 0; j < columns; ++j) {
				result.data[i][j] += data[i][columnsDiv2] * other.data[columnsDiv2][j];
			}
		}
	}

	time = __rdtsc() - tickStart;

	delete[] columnFactor;
	delete[] rowFactor;

	return result;
}

int main()
{
	Matrix original(50, 1000, 1000);

	Matrix mult(50, 1000, 1000);

	unsigned long long timeClassic = 0;
	unsigned long long timeVinograd = 0;

	std::cout << "Counting classic...";
	original.classicMult(mult, timeClassic);
	std::cout << "  finished in " << timeClassic << " ticks!" << std::endl;

	std::cout << "Counting vinograd...";
	original.vinogradMult(mult, timeVinograd);
	std::cout << "  finished in " << timeVinograd << " ticks!" << std::endl;

	if (timeClassic < timeVinograd) {
		std::cout << "Classic win by " << (timeVinograd - timeClassic) * 100 / timeVinograd << "%" << std::endl;
	}
	else {
		std::cout << "Vinograd win by " << (timeClassic - timeVinograd) * 100 / timeClassic << "%" << std::endl;
	}

	return 0;
}
