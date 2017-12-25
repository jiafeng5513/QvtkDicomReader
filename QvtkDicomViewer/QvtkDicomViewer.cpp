#include "myVtkInteractorStyleImage.h"
#include "vtkBiDimensionalCallback.h"
#include "DicomDir.h"
#include "Segmenter.h"
#include <QListView>
#include "QvtkDicomViewer.h"
#include <QMessageBox>
#include <qDebug>
#include <QComboBox>
#include <QStandardItem>
#include <QTableView>
#include <QAction>
#include <QMenu>

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
#include <vtkColorTransferFunction.h>
#include "vtkVolumeRayCastMapper.h"
#include <vtkPiecewiseFunction.h>
#include "vtkStringArray.h"
#include "vtkUnicodeStringArray.h"
#include "vtkImageAppend.h"

#include <dcmtk\config\osconfig.h>
#include <dcmtk\dcmdata\dctk.h>
#include "dcmtk\dcmdata\dcistrmf.h"
#include "SlicePlayer.h"
#include "DicomDataBase.h"
#include <QFileSystemModel>
#include "DicomDirTreeModel.h"

#include <vtkImageSegmentCallback.h>
#include <vtkDecimatePro.h>
#include "ThreeD_Reconstruction.h"

/*
 * 构造方法
 */
QvtkDicomViewer::QvtkDicomViewer(QWidget *parent)
	: QMainWindow(parent)
{
	//无边框,保持在顶端
	//setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
	ui.setupUi(this);
	ui.action_Pointer->setChecked(true);
	CursorType = CURSOR::POINTRE;
	//监控光标类型的修改
	connect(this, SIGNAL(CursorValueChanged()), this, SLOT(OnChangeCursorValue()));
	ui.action_SwitchOfProperty->setChecked(true);
	ui.dockWidget_Dir->setHidden(false);
	icon_Play.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/play_128px_1197036_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	icon_Pause.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/pause_128px_1197034_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	PlayFlag = false;
	ui.action_Stop->setEnabled(false);
	//上下文菜单
	CreateContextMenu();
	//DirTreeRefresh(NULL);
	m_Reg_Window = new Register();//事先初始化配准工具
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
* 响应窗宽窗位模式值的修改,执行一些禁用和选定动作
*/
void QvtkDicomViewer::OnChangeWindowsWL()
{

	ui.action_WindowWL_Default->setChecked(false);
	ui.action_WindowWL_All->setChecked(false);
	ui.action_WindowWL_CT_Abdomen->setChecked(false);
	ui.action_WindowWL_CT_BloodVessel->setChecked(false);
	ui.action_WindowWL_CT_Bones->setChecked(false);
	ui.action_WindowWL_CT_Brain->setChecked(false);
	ui.action_WindowWL_CT_Medias->setChecked(false);
	ui.action_WindowWL_CT_Lungs->setChecked(false);

	switch (ImageWindow)
	{
	case Default:
		ui.action_WindowWL_Default->setChecked(true);
		break;
	case All:
		ui.action_WindowWL_All->setChecked(true);
		break;
	case Abdomen:
		ui.action_WindowWL_CT_Abdomen->setChecked(true);
		break;
	case BloodVessel:
		ui.action_WindowWL_CT_BloodVessel->setChecked(true);
		break;
	case Bones:
		ui.action_WindowWL_CT_Bones->setChecked(true);
		break;
	case Brain:
		ui.action_WindowWL_CT_Brain->setChecked(true);
		break;
	case Medias:
		ui.action_WindowWL_CT_Medias->setChecked(true);
		break;
	case Lungs:
		ui.action_WindowWL_CT_Lungs->setChecked(true);
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
void QvtkDicomViewer::SetUsageText(std::string imagefilename)
{
	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile(imagefilename.c_str());
	if(status.bad())
	{
		//异常
		return;	
	}
	m_pImageViewer->GetRenderer()->RemoveActor(usageTextActor1);//清除
	m_pImageViewer->GetRenderer()->RemoveActor(usageTextActor2);//清除
	m_pImageViewer->GetRenderer()->RemoveActor(usageTextActor3);//清除
#if _DEBUG
	//std::filebuf lb;
	//lb.open("metainfo.log", std::ios::out);
	//std::ostream out2(&lb);
	//fileformat.print(out2);
	//lb.close();
#endif

	OFString temp_OFString;
	std::string TopLeftCorner = "";
	std::string TopRightCorner = "";
	std::string LowerRightCorner = "";

#pragma region 右上角信息
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_PatientName, temp_OFString, OFTrue).good())
	{
		//TopRightCorner.append("Patient Name:");
		TopRightCorner.append(temp_OFString.c_str());
		TopRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_PatientID, temp_OFString, OFTrue).good())
	{
		//TopRightCorner.append("Patient ID:");
		TopRightCorner.append(temp_OFString.c_str());
		TopRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_PatientSex, temp_OFString, OFTrue).good())
	{
		//TopRightCorner.append("Patient Sex:");
		TopRightCorner.append(temp_OFString.c_str());
		TopRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_InstitutionName, temp_OFString, OFTrue).good())
	{
		//TopRightCorner.append("InstitutionName:");
		TopRightCorner.append(temp_OFString.c_str());
		TopRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_StudyID, temp_OFString, OFTrue).good())
	{
		//TopRightCorner.append("Study ID:");
		TopRightCorner.append(temp_OFString.c_str());
		TopRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_StudyDescription, temp_OFString, OFTrue).good())
	{
		//TopRightCorner.append("Study Description:");
		TopRightCorner.append(temp_OFString.c_str());
		TopRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_SeriesDescription, temp_OFString, OFTrue).good())
	{
		//TopRightCorner.append("Series Description:");
		TopRightCorner.append(temp_OFString.c_str());
		TopRightCorner.append("\n");
	}
#pragma endregion

#pragma region 左上角信息
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_SeriesNumber, temp_OFString, OFTrue).good())
	{
		TopLeftCorner.append("Series Number:");
		TopLeftCorner.append(temp_OFString.c_str());
		TopLeftCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_WindowCenter, temp_OFString, OFTrue).good())
	{//窗位
		TopLeftCorner.append("Window Center:");
		TopLeftCorner.append(temp_OFString.c_str());
		TopLeftCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_WindowWidth, temp_OFString, OFTrue).good())
	{//窗宽
		TopLeftCorner.append("Window Width:");
		TopLeftCorner.append(temp_OFString.c_str());
		TopLeftCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_SpacingBetweenSlices, temp_OFString, OFTrue).good())
	{//层厚,mm
		TopLeftCorner.append("Spacing Between Slices:");
		TopLeftCorner.append(temp_OFString.c_str());
		TopLeftCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_SliceLocation, temp_OFString, OFTrue).good())
	{//层间距mm
		TopLeftCorner.append("Slice Location:");
		TopLeftCorner.append(temp_OFString.c_str());
		TopLeftCorner.append("\n");
	}
#pragma endregion 

#pragma region 右下角信息
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_MagneticFieldStrength, temp_OFString, OFTrue).good())
	{//磁场强度
		LowerRightCorner.append("Magnetic Field Strength:");
		LowerRightCorner.append(temp_OFString.c_str());
		LowerRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_RepetitionTime, temp_OFString, OFTrue).good())
	{//重复时间
		LowerRightCorner.append("Repetition Time:");
		LowerRightCorner.append(temp_OFString.c_str());
		LowerRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_EchoTime, temp_OFString, OFTrue).good())
	{//回波时间
		LowerRightCorner.append("Echo Time:");
		LowerRightCorner.append(temp_OFString.c_str());
		LowerRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_ContentDate, temp_OFString, OFTrue).good())
	{//日期
		LowerRightCorner.append("Content Date:");
		LowerRightCorner.append(temp_OFString.c_str());
		LowerRightCorner.append("\n");
	}
	if (fileformat.getDataset()->findAndGetOFStringArray(DCM_ContentTime, temp_OFString, OFTrue).good())
	{//时间
		LowerRightCorner.append("Content Time:");
		LowerRightCorner.append(temp_OFString.c_str());
		LowerRightCorner.append("\n");
	}
#pragma endregion 

	const char* message_TopLeftCorner = TopLeftCorner.c_str();
	const char* message_TopRightCorner = TopRightCorner.c_str();
	const char* message_LowerRightCorner = LowerRightCorner.c_str();
	//文字样式:左上角
	vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();   
	usageTextProp->SetFontSize(20);				
	usageTextProp->SetColor(1.0, 1.0, 0.0);			
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();		
	vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput(message_TopLeftCorner);
	usageTextMapper->SetTextProperty(usageTextProp);
	usageTextActor1 = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor1->SetMapper(usageTextMapper);
	usageTextActor1->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor1->GetPositionCoordinate()->SetValue(0.05, 0.95);//坐标
	m_pImageViewer->GetRenderer()->AddActor2D(usageTextActor1);
	//右上角
	vtkSmartPointer<vtkTextProperty> usageTextProp2 = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp2->SetFontFamilyToCourier();
	usageTextProp2->SetFontSize(20);				
	usageTextProp2->SetColor(0.0, 1.0, 0.0);			
	usageTextProp2->SetVerticalJustificationToTop();
	usageTextProp2->SetJustificationToRight();		
	vtkSmartPointer<vtkTextMapper> usageTextMapper2 = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper2->SetInput(message_TopRightCorner);
	usageTextMapper2->SetTextProperty(usageTextProp2);
	usageTextActor2 = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor2->SetMapper(usageTextMapper2);
	usageTextActor2->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor2->GetPositionCoordinate()->SetValue(0.95, 0.95);//坐标
	m_pImageViewer->GetRenderer()->AddActor2D(usageTextActor2);
	//右下角
	vtkSmartPointer<vtkTextProperty> usageTextProp3 = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp3->SetFontFamilyToCourier();
	usageTextProp3->SetFontSize(20);				
	usageTextProp3->SetColor(1.0, 1.0, 0.0);			
	usageTextProp3->SetVerticalJustificationToBottom();
	usageTextProp3->SetJustificationToRight();		
	vtkSmartPointer<vtkTextMapper> usageTextMapper3 = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper3->SetInput(message_LowerRightCorner);
	usageTextMapper3->SetTextProperty(usageTextProp3);
	usageTextActor3 = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor3->SetMapper(usageTextMapper3);
	usageTextActor3->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor3->GetPositionCoordinate()->SetValue(0.95, 0.05);//坐标
	m_pImageViewer->GetRenderer()->AddActor2D(usageTextActor3);
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
	connect(action_New_Query_Patient_Msg, SIGNAL(triggered()), this, SLOT(OnShowDicomCurrentTags()));

	QAction* action_New_Render_Series = new QAction(QStringLiteral("&显示这个Series"), ui.treeView);
	connect(action_New_Render_Series, SIGNAL(triggered()), this, SLOT(OnShowSelectedSeries()));

	QAction* action_3D_Reconstruction = new QAction(QStringLiteral("&3D重建"), ui.treeView);
	connect(action_3D_Reconstruction, SIGNAL(triggered()), this, SLOT(On3D_Reconstruction()));

	QAction* action_New_Render_Image = new QAction(QStringLiteral("&显示这个Image"), ui.treeView);
	connect(action_New_Render_Image, SIGNAL(triggered()), this, SLOT(OnShowSelectedImage()));

	QAction* action_Segment_Image = new QAction(QStringLiteral("&对该Image进行分割"), ui.treeView);
	connect(action_Segment_Image, SIGNAL(triggered()), this, SLOT(OnSegmentImage()));

	//树右键菜单->空树
	TreeViewMenu_OnEmpty = new QMenu(ui.treeView);
	TreeViewMenu_OnEmpty->addAction(action_New_Open_DICOMDIR_File);
	//树右键菜单->Patient节点
	TreeViewMenu_OnPatient = new QMenu(ui.treeView);
	TreeViewMenu_OnPatient->addAction(action_New_Query_Patient_Msg);
	//树右键菜单->Series节点
	TreeViewMenu_OnSeries = new QMenu(ui.treeView);
	TreeViewMenu_OnSeries->addAction(action_New_Render_Series);
	TreeViewMenu_OnSeries->addAction(action_3D_Reconstruction);
	//树右键菜单->Image节点
	TreeViewMenu_OnImage = new QMenu(ui.treeView);
	TreeViewMenu_OnImage->addAction(action_New_Render_Image);
	TreeViewMenu_OnImage->addAction(action_Segment_Image);
}
/*
 * 显示当前series中的第Index张图,Index从0开始,与滚动条配合
 */
void QvtkDicomViewer::ShowImageByIndex(int Index)
{
	RenderRefresh(CurrentPatient->getDicomImageByIndex(Index)->AbsFilePath, 
				  Index + 1, 
				  CurrentPatient->getCurrentDicomSeries()->ImageList.size());
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
* 修改窗宽窗位模式
*/
void QvtkDicomViewer::setWindowWL(WINDOWWL newWL)
{
	ImageWindow = newWL;
	emit WindowWLChanged();//值更改,发出信号
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
	SetSliceText(1, NumOfImage);// 初始化切片页码信息
	SetUsageText(folder.c_str());// 显示一些Dicom文件头信息
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
	connect(m_slice_player, SIGNAL(isTimeToTurnNextSlice()), this, SLOT(OnForward()), Qt::QueuedConnection);
	connect(m_slice_player, SIGNAL(isTimeToReset()), this, SLOT(OnResetToFirst()), Qt::QueuedConnection);
	/*
	 * 渲染
	 */
	m_pImageViewer->GetRenderer()->ResetCamera();
	ui.qvtkWidget->GetRenderWindow()->Render();
	/*
	 * 配合进度条
	 */
	ui.SliceScrollBar->setRange(0, NumOfImage - 1);
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
void QvtkDicomViewer::DirTreeRefresh(DicomPatient * patient)
{
	QStringList headers;
	headers.append(QStringLiteral("ID"));
	headers.append(QStringLiteral("详细信息"));
	m_dicomdirtreemodel = new DicomDirTreeModel(headers, *patient);
	ui.treeView->setModel(m_dicomdirtreemodel);
	ui.treeView->expandAll();
	for (int column = 0; column < m_dicomdirtreemodel->columnCount(); ++column)
		ui.treeView->resizeColumnToContents(column);
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
	//m_pImageViewer->SetColorLevel(300.0);
	//m_pImageViewer->SetColorWindow(1500.0);
	//m_pImageViewer->Render();
	//m_pImageViewer-
	//double range[2];
	//reader->GetOutput()->GetScalarRange(range);
	//vtkImageShiftScale* shifter = vtkImageShiftScale::New();
	//shifter->SetShift(-1.0*range[0]);//减去最小值
	//shifter->SetScale(255.0*-1 / (range[1] - range[0])); //调节比例 //灰度换算公式：255*X/(range[1]-range[0])
	//shifter->SetOutputScalarTypeToUnsignedChar();
	//shifter->SetInputConnection(reader->GetOutputPort());
	//计算公式： double val = ((double)(*inSI) + shift) * scale;
	//输出 = （输入 + shift）*scale; shift为偏移量, scale灰度变
	//////////////////////////////////////////////////////////////////////////////
	//int subRegion[6] = { 0, 511, 0, 511, 0, 61 };//64, 448, 64, 448, 0, 0
	//ui.qvtkWidget->
	//vtkImageIterator<unsigned char> iter(reader->GetOutput(),reader->GetOutput()->GetExtent());
	//											
	//while (!iter.IsAtEnd())//注意这个迭代有自带的互斥效果					 
	//{								 	
	//	unsigned char *inSI = iter.BeginSpan();									 	
	//	unsigned char *inSIEnd = iter.EndSpan();					 	
	//	while (inSI != inSIEnd)								 	
	//	{								 		
	//		*inSI = 255 - *inSI;								 		
	//		++inSI;									 	
	//	}									 	
	//	iter.NextSpan();								
	//}
	////////////////////////////////////////////////////////////////////////////////
	/*vtkSmartPointer<vtkImageLogic>imageMath =vtkSmartPointer<vtkImageLogic>::New();
	imageMath->SetOperationToNot();
	imageMath->SetInputConnection(reader->GetOutputPort());
	imageMath->Update();*/
	/////////////////////////////////////////////////////////////////////////////////
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
	 if (PrePosition != pos) {//这次触发是正常触发
		 PrePosition = pos;
		 if (ui.treeView->model() == NULL) {
			 //此时树是空的
			 TreeViewMenu_OnEmpty->exec(QCursor::pos());//显示右键菜单
		 }
		 else//树非空的时候才能启动这个
		 {
			 indexSelect = ui.treeView->indexAt(pos);  //当前节点索引
			 switch (m_dicomdirtreemodel->getLevel(indexSelect))
			 {
			 case 1://level==1:Patient
				 TreeViewMenu_OnPatient->exec(QCursor::pos());
				 break;
			 case 2://level==2:Study
				 //目前还没有对应的菜单
				 break;
			 case 3://level==3:Series
				 TreeViewMenu_OnSeries->exec(QCursor::pos());
				
				 break;
			 case 4://level==4:image
				 TreeViewMenu_OnImage->exec(QCursor::pos());

				 break;
			 default://其他情况处理不了,报错
				 QMessageBox::information(this,QStringLiteral("错误!"), QStringLiteral("错误!"));
				 break;
			 }
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
 *树视图右键菜单->显示当前病人的所有信息
 */
 void QvtkDicomViewer::OnShowDicomCurrentTags()
 {
	 QMessageBox::information(this, QStringLiteral("嘿嘿!"), QStringLiteral("这个人很懒,什么都没留下!"));
 }
/*
 * 显示选中的Series
 */
 void QvtkDicomViewer::OnShowSelectedSeries()
 {
	 CurrentPatient->setCurrentDicomSeriesById(m_dicomdirtreemodel->getItem(indexSelect)->itemData[0].toString().toStdString());
	 RenderInitializer(CurrentPatient->getCurrentDicomImage()->AbsFilePath, CurrentPatient->getCurrentDicomSeries()->ImageList.size());
 }
/*
 * 显示当前选中的Image
 */
 void QvtkDicomViewer::OnShowSelectedImage()
 {
	 /*
	  *	打开这张图所在的series并调整到这张图
	  */
	 CurrentPatient->getDicomImageByRfid(m_dicomdirtreemodel->getItem(indexSelect)->itemData[1].toString().toStdString());
	 RenderInitializer(CurrentPatient->getCurrentDicomImage()->AbsFilePath, CurrentPatient->getCurrentDicomSeries()->ImageList.size());
	 ui.SliceScrollBar->setValue(CurrentPatient->indexOfCurrentImage);
 }
/*
 * Slice滚动条值更改事件
 */
 void QvtkDicomViewer::OnSliceScrollBarValueChange(int a)
 {
	 ShowImageByIndex(a);
 }
 /*
  *	测试入口1
  */
 void QvtkDicomViewer::OnTestEntrance_01()
 {
	/*
	 * 现在测试的是显示一张图片并显示叠加文字
	 */
	 RenderInitializer("F:/100098.dcm");
 }
 /*
  *	测试入口2
  */
 void QvtkDicomViewer::OnTestEntrance_02()
 {
	 QStringList headers;
	 headers.append(QStringLiteral("ID"));
	 headers.append(QStringLiteral("详细信息"));

	 //QFile file("defaultTreeContext.txt");
	 //file.open(QIODevice::ReadOnly);
	 //DicomDirTreeModel *model = new DicomDirTreeModel(headers, file.readAll());
	 //file.close();
	 //ui.treeView->setModel(model);
	 //ui.treeView->expandAll();
	 //for (int column = 0; column < model->columnCount(); ++column)
		//ui.treeView->resizeColumnToContents(column);

	 DicomDataBase::getInstance()->Init("F:/Dicom/Test1/DICOMDIR");
	 Current_patientId = "CT475430";
	 CurrentPatient = new DicomPatient(DicomDataBase::getInstance()->getPatientById(Current_patientId));
	 m_dicomdirtreemodel = new DicomDirTreeModel(headers, CurrentPatient);

	 ui.treeView->setModel(m_dicomdirtreemodel);
	 ui.treeView->expandAll();
	 for (int column = 0; column < m_dicomdirtreemodel->columnCount(); ++column)
		 ui.treeView->resizeColumnToContents(column);
 }
 /*
  *	响应三维重建命令
  */
void QvtkDicomViewer::On3D_Reconstruction()
{
	CurrentPatient->setCurrentDicomSeriesById(m_dicomdirtreemodel->getItem(indexSelect)->itemData[0].toString().toStdString());
	//构造
	std::vector<std::string> * filenames_v = new std::vector<std::string>;
	for (int i=0;i<CurrentPatient->getCurrentDicomSeries()->ImageList.size();i++)
	{
		filenames_v->push_back(CurrentPatient->getCurrentDicomSeries()->ImageList.at(i)->AbsFilePath);
	}
	ThreeD_Reconstruction * _3d_reconstructer = new ThreeD_Reconstruction(filenames_v);
	_3d_reconstructer->show();
	_3d_reconstructer->OnReconstruction();
}
/*
 * 响应图像分割操作
 */
void QvtkDicomViewer::OnSegmentImage()
{
	/*
	 * 先显示这张image
	 */
	CurrentPatient->getDicomImageByRfid(m_dicomdirtreemodel->getItem(indexSelect)->itemData[1].toString().toStdString());
	RenderInitializer(CurrentPatient->getCurrentDicomImage()->AbsFilePath, CurrentPatient->getCurrentDicomSeries()->ImageList.size());
	ui.SliceScrollBar->setValue(CurrentPatient->indexOfCurrentImage);
	/*
	 * 启动分割工具并传递关键参数
	 */
	Segmenter *_segmenter = new Segmenter(CurrentPatient->getCurrentDicomImage()->AbsFilePath);
	_segmenter->show();
}
/*
  *	响应DicomDir类传送过来的信号,其中包含了一个病人的ID
  */
 void QvtkDicomViewer::receiveData(QString data,QString dir)
 {
	DicomDataBase * temp_database = DicomDataBase::getInstance();
	Current_patientId = data.toStdString();//当前的病人ID
	//当前病人对象绑定,注意这应该是全局唯一的绑定点
	CurrentPatient = new DicomPatient(temp_database->getPatientById(Current_patientId));
	DirTreeRefresh(CurrentPatient);//刷新树视图
	RenderInitializer(CurrentPatient->getCurrentDicomImage()->AbsFilePath, CurrentPatient->getCurrentDicomSeries()->ImageList.size());
 }
 /*
  * 下一个病人
  */
 void QvtkDicomViewer::OnLatterPatient()
 {
	 if (CurrentPatient == NULL)
		 return;
	 //1.从数据库类接口上换下一个病人
	 DicomPatient* temp=DicomDataBase::getInstance()->get_latter_patient(CurrentPatient);
	 //2.重新绑定当前病人
	 CurrentPatient = new DicomPatient(temp);;
	 DirTreeRefresh(CurrentPatient);//刷新树视图
	 RenderInitializer(CurrentPatient->getCurrentDicomImage()->AbsFilePath, CurrentPatient->getCurrentDicomSeries()->ImageList.size());
 }
 /*
  * 上一个病人
  */
 void QvtkDicomViewer::OnPreviousPatient()
 {
	 if (CurrentPatient == NULL)
		 return;
	 //1.从数据库类接口上换下一个病人
	 DicomPatient* temp = DicomDataBase::getInstance()->get_previous_patient(CurrentPatient);
	 //2.重新绑定当前病人
	 CurrentPatient = new DicomPatient(temp);;
	 DirTreeRefresh(CurrentPatient);//刷新树视图
	 RenderInitializer(CurrentPatient->getCurrentDicomImage()->AbsFilePath, CurrentPatient->getCurrentDicomSeries()->ImageList.size());
 }

 /*
 * 默认窗宽窗位
 */
 void QvtkDicomViewer::OnWindowWL_Defaut()
 {
	 m_pImageViewer->SetColorLevel(40.0);
	 m_pImageViewer->SetColorWindow(400.0);
	 m_pImageViewer->Render();
	 setWindowWL(Default);
 }
 /*
 *	全部动态范围
 */
 void QvtkDicomViewer::OnWindowWL_All()
 {
	 m_pImageViewer->SetColorLevel(1024.0);
	 m_pImageViewer->SetColorWindow(4096.0);
	 m_pImageViewer->Render();
	 setWindowWL(All);
 }
 /*
 *	腹部
 */
 void QvtkDicomViewer::OnWindowWL_CT_Abdomen()
 {
	 m_pImageViewer->SetColorLevel(60.0);
	 m_pImageViewer->SetColorWindow(400.0);
	 m_pImageViewer->Render();
	 setWindowWL(Abdomen);
 }
 /*
 *	血管
 */
 void QvtkDicomViewer::OnWindowWL_CT_BloodVessel()
 {
	 m_pImageViewer->SetColorLevel(300.0);
	 m_pImageViewer->SetColorWindow(600.0);
	 m_pImageViewer->Render();
	 setWindowWL(BloodVessel);
 }
 /*
 *	骨骼
 */
 void QvtkDicomViewer::OnWindowWL_CT_Bones()
 {
	 m_pImageViewer->SetColorLevel(300.0);
	 m_pImageViewer->SetColorWindow(1500.0);
	 m_pImageViewer->Render();
	 setWindowWL(Bones);
 }
 /*
 *	脑
 */
 void QvtkDicomViewer::OnWindowWL_CT_Brain()
 {
	 m_pImageViewer->SetColorLevel(40.0);
	 m_pImageViewer->SetColorWindow(80.0);
	 m_pImageViewer->Render();
	 setWindowWL(Brain);
 }
 /*
 *	纵膈
 */
 void QvtkDicomViewer::OnWindowWL_CT_Medias()
 {
	 m_pImageViewer->SetColorLevel(40.0);
	 m_pImageViewer->SetColorWindow(400.0);
	 m_pImageViewer->Render();
	 setWindowWL(Medias);
 }
 /*
 *	肺
 */
 void QvtkDicomViewer::OnWindowWL_CT_Lungs()
 {
	 m_pImageViewer->SetColorLevel(-400.0);
	 m_pImageViewer->SetColorWindow(1500.0);
	 m_pImageViewer->Render();
	 setWindowWL(Lungs);
 }
 /*
 * 启动配准工具
 */
 void QvtkDicomViewer::OnRegistration()
 {
	 m_Reg_Window->show();
	//为了增加响应速度,初始化代码应该统一起来,这是一个尝试,以后会逐渐改成这样
 }
