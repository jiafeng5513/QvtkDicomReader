/*
 * 重构:2017年7月19日09:57:21
 * 编程:天使微积分
 * 日志:1.更新代码结构,提取若干函数,修复了页码显示的bug,完善注释
 *		2.在此基础上,添加病人信息显示的功能.
 *		3.添加测量小工具
 */
#include "myVtkInteractorStyleImage.h"

#include <vtkActor2D.h>
#include "QvtkDicomViewer.h"
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>

#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>

#include <vtkAngleWidget.h>
/*
 * 构造方法
 */
QvtkDicomViewer::QvtkDicomViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_pImageViewer = vtkSmartPointer< vtkImageViewer2 >::New();
	m_pRenderder = vtkSmartPointer< vtkRenderer >::New();
	// 设置m_QVTKWidget的渲染器
	ui.qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderder);
}
/*
 * 打开文件
 */
void QvtkDicomViewer::OnOpenFile()
{
	//获取Dicom文件夹的绝对路径名
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("打开目录"), "F:/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty() == true)
		return;
	std::string folder = dir.toStdString();
	DoRender(folder);
}
/*
 * 显示给定路径中的Dicom数据
 */
void QvtkDicomViewer::DoRender(std::string folder)
{
	// Read all the DICOM files in the specified directory.
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();
	// Visualize
	m_pImageViewer->SetInputConnection(reader->GetOutputPort());

	// 切片页码信息
	sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg = StatusMessage::Format(m_pImageViewer->GetSliceMin(), m_pImageViewer->GetSliceMax());
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);

	// DICOM文件头信息
#pragma region 获取DICOM文件头信息
	std::string temp = "";
	temp.append("PatientName:"); temp.append(reader->GetPatientName()); temp.append("\n");
	temp.append("DescriptiveName:"); temp.append(reader->GetDescriptiveName());	temp.append("\n");
	temp.append("BitsAllocated:"); temp.append(std::to_string(reader->GetBitsAllocated())); temp.append("\n");
	temp.append("GantryAngle:"); temp.append(std::to_string(reader->GetGantryAngle())); temp.append("\n");
	temp.append("StudyID:"); temp.append(reader->GetStudyID()); temp.append("\n");
	temp.append("StudyUID:"); temp.append(reader->GetStudyUID()); temp.append("\n");
	temp.append("DataByteOrder:"); temp.append(reader->GetDataByteOrderAsString()); temp.append("\n");
	temp.append("Width:"); temp.append(std::to_string(reader->GetWidth())); temp.append("\n");
	temp.append("Height:"); temp.append(std::to_string(reader->GetHeight())); temp.append("\n");
	const char* message = temp.c_str();
#pragma endregion 
	usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput(message);//////////////////////////////////////////////////////
	usageTextMapper->SetTextProperty(usageTextProp);

	usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);//坐标

	// 创建交互器实例
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	// 交互风格(InteractorStyle)是通过继承vtkInteractorStyleImage实现的
	// 其中重新实现了鼠标滚轮的交互事件
	myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
	//向自定义的交互风格中传递必要的参数,以便于从鼠标滚轮事件中实现切片页码的更新
	myInteractorStyle->SetImageViewer(m_pImageViewer);
	myInteractorStyle->SetStatusMapper(sliceTextMapper);

	//为ImageViewer设置交互器和输出界面,这里的输出界面是QvtkWidget
	m_pImageViewer->SetupInteractor(renderWindowInteractor);
	m_pImageViewer->SetRenderWindow(ui.qvtkWidget->GetRenderWindow());

	// 把交互器中的交互风格改成自定义的交互风格
	// 由于m_pImageViewer->SetupInteractor(renderWindowInteractor)方法调用会自动的
	// 为交互器绑定默认交互风格,所以必须要在那之后再绑定自定义的交互风格
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);

	// 把准备好的文字添加到ImageViewer上
	m_pImageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	m_pImageViewer->GetRenderer()->AddActor2D(usageTextActor);

	// 设置窗口的大小和背景色
	//imageViewer->GetRenderWindow()->SetSize(400, 300);
	//imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);
	// m_pImageViewer->Render();

	// 实例化并设置测距组件
	distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
	distanceWidget->SetInteractor(renderWindowInteractor);
	distanceWidget->CreateDefaultRepresentation();
	static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g mm");

	// 实例化并设置量角器组件
	angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
	angleWidget->SetInteractor(renderWindowInteractor);
	angleWidget->CreateDefaultRepresentation();
	renderWindowInteractor->Start();

	// 启动渲染
	m_pImageViewer->GetRenderer()->ResetCamera();
	// m_pImageViewer->Render();
	ui.qvtkWidget->GetRenderWindow()->Render();
	renderWindowInteractor->Start();
}
/*
 * 工具条->文字(空方法)
 */
void QvtkDicomViewer::OnRenderText()
{
	//读取文本信息
}
/*
 * 工具条->前一张
 */
void QvtkDicomViewer::OnForward()
{
	myInteractorStyle->MoveSliceForward();
}
/*
 * 工具条->后一张
 */
void QvtkDicomViewer::OnBackward()
{
	myInteractorStyle->MoveSliceBackward();
}
/*
 * 选中默认鼠标指针工具
 */
void QvtkDicomViewer::OnSelectedPointer()
{
	angleWidget->EnabledOff();
	distanceWidget->EnabledOff();
	ui.action_Pointer->setChecked(true);
	ui.action_Protractor->setChecked(false);
	ui.action_Ruler->setChecked(false);
}
/*
 * 选中量角器工具
 */
void QvtkDicomViewer::OnSelectedProtractor()
{
	angleWidget->EnabledOn();
	distanceWidget->EnabledOff();
	ui.action_Protractor->setChecked(true);
	ui.action_Pointer->setChecked(false);
	ui.action_Ruler->setChecked(false);
}
/*
 * 选中测距尺工具
 */
void QvtkDicomViewer::OnSelectedRuler()
{
	distanceWidget->EnabledOn();
	angleWidget->EnabledOff();
	ui.action_Ruler->setChecked(true);
	ui.action_Protractor->setChecked(false);
	ui.action_Pointer->setChecked(false);
}
