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


	std::vector<std::string> output;

	clock_t start = clock();
	vector_to_queue_in_order(origin, starting_queue);

	for_all_in_order(starting_queue, up_case, to_upper_queue);
	for_all_in_order(to_upper_queue, ab, switch_ab_queue);

	queue_to_vector_in_order(switch_ab_queue, output);

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
	unsigned length = 8;

	for (unsigned i = 0; i < 10; ++i) {
		length *= 2;

		std::vector<std::string> origin;
		for (unsigned i = 0; i < number; ++i) {
			origin.push_back(generate_string(length));
		}

		std::cout << "STRING LENGTH: " << length << std::endl;

		clock_t time = in_order(origin);
		std::cout << "TIME IN ORDER:\t" << time << std::endl;

		clock_t time2 = pipeline(origin);
		std::cout << "TIME IN PIPELINE:\t" << time2 << std::endl;
		std::cout << "WINNING " << (((int) time == 0) ? (time) : ((time - time2) * 100 / (float) time)) << " %\n" << std::endl;
	}


	return 0;
}
