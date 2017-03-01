#include "Client.h"
#include "Server.h"
#include "Request.h"
#include "Response.h"

void Client::handle_read(const asio::error_code& err, std::size_t rlen){
    if (!err && !stoped_)
    {
		Request req;
		req.parse(buf_, rlen);

		Response res;
		res.set_status(HTTP_STATUS_OK);
		res.set_header("Content-Type", "text/html");
		char data[] = "<html><head><title>From asio</title></head><body>hhhhhhhhhhhhhhhhhhhhhh</body></html>";
		res.set_header("Content-Length", std::to_string(strlen(data)));
		res.set_header("Connection", "keep-alive");
		string tosend = res.stringify()+data; 
		memcpy(to_buf, tosend.c_str(), tosend.size());
		if (socket_.is_open())
			socket_.async_send(buffer(to_buf ), [this](const asio::error_code& ec, std::size_t wr_len){
			
			
			
			});
		run();
        
    }
    else if (err == asio::stream_errc::eof)
    {
		stop();
    }
	else
	{
        cout << err.message() << endl;
		stop();

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

