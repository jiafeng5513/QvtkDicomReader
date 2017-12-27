
#ifndef ITKBiasCorrection_EXPORT_H
#define ITKBiasCorrection_EXPORT_H

#ifdef ITK_STATIC
#  define ITKBiasCorrection_EXPORT
#  define ITKBiasCorrection_HIDDEN
#else
#  ifndef ITKBiasCorrection_EXPORT
#    ifdef ITKBiasCorrection_EXPORTS
        /* We are building this library */
#      define ITKBiasCorrection_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define ITKBiasCorrection_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef ITKBiasCorrection_HIDDEN
#    define ITKBiasCorrection_HIDDEN 
#  endif
#endif

#ifndef ITKBIASCORRECTION_DEPRECATED
#  define ITKBIASCORRECTION_DEPRECATED __declspec(deprecated)
#endif

#ifndef ITKBIASCORRECTION_DEPRECATED_EXPORT
#  define ITKBIASCORRECTION_DEPRECATED_EXPORT ITKBiasCorrection_EXPORT ITKBIASCORRECTION_DEPRECATED
#endif

#ifndef ITKBIASCORRECTION_DEPRECATED_NO_EXPORT
#  define ITKBIASCORRECTION_DEPRECATED_NO_EXPORT ITKBiasCorrection_HIDDEN ITKBIASCORRECTION_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define ITKBIASCORRECTION_NO_DEPRECATED
#endif

#endif
