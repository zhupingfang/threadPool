#include <iostream>
#include "thread_pool.h"

#include <thread>
#include <chrono>




class DickTask : public ThreadPool::Task
{
public:
	ThreadPool::Any Run () override {
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << " func is being :" <<std::this_thread::get_id() <<std::endl;
		return  ThreadPool::Any(99);
	}
};
/*
 *  test:
 *  	ThreadPool::ThreadPool pool;
	pool.Start(10);
 */


int main() {

//	std::shared_ptr<Base<int> > task;
//	task = new Base(1234);
//	ThreadPool::ThreadPool pool;
//	pool.Start(10);
//	DickTask disk;
//	std::shared_ptr < ThreadPool::Task > task;
//	pool.SubmitTask(task);
//
//	std::this_thread::sleep_for(std::chrono::seconds(5));
//
//	//
//	getchar();
	return 0;
}
