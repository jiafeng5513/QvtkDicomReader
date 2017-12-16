#include "Segmenter.h"
#include <vtkDICOMImageReader.h>

/*
 * 默认构造
 */
Segmenter::Segmenter(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}
/*
 * 带参数构造
 */
Segmenter::Segmenter(std::string Filename, QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);
	m_Filename = Filename;

}
/*
 * 析构,处理掉所有的非智能指针对象
 */
Segmenter::~Segmenter()
{
}
/*
 *联通阈值方法
 */
void Segmenter::OnConnectedThreshold()
{
}

/*
 *大津算法,最大类间方差法
 */
void Segmenter::OnOtsuThreshold()
{
}
/*
 *邻域连接法
 */
void Segmenter::OnNeighborhoodConnected()
{
}
/*
 * 置信连接法
 */
void Segmenter::OnConfidenceConnected()
{
}
/*
 * 快速匹配
 */
void Segmenter::OnFastMarching()
{
}
/*
 * 形状检测
 */
void Segmenter::OnShapeDetection()
{
}
/*
 * 响应分割方法的变化/首次设置
 * 
 * 我觉得这里面可以拆开成两部分,一部分一次性的初始化代码放在这个类的构造上,另一部分每次换算法都用执行的放这里
 */
void Segmenter::OnSegmentFuncChange()
{
	// Read the image
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	if (m_Filename=="")
	{
		//提示一下:但是这个问题实际上是不可能出现的
		return;
	}
	reader->SetFileName(m_Filename.c_str());
	reader->Update();

	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	//imageViewer->SetGlobalWarningDisplay(0);

	// Picker 
	vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
	propPicker->PickFromListOn();
	propPicker->AddPickList(imageViewer->GetImageActor());// Give the picker a prop to pick

	// 显示
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	imageViewer->SetupInteractor(renderWindowInteractor);
	imageViewer->SetSize(600, 600);

	vtkRenderer* renderer = imageViewer->GetRenderer();
	renderer->ResetCamera();
	renderer->GradientBackgroundOn();
	renderer->SetBackground(0, 0, 0);//背景颜色
	//renderer->SetBackground2(1, 1, 1);

	// Annotate the image with window/level and mouse over pixel information
	vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
	cornerAnnotation->SetLinearFontScaleFactor(2);
	cornerAnnotation->SetNonlinearFontScaleFactor(1);
	cornerAnnotation->SetMaximumFontSize(20);
	cornerAnnotation->SetText(0, "Off Image");
	cornerAnnotation->SetText(3, "<window>\n<level>");
	cornerAnnotation->GetTextProperty()->SetColor(1, 0, 0);

	imageViewer->GetRenderer()->AddViewProp(cornerAnnotation);

	// Callback listens to MouseMoveEvents invoked by the interactor's style
	vtkSmartPointer<vtkImageSegmentCallback> callback = vtkSmartPointer<vtkImageSegmentCallback>::New();//交互回调定义在上面
	callback->SetViewer(imageViewer);
	callback->SetAnnotation(cornerAnnotation);
	callback->SetPicker(propPicker);
	callback->SetQvtk(ui.qvtkWidget_Segment);
	callback->SetCount(m_segment_fun);//注意值的对应
	//callback->SetDir(dir);//这个函数还有用么
	// InteractorStyleImage allows for the following controls:
	// 1) middle mouse + move = camera pan
	// 2) left mouse + move = window/level
	// 3) right mouse + move = camera zoom
	// 4) middle mouse wheel scroll = zoom
	// 5) 'r' = reset window/level
	// 6) shift + 'r' = reset camera
	vtkInteractorStyleImage* imageStyle = imageViewer->GetInteractorStyle();
	ui.qvtkWidget_Segment->SetRenderWindow(imageViewer->GetRenderWindow());

	imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
	imageStyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);

	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
	//reader->Delete();

	//return EXIT_SUCCESS;
}
