#include "gs.h"

gs::GS::GS()
{
    CornerHor = 0;
    CornerVer = 0;
}

void gs::GS::Init(double cornerHor, double cornerVer)
{
    CornerHor = cornerHor;
    CornerVer = cornerVer;
}

gs::GS_STATUS gs::GS::Process(TargetCoordPelengs *target)
{
    if((target->x)&&(target->y)){
    target->pelengHor = atan( target->x / 320 * tan(CornerHor/2.0*PI/180) )/PI*180 - CornerHor/2.0 ;
    target->pelengVer = CornerVer/2.0 - atan( target->y / 240 * tan(CornerVer/2.0*PI/180) )/PI*180 ;
    }
    return GS_SUCCESS;
}
