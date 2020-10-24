#include <thread>
#include <iostream>

struct func {
	int& i;
	func(int &i_):i(i_){}
	void operator()()
	{
		for (unsigned j = 0; j < 1000000; ++j)
		{
			do_something(i);//对悬空引用可能的访问
		}
	}
	void do_something(int i)
	{
		std::cout << i << "\n";
	}
};
void oops()
{
	int some_local_state = 0;
	func my_func(some_local_state);//定义对象
	std::thread my_thread(my_func);
	my_thread.detach();
	//my_thread.join();
}
int main()
{
	oops();
	return 0;
}