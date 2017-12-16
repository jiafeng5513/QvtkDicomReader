#include <vtkImageCast.h>
#include <vtkBMPWriter.h>
#include <vtkPNGWriter.h>
#include <head_all.h>
#include <DicomSeg.h>
#include "itkRescaleIntensityImageFilter.h"  
#include "itkGDCMImageIO.h"  
#include "itkBMPImageIO.h"  
#include "itkCastImageFilter.h"  
#include <vtkDICOMImageReader.h>
#include "vtkImageSegmentCallback.h"

/*===================================class : MyStyle========================================*/
/*
 * 静态构造
 */
MyStyle*MyStyle:: New()
{
	return new MyStyle;
}
/*
 * 重载左键按下
 */
void MyStyle::OnLeftButtonDown()
{
	std::cout << "Pressed left mouse button." << std::endl;
	// Forward events  
	//vtkInteractorStyleImage::OnLeftButtonDown();
}

/*===========================class : vtkImageSegmentCallback=================================*/

/*
 * 静态构造(配合智能指针)
 */
vtkImageSegmentCallback *vtkImageSegmentCallback::New()
{
	return new vtkImageSegmentCallback;
}
/*
 * 构造(为了支持非智能指针,不要设私有)
 */
vtkImageSegmentCallback::vtkImageSegmentCallback()
{
	this->Viewer = 0;
	this->Picker = 0;
	this->Annotation = 0;
	this->PointData = vtkPointData::New();
}
/*
 * 析构
 */
vtkImageSegmentCallback::~vtkImageSegmentCallback()
{
	this->Viewer = 0;
	this->Picker = 0;
	this->Annotation = 0;
	this->PointData->Delete();
}
//setter
void vtkImageSegmentCallback::SetPicker(vtkPropPicker *picker)
{
	this->Picker = picker;
}
//setter
void vtkImageSegmentCallback::SetAnnotation(vtkCornerAnnotation *annotation)
{
	this->Annotation = annotation;
}
//setter
void vtkImageSegmentCallback::SetViewer(vtkImageViewer2 *viewer)
{
	this->Viewer = viewer;
}
//setter
void vtkImageSegmentCallback::SetQvtk(QVTKWidget* qvtk)
{
	this->qvtk = qvtk;
}
//setter
void vtkImageSegmentCallback::SetCount(int count)
{
	this->count = count;
}
//setter
void vtkImageSegmentCallback::SetDir(std::string dir)
{
	//this->dir = dir;
}
/*
 * 对不同分割算法的分发
 */
void vtkImageSegmentCallback::Execute(vtkObject *, unsigned long event, void *)
	{
		//this->Viewer;
		MyStyle *mystyle = new MyStyle;
		vtkRenderWindowInteractor *interactor =
			this->Viewer->GetRenderWindow()->GetInteractor();
		//interactor->SetInteractorStyle(mystyle);
		vtkRenderer* renderer = this->Viewer->GetRenderer();
		vtkImageActor* actor = this->Viewer->GetImageActor();
		vtkImageData* image = this->Viewer->GetInput();
		vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(
			interactor->GetInteractorStyle());

		// Pick at the mouse location provided by the interactor
		this->Picker->Pick(interactor->GetEventPosition()[0],
			interactor->GetEventPosition()[1],
			0.0, renderer);

		// There could be other props assigned to this picker, so
		// make sure we picked the image actor
		vtkAssemblyPath* path = this->Picker->GetPath();
		bool validPick = false;
		if (path)
		{
			vtkCollectionSimpleIterator sit;
			path->InitTraversal(sit);
			vtkAssemblyNode *node;
			for (int i = 0; i < path->GetNumberOfItems() && !validPick; ++i)
			{
				node = path->GetNextNode(sit);
				if (actor == vtkImageActor::SafeDownCast(node->GetViewProp()))
				{
					validPick = true;
				}
			}
		}

		if (!validPick)
		{
			this->Annotation->SetText(0, "Off Image");
			interactor->Render();
			// Pass the event further on
			style->OnMouseMove();
			return;
		}

		// Get the world coordinates of the pick
		double pos[3];
		this->Picker->GetPickPosition(pos);
		// Fixes some numerical problems with the picking
		double *bounds = actor->GetDisplayBounds();
		int axis = this->Viewer->GetSliceOrientation();
		pos[axis] = bounds[2 * axis];

		vtkPointData* pd = image->GetPointData();
		if (!pd)
		{
			return;
		}

		this->PointData->InterpolateAllocate(pd, 1, 1);


		// Use tolerance as a function of size of source data
		double tol2 = image->GetLength();
		tol2 = tol2 ? tol2 * tol2 / 1000.0 : 0.001;

		// Find the cell that contains pos
		int subId;
		double pcoords[3], weights[8];
		vtkCell* cell = image->FindAndGetCell(
			pos, NULL, -1, tol2, subId, pcoords, weights);
		int pixel_avg = 0;
		if (cell)
		{
			// Interpolate the point data
			this->PointData->InterpolatePoint(pd, 0, cell->PointIds, weights);
			int components =
				this->PointData->GetScalars()->GetNumberOfComponents();
			double* tuple = this->PointData->GetScalars()->GetTuple(0);

			std::string message = "Location: ( ";
			message += vtkVariant(pos[0]).ToString();
			message += ", ";
			message += vtkVariant(pos[1]).ToString();
			message += ", ";
			message += vtkVariant(pos[2]).ToString();
			message += " )\nValue: ( ";

			for (int c = 0; c < components; ++c)
			{
				message += vtkVariant(tuple[c]).ToString();
				if (c != components - 1)
				{
					message += ", ";
					//message += event;
				}
			}
			message += " )";
			this->Annotation->SetText(0, message.c_str());
			interactor->Render();
			style->OnMouseMove();
		}
		if (event == vtkCommand::LeftButtonPressEvent)
		{
			char pos_x[20];
			sprintf(pos_x, "%.7lf\n", pos[0]);
			char pos_y[20];
			sprintf(pos_y, "%.7lf\n", pos[1]);
			double* pixel = this->PointData->GetScalars()->GetTuple(0);
			char pixel_f[20] = {};
			sprintf(pixel_f, "%.7lf\n", pixel[0] - 600);
			char pixel_s[20] = {};
			sprintf(pixel_s, "%.7lf\n", pixel[0] + 600);

			//char* dir = const_cast<char*> (this->dir.c_str());

			char * argv[] = {
				" ",
				"F:/Dicom/Test1/DICOM/S427870/S20/I10",
				//"C://Users//bao//Desktop//test-image//BrainProtonDensitySlice.png",
				//dir,
				"..//segment//ConnectedThresholdOutput1.dcm",
				pos_x,
				pos_y,
				pixel_f,
				pixel_s };
			char * argv1[] =
			{
				" ",
				"F:/Dicom/Test1/DICOM/S427870/S20/I10",
				//"C://Users//bao//Desktop//test-image//BrainProtonDensitySlice.png",
				//dir,
				"OtsuMultipleThresholdsOutput",
				"dcm",
				"4",
				pixel_f,
				pixel_s
				//argv[5] = "150";
				//argv[6] = "180";
			};
			char * argv2[] =
			{
				" ",
				"F:/Dicom/Test1/DICOM/S427870/S20/I10",
				//"C://Users//bao//Desktop//test-image//BrainProtonDensitySlice.png",
				//dir,
				"NeighconnectedOutput.dcm",
				pos_x,
				pos_y,
				pixel_f,
				pixel_s
			};
			char * argv3[] =
			{
				" ",
				"F:/Dicom/Test1/DICOM/S427870/S20/I10",
				//"C://Users//bao//Desktop//test-image//BrainProtonDensitySlice.png",
				//dir,
				"..//segment//ConfidenceConnectedOutput1.dcm",
				pos_x,
				pos_y,
			};
			char * argv4[] =
			{
				" ",
				//"C://Users//bao//Desktop//test-image//VisibleWomanEyeSlice.png",
				"F:/Dicom/Test1/DICOM/S427870/S20/I10",
				//dir,
				"..//segment//WatershedSegmentation1Output1.dcm",
				"2",
				"10",
				"0",
				"0.05",
				"1"
			};
			char * argv5[] =
			{
				" ",
				//"C://Users//bao//Desktop//test-image//BrainProtonDensitySlice.png",
				//dir,
				"F:/Dicom/Test1/DICOM/S427870/S20/I10",
				"..//segment//FastMarchingImageFilterOutput5.dcm",
				pos_x,
				pos_y,
				"1.0" ,
				"-0.5" ,
				"3.0",
				"100",
				"100",
				"..//segment//FastMarchingFilterOutput1.dcm",
				"..//segment//FastMarchingFilterOutput2.dcm",
				"..//segment//FastMarchingFilterOutput3.dcm"
			};
			char * argv6[] =
			{
				"",
				//"C://Users//bao//Desktop//test-image//BrainProtonDensitySlice.png",
				//dir,
				"F:/Dicom/Test1/DICOM/S427870/S20/I10",
				"..//segment//ShapeDetectionLevelSetFilterOutput5.dcm",
				pos_x,
				pos_y,
				"5",
				"1.0"  ,
				"-0.5" ,
				"3.0"   ,
				".05" ,
				"1"
			};
			SegmentFunc seg_enum;
			seg_enum = (SegmentFunc)this->count;
			switch (seg_enum)
			{
			case  NULL_Seg:
				break;
			case  Seg_connectedthres:
				seg_connectedthres(1, argv, this->qvtk);
				break;
			case  Seg_ostu:
				seg_ostu(1, argv1, this->qvtk);
				break;
			case  Seg_neighconnected:

				seg_neighconnected(1, argv2, this->qvtk);
				break;
			case  Seg_confidconnected:

				seg_confidconnected(1, argv3, this->qvtk);
				break;
			case  Seg_waterseg:

				seg_waterseg(1, argv4, this->qvtk);
				break;
			case  Seg_fastmarching:

				seg_fastmarching(1, argv5, this->qvtk);
				break;
			case  Seg_shapedectection:

				seg_shapedectection(1, argv6, this->qvtk);
				break;
			default:
				break;
			}
		}
	}



/*
 *	 //std::string dir = CurrentPatient->getCurrentDicomImage()->ReferencedFileID;
	 char* argv[] = { "   ", "F:/Dicom/Test1/DICOM/S427870/S20/I10" };
	 std::string dir = "";
	 pickpixel(count,argv, ui.qvtkWidget,dir);
  
 * 参数:int count :			使用何种分割算法
 *      char * argv []		待处理文件名
 *      QVTKWidget *qvtk	向何处输出
 *      std::string dir		不知道
 * 功能:分割功能的入口
 * 
 */
int pickpixel(int count, char* argv[], QVTKWidget *qvtk,std::string dir)
{
	std::cout << "Usage: " << argv[0]
		<< " Filename" << std::endl;
	// Read the image
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName(argv[1]);
	reader->Update();

	/*vtkImageCast *imgcast = vtkImageCast::New();
	imgcast->SetInputData(reader->GetOutput());
	imgcast->SetOutputScalarTypeToUnsignedChar();

	vtkBMPWriter *writer = vtkBMPWriter::New();
	imgcast->Update();
	writer->SetInputData(imgcast->GetOutput());
	writer->SetFileName("444.bmp");
	writer->Write();
	writer->Update();*/


	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());

	imageViewer->SetGlobalWarningDisplay(0);
	// Picker to pick pixels
	vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
	propPicker->PickFromListOn();

	// Give the picker a prop to pick
	propPicker->AddPickList(imageViewer->GetImageActor());

	// Visualize
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	imageViewer->SetupInteractor(renderWindowInteractor);
	imageViewer->SetSize(600, 600);

	vtkRenderer* renderer = imageViewer->GetRenderer();
	renderer->ResetCamera();
	renderer->GradientBackgroundOn();
	renderer->SetBackground(0, 0, 0);
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
	callback->SetQvtk(qvtk);
	callback->SetCount(count);
	callback->SetDir(dir);
	// InteractorStyleImage allows for the following controls:
	// 1) middle mouse + move = camera pan
	// 2) left mouse + move = window/level
	// 3) right mouse + move = camera zoom
	// 4) middle mouse wheel scroll = zoom
	// 5) 'r' = reset window/level
	// 6) shift + 'r' = reset camera
	vtkInteractorStyleImage* imageStyle = imageViewer->GetInteractorStyle();
	qvtk->SetRenderWindow(imageViewer->GetRenderWindow());

	imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
	imageStyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);

	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
	//reader->Delete();

	return EXIT_SUCCESS;
}
