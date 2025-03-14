#pragma once

#ifndef __CSC_MATH__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_math.hpp"

#include "csc_end.h"
#include <cmath>
#include "csc_begin.h"

namespace CSC {
struct MathProcLayout {} ;

class MathProcImplHolder final implement Fat<MathProcHolder ,MathProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	BOOL is_inf (CREF<FLT32> a) const override {
		return std::isinf (a) ;
	}

	BOOL is_inf (CREF<FLT64> a) const override {
		return std::isinf (a) ;
	}

	BOOL is_low (CREF<FLT32> a) const override {
		if (a < -FLT32_LOW)
			return FALSE ;
		if (a > +FLT32_LOW)
			return FALSE ;
		return TRUE ;
	}

	BOOL is_low (CREF<FLT64> a) const override {
		if (a < -FLT64_LOW)
			return FALSE ;
		if (a > +FLT64_LOW)
			return FALSE ;
		return TRUE ;
	}

	VAL32 step (CREF<VAL32> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	VAL64 step (CREF<VAL64> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	FLT32 step (CREF<FLT32> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	FLT64 step (CREF<FLT64> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	VAL32 sign (CREF<VAL32> a) const override {
		if (a >= 0)
			return 1 ;
		return -1 ;
	}

	VAL64 sign (CREF<VAL64> a) const override {
		if (a >= 0)
			return 1 ;
		return -1 ;
	}

	FLT32 sign (CREF<FLT32> a) const override {
		if (a >= 0)
			return 1 ;
		return -1 ;
	}

	FLT64 sign (CREF<FLT64> a) const override {
		if (a >= 0)
			return 1 ;
		return -1 ;
	}

	VAL32 square (CREF<VAL32> a) const override {
		return a * a ;
	}

	VAL64 square (CREF<VAL64> a) const override {
		return a * a ;
	}

	FLT32 square (CREF<FLT32> a) const override {
		return a * a ;
	}

	FLT64 square (CREF<FLT64> a) const override {
		return a * a ;
	}

	FLT32 sqrt (CREF<FLT32> a) const override {
		return std::sqrt (a) ;
	}

	FLT64 sqrt (CREF<FLT64> a) const override {
		return std::sqrt (a) ;
	}

	VAL32 cubic (CREF<VAL32> a) const override {
		return a * a * a ;
	}

	VAL64 cubic (CREF<VAL64> a) const override {
		return a * a * a ;
	}

	FLT32 cubic (CREF<FLT32> a) const override {
		return a * a * a ;
	}

	FLT64 cubic (CREF<FLT64> a) const override {
		return a * a * a ;
	}

	FLT32 cbrt (CREF<FLT32> a) const override {
		return std::cbrt (a) ;
	}

	FLT64 cbrt (CREF<FLT64> a) const override {
		return std::cbrt (a) ;
	}

	FLT32 pow (CREF<FLT32> a ,CREF<VAL32> b) const override {
		return FLT32 (std::pow (a ,b)) ;
	}

	FLT64 pow (CREF<FLT64> a ,CREF<VAL32> b) const override {
		return FLT64 (std::pow (a ,b)) ;
	}

	FLT32 hypot (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::hypot (a ,b) ;
	}

	FLT64 hypot (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::hypot (a ,b) ;
	}

	VAL32 abs (CREF<VAL32> a) const override {
		if (a == VAL32_ABS)
			return 0 ;
		if (a < 0)
			return -a ;
		return a ;
	}

	VAL64 abs (CREF<VAL64> a) const override {
		if (a == VAL64_ABS)
			return 0 ;
		if (a < 0)
			return -a ;
		return a ;
	}

	FLT32 abs (CREF<FLT32> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	FLT64 abs (CREF<FLT64> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	FLT32 inverse (CREF<FLT32> a) const override {
		if (abs (a) < FLT32_EPS)
			return 0 ;
		return 1 / a ;
	}

	FLT64 inverse (CREF<FLT64> a) const override {
		if (abs (a) < FLT64_EPS)
			return 0 ;
		return 1 / a ;
	}

	FLT32 floor (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::floor (a * inverse (b)) * b ;
	}

	FLT64 floor (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::floor (a * inverse (b)) * b ;
	}

	FLT32 round (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::round (a * inverse (b)) * b ;
	}

	FLT64 round (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::round (a * inverse (b)) * b ;
	}

	FLT32 ceil (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::ceil (a * inverse (b)) * b ;
	}

	FLT64 ceil (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::ceil (a * inverse (b)) * b ;
	}

	VAL32 clamp (CREF<VAL32> a ,CREF<VAL32> min_ ,CREF<VAL32> max_) const override {
		if (a <= min_)
			return min_ ;
		if (a >= max_)
			return max_ ;
		return a ;
	}

	VAL64 clamp (CREF<VAL64> a ,CREF<VAL64> min_ ,CREF<VAL64> max_) const override {
		if (a <= min_)
			return min_ ;
		if (a >= max_)
			return max_ ;
		return a ;
	}

	FLT32 clamp (CREF<FLT32> a ,CREF<FLT32> min_ ,CREF<FLT32> max_) const override {
		if (a <= min_)
			return min_ ;
		if (a >= max_)
			return max_ ;
		return a ;
	}

	FLT64 clamp (CREF<FLT64> a ,CREF<FLT64> min_ ,CREF<FLT64> max_) const override {
		if (a <= min_)
			return min_ ;
		if (a >= max_)
			return max_ ;
		return a ;
	}

	VAL32 lerp (CREF<FLT64> a ,CREF<VAL32> min_ ,CREF<VAL32> max_) const override {
		const auto r1x = MathProc::abs (a) ;
		const auto r2x = r1x - MathProc::floor (r1x / 2 ,FLT64 (1)) * 2 ;
		const auto r3x = 1 - MathProc::abs (r2x - 1) ;
		const auto r4x = FLT64 (max_ - min_) * r3x ;
		const auto r5x = VAL32 (round (r4x ,FLT64 (1))) ;
		return min_ + r5x ;
	}

	VAL64 lerp (CREF<FLT64> a ,CREF<VAL64> min_ ,CREF<VAL64> max_) const override {
		const auto r1x = MathProc::abs (a) ;
		const auto r2x = r1x - MathProc::floor (r1x / 2 ,FLT64 (1)) * 2 ;
		const auto r3x = 1 - MathProc::abs (r2x - 1) ;
		const auto r4x = FLT64 (max_ - min_) * r3x ;
		const auto r5x = VAL64 (round (r4x ,FLT64 (1))) ;
		return min_ + r5x ;
	}

	FLT32 cos (CREF<FLT32> a) const override {
		return std::cos (a) ;
	}

	FLT64 cos (CREF<FLT64> a) const override {
		return std::cos (a) ;
	}

	FLT32 sin (CREF<FLT32> a) const override {
		return std::sin (a) ;
	}

	FLT64 sin (CREF<FLT64> a) const override {
		return std::sin (a) ;
	}

	FLT32 tan (CREF<FLT32> a) const override {
		return std::tan (a) ;
	}

	FLT64 tan (CREF<FLT64> a) const override {
		return std::tan (a) ;
	}

	FLT32 acos (CREF<FLT32> a) const override {
		return std::acos (a) ;
	}

	FLT64 acos (CREF<FLT64> a) const override {
		return std::acos (a) ;
	}

	FLT32 asin (CREF<FLT32> a) const override {
		return std::asin (a) ;
	}

	FLT64 asin (CREF<FLT64> a) const override {
		return std::asin (a) ;
	}

	FLT32 atan (CREF<FLT32> y ,CREF<FLT32> x) const override {
		return std::atan2 (y ,x) ;
	}

	FLT64 atan (CREF<FLT64> y ,CREF<FLT64> x) const override {
		return std::atan2 (y ,x) ;
	}

	FLT32 exp (CREF<FLT32> a) const override {
		return std::exp (a) ;
	}

	FLT64 exp (CREF<FLT64> a) const override {
		return std::exp (a) ;
	}

	FLT32 log (CREF<FLT32> a) const override {
		return std::log (a) ;
	}

	FLT64 log (CREF<FLT64> a) const override {
		return std::log (a) ;
	}

	FLT32 pdf (CREF<FLT32> a) const override {
		const auto r1x = -square (a) * FLT32 (0.5) ;
		return exp (r1x) * FLT32 (MATH_PDF0) ;
	}

	FLT64 pdf (CREF<FLT64> a) const override {
		const auto r1x = -square (a) * FLT64 (0.5) ;
		return exp (r1x) * FLT64 (MATH_PDF0) ;
	}

	FLT32 cbf (CREF<FLT32> a) const override {
		const auto r1x = a * FLT32 (inverse (MATH_SQRT2)) ;
		return (1 + std::erf (r1x)) * FLT32 (0.5) ;
	}

	FLT64 cbf (CREF<FLT64> a) const override {
		const auto r1x = a * FLT64 (inverse (MATH_SQRT2)) ;
		return (1 + std::erf (r1x)) * FLT64 (0.5) ;
	}

	BOOL all_of (CREF<BOOL> a ,CREF<WrapperLayout> b) const override {
		if (!a)
			return FALSE ;
		auto &&rax = keep[TYPE<Wrapper<BOOL>>::expr] (b) ;
		for (auto &&i : iter (0 ,rax.rank ())) {
			if (!rax[i])
				return FALSE ;
		}
		return TRUE ;
	}

	BOOL any_of (CREF<BOOL> a ,CREF<WrapperLayout> b) const override {
		if (a)
			return TRUE ;
		auto &&rax = keep[TYPE<Wrapper<BOOL>>::expr] (b) ;
		for (auto &&i : iter (0 ,rax.rank ())) {
			if (rax[i])
				return TRUE ;
		}
		return FALSE ;
	}

	VAL32 max_of (CREF<VAL32> a ,CREF<WrapperLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	VAL64 max_of (CREF<VAL64> a ,CREF<WrapperLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	FLT32 max_of (CREF<FLT32> a ,CREF<WrapperLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	FLT64 max_of (CREF<FLT64> a ,CREF<WrapperLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	template <class ARG1>
	forceinline ARG1 max_of_impl (CREF<ARG1> a ,CREF<WrapperLayout> b) const {
		ARG1 ret = a ;
		auto &&rax = keep[TYPE<Wrapper<ARG1>>::expr] (b) ;
		for (auto &&i : iter (0 ,rax.rank ())) {
			if (ret >= rax[i])
				continue ;
			ret = rax[i] ;
		}
		return move (ret) ;
	}

	VAL32 min_of (CREF<VAL32> a ,CREF<WrapperLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	VAL64 min_of (CREF<VAL64> a ,CREF<WrapperLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	FLT32 min_of (CREF<FLT32> a ,CREF<WrapperLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	FLT64 min_of (CREF<FLT64> a ,CREF<WrapperLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	template <class ARG1>
	forceinline ARG1 min_of_impl (CREF<ARG1> a ,CREF<WrapperLayout> b) const {
		ARG1 ret = a ;
		auto &&rax = keep[TYPE<Wrapper<ARG1>>::expr] (b) ;
		for (auto &&i : iter (0 ,rax.rank ())) {
			if (ret <= rax[i])
				continue ;
			ret = rax[i] ;
		}
		return move (ret) ;
	}
} ;

exports CREF<OfThis<UniqueRef<MathProcLayout>>> MathProcHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<MathProcLayout>> ret ;
		ret.mThis = UniqueRef<MathProcLayout>::make () ;
		MathProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<MathProcHolder> MathProcHolder::hold (VREF<MathProcLayout> that) {
	return VFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}

exports CFat<MathProcHolder> MathProcHolder::hold (CREF<MathProcLayout> that) {
	return CFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}

class NormalErrorImplHolder final implement Fat<NormalErrorHolder ,NormalErrorLayout> {
public:
	void concat (CREF<FLT64> error) override {
		const auto r1x = FLT64 (self.mCount) ;
		const auto r2x = MathProc::inverse (r1x + 1) ;
		const auto r3x = error - self.mAvgError ;
		self.mMaxError = MathProc::max_of (self.mMaxError ,error) ;
		self.mAvgError = self.mAvgError + r3x * r2x ;
		const auto r4x = r1x * r2x * MathProc::square (self.mStdError) + r1x * MathProc::square (r3x * r2x) ;
		self.mStdError = MathProc::sqrt (r4x) ;
		self.mCount = LENGTH (r1x + 1) ;
	}
} ;

exports VFat<NormalErrorHolder> NormalErrorHolder::hold (VREF<NormalErrorLayout> that) {
	return VFat<NormalErrorHolder> (NormalErrorImplHolder () ,that) ;
}

exports CFat<NormalErrorHolder> NormalErrorHolder::hold (CREF<NormalErrorLayout> that) {
	return CFat<NormalErrorHolder> (NormalErrorImplHolder () ,that) ;
}

struct FloatProcLayout {} ;

class FloatProcImplHolder final implement Fat<FloatProcHolder ,FloatProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	LENGTH value_precision () const override {
		return LENGTH (18) ;
	}

	LENGTH float_precision () const override {
		return LENGTH (15) ;
	}

	FLT64 encode (CREF<Notation> fexp2) const override {
		assert (fexp2.mRadix == 2) ;
		auto rax = fexp2 ;
		if ifdo (TRUE) {
			if (rax.mMantissa == 0)
				discard ;
			while (TRUE) {
				if (!ByteProc::any_bit (QUAD (rax.mMantissa) ,QUAD (0XFFE0000000000000)))
					break ;
				rax.mMantissa = VAL64 (QUAD (rax.mMantissa) >> 1) ;
				rax.mDownflow = 0 ;
				rax.mExponent++ ;
			}
			while (TRUE) {
				if (ByteProc::any_bit (QUAD (rax.mMantissa) ,QUAD (0XFFF0000000000000)))
					break ;
				rax.mMantissa = VAL64 (QUAD (rax.mMantissa) << 1) ;
				rax.mDownflow = 0 ;
				rax.mExponent-- ;
			}
		}
		if ifdo (TRUE) {
			const auto r1x = LENGTH (-1074) - LENGTH (rax.mExponent) ;
			if (r1x <= 0)
				discard ;
			rax.mMantissa = VAL64 (QUAD (rax.mMantissa) >> r1x) ;
			rax.mDownflow = 0 ;
			rax.mExponent = -1075 ;
		}
		rax.mExponent += 1075 ;
		if ifdo (TRUE) {
			if (rax.mMantissa != 0)
				discard ;
			rax.mExponent = 0 ;
		}
		const auto r2x = invoke ([&] () {
			if (!fexp2.mSign)
				return QUAD (0X00) ;
			return QUAD (0X8000000000000000) ;
		}) ;
		const auto r3x = (QUAD (rax.mExponent) << 52) & QUAD (0X7FF0000000000000) ;
		const auto r4x = QUAD (rax.mMantissa) & QUAD (0X000FFFFFFFFFFFFF) ;
		const auto r5x = r2x | r3x | r4x ;
		return bitwise[TYPE<FLT64>::expr] (r5x) ;
	}

	Notation decode (CREF<FLT64> float_) const override {
		Notation ret ;
		ret.mRadix = 2 ;
		ret.mPrecision = 0 ;
		const auto r1x = bitwise[TYPE<QUAD>::expr] (float_) ;
		const auto r2x = r1x & QUAD (0X7FF0000000000000) ;
		const auto r3x = r1x & QUAD (0X000FFFFFFFFFFFFF) ;
		ret.mSign = ByteProc::any_bit (r1x ,QUAD (0X8000000000000000)) ;
		ret.mMantissa = VAL64 (r3x) ;
		ret.mDownflow = 0 ;
		if ifdo (TRUE) {
			if (r2x == QUAD (0X00))
				discard ;
			ret.mMantissa = VAL64 (QUAD (ret.mMantissa) | QUAD (0X0010000000000000)) ;
		}
		ret.mExponent = VAL64 (r2x >> 52) ;
		ret.mExponent -= 1075 - LENGTH (r2x == QUAD (0X00)) ;
		if ifdo (TRUE) {
			if (ret.mMantissa != 0)
				discard ;
			ret.mExponent = 0 ;
		}
		if ifdo (TRUE) {
			if (ret.mMantissa == 0)
				discard ;
			while (TRUE) {
				if (ByteProc::any_bit (QUAD (ret.mMantissa) ,QUAD (0X0000000000000001)))
					break ;
				ret.mMantissa = VAL64 (QUAD (ret.mMantissa) >> 1) ;
				ret.mExponent++ ;
			}
		}
		return move (ret) ;
	}

	Notation fexp2_multiply (CREF<Notation> obj1 ,CREF<Notation> obj2) const {
		assert (obj1.mRadix == 2) ;
		assert (obj2.mRadix == 2) ;
		Notation ret ;
		ret.mRadix = 2 ;
		ret.mPrecision = 0 ;
		ret.mSign = MathProc::any_of (obj1.mSign ,obj2.mSign) ;
		const auto r1x = LENGTH (32) ;
		const auto r2x = QUAD (ByteProc::pow_bit (r1x) - 1) ;
		const auto r3x = ByteProc::pow_bit (r1x - 1) ;
		const auto r4x = VAL64 (QUAD (obj1.mMantissa) >> r1x) ;
		const auto r5x = VAL64 (QUAD (obj1.mMantissa) & r2x) ;
		const auto r6x = VAL64 (QUAD (obj2.mMantissa) >> r1x) ;
		const auto r7x = VAL64 (QUAD (obj2.mMantissa) & r2x) ;
		const auto r8x = r4x * r6x ;
		const auto r9x = r5x * r6x ;
		const auto r10x = r4x * r7x ;
		const auto r11x = r5x * r7x ;
		//@error: lose a part of precision
		auto act = TRUE ;
		if ifdo (act) {
			if (r8x == 0)
				discard ;
			const auto r12x = VAL64 (QUAD (r9x) >> r1x) ;
			const auto r13x = VAL64 (QUAD (r9x) & r2x) ;
			const auto r14x = VAL64 (QUAD (r10x) >> r1x) ;
			const auto r15x = VAL64 (QUAD (r10x) & r2x) ;
			const auto r16x = VAL64 (QUAD (r11x) >> r1x) ;
			const auto r17x = VAL64 (QUAD (r13x + r15x + r16x + r3x) >> r1x) ;
			ret.mMantissa = r8x + r12x + r14x + r17x ;
			ret.mDownflow = 0 ;
			ret.mExponent = obj1.mExponent + obj2.mExponent + r1x * 2 ;
		}
		if ifdo (act) {
			if (r9x == 0)
				if (r10x == 0)
					discard ;
			const auto r18x = VAL64 (QUAD (r11x + r3x) >> r1x) ;
			ret.mMantissa = r9x + r10x + r18x ;
			ret.mDownflow = 0 ;
			ret.mExponent = obj1.mExponent + obj2.mExponent + r1x ;
		}
		if ifdo (act) {
			ret.mMantissa = r11x ;
			ret.mDownflow = 0 ;
			ret.mExponent = obj1.mExponent + obj2.mExponent ;
		}
		return move (ret) ;
	}

	Notation fexp2_from_fexp10 (CREF<Notation> fexp10) const override {
		assert (fexp10.mRadix == 10) ;
		Notation ret ;
		ret.mRadix = 2 ;
		ret.mPrecision = 0 ;
		ret.mSign = fexp10.mSign ;
		ret.mMantissa = fexp10.mMantissa ;
		ret.mDownflow = 0 ;
		ret.mExponent = 0 ;
		const auto r1x = FEXP2Cache::instance ()[fexp10.mExponent] ;
		ret = fexp2_multiply (ret ,r1x) ;
		return move (ret) ;
	}

	Notation fexp10_downflow (CREF<Notation> fexp10 ,CREF<VAL64> half) const {
		Notation ret = fexp10 ;
		if ifdo (TRUE) {
			if (ret.mMantissa >= 0)
				discard ;
			ret.mMantissa = VAL64 (QUAD (ret.mMantissa) >> 1) ;
			ret.mMantissa /= 5 ;
			ret.mExponent++ ;
		}
		const auto r1x = MathProc::square (half) ;
		while (TRUE) {
			if (ret.mMantissa < r1x)
				break ;
			ret.mMantissa /= 10 ;
			ret.mExponent++ ;
		}
		return move (ret) ;
	}

	Notation fexp10_multiply (CREF<Notation> obj1 ,CREF<Notation> obj2) const {
		assert (obj1.mRadix == 10) ;
		assert (obj2.mRadix == 10) ;
		Notation ret ;
		ret.mRadix = 10 ;
		ret.mPrecision = 0 ;
		ret.mSign = MathProc::any_of (obj1.mSign ,obj2.mSign) ;
		const auto r1x = LENGTH (9) ;
		const auto r2x = VAL64 (1000000000) ;
		const auto r3x = VAL64 (500000000) ;
		const auto r4x = fexp10_downflow (obj1 ,r2x) ;
		const auto r5x = fexp10_downflow (obj2 ,r2x) ;
		const auto r6x = r4x.mMantissa / r2x ;
		const auto r7x = r4x.mMantissa % r2x ;
		const auto r8x = r5x.mMantissa / r2x ;
		const auto r9x = r5x.mMantissa % r2x ;
		const auto r10x = r6x * r8x ;
		const auto r11x = r7x * r8x ;
		const auto r12x = r6x * r9x ;
		const auto r13x = r7x * r9x ;
		//@error: lose a part of precision
		auto act = TRUE ;
		if ifdo (act) {
			if (r10x == 0)
				discard ;
			const auto r14x = r11x / r2x ;
			const auto r15x = r11x % r2x ;
			const auto r16x = r12x / r2x ;
			const auto r17x = r12x % r2x ;
			const auto r18x = r13x / r2x ;
			const auto r19x = (r15x + r17x + r18x + r3x) / r2x ;
			ret.mMantissa = r10x + r14x + r16x + r19x ;
			ret.mDownflow = 0 ;
			ret.mExponent = r4x.mExponent + r5x.mExponent + r1x * 2 ;
		}
		if ifdo (act) {
			if (r11x == 0)
				if (r12x == 0)
					discard ;
			const auto r20x = (r13x + r3x) / r2x ;
			ret.mMantissa = r11x + r12x + r20x ;
			ret.mDownflow = 0 ;
			ret.mExponent = r4x.mExponent + r5x.mExponent + r1x ;
		}
		if ifdo (act) {
			ret.mMantissa = r13x ;
			ret.mDownflow = 0 ;
			ret.mExponent = r4x.mExponent + r5x.mExponent ;
		}
		return move (ret) ;
	}

	Notation fexp10_from_fexp2 (CREF<Notation> fexp2) const override {
		assert (fexp2.mRadix == 2) ;
		Notation ret ;
		ret.mRadix = 10 ;
		ret.mPrecision = 0 ;
		ret.mSign = fexp2.mSign ;
		ret.mMantissa = fexp2.mMantissa ;
		ret.mDownflow = 0 ;
		ret.mExponent = 0 ;
		const auto r1x = FEXP10Cache::instance ()[fexp2.mExponent] ;
		ret = fexp10_multiply (ret ,r1x) ;
		return move (ret) ;
	}
} ;

exports CREF<OfThis<UniqueRef<FloatProcLayout>>> FloatProcHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<FloatProcLayout>> ret ;
		ret.mThis = UniqueRef<FloatProcLayout>::make () ;
		FloatProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FloatProcHolder> FloatProcHolder::hold (VREF<FloatProcLayout> that) {
	return VFat<FloatProcHolder> (FloatProcImplHolder () ,that) ;
}

exports CFat<FloatProcHolder> FloatProcHolder::hold (CREF<FloatProcLayout> that) {
	return CFat<FloatProcHolder> (FloatProcImplHolder () ,that) ;
}

template class External<FEXP2CacheHolder ,FEXP2CacheLayout> ;

exports CREF<OfThis<UniqueRef<FEXP2CacheLayout>>> FEXP2CacheHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<FEXP2CacheLayout>> ret ;
		External<FEXP2CacheHolder ,FEXP2CacheLayout>::declare ().create (ret.mThis) ;
		FEXP2CacheHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FEXP2CacheHolder> FEXP2CacheHolder::hold (VREF<FEXP2CacheLayout> that) {
	return VFat<FEXP2CacheHolder> (External<FEXP2CacheHolder ,FEXP2CacheLayout>::declare () ,that) ;
}

exports CFat<FEXP2CacheHolder> FEXP2CacheHolder::hold (CREF<FEXP2CacheLayout> that) {
	return CFat<FEXP2CacheHolder> (External<FEXP2CacheHolder ,FEXP2CacheLayout>::declare () ,that) ;
}

template class External<FEXP10CacheHolder ,FEXP10CacheLayout> ;

exports CREF<OfThis<UniqueRef<FEXP10CacheLayout>>> FEXP10CacheHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<FEXP10CacheLayout>> ret ;
		External<FEXP10CacheHolder ,FEXP10CacheLayout>::declare ().create (ret.mThis) ;
		FEXP10CacheHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FEXP10CacheHolder> FEXP10CacheHolder::hold (VREF<FEXP10CacheLayout> that) {
	return VFat<FEXP10CacheHolder> (External<FEXP10CacheHolder ,FEXP10CacheLayout>::declare () ,that) ;
}

exports CFat<FEXP10CacheHolder> FEXP10CacheHolder::hold (CREF<FEXP10CacheLayout> that) {
	return CFat<FEXP10CacheHolder> (External<FEXP10CacheHolder ,FEXP10CacheLayout>::declare () ,that) ;
}

struct ByteProcLayout {} ;

class ByteProcImplHolder final implement Fat<ByteProcHolder ,ByteProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	BYTE split_low (CREF<WORD> a) const override {
		return BYTE (a) ;
	}

	WORD split_low (CREF<CHAR> a) const override {
		return WORD (a) ;
	}

	CHAR split_low (CREF<QUAD> a) const override {
		return CHAR (a) ;
	}

	BYTE split_high (CREF<WORD> a) const override {
		return BYTE (a >> 8) ;
	}

	WORD split_high (CREF<CHAR> a) const override {
		return WORD (a >> 16) ;
	}

	CHAR split_high (CREF<QUAD> a) const override {
		return CHAR (a >> 32) ;
	}

	WORD merge (CREF<BYTE> high_ ,CREF<BYTE> low_) const override {
		return (WORD (high_) << 8) | WORD (low_) ;
	}

	CHAR merge (CREF<WORD> high_ ,CREF<WORD> low_) const override {
		return (CHAR (high_) << 16) | CHAR (low_) ;
	}

	QUAD merge (CREF<CHAR> high_ ,CREF<CHAR> low_) const override {
		return (QUAD (high_) << 32) | QUAD (low_) ;
	}

	BYTE reverse (CREF<BYTE> a) const override {
		return a ;
	}

	WORD reverse (CREF<WORD> a) const override {
		auto rax = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<WORD>>>::expr] (a) ;
		swap (rax[0] ,rax[1]) ;
		return bitwise[TYPE<WORD>::expr] (rax) ;
	}

	CHAR reverse (CREF<CHAR> a) const override {
		auto rax = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<CHAR>>>::expr] (a) ;
		swap (rax[0] ,rax[3]) ;
		swap (rax[1] ,rax[2]) ;
		return bitwise[TYPE<CHAR>::expr] (rax) ;
	}

	QUAD reverse (CREF<QUAD> a) const override {
		auto rax = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<QUAD>>>::expr] (a) ;
		swap (rax[0] ,rax[7]) ;
		swap (rax[1] ,rax[6]) ;
		swap (rax[2] ,rax[5]) ;
		swap (rax[3] ,rax[4]) ;
		return bitwise[TYPE<QUAD>::expr] (rax) ;
	}

	BOOL any_bit (CREF<BYTE> a ,CREF<BYTE> mask) const override {
		return (a & mask) != BYTE (0X00) ;
	}

	BOOL any_bit (CREF<WORD> a ,CREF<WORD> mask) const override {
		return (a & mask) != WORD (0X00) ;
	}

	BOOL any_bit (CREF<CHAR> a ,CREF<CHAR> mask) const override {
		return (a & mask) != CHAR (0X00) ;
	}

	BOOL any_bit (CREF<QUAD> a ,CREF<QUAD> mask) const override {
		return (a & mask) != QUAD (0X00) ;
	}

	BOOL all_bit (CREF<BYTE> a ,CREF<BYTE> mask) const override {
		return (a & mask) == mask ;
	}

	BOOL all_bit (CREF<WORD> a ,CREF<WORD> mask) const override {
		return (a & mask) == mask ;
	}

	BOOL all_bit (CREF<CHAR> a ,CREF<CHAR> mask) const override {
		return (a & mask) == mask ;
	}

	BOOL all_bit (CREF<QUAD> a ,CREF<QUAD> mask) const override {
		return (a & mask) == mask ;
	}

	BYTE binary (CREF<BYTE> a) const override {
		if (a == BYTE (0X00))
			return BYTE (0X00) ;
		return ~BYTE (0X00) ;
	}

	WORD binary (CREF<WORD> a) const override {
		if (a == WORD (0X00))
			return WORD (0X00) ;
		return ~WORD (0X00) ;
	}

	CHAR binary (CREF<CHAR> a) const override {
		if (a == CHAR (0X00))
			return CHAR (0X00) ;
		return ~CHAR (0X00) ;
	}

	QUAD binary (CREF<QUAD> a) const override {
		if (a == QUAD (0X00))
			return QUAD (0X00) ;
		return ~QUAD (0X00) ;
	}

	INDEX pow_bit (CREF<LENGTH> nth) const override {
		return INDEX (QUAD (0X01) << nth) ;
	}

	LENGTH popcount (CREF<BYTE> a) const override {
		static const ARR<VAL32 ,ENUM<256>> mCache {
			0 ,1 ,1 ,2 ,1 ,2 ,2 ,3 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,5 ,6 ,6 ,7 ,6 ,7 ,7 ,8} ;
		return LENGTH (mCache[INDEX (a)]) ;
	}

	LENGTH lowcount (CREF<BYTE> a) const override {
		static const ARR<VAL32 ,ENUM<256>> mCache {
			8 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			7 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0} ;
		return LENGTH (mCache[INDEX (a)]) ;
	}
} ;

exports CREF<OfThis<UniqueRef<ByteProcLayout>>> ByteProcHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<ByteProcLayout>> ret ;
		ret.mThis = UniqueRef<ByteProcLayout>::make () ;
		ByteProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ByteProcHolder> ByteProcHolder::hold (VREF<ByteProcLayout> that) {
	return VFat<ByteProcHolder> (ByteProcImplHolder () ,that) ;
}

exports CFat<ByteProcHolder> ByteProcHolder::hold (CREF<ByteProcLayout> that) {
	return CFat<ByteProcHolder> (ByteProcImplHolder () ,that) ;
}

class IntegerImplHolder final implement Fat<IntegerHolder ,IntegerLayout> {
private:
	using INTEGER_MIN_SIZE = RANK8 ;

public:
	void initialize (CREF<LENGTH> size_) override {
		const auto r1x = inline_alignas (size_ ,INTEGER_MIN_SIZE::expr) + INTEGER_MIN_SIZE::expr ;
		self.mInteger = RefBuffer<BYTE> (r1x) ;
		self.mWidth = size_ ;
	}

	void initialize (CREF<IntegerLayout> that) override {
		self.mInteger = RefBuffer<BYTE> (that.mWidth) ;
		for (auto &&i : iter (0 ,that.mWidth))
			self.mInteger[i] = that.mInteger[i] ;
	}

	static BYTE get (CREF<IntegerLayout> that ,CREF<INDEX> index) {
		INDEX ix = MathProc::min_of (index ,that.mWidth - 1) ;
		return that.mInteger[ix] ;
	}

	LENGTH size () const override {
		if (!self.mInteger.exist ())
			return 0 ;
		return self.mWidth ;
	}

	VAL64 fetch () const override {
		auto rax = QUAD (0X00) ;
		const auto r1x = SIZE_OF<VAL64>::expr ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = QUAD (get (self ,i)) << (i * 8) ;
			rax |= r2x ;
		}
		return VAL64 (rax) ;
	}

	void store (CREF<VAL64> item) override {
		const auto r1x = SIZE_OF<VAL64>::expr ;
		assert (self.mInteger.size () >= r1x) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = QUAD (item) >> (i * 8) ;
			self.mInteger[i] = BYTE (r2x) ;
		}
		self.mWidth = r1x ;
		check_mask (self) ;
	}

	BOOL equal (CREF<IntegerLayout> that) const override {
		const auto r1x = inline_max (self.mWidth ,that.mWidth) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = r1x - 1 - i ;
			const auto r2x = inline_equal (get (self ,ix) ,get (that ,ix)) ;
			if (!r2x)
				return r2x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<IntegerLayout> that) const override {
		const auto r1x = inline_max (self.mWidth ,that.mWidth) ;
		const auto r2x = -inline_compr (get (self ,r1x) ,get (that ,r1x)) ;
		if (r2x != ZERO)
			return r2x ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = r1x - 1 - i ;
			const auto r3x = inline_compr (get (self ,ix) ,get (that ,ix)) ;
			if (r3x != ZERO)
				return r3x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		const auto r1x = self.mWidth ;
		for (auto &&i : iter (0 ,r1x)) {
			visitor.push (get (self ,i)) ;
		}
		visitor.leave () ;
	}

	IntegerLayout sadd (CREF<IntegerLayout> that) const override {
		IntegerLayout ret ;
		const auto r1x = inline_max (self.mWidth ,that.mWidth) ;
		IntegerHolder::hold (ret)->initialize (r1x) ;
		auto rax = VAL32 (0) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = VAL32 (get (self ,i)) + VAL32 (get (that ,i)) + rax ;
			ret.mInteger[i] = BYTE (r2x) ;
			rax = VAL32 (CHAR (r2x) >> 8) ;
		}
		ret.mWidth = r1x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	IntegerLayout ssub (CREF<IntegerLayout> that) const override {
		IntegerLayout ret ;
		const auto r1x = inline_max (self.mWidth ,that.mWidth) ;
		IntegerHolder::hold (ret)->initialize (r1x) ;
		auto rax = VAL32 (0) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = VAL32 (get (self ,i)) - VAL32 (get (that ,i)) - rax ;
			rax = VAL32 (r2x < 0) ;
			const auto r3x = r2x + 256 * rax ;
			ret.mInteger[i] = BYTE (r3x) ;
		}
		ret.mWidth = r1x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	IntegerLayout smul (CREF<IntegerLayout> that) const override {
		IntegerLayout ret ;
		const auto r1x = inline_max (self.mWidth + that.mWidth - 2 ,1) ;
		IntegerHolder::hold (ret)->initialize (r1x) ;
		inline_memset (Pointer::from (ret.mInteger.deref) ,r1x) ;
		for (auto &&i : iter (0 ,r1x)) {
			auto rax = VAL32 (0) ;
			for (auto &&j : iter (0 ,r1x)) {
				INDEX iy = i + j ;
				if (iy >= r1x)
					continue ;
				const auto r2x = VAL32 (get (self ,i)) * VAL32 (get (that ,j)) + rax ;
				const auto r3x = r2x + VAL32 (ret.mInteger[iy]) ;
				ret.mInteger[iy] = BYTE (r3x) ;
				rax = VAL32 (CHAR (r3x) >> 8) ;
			}
		}
		INDEX ix = r1x ;
		if ifdo (TRUE) {
			const auto r4x = invoke ([&] () {
				for (auto &&i : iter (0 ,ix))
					if (ret.mInteger[i] != BYTE (0X00))
						return i ;
				return ix ;
			}) ;
			if (r4x == ix)
				discard ;
			const auto r5x = get (self ,ix) ^ get (that ,ix) ;
			ret.mInteger[ix] = r5x ;
			ix++ ;
		}
		ret.mWidth = ix ;
		check_mask (ret) ;
		return move (ret) ;
	}

	IntegerLayout sdiv (CREF<IntegerLayout> that) const override {
		IntegerLayout ret ;
		auto rax = IntegerLayout () ;
		const auto r1x = IntegerHolder::hold (self)->compr (Integer::zero ()) ;
		const auto r2x = IntegerHolder::hold (that)->compr (Integer::zero ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x != 0)
				discard ;
			//@info: Integer division by zero
			assume (FALSE) ;
		}
		if ifdo (act) {
			if (r1x < 0)
				discard ;
			if (r2x < 0)
				discard ;
			sdiv_abs (ret ,rax ,self ,that) ;
		}
		if ifdo (act) {
			if (r1x >= 0)
				discard ;
			if (r2x < 0)
				discard ;
			const auto r3x = IntegerHolder::hold (self)->minus () ;
			sdiv_abs (ret ,rax ,r3x ,that) ;
			ret = IntegerHolder::hold (ret)->minus () ;
		}
		if ifdo (act) {
			if (r1x < 0)
				discard ;
			if (r2x >= 0)
				discard ;
			const auto r4x = IntegerHolder::hold (that)->minus () ;
			sdiv_abs (ret ,rax ,self ,r4x) ;
			ret = IntegerHolder::hold (ret)->minus () ;
		}
		if ifdo (act) {
			if (r1x >= 0)
				discard ;
			if (r2x >= 0)
				discard ;
			const auto r5x = IntegerHolder::hold (self)->minus () ;
			const auto r6x = IntegerHolder::hold (that)->minus () ;
			sdiv_abs (ret ,rax ,r5x ,r6x) ;
		}
		return move (ret) ;
	}

	IntegerLayout smod (CREF<IntegerLayout> that) const override {
		IntegerLayout ret ;
		auto rax = IntegerLayout () ;
		const auto r1x = IntegerHolder::hold (self)->compr (Integer::zero ()) ;
		const auto r2x = IntegerHolder::hold (that)->compr (Integer::zero ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x != 0)
				discard ;
			//@info: Integer division by zero
			assume (FALSE) ;
		}
		if ifdo (act) {
			if (r1x < 0)
				discard ;
			if (r2x < 0)
				discard ;
			sdiv_abs (rax ,ret ,self ,that) ;
		}
		if ifdo (act) {
			if (r1x >= 0)
				discard ;
			if (r2x < 0)
				discard ;
			const auto r3x = IntegerHolder::hold (self)->minus () ;
			sdiv_abs (rax ,ret ,r3x ,that) ;
			ret = IntegerHolder::hold (ret)->minus () ;
		}
		if ifdo (act) {
			if (r1x < 0)
				discard ;
			if (r2x >= 0)
				discard ;
			const auto r4x = IntegerHolder::hold (that)->minus () ;
			sdiv_abs (rax ,ret ,self ,r4x) ;
		}
		if ifdo (act) {
			if (r1x >= 0)
				discard ;
			if (r2x >= 0)
				discard ;
			const auto r5x = IntegerHolder::hold (self)->minus () ;
			const auto r6x = IntegerHolder::hold (that)->minus () ;
			sdiv_abs (rax ,ret ,r5x ,r6x) ;
			ret = IntegerHolder::hold (ret)->minus () ;
		}
		return move (ret) ;
	}

	void sdiv_abs (VREF<IntegerLayout> quotient ,VREF<IntegerLayout> remainder ,CREF<IntegerLayout> dividend ,CREF<IntegerLayout> divisor) const {
		const auto r1x = inline_max (dividend.mWidth ,divisor.mWidth) ;
		const auto r2x = r1x + 1 ;
		IntegerHolder::hold (quotient)->initialize (r2x) ;
		inline_memset (Pointer::from (quotient.mInteger.deref) ,r2x) ;
		IntegerHolder::hold (remainder)->initialize (r2x) ;
		inline_memset (Pointer::from (remainder.mInteger.deref) ,r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = r1x - 1 - i ;
			for (auto &&j : iter (0 ,8)) {
				INDEX jx = 8 - 1 - j ;
				const auto r3x = ByteProc::pow_bit (jx) ;
				for (auto &&k : iter (0 ,r1x - 1)) {
					INDEX kx = r1x - 1 - k ;
					remainder.mInteger[kx] = (remainder.mInteger[kx] << 1) | (remainder.mInteger[kx - 1] >> 7) ;
				}
				if ifdo (TRUE) {
					if (r1x == 0)
						discard ;
					INDEX kx = 0 ;
					remainder.mInteger[kx] = remainder.mInteger[kx] << 1 ;
				}
				remainder.mInteger[0] |= (get (dividend ,ix) >> jx) & BYTE (0X01) ;
				const auto r4x = IntegerHolder::hold (remainder)->compr (Integer::zero ()) ;
				assert (r4x >= 0) ;
				auto act = TRUE ;
				if ifdo (act) {
					const auto r5x = IntegerHolder::hold (remainder)->compr (divisor) ;
					if (r5x < ZERO)
						discard ;
					auto rax = VAL32 (0) ;
					for (auto &&k : iter (0 ,r1x)) {
						const auto r6x = VAL32 (get (remainder ,k)) - VAL32 (get (divisor ,k)) - rax ;
						rax = VAL32 (r6x < 0) ;
						const auto r7x = r6x + 256 * rax ;
						remainder.mInteger[k] = BYTE (r7x) ;
					}
					quotient.mInteger[ix] |= BYTE (r3x) ;
				}
				if ifdo (act) {
					quotient.mInteger[ix] &= ~BYTE (r3x) ;
				}
			}
		}
		check_mask (quotient) ;
		check_mask (remainder) ;
	}

	IntegerLayout sabs () const override {
		if (get (self ,self.mWidth - 1) == BYTE (0XFF))
			return minus () ;
		IntegerLayout ret ;
		IntegerHolder::hold (ret)->initialize (self) ;
		return move (ret) ;
	}

	IntegerLayout minus () const override {
		IntegerLayout ret ;
		IntegerHolder::hold (ret)->initialize (self.mWidth) ;
		for (auto &&i : iter (0 ,self.mWidth))
			ret.mInteger[i] = ~get (self ,i) ;
		IntegerHolder::hold (ret)->increase () ;
		return move (ret) ;
	}

	IntegerLayout lshift (CREF<LENGTH> scale) const override {
		assert (scale >= 0) ;
		assert (self.mWidth > 0) ;
		IntegerLayout ret ;
		const auto r1x = scale / 8 ;
		const auto r2x = scale % 8 ;
		const auto r3x = 8 - r2x ;
		const auto r4x = self.mWidth + r1x + 1 ;
		IntegerHolder::hold (ret)->initialize (r4x) ;
		for (auto &&i : iter (0 ,r4x - r1x - 1)) {
			INDEX ix = r4x - 1 - i ;
			const auto r5x = get (self ,ix - r1x) ;
			const auto r6x = get (self ,ix - r1x - 1) ;
			ret.mInteger[ix] = (r5x << r2x) | (r6x >> r3x) ;
		}
		const auto r7x = BYTE (0X00) ;
		if ifdo (TRUE) {
			if (r4x - r1x <= 0)
				discard ;
			INDEX ix = r1x ;
			const auto r8x = get (self ,ix - r1x) ;
			ret.mInteger[ix] = (r8x << r2x) | (r7x >> r3x) ;
		}
		const auto r9x = inline_max (r4x - r1x ,0) ;
		for (auto &&i : iter (r9x ,r4x)) {
			INDEX ix = r4x - 1 - i ;
			ret.mInteger[ix] = r7x ;
		}
		ret.mWidth = r4x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	IntegerLayout rshift (CREF<LENGTH> scale) const override {
		assert (scale >= 0) ;
		IntegerLayout ret ;
		const auto r1x = scale / 8 ;
		const auto r2x = scale % 8 ;
		const auto r3x = 8 - r2x ;
		const auto r4x = self.mWidth ;
		IntegerHolder::hold (ret)->initialize (r4x) ;
		for (auto &&i : iter (0 ,r4x - r1x - 1)) {
			const auto r5x = get (self ,i + r1x) ;
			const auto r6x = get (self ,i + r1x + 1) ;
			ret.mInteger[i] = (r5x >> r2x) | (r6x << r3x) ;
		}
		const auto r7x = get (self ,r4x) ;
		if ifdo (TRUE) {
			if (r4x - r1x <= 0)
				discard ;
			INDEX ix = r4x - r1x - 1 ;
			const auto r8x = get (self ,ix + r1x) ;
			ret.mInteger[ix] = (r8x >> r2x) | (r7x << r3x) ;
		}
		const auto r9x = inline_max (r4x - r1x ,0) ;
		for (auto &&i : iter (r9x ,r4x)) {
			ret.mInteger[i] = r7x ;
		}
		ret.mWidth = r4x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	void increase () override {
		const auto r1x = self.mWidth ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= r1x)
				break ;
			const auto r2x = VAL32 (get (self ,ix)) + 1 ;
			self.mInteger[ix] = BYTE (r2x) ;
			if (get (self ,ix) != BYTE (0X00))
				break ;
			ix++ ;
		}
		check_mask (self) ;
	}

	void decrease () override {
		const auto r1x = self.mWidth ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= r1x)
				break ;
			const auto r2x = VAL32 (get (self ,ix)) - 1 ;
			self.mInteger[ix] = BYTE (r2x) ;
			if (get (self ,ix) != BYTE (0XFF))
				break ;
			ix++ ;
		}
		check_mask (self) ;
	}

	static void check_mask (VREF<IntegerLayout> that) {
		if ifdo (TRUE) {
			INDEX ix = that.mWidth - 1 ;
			const auto r1x = that.mInteger[ix] ;
			if (r1x == BYTE (0X00))
				discard ;
			if (r1x == BYTE (0XFF))
				discard ;
			if ifdo (TRUE) {
				if (ix + 1 < that.mInteger.size ())
					discard ;
				const auto r2x = that.mInteger.size () + INTEGER_MIN_SIZE::expr ;
				that.mInteger.resize (r2x) ;
			}
			ix++ ;
			that.mInteger[ix] = ByteProc::binary (r1x & BYTE (0X80)) ;
			ix++ ;
			that.mWidth = ix ;
		}
		if ifdo (TRUE) {
			INDEX ix = that.mWidth - 1 ;
			const auto r3x = that.mInteger[ix] ;
			ix-- ;
			while (TRUE) {
				if (ix < 0)
					break ;
				if (that.mInteger[ix] != r3x)
					break ;
				ix-- ;
			}
			ix++ ;
			ix++ ;
			that.mWidth = ix ;
		}
	}
} ;

exports VFat<IntegerHolder> IntegerHolder::hold (VREF<IntegerLayout> that) {
	return VFat<IntegerHolder> (IntegerImplHolder () ,that) ;
}

exports CFat<IntegerHolder> IntegerHolder::hold (CREF<IntegerLayout> that) {
	return CFat<IntegerHolder> (IntegerImplHolder () ,that) ;
}

class JetImplHolder final implement Fat<JetHolder ,JetLayout> {
public:
	void initialize (CREF<LENGTH> size_ ,CREF<FLT64> item) override {
		assert (size_ > 0) ;
		self.mThis->mFX = item ;
		self.mThis->mEX = 0 ;
		self.mThis->mDX = RefBuffer<FLT64> (size_) ;
		inline_memset (Pointer::from (self.mThis->mDX.deref) ,self.mThis->mDX.size () * SIZE_OF<FLT64>::expr) ;
		self.mThis->mSlot = NONE ;
	}

	void initialize (CREF<LENGTH> size_ ,CREF<FLT64> item ,CREF<INDEX> slot) override {
		assert (inline_between (slot ,0 ,size_)) ;
		initialize (size_ ,item) ;
		self.mThis->mSlot = slot ;
		self.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			if (node.mSlot == NONE)
				return ;
			auto &&rax = keep[TYPE<Wrapper<FLT64>>::expr] (params) ;
			assume (node.mSlot < rax.rank ()) ;
			node.mFX = rax[node.mSlot] ;
			node.mEX = 0 ;
			inline_memset (Pointer::from (node.mDX.deref) ,node.mDX.size () * SIZE_OF<FLT64>::expr) ;
			node.mDX[node.mSlot] = 1 ;
			check_fx (node) ;
		}) ;
	}

	FLT64 fx () const override {
		return self.mThis->mFX ;
	}

	FLT64 ex () const override {
		return self.mThis->mEX ;
	}

	FLT64 dx (CREF<INDEX> slot) const override {
		return self.mThis->mDX[slot] ;
	}
	
	void once (CREF<WrapperLayout> params) const override {
		//@warn: it breaks Ref::exclusive to copy and write
		auto rax = self.mThis ;
		once (rax ,params) ;
	}

	void once (VREF<Ref<JetNode>> node ,CREF<WrapperLayout> params) const {
		if (!node.exist ())
			return ;
		once (node->mFake ,params) ;
		once (node->mThat ,params) ;
		node->mEval (node.deref ,params) ;
	}

	JetLayout sadd (CREF<JetLayout> that) const override {
		assert (self.mThis->mDX.size () == that.mThis->mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			auto act = TRUE ;
			if ifdo (act) {
				if (node.mFake->mEX != node.mThat->mEX)
					discard ;
				node.mFX = node.mFake->mFX + node.mThat->mFX ;
				node.mEX = node.mFake->mEX ;
				for (auto &&i : iter (0 ,node.mDX.size ()))
					node.mDX[i] = node.mFake->mDX[i] + node.mThat->mDX[i] ;
			}
			if ifdo (act) {
				if (node.mFake->mEX < node.mThat->mEX)
					discard ;
				copy_node (node ,node.mFake.deref ,+1) ;
			}
			if ifdo (act) {
				copy_node (node ,node.mThat.deref ,+1) ;
			}
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		ret.mThis->mThat = that.mThis ;
		return move (ret) ;
	}

	JetLayout ssub (CREF<JetLayout> that) const override {
		assert (self.mThis->mDX.size () == that.mThis->mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			auto act = TRUE ;
			if ifdo (act) {
				if (node.mFake->mEX != node.mThat->mEX)
					discard ;
				node.mFX = node.mFake->mFX - node.mThat->mFX ;
				node.mEX = node.mFake->mEX ;
				for (auto &&i : iter (0 ,node.mDX.size ()))
					node.mDX[i] = node.mFake->mDX[i] - node.mThat->mDX[i] ;
			}
			if ifdo (act) {
				if (node.mFake->mEX < node.mThat->mEX)
					discard ;
				copy_node (node ,node.mFake.deref ,+1) ;
			}
			if ifdo (act) {
				copy_node (node ,node.mThat.deref ,-1) ;
			}
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		ret.mThis->mThat = that.mThis ;
		return move (ret) ;
	}

	JetLayout smul (CREF<JetLayout> that) const override {
		assert (self.mThis->mDX.size () == that.mThis->mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			node.mFX = node.mFake->mFX * node.mThat->mFX ;
			node.mEX = round_ex (node.mFake->mEX + node.mThat->mEX) ;
			const auto r1x = node.mThat->mFX ;
			const auto r2x = node.mFake->mFX ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * node.mFake->mDX[i] + r2x * node.mThat->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		ret.mThis->mThat = that.mThis ;
		return move (ret) ;
	}

	JetLayout sdiv (CREF<JetLayout> that) const override {
		assert (self.mThis->mDX.size () == that.mThis->mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			const auto r1x = 1 / node.mThat->mFX ;
			node.mFX = node.mFake->mFX * r1x ;
			node.mEX = round_ex (node.mFake->mEX - node.mThat->mEX) ;
			const auto r2x = MathProc::square (r1x) ;
			const auto r3x = r2x * node.mThat->mFX ;
			const auto r4x = -r2x * node.mFake->mFX ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r3x * node.mFake->mDX[i] + r4x * node.mThat->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		ret.mThis->mThat = that.mThis ;
		return move (ret) ;
	}

	JetLayout inverse () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			const auto r1x = 1 / node.mFake->mFX ;
			node.mFX = r1x ;
			node.mEX = -node.mFake->mEX ;
			const auto r2x = -MathProc::square (r1x) ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout ssqrt () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			node.mFX = MathProc::sqrt (node.mFake->mFX) ;
			node.mEX = round_ex (node.mFake->mEX / 2) ;
			const auto r1x = 1 / (2 * node.mFX) ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout scbrt () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			node.mFX = MathProc::cbrt (node.mFake->mFX) ;
			node.mEX = round_ex (node.mFake->mEX / 3) ;
			const auto r1x = 1 / (3 * MathProc::square (node.mFX)) ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout spow (CREF<VAL32> that) const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			const auto r1x = node.mThat->mFX ;
			const auto r2x = VAL32 (MathProc::round (r1x - 1 ,FLT64 (1))) ;
			const auto r3x = MathProc::pow (node.mFake->mFX ,r2x) ;
			node.mFX = r3x * node.mFake->mFX ;
			node.mEX = round_ex (node.mFake->mEX * r1x) ;
			const auto r4x = r3x * r1x ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r4x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		ret.mThis->mThat = Ref<JetNode>::make () ;
		ret.mThis->mThat->mFX = that ;
		ret.mThis->mThat->mSlot = NONE ;
		return move (ret) ;
	}

	JetLayout shypot (CREF<JetLayout> that) const override {
		assert (self.mThis->mDX.size () == that.mThis->mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			auto act = TRUE ;
			if ifdo (act) {
				if (node.mFake->mEX != node.mThat->mEX)
					discard ;
				node.mFX = MathProc::hypot (node.mFake->mFX ,node.mThat->mFX) ;
				node.mEX = node.mFake->mEX ;
				const auto r1x = 1 / node.mFX ;
				const auto r2x = r1x * node.mFake->mFX ;
				const auto r3x = r1x * node.mThat->mFX ;
				for (auto &&i : iter (0 ,node.mDX.size ()))
					node.mDX[i] = r2x * node.mFake->mDX[i] + r3x * node.mThat->mDX[i] ;
			}
			if ifdo (act) {
				if (node.mFake->mEX < node.mThat->mEX)
					discard ;
				copy_node (node ,node.mFake.deref ,+1) ;
			}
			if ifdo (act) {
				copy_node (node ,node.mThat.deref ,+1) ;
			}
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		ret.mThis->mThat = that.mThis ;
		return move (ret) ;
	}

	JetLayout sabs () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			auto act = TRUE ;
			if ifdo (act) {
				if (node.mFake->mFX >= 0)
					discard ;
				copy_node (node ,node.mFake.deref ,+1) ;
			}
			if ifdo (act) {
				copy_node (node ,node.mFake.deref ,-1) ;
			}
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout minus () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			copy_node (node ,node.mFake.deref ,-1) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout ssin () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			assume (node.mFake->mEX == 0) ;
			node.mFX = MathProc::sin (node.mFake->mFX) ;
			node.mEX = node.mFake->mEX ;
			const auto r1x = MathProc::cos (node.mFake->mFX) ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout scos () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			assume (node.mFake->mEX == 0) ;
			node.mFX = MathProc::cos (node.mFake->mFX) ;
			node.mEX = node.mFake->mEX ;
			const auto r1x = -MathProc::sin (node.mFake->mFX) ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout stan () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			assume (node.mFake->mEX == 0) ;
			node.mFX = MathProc::tan (node.mFake->mFX) ;
			node.mEX = node.mFake->mEX ;
			const auto r1x = 1 + MathProc::square (node.mFX) ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout sasin () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			assume (node.mFake->mEX == 0) ;
			node.mFX = MathProc::asin (node.mFake->mFX) ;
			node.mEX = node.mFake->mEX ;
			const auto r1x = MathProc::sqrt (1 - MathProc::square (node.mFake->mFX)) ;
			const auto r2x = 1 / r1x ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout sacos () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			assume (node.mFake->mEX == 0) ;
			node.mFX = MathProc::acos (node.mFake->mFX) ;
			node.mEX = node.mFake->mEX ;
			const auto r1x = -MathProc::sqrt (1 - MathProc::square (node.mFake->mFX)) ;
			const auto r2x = 1 / r1x ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout satan (CREF<JetLayout> that) const override {
		assert (self.mThis->mDX.size () == that.mThis->mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			assume (node.mFake->mEX == 0) ;
			assume (node.mThat->mEX == 0) ;
			node.mFX = MathProc::atan (node.mFake->mFX ,node.mThat->mFX) ;
			node.mEX = node.mFake->mEX ;
			const auto r1x = MathProc::square (node.mFake->mFX) + MathProc::square (node.mThat->mFX) ;
			const auto r2x = 1 / r1x ;
			const auto r3x = -r2x * node.mThat->mFX ;
			const auto r4x = r2x * node.mFake->mFX ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r3x * node.mFake->mDX[i] + r4x * node.mThat->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		ret.mThis->mThat = that.mThis ;
		return move (ret) ;
	}

	JetLayout sexp () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			assume (node.mFake->mEX == 0) ;
			const auto r1x = MathProc::exp (node.mFake->mFX) ;
			node.mFX = r1x ;
			node.mEX = node.mFake->mEX ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	JetLayout slog () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (self.mThis->mDX.size () ,0) ;
		ret.mThis->mEval = JetEvalFunction ([] (VREF<JetNode> node ,CREF<WrapperLayout> params) {
			node.mFX = MathProc::log (node.mFake->mFX) ;
			const auto r1x = MathProc::sign (node.mFake->mEX) ;
			const auto r2x = MathProc::step (MathProc::abs (node.mFake->mEX)) ;
			node.mEX = round_ex (r1x * r2x) ;
			const auto r3x = 1 / node.mFake->mFX ;
			for (auto &&i : iter (0 ,node.mDX.size ()))
				node.mDX[i] = r3x * node.mFake->mDX[i] ;
			check_fx (node) ;
		}) ;
		ret.mThis->mFake = self.mThis ;
		return move (ret) ;
	}

	static void copy_node (VREF<JetNode> dst ,CREF<JetNode> src ,CREF<FLT64> si) {
		dst.mFX = si * src.mFX ;
		dst.mEX = src.mEX ;
		for (auto &&i : iter (0 ,dst.mDX.size ()))
			dst.mDX[i] = si * src.mDX[i] ;
	}

	static FLT64 round_ex (CREF<FLT64> ex) {
		const auto r1x = MathProc::round (ex ,FLT64 (0.01)) ;
		const auto r2x = MathProc::inverse (ex - r1x) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	static void check_fx (VREF<JetNode> node) {
		auto act = TRUE ;
		if ifdo (act) {
			if (!MathProc::is_low (node.mFX))
				discard ;
			node.mFX = 1 ;
			node.mEX-- ;
			inline_memset (Pointer::from (node.mDX.deref) ,node.mDX.size () * SIZE_OF<FLT64>::expr) ;
		}
		if ifdo (act) {
			const auto r1x = 1 / node.mFX ;
			if (!MathProc::is_low (r1x))
				discard ;
			node.mFX = 1 ;
			node.mEX++ ;
			inline_memset (Pointer::from (node.mDX.deref) ,node.mDX.size () * SIZE_OF<FLT64>::expr) ;
		}
	}
} ;

exports VFat<JetHolder> JetHolder::hold (VREF<JetLayout> that) {
	return VFat<JetHolder> (JetImplHolder () ,that) ;
}

exports CFat<JetHolder> JetHolder::hold (CREF<JetLayout> that) {
	return CFat<JetHolder> (JetImplHolder () ,that) ;
}

struct HashProcLayout {} ;

class HashProcImplHolder final implement Fat<HashProcHolder ,HashProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_) const override {
		return fnvhash32 (src ,size_ ,CHAR (2166136261UL)) ;
	}

	CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<CHAR> val) const override {
		CHAR ret = val ;
		auto &&rax = keep[TYPE<ARR<BYTE>>::expr] (src) ;
		for (auto &&i : iter (0 ,size_)) {
			ret ^= CHAR (rax[i]) ;
			ret = CHAR (VAL32 (ret) * VAL32 (16777619UL)) ;
		}
		return move (ret) ;
	}

	QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_) const override {
		return fnvhash64 (src ,size_ ,QUAD (14695981039346656037ULL)) ;
	}

	QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<QUAD> val) const override {
		QUAD ret = val ;
		auto &&rax = keep[TYPE<ARR<BYTE>>::expr] (src) ;
		for (auto &&i : iter (0 ,size_)) {
			ret ^= QUAD (rax[i]) ;
			ret = QUAD (VAL64 (ret) * VAL64 (1099511628211ULL)) ;
		}
		return move (ret) ;
	}

	BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_) const override {
		return crchash8 (src ,size_ ,BYTE (0X00)) ;
	}

	BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<BYTE> val) const override {
		static const ARR<csc_uint8_t ,ENUM<256>> mCache {
			0X00 ,0X5E ,0XBC ,0XE2 ,0X61 ,0X3F ,0XDD ,0X83 ,0XC2 ,0X9C ,0X7E ,0X20 ,0XA3 ,0XFD ,0X1F ,0X41 ,0X9D ,0XC3 ,0X21 ,0X7F ,0XFC ,0XA2 ,0X40 ,0X1E ,0X5F ,0X01 ,0XE3 ,0XBD ,0X3E ,0X60 ,0X82 ,0XDC ,0X23 ,0X7D ,0X9F ,0XC1 ,0X42 ,0X1C ,0XFE ,0XA0 ,0XE1 ,0XBF ,0X5D ,0X03 ,0X80 ,0XDE ,0X3C ,0X62 ,0XBE ,0XE0 ,0X02 ,0X5C ,0XDF ,0X81 ,0X63 ,0X3D ,0X7C ,0X22 ,0XC0 ,0X9E ,0X1D ,0X43 ,0XA1 ,0XFF ,0X46 ,0X18 ,0XFA ,0XA4 ,0X27 ,0X79 ,0X9B ,0XC5 ,0X84 ,0XDA ,0X38 ,0X66 ,0XE5 ,0XBB ,0X59 ,0X07 ,0XDB ,0X85 ,0X67 ,0X39 ,0XBA ,0XE4 ,0X06 ,0X58 ,0X19 ,0X47 ,0XA5 ,0XFB ,0X78 ,0X26 ,0XC4 ,0X9A ,0X65 ,0X3B ,0XD9 ,0X87 ,0X04 ,0X5A ,0XB8 ,0XE6 ,0XA7 ,0XF9 ,0X1B ,0X45 ,0XC6 ,0X98 ,0X7A ,0X24 ,0XF8 ,0XA6 ,0X44 ,0X1A ,0X99 ,0XC7 ,0X25 ,0X7B ,0X3A ,0X64 ,0X86 ,0XD8 ,0X5B ,0X05 ,0XE7 ,0XB9 ,0X8C ,0XD2 ,0X30 ,0X6E ,0XED ,0XB3 ,0X51 ,0X0F ,0X4E ,0X10 ,0XF2 ,0XAC ,0X2F ,0X71 ,0X93 ,0XCD ,0X11 ,0X4F ,0XAD ,0XF3 ,0X70 ,0X2E ,0XCC ,0X92 ,0XD3 ,0X8D ,0X6F ,0X31 ,0XB2 ,0XEC ,0X0E ,0X50 ,0XAF ,0XF1 ,0X13 ,0X4D ,0XCE ,0X90 ,0X72 ,0X2C ,0X6D ,0X33 ,0XD1 ,0X8F ,0X0C ,0X52 ,0XB0 ,0XEE ,0X32 ,0X6C ,0X8E ,0XD0 ,0X53 ,0X0D ,0XEF ,0XB1 ,0XF0 ,0XAE ,0X4C ,0X12 ,0X91 ,0XCF ,0X2D ,0X73 ,0XCA ,0X94 ,0X76 ,0X28 ,0XAB ,0XF5 ,0X17 ,0X49 ,0X08 ,0X56 ,0XB4 ,0XEA ,0X69 ,0X37 ,0XD5 ,0X8B ,0X57 ,0X09 ,0XEB ,0XB5 ,0X36 ,0X68 ,0X8A ,0XD4 ,0X95 ,0XCB ,0X29 ,0X77 ,0XF4 ,0XAA ,0X48 ,0X16 ,0XE9 ,0XB7 ,0X55 ,0X0B ,0X88 ,0XD6 ,0X34 ,0X6A ,0X2B ,0X75 ,0X97 ,0XC9 ,0X4A ,0X14 ,0XF6 ,0XA8 ,0X74 ,0X2A ,0XC8 ,0X96 ,0X15 ,0X4B ,0XA9 ,0XF7 ,0XB6 ,0XE8 ,0X0A ,0X54 ,0XD7 ,0X89 ,0X6B ,0X35} ;
		BYTE ret = val ;
		auto &&rax = keep[TYPE<ARR<BYTE>>::expr] (src) ;
		for (auto &&i : iter (0 ,size_)) {
			const auto r1x = INDEX (ret ^ BYTE (rax[i])) ;
			ret = BYTE (mCache[r1x]) ;
		}
		return move (ret) ;
	}

	WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_) const override {
		return crchash16 (src ,size_ ,WORD (0X00)) ;
	}

	WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<WORD> val) const override {
		static const ARR<csc_uint16_t ,ENUM<256>> mCache {
			0x0000 ,0x1021 ,0x2042 ,0x3063 ,0x4084 ,0x50A5 ,0x60C6 ,0x70E7 ,0x8108 ,0x9129 ,0xA14A ,0xB16B ,0xC18C ,0xD1AD ,0xE1CE ,0xF1EF ,0x1231 ,0x0210 ,0x3273 ,0x2252 ,0x52B5 ,0x4294 ,0x72F7 ,0x62D6 ,0x9339 ,0x8318 ,0xB37B ,0xA35A ,0xD3BD ,0xC39C ,0xF3FF ,0xE3DE ,0x2462 ,0x3443 ,0x0420 ,0x1401 ,0x64E6 ,0x74C7 ,0x44A4 ,0x5485 ,0xA56A ,0xB54B ,0x8528 ,0x9509 ,0xE5EE ,0xF5CF ,0xC5AC ,0xD58D ,0x3653 ,0x2672 ,0x1611 ,0x0630 ,0x76D7 ,0x66F6 ,0x5695 ,0x46B4 ,0xB75B ,0xA77A ,0x9719 ,0x8738 ,0xF7DF ,0xE7FE ,0xD79D ,0xC7BC ,0x48C4 ,0x58E5 ,0x6886 ,0x78A7 ,0x0840 ,0x1861 ,0x2802 ,0x3823 ,0xC9CC ,0xD9ED ,0xE98E ,0xF9AF ,0x8948 ,0x9969 ,0xA90A ,0xB92B ,0x5AF5 ,0x4AD4 ,0x7AB7 ,0x6A96 ,0x1A71 ,0x0A50 ,0x3A33 ,0x2A12 ,0xDBFD ,0xCBDC ,0xFBBF ,0xEB9E ,0x9B79 ,0x8B58 ,0xBB3B ,0xAB1A ,0x6CA6 ,0x7C87 ,0x4CE4 ,0x5CC5 ,0x2C22 ,0x3C03 ,0x0C60 ,0x1C41 ,0xEDAE ,0xFD8F ,0xCDEC ,0xDDCD ,0xAD2A ,0xBD0B ,0x8D68 ,0x9D49 ,0x7E97 ,0x6EB6 ,0x5ED5 ,0x4EF4 ,0x3E13 ,0x2E32 ,0x1E51 ,0x0E70 ,0xFF9F ,0xEFBE ,0xDFDD ,0xCFFC ,0xBF1B ,0xAF3A ,0x9F59 ,0x8F78 ,0x9188 ,0x81A9 ,0xB1CA ,0xA1EB ,0xD10C ,0xC12D ,0xF14E ,0xE16F ,0x1080 ,0x00A1 ,0x30C2 ,0x20E3 ,0x5004 ,0x4025 ,0x7046 ,0x6067 ,0x83B9 ,0x9398 ,0xA3FB ,0xB3DA ,0xC33D ,0xD31C ,0xE37F ,0xF35E ,0x02B1 ,0x1290 ,0x22F3 ,0x32D2 ,0x4235 ,0x5214 ,0x6277 ,0x7256 ,0xB5EA ,0xA5CB ,0x95A8 ,0x8589 ,0xF56E ,0xE54F ,0xD52C ,0xC50D ,0x34E2 ,0x24C3 ,0x14A0 ,0x0481 ,0x7466 ,0x6447 ,0x5424 ,0x4405 ,0xA7DB ,0xB7FA ,0x8799 ,0x97B8 ,0xE75F ,0xF77E ,0xC71D ,0xD73C ,0x26D3 ,0x36F2 ,0x0691 ,0x16B0 ,0x6657 ,0x7676 ,0x4615 ,0x5634 ,0xD94C ,0xC96D ,0xF90E ,0xE92F ,0x99C8 ,0x89E9 ,0xB98A ,0xA9AB ,0x5844 ,0x4865 ,0x7806 ,0x6827 ,0x18C0 ,0x08E1 ,0x3882 ,0x28A3 ,0xCB7D ,0xDB5C ,0xEB3F ,0xFB1E ,0x8BF9 ,0x9BD8 ,0xABBB ,0xBB9A ,0x4A75 ,0x5A54 ,0x6A37 ,0x7A16 ,0x0AF1 ,0x1AD0 ,0x2AB3 ,0x3A92 ,0xFD2E ,0xED0F ,0xDD6C ,0xCD4D ,0xBDAA ,0xAD8B ,0x9DE8 ,0x8DC9 ,0x7C26 ,0x6C07 ,0x5C64 ,0x4C45 ,0x3CA2 ,0x2C83 ,0x1CE0 ,0x0CC1 ,0xEF1F ,0xFF3E ,0xCF5D ,0xDF7C ,0xAF9B ,0xBFBA ,0x8FD9 ,0x9FF8 ,0x6E17 ,0x7E36 ,0x4E55 ,0x5E74 ,0x2E93 ,0x3EB2 ,0x0ED1 ,0x1EF0} ;
		WORD ret = val ;
		auto &&rax = keep[TYPE<ARR<BYTE>>::expr] (src) ;
		for (auto &&i : iter (0 ,size_)) {
			const auto r1x = (ret >> 8) ^ WORD (rax[i]) ;
			const auto r2x = INDEX (r1x & WORD (0XFF)) ;
			ret = WORD (mCache[r2x]) ^ (ret << 8) ;
		}
		return move (ret) ;
	}
} ;

exports CREF<OfThis<UniqueRef<HashProcLayout>>> HashProcHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<HashProcLayout>> ret ;
		ret.mThis = UniqueRef<HashProcLayout>::make () ;
		HashProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<HashProcHolder> HashProcHolder::hold (VREF<HashProcLayout> that) {
	return VFat<HashProcHolder> (HashProcImplHolder () ,that) ;
}

exports CFat<HashProcHolder> HashProcHolder::hold (CREF<HashProcLayout> that) {
	return CFat<HashProcHolder> (HashProcImplHolder () ,that) ;
}
} ;