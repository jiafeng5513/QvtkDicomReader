#pragma once

#include <QWidget>
#include "ui_Startup.h"

class Startup : public QWidget
{
	Q_OBJECT

public:
	Startup(QWidget *parent = Q_NULLPTR);
	~Startup();

private:
	Ui::Startup ui;
};
