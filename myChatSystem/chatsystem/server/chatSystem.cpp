#include <pthread.h>
#include "udp_server.h"

void usage(std::string proc)
{
	std::cout << "Usage: " << proc << " port" << std::endl;
}

void *recvMsg(void *arg)
{
	server *s = (server*)arg;

	std::string str;
	for( ; ; ){
		s->recvData(str);
		std::cout << "debug: " << str << std::endl;
	}
}

void *sendMsg(void *arg)
{
	server *s = (server*)arg;
	for( ; ; ){
		s->broadcastData();
	}
}

// ./chatSystem 8080
int main(int argc, char *argv[])
{
	if(argc != 2){
		usage(argv[0]);
		return 1;
	}
	int port = atoi(argv[1]);
	server s(port);
	s.serverInit();

	pthread_t recver, sender;
	pthread_create(&recver, NULL, recvMsg, (void *)&s);
	pthread_create(&sender, NULL, sendMsg, (void *)&s);

	pthread_join(recver, NULL);
	pthread_join(sender, NULL);

	return 0;
}


















