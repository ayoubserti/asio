#include "Client.h"
#include "Server.h"

void Client::handle_read(const asio::error_code& err, std::size_t rlen){
    if (!err && !stoped_)
    {
        buf_[rlen] = '\0';
        cout << buf_ << endl;
		
		if (socket_.is_open())
			socket_.async_send(buffer("HTTP/1.1 200 OK\nConnection: Closed\n Content-Length: 17\n\n Hello from Asio\n"), [this](const asio::error_code& ec, std::size_t wr_len){
			
				this->stop();
			
			});
		run();
        
    }
    else if (err == asio::stream_errc::eof)
    {
		//stop();
    }
	else
	{
        cout << err.message() << endl;

    }
}

Client::Client(Server& server) :server_(server),
     socket_(server.get_io_service())
{
     //memset(buf_, 0, 512); 
}

void Client::run() 
{
	if (!stoped_)
		socket_.async_receive(buffer(buf_), bind(&Client::handle_read, this, _1, _2));
}

void Client::on_close()
{
	stoped_ = true;
    server_.unregister_client(this);
}

void Client::stop()
{
	asio::error_code ec;
	socket_.close(ec);
	stoped_ = true;
	on_close();
}

