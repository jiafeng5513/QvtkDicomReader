#pragma once
#include <string>
#include <vector>
#include "DicomStudy.h"

/*
 * ����洢�Ͳ�����ص�һЩ��Ϣ
 * ��Dicom��׼��,һ�����ݿ��д���Ŷ�����˵���Ϣ
 * ÿ�����˿�����1�����Study,ÿ��Study������1�����Series,ÿ��Series�����ж���Image,
 * ͬһ�����˵������¼���Ϣ�Ĳ��������Ͳ���ID����ͬ��
 * ע��,��Ҳ������������ӡ��,���ǹ��ڲ��˵ĸ�����Ϣ����������ڲ�������㼶��
 * ���ǿ��ǵ�,һ���˵�һ�������ܶ�μ��,ÿ�μ�鶼�п���ʹ��1������ҽѧӰ���ֶ�,������������Ϣ
 * ��������,���ʱ��,�����Ա�,���ǲ��̶���,��Щ��ϢҪ����ÿ�μ����䶯,
 * ����Dicom��,������Ϊÿ��������Ψһ��PatientID��Ϊʶ��,ֻҪID��ͬ,���н���PACSϵͳ��ҽѧӰ�񶼽�
 * ����ͬһ��Ŀ¼������
 * �������ǲ����ǲ��˸��������
 */
class DicomPatient
{
public:
	DicomPatient();
	~DicomPatient();
public:
	std::string PatientName;
	std::string PatientID;
	std::vector<DicomStudy*> StudyList; //������˵�Study�б�
};
