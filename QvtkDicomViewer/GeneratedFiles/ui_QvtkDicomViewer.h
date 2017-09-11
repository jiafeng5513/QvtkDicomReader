/********************************************************************************
** Form generated from reading UI file 'QvtkDicomViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QVTKDICOMVIEWER_H
#define UI_QVTKDICOMVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_QvtkDicomViewerClass
{
public:
    QAction *action_Open;
    QAction *action_RenderText;
    QAction *action_back;
    QAction *action_next;
    QAction *action_Pointer;
    QAction *action_Protractor;
    QAction *action_Ruler;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVTKWidget *qvtkWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QvtkDicomViewerClass)
    {
        if (QvtkDicomViewerClass->objectName().isEmpty())
            QvtkDicomViewerClass->setObjectName(QStringLiteral("QvtkDicomViewerClass"));
        QvtkDicomViewerClass->resize(817, 557);
        action_Open = new QAction(QvtkDicomViewerClass);
        action_Open->setObjectName(QStringLiteral("action_Open"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/open_128px_1187505_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Open->setIcon(icon);
        action_RenderText = new QAction(QvtkDicomViewerClass);
        action_RenderText->setObjectName(QStringLiteral("action_RenderText"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/text_128px_1188178_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_RenderText->setIcon(icon1);
        action_back = new QAction(QvtkDicomViewerClass);
        action_back->setObjectName(QStringLiteral("action_back"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/front_128px_1143868_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_back->setIcon(icon2);
        action_next = new QAction(QvtkDicomViewerClass);
        action_next->setObjectName(QStringLiteral("action_next"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/next_128px_1143867_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_next->setIcon(icon3);
        action_Pointer = new QAction(QvtkDicomViewerClass);
        action_Pointer->setObjectName(QStringLiteral("action_Pointer"));
        action_Pointer->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/mouse_128px_1153948_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Pointer->setIcon(icon4);
        action_Protractor = new QAction(QvtkDicomViewerClass);
        action_Protractor->setObjectName(QStringLiteral("action_Protractor"));
        action_Protractor->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/Angle_Thingy_128px_559411_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Protractor->setIcon(icon5);
        action_Ruler = new QAction(QvtkDicomViewerClass);
        action_Ruler->setObjectName(QStringLiteral("action_Ruler"));
        action_Ruler->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/ruler_128px_1209146_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Ruler->setIcon(icon6);
        centralWidget = new QWidget(QvtkDicomViewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        qvtkWidget = new QVTKWidget(centralWidget);
        qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));

        gridLayout->addWidget(qvtkWidget, 0, 0, 1, 1);

        QvtkDicomViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QvtkDicomViewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 817, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        QvtkDicomViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QvtkDicomViewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QvtkDicomViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QvtkDicomViewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QvtkDicomViewerClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(action_Open);
        menu_2->addAction(action_RenderText);
        menu_2->addAction(action_back);
        menu_2->addAction(action_next);
        menu_3->addAction(action_Pointer);
        menu_3->addAction(action_Protractor);
        menu_3->addAction(action_Ruler);
        mainToolBar->addAction(action_Open);
        mainToolBar->addAction(action_RenderText);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_back);
        mainToolBar->addAction(action_next);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Pointer);
        mainToolBar->addAction(action_Protractor);
        mainToolBar->addAction(action_Ruler);
        mainToolBar->addSeparator();

        retranslateUi(QvtkDicomViewerClass);
        QObject::connect(action_Open, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnOpenFile()));
        QObject::connect(action_RenderText, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnRenderText()));
        QObject::connect(action_back, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnBackward()));
        QObject::connect(action_next, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnForward()));
        QObject::connect(action_Pointer, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedPointer()));
        QObject::connect(action_Protractor, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedProtractor()));
        QObject::connect(action_Ruler, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedRuler()));

        QMetaObject::connectSlotsByName(QvtkDicomViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *QvtkDicomViewerClass)
    {
        QvtkDicomViewerClass->setWindowTitle(QApplication::translate("QvtkDicomViewerClass", "QvtkDicomViewer", Q_NULLPTR));
        action_Open->setText(QApplication::translate("QvtkDicomViewerClass", "\346\211\223\345\274\200", Q_NULLPTR));
        action_RenderText->setText(QApplication::translate("QvtkDicomViewerClass", "\350\276\223\345\207\272\344\277\241\346\201\257", Q_NULLPTR));
        action_back->setText(QApplication::translate("QvtkDicomViewerClass", "\344\270\212\344\270\200\345\274\240", Q_NULLPTR));
        action_next->setText(QApplication::translate("QvtkDicomViewerClass", "\344\270\213\344\270\200\345\274\240", Q_NULLPTR));
        action_Pointer->setText(QApplication::translate("QvtkDicomViewerClass", "\346\227\240", Q_NULLPTR));
        action_Protractor->setText(QApplication::translate("QvtkDicomViewerClass", "\351\207\217\350\247\222\345\231\250", Q_NULLPTR));
        action_Ruler->setText(QApplication::translate("QvtkDicomViewerClass", "\346\265\213\350\267\235\345\260\272", Q_NULLPTR));
        menu->setTitle(QApplication::translate("QvtkDicomViewerClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("QvtkDicomViewerClass", "\344\277\241\346\201\257", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("QvtkDicomViewerClass", "\345\267\245\345\205\267", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QvtkDicomViewerClass: public Ui_QvtkDicomViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QVTKDICOMVIEWER_H
