#include <iostream>

#define ASIO_STANDALONE 1
#include "asio.hpp"


using namespace std;
using namespace asio;
using TcpEndPoint = ip::tcp::endpoint;
using TcpSocket = ip::tcp::socket;

int main()
{
	io_service myservice;
	TcpSocket sock(myservice);
	TcpEndPoint ep(ip::address::from_string("77.238.184.150"),80);
	sock.async_connect(ep, [&sock](const asio::error_code& ec){
		if (!ec)
		{
			//sock.async_send()
			sock.async_send(buffer("GET / HTTP1.1\nhost:www.yahoo.fr\nuser-agent:Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36\n\n"), [&sock](const asio::error_code& ec, std::size_t bytes_transferred){
				cout << bytes_transferred << endl;
				if (!ec){
					char data[8000];
					auto f = [&sock, &data](const asio::error_code& ec, std::size_t bytes_transferred){
						data[bytes_transferred] = 0;
						cout << data << endl;
						
					};
					sock.async_receive(buffer(data),f);
					
				}
			});
		}
	});

	myservice.run();




    return 0;
}