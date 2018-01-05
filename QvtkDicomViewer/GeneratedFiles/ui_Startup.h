/********************************************************************************
** Form generated from reading UI file 'Startup.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTUP_H
#define UI_STARTUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Startup
{
public:

    void setupUi(QWidget *Startup)
    {
        if (Startup->objectName().isEmpty())
            Startup->setObjectName(QStringLiteral("Startup"));
        Startup->setEnabled(true);
        Startup->resize(660, 300);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Startup->sizePolicy().hasHeightForWidth());
        Startup->setSizePolicy(sizePolicy);
        Startup->setMinimumSize(QSize(660, 300));
        Startup->setMaximumSize(QSize(660, 300));
        Startup->setStyleSheet(QStringLiteral("image: url(:/QvtkDicomViewer/Resources/Avalon_start.png);"));

        retranslateUi(Startup);

        QMetaObject::connectSlotsByName(Startup);
    } // setupUi

    void retranslateUi(QWidget *Startup)
    {
        Startup->setWindowTitle(QApplication::translate("Startup", "Startup", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Startup: public Ui_Startup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUP_H
