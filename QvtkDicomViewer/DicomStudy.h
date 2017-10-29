#pragma once
#include <string>
#include <vector>
#include "DicomSeries.h"

/*
 * 存储关于study的数据
 * 在Dicom标准中,Study指的是一系列series组成的集合,
 * 一个study中的所有series是同种设备(如CT,MRI,PETCT等)生成的,
 * 同一个病人可能会同时进行多种设备的检查,这种时候每个病人就对应多个Study,
 * 如果病人只进行了一种检查,就只有一个Study.
 */
class DicomStudy
{
public:
	DicomStudy();
	~DicomStudy();
public:
	std::string ModulationType;    //检查类型
	std::string InstitutionName;
	std::string PatientBirthDate;
	std::string PatientSex;
	std::string PatientAge;
	std::string StudyId;
	std::vector<DicomSeries> SeriesList;//该Study中的Series列表
};

