#ifndef GK_H
#define GK_H

#include "structs.h"

namespace gk
{
	//Перечислитель ошибок
	enum GK_STATUS
	{
		GK_SUCCESS = 0,	//Успешное выполнение модели
		GK_NULLPTR = 1		//Один из переданных входных указателей - нулевой
	};

	class GK
	{
    public:
		GK_STATUS Process(Coordinates pos, Coordinates speed, Coordinates *newPos, Coordinates *newSpeed, Coordinates lastTarget, Coordinates target, bool zg, Coordinates disturb);
	};
}

#endif // GK_H
