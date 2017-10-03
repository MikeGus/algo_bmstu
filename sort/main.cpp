#include <iostream>
#include <algorithm>
#include <ctime>
#include <stdint.h>

void bubbleSort(int*& array, unsigned size, unsigned long long& time) {

	unsigned long long tickStart = __rdtsc();
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
	time = __rdtsc() - tickStart;
}


void selectionSort(int*& array, unsigned arraySize, unsigned long long& time) {

	unsigned long long tickStart = __rdtsc();

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

	time = __rdtsc() - tickStart;
}

void insertionSort(int*& array, unsigned arraySize, unsigned long long& time) {

	unsigned long long tickStart = __rdtsc();

	for (unsigned i = 1; i < arraySize; ++i) {
		int currentElement = array[i];
		for (unsigned j = 0; j < i; ++j) {
			if (currentElement < array[j]) {
				memmove(array + j + 1, array + j, (i - j) * sizeof(int));
				array[j] = currentElement;
				break;
			}
		}
	}

	time = __rdtsc() - tickStart;
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


void testAlgorithm(unsigned arraySize, void (*func)(int*&, unsigned, unsigned long long&), unsigned numberOfTests = 10) {

	unsigned long long sortTimeBest = 0;
	unsigned long long sortTimeWorst = 0;
	unsigned long long sortTimeRandom = 0;

	int* arrayBest = makeArray(arraySize, 1);
	int* arrayWorst = makeArray(arraySize, -1);
	int* arrayRandom = makeArray(arraySize);

	unsigned long long bufferTime = 0;
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

	sortTimeBest /= numberOfTests;
	sortTimeWorst /= numberOfTests;
	sortTimeRandom /= numberOfTests;

	std::cout << "BEST:   " << sortTimeBest << std::endl;
	std::cout << "WORST:  " << sortTimeWorst << std::endl;
	std::cout << "RANDOM: " << sortTimeRandom << std::endl;

	delete[] arrayBest;
	delete[] arrayWorst;
	delete[] arrayRandom;
}

int main()
{
	unsigned arraySize = 1000;
	unsigned numberOfTests = 100;

	int* ar = makeArray(10, 1);
	printArray(ar, 10);
	unsigned long long a;
	insertionSort(ar, 10, a);
	printArray(ar, 10);
	delete[] ar;


	std::cout << "\nSelection sort:" << std::endl;
	testAlgorithm(arraySize, selectionSort, numberOfTests);

	std::cout << "\nInsertion sort:" << std::endl;
	testAlgorithm(arraySize, insertionSort, numberOfTests);

	std::cout << "\nBubble sort:" << std::endl;
	testAlgorithm(arraySize, bubbleSort, numberOfTests);

	return 0;
}
