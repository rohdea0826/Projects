#include "udp_server.h"

server::server(int _port)
{
	port = _port;
}

void server::serverInit()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		std::cerr << "socket error" << std::endl;
		exit(1);
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		std::cerr << "bind error" << std::endl;
		exit(2);
	}
}

void server::recvData(std::string &out)
{
	char buf[MAX];
	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);
	ssize_t s = recvfrom(sock, buf, sizeof(buf), 0,\
			(struct sockaddr*)&peer, &len);
	if(s > 0){
		buf[s] = 0;
		out = buf;
		pool.putData(out);
		data d;
		d.unserialize(out);

		if(d.command == "quit"){
			std::map<uint32_t, struct sockaddr_in>::iterator iter = online.find(peer.sin_addr.s_addr);
			if(iter != online.end()){
				online.erase(iter->first);
			}
		}
		else{
			online.insert(std::pair<uint32_t, struct sockaddr_in>(peer.sin_addr.s_addr, peer));
		}
	}
}

void server::sendData(const std::string &in, struct sockaddr_in &peer)
{
	sendto(sock, in.c_str(), in.size(), 0,\
			(struct sockaddr*)&peer, sizeof(peer));
}

void server::broadcastData()
{
	std::string str;
	pool.getData(str);
	std::map<uint32_t, struct sockaddr_in>::iterator iter = online.begin();
	for(; iter != online.end(); iter++){
		sendData(str, iter->second);
	}
}

server::~server()
{
	close(sock);
}














