#pragma once
#include <string>

/*
 *实体类,用于Dicom数据文件的读取和内存存储
 *
 *构造:1.从DICOMDIR文件构造
 *	   2.从含有DICOMDIR文件的文件夹构造
 *	   3.从含有一个Series的文件夹构造
 *	   4.从单张图片构造
 *	   
 *2017年10月28日21:37:38
 *	  目前只考虑从DICOMDIR文件构造的情况
 *
 */
class DicomDataBase
{
public:
	DicomDataBase(std::string dir);
	~DicomDataBase();
};

