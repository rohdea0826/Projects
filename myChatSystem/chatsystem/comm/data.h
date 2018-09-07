#pragma once

#include <iostream>
#include <string>
#include "base_json.h"

class data{
	public:
		data();
		void serialize(std::string &outString);
		void unserialize(std::string &inString);
		~data();
	public:
    	std::string nick_name;
    	std::string school;
    	std::string message;
    	std::string command; //?
};
