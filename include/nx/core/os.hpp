/*
 * Copyright (C) 2013 Jacob McIntosh <nacitar at ubercpp dot com> 
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*! @file
 *  Platform detection and initialization; required everywhere.
 */ 

#ifndef NX_CORE_OS_HPP_
#define NX_CORE_OS_HPP_

// C++11 requirement
#if (\
    (__cplusplus < 201103L && !defined(__GXX_EXPERIMENTAL_CXX0X__)) || \
    (defined(GCC_VERSION) && GCC_VERSION < 40700) \
)
  #error This library is written with c++11 in mind; backwards compatibility \
      has been removed.  If using gcc, this requires 4.7+
#endif

// TODO(nacitar): Toolchain macros; decide about using these later
#if 0
#if defined(__GNUC__)
  #define NX_TC_GCC 1
  #define NX_ALIGN_TO(bytes) __attribute__((aligned(bytes)))
  #define NX_MAY_ALIAS __attribute__((__may_alias__))
  #define NX_LIKELY(x) __builtin_expect((x),1)
  #define NX_UNLIKELY(x) __builtin_expect((x),0)
#else
  #define NX_LIKELY(x) (x)
  #define NX_UNLIKELY(x) (x)
  // NOTE: NX_MAY_ALIAS - NOT DEFINING IT FOR NON-GCC, NOT EVEN EMPTILY.
  // We want a compilation error, or for you to #ifdef it!
#endif
#if _MSC_VER > 1300 // .net 2002+
  #define TC_VC_ 1
  #define NX_ALIGN_TO(bytes) __declspec( align( bytes ) )
#endif
#endif

// OS detection 
#if (\
    defined(WINDOWS) || defined(WIN32) || defined(_WIN32) || \
    defined(WIN64) || defined(_WIN64) \
)
  #define NX_OS_WINDOWS 1
#elif (\
    defined(__linux) || defined(__linux__) || \
    defined(linux) || defined(__gnu_linux__) || defined(LINUX) \
)
  #define NX_OS_LINUX 1
#elif defined(__APPLE__)
  #define NX_OS_MAC 1
#elif defined (__SVR4) && defined (__sun)
  #define NX_OS_SOLARIS 1
#else
  #define NX_OS_OTHER 1
#endif

// OS initialization/ensuring important system defines are set 
#if defined(NX_OS_WINDOWS)
  #define WINVER 0x0500
  #define _WIN32_WINNT 0x0501
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <windows.h>
  #ifndef PATH_MAX
    #define PATH_MAX MAX_PATH
  #endif
#else
  #include <limits.h>
  #define MAX_PATH PATH_MAX
#endif

#ifndef CHAR_BIT
  #include <climits> // just for CHAR_BIT
  
  #ifndef CHAR_BIT
    #error header <climits> does not define CHAR_BIT! either explicitly define \
        it here or include the appropriate header!
  #endif
#endif
#endif  // NX_CORE_OS_HPP_
