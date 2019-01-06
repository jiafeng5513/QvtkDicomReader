/********************************************************************************
** Form generated from reading UI file 'Segmenter.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGMENTER_H
#define UI_SEGMENTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Segmenter
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox_LevelSet;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_ShapeDetection;
    QPushButton *pushButton_FastMarching;
    QGroupBox *groupBox_RegionGrowing;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_ConnectedThreshold;
    QPushButton *pushButton_OtsuThreshold;
    QPushButton *pushButton_ConfidenceConnected;
    QPushButton *pushButton_NeighborhoodConnected;
    QVTKWidget *qvtkWidget_Segment;

    void setupUi(QWidget *Segmenter)
    {
        if (Segmenter->objectName().isEmpty())
            Segmenter->setObjectName(QStringLiteral("Segmenter"));
        Segmenter->resize(707, 529);
        gridLayout = new QGridLayout(Segmenter);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox_LevelSet = new QGroupBox(Segmenter);
        groupBox_LevelSet->setObjectName(QStringLiteral("groupBox_LevelSet"));
        groupBox_LevelSet->setMaximumSize(QSize(16777215, 55));
        horizontalLayout = new QHBoxLayout(groupBox_LevelSet);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_ShapeDetection = new QPushButton(groupBox_LevelSet);
        pushButton_ShapeDetection->setObjectName(QStringLiteral("pushButton_ShapeDetection"));

        horizontalLayout->addWidget(pushButton_ShapeDetection);

        pushButton_FastMarching = new QPushButton(groupBox_LevelSet);
        pushButton_FastMarching->setObjectName(QStringLiteral("pushButton_FastMarching"));

        horizontalLayout->addWidget(pushButton_FastMarching);


        gridLayout->addWidget(groupBox_LevelSet, 0, 2, 1, 1);

        groupBox_RegionGrowing = new QGroupBox(Segmenter);
        groupBox_RegionGrowing->setObjectName(QStringLiteral("groupBox_RegionGrowing"));
        groupBox_RegionGrowing->setMaximumSize(QSize(16777215, 55));
        horizontalLayout_2 = new QHBoxLayout(groupBox_RegionGrowing);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_ConnectedThreshold = new QPushButton(groupBox_RegionGrowing);
        pushButton_ConnectedThreshold->setObjectName(QStringLiteral("pushButton_ConnectedThreshold"));

        horizontalLayout_2->addWidget(pushButton_ConnectedThreshold);

        pushButton_OtsuThreshold = new QPushButton(groupBox_RegionGrowing);
        pushButton_OtsuThreshold->setObjectName(QStringLiteral("pushButton_OtsuThreshold"));

        horizontalLayout_2->addWidget(pushButton_OtsuThreshold);

        pushButton_ConfidenceConnected = new QPushButton(groupBox_RegionGrowing);
        pushButton_ConfidenceConnected->setObjectName(QStringLiteral("pushButton_ConfidenceConnected"));

        horizontalLayout_2->addWidget(pushButton_ConfidenceConnected);

        pushButton_NeighborhoodConnected = new QPushButton(groupBox_RegionGrowing);
        pushButton_NeighborhoodConnected->setObjectName(QStringLiteral("pushButton_NeighborhoodConnected"));

        horizontalLayout_2->addWidget(pushButton_NeighborhoodConnected);

        pushButton_ConnectedThreshold->raise();
        pushButton_OtsuThreshold->raise();
        pushButton_NeighborhoodConnected->raise();
        pushButton_ConfidenceConnected->raise();

        gridLayout->addWidget(groupBox_RegionGrowing, 0, 0, 1, 2);

        qvtkWidget_Segment = new QVTKWidget(Segmenter);
        qvtkWidget_Segment->setObjectName(QStringLiteral("qvtkWidget_Segment"));
        qvtkWidget_Segment->setMinimumSize(QSize(550, 450));

        gridLayout->addWidget(qvtkWidget_Segment, 1, 0, 1, 3);


        retranslateUi(Segmenter);
        QObject::connect(pushButton_ConnectedThreshold, SIGNAL(clicked()), Segmenter, SLOT(OnConnectedThreshold()));
        QObject::connect(pushButton_OtsuThreshold, SIGNAL(clicked()), Segmenter, SLOT(OnOtsuThreshold()));
        QObject::connect(pushButton_ConfidenceConnected, SIGNAL(clicked()), Segmenter, SLOT(OnConfidenceConnected()));
        QObject::connect(pushButton_NeighborhoodConnected, SIGNAL(clicked()), Segmenter, SLOT(OnNeighborhoodConnected()));
        QObject::connect(pushButton_ShapeDetection, SIGNAL(clicked()), Segmenter, SLOT(OnShapeDetection()));
        QObject::connect(pushButton_FastMarching, SIGNAL(clicked()), Segmenter, SLOT(OnFastMarching()));
        QObject::connect(pushButton_OtsuThreshold, SIGNAL(clicked()), Segmenter, SLOT(OnWatershedThreshold()));

        QMetaObject::connectSlotsByName(Segmenter);
    } // setupUi

    void retranslateUi(QWidget *Segmenter)
    {
        Segmenter->setWindowTitle(QApplication::translate("Segmenter", "\345\210\206\345\211\262", nullptr));
        groupBox_LevelSet->setTitle(QApplication::translate("Segmenter", "\346\260\264\345\271\263\351\233\206\346\263\225", nullptr));
        pushButton_ShapeDetection->setText(QApplication::translate("Segmenter", "\345\275\242\347\212\266\346\243\200\346\265\213\346\263\225", nullptr));
        pushButton_FastMarching->setText(QApplication::translate("Segmenter", "\345\277\253\351\200\237\345\214\271\351\205\215\346\263\225", nullptr));
        groupBox_RegionGrowing->setTitle(QApplication::translate("Segmenter", "\345\214\272\345\237\237\347\224\237\351\225\277\346\263\225", nullptr));
        pushButton_ConnectedThreshold->setText(QApplication::translate("Segmenter", "\350\201\224\351\200\232\351\230\210\345\200\274\346\263\225", nullptr));
        pushButton_OtsuThreshold->setText(QApplication::translate("Segmenter", "\345\210\206\346\260\264\345\262\255\346\263\225", nullptr));
        pushButton_ConfidenceConnected->setText(QApplication::translate("Segmenter", "\347\275\256\344\277\241\350\277\236\346\216\245\346\263\225", nullptr));
        pushButton_NeighborhoodConnected->setText(QApplication::translate("Segmenter", "\351\202\273\345\237\237\350\277\236\346\216\245\346\263\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Segmenter: public Ui_Segmenter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGMENTER_H
