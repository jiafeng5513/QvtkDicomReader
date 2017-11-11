#pragma once
#include <string>
#include <vector>
#include "DicomStudy.h"

/*
 * 该类存储和病人相关的一些信息
 * 在Dicom标准中,一个数据库中存放着多个病人的信息
 * 每个病人可能有1到多个Study,每个Study可能有1到多个Series,每个Series可能有多张Image,
 * 同一个病人的所有下级信息的病人姓名和病人ID是相同的
 * 注意,这也许不符合主观印象,但是关于病人的个人信息并不都存放在病人这个层级上
 * 我们考虑到,一个人的一生会做很多次检查,每次检查都有可能使用1到多种医学影像手段,所以其他的信息
 * 比如年龄,检查时间,甚至性别,都是不固定的,这些信息要随着每次检查而变动,
 * 而在Dicom中,我们认为每个病人有唯一的PatientID作为识别,只要ID相同,所有进入PACS系统的医学影像都将
 * 进入同一个目录被管理
 * 这里我们不考虑病人改名的情况
 */
class DicomPatient
{
public:
	DicomPatient();//构造
	DicomPatient(DicomPatient*old);//拷贝构造函数
	~DicomPatient();
public:
	std::string PatientName;
	std::string PatientID;
	std::vector<DicomStudy*> StudyList; //这个病人的Study列表
	int indexOfCurrentStudy; //当前正在读取的Study的索引号.从0开始,默认为0
	int indexOfCurrentSeries;//当前正在读取的Series的索引号,从0开始,默认为0
	int indexOfCurrentImage; //当前正在读取的Image的索引号,从0开始,默认为0

	void reset();//复位

	DicomStudy*getCurrentDicomStudy();//获取当前Study
	void setCurrentDicomStudyById(std::string &studyid);//通过studyid设置当前的study

	DicomSeries* getCurrentDicomSeries();//获取当前Series
	void setCurrentDicomSeriesById(std::string &seriesnum);//通过seriesnum设置当前的Series

	DicomImage* getCurrentDicomImage();//获取当前Image
	DicomImage* getNextDicomImage();//获取下一张Image,并移动指针,越界自动回到第一张
	DicomImage* getDicomImageByRfid(std::string &rfid);//通过RFID在当前病人的信息中检索出一张图片
	DicomImage* getDicomImageByIndex(int index);//取当前study,当前series的第index张图片,如果越界,返回原来索引所指向的那一张图片
};

