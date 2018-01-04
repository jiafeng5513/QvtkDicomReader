#include "CommonHelper.h"
#include <sstream>
#include <iostream>
#include <ppltasks.h>
#include <fstream>

CommonHelper::CommonHelper()
{
}


CommonHelper::~CommonHelper()
{
}
/*
 * 加载属性表
 */
void CommonHelper::setStyle(const QString& style)
{
	QFile qss(style);
	qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();
}
/*
 * 用于判断一个文件是否为遵循Dicom3.0标准的图片文件或Dir文件
 * 该函数不完善,请谨慎使用.
 * 在某些极端特殊情况下,将会吧把空的文本文档当做合法的文件,
 * 实际上我们在这里只是校验了前128字节的信息
 */
 bool CommonHelper::isADicomOrDicomDir(std::string filename)
 {
	
	 std::filebuf *pbuf;
	 std::ifstream filestr;
	 long size;
	 char * buffer;

	 filestr.open(filename, std::ios::binary);// 二进制打开 
										 // 获取filestr对应buffer对象的指针  
	 pbuf = filestr.rdbuf();

	 //调用buffer对象方法获取文件大小 
	 size = pbuf->pubseekoff(0, std::ios::end, std::ios::in);
	 //size = 132;
	 pbuf->pubseekpos(0, std::ios::in);

	 // 分配内存空间 
	 buffer = new char[size];

	 // 获取文件内容 
	 pbuf->sgetn(buffer, size);

	 filestr.close();
	 //std::cout.write(buffer, size);
	 // 使用
	 int b0 = buffer[0] & 255, b1 = buffer[1] & 255, b2 = buffer[2] & 255, b3 = buffer[3] & 255;

	 if (buffer[128] == 68 && buffer[129] == 73 && buffer[130] == 67 && buffer[131] == 77)
	 {
		 return true;
	 }
	 else if ((b0 == 8 || b0 == 2) && b1 == 0 && b3 == 0)
	 {
		 return true;
	 }
	 return false;

	 delete[]buffer;
	 delete[]buffer;
 }
