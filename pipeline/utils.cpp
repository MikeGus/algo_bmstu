#include "utils.h"
#include <algorithm>
#include <mutex>
#include <vector>
#include <queue>
#include <iostream>
#include <random>


void to_upper_case(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

char& switch_ab_ch(char& ch) {
	if (ch == 'A') {
		ch = 'B';
	} else if (ch == 'a') {
		ch = 'b';
	}

	return ch;
}

void switch_ab(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), switch_ab_ch);
}

void vector_to_queue(std::mutex& mutex, std::vector<std::string>& origin, std::queue<std::string>& destination,
					 bool& finished) {
	for (std::string& str : origin) {
		mutex.lock();
		destination.push(str);
		mutex.unlock();
	}

	finished = true;
}

void queue_to_vector(std::mutex& mutex, std::queue<std::string>& origin, bool& previous_over, std::vector<std::string>& destination) {
	while (!origin.empty() || !previous_over) {
		if (!origin.empty()) {

			mutex.lock();
			destination.push_back(origin.front());
			origin.pop();
			mutex.unlock();
		}
	}
}

void for_all(std::mutex& mutex, std::queue<std::string>& origin, bool& previous_over, std::function<void(std::string&)> func,
			 std::queue<std::string>& destination, bool& finished) {
	while (!origin.empty() || !previous_over) {
		if (!origin.empty()) {
			std::string val(origin.front());
			func(val);

			mutex.lock();
			origin.pop();
			destination.push(val);
			mutex.unlock();
		}
	}

	finished = true;
}


std::string generate_string(unsigned size) {
	char alpha[] = "abcdefghijklmnopqrstuvwxyz";
	std::string result;
	srand(time(NULL));
	for (unsigned i = 0; i < size; ++i) {
		result.push_back(alpha[rand() % 26]);
	}
	return result;
}
