#pragma once
#include <head_all.h>
#include <QVTKWidget.h>
#include <itkImageToVTKImageFilter.h>
#include "itkGDCMImageIO.h"

enum  SegmentFunc
{
	NULL_Seg,
	Seg_connectedthres,
	Seg_neighconnected,
	Seg_confidconnected,
	Seg_waterseg,
	Seg_fastmarching,
	Seg_shapedectection,
	Seg_gibblsprior
};
const unsigned int Dimension = 2;
typedef  float  InputPixelType;
typedef  float  OutputPixelType;
typedef  float  InternalPixelType;

typedef itk::Image< InputPixelType, Dimension >   InputImageType;
typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
typedef itk::ImageFileReader< InternalImageType > ReaderType;
typedef itk::ImageFileWriter<  OutputImageType  > WriterType;
typedef itk::GDCMImageIO      ImageIOType;
typedef itk::CastImageFilter< InternalImageType, OutputImageType >CastingFilterType;
typedef itk::CurvatureFlowImageFilter< InternalImageType, InternalImageType > CurvatureFlowImageFilterType;//平滑用的

class MyStyle : public vtkInteractorStyleImage
{
public:
	static MyStyle* New();
	void OnLeftButtonDown() override;
};
class vtkImageSegmentCallback : public vtkCommand
{
public:
	static vtkImageSegmentCallback *New();
	vtkImageSegmentCallback();
	~vtkImageSegmentCallback();
	void SetPicker(vtkPropPicker *picker);
	void SetAnnotation(vtkCornerAnnotation *annotation);
	void SetViewer(vtkImageViewer2 *viewer);
	void SetDir(std::string dir);
	void SetQvtk(QVTKWidget* qvtk);
	void SetCount(int count);
	virtual void Execute(vtkObject *, unsigned long event, void *);
private:
	///分割算法
	void seg_connectedthres(std::string filename,double pos_x,double pos_y,double pixel_f,double pixel_s);	   //区域生长法
	void seg_neighconnected(std::string filename, double pos_x, double pos_y, double pixel_f, double pixel_s); //邻域连接法
	void seg_confidconnected(std::string filename, double pos_x, double pos_y);								   //置信连接法
	void seg_waterseg(std::string filename, double NumberOfIterations,double ConductanceParameter, 
									double UsePrincipleComponents, double watershedLevel, double Threshold);   //分水岭算法
	void seg_fastmarching(std::string filename, double pos_x, double pos_y, double alpha, double beta, 
									double sigma, double UpperThreshold, double StoppingValue);				   //快速匹配
	void seg_shapedectection(std::string filename, double pos_x, double pos_y, double seedValue, double alpha, 
									double beta, double sigma,double curvatureScaling, double propagationScaling);//形状检测

private:
	
	vtkImageViewer2 *Viewer;
	vtkPropPicker *Picker;
	vtkCornerAnnotation *Annotation;
	vtkPointData* PointData;
	QVTKWidget* qvtk;
	int count;
};