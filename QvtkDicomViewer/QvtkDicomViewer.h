#pragma once
//内部
#include "ui_QvtkDicomViewer.h"
//Qt
#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFileDialog>
//vtk
#include <vtkRenderWindow.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkAutoInit.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDICOMImageReader.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkActor2D.h>
#include <myVtkInteractorStyleImage.h>
#include <vtkDistanceWidget.h>
#include <vtkAngleWidget.h>

VTK_MODULE_INIT(vtkRenderingFreeType)
//Qt主程序类
class QvtkDicomViewer : public QMainWindow
{
	Q_OBJECT

public:
	QvtkDicomViewer(QWidget *parent = Q_NULLPTR);
	enum CURSOR		//光标类型 
	{POINTRE,		//默认指针
		PROTRACTOR,	//量角器
		RULER			//测距尺
	};
private:
	Ui::QvtkDicomViewerClass ui;
	vtkSmartPointer< vtkImageViewer2 > m_pImageViewer;
	vtkSmartPointer< vtkRenderer > m_pRenderder;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<vtkDICOMImageReader> reader;
	vtkSmartPointer<vtkTextProperty> sliceTextProp;
	vtkSmartPointer<vtkTextMapper> sliceTextMapper;
	vtkSmartPointer<vtkActor2D> sliceTextActor;
	vtkSmartPointer<vtkTextProperty> usageTextProp;
	vtkSmartPointer<vtkTextMapper> usageTextMapper;
	vtkSmartPointer<vtkActor2D> usageTextActor;
	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle;
	vtkSmartPointer<vtkDistanceWidget> distanceWidget;//测距
	vtkSmartPointer<vtkAngleWidget> angleWidget;//测量角度
private:
	///内部操作
	void DoRender(std::string folder);//绑定数据源,显示Dicom数据
public slots:
	void OnOpenFile();
	void OnRenderText();
	void OnForward();//前一张
	void OnBackward();//后一张
	void OnSelectedPointer();//选中默认鼠标指针工具
	void OnSelectedProtractor();//选中量角器工具
	void OnSelectedRuler();//选中测距尺工具
};
