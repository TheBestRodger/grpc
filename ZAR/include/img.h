#pragma once

#include <vector>

#include "IOStructs.h"

std::vector<uint8_t> serialize_img(const Picture_out& img);

Picture_out deserialize_img(const std::vector<uint8_t>& data);
