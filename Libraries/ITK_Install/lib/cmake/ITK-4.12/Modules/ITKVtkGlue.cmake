set(ITKVtkGlue_LOADED 1)
set(ITKVtkGlue_ENABLE_SHARED "1")
set(ITKVtkGlue_DEPENDS "ITKCommon;ITKImageIntensity;ITKVTK")
set(ITKVtkGlue_PUBLIC_DEPENDS "ITKCommon;ITKVTK")
set(ITKVtkGlue_TRANSITIVE_DEPENDS "ITKCommon;ITKImageIntensity;ITKVTK")
set(ITKVtkGlue_PRIVATE_DEPENDS "")
set(ITKVtkGlue_LIBRARIES "ITKVtkGlue")
set(ITKVtkGlue_INCLUDE_DIRS "${ITK_INSTALL_PREFIX}/include/ITK-4.12;D:/Libraries/vtk/vtk8.0.0/VTK_install/include/vtk-8.0")
set(ITKVtkGlue_LIBRARY_DIRS "")
set(ITKVtkGlue_RUNTIME_LIBRARY_DIRS "${ITK_INSTALL_PREFIX}/bin")
set(ITKVtkGlue_TARGETS_FILE "")

set(VTK_DIR "D:/Libraries/vtk/vtk8.0.0/VTK_install/lib/cmake/vtk-8.0")

find_package(VTK NO_MODULE REQUIRED)

if(NOT VTK_VERSION)
  set(VTK_VERSION "8.0.0")
endif()
if(NOT VTK_RENDERING_BACKEND)
  set(VTK_RENDERING_BACKEND OpenGL)
endif()
set(_target_freetypeopengl)
if(TARGET vtkRenderingFreeTypeOpenGL2)
  set(_target_freetypeopengl vtkRenderingFreeTypeOpenGL2)
endif()

if( 8.0.0 VERSION_LESS 6.0.0 )
  set(ITKVtkGlue_VTK_INCLUDE_DIRS D:/Libraries/vtk/vtk8.0.0/VTK_install/include/vtk-8.0)
  set(ITKVtkGlue_VTK_LIBRARIES vtkChartsCore;vtkCommonColor;vtkCommonCore;vtksys;vtkCommonDataModel;vtkCommonMath;vtkCommonMisc;vtkCommonSystem;vtkCommonTransforms;vtkCommonExecutionModel;vtkFiltersGeneral;vtkCommonComputationalGeometry;vtkFiltersCore;vtkInfovisCore;vtkFiltersExtraction;vtkFiltersStatistics;vtkImagingFourier;vtkImagingCore;vtkalglib;vtkRenderingContext2D;vtkRenderingCore;vtkFiltersGeometry;vtkFiltersSources;vtkRenderingFreeType;vtkfreetype;vtkzlib;vtkDICOMParser;vtkDomainsChemistry;vtkIOLegacy;vtkIOCore;vtklz4;vtkIOXMLParser;vtkexpat;vtkDomainsChemistryOpenGL2;vtkRenderingOpenGL2;vtkglew;vtkFiltersAMR;vtkIOXML;vtkParallelCore;vtkFiltersFlowPaths;vtkFiltersGeneric;vtkFiltersHybrid;vtkImagingSources;vtkFiltersHyperTree;vtkFiltersImaging;vtkImagingGeneral;vtkFiltersModeling;vtkFiltersParallel;vtkFiltersParallelImaging;vtkFiltersPoints;vtkFiltersProgrammable;vtkFiltersSMP;vtkFiltersSelection;vtkFiltersTexture;vtkFiltersTopology;vtkFiltersVerdict;verdict;vtkGUISupportQt;vtkInteractionStyle;vtkGUISupportQtSQL;vtkIOSQL;vtksqlite;vtkGeovisCore;vtkIOImage;vtkmetaio;vtkjpeg;vtkpng;vtktiff;vtkInfovisLayout;vtkImagingHybrid;vtkInteractionWidgets;vtkImagingColor;vtkRenderingAnnotation;vtkRenderingVolume;vtkViewsCore;vtkproj4;vtkIOAMR;vtkhdf5_hl;vtkhdf5;vtkIOEnSight;vtkIOExodus;vtkexoIIc;vtkNetCDF;vtkIOExport;vtkRenderingGL2PSOpenGL2;vtkgl2ps;vtklibharu;vtkIOExportOpenGL2;vtkIOGeometry;vtkIOImport;vtkIOInfovis;vtklibxml2;vtkIOLSDyna;vtkIOMINC;vtkIOMovie;vtkoggtheora;vtkIONetCDF;vtknetcdfcpp;vtkIOPLY;vtkIOParallel;vtkjsoncpp;vtkIOParallelXML;vtkIOTecplotTable;vtkIOVideo;vtkImagingMath;vtkImagingMorphological;vtkImagingStatistics;vtkImagingStencil;vtkInteractionImage;vtkRenderingContextOpenGL2;vtkRenderingImage;vtkRenderingLOD;vtkRenderingLabel;vtkRenderingQt;vtkRenderingVolumeOpenGL2;vtkViewsContext2D;vtkViewsInfovis;vtkViewsQt)
else()
  set(_wrap_module)
  if(ITK_WRAP_PYTHON AND PYTHON_VERSION_STRING VERSION_LESS 3.0)
    set(_wrap_module vtkWrappingPythonCore)
  endif()
  vtk_module_config(ITKVtkGlue_VTK
    vtkRenderingOpenGL2
    vtkRenderingFreeType
    
    vtkInteractionStyle
    vtkIOImage
    vtkImagingSources
    
    )

  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${ITKVtkGlue_VTK_DEFINITIONS})
endif()

