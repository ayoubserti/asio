#include <iostream>
#include <functional>
#include <memory>
#include <vector>

#define  _WIN32_WINNT  0x0601

#define ASIO_STANDALONE 1
#include "asio.hpp"


using namespace std;
using namespace asio;
using TcpEndPoint = ip::tcp::endpoint;
using TcpSocket = ip::tcp::socket;
using TcpAcceptor = ip::tcp::acceptor;


using  namespace std::placeholders;

class Server;

class Client
{

	Client(const Client& other) = delete;
	Client& operator=(const Client& other) = delete;
	Client(Client&& other) = delete;

	void handle_read(const asio::error_code& err, std::size_t rlen);
	
public:


	Client(Server& server) ;
	TcpSocket& socket(){ return socket_; }


	void run();
	void stop();
	~Client(){
		//on_close();
	}
	void on_close();
private:
	Server&		server_;
	TcpSocket	socket_;
	bool		stoped_ = false;
	char buf_[100];
	

	
};