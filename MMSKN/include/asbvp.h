#ifndef ASBVP_H
#define ASBVP_H
#include "eac_errors.h"
#include "object_detection_types.h"
#include "idetector.h"
#include "structs.h"
#include <cmath>
#include <QDebug>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
namespace asbvp
{
	//Перечислитель ошибок
    enum ASBVP_STATUS
	{
        ASBVP_SUCCESS = 0,	//Успешное выполнение модели
        ASBVP_NULLPTR = 1,	//Один из переданных входных указателей - нулевой
        ASBVP_BADSIZE = 2	//Размер входного изображения не равен 640x480
	};

    class ASBVP
	{
	public:
        bool flag_ZG = false;
        ASBVP_STATUS Process(const ImageArray tv, const ImageArray ir, TargetCoordinates *targetTV, TargetCoordinates *targetIR, TargetCoordinates detectTarget, Coordinates TargetZG);
	};
}







#endif // ASBVP_H
