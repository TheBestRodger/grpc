#include "fpu.h"

#include <math.h>

using namespace fpu;

//Размеры выходных изображений - 640x480
const unsigned int dstWidth = 640;
const unsigned int dstHeight = 480;

FPU::FPU()
{
	tv.image = new unsigned char[dstWidth * dstHeight];
	tv.width = dstWidth;
	tv.height = dstHeight;
	ir.image = new unsigned char[dstWidth * dstHeight];
	ir.width = dstWidth;
	ir.height = dstHeight;
}

FPU::~FPU()
{
	delete[] tv.image;
	delete[] ir.image;
}

FPU_STATUS FPU::Process(const ImageArray srcTV, const ImageArray srcIR, ImageArray *dstTV, ImageArray *dstIR, Coordinates centerOffset)
{
	//Проверка входных данных
	if(srcTV.image == nullptr || srcIR.image == nullptr || dstTV == nullptr || dstIR == nullptr)
		return FPU_NULLPTR;
	if(srcTV.width < dstWidth || srcTV.height < dstHeight || srcTV.width != srcIR.width || srcTV.height != srcIR.height)
		return FPU_BADSIZE;

	//Исходные размеры
	int srcWidth = srcTV.width;
	int srcHeight = srcTV.height;

	//Выбор сегмента изображения
	int firstRow = lround(srcHeight / 2 - centerOffset.y - dstHeight / 2 - 0.5);
	int lastRow = firstRow + dstHeight - 1;
	int firstColumn = lround(srcWidth / 2 - centerOffset.x - dstWidth / 2 - 0.5);
	int lastColumn = firstColumn + dstWidth - 1;
	if(firstRow < 0 || firstColumn < 0 || lastRow >= srcHeight || lastColumn >= srcWidth)
		return FPU_OUTSIDE;

	//Получение выбранной области
	for(unsigned int srcRow = firstRow, dstRow = 0; dstRow < dstHeight; srcRow++, dstRow++)
		for(unsigned int srcColumn = firstColumn, dstColumn = 0; dstColumn < dstWidth; srcColumn++, dstColumn++)
		{
			tv.image[dstRow * dstWidth + dstColumn] = srcTV.image[srcRow * srcWidth + srcColumn] >> 1;
			ir.image[dstRow * dstWidth + dstColumn] = srcIR.image[srcRow * srcWidth + srcColumn] >> 1;
		}

	//Возврат результата
	*dstTV = tv;
	*dstIR = ir;
	return FPU_SUCCESS;
}
