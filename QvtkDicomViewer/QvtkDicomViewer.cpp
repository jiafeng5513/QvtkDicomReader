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

#include <dcmtk\config\osconfig.h>
#include <dcmtk\dcmdata\dctk.h>

#include "SlicePlayer.h"
#include "DicomDataBase.h"
#include <QFileSystemModel>
#include "TreeItem.h"

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
	ui.dockWidget_Dir->setHidden(false);
	icon_Play.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/play_128px_1197036_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	icon_Pause.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/pause_128px_1197034_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	PlayFlag = false;
	ui.action_Stop->setEnabled(false);
	//上下文菜单
	CreateContextMenu();
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
 * 打开Series文件夹
 */
void QvtkDicomViewer::OnOpenSeriesFolder()
{
	//获取Series文件夹的绝对路径名
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("打开Series目录"), "F:/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty() == true)
		return;
	std::string folder = dir.toStdString();
	RenderInitializer(folder);
}
/*
 * 打开单张Dicom文件
 */
void QvtkDicomViewer::OnOpenDicomFile()
{

}
/*
 * 打开DICOMDIR文件
 */
void QvtkDicomViewer::OnOpenDicomDirFile()
{							  
	//打开文件选择页面
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("打开DICOMDIR文件"), ".", QStringLiteral("全部类型(*.*)"));
	if (path.isEmpty() == true)
		return;
	/*
	 * 先校验path是不是DICOMDIR文件的路径,如果不是,弹出警告并退出
	 * 如果是,执行下面的三条语句
	 */
	DicomDir *m_dicomdir = new DicomDir(path);
	connect(m_dicomdir, SIGNAL(sendData(QString,QString)), this, SLOT(receiveData(QString,QString)));
	m_dicomdir->show();
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
void QvtkDicomViewer::SetSliceText(int current,int max)
{
	m_pImageViewer->GetRenderer()->RemoveActor(sliceTextActor);
	vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg = StatusMessage::Format(current-1, max-1);//显示6/21
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	/*vtkSmartPointer<vtkActor2D>*/ sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);
	m_pImageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	
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
 * 创建树视图上下文菜单
 */
void QvtkDicomViewer::CreateContextMenu()
{

	QAction* action_New_Open_DICOMDIR_File = new QAction(QStringLiteral("&打开DICOMDIR文件"), ui.treeView);
	connect(action_New_Open_DICOMDIR_File, SIGNAL(triggered()), this, SLOT(OnOpenDicomDirFile()));

	QAction* action_New_Query_Patient_Msg = new QAction(QStringLiteral("&显示这个病人的全部信息"), ui.treeView);
	connect(action_New_Query_Patient_Msg, SIGNAL(triggered()), this, SLOT(FolderCreater()));

	QAction* action_New_Render_Series = new QAction(QStringLiteral("&显示这个Series"), ui.treeView);
	connect(action_New_Render_Series, SIGNAL(triggered()), this, SLOT(FolderCreater()));

	QAction* action_New_Render_Image = new QAction(QStringLiteral("&显示这个Image"), ui.treeView);
	connect(action_New_Render_Image, SIGNAL(triggered()), this, SLOT(FolderCreater()));
	
	//树右键菜单->空树
	TreeViewMenu_OnEmpty = new QMenu(ui.treeView);
	TreeViewMenu_OnEmpty->addAction(action_New_Open_DICOMDIR_File);
	//树右键菜单->Patient节点
	TreeViewMenu_OnPatient = new QMenu(ui.treeView);
	TreeViewMenu_OnPatient->addAction(action_New_Query_Patient_Msg);
	//树右键菜单->Series节点
	TreeViewMenu_OnSeries = new QMenu(ui.treeView);
	TreeViewMenu_OnSeries->addAction(action_New_Render_Series);
	//树右键菜单->Image节点
	TreeViewMenu_OnImage = new QMenu(ui.treeView);
	TreeViewMenu_OnImage->addAction(action_New_Render_Image);
}
/*
 * 显示当前series中的第Index张图,Index从0开始,与滚动条配合
 */
void QvtkDicomViewer::ShowImageByIndex(int Index)
{
	RenderRefresh(imageAbsFilePath[Index], Index + 1, imageAbsFilePath.size());
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
 * 渲染器初始化
 * 参数:一张图片的文件路径
 *	    将要显示的图片的数量,默认为1
 * 说明:对于渲染器,要么是渲染单张图,要么是渲染一个图像序列(多张图)
 */
void QvtkDicomViewer::RenderInitializer(std::string folder,int NumOfImage )
{
	/*
	 * reader初始化
	 */
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName(folder.c_str());
	/*
	 * reader参数调整
	 */
	vtkSmartPointer<vtkImageChangeInformation> changer =vtkSmartPointer<vtkImageChangeInformation>::New();
	changer->SetInputData(reader->GetOutput());
	changer->SetOutputOrigin(0, 0, 0);
	changer->SetOutputSpacing(10, 10, 1);
	changer->SetCenterImage(1);
	changer->Update();
	reader->Update();
	/*
	 * renderer初始化
	 */
	m_pRenderder = vtkSmartPointer< vtkRenderer >::New();
	ui.qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderder);
	/*
	 * Viewer初始化
	 */
	m_pImageViewer = vtkSmartPointer< vtkImageViewer2 >::New();
	m_pImageViewer->SetInputConnection(reader->GetOutputPort());
	/*
	 * 叠加文字
	 */
	SetSliceText(1, NumOfImage);// 切片页码信息

	SetUsageText();// 显示一些Dicom文件头信息
	m_pImageViewer->GetRenderer()->AddActor2D(usageTextActor);
	/*
	 * 交互器和交互风格初始化
	 */
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
	m_pImageViewer->SetupInteractor(renderWindowInteractor);
	m_pImageViewer->SetRenderWindow(ui.qvtkWidget->GetRenderWindow());
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
	/*
	 * 挂载一些VTK控件
	 */
	addDistanceWidget();// 实例化并设置测距组件
	addAngleWidget();	// 实例化并设置量角器组件
	addContourWidget();	// 实例化并设置轮廓组件
	addOrientationMarker();//添加坐标轴指示(没有开关)
	addBiDimensionalWidget();
	/*
	 * 自动连续播放功能初始化
	 */
	m_slice_player = new SlicePlayer(0,NumOfImage-1,ui.SliceScrollBar->sliderPosition(),50);
	connect(m_slice_player, SIGNAL(isTimeToTurnNextSlice()), this, SLOT(OnForward()));
	connect(m_slice_player, SIGNAL(isTimeToReset()), this, SLOT(OnResetToFirst()));
	/*
	 * 渲染
	 */
	m_pImageViewer->GetRenderer()->ResetCamera();
	ui.qvtkWidget->GetRenderWindow()->Render();
}
/*
 * 更新渲染/渲染下一帧
 * 调用这个函数之前必须先调用渲染器初始化函数,否则将会发生运行时错误
 */
void QvtkDicomViewer::RenderRefresh(std::string imagefilename,int currentPagenumber,int maxPageNumber)
{
	//切换文件
	reader->SetFileName(imagefilename.c_str());
	//切换页码信息
	SetSliceText(currentPagenumber, maxPageNumber);
	//切换其他信息

	//更新渲染
	reader->Update();
	ui.qvtkWidget->GetRenderWindow()->Render();
}
/*
 * 刷新DIR树
 */
void QvtkDicomViewer::DirTreeRefresh(DicomDataBase * database)
{
	QStandardItem* pItem0 = new QStandardItem(QStringLiteral("病历"));
	pItem0->setToolTip(QStringLiteral("病人姓名"));

	//QStandardItem* pItem1 = new QStandardItem(QStringLiteral("StudyID"));
	//pItem1->setToolTip(QStringLiteral("病历"));

	//QStandardItem* pItem2 = new QStandardItem(QStringLiteral("Series"));
	//pItem2->setToolTip(QStringLiteral("扫描序列"));

	//QStandardItem* pItem3 = new QStandardItem(QStringLiteral("Image"));
	//pItem3->setToolTip(QStringLiteral("Dicom图片"));
	QStandardItemModel *model = new QStandardItemModel();

	model->setHorizontalHeaderItem(0, pItem0);
	//model->setHorizontalHeaderItem(1, pItem1);
	//model->setHorizontalHeaderItem(2, pItem2);
	//model->setHorizontalHeaderItem(3, pItem3);

	ui.treeView->setModel(model);

	QStandardItem* PatientItem = NULL;
	QStandardItem* StudyItem = NULL;
	QStandardItem* SeriesItem = NULL;
	QStandardItem* ImageItem = NULL;
	//DicomTreeItem*test = NULL;

	//model->appendRow(test);
	for(int i=0;i<database->PatientList.size();i++)
	{
		PatientItem= new QStandardItem(("Patient:"+database->PatientList[i]->PatientName).c_str());
		model->appendRow(PatientItem);
		for(int j=0;j<database->PatientList[i]->StudyList.size();j++)
		{
			StudyItem = new QStandardItem(("Study:"+database->PatientList[i]->StudyList[j]->StudyId).c_str());
			PatientItem->appendRow(StudyItem);
			//PatientItem->setChild(PatientItem->row(), 1, StudyItem);
			for(int k=0;k<database->PatientList[i]->StudyList[j]->SeriesList.size();k++)
			{
				SeriesItem = new QStandardItem(("Series:"+database->PatientList[i]->StudyList[j]->SeriesList[k]->SeriseNumber).c_str());
				StudyItem->appendRow(SeriesItem);
				//StudyItem->setChild(StudyItem->row(), 2, SeriesItem);
				for (int l = 0; l<database->PatientList[i]->StudyList[j]->SeriesList[k]->ImageList.size(); l++)
				{
					ImageItem = new QStandardItem(("Image:"+database->PatientList[i]->StudyList[j]->SeriesList[k]->ImageList[l]->ReferencedFileID).c_str());
					SeriesItem->appendRow(ImageItem);
					//SeriesItem->setChild(SeriesItem->row(), 3, ImageItem);
				}
			}
		}
	}
}
/*
 * 工具条->前一张
 */
void QvtkDicomViewer::OnForward()
{
	ui.SliceScrollBar->setSliderPosition(ui.SliceScrollBar->sliderPosition() + 1);
}
/*
 * 工具条->后一张
 */
void QvtkDicomViewer::OnBackward()
{
	ui.SliceScrollBar->setSliderPosition(ui.SliceScrollBar->sliderPosition() - 1);
}

/*
 * 回到第一张
 */
void QvtkDicomViewer::OnResetToFirst()
{
	ui.SliceScrollBar->setSliderPosition(0);
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
	//setCursor(CURSOR::POINTRE);
	//myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
	//RenderInitializer(folder);
	/*
	 * 复位功能换一种写法
	 */
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
		//m_slice_player->setCurrentSlice(myInteractorStyle->getSlice());//先同步页码
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
	//myInteractorStyle->ResetSliceToMin();
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
		 ui.dockWidget_Dir->setHidden(false);
	 }
	 else
	 {
		 ui.dockWidget_Dir->setHidden(true);
	 }
 }
/*
 * 树视图上下文菜单分发
 */
 void QvtkDicomViewer::on_treeView_customContextMenuRequested(QPoint pos)
 {
	/*
	 *为了从根本上解决这个问题,必须从QAbstractModel派生一个Model类,
	 *如果需要,还要有对应的Item类
	 *这个类能直接从数据库对象实例化并带有至少两组字段,
	 *一组是用于显示输出的字段,另一组是用于内部查找索引的ID字段
	 */
	 if (PrePosition != pos) {//这次触发是正常触发
		 PrePosition = pos;
		 if (ui.treeView->model() == NULL) {
			 //此时文件树是空的
			 TreeViewMenu_OnEmpty->exec(QCursor::pos());//显示右键菜单
		 }
		 else//文件树非空的时候才能启动这个
		 {
			 QModelIndex indexSelect = ui.treeView->indexAt(pos);  //当前节点索引
			 QString IndexTxt = indexSelect.data().toString();       //当前节点数据
			 if (IndexTxt.contains("Patient")==true)//如果当前右击发生在病人上
			 {
				 TreeViewMenu_OnPatient->exec(QCursor::pos());
			 }else if(IndexTxt.contains("Series") == true)
			 {
				 TreeViewMenu_OnSeries->exec(QCursor::pos());
			 }else if(IndexTxt.contains("Study") == true)
			 {
				 
			 }else if(IndexTxt.contains("Image") == true)
			 {
				 TreeViewMenu_OnImage->exec(QCursor::pos());
			 }
			 //qDebug() << indexSelect.column();
			 //QMessageBox::warning(this, QStringLiteral("安娜学姐的问候"), _colum);
		 }
	 }
	 else
	 {
		 //这次触发是非正常触发
		 PrePosition.setX(-1);
		 PrePosition.setY(-1);
	 }
 }
/*
 * Slice滚动条值更改事件
 */
 void QvtkDicomViewer::OnSliceScrollBarValueChange(int a)
 {
	 ShowImageByIndex(a);
 }
 /*
  *	测试读取缩略图
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
	 connect(m_dicomdir, SIGNAL(sendData(QString,QString)), this, SLOT(receiveData(QString,QString)));
	 m_dicomdir->show();
 }
 /*
  *	响应DicomDir类传送过来的信号,其中包含了一个病人的ID
  */
 void QvtkDicomViewer::receiveData(QString data,QString dir)
 {
	DicomDataBase * tempDaatabase = DicomDataBase::getInstance();
	Current_patientId = data.toStdString();//当前的病人ID

	DirTreeRefresh(tempDaatabase);

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
			temp_dicom_images_v = temp_database->PatientList[i]->StudyList[0]->SeriesList[0]->ImageList;//注意这两个0
			break;
		}
	}	
	//获取dir文件的前缀
	QFile *dirfile = new QFile(dir);
	auto dirfile_info= QFileInfo(*dirfile);
	QString FolderPrefix =dirfile_info.absolutePath();
	dirfile->close();
	/*
	 * 集合该series中的全部image文件路径
	 */
	//std::vector<std::string> imageAbsFilePath;


	for (int i = 0; i<temp_dicom_images_v.size(); i++)
	{
		QString temp = QString::fromStdString(FolderPrefix.toStdString() +"\\" + temp_dicom_images_v[i]->ReferencedFileID);

		temp.replace(QChar('\\'), QChar('/'));
		imageAbsFilePath.push_back(temp.toStdString());
		qDebug() << temp << "  i:  " << i << "  " << temp_dicom_images_v[i]->InstanceNumber.c_str() << endl;
	}
	RenderInitializer(imageAbsFilePath[0], imageAbsFilePath.size());
	ui.SliceScrollBar->setRange(0, imageAbsFilePath.size()-1);
 }