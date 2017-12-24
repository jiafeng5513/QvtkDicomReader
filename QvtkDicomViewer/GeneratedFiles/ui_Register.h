/********************************************************************************
** Form generated from reading UI file 'Register.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton_SelectImageFix;
    QRadioButton *radioButton_4;
    QPushButton *pushButton_Cancel;
    QPushButton *pushButton_SelectImageMove;
    QVTKWidget *qvtkWidget_Registration;
    QRadioButton *radioButton_3;
    QPushButton *pushButton_Ok;
    QSpacerItem *horizontalSpacer;
    QRadioButton *radioButton_1;
    QRadioButton *radioButton_2;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit_ImageMove;
    QLineEdit *lineEdit_ImageFix;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName(QStringLiteral("Register"));
        Register->resize(698, 616);
        gridLayout = new QGridLayout(Register);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_SelectImageFix = new QPushButton(Register);
        pushButton_SelectImageFix->setObjectName(QStringLiteral("pushButton_SelectImageFix"));
        pushButton_SelectImageFix->setMinimumSize(QSize(100, 25));
        pushButton_SelectImageFix->setMaximumSize(QSize(100, 25));

        gridLayout->addWidget(pushButton_SelectImageFix, 0, 0, 1, 1);

        radioButton_4 = new QRadioButton(Register);
        radioButton_4->setObjectName(QStringLiteral("radioButton_4"));

        gridLayout->addWidget(radioButton_4, 4, 5, 1, 1);

        pushButton_Cancel = new QPushButton(Register);
        pushButton_Cancel->setObjectName(QStringLiteral("pushButton_Cancel"));
        pushButton_Cancel->setMinimumSize(QSize(100, 25));
        pushButton_Cancel->setMaximumSize(QSize(100, 25));

        gridLayout->addWidget(pushButton_Cancel, 3, 7, 1, 1);

        pushButton_SelectImageMove = new QPushButton(Register);
        pushButton_SelectImageMove->setObjectName(QStringLiteral("pushButton_SelectImageMove"));
        pushButton_SelectImageMove->setMinimumSize(QSize(100, 25));
        pushButton_SelectImageMove->setMaximumSize(QSize(100, 25));

        gridLayout->addWidget(pushButton_SelectImageMove, 3, 0, 1, 1);

        qvtkWidget_Registration = new QVTKWidget(Register);
        qvtkWidget_Registration->setObjectName(QStringLiteral("qvtkWidget_Registration"));
        qvtkWidget_Registration->setMinimumSize(QSize(680, 510));

        gridLayout->addWidget(qvtkWidget_Registration, 7, 0, 1, 8);

        radioButton_3 = new QRadioButton(Register);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));

        gridLayout->addWidget(radioButton_3, 4, 4, 1, 1);

        pushButton_Ok = new QPushButton(Register);
        pushButton_Ok->setObjectName(QStringLiteral("pushButton_Ok"));
        pushButton_Ok->setMinimumSize(QSize(100, 25));
        pushButton_Ok->setMaximumSize(QSize(100, 25));

        gridLayout->addWidget(pushButton_Ok, 0, 7, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 4, 1, 1, 1);

        radioButton_1 = new QRadioButton(Register);
        radioButton_1->setObjectName(QStringLiteral("radioButton_1"));

        gridLayout->addWidget(radioButton_1, 4, 2, 1, 1);

        radioButton_2 = new QRadioButton(Register);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        gridLayout->addWidget(radioButton_2, 4, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 4, 6, 1, 1);

        lineEdit_ImageMove = new QLineEdit(Register);
        lineEdit_ImageMove->setObjectName(QStringLiteral("lineEdit_ImageMove"));
        lineEdit_ImageMove->setMinimumSize(QSize(0, 25));
        lineEdit_ImageMove->setMaximumSize(QSize(16777215, 25));

        gridLayout->addWidget(lineEdit_ImageMove, 3, 1, 1, 6);

        lineEdit_ImageFix = new QLineEdit(Register);
        lineEdit_ImageFix->setObjectName(QStringLiteral("lineEdit_ImageFix"));
        lineEdit_ImageFix->setMinimumSize(QSize(0, 25));
        lineEdit_ImageFix->setMaximumSize(QSize(16777215, 25));

        gridLayout->addWidget(lineEdit_ImageFix, 0, 1, 1, 6);


        retranslateUi(Register);
        QObject::connect(pushButton_SelectImageFix, SIGNAL(clicked()), Register, SLOT(OnSelectImageFix()));
        QObject::connect(pushButton_SelectImageMove, SIGNAL(clicked()), Register, SLOT(OnSelectImageMove()));
        QObject::connect(pushButton_Ok, SIGNAL(clicked()), Register, SLOT(OnButtonOk()));
        QObject::connect(pushButton_Cancel, SIGNAL(clicked()), Register, SLOT(OnButtonCancel()));
        QObject::connect(radioButton_4, SIGNAL(clicked()), Register, SLOT(OnSelectImageFix()));

        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QWidget *Register)
    {
        Register->setWindowTitle(QApplication::translate("Register", "Register", Q_NULLPTR));
        pushButton_SelectImageFix->setText(QApplication::translate("Register", "\345\237\272\345\207\206\345\233\276\345\203\217", Q_NULLPTR));
        radioButton_4->setText(QApplication::translate("Register", "\351\255\224\346\263\225\345\217\230\346\215\242", Q_NULLPTR));
        pushButton_Cancel->setText(QApplication::translate("Register", "\351\200\200\345\207\272", Q_NULLPTR));
        pushButton_SelectImageMove->setText(QApplication::translate("Register", "\345\276\205\351\205\215\345\207\206\345\233\276\345\203\217", Q_NULLPTR));
        radioButton_3->setText(QApplication::translate("Register", "\344\273\277\345\260\204\345\217\230\346\215\242", Q_NULLPTR));
        pushButton_Ok->setText(QApplication::translate("Register", "\345\274\200\345\247\213\350\256\241\347\256\227", Q_NULLPTR));
        radioButton_1->setText(QApplication::translate("Register", "\345\271\263\347\247\273\345\217\230\346\215\242", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("Register", "\344\270\255\345\277\203\347\233\270\344\274\274\344\272\214\347\273\264\345\217\230\346\215\242", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
