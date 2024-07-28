#include <iostream>
#include "thread_pool.h"
#include "creat_html.h"
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


void ThreadPools()
{
	ThreadPool::ThreadPool pool;
	// 启动10 个线程
	pool.Start(10);
	DickTask disk;
	ThreadPool::Result result = pool.SubmitTask(std::make_unique<DickTask>());
	int ret = result.Get().Cast<int>();
	std::cout << " ret is"  << ret;
}

void SendHtml()
{
//	CreateHtml email;
//	email.AddContacts({"recipient1@example.com", "recipient2@example.com"});
//	email.AddHtml("<html><body><h1>Hello, World!</h1></body></html>");
//	email.Send("smtp.example.com", 587, "your_email@example.com", "your_email@example.com", "your_password");
}

int main() {

	ThreadPools();
	getchar();
	SendHtml();
	return 0;
}
