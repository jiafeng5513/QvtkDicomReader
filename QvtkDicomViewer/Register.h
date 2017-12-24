#pragma once

#include <QWidget>
#include "ui_Register.h"
#include <QVTKWidget.h>

#define ITK_IO_FACTORY_REGISTER_MANAGER

//上述宏定义的意义目前还无法否定
//缺乏相应的实验

enum Reg_enum
{
	Reg_Null,
	Reg_test,
	Reg_2Dtransform,
	Reg_AffineTrans,
	Reg_Multi
};

class Register : public QWidget
{
	Q_OBJECT

public:
	Register(QWidget *parent = Q_NULLPTR);
	~Register();
	void SetCount(int count);
	void SetQvtk(QVTKWidget* qvtk);
private:
	Ui::Register ui;
private:
	///配准方法
	void TranslationReg(char *argv[]);
	void CenteredSimilarityTransformReg(char *argv[]);
	void AffineTransformReg(char*argv[]);
	void MultiTransformReg(char*argv[]);
private slots:
	void OnSelectImageFix();		//选择基准图像
	void OnSelectImageMove();		//选择待配准图像
	void OnButtonOk();				//ok-开始计算
	void OnButtonCancel();			//退出
private:
	///即将废弃的变量
	int reg_count;
	QVTKWidget* qvtk;
};
