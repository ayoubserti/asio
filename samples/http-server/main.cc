#include "Request.h"
#include "Response.h"
#include "Server.h"

/*	const char* data = "GET /hello.html HTTP/1.1"
"\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)"
"\r\nHost: www.tutorialspoint.com"
"\r\nAccept-Language: en-us"
"\r\nAccept-Encoding: gzip, deflate"
"\r\nContent-Length: 5"
"\r\nContent-Type: application/x-binary"
"\r\nConnection: Keep-Alive\r\n\r\n dddd";
*/

int main(void)
{
	
	Server& server = Server::Get();
	server.start_accept();
	server.Run();

	return 0;
}