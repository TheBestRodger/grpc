#ifndef NIUI_H
#define NIUI_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QMap>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QApplication>
#include <QFileDialog>
#include <QtMath>

#include "structs.h"

#ifdef USE_GRPC
#include "grpc/grpc_helper.h"
#endif

#include "skn.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class NIUI;
}
QT_END_NAMESPACE

class NIUI : public QMainWindow
{
	Q_OBJECT

	Ui::NIUI *ui;

	QGraphicsScene *sceneTV, *sceneIR;

	skn::SKN *mmSKN;

    TargetCoordPelengs target;

	QTimer timer;

    QString tvPath, irPath, jsonPath;
    QStringList tvFrames, irFrames, jsonFrames;
	QStringList frameExtensions;
	int currentFrame{};
	int zgCounter{};

	QPen targetPen;

	QMap<int, QString> errors, subErrors;

#ifdef USE_GRPC
	GrpcHelper grpcHelper;
#endif

	void Process();

public:
	NIUI(QWidget *parent = nullptr);
	~NIUI();

private slots:
	void on_pushButtonTV_clicked();
	void on_pushButtonIR_clicked();
	void on_pushButtonRun_clicked();

	void timer_timeout();
	void on_pushButtonSingleFrame_clicked();
	void on_pushButtonRestart_clicked();
    void on_pushButtonModel_clicked();
    void on_doubleSpinBoxCornerHor_valueChanged(double arg1);
};
#endif // NIUI_H
