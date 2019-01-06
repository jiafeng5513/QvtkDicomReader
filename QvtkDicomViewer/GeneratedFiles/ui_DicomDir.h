/********************************************************************************
** Form generated from reading UI file 'DicomDir.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DICOMDIR_H
#define UI_DICOMDIR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DicomDir
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QPushButton *ButtonOK;
    QPushButton *ButtonCancel;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *DicomDir)
    {
        if (DicomDir->objectName().isEmpty())
            DicomDir->setObjectName(QStringLiteral("DicomDir"));
        DicomDir->resize(630, 429);
        gridLayout = new QGridLayout(DicomDir);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tableWidget = new QTableWidget(DicomDir);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        gridLayout->addWidget(tableWidget, 0, 0, 1, 3);

        ButtonOK = new QPushButton(DicomDir);
        ButtonOK->setObjectName(QStringLiteral("ButtonOK"));

        gridLayout->addWidget(ButtonOK, 1, 0, 1, 1);

        ButtonCancel = new QPushButton(DicomDir);
        ButtonCancel->setObjectName(QStringLiteral("ButtonCancel"));

        gridLayout->addWidget(ButtonCancel, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);


        retranslateUi(DicomDir);
        QObject::connect(ButtonOK, SIGNAL(clicked()), DicomDir, SLOT(OnPushOk()));
        QObject::connect(ButtonCancel, SIGNAL(clicked()), DicomDir, SLOT(OnPushCancel()));

        QMetaObject::connectSlotsByName(DicomDir);
    } // setupUi

    void retranslateUi(QWidget *DicomDir)
    {
        DicomDir->setWindowTitle(QApplication::translate("DicomDir", "\347\227\205\344\272\272\345\210\227\350\241\250", nullptr));
        ButtonOK->setText(QApplication::translate("DicomDir", "\347\241\256\345\256\232", nullptr));
        ButtonCancel->setText(QApplication::translate("DicomDir", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DicomDir: public Ui_DicomDir {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DICOMDIR_H
