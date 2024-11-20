#pragma once

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
class ImageProcImplHolder implement Fat<ImageProcHolder ,ImageProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	ImageLayout make_image (RREF<BoxLayout> image) const override {
		ImageLayout ret ;
		auto &&rax = keep[TYPE<Box<Pin<cv::Mat>>>::expr] (image)->self ;
		const auto r1x = rax.size () ;
		const auto r2x = align_of_cvmat_depth (rax.depth ()) ;
		const auto r3x = rax.channels () ;
		const auto r4x = choose_cvmat_unknown (r2x) ;
		const auto r5x = FLAG (rax.data) ;
		auto &&rbx = keep[TYPE<RefBufferLayout>::expr] (ret.mImage) ;
		RefBufferHolder::hold (rbx)->initialize (r4x ,Box<Pin<cv::Mat>>::make ()) ;
		auto &&rcx = keep[TYPE<Box<Pin<cv::Mat>>>::expr] (RefBufferHolder::hold (rbx)->raw ())->self ;
		assign (rcx ,rax) ;
		rbx.mBuffer = r5x ;
		rbx.mSize = r1x.area () ;
		rbx.mStep = r2x * r3x ;
		ret.mWidth.mCX = r1x.width ;
		ret.mWidth.mCY = r1x.height ;
		ret.mWidth.mStep = r2x * r3x ;
		ImageHolder::hold (ret)->reset () ;
		return move (ret) ;
	}

	ImageLayout make_image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> align ,CREF<LENGTH> channel) const override {
		auto rax = Box<Pin<cv::Mat>>::make () ;
		const auto r1x = cvmat_depth_of_align (align) ;
		const auto r2x = CV_MAKE_TYPE (VAL32 (r1x) ,VAL32 (channel)) ;
		rax->self = cv::Mat (cv::Size (VAL32 (cx_) ,VAL32 (cy_)) ,r2x) ;
		return make_image (move (rax)) ;
	}

	ImageLayout load_image (CREF<String<STR>> file) const override {
		auto rax = Box<Pin<cv::Mat>>::make () ;
		const auto r1x = StringProc::stra_from_strs (file) ;
		rax->self = cv::imread (r1x.self ,cv::IMREAD_UNCHANGED) ;
		assume (!rax->self.empty ()) ;
		return make_image (move (rax)) ;
	}

	VREF<Pointer> poll_image (VREF<ImageLayout> image) const override {
		auto &&rax = keep[TYPE<Box<Pin<cv::Mat>>>::expr] (ImageHolder::hold (image)->raw ()) ;
		return Pointer::from (rax.self) ;
	}

	CREF<Pointer> poll_image (CREF<ImageLayout> image) const override {
		auto &&rax = keep[TYPE<Box<Pin<cv::Mat>>>::expr] (ImageHolder::hold (image)->raw ()) ;
		return Pointer::from (rax.self) ;
	}

	LENGTH cvmat_depth_of_align (CREF<LENGTH> align) const {
		if (align == 1)
			return CV_8U ;
		if (align == 2)
			return CV_16U ;
		if (align == 4)
			return CV_32F ;
		if (align == 8)
			return CV_64F ;
		assume (FALSE) ;
		return 0 ;
	}

	LENGTH align_of_cvmat_depth (CREF<LENGTH> depth) const {
		if (depth == CV_8U)
			return 1 ;
		if (depth == CV_16U)
			return 2 ;
		if (depth == CV_8S)
			return 1 ;
		if (depth == CV_16S)
			return 2 ;
		if (depth == CV_32S)
			return 4 ;
		if (depth == CV_16F)
			return 2 ;
		if (depth == CV_32F)
			return 4 ;
		if (depth == CV_64F)
			return 8 ;
		assume (FALSE) ;
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
		return BufferUnknownBinder<BYTE> () ;
	}

	void save_image (CREF<String<STR>> file ,CREF<ImageLayout> image) const override {
		const auto r1x = StringProc::stra_from_strs (file) ;
		auto &&rax = keep[TYPE<Box<Pin<cv::Mat>>>::expr] (image.mImage.raw ())->self ;
		cv::imwrite (r1x.self ,rax) ;
	}

	Color1B sampler (CREF<Image<Color1B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_impl (image ,FLT32 (x) ,FLT32 (y)) ;
	}

	Color2B sampler (CREF<Image<Color2B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_impl (image ,FLT32 (x) ,FLT32 (y)) ;
	}

	Color3B sampler (CREF<Image<Color3B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_impl (image ,FLT32 (x) ,FLT32 (y)) ;
	}

	Color4B sampler (CREF<Image<Color4B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_impl (image ,FLT32 (x) ,FLT32 (y)) ;
	}

	template <class ARG1>
	ARG1 sampler_impl (CREF<Image<ARG1>> image ,CREF<FLT32> x ,CREF<FLT32> y) const {
		ARG1 ret ;
		const auto r1x = VAL (MathProc::round (x ,FLT32 (1))) ;
		const auto r2x = VAL (MathProc::round (y ,FLT32 (1))) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.cx () - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.cy () - 1) ;
		const auto r5x = MathProc::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = MathProc::max_of (r4x - 1 ,ZERO) ;
		const auto r7x = FLT32 (r3x) + FLT32 (0.5) - x ;
		const auto r8x = FLT32 (r4x) + FLT32 (0.5) - y ;
		const auto r9x = 1 - r7x ;
		const auto r10x = 1 - r8x ;
		if ifdo (TRUE) {
			const auto r11x = cvt_colorf (image.at (r5x ,r6x)) ;
			const auto r12x = cvt_colorf (image.at (r5x ,r4x)) ;
			const auto r13x = cvt_colorf (image.at (r3x ,r4x)) ;
			const auto r14x = cvt_colorf (image.at (r3x ,r6x)) ;
			auto rax = Buffer<FLT32 ,SIZE_OF<ARG1>> () ;
			for (auto &&i : iter (0 ,rax.size ())) {
				rax[i] = 0 ;
				rax[i] += r11x[i] * (r7x * r8x) ;
				rax[i] += r12x[i] * (r7x * r10x) ;
				rax[i] += r13x[i] * (r9x * r10x) ;
				rax[i] += r14x[i] * (r9x * r8x) ;
				rax[i] = MathProc::clamp (rax[i] ,FLT32 (0) ,FLT32 (255)) ;
			}
			ret = cvt_colorb (rax) ;
		}
		return move (ret) ;
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
		return sampler_impl (image ,x ,y) ;
	}

	FLT64 sampler (CREF<Image<FLT64>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_impl (image ,x ,y) ;
	}

	template <class ARG1>
	ARG1 sampler_impl (CREF<Image<ARG1>> image ,CREF<FLT64> x ,CREF<FLT64> y) const {
		ARG1 ret ;
		const auto r1x = VAL (MathProc::round (x ,FLT64 (1))) ;
		const auto r2x = VAL (MathProc::round (y ,FLT64 (1))) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.cx () - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.cy () - 1) ;
		const auto r5x = MathProc::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = MathProc::max_of (r4x - 1 ,ZERO) ;
		const auto r7x = FLT64 (r3x) + FLT64 (0.5) - x ;
		const auto r8x = FLT64 (r4x) + FLT64 (0.5) - y ;
		const auto r9x = 1 - r7x ;
		const auto r10x = 1 - r8x ;
		if ifdo (TRUE) {
			const auto r11x = image.at (r5x ,r6x) ;
			const auto r12x = image.at (r5x ,r4x) ;
			const auto r13x = image.at (r3x ,r4x) ;
			const auto r14x = image.at (r3x ,r6x) ;
			FLT64 ret = FLT64 (0) ;
			ret += r11x * (r7x * r8x) ;
			ret += r12x * (r7x * r10x) ;
			ret += r13x * (r9x * r10x) ;
			ret += r14x * (r9x * r8x) ;
			ret = MathProc::clamp (ret ,FLT64 (0) ,FLT64 (255)) ;
		}
		return move (ret) ;
	}
} ;

static const auto mImageProcExternal = External<ImageProcHolder ,ImageProcLayout>::declare (ImageProcImplHolder ()) ;
} ;