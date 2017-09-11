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
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVTKWidget *qvtkWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
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
        QvtkDicomViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QvtkDicomViewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QvtkDicomViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QvtkDicomViewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QvtkDicomViewerClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(action_Open);
        menu_2->addAction(action_RenderText);
        menu_2->addAction(action_back);
        menu_2->addAction(action_next);
        mainToolBar->addAction(action_Open);
        mainToolBar->addAction(action_RenderText);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_back);
        mainToolBar->addAction(action_next);

        retranslateUi(QvtkDicomViewerClass);
        QObject::connect(action_Open, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnOpenFile()));
        QObject::connect(action_RenderText, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnRenderText()));
        QObject::connect(action_back, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnBackward()));
        QObject::connect(action_next, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnForward()));

        QMetaObject::connectSlotsByName(QvtkDicomViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *QvtkDicomViewerClass)
    {
        QvtkDicomViewerClass->setWindowTitle(QApplication::translate("QvtkDicomViewerClass", "QvtkDicomViewer", Q_NULLPTR));
        action_Open->setText(QApplication::translate("QvtkDicomViewerClass", "\346\211\223\345\274\200", Q_NULLPTR));
        action_RenderText->setText(QApplication::translate("QvtkDicomViewerClass", "\350\276\223\345\207\272\344\277\241\346\201\257", Q_NULLPTR));
        action_back->setText(QApplication::translate("QvtkDicomViewerClass", "\344\270\212\344\270\200\345\274\240", Q_NULLPTR));
        action_next->setText(QApplication::translate("QvtkDicomViewerClass", "\344\270\213\344\270\200\345\274\240", Q_NULLPTR));
        menu->setTitle(QApplication::translate("QvtkDicomViewerClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("QvtkDicomViewerClass", "\344\277\241\346\201\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QvtkDicomViewerClass: public Ui_QvtkDicomViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QVTKDICOMVIEWER_H
