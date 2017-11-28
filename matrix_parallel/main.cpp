#include <iostream>
#include <cstdint>
#include "matrix.h"


int main()
{
	unsigned size = 100;
	unsigned topRandom = 100;

	const unsigned maxNumberOfThreads = 4;

	Matrix original(topRandom, size, size);
	Matrix mult(topRandom, size, size);

	unsigned numberOfTests = 1000;
	std::clock_t buffer = 0;

	for (unsigned numberOfThreads = 1; numberOfThreads <= maxNumberOfThreads; ++numberOfThreads) {

		std::clock_t result = 0;
		for (unsigned i = 0; i < numberOfTests; ++i) {
			original.vinogradParallel(mult, numberOfThreads, buffer);
			result += buffer;
		}
		std::cout << "NUMBER OF THREADS: " << numberOfThreads << std::endl << "TIME: " << result << std::endl;
	}

	return 0;
}
