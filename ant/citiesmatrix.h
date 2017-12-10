#ifndef CITIESMATRIX_H
#define CITIESMATRIX_H

#include <iostream>

class CitiesMatrix
{
	public:
		CitiesMatrix(const unsigned citiesNumber);
		CitiesMatrix(const unsigned citiesNumber, const unsigned minDistance, const unsigned maxDistance);
		~CitiesMatrix();

		CitiesMatrix operator +(const CitiesMatrix& other) const;
		CitiesMatrix& operator +=(const CitiesMatrix& other);

		CitiesMatrix operator +(const double value) const;
		CitiesMatrix& operator +=(const double value);

		CitiesMatrix operator *(const double value) const;
		CitiesMatrix& operator *=(const double value);

		unsigned getCitiesNumber() const;
		unsigned getValue(const unsigned first, const unsigned second) const;

		void setValue(const unsigned first, const unsigned second, const unsigned value);

		friend std::ostream& operator <<(std::ostream& stream, const CitiesMatrix& matrix);

	private:
		const unsigned citiesNumber;
		unsigned** matrix;
};



#endif // CITIESMATRIX_H
