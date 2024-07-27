//
// Created by zhupi on 2024/7/21.
//

#include "thread_pool.h"
#include <functional>
#include <chrono>

namespace ThreadPool
{

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

Thread::Thread(Thread::ThreadFunc func) :func_(func)
{}
//Thread::Thread(const Thread::ThreadFunc &func)
//	: func_(func)
//{}

Thread::~Thread()
{

}
// 启动线程
void Thread::Start()
{
	std::thread *thread = new std::thread(func_);
	thread->detach();   // 和主程序分离
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
ThreadPool::ThreadPool()
{

}
ThreadPool::~ThreadPool()
{

}
// 设置当前任务 模式
void ThreadPool::SetMode(ThreadType mode)
{
	threadMode_ = mode;
}
// 提交任务
void ThreadPool::SubmitTask(std::shared_ptr< Task > task)
{
	// 获取锁
	std::unique_lock<std::mutex > lock(threadMutex_);

	// 判断任务队列是否满了
	// wait wait_for wait_nutil
//	notFull_.wait(lock,[&]()->bool {return taskQue_.size() < taskMaxSize_;}); 	// 会卡死
	bool ret = notFull_.wait_for(lock, std::chrono::seconds(1), [&]()->bool {return taskQue_.size() < taskMaxSize_;}); // 等待一秒
	if (!ret) {
		// error
		return;
	}
//	notFull_.wait_until(lock, )

	// 放入任务
	taskQue_.emplace(task);
	taskMaxSize_++;
	// 通知线程有任务来了
	notFull_.notify_one();
//	notFull_.notify_all();
}

// 开启任务
void ThreadPool::Start(size_t size)
{
	threadSize_ = size;
	for(size_t i = 0; i <threadSize_; i++) {
		std::unique_ptr<Thread> ptr = std::make_unique<Thread>(std::bind(&ThreadPool::ThreadFunc,this));
		threadVec_.emplace_back(std::move(ptr));
	}
	for(size_t i = 0; i <threadSize_; i++) {
		threadVec_[i]->Start();
	}
}

// 停止线程池
void ThreadPool::Stop()
{

}

// 设置任务最大数量
void ThreadPool::SetTaskMaxValue(size_t value)
{
	taskMaxSize_ = value;
}

void ThreadPool::ThreadFunc()
{
	while (true) {
		std::cout << " being start thread ...... " << std::thread().get_id() << std::endl;
		std::shared_ptr<Task > funcPtr;
		{
			// 获取锁
			std::unique_lock<std::mutex> lock( threadMutex_);
			// 等待任务
			notEmpty_.wait(lock, [&] ()->bool { return taskQue_.size() >= 1;});

			// 得到任务
			funcPtr = taskQue_.front();
			std::cout << "get one task ......" << std::endl;
			taskQue_.pop();
			taskMaxSize_--;
			if (taskQue_.size() > 0) {
				notEmpty_.notify_one();
			}
			// 取出一个任务，进行通知
			notFull_.notify_one();
		}
		//  执行任务
		if ( funcPtr != nullptr) {
			funcPtr->Run();
		}
	}
}
};