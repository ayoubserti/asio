#pragma once 
#include "http_parser.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

using namespace std;



class Response 
{
public:
	typedef unordered_map<string, string> HeadersMap;
private:

	http_status status_;
	string		status_str_;
	HeadersMap	headers_;

public:
	string stringify()
	{
		string result{"HTTP/1.1 "};
		result += status_;
		result += " " + status_str_ + "\n";
		for (auto& i : headers_)
		{
			result += i.first + ": " + i.second + "\n";
		}
		result += "\n";
		return result;
	}

};