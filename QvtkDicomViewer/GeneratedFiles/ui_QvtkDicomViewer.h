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
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_QvtkDicomViewerClass
{
public:
    QAction *action_OpenSeriesFolder;
    QAction *action_back;
    QAction *action_next;
    QAction *action_Pointer;
    QAction *action_Protractor;
    QAction *action_Ruler;
    QAction *action_Contour;
    QAction *action_BiDimensional;
    QAction *action_Negative;
    QAction *action_Reset;
    QAction *action_Zoom;
    QAction *action_GrayLevel;
    QAction *action_Move;
    QAction *action_SwitchOfProperty;
    QAction *action_Play;
    QAction *action_Stop;
    QAction *action_DCMTK_x64;
    QAction *action_DCMTK_64_DiconDir;
    QAction *action_OpenDicomDirFile;
    QAction *action_OpenDicomFile;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVTKWidget *qvtkWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menu_5;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_Dir;
    QWidget *dockWidgetContents;
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;

    void setupUi(QMainWindow *QvtkDicomViewerClass)
    {
        if (QvtkDicomViewerClass->objectName().isEmpty())
            QvtkDicomViewerClass->setObjectName(QStringLiteral("QvtkDicomViewerClass"));
        QvtkDicomViewerClass->resize(1179, 738);
        action_OpenSeriesFolder = new QAction(QvtkDicomViewerClass);
        action_OpenSeriesFolder->setObjectName(QStringLiteral("action_OpenSeriesFolder"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/folder_128px_1207828_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_OpenSeriesFolder->setIcon(icon);
        action_back = new QAction(QvtkDicomViewerClass);
        action_back->setObjectName(QStringLiteral("action_back"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/front_128px_1143868_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_back->setIcon(icon1);
        action_next = new QAction(QvtkDicomViewerClass);
        action_next->setObjectName(QStringLiteral("action_next"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/next_128px_1143867_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_next->setIcon(icon2);
        action_Pointer = new QAction(QvtkDicomViewerClass);
        action_Pointer->setObjectName(QStringLiteral("action_Pointer"));
        action_Pointer->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/mouse_128px_1153948_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Pointer->setIcon(icon3);
        action_Protractor = new QAction(QvtkDicomViewerClass);
        action_Protractor->setObjectName(QStringLiteral("action_Protractor"));
        action_Protractor->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/Angle_Thingy_128px_559411_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Protractor->setIcon(icon4);
        action_Ruler = new QAction(QvtkDicomViewerClass);
        action_Ruler->setObjectName(QStringLiteral("action_Ruler"));
        action_Ruler->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/ruler_128px_1209146_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Ruler->setIcon(icon5);
        action_Contour = new QAction(QvtkDicomViewerClass);
        action_Contour->setObjectName(QStringLiteral("action_Contour"));
        action_Contour->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/Bezier_Curve_128px_559452_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Contour->setIcon(icon6);
        action_BiDimensional = new QAction(QvtkDicomViewerClass);
        action_BiDimensional->setObjectName(QStringLiteral("action_BiDimensional"));
        action_BiDimensional->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/Arrow_Cross_128px_1177075_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_BiDimensional->setIcon(icon7);
        action_Negative = new QAction(QvtkDicomViewerClass);
        action_Negative->setObjectName(QStringLiteral("action_Negative"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/video_negative_128px_1138773_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Negative->setIcon(icon8);
        action_Reset = new QAction(QvtkDicomViewerClass);
        action_Reset->setObjectName(QStringLiteral("action_Reset"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/cancel_128px_1209038_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Reset->setIcon(icon9);
        action_Zoom = new QAction(QvtkDicomViewerClass);
        action_Zoom->setObjectName(QStringLiteral("action_Zoom"));
        action_Zoom->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/zoom_in_128px_1200197_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Zoom->setIcon(icon10);
        action_GrayLevel = new QAction(QvtkDicomViewerClass);
        action_GrayLevel->setObjectName(QStringLiteral("action_GrayLevel"));
        action_GrayLevel->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/brightness_128px_1193620_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_GrayLevel->setIcon(icon11);
        action_Move = new QAction(QvtkDicomViewerClass);
        action_Move->setObjectName(QStringLiteral("action_Move"));
        action_Move->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/move_Hand_128px_1154031_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Move->setIcon(icon12);
        action_SwitchOfProperty = new QAction(QvtkDicomViewerClass);
        action_SwitchOfProperty->setObjectName(QStringLiteral("action_SwitchOfProperty"));
        action_SwitchOfProperty->setCheckable(true);
        action_Play = new QAction(QvtkDicomViewerClass);
        action_Play->setObjectName(QStringLiteral("action_Play"));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/play_128px_1197036_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Play->setIcon(icon13);
        action_Stop = new QAction(QvtkDicomViewerClass);
        action_Stop->setObjectName(QStringLiteral("action_Stop"));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/stop_128px_1197040_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Stop->setIcon(icon14);
        action_DCMTK_x64 = new QAction(QvtkDicomViewerClass);
        action_DCMTK_x64->setObjectName(QStringLiteral("action_DCMTK_x64"));
        action_DCMTK_64_DiconDir = new QAction(QvtkDicomViewerClass);
        action_DCMTK_64_DiconDir->setObjectName(QStringLiteral("action_DCMTK_64_DiconDir"));
        action_OpenDicomDirFile = new QAction(QvtkDicomViewerClass);
        action_OpenDicomDirFile->setObjectName(QStringLiteral("action_OpenDicomDirFile"));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/file_128px_1197084_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_OpenDicomDirFile->setIcon(icon15);
        action_OpenDicomFile = new QAction(QvtkDicomViewerClass);
        action_OpenDicomFile->setObjectName(QStringLiteral("action_OpenDicomFile"));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/X_Ray_Hand_128px_1124752_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_OpenDicomFile->setIcon(icon16);
        centralWidget = new QWidget(QvtkDicomViewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        qvtkWidget = new QVTKWidget(centralWidget);
        qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(qvtkWidget->sizePolicy().hasHeightForWidth());
        qvtkWidget->setSizePolicy(sizePolicy);
        qvtkWidget->setSizeIncrement(QSize(0, 0));
        qvtkWidget->setBaseSize(QSize(0, 0));

        gridLayout->addWidget(qvtkWidget, 0, 0, 1, 1);

        QvtkDicomViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QvtkDicomViewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1179, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        menu_5 = new QMenu(menuBar);
        menu_5->setObjectName(QStringLiteral("menu_5"));
        QvtkDicomViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QvtkDicomViewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QvtkDicomViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QvtkDicomViewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QvtkDicomViewerClass->setStatusBar(statusBar);
        dockWidget_Dir = new QDockWidget(QvtkDicomViewerClass);
        dockWidget_Dir->setObjectName(QStringLiteral("dockWidget_Dir"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        horizontalLayout = new QHBoxLayout(dockWidgetContents);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        treeView = new QTreeView(dockWidgetContents);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setContextMenuPolicy(Qt::CustomContextMenu);

        horizontalLayout->addWidget(treeView);

        dockWidget_Dir->setWidget(dockWidgetContents);
        QvtkDicomViewerClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_Dir);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menu->addAction(action_OpenSeriesFolder);
        menu->addAction(action_OpenDicomDirFile);
        menu->addAction(action_OpenDicomFile);
        menu_2->addAction(action_back);
        menu_2->addAction(action_next);
        menu_3->addAction(action_Pointer);
        menu_3->addAction(action_Protractor);
        menu_3->addAction(action_Ruler);
        menu_3->addAction(action_Contour);
        menu_3->addAction(action_BiDimensional);
        menu_3->addAction(action_Negative);
        menu_3->addAction(action_Reset);
        menu_3->addAction(action_Zoom);
        menu_3->addAction(action_GrayLevel);
        menu_3->addAction(action_Move);
        menu_3->addAction(action_Play);
        menu_3->addAction(action_Stop);
        menu_4->addAction(action_SwitchOfProperty);
        menu_5->addAction(action_DCMTK_x64);
        menu_5->addAction(action_DCMTK_64_DiconDir);
        mainToolBar->addAction(action_OpenDicomDirFile);
        mainToolBar->addAction(action_OpenDicomFile);
        mainToolBar->addAction(action_OpenSeriesFolder);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_back);
        mainToolBar->addAction(action_Play);
        mainToolBar->addAction(action_next);
        mainToolBar->addAction(action_Stop);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Pointer);
        mainToolBar->addAction(action_Zoom);
        mainToolBar->addAction(action_GrayLevel);
        mainToolBar->addAction(action_Move);
        mainToolBar->addAction(action_Protractor);
        mainToolBar->addAction(action_Ruler);
        mainToolBar->addAction(action_Contour);
        mainToolBar->addAction(action_BiDimensional);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Negative);
        mainToolBar->addAction(action_Reset);

        retranslateUi(QvtkDicomViewerClass);
        QObject::connect(action_back, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnBackward()));
        QObject::connect(action_next, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnForward()));
        QObject::connect(action_Pointer, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedPointer()));
        QObject::connect(action_Protractor, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedProtractor()));
        QObject::connect(action_Ruler, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedRuler()));
        QObject::connect(action_Contour, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedContour()));
        QObject::connect(action_BiDimensional, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedBiDimensional()));
        QObject::connect(action_Negative, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnNegative()));
        QObject::connect(action_Reset, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnReset()));
        QObject::connect(action_GrayLevel, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedGrayLevel()));
        QObject::connect(action_Zoom, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedZoom()));
        QObject::connect(action_Move, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSelectedMove()));
        QObject::connect(action_SwitchOfProperty, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnSwitchProperty()));
        QObject::connect(action_Play, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnPlay()));
        QObject::connect(action_Stop, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnStop()));
        QObject::connect(action_DCMTK_x64, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnTestDCMTK_x64()));
        QObject::connect(action_DCMTK_64_DiconDir, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnTestReadDICOMDIR()));
        QObject::connect(action_OpenDicomDirFile, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnOpenDicomDirFile()));
        QObject::connect(action_OpenSeriesFolder, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnOpenSeriesFolder()));
        QObject::connect(action_OpenDicomFile, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnOpenDicomFile()));
        QObject::connect(treeView, SIGNAL(customContextMenuRequested(QPoint)), QvtkDicomViewerClass, SLOT(on_treeView_customContextMenuRequested(QPoint)));

        QMetaObject::connectSlotsByName(QvtkDicomViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *QvtkDicomViewerClass)
    {
        QvtkDicomViewerClass->setWindowTitle(QApplication::translate("QvtkDicomViewerClass", "QvtkDicomViewer", Q_NULLPTR));
        action_OpenSeriesFolder->setText(QApplication::translate("QvtkDicomViewerClass", "\346\211\223\345\274\200Series\347\233\256\345\275\225", Q_NULLPTR));
        action_back->setText(QApplication::translate("QvtkDicomViewerClass", "\344\270\212\344\270\200\345\274\240", Q_NULLPTR));
        action_next->setText(QApplication::translate("QvtkDicomViewerClass", "\344\270\213\344\270\200\345\274\240", Q_NULLPTR));
        action_Pointer->setText(QApplication::translate("QvtkDicomViewerClass", "\346\227\240", Q_NULLPTR));
        action_Protractor->setText(QApplication::translate("QvtkDicomViewerClass", "\351\207\217\350\247\222\345\231\250", Q_NULLPTR));
        action_Ruler->setText(QApplication::translate("QvtkDicomViewerClass", "\346\265\213\350\267\235\345\260\272", Q_NULLPTR));
        action_Contour->setText(QApplication::translate("QvtkDicomViewerClass", "\350\275\256\345\273\223", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_Contour->setToolTip(QApplication::translate("QvtkDicomViewerClass", "\350\275\256\345\273\223", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_BiDimensional->setText(QApplication::translate("QvtkDicomViewerClass", "\344\272\214\347\273\264\346\240\207\345\260\272", Q_NULLPTR));
        action_Negative->setText(QApplication::translate("QvtkDicomViewerClass", "\350\264\237\347\211\207", Q_NULLPTR));
        action_Reset->setText(QApplication::translate("QvtkDicomViewerClass", "\345\244\215\344\275\215", Q_NULLPTR));
        action_Zoom->setText(QApplication::translate("QvtkDicomViewerClass", "\347\274\251\346\224\276", Q_NULLPTR));
        action_GrayLevel->setText(QApplication::translate("QvtkDicomViewerClass", "\347\201\260\351\230\266", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_GrayLevel->setToolTip(QApplication::translate("QvtkDicomViewerClass", "\347\201\260\351\230\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_Move->setText(QApplication::translate("QvtkDicomViewerClass", "\347\247\273\345\212\250", Q_NULLPTR));
        action_SwitchOfProperty->setText(QApplication::translate("QvtkDicomViewerClass", "\345\261\236\346\200\247", Q_NULLPTR));
        action_Play->setText(QApplication::translate("QvtkDicomViewerClass", "\346\222\255\346\224\276", Q_NULLPTR));
        action_Stop->setText(QApplication::translate("QvtkDicomViewerClass", "\345\201\234\346\255\242", Q_NULLPTR));
        action_DCMTK_x64->setText(QApplication::translate("QvtkDicomViewerClass", "\350\257\273\345\217\226\347\274\251\347\225\245\345\233\276", Q_NULLPTR));
        action_DCMTK_64_DiconDir->setText(QApplication::translate("QvtkDicomViewerClass", "\350\260\203\347\224\250DCMTK-64\350\257\273\345\217\226DiconDir", Q_NULLPTR));
        action_OpenDicomDirFile->setText(QApplication::translate("QvtkDicomViewerClass", "\346\211\223\345\274\200DICOMDIR\346\226\207\344\273\266", Q_NULLPTR));
        action_OpenDicomFile->setText(QApplication::translate("QvtkDicomViewerClass", "\346\211\223\345\274\200Dicom\346\226\207\344\273\266", Q_NULLPTR));
        menu->setTitle(QApplication::translate("QvtkDicomViewerClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("QvtkDicomViewerClass", "\344\277\241\346\201\257", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("QvtkDicomViewerClass", "\345\267\245\345\205\267", Q_NULLPTR));
        menu_4->setTitle(QApplication::translate("QvtkDicomViewerClass", "\347\252\227\345\217\243", Q_NULLPTR));
        menu_5->setTitle(QApplication::translate("QvtkDicomViewerClass", "\346\265\213\350\257\225", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        dockWidget_Dir->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        dockWidget_Dir->setWindowTitle(QApplication::translate("QvtkDicomViewerClass", "DICOMDIR", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QvtkDicomViewerClass: public Ui_QvtkDicomViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QVTKDICOMVIEWER_H
