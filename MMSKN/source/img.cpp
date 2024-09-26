#include <iostream>

#include "img.h"

PictureRecieve* deserialize_img(const std::vector<uint8_t>& data)
{
	PictureRecieve* img = new PictureRecieve();
	std::memcpy(&img->height, data.data(), sizeof(unsigned));
	std::memcpy(&img->width, data.data() + sizeof(unsigned), sizeof(unsigned));
	std::memcpy(&img->size_pix, data.data() + 2 * sizeof(unsigned), sizeof(unsigned));

	std::cout << img->height << " | " << img->width << " | " << img->size_pix << std::endl;
	img->pix.resize(img->size_pix);
	std::memcpy(img->pix.data(), data.data() + 3 * sizeof(unsigned), img->size_pix);
	for (int i = 0; i < img->size_pix; ++i)
		if ((int)img->pix[i] != 0)
		{
			std::cout << (int)img->pix[i] << " ";
			break;
		}
	return img;
}
