
#ifndef ITKFEM_EXPORT_H
#define ITKFEM_EXPORT_H

#ifdef ITK_STATIC
#  define ITKFEM_EXPORT
#  define ITKFEM_HIDDEN
#else
#  ifndef ITKFEM_EXPORT
#    ifdef ITKFEM_EXPORTS
        /* We are building this library */
#      define ITKFEM_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define ITKFEM_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef ITKFEM_HIDDEN
#    define ITKFEM_HIDDEN 
#  endif
#endif

#ifndef ITKFEM_DEPRECATED
#  define ITKFEM_DEPRECATED __declspec(deprecated)
#endif

#ifndef ITKFEM_DEPRECATED_EXPORT
#  define ITKFEM_DEPRECATED_EXPORT ITKFEM_EXPORT ITKFEM_DEPRECATED
#endif

#ifndef ITKFEM_DEPRECATED_NO_EXPORT
#  define ITKFEM_DEPRECATED_NO_EXPORT ITKFEM_HIDDEN ITKFEM_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define ITKFEM_NO_DEPRECATED
#endif

#endif
