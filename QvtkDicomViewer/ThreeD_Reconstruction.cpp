#include "ThreeD_Reconstruction.h"
#include <vtkDecimatePro.h>
#include "vtkPiecewiseFunction.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkImageAppend.h>
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
#include <vtkDICOMImageReader.h>


/*
 * 默认构造
 */
ThreeD_Reconstruction::ThreeD_Reconstruction(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}
/*
 * 带数据构造方法,从这里启动渲染管线
 */
ThreeD_Reconstruction::ThreeD_Reconstruction(std::vector<std::string>* filenames_v, QWidget* parent)
{
	//
	ui.setupUi(this);
	m_Filenames = vtkSmartPointer<vtkStringArray>::New();
	for(int i=0;i<filenames_v->size();i++)
	{
		m_Filenames->InsertNextValue(filenames_v->at(i));
	}
	setWindowTitle(QStringLiteral("三维重建-正在解算..."));
}
/*
 * 析构,释放所有非智能指针引用
 */
ThreeD_Reconstruction::~ThreeD_Reconstruction()
{

}
/*
 * 启动渲染
 */
void ThreeD_Reconstruction::OnReconstruction()
{
	//RenderPipeline_CPU(m_Filenames);
	RenderPipeline_GPU(m_Filenames);
}
/*
 * 定义CPU渲染管线
 * 输入:vtkStringArray *_FilesYouWant 一个series的全部 按顺序排好的文件名 绝对文件名
 */
void ThreeD_Reconstruction::RenderPipeline_CPU(vtkStringArray *_FilesYouWant)
{
	m_ThreeDRec_Renderer = vtkRenderer::New();             //设置绘制者(绘制对象指针)
	m_ThreeDRec_RenderWindow = vtkRenderWindow::New();	   //设置绘制窗口
	m_ThreeDRec_RenderWindow->AddRenderer(m_ThreeDRec_Renderer);                       //将绘制者加入绘制窗口

	ui.qvtkWidget_3DRec->SetRenderWindow(m_ThreeDRec_RenderWindow);

	renderWindowInteractor = vtkRenderWindowInteractor::New();//设置绘制交互操作窗口的
	renderWindowInteractor->SetRenderWindow(m_ThreeDRec_RenderWindow);    //将绘制窗口添加到交互窗口

	vtkDecimatePro *deci = vtkDecimatePro::New(); //减少数据读取点，以牺牲数据量加速交互
	deci->SetTargetReduction(0.3);

	TrackballCamera = vtkInteractorStyleTrackballCamera::New();//交互摄像机      
	renderWindowInteractor->SetInteractorStyle(TrackballCamera);

	m_MyDICOMImageReader = vtkSmartPointer<vtkMyDICOMImageReader>::New();
	m_MyDICOMImageReader->SetFileNames(_FilesYouWant);

#if _DEBUG
	std::filebuf fb;
	fb.open("vtkdicomviewer.log", std::ios::out);//输出日志文件
	std::ostream out(&fb);
	m_MyDICOMImageReader->Print(out);
	fb.close();
#endif
	m_MyDICOMImageReader->SetFileDimensionality(3);                 //设置显示图像的维数											
	m_MyDICOMImageReader->SetDataScalarType(VTK_UNSIGNED_SHORT);
	m_MyDICOMImageReader->SetDataExtent(0, 255, 0, 255, 0, 62);     //图片属性图片像素256x256，最后两参数表示有124张图
	m_MyDICOMImageReader->SetDataSpacing(0.9, 0.9, 0.9);            //设置像素间间距
	
	m_ImageCast = vtkImageCast::New();								//数据类型转换
	m_ImageCast->SetInputConnection(m_MyDICOMImageReader->GetOutputPort());

	m_ImageCast->SetOutputScalarTypeToUnsignedShort();
	m_ImageCast->ClampOverflowOn();									 //阀值

														
	m_PiecewiseFunction = vtkPiecewiseFunction::New();				 //一维分段函数变换
	m_PiecewiseFunction->AddPoint(20, 0.0);
	m_PiecewiseFunction->AddPoint(255, 0.2);
	//设置颜色传递函数//该函数确定体绘像素的颜色值或者灰度值
	m_colorTransferFunction = vtkColorTransferFunction::New();
	m_colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.5, 0.0);        //添加色彩点（第一个参数索引）
	m_colorTransferFunction->AddRGBPoint(60.0, 1.0, 0.0, 0.0);
	m_colorTransferFunction->AddRGBPoint(128.0, 0.2, 0.1, 0.9);
	m_colorTransferFunction->AddRGBPoint(196.0, 0.27, 0.21, 0.1);
	m_colorTransferFunction->AddRGBPoint(255.0, 0.8, 0.8, 0.8);
	//vtkPiecewiseFunction *gradientTransferFunction = vtkPiecewiseFunction::New();//设置梯度传递函数
	//gradientTransferFunction->AddPoint(20, 0.0);
	//gradientTransferFunction->AddPoint(255, 2.0);
	//gradientTransferFunction->AddSegment (600, 0.73, 900, 0.9);
	//gradientTransferFunction->AddPoint(1300, 0.1); 
	m_volumeProperty = vtkVolumeProperty::New();					 //设定一个体绘容器的属性

	m_volumeProperty->SetColor(m_colorTransferFunction);             //设置颜色
	m_volumeProperty->SetScalarOpacity(m_PiecewiseFunction);		 //不透明度
																	 // m_volumeProperty->SetGradientOpacity(m_PiecewiseFunction);
	m_volumeProperty->ShadeOn();                                     //影阴
	m_volumeProperty->SetInterpolationTypeToLinear();                //直线与样条插值之间逐发函数
	m_volumeProperty->SetAmbient(0.2);                               //环境光系数
	m_volumeProperty->SetDiffuse(0.9);                               //漫反射
	m_volumeProperty->SetSpecular(0.2);                              //高光系数
	m_volumeProperty->SetSpecularPower(10);                          //高光强度 
																     //定义绘制者
																     //运行沿着光线合成
	// m_VolumeRayCastCompositeFunction = vtkVolumeRayCastCompositeFunction::New();                
	//vtkVolumeRayCastIsosurfaceFunction *m_VolumeRayCastCompositeFunction = vtkVolumeRayCastIsosurfaceFunction::New();

	m_FixedPointVolumeRayCastMapper = vtkFixedPointVolumeRayCastMapper::New();   //体绘制器
	//m_FixedPointVolumeRayCastMapper->SetVolumeRayCastFunction(m_VolumeRayCastCompositeFunction);//载入绘制方法
	m_FixedPointVolumeRayCastMapper->SetInputConnection(m_ImageCast->GetOutputPort());     //图像数据输入
	m_FixedPointVolumeRayCastMapper->SetNumberOfThreads(3);
	//定义Volume
	volume1 = vtkVolume::New();										//表示透示图中的一组三维数据
	volume1->SetMapper(m_FixedPointVolumeRayCastMapper);
	volume1->SetProperty(m_volumeProperty);						    //设置体属性
	// //保存
#if _DEBUG
	// vtkVolumeWriter *wSP=vtkVolumeWriter::New();
	//// vtkVolume *wSP=vtkVolume::New();
	// wSP->SetInputConnection(m_ImageCast->GetOutputPort());
	// wSP->SetFileName("F://ct/mmmm.vtk");
	// wSP->Write();
	// wSP->Delete();
#endif
	m_ThreeDRec_Renderer->AddVolume(volume1);						//将Volume装载到绘制类中
	// m_ThreeDRec_Renderer->SetBackground(1, 1, 1);
	//m_ThreeDRec_Renderer->SetBackground(0, 0, 0);
	//m_ThreeDRec_Renderer->SetBackground(255, 255, 255);
	//m_ThreeDRec_RenderWindow->SetSize(600, 600);					//设置背景颜色和绘制窗口大小
									//窗口进行绘制
	renderWindowInteractor->Initialize();
	this->setWindowTitle(QStringLiteral("三维重建-CPU解算完成"));
	renderWindowInteractor->Start();								//初始化并进行交互绘制
	m_ThreeDRec_Renderer->ResetCameraClippingRange();

	m_ThreeDRec_RenderWindow->Render();
}
/*
 * 定义GPU渲染管线
 * 输入:vtkStringArray *_FilesYouWant 一个series的全部 按顺序排好的文件名 绝对文件名
 */
void ThreeD_Reconstruction::RenderPipeline_GPU(vtkStringArray *_FilesYouWant)
{
	m_ThreeDRec_Renderer = vtkRenderer::New();						  //设置绘制者(绘制对象指针)
	m_ThreeDRec_RenderWindow = vtkRenderWindow::New();				  //设置绘制窗口
	m_ThreeDRec_RenderWindow->AddRenderer(m_ThreeDRec_Renderer);      //将绘制者加入绘制窗口
	ui.qvtkWidget_3DRec->SetRenderWindow(m_ThreeDRec_RenderWindow);
	renderWindowInteractor = vtkRenderWindowInteractor::New();		  //设置绘制交互操作窗口的
	renderWindowInteractor->SetRenderWindow(m_ThreeDRec_RenderWindow);//将绘制窗口添加到交互窗口

	vtkDecimatePro *deci = vtkDecimatePro::New();					  //减少数据读取点，以牺牲数据量加速交互
	deci->SetTargetReduction(0.3);

	TrackballCamera = vtkInteractorStyleTrackballCamera::New();       //交互模式:左键控制摄像机环绕       
	renderWindowInteractor->SetInteractorStyle(TrackballCamera);

	m_MyDICOMImageReader = vtkSmartPointer<vtkMyDICOMImageReader>::New();
	m_MyDICOMImageReader->SetFileNames(_FilesYouWant);
	m_MyDICOMImageReader->SetFileDimensionality(3);                   //设置显示图像的维数
	m_MyDICOMImageReader->SetDataScalarType(VTK_UNSIGNED_SHORT);
	m_MyDICOMImageReader->SetDataExtent(0, 255, 0, 255, 0, 123);      //图片属性图片像素256x256，最后两参数表示有124张图
	m_MyDICOMImageReader->SetDataSpacing(0.9, 0.9, 0.9);			  //设置像素间间距
	// reader->SetDataOrigin(0.0, 0.0, 0.0);						  //设置基准点，（一般没有用）做虚拟切片时可能会用的上
	m_ImageCast = vtkImageCast::New();								  //数据类型转换
	m_ImageCast->SetInputConnection(m_MyDICOMImageReader->GetOutputPort());
	m_ImageCast->SetOutputScalarTypeToUnsignedShort();
	m_ImageCast->ClampOverflowOn();									  //阀值

	//设置不透明度传递函数//该函数确定各体绘像素或单位长度值的不透明度
	m_PiecewiseFunction = vtkPiecewiseFunction::New();				  //一维分段函数变换
	m_PiecewiseFunction->AddPoint(20, 0.0);
	m_PiecewiseFunction->AddPoint(255, 0.2);
	//设置颜色传递函数//该函数确定体绘像素的颜色值或者灰度值
	m_colorTransferFunction = vtkColorTransferFunction::New();
	m_colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.5, 0.0);         //添加色彩点（第一个参数索引）
	m_colorTransferFunction->AddRGBPoint(60.0, 1.0, 0.0, 0.0);
	m_colorTransferFunction->AddRGBPoint(128.0, 0.2, 0.1, 0.9);
	m_colorTransferFunction->AddRGBPoint(196.0, 0.27, 0.21, 0.1);
	m_colorTransferFunction->AddRGBPoint(255.0, 0.8, 0.8, 0.8);
	//vtkPiecewiseFunction *gradientTransferFunction = vtkPiecewiseFunction::New();//设置梯度传递函数
	//gradientTransferFunction->AddPoint(20, 0.0);
	//gradientTransferFunction->AddPoint(255, 2.0);
	//gradientTransferFunction->AddSegment (600, 0.73, 900, 0.9);
	//gradientTransferFunction->AddPoint(1300, 0.1); 
	m_volumeProperty = vtkVolumeProperty::New();					 //设定一个体绘容器的属性

	m_volumeProperty->SetColor(m_colorTransferFunction);             //设置颜色
	m_volumeProperty->SetScalarOpacity(m_PiecewiseFunction);		 //不透明度
	// m_volumeProperty->SetGradientOpacity(m_PiecewiseFunction);
	m_volumeProperty->ShadeOn();                                     //影阴
	m_volumeProperty->SetInterpolationTypeToLinear();                //直线与样条插值之间逐发函数
	m_volumeProperty->SetAmbient(0.2);                               //环境光系数
	m_volumeProperty->SetDiffuse(0.9);                               //漫反射
	m_volumeProperty->SetSpecular(0.2);                              //高光系数
	m_volumeProperty->SetSpecularPower(10);                          //高光强度 
	//定义绘制者
	// m_VolumeRayCastCompositeFunction = vtkVolumeRayCastCompositeFunction::New();                   //运行沿着光线合成
	//vtkVolumeRayCastIsosurfaceFunction *m_VolumeRayCastCompositeFunction = vtkVolumeRayCastIsosurfaceFunction::New(); //运行沿着光线合成

	m_volumeMapper_gpu = vtkGPUVolumeRayCastMapper::New();   //体绘制器
															 //m_FixedPointVolumeRayCastMapper->SetVolumeRayCastFunction(m_VolumeRayCastCompositeFunction);              //载入绘制方法
	m_volumeMapper_gpu->SetInputConnection(m_ImageCast->GetOutputPort());     //图像数据输入
	//m_volumeMapper_gpu->SetNumberOfThreads(3);
	//定义Volume
	volume1 = vtkVolume::New();       //表示透示图中的一组三维数据
	volume1->SetMapper(m_volumeMapper_gpu);
	volume1->SetProperty(m_volumeProperty);							 //设置体属性
										 
#if _DEBUG
	//保存	
	// vtkVolumeWriter *wSP=vtkVolumeWriter::New();
	// //vtkVolume *wSP=vtkVolume::New();
	// wSP->SetInputConnection(m_ImageCast->GetOutputPort());
	// wSP->SetFileName("F://ct/mmmm.vtk");
	// wSP->Write();
	// wSP->Delete();
#endif
	m_ThreeDRec_Renderer->AddVolume(volume1);						//将Volume装载到绘制类中
	// ren->SetBackground(1, 1, 1);
	//ren->SetBackground(0, 0, 0);
	//ren->SetBackground(255, 255, 255);
	//m_ThreeDRec_RenderWindow->SetSize(600, 600);					//设置背景颜色和绘制窗口大小
	m_ThreeDRec_RenderWindow->Render();								//窗口进行绘制
	renderWindowInteractor->Initialize();
	this->setWindowTitle(QStringLiteral("三维重建-GPU解算完成"));
	renderWindowInteractor->Start();								//初始化并进行交互绘制
	m_ThreeDRec_Renderer->ResetCameraClippingRange();
}
