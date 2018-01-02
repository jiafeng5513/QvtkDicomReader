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
 *2018年1月2日16:39:00
 *	  考虑增加从单张图片初始化和从series文件夹初始化的功能
 *	  加载性能需要优化
 */

class DicomDataBase
{
public:
	static DicomDataBase* getInstance();
	void Init(std::string dir);//从DICOMDIR文件初始化
	void InitFromSingleImage(std::string ImageFileName);//从单张图片初始化
	void InitFromSeriesFolder(std::string SeriesFolder);//从seriesfolder初始化
private:
	//static DicomDataBase* m_pSingleton;
	DicomDataBase();
	~DicomDataBase();
public:
	std::vector<DicomPatient*> PatientList;//数据库中保存的所有的病人
	std::string DicomDirFilePath;//DicomDir文件的绝对路径
	std::string DicomForderPath;//DicomDIR文件所在的文件夹,在后面接上RefFileID就得到图片的绝对路径
	DicomPatient*getPatientById(std::string &patientid);//使用patient id查询一个patient对象
	DicomPatient*get_previous_patient(DicomPatient*current_patient); //获取前一个病人
	DicomPatient*get_latter_patient(DicomPatient*current_patient);//获取后一个病人
};
///单例模式
//DicomDataBase* DicomDataBase::m_pSingleton = new DicomDataBase();
