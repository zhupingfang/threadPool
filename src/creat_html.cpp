
#include "creat_html.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#endif

CreateHtml::CreateHtml() {
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

CreateHtml::~CreateHtml() {
#ifdef _WIN32
	WSACleanup();
#endif
}

void CreateHtml::AddContacts(const std::vector<std::string>& emailAddress) {
	contacts_.insert(contacts_.end(), emailAddress.begin(), emailAddress.end());
}

void CreateHtml::AddHtml(const std::string& html) {
	html_ = html;
}

void CreateHtml::Send(const std::string& smtp_server, int port, const std::string& from, const std::string& username, const std::string& password) {
	int socket = create_socket(smtp_server, port);

	std::cout << receive_data(socket);

	send_data(socket, "HELO " + smtp_server + "\r\n");
	std::cout << receive_data(socket);

	send_data(socket, "AUTH LOGIN\r\n");
	std::cout << receive_data(socket);

	send_data(socket, base64_encode(username) + "\r\n");
	std::cout << receive_data(socket);

	send_data(socket, base64_encode(password) + "\r\n");
	std::cout << receive_data(socket);

	send_data(socket, "MAIL FROM:<" + from + ">\r\n");
	std::cout << receive_data(socket);

	for (const auto& contact : contacts_) {
		send_data(socket, "RCPT TO:<" + contact + ">\r\n");
		std::cout << receive_data(socket);
	}

	send_data(socket, "DATA\r\n");
	std::cout << receive_data(socket);

	std::string data = "From: " + from + "\r\n";
	data += "To: " + contacts_[0];
	for (size_t i = 1; i < contacts_.size(); ++i) {
		data += ", " + contacts_[i];
	}
	data += "\r\nSubject: Test Email\r\n";
	data += "Content-Type: text/html\r\n";
	data += "\r\n" + html_ + "\r\n.\r\n";

	send_data(socket, data);
	std::cout << receive_data(socket);

	send_data(socket, "QUIT\r\n");
	std::cout << receive_data(socket);

#ifdef _WIN32
	closesocket(socket);
#else
	close(socket);
#endif
}

int CreateHtml::create_socket(const std::string& server, int port) {
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int sock = 0;

	if (getaddrinfo(server.c_str(), std::to_string(port).c_str(), &hints, &res) != 0) {
		perror("getaddrinfo");
		exit(1);
	}

	for (struct addrinfo* p = res; p != NULL; p = p->ai_next) {
		if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			continue;
		}

		if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
#ifdef _WIN32
			closesocket(sock);
#else
			close(sock);
#endif
			continue;
		}

		break;
	}

	freeaddrinfo(res);

	if (sock == -1) {
		perror("socket");
		exit(1);
	}

	return sock;
}

void CreateHtml::send_data(int socket, const std::string& data) {
	send(socket, data.c_str(), data.size(), 0);
}

std::string CreateHtml::receive_data(int socket) {
	char buffer[1024];
	int length = recv(socket, buffer, sizeof(buffer) - 1, 0);
	if (length < 0) {
		perror("recv");
		exit(1);
	}
	buffer[length] = '\0';
	return std::string(buffer);
}

std::string CreateHtml::base64_encode(const std::string& data) {
	static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string result;
	int val = 0, valb = -6;
	for (unsigned char c : data) {
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			result.push_back(base64_chars[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) {
		result.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
	}
	while (result.size() % 4) {
		result.push_back('=');
	}
	return result;
}
