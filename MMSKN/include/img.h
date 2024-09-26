#pragma once

#include <vector>
#include "IOStructs.h"

PictureRecieve* deserialize_img(const std::vector<uint8_t>& data);
