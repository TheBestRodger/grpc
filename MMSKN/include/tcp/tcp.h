#pragma once

#include <boost/asio.hpp>

#include "../IOStructs.h"

using namespace boost::asio;

PictureRecieve* recieve_img_TV(io_context& io_context);

PictureRecieve* recieve_img_IR(io_context& io_context);

void send_data(io_context& io_context, const OESKN_out& data);

