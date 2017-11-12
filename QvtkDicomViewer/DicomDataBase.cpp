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
