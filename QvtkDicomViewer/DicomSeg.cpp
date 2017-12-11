#include <head_all.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkPNGReader.h>
#include <string>  

#include <vtkDICOMImageReader.h>
#include "itkGDCMImageIO.h"  
#include <vtkDICOMImageReader.h>
/*
 *1.确认这个类是干什么的,是否有没被调用的函数,如果有,考虑先做出标识,在未来的版本中考虑删除
 *2.初步判断这个类原来是一个控制台程序的一部分代码扒下来的,关于命令行处理的代码是荣冗余的,需要去掉
 *
 *
 */
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySlice.png}
//    OUTPUTS: {ConnectedThresholdOutput1.png}
//    ARGUMENTS:    60 116 150 180
//  Software Guide : EndCommandLineArgs
void seg_connectedthres(int argc, char * argv[],QVTKWidget* qvtk)
{
	//std::cerr << "Missing Parameters " << std::endl;
	//std::cerr << "Usage: " << argv[0] << std::endl;
	//std::cerr << " inputImage  outputImage seedX seedY lowerThreshold upperThreshold" << std::endl;
	//return EXIT_FAILURE;
	typedef float           InternalPixelType;
	const   unsigned int    Dimension = 2;
	typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
	
	typedef float                                    OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

	typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();

	typedef itk::CastImageFilter< InternalImageType, OutputImageType >
		CastingFilterType;
	CastingFilterType::Pointer caster = CastingFilterType::New();

	typedef  itk::ImageFileReader< InternalImageType > ReaderType;
	typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;

	ReaderType::Pointer reader = ReaderType::New();
	WriterType::Pointer writer = WriterType::New();

	//argv[1] = "C://Users//bao//Desktop//test-image//BrainProtonDensitySlice.png";
	//argv[2] = "C://Users//bao//Desktop//test-image//ConnectedThresholdOutput1.png";
	//argv[3] = "60";
	//argv[4] = "116";
	//argv[5] = "150";
	//argv[6] = "180";
	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	reader->SetFileName(argv[1]);
	reader->SetImageIO(gdcmIO);
	reader->Update();
	writer->SetFileName(argv[2]);
	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();

	typedef itk::CurvatureFlowImageFilter< InternalImageType, InternalImageType >
		CurvatureFlowImageFilterType;

	CurvatureFlowImageFilterType::Pointer smoothing =
		CurvatureFlowImageFilterType::New();

	typedef itk::ConnectedThresholdImageFilter< InternalImageType,
		InternalImageType > ConnectedFilterType;

	ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();

	smoothing->SetInput(reader->GetOutput());
	connectedThreshold->SetInput(smoothing->GetOutput());
	caster->SetInput(connectedThreshold->GetOutput());
	writer->SetInput(caster->GetOutput());
	connector->SetInput(caster->GetOutput());
	smoothing->SetNumberOfIterations(5);
	smoothing->SetTimeStep(0.125);



	const InternalPixelType lowerThreshold = atof(argv[5]);
	const InternalPixelType upperThreshold = atof(argv[6]);

	// Software Guide : BeginCodeSnippet
	connectedThreshold->SetLower(lowerThreshold);
	connectedThreshold->SetUpper(upperThreshold);

	connectedThreshold->SetReplaceValue(255);

	InternalImageType::IndexType  index;

	//    ARGUMENTS:    60 116 150 180
	index[0] = atoi(argv[3]);
	index[1] = atoi(argv[4]);

	// Software Guide : BeginCodeSnippet
	connectedThreshold->SetSeed(index);

	try
	{
		writer->Update();
	}

	catch (itk::ExceptionObject & excep)
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr << excep << std::endl;
	}
	vtkSmartPointer<vtkImageActor> actor =
		vtkSmartPointer<vtkImageActor>::New();
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
	renderer->SetBackground(0.0,0.0,0.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();
	qvtk->SetRenderWindow(renderWindow);
	
	renderWindow->Render();

	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	renderWindowInteractor->Start();

}
//  Software Guide : BeginCommandLineArgs
//    INPUTS: {BrainProtonDensitySlice.png}
//    ARGUMENTS:   OtsuMultipleThresholdsOutput png 4
//  Software Guide : EndCommandLineArgs
void seg_ostu(int argc, char * argv[], QVTKWidget* qvtk)
{

	//argv[1] = "C://Users//bao//Desktop//test-image//BrainProtonDensitySlice.png";
	//argv[2] = "C://Users//bao//Desktop//test-image//OtsuMultipleThresholdsOutput";
	//argv[3] = "png";
	//argv[4] = "4";

	typedef  float  InputPixelType;
	typedef  float  OutputPixelType;

	typedef itk::Image< InputPixelType, 2 >   InputImageType;
	typedef itk::Image< OutputPixelType, 2 >  OutputImageType;

	typedef itk::Statistics::ScalarImageToHistogramGenerator<
		InputImageType > ScalarImageToHistogramGeneratorType;

	typedef ScalarImageToHistogramGeneratorType::HistogramType HistogramType;

	typedef itk::OtsuMultipleThresholdsCalculator< HistogramType >
		CalculatorType;
	// Software Guide : EndCodeSnippet

	typedef itk::ImageFileReader< InputImageType >  ReaderType;
	typedef itk::ImageFileWriter< OutputImageType > WriterType;

	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();

	typedef itk::BinaryThresholdImageFilter<
		InputImageType, OutputImageType >  FilterType;

	ScalarImageToHistogramGeneratorType::Pointer scalarImageToHistogramGenerator
		= ScalarImageToHistogramGeneratorType::New();

	CalculatorType::Pointer calculator = CalculatorType::New();
	FilterType::Pointer filter = FilterType::New();
	// Software Guide : EndCodeSnippet

	ReaderType::Pointer reader = ReaderType::New();
	WriterType::Pointer writer = WriterType::New();
	scalarImageToHistogramGenerator->SetNumberOfBins(128);
	calculator->SetNumberOfThresholds(atoi(argv[4]));
	// Software Guide : EndCodeSnippet

	const OutputPixelType outsideValue = 0;
	const OutputPixelType insideValue = 255;

	filter->SetOutsideValue(outsideValue);
	filter->SetInsideValue(insideValue);

	//Connect Pipeline
	reader->SetFileName(argv[1]);
	reader->SetImageIO(gdcmIO);

	scalarImageToHistogramGenerator->SetInput(reader->GetOutput());
	calculator->SetInputHistogram(
		scalarImageToHistogramGenerator->GetOutput());
	filter->SetInput(reader->GetOutput());
	connector->SetInput(filter->GetOutput());
	writer->SetInput(filter->GetOutput());
	// Software Guide : EndCodeSnippet

	//Invoke pipeline
	try
	{
		reader->Update();
	}
	catch (itk::ExceptionObject & excp)
	{
		std::cerr << "Exception thrown while reading image" << excp << std::endl;
	}
	scalarImageToHistogramGenerator->Compute();

	try
	{
		calculator->Compute();
	}
	catch (itk::ExceptionObject & excp)
	{
		std::cerr << "Exception thrown " << excp << std::endl;
	}

	const CalculatorType::OutputType &thresholdVector = calculator->GetOutput();

	std::string outputFileBase = argv[2];

	InputPixelType lowerThreshold = itk::NumericTraits<InputPixelType>::min();
	InputPixelType upperThreshold;

	// Software Guide : BeginCodeSnippet
	typedef CalculatorType::OutputType::const_iterator ThresholdItType;

	vtkSmartPointer<vtkDICOMImageReader> dcmreader[4];
	vtkSmartPointer<vtkRenderer>  renderer[4];
	vtkSmartPointer<vtkImageActor> actor[4];

	vtkSmartPointer<vtkRenderWindow> renWin
		= vtkSmartPointer<vtkRenderWindow>::New();

	int actor_count = 0;
	for (ThresholdItType itNum = thresholdVector.begin();
		itNum != thresholdVector.end();
		++itNum)
	{
		renderer[actor_count] = vtkSmartPointer<vtkRenderer>::New();
		dcmreader[actor_count] = vtkSmartPointer<vtkDICOMImageReader>::New();
		actor[actor_count] = vtkSmartPointer<vtkImageActor>::New();

		std::cout << "OtsuThreshold["
			<< (int)(itNum - thresholdVector.begin())
			<< "] = "
			<< static_cast<itk::NumericTraits<
			CalculatorType::MeasurementType>::PrintType>(*itNum)
			<< std::endl;
		// Software Guide : EndCodeSnippet

		upperThreshold = static_cast<InputPixelType>(*itNum);

		filter->SetLowerThreshold(lowerThreshold);
		filter->SetUpperThreshold(upperThreshold);

		lowerThreshold = upperThreshold;

		writer->SetImageIO(gdcmIO);
		writer->UseInputMetaDataDictionaryOff();

		std::ostringstream outputFilename;
		outputFilename << outputFileBase
			<< std::setfill('0') << std::setw(3) << (itNum - thresholdVector.begin())
			<< "."
			<< argv[3];
		writer->SetFileName(outputFilename.str());
		try
		{
			writer->Update();
		}
		catch (itk::ExceptionObject & excp)
		{
			std::cerr << "Exception thrown " << excp << std::endl;
		}
		dcmreader[actor_count]->SetFileName(outputFilename.str().data());
		dcmreader[actor_count]->GlobalWarningDisplayOff();
		dcmreader[actor_count]->Update();
		actor[actor_count]->GetMapper()->SetInputData(dcmreader[actor_count]->GetOutput());
		//actor[actor_count]->SetMapper(pngreader[actor_count]->GetOutput());

		renderer[actor_count]->AddActor(actor[actor_count]);


		renderer[actor_count]->SetBackground(0.0, 0.0, 0.0);
		if (actor_count == 0)
		{
			renderer[actor_count]->SetViewport(0.0, 0.0, 0.5, 0.5);
		}
		if (actor_count == 1)
		{
			renderer[actor_count]->SetViewport(0.5, 0.0, 1.0, 0.5);
		}
		if (actor_count == 2)
		{
			renderer[actor_count]->SetViewport(0.0, 0.5, 0.5, 1.0);
		}
		if (actor_count == 3)
		{
			renderer[actor_count]->SetViewport(0.5, 0.5, 1.0, 1.0);
		}
		
		renWin->AddRenderer(renderer[actor_count]);
		//renderer[actor_count]->Render();
		++actor_count;
	}
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	qvtk->SetRenderWindow(renWin);
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renWin);
	renderWindowInteractor->Initialize();
	renWin->Render();
	renderWindowInteractor->Start();

	upperThreshold = itk::NumericTraits<InputPixelType>::max();
	filter->SetLowerThreshold(lowerThreshold);
	filter->SetUpperThreshold(upperThreshold);

	std::ostringstream outputFilename2;
	outputFilename2 << outputFileBase
		<< std::setfill('0') << std::setw(3) << thresholdVector.size()
		<< "."
		<< argv[3];
	writer->SetFileName(outputFilename2.str());

	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject & excp)
	{
		std::cerr << "Exception thrown " << excp << std::endl;
	}
}
void seg_neighconnected(int argc, char * argv[], QVTKWidget* qvtk)
{
	if (argc < 7)
	{
		std::cerr << "Missing Parameters." << std::endl;
		std::cerr << "Usage: " << argv[0]
			<< " inputImage outputImage"
			<< " seedX seedY"
			<< " lowerThreshold upperThreshold" << std::endl;
	}

	typedef   float           InternalPixelType;
	const     unsigned int    Dimension = 2;
	typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
	// Software Guide : EndCodeSnippet


	typedef float                                    OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

	typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();

	typedef itk::CastImageFilter< InternalImageType, OutputImageType >
		CastingFilterType;
	CastingFilterType::Pointer caster = CastingFilterType::New();


	// We instantiate reader and writer types
	//
	typedef  itk::ImageFileReader< InternalImageType > ReaderType;
	typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;

	ReaderType::Pointer reader = ReaderType::New();
	WriterType::Pointer writer = WriterType::New();


	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	reader->SetFileName(argv[1]);
	reader->SetImageIO(gdcmIO);
	reader->Update();

	typedef itk::CurvatureFlowImageFilter<InternalImageType, InternalImageType>
		CurvatureFlowImageFilterType;

	CurvatureFlowImageFilterType::Pointer smoothing =
		CurvatureFlowImageFilterType::New();

	typedef itk::NeighborhoodConnectedImageFilter<InternalImageType,
		InternalImageType > ConnectedFilterType;

	ConnectedFilterType::Pointer neighborhoodConnected
		= ConnectedFilterType::New();

	smoothing->SetInput(reader->GetOutput());
	neighborhoodConnected->SetInput(smoothing->GetOutput());
	caster->SetInput(neighborhoodConnected->GetOutput());
	connector->SetInput(caster->GetOutput());
	writer->SetInput(caster->GetOutput());

	smoothing->SetNumberOfIterations(3);
	smoothing->SetTimeStep(0.125);


	const InternalPixelType lowerThreshold = atof(argv[5]);
	const InternalPixelType upperThreshold = atof(argv[6]);

	// Software Guide : BeginCodeSnippet
	neighborhoodConnected->SetLower(lowerThreshold);
	neighborhoodConnected->SetUpper(upperThreshold);

	InternalImageType::SizeType radius;

	radius[0] = 2;   // two pixels along X
	radius[1] = 2;   // two pixels along Y

	neighborhoodConnected->SetRadius(radius);

	InternalImageType::IndexType index;

	index[0] = atoi(argv[3]);
	index[1] = atoi(argv[4]);

	neighborhoodConnected->SetSeed(index);
	neighborhoodConnected->SetReplaceValue(255);

	vtkSmartPointer<vtkImageActor> actor =
		vtkSmartPointer<vtkImageActor>::New();
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

	qvtk->SetRenderWindow(renderWindow);

	renderWindow->Render();
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
	try
	{
		writer->SetFileName(argv[2]);
		writer->SetImageIO(gdcmIO);
		writer->UseInputMetaDataDictionaryOff();
		//connector->Update();
		writer->Update();
	}
	catch (itk::ExceptionObject & excep)
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr << excep << std::endl;
	}
}
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySlice.png}
//    OUTPUTS: {ConfidenceConnectedOutput1.png}
//    ARGUMENTS:    60 116
//  Software Guide : EndCommandLineArgs
void seg_confidconnected(int argc, char * argv[], QVTKWidget* qvtk)
{

	if (argc < 5)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0];
		std::cerr << " inputImage  outputImage seedX seedY " << std::endl;
	}

	typedef   float           InternalPixelType;
	const     unsigned int    Dimension = 2;
	typedef itk::Image< InternalPixelType, Dimension > InternalImageType;
	// Software Guide : EndCodeSnippet

	typedef float                            OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

	typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();

	typedef itk::CastImageFilter< InternalImageType, OutputImageType >
		CastingFilterType;
	CastingFilterType::Pointer caster = CastingFilterType::New();


	// We instantiate reader and writer types
	//
	typedef itk::ImageFileReader< InternalImageType > ReaderType;
	typedef itk::ImageFileWriter< OutputImageType >   WriterType;

	ReaderType::Pointer reader = ReaderType::New();
	WriterType::Pointer writer = WriterType::New();

	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	reader->SetFileName(argv[1]);
	reader->SetImageIO(gdcmIO);
	reader->Update();
	writer->SetFileName(argv[2]);
	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();

	typedef itk::CurvatureFlowImageFilter< InternalImageType, InternalImageType >
		CurvatureFlowImageFilterType;

	CurvatureFlowImageFilterType::Pointer smoothing =
		CurvatureFlowImageFilterType::New();

	typedef itk::ConfidenceConnectedImageFilter<
		InternalImageType, InternalImageType> ConnectedFilterType;

	ConnectedFilterType::Pointer confidenceConnected
		= ConnectedFilterType::New();

	smoothing->SetInput(reader->GetOutput());
	confidenceConnected->SetInput(smoothing->GetOutput());
	caster->SetInput(confidenceConnected->GetOutput());
	writer->SetInput(caster->GetOutput());
	connector->SetInput(caster->GetOutput());
	smoothing->SetNumberOfIterations(5);
	smoothing->SetTimeStep(0.125);

	confidenceConnected->SetMultiplier(2.5);

	confidenceConnected->SetNumberOfIterations(5);

	confidenceConnected->SetReplaceValue(255);

	InternalImageType::IndexType  index;

	index[0] = atoi(argv[3]);
	index[1] = atoi(argv[4]);

	confidenceConnected->SetSeed(index);

	confidenceConnected->SetInitialNeighborhoodRadius(2);

	vtkSmartPointer<vtkImageActor> actor =
		vtkSmartPointer<vtkImageActor>::New();
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

	qvtk->SetRenderWindow(renderWindow);

	renderWindow->Render();
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	renderWindowInteractor->Start();

	try
	{
		//connector->Update();
		writer->Update();
	}
	catch (itk::ExceptionObject & excep)
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr << excep << std::endl;
	}
}
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {VisibleWomanEyeSlice.png}
//    OUTPUTS: {WatershedSegmentation1Output1.png}
//    ARGUMENTS:    2 10 0 0.05 1
//  Software Guide : EndCommandLineArgs
void seg_waterseg(int argc, char *argv[], QVTKWidget* qvtk)
{
	if (argc < 8)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0];
		std::cerr << " inputImage outputImage conductanceTerm diffusionIterations lowerThreshold outputScaleLevel gradientMode " << std::endl;
	}

	//typedef   float           InternalPixelType;
	typedef itk::RGBPixel< float >       RGBPixelType;
	typedef itk::Image< RGBPixelType, 2 >   DcmImageType;
	typedef itk::Vector< float, 3 >              VectorPixelType;
	typedef itk::Image< VectorPixelType, 2 >     VectorImageType;
	typedef itk::Image< itk::IdentifierType, 2 > LabeledImageType;
	typedef itk::Image< float, 2 >               ScalarImageType;

	typedef itk::ImageFileReader< DcmImageType >   FileReaderType;
	typedef itk::VectorCastImageFilter< DcmImageType, VectorImageType >
		CastFilterType;
	typedef itk::VectorGradientAnisotropicDiffusionImageFilter<
		VectorImageType, VectorImageType >
		DiffusionFilterType;
	typedef itk::VectorGradientMagnitudeImageFilter< VectorImageType >
		GradientMagnitudeFilterType;
	typedef itk::WatershedImageFilter< ScalarImageType >
		WatershedFilterType;
	// Software Guide : EndCodeSnippet

	typedef itk::ImageFileWriter<DcmImageType> FileWriterType;

	FileReaderType::Pointer reader = FileReaderType::New();
	
	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	reader->SetFileName(argv[1]);
	reader->SetImageIO(gdcmIO);
	reader->Update();

	CastFilterType::Pointer caster = CastFilterType::New();


	DiffusionFilterType::Pointer diffusion = DiffusionFilterType::New();
	diffusion->SetNumberOfIterations(atoi(argv[4]));
	diffusion->SetConductanceParameter(atof(argv[3]));
	diffusion->SetTimeStep(0.125);

	GradientMagnitudeFilterType::Pointer
		gradient = GradientMagnitudeFilterType::New();
	gradient->SetUsePrincipleComponents(atoi(argv[7]));

	WatershedFilterType::Pointer watershed = WatershedFilterType::New();
	watershed->SetLevel(atof(argv[6]));
	watershed->SetThreshold(atof(argv[5]));


	// Software Guide : BeginCodeSnippet
	typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long>
		ColorMapFunctorType;
	typedef itk::UnaryFunctorImageFilter<LabeledImageType,
		DcmImageType, ColorMapFunctorType> ColorMapFilterType;
	ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
	// Software Guide : EndCodeSnippet


	FileWriterType::Pointer writer = FileWriterType::New();
	writer->SetFileName(argv[2]);
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
static void PrintCommandLineUsage(const int argc, const char * const argv[])
{
	std::cerr << "Missing Parameters " << std::endl;
	std::cerr << "Usage: " << argv[0];
	std::cerr << " inputImage  outputImage seedX seedY";
	std::cerr << " Sigma SigmoidAlpha SigmoidBeta TimeThreshold StoppingValue";
	std::cerr << " smoothingOutputImage gradientMagnitudeOutputImage sigmoidOutputImage" << std::endl;

	for (int qq = 0; qq< argc; ++qq)
	{
		std::cout << "argv[" << qq << "] = " << argv[qq] << std::endl;
	}
}
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySlice.png}
//    OUTPUTS: {FastMarchingImageFilterOutput5.png}
//    ARGUMENTS:    81 114 1.0  -0.5  3.0   100 100
//    OUTPUTS: {FastMarchingFilterOutput1.png}
//    OUTPUTS: {FastMarchingFilterOutput2.png}
//    OUTPUTS: {FastMarchingFilterOutput3.png}
//  Software Guide : EndCommandLineArgs
void seg_fastmarching(int argc, char * argv[], QVTKWidget* qvtk)
{

	if (argc != 13)
	{
		PrintCommandLineUsage(argc, argv);
		//return EXIT_FAILURE;
	}
	typedef float           InternalPixelType;
	const   unsigned int    Dimension = 2;
	typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;

	typedef float                            OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

	typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
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

	typedef itk::BinaryThresholdImageFilter< InternalImageType,
		OutputImageType    >    ThresholdingFilterType;
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();


	const InternalPixelType  timeThreshold = atof(argv[8]);

	// Software Guide : BeginCodeSnippet
	thresholder->SetLowerThreshold(0.0);
	thresholder->SetUpperThreshold(timeThreshold);

	thresholder->SetOutsideValue(0);
	thresholder->SetInsideValue(255);

	typedef  itk::ImageFileReader< InternalImageType > ReaderType;
	typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;
	// Software Guide : EndCodeSnippet


	ReaderType::Pointer reader = ReaderType::New();
	WriterType::Pointer writer = WriterType::New();

	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	reader->SetFileName(argv[1]);
	reader->SetImageIO(gdcmIO);
	reader->Update();

	typedef itk::RescaleIntensityImageFilter<
		InternalImageType,
		OutputImageType >   CastFilterType;


	typedef   itk::CurvatureAnisotropicDiffusionImageFilter<
		InternalImageType,
		InternalImageType >  SmoothingFilterType;
	// Software Guide : EndCodeSnippet


	//  Software Guide : BeginLatex
	//
	//  Then, the filter is created by invoking the \code{New()} method and
	//  assigning the result to a \doxygen{SmartPointer}.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	SmoothingFilterType::Pointer smoothing = SmoothingFilterType::New();
	// Software Guide : EndCodeSnippet


	//  Software Guide : BeginLatex
	//
	//  The types of the
	//  GradientMagnitudeRecursiveGaussianImageFilter and
	//  SigmoidImageFilter are instantiated using the internal image
	//  type.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	typedef   itk::GradientMagnitudeRecursiveGaussianImageFilter<
		InternalImageType,
		InternalImageType >  GradientFilterType;
	typedef   itk::SigmoidImageFilter<
		InternalImageType,
		InternalImageType >  SigmoidFilterType;
	// Software Guide : EndCodeSnippet


	//  Software Guide : BeginLatex
	//
	//  The corresponding filter objects are instantiated with the
	//  \code{New()} method.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	GradientFilterType::Pointer  gradientMagnitude = GradientFilterType::New();
	SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();
	// Software Guide : EndCodeSnippet


	//  Software Guide : BeginLatex
	//
	//  The minimum and maximum values of the SigmoidImageFilter output are
	//  defined with the methods \code{SetOutputMinimum()} and
	//  \code{SetOutputMaximum()}. In our case, we want these two values to be
	//  $0.0$ and $1.0$ respectively in order to get a nice speed image to feed
	//  to the FastMarchingImageFilter. Additional details on the use of
	//  the SigmoidImageFilter are presented in
	//  Section~\ref{sec:IntensityNonLinearMapping}.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	sigmoid->SetOutputMinimum(0.0);
	sigmoid->SetOutputMaximum(1.0);
	// Software Guide : EndCodeSnippet


	//  Software Guide : BeginLatex
	//
	//  We now declare the type of the FastMarchingImageFilter.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	typedef  itk::FastMarchingImageFilter< InternalImageType,
		InternalImageType >    FastMarchingFilterType;
	// Software Guide : EndCodeSnippet


	//  Software Guide : BeginLatex
	//
	//  Then, we construct one filter of this class using the \code{New()}
	//  method.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	FastMarchingFilterType::Pointer  fastMarching
		= FastMarchingFilterType::New();
	// Software Guide : EndCodeSnippet


	//  Software Guide : BeginLatex
	//
	//  The filters are now connected in a pipeline shown in
	//  Figure~\ref{fig:FastMarchingCollaborationDiagram} using the following
	//  lines.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	smoothing->SetInput(reader->GetOutput());
	gradientMagnitude->SetInput(smoothing->GetOutput());
	sigmoid->SetInput(gradientMagnitude->GetOutput());
	fastMarching->SetInput(sigmoid->GetOutput());
	thresholder->SetInput(fastMarching->GetOutput());
	writer->SetInput(thresholder->GetOutput());
	writer->SetFileName(argv[2]);
	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();

	connector[0]->SetInput(thresholder->GetOutput());

	smoothing->SetTimeStep(0.125);
	smoothing->SetNumberOfIterations(5);
	smoothing->SetConductanceParameter(9.0);

	const double sigma = atof(argv[5]);

	// Software Guide : BeginCodeSnippet
	gradientMagnitude->SetSigma(sigma);

	const double alpha = atof(argv[6]);
	const double beta = atof(argv[7]);


	// Software Guide : BeginCodeSnippet
	sigmoid->SetAlpha(alpha);
	sigmoid->SetBeta(beta);
	
	typedef FastMarchingFilterType::NodeContainer           NodeContainer;
	typedef FastMarchingFilterType::NodeType                NodeType;
	NodeContainer::Pointer seeds = NodeContainer::New();
	//  Software Guide : EndCodeSnippet


	InternalImageType::IndexType  seedPosition;

	seedPosition[0] = atoi(argv[3]);
	seedPosition[1] = atoi(argv[4]);

	NodeType node;
	const double seedValue = 0.0;

	node.SetValue(seedValue);
	node.SetIndex(seedPosition);
	
	seeds->Initialize();
	seeds->InsertElement(0, node);
	
	fastMarching->SetTrialPoints(seeds);
	
	CastFilterType::Pointer caster1 = CastFilterType::New();
	WriterType::Pointer writer1 = WriterType::New();

	writer1->SetImageIO(gdcmIO);
	writer1->UseInputMetaDataDictionaryOff();
	try
	{
		caster1->SetInput(smoothing->GetOutput());
		writer1->SetInput(caster1->GetOutput());
		connector[1]->SetInput(caster1->GetOutput());
		writer1->SetFileName(argv[10]);
		caster1->SetOutputMinimum(0);
		caster1->SetOutputMaximum(255);
		writer1->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		//return EXIT_FAILURE;
	}

    CastFilterType::Pointer caster2 = CastFilterType::New();
	WriterType::Pointer writer2 = WriterType::New();

	writer2->SetImageIO(gdcmIO);
	writer2->UseInputMetaDataDictionaryOff();
	try
	{
		
		caster2->SetInput(gradientMagnitude->GetOutput());
		writer2->SetInput(caster2->GetOutput());
		connector[2]->SetInput(caster2->GetOutput());
		writer2->SetFileName(argv[11]);
		caster2->SetOutputMinimum(0);
		caster2->SetOutputMaximum(255);
		writer2->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		//return EXIT_FAILURE;
	}

	CastFilterType::Pointer caster3 = CastFilterType::New();
	WriterType::Pointer writer3 = WriterType::New();
	writer3->SetImageIO(gdcmIO);
	writer3->UseInputMetaDataDictionaryOff();
	try
	{
		
		caster3->SetInput(sigmoid->GetOutput());
		writer3->SetInput(caster3->GetOutput());
		connector[3]->SetInput(caster3->GetOutput());
		writer3->SetFileName(argv[12]);
		caster3->SetOutputMinimum(0);
		caster3->SetOutputMaximum(255);
		writer3->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		//return EXIT_FAILURE;
	}

	//  Software Guide : BeginLatex
	//
	//  The FastMarchingImageFilter requires the user to specify the
	//  size of the image to be produced as output. This is done using the
	//  \code{SetOutputSize()} method. Note that the size is obtained here from the
	//  output image of the smoothing filter. The size of this image is valid
	//  only after the \code{Update()} method of this filter has been called
	//  directly or indirectly.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	fastMarching->SetOutputSize(
		reader->GetOutput()->GetBufferedRegion().GetSize());
	// Software Guide : EndCodeSnippet


	//  Software Guide : BeginLatex
	//
	//  Since the front representing the contour will propagate continuously
	//  over time, it is desirable to stop the process once a certain time has
	//  been reached. This allows us to save computation time under the
	//  assumption that the region of interest has already been computed. The
	//  value for stopping the process is defined with the method
	//  \code{SetStoppingValue()}. In principle, the stopping value should be a
	//  little bit higher than the threshold value.
	//
	//  \index{itk::FastMarchingImageFilter!SetStoppingValue()}
	//
	//  Software Guide : EndLatex

	const double stoppingTime = atof(argv[9]);

	// Software Guide : BeginCodeSnippet
	fastMarching->SetStoppingValue(stoppingTime);

	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject & excep)
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr << excep << std::endl;
		//return EXIT_FAILURE;
	}

	CastFilterType::Pointer caster4 = CastFilterType::New();
	WriterType::Pointer writer4 = WriterType::New();
	writer4->SetImageIO(gdcmIO);
	writer4->UseInputMetaDataDictionaryOff();
	try
	{
		
		caster4->SetInput(fastMarching->GetOutput());
		writer4->SetInput(caster4->GetOutput());
		connector[4]->SetInput(caster4->GetOutput());
		writer4->SetFileName("FastMarchingFilterOutput4.dcm");
		caster4->SetOutputMinimum(0);
		caster4->SetOutputMaximum(255);
		writer4->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		//return EXIT_FAILURE;
	}
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
//	renderer[4] = vtkSmartPointer<vtkRenderer>::New();

	renderer[0]->AddActor(actor[0]);
	renderer[1]->AddActor(actor[1]);
	renderer[2]->AddActor(actor[2]);
	renderer[3]->AddActor(actor[3]);
	//renderer[actor_count]->SetBackground(1.0, 0.0, 0.0);
	renderer[0]->SetViewport(0.0, 0.0, 0.5, 0.5);
	renderer[1]->SetViewport(0.5, 0.0, 1.0, 0.5);
	renderer[2]->SetViewport(0.0, 0.5, 0.5, 1.0);
	renderer[3]->SetViewport(0.5, 0.5, 1.0, 1.0);
	

	//renWin->AddRenderer(renderer[actor_count]);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer[0]);
	renderWindow->AddRenderer(renderer[1]);
	renderWindow->AddRenderer(renderer[2]);
	renderWindow->AddRenderer(renderer[3]);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	qvtk->SetRenderWindow(renderWindow);
	renderWindow->Render();

	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	renderWindowInteractor->Start();

	typedef itk::ImageFileWriter< InternalImageType > InternalWriterType;

	InternalWriterType::Pointer mapWriter = InternalWriterType::New();
	mapWriter->SetInput(fastMarching->GetOutput());
	mapWriter->SetFileName("FastMarchingFilterOutput4.mha");
	mapWriter->Update();

	InternalWriterType::Pointer speedWriter = InternalWriterType::New();
	speedWriter->SetInput(sigmoid->GetOutput());
	speedWriter->SetFileName("FastMarchingFilterOutput3.mha");
	speedWriter->Update();

	InternalWriterType::Pointer gradientWriter = InternalWriterType::New();
	gradientWriter->SetInput(gradientMagnitude->GetOutput());
	gradientWriter->SetFileName("FastMarchingFilterOutput2.mha");
	gradientWriter->Update();
}
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySlice.png}
//    OUTPUTS: {ShapeDetectionLevelSetFilterOutput5.png}
//    ARGUMENTS:    81 114 5 1.0  -0.5  3.0   .05 1
//  Software Guide : EndCommandLineArgs
void seg_shapedectection(int argc, char *argv[], QVTKWidget* qvtk)
{
	if (argc < 11)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0];
		std::cerr << " inputImage  outputImage";
		std::cerr << " seedX seedY InitialDistance";
		std::cerr << " Sigma SigmoidAlpha SigmoidBeta ";
		std::cerr << " curvatureScaling propagationScaling" << std::endl;
		//return EXIT_FAILURE;
	}


	typedef   float           InternalPixelType;
	const     unsigned int    Dimension = 2;
	typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
	
	typedef float                            OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
	
	typedef itk::BinaryThresholdImageFilter< InternalImageType, OutputImageType >
		ThresholdingFilterType;
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
	// Software Guide : BeginCodeSnippet
	thresholder->SetLowerThreshold(-1000.0);
	thresholder->SetUpperThreshold(0.0);

	thresholder->SetOutsideValue(0);
	thresholder->SetInsideValue(255);
	
	typedef  itk::ImageFileReader< InternalImageType > ReaderType;
	typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;

	ReaderType::Pointer reader = ReaderType::New();
	WriterType::Pointer writer = WriterType::New();

	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	reader->SetFileName(argv[1]);
	reader->SetImageIO(gdcmIO);
	reader->Update();

	writer->SetFileName(argv[2]);
	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();

	typedef itk::RescaleIntensityImageFilter<InternalImageType, OutputImageType>
		CastFilterType;

	typedef   itk::CurvatureAnisotropicDiffusionImageFilter<
		InternalImageType,
		InternalImageType >  SmoothingFilterType;

	SmoothingFilterType::Pointer smoothing = SmoothingFilterType::New();
	
	typedef   itk::GradientMagnitudeRecursiveGaussianImageFilter<
		InternalImageType,
		InternalImageType >  GradientFilterType;

	typedef   itk::SigmoidImageFilter<
		InternalImageType,
		InternalImageType >  SigmoidFilterType;

	GradientFilterType::Pointer  gradientMagnitude = GradientFilterType::New();
	SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();
	
	sigmoid->SetOutputMinimum(0.0);
	sigmoid->SetOutputMaximum(1.0);
	
	typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType >
		FastMarchingFilterType;
	
	FastMarchingFilterType::Pointer  fastMarching
		= FastMarchingFilterType::New();
	
	typedef  itk::ShapeDetectionLevelSetImageFilter< InternalImageType,
		InternalImageType >    ShapeDetectionFilterType;
	ShapeDetectionFilterType::Pointer
		shapeDetection = ShapeDetectionFilterType::New();
	smoothing->SetInput(reader->GetOutput());
	gradientMagnitude->SetInput(smoothing->GetOutput());
	sigmoid->SetInput(gradientMagnitude->GetOutput());

	shapeDetection->SetInput(fastMarching->GetOutput());
	shapeDetection->SetFeatureImage(sigmoid->GetOutput());

	thresholder->SetInput(shapeDetection->GetOutput());

	writer->SetInput(thresholder->GetOutput());

	smoothing->SetTimeStep(0.125);
	smoothing->SetNumberOfIterations(5);
	smoothing->SetConductanceParameter(9.0);
	

	const double sigma = atof(argv[6]);

	gradientMagnitude->SetSigma(sigma);
	

	const double alpha = atof(argv[7]);
	const double beta = atof(argv[8]);

	// Software Guide : BeginCodeSnippet
	sigmoid->SetAlpha(alpha);
	sigmoid->SetBeta(beta);
	
	typedef FastMarchingFilterType::NodeContainer           NodeContainer;
	typedef FastMarchingFilterType::NodeType                NodeType;
	NodeContainer::Pointer seeds = NodeContainer::New();
	//  Software Guide : EndCodeSnippet


	InternalImageType::IndexType  seedPosition;

	seedPosition[0] = atoi(argv[3]);
	seedPosition[1] = atoi(argv[4]);

	const double initialDistance = atof(argv[5]);

	// Software Guide : BeginCodeSnippet
	NodeType node;
	const double seedValue = -initialDistance;

	node.SetValue(seedValue);
	node.SetIndex(seedPosition);
	
	seeds->Initialize();
	seeds->InsertElement(0, node);
	
	fastMarching->SetTrialPoints(seeds);
	
	fastMarching->SetSpeedConstant(1.0);
	
	//
	typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
	ConnectorType::Pointer connector[5];
	connector[0]->GlobalWarningDisplayOff();
	connector[1]->GlobalWarningDisplayOff();
	connector[2]->GlobalWarningDisplayOff();
	connector[3]->GlobalWarningDisplayOff();
	connector[4]->GlobalWarningDisplayOff();

	connector[0]= ConnectorType::New();
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

	WriterType::Pointer writer1 = WriterType::New();
	WriterType::Pointer writer2 = WriterType::New();
	WriterType::Pointer writer3 = WriterType::New();
	WriterType::Pointer writer4 = WriterType::New();
	writer1->SetImageIO(gdcmIO);
	writer1->UseInputMetaDataDictionaryOff();
	writer2->SetImageIO(gdcmIO);
	writer2->UseInputMetaDataDictionaryOff();
	writer3->SetImageIO(gdcmIO);
	writer3->UseInputMetaDataDictionaryOff();
	writer4->SetImageIO(gdcmIO);
	writer4->UseInputMetaDataDictionaryOff();

	caster1->SetInput(smoothing->GetOutput());
	writer1->SetInput(caster1->GetOutput());
	//connector[1]->SetInput(caster1->GetOutput());
	writer1->SetFileName("ShapeDetectionLevelSetFilterOutput1.dcm");
	caster1->SetOutputMinimum(0);
	caster1->SetOutputMaximum(255);
	writer1->Update();

	caster2->SetInput(gradientMagnitude->GetOutput());
	writer2->SetInput(caster2->GetOutput());
	//connector[2]->SetInput(caster2->GetOutput());
	writer2->SetFileName("ShapeDetectionLevelSetFilterOutput2.dcm");
	caster2->SetOutputMinimum(0);
	caster2->SetOutputMaximum(255);
	writer2->Update();

	caster3->SetInput(sigmoid->GetOutput());
	writer3->SetInput(caster3->GetOutput());
	//connector[3]->SetInput(caster3->GetOutput());
	writer3->SetFileName("ShapeDetectionLevelSetFilterOutput3.dcm");
	caster3->SetOutputMinimum(0);
	caster3->SetOutputMaximum(255);
	writer3->Update();

	caster4->SetInput(fastMarching->GetOutput());
	writer4->SetInput(caster4->GetOutput());
	//connector[4]->SetInput(caster4->GetOutput());
	writer4->SetFileName("ShapeDetectionLevelSetFilterOutput4.dcm");
	caster4->SetOutputMinimum(0);
	caster4->SetOutputMaximum(255);

	fastMarching->SetOutputSize(
		reader->GetOutput()->GetBufferedRegion().GetSize());

	const double curvatureScaling = atof(argv[9]);
	const double propagationScaling = atof(argv[10]);

	//  Software Guide : BeginCodeSnippet
	shapeDetection->SetPropagationScaling(propagationScaling);
	shapeDetection->SetCurvatureScaling(curvatureScaling);
	
	shapeDetection->SetMaximumRMSError(0.02);
	shapeDetection->SetNumberOfIterations(800);

	connector[1]->SetInput(caster1->GetOutput());
	connector[2]->SetInput(caster2->GetOutput());
	connector[3]->SetInput(caster3->GetOutput());
	connector[4]->SetInput(thresholder->GetOutput());

	//connector[0]->Update();
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
	//renderer[actor_count]->SetBackground(1.0, 0.0, 0.0);
	renderer[1]->SetViewport(0.0, 0.0, 0.5, 0.5);
	renderer[2]->SetViewport(0.5, 0.0, 1.0, 0.5);
	renderer[3]->SetViewport(0.0, 0.5, 0.5, 1.0);
	renderer[4]->SetViewport(0.5, 0.5, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer[1]);
	renderWindow->AddRenderer(renderer[2]);
	renderWindow->AddRenderer(renderer[3]);
	renderWindow->AddRenderer(renderer[4]);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();


	qvtk->SetRenderWindow(renderWindow);
	renderWindow->Render();

	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	renderWindowInteractor->Start();

	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject & excep)
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr << excep << std::endl;
		//return EXIT_FAILURE;
	}
	
	std::cout << std::endl;
	std::cout << "Max. no. iterations: " << shapeDetection->GetNumberOfIterations() << std::endl;
	std::cout << "Max. RMS error: " << shapeDetection->GetMaximumRMSError() << std::endl;
	std::cout << std::endl;
	std::cout << "No. elpased iterations: " << shapeDetection->GetElapsedIterations() << std::endl;
	std::cout << "RMS change: " << shapeDetection->GetRMSChange() << std::endl;

	writer4->Update();

	typedef itk::ImageFileWriter< InternalImageType > InternalWriterType;

	InternalWriterType::Pointer mapWriter = InternalWriterType::New();
	mapWriter->SetInput(fastMarching->GetOutput());
	mapWriter->SetFileName("ShapeDetectionLevelSetFilterOutput4.mha");
	mapWriter->Update();

	InternalWriterType::Pointer speedWriter = InternalWriterType::New();
	speedWriter->SetInput(sigmoid->GetOutput());
	speedWriter->SetFileName("ShapeDetectionLevelSetFilterOutput3.mha");
	speedWriter->Update();

	InternalWriterType::Pointer gradientWriter = InternalWriterType::New();
	gradientWriter->SetInput(gradientMagnitude->GetOutput());
	gradientWriter->SetFileName("ShapeDetectionLevelSetFilterOutput2.mha");
	gradientWriter->Update();
}
