#include "myVtkInteractorStyleImage.h"
#include "QvtkDicomViewer.h"

#include <vtkActor2D.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkAngleWidget.h>
#include <vtkImageChangeInformation.h>

#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <QMessageBox>
#include <qDebug>
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
{//jiafeng
	// Read all the DICOM files in the specified directory.
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	//reader->SetDataSpacing(50, 50, 1.5);
	reader->Update();
	//=================================
	//vtkSmartPointer<vtkImageChangeInformation> changer =vtkSmartPointer<vtkImageChangeInformation>::New();
	//changer->SetInputData(reader->GetOutputDataObject(4));
	////miao?ha?
	//changer->SetOutputOrigin(100, 100, 0);
	//changer->SetOutputSpacing(5, 5, 1);
	//changer->SetCenterImage(1);
	//changer->Update();
	//===================================
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
	temp.append("Data Spacing:"); 
	temp.append("("); temp.append(std::to_string(*reader->GetDataSpacing()));
	temp.append(","); temp.append(std::to_string(*(reader->GetDataSpacing() + 1)));
	temp.append(","); temp.append(std::to_string(*(reader->GetDataSpacing() + 2)));
	temp.append(")");temp.append("\n");
	const char* message = temp.c_str();
	//measurement
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
	static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetScale(1.42f);//?
	/*reader->GetPixelSpacing();
	 *
	m_pImageViewer->GetInteractorStyle();*/
	//reader->SetDataSpacing(3.2,3.2,1.5);
	//VTK世界坐标系的单位÷所需的单位=SetScale()
	// 实例化并设置量角器组件
	angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
	angleWidget->SetInteractor(renderWindowInteractor);
	angleWidget->CreateDefaultRepresentation();
	renderWindowInteractor->Start();
	// 启动渲染
	//m_pImageViewer->GetRenderer()->ResetCamera();
	//// m_pImageViewer->Render();
	//ui.qvtkWidget->GetRenderWindow()->Render();
	//renderWindowInteractor->Start();
	//qDebug() << reader->GetDirectoryName();
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
