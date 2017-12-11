
#ifndef VTKNETCDFCPP_EXPORT_H
#define VTKNETCDFCPP_EXPORT_H

#ifdef VTKNETCDFCPP_STATIC_DEFINE
#  define VTKNETCDFCPP_EXPORT
#  define VTKNETCDFCPP_NO_EXPORT
#else
#  ifndef VTKNETCDFCPP_EXPORT
#    ifdef vtknetcdfcpp_EXPORTS
        /* We are building this library */
#      define VTKNETCDFCPP_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKNETCDFCPP_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKNETCDFCPP_NO_EXPORT
#    define VTKNETCDFCPP_NO_EXPORT 
#  endif
#endif

#ifndef VTKNETCDFCPP_DEPRECATED
#  define VTKNETCDFCPP_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKNETCDFCPP_DEPRECATED_EXPORT
#  define VTKNETCDFCPP_DEPRECATED_EXPORT VTKNETCDFCPP_EXPORT VTKNETCDFCPP_DEPRECATED
#endif

#ifndef VTKNETCDFCPP_DEPRECATED_NO_EXPORT
#  define VTKNETCDFCPP_DEPRECATED_NO_EXPORT VTKNETCDFCPP_NO_EXPORT VTKNETCDFCPP_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKNETCDFCPP_NO_DEPRECATED
#    define VTKNETCDFCPP_NO_DEPRECATED
#  endif
#endif

#endif
