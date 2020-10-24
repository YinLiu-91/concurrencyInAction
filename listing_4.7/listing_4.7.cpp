#include <future>
#include <string>
#include <iostream>
struct X {
	void foo(int i, std::string const & s)
	{
		std::string s1=s+ std::string(std::to_string(i));
		std::cout << "正在执行foo函数" << std::endl;
	}
	std::string bar(std::string const& s)
	{
		std::cout << "正在执行bar函数" << std::endl;
		return s + "bar";
	}
};

X x;
auto f1 = std::async(&X::foo, &x, 42, "hello");
auto f2 = std::async(&X::bar, &x, "goodbye");

struct Y {
	double operator()(double d) {
		std::cout << "执行Y: " << d << std::endl;
		return d + 1;
	}
};

Y y;
auto f3 = std::async(Y(), 3.141);
auto f4 = std::async(std::ref(y), 2.718);

class move_only {
	public:
		move_only() {
			std::cout << "执行了move_only的默认构造函数" << std::endl;
		}
		move_only(move_only && rhs) {
			std::cout << "执行了move_only的move构造函数 " << std::endl;
		}
		move_only(move_only& rhs) = delete;
		move_only& operator=(move_only&& rhs) {
			std::cout << "执行力move_only的移动赋值函数 " << std::endl;
		}
		move_only& operator=(move_only const& rhs) = delete;

		void operator()() { std::cout << "执行了函数对象" << std::endl; }
};

auto f5 = std::async(move_only());

int main()
{
	f1.get();
	f2.get();
	f3.get();
	f4.get();
	f5.get();

	 
	return 0; 
}