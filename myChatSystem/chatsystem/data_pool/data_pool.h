#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <semaphore.h>

#define NUM 128

class dataPool{
	public:
		dataPool(int _cap=NUM);
		void putData(const std::string &data);
		void getData(std::string &data);
		~dataPool();

	private:
		std::vector<std::string> pool;
		int cap;
		int product_step;
		int consume_step;
		sem_t blankSem;
		sem_t dataSem;
};
