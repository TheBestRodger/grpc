#ifndef FCO_H
#define FCO_H

#include "structs.h"

namespace fco
{

    //Параметры
    const unsigned int expandSize = 450;
    const unsigned int firstRow = 1;
    const unsigned int lastRow = 479;
    const unsigned int firstColumn = 1;
    const unsigned int lastColumn = 639;

    //Размеры входных изображений - 640x480
    const unsigned int srcWidth = 640;
    const unsigned int srcHeight = 480;
    //Размеры выходных изображений - на expandSize больше с каждой стороны
    const unsigned int dstWidth = 640 + expandSize * 2;
    const unsigned int dstHeight = 480 + expandSize * 2;


	//Перечислитель ошибок
	enum FCO_STATUS
	{
		FCO_SUCCESS = 0,	//Успешное выполнение модели
		FCO_NULLPTR = 1,	//Один из переданных входных указателей - нулевой
		FCO_BADSIZE = 2		//Размер входного изображения не равен 640x480
	};

	class FCO
	{
		ImageArray tv, ir;

	public:
		FCO();
		~FCO();
		FCO_STATUS Process(const ImageArray srcTV, const ImageArray srcIR, ImageArray *dstTV, ImageArray *dstIR);
	};
}

#endif // FCO_H
