#include "http_parser.h"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;





class Request
{
	
	http_method method_;
	string url_;
	typedef unordered_map<string, string> HeadersMap;
	HeadersMap  headers_;
	string method_str_;
	string next_header_;

public:
	int on_message_begin(http_parser *p)
	{
		cerr << "Message Begin" << endl;
		Request* req = new Request;
		p->data = req;
		return 0;
	}

	int on_header_field(http_parser *p, const char *buf, size_t len)
	{
		reinterpret_cast<Request*>(p->data)->next_header_ = string(buf, len);  return 0;
	}

	int on_header_value(http_parser *p, const char *buf, size_t len)
	{
		Request* o = reinterpret_cast<Request*>(p->data);
		o->headers_[o->next_header_] = string(buf, len);  
		return 0;
	}



	
};
    
    
int request_url_cb(http_parser *p, const char *buf, size_t len){ 
	Request* req = reinterpret_cast<Request*>(p->data);
	http_parser_url url;
	http_parser_url_init(&url);
	cout << http_parser_parse_url(buf, len, 0, &url);
	req->url_ = string(buf, len);
	return 0; 
}
    
int body_cb(http_parser *p, const char *buf, size_t len) { cout << string(buf, len) << endl;  return 0; }
    
int headers_complete_cb(http_parser *p) {
		
	Request* o = reinterpret_cast<Request*>(p->data);
	for (auto&i : o->headers_)
	{
		cout << i.first << "      " << i.second << endl;
	}

	 return 0; }
    
int message_complete_cb(http_parser *p) { 
	Request* req = reinterpret_cast<Request*>(p->data);
	req->method_str_ = http_method_str((http_method)p->method);
	req->method_ = (http_method)p->method;

	cerr << "Message Complete" << endl;  return 0; }
    
int response_status_cb(http_parser *p, const char *buf, size_t len) { return 0; }
    
int chunk_header_cb(http_parser *p){ return 0; }
    
int chunk_complete_cb(http_parser *p) { return 0; }

int main(void)
{
	
	const char* data = "GET /hello.php?d=112&l=222 HTTP/1.1"
		"\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)"
		"\r\nHost: www.tutorialspoint.com"
		"\r\nAccept-Language: en-us"
		"\r\nAccept-Encoding: gzip, deflate"
		"\r\nContent-Length: 5"
		"\r\nContent-Type: application/x-binary"
		"\r\nConnection: Keep-Alive\r\n\r\n dddd";
	
	size_t requestlen = strlen(data);
	http_parser parser;
	http_parser_settings settings;
	
	settings.on_url = request_url_cb;
	settings.on_message_begin = message_begin_cb;
	settings.on_header_field = header_field_cb;
	settings.on_header_value = header_value_cb;
	//settings.on_url = pause_request_url_cb;
	settings.on_status = [](http_parser *p, const char *buf, size_t len) -> int{
		cout << "Status " << buf << len;
		return 0;
	};
	settings.on_body = body_cb;
	settings.on_headers_complete = headers_complete_cb;
	settings.on_message_complete = message_complete_cb;
	settings.on_chunk_header = chunk_header_cb;
	settings.on_chunk_complete = chunk_complete_cb;

	//cout << http_parser_version();
	http_parser_init(&parser, http_parser_type::HTTP_REQUEST);
	

	cout << "parsed data length:" <<  http_parser_execute(&parser, &settings, data, requestlen) << endl;
	
	cout << "Request Lenght: " << requestlen << endl;
	return 0;
}