#include <iostream>
#include <functional>
#include <memory>


#define ASIO_STANDALONE 1
#include "asio.hpp"


using namespace std;
using namespace asio;
using TcpEndPoint = ip::tcp::endpoint;
using TcpSocket = ip::tcp::socket;
using TcpAcceptor = ip::tcp::acceptor;

using  namespace std::placeholders;


class Client
{

	Client(const Client& other) = delete;
	Client& operator=(const Client& other) = delete;
	Client(Client&& other) = delete;
		
	void handle_read( const asio::error_code& err, std::size_t rlen){
		if (!err)
		{
			buf_[rlen] = '\0';
			cout << buf_ << endl;
			run();
		}
		else {
			cout << err.message() << endl;
			
		}
		
	}
public:


	Client(io_service& service) :socket_(service){ memset(buf_, 0, 2048); }
	TcpSocket& socket(){ return socket_; }


	void run() {
		socket_.async_receive(buffer(buf_), bind(&Client::handle_read, this, _1, _2));
	}

private:
	TcpSocket	socket_;
	char buf_[2048];

	
};


class Server
{
	static Server*  sInstance;
	io_service		service_;
	TcpEndPoint		ep_;
	TcpAcceptor		acceptor_;
	
	Server() :ep_(ip::address::from_string("127.0.0.1"), 8100),acceptor_(service_,ep_){
		
	}


	Server(const Server&) = delete;
	Server(Server&&) = delete;
	Server operator=(const Server&) = delete;
	Server operator=(Server&&) = delete;


	//accept handler
	void accept_handler(shared_ptr<Client> client, const asio::error_code& ec){
		if (!ec)
			client->run();

	//	Start_Accept();
		
	}

public:
	static Server& Get()
	{
		if (sInstance == nullptr)
		{
			sInstance = new Server();
		}
		return *sInstance;
	}

	void Run()
	{

		shared_ptr<Client> client(new Client(service_));
		acceptor_.async_accept(client->socket(), std::bind(&Server::accept_handler, this, client, _1));
		service_.run();
	}

	void Start_Accept()
	{

		shared_ptr<Client> client(new Client(service_));
		acceptor_.async_accept(client->socket(), std::bind(&Server::accept_handler, this, client, _1));
	}

	 


};

Server* Server::sInstance = nullptr;





int main(){

	Server& server = Server::Get();
	//server.Start_Accept();
	server.Run();

	/*io_service server_service;
	TcpEndPoint ep(ip::address::from_string("127.0.0.1"), 8000);
	shared_ptr<TcpAcceptor> server_acc(new TcpAcceptor(server_service, ep));
	//vector<shared_ptr<Client>> clients;
	shared_ptr<TcpSocket> sock_ptr(new TcpSocket(server_service));
	shared_ptr<Client> client(new Client(sock_ptr));
	//clients.push_back(client);
	TcpSocket& sock = client->getSocket();


	server_acc->async_accept(sock, std::bind(server_handle_accept, server_acc, client, _1));

	
	server_service.run();*/

	return 0;
}