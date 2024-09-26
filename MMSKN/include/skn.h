#ifndef SKN_H
#define SKN_H

#include "structs.h"
#include "fco.h"
#include "fpu.h"
#include "asbvp.h"
#include "as.h"
#include "gk.h"
#include "spobvp.h"
#include "gs.h"
#include <QDebug>


namespace skn
{
	//Перечислители ошибок
	enum SKN_ERROR
	{
		SKN_SUCCESS = 0,	//Успешное выполнение всех моделей
		SKN_NULLPTR = 1		//Один из переданных входных указателей - нулевой
	};
	enum SKN_SUB_ERROR
	{
		SKN_SUB_SUCCESS = 0,	//Успешное выполнение всех моделей
		SKN_FCO_ERROR = 1,		//Ошибка в модели ФЦО
		SKN_FPU_ERROR = 2,		//Ошибка в модели ФПУ
		SKN_SPOBVP_ERROR = 3,	//Ошибка в модели СПОБВП
		SKN_AS_ERROR = 4,		//Ошибка в модели АС
		SKN_GK_ERROR = 5		//Ошибка в модели ГК
	};

	class SKN
	{
		fco::FCO *mmFCO;
		fpu::FPU *mmFPU;
        asbvp::ASBVP *mmASBVP;
		as::AS *mmAS;
		gk::GK *mmGK;
        spobvp::SPOBVP *mmSPOBVP;
        gs::GS *mmGS;

		ImageArray fcoTV, fcoIR, fpuTV, fpuIR;

		TargetCoordinates targetTV, targetIR;
		Coordinates lastTarget, target;

		Coordinates pos;
		Coordinates speed;

		bool zg;
		bool rz;

		int zgDelay;

        int badFrames = 0;

	public:
		SKN();
		~SKN();
		void Reset();
		void SetZgDelay(int delay);
        void InitDetector(QString path_to_model);
        void InitGS(double cornerHor, double cornerVer);
        long long Process(ImageArray tvIn, ImageArray irIn, ImageArray* tvOut, ImageArray* irOut, TargetCoordPelengs *posTarget, double CornerHor, double CornerVer);
		void ResetZG();
	};
}

#endif // SKN_H
