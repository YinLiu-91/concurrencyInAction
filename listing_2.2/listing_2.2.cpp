#include <thread>
#include <iostream>
struct func {
	int& i;
	func(int& i_) :i(i_) {}
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

void do_something_in_current_thread()
{}
void f()
{
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread t(my_func);
	try {
		do_something_in_current_thread();
	}
	catch (...)
	{
		t.join();
		throw;

	}
	t.join();
}

int main()
{
	auto myTotalThreads = std::thread::hardware_concurrency();//返回线程的数量
	f();
	return 0;
}