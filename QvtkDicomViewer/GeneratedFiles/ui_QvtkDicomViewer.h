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
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
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
    QDockWidget *dockWidget_1;
    QWidget *dockWidgetContents_3;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit_Modality;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lineEdit_Hospital;
    QLabel *label_6;
    QLineEdit *lineEdit_Dimensions;
    QLabel *label_7;
    QLabel *label_11;
    QLineEdit *lineEdit_Origins;
    QLineEdit *lineEdit_Spacing;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_10;
    QLineEdit *lineEdit_Name;
    QLineEdit *lineEdit_Age;
    QLineEdit *lineEdit_ID;
    QLabel *label_9;
    QLineEdit *lineEdit_Sex;
    QLabel *label_8;
    QFrame *line;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_Time;
    QLineEdit *lineEdit_Manufacturer;

    void setupUi(QMainWindow *QvtkDicomViewerClass)
    {
        if (QvtkDicomViewerClass->objectName().isEmpty())
            QvtkDicomViewerClass->setObjectName(QStringLiteral("QvtkDicomViewerClass"));
        QvtkDicomViewerClass->resize(1179, 738);
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
        action_Contour = new QAction(QvtkDicomViewerClass);
        action_Contour->setObjectName(QStringLiteral("action_Contour"));
        action_Contour->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/Bezier_Curve_128px_559452_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Contour->setIcon(icon7);
        action_BiDimensional = new QAction(QvtkDicomViewerClass);
        action_BiDimensional->setObjectName(QStringLiteral("action_BiDimensional"));
        action_BiDimensional->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/Arrow_Cross_128px_1177075_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_BiDimensional->setIcon(icon8);
        action_Negative = new QAction(QvtkDicomViewerClass);
        action_Negative->setObjectName(QStringLiteral("action_Negative"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/video_negative_128px_1138773_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Negative->setIcon(icon9);
        action_Reset = new QAction(QvtkDicomViewerClass);
        action_Reset->setObjectName(QStringLiteral("action_Reset"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/cancel_128px_1209038_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Reset->setIcon(icon10);
        action_Zoom = new QAction(QvtkDicomViewerClass);
        action_Zoom->setObjectName(QStringLiteral("action_Zoom"));
        action_Zoom->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/zoom_in_128px_1200197_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Zoom->setIcon(icon11);
        action_GrayLevel = new QAction(QvtkDicomViewerClass);
        action_GrayLevel->setObjectName(QStringLiteral("action_GrayLevel"));
        action_GrayLevel->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/brightness_128px_1193620_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_GrayLevel->setIcon(icon12);
        action_Move = new QAction(QvtkDicomViewerClass);
        action_Move->setObjectName(QStringLiteral("action_Move"));
        action_Move->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/move_Hand_128px_1154031_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Move->setIcon(icon13);
        action_SwitchOfProperty = new QAction(QvtkDicomViewerClass);
        action_SwitchOfProperty->setObjectName(QStringLiteral("action_SwitchOfProperty"));
        action_SwitchOfProperty->setCheckable(true);
        action_Play = new QAction(QvtkDicomViewerClass);
        action_Play->setObjectName(QStringLiteral("action_Play"));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/play_128px_1197036_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Play->setIcon(icon14);
        action_Stop = new QAction(QvtkDicomViewerClass);
        action_Stop->setObjectName(QStringLiteral("action_Stop"));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/stop_128px_1197040_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_Stop->setIcon(icon15);
        action_DCMTK_x64 = new QAction(QvtkDicomViewerClass);
        action_DCMTK_x64->setObjectName(QStringLiteral("action_DCMTK_x64"));
        action_DCMTK_64_DiconDir = new QAction(QvtkDicomViewerClass);
        action_DCMTK_64_DiconDir->setObjectName(QStringLiteral("action_DCMTK_64_DiconDir"));
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
        dockWidget_1 = new QDockWidget(QvtkDicomViewerClass);
        dockWidget_1->setObjectName(QStringLiteral("dockWidget_1"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        gridLayout_2 = new QGridLayout(dockWidgetContents_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        lineEdit_Modality = new QLineEdit(dockWidgetContents_3);
        lineEdit_Modality->setObjectName(QStringLiteral("lineEdit_Modality"));

        gridLayout_2->addWidget(lineEdit_Modality, 8, 1, 2, 1);

        label_3 = new QLabel(dockWidgetContents_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 6, 0, 3, 1);

        label_4 = new QLabel(dockWidgetContents_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 9, 0, 1, 1);

        label_5 = new QLabel(dockWidgetContents_3);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 10, 0, 1, 1);

        lineEdit_Hospital = new QLineEdit(dockWidgetContents_3);
        lineEdit_Hospital->setObjectName(QStringLiteral("lineEdit_Hospital"));

        gridLayout_2->addWidget(lineEdit_Hospital, 10, 1, 1, 1);

        label_6 = new QLabel(dockWidgetContents_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 11, 0, 1, 1);

        lineEdit_Dimensions = new QLineEdit(dockWidgetContents_3);
        lineEdit_Dimensions->setObjectName(QStringLiteral("lineEdit_Dimensions"));

        gridLayout_2->addWidget(lineEdit_Dimensions, 11, 1, 2, 1);

        label_7 = new QLabel(dockWidgetContents_3);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_2->addWidget(label_7, 12, 0, 2, 1);

        label_11 = new QLabel(dockWidgetContents_3);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 14, 0, 1, 1);

        lineEdit_Origins = new QLineEdit(dockWidgetContents_3);
        lineEdit_Origins->setObjectName(QStringLiteral("lineEdit_Origins"));

        gridLayout_2->addWidget(lineEdit_Origins, 13, 1, 1, 1);

        lineEdit_Spacing = new QLineEdit(dockWidgetContents_3);
        lineEdit_Spacing->setObjectName(QStringLiteral("lineEdit_Spacing"));

        gridLayout_2->addWidget(lineEdit_Spacing, 14, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 15, 1, 1, 1);

        label_10 = new QLabel(dockWidgetContents_3);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_2->addWidget(label_10, 2, 0, 1, 1);

        lineEdit_Name = new QLineEdit(dockWidgetContents_3);
        lineEdit_Name->setObjectName(QStringLiteral("lineEdit_Name"));

        gridLayout_2->addWidget(lineEdit_Name, 0, 1, 1, 1);

        lineEdit_Age = new QLineEdit(dockWidgetContents_3);
        lineEdit_Age->setObjectName(QStringLiteral("lineEdit_Age"));

        gridLayout_2->addWidget(lineEdit_Age, 2, 1, 1, 1);

        lineEdit_ID = new QLineEdit(dockWidgetContents_3);
        lineEdit_ID->setObjectName(QStringLiteral("lineEdit_ID"));

        gridLayout_2->addWidget(lineEdit_ID, 3, 1, 1, 1);

        label_9 = new QLabel(dockWidgetContents_3);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_2->addWidget(label_9, 1, 0, 1, 1);

        lineEdit_Sex = new QLineEdit(dockWidgetContents_3);
        lineEdit_Sex->setObjectName(QStringLiteral("lineEdit_Sex"));

        gridLayout_2->addWidget(lineEdit_Sex, 1, 1, 1, 1);

        label_8 = new QLabel(dockWidgetContents_3);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_2->addWidget(label_8, 0, 0, 1, 1);

        line = new QFrame(dockWidgetContents_3);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line, 4, 0, 1, 2);

        label = new QLabel(dockWidgetContents_3);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 3, 0, 1, 1);

        label_2 = new QLabel(dockWidgetContents_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 5, 0, 1, 1);

        lineEdit_Time = new QLineEdit(dockWidgetContents_3);
        lineEdit_Time->setObjectName(QStringLiteral("lineEdit_Time"));

        gridLayout_2->addWidget(lineEdit_Time, 5, 1, 2, 1);

        lineEdit_Manufacturer = new QLineEdit(dockWidgetContents_3);
        lineEdit_Manufacturer->setObjectName(QStringLiteral("lineEdit_Manufacturer"));

        gridLayout_2->addWidget(lineEdit_Manufacturer, 7, 1, 1, 1);

        dockWidget_1->setWidget(dockWidgetContents_3);
        QvtkDicomViewerClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_1);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menu->addAction(action_Open);
        menu_2->addAction(action_RenderText);
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
        mainToolBar->addAction(action_Open);
        mainToolBar->addAction(action_RenderText);
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
        QObject::connect(action_Open, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnOpenFile()));
        QObject::connect(action_RenderText, SIGNAL(triggered()), QvtkDicomViewerClass, SLOT(OnRenderText()));
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
        action_DCMTK_x64->setText(QApplication::translate("QvtkDicomViewerClass", "\350\260\203\347\224\250DCMTK-x64", Q_NULLPTR));
        action_DCMTK_64_DiconDir->setText(QApplication::translate("QvtkDicomViewerClass", "\350\260\203\347\224\250DCMTK-64\350\257\273\345\217\226DiconDir", Q_NULLPTR));
        menu->setTitle(QApplication::translate("QvtkDicomViewerClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("QvtkDicomViewerClass", "\344\277\241\346\201\257", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("QvtkDicomViewerClass", "\345\267\245\345\205\267", Q_NULLPTR));
        menu_4->setTitle(QApplication::translate("QvtkDicomViewerClass", "\347\252\227\345\217\243", Q_NULLPTR));
        menu_5->setTitle(QApplication::translate("QvtkDicomViewerClass", "\346\265\213\350\257\225", Q_NULLPTR));
        dockWidget_1->setWindowTitle(QApplication::translate("QvtkDicomViewerClass", "\345\261\236\346\200\247", Q_NULLPTR));
        label_3->setText(QApplication::translate("QvtkDicomViewerClass", "\345\210\266\351\200\240\345\225\206:", Q_NULLPTR));
        label_4->setText(QApplication::translate("QvtkDicomViewerClass", "\346\243\200\346\265\213\346\211\213\346\256\265:", Q_NULLPTR));
        label_5->setText(QApplication::translate("QvtkDicomViewerClass", "\345\214\273\351\231\242:", Q_NULLPTR));
        label_6->setText(QApplication::translate("QvtkDicomViewerClass", "\345\260\272\345\257\270:", Q_NULLPTR));
        label_7->setText(QApplication::translate("QvtkDicomViewerClass", "\345\216\237\347\202\271:", Q_NULLPTR));
        label_11->setText(QApplication::translate("QvtkDicomViewerClass", "\345\203\217\347\264\240\351\227\264\351\232\224:", Q_NULLPTR));
        label_10->setText(QApplication::translate("QvtkDicomViewerClass", "\345\271\264\351\276\204:", Q_NULLPTR));
        label_9->setText(QApplication::translate("QvtkDicomViewerClass", "\346\200\247\345\210\253:", Q_NULLPTR));
        label_8->setText(QApplication::translate("QvtkDicomViewerClass", "\345\247\223\345\220\215;", Q_NULLPTR));
        label->setText(QApplication::translate("QvtkDicomViewerClass", "ID:", Q_NULLPTR));
        label_2->setText(QApplication::translate("QvtkDicomViewerClass", "\346\213\215\346\221\204\346\227\266\351\227\264:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QvtkDicomViewerClass: public Ui_QvtkDicomViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QVTKDICOMVIEWER_H
