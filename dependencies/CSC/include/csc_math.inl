#pragma once

#ifndef __CSC_MATH__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_math.hpp"

#include "csc_end.h"
#include <cmath>
#include "csc_begin.h"

namespace CSC {
class MathProcImplHolder implement Fat<MathProcHolder ,MathProcLayout> {
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

	VAL32 abs (CREF<VAL32> a) const override {
		if (a == VAL32_LOW)
			return 0 ;
		if (a < 0)
			return -a ;
		return a ;
	}

	VAL64 abs (CREF<VAL64> a) const override {
		if (a == VAL64_LOW)
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

	VAL32 clamp (CREF<VAL32> a ,CREF<VAL32> lb ,CREF<VAL32> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	VAL64 clamp (CREF<VAL64> a ,CREF<VAL64> lb ,CREF<VAL64> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	FLT32 clamp (CREF<FLT32> a ,CREF<FLT32> lb ,CREF<FLT32> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	FLT64 clamp (CREF<FLT64> a ,CREF<FLT64> lb ,CREF<FLT64> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	VAL32 lerp (CREF<FLT64> a ,CREF<VAL32> lb ,CREF<VAL32> rb) const override {
		const auto r1x = MathProc::abs (a) ;
		const auto r2x = r1x - MathProc::floor (r1x / 2 ,FLT64 (1)) * 2 ;
		const auto r3x = FLT64 (rb - lb) * r2x ;
		const auto r4x = VAL32 (round (r3x ,FLT64 (1))) ;
		return lb + r4x ;
	}

	VAL64 lerp (CREF<FLT64> a ,CREF<VAL64> lb ,CREF<VAL64> rb) const override {
		const auto r1x = MathProc::abs (a) ;
		const auto r2x = r1x - MathProc::floor (r1x / 2 ,FLT64 (1)) * 2 ;
		const auto r3x = FLT64 (rb - lb) * r2x ;
		const auto r4x = VAL64 (round (r3x ,FLT64 (1))) ;
		return lb + r4x ;
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

	FLT32 arccos (CREF<FLT32> a) const override {
		return std::acos (a) ;
	}

	FLT64 arccos (CREF<FLT64> a) const override {
		return std::acos (a) ;
	}

	FLT32 arcsin (CREF<FLT32> a) const override {
		return std::asin (a) ;
	}

	FLT64 arcsin (CREF<FLT64> a) const override {
		return std::asin (a) ;
	}

	FLT32 arctan (CREF<FLT32> y ,CREF<FLT32> x) const override {
		return std::atan2 (y ,x) ;
	}

	FLT64 arctan (CREF<FLT64> y ,CREF<FLT64> x) const override {
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
		auto &&rax = keep[TYPE<Wrapper<BOOL>>::expr] (Pointer::from (b)) ;
		for (auto &&i : iter (0 ,b.mRank)) {
			if (!rax[i])
				return FALSE ;
		}
		return TRUE ;
	}

	BOOL any_of (CREF<BOOL> a ,CREF<WrapperLayout> b) const override {
		if (a)
			return TRUE ;
		auto &&rax = keep[TYPE<Wrapper<BOOL>>::expr] (Pointer::from (b)) ;
		for (auto &&i : iter (0 ,b.mRank)) {
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
	ARG1 max_of_impl (CREF<ARG1> a ,CREF<WrapperLayout> b) const {
		ARG1 ret = a ;
		auto &&rax = keep[TYPE<Wrapper<ARG1>>::expr] (Pointer::from (b)) ;
		for (auto &&i : iter (0 ,b.mRank)) {
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
	ARG1 min_of_impl (CREF<ARG1> a ,CREF<WrapperLayout> b) const {
		ARG1 ret = a ;
		auto &&rax = keep[TYPE<Wrapper<ARG1>>::expr] (Pointer::from (b)) ;
		for (auto &&i : iter (0 ,b.mRank)) {
			if (ret <= rax[i])
				continue ;
			ret = rax[i] ;
		}
		return move (ret) ;
	}
} ;

exports VFat<MathProcHolder> MathProcHolder::hold (VREF<MathProcLayout> that) {
	return VFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}

exports CFat<MathProcHolder> MathProcHolder::hold (CREF<MathProcLayout> that) {
	return CFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}

class NormalErrorImplHolder implement Fat<NormalErrorHolder ,NormalErrorLayout> {
public:
	void update (CREF<FLT64> error) override {
		const auto r1x = FLT64 (fake.mCount) ;
		const auto r2x = MathProc::inverse (r1x + 1) ;
		const auto r3x = error - fake.mAvgError ;
		fake.mMaxError = MathProc::max_of (fake.mMaxError ,error) ;
		fake.mAvgError = fake.mAvgError + r3x * r2x ;
		const auto r4x = r1x * r2x * MathProc::square (fake.mStdError) + r1x * MathProc::square (r3x * r2x) ;
		fake.mStdError = MathProc::sqrt (r4x) ;
		fake.mCount = LENGTH (r1x + 1) ;
	}
} ;

exports VFat<NormalErrorHolder> NormalErrorHolder::hold (VREF<NormalErrorLayout> that) {
	return VFat<NormalErrorHolder> (NormalErrorImplHolder () ,that) ;
}

exports CFat<NormalErrorHolder> NormalErrorHolder::hold (CREF<NormalErrorLayout> that) {
	return CFat<NormalErrorHolder> (NormalErrorImplHolder () ,that) ;
}

class FloatProcImplHolder implement Fat<FloatProcHolder ,FloatProcLayout> {
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
				if (!ByteProc::bit_any (QUAD (rax.mMantissa) ,QUAD (0XFFE0000000000000)))
					break ;
				rax.mMantissa = VAL64 (QUAD (rax.mMantissa) >> 1) ;
				rax.mDownflow = 0 ;
				rax.mExponent++ ;
			}
			while (TRUE) {
				if (ByteProc::bit_any (QUAD (rax.mMantissa) ,QUAD (0XFFF0000000000000)))
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
		ret.mSign = ByteProc::bit_any (r1x ,QUAD (0X8000000000000000)) ;
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
				if (ByteProc::bit_any (QUAD (ret.mMantissa) ,QUAD (0X0000000000000001)))
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
		const auto r2x = QUAD (ByteProc::bit_pow (r1x) - 1) ;
		const auto r3x = ByteProc::bit_pow (r1x - 1) ;
		const auto r4x = VAL64 (QUAD (obj1.mMantissa) >> r1x) ;
		const auto r5x = VAL64 (QUAD (obj1.mMantissa) & r2x) ;
		const auto r6x = VAL64 (QUAD (obj2.mMantissa) >> r1x) ;
		const auto r7x = VAL64 (QUAD (obj2.mMantissa) & r2x) ;
		const auto r8x = r4x * r6x ;
		const auto r9x = r5x * r6x ;
		const auto r10x = r4x * r7x ;
		const auto r11x = r5x * r7x ;
		//@warn: lose a part of precision
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
		//@warn: lose a part of precision
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

exports VFat<FloatProcHolder> FloatProcHolder::hold (VREF<FloatProcLayout> that) {
	return VFat<FloatProcHolder> (FloatProcImplHolder () ,that) ;
}

exports CFat<FloatProcHolder> FloatProcHolder::hold (CREF<FloatProcLayout> that) {
	return CFat<FloatProcHolder> (FloatProcImplHolder () ,that) ;
}

template class External<FEXP2CacheHolder ,FEXP2CacheLayout> ;

exports VFat<FEXP2CacheHolder> FEXP2CacheHolder::hold (VREF<FEXP2CacheLayout> that) {
	return VFat<FEXP2CacheHolder> (External<FEXP2CacheHolder ,FEXP2CacheLayout>::instance () ,that) ;
}

exports CFat<FEXP2CacheHolder> FEXP2CacheHolder::hold (CREF<FEXP2CacheLayout> that) {
	return CFat<FEXP2CacheHolder> (External<FEXP2CacheHolder ,FEXP2CacheLayout>::instance () ,that) ;
}

template class External<FEXP10CacheHolder ,FEXP10CacheHolder> ;

exports VFat<FEXP10CacheHolder> FEXP10CacheHolder::hold (VREF<FEXP10CacheLayout> that) {
	return VFat<FEXP10CacheHolder> (External<FEXP10CacheHolder ,FEXP10CacheLayout>::instance () ,that) ;
}

exports CFat<FEXP10CacheHolder> FEXP10CacheHolder::hold (CREF<FEXP10CacheLayout> that) {
	return CFat<FEXP10CacheHolder> (External<FEXP10CacheHolder ,FEXP10CacheLayout>::instance () ,that) ;
}

class ByteProcImplHolder implement Fat<ByteProcHolder ,ByteProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	BYTE bit_low (CREF<WORD> a) const override {
		return BYTE (a) ;
	}

	WORD bit_low (CREF<CHAR> a) const override {
		return WORD (a) ;
	}

	CHAR bit_low (CREF<QUAD> a) const override {
		return CHAR (a) ;
	}

	BYTE bit_high (CREF<WORD> a) const override {
		return BYTE (a >> 8) ;
	}

	WORD bit_high (CREF<CHAR> a) const override {
		return WORD (a >> 16) ;
	}

	CHAR bit_high (CREF<QUAD> a) const override {
		return CHAR (a >> 32) ;
	}

	WORD bit_merge (CREF<BYTE> high ,CREF<BYTE> low) const override {
		return (WORD (high) << 8) | WORD (low) ;
	}

	CHAR bit_merge (CREF<WORD> high ,CREF<WORD> low) const override {
		return (CHAR (high) << 16) | CHAR (low) ;
	}

	QUAD bit_merge (CREF<CHAR> high ,CREF<CHAR> low) const override {
		return (QUAD (high) << 32) | QUAD (low) ;
	}

	BOOL bit_any (CREF<BYTE> curr ,CREF<BYTE> mask) const override {
		return (curr & mask) != BYTE (0X00) ;
	}

	BOOL bit_any (CREF<WORD> curr ,CREF<WORD> mask) const override {
		return (curr & mask) != WORD (0X00) ;
	}

	BOOL bit_any (CREF<CHAR> curr ,CREF<CHAR> mask) const override {
		return (curr & mask) != CHAR (0X00) ;
	}

	BOOL bit_any (CREF<QUAD> curr ,CREF<QUAD> mask) const override {
		return (curr & mask) != QUAD (0X00) ;
	}

	BOOL bit_all (CREF<BYTE> curr ,CREF<BYTE> mask) const override {
		return (curr & mask) == mask ;
	}

	BOOL bit_all (CREF<WORD> curr ,CREF<WORD> mask) const override {
		return (curr & mask) == mask ;
	}

	BOOL bit_all (CREF<CHAR> curr ,CREF<CHAR> mask) const override {
		return (curr & mask) == mask ;
	}

	BOOL bit_all (CREF<QUAD> curr ,CREF<QUAD> mask) const override {
		return (curr & mask) == mask ;
	}

	BYTE bit_reverse (CREF<BYTE> a) const override {
		return a ;
	}

	WORD bit_reverse (CREF<WORD> a) const override {
		auto rax = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<WORD>>>::expr] (a) ;
		swap (rax[0] ,rax[1]) ;
		return bitwise[TYPE<WORD>::expr] (rax) ;
	}

	CHAR bit_reverse (CREF<CHAR> a) const override {
		auto rax = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<CHAR>>>::expr] (a) ;
		swap (rax[0] ,rax[3]) ;
		swap (rax[1] ,rax[2]) ;
		return bitwise[TYPE<CHAR>::expr] (rax) ;
	}

	QUAD bit_reverse (CREF<QUAD> a) const override {
		auto rax = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<QUAD>>>::expr] (a) ;
		swap (rax[0] ,rax[7]) ;
		swap (rax[1] ,rax[6]) ;
		swap (rax[2] ,rax[5]) ;
		swap (rax[3] ,rax[4]) ;
		return bitwise[TYPE<QUAD>::expr] (rax) ;
	}

	INDEX bit_pow (CREF<LENGTH> nth) const override {
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

exports VFat<ByteProcHolder> ByteProcHolder::hold (VREF<ByteProcLayout> that) {
	return VFat<ByteProcHolder> (ByteProcImplHolder () ,that) ;
}

exports CFat<ByteProcHolder> ByteProcHolder::hold (CREF<ByteProcLayout> that) {
	return CFat<ByteProcHolder> (ByteProcImplHolder () ,that) ;
}

class IntegerImplHolder implement Fat<IntegerHolder ,IntegerLayout> {
public:
	void initialize (CREF<LENGTH> size_ ,CREF<VAL64> item) override {
		fake.mInteger = RefBuffer<BYTE> (size_) ;
		store (item) ;
	}

	LENGTH size () const override {
		return fake.mInteger.size () ;
	}

	LENGTH precision () const override {
		const auto r1x = find_highest () ;
		if (r1x == NONE)
			return 0 ;
		const auto r2x = ByteProc::lowcount (fake.mInteger[r1x]) + 1 ;
		const auto r3x = r1x * 8 + r2x ;
		return r3x ;
	}

	INDEX find_highest () const {
		for (auto &&i : iter (0 ,fake.mInteger.size ())) {
			INDEX ix = fake.mInteger.size () - 1 - i ;
			if (fake.mInteger[ix] != BYTE (0X00))
				return ix ;
		}
		return NONE ;
	}

	VAL64 fetch () const override {
		VAL64 ret = 0 ;
		const auto r1x = MathProc::min_of (fake.mInteger.size () ,SIZE_OF<VAL64>::expr) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = QUAD (fake.mInteger[i]) << (i * 8) ;
			ret = VAL64 (QUAD (ret) | r2x) ;
		}
		return move (ret) ;
	}

	void store (CREF<VAL64> item) override {
		const auto r1x = MathProc::min_of (fake.mInteger.size () ,SIZE_OF<VAL64>::expr) ;
		const auto r2x = QUAD (item) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = BYTE (r2x >> (i * 8)) ;
			fake.mInteger[i] = r3x ;
		}
		const auto r4x = invoke ([&] () {
			if (item >= 0)
				return BYTE (0X00) ;
			return BYTE (0XFF) ;
		}) ;
		for (auto &&i : iter (r1x ,fake.mInteger.size ()))
			fake.mInteger[i] = r4x ;
	}

	BOOL equal (CREF<IntegerLayout> that) const override {
		const auto r1x = fake.mInteger.size () ;
		const auto r2x = that.mInteger.size () ;
		if (r1x != r2x)
			return FALSE ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = inline_equal (fake.mInteger[i] ,that.mInteger[i]) ;
			if (!r3x)
				return r3x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<IntegerLayout> that) const override {
		const auto r1x = fake.mInteger.size () ;
		const auto r2x = that.mInteger.size () ;
		const auto r3x = inline_compr (r1x ,r2x) ;
		if (r3x != ZERO)
			return r3x ;
		if ifdo (TRUE) {
			if (r1x == 0)
				discard ;
			const auto r4x = fake.mInteger[0] & BYTE (0X80) ;
			const auto r5x = that.mInteger[0] & BYTE (0X80) ;
			const auto r6x = inline_compr (r4x ,r5x) ;
			if (r6x == ZERO)
				discard ;
			return -r6x ;
		}
		for (auto &&i : iter (0 ,r1x)) {
			const auto r7x = inline_compr (fake.mInteger[i] ,that.mInteger[i]) ;
			if (r7x != ZERO)
				return r7x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorFriend> visitor) const override {
		visitor.enter () ;
		const auto r1x = fake.mInteger.size () ;
		for (auto &&i : iter (0 ,r1x)) {
			visitor.push (fake.mInteger[i]) ;
		}
		visitor.leave () ;
	}

	IntegerLayout sadd (CREF<IntegerLayout> that) const override {
		assert (fake.mInteger.size () == that.mInteger.size ()) ;
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ())) {
			const auto r1x = VAL64 (fake.mInteger[i]) + VAL64 (that.mInteger[i]) + rax ;
			const auto r2x = QUAD (r1x) ;
			ret.mInteger[i] = BYTE (r2x) ;
			rax = VAL64 (r2x >> 8) ;
		}
		return move (ret) ;
	}

	IntegerLayout ssub (CREF<IntegerLayout> that) const override {
		assert (fake.mInteger.size () == that.mInteger.size ()) ;
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ())) {
			const auto r1x = VAL64 (fake.mInteger[i]) - VAL64 (that.mInteger[i]) - rax ;
			const auto r2x = VAL64 (r1x < 0) ;
			const auto r3x = r1x + 256 * r2x ;
			const auto r4x = QUAD (r3x) ;
			ret.mInteger[i] = BYTE (r4x) ;
			rax = r2x ;
		}
		return move (ret) ;
	}

	IntegerLayout smul (CREF<IntegerLayout> that) const override {
		assert (fake.mInteger.size () == that.mInteger.size ()) ;
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ())) {
			for (auto &&j : iter (0 ,i + 1)) {
				const auto r1x = VAL64 (fake.mInteger[j]) * VAL64 (that.mInteger[i - j]) ;
				rax += r1x ;
			}
			const auto r2x = QUAD (rax) ;
			ret.mInteger[i] = BYTE (r2x) ;
			rax = VAL64 (r2x >> 8) ;
		}
		return move (ret) ;
	}

	IntegerLayout smul (CREF<VAL64> scale) const override {
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		const auto r1x = find_highest () + 1 ;
		for (auto &&i : iter (r1x ,fake.mInteger.size ()))
			ret.mInteger[i] = BYTE (0X00) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = VAL64 (fake.mInteger[i]) * scale + rax ;
			const auto r3x = QUAD (r2x) ;
			ret.mInteger[i] = BYTE (r3x) ;
			rax = VAL64 (r3x >> 8) ;
		}
		INDEX ix = r1x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			const auto r4x = QUAD (rax) ;
			ret.mInteger[ix] = BYTE (r4x) ;
			rax = VAL64 (r4x >> 8) ;
			ix++ ;
		}
		return move (ret) ;
	}

	IntegerLayout sdiv (CREF<VAL64> scale) const override {
		assert (scale != ZERO) ;
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		const auto r1x = find_highest () + 1 ;
		for (auto &&i : iter (r1x ,fake.mInteger.size ()))
			ret.mInteger[i] = BYTE (0X00) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = r1x - 1 - i ;
			const auto r2x = VAL64 (fake.mInteger[ix]) + rax ;
			const auto r3x = r2x / scale ;
			const auto r4x = QUAD (r3x) ;
			ret.mInteger[ix] = BYTE (r4x) ;
			const auto r5x = r2x - r3x * scale ;
			rax = VAL64 (QUAD (r5x) << 8) ;
		}
		return move (ret) ;
	}

	IntegerLayout smod (CREF<VAL64> scale) const override {
		assert (scale != ZERO) ;
		auto rax = VAL64 (0) ;
		const auto r1x = find_highest () + 1 ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = r1x - 1 - i ;
			const auto r2x = VAL64 (fake.mInteger[ix]) + rax ;
			const auto r3x = r2x / scale ;
			const auto r4x = r2x - r3x * scale ;
			rax = VAL64 (QUAD (r4x) << 8) ;
		}
		return Integer (fake.mInteger.size () ,rax) ;
	}

	IntegerLayout plus () const override {
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ()))
			ret.mInteger[i] = fake.mInteger[i] ;
		return move (ret) ;
	}

	IntegerLayout minus () const override {
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ()))
			ret.mInteger[i] = ~fake.mInteger[i] ;
		IntegerHolder::hold (ret)->increase () ;
		return move (ret) ;
	}

	void increase () override {
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= fake.mInteger.size ())
				break ;
			fake.mInteger[ix] = BYTE (LENGTH (fake.mInteger[ix]) + 1) ;
			if (fake.mInteger[ix] != BYTE (0X00))
				break ;
			ix++ ;
		}
	}

	void decrease () override {
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= fake.mInteger.size ())
				break ;
			fake.mInteger[ix] = BYTE (LENGTH (fake.mInteger[ix]) - 1) ;
			if (fake.mInteger[ix] != BYTE (0XFF))
				break ;
			ix++ ;
		}
	}
} ;

exports VFat<IntegerHolder> IntegerHolder::hold (VREF<IntegerLayout> that) {
	return VFat<IntegerHolder> (IntegerImplHolder () ,that) ;
}

exports CFat<IntegerHolder> IntegerHolder::hold (CREF<IntegerLayout> that) {
	return CFat<IntegerHolder> (IntegerImplHolder () ,that) ;
}

class HashProcImplHolder implement Fat<HashProcHolder ,HashProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_) const override {
		return fnvhash32 (src ,size_ ,CHAR (2166136261UL)) ;
	}

	CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<CHAR> curr) const override {
		CHAR ret = curr ;
		auto &&rax = keep[TYPE<ARR<BYTE>>::expr] (src) ;
		for (auto &&i : iter (0 ,size_)) {
			ret = ret ^ CHAR (rax[i]) ;
			ret = CHAR (VAL32 (ret) * VAL32 (16777619UL)) ;
		}
		return move (ret) ;
	}

	QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_) const override {
		return fnvhash64 (src ,size_ ,QUAD (14695981039346656037ULL)) ;
	}

	QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<QUAD> curr) const override {
		QUAD ret = curr ;
		auto &&rax = keep[TYPE<ARR<BYTE>>::expr] (src) ;
		for (auto &&i : iter (0 ,size_)) {
			ret = ret ^ QUAD (rax[i]) ;
			ret = QUAD (VAL64 (ret) * VAL64 (1099511628211ULL)) ;
		}
		return move (ret) ;
	}

	BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_) const override {
		return crchash8 (src ,size_ ,BYTE (0X00)) ;
	}

	BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<BYTE> curr) const override {
		static const ARR<csc_uint8_t ,ENUM<256>> mCache {
			0X00 ,0X5E ,0XBC ,0XE2 ,0X61 ,0X3F ,0XDD ,0X83 ,0XC2 ,0X9C ,0X7E ,0X20 ,0XA3 ,0XFD ,0X1F ,0X41 ,0X9D ,0XC3 ,0X21 ,0X7F ,0XFC ,0XA2 ,0X40 ,0X1E ,0X5F ,0X01 ,0XE3 ,0XBD ,0X3E ,0X60 ,0X82 ,0XDC ,0X23 ,0X7D ,0X9F ,0XC1 ,0X42 ,0X1C ,0XFE ,0XA0 ,0XE1 ,0XBF ,0X5D ,0X03 ,0X80 ,0XDE ,0X3C ,0X62 ,0XBE ,0XE0 ,0X02 ,0X5C ,0XDF ,0X81 ,0X63 ,0X3D ,0X7C ,0X22 ,0XC0 ,0X9E ,0X1D ,0X43 ,0XA1 ,0XFF ,0X46 ,0X18 ,0XFA ,0XA4 ,0X27 ,0X79 ,0X9B ,0XC5 ,0X84 ,0XDA ,0X38 ,0X66 ,0XE5 ,0XBB ,0X59 ,0X07 ,0XDB ,0X85 ,0X67 ,0X39 ,0XBA ,0XE4 ,0X06 ,0X58 ,0X19 ,0X47 ,0XA5 ,0XFB ,0X78 ,0X26 ,0XC4 ,0X9A ,0X65 ,0X3B ,0XD9 ,0X87 ,0X04 ,0X5A ,0XB8 ,0XE6 ,0XA7 ,0XF9 ,0X1B ,0X45 ,0XC6 ,0X98 ,0X7A ,0X24 ,0XF8 ,0XA6 ,0X44 ,0X1A ,0X99 ,0XC7 ,0X25 ,0X7B ,0X3A ,0X64 ,0X86 ,0XD8 ,0X5B ,0X05 ,0XE7 ,0XB9 ,0X8C ,0XD2 ,0X30 ,0X6E ,0XED ,0XB3 ,0X51 ,0X0F ,0X4E ,0X10 ,0XF2 ,0XAC ,0X2F ,0X71 ,0X93 ,0XCD ,0X11 ,0X4F ,0XAD ,0XF3 ,0X70 ,0X2E ,0XCC ,0X92 ,0XD3 ,0X8D ,0X6F ,0X31 ,0XB2 ,0XEC ,0X0E ,0X50 ,0XAF ,0XF1 ,0X13 ,0X4D ,0XCE ,0X90 ,0X72 ,0X2C ,0X6D ,0X33 ,0XD1 ,0X8F ,0X0C ,0X52 ,0XB0 ,0XEE ,0X32 ,0X6C ,0X8E ,0XD0 ,0X53 ,0X0D ,0XEF ,0XB1 ,0XF0 ,0XAE ,0X4C ,0X12 ,0X91 ,0XCF ,0X2D ,0X73 ,0XCA ,0X94 ,0X76 ,0X28 ,0XAB ,0XF5 ,0X17 ,0X49 ,0X08 ,0X56 ,0XB4 ,0XEA ,0X69 ,0X37 ,0XD5 ,0X8B ,0X57 ,0X09 ,0XEB ,0XB5 ,0X36 ,0X68 ,0X8A ,0XD4 ,0X95 ,0XCB ,0X29 ,0X77 ,0XF4 ,0XAA ,0X48 ,0X16 ,0XE9 ,0XB7 ,0X55 ,0X0B ,0X88 ,0XD6 ,0X34 ,0X6A ,0X2B ,0X75 ,0X97 ,0XC9 ,0X4A ,0X14 ,0XF6 ,0XA8 ,0X74 ,0X2A ,0XC8 ,0X96 ,0X15 ,0X4B ,0XA9 ,0XF7 ,0XB6 ,0XE8 ,0X0A ,0X54 ,0XD7 ,0X89 ,0X6B ,0X35} ;
		BYTE ret = curr ;
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

	WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<WORD> curr) const override {
		static const ARR<csc_uint16_t ,ENUM<256>> mCache {
			0x0000 ,0x1021 ,0x2042 ,0x3063 ,0x4084 ,0x50A5 ,0x60C6 ,0x70E7 ,0x8108 ,0x9129 ,0xA14A ,0xB16B ,0xC18C ,0xD1AD ,0xE1CE ,0xF1EF ,0x1231 ,0x0210 ,0x3273 ,0x2252 ,0x52B5 ,0x4294 ,0x72F7 ,0x62D6 ,0x9339 ,0x8318 ,0xB37B ,0xA35A ,0xD3BD ,0xC39C ,0xF3FF ,0xE3DE ,0x2462 ,0x3443 ,0x0420 ,0x1401 ,0x64E6 ,0x74C7 ,0x44A4 ,0x5485 ,0xA56A ,0xB54B ,0x8528 ,0x9509 ,0xE5EE ,0xF5CF ,0xC5AC ,0xD58D ,0x3653 ,0x2672 ,0x1611 ,0x0630 ,0x76D7 ,0x66F6 ,0x5695 ,0x46B4 ,0xB75B ,0xA77A ,0x9719 ,0x8738 ,0xF7DF ,0xE7FE ,0xD79D ,0xC7BC ,0x48C4 ,0x58E5 ,0x6886 ,0x78A7 ,0x0840 ,0x1861 ,0x2802 ,0x3823 ,0xC9CC ,0xD9ED ,0xE98E ,0xF9AF ,0x8948 ,0x9969 ,0xA90A ,0xB92B ,0x5AF5 ,0x4AD4 ,0x7AB7 ,0x6A96 ,0x1A71 ,0x0A50 ,0x3A33 ,0x2A12 ,0xDBFD ,0xCBDC ,0xFBBF ,0xEB9E ,0x9B79 ,0x8B58 ,0xBB3B ,0xAB1A ,0x6CA6 ,0x7C87 ,0x4CE4 ,0x5CC5 ,0x2C22 ,0x3C03 ,0x0C60 ,0x1C41 ,0xEDAE ,0xFD8F ,0xCDEC ,0xDDCD ,0xAD2A ,0xBD0B ,0x8D68 ,0x9D49 ,0x7E97 ,0x6EB6 ,0x5ED5 ,0x4EF4 ,0x3E13 ,0x2E32 ,0x1E51 ,0x0E70 ,0xFF9F ,0xEFBE ,0xDFDD ,0xCFFC ,0xBF1B ,0xAF3A ,0x9F59 ,0x8F78 ,0x9188 ,0x81A9 ,0xB1CA ,0xA1EB ,0xD10C ,0xC12D ,0xF14E ,0xE16F ,0x1080 ,0x00A1 ,0x30C2 ,0x20E3 ,0x5004 ,0x4025 ,0x7046 ,0x6067 ,0x83B9 ,0x9398 ,0xA3FB ,0xB3DA ,0xC33D ,0xD31C ,0xE37F ,0xF35E ,0x02B1 ,0x1290 ,0x22F3 ,0x32D2 ,0x4235 ,0x5214 ,0x6277 ,0x7256 ,0xB5EA ,0xA5CB ,0x95A8 ,0x8589 ,0xF56E ,0xE54F ,0xD52C ,0xC50D ,0x34E2 ,0x24C3 ,0x14A0 ,0x0481 ,0x7466 ,0x6447 ,0x5424 ,0x4405 ,0xA7DB ,0xB7FA ,0x8799 ,0x97B8 ,0xE75F ,0xF77E ,0xC71D ,0xD73C ,0x26D3 ,0x36F2 ,0x0691 ,0x16B0 ,0x6657 ,0x7676 ,0x4615 ,0x5634 ,0xD94C ,0xC96D ,0xF90E ,0xE92F ,0x99C8 ,0x89E9 ,0xB98A ,0xA9AB ,0x5844 ,0x4865 ,0x7806 ,0x6827 ,0x18C0 ,0x08E1 ,0x3882 ,0x28A3 ,0xCB7D ,0xDB5C ,0xEB3F ,0xFB1E ,0x8BF9 ,0x9BD8 ,0xABBB ,0xBB9A ,0x4A75 ,0x5A54 ,0x6A37 ,0x7A16 ,0x0AF1 ,0x1AD0 ,0x2AB3 ,0x3A92 ,0xFD2E ,0xED0F ,0xDD6C ,0xCD4D ,0xBDAA ,0xAD8B ,0x9DE8 ,0x8DC9 ,0x7C26 ,0x6C07 ,0x5C64 ,0x4C45 ,0x3CA2 ,0x2C83 ,0x1CE0 ,0x0CC1 ,0xEF1F ,0xFF3E ,0xCF5D ,0xDF7C ,0xAF9B ,0xBFBA ,0x8FD9 ,0x9FF8 ,0x6E17 ,0x7E36 ,0x4E55 ,0x5E74 ,0x2E93 ,0x3EB2 ,0x0ED1 ,0x1EF0} ;
		WORD ret = curr ;
		auto &&rax = keep[TYPE<ARR<BYTE>>::expr] (src) ;
		for (auto &&i : iter (0 ,size_)) {
			const auto r1x = (ret >> 8) ^ WORD (rax[i]) ;
			const auto r2x = INDEX (r1x & WORD (0XFF)) ;
			ret = WORD (mCache[r2x]) ^ (ret << 8) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<HashProcHolder> HashProcHolder::hold (VREF<HashProcLayout> that) {
	return VFat<HashProcHolder> (HashProcImplHolder () ,that) ;
}

exports CFat<HashProcHolder> HashProcHolder::hold (CREF<HashProcLayout> that) {
	return CFat<HashProcHolder> (HashProcImplHolder () ,that) ;
}
} ;