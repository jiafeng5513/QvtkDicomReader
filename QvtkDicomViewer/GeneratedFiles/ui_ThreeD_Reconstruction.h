/********************************************************************************
** Form generated from reading UI file 'ThreeD_Reconstruction.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THREED_RECONSTRUCTION_H
#define UI_THREED_RECONSTRUCTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ThreeD_Reconstruction
{
public:
    QGridLayout *gridLayout;
    QVTKWidget *qvtkWidget_3DRec;

    void setupUi(QWidget *ThreeD_Reconstruction)
    {
        if (ThreeD_Reconstruction->objectName().isEmpty())
            ThreeD_Reconstruction->setObjectName(QStringLiteral("ThreeD_Reconstruction"));
        ThreeD_Reconstruction->resize(809, 605);
        gridLayout = new QGridLayout(ThreeD_Reconstruction);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        qvtkWidget_3DRec = new QVTKWidget(ThreeD_Reconstruction);
        qvtkWidget_3DRec->setObjectName(QStringLiteral("qvtkWidget_3DRec"));

        gridLayout->addWidget(qvtkWidget_3DRec, 0, 0, 1, 1);


        retranslateUi(ThreeD_Reconstruction);

        QMetaObject::connectSlotsByName(ThreeD_Reconstruction);
    } // setupUi

    void retranslateUi(QWidget *ThreeD_Reconstruction)
    {
        ThreeD_Reconstruction->setWindowTitle(QApplication::translate("ThreeD_Reconstruction", "\344\270\211\347\273\264\351\207\215\345\273\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ThreeD_Reconstruction: public Ui_ThreeD_Reconstruction {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THREED_RECONSTRUCTION_H
