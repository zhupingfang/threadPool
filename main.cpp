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
/*
 *  test:
 *  	ThreadPool::ThreadPool pool;
	pool.Start(10);
 */


int main() {

//	ThreadPool::ThreadPool pool;
//	pool.Start(10);
//	DickTask disk;
//	pool.SubmitTask(std::make_unique<DickTask>());
//
//	getchar();

//	CreateHtml email;
//	email.AddContacts({"recipient1@example.com", "recipient2@example.com"});
//	email.AddHtml("<html><body><h1>Hello, World!</h1></body></html>");
//	email.Send("smtp.example.com", 587, "your_email@example.com", "your_email@example.com", "your_password");


	CreateHtml email;
	email.AddContacts({"2777890989@qq.com", "2777890989@qq.com"});
	email.AddHtml("<html><body><h1>Hello, World!</h1></body></html>");
	email.Send("smtp.qq.com", 587, "2777890989@qq.com", "2777890989@qq.com", "zeqsfvmmvpwbdgfg");


	return 0;
}
