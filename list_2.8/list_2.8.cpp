#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <omp.h>
template<typename Iterator, typename T>
struct accumulate_block
{
	void operator()(Iterator first, Iterator last, T& result)
	{
		result = std::accumulate(first, last, result);
	}
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
	unsigned long const length = std::distance(first, last);
	if (!length)return init;
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads =
		(length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();

	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

	unsigned long const block_size = length / num_threads;

	std::vector<T> results(num_threads);//为中间结果创建vector
	std::vector<std::thread> threads(num_threads - 1);//创建线程，因为已经有主线程，所以少一个

	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));//注意有ref
		block_start = block_end;
	}

	accumulate_block<Iterator, T>()
		(block_start, last, results[num_threads - 1]);
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	return std::accumulate(results.begin(), results.end(), init);
}
int main()
{
	std::vector<int> vi;
	int vi_size = 1000000000;
	vi.resize(vi_size);
#pragma  omp parallel for //num_threads(12)
	for (int i = 0; i < vi_size; ++i)//大部分耗在赋值上,可加omp优化
	{
		vi[i]=10;
	}
	int sum = parallel_accumulate(vi.begin(), vi.end(), 5);
	std::cout << "sum=" << sum << std::endl;
	return 0;
}