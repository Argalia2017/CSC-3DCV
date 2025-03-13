﻿#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_image.hpp"

#include "csc_end.h"
#ifdef __CSC_VER_DEBUG__
#define CV_IGNORE_DEBUG_BUILD_GUARD
#endif

#include <opencv2/opencv.hpp>
#include "csc_begin.h"

namespace CSC {
struct ImageProcImplLayout {} ;

class ImageProcImplHolder final implement Fat<ImageProcHolder ,ImageProcImplLayout> {
public:
	void initialize () override {
		noop () ;
	}

	ImageLayout make_image (RREF<BoxLayout> image) const override {
		ImageLayout ret ;
		auto &&rax = keep[TYPE<Box<cv::Mat>>::expr] (image).self ;
		const auto r1x = rax.size () ;
		const auto r2x = LENGTH (rax.depth ()) ;
		const auto r3x = align_of_cvmat_depth (r2x) ;
		const auto r4x = choose_cvmat_unknown (r3x) ;
		const auto r5x = r3x * rax.channels () ;
		const auto r6x = LENGTH (rax.step[0]) ;
		assume (r6x % r5x == 0) ;
		const auto r7x = r6x / r5x ;
		const auto r8x = FLAG (rax.data) ;
		const auto r9x = r7x * LENGTH (r1x.height) ;
		const auto r10x = Slice (r8x ,r9x ,r5x) ;
		RefBufferHolder::hold (ret.mImage)->initialize (r4x ,r10x ,Box<Pin<cv::Mat>>::make ()) ;
		auto &&rbx = keep[TYPE<Box<cv::Mat>>::expr] (RefBufferHolder::hold (ret.mImage)->raw ()).self ;
		assign (rbx ,rax) ;
		ret.mWidth = LENGTH (r1x.width) ;
		ret.mStride = r7x ;
		ImageHolder::hold (ret)->reset () ;
		return move (ret) ;
	}

	ImageLayout make_image (CREF<ImageShape> shape) const override {
		auto rax = Box<Pin<cv::Mat>>::make () ;
		const auto r1x = CV_MAKE_TYPE (CV_8U ,VAL32 (shape.mStep)) ;
		auto rbx = cv::Mat (cv::Size (VAL32 (shape.mCX) ,VAL32 (shape.mCY)) ,r1x) ;
		rax->set (rbx) ;
		return make_image (move (rax)) ;
	}

	ImageLayout make_image (CREF<ImageShape> shape ,CREF<Clazz> clazz ,CREF<LENGTH> channel) const override {
		auto rax = Box<Pin<cv::Mat>>::make () ;
		const auto r1x = cvmat_depth_of_clazz (clazz) ;
		const auto r2x = CV_MAKE_TYPE (VAL32 (r1x) ,VAL32 (channel)) ;
		auto rbx = cv::Mat (cv::Size (VAL32 (shape.mCX) ,VAL32 (shape.mCY)) ,r2x) ;
		rax->set (rbx) ;
		return make_image (move (rax)) ;
	}

	LENGTH cvmat_depth_of_clazz (CREF<Clazz> clazz) const {
		if (clazz.type_align () == 1)
			return CV_8U ;
		if (clazz.type_align () == 2)
			return CV_16U ;
		if (clazz == Clazz (TYPE<FLT32>::expr))
			return CV_32F ;
		if (clazz == Clazz (TYPE<FLT64>::expr))
			return CV_64F ;
		assume (FALSE) ;
		return CV_8U ;
	}

	LENGTH align_of_cvmat_depth (CREF<LENGTH> depth) const {
		if (depth == CV_8U)
			return 1 ;
		if (depth == CV_8S)
			return 1 ;
		if (depth == CV_16U)
			return 2 ;
		if (depth == CV_16S)
			return 2 ;
		if (depth == CV_32S)
			return 4 ;
		if (depth == CV_32F)
			return 4 ;
		if (depth == CV_64F)
			return 8 ;
		if (depth == CV_16F)
			return 2 ;
		return 0 ;
	}

	Unknown choose_cvmat_unknown (CREF<LENGTH> align) const {
		if (align == SIZE_OF<BYTE>::expr)
			return BufferUnknownBinder<BYTE> () ;
		if (align == SIZE_OF<WORD>::expr)
			return BufferUnknownBinder<WORD> () ;
		if (align == SIZE_OF<CHAR>::expr)
			return BufferUnknownBinder<CHAR> () ;
		if (align == SIZE_OF<QUAD>::expr)
			return BufferUnknownBinder<QUAD> () ;
		assume (FALSE) ;
		return Unknown (ZERO) ;
	}

	VREF<Pointer> peek_image (VREF<ImageLayout> image) const override {
		assert (ImageHolder::hold (image)->fixed ()) ;
		auto &&rax = keep[TYPE<Box<Pin<cv::Mat>>>::expr] (ImageHolder::hold (image)->raw ()) ;
		return rax.self ;
	}

	CREF<Pointer> peek_image (CREF<ImageLayout> image) const override {
		assert (ImageHolder::hold (image)->fixed ()) ;
		auto &&rax = keep[TYPE<Box<Pin<cv::Mat>>>::expr] (ImageHolder::hold (image)->raw ()) ;
		return rax.self ;
	}

	ImageLayout load_image (CREF<String<STR>> file) const override {
		auto rax = Box<Pin<cv::Mat>>::make () ;
		const auto r1x = StringProc::stra_from_strs (file) ;
		auto rbx = cv::imread (r1x.self ,cv::IMREAD_UNCHANGED) ;
		assume (!rbx.empty ()) ;
		rax->set (rbx) ;
		return make_image (move (rax)) ;
	}

	void save_image (CREF<String<STR>> file ,CREF<ImageLayout> image) const override {
		const auto r1x = StringProc::stra_from_strs (file) ;
		const auto r2x = ImageHolder::hold (image)->bx () ;
		const auto r3x = ImageHolder::hold (image)->by () ;
		const auto r4x = ImageHolder::hold (image)->cx () ;
		const auto r5x = ImageHolder::hold (image)->cy () ;
		auto &&rax = keep[TYPE<Box<cv::Mat>>::expr] (image.mImage.raw ()).self ;
		const auto r6x = cv::Rect (VAL32 (r2x) ,VAL32 (r3x) ,VAL32 (r4x) ,VAL32 (r5x)) ;
		auto rbx = rax (r6x) ;
		cv::imwrite (r1x.self ,rbx) ;
	}

	Color1B sampler (CREF<Image<Color1B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_f32_impl (image ,FLT32 (x) ,FLT32 (y)) ;
	}

	Color2B sampler (CREF<Image<Color2B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_f32_impl (image ,FLT32 (x) ,FLT32 (y)) ;
	}

	Color3B sampler (CREF<Image<Color3B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_f32_impl (image ,FLT32 (x) ,FLT32 (y)) ;
	}

	Color4B sampler (CREF<Image<Color4B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_f32_impl (image ,FLT32 (x) ,FLT32 (y)) ;
	}

	template <class ARG1>
	forceinline ARG1 sampler_f32_impl (CREF<Image<ARG1>> image ,CREF<FLT32> x ,CREF<FLT32> y) const {
		const auto r1x = INDEX (MathProc::round (x ,FLT32 (1))) ;
		const auto r2x = INDEX (MathProc::round (y ,FLT32 (1))) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.cx () - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.cy () - 1) ;
		const auto r5x = MathProc::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = MathProc::max_of (r4x - 1 ,ZERO) ;
		const auto r7x = FLT32 (r3x) + FLT32 (0.5) - x ;
		const auto r8x = FLT32 (r4x) + FLT32 (0.5) - y ;
		const auto r9x = r7x * r8x ;
		const auto r10x = r7x * (1 - r8x) ;
		const auto r11x = (1 - r7x) * (1 - r8x) ;
		const auto r12x = (1 - r7x) * r8x ;
		const auto r13x = cvt_colorf (image.at (r5x ,r6x)) ;
		const auto r14x = cvt_colorf (image.at (r5x ,r4x)) ;
		const auto r15x = cvt_colorf (image.at (r3x ,r4x)) ;
		const auto r16x = cvt_colorf (image.at (r3x ,r6x)) ;
		auto rax = Buffer<FLT32 ,SIZE_OF<ARG1>> () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			rax[i] = 0 ;
			rax[i] += r13x[i] * r9x ;
			rax[i] += r14x[i] * r10x ;
			rax[i] += r15x[i] * r11x ;
			rax[i] += r16x[i] * r12x ;
			rax[i] = MathProc::clamp (rax[i] ,FLT32 (0) ,FLT32 (255)) ;
		}
		return cvt_colorb (rax) ;
	}

	Buffer1<FLT32> cvt_colorf (CREF<Color1B> a) const {
		Buffer1<FLT32> ret ;
		ret[0] = FLT32 (a.mB) ;
		return move (ret) ;
	}

	Buffer2<FLT32> cvt_colorf (CREF<Color2B> a) const {
		Buffer2<FLT32> ret ;
		ret[0] = FLT32 (a.mB) ;
		ret[1] = FLT32 (a.mG) ;
		return move (ret) ;
	}

	Buffer3<FLT32> cvt_colorf (CREF<Color3B> a) const {
		Buffer3<FLT32> ret ;
		ret[0] = FLT32 (a.mB) ;
		ret[1] = FLT32 (a.mG) ;
		ret[2] = FLT32 (a.mR) ;
		return move (ret) ;
	}

	Buffer4<FLT32> cvt_colorf (CREF<Color4B> a) const {
		Buffer4<FLT32> ret ;
		ret[0] = FLT32 (a.mB) ;
		ret[1] = FLT32 (a.mG) ;
		ret[2] = FLT32 (a.mR) ;
		ret[3] = FLT32 (a.mA) ;
		return move (ret) ;
	}

	Color1B cvt_colorb (CREF<Buffer1<FLT32>> a) const {
		Color1B ret ;
		ret.mB = BYTE (VAL32 (a[0])) ;
		return move (ret) ;
	}

	Color2B cvt_colorb (CREF<Buffer2<FLT32>> a) const {
		Color2B ret ;
		ret.mB = BYTE (VAL32 (a[0])) ;
		ret.mG = BYTE (VAL32 (a[1])) ;
		return move (ret) ;
	}

	Color3B cvt_colorb (CREF<Buffer3<FLT32>> a) const {
		Color3B ret ;
		ret.mB = BYTE (VAL32 (a[0])) ;
		ret.mG = BYTE (VAL32 (a[1])) ;
		ret.mR = BYTE (VAL32 (a[2])) ;
		return move (ret) ;
	}

	Color4B cvt_colorb (CREF<Buffer4<FLT32>> a) const {
		Color4B ret ;
		ret.mB = BYTE (VAL32 (a[0])) ;
		ret.mG = BYTE (VAL32 (a[1])) ;
		ret.mR = BYTE (VAL32 (a[2])) ;
		ret.mA = BYTE (VAL32 (a[3])) ;
		return move (ret) ;
	}

	FLT32 sampler (CREF<Image<FLT32>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_f64_impl (image ,x ,y) ;
	}

	FLT64 sampler (CREF<Image<FLT64>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_f64_impl (image ,x ,y) ;
	}

	template <class ARG1>
	forceinline ARG1 sampler_f64_impl (CREF<Image<ARG1>> image ,CREF<FLT64> x ,CREF<FLT64> y) const {
		const auto r1x = INDEX (MathProc::round (x ,FLT64 (1))) ;
		const auto r2x = INDEX (MathProc::round (y ,FLT64 (1))) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.cx () - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.cy () - 1) ;
		const auto r5x = MathProc::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = MathProc::max_of (r4x - 1 ,ZERO) ;
		const auto r7x = FLT64 (r3x) + FLT64 (0.5) - x ;
		const auto r8x = FLT64 (r4x) + FLT64 (0.5) - y ;
		const auto r9x = r7x * r8x ;
		const auto r10x = r7x * (1 - r8x) ;
		const auto r11x = (1 - r7x) * (1 - r8x) ;
		const auto r12x = (1 - r7x) * r8x ;
		const auto r13x = image.at (r5x ,r6x) ;
		const auto r14x = image.at (r5x ,r4x) ;
		const auto r15x = image.at (r3x ,r4x) ;
		const auto r16x = image.at (r3x ,r6x) ;
		auto rax = FLT64 (0) ;
		rax += r13x * r9x ;
		rax += r14x * r10x ;
		rax += r15x * r11x ;
		rax += r16x * r12x ;
		return ARG1 (rax) ;
	}
} ;

static const auto mImageProcExternal = External<ImageProcHolder ,ImageProcLayout> (ImageProcImplHolder ()) ;
} ;