#pragma once

#undef snprintf
#include <boost/asio.hpp>

#include "../IOStructs.h"

using namespace boost::asio;

OESKN_in* receive_data(io_context& io_context);

void send_TV_data(const Picture_out& pic);

void send_IR_data(const Picture_out& pic);
