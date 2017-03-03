#pragma once 
#include "http_parser.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

using namespace std;
using namespace std::placeholders;


class Response;


class Request
{
public :
	typedef unordered_map<string, string> HeadersMap;
private:	
	http_method method_;
	string url_;
	
	HeadersMap  headers_;
	string method_str_;
	string next_header_;
	char* body_=nullptr;
	size_t body_len_=0;
	


	
	static int on_message_begin(http_parser *p)
	{
		return 0;
	}

	static int on_header_field(http_parser *p, const char *buf, size_t len)
	{
		reinterpret_cast<Request*>(p->data)->next_header_ = std::move(string(buf, len));  return 0;
	}

	static int on_header_value(http_parser *p, const char *buf, size_t len)
	{
		Request* o = reinterpret_cast<Request*>(p->data);
		o->headers_[o->next_header_] = std::move(string(buf, len));  
		return 0;
	}


	static int on_request_url(http_parser *p, const char *buf, size_t len){
		Request* req = reinterpret_cast<Request*>(p->data);
		http_parser_url url;
		http_parser_url_init(&url);
		cout << http_parser_parse_url(buf, len, 0, &url);
		req->url_ = string(buf, len);
		return 0;
	}

	static int on_body(http_parser *p, const char *buf, size_t len)
	{
		Request* req = reinterpret_cast<Request*>(p->data);
		req->body_ = reinterpret_cast<char*>(::malloc(len));
		req->body_len_ = len;
		::memcpy(req->body_, buf, len);
		return 0; 
	}


	static int on_headers_complete(http_parser *p) 
	{

		Request* o = reinterpret_cast<Request*>(p->data);
		for (auto&i : o->headers_)
		{
			cout << i.first << "      " << i.second << endl;
		}

		return 0;
	}
	
	static int on_message_complete(http_parser *p) 
	{
		Request* req = reinterpret_cast<Request*>(p->data);
		req->method_str_ = http_method_str((http_method)p->method);
		req->method_ = (http_method)p->method;

		cerr << "Message Complete" << endl;
		return 0;
	}

	static int on_chunk_header(http_parser *p){
		return 0; 
	}

	static int on_chunk_complete(http_parser *p) {
		return 0;
	}

	static int on_status(http_parser *p, const char *buf, size_t len)
	{
		return 0;

	}
public:
	int parse(const char* buf,std::size_t len){
		
		http_parser parser;
		http_parser_init(&parser, http_parser_type::HTTP_BOTH);
		parser.data = this;
		http_parser_settings settings;

		settings.on_url = &Request::on_request_url;
		settings.on_message_begin = &Request::on_message_begin;
		settings.on_header_field = &Request::on_header_field;
		settings.on_header_value = &Request::on_header_value;

		settings.on_status = &Request::on_status;
		settings.on_body = &Request::on_body;
		settings.on_headers_complete = &Request::on_headers_complete;
		settings.on_message_complete = &Request::on_message_complete;
		settings.on_chunk_header = &Request::on_chunk_header;
		settings.on_chunk_complete = &Request::on_chunk_complete;

		int len_of_done = http_parser_execute(&parser, &settings, buf, len);
		return len_of_done;
		
	}

    size_t get_body_length()
    {
        return body_len_;
    }

    char* get_body(){
        return body_;
    }

    HeadersMap get_headers()
    {
        return headers_;
    }

    string get_url()
    {
        return url_;
    }
    
    string get_method_str()
    {
        return method_str_;
    }
	
	string stringify()
	{
		string result{};
		result += method_str_ + " " ;
		result += url_ + " HTTP/1.1\n";
		for (auto& i : headers_)
		{
			result += i.first + ": " + i.second + "\n";
		}
		result += "\n";
		return result;
	}
	string get_header(const string& name)
	{
		return headers_[name];
	}
};
    