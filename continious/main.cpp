#include <iostream>

class AverageCounter {
	public:
		AverageCounter();
		double getAverage();

		friend std::istream& operator>>(std::istream& input, AverageCounter& counter);
		friend std::ostream& operator<<(std::ostream& output, AverageCounter& counter);

	private:
		void update(double value);
		unsigned count;
		double combined;
};

AverageCounter::AverageCounter(): count(0), combined(0.0) {}

void AverageCounter::update(double value) {
	combined += value;
	++count;
}

std::istream& operator>>(std::istream& input, AverageCounter& counter) {
	double value = 0;
	if (input >> value) {
		counter.update(value);
		return input;
	}
	return input >> value;
}

std::ostream& operator<<(std::ostream& output, AverageCounter& counter) {
	return output << "Average value: " << counter.getAverage();
}

double AverageCounter::getAverage() {
	return combined / count;
}

int main()
{
	AverageCounter counter;
	std::cout << "Type value: ";
	while (std::cin >> counter) {
		std::cout << counter << std::endl;
		std::cout << "Type value: ";
	}
	std::cout << "\nEND OF INPUT" << std::endl;
	return 0;
}
