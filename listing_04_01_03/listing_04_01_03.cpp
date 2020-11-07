#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <chrono>
#include <iostream>
bool more_data_to_prepare() {
	return true;
}

struct data_chunk {
	data_chunk() { std::cout << "执行data_chunk初始化" << std::endl; }
};

data_chunk prepare_data() {
	return data_chunk();
}

void process(data_chunk & ) {
	std::cout << "process data" << std::endl;
}

bool is_last_chunk(data_chunk& a)
{
	return false;

}

std::mutex mut;
std::queue<data_chunk> data_queue;//在线程间传递数据的队列
std::condition_variable data_cond;//条件变量和互斥元一起时使用

void data_preparation_thread() {
	while (more_data_to_prepare()) {
		data_chunk const data = prepare_data();
		std::lock_guard<std::mutex> lk(mut);//尝试锁住mut
		data_queue.push(data);
		data_cond.notify_one();//通知等待的线程
		std::chrono::milliseconds dura(5000);
		std::this_thread::sleep_for(dura);
	}
}


void data_processing_thread() {
	while (true) {
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [] {return !data_queue.empty(); });//和上面的data_cond.notify_one()配对,接受到信号解锁
		data_chunk data = data_queue.front();
		data_queue.pop();
		lk.unlock();//使用unique_lock的目的就是加锁，解锁方便这里使用完毕后解锁
		process(data);//这里为什么会有data_chunk的构造？？不是构造，是上面解锁后，就有了另一个线程的数据准备
		if(is_last_chunk(data))
			break;
	}
}

int main() {

	std::thread t1(data_preparation_thread);
	std::thread t2(data_processing_thread);
	t1.join();
	t2.join();


	return 0;
}

