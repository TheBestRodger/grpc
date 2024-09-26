#include "asbvp.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>

using namespace eac;
using namespace asbvp;



ASBVP_STATUS ASBVP::Process(const ImageArray tv, const ImageArray ir, TargetCoordinates *targetTV, TargetCoordinates *targetIR, TargetCoordinates detectTarget, Coordinates TargetZG)
{


	//Размеры входных изображений - 640x480
	const unsigned int srcWidth = 640;
	const unsigned int srcHeight = 480;

	//Проверка входных данных
	if(tv.image == nullptr || ir.image == nullptr || targetTV == nullptr || targetIR == nullptr)
        return ASBVP_NULLPTR;
	if(tv.width != srcWidth || tv.height != srcHeight || ir.width != srcWidth || ir.height != srcHeight)
        return ASBVP_BADSIZE;

    //qDebug()<<detectTarget.x<<detectTarget.y;

    if((detectTarget.x >= 0) && (detectTarget.y >=0))
    {
        if((abs(TargetZG.x - detectTarget.x) >= detectTarget.w)||(abs(TargetZG.y - detectTarget.y) >= detectTarget.h))
            flag_ZG = true;
        targetTV->found = true;
        targetTV->x = detectTarget.x;
        targetTV->y = detectTarget.y;
        targetIR->found = true;
        targetIR->x = detectTarget.x;
        targetIR->y = detectTarget.y;
    }
    else
    {
        //flag_ZG = false;
        targetTV->found = false;
        targetIR->found = false;
    }




    return ASBVP_SUCCESS;
}
