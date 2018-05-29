
#ifndef VTKLZ4_EXPORT_H
#define VTKLZ4_EXPORT_H

#ifdef VTKLZ4_STATIC_DEFINE
#  define VTKLZ4_EXPORT
#  define VTKLZ4_NO_EXPORT
#else
#  ifndef VTKLZ4_EXPORT
#    ifdef vtklz4_EXPORTS
        /* We are building this library */
#      define VTKLZ4_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKLZ4_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKLZ4_NO_EXPORT
#    define VTKLZ4_NO_EXPORT 
#  endif
#endif

#ifndef VTKLZ4_DEPRECATED
#  define VTKLZ4_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKLZ4_DEPRECATED_EXPORT
#  define VTKLZ4_DEPRECATED_EXPORT VTKLZ4_EXPORT VTKLZ4_DEPRECATED
#endif

#ifndef VTKLZ4_DEPRECATED_NO_EXPORT
#  define VTKLZ4_DEPRECATED_NO_EXPORT VTKLZ4_NO_EXPORT VTKLZ4_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKLZ4_NO_DEPRECATED
#    define VTKLZ4_NO_DEPRECATED
#  endif
#endif

#endif
