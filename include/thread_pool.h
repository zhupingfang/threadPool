//
// Created by zhupi on 2024/7/21.
//

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <queue>
#include <memory>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <functional>

namespace ThreadPool
{

enum class ThreadType {
	THREAD_FIXED,
	THREAD_CACHED
};

class Task
{
public:
	virtual void Run() = 0;
};

class Thread
{
public:
	using ThreadFunc = std::function<void()>;

	explicit Thread(ThreadFunc func);
//	explicit Thread(const ThreadFunc &func);
	 virtual ~Thread();
	// 启动线程
	void Start();
private:
	ThreadFunc func_;
};

class ThreadPool
{
	friend Thread;
public:
	ThreadPool();
	~ThreadPool();
	// 设置当前任务 模式
	void SetMode(ThreadType mode);
	// 提交任务
	void SubmitTask(std::shared_ptr< Task > task);
	// 开启任务
	void Start(size_t size = 4);
	// 停止线程池
	void Stop();
	// 设置任务最大数量
	void SetTaskMaxValue(size_t value);

	ThreadPool(const ThreadPool &) = delete;
	ThreadPool operator=(const ThreadPool &) = delete;
private:
	void ThreadFunc();
private:
	std::vector< std::unique_ptr<Thread> > threadVec_ {};    // 线程容器
	std::queue<std::shared_ptr<Task>> taskQue_ {}; // 任务队列
	std::atomic_int taskSize_ {0};              // 任务数量

	size_t taskMaxSize_ {1024};                      // 任务队列默认最大值
	size_t threadSize_ {4};                        // 线程数量

	std::mutex threadMutex_ {};                         // 线程锁
	std::condition_variable notFull_;
	std::condition_variable notEmpty_;
	ThreadType threadMode_ {ThreadType::THREAD_FIXED};  //线程池的类型
};

}


#endif //THREAD_POOL_H
