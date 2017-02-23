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

class Reciever;

template<class T>
 void Handler(shared_ptr<Reciever> reciever, const asio::error_code& ec, std::size_t bytes_received){

	if (!ec){
		//synchronously write file 
		FILE* file = fopen("file.html", "a+");
		fwrite(reciever->buf_,1, bytes_received, file);
		fclose(file);
		reciever->total_read_len_ += bytes_received;
		reciever->socket_->async_receive(buffer(reciever->buf_), std::bind(Handler<T>, reciever, _1,_2));
	}
	if (ec == asio::error::eof)
	{
		cout << reciever->total_read_len_ << endl;
		std::cout << "EOF" << std::endl;
		
	}
}
class Reciever 
{

public:
	char buf_[100];
	std::size_t total_read_len_;
	shared_ptr<TcpSocket> socket_;
	
	windows::stream_handle sh_;

	/*template<class T>
	friend void Handler(shared_ptr<Reciever> reciever, shared_ptr<TcpSocket> socket, T& buf, const asio::error_code& ec, std::size_t bytes_received);*/

	Reciever(shared_ptr<TcpSocket> socket) :total_read_len_(0),socket_(socket),sh_(socket->get_io_service()){  }
	Reciever(const Reciever& other) : Reciever(other.socket_){}
	
	void operator()(){
		socket_->async_receive(buffer(buf_), bind(Handler<char*>, make_shared<Reciever>(const_cast<Reciever&>(*this)), _1, _2));
	}

	
};



int main()
{
	io_service myservice;
	shared_ptr<TcpSocket> sock(new TcpSocket(myservice));
	TcpEndPoint ep(ip::address::from_string("185.53.179.8"),80);
	Reciever recv(sock);

	sock->async_connect(ep, [&sock, &recv](const asio::error_code& ec){
		if (!ec)
		{
			char* header = "GET /docs/index.html HTTP/1.1\n"
				"Host: www.nowhere123.com\n"
				"Accept : image / gif, image / jpeg, */*\n"
				"Accept-Language: en-us\n"
				
				"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\n\n";

			sock->async_send(buffer(header,strlen(header)), [&sock, &recv](const asio::error_code& ec, std::size_t bytes_transferred){
				
				if (!ec){

					recv();					
				}
			});
		}
	});

	myservice.run();




    return 0;
}