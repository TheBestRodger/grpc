#include "as.h"

using namespace as;

AS::AS()
{
	Reset();
}

void AS::Reset()
{
	lastRz = false;
}

AS_STATUS AS::Process(ImageArray tv, ImageArray ir, TargetCoordinates targetTV, TargetCoordinates targetIR, Coordinates *target, bool rz, bool *zg)
{
	//Размеры входных изображений - 640x480
	const unsigned int width = 640;
	const unsigned int height = 480;

	//Проверка входных данных
	if(tv.image == nullptr || ir.image == nullptr || target == nullptr || zg == nullptr)
		return AS_NULLPTR;
	if(tv.width != width || tv.height != height || ir.width != width || ir.height != height)
		return AS_BADSIZE;

	if(rz)
	{
        if(!lastRz)
        {
            tracker.SetCU(int(targetTV.y), int(targetTV.x), true);
            tracker.SetCU(int(targetIR.y), int(targetIR.x), false);
        }
        lastRz = rz;

		int y, x;
		tracker.Tracker((signed char*)tv.image, (signed char*)ir.image, y, x, *zg, true);
		target->x = x;
		target->y = y;
	}
	else
		*zg = false;

	return AS_SUCCESS;
}
