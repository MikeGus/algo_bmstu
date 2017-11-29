#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <functional>
#include <vector>
#include <queue>
#include <mutex>

char& switch_ab_ch(char& ch);

void to_upper_case(std::string& str);
void switch_ab(std::string& str);

void vector_to_queue(std::mutex& mutex, std::vector<std::string>& origin, std::queue<std::string>& destination, bool& finished);

void queue_to_vector(std::mutex& mutex, std::queue<std::string>& origin, bool& previous_over,
					 std::vector<std::string>& destination);

void for_all(std::mutex& mutex, std::queue<std::string>& origin, bool& previous_over, std::function<void(std::string&)> func,
			 std::queue<std::string>& destination, bool& finished);

std::string generate_string(unsigned size);


#endif // UTILS_H
