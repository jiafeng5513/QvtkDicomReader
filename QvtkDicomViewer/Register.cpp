#include "Register.h"
#include <QFileDialog>

#include<head_all.h>//这是一个不可控点

#include "itkGDCMImageIO.h"  
#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRegularStepGradientDescentBaseOptimizer.h"

/*
 * 构造
 */
Register::Register(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	for(int i=0;i<4;i++ )
	{
		actor[i] = vtkSmartPointer<vtkImageActor>::New();
		renderer[i] = vtkSmartPointer<vtkRenderer>::New();
		renderWindowInteractor[i] = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		style[i] = vtkSmartPointer<vtkInteractorStyleImage>::New();
		connector[i] = ConnectorType::New();
	}
	m_output_widgets[0]=ui.qvtkWidget_Registration_UL;
	m_output_widgets[1]=ui.qvtkWidget_Registration_UR;
	m_output_widgets[2]=ui.qvtkWidget_Registration_DL;
	m_output_widgets[3]=ui.qvtkWidget_Registration_DR;

}
/*
 * 析构
 */
Register::~Register()
{
}
/*
 * 平移变换-重构完成
 */
void Register::TranslationReg(char* argv[])
{

	typedef itk::Image< PixelType, 2 >  FixedImageType;
	typedef itk::Image< PixelType, 2 >  MovingImageType;
	typedef itk::TranslationTransform< double, 2 > TransformType;//配准方案
	typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;//优化求解器
																			 //用于比较两张图片的相似程度
	typedef itk::MeanSquaresImageToImageMetricv4<FixedImageType, MovingImageType >  MetricType;
	//用于关联整个处理流程
	typedef itk::ImageRegistrationMethodv4<FixedImageType, MovingImageType, TransformType >  RegistrationType;
	//实例化使用智能指针和工厂方法
	MetricType::Pointer         metric = MetricType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();
	//相关的组件挂载到registration上
	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);
	//加入线性插值器
	typedef itk::LinearInterpolateImageFunction<FixedImageType, double > FixedLinearInterpolatorType;
	typedef itk::LinearInterpolateImageFunction<MovingImageType, double > MovingLinearInterpolatorType;
	FixedLinearInterpolatorType::Pointer fixedInterpolator = FixedLinearInterpolatorType::New();
	MovingLinearInterpolatorType::Pointer movingInterpolator = MovingLinearInterpolatorType::New();
	metric->SetFixedInterpolator(fixedInterpolator);
	metric->SetMovingInterpolator(movingInterpolator);
	//绘图和输出准备
	//typedef unsigned char                            OutputPixelType;
	

	

	//加入输入图片文件读取器(注意此处使用的是ITK的类型)
	typedef itk::ImageFileReader< FixedImageType  >   FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType >   MovingImageReaderType;
	FixedImageReaderType::Pointer   fixedImageReader = FixedImageReaderType::New();
	MovingImageReaderType::Pointer  movingImageReader = MovingImageReaderType::New();
	fixedImageReader->SetFileName(argv[1]);
	movingImageReader->SetFileName(argv[2]);
	registration->SetFixedImage(fixedImageReader->GetOutput());
	registration->SetMovingImage(movingImageReader->GetOutput());
	connector[0]->SetInput(fixedImageReader->GetOutput());//显示输入1
	connector[1]->SetInput(movingImageReader->GetOutput());//显示输入2


	//加入平移变换器(给移动过的那张图用的)
	TransformType::Pointer movingInitialTransform = TransformType::New();
	TransformType::ParametersType initialParameters(movingInitialTransform->GetNumberOfParameters());
	initialParameters[0] = 0.0;  // Initial offset in mm along X
	initialParameters[1] = 0.0;  // Initial offset in mm along Y
	movingInitialTransform->SetParameters(initialParameters);
	registration->SetMovingInitialTransform(movingInitialTransform);

	//加入恒等变换器(给没移动过的那张图用的)
	TransformType::Pointer   identityTransform = TransformType::New();
	identityTransform->SetIdentity();
	registration->SetFixedInitialTransform(identityTransform);

	//梯度下降求解器的初始化
	optimizer->SetLearningRate(4);
	optimizer->SetMinimumStepLength(0.001);
	optimizer->SetRelaxationFactor(0.5);
	optimizer->SetNumberOfIterations(200);

	//平滑
	const unsigned int numberOfLevels = 1;
	RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
	shrinkFactorsPerLevel.SetSize(1);
	shrinkFactorsPerLevel[0] = 1;
	RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
	smoothingSigmasPerLevel.SetSize(1);
	smoothingSigmasPerLevel[0] = 0;
	registration->SetNumberOfLevels(numberOfLevels);
	registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
	registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);

	//触发registration
	try
	{
		registration->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		return /*EXIT_FAILURE*/;
	}
	//获取结果,结果两个方向上的变换
	TransformType::ConstPointer transform = registration->GetTransform();
	TransformType::ParametersType finalParameters = transform->GetParameters();
	const double TranslationAlongX = finalParameters[0];
	const double TranslationAlongY = finalParameters[1];

	//查询收敛所需的迭代次数,迭代次数过大可能是学习率过小导致的(仅供调试,没有后续使用)
	const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
	//最佳值(仅供调试,没有后续使用)
	const double bestValue = optimizer->GetValue();
	//复合变换类型的定义何实例的定义以及绑定
	//最终的结果是靠这个来输出的
	typedef itk::CompositeTransform<double, 2 > CompositeTransformType;
	CompositeTransformType::Pointer outputCompositeTransform = CompositeTransformType::New();
	outputCompositeTransform->AddTransform(movingInitialTransform);
	outputCompositeTransform->AddTransform(registration->GetModifiableTransform());

	//重采样,对待配准图像使用上述变换进行采样
	typedef itk::ResampleImageFilter<MovingImageType, FixedImageType >  ResampleFilterType;
	ResampleFilterType::Pointer resampler = ResampleFilterType::New();
	resampler->SetInput(movingImageReader->GetOutput());
	resampler->SetTransform(outputCompositeTransform);
	//给重采样器填充其他参数
	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(fixedImage->GetOrigin());
	resampler->SetOutputSpacing(fixedImage->GetSpacing());
	resampler->SetOutputDirection(fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(100);
	//准备输出

	typedef itk::CastImageFilter<FixedImageType, OutputImageType >          CastFilterType;
	typedef itk::ImageFileWriter< OutputImageType >  WriterType;
	//转接器

	//输出1:输出的是配准结果
	//WriterType::Pointer      writer = WriterType::New();
	CastFilterType::Pointer  caster = CastFilterType::New();
	connector[3]->SetInput(caster->GetOutput());//显示输出结果
	//writer->SetFileName(argv[3]);
	caster->SetInput(resampler->GetOutput());
	//writer->SetInput(caster->GetOutput());
	//writer->Update();

	//输出2:视差图A
	typedef itk::SubtractImageFilter<FixedImageType, FixedImageType, FixedImageType > DifferenceFilterType;
	DifferenceFilterType::Pointer difference = DifferenceFilterType::New();
	difference->SetInput1(fixedImageReader->GetOutput());
	difference->SetInput2(resampler->GetOutput());
	typedef itk::RescaleIntensityImageFilter<FixedImageType, OutputImageType > RescalerType;
	RescalerType::Pointer intensityRescaler = RescalerType::New();
	intensityRescaler->SetInput(difference->GetOutput());
	intensityRescaler->SetOutputMinimum(0);
	intensityRescaler->SetOutputMaximum(255);
	resampler->SetDefaultPixelValue(1);
	connector[2]->SetInput(intensityRescaler->GetOutput());//显示计算过程
	//WriterType::Pointer writer2 = WriterType::New();
	//writer2->SetInput(intensityRescaler->GetOutput());
	//writer2->SetFileName(argv[4]);
	//writer2->Update();

	//输出3:视差图B
	resampler->SetTransform(identityTransform);
	//writer2->SetFileName(argv[5]);
	//writer2->Update();

	updateOutputImage();
}
/*
 * 中心相似二维变换-重构完成
 */
void Register::CenteredSimilarityTransformReg(char * argv[])
{
	typedef itk::Image< PixelType, 2 >  FixedImageType;
	typedef itk::Image< PixelType, 2 >  MovingImageType;
	//中心相似二维变换
	typedef itk::CenteredSimilarity2DTransform< double > TransformType;
	typedef itk::RegularStepGradientDescentOptimizerv4<double>         OptimizerType;
	typedef itk::MeanSquaresImageToImageMetricv4< FixedImageType, MovingImageType >    MetricType;
	typedef itk::ImageRegistrationMethodv4< FixedImageType, MovingImageType, TransformType > RegistrationType;

	MetricType::Pointer         metric = MetricType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();
	//registration负责串联各个组件
	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);
	//绘图和输出准备
	//typedef unsigned char                            OutputPixelType;




	//变换器
	TransformType::Pointer  transform = TransformType::New();
	//读入输入文件
	typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();
	MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();
	fixedImageReader->SetFileName(argv[1]);
	movingImageReader->SetFileName(argv[2]);
	registration->SetFixedImage(fixedImageReader->GetOutput());
	registration->SetMovingImage(movingImageReader->GetOutput());
	connector[0]->SetInput(fixedImageReader->GetOutput());//显示输入1
	connector[1]->SetInput(movingImageReader->GetOutput());//显示输入2
	//变换器初始化
	typedef itk::CenteredTransformInitializer<TransformType, FixedImageType, MovingImageType > TransformInitializerType;
	TransformInitializerType::Pointer initializer = TransformInitializerType::New();
	initializer->SetTransform(transform);
	initializer->SetFixedImage(fixedImageReader->GetOutput());
	initializer->SetMovingImage(movingImageReader->GetOutput());
	initializer->MomentsOn();
	initializer->InitializeTransform();
	double initialScale = atof(argv[7]);
	double initialAngle = atof(argv[8]);
	transform->SetScale(initialScale);
	transform->SetAngle(initialAngle);
	//绑定变换器
	registration->SetInitialTransform(transform);
	registration->InPlaceOn();
	//优化求解器初始化
	typedef OptimizerType::ScalesType OptimizerScalesType;
	OptimizerScalesType optimizerScales(transform->GetNumberOfParameters());
	const double translationScale = 1.0 / 100.0;
	optimizerScales[0] = 10.0;
	optimizerScales[1] = 1.0;
	optimizerScales[2] = translationScale;
	optimizerScales[3] = translationScale;
	optimizerScales[4] = translationScale;
	optimizerScales[5] = translationScale;
	optimizer->SetScales(optimizerScales);
	double steplength = atof(argv[6]);
	optimizer->SetLearningRate(steplength);
	optimizer->SetMinimumStepLength(0.0001);
	optimizer->SetNumberOfIterations(500);
	//进行配准
	const unsigned int numberOfLevels = 1;
	RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
	shrinkFactorsPerLevel.SetSize(1);
	shrinkFactorsPerLevel[0] = 1;
	RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
	smoothingSigmasPerLevel.SetSize(1);
	smoothingSigmasPerLevel[0] = 0;
	registration->SetNumberOfLevels(numberOfLevels);
	registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
	registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);

	try
	{
		registration->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		return /*EXIT_FAILURE*/;
	}
	//一组调试信息(仅供调试,不用做后续计算)
	TransformType::ParametersType finalParameters = transform->GetParameters();
	const double finalScale = finalParameters[0];
	const double finalAngle = finalParameters[1];
	const double finalRotationCenterX = finalParameters[2];
	const double finalRotationCenterY = finalParameters[3];
	const double finalTranslationX = finalParameters[4];
	const double finalTranslationY = finalParameters[5];
	const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
	const double bestValue = optimizer->GetValue();
	const double finalAngleInDegrees = finalAngle * 180.0 / itk::Math::pi;

	//重采样
	typedef itk::ResampleImageFilter< MovingImageType, FixedImageType > ResampleFilterType;
	ResampleFilterType::Pointer resampler = ResampleFilterType::New();

	resampler->SetTransform(transform);
	resampler->SetInput(movingImageReader->GetOutput());

	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(fixedImage->GetOrigin());
	resampler->SetOutputSpacing(fixedImage->GetSpacing());
	resampler->SetOutputDirection(fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(100);

	/*typedef  unsigned char  OutputPixelType;//png输出必须用unsigned char
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;*/
	typedef itk::CastImageFilter< FixedImageType, OutputImageType >CastFilterType;
	typedef itk::ImageFileWriter< OutputImageType >  WriterType;

	//输出1
	//WriterType::Pointer      writer = WriterType::New();
	CastFilterType::Pointer  caster = CastFilterType::New();
	//writer->SetFileName(argv[3]);
	connector[3]->SetInput(caster->GetOutput());//到底放caster前面还是后面
	caster->SetInput(resampler->GetOutput());
	
	//writer->SetInput(caster->GetOutput());
	//writer->Update();

	//输出2
	typedef itk::SubtractImageFilter<FixedImageType, FixedImageType, FixedImageType > DifferenceFilterType;
	DifferenceFilterType::Pointer difference = DifferenceFilterType::New();
	typedef itk::RescaleIntensityImageFilter<FixedImageType, OutputImageType >   RescalerType;
	RescalerType::Pointer intensityRescaler = RescalerType::New();
	intensityRescaler->SetInput(difference->GetOutput());
	intensityRescaler->SetOutputMinimum(0);
	intensityRescaler->SetOutputMaximum(255);
	difference->SetInput1(fixedImageReader->GetOutput());
	difference->SetInput2(resampler->GetOutput());
	resampler->SetDefaultPixelValue(1);
	
	connector[2]->SetInput(intensityRescaler->GetOutput());

	//WriterType::Pointer writer2 = WriterType::New();
	//writer2->SetInput(intensityRescaler->GetOutput());
	//writer2->SetFileName(argv[5]);
	//writer2->Update();

	//输出3
	typedef itk::IdentityTransform< double, 2 > IdentityTransformType;
	IdentityTransformType::Pointer identity = IdentityTransformType::New();
	resampler->SetTransform(identity);
	//writer2->SetFileName(argv[4]);
	//writer2->Update();


	updateOutputImage();


	return /*EXIT_SUCCESS*/;
}
/*
 * 仿射变换-重构完成
 */
void Register::AffineTransformReg(char * argv[])
{
	typedef itk::Image< PixelType, 2 >  FixedImageType;
	typedef itk::Image< PixelType, 2 >  MovingImageType;
	//放射变换
	typedef itk::AffineTransform< double, 2  > TransformType;
	typedef itk::RegularStepGradientDescentOptimizerv4<double>       OptimizerType;
	typedef itk::MeanSquaresImageToImageMetricv4<FixedImageType, MovingImageType > MetricType;
	typedef itk::ImageRegistrationMethodv4<FixedImageType, MovingImageType, TransformType > RegistrationType;

	MetricType::Pointer         metric = MetricType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();
	//registration负责串联所有的组件
	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);
	//变换器
	TransformType::Pointer  transform = TransformType::New();

	//绘图和输出准备
	//typedef unsigned char                            OutputPixelType;
	


	//读入两个输入文件
	typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();
	MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();
	fixedImageReader->SetFileName(argv[1]);
	movingImageReader->SetFileName(argv[2]);
	registration->SetFixedImage(fixedImageReader->GetOutput());
	registration->SetMovingImage(movingImageReader->GetOutput());
	connector[0]->SetInput(fixedImageReader->GetOutput());
	connector[1]->SetInput(movingImageReader->GetOutput());
	//初始化变换器
	typedef itk::CenteredTransformInitializer<TransformType, FixedImageType, MovingImageType > TransformInitializerType;
	TransformInitializerType::Pointer initializer = TransformInitializerType::New();
	initializer->SetTransform(transform);
	initializer->SetFixedImage(fixedImageReader->GetOutput());
	initializer->SetMovingImage(movingImageReader->GetOutput());
	initializer->MomentsOn();
	initializer->InitializeTransform();
	registration->SetInitialTransform(transform);
	registration->InPlaceOn();
	//优化求解器初始化
	double translationScale = 1.0 / 1000.0;
	typedef OptimizerType::ScalesType       OptimizerScalesType;
	OptimizerScalesType optimizerScales(transform->GetNumberOfParameters());
	optimizerScales[0] = 1.0;
	optimizerScales[1] = 1.0;
	optimizerScales[2] = 1.0;
	optimizerScales[3] = 1.0;
	optimizerScales[4] = translationScale;
	optimizerScales[5] = translationScale;
	optimizer->SetScales(optimizerScales);

	double steplength = atof(argv[6]);
	unsigned int maxNumberOfIterations = atoi(argv[7]);

	optimizer->SetLearningRate(steplength);
	optimizer->SetMinimumStepLength(0.0001);
	optimizer->SetNumberOfIterations(maxNumberOfIterations);
	//平滑
	const unsigned int numberOfLevels = 1;
	RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
	shrinkFactorsPerLevel.SetSize(1);
	shrinkFactorsPerLevel[0] = 1;
	RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
	smoothingSigmasPerLevel.SetSize(1);
	smoothingSigmasPerLevel[0] = 0;
	registration->SetNumberOfLevels(numberOfLevels);
	registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
	registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);
	//触发registration
	try
	{
		registration->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << err << std::endl;
		return;
	}
	//一组调试信息(只用来调试,没有后续使用)
	const TransformType::ParametersType finalParameters = registration->GetOutput()->Get()->GetParameters();
	const double finalRotationCenterX = transform->GetCenter()[0];
	const double finalRotationCenterY = transform->GetCenter()[1];
	const double finalTranslationX = finalParameters[4];
	const double finalTranslationY = finalParameters[5];

	const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
	const double bestValue = optimizer->GetValue();


	//Compute the rotation angle and scaling from SVD of the matrix
	// \todo Find a way to figure out if the scales are along X or along Y.
	// VNL returns the eigenvalues ordered from largest to smallest.

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

	//std::cout << " Scale 1         = " << svd.W(0) << std::endl;
	//std::cout << " Scale 2         = " << svd.W(1) << std::endl;
	//std::cout << " Angle (degrees) = " << angleInDegrees << std::endl;


	// 重采样
	typedef itk::ResampleImageFilter<MovingImageType, FixedImageType >  ResampleFilterType;
	ResampleFilterType::Pointer resampler = ResampleFilterType::New();
	resampler->SetTransform(transform);
	resampler->SetInput(movingImageReader->GetOutput());
	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(fixedImage->GetOrigin());
	resampler->SetOutputSpacing(fixedImage->GetSpacing());
	resampler->SetOutputDirection(fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(100);

	//typedef  unsigned char  OutputPixelType;
	//typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
	typedef itk::CastImageFilter<FixedImageType, OutputImageType > CastFilterType;
	typedef itk::ImageFileWriter< OutputImageType >  WriterType;

	//输出1
	//WriterType::Pointer      writer = WriterType::New();
	CastFilterType::Pointer  caster = CastFilterType::New();
	//writer->SetFileName(argv[3]);
	connector[3]->SetInput(caster->GetOutput());
	caster->SetInput(resampler->GetOutput());
	//writer->SetInput(caster->GetOutput());
	//writer->Update();

	//输出2
	typedef itk::SubtractImageFilter<FixedImageType, FixedImageType, FixedImageType > DifferenceFilterType;
	DifferenceFilterType::Pointer difference = DifferenceFilterType::New();
	difference->SetInput1(fixedImageReader->GetOutput());
	difference->SetInput2(resampler->GetOutput());
	//WriterType::Pointer writer2 = WriterType::New();

	typedef itk::RescaleIntensityImageFilter<FixedImageType, OutputImageType >   RescalerType;
	RescalerType::Pointer intensityRescaler = RescalerType::New();
	intensityRescaler->SetInput(difference->GetOutput());
	intensityRescaler->SetOutputMinimum(0);
	intensityRescaler->SetOutputMaximum(255);

	//writer2->SetInput(intensityRescaler->GetOutput());
	connector[2]->SetInput(intensityRescaler->GetOutput());
	resampler->SetDefaultPixelValue(1);
	//writer2->SetFileName(argv[5]);
	//writer2->Update();

	//输出3
	typedef itk::IdentityTransform< double, 2 > IdentityTransformType;
	IdentityTransformType::Pointer identity = IdentityTransformType::New();
	resampler->SetTransform(identity);
	//writer2->SetFileName(argv[4]);
	//writer2->Update();
	updateOutputImage();
	return;
}
/*
 * 多种方式变换
 */
void Register::MultiTransformReg(char * argv[])
{
	//转移参数
	const std::string fixedImageFile = argv[1];
	const std::string movingImageFile = argv[2];
	const std::string outImagefile = argv[3];
	const PixelType backgroundGrayLevel = atoi(argv[4]);
	const std::string checkerBoardBefore = argv[5];
	const std::string checkerBoardAfter = argv[6];
	const int numberOfBins = 0;

	typedef itk::Image< PixelType, 2 >  FixedImageType;
	typedef itk::Image< PixelType, 2 >  MovingImageType;
	typedef itk::TranslationTransform< double, 2 >              TransformType;
	typedef itk::RegularStepGradientDescentOptimizerv4<double>          OptimizerType;
	typedef itk::MattesMutualInformationImageToImageMetricv4<FixedImageType, MovingImageType > MetricType;
	typedef itk::ImageRegistrationMethodv4<FixedImageType, MovingImageType, TransformType > RegistrationType;
	//registration负责串联各种组件
	TransformType::Pointer      transform = TransformType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	MetricType::Pointer         metric = MetricType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();
	registration->SetOptimizer(optimizer);
	registration->SetMetric(metric);
	//绘图和输出准备
	//typedef unsigned char                            OutputPixelType;
	//读取两幅输入文件
	typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();
	MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();
	fixedImageReader->SetFileName(fixedImageFile);
	movingImageReader->SetFileName(movingImageFile);
	registration->SetFixedImage(fixedImageReader->GetOutput());
	registration->SetMovingImage(movingImageReader->GetOutput());
	connector[0]->SetInput(fixedImageReader->GetOutput());
	connector[1]->SetInput(movingImageReader->GetOutput());
	//初始化
	typedef OptimizerType::ParametersType ParametersType;
	ParametersType initialParameters(transform->GetNumberOfParameters());
	initialParameters[0] = 0.0;  // Initial offset in mm along X
	initialParameters[1] = 0.0;  // Initial offset in mm along Y
	transform->SetParameters(initialParameters);
	registration->SetInitialTransform(transform);
	registration->InPlaceOn();
	metric->SetNumberOfHistogramBins(24);

	optimizer->SetNumberOfIterations(200);
	optimizer->SetRelaxationFactor(0.5);
	//平滑
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
	//触发registration
	try
	{
		registration->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
		return /*EXIT_FAILURE*/;
	}
	//一组测试数据(只用来测试,没有后续使用)
	ParametersType finalParameters = transform->GetParameters();
	double TranslationAlongX = finalParameters[0];
	double TranslationAlongY = finalParameters[1];
	unsigned int numberOfIterations = optimizer->GetCurrentIteration();
	double bestValue = optimizer->GetValue();

	//重采样
	typedef itk::ResampleImageFilter<MovingImageType, FixedImageType >    ResampleFilterType;
	ResampleFilterType::Pointer resample = ResampleFilterType::New();
	resample->SetTransform(transform);
	resample->SetInput(movingImageReader->GetOutput());
	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
	resample->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resample->SetOutputOrigin(fixedImage->GetOrigin());
	resample->SetOutputSpacing(fixedImage->GetSpacing());
	resample->SetOutputDirection(fixedImage->GetDirection());
	resample->SetDefaultPixelValue(backgroundGrayLevel);
	//准备输出
	//typedef  unsigned char  OutputPixelType;
	//typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
	typedef itk::CastImageFilter<FixedImageType, OutputImageType > CastFilterType;
	typedef itk::ImageFileWriter< OutputImageType >  WriterType;

	//输出1
	//WriterType::Pointer      writer = WriterType::New();
	CastFilterType::Pointer  caster = CastFilterType::New();
	//writer->SetFileName(outImagefile);
	
	caster->SetInput(resample->GetOutput());
	caster->Update();
connector[3]->SetInput(resample->GetOutput());
	//writer->SetInput(caster->GetOutput());
	//writer->Update();

	// 准备好棋盘格图像
	typedef itk::CheckerBoardImageFilter< FixedImageType > CheckerBoardFilterType;
	CheckerBoardFilterType::Pointer checker = CheckerBoardFilterType::New();
	checker->SetInput1(fixedImage);
	checker->SetInput2(resample->GetOutput());
	caster->SetInput(checker->GetOutput());
	
	//writer->SetInput(caster->GetOutput());
	resample->SetDefaultPixelValue(0);

	//输出:2 registration之前的棋盘格
	TransformType::Pointer identityTransform = TransformType::New();
	identityTransform->SetIdentity();
	resample->SetTransform(identityTransform);
	//writer->SetFileName(checkerBoardBefore);
	//writer->Update();

	// 输出3: registration之后的棋盘格
	resample->SetTransform(transform);
connector[2]->SetInput(caster->GetOutput());
	//writer->SetFileName(checkerBoardAfter);
	//writer->Update();
	updateOutputImage();
	return /*EXIT_SUCCESS*/;
}
/*
 * 刷新图片
 */
void Register::updateOutputImage()
{
	for (int i = 0; i<4; i++)
	{
		connector[i]->Update();
		actor[i]->GetMapper()->SetInputData(connector[i]->GetOutput());
		renderer[i]->AddActor(actor[i]);
		m_output_widgets[i]->GetRenderWindow()->AddRenderer(renderer[i]);
		//m_output_widgets[i]->GetRenderWindow()->Render();
		renderWindowInteractor[i]->SetRenderWindow(m_output_widgets[i]->GetRenderWindow());
		renderWindowInteractor[i]->SetInteractorStyle(style[i]);
		renderWindowInteractor[i]->Initialize();
	}
	for (int i = 0; i<4; i++)
	{
		m_output_widgets[i]->GetRenderWindow()->Render();
	}
	//下边的这个有阻塞,不要放在循环里
	renderWindowInteractor[0]->Start();
	renderWindowInteractor[1]->Start();
	renderWindowInteractor[2]->Start();
	renderWindowInteractor[3]->Start();
}

/*
 * 选择基准图片
 */
void Register::OnSelectImageFix()
{
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择基准图像"), NULL, tr("*.*"));
	ui.lineEdit_ImageFix->setText(fileName);
}
/*
 * 选择待配准图片
 */
void Register::OnSelectImageMove()
{
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择待配准图像"), NULL, tr("*.*"));
	ui.lineEdit_ImageMove->setText(fileName);
}
/*
 * 开始计算
 */
void Register::OnButtonOk()
{
	QString str = ui.lineEdit_ImageFix->text();
	QString str2 = ui.lineEdit_ImageMove->text();
	QByteArray arr = str.toLatin1();

	char *chr = arr.data();

	QByteArray arr2 = str.toLatin1();


	char *chr2 = arr2.data();

	Reg_enum  reg_enum = (Reg_enum)this->reg_count;
	char * argv[] = {
		"ImageRegistration1.exe",
		"D:\\Libraries\\ITK\\ITK_4.12.0\\ITK_src\\Examples\\Data\\BrainProtonDensitySliceBorder20.png",
		"D:\\Libraries\\ITK\\ITK_4.12.0\\ITK_src\\Examples\\Data\\BrainProtonDensitySliceShifted13x17y.png",
		"F:\\registration\\ImageRegistration1Output.png",
		"F:\\registration\\ImageRegistration1DifferenceAfter.png",
		"F:\\registration\\ImageRegistration1DifferenceBefore.png"
	};

	char * argv1[] = {
		"ImageRegistration7.exe",
		"D:\\Libraries\\ITK\\ITK_4.12.0\\ITK_src\\Examples\\Data\\BrainProtonDensitySliceBSplined10.png",
		"D:\\Libraries\\ITK\\ITK_4.12.0\\ITK_src\\Examples\\Data\\BrainProtonDensitySliceR10X13Y17S12.png",
		"F:\\registration\\ImageRegistration7Output.dcm",
		"F:\\registration\\ImageRegistration7DifferenceBefore.dcm",
		"F:\\registration\\ImageRegistration7DifferenceAfter.dcm",
		"1.0",
		"1.0",
		"0,0"
	};

	char * argv2[] = {
		"ImageRegistration9.exe ",//0
		"D:\\Libraries\\ITK\\ITK_4.12.0\\ITK_src\\Examples\\Data\\BrainProtonDensitySliceBorder20.png",//1
		"D:\\Libraries\\ITK\\ITK_4.12.0\\ITK_src\\Examples\\Data\\BrainProtonDensitySliceR10X13Y17.png",//2
		"F:\\registration\\ImageRegistration9Output.dcm",//3
		"F:\\registration\\ImageRegistration9DifferenceBefore.dcm",//4
		"F:\\registration\\ImageRegistration9DifferenceAfter.dcm",//5
		"1.0",//6
		"300"//7
	};

	char * argv3[] = {
		"MultiResImageRegistration1.exe",//0
		"D:\\Libraries\\ITK\\ITK_4.12.0\\ITK_src\\Examples\\Data\\BrainT1SliceBorder20.png",//1
		"D:\\Libraries\\ITK\\ITK_4.12.0\\ITK_src\\Examples\\Data\\BrainProtonDensitySliceShifted13x17y.png",//2
		"F:\\registration\\MultiResImageRegistration1Output.dcm",//3
		"128",//4
		"F:\\registration\\MultiResImageRegistration1CheckerboardBefore.dcm",//5
		"F:\\registration\\MultiResImageRegistration1CheckerboardAfter.dcm"//6
	};
	switch (reg_enum)
	{
	case Reg_Null:
		break;
	case Reg_test:
		TranslationReg(argv);
		break;
	case Reg_2Dtransform:
		CenteredSimilarityTransformReg(argv1);
		break;
	case Reg_AffineTrans:
		AffineTransformReg(argv2);
		break;
	case Reg_Multi:
		MultiTransformReg(argv3);
		break;
	default:
		break;
	}
	//this->close();
}
/*
 * 退出
 */
void Register::OnButtonCancel()
{
	this->close();
}