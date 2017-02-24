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
public:
	shared_ptr<TcpSocket> socket_;
	char buf_[2048];

	Client(shared_ptr<TcpSocket> sock) :socket_(sock){ memset(buf_, 0, 2048); }
	
	static void handle_read(shared_ptr<Client> client, const asio::error_code& err, std::size_t rlen){
		if (!err)
		{
			client->buf_[rlen] = '\0';
			cout << client->buf_ << endl;
			client->socket_->async_receive(buffer(client->buf_), std::bind(handle_read, client, _1, _2));
		}
		else cout << err.message() << endl;
		
	}

	TcpSocket& getSocket(){ return *(socket_.get()); }


	void run() {
		socket_->async_receive(buffer(buf_), bind(Client::handle_read, shared_ptr<Client>(const_cast<Client*>(this)), _1, _2));
	}

	Client(const Client& other) = delete;
	Client& operator=(const Client& other) = delete;
	Client( Client&& other) = delete;
};

static void server_handle_accept(TcpAcceptor& acceptor, Client& client, const asio::error_code& err){

	client.run();
	/*TcpSocket sock(acceptor.get_io_service());
	acceptor.async_accept(sock,[](const asio::error_code& ec){
		cout << "New Connection" << endl;
	});*/
	

	
}


int main(){

	io_service server_service;
	TcpEndPoint ep(ip::address::from_string("127.0.0.1"), 8000);
	TcpAcceptor server_acc(server_service, ep);
	vector<Client*> clients;
	shared_ptr<TcpSocket> sock_ptr(new TcpSocket(server_service));
	Client client(sock_ptr);
	clients.push_back(&client);
	TcpSocket& sock = client.getSocket();

	server_acc.async_accept(sock, std::bind(server_handle_accept,server_acc, client, _1));
	//[&sock, &client](const asio::error_code& err){
		//every time a connection come we create a new free socket;
		//shared_ptr<TcpSocket> new_sock_ptr(new TcpSocket(sock.get_io_service()));
		//client.run();
		/*sock.async_receive(buffer(client.buf_), [&client](const asio::error_code& err, std::size_t rlen){
			cout << client.buf_ << endl;
		});*/
	//});
	
	server_service.run();

	return 0;
}