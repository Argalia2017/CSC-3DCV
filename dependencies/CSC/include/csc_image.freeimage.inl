#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_image.hpp"

#include "csc_end.h"
#include <FreeImage.h>
#include "csc_begin.h"

inline namespace {
using HFIBITMAP = CSC::DEF<FIBITMAP *> ;
} ;

namespace CSC {
struct ImageProcLayout {
	UniqueRef<BOOL> mContext ;
} ;

class ImageProcImplHolder final implement Fat<ImageProcHolder ,ImageProcLayout> {
public:
	void create (VREF<UniqueRef<ImageProcLayout>> that) const override {
		that = UniqueRef<ImageProcLayout>::make () ;
	}

	void initialize () override {
		self.mContext = UniqueRef<BOOL> ([&] (VREF<BOOL> me) {
			FreeImage_Initialise () ;
			me = TRUE ;
		} ,[&] (VREF<BOOL> me) {
			FreeImage_DeInitialise () ;
		}) ;
	}

	ImageLayout make_image (RREF<BoxLayout> image) const override {
		ImageLayout ret ;
		auto &&rax = keep[TYPE<Box<UniqueRef<HFIBITMAP>>>::expr] (image).deref ;
		const auto r1x = LENGTH (FreeImage_GetWidth (rax)) ;
		const auto r2x = LENGTH (FreeImage_GetHeight (rax)) ;
		const auto r3x = align_of_fibitmap_type (FreeImage_GetImageType (rax)) ;
		const auto r4x = choose_fibitmap_unknown (r3x) ;
		if ifdo (TRUE) {
			const auto r5x = LENGTH (FreeImage_GetBPP (rax)) / 8 ;
			const auto r6x = LENGTH (FreeImage_GetPitch (rax)) ;
			if (r6x % r5x == 0)
				discard ;
			const auto r7x = inline_alignas (r1x ,8) ;
			const auto r8x = r5x * 8 ;
			auto rbx = UniqueRef<HFIBITMAP> ([&] (VREF<HFIBITMAP> me) {
				me = FreeImage_Allocate (VAL32 (r7x) ,VAL32 (r2x) ,VAL32 (r8x)) ;
				assume (me != NULL) ;
			} ,[&] (VREF<HFIBITMAP> me) {
				FreeImage_Unload (me) ;
			}) ;
			const auto r9x = FreeImage_GetBits (rbx) ;
			const auto r10x = LENGTH (FreeImage_GetPitch (rbx)) ;
			FreeImage_ConvertToRawBits (r9x ,rax ,VAL32 (r10x) ,VAL32 (r8x) ,0 ,0 ,0) ;			
			swap (rax ,rbx) ;
		}
		const auto r11x = LENGTH (FreeImage_GetBPP (rax)) / 8 ;
		const auto r12x = LENGTH (FreeImage_GetPitch (rax)) ;
		assume (r12x % r11x == 0) ;
		const auto r13x = r12x / r11x ;
		const auto r14x = FLAG (FreeImage_GetBits (rax)) ;
		const auto r15x = r13x * r2x ;
		const auto r16x = Slice (r14x ,r15x ,r11x) ;
		RefBufferHolder::hold (ret.mImage)->initialize (r4x ,r16x ,move (image)) ;
		ret.mWidth = r1x ;
		ret.mStride = r13x ;
		ImageHolder::hold (ret)->reset () ;
		return move (ret) ;
	}

	ImageLayout make_image (CREF<ImageShape> shape) const override {
		auto rax = Box<UniqueRef<HFIBITMAP>>::make () ;
		const auto r1x = shape.mStep * 8 ;
		rax.deref = UniqueRef<HFIBITMAP> ([&] (VREF<HFIBITMAP> me) {
			me = FreeImage_Allocate (VAL32 (shape.mCX) ,VAL32 (shape.mCY) ,VAL32 (r1x)) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFIBITMAP> me) {
			FreeImage_Unload (me) ;
		}) ;
		return make_image (move (rax)) ;
	}

	ImageLayout make_image (CREF<ImageShape> shape ,CREF<Clazz> clazz ,CREF<LENGTH> channel) const override {
		auto rax = Box<UniqueRef<HFIBITMAP>>::make () ;
		const auto r1x = fibitmap_type_of_clazz (clazz) ;
		const auto r2x = align_of_fibitmap_type (r1x) * channel * 8 ;
		rax.deref = UniqueRef<HFIBITMAP> ([&] (VREF<HFIBITMAP> me) {
			me = FreeImage_AllocateT (r1x ,VAL32 (shape.mCX) ,VAL32 (shape.mCY) ,VAL32 (r2x)) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFIBITMAP> me) {
			FreeImage_Unload (me) ;
		}) ;
		return make_image (move (rax)) ;
	}

	FREE_IMAGE_TYPE fibitmap_type_of_clazz (CREF<Clazz> clazz) const {
		if (clazz.type_align () == 1)
			return FIT_BITMAP ;
		if (clazz.type_align () == 2)
			return FIT_RGBA16 ;
		if (clazz == Clazz (TYPE<FLT32>::expr))
			return FIT_FLOAT ;
		if (clazz == Clazz (TYPE<FLT64>::expr))
			return FIT_DOUBLE ;
		assume (FALSE) ;
		return FIT_UNKNOWN ;
	}

	LENGTH align_of_fibitmap_type (CREF<Just<FREE_IMAGE_TYPE>> type_) const {
		if (type_ == FIT_UNKNOWN)
			return 8 ;
		if (type_ == FIT_BITMAP)
			return 1 ;
		if (type_ == FIT_UINT16)
			return 2 ;
		if (type_ == FIT_INT16)
			return 2 ;
		if (type_ == FIT_UINT32)
			return 4 ;
		if (type_ == FIT_INT32)
			return 4 ;
		if (type_ == FIT_FLOAT)
			return 4 ;
		if (type_ == FIT_DOUBLE)
			return 8 ;
		if (type_ == FIT_COMPLEX)
			return 8 ;
		if (type_ == FIT_RGB16)
			return 2 ;
		if (type_ == FIT_RGBA16)
			return 2 ;
		if (type_ == FIT_RGBF)
			return 4 ;
		if (type_ == FIT_RGBAF)
			return 4 ;
		return 0 ;
	}

	Unknown choose_fibitmap_unknown (CREF<LENGTH> align) const {
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
		auto &&rax = keep[TYPE<Box<UniqueRef<HFIBITMAP>>>::expr] (ImageHolder::hold (image)->raw ()) ;
		return Pointer::from (rax.deref) ;
	}

	CREF<Pointer> peek_image (CREF<ImageLayout> image) const override {
		assert (ImageHolder::hold (image)->fixed ()) ;
		auto &&rax = keep[TYPE<Box<UniqueRef<HFIBITMAP>>>::expr] (ImageHolder::hold (image)->raw ()) ;
		return Pointer::from (rax.deref) ;
	}

	ImageLayout load_image (CREF<String<STR>> file) const override {
		auto rax = Box<UniqueRef<HFIBITMAP>>::make () ;
		const auto r1x = StringProc::stra_from_strs (file) ;
		const auto r2x = FreeImage_GetFIFFromFilename (r1x.deref) ;
		assume (r2x != FIF_UNKNOWN) ;
		rax.deref = UniqueRef<HFIBITMAP> ([&] (VREF<HFIBITMAP> me) {
			me = FreeImage_Load (r2x ,r1x.deref) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFIBITMAP> me) {
			FreeImage_Unload (me) ;
		}) ;
		return make_image (move (rax)) ;
	}

	void save_image (CREF<String<STR>> file ,CREF<ImageLayout> image) const override {
		const auto r1x = StringProc::stra_from_strs (file) ;
		const auto r2x = FreeImage_GetFIFFromFilename (r1x.deref) ;
		assume (r2x != FIF_UNKNOWN) ;
		const auto r3x = ImageHolder::hold (image)->bx () ;
		const auto r4x = ImageHolder::hold (image)->by () ;
		const auto r5x = r3x + ImageHolder::hold (image)->cx () ;
		const auto r6x = r4x + ImageHolder::hold (image)->cy () ;
		auto &&rax = keep[TYPE<Box<UniqueRef<HFIBITMAP>>>::expr] (image.mImage.raw ()).deref ;
		auto rbx = UniqueRef<HFIBITMAP> ([&] (VREF<HFIBITMAP> me) {
			me = FreeImage_CreateView (rax ,VAL32 (r3x) ,VAL32 (r4x) ,VAL32 (r5x) ,VAL32 (r6x)) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFIBITMAP> me) {
			FreeImage_Unload (me) ;
		}) ;
		FreeImage_Save (r2x ,rbx ,r1x.deref) ;
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