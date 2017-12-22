#include <vtkImageCast.h>
#include <vtkBMPWriter.h>
#include <vtkPNGWriter.h>
#include <head_all.h>
#include "itkRescaleIntensityImageFilter.h"  
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

//====================================================================================

/*
 * 区域生长法-重构
 * filename			:	待处理文件名
 * pos_x,pos_y		:	种子点
 * pixel_f,pixel_s	:	上下界
 */
void vtkImageSegmentCallback::seg_connectedthres(std::string filename, double pos_x, double pos_y, double pixel_f, double pixel_s)
{
	ConnectorType::Pointer connector = ConnectorType::New();
	CastingFilterType::Pointer caster = CastingFilterType::New();
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	reader->SetFileName(filename);
	reader->SetImageIO(gdcmIO);
	reader->Update();

	CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();
	typedef itk::ConnectedThresholdImageFilter< InternalImageType, InternalImageType > ConnectedFilterType;
	ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();

	smoothing->SetInput(reader->GetOutput());
	connectedThreshold->SetInput(smoothing->GetOutput());
	caster->SetInput(connectedThreshold->GetOutput());
	connector->SetInput(caster->GetOutput());
	smoothing->SetNumberOfIterations(5);
	smoothing->SetTimeStep(0.125);

	const InternalPixelType lowerThreshold = pixel_f;
	const InternalPixelType upperThreshold = pixel_s;

	connectedThreshold->SetLower(lowerThreshold);
	connectedThreshold->SetUpper(upperThreshold);

	connectedThreshold->SetReplaceValue(255);

	InternalImageType::IndexType  index;
	index[0] = pos_x;
	index[1] = pos_y;
	connectedThreshold->SetSeed(index);

	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
#if VTK_MAJOR_VERSION <= 5
	actor->SetInput(connector->GetOutput());
#else
	connector->Update();
	actor->GetMapper()->SetInputData(connector->GetOutput());
#endif
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->ResetCamera();
	renderer->SetBackground(0.0, 0.0, 0.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	qvtk->SetRenderWindow(renderWindow);

	renderWindow->Render();
	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
/*
 * 邻域连接法-重构
 */
void vtkImageSegmentCallback::seg_neighconnected(std::string filename, double pos_x, double pos_y, double pixel_f, double pixel_s)
{

	// Software Guide : EndCodeSnippet
	ConnectorType::Pointer connector = ConnectorType::New();
	CastingFilterType::Pointer caster = CastingFilterType::New();
	// We instantiate reader and writer types
	//
	ReaderType::Pointer reader = ReaderType::New();
	//WriterType::Pointer writer = WriterType::New();
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	reader->SetFileName(filename);
	reader->SetImageIO(gdcmIO);
	reader->Update();

	CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();

	typedef itk::NeighborhoodConnectedImageFilter<InternalImageType, InternalImageType > ConnectedFilterType;
	ConnectedFilterType::Pointer neighborhoodConnected = ConnectedFilterType::New();

	smoothing->SetInput(reader->GetOutput());
	neighborhoodConnected->SetInput(smoothing->GetOutput());
	caster->SetInput(neighborhoodConnected->GetOutput());
	connector->SetInput(caster->GetOutput());
	//writer->SetInput(caster->GetOutput());

	smoothing->SetNumberOfIterations(3);
	smoothing->SetTimeStep(0.125);

	const InternalPixelType lowerThreshold = pixel_f;
	const InternalPixelType upperThreshold = pixel_s;

	// Software Guide : BeginCodeSnippet
	neighborhoodConnected->SetLower(lowerThreshold);
	neighborhoodConnected->SetUpper(upperThreshold);

	InternalImageType::SizeType radius;

	radius[0] = 2;   // two pixels along X
	radius[1] = 2;   // two pixels along Y

	neighborhoodConnected->SetRadius(radius);

	InternalImageType::IndexType index;

	index[0] = pos_x;
	index[1] = pos_y;

	neighborhoodConnected->SetSeed(index);
	neighborhoodConnected->SetReplaceValue(255);

	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
#if VTK_MAJOR_VERSION <= 5
	actor->SetInput(connector->GetOutput());
#else
	connector->Update();
	actor->GetMapper()->SetInputData(connector->GetOutput());
#endif
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->ResetCamera();

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();

	qvtk->SetRenderWindow(renderWindow);

	renderWindow->Render();
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
	//try
	//{
	//	writer->SetFileName(argv[2]);
	//	writer->SetImageIO(gdcmIO);
	//	writer->UseInputMetaDataDictionaryOff();
	//	//connector->Update();
	//	writer->Update();
	//}
	//catch (itk::ExceptionObject & excep)
	//{
	//	std::cerr << "Exception caught !" << std::endl;
	//	std::cerr << excep << std::endl;
	//}
}
/*
 * 置信连接法-重构
 */
void vtkImageSegmentCallback::seg_confidconnected(std::string filename, double pos_x, double pos_y)
{
	// Software Guide : EndCodeSnippet
	ConnectorType::Pointer connector = ConnectorType::New();
	CastingFilterType::Pointer caster = CastingFilterType::New();

	// We instantiate reader and writer types
	ReaderType::Pointer reader = ReaderType::New();
	//WriterType::Pointer writer = WriterType::New();
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	reader->SetFileName(filename);
	reader->SetImageIO(gdcmIO);
	reader->Update();
	/*writer->SetFileName(argv[2]);
	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();*/


	CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();
	typedef itk::ConfidenceConnectedImageFilter<InternalImageType, InternalImageType> ConnectedFilterType;
	ConnectedFilterType::Pointer confidenceConnected = ConnectedFilterType::New();

	smoothing->SetInput(reader->GetOutput());
	confidenceConnected->SetInput(smoothing->GetOutput());
	caster->SetInput(confidenceConnected->GetOutput());
	//writer->SetInput(caster->GetOutput());
	connector->SetInput(caster->GetOutput());
	smoothing->SetNumberOfIterations(5);
	smoothing->SetTimeStep(0.125);

	confidenceConnected->SetMultiplier(2.5);

	confidenceConnected->SetNumberOfIterations(5);

	confidenceConnected->SetReplaceValue(255);

	InternalImageType::IndexType  index;

	index[0] = pos_x;
	index[1] = pos_y;

	confidenceConnected->SetSeed(index);

	confidenceConnected->SetInitialNeighborhoodRadius(2);

	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
#if VTK_MAJOR_VERSION <= 5
	actor->SetInput(connector->GetOutput());
#else
	connector->Update();
	actor->GetMapper()->SetInputData(connector->GetOutput());
#endif
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->ResetCamera();

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();

	qvtk->SetRenderWindow(renderWindow);

	renderWindow->Render();
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	renderWindowInteractor->Start();

	//try
	//{
	//	//connector->Update();
	//	writer->Update();
	//}
	//catch (itk::ExceptionObject & excep)
	//{
	//	std::cerr << "Exception caught !" << std::endl;
	//	std::cerr << excep << std::endl;
	//}
}
/*
 * 分水岭算法-重构
 */
void vtkImageSegmentCallback::seg_waterseg(std::string filename,double NumberOfIterations,
	double ConductanceParameter,double UsePrincipleComponents,double watershedLevel,double Threshold)
{
	typedef itk::RGBPixel< float >       RGBPixelType;
	typedef itk::Image< RGBPixelType, 2 >   DcmImageType;
	typedef itk::Vector< float, 3 >              VectorPixelType;
	typedef itk::Image< VectorPixelType, 2 >     VectorImageType;
	typedef itk::Image< itk::IdentifierType, 2 > LabeledImageType;
	typedef itk::Image< float, 2 >               ScalarImageType;
	typedef itk::ImageFileReader< DcmImageType >   FileReaderType;
	typedef itk::VectorCastImageFilter< DcmImageType, VectorImageType >CastFilterType;
	typedef itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType >DiffusionFilterType;
	typedef itk::VectorGradientMagnitudeImageFilter< VectorImageType > GradientMagnitudeFilterType;
	typedef itk::WatershedImageFilter< ScalarImageType >WatershedFilterType;
	typedef itk::ImageFileWriter<DcmImageType> FileWriterType;

	FileReaderType::Pointer reader = FileReaderType::New();
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	reader->SetFileName(filename);
	reader->SetImageIO(gdcmIO);
	reader->Update();

	CastFilterType::Pointer caster = CastFilterType::New();


	DiffusionFilterType::Pointer diffusion = DiffusionFilterType::New();
	diffusion->SetNumberOfIterations(NumberOfIterations);
	diffusion->SetConductanceParameter(ConductanceParameter);
	diffusion->SetTimeStep(0.125);

	GradientMagnitudeFilterType::Pointer gradient = GradientMagnitudeFilterType::New();
	gradient->SetUsePrincipleComponents(UsePrincipleComponents);

	WatershedFilterType::Pointer watershed = WatershedFilterType::New();
	watershed->SetLevel(watershedLevel);//watershedLevel
	watershed->SetThreshold(Threshold);


	// Software Guide : BeginCodeSnippet
	typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long>
		ColorMapFunctorType;
	typedef itk::UnaryFunctorImageFilter<LabeledImageType,
		DcmImageType, ColorMapFunctorType> ColorMapFilterType;
	ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
	// Software Guide : EndCodeSnippet


	FileWriterType::Pointer writer = FileWriterType::New();
	writer->SetFileName("WatershedSegmentation1Output1.dcm");
	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();
	typedef itk::ImageToVTKImageFilter<DcmImageType>   ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();

	caster->SetInput(reader->GetOutput());
	diffusion->SetInput(caster->GetOutput());
	gradient->SetInput(diffusion->GetOutput());
	watershed->SetInput(gradient->GetOutput());
	colormapper->SetInput(watershed->GetOutput());
	writer->SetInput(colormapper->GetOutput());
	connector->SetGlobalWarningDisplay(0);
	connector->GlobalWarningDisplayOff();
	connector->SetInput(colormapper->GetOutput());
	// Software Guide : EndCodeSnippet
	vtkSmartPointer<vtkImageActor> actor =
		vtkSmartPointer<vtkImageActor>::New();
	actor->SetGlobalWarningDisplay(0);
#if VTK_MAJOR_VERSION <= 5
	actor->SetInput(connector->GetOutput());
#else
	connector->Update();
	actor->GetMapper()->SetInputData(connector->GetOutput());
#endif
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->ResetCamera();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();
	renderWindowInteractor->SetGlobalWarningDisplay(0);
	qvtk->SetRenderWindow(renderWindow);
	renderWindow->SetGlobalWarningDisplay(0);
	renderWindow->Render();
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	renderWindowInteractor->Start();
	try
	{
		connector->Update();
		writer->Update();
	}
	catch (itk::ExceptionObject &e)
	{
		std::cerr << e << std::endl;
		//return EXIT_FAILURE;
	}
}
/*
 * 快速匹配-重构
 */
void vtkImageSegmentCallback::seg_fastmarching(std::string filename,double pos_x,double pos_y,
	          double alpha,double beta,double sigma,double UpperThreshold,double StoppingValue)
{
	ConnectorType::Pointer connector[5];
	connector[0]->GlobalWarningDisplayOff();
	connector[1]->GlobalWarningDisplayOff();
	connector[2]->GlobalWarningDisplayOff();
	connector[3]->GlobalWarningDisplayOff();
	connector[4]->GlobalWarningDisplayOff();

	connector[0] = ConnectorType::New();
	connector[1] = ConnectorType::New();
	connector[2] = ConnectorType::New();
	connector[3] = ConnectorType::New();
	connector[4] = ConnectorType::New();

	typedef itk::BinaryThresholdImageFilter< InternalImageType, OutputImageType    >    ThresholdingFilterType;
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();

	thresholder->SetLowerThreshold(0.0);
	thresholder->SetUpperThreshold(UpperThreshold);

	thresholder->SetOutsideValue(0);
	thresholder->SetInsideValue(255);

	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	reader->SetFileName(filename);
	reader->SetImageIO(gdcmIO);
	reader->Update();

	typedef itk::RescaleIntensityImageFilter<InternalImageType, OutputImageType >   CastFilterType;
	typedef itk::CurvatureAnisotropicDiffusionImageFilter<InternalImageType, InternalImageType >  SmoothingFilterType;
	SmoothingFilterType::Pointer smoothing = SmoothingFilterType::New();
	typedef   itk::GradientMagnitudeRecursiveGaussianImageFilter<InternalImageType, InternalImageType >  GradientFilterType;
	typedef   itk::SigmoidImageFilter<InternalImageType, InternalImageType >  SigmoidFilterType;

	GradientFilterType::Pointer  gradientMagnitude = GradientFilterType::New();
	SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();

	sigmoid->SetOutputMinimum(0.0);
	sigmoid->SetOutputMaximum(1.0);

	typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType >    FastMarchingFilterType;

	FastMarchingFilterType::Pointer  fastMarching= FastMarchingFilterType::New();

	smoothing->SetInput(reader->GetOutput());
	gradientMagnitude->SetInput(smoothing->GetOutput());
	sigmoid->SetInput(gradientMagnitude->GetOutput());
	fastMarching->SetInput(sigmoid->GetOutput());
	thresholder->SetInput(fastMarching->GetOutput());

	connector[0]->SetInput(thresholder->GetOutput());

	smoothing->SetTimeStep(0.125);
	smoothing->SetNumberOfIterations(5);
	smoothing->SetConductanceParameter(9.0);

	gradientMagnitude->SetSigma(sigma);

	sigmoid->SetAlpha(alpha);
	sigmoid->SetBeta(beta);

	typedef FastMarchingFilterType::NodeContainer           NodeContainer;
	typedef FastMarchingFilterType::NodeType                NodeType;
	NodeContainer::Pointer seeds = NodeContainer::New();

	InternalImageType::IndexType  seedPosition;

	seedPosition[0] = pos_x;
	seedPosition[1] = pos_y;

	NodeType node;
	const double seedValue = 0.0;

	node.SetValue(seedValue);
	node.SetIndex(seedPosition);

	seeds->Initialize();
	seeds->InsertElement(0, node);

	fastMarching->SetTrialPoints(seeds);

	CastFilterType::Pointer caster1 = CastFilterType::New();
	caster1->SetInput(smoothing->GetOutput());
	connector[1]->SetInput(caster1->GetOutput());
	caster1->SetOutputMinimum(0);
	caster1->SetOutputMaximum(255);

	CastFilterType::Pointer caster2 = CastFilterType::New();
	caster2->SetInput(gradientMagnitude->GetOutput());
	connector[2]->SetInput(caster2->GetOutput());
	caster2->SetOutputMinimum(0);
	caster2->SetOutputMaximum(255);

	CastFilterType::Pointer caster3 = CastFilterType::New();
	caster3->SetInput(sigmoid->GetOutput());
	connector[3]->SetInput(caster3->GetOutput());
	caster3->SetOutputMinimum(0);
	caster3->SetOutputMaximum(255);


	fastMarching->SetOutputSize(reader->GetOutput()->GetBufferedRegion().GetSize());
	fastMarching->SetStoppingValue(StoppingValue);



	CastFilterType::Pointer caster4 = CastFilterType::New();
	caster4->SetInput(fastMarching->GetOutput());
	connector[4]->SetInput(caster4->GetOutput());
	caster4->SetOutputMinimum(0);
	caster4->SetOutputMaximum(255);

	vtkSmartPointer<vtkImageActor> actor[5];
	actor[0] = vtkSmartPointer<vtkImageActor>::New();
	actor[1] = vtkSmartPointer<vtkImageActor>::New();
	actor[2] = vtkSmartPointer<vtkImageActor>::New();
	actor[3] = vtkSmartPointer<vtkImageActor>::New();
	actor[4] = vtkSmartPointer<vtkImageActor>::New();

	connector[0]->Update();
	connector[1]->Update();
	connector[2]->Update();
	connector[3]->Update();
	connector[4]->Update();

	actor[0]->GetMapper()->SetInputData(connector[0]->GetOutput());
	actor[1]->GetMapper()->SetInputData(connector[1]->GetOutput());
	actor[2]->GetMapper()->SetInputData(connector[2]->GetOutput());
	actor[3]->GetMapper()->SetInputData(connector[3]->GetOutput());
	actor[4]->GetMapper()->SetInputData(connector[4]->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer[4];
	renderer[0] = vtkSmartPointer<vtkRenderer>::New();
	renderer[1] = vtkSmartPointer<vtkRenderer>::New();
	renderer[2] = vtkSmartPointer<vtkRenderer>::New();
	renderer[3] = vtkSmartPointer<vtkRenderer>::New();

	renderer[0]->AddActor(actor[0]);
	renderer[1]->AddActor(actor[1]);
	renderer[2]->AddActor(actor[2]);
	renderer[3]->AddActor(actor[3]);

	renderer[0]->SetViewport(0.0, 0.0, 0.5, 0.5);
	renderer[1]->SetViewport(0.5, 0.0, 1.0, 0.5);
	renderer[2]->SetViewport(0.0, 0.5, 0.5, 1.0);
	renderer[3]->SetViewport(0.5, 0.5, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer[0]);
	renderWindow->AddRenderer(renderer[1]);
	renderWindow->AddRenderer(renderer[2]);
	renderWindow->AddRenderer(renderer[3]);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();

	qvtk->SetRenderWindow(renderWindow);
	renderWindow->Render();
	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
 /*
 * 形状检测-重构
 */
void vtkImageSegmentCallback::seg_shapedectection(std::string filename,double pos_x,double pos_y,
	double seedValue,double alpha,double beta, double sigma,double curvatureScaling,double propagationScaling)
{
	typedef itk::BinaryThresholdImageFilter< InternalImageType, OutputImageType >ThresholdingFilterType;
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
	thresholder->SetLowerThreshold(-1000.0);
	thresholder->SetUpperThreshold(0.0);

	thresholder->SetOutsideValue(0);
	thresholder->SetInsideValue(255);

	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	reader->SetFileName(filename);
	reader->SetImageIO(gdcmIO);
	reader->Update();

	typedef itk::RescaleIntensityImageFilter<InternalImageType, OutputImageType>CastFilterType;
	typedef itk::CurvatureAnisotropicDiffusionImageFilter<InternalImageType, InternalImageType >  SmoothingFilterType;

	SmoothingFilterType::Pointer smoothing = SmoothingFilterType::New();

	typedef   itk::GradientMagnitudeRecursiveGaussianImageFilter<InternalImageType, InternalImageType >  GradientFilterType;
	typedef   itk::SigmoidImageFilter<InternalImageType, InternalImageType >  SigmoidFilterType;

	GradientFilterType::Pointer  gradientMagnitude = GradientFilterType::New();
	SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();

	sigmoid->SetOutputMinimum(0.0);
	sigmoid->SetOutputMaximum(1.0);

	typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType >FastMarchingFilterType;

	FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();

	typedef  itk::ShapeDetectionLevelSetImageFilter< InternalImageType, InternalImageType >    ShapeDetectionFilterType;
	ShapeDetectionFilterType::Pointer shapeDetection = ShapeDetectionFilterType::New();
	smoothing->SetInput(reader->GetOutput());
	gradientMagnitude->SetInput(smoothing->GetOutput());
	sigmoid->SetInput(gradientMagnitude->GetOutput());

	shapeDetection->SetInput(fastMarching->GetOutput());
	shapeDetection->SetFeatureImage(sigmoid->GetOutput());

	thresholder->SetInput(shapeDetection->GetOutput());

	smoothing->SetTimeStep(0.125);
	smoothing->SetNumberOfIterations(5);
	smoothing->SetConductanceParameter(9.0);

	gradientMagnitude->SetSigma(sigma);
	sigmoid->SetAlpha(alpha);
	sigmoid->SetBeta(beta);

	typedef FastMarchingFilterType::NodeContainer           NodeContainer;
	typedef FastMarchingFilterType::NodeType                NodeType;
	NodeContainer::Pointer seeds = NodeContainer::New();


	InternalImageType::IndexType  seedPosition;
	seedPosition[0] = pos_x;
	seedPosition[1] = pos_y;
	NodeType node;
	node.SetValue(-seedValue);
	node.SetIndex(seedPosition);

	seeds->Initialize();
	seeds->InsertElement(0, node);

	fastMarching->SetTrialPoints(seeds);

	fastMarching->SetSpeedConstant(1.0);

	//
	ConnectorType::Pointer connector[5];
	connector[0]->GlobalWarningDisplayOff();
	connector[1]->GlobalWarningDisplayOff();
	connector[2]->GlobalWarningDisplayOff();
	connector[3]->GlobalWarningDisplayOff();
	connector[4]->GlobalWarningDisplayOff();

	connector[0] = ConnectorType::New();
	connector[1] = ConnectorType::New();
	connector[2] = ConnectorType::New();
	connector[3] = ConnectorType::New();
	connector[4] = ConnectorType::New();

	vtkSmartPointer<vtkImageActor> actor[5];
	actor[0] = vtkSmartPointer<vtkImageActor>::New();
	actor[1] = vtkSmartPointer<vtkImageActor>::New();
	actor[2] = vtkSmartPointer<vtkImageActor>::New();
	actor[3] = vtkSmartPointer<vtkImageActor>::New();
	actor[4] = vtkSmartPointer<vtkImageActor>::New();

	CastFilterType::Pointer caster1 = CastFilterType::New();
	CastFilterType::Pointer caster2 = CastFilterType::New();
	CastFilterType::Pointer caster3 = CastFilterType::New();
	CastFilterType::Pointer caster4 = CastFilterType::New();

	caster1->SetInput(smoothing->GetOutput());
	caster1->SetOutputMinimum(0);
	caster1->SetOutputMaximum(255);
	
	caster2->SetInput(gradientMagnitude->GetOutput());
	caster2->SetOutputMinimum(0);
	caster2->SetOutputMaximum(255);
	
	caster3->SetInput(sigmoid->GetOutput());
	caster3->SetOutputMinimum(0);
	caster3->SetOutputMaximum(255);

	caster4->SetInput(fastMarching->GetOutput());
	caster4->SetOutputMinimum(0);
	caster4->SetOutputMaximum(255);

	fastMarching->SetOutputSize(reader->GetOutput()->GetBufferedRegion().GetSize());

	//  Software Guide : BeginCodeSnippet
	shapeDetection->SetPropagationScaling(propagationScaling);
	shapeDetection->SetCurvatureScaling(curvatureScaling);

	shapeDetection->SetMaximumRMSError(0.02);
	shapeDetection->SetNumberOfIterations(800);

	connector[1]->SetInput(caster1->GetOutput());
	connector[2]->SetInput(caster2->GetOutput());
	connector[3]->SetInput(caster3->GetOutput());
	connector[4]->SetInput(thresholder->GetOutput());

	connector[1]->Update();
	connector[2]->Update();
	connector[3]->Update();
	connector[4]->Update();

	actor[1]->GetMapper()->SetInputData(connector[1]->GetOutput());
	actor[2]->GetMapper()->SetInputData(connector[2]->GetOutput());
	actor[3]->GetMapper()->SetInputData(connector[3]->GetOutput());
	actor[4]->GetMapper()->SetInputData(connector[4]->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer[5];
	renderer[1] = vtkSmartPointer<vtkRenderer>::New();
	renderer[2] = vtkSmartPointer<vtkRenderer>::New();
	renderer[3] = vtkSmartPointer<vtkRenderer>::New();
	renderer[4] = vtkSmartPointer<vtkRenderer>::New();

	renderer[1]->AddActor(actor[1]);
	renderer[2]->AddActor(actor[2]);
	renderer[3]->AddActor(actor[3]);
	renderer[4]->AddActor(actor[4]);
	
	renderer[1]->SetViewport(0.0, 0.0, 0.5, 0.5);
	renderer[2]->SetViewport(0.5, 0.0, 1.0, 0.5);
	renderer[3]->SetViewport(0.0, 0.5, 0.5, 1.0);
	renderer[4]->SetViewport(0.5, 0.5, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer[1]);
	renderWindow->AddRenderer(renderer[2]);
	renderWindow->AddRenderer(renderer[3]);
	renderWindow->AddRenderer(renderer[4]);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();

	qvtk->SetRenderWindow(renderWindow);
	renderWindow->Render();
	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
//====================================================================================

/*
 * 对不同分割算法的分发
 */
void vtkImageSegmentCallback::Execute(vtkObject *, unsigned long event, void *)
{
	MyStyle *mystyle = new MyStyle;
	vtkRenderWindowInteractor *interactor = this->Viewer->GetRenderWindow()->GetInteractor();
	vtkRenderer* renderer = this->Viewer->GetRenderer();
	vtkImageActor* actor = this->Viewer->GetImageActor();
	vtkImageData* image = this->Viewer->GetInput();
	vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());

	// Pick at the mouse location provided by the interactor
	// 鼠标位置
	this->Picker->Pick(interactor->GetEventPosition()[0],
					   interactor->GetEventPosition()[1],0.0, renderer);

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
	vtkCell* cell = image->FindAndGetCell(pos, NULL, -1, tol2, subId, pcoords, weights);
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
	if (event == vtkCommand::LeftButtonPressEvent)//鼠标左键触发之后再分发到具体的算法
	{
		double pos_x = pos[0];
		double pos_y = pos[1];

		double* pixel = this->PointData->GetScalars()->GetTuple(0);
		double pixel_f = pixel[0] - 600;
		double pixel_s = pixel[0] + 600;

		SegmentFunc seg_enum;
		seg_enum = (SegmentFunc)this->count;
		switch (seg_enum)
		{
		case  NULL_Seg:
			break;
		case  Seg_connectedthres://ok
			seg_connectedthres("F:/Dicom/Test1/DICOM/S427870/S20/I10", pos_x, pos_y, pixel_f, pixel_s);
			break;
		case  Seg_neighconnected://ok
			seg_neighconnected("F:/Dicom/Test1/DICOM/S427870/S20/I10", pos_x, pos_y, pixel_f, pixel_s);
			break;
		case  Seg_confidconnected://ok
			seg_confidconnected("F:/Dicom/Test1/DICOM/S427870/S20/I10", pos_x, pos_y);
			break;
		case  Seg_waterseg:
			seg_waterseg("F:/Dicom/Test1/DICOM/S427870/S20/I10", 10, 2, 1, 0.05, 0);
			break;
		case  Seg_fastmarching://ok
			seg_fastmarching("F:/Dicom/Test1/DICOM/S427870/S20/I10", pos_x, pos_y,-0.5, 3.0, 1.0, 100.0, 100.0);
			break;
		case  Seg_shapedectection://ok
			seg_shapedectection("F:/Dicom/Test1/DICOM/S427870/S20/I10", pos_x, pos_y,5, -0.5, 3.0, 1.0, 0.05, 1.0);
			break;
		default:
			break;
		}
	}
}
