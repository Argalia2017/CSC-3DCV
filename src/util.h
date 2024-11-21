#pragma once

#include <csc.hpp>
#include <csc_type.hpp>
#include <csc_core.hpp>
#include <csc_basic.hpp>
#include <csc_math.hpp>
#include <csc_array.hpp>
#include <csc_image.hpp>
#include <csc_matrix.hpp>
#include <csc_stream.hpp>
#include <csc_string.hpp>
#include <csc_runtime.hpp>
#include <csc_thread.hpp>

#ifndef DLLEXTERN
#define DLLEXTERN __declspec (dllexport)
#define DLLAPI __stdcall
#endif

namespace CSC3DCV {
using namespace CSC ;

using CSC::BOOL ;
using CSC::BYTE ;
using CSC::WORD ;
using CSC::CHAR ;

using CSC::move ;
using CSC::swap ;
} ;