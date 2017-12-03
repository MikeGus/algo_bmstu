#include <iostream>
#include <thread>

class Matrix {
	public:
		Matrix(unsigned rows = 1, unsigned columns = 1);
		Matrix(int randMax, unsigned rows = 1, unsigned columns = 1);
		Matrix(const Matrix& other);
		~Matrix();


		Matrix vinogradMult(const Matrix& other, std::clock_t& time);
		Matrix vinogradMultParallel(const Matrix& other, std::clock_t& time);

	private:
		void column_factor(unsigned*& columnFactor);
		void row_factor(unsigned*& rowFactor);

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

void Matrix::row_factor(unsigned*& rowFactor) {
	rowFactor = new unsigned[rows];
	for (unsigned i = 0; i < rows; ++i) {
		rowFactor[i] = data[i][0] * data[i][1];
		unsigned buffer = rowFactor[i];
		for (unsigned j = 2; j < columns - 1; j += 2) {
			buffer += data[i][j] * data[i][j + 1];
		}
		rowFactor[i] += buffer;
	}
}

void Matrix::column_factor(unsigned*& columnFactor) {

	columnFactor = new unsigned[columns];

	for (unsigned i = 0; i < columns; ++i) {
		columnFactor[i] = data[0][i] * data[1][i];
		unsigned buffer = columnFactor[i];
		for (unsigned j = 2; j < rows - 1; j += 2) {
			buffer += data[j][i] * data[j + 1][i];
		}
		columnFactor[i] = buffer;
	}
}


Matrix Matrix::vinogradMult(const Matrix &other, std::clock_t& time) {
	if (columns != other.rows) {
		throw std::logic_error("Sizes don't match!");
	}
	Matrix result(rows, other.columns);


	std::clock_t start = std::clock();

	unsigned* rowFactor = nullptr;
	row_factor(rowFactor);
	unsigned* columnFactor = nullptr;
	column_factor(columnFactor);


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
			for (unsigned j = 0; j < other.columns; ++j) {
				result.data[i][j] += data[i][columnsDiv2] * other.data[columnsDiv2][j];
			}
		}
	}

	time = std::clock() - start;

	delete[] columnFactor;
	delete[] rowFactor;

	return result;
}


Matrix Matrix::vinogradMultParallel(const Matrix &other, std::clock_t& time) {
	if (columns != other.rows) {
		throw std::logic_error("Sizes don't match!");
	}
	Matrix result(rows, other.columns);
	std::clock_t start = std::clock();

	unsigned* rowFactor = nullptr;
	std::thread t1(&Matrix::row_factor, this, rowFactor);
	unsigned* columnFactor = nullptr;
	std::thread t2(&Matrix::column_factor, &other, columnFactor);

	t1.join();
	t2.join();

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
			for (unsigned j = 0; j < other.columns; ++j) {
				result.data[i][j] += data[i][columnsDiv2] * other.data[columnsDiv2][j];
			}
		}
	}

	time = std::clock() - start;

	delete[] columnFactor;
	delete[] rowFactor;

	return result;
}

int main()
{
	unsigned size = 500;
	unsigned topRandom = 1000;

	Matrix original(topRandom, size, size);
	Matrix mult(topRandom, size, size);

	std::clock_t timeVinograd = 0;
	std::clock_t timeVinogradParallel = 0;


	original.vinogradMult(mult, timeVinograd);
	original.vinogradMultParallel(mult, timeVinogradParallel);

	std::cout << timeVinograd << std::endl << timeVinogradParallel << std::endl;

	return 0;
}
