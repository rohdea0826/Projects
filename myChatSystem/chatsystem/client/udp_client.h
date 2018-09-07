#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jsoncpp/json/json.h>
#include <unistd.h>
#include <ncurses.h>


#define MAX 1024
class client{
	public:
		client(std::string _ip, int _port);
		void clientInit();
		void recvData(std::string &out);
		void sendData(const std::string &in);
		~client();

	private:
		int sock;
		struct sockaddr_in server;
};








