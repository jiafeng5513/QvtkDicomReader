#include "DicomPatient.h"


/*
 * 默认构造函数
 */
DicomPatient::DicomPatient()
{
	indexOfCurrentStudy = 0; 
	indexOfCurrentSeries = 0;
	indexOfCurrentImage = 0;
}
/*
 * 拷贝构造函数
 * 对于Patient对象进行深拷贝,但是对于Studylist中索引的和间接索引的对象是地址拷贝
 * 也就是浅拷贝,由于数据类这边不需要进行写操纵,所以这是无所谓的,只要读出的值是对的
 * 就可以了
 * 未来应该进行属性封装阻止一切来自外部的对指针的修改
 */
DicomPatient::DicomPatient(DicomPatient * old)
{
	this->PatientName = old->PatientName;
	this->PatientID =old->PatientID;
	this->indexOfCurrentStudy = old->indexOfCurrentStudy; 
	this->indexOfCurrentSeries = old->indexOfCurrentSeries;
	this->indexOfCurrentImage = old->indexOfCurrentImage; 

	for(int i=0;i<old->StudyList.size();i++)
	{
		this->StudyList.push_back(old->StudyList[i]);
	}
}
/*
 * 析构
 */
DicomPatient::~DicomPatient()
{
}
/*
 * 复位,将所有的索引复位
 */
void DicomPatient::reset()
{
	indexOfCurrentStudy = 0;
	indexOfCurrentSeries = 0;
	indexOfCurrentImage = 0;
}
/*
 * 获取当前Study
 */
DicomStudy * DicomPatient::getCurrentDicomStudy()
{
	return this->StudyList[indexOfCurrentStudy];
}
/*
 * 通过studyid设置当前的study
 */
void DicomPatient::setCurrentDicomStudyById(std::string &studyid)
{
	int index = 0;
	for(index=0;index<this->StudyList.size(); index++)
	{
		if (StudyList[index]->StudyId == studyid)
		{
			indexOfCurrentStudy = index;
			return;
		}
	}
	indexOfCurrentStudy = 0;
}
/*
 * 获取当前Series
 */
DicomSeries * DicomPatient::getCurrentDicomSeries()
{
	return this->StudyList[indexOfCurrentStudy]->SeriesList[indexOfCurrentSeries];
}
/*
 * 通过seriesnum设置当前的Series
 */
void DicomPatient::setCurrentDicomSeriesById(std::string & seriesnum)
{
	//检索所有的series,检索到的时候直接把study和series索引全都切换
	for(int i=0;i<this->StudyList.size();i++)
	{
		for(int j=0;j<this->StudyList[i]->SeriesList.size();j++)
		{
			if(this->StudyList[i]->SeriesList[j]->SeriseNumber==seriesnum)
			{
				indexOfCurrentSeries = j;
				indexOfCurrentStudy = i;
				return;
			}
		}
	}
}
/*
 * 获取当前Image
 */
DicomImage * DicomPatient::getCurrentDicomImage()
{
	return this->StudyList[indexOfCurrentStudy]->SeriesList[indexOfCurrentSeries]->ImageList[indexOfCurrentImage];
}
/*
 * 获取下一张Image,并移动指针,越界自动回到第一张
 */
DicomImage * DicomPatient::getNextDicomImage()
{
	indexOfCurrentImage = 
		(indexOfCurrentImage == StudyList[indexOfCurrentStudy]->
								SeriesList[indexOfCurrentSeries]->
								ImageList.size() - 1 ?
								0 : indexOfCurrentImage + 1);
	return getCurrentDicomImage();
}
/*
 * 通过RFID在当前病人的信息中检索出一张图片
 */
DicomImage * DicomPatient::getDicomImageByRfid(std::string & rfid)
{
	//检索这个病人的全部Image
	//一旦检索成功,立即切换Study,Series,Image的Current索引,并返回当前图片
	for(int i=0;i<this->StudyList.size();i++)
	{
		for(int j=0;j<this->StudyList[i]->SeriesList.size();j++)
		{
			for(int k=0;k<this->StudyList[i]->SeriesList[j]->ImageList.size();k++)
			{
				if(this->StudyList[i]->SeriesList[j]->ImageList[k]->ReferencedFileID== rfid)
				{
					indexOfCurrentStudy = i;
					indexOfCurrentSeries = j;
					indexOfCurrentImage = k;
					return  getCurrentDicomImage();
				}
			}
		}
	}
}
