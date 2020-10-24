//2020.10.18
#include <future>
#include <iostream>

int find_the_answer_to_ltuae()
{
	std::cout << "找到世界的答案" << std::endl;
	return 1;
}
void do_other_stuff()
{
	std::cout << "去做其他的事情" << std::endl;
}
int main()
{
	std::future<int> the_answer = std::async(find_the_answer_to_ltuae);
	do_other_stuff();
	std::cout << "the answer is " << the_answer.get() << std::endl;
	return 0;
}