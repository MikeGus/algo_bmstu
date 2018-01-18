#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <stdint.h>

void bubbleSort(int*& array, unsigned size, clock_t& time) {

	clock_t tickStart = clock();
	bool swapped = true;
	unsigned iMaxPos = size - 1;
	unsigned jMaxPos = size - 1;
	for (unsigned i = 0; i < iMaxPos && swapped == true; ++i, --jMaxPos) {
		swapped = false;
		for (unsigned j = 0; j < jMaxPos; ++j) {
			if (array[j] > array[j + 1]) {
				std::swap(array[j], array[j + 1]);
				swapped = true;
			}
		}
	}
	time = clock() - tickStart;
}


void selectionSort(int*& array, unsigned arraySize, clock_t& time) {

	clock_t tickStart = clock();

	unsigned maxI = arraySize - 1;
	for (unsigned i = 0; i < maxI; ++i) {
		unsigned minIndex = i;
		for (unsigned j = i + 1; j < arraySize; ++j) {
			if (array[j] < array[minIndex]) {
				minIndex = j;
			}
		}
		std::swap(array[i], array[minIndex]);
	}

	time = clock() - tickStart;
}

void insertionSort(int*& array, unsigned arraySize, clock_t& time) {

	clock_t tickStart = clock();

	for (unsigned i = 1; i < arraySize; ++i) {
		int currentElement = array[i];
		bool first = true;
		for (unsigned j = i; j > 0; --j) {
			if (currentElement >= array[j - 1]) {
				memmove(array + j + 1, array + j, (i - j) * sizeof(int));
				array[j] = currentElement;
				first = false;
				break;
			}
		}
		if (first) {
			memmove(array + 1, array, i * sizeof(int));
			array[0] = currentElement;
		}
	}

	time = clock() - tickStart;
}


void printArray(int* array, unsigned size) {
	for (unsigned i = 0; i < size; ++i) {
		std::cout << array[i] << " ";
	}
	std::cout << std::endl;
}

void fillArray(int*& array, unsigned arraySize, int key = 0) {
	if (key == -1) {
		int element = arraySize;
		for (unsigned i = 0; i < arraySize; ++i, --element) {
			array[i] = element;
		}
	}
	else if (key == 1) {
		for (unsigned i = 0; i < arraySize; ++i) {
			array[i] = i;
		}
	}
	else {
		srand(time(NULL));
		for (unsigned i = 0; i < arraySize; ++i){
			array[i] = rand() % arraySize;
		}
	}
}

int* makeArray(unsigned arraySize, int key = 0) {
	int* array = new int[arraySize];
	fillArray(array, arraySize, key);
	return array;
}


void testAlgorithm(unsigned arraySize, void (*func)(int*&, unsigned, clock_t&), unsigned numberOfTests = 10) {

	clock_t sortTimeBest = 0;
	clock_t sortTimeWorst = 0;
	clock_t sortTimeRandom = 0;

	int* arrayBest = makeArray(arraySize, 1);
	int* arrayWorst = makeArray(arraySize, -1);
	int* arrayRandom = makeArray(arraySize);

	clock_t bufferTime;
	for (unsigned i = 0; i < numberOfTests; ++i){
		func(arrayBest, arraySize, bufferTime);
		sortTimeBest += bufferTime;
		func(arrayWorst, arraySize, bufferTime);
		sortTimeWorst += bufferTime;
		func(arrayRandom, arraySize, bufferTime);
		sortTimeRandom += bufferTime;

		fillArray(arrayBest, arraySize, 1);
		fillArray(arrayWorst, arraySize, -1);
		fillArray(arrayRandom, arraySize, 0);
	}

	std::cout << "BEST:   " << sortTimeBest << std::endl;
	std::cout << "WORST:  " << sortTimeWorst << std::endl;
	std::cout << "RANDOM: " << sortTimeRandom << std::endl;

	delete[] arrayBest;
	delete[] arrayWorst;
	delete[] arrayRandom;
}

int main()
{

	unsigned numberOfTests = 100;
	std::vector<unsigned> sizes = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
	for (unsigned arraySize : sizes) {
		std::cout << "\narraySize: " << arraySize << std::endl;

		std::cout << "Selection sort:" << std::endl;
		testAlgorithm(arraySize, selectionSort, numberOfTests);

		std::cout << "\nInsertion sort:" << std::endl;
		testAlgorithm(arraySize, insertionSort, numberOfTests);

		std::cout << "\nBubble sort:" << std::endl;
		testAlgorithm(arraySize, bubbleSort, numberOfTests);
	}

	return 0;
}
