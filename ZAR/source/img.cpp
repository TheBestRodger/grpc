#include <iostream>

#include "img.h"

std::vector<uint8_t> serialize_img(const Picture_out& img)
{
	std::cout << img.height << " | " << img.width << " | " << img.size_pix << std::endl;
	std::vector<uint8_t> data(sizeof(unsigned) * 3 + img.size_pix);
	std::memcpy(data.data(), &img.height, sizeof(unsigned));
	std::memcpy(data.data() + sizeof(unsigned), &img.width, sizeof(unsigned));
	std::memcpy(data.data() + 2 * sizeof(unsigned), &img.size_pix, sizeof(unsigned));
	std::memcpy(data.data() + 3 * sizeof(unsigned), img.pix.data(), img.size_pix);

	return data;
}

Picture_out deserialize_img(const std::vector<uint8_t>& data)
{
	Picture_out img;

	std::memcpy(&img.height, data.data(), sizeof(unsigned));
	std::memcpy(&img.width, data.data() + sizeof(unsigned), sizeof(unsigned));
	std::memcpy(&img.size_pix, data.data() + 2 * sizeof(unsigned), sizeof(unsigned));
	img.pix.resize(img.size_pix);
	std::memcpy(img.pix.data(), data.data() + 3 * sizeof(unsigned), img.size_pix);

	std::cout << img.height << " | " << img.width << " | " << img.size_pix << std::endl;
	for (int i = 0; i < img.size_pix; ++i)
		if ((int)img.pix[i] != 0) {
			std::cout << (int)img.pix[i] << " " << i << std::endl;
			break;
		}
	return img;
}

