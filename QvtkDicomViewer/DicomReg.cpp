#include<head_all.h>
#include <vtkCamera.h>
#include <vtkPNGReader.h>
#include "itkGDCMImageIO.h"  
#include <vtkDICOMImageReader.h>
#include "itkRegularStepGradientDescentBaseOptimizer.h"
class CommandIterationUpdate_test : public itk::Command
{
public:
	typedef CommandIterationUpdate_test   Self;
	typedef itk::Command             Superclass;
	typedef itk::SmartPointer<Self>  Pointer;
	itkNewMacro(Self);

protected:
	CommandIterationUpdate_test() {};

public:

	typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
	typedef const OptimizerType*                     OptimizerPointer;

	void Execute(itk::Object *caller, const itk::EventObject & event)
	{
		Execute((const itk::Object *)caller, event);
	}

	void Execute(const itk::Object * object, const itk::EventObject & event)
	{
		OptimizerPointer optimizer = static_cast< OptimizerPointer >(object);

		if (!itk::IterationEvent().CheckEvent(&event))
		{
			return;
		}

		std::cout << optimizer->GetCurrentIteration() << " = ";
		std::cout << optimizer->GetValue() << " : ";
		std::cout << optimizer->GetCurrentPosition() << std::endl;
	}

};
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySliceBorder20.png}
//    INPUTS:  {BrainProtonDensitySliceShifted13x17y.png}
//    OUTPUTS: {ImageRegistration1Output.png}
//    OUTPUTS: {ImageRegistration1DifferenceAfter.png}
//    OUTPUTS: {ImageRegistration1DifferenceBefore.png}
//  Software Guide : EndCommandLineArgs
void reg_test(int argc, char *argv[],QVTKWidget* qvtk)
{
	if (argc < 4)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0];
		std::cerr << " fixedImageFile  movingImageFile ";
		std::cerr << "outputImagefile [differenceImageAfter]";
		std::cerr << "[differenceImageBefore]" << std::endl;
		//return EXIT_FAILURE;
	}
	const    unsigned int    Dimension = 2;
	typedef  float    PixelType;
	
	typedef itk::Image< PixelType, Dimension >  FixedImageType;
	typedef itk::Image< PixelType, Dimension >  MovingImageType;
	
	typedef itk::TranslationTransform< double, Dimension > TransformType;
	typedef itk::RegularStepGradientDescentOptimizer       OptimizerType;

	typedef itk::MeanSquaresImageToImageMetric<
		FixedImageType,
		MovingImageType >    MetricType;
	
	typedef itk::LinearInterpolateImageFunction<
		MovingImageType,
		double          >    InterpolatorType;

	typedef itk::ImageRegistrationMethod<
		FixedImageType,
		MovingImageType >    RegistrationType;
	
	MetricType::Pointer         metric = MetricType::New();
	TransformType::Pointer      transform = TransformType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	InterpolatorType::Pointer   interpolator = InterpolatorType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();
	
	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);
	registration->SetTransform(transform);
	registration->SetInterpolator(interpolator);
	
	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();
	MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

	fixedImageReader->SetFileName(argv[1]);
	fixedImageReader->SetImageIO(gdcmIO);
	movingImageReader->SetFileName(argv[2]);
	movingImageReader->SetImageIO(gdcmIO);

	registration->SetFixedImage(fixedImageReader->GetOutput());
	registration->SetMovingImage(movingImageReader->GetOutput());
	
	fixedImageReader->Update();
	registration->SetFixedImageRegion(
		fixedImageReader->GetOutput()->GetBufferedRegion());

	typedef RegistrationType::ParametersType ParametersType;
	ParametersType initialParameters(transform->GetNumberOfParameters());

	initialParameters[0] = 0.0;  // Initial offset in mm along X
	initialParameters[1] = 0.0;  // Initial offset in mm along Y

	registration->SetInitialTransformParameters(initialParameters);
	
	optimizer->SetMaximumStepLength(4.00);
	optimizer->SetMinimumStepLength(0.01);
	
	optimizer->SetNumberOfIterations(200);
	
	CommandIterationUpdate_test::Pointer observer = CommandIterationUpdate_test::New();
	optimizer->AddObserver(itk::IterationEvent(), observer);

	try
	{
		registration->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		//return EXIT_FAILURE;
	}
	
	ParametersType finalParameters = registration->GetLastTransformParameters();
	
	const double TranslationAlongX = finalParameters[0];
	const double TranslationAlongY = finalParameters[1];
	
	const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
	
	const double bestValue = optimizer->GetValue();
	
	std::cout << "Result = " << std::endl;
	std::cout << " Translation X = " << TranslationAlongX << std::endl;
	std::cout << " Translation Y = " << TranslationAlongY << std::endl;
	std::cout << " Iterations    = " << numberOfIterations << std::endl;
	std::cout << " Metric value  = " << bestValue << std::endl;

	typedef itk::ResampleImageFilter<
		MovingImageType,
		FixedImageType >    ResampleFilterType;
	
	ResampleFilterType::Pointer resampler = ResampleFilterType::New();
	resampler->SetInput(movingImageReader->GetOutput());
	
	resampler->SetTransform(registration->GetOutput()->Get());
	
	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(fixedImage->GetOrigin());
	resampler->SetOutputSpacing(fixedImage->GetSpacing());
	resampler->SetOutputDirection(fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(100);
	
	typedef float                            OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
	typedef itk::CastImageFilter<
		FixedImageType,
		OutputImageType >          CastFilterType;
	typedef itk::ImageFileWriter< OutputImageType >  WriterType;
	// Software Guide : EndCodeSnippet


	//  Software Guide : BeginLatex
	//
	//  The filters are created by invoking their \code{New()}
	//  method.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	WriterType::Pointer      writer = WriterType::New();
	CastFilterType::Pointer  caster = CastFilterType::New();
	// Software Guide : EndCodeSnippet

	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();
	writer->SetFileName(argv[3]);


	//  Software Guide : BeginLatex
	//
	//  The filters are connected together and the \code{Update()} method of the
	//  writer is invoked in order to trigger the execution of the pipeline.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	caster->SetInput(resampler->GetOutput());

	writer->SetInput(caster->GetOutput());
	writer->Update();
	
	typedef itk::SubtractImageFilter<
		FixedImageType,
		FixedImageType,
		FixedImageType > DifferenceFilterType;

	DifferenceFilterType::Pointer difference = DifferenceFilterType::New();

	difference->SetInput1(fixedImageReader->GetOutput());
	difference->SetInput2(resampler->GetOutput());
	
	typedef itk::RescaleIntensityImageFilter<
		FixedImageType,
		OutputImageType >   RescalerType;

	RescalerType::Pointer intensityRescaler = RescalerType::New();

	intensityRescaler->SetInput(difference->GetOutput());
	intensityRescaler->SetOutputMinimum(0);
	intensityRescaler->SetOutputMaximum(255);

	resampler->SetDefaultPixelValue(1);
	WriterType::Pointer writer2 = WriterType::New();
	writer2->SetImageIO(gdcmIO);
	writer2->UseInputMetaDataDictionaryOff();
	writer2->SetInput(intensityRescaler->GetOutput());
	
	if (argc > 4)
	{
		writer2->SetFileName(argv[4]);
		writer2->Update();
	}

	TransformType::Pointer identityTransform = TransformType::New();
	identityTransform->SetIdentity();
	resampler->SetTransform(identityTransform);

	if (argc > 5)
	{
		writer2->SetFileName(argv[5]);
		writer2->Update();
	}
	//typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
	//ConnectorType::Pointer connector = ConnectorType::New();

	vtkSmartPointer<vtkDICOMImageReader> dcmreader;
	vtkSmartPointer<vtkDICOMImageReader> dcmreader2;
	vtkSmartPointer<vtkRenderer>  renderer[2];
	vtkSmartPointer<vtkImageActor> actor[2];

	renderer[0] = vtkSmartPointer<vtkRenderer>::New();
	dcmreader= vtkSmartPointer<vtkDICOMImageReader>::New();
	actor[0] = vtkSmartPointer<vtkImageActor>::New();

	renderer[1] = vtkSmartPointer<vtkRenderer>::New();
	dcmreader2 = vtkSmartPointer<vtkDICOMImageReader>::New();
	actor[1] = vtkSmartPointer<vtkImageActor>::New();

	dcmreader->SetFileName(argv[1]);
	dcmreader->Update();
	actor[0]->GetMapper()->SetInputData(dcmreader->GetOutput());
	
	//actor[0]->SetOpacity();

	dcmreader2->SetFileName(argv[3]);
	dcmreader2->Update();
	actor[1]->GetMapper()->SetInputData(dcmreader2->GetOutput());
	actor[1]->SetOpacity(0.5);

	renderer[0]->AddActor(actor[0]);
	renderer[1]->AddActor(actor[1]);

	renderer[0]->SetLayer(0);
	renderer[1]->SetLayer(1);

	vtkSmartPointer<vtkRenderWindow> renwin
		= vtkSmartPointer<vtkRenderWindow>::New();

	qvtk->SetRenderWindow(renwin);
	renwin->SetNumberOfLayers(2);
	renwin->AddRenderer(renderer[0]);
	renwin->AddRenderer(renderer[1]);
	renwin->Render();
}

class CommandIterationUpdate_2Dtransform : public itk::Command
{
public:
	typedef  CommandIterationUpdate_2Dtransform   Self;
	typedef  itk::Command             Superclass;
	typedef itk::SmartPointer<Self>   Pointer;
	itkNewMacro(Self);

protected:
	CommandIterationUpdate_2Dtransform() {};

public:
	typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
	typedef   const OptimizerType *                  OptimizerPointer;

	void Execute(itk::Object *caller, const itk::EventObject & event)
	{
		Execute((const itk::Object *)caller, event);
	}

	void Execute(const itk::Object * object, const itk::EventObject & event)
	{
		OptimizerPointer optimizer = static_cast< OptimizerPointer >(object);
		if (!itk::IterationEvent().CheckEvent(&event))
		{
			return;
		}
		std::cout << optimizer->GetCurrentIteration() << "   ";
		std::cout << optimizer->GetValue() << "   ";
		std::cout << optimizer->GetCurrentPosition() << std::endl;
	}
};
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySliceBorder20.png}
//    INPUTS:  {BrainProtonDensitySliceR10X13Y17S12.png}
//    OUTPUTS: {ImageRegistration7Output.png}
//    OUTPUTS: {ImageRegistration7DifferenceBefore.png}
//    OUTPUTS: {ImageRegistration7DifferenceAfter.png}
//    ARGUMENTS:    1.0   1.0   0.0
//  Software Guide : EndCommandLineArgs
void reg_2Dtransform(int argc, char *argv[], QVTKWidget* qvtk)
{
	if (argc < 4)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0];
		std::cerr << " fixedImageFile  movingImageFile ";
		std::cerr << " outputImagefile  [differenceBeforeRegistration] ";
		std::cerr << " [differenceAfterRegistration] ";
		std::cerr << " [steplength] ";
		std::cerr << " [initialScaling] [initialAngle] ";
		std::cerr << std::endl;
		//return EXIT_FAILURE;
	}

	const    unsigned int    Dimension = 2;
	typedef  float           PixelType;

	typedef itk::Image< PixelType, Dimension >  FixedImageType;
	typedef itk::Image< PixelType, Dimension >  MovingImageType;

	typedef itk::CenteredSimilarity2DTransform< double > TransformType;
	

	typedef itk::RegularStepGradientDescentOptimizer       OptimizerType;
	typedef itk::MeanSquaresImageToImageMetric< FixedImageType, MovingImageType >
		MetricType;
	typedef itk::LinearInterpolateImageFunction< MovingImageType, double >
		InterpolatorType;
	typedef itk::ImageRegistrationMethod< FixedImageType, MovingImageType >
		RegistrationType;

	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	MetricType::Pointer         metric = MetricType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	InterpolatorType::Pointer   interpolator = InterpolatorType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();

	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);
	registration->SetInterpolator(interpolator);

	TransformType::Pointer  transform = TransformType::New();
	registration->SetTransform(transform);
	
	typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;

	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();
	MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

	fixedImageReader->SetFileName(argv[1]);
	fixedImageReader->SetImageIO(gdcmIO);

	movingImageReader->SetFileName(argv[2]);
	movingImageReader->SetImageIO(gdcmIO);

	registration->SetFixedImage(fixedImageReader->GetOutput());
	registration->SetMovingImage(movingImageReader->GetOutput());
	fixedImageReader->Update();

	registration->SetFixedImageRegion(
		fixedImageReader->GetOutput()->GetBufferedRegion());

	typedef itk::CenteredTransformInitializer<
		TransformType, FixedImageType,
		MovingImageType >  TransformInitializerType;

	TransformInitializerType::Pointer initializer
		= TransformInitializerType::New();

	initializer->SetTransform(transform);

	initializer->SetFixedImage(fixedImageReader->GetOutput());
	initializer->SetMovingImage(movingImageReader->GetOutput());

	initializer->MomentsOn();

	initializer->InitializeTransform();
	
	double initialScale = 1.0;

	if (argc > 7)
	{
		initialScale = atof(argv[7]);
	}

	double initialAngle = 0.0;

	if (argc > 8)
	{
		initialAngle = atof(argv[8]);
	}

	// Software Guide : BeginCodeSnippet
	transform->SetScale(initialScale);
	transform->SetAngle(initialAngle);
	
	registration->SetInitialTransformParameters(transform->GetParameters());
	
	typedef OptimizerType::ScalesType       OptimizerScalesType;
	OptimizerScalesType optimizerScales(transform->GetNumberOfParameters());
	const double translationScale = 1.0 / 100.0;

	optimizerScales[0] = 10.0;
	optimizerScales[1] = 1.0;
	optimizerScales[2] = translationScale;
	optimizerScales[3] = translationScale;
	optimizerScales[4] = translationScale;
	optimizerScales[5] = translationScale;

	optimizer->SetScales(optimizerScales);

	double steplength = 1.0;

	if (argc > 6)
	{
		steplength = atof(argv[6]);
	}

	// Software Guide : BeginCodeSnippet
	optimizer->SetMaximumStepLength(steplength);
	optimizer->SetMinimumStepLength(0.0001);
	optimizer->SetNumberOfIterations(500);
	
	CommandIterationUpdate_2Dtransform::Pointer observer = CommandIterationUpdate_2Dtransform::New();
	optimizer->AddObserver(itk::IterationEvent(), observer);

	try
	{
		registration->Update();
		std::cout << "Optimizer stop condition: "
			<< registration->GetOptimizer()->GetStopConditionDescription()
			<< std::endl;
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		//return EXIT_FAILURE;
	}

	OptimizerType::ParametersType finalParameters =
		registration->GetLastTransformParameters();


	const double finalScale = finalParameters[0];
	const double finalAngle = finalParameters[1];
	const double finalRotationCenterX = finalParameters[2];
	const double finalRotationCenterY = finalParameters[3];
	const double finalTranslationX = finalParameters[4];
	const double finalTranslationY = finalParameters[5];

	const unsigned int numberOfIterations = optimizer->GetCurrentIteration();

	const double bestValue = optimizer->GetValue();


	// Print out results
	//
	const double finalAngleInDegrees = finalAngle * 180.0 / itk::Math::pi;

	std::cout << std::endl;
	std::cout << "Result = " << std::endl;
	std::cout << " Scale         = " << finalScale << std::endl;
	std::cout << " Angle (radians) " << finalAngle << std::endl;
	std::cout << " Angle (degrees) " << finalAngleInDegrees << std::endl;
	std::cout << " Center X      = " << finalRotationCenterX << std::endl;
	std::cout << " Center Y      = " << finalRotationCenterY << std::endl;
	std::cout << " Translation X = " << finalTranslationX << std::endl;
	std::cout << " Translation Y = " << finalTranslationY << std::endl;
	std::cout << " Iterations    = " << numberOfIterations << std::endl;
	std::cout << " Metric value  = " << bestValue << std::endl;

	typedef itk::ResampleImageFilter< MovingImageType,
		FixedImageType > ResampleFilterType;

	TransformType::Pointer finalTransform = TransformType::New();

	finalTransform->SetParameters(finalParameters);
	finalTransform->SetFixedParameters(transform->GetFixedParameters());

	ResampleFilterType::Pointer resampler = ResampleFilterType::New();

	resampler->SetTransform(finalTransform);
	resampler->SetInput(movingImageReader->GetOutput());

	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(fixedImage->GetOrigin());
	resampler->SetOutputSpacing(fixedImage->GetSpacing());
	resampler->SetOutputDirection(fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(100);

	typedef  float  OutputPixelType;

	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

	typedef itk::CastImageFilter< FixedImageType, OutputImageType >
		CastFilterType;

	typedef itk::ImageFileWriter< OutputImageType >  WriterType;


	WriterType::Pointer      writer = WriterType::New();

	CastFilterType::Pointer  caster = CastFilterType::New();

	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();
	writer->SetFileName(argv[3]);

	caster->SetInput(resampler->GetOutput());
	writer->SetInput(caster->GetOutput());
	writer->Update();


	typedef itk::SubtractImageFilter<
		FixedImageType,
		FixedImageType,
		FixedImageType > DifferenceFilterType;

	DifferenceFilterType::Pointer difference = DifferenceFilterType::New();


	typedef itk::RescaleIntensityImageFilter<
		FixedImageType,
		OutputImageType >   RescalerType;

	RescalerType::Pointer intensityRescaler = RescalerType::New();

	intensityRescaler->SetInput(difference->GetOutput());
	intensityRescaler->SetOutputMinimum(0);
	intensityRescaler->SetOutputMaximum(255);

	difference->SetInput1(fixedImageReader->GetOutput());
	difference->SetInput2(resampler->GetOutput());

	resampler->SetDefaultPixelValue(1);

	WriterType::Pointer writer2 = WriterType::New();
	writer2->SetImageIO(gdcmIO);
	writer2->UseInputMetaDataDictionaryOff();
	writer2->SetInput(intensityRescaler->GetOutput());


	// Compute the difference image between the
	// fixed and resampled moving image.
	if (argc > 5)
	{
		writer2->SetFileName(argv[5]);
		writer2->Update();
	}


	typedef itk::IdentityTransform< double, Dimension > IdentityTransformType;
	IdentityTransformType::Pointer identity = IdentityTransformType::New();

	// Compute the difference image between the
	// fixed and moving image before registration.
	if (argc > 4)
	{
		resampler->SetTransform(identity);
		writer2->SetFileName(argv[4]);
		writer2->Update();
	}
	vtkSmartPointer<vtkDICOMImageReader> dcmreader;
	vtkSmartPointer<vtkDICOMImageReader> dcmreader2;
	vtkSmartPointer<vtkRenderer>  renderer[2];
	vtkSmartPointer<vtkImageActor> actor[2];

	renderer[0] = vtkSmartPointer<vtkRenderer>::New();
	dcmreader = vtkSmartPointer<vtkDICOMImageReader>::New();
	actor[0] = vtkSmartPointer<vtkImageActor>::New();

	renderer[1] = vtkSmartPointer<vtkRenderer>::New();
	dcmreader2 = vtkSmartPointer<vtkDICOMImageReader>::New();
	actor[1] = vtkSmartPointer<vtkImageActor>::New();

	dcmreader->SetFileName(argv[1]);
	dcmreader->Update();
	actor[0]->GetMapper()->SetInputData(dcmreader->GetOutput());

	//actor[0]->SetOpacity();

	dcmreader2->SetFileName(argv[3]);
	dcmreader2->Update();
	actor[1]->GetMapper()->SetInputData(dcmreader2->GetOutput());
	actor[1]->SetOpacity(0.5);
	//actor[1]->AddPosition(100,100,100);

	renderer[0]->AddActor(actor[0]);
	renderer[1]->AddActor(actor[1]);

	renderer[0]->SetLayer(0);
	renderer[1]->SetLayer(1);

	vtkSmartPointer<vtkRenderWindow> renwin
		= vtkSmartPointer<vtkRenderWindow>::New();

	qvtk->SetRenderWindow(renwin);
	renwin->SetNumberOfLayers(2);
	renwin->AddRenderer(renderer[0]);
	renwin->AddRenderer(renderer[1]);
	renwin->Render();

}

class CommandIterationUpdate_AffinTrans : public itk::Command
{
public:
	typedef  CommandIterationUpdate_AffinTrans   Self;
	typedef  itk::Command             Superclass;
	typedef itk::SmartPointer<Self>   Pointer;
	itkNewMacro(Self);

protected:
	CommandIterationUpdate_AffinTrans() {};

public:
	typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
	typedef   const OptimizerType *                  OptimizerPointer;

	void Execute(itk::Object *caller, const itk::EventObject & event)
	{
		Execute((const itk::Object *)caller, event);
	}

	void Execute(const itk::Object * object, const itk::EventObject & event)
	{
		OptimizerPointer optimizer = static_cast< OptimizerPointer >(object);
		if (!itk::IterationEvent().CheckEvent(&event))
		{
			return;
		}
		std::cout << optimizer->GetCurrentIteration() << "   ";
		std::cout << optimizer->GetValue() << "   ";
		std::cout << optimizer->GetCurrentPosition();

		// Print the angle for the trace plot
		vnl_matrix<double> p(2, 2);
		p[0][0] = (double)optimizer->GetCurrentPosition()[0];
		p[0][1] = (double)optimizer->GetCurrentPosition()[1];
		p[1][0] = (double)optimizer->GetCurrentPosition()[2];
		p[1][1] = (double)optimizer->GetCurrentPosition()[3];
		vnl_svd<double> svd(p);
		vnl_matrix<double> r(2, 2);
		r = svd.U() * vnl_transpose(svd.V());
		double angle = std::asin(r[1][0]);
		std::cout << " AffineAngle: " << angle * 180.0 / itk::Math::pi << std::endl;
	}
};
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySliceBorder20.png}
//    INPUTS:  {BrainProtonDensitySliceR10X13Y17.png}
//    OUTPUTS: {ImageRegistration9Output.png}
//    OUTPUTS: {ImageRegistration9DifferenceBefore.png}
//    OUTPUTS: {ImageRegistration9DifferenceAfter.png}
//    ARGUMENTS:    1.0 300
//  Software Guide : EndCommandLineArgs
void reg_AffineTrans(int argc, char *argv[], QVTKWidget* qvtk)
{
	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	if (argc < 4)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0];
		std::cerr << "   fixedImageFile  movingImageFile " << std::endl;
		std::cerr << "   outputImagefile  [differenceBeforeRegistration] " << std::endl;
		std::cerr << "   [differenceAfterRegistration] " << std::endl;
		std::cerr << "   [stepLength] [maxNumberOfIterations] " << std::endl;
		//return EXIT_FAILURE;
	}

	const    unsigned int    Dimension = 2;
	typedef  float           PixelType;

	typedef itk::Image< PixelType, Dimension >  FixedImageType;
	typedef itk::Image< PixelType, Dimension >  MovingImageType;
	
	typedef itk::AffineTransform< double, Dimension  > TransformType;
	
	typedef itk::RegularStepGradientDescentOptimizer       OptimizerType;
	typedef itk::MeanSquaresImageToImageMetric<
		FixedImageType, MovingImageType >            MetricType;
	typedef itk::LinearInterpolateImageFunction<
		MovingImageType, double          >           InterpolatorType;
	typedef itk::ImageRegistrationMethod<
		FixedImageType, MovingImageType >            RegistrationType;

	MetricType::Pointer         metric = MetricType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	InterpolatorType::Pointer   interpolator = InterpolatorType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();

	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);
	registration->SetInterpolator(interpolator);

	TransformType::Pointer  transform = TransformType::New();
	registration->SetTransform(transform);
	
	typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();
	MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();
	fixedImageReader->SetFileName(argv[1]);
	fixedImageReader->SetImageIO(gdcmIO);
	movingImageReader->SetFileName(argv[2]);
	movingImageReader->SetImageIO(gdcmIO);

	registration->SetFixedImage(fixedImageReader->GetOutput());
	registration->SetMovingImage(movingImageReader->GetOutput());
	fixedImageReader->Update();

	registration->SetFixedImageRegion(
		fixedImageReader->GetOutput()->GetBufferedRegion());

	typedef itk::CenteredTransformInitializer<
		TransformType, FixedImageType,
		MovingImageType >  TransformInitializerType;
	TransformInitializerType::Pointer initializer
		= TransformInitializerType::New();
	initializer->SetTransform(transform);
	initializer->SetFixedImage(fixedImageReader->GetOutput());
	initializer->SetMovingImage(movingImageReader->GetOutput());
	initializer->MomentsOn();
	initializer->InitializeTransform();
	
	registration->SetInitialTransformParameters(
		transform->GetParameters());
	
	double translationScale = 1.0 / 1000.0;
	if (argc > 8)
	{
		//translationScale = atof(argv[8]);
	}

	typedef OptimizerType::ScalesType       OptimizerScalesType;
	OptimizerScalesType optimizerScales(transform->GetNumberOfParameters());

	optimizerScales[0] = 1.0;
	optimizerScales[1] = 1.0;
	optimizerScales[2] = 1.0;
	optimizerScales[3] = 1.0;
	optimizerScales[4] = translationScale;
	optimizerScales[5] = translationScale;

	optimizer->SetScales(optimizerScales);
	

	double steplength = 0.1;

	if (argc > 6)
	{
		steplength = atof(argv[6]);
	}


	unsigned int maxNumberOfIterations = 300;

	if (argc > 7)
	{
		maxNumberOfIterations = atoi(argv[7]);
	}

	optimizer->SetMaximumStepLength(steplength);
	optimizer->SetMinimumStepLength(0.0001);
	optimizer->SetNumberOfIterations(maxNumberOfIterations);
	
	optimizer->MinimizeOn();
	
	CommandIterationUpdate_AffinTrans::Pointer observer = CommandIterationUpdate_AffinTrans::New();
	optimizer->AddObserver(itk::IterationEvent(), observer);

	try
	{
		registration->Update();
		std::cout << "Optimizer stop condition: "
			<< registration->GetOptimizer()->GetStopConditionDescription()
			<< std::endl;
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		//return EXIT_FAILURE;
	}
	
	OptimizerType::ParametersType finalParameters =
		registration->GetLastTransformParameters();

	const double finalRotationCenterX = transform->GetCenter()[0];
	const double finalRotationCenterY = transform->GetCenter()[1];
	const double finalTranslationX = finalParameters[4];
	const double finalTranslationY = finalParameters[5];

	const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
	const double bestValue = optimizer->GetValue();
	
	std::cout << "Result = " << std::endl;
	std::cout << " Center X      = " << finalRotationCenterX << std::endl;
	std::cout << " Center Y      = " << finalRotationCenterY << std::endl;
	std::cout << " Translation X = " << finalTranslationX << std::endl;
	std::cout << " Translation Y = " << finalTranslationY << std::endl;
	std::cout << " Iterations    = " << numberOfIterations << std::endl;
	std::cout << " Metric value  = " << bestValue << std::endl;

	vnl_matrix<double> p(2, 2);
	p[0][0] = (double)finalParameters[0];
	p[0][1] = (double)finalParameters[1];
	p[1][0] = (double)finalParameters[2];
	p[1][1] = (double)finalParameters[3];
	vnl_svd<double> svd(p);
	vnl_matrix<double> r(2, 2);
	r = svd.U() * vnl_transpose(svd.V());
	double angle = std::asin(r[1][0]);

	const double angleInDegrees = angle * 180.0 / itk::Math::pi;

	std::cout << " Scale 1         = " << svd.W(0) << std::endl;
	std::cout << " Scale 2         = " << svd.W(1) << std::endl;
	std::cout << " Angle (degrees) = " << angleInDegrees << std::endl;
	typedef itk::ResampleImageFilter<
		MovingImageType,
		FixedImageType >    ResampleFilterType;

	TransformType::Pointer finalTransform = TransformType::New();

	finalTransform->SetParameters(finalParameters);
	finalTransform->SetFixedParameters(transform->GetFixedParameters());

	ResampleFilterType::Pointer resampler = ResampleFilterType::New();

	resampler->SetTransform(finalTransform);
	resampler->SetInput(movingImageReader->GetOutput());

	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(fixedImage->GetOrigin());
	resampler->SetOutputSpacing(fixedImage->GetSpacing());
	resampler->SetOutputDirection(fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(100);

	typedef  float  OutputPixelType;

	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

	typedef itk::CastImageFilter<
		FixedImageType,
		OutputImageType > CastFilterType;

	typedef itk::ImageFileWriter< OutputImageType >  WriterType;


	WriterType::Pointer      writer = WriterType::New();
	CastFilterType::Pointer  caster = CastFilterType::New();

	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();
	writer->SetFileName(argv[3]);

	caster->SetInput(resampler->GetOutput());
	writer->SetInput(caster->GetOutput());
	writer->Update();


	typedef itk::SubtractImageFilter<
		FixedImageType,
		FixedImageType,
		FixedImageType > DifferenceFilterType;

	DifferenceFilterType::Pointer difference = DifferenceFilterType::New();

	difference->SetInput1(fixedImageReader->GetOutput());
	difference->SetInput2(resampler->GetOutput());

	WriterType::Pointer writer2 = WriterType::New();
	writer2->SetImageIO(gdcmIO);
	writer2->UseInputMetaDataDictionaryOff();
	typedef itk::RescaleIntensityImageFilter<
		FixedImageType,
		OutputImageType >   RescalerType;

	RescalerType::Pointer intensityRescaler = RescalerType::New();

	intensityRescaler->SetInput(difference->GetOutput());
	intensityRescaler->SetOutputMinimum(0);
	intensityRescaler->SetOutputMaximum(255);

	writer2->SetInput(intensityRescaler->GetOutput());
	resampler->SetDefaultPixelValue(1);

	// Compute the difference image between the
	// fixed and resampled moving image.
	if (argc > 5)
	{
		writer2->SetFileName(argv[5]);
		writer2->Update();
	}
	typedef itk::IdentityTransform< double, Dimension > IdentityTransformType;
	IdentityTransformType::Pointer identity = IdentityTransformType::New();

	// Compute the difference image between the
	// fixed and moving image before registration.
	if (argc > 4)
	{
		resampler->SetTransform(identity);
		writer2->SetFileName(argv[4]);
		writer2->Update();
	}
	vtkSmartPointer<vtkDICOMImageReader> dcmreader;
	vtkSmartPointer<vtkDICOMImageReader> dcmreader2;
	vtkSmartPointer<vtkRenderer>  renderer[2];
	vtkSmartPointer<vtkImageActor> actor[2];

	renderer[0] = vtkSmartPointer<vtkRenderer>::New();
	dcmreader = vtkSmartPointer<vtkDICOMImageReader>::New();
	actor[0] = vtkSmartPointer<vtkImageActor>::New();

	renderer[1] = vtkSmartPointer<vtkRenderer>::New();
	dcmreader2 = vtkSmartPointer<vtkDICOMImageReader>::New();
	actor[1] = vtkSmartPointer<vtkImageActor>::New();

	dcmreader->SetFileName(argv[1]);
	dcmreader->Update();
	actor[0]->GetMapper()->SetInputData(dcmreader->GetOutput());

	//actor[0]->SetOpacity();

	dcmreader2->SetFileName(argv[3]);
	dcmreader2->Update();
	actor[1]->GetMapper()->SetInputData(dcmreader2->GetOutput());
	actor[1]->SetOpacity(0.5);

	//actor[1]->AddPosition(100,100,100);

	renderer[0]->AddActor(actor[0]);
	renderer[1]->AddActor(actor[1]);

	renderer[0]->SetLayer(0);
	//renderer[0]->SetBackground(1.0, 0.0, 0.0);
	renderer[1]->SetLayer(1);

	vtkSmartPointer<vtkRenderWindow> renwin
		= vtkSmartPointer<vtkRenderWindow>::New();

	qvtk->SetRenderWindow(renwin);
	renwin->SetNumberOfLayers(2);
	renwin->AddRenderer(renderer[0]);
	renwin->AddRenderer(renderer[1]);
	renwin->Render();



}

template <typename TRegistration>
class RegistrationInterfaceCommand : public itk::Command
{
public:
	typedef  RegistrationInterfaceCommand   Self;
	typedef  itk::Command                   Superclass;
	typedef  itk::SmartPointer<Self>        Pointer;
	itkNewMacro(Self);

protected:
	RegistrationInterfaceCommand() {};
public:
	typedef   TRegistration      RegistrationType;
	typedef   RegistrationType * RegistrationPointer;
	typedef   itk::RegularStepGradientDescentOptimizerv4<double>  OptimizerType;
	typedef   OptimizerType * OptimizerPointer;
	void Execute(itk::Object * object,
		const itk::EventObject & event) ITK_OVERRIDE
	{
		if (!(itk::MultiResolutionIterationEvent().CheckEvent(&event)))
		{
			return;
		}


		// Software Guide : BeginLatex
		//
		// We then convert the input object pointer to a RegistrationPointer.
		// Note that no error checking is done here to verify the
		// \code{dynamic\_cast} was successful since we know the actual object
		// is a registration method. Then we ask for the optimizer object
		// from the registration method.
		//
		// Software Guide : EndLatex

		// Software Guide : BeginCodeSnippet
		RegistrationPointer registration =
			static_cast<RegistrationPointer>(object);
		OptimizerPointer optimizer = static_cast< OptimizerPointer >(
			registration->GetModifiableOptimizer());
		// Software Guide : EndCodeSnippet

		unsigned int currentLevel = registration->GetCurrentLevel();
		typename RegistrationType::ShrinkFactorsPerDimensionContainerType shrinkFactors =
			registration->GetShrinkFactorsPerDimension(currentLevel);
		typename RegistrationType::SmoothingSigmasArrayType smoothingSigmas =
			registration->GetSmoothingSigmasPerLevel();

		std::cout << "-------------------------------------" << std::endl;
		std::cout << " Current level = " << currentLevel << std::endl;
		std::cout << "    shrink factor = " << shrinkFactors << std::endl;
		std::cout << "    smoothing sigma = " << smoothingSigmas[currentLevel] << std::endl;
		std::cout << std::endl;

		// Software Guide : BeginLatex
		//
		// If this is the first resolution level we set the learning rate
		// (representing the first step size) and the minimum step length (representing
		// the convergence criterion) to large values.  At each subsequent resolution
		// level, we will reduce the minimum step length by a factor of 5 in order to
		// allow the optimizer to focus on progressively smaller regions. The learning
		// rate is set up to the current step length. In this way, when the
		// optimizer is reinitialized at the beginning of the registration process for
		// the next level, the step length will simply start with the last value used
		// for the previous level. This will guarantee the continuity of the path
		// taken by the optimizer through the parameter space.
		//
		// Software Guide : EndLatex

		// Software Guide : BeginCodeSnippet
		if (registration->GetCurrentLevel() == 0)
		{
			optimizer->SetLearningRate(16.00);
			optimizer->SetMinimumStepLength(2.5);
		}
		else
		{
			optimizer->SetLearningRate(optimizer->GetCurrentStepLength());
			optimizer->SetMinimumStepLength(
				optimizer->GetMinimumStepLength() * 0.2);
		}
		// Software Guide : EndCodeSnippet
	}

	// Software Guide : BeginLatex
	//
	// Another version of the \code{Execute()} method accepting a \code{const}
	// input object is also required since this method is defined as pure virtual
	// in the base class.  This version simply returns without taking any action.
	//
	// Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	void Execute(const itk::Object *, const itk::EventObject &) ITK_OVERRIDE
	{
		return;
	}
};
class CommandIterationUpdate_Multi : public itk::Command
{
public:
	typedef  CommandIterationUpdate_Multi   Self;
	typedef  itk::Command             Superclass;
	typedef  itk::SmartPointer<Self>  Pointer;
	itkNewMacro(Self);

protected:
	CommandIterationUpdate_Multi() : m_CumulativeIterationIndex(0) {};

public:
	typedef   itk::RegularStepGradientDescentOptimizerv4<double>  OptimizerType;
	typedef   const OptimizerType *                               OptimizerPointer;

	void Execute(itk::Object *caller, const itk::EventObject & event) ITK_OVERRIDE
	{
		Execute((const itk::Object *)caller, event);
	}

	void Execute(const itk::Object * object, const itk::EventObject & event) ITK_OVERRIDE
	{
		OptimizerPointer optimizer = static_cast< OptimizerPointer >(object);
		if (!(itk::IterationEvent().CheckEvent(&event)))
		{
			return;
		}
		std::cout << optimizer->GetCurrentIteration() << "   ";
		std::cout << optimizer->GetValue() << "   ";
		std::cout << optimizer->GetCurrentPosition() << "   ";
		std::cout << m_CumulativeIterationIndex++ << std::endl;
	}
private:
	unsigned int m_CumulativeIterationIndex;
};
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainT1SliceBorder20.png}
//    INPUTS:  {BrainProtonDensitySliceShifted13x17y.png}
//    OUTPUTS: {MultiResImageRegistration1Output.png}
//    ARGUMENTS: 128
//    OUTPUTS: {MultiResImageRegistration1CheckerboardBefore.png}
//    OUTPUTS: {MultiResImageRegistration1CheckerboardAfter.png}
//  Software Guide : EndCommandLineArgs
void reg_Multi(int argc, char* argv[], QVTKWidget* qvtk)
{

	if (argc < 4)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0];
		std::cerr << " fixedImageFile  movingImageFile ";
		std::cerr << " outputImagefile [backgroundGrayLevel]";
		std::cerr << " [checkerBoardBefore] [checkerBoardAfter]";
		std::cerr << " [numberOfBins] " << std::endl;
		//return EXIT_FAILURE;
	}
	typedef itk::GDCMImageIO      ImageIOType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	const    unsigned int    Dimension = 2;
	typedef  float           PixelType;

	const std::string fixedImageFile = argv[1];
	const std::string movingImageFile = argv[2];
	const std::string outImagefile = argv[3];
	const PixelType backgroundGrayLevel = (argc >4) ? atoi(argv[4]) : 100;
	const std::string checkerBoardBefore = (argc >5) ? argv[5] : "";
	const std::string checkerBoardAfter = (argc >6) ? argv[6] : "";
	const int numberOfBins = (argc >7) ? atoi(argv[7]) : 0;

	typedef itk::Image< PixelType, Dimension >  FixedImageType;
	typedef itk::Image< PixelType, Dimension >  MovingImageType;

	//  Software Guide : BeginLatex
	//
	//  The fixed and moving image types are defined as in previous
	//  examples. The downsampled images for different resolution levels
	//  are created internally by the registration method based on the
	//  values provided for \emph{ShrinkFactor} and \emph{SmoothingSigma}
	//  vectors.
	//
	//  The types for the registration components are then derived using
	//  the fixed and moving image type, as in previous examples.
	//
	//  Software Guide : EndLatex

	typedef itk::TranslationTransform< double, Dimension >              TransformType;

	typedef itk::RegularStepGradientDescentOptimizerv4<double>          OptimizerType;

	typedef itk::MattesMutualInformationImageToImageMetricv4<
		FixedImageType,
		MovingImageType > MetricType;
	typedef itk::ImageRegistrationMethodv4<
		FixedImageType,
		MovingImageType,
		TransformType >                 RegistrationType;

	//  All the components are instantiated using their \code{New()} method
	//  and connected to the registration object as in previous example.
	//
	TransformType::Pointer      transform = TransformType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	MetricType::Pointer         metric = MetricType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();

	registration->SetOptimizer(optimizer);
	registration->SetMetric(metric);

	typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;

	FixedImageReaderType::Pointer  fixedImageReader =
		FixedImageReaderType::New();
	MovingImageReaderType::Pointer movingImageReader =
		MovingImageReaderType::New();
	fixedImageReader->SetImageIO(gdcmIO);
	fixedImageReader->SetFileName(fixedImageFile);
	movingImageReader->SetImageIO(gdcmIO);
	movingImageReader->SetFileName(movingImageFile);

	registration->SetFixedImage(fixedImageReader->GetOutput());
	registration->SetMovingImage(movingImageReader->GetOutput());


	typedef OptimizerType::ParametersType ParametersType;
	ParametersType initialParameters(transform->GetNumberOfParameters());

	initialParameters[0] = 0.0;  // Initial offset in mm along X
	initialParameters[1] = 0.0;  // Initial offset in mm along Y

	transform->SetParameters(initialParameters);

	registration->SetInitialTransform(transform);
	registration->InPlaceOn();

	metric->SetNumberOfHistogramBins(24);

	if (argc > 7)
	{
		// optionally, override the values with numbers taken from the command line arguments.
		metric->SetNumberOfHistogramBins(numberOfBins);
	}

	//  Software Guide : BeginLatex
	//
	//  To set the optimizer parameters, note that \emph{LearningRate}
	//  and \emph{MinimumStepLength} are set in the obsever at the begining
	//  of each resolution level. The other optimizer parameters are set
	//  as follows.
	//
	//  Software Guide : EndLatex

	//  Software Guide : BeginCodeSnippet
	optimizer->SetNumberOfIterations(200);
	optimizer->SetRelaxationFactor(0.5);
	// Software Guide : EndCodeSnippet

	// Create the Command observer and register it with the optimizer.
	//
	CommandIterationUpdate_Multi::Pointer observer = CommandIterationUpdate_Multi::New();
	optimizer->AddObserver(itk::IterationEvent(), observer);


	//  Software Guide : BeginLatex
	//
	//  We set the number of multi-resolution levels to three and set
	//  the corresponding shrink factor and smoothing sigma values for each
	//  resolution level. Using smoothing in the subsampled images in
	//  low-resolution levels can avoid large fluctuations in the
	//  metric function, which prevents the optimizer from becoming trapped in
	//  local minima. In this simple example we have no smoothing, and we have
	//  used small shrinkings for the first two resolution levels.
	//
	//  \index{itk::Image\-Registration\-Methodv4!SetNumberOfLevels()}
	//  \index{itk::Image\-Registration\-Methodv4!SetShrinkFactorsPerLevel()}
	//  \index{itk::Image\-Registration\-Methodv4!SetSmoothingSigmasPerLevel()}
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	const unsigned int numberOfLevels = 3;

	RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
	shrinkFactorsPerLevel.SetSize(3);
	shrinkFactorsPerLevel[0] = 3;
	shrinkFactorsPerLevel[1] = 2;
	shrinkFactorsPerLevel[2] = 1;

	RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
	smoothingSigmasPerLevel.SetSize(3);
	smoothingSigmasPerLevel[0] = 0;
	smoothingSigmasPerLevel[1] = 0;
	smoothingSigmasPerLevel[2] = 0;

	registration->SetNumberOfLevels(numberOfLevels);
	registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);
	registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
	// Software Guide : EndCodeSnippet

	//  Software Guide : BeginLatex
	//
	//  Once all the registration components are in place we can create
	//  an instance of our interface command and connect it to the
	//  registration object using the \code{AddObserver()} method.
	//
	//  Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	typedef RegistrationInterfaceCommand<RegistrationType> CommandType;
	CommandType::Pointer command = CommandType::New();

	registration->AddObserver(itk::MultiResolutionIterationEvent(), command);
	// Software Guide : EndCodeSnippet

	//  Software Guide : BeginLatex
	//
	//  Then we trigger the registration process by calling \code{Update()}.
	//
	//  Software Guide : EndLatex

	try
	{
		registration->Update();
		std::cout << "Optimizer stop condition: "
			<< registration->GetOptimizer()->GetStopConditionDescription()
			<< std::endl;
	}
	catch (itk::ExceptionObject & err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
		//return EXIT_FAILURE;
	}

	ParametersType finalParameters = transform->GetParameters();

	double TranslationAlongX = finalParameters[0];
	double TranslationAlongY = finalParameters[1];

	unsigned int numberOfIterations = optimizer->GetCurrentIteration();

	double bestValue = optimizer->GetValue();


	// Print out results
	//
	std::cout << "Result = " << std::endl;
	std::cout << " Translation X = " << TranslationAlongX << std::endl;
	std::cout << " Translation Y = " << TranslationAlongY << std::endl;
	std::cout << " Iterations    = " << numberOfIterations << std::endl;
	std::cout << " Metric value  = " << bestValue << std::endl;

	typedef itk::ResampleImageFilter<
		MovingImageType,
		FixedImageType >    ResampleFilterType;

	ResampleFilterType::Pointer resample = ResampleFilterType::New();

	resample->SetTransform(transform);
	resample->SetInput(movingImageReader->GetOutput());

	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();


	resample->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resample->SetOutputOrigin(fixedImage->GetOrigin());
	resample->SetOutputSpacing(fixedImage->GetSpacing());
	resample->SetOutputDirection(fixedImage->GetDirection());
	resample->SetDefaultPixelValue(backgroundGrayLevel);

	typedef  float  OutputPixelType;

	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

	typedef itk::CastImageFilter<
		FixedImageType,
		OutputImageType > CastFilterType;

	typedef itk::ImageFileWriter< OutputImageType >  WriterType;


	WriterType::Pointer      writer = WriterType::New();
	CastFilterType::Pointer  caster = CastFilterType::New();

	writer->SetImageIO(gdcmIO);
	writer->UseInputMetaDataDictionaryOff();
	writer->SetFileName(outImagefile);


	caster->SetInput(resample->GetOutput());
	writer->SetInput(caster->GetOutput());
	writer->Update();

	//
	// Generate checkerboards before and after registration
	//
	typedef itk::CheckerBoardImageFilter< FixedImageType > CheckerBoardFilterType;

	CheckerBoardFilterType::Pointer checker = CheckerBoardFilterType::New();

	checker->SetInput1(fixedImage);
	checker->SetInput2(resample->GetOutput());

	caster->SetInput(checker->GetOutput());
	writer->SetInput(caster->GetOutput());

	resample->SetDefaultPixelValue(0);

	// Before registration
	TransformType::Pointer identityTransform = TransformType::New();
	identityTransform->SetIdentity();
	resample->SetTransform(identityTransform);

	if (checkerBoardBefore != std::string(""))
	{
		writer->SetFileName(checkerBoardBefore);
		writer->Update();
	}


	// After registration
	resample->SetTransform(transform);
	if (checkerBoardAfter != std::string(""))
	{
		writer->SetFileName(checkerBoardAfter);
		writer->Update();
	}
	vtkSmartPointer<vtkDICOMImageReader> dcmreader;
	vtkSmartPointer<vtkDICOMImageReader> dcmreader2;
	vtkSmartPointer<vtkRenderer>  renderer[2];
	vtkSmartPointer<vtkImageActor> actor[2];

	renderer[0] = vtkSmartPointer<vtkRenderer>::New();
	dcmreader = vtkSmartPointer<vtkDICOMImageReader>::New();
	actor[0] = vtkSmartPointer<vtkImageActor>::New();

	renderer[1] = vtkSmartPointer<vtkRenderer>::New();
	dcmreader2 = vtkSmartPointer<vtkDICOMImageReader>::New();
	actor[1] = vtkSmartPointer<vtkImageActor>::New();

	dcmreader->SetFileName(argv[1]);
	dcmreader->Update();
	actor[0]->GetMapper()->SetInputData(dcmreader->GetOutput());

	//actor[0]->SetOpacity();

	dcmreader2->SetFileName(argv[3]);
	dcmreader2->Update();
	actor[1]->GetMapper()->SetInputData(dcmreader2->GetOutput());
	actor[1]->SetOpacity(0.5);
	//actor[1]->AddPosition(100,100,100);

	renderer[0]->AddActor(actor[0]);
	renderer[1]->AddActor(actor[1]);

	renderer[0]->SetLayer(0);
	renderer[1]->SetLayer(1);

	vtkSmartPointer<vtkRenderWindow> renwin
		= vtkSmartPointer<vtkRenderWindow>::New();

	qvtk->SetRenderWindow(renwin);
	renwin->SetNumberOfLayers(2);
	renwin->AddRenderer(renderer[0]);
	renwin->AddRenderer(renderer[1]);
	renwin->Render();
}