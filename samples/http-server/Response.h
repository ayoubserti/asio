#ifndef _RESPONSE_H_
#define _RESPONSE_H_
#include "http_parser.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

using namespace std;

struct http_status_stingifier
{
	int num;
	string msg;
};

static http_status_stingifier all_http_status[] = {
#define XX(num,name,msg) {num , #msg},
	HTTP_STATUS_MAP(XX)
#undef XX

};



class Response 
{
public:
	typedef unordered_map<string, string> HeadersMap;
private:

	http_status status_;
	string		status_str_;
	HeadersMap	headers_;
	char*		body_ =nullptr;
	std::size_t body_length;

public:
	string stringify()
	{
		string result{"HTTP/1.1 "};
		result += to_string(status_);
		result += " " + status_str_ + "\n";
		for (auto& i : headers_)
		{
			result += i.first + ": " + i.second + "\n";
		}
		result += "\n";
		return result;
	}

	void set_status(http_status status)
	{
		status_ = status;
		for (auto& i : all_http_status)
		{
			if (i.num == status_)
			{
				status_str_ = i.msg;
				break;
			}
				
		}
	}

	
	
	void set_header(const string& name, std::size_t value)
	{

		headers_[name] = std::to_string((long long)value);
	}


	void set_header(const string& name, const string& value)
	{
		headers_[name] = value;
	}
	void append_body(const char* buf, size_t len)
	{
		body_ = reinterpret_cast<char*>(::malloc(len));
		body_length = len;
	}

	~Response(){
		if (body_ != nullptr)
			::free(body_);
	}
	
};

#endif