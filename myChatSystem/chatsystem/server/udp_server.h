#pragma once

#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <jsoncpp/json/json.h>
#include "data_pool.h"
#include "data.h"

#define MAX 1024

class server{
	public:
		server(int _port);
		void serverInit();
		void recvData(std::string &out);
		void sendData(const std::string &in, struct sockaddr_in &peer);
		void broadcastData();
		~server();

	private:
		int sock;
		int port;
		dataPool pool;
		std::map<uint32_t, struct sockaddr_in> online;
};









