#ifndef STRUCTS_H
#define STRUCTS_H
#define PI 3.14159265
#include <cmath>
//Структура для передачи изображения
struct ImageArray
{
	unsigned int width;
	unsigned int height;
	unsigned char *image;
};

//Структура для передачи двумерных координат
struct Coordinates
{
	double x;
	double y;
    double w = 0;
    double h = 0;
};

//Структура для передачи двумерных координат цели
struct TargetCoordinates:Coordinates
{
	bool found;
};

struct TargetCoordPelengs:TargetCoordinates
{
    double pelengHor;
    double pelengVer;
};

#endif // STRUCTS_H
