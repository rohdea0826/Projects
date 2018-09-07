#pragma once

#include <iostream>
#include <json/json.h>

void stringToValue(std::string &inString, Json::Value &root);
void valueToString(Json::Value &root, std::string &outString);
