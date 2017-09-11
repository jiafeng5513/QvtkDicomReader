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

VTK_MODULE_INIT(vtkRenderingFreeType)
//Qt主程序类
class QvtkDicomViewer : public QMainWindow
{
	Q_OBJECT

public:
	QvtkDicomViewer(QWidget *parent = Q_NULLPTR);
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
public slots:
	void OnOpenFile();
	void OnRenderText();
	void OnForward();//前一张
	void OnBackward();//后一张
};
