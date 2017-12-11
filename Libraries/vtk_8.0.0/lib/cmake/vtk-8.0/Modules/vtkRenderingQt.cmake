set(vtkRenderingQt_LOADED 1)
set(vtkRenderingQt_DEPENDS "vtkCommonCore;vtkCommonDataModel;vtkCommonExecutionModel;vtkCommonSystem;vtkFiltersSources;vtkFiltersTexture;vtkGUISupportQt;vtkRenderingCore;vtkRenderingLabel")
set(vtkRenderingQt_LIBRARIES "vtkRenderingQt")
set(vtkRenderingQt_INCLUDE_DIRS "${VTK_INSTALL_PREFIX}/include/vtk-8.0")
set(vtkRenderingQt_LIBRARY_DIRS "")
set(vtkRenderingQt_RUNTIME_LIBRARY_DIRS "${VTK_INSTALL_PREFIX}/bin")
set(vtkRenderingQt_WRAP_HIERARCHY_FILE "${CMAKE_CURRENT_LIST_DIR}/vtkRenderingQtHierarchy.txt")
set(vtkRenderingQt_KIT "")

if(NOT Qt5_DIR)
  set(Qt5_DIR "D:/Libraries/Qt/Qt5.9.1/5.9.1/msvc2017_64/lib/cmake/Qt5")
endif()
find_package(Qt5 REQUIRED QUIET COMPONENTS Widgets)


