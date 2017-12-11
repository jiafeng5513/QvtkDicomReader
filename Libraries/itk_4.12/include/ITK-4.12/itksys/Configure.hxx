/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing#kwsys for details.  */
#ifndef itksys_Configure_hxx
#define itksys_Configure_hxx

/* Include C configuration.  */
#include <itksys/Configure.h>

/* Whether wstring is available.  */
#define itksys_STL_HAS_WSTRING 1
/* Whether <ext/stdio_filebuf.h> is available. */
#define itksys_CXX_HAS_EXT_STDIO_FILEBUF_H                         \
  0

/* If building a C++ file in kwsys itself, give the source file
   access to the macros without a configured namespace.  */
#if defined(KWSYS_NAMESPACE)
#if !itksys_NAME_IS_KWSYS
#define kwsys itksys
#endif
#define KWSYS_NAME_IS_KWSYS itksys_NAME_IS_KWSYS
#define KWSYS_STL_HAS_WSTRING itksys_STL_HAS_WSTRING
#define KWSYS_CXX_HAS_EXT_STDIO_FILEBUF_H                                     \
  itksys_CXX_HAS_EXT_STDIO_FILEBUF_H
#endif

#endif
