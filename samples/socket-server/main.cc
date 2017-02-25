#include "Client.h"
#include "Server.h"










int main(){

	Server& server = Server::Get();
	server.start_accept();
	server.Run();

	return 0;
}