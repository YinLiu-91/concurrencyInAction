#include <string>
#include <future>

#include <iostream>
struct X {
	X(){std::cout<< "正在调用X的构造函数" << std::endl;
	}
	void foo(int, std::string const&) {
		std::cout << "正在调用foo" << std::endl;
	}
	std::string bar(std::string const&) { std::string a = "正在调用bar"; std::cout << a << std::endl; return a; }
};

X x;
auto f1 = std::async(std::launch::deferred, &X::foo, &x, 42, "hello");//这里启动一个异步任务
auto f2 = std::async(std::launch::deferred, &X::bar, x, "goodbye");

struct Y {
	double operator()(double) { std::cout << "正在调用Y函数对象" << std::endl; return 0; }
};

Y y;
auto f3 = std::async(std::launch::deferred, Y(), 3.141);
auto f4 = std::async(std::launch::deferred, std::ref(y), 2.718);
void  baz(X&a) {   }
auto f6 = std::async(std::launch::deferred, baz, std::ref(x));

class move_only {
public:
	move_only() { std::cout << "调用了move_only的构造函数" << std:: endl; }
	move_only(move_only const &) {
		std::cout << "调用了move_only的拷贝构造" << std::endl;
	}
	move_only(move_only&&) {
		std::cout << "调用了move_only的移动拷贝构造" << std::endl;
	}
	move_only(move_only const&&) = delete;
	move_only& operator=(move_only&&) {}
	move_only& operator=(move_only const&&) = delete;
	void operator()() { std::cout << "调用了move_only函数对象" << std::endl; }
};
auto f5 = std::async(std::launch::deferred, move_only());//首先，构造move_only对象，

int main() {
	f1.get();
	f2.get();
	f3.get();
	f4.get();
	f5.get();
	f6.get();
	return 0;
}