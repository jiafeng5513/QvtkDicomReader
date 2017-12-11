#pragma once
#include "itkOtsuMultipleThresholdsCalculator.h"
// Software Guide : EndCodeSnippet
#include "itkPNGImageIOFactory.h"  
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarImageToHistogramGenerator.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkNumericTraits.h"

#include "itkConnectedThresholdImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"

#include "itkNeighborhoodConnectedImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"

#include "itkConfidenceConnectedImageFilter.h"


#include "itkCurvatureFlowImageFilter.h"

#include "itkConfidenceConnectedImageFilter.h"

#include <iostream>
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkVectorGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkVectorCastImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"

#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSigmoidImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSigmoidImageFilter.h"

#include "itkFastMarchingImageFilter.h"
#include "itkShapeDetectionLevelSetImageFilter.h"

#include "itkBinaryThresholdImageFilter.h"

#include <iostream>
#include <string>
#include <math.h>
#include "itkRGBGibbsPriorFilter.h"
#include "itkImageClassifierBase.h"
#include "itkImageGaussianModelEstimator.h"
#include "itkMahalanobisDistanceMembershipFunction.h"
#include "itkMinimumDecisionRule.h"
#include "itkSize.h"
#include "itkVector.h"


#include "itkImageRegistrationMethod.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSubtractImageFilter.h"

#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"


#include "itkCenteredTransformInitializer.h"

#include "itkCenteredSimilarity2DTransform.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkIdentityTransform.h"

#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"


#include "itkCenteredTransformInitializer.h"

#include "itkAffineTransform.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMattesMutualInformationImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkCheckerBoardImageFilter.h"

#include <vtkSmartPointer.h>

#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkAssemblyPath.h>
#include <vtkCell.h>
#include <vtkCommand.h>
#include <vtkCornerAnnotation.h>
#include <vtkDataArray.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPointData.h>
#include <vtkPropPicker.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextProperty.h>
#include <QVTKWidget.h>

#include <itkImageToVTKImageFilter.h>

#include "itkCommand.h"

#include <itkImage.h>
#include <itkImageFileReader.h>

#include <itkImageToVTKImageFilter.h>

#include "vtkVersion.h"
#include "vtkImageViewer.h"
#include "vtkImageMapper3D.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "itkRGBPixel.h"

#define   NUM_CLASSES         3
#define   MAX_NUM_ITER        1

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <iomanip>
#include <stdio.h>