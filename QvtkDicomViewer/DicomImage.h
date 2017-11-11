#pragma once
#include <string>

/*
 * 存储单张Dicom图像的相关信息
 * 
 * 构造:
 * 
 */
class DicomImage
{
public:
	DicomImage();
	~DicomImage();
public:
	std::string ReferencedFileID;    //Dicom图像的绝对路径
	std::string InstanceCreationDate;//图片建立时间
	std::string kpv;				 //看起来可能有点用
	std::string SliceLocation;		 //emmm
	std::string InstanceNumber;      //组内编号
	std::string AbsFilePath;		 //该文件在文件系统上的绝对文件名
	int row;
	int col;//图片尺寸
	std::string PixelSpacing;        //像素空间
public:
	std::string getReferencedFileID();
	void setReferencedFileID(std::string value);

	std::string getInstanceCreationDate();
	void setInstanceCreationDate(std::string value);

	std::string getkpv();
	void setkpv(std::string value);

	std::string getSliceLocation();
	void setSliceLocation(std::string value);

	std::string getPixelSpacing();
	void setPixelSpacing(std::string value);

	int getRow();
	void setRow(int value);

	int getCol();
	void setCol(int value);
};

