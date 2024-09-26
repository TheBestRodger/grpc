#ifndef GS_H
#define GS_H


#include "structs.h"

namespace gs
{
//Перечислитель ошибок
enum GS_STATUS
{
    GS_SUCCESS = 0,	//Успешное выполнение модели
    GK_NULLPTR = 1		//Один из переданных входных указателей - нулевой
};

class GS
{
public:
    double CornerHor;
    double CornerVer;
    GS();
    void Init(double cornerHor, double cornerVer);
    GS_STATUS Process(TargetCoordPelengs *target);
};
}
#endif // GS_H
