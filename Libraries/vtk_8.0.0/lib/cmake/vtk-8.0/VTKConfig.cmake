#-----------------------------------------------------------------------------
#
# VTKConfig.cmake - VTK CMake configuration file for external projects.
#
# This file is configured by VTK and used by the UseVTK.cmake module
# to load VTK's settings for an external project.

# Compute the installation prefix from this VTKConfig.cmake file location.
set(_vtk_installed_prefix "D:/Libraries/vtk/vtk8.0.0/VTK_install/lib/cmake/vtk-8.0")
set(_vtk_requested_prefix "${CMAKE_CURRENT_LIST_DIR}")
get_filename_component(_vtk_installed_prefix_full "${_vtk_installed_prefix}" REALPATH)
get_filename_component(_vtk_requested_prefix_full "${_vtk_requested_prefix}" REALPATH)
if (_vtk_installed_prefix_full STREQUAL _vtk_requested_prefix_full)
  set(VTK_INSTALL_PREFIX "D:/Libraries/vtk/vtk8.0.0/VTK_install")
else ()
  set(VTK_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}")
  get_filename_component(VTK_INSTALL_PREFIX "${VTK_INSTALL_PREFIX}" PATH)
  get_filename_component(VTK_INSTALL_PREFIX "${VTK_INSTALL_PREFIX}" PATH)
  get_filename_component(VTK_INSTALL_PREFIX "${VTK_INSTALL_PREFIX}" PATH)
endif ()
set(VTK_MODULES_DIR "${VTK_INSTALL_PREFIX}/lib/cmake/vtk-8.0/Modules")

set (__vtk_install_tree TRUE)

if (CMAKE_VERSION VERSION_LESS "3.3")
    message(FATAL_ERROR "VTK now requires CMake 3.3 or newer")
  endif()

#-----------------------------------------------------------------------------
# Minimum compiler version check: GCC >= 4.6
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
    CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
  message(FATAL_ERROR "GCC 4.6 or later is required.")
endif ()

#-----------------------------------------------------------------------------
# Minimum compiler version check: LLVM Clang >= 3.0
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
    CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.0)
  message(FATAL_ERROR "LLVM Clang 3.0 or later is required.")
endif ()

#-----------------------------------------------------------------------------
# Minimum compiler version check: Apple Clang >= 3.0 (Xcode 4.2)
if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" AND
    CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.0)
  message(FATAL_ERROR "Apple Clang 3.0 or later is required.")
endif ()

#-----------------------------------------------------------------------------
# Minimum compiler version check: Microsoft C/C++ >= 17.0 (aka VS 2012 aka VS 11.0)
if ("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC" AND
    CMAKE_CXX_COMPILER_VERSION VERSION_LESS 17.0)
  message(FATAL_ERROR "Microsoft Visual Studio 2012 or later is required.")
endif ()

#-----------------------------------------------------------------------------
# Minimum compiler version check: Intel C++ (ICC) >= 14
if (CMAKE_CXX_COMPILER_ID STREQUAL "Intel" AND
    CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14.0)
  message(FATAL_ERROR "Intel C++ (ICC) 14.0 or later is required.")
endif ()

# The C and C++ flags added by VTK to the cmake-configured flags.
SET(VTK_REQUIRED_C_FLAGS "")
SET(VTK_REQUIRED_CXX_FLAGS "")
SET(VTK_REQUIRED_EXE_LINKER_FLAGS "")
SET(VTK_REQUIRED_SHARED_LINKER_FLAGS "")
SET(VTK_REQUIRED_MODULE_LINKER_FLAGS "")

# The VTK version number
SET(VTK_MAJOR_VERSION "8")
SET(VTK_MINOR_VERSION "0")
SET(VTK_BUILD_VERSION "0")

# The location of the UseVTK.cmake file.
SET(VTK_CMAKE_DIR "${VTK_INSTALL_PREFIX}/lib/cmake/vtk-8.0")
SET(VTK_USE_FILE "${VTK_CMAKE_DIR}/UseVTK.cmake")

# The rendering backend VTK was configured to use.
set(VTK_RENDERING_BACKEND "OpenGL2")

if (__vtk_install_tree)
  if (WIN32)
    set (VTK_RUNTIME_DIRS "D:/Libraries/vtk/vtk8.0.0/VTK_install/bin")
  else ()
    set (VTK_RUNTIME_DIRS "D:/Libraries/vtk/vtk8.0.0/VTK_install/lib")
  endif ()
else()
  if (WIN32)
    set (VTK_RUNTIME_DIRS "D:/Libraries/vtk/vtk8.0.0/VTK_bin/bin")
  else ()
    set (VTK_RUNTIME_DIRS "D:/Libraries/vtk/vtk8.0.0/VTK_bin/lib")
  endif ()
endif()

#-----------------------------------------------------------------------------
# Load requested modules.

# List of available VTK modules.
set(VTK_MODULES_ENABLED)

# Determine list of available VTK-modules by scanning the VTK_MODULES_DIR.
file(GLOB config_files RELATIVE "${VTK_MODULES_DIR}" "${VTK_MODULES_DIR}/*.cmake")
foreach (_file ${config_files})
  if (NOT "${_file}" MATCHES "[^\\-]+-[a-zA-Z]+\\.cmake")
    string(REGEX REPLACE "\\.cmake$" "" _module "${_file}")
    list(APPEND VTK_MODULES_ENABLED "${_module}")
  endif()
endforeach()

# Import VTK targets.
set(VTK_CONFIG_TARGETS_FILE "${VTK_INSTALL_PREFIX}/lib/cmake/vtk-8.0/VTKTargets.cmake")
if(NOT TARGET vtkCommonCore)
  include("${VTK_CONFIG_TARGETS_FILE}")
endif()

# Load module interface macros.
include("${VTK_INSTALL_PREFIX}/lib/cmake/vtk-8.0/vtkModuleAPI.cmake")

# Compute set of requested modules.
if(VTK_FIND_COMPONENTS)
  # Specific modules requested by find_package(VTK).
  set(VTK_MODULES_REQUESTED "${VTK_FIND_COMPONENTS}")
else()
  # No specific modules requested.  Use all of them.
  set(VTK_MODULES_REQUESTED "${VTK_MODULES_ENABLED}")
endif()

# Load requested modules and their dependencies into variables:
#  VTK_DEFINITIONS     = Preprocessor definitions
#  VTK_LIBRARIES       = Libraries to link
#  VTK_INCLUDE_DIRS    = Header file search path
#  VTK_LIBRARY_DIRS    = Library search path (for outside dependencies)
vtk_module_config(VTK ${VTK_MODULES_REQUESTED})

#-----------------------------------------------------------------------------

# VTK global configuration options.
SET(VTK_BUILD_SHARED_LIBS "ON")
SET(VTK_LEGACY_REMOVE "OFF")
SET(VTK_LEGACY_SILENT "OFF")
SET(VTK_WRAP_PYTHON "OFF")
SET(VTK_WRAP_TCL "OFF")
SET(VTK_WRAP_JAVA "OFF")
SET(VTK_QT_VERSION "5")
SET(VTK_ENABLE_KITS "OFF")

# Do not add options or information here that is specific to a
# particular module.  Instead set <module>_EXPORT_OPTIONS and/or
# <module>_EXPORT_CODE_BUILD and <module>_EXPORT_CODE_INSTALL
# at the top of the module CMakeLists.txt file.
