#include "Client.h"
#include "Server.h"
#include "Request.h"
#include "Response.h"
#include "FolderHost.h"

void Client::handle_read(const asio::error_code& err, std::size_t rlen){
    if (!err && !stoped_)
    {
		Request req;
		req.parse(buf_, rlen); //TODO make it asynchronous

		FolderHost fhost(req.get_header("Host"));

		auto url = req.get_url();
		if (url == "/")
		{
			url = "/index.html";
		}
		bool file_exist = fhost.file_exist(url);
		if (!file_exist)
		{
			Response res;
			res.set_status(HTTP_STATUS_NOT_FOUND);
			res.set_header("Content-Length", 0);
			socket_.send(buffer(res.stringify().c_str(), res.stringify().size())); // synchronous send for a small data;

		}
		else
		{
			auto file_mime = fhost.get_file_mime(url);
			auto file_length = fhost.get_file_length(url);
			
			Response res;
			res.set_status(HTTP_STATUS_OK);
			res.set_header("Content-Length", file_length);
			res.set_header("Connection", "keep-alive");
			res.set_header("Content-Type", file_mime);
			char *data = (char*)::malloc(file_length);
			fhost.get_file_content(url, data, file_length);
			string tosend = res.stringify();
			
			memcpy(to_buf, tosend.c_str(), tosend.size());
			if (socket_.is_open())
			{
				socket_.async_send(buffer(to_buf, tosend.size()), [this, &data, file_length](const asio::error_code& ec, std::size_t wr_len){
					socket_.async_send(buffer(data, file_length), [file_length, &data,this](const asio::error_code& ec, std::size_t wr_len2){
						//if (wr_len2 == file_length);
							//::free(data);
						//if (file_length == wr_len2) this->stop();
					});
				});

			}

		}

		/*
		Response res;
		res.set_status(HTTP_STATUS_OK);
		res.set_header("Content-Type", "text/html");
		if (req.get_url() == "/favicon.ico")
		{
			res.set_header("Content-Type", "image/x-icon");
			auto filelen = fhost.get_file_length(req.get_url());
			auto mime = fhost.get_file_mime(req.get_url());
			res.set_header("Content-Length", filelen);
			res.set_header("Connection", "keep-alive");
			char *data = (char*)::malloc(filelen);
			fhost.get_file_content(req.get_url(), data, filelen);
			string tosend = res.stringify();
			memcpy(to_buf, tosend.c_str(), tosend.size());
			if (socket_.is_open())
			{
				socket_.async_send(buffer(to_buf,tosend.size()), [this,&data,filelen](const asio::error_code& ec, std::size_t wr_len){
					socket_.async_send(buffer(data, filelen), [filelen,&data](const asio::error_code& ec, std::size_t wr_len){
						if (wr_len == filelen)
							::free(data);
					});
				});
				
			}
				

		}
		else
		{
			char data[] = "<html><head><title>From asio</title></head><body>Message From Asio</body></html>";
			res.set_header("Content-Length", std::to_string(strlen(data)));
			res.set_header("Connection", "keep-alive");
			string tosend = res.stringify() + data;
			memcpy(to_buf, tosend.c_str(), tosend.size());
			if (socket_.is_open())
				socket_.async_send(buffer(to_buf), [this](const asio::error_code& ec, std::size_t wr_len){



			});
		}
		*/
		
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

