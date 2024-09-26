#ifndef AS_H
#define AS_H

#include "structs.h"

#ifdef _WIN32
#include "as/win_trackerDll_vs/trackerDll.h"
#elif defined __linux__
#include "as/astra_libs/libtracker.h"
#endif

namespace as
{
	//Перечислитель ошибок
	enum AS_STATUS
	{
		AS_SUCCESS = 0,	//Успешное выполнение модели
		AS_NULLPTR = 1,	//Один из переданных входных указателей - нулевой
		AS_BADSIZE = 2	//Размер входного изображения не равен 640x480
	};

	class AS
	{
		TrackerDll tracker;

		bool lastRz;

	public:
		AS();
		void Reset();
        AS_STATUS Process(ImageArray tv, ImageArray ir, TargetCoordinates targetTV, TargetCoordinates targetIR, Coordinates* target, bool rz, bool *zg);
	};
}

#endif // AS_H
