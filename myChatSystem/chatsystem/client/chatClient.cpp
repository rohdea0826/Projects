#include <pthread.h>
#include <signal.h>
#include "udp_client.h"
#include "data.h"
#include "window.h"

void usage(std::string proc)
{
	std::cout << "Usage: " << proc << " ip port" << std::endl;
}

volatile int quitFlag = 0;

typedef struct {
	client *cp;
	window *wp;
}clientInfo_t;

clientInfo_t cw;

void *runHeader(void *arg)
{
	clientInfo_t *p = (clientInfo_t*)arg;
	client *cp = p->cp;
	window *wp = p->wp;

	std::string message = "Welcome To My mini QQ";
	wp->drawHeader();

	int y, x;
	int i = 1;
	int dir = 0;
	for(;;){
		box(wp->getHeader(), 0, 0);
		getmaxyx(wp->getHeader(), y, x);
		wp->putStringToWin(wp->getHeader(), y/2, i, message);
		usleep(100000);
		wp->clearWinLines(wp->getHeader(), y/2, 1);
		if( i > x - message.size() - 2 ){
			dir = 1;
		}
		if( i < 2 ){
			dir = 0;
		}
		if(dir == 0){
			i++;
		}
		else{
			i--;
		}
	//	i %= (x - message.size() - 1);
	}


}

void addUser(std::vector<std::string> &flist, std::string &f)
{
	std::vector<std::string>::iterator iter = flist.begin();
	for( ; iter != flist.end(); iter++ ){
		if(*iter == f){
			return;
		}
	}

	flist.push_back(f);
}

void delUser(std::vector<std::string> &flist, std::string &f)
{
	std::vector<std::string>::iterator iter = flist.begin();
	for( ; iter != flist.end(); iter++){
		if( *iter == f){
			flist.erase(iter);
			return;
		}
	}
}

void *runOutputFlist(void *arg)
{
	clientInfo_t *p = (clientInfo_t*)arg;
	client *cp = p->cp;
	window *wp = p->wp;

	wp->drawOutput();
	wp->drawFlist();
	std::string inString;
	data d;
	std::string showString;
	int y, x;
	int i = 1;
	std::vector<std::string> flist;
	for(;;){
		cp->recvData(inString);
		d.unserialize(inString);
		getmaxyx(wp->getOutput(), y, x);
		showString = d.nick_name;
		showString += "-";
		showString += d.school;

		if(d.command == "quit"){
	    	delUser(flist, showString);
		}
		else
		{
	    	addUser(flist, showString);

	    	showString += "# ";
	    	showString += d.message;
	    	if( i > y - 2 ){
	    		wp->drawOutput();
	    		i = 1;
	    	}
	    	wp->putStringToWin(wp->getOutput(), i++, 1, showString);
		}

		int j = 0;
		wp->drawFlist();
		for( ; j < flist.size(); j++){
			wp->putStringToWin(wp->getFlist(), j+1, 1, flist[j]);
		}
	}
}

void *runInput(void *arg)
{
	clientInfo_t *p = (clientInfo_t*)arg;
	client *cp = p->cp;
	window *wp = p->wp;

	std::string tips = "Please Enter# ";
	data d;
	d.nick_name="Rohdea";
	d.school = "XATU";
	d.command = "None";
	std::string sendString;
	for(;;){
		wp->drawInput();
		wp->putStringToWin(wp->getInput(), 2, 2, tips);
		wp->getStringFromWin(wp->getInput(), d.message);

		d.serialize(sendString);
		cp->sendData(sendString);
	}
}

void sendQuit(int sig)
{
	client *cp = cw.cp;

	std::string sendString;
	data d;
	d.nick_name="Rohdea";
	d.school = "XATU";
	d.message="";
	d.command = "quit";

	d.serialize(sendString);
	cp->sendData(sendString);
	quitFlag = 1;
}

// ./chatClient 192.168.XX.XX 8080
int main(int argc, char *argv[])
{
	if(argc != 3){
		usage(argv[0]);
		return 1;
	}
	std::string ip = argv[1];
	int port = atoi(argv[2]);

	client c(ip, port);
	c.clientInit();
	window w;

	cw.cp = &c;
	cw.wp = &w;

	signal(2, sendQuit);

	pthread_t header, outputFlist, input;
	pthread_create(&header, NULL, runHeader, (void *)&cw);
	pthread_create(&outputFlist, NULL, runOutputFlist, (void *)&cw);
	pthread_create(&input, NULL, runInput, (void *)&cw);

	while(!quitFlag){
		sleep(1);
	}

	pthread_cancel(header);
	pthread_cancel(outputFlist);
	pthread_cancel(input);

	pthread_join(header, NULL);
	pthread_join(outputFlist, NULL);
	pthread_join(input, NULL);


	//std::string str;
	//while(1){
	//	std::cout << "Please Enter# ";
	//	std::cin >> str;
	//	c.sendData(str);
	//	str="";
	//	c.recvData(str);
	//	std::cout << "server echo# " << str << std::endl;
	//}

	return 0;
}


















