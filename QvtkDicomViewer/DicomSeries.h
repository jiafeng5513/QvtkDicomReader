#pragma once
#include <string>
#include "DicomImage.h"
#include <vector>

/*
 * 用于存储series有关的信息
 * 在Dicom标准中,series指的是连续的若干张图片构成的一个序列
 * 
 * 构造:
 */
class DicomSeries
{
public:
	DicomSeries();
	~DicomSeries();
public:
	std::string Manufacturer = "";					//制造商
	std::string StationName = "";					//仪器名称
	std::string SeriseNumber = "";					//
	std::string NumberOfSeriesRelatedInstances = "";	//
	std::string XmlFilePath = "";					//这个series中的xml文件的绝对文件路径
	std::vector<DicomImage*> ImageList;			//该Series中的图片存储在这个表里
};

