#include "myVtkInteractorStyleImage.h"

#include <vtkActor2D.h>
#include "QvtkDicomViewer.h"
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>

QvtkDicomViewer::QvtkDicomViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_pImageViewer = vtkSmartPointer< vtkImageViewer2 >::New();
	m_pRenderder = vtkSmartPointer< vtkRenderer >::New();
	// 设置m_QVTKWidget的渲染器
	ui.qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderder);
}

void QvtkDicomViewer::OnOpenFile()
{
	//1.获取Dicom文件夹的绝对路径名
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("打开目录"), "F:/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty() == true)
		return;
	std::string folder = dir.toStdString();

	// Read all the DICOM files in the specified directory.
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();

	// Visualize
	m_pImageViewer->SetInputConnection(reader->GetOutputPort());

	// slice status message
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

	// usage hint message
	usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput("- Slice with mouse wheel\n  or Up/Down-Key\n- Zoom with pressed right\n  mouse button while dragging");
	usageTextMapper->SetTextProperty(usageTextProp);

	usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);

	// create an interactor with our own style (inherit from vtkInteractorStyleImage)
	// in order to catch mousewheel and key events
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	myInteractorStyle =vtkSmartPointer<myVtkInteractorStyleImage>::New();

	// make imageviewer2 and sliceTextMapper visible to our interactorstyle
	// to enable slice status message updates when scrolling through the slices
	myInteractorStyle->SetImageViewer(m_pImageViewer);
	myInteractorStyle->SetStatusMapper(sliceTextMapper);

	m_pImageViewer->SetupInteractor(renderWindowInteractor);
	m_pImageViewer->SetRenderWindow(ui.qvtkWidget->GetRenderWindow());
	// make the interactor use our own interactorstyle
	// cause SetupInteractor() is defining it's own default interatorstyle 
	// this must be called after SetupInteractor()
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
	// add slice status message and usage hint message to the renderer
	m_pImageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	m_pImageViewer->GetRenderer()->AddActor2D(usageTextActor);

	// initialize rendering and interaction
	//imageViewer->GetRenderWindow()->SetSize(400, 300);
	//imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);
	// m_pImageViewer->Render();
	m_pImageViewer->GetRenderer()->ResetCamera();
	// m_pImageViewer->Render();
	ui.qvtkWidget->GetRenderWindow()->Render();
	renderWindowInteractor->Start();
}

void QvtkDicomViewer::OnRenderText()
{
	//读取文本信息
}

void QvtkDicomViewer::OnForward()
{
	if (m_pImageViewer->GetSlice() < m_pImageViewer->GetSliceMax()) {
		//cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
		m_pImageViewer->SetSlice(m_pImageViewer->GetSlice()+1);
		//std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
		//_StatusMapper->SetInput(msg.c_str());
		QString str;
		str.sprintf("%d/%d", m_pImageViewer->GetSlice(), m_pImageViewer->GetSliceMax());
		ui.statusBar->showMessage(str);
		m_pImageViewer->Render();
	}
}

void QvtkDicomViewer::OnBackward()
{
	if (m_pImageViewer->GetSlice() > m_pImageViewer->GetSliceMin()) {
		//cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
		m_pImageViewer->SetSlice(m_pImageViewer->GetSlice() - 1);
		//std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
		//_StatusMapper->SetInput(msg.c_str());
		QString str;
		str.sprintf("%d/%d", m_pImageViewer->GetSlice(), m_pImageViewer->GetSliceMax());
		ui.statusBar->showMessage(str);
		m_pImageViewer->Render();
	}
}
