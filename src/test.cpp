﻿#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <utility>

#include <opencv2/opencv.hpp>
#include <csc_begin.h>

using namespace CSC3DCV ;

inline Image<FLT32> convert_to_flt32_image (CREF<Image<BYTE>> image) {
	Image<FLT32> ret = Image<FLT32> (image.shape ()) ;
	ret.fill (infinity) ;
	for (auto &&i : image.range ()) {
		const auto r2x = image[i] ;
		const auto r3x = FLT32 (VAL32 (r2x)) ;
		if (MathProc::is_low (r3x))
			continue ;
		const auto r4x = FLT32 (255) - r3x + 100 ;
		ret[i] = r4x ;
	}
	return move (ret) ;
}

inline Image<FLT32> convert_to_flt32_image (CREF<Image<Color3B>> image) {
	Image<FLT32> ret = Image<FLT32> (image.shape ()) ;
	ret.fill (infinity) ;
	const auto r1x = MathProc::inverse (FLT32 (3)) ;
	for (auto &&i : image.range ()) {
		const auto r2x = image[i] ;
		const auto r3x = FLT32 (VAL32 (r2x.mR) + VAL32 (r2x.mG) + VAL32 (r2x.mB)) * r1x ;
		if (MathProc::is_low (r3x))
			continue ;
		const auto r4x = FLT32 (255) - r3x + 100 ;
		ret[i] = r4x ;
	}
	return move (ret) ;
}

inline void save_final_ply () {
	const auto r1x = Path (slice ("D:/Documents/C++/csc_3dcv/data/2/image.jpg")) ;
	const auto r2x = Path (slice ("D:/Documents/C++/csc_3dcv/data/2/depth.png")) ;
	const auto r3x = Image<Color3B> (ImageProc::load_image (r1x)) ;
	const auto r4x = Image<BYTE> (ImageProc::load_image (r2x)) ;
	const auto r5x = convert_to_flt32_image (r4x) ;
	const auto r6x = Path (slice ("D:/Documents/C++/csc_3dcv/data/2/final.ply")) ;
	auto mWriter = StreamFileByteWriter (r6x) ;
	const auto GAP = slice ("\n") ;
	mWriter.deref << slice ("ply") << GAP ;
	mWriter.deref << slice ("format binary_little_endian 1.0") << GAP ;
	const auto r7x = invoke ([&] () {
		LENGTH ret = 0 ;
		for (auto &&i : r3x.range ())
			ret += !MathProc::is_inf (r5x[i]) ;
		return move (ret) ;
	}) ;
	mWriter.deref << slice ("element vertex ") << StringBuild<STRU8>::make (r7x) << GAP ;
	mWriter.deref << slice ("property float x") << GAP ;
	mWriter.deref << slice ("property float y") << GAP ;
	mWriter.deref << slice ("property float z") << GAP ;
	mWriter.deref << slice ("property uchar red") << GAP ;
	mWriter.deref << slice ("property uchar green") << GAP ;
	mWriter.deref << slice ("property uchar blue") << GAP ;
	mWriter.deref << slice ("end_header") << GAP ;
	const auto r8x = Vector (3200 ,5256 ,0 ,2).projection () ;
	const auto r9x = PerspectiveMatrix (r8x[0] ,r8x[0] ,r8x[0] ,r8x[1]) ;
	const auto r10x = DuplexMatrix (r9x) ;
	for (auto &&i : r3x.range ()) {
		if (MathProc::is_inf (r5x[i]))
			continue ;
		const auto r11x = Vector (i) ;
		const auto r12x = (r10x[1] * r11x).normalize () ;
		const auto r13x = r12x * r5x[i] + Vector::axis_w () ;
		mWriter.deref << FLT32 (r13x[0]) ;
		mWriter.deref << FLT32 (r13x[1]) ;
		mWriter.deref << FLT32 (r13x[2]) ;
		mWriter.deref << r3x[i].mR ;
		mWriter.deref << r3x[i].mG ;
		mWriter.deref << r3x[i].mB ;
	}
	mWriter.flush () ;
}

inline void save_sphere_ply () {
	const auto r1x = Path (slice ("D:/Documents/C++/csc_3dcv/data/2/image.jpg")) ;
	const auto r3x = Image<Color3B> (ImageProc::load_image (r1x)) ;
	const auto r6x = Path (slice ("D:/Documents/C++/csc_3dcv/data/2/sphere.ply")) ;
	auto mWriter = StreamFileByteWriter (r6x) ;
	const auto GAP = slice ("\n") ;
	mWriter.deref << slice ("ply") << GAP ;
	mWriter.deref << slice ("format binary_little_endian 1.0") << GAP ;
	const auto r7x = r3x.shape ().size () ;
	mWriter.deref << slice ("element vertex ") << StringBuild<STRU8>::make (r7x) << GAP ;
	mWriter.deref << slice ("property float x") << GAP ;
	mWriter.deref << slice ("property float y") << GAP ;
	mWriter.deref << slice ("property float z") << GAP ;
	mWriter.deref << slice ("property uchar red") << GAP ;
	mWriter.deref << slice ("property uchar green") << GAP ;
	mWriter.deref << slice ("property uchar blue") << GAP ;
	mWriter.deref << slice ("end_header") << GAP ;
	const auto r8x = Vector (3200 ,5256 ,0 ,2).projection () ;
	const auto r9x = PerspectiveMatrix (r8x[0] ,r8x[0] ,r8x[0] ,r8x[1]) ;
	const auto r10x = DuplexMatrix (r9x) ;
	for (auto &&i : r3x.range ()) {
		const auto r11x = Vector (i) ;
		const auto r12x = (r10x[1] * r11x).normalize () ;
		const auto r13x = r12x + Vector::axis_w () ;
		mWriter.deref << FLT32 (r13x[0]) ;
		mWriter.deref << FLT32 (r13x[1]) ;
		mWriter.deref << FLT32 (r13x[2]) ;
		mWriter.deref << r3x[i].mR ;
		mWriter.deref << r3x[i].mG ;
		mWriter.deref << r3x[i].mB ;
	}
	mWriter.flush () ;
}