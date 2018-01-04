#pragma once
#include <QString>
#include <QFileDialog>
#include <qapplication.h>

/*
 * 用于存放一些通用的工具方法
 * 例如资源加载,常用的复杂类型转换等
 * 注意该类的对外方法要求必须是静态的
 * 原则上本类中应该尽量没有属性和字段
 */
class CommonHelper
{
public:
	CommonHelper();
	~CommonHelper();
	static void setStyle(const QString& style);//加载属性表
	static bool isADicomOrDicomDir(std::string filename);//用于判断一个文件是否为遵循Dicom3.0标准的图片文件或Dir文件
};

