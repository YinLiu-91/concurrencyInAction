#include <chrono>
#include <iostream>
int main() {
	auto now1 = std::chrono::system_clock::now();
	std::cout << now1 << std::endl;
	return 0;
}