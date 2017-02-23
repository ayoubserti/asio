#include <iostream>
#include <functional>
#include <memory>

#define ASIO_STANDALONE 1
#include "asio.hpp"


using namespace std;
using namespace asio;
using TcpEndPoint = ip::tcp::endpoint;
using TcpSocket = ip::tcp::socket;
using  namespace std::placeholders;

template<class T>
static void Handler(shared_ptr<TcpSocket> socket, T& buf, const asio::error_code& ec, std::size_t bytes_received){

	if (!ec){
		cout << buf << endl;
		socket->async_receive(buffer(buf,100), bind(Handler<T>, socket, buf, _1, _2));
	}
}
struct Reciever
{
	char buf_[100];
	shared_ptr<TcpSocket> socket_;
	Reciever(shared_ptr<TcpSocket> socket) :socket_(socket){ /*buf_.reserve(100);*/ }
	
	void operator()(){
		socket_->async_receive(buffer(buf_,100), bind(Handler<char*>,socket_,buf_,_1,_2));
	}

	
};



int main()
{
	io_service myservice;
	shared_ptr<TcpSocket> sock(new TcpSocket(myservice));
	TcpEndPoint ep(ip::address::from_string("77.238.184.150"),80);
	Reciever recv(sock);

	sock->async_connect(ep, [&sock, &recv](const asio::error_code& ec){
		if (!ec)
		{
			//sock.async_send()
			sock->async_send(buffer("GET / HTTP1.1\nhost:www.yahoo.fr\nuser-agent:Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36\n\n"), [&sock,&recv](const asio::error_code& ec, std::size_t bytes_transferred){
				cout << bytes_transferred << endl;
				if (!ec){

					recv();					
				}
			});
		}
	});

	myservice.run();




    return 0;
}