#include <thread>
#include <iostream>
class back_ground_tast {
public:
	back_ground_tast() {
		std::cout << "构造了对象" << std::endl;
	}
	back_ground_tast(back_ground_tast const& rhs) {//线程利用拷贝构造函数将对象拷贝到新线程中去
		std::cout << "拷贝构造了对象" << std::endl;
	}
public:
	void operator()()const {
		std::cout << "启动了函数对象" << std::endl;
	}
};

int main()
{
	back_ground_tast f;
	std::thread my_thread(f);
	my_thread.join();
	return 0;
} 