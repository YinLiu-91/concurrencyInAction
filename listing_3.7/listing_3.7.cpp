#include <mutex>
class hierarchical_mutex {
public:
	explicit hierarchical_mutex(unsigned level)
	{}
	void lock() {}
	void unlock(){}
};

hierarchical_mutex heigh_lelvel_mutex(10000);
hierarchical_mutex low_level_mutex(5000);

int dol_lo_level_stuff()
{
	return 42;
}

int low_level_func()
{
	std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
	return dol_lo_level_stuff();
}

void hith_level_stuff(int some_param)
{}
void high_level_func()
{
	std::lock_guard<hierarchical_mutex> lk(heigh_lelvel_mutex);
	high_level_func(low_level_func());
}
void thread_a()
{
	high_level_func();
}

hierarchical_mutex other_mutex(100);
void do_other_stuff()
{

}

void other_stuff()
{
	high_level_func();
	do_other_stuff();
}

void thread_b()
{
	std::lock_guard<hierarchical_mutex>lk(other_mutex);
	other_stuff();
}

int main()
{

	return 0;
}