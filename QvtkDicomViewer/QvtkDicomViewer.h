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
#include "DicomPatient.h"
#include "DicomDirTreeModel.h"
#include <Reg_Selector.h>
#include <vtkImageCast.h>

#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkSmartPointer.h>  
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>  
#include <vtkGPUVolumeRayCastMapper.h>  
#include <vtkVolumeProperty.h>  
#include <vtkPiecewiseFunction.h>  
#include <vtkColorTransferFunction.h>  
#include <vtkVolume.h>  
#include <vtkRenderer.h>  
#include <vtkRenderWindow.h>  
#include <vtkRenderWindowInteractor.h>  
#include <vtkCamera.h>
#include "vtkUnsignedCharArray.h"    
#include "vtkPiecewiseFunction.h"    
#include "vtkColorTransferFunction.h"    
#include "vtkVolumeRayCastCompositeFunction.h"    
#include "vtkVolumeRayCastMapper.h"    
#include "vtkImageData.h"       
#include "vtkRenderWindow.h"    
#include "vtkWin32OpenGLRenderWindow.h"  
#include "vtkInteractorStyleTrackballActor.h"  
#include "vtkImageChangeInformation.h"  
#include "vtkGPUVolumeRayCastMapper.h"  
#include "vtkGPUInfoList.h"  
#include "vtkGPUInfo.h"  
#include <vtkStructuredPoints.h>  
#include <vtkStructuredPointsReader.h>   
#include <vtkPiecewiseFunction.h>  
#include <vtkRenderer.h>  
#include <vtkRenderWindow.h>  
#include <vtkRenderWindowInteractor.h>  
#include <vtkVolumeProperty.h>  
#include <vtkVolumeRayCastIsosurfaceFunction.h>  
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <QComboBox>
class DicomDataBase;
/*
 * 依赖说明:
 * 1.附加依赖项:与Qt相关的依赖项在解决方案->属性中设置
 *				其他依赖项在属性管理器->Debug_x64,Release_x64中设置
 * 2.库目录和包含目录:在解决方案->属性中设置
 * 3.依赖版本:Qt5.9.1
 *			  vtk8.0.0
 *			  dcmtk3.6.2(64位msvc15-2017)
 * 4.Qt目录:
 *			在解决方案目录->QvtkDicomViewer->QvtkDicomViewer.vcxproj.user文件中
 *			含有Qt的目录
 */
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)
#include <vtkAutoInit.h>
#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeType,vtkRenderingOpenGL2) 
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)

//Qt主程序类
class QvtkDicomViewer : public QMainWindow
{
	Q_OBJECT

public:
	QvtkDicomViewer(QWidget *parent = Q_NULLPTR);
	QComboBox* reg_combo;
	QComboBox* seg_combo;
	Reg_Selector Reg_Selector_Window;
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
	enum WINDOWWL
	{
		Default,		//默认窗宽窗位
		All,			//全部动态范围
		Abdomen,		//腹部
		BloodVessel,	//血管
		Bones,			//骨骼
		Brain,			//脑
		Medias,			//纵膈
		Lungs			//肺
	}ImageWindow;
private:
	//CURSOR CursorType;//光标类型
	Ui::QvtkDicomViewerClass ui;
	vtkSmartPointer<vtkImageViewer2 > m_pImageViewer;
	vtkSmartPointer<vtkRenderer > m_pRenderder;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<vtkDICOMImageReader> reader;
	//vtkSmartPointer<vtkTextProperty> sliceTextProp;
	//vtkSmartPointer<vtkTextMapper> sliceTextMapper;
	vtkSmartPointer<vtkActor2D> sliceTextActor;//必须是全局的.否则无法擦除上一次得到痕迹
	//vtkSmartPointer<vtkTextProperty> usageTextProp;
	//vtkSmartPointer<vtkTextMapper> usageTextMapper;
	vtkSmartPointer<vtkActor2D> usageTextActor1;
	vtkSmartPointer<vtkActor2D> usageTextActor2;
	vtkSmartPointer<vtkActor2D> usageTextActor3;
	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle;
	vtkSmartPointer<vtkDistanceWidget> distanceWidget;//测距
	vtkSmartPointer<vtkAngleWidget> angleWidget;//测量角度
	vtkSmartPointer<vtkContourWidget> contourWidget;//轮廓选框
	vtkSmartPointer<vtkAxesActor> axes;
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation;
	vtkSmartPointer<vtkOrientationMarkerWidget> widget;
	vtkSmartPointer<vtkBiDimensionalWidget> biDimensionalWidget;
	vtkSmartPointer<vtkBiDimensionalCallback> biDimensionalCallback;

	//包旭12.6加体绘制
	vtkRenderer *ren;
	vtkRenderWindow *renWin;
	vtkImageCast *readerImageCast;
	vtkPiecewiseFunction *opacityTransferFunction;
	vtkColorTransferFunction *colorTransferFunction;
	vtkVolumeProperty *volumeProperty;
	vtkVolumeRayCastCompositeFunction *compositeFunction;
	vtkFixedPointVolumeRayCastMapper *volumeMapper;
	vtkVolume *volume1;
	vtkSmartPointer<vtkDICOMImageReader> reader1;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
	vtkGPUVolumeRayCastMapper *volumeMapper_gpu;
	//////////////////////
	QIcon icon_Play;//播放图标
	QIcon icon_Pause;//暂停图标
	bool PlayFlag;//false:图标应为播放,处于准备播放状态,true:图标应为暂停,处于播放状态并准备暂停
	SlicePlayer *m_slice_player;//这是一个线程类

	QPoint PrePosition;					//前一次的右键点击位置
	QMenu * TreeViewMenu_OnEmpty;		//树右键菜单->树为空
	QMenu * TreeViewMenu_OnPatient;		//树右键菜单->病人节点
	QMenu * TreeViewMenu_OnSeries;		//树右键菜单->Series节点
	QMenu * TreeViewMenu_OnImage;		//树右键菜单->Image节点


	std::string Current_patientId;//当前的病人ID
	DicomPatient * CurrentPatient;		//当前病人
	QModelIndex indexSelect;//树视图中
	DicomDirTreeModel *m_dicomdirtreemodel;

	QAction * volume;
	QAction * volume_gpu;
private:
	///内部操作
	void setCursor(CURSOR newValue);
	void setWindowWL(WINDOWWL newWL);
	void RenderInitializer(std::string folder, int NumOfImage = 1);//渲染器初始化
	void RenderRefresh(std::string imagefilename, int currentPagenumber, int maxPageNumber);//更新渲染
	void DirTreeRefresh(DicomPatient * patient);
	void addDistanceWidget();
	void addAngleWidget();
	void addContourWidget();
	void SetSliceText(int current, int max);			//叠加显示页码信息
	void SetUsageText(std::string imagefilename);
	void addOrientationMarker();
	void addBiDimensionalWidget();
	void CreateContextMenu();//树视图上下文菜单
	void ShowImageByIndex(int Index);//显示当前series中的第Index张图,Index从0开始,与滚动条配合
signals :
	void CursorValueChanged();      //自定义值更改信号,用于监控当前光标的变化
	void WindowWLChanged();			//自定义值更改信号,用于监控当前窗宽窗位模式的变化
public slots:
	void OnChangeCursorValue();		//响应光标值的修改,执行一些刷新和禁用操作
	void OnChangeWindowsWL();		//响应窗宽窗位模式值的修改,执行一些禁用和选定动作
	void OnOpenSeriesFolder();		//改成OnOpenSeriesFolder
	void OnOpenDicomFile();			//打开单张Dicom文件
	void OnOpenDicomDirFile();		//打开DICOMDIR文件
	void OnForward();				//前一张按键
	void OnBackward();				//后一张按键
	void OnResetToFirst();			//回到第一张
	void OnSelectedPointer();		//选中默认鼠标指针工具
	void OnSelectedProtractor();	//选中量角器工具
	void OnSelectedRuler();			//选中测距尺工具
	void OnSelectedContour();		//选中轮廓工具
	void OnSelectedBiDimensional();	//选中二维标尺工具
	void OnSelectedGrayLevel();		//选中灰阶工具
	void OnSelectedZoom();			//选中缩放工具
	void OnSelectedMove();			//选中移动工具
	void OnNegative();				//使用负片效果
	void OnReset();					//复位
	void OnPlay();					//播放/暂停
	void OnStop();					//停止
	void OnSwitchProperty();		//属性docking窗口的开关
	void on_treeView_customContextMenuRequested(QPoint pos);//树视图上下文菜单分发
	void OnShowDicomCurrentTags();	//显示当前病人的所有信息
	void OnShowSelectedSeries();	//显示选中的Series
	void OnShowSelectedImage();		//显示当前选中的Image
	void OnSliceScrollBarValueChange(int a);//Slice滚动条值更改事件
	void receiveData(QString data,QString dir);//响应DicomDir类传送过来的信号,其中包含了一个病人的ID
	void OnLatterPatient();//上一个病人
	void OnPreviousPatient();//下一个病人

	void OnWindowWL_Defaut();			//默认窗宽窗位
	void OnWindowWL_All();				//全部动态范围
	void OnWindowWL_CT_Abdomen();		//腹部
	void OnWindowWL_CT_BloodVessel();	//血管
	void OnWindowWL_CT_Bones();			//骨骼
	void OnWindowWL_CT_Brain();			//脑
	void OnWindowWL_CT_Medias();		//纵膈
	void OnWindowWL_CT_Lungs();		    //肺
	///测试入口
	void OnTestEntrance_01();//测试入口1
	void OnTestEntrance_02();//测试入口2
    //bao 11.27日加
	void Slots_Seg(int count);
	void Slots_Reg(int count);
	void Slots_PickPixel(int count,QVTKWidget *qvtk);
	void Slots_Volume();
	void Slots_Volume_gpu();
};
