#ifndef FPU_H
#define FPU_H

#include "structs.h"

namespace fpu
{
	//Перечислитель ошибок
	enum FPU_STATUS
	{
		FPU_SUCCESS = 0,	//Успешное выполнение модели
		FPU_NULLPTR = 1,	//Один из переданных входных указателей - нулевой
		FPU_BADSIZE = 2,	//Размер входного изображения меньше 640x480 либо размеры ТВ и ТПВ изображений разные
		FPU_OUTSIDE = 3		//Выходное изображение выходит за пределы входного при указанном смещении
	};

	class FPU
	{
		ImageArray tv, ir;

	public:
		FPU();
		~FPU();
		FPU_STATUS Process(const ImageArray srcTV, const ImageArray srcIR, ImageArray *dstTV, ImageArray *dstIR, Coordinates centerOffset);
	};
}

#endif // FPU_H
