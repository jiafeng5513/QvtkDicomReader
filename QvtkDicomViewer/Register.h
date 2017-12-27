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
#include <itkImageFileReader.h>

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
	typedef itk::Image< PixelType, 2 >  FixedImageType;
	typedef itk::Image< PixelType, 2 >  MovingImageType;
	typedef itk::ImageFileReader< FixedImageType  >   FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType >   MovingImageReaderType;
public:
	Register(QWidget *parent = Q_NULLPTR);
	~Register();
private:
	Ui::Register ui;
private:
	///配准方法
	void TranslationReg(FixedImageReaderType::Pointer _fixedImageReader, MovingImageReaderType::Pointer _movingImageReader);
	void CenteredSimilarityTransformReg(FixedImageReaderType::Pointer _fixedImageReader,
		MovingImageReaderType::Pointer _movingImageReader, double initialScale=1.0, double initialAngle=0.0, double steplength=1.0);
	void AffineTransformReg(FixedImageReaderType::Pointer _fixedImageReader, MovingImageReaderType::Pointer _movingImageReader, double steplength=1.0, unsigned int maxNumberOfIterations=300);
	void MultiTransformReg(FixedImageReaderType::Pointer _fixedImageReader, MovingImageReaderType::Pointer _movingImageReader, PixelType backgroundGrayLevel=128.0);

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
	FixedImageReaderType::Pointer   fixedImageReader ;
	MovingImageReaderType::Pointer  movingImageReader ;
	FixedImageReaderType::Pointer   defaultImageReader;//用于加载默认图片用来初始化
	RegistrationFunc m_CurrentRegFunc;//选路变量

};