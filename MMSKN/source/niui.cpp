#include "niui.h"
#include "ui_niui.h"

#include "skn.h"
#include <iostream>
#include <IOStructs.h>
#include "tcp\tcp.h"
 
 // УКАЗАТЬ СВОЙ ПУТЬ ДО МОДЕЛИ
 const QString default_path_to_onnx = "D:\\Projects\\NEWKMM\\data\\ssd-resnet18.onnx";


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NIUI w;
	w.show();
	return a.exec();
}

void NIUI::Process()
{
	//Размеры изображений - 640x480
	const int srcWidth = 640;
	const int srcHeight = 480;
	
	PictureRecieve* TV_img{};
	PictureRecieve* IR_img{};

	// Получение снимков
#ifdef USE_GRPC
	// при помощи gRPC
	TV_img = grpcHelper.recieve_img_TV();
	IR_img = grpcHelper.recieve_img_IR();
#else
	// при помощи TCP
	io_context io_context_1;
	io_context io_context_2;
	TV_img = recieve_img_TV(io_context_2);
	IR_img = recieve_img_IR(io_context_1);
#endif

	if (!TV_img || !IR_img) {
		delete TV_img;
		delete IR_img;
		return;
	}

	currentFrame++;
	OESKN_out out;
	//Загрузка кадров
	QImage::Format format = QImage::Format_Grayscale8;
	QImage tvImage(TV_img->pix.data(), TV_img->width, TV_img->height, format); //(tvPath + "/" + tvFrames[currentFrame]);
	QImage irImage(IR_img->pix.data(), IR_img->width, IR_img->height, format);

	QImage tvNonProcessImage(TV_img->pix.data(), TV_img->width, TV_img->height, format);
    QImage irNonProcessImage(IR_img->pix.data(), IR_img->width, IR_img->height, format);
	
	std::cout << tvImage.isNull() << irImage.isNull();

	if (tvImage.isNull() || irImage.isNull()) {
		delete TV_img;
		delete IR_img;
		return;
	}

    //Загрузка данных распознавания
    TargetCoordinates detectTarget;


	// //Преобразование в оттенки серого
    if(tvImage.format() != QImage::Format_Grayscale8){
		tvImage = tvImage.convertToFormat(QImage::Format_Grayscale8);
        tvNonProcessImage = tvImage;
    }
    if(irImage.format() != QImage::Format_Grayscale8){

        irImage = irImage.convertToFormat(QImage::Format_Grayscale8);
        irNonProcessImage = irImage;
    }
    qDebug() << tvImage << tvImage.width();
    qDebug() << irImage << irImage.height();
    qDebug() << tvNonProcessImage;
    qDebug() << irNonProcessImage;
	//Преобразование размера
     if(tvImage.width() != srcWidth || tvImage.height() != srcHeight)
         tvImage = tvImage.copy((tvImage.width() - srcWidth) / 2, (tvImage.height() - srcHeight) / 2, srcWidth, srcHeight);
     if(irImage.width() != srcWidth || irImage.height() != srcHeight)
         irImage = irImage.copy((irImage.width() - srcWidth) / 2, (irImage.height() - srcHeight) / 2, srcWidth, srcHeight);

	//Формирование структур
	ImageArray tv, ir, tvRes, irRes;

	tv.width = tvImage.width();
	tv.height = tvImage.height();
	tv.image = tvImage.bits();
	ir.width = irImage.width();
	ir.height = irImage.height();
	ir.image = irImage.bits();
	tvRes.image = nullptr;
	irRes.image = nullptr;
	//Обработка
	on_pushButtonModel_clicked();
    long long retCode = mmSKN->Process(tv, ir, &tvRes, &irRes, &target, ui->doubleSpinBoxCornerHor->value(), ui->doubleSpinBoxCprnerVer->value());

    //Отображение пеленгов
    ui->lineEditPelengHor->setText(QString::number(target.pelengHor));
    ui->lineEditPelengVer->setText(QString::number(target.pelengVer));

	//Преобразование выходного изображения в QImage
	tvImage = QImage(tvRes.width, tvRes.height, QImage::Format_Grayscale8);
	if(tvRes.image)
		for(int y = 0; y < tvImage.height(); y++)
		{
			unsigned char *row = tvImage.scanLine(y);
			for(int x = 0; x < tvImage.width(); x++)
				row[x] = (unsigned char)(tvRes.image[y * tvRes.width + x] << 1);
		}
	irImage = QImage(irRes.width, irRes.height, QImage::Format_Grayscale8);
	if(irRes.image)
		for(int y = 0; y < irImage.height(); y++)
		{
			unsigned char *row = irImage.scanLine(y);
			for(int x = 0; x < irImage.width(); x++)
				row[x] = (unsigned char)(irRes.image[y * irRes.width + x] << 1);
		}

	//Очистка сцен
	sceneTV->clear();
	sceneIR->clear();

	if(retCode != skn::SKN_SUCCESS)
	{
		//Отображение ошибок
		int badModel = retCode >> 32;
		int code = retCode & 0xFFFFFFFF;
		sceneTV->addSimpleText(subErrors[badModel]);
		sceneIR->addSimpleText(errors[code]);
	}
	else
	{
		//Вывод изображения
        // sceneTV->addPixmap(QPixmap::fromImage(tvImage));
        // sceneIR->addPixmap(QPixmap::fromImage(irImage));
        // qDebug() << tvNonProcessImage;
        // qDebug() << irNonProcessImage;
        sceneTV->addPixmap(QPixmap::fromImage(tvNonProcessImage));
        sceneIR->addPixmap(QPixmap::fromImage(irNonProcessImage));

		//Вывод захвата
		const int crossSize = 10;
		const int edgeOffset = 2;
		if(target.found)
		{
			out.fiv = 0.1;
			out.fig = 0.1;

			out.omv = 0.1;
			out.omg = 0.1;

			out.pelengHor = target.pelengHor;
			out.pelengVer = target.pelengVer;
			out.ZG = target.found;
			if(target.x >= crossSize && target.x < tvImage.width() - crossSize && target.y >= crossSize && target.y < tvImage.height() - crossSize)
			{
				int cx = target.x;
				int cy = target.y;
				int left = target.x - crossSize;
				int right = target.x + crossSize;
				int top = target.y - crossSize;
				int bottom = target.y + crossSize;
				sceneTV->addLine(left, cy, right, cy, targetPen);
				sceneTV->addLine(cx, top, cx, bottom, targetPen);
				sceneIR->addLine(left, cy, right, cy, targetPen);
				sceneIR->addLine(cx, top, cx, bottom, targetPen);
			}
			else
			{
				int ex, ey;
				int lx, ly;
				int b1x, b1y, b2x, b2y;
				if(target.x < crossSize && target.y < crossSize)
				{
					ex = edgeOffset;
					ey = edgeOffset;
					lx = ex + crossSize;
					ly = ey + crossSize;
					b1x = ex;
					b1y = ey + crossSize / 2;
					b2x = ex + crossSize / 2;
					b2y = ey;
				}
				else if(target.x >= tvImage.width() - crossSize && target.y < crossSize)
				{
					ex = tvImage.width() - 1 - edgeOffset;
					ey = edgeOffset;
					lx = ex - crossSize;
					ly = ey + crossSize;
					b1x = ex;
					b1y = ey + crossSize / 2;
					b2x = ex - crossSize / 2;
					b2y = ey;
				}
				else if(target.x < crossSize && target.y >= tvImage.height() - crossSize)
				{
					ex = edgeOffset;
					ey = tvImage.height() - 1 - edgeOffset;
					lx = ex + crossSize;
					ly = ey - crossSize;
					b1x = ex;
					b1y = ey - crossSize / 2;
					b2x = ex + crossSize / 2;
					b2y = ey;
				}
				else if(target.x >= tvImage.width() - crossSize && target.y >= tvImage.height() - crossSize)
				{
					ex = tvImage.width() - 1 - edgeOffset;
					ey = tvImage.height() - 1 - edgeOffset;
					lx = ex - crossSize;
					ly = ey - crossSize;
					b1x = ex;
					b1y = ey - crossSize / 2;
					b2x = ex - crossSize / 2;
					b2y = ey;
				}
				else if(target.x < crossSize)
				{
					ex = edgeOffset;
					ey = target.y;
					lx = ex + crossSize;
					ly = ey;
					b1x = ex + crossSize / 2;
					b1y = ey - crossSize / 2;
					b2x = ex + crossSize / 2;
					b2y = ey + crossSize / 2;
				}
				else if(target.x >= tvImage.width() - crossSize)
				{
					ex = tvImage.width() - 1 - edgeOffset;
					ey = target.y;
					lx = ex - crossSize;
					ly = ey;
					b1x = ex - crossSize / 2;
					b1y = ey - crossSize / 2;
					b2x = ex - crossSize / 2;
					b2y = ey + crossSize / 2;
				}
				else if(target.y < crossSize)
				{
					ex = target.x;
					ey = edgeOffset;
					lx = ex;
					ly = ey + crossSize;
					b1x = ex - crossSize / 2;
					b1y = ey + crossSize / 2;
					b2x = ex + crossSize / 2;
					b2y = ey + crossSize / 2;
				}
				else /*if(target.y >= tvImage.height() - crossSize)*/
				{
					ex = target.x;
					ey = tvImage.height() - 1 - edgeOffset;
					lx = ex;
					ly = ey - crossSize;
					b1x = ex - crossSize / 2;
					b1y = ey - crossSize / 2;
					b2x = ex + crossSize / 2;
					b2y = ey - crossSize / 2;
				}
				sceneTV->addLine(ex, ey, lx, ly, targetPen);
				sceneTV->addLine(ex, ey, b1x, b1y, targetPen);
				sceneTV->addLine(ex, ey, b2x, b2y, targetPen);
				sceneIR->addLine(ex, ey, lx, ly, targetPen);
				sceneIR->addLine(ex, ey, b1x, b1y, targetPen);
				sceneIR->addLine(ex, ey, b2x, b2y, targetPen);
			}
		}
	}
	
	io_context io_context_send;
	send_data(io_context_send, out);

	//Перезахват !zgCounter - нигде не задано значение ранее
	if(zgCounter-- == 0)
	{

		mmSKN->ResetZG();

	}

	delete TV_img;
	delete IR_img;
}

NIUI::NIUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::NIUI)
{
	ui->setupUi(this);

	sceneTV = new QGraphicsScene;
	sceneIR = new QGraphicsScene;

	ui->graphicsViewTV->setScene(sceneTV);
	ui->graphicsViewIR->setScene(sceneIR);

	mmSKN = new skn::SKN;

	timer.setParent(this);
	timer.setInterval(19);
	connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));

	frameExtensions << "*.bmp";
	frameExtensions << "*.png";
    frameExtensions << "*.jpg";
    frameExtensions << "*.json";

	targetPen.setColor(Qt::yellow);
	targetPen.setWidth(2);

    mmSKN->InitGS(ui->doubleSpinBoxCornerHor->value(), ui->doubleSpinBoxCprnerVer->value());

	subErrors[skn::SKN_SUB_SUCCESS] = "Ошибка в модели СКН.";
	subErrors[skn::SKN_FCO_ERROR] = "Ошибка в модели ФЦО.";
	subErrors[skn::SKN_FPU_ERROR] = "Ошибка в модели ФПУ.";
	subErrors[skn::SKN_SPOBVP_ERROR] = "Ошибка в модели СПОБПВ.";
	subErrors[skn::SKN_AS_ERROR] = "Ошибка в модели АС.";
	subErrors[skn::SKN_AS_ERROR] = "Ошибка в модели ГК.";
	errors[1] = "В модель передан нулевой указатель.";
	errors[2] = "Неверный размер изображения.";
	errors[3] = "Выход за пределы изображения.";

#ifndef QT_NO_DEBUG
	#ifdef Q_OS_WIN
        ui->lineEditTV->setText("C:/Users/User/Documents/KMM/KMM/MNITI/TV");
        ui->lineEditIR->setText("C:/Users/User/Documents/KMM/KMM/MNITI/IR");
        ui->lineEditJSON->setText("C:/Users/User/Documents/KMM/KMM/MNITI/JSON");
	#elif defined Q_OS_LINUX
        ui->lineEditTV->setText("/home/user/Documents/MNITI2/MNITI/TV");
        ui->lineEditIR->setText("/home/user/Documents/MNITI2/MNITI/IR");
        ui->lineEditJSON->setText("/home/user/Documents/MNITI2/MNITI/JSON");
	#endif
#endif

	ui->lineEditModel->setText(default_path_to_onnx);

	grpcHelper.setHost("localhost");
	grpcHelper.setPort(50051);
}

NIUI::~NIUI()
{
	delete sceneTV;
	delete sceneIR;
	delete mmSKN;
	delete ui;
}




void NIUI::on_pushButtonTV_clicked()
{
	QString path = QFileDialog::getExistingDirectory(this, "Выберите папку с ТВ каналом", ui->lineEditTV->text());
	if(!path.isEmpty())
		ui->lineEditTV->setText(path);
}

void NIUI::on_pushButtonIR_clicked()
{
	QString path = QFileDialog::getExistingDirectory(this, "Выберите папку с ИК каналом", ui->lineEditIR->text());
	if(!path.isEmpty())
		ui->lineEditIR->setText(path);
}


void NIUI::on_pushButtonRun_clicked()
{
	if(ui->pushButtonRun->isChecked())
	{
        //ui->spinBoxClass->setEnabled(false);
		ui->pushButtonSingleFrame->setEnabled(false);
		ui->pushButtonTV->setEnabled(false);
		ui->pushButtonIR->setEnabled(false);
		ui->lineEditTV->setEnabled(false);
		ui->lineEditIR->setEnabled(false);
        ui->pushButtonModel->setEnabled(false);
        ui->lineEditModel->setEnabled(false);

		timer.start();
	}
	else
	{
		timer.stop();
		ui->lineEditIR->setEnabled(true);
		ui->lineEditTV->setEnabled(true);
		ui->pushButtonIR->setEnabled(true);
		ui->pushButtonTV->setEnabled(true);
		ui->pushButtonSingleFrame->setEnabled(true);
        ui->pushButtonModel->setEnabled(true);
        ui->lineEditModel->setEnabled(true);
	}
}

void NIUI::timer_timeout()
{
    //QCoreApplication::processEvents();
	Process();
}

void NIUI::on_pushButtonSingleFrame_clicked()
{
	Process();
}

void NIUI::on_pushButtonRestart_clicked()
{
	tvPath.clear();
	irPath.clear();
	sceneTV->clear();
	sceneIR->clear();
}




void NIUI::on_pushButtonModel_clicked()
{
    QString path_to_onnx = default_path_to_onnx;
	path_to_onnx = QFileDialog::getOpenFileName(this, tr("Выберите папку с ИК каналом"),
		ui->lineEditModel->text(),  tr("Models (*.onnx)"));
    qDebug()<< path_to_onnx;
    if(!path_to_onnx.isEmpty())
    {
        ui->lineEditModel->setText(path_to_onnx);
        mmSKN->InitDetector(path_to_onnx);
    }
}


void NIUI::on_doubleSpinBoxCornerHor_valueChanged(double arg1)
{
    mmSKN->InitGS(ui->doubleSpinBoxCornerHor->value(), ui->doubleSpinBoxCprnerVer->value());
}

