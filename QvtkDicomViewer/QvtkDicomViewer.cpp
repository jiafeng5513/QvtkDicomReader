#include "myVtkInteractorStyleImage.h"
#include "vtkBiDimensionalCallback.h"
#include "DicomDir.h"

#include "QvtkDicomViewer.h"
#include <QMessageBox>
#include <qDebug>
#include <QComboBox>
#include <QStandardItem>
#include <QTableView>

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
#include <vtkImageIterator.h>
#include <vtkStringArray.h>
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkVolumeRayCastMapper.h"

#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkPiecewiseFunction.h"
#include "itkNumericSeriesFileNames.h"

#include <dcmtk\config\osconfig.h>
#include <dcmtk\dcmdata\dctk.h>

#include "SlicePlayer.h"
#include "DicomDataBase.h"

void functest()
{
	qDebug()<< QStringLiteral("调用成功!");
}
/*
 * 构造方法
 */
QvtkDicomViewer::QvtkDicomViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.action_Pointer->setChecked(true);
	CursorType = CURSOR::POINTRE;
	//监控光标类型的修改
	connect(this, SIGNAL(CursorValueChanged()), this, SLOT(OnChangeCursorValue()));
	//创建并构造一个下拉列表
	QComboBox* _Combobox = new QComboBox();
	_Combobox->addItem(QStringLiteral("骨骼"));
	_Combobox->addItem(QStringLiteral("肌肉"));
	_Combobox->addItem(QStringLiteral("查克拉"));
	//添加到菜单栏
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addWidget(_Combobox);
	ui.mainToolBar->addSeparator();
	//自定义初始化
	ui.action_SwitchOfProperty->setChecked(true);
	ui.dockWidget_Msg->setHidden(false);
	icon_Play.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/play_128px_1197036_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	icon_Pause.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/pause_128px_1197034_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	PlayFlag = false;
	ui.action_Stop->setEnabled(false);


	//DirTreeRefresh(NULL);
}
/*
 * 响应光标值的修改,执行一些刷新和禁用操作
 */
void QvtkDicomViewer::OnChangeCursorValue()
{
	biDimensionalWidget->EnabledOff();
	contourWidget->EnabledOff();
	distanceWidget->EnabledOff();
	angleWidget->EnabledOff();

	ui.action_BiDimensional->setChecked(false);
	ui.action_Contour->setChecked(false);
	ui.action_Ruler->setChecked(false);
	ui.action_Protractor->setChecked(false);
	ui.action_Pointer->setChecked(false);

	ui.action_GrayLevel->setChecked(false);
	ui.action_Zoom->setChecked(false);
	ui.action_Move->setChecked(false);
	switch (CursorType)
	{
	case POINTRE://默认指针
		ui.action_Pointer->setChecked(true);
		break;
	case ZOOM://缩放
		ui.action_Zoom->setChecked(true);
		break;
	case GRAYLEVEL://灰阶
		ui.action_GrayLevel->setChecked(true);
		break;//量角器
	case PROTRACTOR:
		angleWidget->EnabledOn();
		angleWidget->SetWidgetStateToStart();
		ui.action_Protractor->setChecked(true);
		break;
	case RULER://测距尺
		distanceWidget->EnabledOn();
		distanceWidget->SetWidgetStateToStart();
		ui.action_Ruler->setChecked(true);
		break;
	case CONTOUR://轮廓
		contourWidget->EnabledOff();
		ui.action_Contour->setChecked(true);
		break;
	case BIDI://二维尺
		biDimensionalWidget->EnabledOn();
		biDimensionalWidget->SetWidgetStateToStart();
		ui.action_BiDimensional->setChecked(true);
		break;
	case MOVE:
		ui.action_Move->setChecked(true);
		break;
	default:
		break;
	}
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
	folder = dir.toStdString();
	DoRender(folder);
	//0.判断所选文件是DIR文件,DICOM文件夹,还是单幅DICOM图片
		//0.1对于DIR文件,使用DCMTK-64打开,加载目录信息,弹出文件列表,供用户选择DICOM文件夹
		//0.2对于DICOM文件夹,直接视为连续图片加载,并激活连续播放等功能
		//0.3对于单幅DICOM图片,直接显示,并激活除去连续播放的其他功能
	//1.

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
	usageTextProp->SetColor(1.0, 1.0, 0.0);
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
	widget->SetViewport(0.0, 0.0, 0.2, 0.2);
	widget->SetEnabled(1);
	widget->InteractiveOff();//禁用交互
	//widget->InteractiveOn();
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
 *修改当前光标类型
 */
void QvtkDicomViewer::setCursor(CURSOR newValue)
{
	CursorType = newValue;
	emit CursorValueChanged();//值更改,发出信号
}
/*
 * 显示给定路径中的Dicom数据
 */
void QvtkDicomViewer::DoRender(std::string folder)
{
	// reader输出化并绑定文件
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	//reader->SetFileNames();
	/*
	 *vtkDICOMImagereader.cxx这个文件是SetDirectoryName这个函数的实现点
	 *重写之后需要重新编译vtk8.0.0
	 */
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

	//================================================================================
	//图像变换新功能实验区域
	//================================================================================
	// 创建交互器实例
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	// 交互风格(InteractorStyle)是通过继承vtkInteractorStyleImage实现的
	// 其中重新实现了鼠标滚轮的交互事件
	myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
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

	// 给播放器进行初始化
	m_slice_player = new SlicePlayer(myInteractorStyle->getMinSlice(),
									 myInteractorStyle->getMaxSlice(),
									 myInteractorStyle->getSlice(),
									 50);
	connect(m_slice_player, SIGNAL(isTimeToTurnNextSlice()), this, SLOT(OnForward()));
	connect(m_slice_player, SIGNAL(isTimeToReset()), this, SLOT(OnResetToFirst()));
	// 启动渲染
#pragma region 启动渲染
	// m_pImageViewer->Render();//貌似不需要?
	m_pImageViewer->GetRenderer()->ResetCamera();
	ui.qvtkWidget->GetRenderWindow()->Render();
	renderWindowInteractor->Start();
#pragma endregion 
}
/*
 * 使用VTK单张图片读取方法加载一个series
 * 测试时:自动连续播放
 */
void QvtkDicomViewer::SeriesRender(std::string first)
{
	// reader输出化并绑定文件
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName(first.c_str());
	reader->Update();
	m_pRenderder = vtkSmartPointer< vtkRenderer >::New();
	// Renderer绑定输出窗口到Qvtkwidget
	ui.qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderder);
	//Viewer初始化并绑定reader
	m_pImageViewer = vtkSmartPointer< vtkImageViewer2 >::New();
	m_pImageViewer->SetInputConnection(reader->GetOutputPort());
	m_pImageViewer->SetRenderWindow(ui.qvtkWidget->GetRenderWindow());
	m_pImageViewer->GetRenderer()->ResetCamera();
	ui.qvtkWidget->GetRenderWindow()->Render();
}
/*
 * 刷新DIR树
 */
void QvtkDicomViewer::DirTreeRefresh(DicomDataBase * database)
{
	QStandardItem* pItem0 = new QStandardItem(QStringLiteral("Patient"));
	pItem0->setToolTip(QStringLiteral("病人姓名"));

	QStandardItem* pItem1 = new QStandardItem(QStringLiteral("StudyID"));
	pItem1->setToolTip(QStringLiteral("病历"));

	QStandardItem* pItem2 = new QStandardItem(QStringLiteral("Series"));
	pItem2->setToolTip(QStringLiteral("扫描序列"));

	QStandardItem* pItem3 = new QStandardItem(QStringLiteral("Image"));
	pItem3->setToolTip(QStringLiteral("Dicom图片"));
	QStandardItemModel *model = new QStandardItemModel();

	model->setHorizontalHeaderItem(0, pItem0);
	model->setHorizontalHeaderItem(1, pItem1);
	model->setHorizontalHeaderItem(2, pItem2);
	model->setHorizontalHeaderItem(3, pItem3);

	ui.treeView->setModel(model);



	//QMap<QString, QString> m_IconMap;//存放公共图标   
	//QMap<QString, QString> m_CarportIconMap;  //车库公共图标  

	//										//初始化图标库  
	//m_CarportIconMap[QStringLiteral("Port1")] = QStringLiteral("3942000.ico");
	//m_CarportIconMap[QStringLiteral("Port2")] = QStringLiteral("3942001.ico");
	//m_CarportIconMap[QStringLiteral("Port3")] = QStringLiteral("3942002.ico");
	//m_CarportIconMap[QStringLiteral("Port4")] = QStringLiteral("3942003.ico");
	//m_CarportIconMap[QStringLiteral("Port5")] = QStringLiteral("3942004.ico");

	//m_IconMap[QStringLiteral("Audi")] = QStringLiteral("audi.ico");
	//m_IconMap[QStringLiteral("Bmw")] = QStringLiteral("bmw.ico");
	//m_IconMap[QStringLiteral("Buick")] = QStringLiteral("Buick.ico");
	//m_IconMap[QStringLiteral("Cadillac")] = QStringLiteral("cadillac.ico");
	//m_IconMap[QStringLiteral("Ferrari")] = QStringLiteral("ferrari.ico");
	//m_IconMap[QStringLiteral("Ford")] =QStringLiteral("ford.ico");
	//m_IconMap[QStringLiteral("Hyundai")] = QStringLiteral("hyundai.ico");
	//m_IconMap[QStringLiteral("Lexus")] = QStringLiteral("lexus.ico");
	//m_IconMap[QStringLiteral("Mazda")] = QStringLiteral("mazda.ico");
	//m_IconMap[QStringLiteral("Mercedesbenz")] = QStringLiteral("mercedesbenz.ico");
	//m_IconMap[QStringLiteral("Nissan")] = QStringLiteral("nissan.ico");
	//m_IconMap[QStringLiteral("Toyota")] = QStringLiteral("toyota.ico");
	//m_IconMap[QStringLiteral("Volkswagen")] = QStringLiteral("Volkswagen.ico");

	//QStandardItem* pStandardItem = NULL;
	//QStandardItem* pStandardChildItem = NULL;

	QStandardItem* PatientItem = NULL;
	QStandardItem* StudyItem = NULL;
	QStandardItem* SeriesItem = NULL;
	QStandardItem* ImageItem = NULL;

	for(int i=0;i<database->PatientList.size();i++)
	{
		PatientItem= new QStandardItem(database->PatientList[i]->PatientName.c_str());
		model->appendRow(PatientItem);
		for(int j=0;j<database->PatientList[i]->StudyList.size();j++)
		{
			StudyItem = new QStandardItem(database->PatientList[i]->StudyList[j]->StudyId.c_str());
			PatientItem->appendRow(StudyItem);
			PatientItem->setChild(PatientItem->row(), 1, StudyItem);
			for(int k=0;k<database->PatientList[i]->StudyList[j]->SeriesList.size();k++)
			{
				SeriesItem = new QStandardItem(database->PatientList[i]->StudyList[j]->SeriesList[k]->SeriseNumber.c_str());
				StudyItem->appendRow(SeriesItem);
				StudyItem->setChild(StudyItem->row(), 2, SeriesItem);
				for (int l = 0; l<database->PatientList[i]->StudyList[j]->SeriesList[k]->ImageList.size(); l++)
				{
					ImageItem = new QStandardItem(database->PatientList[i]->StudyList[j]->SeriesList[k]->ImageList[l]->ReferencedFileID.c_str());
					SeriesItem->appendRow(ImageItem);
					SeriesItem->setChild(SeriesItem->row(), 3, ImageItem);
				}
			}
		}
	}

	//int i = 0;
	//QMap<QString, QString>::const_iterator it1 = m_CarportIconMap.constBegin();
	//for (QMap<QString, QString>::const_iterator it = m_IconMap.constBegin(); it != m_IconMap.constEnd(); ++it)
	//{
	//	if (i % 3 == 0)
	//	{
	//		pStandardItem = new QStandardItem((it1 + (i / 3)).value());
	//		model->appendRow(pStandardItem);
	//	}
	//	pStandardChildItem = new QStandardItem(it.value());
	//	pStandardItem->appendRow(pStandardChildItem);
	//	pStandardItem->setChild(pStandardChildItem->row(), 1, new QStandardItem(QString("this is %1").arg(it.key())));
	//	++i;
	//}

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
 * 回到第一张
 */
void QvtkDicomViewer::OnResetToFirst()
{
	myInteractorStyle->ResetSliceToMin();
}
/*
 * 选中默认鼠标指针工具
 */
void QvtkDicomViewer::OnSelectedPointer()
{
	setCursor(CURSOR::POINTRE);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
}
/*
 * 选中量角器工具
 */
void QvtkDicomViewer::OnSelectedProtractor()
{
	setCursor(CURSOR::PROTRACTOR);
}
/*
 * 选中测距尺工具
 */
void QvtkDicomViewer::OnSelectedRuler()
{
	setCursor(CURSOR::RULER);
}
/*
 * 选中轮廓工具
 */
void QvtkDicomViewer::OnSelectedContour()
{
	setCursor(CURSOR::CONTOUR);
}
/*
 * 选中二维标尺工具
 */
void QvtkDicomViewer::OnSelectedBiDimensional()
{
	setCursor(CURSOR::BIDI);
}
/*
 * 选中灰阶工具
 */
void QvtkDicomViewer::OnSelectedGrayLevel()
{
	setCursor(CURSOR::GRAYLEVEL);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::GRAYLEVEL;
}
/*
 * 选中缩放工具
 */
void QvtkDicomViewer::OnSelectedZoom()
{
	setCursor(CURSOR::ZOOM);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::ZOOM;
}
/*
 * 选中移动工具
 */
void QvtkDicomViewer::OnSelectedMove()
{
	setCursor(CURSOR::MOVE);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::MOVE;
}
/*
 * 使用负片效果
 */
void QvtkDicomViewer::OnNegative()
{
	int subRegion[6] = { 0, 511, 0, 511, 0, 61 };//64, 448, 64, 448, 0, 0
												
	vtkImageIterator<unsigned char> iter(reader->GetOutput(), subRegion);
												
	while (!iter.IsAtEnd())//注意这个迭代有自带的互斥效果					 
	{								 	
		unsigned char *inSI = iter.BeginSpan();									 	
		unsigned char *inSIEnd = iter.EndSpan();					 	
		while (inSI != inSIEnd)								 	
		{								 		
			*inSI = 255 - *inSI;								 		
			++inSI;									 	
		}									 	
		iter.NextSpan();								
	}
	reader->Update();//刷新
}
/*
 * 复位按钮
 */
void QvtkDicomViewer::OnReset()
{
	setCursor(CURSOR::POINTRE);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
	DoRender(folder);
}
/*
 * 播放
 */
void QvtkDicomViewer::OnPlay()
{
	//myInteractorStyle->MoveSliceBackward();
	//
	if (PlayFlag==true)//此时正在播放,按键之后应该进行暂停,并将图标切换为播放
	{
		ui.action_Play->setIcon(icon_Play);
		ui.action_Play->setText(QStringLiteral("播放"));
		m_slice_player->pause.lock();//锁定线程
		ui.action_Stop->setEnabled(false);
		

	}else//此时为停止状态,案件之后应该进行播放,并将图标切换为暂停
	{
		ui.action_Play->setIcon(icon_Pause);
		ui.action_Play->setText(QStringLiteral("暂停"));
		m_slice_player->setCurrentSlice(myInteractorStyle->getSlice());//先同步页码
		if (m_slice_player->isRunning()==true)//线程被加锁了
		{
			m_slice_player->pause.unlock();
		}else//线程没有启动(第一次运行)
		{
			m_slice_player->start();
		}
		ui.action_Stop->setEnabled(true);
	}
	PlayFlag = !PlayFlag;
}
/*
 * 停止
 */
void QvtkDicomViewer::OnStop()
{
	ui.action_Play->setIcon(icon_Play);
	ui.action_Play->setText(QStringLiteral("播放"));
	PlayFlag = false;
	m_slice_player->pause.lock();
	myInteractorStyle->ResetSliceToMin();
	ui.action_Stop->setEnabled(false);
}
/*
 * 属性docking窗口的开关
 */
 void QvtkDicomViewer::OnSwitchProperty()
 {
	 //ui.action_SwitchOfProperty->isChecked();
	 if (ui.action_SwitchOfProperty->isChecked()==true)
	 {
		 ui.dockWidget_Msg->setHidden(false);
	 }
	 else
	 {
		 ui.dockWidget_Msg->setHidden(true);
	 }
 }
 /*
  *	测试调用DCMTK-x64读取元数据
  */
 void QvtkDicomViewer::OnTestDCMTK_x64()
 {
	 DcmFileFormat fileformat;
	 OFCondition oc = fileformat.loadFile("E:/源码和示例程序/DICOM/S427870/S10/I10");
	 if (oc.good()) {
		 OFString patientName;
		 QString temp = "";
		 if (fileformat.getDataset()->findAndGetOFString(DCM_PatientName, patientName).good())
		 {
			 temp.append("Patient Name:");
			 temp.append(patientName.c_str());
			 temp.append("\n");
		 }
		 if (fileformat.getDataset()->findAndGetOFString(DCM_PatientID, patientName).good())
		 {
			 temp.append("Patient ID:");
			 temp.append(patientName.c_str());
			 temp.append("\n");
		 }
		 ///DCM_PatientBirthDate DCM_PatientSex
		 if (fileformat.getDataset()->findAndGetOFString(DCM_PatientBirthDate, patientName).good())
		 {
			 temp.append("Patient Birth Date:");
			 temp.append(patientName.c_str());
			 temp.append("\n");
		 }

		 if (fileformat.getDataset()->findAndGetOFString(DCM_PatientSex, patientName).good())
		 {
			 temp.append("Patient Sex:");
			 temp.append(patientName.c_str());
			 temp.append("\n");
		 }

		 QMessageBox::information(this, "Information", temp);
	 }
 }
 /*
  *	测试调用DCMTK-x64读取DIR文件
  */
 void QvtkDicomViewer::OnTestReadDICOMDIR()
 {
	/*===================该函数的全局变量区===================*/
	 QString path;
	 DcmDirectoryRecord * root, *root1;//文件的绝对路径
	/*=======================================================*/
	 //打开文件选择页面
	 path = QFileDialog::getOpenFileName(this, QStringLiteral("打开文件"), ".", QStringLiteral("全部类型(*.*)"));
	 if (path.isEmpty() == true)
		 return;
	 DicomDir *m_dicomdir = new DicomDir(path);
	 connect(m_dicomdir, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
	 m_dicomdir->show();
 }
 /*
  *	响应DicomDir类传送过来的信号,其中包含了一个病人的ID
  */
 void QvtkDicomViewer::receiveData(QString data)
 {
	/*
	 * 在此处,应该能使用PatientID从数据库中查询出来这个病人的
	 * 信息,
	 * 包括study,series,image
	 * 实际上此处只需要保存好这个ID,使用的时候现场查询即可
	 * 实现查询需要消耗很多空间保存这棵子树的信息
	 */

	/*
	 * 输出调试信息
	 */
#if _DEBUG
	QString temp;
	DicomDataBase * tempDaatabase = DicomDataBase::getInstance();
	Current_patientId = data.toStdString();
	for (int i=0;i<tempDaatabase->PatientList.size();i++)
	{
		if (tempDaatabase->PatientList[i]->PatientID == data.toStdString())
		{
			temp.append(QStringLiteral("PatientID:"));temp.append(data); temp.append("\n");
			temp.append(QStringLiteral("PatientName:")); temp.append(QString::fromStdString(tempDaatabase->PatientList[i]->PatientName)); temp.append("\n");
			temp.append(QStringLiteral("Study.SIZE:"));
			temp.append(QString::fromStdString(std::to_string(tempDaatabase->PatientList[i]->StudyList.size()))); temp.append("\n");
			temp.append(QStringLiteral("Series.SIZE:"));
			temp.append(QString::fromStdString(std::to_string(tempDaatabase->PatientList[i]->StudyList[0]->SeriesList.size()))); temp.append("\n");
			break;
		}
	}
	QMessageBox::information(this, QStringLiteral("Debug"), temp);
	DirTreeRefresh(tempDaatabase);
#endif
	//RenderTest();
	//itkRenderTest();

	DicomDataBase * temp_database = DicomDataBase::getInstance();
	std::vector<DicomImage*> temp_dicom_images_v;//要打开的series中的图片序列
												 //找到目标series
												 /*
												 * 目前没有选择series的功能,测试时找到选定的病人,默认加载他的第一个study的第一个series
												 */
	for (int i = 0; i<temp_database->PatientList.size(); i++)
	{
		if (temp_database->PatientList[i]->PatientID == Current_patientId)
		{
			temp_dicom_images_v = temp_database->PatientList[i]->StudyList[0]->SeriesList[0]->ImageList;
			break;
		}
	}
	/*
	 * 集合该series中的全部image文件路径
	 */
	std::vector<std::string> imageAbsFilePath;
	for (int i = 0; i<temp_dicom_images_v.size(); i++)
	{
		QString temp = QString::fromStdString("F:\\Dicom\\Test2\\" + temp_dicom_images_v[i]->ReferencedFileID);
		temp.replace(QChar('\\'), QChar('/'));
		imageAbsFilePath.push_back(temp.toStdString());
		qDebug() << temp << "  i:  " << i << "  " << temp_dicom_images_v[i]->InstanceNumber.c_str() << endl;
	}
	SeriesRender(imageAbsFilePath[0]);
	for (int i = 0; i<temp_dicom_images_v.size(); i++)
	{
		reader->SetFileName(imageAbsFilePath[i].c_str());
		reader->Update();
		ui.qvtkWidget->GetRenderWindow()->Render();
	}
 }