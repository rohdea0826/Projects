#pragma once

#include <iostream>
#include <string>
#include "base_json.h"

class my_data{
	public:
        my_data();
		void serialize(std::string &outString);
		void unserialize(std::string &inString);
        ~my_data();
	public:
    	std::string nick_name;
    	std::string school;
    	std::string message;
    	std::string command; //?
};
