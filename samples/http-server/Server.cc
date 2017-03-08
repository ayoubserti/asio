#include "Server.h"
#include "Client.h"

Server::Server() 
	:ep_(ip::address_v4::any(), 8100)
,acceptor_(service_,ep_)
{
		
}

//accept handler
void Server::accept_handler(shared_ptr<Client> client, const asio::error_code& ec)
{
		if (!ec)
			client->run();

		start_accept();
}


void Server::unregister_client(const Client* client){
		ClientVector::iterator it =  std::find_if(clients_.begin(), clients_.end(), [&client](shared_ptr<Client> elem)->bool {
			if (client == elem.get())
			{
				return true;
			}
			return false;
		});

		clients_.erase(it);
}

void Server::start_accept()
{

    shared_ptr<Client> client(new Client(*sInstance));
    clients_.push_back(client); //keep trace of connected client
    acceptor_.async_accept(client->socket(), std::bind(&Server::accept_handler, this, client, _1));
}

Server* Server::sInstance = nullptr;