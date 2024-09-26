#pragma once

#pragma pack(push, 1)

struct OESKN_in
{
	// (для Терции-2)
	// Текущая дальность от системы управления изделия [м]
	unsigned int distance{};	//Controlsys RVector r

	// (для Терции-2) Угол поворота эталона [рад]
	double gam_i{};				//TmData R_et, psi_et,R_et.y

	// Координаты изделия struct mmdata
	double B{};					// широта  [гр] mislemotion b
	double L{};					// долгота [гр] b
	double H{};					// высота  [м]  R.y

	// Поворот изделия (курсовая схема поворота) mislemotion
	double tet{};				// угол тангажа  [рад] orn.t
	double psi{};				// угол рыскания [рад] orn.p
	double gam{};				// угол крена    [рад] orn.g

	// Целеуказание (тангажная схема поворота для Терции-2) HHdata
	double fiv_z{};				// вертикальный пеленг цели   [рад] phig
	double fig_z{};				// горизонтальный пеленг цели [рад] phiv

	// Режим работы:
	// == 0 - Дежурный
	// != 0 - Рабочий
	int mode{};

	// Разовые команды
	int frontier{};				// Выход на рубеж
};

struct Picture_out
{
	std::vector<uint8_t> pix;
	unsigned size_pix{};

	unsigned height{};
	unsigned width{};
};

struct OESKN_out
{
	// Углы поворота координатора (тангажная схема поворота для Терции-2)
	double fiv{};				// вертикальный угол   [рад]
	double fig{};				// горизонтальный угол [рад]

	// Скорости поворота координатора
	double omv{};				// вертикальная   [рад/с]
	double omg{};				// горизонтальная [рад/с]

	// Углы пеленга цели в связанной СК
	double fiv_c{};				// вертикальный угол   [рад]
	double fig_c{};				// горизонтальный угол [рад]

	// Признак захвата цели
	int ZG{};
};

#pragma pack(pop)