#include "fco.h"

using namespace fco;




// //Параметры
// const unsigned int expandSize = 100;
// const unsigned int firstRow = 1;
// const unsigned int lastRow = 479;
// const unsigned int firstColumn = 1;
// const unsigned int lastColumn = 639;

// //Размеры входных изображений - 640x480
// const unsigned int srcWidth = 640;
// const unsigned int srcHeight = 480;
// //Размеры выходных изображений - на expandSize больше с каждой стороны
// const unsigned int dstWidth = 640 + expandSize * 2;
// const unsigned int dstHeight = 480 + expandSize * 2;



FCO::FCO()
{
	tv.image = new unsigned char[dstWidth * dstHeight];
	tv.width = dstWidth;
	tv.height = dstHeight;
	ir.image = new unsigned char[dstWidth * dstHeight];
	ir.width = dstWidth;
	ir.height = dstHeight;
}

FCO::~FCO()
{
	delete[] tv.image;
	delete[] ir.image;
}

FCO_STATUS FCO::Process(const ImageArray srcTV, const ImageArray srcIR, ImageArray *dstTV, ImageArray *dstIR)
{
	//Проверка входных данных
	if(srcTV.image == nullptr || srcIR.image == nullptr || dstTV == nullptr || dstIR == nullptr)
		return FCO_NULLPTR;
	if(srcTV.width != srcWidth || srcTV.height != srcHeight || srcIR.width != srcWidth || srcIR.height != srcHeight)
		return FCO_BADSIZE;

	//Получение центральной области
	for(unsigned int srcRow = firstRow, dstRow = expandSize + firstRow; srcRow <= lastRow; srcRow++, dstRow++)
		for(unsigned int srcColumn = firstColumn, dstColumn = expandSize + firstColumn; srcColumn <= lastColumn; srcColumn++, dstColumn++)
		{
			tv.image[dstRow * dstWidth + dstColumn] = srcTV.image[srcRow * srcWidth + srcColumn];
			ir.image[dstRow * dstWidth + dstColumn] = srcIR.image[srcRow * srcWidth + srcColumn];
		}

	//Зеркалирование краёв
	for(unsigned int srcRow = expandSize + firstRow * 2 - 1, dstRow = expandSize; dstRow < expandSize + firstRow; srcRow--, dstRow++)
		for(unsigned int column = expandSize + firstColumn; column <= expandSize + lastColumn; column++)
		{
			tv.image[dstRow * dstWidth + column] = tv.image[srcRow * dstWidth + column];
			ir.image[dstRow * dstWidth + column] = ir.image[srcRow * dstWidth + column];
		}
	for(unsigned int srcRow = expandSize + lastRow, dstRow = expandSize + lastRow + 1; dstRow < expandSize + srcHeight; srcRow--, dstRow++)
		for(unsigned int column = expandSize + firstColumn; column <= expandSize + lastColumn; column++)
		{
			tv.image[dstRow * dstWidth + column] = tv.image[srcRow * dstWidth + column];
			ir.image[dstRow * dstWidth + column] = ir.image[srcRow * dstWidth + column];
		}
	for(unsigned int row = expandSize; row < expandSize + srcHeight; row++)
	{
		for(unsigned int srcColumn = expandSize + firstColumn * 2 - 1, dstColumn = expandSize; dstColumn < expandSize + firstColumn; srcColumn--, dstColumn++)
		{
			tv.image[row * dstWidth + dstColumn] = tv.image[row * dstWidth + srcColumn];
			ir.image[row * dstWidth + dstColumn] = ir.image[row * dstWidth + srcColumn];
		}
		for(unsigned int srcColumn = expandSize + lastColumn, dstColumn = expandSize + lastColumn + 1; dstColumn < expandSize + srcWidth; srcColumn--, dstColumn++)
		{
			tv.image[row * dstWidth + dstColumn] = tv.image[row * dstWidth + srcColumn];
			ir.image[row * dstWidth + dstColumn] = ir.image[row * dstWidth + srcColumn];
		}
	}

	//Расширение зеркалированием краёв
	for(unsigned int srcRow = expandSize * 2 - 1, dstRow = 0; dstRow < expandSize; srcRow--, dstRow++)
		for(unsigned int column = expandSize; column < expandSize + srcWidth; column++)
		{
			tv.image[dstRow * dstWidth + column] = tv.image[srcRow * dstWidth + column];
			ir.image[dstRow * dstWidth + column] = ir.image[srcRow * dstWidth + column];
		}
	for(unsigned int srcRow = expandSize + srcHeight - 1, dstRow = expandSize + srcHeight; dstRow < dstHeight; srcRow--, dstRow++)
		for(unsigned int column = expandSize; column < expandSize + srcWidth; column++)
		{
			tv.image[dstRow * dstWidth + column] = tv.image[srcRow * dstWidth + column];
			ir.image[dstRow * dstWidth + column] = ir.image[srcRow * dstWidth + column];
		}
	for(unsigned int row = 0; row < dstHeight; row++)
	{
		for(unsigned int srcColumn = expandSize * 2 - 1, dstColumn = 0; dstColumn < expandSize; srcColumn--, dstColumn++)
		{
			tv.image[row * dstWidth + dstColumn] = tv.image[row * dstWidth + srcColumn];
			ir.image[row * dstWidth + dstColumn] = ir.image[row * dstWidth + srcColumn];
		}
		for(unsigned int srcColumn = expandSize + srcWidth - 1, dstColumn = expandSize + srcWidth; dstColumn < dstWidth; srcColumn--, dstColumn++)
		{
			tv.image[row * dstWidth + dstColumn] = tv.image[row * dstWidth + srcColumn];
			ir.image[row * dstWidth + dstColumn] = ir.image[row * dstWidth + srcColumn];
		}
	}

	//Возврат результата
	*dstTV = tv;
	*dstIR = ir;
	return FCO_SUCCESS;
}
