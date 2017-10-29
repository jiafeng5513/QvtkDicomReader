#pragma once
#include <string>
#include <vector>
#include "DicomPatient.h"

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
	static DicomDataBase* getInstance();
	void Init(std::string dir);
private:
	//static DicomDataBase* m_pSingleton;
	DicomDataBase();
	~DicomDataBase();
public:
	std::vector<DicomPatient*> PatientList;//数据库中保存的所有的病人
	std::string DicomDirFilePath;//DicomDir文件的绝对路径
	std::string DicomForderPath;//DicomDIR文件所在的文件夹,在后面接上RefFileID就得到图片的绝对路径
};
///单例模式
//DicomDataBase* DicomDataBase::m_pSingleton = new DicomDataBase();
