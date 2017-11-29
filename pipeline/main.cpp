#include <iostream>
#include <queue>
#include <string>
#include "utils.h"
#include <ctime>
#include <functional>
#include <thread>
#include <mutex>

clock_t in_order(std::vector<std::string>& origin) {

	std::queue<std::string> starting_queue;
	std::queue<std::string> to_upper_queue;
	std::queue<std::string> switch_ab_queue;

	std::function<void(std::string&)>up_case(to_upper_case);
	std::function<void(std::string&)>ab(switch_ab);

	std::mutex mutex;

	std::vector<std::string> output;
	bool initializing_over(false);
	bool upper_over(false);
	bool ab_switch_over(false);


	clock_t start = clock();
	vector_to_queue(std::ref(mutex), origin, starting_queue, initializing_over);

	for_all(std::ref(mutex), starting_queue, initializing_over, up_case, to_upper_queue, upper_over);
	for_all(std::ref(mutex), to_upper_queue, upper_over, ab, switch_ab_queue, ab_switch_over);

	queue_to_vector(std::ref(mutex), switch_ab_queue, ab_switch_over, output);

	clock_t end = clock() - start;


	return end;
}

clock_t pipeline(std::vector<std::string>& origin) {

	std::queue<std::string> starting_queue;
	std::queue<std::string> to_upper_queue;
	std::queue<std::string> switch_ab_queue;

	std::mutex mutex;

	std::function<void(std::string&)>up_case(to_upper_case);
	std::function<void(std::string&)>ab(switch_ab);

	std::vector<std::string> output;
	bool initializing_over(false);
	bool upper_over(false);
	bool switch_ab_over(false);

	clock_t start = clock();
	std::thread stage1(vector_to_queue, std::ref(mutex), std::ref(origin), std::ref(starting_queue), std::ref(initializing_over));

	std::thread stage2(for_all, std::ref(mutex), std::ref(starting_queue), std::ref(initializing_over), up_case,
					   std::ref(to_upper_queue), std::ref(upper_over));

	std::thread stage3(for_all, std::ref(mutex), std::ref(to_upper_queue), std::ref(upper_over), ab, std::ref(switch_ab_queue),
					   std::ref(switch_ab_over));

	std::thread stage4(queue_to_vector, std::ref(mutex), std::ref(switch_ab_queue), std::ref(switch_ab_over), std::ref(output));

	if (stage1.joinable()) {
		stage1.join();
	}
	if (stage2.joinable()) {
		stage2.join();
	}
	if (stage3.joinable()) {
		stage3.join();
	}
	if (stage4.joinable()) {
		stage4.join();
	}


	clock_t end = clock() - start;

	return end;
}


int main()
{
	unsigned number = 10000;
	unsigned length = 100;
	std::vector<std::string> origin;
	for (unsigned i = 0; i < number; ++i) {
		origin.push_back(generate_string(length));
	}

	clock_t time;

	std::cout << "STRING NUMBER: " << number << std::endl << "STRING LENGTH: " << length << std::endl;

	time = in_order(origin);
	std::cout << "TIME IN ORDER:\t" << time << std::endl;

	time = pipeline(origin);
	std::cout << "TIME IN PIPELINE:\t" << time << std::endl;


	return 0;
}
