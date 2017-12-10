#include <iostream>
#include <vector>
#include <cmath>
#include <list>
#include "citiesmatrix.h"


unsigned antColony(const unsigned citiesNumber, std::vector<unsigned>& route, const CitiesMatrix& distance,
				   const double alpha, const double beta, const unsigned elites, const double Q,
				   const double evaporation, const unsigned maxTime) {
	unsigned length = 0;
	route.clear();

	CitiesMatrix pheromones(citiesNumber);

	for (unsigned time = 0; time < maxTime; ++time) {
		CitiesMatrix antPheromones(citiesNumber);

		for (unsigned currentCity = 0; currentCity < citiesNumber; ++currentCity) {
			std::list<unsigned> unvisitedCities;
			for (unsigned j = 0; j < currentCity; ++j) {
				unvisitedCities.push_back(j);
			}
			for (unsigned j = currentCity + 1; j < citiesNumber; ++j) {
				unvisitedCities.push_back(j);
			}

			std::vector<unsigned> visitedCities;
			visitedCities.push_back(currentCity);
			unsigned bufferLength = 0;

			while (unvisitedCities.size() > 0) {
				unsigned nextCity = citiesNumber;
				double maxProbability = 0;

				for (unsigned city : unvisitedCities) {
					if (distance.getValue(currentCity, city) != 0) {
						double fullFactor = 0;
						for (unsigned j : unvisitedCities) {
							fullFactor += pow(pheromones.getValue(currentCity, j), alpha) +\
									pow(1.0 / distance.getValue(currentCity, j), beta);
						}

						double probability = (pow(pheromones.getValue(currentCity, city), alpha) +\
								pow(1.0 / distance.getValue(currentCity, city), beta)) / fullFactor;
						if (probability > maxProbability) {
							maxProbability = probability;
							nextCity = city;
						}
					} else {
						throw std::logic_error("Zero distance occured!");
					}
				}

				if (nextCity == citiesNumber) {
					throw std::logic_error("Ant is isolated!");
				}

				visitedCities.push_back(nextCity);
				unvisitedCities.remove(nextCity);

				bufferLength += distance.getValue(currentCity, nextCity);
			}
			unsigned newLength = bufferLength + distance.getValue(visitedCities.front(), visitedCities.back());
			if (newLength < length || length == 0) {
				length = newLength;
				route = visitedCities;
			}

			auto it = visitedCities.cbegin();
			for (auto it2 = it + 1; it2 != visitedCities.cend(); ++it, ++it2) {
				antPheromones.setValue(*it, *it2, antPheromones.getValue(*it, *it2) + Q / bufferLength);
			}
		}
		auto it = route.cbegin();
		for (auto it2 = it + 1; it2 != route.cend(); ++it, ++it2) {
			antPheromones.setValue(*it, *it2, antPheromones.getValue(*it, *it2) + Q * elites / length);
		}

		pheromones *= 1 - evaporation;
		pheromones += antPheromones;
	}

	return length;
}


int main() {
	const unsigned citiesNumber = 4;

	double alpha = 1;
	double beta = 1;
	const unsigned elites = 3;
	double evaporation = 0.5;
	double Q = 10;
	const unsigned maxTime =300;

	CitiesMatrix distance(citiesNumber, 1, 10);
	std::vector<unsigned> route;

	std::cout << distance << std::endl;
	const unsigned result = antColony(citiesNumber, route, distance, alpha, beta, elites, Q, evaporation, maxTime);
	std::cout << "Distance: " << result << std::endl << "Route: ";
	for (unsigned elem : route) {
		std::cout << elem << " ";
	}

	std::cout << std::endl;

	return 0;
}
