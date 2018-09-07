#include "data.h"

my_data::my_data()
{
}

void my_data::serialize(std::string &outString)
{
	Json::Value root;
	root["nick_name"] = nick_name;
	root["school"] = school;
	root["message"] = message;
	root["command"] = command;

	valueToString(root, outString);
}

void my_data::unserialize(std::string &inString)
{
	Json::Value root;
	stringToValue(inString, root);
	nick_name = root["nick_name"].asString();
	school = root["school"].asString();
	message = root["message"].asString();
	command = root["command"].asString();
}

my_data::~my_data()
{
}
















