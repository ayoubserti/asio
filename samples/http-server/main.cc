#include "http_parser.h"
#include <iostream>
#include <string>


using namespace std;


int message_begin_cb(http_parser *p){
	cout << "Message Begin" << endl;
	
	return 0;
}

int header_field_cb(http_parser *p, const char *buf, size_t len){ cout << string(buf,len) << endl;  return 0; }
    
int header_value_cb(http_parser *p, const char *buf, size_t len){ cout << string(buf, len) << endl;  return 0; }
    
int request_url_cb(http_parser *p, const char *buf, size_t len){ cout << string(buf, len) << endl;  return 0; }
    
int body_cb(http_parser *p, const char *buf, size_t len) { cout << string(buf, len) << endl;  return 0; }
    
int headers_complete_cb(http_parser *p) { return 0; }
    
int message_complete_cb(http_parser *p) { cout << "Message Complete" << endl;  return 0; }
    
int response_status_cb(http_parser *p, const char *buf, size_t len) { return 0; }
    
int chunk_header_cb(http_parser *p){ return 0; }
    
int chunk_complete_cb(http_parser *p) { return 0; }

int main(void)
{
	
	const char* data = "POST /hello.htm HTTP/1.1"
		"\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)"
		"\r\nHost: www.tutorialspoint.com"
		"\r\nAccept-Language: en-us"
		"\r\nAccept-Encoding: gzip, deflate"
		"\r\nConnection: Keep-Alive\r\n\r\n dddd";
	
	size_t requestlen = strlen(data);
	http_parser parser;
	http_parser_settings settings;
	settings.on_url = [](http_parser* parser, const char* src, size_t len)-> int{
		cout << string(src, len) << endl;
	
		return 0;
	};

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
	http_parser_init(&parser, http_parser_type::HTTP_BOTH);

	cout << "parsed data length:" <<  http_parser_execute(&parser, &settings, data, requestlen) << endl;
	
	cout << "Request Lenght: " << requestlen << endl;
	return 0;
}