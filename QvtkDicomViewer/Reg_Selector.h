#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_Reg_Selector.h"
#include <QPushButton>
#include <DicomReg.h>

class Reg_Selector : public QMainWindow
{
	Q_OBJECT

public:
	Reg_Selector(QWidget *parent = Q_NULLPTR);
	void SetCount(int count);
	void SetQvtk(QVTKWidget* qvtk);
private slots:
	void createContentMenu();//创建上下文菜单  
	void createContentMenu2();

	void cancel();
	void submit();
	
private:
	int reg_count;
	QVTKWidget* qvtk;
	Ui::Form ui;
};