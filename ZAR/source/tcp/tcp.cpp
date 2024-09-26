#include <iostream>

#include "img.h"
#include "tcp/tcp.h"

OESKN_in* receive_data(io_context& io_context)
{
	unsigned int m_tcp_local_port = 9334;
	std::string  m_tcp_remote_ip = "10.25.106.45";
	ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), m_tcp_local_port));// ip + port
	ip::tcp::socket socket(io_context);
	acceptor.accept(socket);
	std::vector<char> m_buffer;
	m_buffer.resize(sizeof(OESKN_in));
	boost::system::error_code ec;
	read(socket, buffer(m_buffer), ec);
	if (ec)
		throw std::runtime_error(std::string("Network error: ") + ec.message());

	OESKN_in* p_data = reinterpret_cast<OESKN_in*>(m_buffer.data());

	std::cout << "Distance: " << p_data->distance << std::endl;
	std::cout << "Latitude: " << p_data->B << " Longitude: " << p_data->L << " Height: " << p_data->H << std::endl;

	return p_data;
}

void send_TV_data(const Picture_out& pic)
{
	boost::asio::io_context io_context;
	ip::tcp::resolver resolver(io_context);
	ip::tcp::resolver::results_type endpoint = resolver.resolve("10.25.106.45", "10101");
	ip::tcp::socket socket(io_context);

	boost::system::error_code ec;
	while (true) {
		connect(socket, endpoint, ec);
		if (!ec) {
			std::vector<uint8_t> data = serialize_img(pic);

			std::cout << data.size() << std::endl;
			write(socket, buffer(data, data.size()));
			break;
		}
		else
		{
			std::cerr << "TV Reconecting... \n"; //<< ec.message() << std::endl;
		}
	}
}

void send_IR_data(const Picture_out& pic)
{
	boost::asio::io_context io_context;
	ip::tcp::resolver resolver(io_context);
	ip::tcp::resolver::results_type endpoint = resolver.resolve("10.25.106.45", "10100");
	ip::tcp::socket socket(io_context);
	boost::system::error_code ec;
	while (true) {
		connect(socket, endpoint, ec);
		if (!ec) {
			std::vector<uint8_t> data = serialize_img(pic);

			std::cout << data.size() << std::endl;
			write(socket, buffer(data, data.size()));
			break;
		}
		else
		{
			std::cerr << "IR Reconecting... \n"; //<< ec.message() << std::endl;
		}

	}
}
