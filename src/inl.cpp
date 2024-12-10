#include "util.h"

#define _CRT_SECURE_NO_WARNINGS

#include <csc_end.h>
#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif
#include <csc_begin.h>

#include <csc_core.inl>
#include <csc_basic.inl>
#include <csc_math.inl>
#include <csc_array.inl>
#include <csc_image.inl>
#include <csc_matrix.inl>
#include <csc_stream.inl>
#include <csc_string.inl>
#include <csc_runtime.inl>
#include <csc_file.inl>
#include <csc_thread.inl>

#define __CSC_API_WITH_SYS__

#ifdef __CSC_COMPILER_MSVC__
#define __CSC_API_WITH_EIGEN__
#define __CSC_API_WITH_OPENCV__
#endif

#include <csc_math.cache.inl>

#ifdef __CSC_API_WITH_SYS__
#ifdef __CSC_SYSTEM_WINDOWS__
#include <csc_runtime.windows.inl>
#include <csc_file.windows.inl>
#endif

#ifdef __CSC_SYSTEM_LINUX__
#include <csc_runtime.linux.inl>
#include <csc_file.linux.inl>
#endif
#endif

#ifdef __CSC_API_WITH_EIGEN__
#include <csc_matrix.eigen.inl>
#endif

#ifdef __CSC_API_WITH_OPENCV__
#include <csc_image.opencv.inl>

#ifdef __CSC_PLATFORM_X64__
#pragma comment (lib ,"opencv_world470.lib")
#endif
#endif