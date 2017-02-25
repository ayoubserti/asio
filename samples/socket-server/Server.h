#include <iostream>
#include <functional>
#include <memory>
#include <vector>

#define _WIN32_WINNT  0x0601
#define ASIO_STANDALONE 1
#include "asio.hpp"


using namespace std;
using namespace asio;
using TcpEndPoint = ip::tcp::endpoint;
using TcpSocket = ip::tcp::socket;
using TcpAcceptor = ip::tcp::acceptor;

using  namespace std::placeholders;

class Client;

class Server
{
	static Server*  sInstance;
	io_service		service_;
	TcpEndPoint		ep_;
	TcpAcceptor		acceptor_;
	
	using  ClientVector = vector<shared_ptr<Client>>;
	ClientVector clients_;
	
	
	Server();


	Server(const Server&) = delete;
	Server(Server&&) = delete;
	Server operator=(const Server&) = delete;
	Server operator=(Server&&) = delete;


	//accept handler
	void accept_handler(shared_ptr<Client> client, const asio::error_code& ec);
	
	void unregister_client(const Client* client);

	friend class Client;

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
		service_.run();
	}

    void start_accept();

	io_service& get_io_service() {
		return service_;
	}

	 


};
