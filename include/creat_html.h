//
// Created by zhupi on 2024/7/28.
//

#ifndef CREAT_HTML_H
#define CREAT_HTML_H

#include <string>
#include <vector>

// 以下是CreateHtml类的实现，通过SMTP协议发送邮件：
#include <iostream>
#include <vector>
#include <string>

class CreateHtml {
public:
	CreateHtml();
	~CreateHtml();

	void AddContacts(const std::vector<std::string>& emailAddress);
	void AddHtml(const std::string& html);
	void Send(const std::string& smtp_server, int port, const std::string& from, const std::string& username, const std::string& password);

private:
	std::vector<std::string> contacts_;
	std::string html_;

	int create_socket(const std::string& server, int port);
	void send_data(int socket, const std::string& data);
	std::string receive_data(int socket);
	std::string base64_encode(const std::string& data);
};
#endif //CREAT_HTML_H
