#include <exception>
#include <mutex>
#include <stack>
#include <thread>
#include <future>
#include <chrono>
#include <memory>
struct empty_stack :std::exception {
	const char* what()const throw();
};

template<typename T>
class threadsafe_stack {
private: 
	std::stack<T> data;
	mutable std::mutex m;
public:
	threadsafe_stack(){}
	threadsafe_stack(const threadsafe_stack& othter) {
		std::lock_guard<std::mutex> lock(othter.m);
		data = othter.data;
	}
	threadsafe_stack& operator=(const threadsafe_stack&) = delete;//为什么要删除？？
	void push(T new_value) {
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(new_value));//虽然使用了非引用量，但是这里可以move进入data
	}

	std::shared_ptr<T>pop() {//此threadsafe_stack省略了top，解决了top和pop的竞争条件
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())throw empty_stack();
		std::shared_ptr<T> const res(
			std::make_shared<T>(std::move(data.top())));//??????
		data.pop();
		return res;
		
	}
	void pop(T& value) {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())throw empty_stack();
		value = std::move(data.top());//函数取的是引用
		data.pop();
	}
	bool empty()const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}


};
template<typename T>
void input(threadsafe_stack<T> & s1,T const  & t1) {
	for (int i = 0; i < 10000; ++i)
	{
		s1.push(t1);
		std::chrono::milliseconds dura(1000); // 休息 5 s
		std::this_thread::sleep_for(dura);
	}
}
template<typename T>
void pop(threadsafe_stack<T>& s1,T&t1)
{
	s1.pop(t1);
}

#include <iostream>
int main() {

	threadsafe_stack<int > thstack;
	thstack.push(3);
	auto f1 = std::async(input<int>,std::ref(thstack),4);

	int  iout;
	//auto f2= std::async(pop<int>, std::ref(thstack),std::ref(iout));
 	f1.get();
	//f2.get();
	//std::cout << iout << std::endl;
	return 0;
}