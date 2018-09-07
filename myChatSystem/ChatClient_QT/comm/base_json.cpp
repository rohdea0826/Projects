#include "base_json.h"

void stringToValue(std::string &inString, Json::Value &root)
{
	Json::Reader r;
	r.parse(inString, root, false);
}

void valueToString(Json::Value &root, std::string &outString)
{
	Json::FastWriter w;
	outString = w.write(root);
}
