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
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_Ok;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_2;
    QPushButton *pushButton_SelectImageFix;
    QLineEdit *lineEdit_ImageFix;
    QRadioButton *radioButton_1;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_SelectImageMove;
    QRadioButton *radioButton_3;
    QLineEdit *lineEdit_ImageMove;
    QPushButton *pushButton_Cancel;
    QWidget *widget;
    QGridLayout *gridLayout_5;
    QSplitter *splitter_3;
    QSplitter *splitter;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QVTKWidget *qvtkWidget_Registration_UL;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_3;
    QVTKWidget *qvtkWidget_Registration_DL;
    QLabel *label;
    QSplitter *splitter_2;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QVTKWidget *qvtkWidget_Registration_UR;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_4;
    QVTKWidget *qvtkWidget_Registration_DR;
    QLabel *label_4;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName(QStringLiteral("Register"));
        Register->resize(840, 719);
        gridLayout_2 = new QGridLayout(Register);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pushButton_Ok = new QPushButton(Register);
        pushButton_Ok->setObjectName(QStringLiteral("pushButton_Ok"));
        pushButton_Ok->setMinimumSize(QSize(100, 25));
        pushButton_Ok->setMaximumSize(QSize(100, 25));

        gridLayout_2->addWidget(pushButton_Ok, 0, 7, 1, 1);

        radioButton_4 = new QRadioButton(Register);
        radioButton_4->setObjectName(QStringLiteral("radioButton_4"));

        gridLayout_2->addWidget(radioButton_4, 2, 5, 1, 1);

        radioButton_2 = new QRadioButton(Register);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        gridLayout_2->addWidget(radioButton_2, 2, 3, 1, 1);

        pushButton_SelectImageFix = new QPushButton(Register);
        pushButton_SelectImageFix->setObjectName(QStringLiteral("pushButton_SelectImageFix"));
        pushButton_SelectImageFix->setMinimumSize(QSize(100, 25));
        pushButton_SelectImageFix->setMaximumSize(QSize(100, 25));

        gridLayout_2->addWidget(pushButton_SelectImageFix, 0, 0, 1, 1);

        lineEdit_ImageFix = new QLineEdit(Register);
        lineEdit_ImageFix->setObjectName(QStringLiteral("lineEdit_ImageFix"));
        lineEdit_ImageFix->setMinimumSize(QSize(0, 25));
        lineEdit_ImageFix->setMaximumSize(QSize(16777215, 25));

        gridLayout_2->addWidget(lineEdit_ImageFix, 0, 1, 1, 6);

        radioButton_1 = new QRadioButton(Register);
        radioButton_1->setObjectName(QStringLiteral("radioButton_1"));

        gridLayout_2->addWidget(radioButton_1, 2, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(115, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(115, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 2, 6, 1, 1);

        pushButton_SelectImageMove = new QPushButton(Register);
        pushButton_SelectImageMove->setObjectName(QStringLiteral("pushButton_SelectImageMove"));
        pushButton_SelectImageMove->setMinimumSize(QSize(100, 25));
        pushButton_SelectImageMove->setMaximumSize(QSize(100, 25));

        gridLayout_2->addWidget(pushButton_SelectImageMove, 1, 0, 1, 1);

        radioButton_3 = new QRadioButton(Register);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));

        gridLayout_2->addWidget(radioButton_3, 2, 4, 1, 1);

        lineEdit_ImageMove = new QLineEdit(Register);
        lineEdit_ImageMove->setObjectName(QStringLiteral("lineEdit_ImageMove"));
        lineEdit_ImageMove->setMinimumSize(QSize(0, 25));
        lineEdit_ImageMove->setMaximumSize(QSize(16777215, 25));

        gridLayout_2->addWidget(lineEdit_ImageMove, 1, 1, 1, 6);

        pushButton_Cancel = new QPushButton(Register);
        pushButton_Cancel->setObjectName(QStringLiteral("pushButton_Cancel"));
        pushButton_Cancel->setMinimumSize(QSize(100, 25));
        pushButton_Cancel->setMaximumSize(QSize(100, 25));

        gridLayout_2->addWidget(pushButton_Cancel, 1, 7, 1, 1);

        widget = new QWidget(Register);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_5 = new QGridLayout(widget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        splitter_3 = new QSplitter(widget);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_3);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        widget_2 = new QWidget(splitter);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(60, 16));
        label_2->setMaximumSize(QSize(60, 16));

        verticalLayout_2->addWidget(label_2);

        qvtkWidget_Registration_UL = new QVTKWidget(widget_2);
        qvtkWidget_Registration_UL->setObjectName(QStringLiteral("qvtkWidget_Registration_UL"));
        qvtkWidget_Registration_UL->setMinimumSize(QSize(340, 255));

        verticalLayout_2->addWidget(qvtkWidget_Registration_UL);

        splitter->addWidget(widget_2);
        widget_3 = new QWidget(splitter);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        verticalLayout_3 = new QVBoxLayout(widget_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        qvtkWidget_Registration_DL = new QVTKWidget(widget_3);
        qvtkWidget_Registration_DL->setObjectName(QStringLiteral("qvtkWidget_Registration_DL"));
        qvtkWidget_Registration_DL->setMinimumSize(QSize(340, 255));

        verticalLayout_3->addWidget(qvtkWidget_Registration_DL);

        label = new QLabel(widget_3);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(60, 16));
        label->setMaximumSize(QSize(60, 16));

        verticalLayout_3->addWidget(label);

        splitter->addWidget(widget_3);
        splitter_3->addWidget(splitter);
        splitter_2 = new QSplitter(splitter_3);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        widget_4 = new QWidget(splitter_2);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        verticalLayout = new QVBoxLayout(widget_4);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(60, 16));
        label_3->setMaximumSize(QSize(60, 16));

        verticalLayout->addWidget(label_3);

        qvtkWidget_Registration_UR = new QVTKWidget(widget_4);
        qvtkWidget_Registration_UR->setObjectName(QStringLiteral("qvtkWidget_Registration_UR"));
        qvtkWidget_Registration_UR->setMinimumSize(QSize(340, 255));

        verticalLayout->addWidget(qvtkWidget_Registration_UR);

        splitter_2->addWidget(widget_4);
        widget_5 = new QWidget(splitter_2);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        verticalLayout_4 = new QVBoxLayout(widget_5);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        qvtkWidget_Registration_DR = new QVTKWidget(widget_5);
        qvtkWidget_Registration_DR->setObjectName(QStringLiteral("qvtkWidget_Registration_DR"));
        qvtkWidget_Registration_DR->setMinimumSize(QSize(340, 255));

        verticalLayout_4->addWidget(qvtkWidget_Registration_DR);

        label_4 = new QLabel(widget_5);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(60, 16));
        label_4->setMaximumSize(QSize(60, 16));

        verticalLayout_4->addWidget(label_4);

        splitter_2->addWidget(widget_5);
        splitter_3->addWidget(splitter_2);

        gridLayout_5->addWidget(splitter_3, 0, 0, 1, 1);


        gridLayout_2->addWidget(widget, 3, 0, 1, 8);


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
        pushButton_Ok->setText(QApplication::translate("Register", "\345\274\200\345\247\213\350\256\241\347\256\227", Q_NULLPTR));
        radioButton_4->setText(QApplication::translate("Register", "\351\255\224\346\263\225\345\217\230\346\215\242", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("Register", "\344\270\255\345\277\203\347\233\270\344\274\274\344\272\214\347\273\264\345\217\230\346\215\242", Q_NULLPTR));
        pushButton_SelectImageFix->setText(QApplication::translate("Register", "\345\237\272\345\207\206\345\233\276\345\203\217", Q_NULLPTR));
        radioButton_1->setText(QApplication::translate("Register", "\345\271\263\347\247\273\345\217\230\346\215\242", Q_NULLPTR));
        pushButton_SelectImageMove->setText(QApplication::translate("Register", "\345\276\205\351\205\215\345\207\206\345\233\276\345\203\217", Q_NULLPTR));
        radioButton_3->setText(QApplication::translate("Register", "\344\273\277\345\260\204\345\217\230\346\215\242", Q_NULLPTR));
        pushButton_Cancel->setText(QApplication::translate("Register", "\351\200\200\345\207\272", Q_NULLPTR));
        label_2->setText(QApplication::translate("Register", "\345\237\272\345\207\206\345\233\276\345\203\217", Q_NULLPTR));
        label->setText(QApplication::translate("Register", "\344\270\255\351\227\264\347\273\223\346\236\234", Q_NULLPTR));
        label_3->setText(QApplication::translate("Register", "\345\276\205\351\205\215\345\207\206\345\233\276\345\203\217", Q_NULLPTR));
        label_4->setText(QApplication::translate("Register", "\346\234\200\347\273\210\347\273\223\346\236\234", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
