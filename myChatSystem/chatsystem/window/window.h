#pragma once

#include <iostream>
#include <ncurses.h>

#define MAX 1024

class window{
	public:
		window();
		void putStringToWin(WINDOW *w, int y, int x,\
				const std::string &messge);
		void getStringFromWin(WINDOW *w, std::string &outString);
		void clearWinLines(WINDOW *w, int begin, int num);
		void drawHeader();
		void drawOutput();
		void drawFlist();
		void drawInput();
		~window();
		WINDOW *getHeader();
		WINDOW *getOutput();
		WINDOW *getFlist();
		WINDOW *getInput();
	private:
		WINDOW *header;
		WINDOW *output;
		WINDOW *flist;
		WINDOW *input;
};
