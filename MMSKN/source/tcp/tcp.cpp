#include <iostream>

#include "IOStructs.h"
#include "img.h"

#include "tcp\tcp.h"

PictureRecieve* recieve_img(io_context& io_context, unsigned tcp_locale_port)
{
	ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), tcp_locale_port));
	ip::tcp::socket socket(io_context);
	acceptor.accept(socket);
	std::vector<uint8_t> m_buffer;
	m_buffer.resize(307212);
	boost::system::error_code ec;
	read(socket, buffer(m_buffer), ec);

	if (ec)
		throw std::runtime_error(std::string("Network error: ") + ec.message());

	auto img = deserialize_img(m_buffer);
	//std::cout << img.height << " | " << img.width << " | " << img.size_pix << std::endl;

	return img;
}

PictureRecieve* recieve_img_TV(io_context& io_context)
{
	unsigned tcp_locale_port = 10101;
	return recieve_img(io_context, tcp_locale_port);
}

PictureRecieve* recieve_img_IR(io_context& io_context)
{
	unsigned tcp_locale_port = 10100;
	return recieve_img(io_context, tcp_locale_port);
}

void send_data(io_context& io_context, const OESKN_out& data)
{
	ip::tcp::resolver resolver(io_context);
	ip::tcp::resolver::results_type endpoints = resolver.resolve("10.25.106.45", "9999");
	ip::tcp::socket socket(io_context);
	boost::system::error_code ec;
	while (true) {
		connect(socket, endpoints, ec);
		if (!ec) {
			write(socket, boost::asio::buffer(&data, sizeof(data)));
			break;
		}
		else
		{
			std::cerr << "Cant't send / skip\n"; //<< ec.message() << std::endl;
			break;
		}

	}
}
