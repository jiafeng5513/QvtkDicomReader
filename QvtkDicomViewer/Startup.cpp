#include "Startup.h"

Startup::Startup(QWidget *parent)
	: QWidget(parent)
{
	//无边框,保持在顶端
	//setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
	ui.setupUi(this);
}

Startup::~Startup()
{
}
