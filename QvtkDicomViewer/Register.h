#pragma once

#include <QWidget>
#include "ui_Register.h"
#include <QVTKWidget.h>
#include <vtkImageActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>

#define ITK_IO_FACTORY_REGISTER_MANAGER

//上述宏定义的意义目前还无法否定
//缺乏相应的实验

enum RegistrationFunc
{
	RegFunc_Translation,
	RegFunc_CenteredSimilarity,
	RegFunc_Affine,
	RegFunc_Multi
};

class Register : public QWidget
{
	Q_OBJECT
	typedef  float           PixelType;
	typedef itk::Image< PixelType, 2 > OutputImageType;
	typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
public:
	Register(QWidget *parent = Q_NULLPTR);
	~Register();
private:
	Ui::Register ui;
private:
	///配准方法
	void TranslationReg(char *argv[]);
	void CenteredSimilarityTransformReg(char *argv[]);
	void AffineTransformReg(char*argv[]);
	void MultiTransformReg(char*argv[]);

	void updateOutputImage();
private slots:
	void OnSelectImageFix();		    //选择基准图像
	void OnSelectImageMove();		    //选择待配准图像
	void OnButtonOk();				    //ok-开始计算
	void OnButtonCancel();			    //退出
	void OnSelectTranslation();			//选择平移变换
	void OnSelectCenteredSimilarity();	//选择中心相似二维变换
	void OnSelectAffine();				//选择仿射变换
	void OnSelectMulit();				//选择膜法变换
private:
	vtkSmartPointer<vtkImageActor> actor[4];
	vtkSmartPointer<vtkRenderer> renderer[4];
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor[4];
	vtkSmartPointer<vtkInteractorStyleImage> style[4];
	QVTKWidget * m_output_widgets[4];
	ConnectorType::Pointer connector[4];
	///即将废弃的变量
	int reg_count= RegFunc_Multi;
	RegistrationFunc m_CurrentRegFunc;//选路变量

};