#include <iostream>
#include <windows.h>
#include <algorithm>
#include <stdint.h>
#include <ctime>


int levensteinRecursive3(char* strOrig, char* strRes, unsigned long long& time) {

	unsigned long long tickStart = __rdtsc();

	unsigned lenOrig = strlen(strOrig);
	if (lenOrig == 0) {
		time = __rdtsc() - tickStart;
		return strlen(strRes);
	}
	unsigned lenRes = strlen(strRes);
	if (lenRes == 0) {
		time = __rdtsc() - tickStart;
		return lenOrig;
	}

	bool symbNotSame = false;
	if (*strOrig != *strRes) {
		symbNotSame = true;
	}

	int result = std::min(std::min(levensteinRecursive3(strOrig + 1, strRes, time) + 1,
						 levensteinRecursive3(strOrig, strRes + 1, time) + 1),
					 levensteinRecursive3(strOrig + 1, strRes + 1, time) + symbNotSame);

	time = __rdtsc() - tickStart;

	return result;
}


int levensteinIterative3(char* strOrig, char* strRes, unsigned long long& time) {

	unsigned long long tickStart = __rdtsc();

	unsigned lenOrig = strlen(strOrig);
	if (lenOrig == 0) {
		time = __rdtsc() - tickStart;
		return strlen(strRes);
	}
	unsigned lenRes = strlen(strRes);
	if (lenRes == 0) {
		time = __rdtsc() - tickStart;
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
	time = __rdtsc() - tickStart;

	int result = storage[0][columns - 1];
	delete storage[0];
	delete storage[1];

	return result;
}


int levensteinIterative4(char* strOrig, char* strRes, unsigned long long& time) {

	unsigned lenOrig = strlen(strOrig);
	unsigned lenRes = strlen(strRes);

	if (lenOrig < 2 || lenRes < 2) {
		return levensteinIterative3(strOrig, strRes, time);
	}

	unsigned long long tickStart = __rdtsc();

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

	time = __rdtsc() - tickStart;

	int result = storage[1][columns - 1];
	delete storage[0];
	delete storage[1];
	delete storage[2];

	return result;
}


int main()
{
	char str1[] = "abcd";
	char str2[] = "badc";

	int numberOfTests = 10;

	unsigned long long recursiveTime = 0;
	unsigned long long iterativeTime = 0;
	unsigned long long iterative4Time = 0;
	unsigned long long bufferTime = 0;

	int resultRec = levensteinRecursive3(str1, str2, bufferTime);
	int resultIt = levensteinIterative3(str1, str2, bufferTime);
	int resultIt4 = levensteinIterative4(str1, str2, bufferTime);

	for (int i = 0; i < numberOfTests; ++i) {
		levensteinRecursive3(str1, str2, bufferTime);
		recursiveTime += bufferTime;
		levensteinIterative3(str1, str2, bufferTime);
		iterativeTime += bufferTime;
		levensteinIterative4(str1, str2, bufferTime);
		iterative4Time += bufferTime;
	}

	recursiveTime /= numberOfTests;
	iterativeTime /= numberOfTests;
	iterative4Time /= numberOfTests;

	std::cout << "recursive:\n" << resultRec << std::endl << recursiveTime << std::endl;
	std::cout << "iterative:\n" << resultIt << std::endl << iterativeTime << std::endl;
	std::cout << "iterative4:\n" << resultIt4 << std::endl << iterative4Time << std::endl;

	return 0;
}
