//2020.10.18
#include<future>
#include <chrono>
#include <iostream>

void mythread(std::promise<int>& tmpt, int calc)
{
	calc++;
	calc *= 10;

	//等待5s
	std::chrono::milliseconds dura(5000);
	std::this_thread::sleep_for(dura);
	int t = calc;
	tmpt.set_value(t);
	return;
}
void mythread2(std::future<int>& tmpf) {
	
	std::cout << tmpf.get() << std::endl;
}
int main() {
	std::promise<int> myprom;//声明promise对象，
	std::thread t1(mythread, std::ref(myprom), 180);
	t1.join();

	//获取结果
	std::future<int> fu1 = myprom.get_future();

	std::thread t2(mythread2, std::ref(fu1));
	t2.join();
	//auto result = fu1.get();
	//std::cout << result << std::endl;
	return 0;
}