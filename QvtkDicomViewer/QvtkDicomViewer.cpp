#include "myVtkInteractorStyleImage.h"
#include "vtkBiDimensionalCallback.h"

#include "QvtkDicomViewer.h"
#include <QMessageBox>
#include <qDebug>

#include <vtkActor2D.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkAngleWidget.h>
#include <vtkImageChangeInformation.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include "vtkImageData.h"
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkMath.h>
#include <vtkCellArray.h>

#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

/*
 * 构造方法
 */
QvtkDicomViewer::QvtkDicomViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.action_Pointer->setChecked(true);
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
 * 添加测距尺
 */
void QvtkDicomViewer::addDistanceWidget()
{
	distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
	distanceWidget->SetInteractor(renderWindowInteractor);
	distanceWidget->CreateDefaultRepresentation();
	//reader->SetDataSpacing(50, 50, 1.5);
	//static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetScale(1.42f);//?
	//VTK世界坐标系的单位÷所需的单位=SetScale()
	static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g mm");
}
/*
 * 添加量角器
 */
void QvtkDicomViewer::addAngleWidget()
{
	angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
	angleWidget->SetInteractor(renderWindowInteractor);
	angleWidget->CreateDefaultRepresentation();
}
/*
 * 添加轮廓工具
 */
void QvtkDicomViewer::addContourWidget()
{
	contourWidget =vtkSmartPointer<vtkContourWidget>::New();

	///调整线的颜色
	//contourRepresentation = vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
	//contourRepresentation->GetLinesProperty()->SetColor(1, 0, 0); // Set color to red
	//contourWidget->SetRepresentation(contourRepresentation);
	// Generate a set of points arranged in a circle
	int numPts = 10;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	for (int i = 0; i < numPts; i++)
	{
		// Create numPts points evenly spread around a circumference of radius 0.1
		const double angle = 2.0*vtkMath::Pi()*i / numPts;
		points->InsertPoint(static_cast<vtkIdType>(i), 10*cos(angle), 10*sin(angle), 0.0);
	}

	// Create a cell array to connect the points into meaningful geometry
	vtkIdType* vertexIndices = new vtkIdType[numPts + 1];
	for (int i = 0; i < numPts; i++) { vertexIndices[i] = static_cast<vtkIdType>(i); }
	// Set the last vertex to 0; this means the last line segment will join the 19th point (vertices[19])
	// with the first one (vertices[0]), thus closing the circle.
	vertexIndices[numPts] = 0;
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(numPts + 1, vertexIndices);

	// Create polydata to hold the geometry just created, and populate it
	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(points);
	polydata->SetLines(lines);

	contourWidget->SetInteractor(renderWindowInteractor);

}
/*
 * 添加切片页码
 */
void QvtkDicomViewer::SetSliceText()
{
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
}
/*
 * 添加Dicom文件头信息
 */
void QvtkDicomViewer::SetUsageText()
{
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
	temp.append("Data Spacing:"); 
	temp.append("("); temp.append(std::to_string(*reader->GetDataSpacing()));
	temp.append(","); temp.append(std::to_string(*(reader->GetDataSpacing() + 1)));
	temp.append(","); temp.append(std::to_string(*(reader->GetDataSpacing() + 2)));
	temp.append(")");temp.append("\n");
	const char* message = temp.c_str();
	//measurement

	// DICOM文件头信息
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
}
/*
 * 添加坐标轴指示
 */
void QvtkDicomViewer::addOrientationMarker()
{
	axes =vtkSmartPointer<vtkAxesActor>::New();
	widget =vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	widget->SetOrientationMarker(axes);
	widget->SetInteractor(renderWindowInteractor);
	widget->SetViewport(0.0, 0.0, 0.4, 0.4);
	widget->SetEnabled(1);
	widget->InteractiveOn();
}
/*
 * 添加二维尺度标尺
 */
void QvtkDicomViewer::addBiDimensionalWidget()
{
	biDimensionalWidget =vtkSmartPointer<vtkBiDimensionalWidget>::New();
	biDimensionalWidget->SetInteractor(renderWindowInteractor);
	biDimensionalWidget->CreateDefaultRepresentation();
	biDimensionalCallback =vtkSmartPointer<vtkBiDimensionalCallback>::New();
	biDimensionalWidget->AddObserver(vtkCommand::InteractionEvent, biDimensionalCallback);
}
/*
 * 显示给定路径中的Dicom数据
 */
void QvtkDicomViewer::DoRender(std::string folder)
{
	// reader输出化并绑定文件
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	//尝试修改一些参数
	vtkSmartPointer<vtkImageChangeInformation> changer =vtkSmartPointer<vtkImageChangeInformation>::New();
	changer->SetInputData(reader->GetOutput());
	changer->SetOutputOrigin(0, 0, 0);
	changer->SetOutputSpacing(10, 10, 1);
	changer->SetCenterImage(1);
	changer->Update();

	reader->Update();//这个究竟应该放在那里合适呢?
	// Renderer初始化
	m_pRenderder = vtkSmartPointer< vtkRenderer >::New();
	// Renderer绑定输出窗口到Qvtkwidget
	ui.qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderder);
	//Viewer初始化并绑定reader
	m_pImageViewer = vtkSmartPointer< vtkImageViewer2 >::New();
	m_pImageViewer->SetInputConnection(reader->GetOutputPort());
	
	SetSliceText();// 切片页码信息
	SetUsageText();// 显示一些Dicom文件头信息

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

	//挂载一些窗口组件
	addDistanceWidget();// 实例化并设置测距组件
	addAngleWidget();	// 实例化并设置量角器组件
	addContourWidget();	// 实例化并设置轮廓组件
	addOrientationMarker();//添加坐标轴指示(没有开关)
	addBiDimensionalWidget();
	// 启动渲染
	// m_pImageViewer->Render();//貌似不需要?
	m_pImageViewer->GetRenderer()->ResetCamera();
	ui.qvtkWidget->GetRenderWindow()->Render();
	renderWindowInteractor->Start();
}
/*
 *使用ITK获取元数据,并显示在Docking界面上
 */
void QvtkDicomViewer::GetMetaDataAndRender(std::string folder)
{
	typedef signed short InputPixelType;//dicom 对应数据类型

	const unsigned int   InputDimension = 2;
	typedef itk::Image< InputPixelType, InputDimension > InputImageType;
	typedef itk::ImageSeriesReader< InputImageType > ReaderType;
	typedef itk::GDCMImageIO ImageIOType;//GDCMImageIO读DICOM

	ReaderType::Pointer _reader = ReaderType::New();
	_reader->SetFileName(folder);

	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
	//关联GDCMImageIO类后，DICOM数据信息就读入内存，ITK能获取更加全面的信息（比起VTK）
	_reader->SetImageIO(gdcmImageIO);
	_reader->Update();
	_reader->GetMetaDataDictionary();//获取DIOCM头文件中信息
	gdcmImageIO->GetMetaDataDictionary();//获取DIOCM头文件中信息

										 //举例获取头文件中部分信息
	char* name = new char[50];			//病人姓名
	char* patientID = new char[50];		//病人ID
	char* time = new char[50];			//时间
	char* manufacture = new char[50];	//制造商
	char* modility = new char[50];		//检测手段
	char* hospital = new char[50];		//医院
	char* sex = new char[50];			//性别
	char* age = new char[50];			//年龄

	unsigned int dim = 0;				//尺寸
	int ori = 0;						//朝向
	int spa = 0;						//空间
										//还能获取很多文件头信息

	int pixelType = gdcmImageIO->GetPixelType();
	int componetType = gdcmImageIO->GetComponentType();
	int fileType = gdcmImageIO->GetFileType();
	int componetSize = gdcmImageIO->GetComponentSize();
	int dimension = gdcmImageIO->GetNumberOfDimensions();

	ImageIOType::ByteOrder byteOrder = gdcmImageIO->GetByteOrder();
	ImageIOType::SizeType imgSize = gdcmImageIO->GetImageSizeInPixels();

	gdcmImageIO->GetPatientName(name);
	gdcmImageIO->GetPatientID(patientID);
	gdcmImageIO->GetStudyDate(time);
	gdcmImageIO->GetManufacturer(manufacture);
	gdcmImageIO->GetModality(modility);
	gdcmImageIO->GetInstitution(hospital);
	gdcmImageIO->GetPatientSex(sex);
	gdcmImageIO->GetPatientAge(age);
	QString temp = "(";
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetDimensions(0)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetDimensions(1))));temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetDimensions(2))));temp.append(")");
	ui.lineEdit_Dimensions->setText(temp);
	temp = "(";
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetOrigin(0)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetOrigin(1)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetOrigin(2)))); temp.append(")");
	ui.lineEdit_Origins->setText(temp);
	temp = "(";
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetSpacing(0)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetSpacing(1)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetSpacing(2)))); temp.append(")");
	ui.lineEdit_Spacing->setText(temp);
	
	ui.lineEdit_Name->setText(name);
	ui.lineEdit_ID->setText(patientID);
	ui.lineEdit_Time->setText(time);
	ui.lineEdit_Manufacturer->setText(manufacture);
	ui.lineEdit_Modality->setText(modility);
	ui.lineEdit_Hospital->setText(hospital);
	ui.lineEdit_Sex->setText(sex);
	ui.lineEdit_Age->setText(age);
	
	//ui.lineEdit_Spacing->setText(std::to_string(sp);
	/*return temp;*/
	//ui.lineEdit_Age->setText(QStringLiteral("大哥"));
}
/*
 * 工具条->显示文件头信息
 */
void QvtkDicomViewer::OnRenderText()
{
	//读取文本信息
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("打开文件"), ".", QStringLiteral("全部类型(*.*)"));
	if (path.isEmpty() == true)
		return;
	std::string folder = path.toStdString();
	GetMetaDataAndRender(folder);
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
	contourWidget->EnabledOff();
	biDimensionalWidget->EnabledOff();
	ui.action_Pointer->setChecked(true);
	ui.action_Protractor->setChecked(false);
	ui.action_Ruler->setChecked(false);
	ui.action_Contour->setChecked(false);
	ui.action_BiDimensional->setChecked(false);
}
/*
 * 选中量角器工具
 */
void QvtkDicomViewer::OnSelectedProtractor()
{
	angleWidget->EnabledOn();
	angleWidget->SetWidgetStateToStart();
	distanceWidget->EnabledOff();
	contourWidget->EnabledOff();
	biDimensionalWidget->EnabledOff();
	ui.action_Protractor->setChecked(true);
	ui.action_Pointer->setChecked(false);
	ui.action_Ruler->setChecked(false);
	ui.action_Contour->setChecked(false);
	ui.action_BiDimensional->setChecked(false);
}
/*
 * 选中测距尺工具
 */
void QvtkDicomViewer::OnSelectedRuler()
{
	distanceWidget->EnabledOn();
	distanceWidget->SetWidgetStateToStart();
	angleWidget->EnabledOff();
	contourWidget->EnabledOff();
	biDimensionalWidget->EnabledOff();
	ui.action_Ruler->setChecked(true);
	ui.action_Protractor->setChecked(false);
	ui.action_Pointer->setChecked(false);
	ui.action_Contour->setChecked(false);
	ui.action_BiDimensional->setChecked(false);
}
/*
 * 选中轮廓工具
 */
void QvtkDicomViewer::OnSelectedContour()
{
	contourWidget->EnabledOn();
	distanceWidget->EnabledOff();
	angleWidget->EnabledOff();
	biDimensionalWidget->EnabledOff();
	//contourWidget->Initialize(polydata);
	ui.action_Contour->setChecked(true);
	ui.action_Ruler->setChecked(false);
	ui.action_Protractor->setChecked(false);
	ui.action_Pointer->setChecked(false);
	ui.action_BiDimensional->setChecked(false);
}
/*
 * 选中二维标尺工具
 */
void QvtkDicomViewer::OnSelectedBiDimensional()
{
	biDimensionalWidget->EnabledOn();
	biDimensionalWidget->SetWidgetStateToStart();
	contourWidget->EnabledOff();
	distanceWidget->EnabledOff();
	angleWidget->EnabledOff();
	ui.action_BiDimensional->setChecked(true);
	ui.action_Contour->setChecked(false);
	ui.action_Ruler->setChecked(false);
	ui.action_Protractor->setChecked(false);
	ui.action_Pointer->setChecked(false);

}
