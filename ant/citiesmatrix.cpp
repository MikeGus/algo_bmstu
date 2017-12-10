#include "citiesmatrix.h"
#include <random>
#include <ctime>
#include <stdexcept>


CitiesMatrix::CitiesMatrix(const unsigned citiesNumber): citiesNumber(citiesNumber) {
	matrix = new unsigned*[citiesNumber];
	for (unsigned i = 0; i < citiesNumber; ++i) {
		matrix[i] = new unsigned[citiesNumber]();
	}
}

CitiesMatrix::CitiesMatrix(const unsigned citiesNumber, const unsigned minDistance, const unsigned maxDistance):
	citiesNumber(citiesNumber) {

	srand(time(nullptr));

	matrix = new unsigned*[citiesNumber];
	for (unsigned i = 0; i < citiesNumber; ++i) {
		matrix[i] = new unsigned[citiesNumber]();
	}

	const unsigned diversity = maxDistance - minDistance;
	for (unsigned i = 0; i < citiesNumber; ++i) {
		for (unsigned j = i + 1; j < citiesNumber; ++j) {
			matrix[i][j] = matrix[j][i] = minDistance + rand() % diversity;
		}
	}
}

CitiesMatrix::~CitiesMatrix() {
	for (unsigned i = 0; i < citiesNumber; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;
 }

unsigned CitiesMatrix::getCitiesNumber() const {
	return citiesNumber;
}

unsigned CitiesMatrix::getValue(const unsigned first, const unsigned second) const {
	if (first >= citiesNumber || second >= citiesNumber) {
		throw std::logic_error("Invalid city index!");
	}
	return matrix[first][second];
}

void CitiesMatrix::setValue(const unsigned first, const unsigned second, const unsigned value) {
	matrix[first][second] = matrix[second][first] = value;
}

CitiesMatrix CitiesMatrix::operator+(const CitiesMatrix& other) const {
	CitiesMatrix result(citiesNumber);
	for (unsigned i = 0; i < citiesNumber; ++i) {
		for (unsigned j = 0; j < citiesNumber; ++j) {
			result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
		}
	}
	return result;
}

CitiesMatrix& CitiesMatrix::operator +=(const CitiesMatrix& other) {
	for (unsigned i = 0; i < citiesNumber; ++i) {
		for (unsigned j = 0; j < citiesNumber; ++j) {
			matrix[i][j] += other.matrix[i][j];
		}
	}

	return *this;
}

CitiesMatrix CitiesMatrix::operator +(const double value) const {
	CitiesMatrix result(citiesNumber);
	for (unsigned i = 0; i < citiesNumber; ++i) {
		for (unsigned j = 0; j < citiesNumber; ++j) {
			result.matrix[i][j] = matrix[i][j] + value;
		}
	}

	return *this;
}
CitiesMatrix& CitiesMatrix::operator +=(const double value) {
	for (unsigned i = 0; i < citiesNumber; ++i) {
		for (unsigned j = 0; j < citiesNumber; ++j) {
			matrix[i][j] += value;
		}
	}

	return *this;
}

CitiesMatrix CitiesMatrix::operator *(const double value) const {
	CitiesMatrix result(citiesNumber);
	for (unsigned i = 0; i < citiesNumber; ++i) {
		for (unsigned j = 0; j < citiesNumber; ++j) {
			result.matrix[i][j] = matrix[i][j] * value;
		}
	}
	return result;
}

CitiesMatrix& CitiesMatrix::operator *=(const double value) {
	for (unsigned i = 0; i < citiesNumber; ++i) {
		for (unsigned j = 0; j < citiesNumber; ++j) {
			matrix[i][j] *= value;
		}
	}
	return *this;
}

std::ostream& operator <<(std::ostream& stream, const CitiesMatrix& matrix) {
	for (unsigned i = 0; i < matrix.citiesNumber; ++i) {
		for (unsigned j = 0; j < matrix.citiesNumber; ++j) {
			stream << matrix.matrix[i][j] << " ";
		}
		stream << std::endl;
	}

	return stream;
}
