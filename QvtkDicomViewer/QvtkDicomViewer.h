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
#include <vtkContourWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkBiDimensionalWidget.h>
#include "vtkBiDimensionalCallback.h"
#include "SlicePlayer.h"
//itk

class DicomDataBase;
/*
 * 依赖说明:
 * 1.附加依赖项:与Qt相关的依赖项在解决方案->属性中设置
 *				其他依赖项在属性管理器->Debug_x64,Release_x64中设置
 * 2.库目录和包含目录:在解决方案->属性中设置
 * 3.依赖版本:Qt5.9.1
 *			  vtk8.0.0
 *			  itk4.12.0
 *			  dcmtk3.6.2(64位msvc15-2017)
 * 4.Qt目录:
 *			在解决方案目录->QvtkDicomViewer->QvtkDicomViewer.vcxproj.user文件中
 *			含有Qt的目录
 */
VTK_MODULE_INIT(vtkRenderingFreeType)
//Qt主程序类
class QvtkDicomViewer : public QMainWindow
{
	Q_OBJECT

public:
	QvtkDicomViewer(QWidget *parent = Q_NULLPTR);
	enum CURSOR		
	{	POINTRE,		//默认指针
		ZOOM,			//缩放
		GRAYLEVEL,		//灰阶
		PROTRACTOR,		//量角器
		RULER,			//测距尺
		CONTOUR,		//轮廓
		BIDI,			//二维尺
		MOVE			//移动
	}CursorType;
private:
	//CURSOR CursorType;//光标类型
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
	vtkSmartPointer<vtkContourWidget> contourWidget;//轮廓选框
	vtkSmartPointer<vtkAxesActor> axes;
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation;
	vtkSmartPointer<vtkOrientationMarkerWidget> widget;
	vtkSmartPointer<vtkBiDimensionalWidget> biDimensionalWidget;
	vtkSmartPointer<vtkBiDimensionalCallback> biDimensionalCallback;
	std::string folder;//存储当前读取的Dicom的目录,是否有必要作为全局变量现在尚无定论
	QIcon icon_Play;//播放图标
	QIcon icon_Pause;//暂停图标
	bool PlayFlag;//false:图标应为播放,处于准备播放状态,true:图标应为暂停,处于播放状态并准备暂停
	SlicePlayer *m_slice_player;//这是一个线程类
	std::string Current_patientId;//当前的病人ID
private:
	///内部操作
	void setCursor(CURSOR newValue);
	void DoRender(std::string folder);//绑定数据源,显示Dicom数据
	void SeriesRender(std::string first);//手动加载一个series
	void DirTreeRefresh(DicomDataBase * database);
	void GetMetaDataAndRender(std::string folder);//使用ITK获取元数据,并显示在Docking界面上
	void addDistanceWidget();
	void addAngleWidget();
	void addContourWidget();
	void SetSliceText();
	void SetUsageText();
	void addOrientationMarker();
	void addBiDimensionalWidget();
signals :
	void CursorValueChanged();        //自定义值更改信号,用于监控当前光标的变化
public slots:

	void OnChangeCursorValue();//响应光标值的修改,执行一些刷新和禁用操作
	void OnOpenFile();
	void OnRenderText();
	void OnForward();//前一张
	void OnBackward();//后一张
	void OnResetToFirst();//回到第一张
	void OnSelectedPointer();//选中默认鼠标指针工具
	void OnSelectedProtractor();//选中量角器工具
	void OnSelectedRuler();//选中测距尺工具
	void OnSelectedContour();//选中轮廓工具
	void OnSelectedBiDimensional();//选中二维标尺工具
	void OnSelectedGrayLevel();//选中灰阶工具
	void OnSelectedZoom();//选中缩放工具
	void OnSelectedMove();//选中移动工具
	void OnNegative();//使用负片效果
	void OnReset();//复位
	void OnPlay();//播放/暂停
	void OnStop();//停止
	void OnSwitchProperty();//属性docking窗口的开关
	///正在测试的功能
	void OnTestDCMTK_x64();//测试调用DCMTK-x64读取元数据
	void OnTestReadDICOMDIR();//测试调用DCMTK-x64读取DIR文件
	void receiveData(QString data);//响应DicomDir类传送过来的信号,其中包含了一个病人的ID
};
