#include <iostream>
#include <windows.h>
#include <algorithm>
#include <stdint.h>
#include <ctime>


int levensteinRecursive3(char* strOrig, char* strRes) {

	unsigned lenOrig = strlen(strOrig);
	if (lenOrig == 0) {
		return strlen(strRes);
	}
	unsigned lenRes = strlen(strRes);
	if (lenRes == 0) {
		return lenOrig;
	}

	bool symbNotSame = false;
	if (*strOrig != *strRes) {
		symbNotSame = true;
	}

	int result = std::min(std::min(levensteinRecursive3(strOrig + 1, strRes) + 1,
						 levensteinRecursive3(strOrig, strRes + 1) + 1),
					 levensteinRecursive3(strOrig + 1, strRes + 1) + symbNotSame);

	return result;
}


int levensteinIterative3(char* strOrig, char* strRes) {

	unsigned lenOrig = strlen(strOrig);
	if (lenOrig == 0) {
		return strlen(strRes);
	}
	unsigned lenRes = strlen(strRes);
	if (lenRes == 0) {
		return lenOrig;
	}

	unsigned columns = lenRes + 1;
	unsigned rows = lenOrig + 1;

	int* storage[2];
	storage[0] = new int[columns];
	storage[1] = new int[columns];

	for (unsigned i = 0; i < columns; ++i) {
		storage[0][i] = i;
	}

	for (unsigned i = 1; i < rows; ++i) {
		storage[1][0] = storage[0][0] + 1;
		for (unsigned j = 1; j < columns; ++j) {
			bool symbNotSame = false;
			if (strRes[j - 1] != strOrig[i - 1]) {
				symbNotSame = true;
			}

			storage[1][j] = std::min(std::min(storage[0][j] + 1,
							storage[1][j - 1] + 1),
							storage[0][j - 1] + symbNotSame);
		}
		std::swap(storage[0], storage[1]);
	}

	int result = storage[0][columns - 1];
	delete storage[0];
	delete storage[1];

	return result;
}


int levensteinIterative4(char* strOrig, char* strRes) {

	unsigned lenOrig = strlen(strOrig);
	unsigned lenRes = strlen(strRes);

	if (lenOrig < 2 || lenRes < 2) {
		return levensteinIterative3(strOrig, strRes);
	}

	unsigned columns = lenRes + 1;
	unsigned rows = lenOrig + 1;

	int* storage[3];
	storage[0] = new int[columns];
	storage[1] = new int[columns];
	storage[2] = new int[columns];

	for (unsigned i = 0; i < columns; ++i) {
		storage[0][i] = i;
	}

	storage[1][0] = 1;
	for (unsigned i = 1; i < columns; ++i) {
		bool symbNotSame = false;
		if (strRes[i - 1] != strOrig[0]) {
			symbNotSame = true;
		}

		storage[1][i] = std::min(std::min(storage[0][i] + 1, storage[1][i - 1] + 1),
				storage[0][i - 1] + symbNotSame);
	}

	for (unsigned i = 2; i < rows; ++i) {
		storage[2][0] = storage[1][0] + 1;
		bool symbNotSame = false;
		if (strRes[0] != strOrig[i - 1]) {
			symbNotSame = true;
		}

		storage[2][1] = std::min(std::min(storage[1][1] + 1, storage[2][0] + 1),
				storage[1][0] + symbNotSame);

		for (unsigned j = 2; j < columns; ++j) {
			bool symbNotSame = false;
			if (strRes[j - 1] != strOrig[i - 1]) {
				symbNotSame = true;
			}

			storage[2][j] = std::min(std::min(std::min(storage[1][j] + 1, storage[2][j - 1] + 1),
					storage[1][j - 1] + symbNotSame), storage[0][j - 2] + 1);
		}

		std::swap(storage[0], storage[1]);
		std::swap(storage[1], storage[2]);
	}

	int result = storage[1][columns - 1];
	delete storage[0];
	delete storage[1];
	delete storage[2];

	return result;
}

void testAlgo(int (*algo)(char*, char*), unsigned numberOfTests, char* str1, char* str2) {
	for (unsigned i = 0; i < numberOfTests; ++i) {
		algo(str1, str2);
	}
}

char* generateRandomString(unsigned size, unsigned numberOfSymbols) {

	char* result = new char[size + 1];

	for (unsigned i = 0; i < size; ++i) {
		result[i] = (char) (65 + rand() % numberOfSymbols);
	}
	result[size] = '\0';

	return result;
}


int main()
{

	srand(time(NULL));

	unsigned size1 = 100;
	unsigned size2 = 100;

	unsigned numberOfSymbols = 2;

	char* str1 = generateRandomString(size1, numberOfSymbols);
	char* str2 = generateRandomString(size2, numberOfSymbols);

//	std::cout << str1 << std::endl << str2;

	unsigned numberOfTests = 100;

	std::clock_t beginTime = std::clock();
//	testAlgo(levensteinRecursive3, numberOfTests, str1, str2);
//	std::clock_t recursiveTime = std::clock() - beginTime;

	beginTime = std::clock();
	testAlgo(levensteinIterative3, numberOfTests, str1, str2);
	std::clock_t iterativeTime = std::clock() - beginTime;

	beginTime = std::clock();
	testAlgo(levensteinIterative4, numberOfTests, str1, str2);
	std::clock_t iterative4Time = std::clock() - beginTime;

//	int resultRec = levensteinRecursive3(str1, str2);
	int resultIt = levensteinIterative3(str1, str2);
	int resultIt4 = levensteinIterative4(str1, str2);

	delete[] str1;
	delete[] str2;

//	std::cout << "recursive:\n" << resultRec << std::endl << recursiveTime << std::endl;
	std::cout << "iterative:\n" << resultIt << std::endl << iterativeTime << std::endl;
	std::cout << "iterative4:\n" << resultIt4 << std::endl << iterative4Time << std::endl;

	return 0;
}
