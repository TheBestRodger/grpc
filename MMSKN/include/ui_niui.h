/********************************************************************************
** Form generated from reading UI file 'niuiykYhgM.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef NIUIYKYHGM_H
#define NIUIYKYHGM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NIUI
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayoutDisplay;
    QGraphicsView *graphicsViewTV;
    QGraphicsView *graphicsViewIR;
    QHBoxLayout *horizontalLayoutTV;
    QLineEdit *lineEditTV;
    QPushButton *pushButtonTV;
    QHBoxLayout *horizontalLayoutIR;
    QLineEdit *lineEditIR;
    QPushButton *pushButtonIR;
    QHBoxLayout *horizontalLayout_Model;
    QLineEdit *lineEditModel;
    QPushButton *pushButtonModel;
    QHBoxLayout *horizontalLayoutControls;
    QPushButton *pushButtonRun;
    QPushButton *pushButtonSingleFrame;
    QPushButton *pushButtonRestart;
    QDoubleSpinBox *doubleSpinBoxCornerHor;
    QDoubleSpinBox *doubleSpinBoxCprnerVer;
    QSpacerItem *horizontalSpacerControls;
    QHBoxLayout *horizontalLayoutPelengs;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QLineEdit *lineEditPelengHor;
    QLabel *label_2;
    QLineEdit *lineEditPelengVer;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *NIUI)
    {
        if (NIUI->objectName().isEmpty())
            NIUI->setObjectName(QString::fromUtf8("NIUI"));
        NIUI->resize(1306, 701);
        centralwidget = new QWidget(NIUI);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayoutDisplay = new QHBoxLayout();
        horizontalLayoutDisplay->setObjectName(QString::fromUtf8("horizontalLayoutDisplay"));
        graphicsViewTV = new QGraphicsView(centralwidget);
        graphicsViewTV->setObjectName(QString::fromUtf8("graphicsViewTV"));
        graphicsViewTV->setMinimumSize(QSize(640, 480));
        graphicsViewTV->setFrameShape(QFrame::NoFrame);

        horizontalLayoutDisplay->addWidget(graphicsViewTV);

        graphicsViewIR = new QGraphicsView(centralwidget);
        graphicsViewIR->setObjectName(QString::fromUtf8("graphicsViewIR"));
        graphicsViewIR->setMinimumSize(QSize(640, 480));
        graphicsViewIR->setFrameShape(QFrame::NoFrame);

        horizontalLayoutDisplay->addWidget(graphicsViewIR);


        verticalLayout->addLayout(horizontalLayoutDisplay);

        horizontalLayoutTV = new QHBoxLayout();
        horizontalLayoutTV->setObjectName(QString::fromUtf8("horizontalLayoutTV"));
        lineEditTV = new QLineEdit(centralwidget);
        lineEditTV->setObjectName(QString::fromUtf8("lineEditTV"));

        horizontalLayoutTV->addWidget(lineEditTV);

        pushButtonTV = new QPushButton(centralwidget);
        pushButtonTV->setObjectName(QString::fromUtf8("pushButtonTV"));

        horizontalLayoutTV->addWidget(pushButtonTV);


        verticalLayout->addLayout(horizontalLayoutTV);

        horizontalLayoutIR = new QHBoxLayout();
        horizontalLayoutIR->setObjectName(QString::fromUtf8("horizontalLayoutIR"));
        lineEditIR = new QLineEdit(centralwidget);
        lineEditIR->setObjectName(QString::fromUtf8("lineEditIR"));

        horizontalLayoutIR->addWidget(lineEditIR);

        pushButtonIR = new QPushButton(centralwidget);
        pushButtonIR->setObjectName(QString::fromUtf8("pushButtonIR"));

        horizontalLayoutIR->addWidget(pushButtonIR);


        verticalLayout->addLayout(horizontalLayoutIR);

        horizontalLayout_Model = new QHBoxLayout();
        horizontalLayout_Model->setObjectName(QString::fromUtf8("horizontalLayout_Model"));
        lineEditModel = new QLineEdit(centralwidget);
        lineEditModel->setObjectName(QString::fromUtf8("lineEditModel"));

        horizontalLayout_Model->addWidget(lineEditModel);

        pushButtonModel = new QPushButton(centralwidget);
        pushButtonModel->setObjectName(QString::fromUtf8("pushButtonModel"));

        horizontalLayout_Model->addWidget(pushButtonModel);


        verticalLayout->addLayout(horizontalLayout_Model);

        horizontalLayoutControls = new QHBoxLayout();
        horizontalLayoutControls->setObjectName(QString::fromUtf8("horizontalLayoutControls"));
        pushButtonRun = new QPushButton(centralwidget);
        pushButtonRun->setObjectName(QString::fromUtf8("pushButtonRun"));
        pushButtonRun->setCheckable(true);

        horizontalLayoutControls->addWidget(pushButtonRun);

        pushButtonSingleFrame = new QPushButton(centralwidget);
        pushButtonSingleFrame->setObjectName(QString::fromUtf8("pushButtonSingleFrame"));

        horizontalLayoutControls->addWidget(pushButtonSingleFrame);

        pushButtonRestart = new QPushButton(centralwidget);
        pushButtonRestart->setObjectName(QString::fromUtf8("pushButtonRestart"));

        horizontalLayoutControls->addWidget(pushButtonRestart);

        doubleSpinBoxCornerHor = new QDoubleSpinBox(centralwidget);
        doubleSpinBoxCornerHor->setObjectName(QString::fromUtf8("doubleSpinBoxCornerHor"));
        doubleSpinBoxCornerHor->setSingleStep(0.100000000000000);
        doubleSpinBoxCornerHor->setValue(4.000000000000000);

        horizontalLayoutControls->addWidget(doubleSpinBoxCornerHor);

        doubleSpinBoxCprnerVer = new QDoubleSpinBox(centralwidget);
        doubleSpinBoxCprnerVer->setObjectName(QString::fromUtf8("doubleSpinBoxCprnerVer"));
        doubleSpinBoxCprnerVer->setSingleStep(0.100000000000000);
        doubleSpinBoxCprnerVer->setValue(3.000000000000000);

        horizontalLayoutControls->addWidget(doubleSpinBoxCprnerVer);

        horizontalSpacerControls = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutControls->addItem(horizontalSpacerControls);


        verticalLayout->addLayout(horizontalLayoutControls);

        horizontalLayoutPelengs = new QHBoxLayout();
        horizontalLayoutPelengs->setObjectName(QString::fromUtf8("horizontalLayoutPelengs"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutPelengs->addItem(horizontalSpacer_3);

        horizontalSpacer_2 = new QSpacerItem(80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutPelengs->addItem(horizontalSpacer_2);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayoutPelengs->addWidget(label);

        lineEditPelengHor = new QLineEdit(centralwidget);
        lineEditPelengHor->setObjectName(QString::fromUtf8("lineEditPelengHor"));

        horizontalLayoutPelengs->addWidget(lineEditPelengHor);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayoutPelengs->addWidget(label_2);

        lineEditPelengVer = new QLineEdit(centralwidget);
        lineEditPelengVer->setObjectName(QString::fromUtf8("lineEditPelengVer"));

        horizontalLayoutPelengs->addWidget(lineEditPelengVer);

        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayoutPelengs->addItem(horizontalSpacer);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutPelengs->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayoutPelengs);

        NIUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(NIUI);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1306, 21));
        NIUI->setMenuBar(menubar);
        statusbar = new QStatusBar(NIUI);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        NIUI->setStatusBar(statusbar);

        retranslateUi(NIUI);

        QMetaObject::connectSlotsByName(NIUI);
    } // setupUi

    void retranslateUi(QMainWindow *NIUI)
    {
        NIUI->setWindowTitle(QCoreApplication::translate("NIUI", "NIUI", nullptr));
        pushButtonTV->setText(QCoreApplication::translate("NIUI", "\320\242\320\222", nullptr));
        pushButtonIR->setText(QCoreApplication::translate("NIUI", "\320\242\320\237\320\222", nullptr));
        pushButtonModel->setText(QCoreApplication::translate("NIUI", "\320\234\320\276\320\264\320\265\320\273\321\214", nullptr));
        pushButtonRun->setText(QCoreApplication::translate("NIUI", "\320\240\320\260\320\261\320\276\321\202\320\260", nullptr));
        pushButtonSingleFrame->setText(QCoreApplication::translate("NIUI", "\320\250\320\260\320\263", nullptr));
        pushButtonRestart->setText(QCoreApplication::translate("NIUI", "\320\237\320\265\321\200\320\265\320\267\320\260\320\277\321\203\321\201\320\272 \321\204\320\270\320\273\321\214\320\274\320\260", nullptr));
        doubleSpinBoxCornerHor->setPrefix(QCoreApplication::translate("NIUI", "\321\203\320\263\320\276\320\273 \320\277\320\276 \320\263\320\276\321\200\320\270\320\267\320\276\320\275\321\202\320\260\320\273\320\270 ", nullptr));
        doubleSpinBoxCprnerVer->setPrefix(QCoreApplication::translate("NIUI", "\321\203\320\263\320\276\320\273 \320\277\320\276 \320\262\320\265\321\200\321\202\320\270\320\272\320\260\320\273\320\270 ", nullptr));
        label->setText(QCoreApplication::translate("NIUI", "\320\277\320\265\320\273\320\265\320\275\320\263 \320\277\320\276 \320\263\320\276\321\200\320\270\320\267\320\276\320\275\321\202\320\260\320\273\320\270", nullptr));
        label_2->setText(QCoreApplication::translate("NIUI", "\320\277\320\265\320\273\320\265\320\275\320\263 \320\277\320\276 \320\262\320\265\321\200\321\202\320\270\320\272\320\260\320\273\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NIUI: public Ui_NIUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // NIUIYKYHGM_H
