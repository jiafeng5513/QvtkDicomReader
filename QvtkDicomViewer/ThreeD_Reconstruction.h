#pragma once

#include <QWidget>
#include "ui_ThreeD_Reconstruction.h"
#include <vtkStringArray.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include "vtkMyDICOMImageReader.h"
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkImageCast.h>

/*
 * 3维重建
 * 这里是负责显示三维重建结果的地方
 * 为了避免一些不必要的麻烦,使交互模式更符合常识
 * 三维重建的结果不在主界面的画布上显示
 * 这样可以针对三维操作设计一些交互方式
 *
 * 初始化:我认为这个类应该是一个允许复数实例的类
 * 传入一个series的全部文件名,规定存储形式为vector<std::string>
 * 首先进行排序
 * 其次送入渲染管线进行渲染
 * 管线有两套;GPU 和 CPU
 * 
 * 注意:这个类的合理性基于这样一种假设:
 *		"从DicomDir中得到的文件的是按照扫描顺序排列的"
 *		假设必须存在的原因是在这里对文件按照series id 排列是不合适的,这样会形成大量的冗余代码且增加耦合性
 *		
 *		如果未来有这样一种需求:
 *		"从不含有DicomDir File的,含有分散DCM文件的文件夹中的文件中进行三维重建"
 *		那么我们的假设就没有意义了,那个时候必须从在本类初始化之前,在外面想办法对他们排序
 *		
 *		总之本类选择无条件相信来的文件名是排序好的
 *		
 * 额外发现:
 *     ReShaper的重构在代码未通过编译的时候并不能保证不出问题
 */
class ThreeD_Reconstruction : public QWidget
{
	Q_OBJECT

public:
	ThreeD_Reconstruction(QWidget *parent = Q_NULLPTR);//默认构造
	ThreeD_Reconstruction(std::vector<std::string>*filenames_v,QWidget *parent = Q_NULLPTR);//带数据构造
	~ThreeD_Reconstruction();

	/*
	 * 渲染管线不反回,所以我要先显示窗口再调用渲染
	 */
	void OnReconstruction();
private:
	vtkRenderer * m_ThreeDRec_Renderer;
	vtkRenderWindow *m_ThreeDRec_RenderWindow;
	vtkImageCast *m_ImageCast;
	vtkPiecewiseFunction *m_PiecewiseFunction;
	vtkColorTransferFunction *m_colorTransferFunction;
	vtkVolumeProperty *m_volumeProperty;
	vtkVolumeRayCastCompositeFunction *m_VolumeRayCastCompositeFunction;
	vtkFixedPointVolumeRayCastMapper *m_FixedPointVolumeRayCastMapper;
	vtkVolume *volume1;
	vtkSmartPointer<vtkMyDICOMImageReader> m_MyDICOMImageReader;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> TrackballCamera;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkGPUVolumeRayCastMapper *m_volumeMapper_gpu;

	vtkSmartPointer<vtkStringArray> m_Filenames;
private:
	Ui::ThreeD_Reconstruction ui;
	void RenderPipeline_CPU(vtkStringArray*);//CPU渲染管线
	void RenderPipeline_GPU(vtkStringArray*);//GPU渲染管线
};
