#include "skn.h"

using namespace skn;

SKN::SKN()
{
	mmFCO = new fco::FCO;
	mmFPU = new fpu::FPU;
    mmASBVP = new asbvp::ASBVP;
	mmAS = new as::AS;
	mmGK = new gk::GK;
    mmSPOBVP = new spobvp::SPOBVP;
    mmGS = new gs::GS;

	Reset();
}

SKN::~SKN()
{
	delete mmFCO;
	delete mmFPU;
    delete mmASBVP;
    delete mmSPOBVP;
	delete mmAS;
	delete mmGK;
}

void SKN::Reset()
{
	pos.x = 0;
	pos.y = 0;
	speed.x = 0;
	speed.y = 0;

    target.x = 0;
    target.y = 0;
	lastTarget = target;

	fcoTV.width = 0;
	fcoTV.height = 0;
	fcoTV.image = nullptr;
	fcoIR = fcoTV;
	fpuTV = fcoTV;
	fpuIR = fpuTV;

	zg = false;
	rz = false;

	zgDelay = 0;
    badFrames = 0;
    mmASBVP->flag_ZG = true;

    //mmSPOBVP->InitDetector("C:\\Users\\User\\Documents\\KMM\\KMM\\objectdetection_technic\\ssd-resnet18.onnx");

	mmAS->Reset();
}

void SKN::SetZgDelay(int delay)
{
	zgDelay = delay;
}

void SKN::InitDetector(QString path_to_model)
{
     mmSPOBVP->InitDetector(path_to_model);
}

void SKN::InitGS(double cornerHor, double cornerVer)
{
    mmGS->Init(cornerHor, cornerVer);
}


long long SKN::Process(ImageArray tvIn, ImageArray irIn, ImageArray *tvOut, ImageArray *irOut, TargetCoordPelengs *posTarget, double CornerHor, double CornerVer)
{
	//Проверка входных данных
	if(tvIn.image == nullptr || irIn.image == nullptr || tvOut == nullptr || irOut == nullptr || posTarget == nullptr)
		return SKN_NULLPTR;

	int retCode;

    //Передача изображений на модель ФЦО
    retCode = mmFCO->Process(tvIn, irIn, &fcoTV, &fcoIR);
    if(retCode != fco::FCO_SUCCESS)
        return retCode + ((long long)SKN_FCO_ERROR << 32);

    //Передача ФЦО на модель ФПУ
    retCode = mmFPU->Process(fcoTV, fcoIR, &fpuTV, &fpuIR, pos);
    if(retCode != fpu::FPU_SUCCESS)
        return retCode + ((long long)SKN_FPU_ERROR << 32);


    TargetCoordinates detectTarget_net;

    //Работа СПО БВП
    retCode = mmSPOBVP->Process(fpuTV, fpuIR, &detectTarget_net);
    if(retCode != spobvp::SPOBVP_SUCCESS)
        return retCode + ((long long)SKN_SPOBVP_ERROR << 32);


    //Работа АС БВП

    retCode = mmASBVP->Process(fpuTV, fpuIR, &targetTV, &targetIR, detectTarget_net, target);
    if(retCode != asbvp::ASBVP_SUCCESS)
        return retCode + ((long long)SKN_SPOBVP_ERROR << 32);
    if(targetTV.found && targetIR.found){
        rz = true;
        badFrames = 0;
    }
    else{

        rz = true;
        targetTV.x = target.x;
        targetTV.y = target.y;
        targetIR.x = target.x;
        targetIR.y = target.y;
        badFrames++;
    }

    //если на 10 кадрах подряд нет распознавания, снятие захвата
    if(badFrames > 12)
        rz = false;

    //Проверка наличия сигнала о сбросе захвата от AC БВП
    if(mmASBVP->flag_ZG)
    {
        if((detectTarget_net.x <= fco::lastColumn)&&(detectTarget_net.x >= fco::firstColumn)&&(detectTarget_net.y <= fco::lastRow)&&(detectTarget_net.y >= fco::firstRow))
        {
            // qDebug()<<"\nсброс захвата";
            mmAS->Reset();
            mmASBVP->flag_ZG = false;
        }
    }

    //Выполнение сопровождения
    retCode = mmAS->Process(fpuTV, fpuIR, targetTV, targetIR, &target, rz, &zg);
    if(retCode != as::AS_SUCCESS)
        return retCode + ((long long)SKN_AS_ERROR << 32);

	lastTarget = target;

	//Передача результатов
	*tvOut = fpuTV;
	*irOut = fpuIR;
	posTarget->x = target.x;
	posTarget->y = target.y;

    mmGS->Process(posTarget);

    // posTarget->pelengHor = atan( target.x / 320 * tan(CornerHor/2.0*PI/180) )/PI*180 - CornerHor/2.0 ;
    // posTarget->pelengVer = CornerVer/2.0 - atan( target.y / 240 * tan(CornerVer/2.0*PI/180) )/PI*180 ;

	posTarget->found = zg;

	return SKN_SUCCESS;
}

void SKN::ResetZG()
{
	mmAS->Reset();
	zg = false;
	rz = false;
}
