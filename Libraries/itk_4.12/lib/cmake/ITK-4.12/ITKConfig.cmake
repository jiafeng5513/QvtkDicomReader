#-----------------------------------------------------------------------------
#
# ITKConfig.cmake - ITK CMake configuration file for external projects.
#
# This file is configured by ITK and used by the UseITK.cmake module
# to load ITK's settings for an external project.

# Compute the installation prefix from this ITKConfig.cmake file location.
get_filename_component(ITK_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(ITK_INSTALL_PREFIX "${ITK_INSTALL_PREFIX}" PATH)
get_filename_component(ITK_INSTALL_PREFIX "${ITK_INSTALL_PREFIX}" PATH)
get_filename_component(ITK_INSTALL_PREFIX "${ITK_INSTALL_PREFIX}" PATH)
set(ITK_MODULES_DIR "${ITK_INSTALL_PREFIX}/lib/cmake/ITK-4.12/Modules")

# The C and C++ flags added by ITK to the cmake-configured flags.
set(ITK_REQUIRED_C_FLAGS "")
set(ITK_REQUIRED_CXX_FLAGS " /bigobj")
set(ITK_REQUIRED_LINK_FLAGS "")

# The ITK version number
set(ITK_VERSION_MAJOR "4")
set(ITK_VERSION_MINOR "12")
set(ITK_VERSION_PATCH "0")

# If ITK was built with version 3 compatibility features.
set(ITKV3_COMPATIBILITY "OFF")

# Remove all legacy code completely.
set(ITK_LEGACY_REMOVE "OFF")

# Silence all legacy code messages.
set(ITK_LEGACY_SILENT "OFF")

# Remove code that will become legacy in future releases completely.
set(ITK_FUTURE_LEGACY_REMOVE "0")

# ITK's CMake directory with modules ITK uses.
set(ITK_CMAKE_DIR "${ITK_INSTALL_PREFIX}/lib/cmake/ITK-4.12")

# The location of the UseITK.cmake file.
set(ITK_USE_FILE "${ITK_INSTALL_PREFIX}/lib/cmake/ITK-4.12/UseITK.cmake")

# Whether ITK was built with shared libraries.
set(ITK_BUILD_SHARED "ON")

# Whether ITK examples were built.
set(ITK_BUILD_EXAMPLES "OFF")

# Whether ITK documentation was built.
set(ITK_BUILD_DOCUMENTATION "OFF")

# List of available ITK modules.
set(ITK_MODULES_ENABLED "ITKDoubleConversion;ITKKWIML;ITKKWSys;ITKVNL;ITKVNLInstantiation;ITKCommon;ITKFiniteDifference;ITKImageFilterBase;ITKCurvatureFlow;ITKImageAdaptors;ITKNetlib;ITKStatistics;ITKTransform;ITKImageFunction;ITKImageGrid;ITKAnisotropicSmoothing;ITKLabelMap;ITKImageCompose;ITKMesh;ITKZLIB;ITKMetaIO;ITKSpatialObjects;ITKImageStatistics;ITKPath;ITKImageIntensity;ITKThresholding;ITKConnectedComponents;ITKMathematicalMorphology;ITKBinaryMathematicalMorphology;ITKImageLabel;ITKNarrowBand;ITKDistanceMap;ITKQuadEdgeMesh;ITKFastMarching;ITKIOImageBase;ITKImageCompare;ITKSmoothing;ITKImageGradient;ITKImageSources;ITKImageFeature;ITKOptimizers;ITKSignedDistanceFunction;ITKLevelSets;ITKAntiAlias;ITKPolynomials;ITKBiasCorrection;ITKBioCell;ITKClassifiers;ITKColormap;ITKFFT;ITKConvolution;ITKDICOMParser;ITKDeconvolution;ITKDeformableMesh;ITKDenoising;ITKDiffusionTensorImage;ITKDisplacementField;ITKEigen;ITKExpat;ITKIOXML;ITKIOSpatialObjects;ITKRegistrationCommon;ITKFEM;ITKPDEDeformableRegistration;ITKFEMRegistration;ITKGDCM;ITKNIFTI;ITKGIFTI;ITKGPUCommon;ITKGPUFiniteDifference;ITKGPUAnisotropicSmoothing;ITKGPUImageFilterBase;ITKGPURegistrationCommon;ITKGPUPDEDeformableRegistration;ITKGPUSmoothing;ITKGPUThresholding;ITKHDF5;ITKIOBMP;ITKIOBioRad;ITKIOCSV;ITKIOGDCM;ITKIOIPL;ITKIOGE;ITKIOGIPL;ITKIOHDF5;ITKJPEG;ITKIOJPEG;ITKTIFF;ITKIOTIFF;ITKIOLSM;ITKIOMRC;ITKIOMesh;ITKIOMeta;ITKIONIFTI;ITKNrrdIO;ITKIONRRD;ITKPNG;ITKIOPNG;ITKIORAW;ITKIOSiemens;ITKIOStimulate;ITKTransformFactory;ITKIOTransformBase;ITKIOTransformHDF5;ITKIOTransformInsightLegacy;ITKIOTransformMatlab;ITKIOVTK;ITKImageFusion;ITKImageNoise;ITKIntegratedTest;ITKKLMRegionGrowing;ITKLabelVoting;ITKLevelSetsv4;ITKMarkovRandomFieldsClassifiers;ITKOptimizersv4;ITKMetricsv4;ITKNeuralNetworks;ITKQuadEdgeMeshFiltering;ITKRegionGrowing;ITKRegistrationMethodsv4;ITKSpatialFunction;ITKTestKernel;ITKVTK;ITKVideoCore;ITKVideoFiltering;ITKVideoIO;ITKVoronoi;ITKVtkGlue;ITKWatersheds")

# Import ITK targets.
set(ITK_CONFIG_TARGETS_FILE "${ITK_INSTALL_PREFIX}/lib/cmake/ITK-4.12/ITKTargets.cmake")
if(NOT ITK_TARGETS_IMPORTED)
  set(ITK_TARGETS_IMPORTED 1)
  include("${ITK_CONFIG_TARGETS_FILE}")
endif()

# Load module interface macros.
include("${ITK_INSTALL_PREFIX}/lib/cmake/ITK-4.12/ITKModuleAPI.cmake")

# Compute set of requested modules.
if(ITK_FIND_COMPONENTS)
  # Specific modules requested by find_package(ITK).
  set(ITK_MODULES_REQUESTED "${ITK_FIND_COMPONENTS}")
else()
  # No specific modules requested.  Use all of them.
  set(ITK_MODULES_REQUESTED "${ITK_MODULES_ENABLED}")
endif()

# Load requested modules and their dependencies into variables:
#  ITK_LIBRARIES       = Libraries to link
#  ITK_INCLUDE_DIRS    = Header file search path
#  ITK_LIBRARY_DIRS    = Library search path (for outside dependencies)
#  ITK_RUNTIME_LIBRARY_DIRS = Runtime linker search path
itk_module_config(ITK ${ITK_MODULES_REQUESTED})

# Add configuration with FFTW
set(ITK_USE_FFTWD "OFF")
set(ITK_USE_FFTWF "OFF")
set(ITK_USE_SYSTEM_FFTW "OFF")
set(ITK_FFTW_INCLUDE_PATH "${ITK_INSTALL_PREFIX}/include/ITK-4.12/Algorithms")
set(ITK_FFTW_LIBDIR "${ITK_INSTALL_PREFIX}/lib/ITK-4.12")

# Add FFTW include and library directories
if (ITK_USE_FFTWF OR ITK_USE_FFTWD)
  set(ITK_INCLUDE_DIRS ${ITK_INCLUDE_DIRS} "${ITK_FFTW_INCLUDE_PATH}")
  set(ITK_LIBRARY_DIRS ${ITK_LIBRARY_DIRS} "${ITK_FFTW_LIBDIR}")
endif()

# Add configuration with GPU
set(ITK_USE_GPU "OFF")

# Wrapping
set(ITK_WRAPPING "OFF")
if( NOT DEFINED ITK_WRAP_PYTHON)
  set(ITK_WRAP_PYTHON "OFF")
endif()
if( NOT DEFINED ITK_WRAP_JAVA)
  set(ITK_WRAP_JAVA "OFF")
endif()
if(NOT DEFINED ITK_WRAP_RUBY)
  set(ITK_WRAP_RUBY "OFF")
endif()
if( NOT DEFINED ITK_WRAP_PERL)
  set(ITK_WRAP_PERL "OFF")
endif()
if(NOT DEFINED ITK_WRAP_TCL)
  set(ITK_WRAP_TCL "OFF")
endif()

set(ITK_WRAP_PYTHON_VERSION "3.6.1")
