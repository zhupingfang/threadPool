//
// Created by 巴尔悍匪 on 2024/7/21.
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

// 想简化关于存储任意类型的实现
template < typename T >
class Base {
public:
	Base(T data) = delete;
	Base(T && data) :data_(data)
	{}
	Base & operator=(const Base & ba) = default;
	Base(T & data) : data_(data)
	{
	}
private:
	T data_ {};
};

// 实现一个 any 类型，可以储存任何数据 c17已经实现
class Any
{
public:
	Any() = default;
	~Any() = default;
	Any(Any &&) = default;
	Any &operator=(Any &&) = default;

	Any(const Any &) = delete;
	Any &operator=(const Any &) = delete;

	template < typename T >
	Any (T data) : basePtr_(std::make_unique< Dervir< T >>(data))
	{}
//
//	template < typename T >
//	Any (T &data) : basePtr_(std::make_unique< Dervir< T >>(data))
//	{}

	// 得到数据
	template<typename T>
	T Cast()
	{
		Dervir< T > *dir = dynamic_cast< Dervir< T >* >(basePtr_.get());
		if (dir == nullptr) {
			throw "basePtr_ dynamic_cast to Dervir is fail!";
		}
		return dir->data_;
	}
private:
	// 基类对象
	class BaseDataPtr
	{
	public:
		virtual ~BaseDataPtr() = default;
	};

	// 子类实现
	template<typename T>
	class Dervir : public BaseDataPtr
	{
	public:
		Dervir(T &data) : data_(data)
		{}
		T data_;
	};

private:
	// 父类指针的成员变量
	std::unique_ptr< BaseDataPtr > basePtr_ {nullptr};
};

// 实现一个简单的信号量， c20已经实现
class Semaphore
{
public:
	explicit Semaphore(size_t size) : number_ (size)
	{}

	// 获取一个信号量
	void Wait()
	{
		std::unique_lock lock(mutex_);
		cond_.wait(lock, [&] ()->bool {return number_ > 0; });
		number_--;
	}
	// 增加一个信号量
	void Push()
	{
		std::unique_lock lock(mutex_);
		number_++;
		cond_.notify_all();
	}
private:
	size_t number_ {0};
	std::mutex mutex_;
	std::condition_variable cond_ {};
};

// 声明类型
class Task;
// 实现一个线程执行完毕后的返回值
class Result
{
public:
	Result(std::shared_ptr< Task > task, bool isValid);
	// 开放给用户的
	Any Get();
	// 放开给 线程使用， 记录返回值
	void SetValue(Any && data);
private:
	Any any_;                        // 存储的返回值
	Semaphore sem_ {0};         // 信号量
	std::shared_ptr< Task > task_;   // 存储任务的指针
	bool isAviable_ {false};         // 返回值是否可用
};
// ---------------------------------------------------------------------------
enum class ThreadType {
	THREAD_FIXED,
	THREAD_CACHED
};

// 任务
class Task
{
public:
	Task()
	{}
	virtual Any Run() = 0;
	void SetResult(Result *ptr) {
		res_ = ptr;
	}
	// 线程中真正调用的是这个函数，为啥不直接调用Run方法，为了将返回值进行设置
	void ThreadRun()
	{
		if (res_ != nullptr) {
			res_->SetValue(std::move(this->Run()));
		}
	}

private:
	Result *res_ {nullptr};  // Result的声明周期是长于Task的,在将任务放入任务队列的时候，先将这里赋值为它对应的返回值对象
};

// 线程
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

// 线程池主题结构
class ThreadPool
{
	friend Thread;
public:
	ThreadPool();
	~ThreadPool();
	// 设置当前任务 模式
	void SetMode(ThreadType mode);
	// 提交任务
	Result SubmitTask(std::shared_ptr< Task > task);
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
	std::queue<std::shared_ptr<Task>> taskQue_ {};           // 任务队列
	std::atomic_int taskSize_ {0};                        // 任务数量

	size_t taskMaxSize_ {1024};                             // 任务队列默认最大值
	size_t threadSize_ {4};                                 // 线程数量

	std::mutex threadMutex_ {};                             // 线程锁
	std::condition_variable notFull_;
	std::condition_variable notEmpty_;
	ThreadType threadMode_ {ThreadType::THREAD_FIXED};  //线程池的类型
};

}


#endif //THREAD_POOL_H
