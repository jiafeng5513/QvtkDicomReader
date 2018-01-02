#include "DicomDataBase.h"
//dcmtk-64
#include <dcmdata/dcdicdir.h>
#include <dcmdata/dcdeftag.h>
#include "DicomDir.h"
#include <QFileSystemModel>

/*
 * 静态:获取实例,实现单件模式
 */
DicomDataBase * DicomDataBase::getInstance()
{
	/*if (NULL == m_pSingleton)
	{
		m_pSingleton = new DicomDataBase();
	}*/
	static DicomDataBase single;
	return &single;
	//return m_pSingleton;
}
/*
 * 从DICOMDIR初始化
 */
 void DicomDataBase::Init(std::string dir)
 {
	//获取DICOMDIR文件所在的文件夹

	 QFile *dirfile = new QFile(QString::fromStdString(dir));
	 auto dirfile_info = QFileInfo(*dirfile);
	 QString FolderPrefix = dirfile_info.absolutePath();//取到前缀
	 dirfile->close();//释放文件

	 DcmDicomDir dicomdir(dir.c_str());
#if _DEBUG
	 std::filebuf fb;
	 fb.open("DicomDir.log", std::ios::out);//输出日志文件
	 std::ostream out(&fb);
	 dicomdir.print(out);
	 fb.close();

	 DcmFileFormat DCMFF = dicomdir.getDirFileFormat();
	 std::filebuf lb;
	 lb.open("DcmFileFormat.log", std::ios::out);//输出日志文件2
	 std::ostream out2(&lb);
	 DCMFF.print(out2);
	 lb.close();
#endif

	 DcmDirectoryRecord * DicomDirRootRecord = &(dicomdir.getRootRecord());//用于提取该文件的根目录

																		   //bool flag = DicomDirRootRecord->isEmpty();

	 DcmDirectoryRecord *   PatientRecord = NULL;//第一层,patient(N)
	 DcmDirectoryRecord *   StudyRecord = NULL;	//第二层,study	(N)
	 DcmDirectoryRecord *   SeriesRecord = NULL;	//第三层,series	(N)
	 DcmDirectoryRecord *   ImageRecord = NULL;	//第四层,Image	(N)
	 OFString tmpString;


	 int i = 0, j = 0, k = 0, l = 0;//四层循环变量
	 while (((PatientRecord = DicomDirRootRecord->getSub(i)) != NULL))
	 {
		 DicomPatient *m_patient = new DicomPatient();
		 if (PatientRecord->findAndGetOFString(DCM_PatientID, tmpString).good())
		 {
			 m_patient->PatientID = tmpString.c_str();
		 }
		 if (PatientRecord->findAndGetOFString(DCM_PatientName, tmpString).good())
		 {
			 m_patient->PatientName = tmpString.c_str();
		 }
		 while (((StudyRecord = PatientRecord->getSub(j)) != NULL))
		 {
			 DicomStudy *m_study = new DicomStudy();
			 if (StudyRecord->findAndGetOFString(DCM_PatientBirthDate, tmpString).good())
			 {
				 m_study->PatientBirthDate = tmpString.c_str();
			 }
			 if (StudyRecord->findAndGetOFString(DCM_PatientSex, tmpString).good())
			 {
				 m_study->PatientSex = tmpString.c_str();
			 }
			 if (StudyRecord->findAndGetOFString(DCM_PatientAge, tmpString).good())
			 {
				 m_study->PatientAge = tmpString.c_str();
			 }
			 if (StudyRecord->findAndGetOFString(DCM_StudyID, tmpString).good())
			 {
				 m_study->StudyId = tmpString.c_str();
			 }
			 if (StudyRecord->findAndGetOFString(DCM_StudyID, tmpString).good())
			 {
				 m_study->StudyId = tmpString.c_str();
			 }
			 if (StudyRecord->findAndGetOFString(DCM_ModalityLUTType, tmpString).good())
			 {
				 m_study->ModulationType = tmpString.c_str();
			 }
			 if (StudyRecord->findAndGetOFString(DCM_InstitutionName, tmpString).good())
			 {
				 m_study->InstitutionName = tmpString.c_str();
			 }
			 while (((SeriesRecord = StudyRecord->getSub(k)) != NULL))
			 {
				 DicomSeries *m_series = new DicomSeries();
				 if (SeriesRecord->findAndGetOFString(DCM_SeriesNumber, tmpString).good())
				 {
					 m_series->SeriseNumber = tmpString.c_str();
				 }
				 if (SeriesRecord->findAndGetOFString(DCM_StationName, tmpString).good())
				 {
					 m_series->StationName = tmpString.c_str();
				 }
				 if (SeriesRecord->findAndGetOFString(DCM_Manufacturer, tmpString).good())
				 {
					 m_series->Manufacturer = tmpString.c_str();
				 }
				 if (SeriesRecord->findAndGetOFString(DCM_NumberOfSeriesRelatedInstances, tmpString).good())
				 {
					 m_series->NumberOfSeriesRelatedInstances = tmpString.c_str();
				 }
				 while (((ImageRecord = SeriesRecord->getSub(l)) != NULL))
				 {
					 DicomImage * m_image = new DicomImage();
					 if (ImageRecord->findAndGetOFStringArray(DCM_ReferencedFileID, tmpString, true).good())
					 {
						 m_image->ReferencedFileID = tmpString.c_str();
						 QString temp = QString::fromStdString(FolderPrefix.toStdString() + "\\" + m_image->ReferencedFileID);
						 temp.replace(QChar('\\'), QChar('/'));
						 m_image->AbsFilePath = temp.toStdString();
					 }
					 if (ImageRecord->findAndGetOFString(DCM_InstanceCreationDate, tmpString).good())
					 {
						 m_image->InstanceCreationDate = tmpString.c_str();
					 }
					 if (ImageRecord->findAndGetOFString(DCM_KVP, tmpString).good())
					 {
						 m_image->kpv = tmpString.c_str();
					 }
					 if (ImageRecord->findAndGetOFString(DCM_SliceLocation, tmpString).good())
					 {
						 m_image->SliceLocation = tmpString.c_str();
					 }
					 if (ImageRecord->findAndGetOFString(DCM_Rows, tmpString).good())
					 {
						 m_image->row = atoi(tmpString.c_str());
					 }
					 if (ImageRecord->findAndGetOFString(DCM_Columns, tmpString).good())
					 {
						 m_image->col = atoi(tmpString.c_str());
					 }
					 if (ImageRecord->findAndGetOFString(DCM_InstanceNumber, tmpString).good())
					 {
						 m_image->InstanceNumber = tmpString.c_str();
					 }
					 l++;
					 m_series->ImageList.push_back(m_image);
				 }
				 l = 0;
				 k++;
				 m_study->SeriesList.push_back(m_series);
			 }
			 k = 0;
			 j++;
			 m_patient->StudyList.push_back(m_study);
		 }
		 j = 0;
		 i++;
		 this->PatientList.push_back(m_patient);
	 }
 }
 /*
  * 从单张图片初始化
  */
 void DicomDataBase::InitFromSingleImage(std::string ImageFileName)
 {
	//构造Patient/Study/Series,然后添加这张图
	/*
	 * patient的ID和Name
	 * Study的ID
	 * 所在的SeriesID
	 * 从图片文件中获取这些信息并做完整性验证,如果验证失败要用缺省信息补全
	 */
	 DcmFileFormat fileformat;
	 OFCondition status = fileformat.loadFile(ImageFileName.c_str());
	 if (status.bad())
	 {
		 //异常
		 return;
	 }
	 OFString temp_OFString;
	 //构造开始
	 //image
	 DicomImage * m_image = new DicomImage();
	 if (fileformat.getDataset()->findAndGetOFStringArray(DCM_ReferencedFileID, temp_OFString, true).good())
	 {
		 m_image->ReferencedFileID = temp_OFString.c_str();
		 m_image->AbsFilePath = ImageFileName;
	 }
	 //series
	 DicomSeries *m_series = new DicomSeries();
	 if (fileformat.getDataset()->findAndGetOFStringArray(DCM_SeriesNumber, temp_OFString, OFTrue).good())
	 {
		 m_series->SeriseNumber = temp_OFString.c_str();
	 }
	 m_series->ImageList.push_back(m_image);
	 //study
	 DicomStudy *m_study = new DicomStudy();
	 if (fileformat.getDataset()->findAndGetOFStringArray(DCM_StudyID, temp_OFString, OFTrue).good())
	 {
		 m_study->StudyId = temp_OFString.c_str();
	 }
	 m_study->SeriesList.push_back(m_series);
	 //patient
	 DicomPatient *m_patient = new DicomPatient();
	 if (fileformat.getDataset()->findAndGetOFStringArray(DCM_PatientName, temp_OFString, OFTrue).good())
	 {
		 m_patient->PatientName = temp_OFString.c_str();
	 }
	 if (fileformat.getDataset()->findAndGetOFStringArray(DCM_PatientID, temp_OFString, OFTrue).good())
	 {
		 m_patient->PatientID = temp_OFString.c_str();
	 }
	 this->PatientList.push_back(m_patient);
 }
 /*
  *	从seriesfolder初始化
  */
void DicomDataBase::InitFromSeriesFolder(std::string SeriesFolder)
{
	//构造Patient/Study,然后添加这个Series中的全部图
	QDir * folder = new QDir(QString::fromStdString(SeriesFolder));
	QStringList files = folder->entryList(QDir::Files | QDir::Readable, QDir::Name);//获取文件夹中的全部文件的名字
	QString Prefix = folder->absolutePath();//构造文件名前缀
	Prefix.append("\\");
	Prefix.replace(QChar('\\'), QChar('/'));
	
	//对文件进行排序,依次视为DICOM文件打开,按照instance number (0020,0013)从小到大排序
	//排序结果放在另一个容器中,
	//如果打开失败,认为该文件不是正确的DICOM文件.不添加到新的容器中

	/*
	 * 根据要排序的文件的特点,采用散列查找的方式排序是最高效的方法
	 */
	std::map<int,std::string > * AllTheFiles = new std::map<int, std::string>();
	OFString temp_OFString;
	//1.入库
	for(int i=0;i<files.size();i++)
	{
		QString _currentfilename = Prefix;
		_currentfilename.append(files.at(i));
		DcmFileFormat fileformat;
		OFCondition status = fileformat.loadFile(_currentfilename.toStdString().c_str());
		if (status.good())
		{
			if (fileformat.getDataset()->findAndGetOFStringArray(DCM_InstanceNumber, temp_OFString, true).good())
			{
				AllTheFiles->insert(std::map<int, std::string>::value_type(std::atoi(temp_OFString.c_str()), files.at(i).toStdString()));
			}
		}
		else
		{
			continue;
		}
	}
	//2.按顺序检出
	std::vector<std::string> * RfidInSeriesFolder=new std::vector<std::string>();
	std::map<int, std::string>::iterator iter;
	for(int i=1;i<AllTheFiles->size()+1;i++)
	{
		iter = AllTheFiles->find(i);
		if (iter != AllTheFiles->end())//这个标号查找成功,出库
		{
			RfidInSeriesFolder->push_back(iter->second);
		}
		else
		{
			continue;
		}
	}

	//开始构造
	/*
	 * 				 DicomSeries *m_series = new DicomSeries();
				 if (SeriesRecord->findAndGetOFString(DCM_SeriesNumber, tmpString).good())
				 {
					 m_series->SeriseNumber = tmpString.c_str();
				 }
	 */
	for(int i=0;i<RfidInSeriesFolder->size();i++)
	{
		DicomImage * m_image = new DicomImage();
		if (ImageRecord->findAndGetOFStringArray(DCM_ReferencedFileID, tmpString, true).good())
		{
			m_image->ReferencedFileID = tmpString.c_str();
			QString temp = QString::fromStdString(FolderPrefix.toStdString() + "\\" + m_image->ReferencedFileID);
			temp.replace(QChar('\\'), QChar('/'));
			m_image->AbsFilePath = temp.toStdString();
		}
	}

}

/*
 * 构造(私有)
 */
DicomDataBase::DicomDataBase()
{
}


DicomDataBase::~DicomDataBase()
{
}
/*
 * 使用patient id查询一个patient对象
 */
DicomPatient * DicomDataBase::getPatientById(std::string &patientid)
{
	for (int i=0;i<PatientList.size();i++)
	{
		if (PatientList[i]->PatientID == patientid)
		{
			return PatientList[i];
		}
	}
	return nullptr;
}
/*
 * 获取前一个病人
 * 参数:当前病人
 * 特性:越界时返回原值
 */
 DicomPatient * DicomDataBase::get_previous_patient(DicomPatient * current_patient)
 {
	for(int i=0;i<PatientList.size();i++)
	{
		if (PatientList[i]->PatientID== current_patient->PatientID)//找到当前病人
		{
			return (i - 1 < 0 ? PatientList[i] : PatientList[i- 1]);
		}
	}
	 return current_patient;
 }
 /*
  *	获取后一个病人
  *	参数:当前病人
  *	特性:越界返回原值
  */
 DicomPatient * DicomDataBase::get_latter_patient(DicomPatient * current_patient)
 {
	 for (int i = 0; i<PatientList.size(); i++)
	 {
		 if (PatientList[i]->PatientID == current_patient->PatientID)//找到当前病人
		 {
			 return (i + 2 >PatientList.size() ? PatientList[i] : PatientList[i + 1]);
		 }
	 }
	 return current_patient;
 }
