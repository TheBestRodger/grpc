#include "gk.h"

using namespace gk;

GK_STATUS GK::Process(Coordinates pos, Coordinates speed, Coordinates *newPos, Coordinates *newSpeed, Coordinates lastTarget, Coordinates target, bool zg, Coordinates disturb)
{
	//Проверка входных данных
	if(newPos == nullptr || newSpeed == nullptr)
		return GK_NULLPTR;

	//Параметры
    const double leftLimit = -400;
    const double rightLimit = 400;
    const double topLimit = -400;
    const double bottomLimit = 400;
	const double horSpeedLimit = 1000;
	const double verSpeedLimit = 1000;
    const double speedMulti = 0.75;

	Coordinates targetOffset;

	if(zg)
	{
		//Определение смещения цели
		targetOffset.x = lastTarget.x - target.x;
		targetOffset.y = lastTarget.y - target.y;

		//Перемещение к цели
        newSpeed->x = targetOffset.x * 50.0;
        newSpeed->y = targetOffset.y * 50.0;
	}
	else
	{
		//Возврат к началу координат
		newSpeed->x = -pos.x;
		newSpeed->y = -pos.y;
	}

	//Определение скорости
	newSpeed->x *= speedMulti;
	newSpeed->y *= speedMulti;
	if(newSpeed->x > horSpeedLimit)
		newSpeed->x = horSpeedLimit;
	else if(newSpeed->x < -horSpeedLimit)
		newSpeed->x = -horSpeedLimit;
	if(newSpeed->y > verSpeedLimit)
		newSpeed->y = verSpeedLimit;
	else if(newSpeed->y < -verSpeedLimit)
		newSpeed->y = -verSpeedLimit;

	//Определение нового положения
	newPos->x = pos.x + newSpeed->x / 50.0 + disturb.x;
	newPos->y = pos.y + newSpeed->y / 50.0 + disturb.y;
	if(newPos->x < leftLimit)
		newPos->x = leftLimit;
	else if(newPos->x > rightLimit)
		newPos->x = rightLimit;
	if(newPos->y < topLimit)
		newPos->y = topLimit;
	else if(newPos->y > bottomLimit)
		newPos->y = bottomLimit;

	return GK_SUCCESS;
}
