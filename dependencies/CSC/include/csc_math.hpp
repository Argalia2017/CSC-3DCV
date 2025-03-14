#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
static constexpr auto MATH_E = FLT64 (2.71828182845904523536) ;
static constexpr auto MATH_PI = FLT64 (3.14159265358979323846) ;
static constexpr auto MATH_SQRT2 = FLT64 (1.41421356237309504880) ;
static constexpr auto MATH_PDF0 = FLT64 (0.39894228040143267794) ;
static constexpr auto MATH_R = MATH_PI / FLT64 (180) ;
static constexpr auto MATH_LN2 = FLT64 (0.693147180559945309417) ;
static constexpr auto MATH_LN10 = FLT64 (2.30258509299404568402) ;

struct MathProcLayout ;

struct MathProcHolder implement Interface {
	imports CREF<OfThis<UniqueRef<MathProcLayout>>> instance () ;
	imports VFat<MathProcHolder> hold (VREF<MathProcLayout> that) ;
	imports CFat<MathProcHolder> hold (CREF<MathProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BOOL is_inf (CREF<FLT32> a) const = 0 ;
	virtual BOOL is_inf (CREF<FLT64> a) const = 0 ;
	virtual BOOL is_low (CREF<FLT32> a) const = 0 ;
	virtual BOOL is_low (CREF<FLT64> a) const = 0 ;
	virtual VAL32 step (CREF<VAL32> a) const = 0 ;
	virtual VAL64 step (CREF<VAL64> a) const = 0 ;
	virtual FLT32 step (CREF<FLT32> a) const = 0 ;
	virtual FLT64 step (CREF<FLT64> a) const = 0 ;
	virtual VAL32 sign (CREF<VAL32> a) const = 0 ;
	virtual VAL64 sign (CREF<VAL64> a) const = 0 ;
	virtual FLT32 sign (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sign (CREF<FLT64> a) const = 0 ;
	virtual VAL32 square (CREF<VAL32> a) const = 0 ;
	virtual VAL64 square (CREF<VAL64> a) const = 0 ;
	virtual FLT32 square (CREF<FLT32> a) const = 0 ;
	virtual FLT64 square (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sqrt (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sqrt (CREF<FLT64> a) const = 0 ;
	virtual VAL32 cubic (CREF<VAL32> a) const = 0 ;
	virtual VAL64 cubic (CREF<VAL64> a) const = 0 ;
	virtual FLT32 cubic (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cubic (CREF<FLT64> a) const = 0 ;
	virtual FLT32 cbrt (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cbrt (CREF<FLT64> a) const = 0 ;
	virtual FLT32 pow (CREF<FLT32> a ,CREF<VAL32> b) const = 0 ;
	virtual FLT64 pow (CREF<FLT64> a ,CREF<VAL32> b) const = 0 ;
	virtual FLT32 hypot (CREF<FLT32> a ,CREF<FLT32> b) const = 0 ;
	virtual FLT64 hypot (CREF<FLT64> a ,CREF<FLT64> b) const = 0 ;
	virtual VAL32 abs (CREF<VAL32> a) const = 0 ;
	virtual VAL64 abs (CREF<VAL64> a) const = 0 ;
	virtual FLT32 abs (CREF<FLT32> a) const = 0 ;
	virtual FLT64 abs (CREF<FLT64> a) const = 0 ;
	virtual FLT32 inverse (CREF<FLT32> a) const = 0 ;
	virtual FLT64 inverse (CREF<FLT64> a) const = 0 ;
	virtual FLT32 floor (CREF<FLT32> a ,CREF<FLT32> b) const = 0 ;
	virtual FLT64 floor (CREF<FLT64> a ,CREF<FLT64> b) const = 0 ;
	virtual FLT32 round (CREF<FLT32> a ,CREF<FLT32> b) const = 0 ;
	virtual FLT64 round (CREF<FLT64> a ,CREF<FLT64> b) const = 0 ;
	virtual FLT32 ceil (CREF<FLT32> a ,CREF<FLT32> b) const = 0 ;
	virtual FLT64 ceil (CREF<FLT64> a ,CREF<FLT64> b) const = 0 ;
	virtual VAL32 clamp (CREF<VAL32> a ,CREF<VAL32> min_ ,CREF<VAL32> max_) const = 0 ;
	virtual VAL64 clamp (CREF<VAL64> a ,CREF<VAL64> min_ ,CREF<VAL64> max_) const = 0 ;
	virtual FLT32 clamp (CREF<FLT32> a ,CREF<FLT32> min_ ,CREF<FLT32> max_) const = 0 ;
	virtual FLT64 clamp (CREF<FLT64> a ,CREF<FLT64> min_ ,CREF<FLT64> max_) const = 0 ;
	virtual VAL32 lerp (CREF<FLT64> a ,CREF<VAL32> min_ ,CREF<VAL32> max_) const = 0 ;
	virtual VAL64 lerp (CREF<FLT64> a ,CREF<VAL64> min_ ,CREF<VAL64> max_) const = 0 ;
	virtual FLT32 cos (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cos (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sin (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sin (CREF<FLT64> a) const = 0 ;
	virtual FLT32 tan (CREF<FLT32> a) const = 0 ;
	virtual FLT64 tan (CREF<FLT64> a) const = 0 ;
	virtual FLT32 acos (CREF<FLT32> a) const = 0 ;
	virtual FLT64 acos (CREF<FLT64> a) const = 0 ;
	virtual FLT32 asin (CREF<FLT32> a) const = 0 ;
	virtual FLT64 asin (CREF<FLT64> a) const = 0 ;
	virtual FLT32 atan (CREF<FLT32> y ,CREF<FLT32> x) const = 0 ;
	virtual FLT64 atan (CREF<FLT64> y ,CREF<FLT64> x) const = 0 ;
	virtual FLT32 exp (CREF<FLT32> a) const = 0 ;
	virtual FLT64 exp (CREF<FLT64> a) const = 0 ;
	virtual FLT32 log (CREF<FLT32> a) const = 0 ;
	virtual FLT64 log (CREF<FLT64> a) const = 0 ;
	virtual FLT32 pdf (CREF<FLT32> a) const = 0 ;
	virtual FLT64 pdf (CREF<FLT64> a) const = 0 ;
	virtual FLT32 cbf (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cbf (CREF<FLT64> a) const = 0 ;
	virtual BOOL all_of (CREF<BOOL> a ,CREF<WrapperLayout> b) const = 0 ;
	virtual BOOL any_of (CREF<BOOL> a ,CREF<WrapperLayout> b) const = 0 ;
	virtual VAL32 max_of (CREF<VAL32> a ,CREF<WrapperLayout> b) const = 0 ;
	virtual VAL64 max_of (CREF<VAL64> a ,CREF<WrapperLayout> b) const = 0 ;
	virtual FLT32 max_of (CREF<FLT32> a ,CREF<WrapperLayout> b) const = 0 ;
	virtual FLT64 max_of (CREF<FLT64> a ,CREF<WrapperLayout> b) const = 0 ;
	virtual VAL32 min_of (CREF<VAL32> a ,CREF<WrapperLayout> b) const = 0 ;
	virtual VAL64 min_of (CREF<VAL64> a ,CREF<WrapperLayout> b) const = 0 ;
	virtual FLT32 min_of (CREF<FLT32> a ,CREF<WrapperLayout> b) const = 0 ;
	virtual FLT64 min_of (CREF<FLT64> a ,CREF<WrapperLayout> b) const = 0 ;
} ;

class MathProc implement OfThis<UniqueRef<MathProcLayout>> {
public:
	static CREF<MathProc> instance () {
		return keep[TYPE<MathProc>::expr] (MathProcHolder::instance ()) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static BOOL is_inf (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->is_inf (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static BOOL is_low (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->is_low (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 step (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->step (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 sign (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->sign (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 square (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->square (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 sqrt (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->sqrt (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 cubic (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->cubic (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 cbrt (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->cbrt (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 pow (CREF<ARG1> a ,CREF<VAL32> b) {
		return MathProcHolder::hold (instance ())->pow (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 hypot (CREF<ARG1> a ,CREF<ARG1> b) {
		return MathProcHolder::hold (instance ())->hypot (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 abs (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->abs (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 inverse (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->inverse (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 floor (CREF<ARG1> a ,CREF<ARG1> b) {
		return MathProcHolder::hold (instance ())->floor (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 round (CREF<ARG1> a ,CREF<ARG1> b) {
		return MathProcHolder::hold (instance ())->round (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 ceil (CREF<ARG1> a ,CREF<ARG1> b) {
		return MathProcHolder::hold (instance ())->ceil (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 clamp (CREF<ARG1> a ,CREF<ARG1> min_ ,CREF<ARG1> max_) {
		return MathProcHolder::hold (instance ())->clamp (a ,min_ ,max_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	static ARG1 lerp (CREF<FLT64> a ,CREF<ARG1> min_ ,CREF<ARG1> max_) {
		return MathProcHolder::hold (instance ())->lerp (a ,min_ ,max_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 cos (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->cos (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 sin (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->sin (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 tan (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->tan (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 acos (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->acos (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 asin (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->asin (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 atan (CREF<ARG1> y ,CREF<ARG1> x) {
		return MathProcHolder::hold (instance ())->atan (y ,x) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 exp (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->exp (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 log (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->log (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 pdf (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->pdf (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 cbf (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->cbf (a) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<IS_BOOL<ARG1>> ,class = REQUIRE<ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>>>
	static BOOL all_of (CREF<ARG1> a ,CREF<ARG2>...b) {
		return MathProcHolder::hold (instance ())->all_of (a ,MakeWrapper (b...)) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<IS_BOOL<ARG1>> ,class = REQUIRE<ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>>>
	static BOOL any_of (CREF<ARG1> a ,CREF<ARG2>...b) {
		return MathProcHolder::hold (instance ())->any_of (a ,MakeWrapper (b...)) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<IS_SCALAR<ARG1>> ,class = REQUIRE<ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>>>
	static ARG1 max_of (CREF<ARG1> a ,CREF<ARG2>...b) {
		return MathProcHolder::hold (instance ())->max_of (a ,MakeWrapper (b...)) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<IS_SCALAR<ARG1>> ,class = REQUIRE<ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>>>
	static ARG1 min_of (CREF<ARG1> a ,CREF<ARG2>...b) {
		return MathProcHolder::hold (instance ())->min_of (a ,MakeWrapper (b...)) ;
	}
} ;

struct NormalErrorLayout {
	LENGTH mCount ;
	FLT64 mMaxError ;
	FLT64 mAvgError ;
	FLT64 mStdError ;

public:
	implicit NormalErrorLayout () noexcept {
		mCount = 0 ;
		mMaxError = 0 ;
		mAvgError = 0 ;
		mStdError = 0 ;
	}
} ;

struct NormalErrorHolder implement Interface {
	imports VFat<NormalErrorHolder> hold (VREF<NormalErrorLayout> that) ;
	imports CFat<NormalErrorHolder> hold (CREF<NormalErrorLayout> that) ;

	virtual void concat (CREF<FLT64> error) = 0 ;
} ;

class NormalError implement NormalErrorLayout {
public:
	implicit NormalError () = default ;

	void concat (CREF<FLT64> error) { 
		return NormalErrorHolder::hold (thiz)->concat (error) ;
	}

	forceinline void operator+= (CREF<FLT64> error) {
		return concat (error) ;
	}
} ;

struct Notation {
	FLAG mRadix ;
	LENGTH mPrecision ;
	BOOL mSign ;
	VAL64 mMantissa ;
	VAL64 mDownflow ;
	VAL64 mExponent ;
} ;

struct FEXP2CacheLayout ;

struct FEXP2CacheHolder implement Interface {
	imports CREF<OfThis<UniqueRef<FEXP2CacheLayout>>> instance () ;
	imports VFat<FEXP2CacheHolder> hold (VREF<FEXP2CacheLayout> that) ;
	imports CFat<FEXP2CacheHolder> hold (CREF<FEXP2CacheLayout> that) ;

	virtual void create (VREF<UniqueRef<FEXP2CacheLayout>> that) const = 0 ;
	virtual void initialize () = 0 ;
	virtual void get (CREF<VAL64> index ,VREF<Notation> item) const = 0 ;
} ;

class FEXP2Cache implement OfThis<UniqueRef<FEXP2CacheLayout>> {
public:
	static CREF<FEXP2Cache> instance () {
		return keep[TYPE<FEXP2Cache>::expr] (FEXP2CacheHolder::instance ()) ;
	}

	forceinline Notation operator[] (CREF<VAL64> index) const {
		Notation ret ;
		FEXP2CacheHolder::hold (thiz)->get (index ,ret) ;
		return move (ret) ;
	}
} ;

struct FEXP10CacheLayout ;

struct FEXP10CacheHolder implement Interface {
	imports CREF<OfThis<UniqueRef<FEXP10CacheLayout>>> instance () ;
	imports VFat<FEXP10CacheHolder> hold (VREF<FEXP10CacheLayout> that) ;
	imports CFat<FEXP10CacheHolder> hold (CREF<FEXP10CacheLayout> that) ;

	virtual void create (VREF<UniqueRef<FEXP10CacheLayout>> that) const = 0 ;
	virtual void initialize () = 0 ;
	virtual void get (CREF<VAL64> index ,VREF<Notation> item) const = 0 ;
} ;

class FEXP10Cache implement OfThis<UniqueRef<FEXP10CacheLayout>> {
public:
	static CREF<FEXP10Cache> instance () {
		return keep[TYPE<FEXP10Cache>::expr] (FEXP10CacheHolder::instance ()) ;
	}

	forceinline Notation operator[] (CREF<VAL64> index) const {
		Notation ret ;
		FEXP10CacheHolder::hold (thiz)->get (index ,ret) ;
		return move (ret) ;
	}
} ;

struct FloatProcLayout ;

struct FloatProcHolder implement Interface {
	imports CREF<OfThis<UniqueRef<FloatProcLayout>>> instance () ;
	imports VFat<FloatProcHolder> hold (VREF<FloatProcLayout> that) ;
	imports CFat<FloatProcHolder> hold (CREF<FloatProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH value_precision () const = 0 ;
	virtual LENGTH float_precision () const = 0 ;
	virtual FLT64 encode (CREF<Notation> fexp2) const = 0 ;
	virtual Notation decode (CREF<FLT64> float_) const = 0 ;
	virtual Notation fexp2_from_fexp10 (CREF<Notation> fexp10) const = 0 ;
	virtual Notation fexp10_from_fexp2 (CREF<Notation> fexp2) const = 0 ;
} ;

class FloatProc implement OfThis<UniqueRef<FloatProcLayout>> {
public:
	static CREF<FloatProc> instance () {
		return keep[TYPE<FloatProc>::expr] (FloatProcHolder::instance ()) ;
	}

	static LENGTH value_precision () {
		return FloatProcHolder::hold (instance ())->value_precision () ;
	}

	static LENGTH float_precision () {
		return FloatProcHolder::hold (instance ())->float_precision () ;
	}

	static FLT64 encode (CREF<Notation> fexp2) {
		return FloatProcHolder::hold (instance ())->encode (fexp2) ;
	}

	static Notation decode (CREF<FLT64> float_) {
		return FloatProcHolder::hold (instance ())->decode (float_) ;
	}

	static Notation fexp2_from_fexp10 (CREF<Notation> fexp10) {
		return FloatProcHolder::hold (instance ())->fexp2_from_fexp10 (fexp10) ;
	}

	static Notation fexp10_from_fexp2 (CREF<Notation> fexp2) {
		return FloatProcHolder::hold (instance ())->fexp10_from_fexp2 (fexp2) ;
	}
} ;

struct ByteProcLayout ;

struct ByteProcHolder implement Interface {
	imports CREF<OfThis<UniqueRef<ByteProcLayout>>> instance () ;
	imports VFat<ByteProcHolder> hold (VREF<ByteProcLayout> that) ;
	imports CFat<ByteProcHolder> hold (CREF<ByteProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BYTE split_low (CREF<WORD> a) const = 0 ;
	virtual WORD split_low (CREF<CHAR> a) const = 0 ;
	virtual CHAR split_low (CREF<QUAD> a) const = 0 ;
	virtual BYTE split_high (CREF<WORD> a) const = 0 ;
	virtual WORD split_high (CREF<CHAR> a) const = 0 ;
	virtual CHAR split_high (CREF<QUAD> a) const = 0 ;
	virtual WORD merge (CREF<BYTE> high_ ,CREF<BYTE> low_) const = 0 ;
	virtual CHAR merge (CREF<WORD> high_ ,CREF<WORD> low_) const = 0 ;
	virtual QUAD merge (CREF<CHAR> high_ ,CREF<CHAR> low_) const = 0 ;
	virtual BYTE reverse (CREF<BYTE> a) const = 0 ;
	virtual WORD reverse (CREF<WORD> a) const = 0 ;
	virtual CHAR reverse (CREF<CHAR> a) const = 0 ;
	virtual QUAD reverse (CREF<QUAD> a) const = 0 ;
	virtual BOOL any_bit (CREF<BYTE> a ,CREF<BYTE> mask) const = 0 ;
	virtual BOOL any_bit (CREF<WORD> a ,CREF<WORD> mask) const = 0 ;
	virtual BOOL any_bit (CREF<CHAR> a ,CREF<CHAR> mask) const = 0 ;
	virtual BOOL any_bit (CREF<QUAD> a ,CREF<QUAD> mask) const = 0 ;
	virtual BOOL all_bit (CREF<BYTE> a ,CREF<BYTE> mask) const = 0 ;
	virtual BOOL all_bit (CREF<WORD> a ,CREF<WORD> mask) const = 0 ;
	virtual BOOL all_bit (CREF<CHAR> a ,CREF<CHAR> mask) const = 0 ;
	virtual BOOL all_bit (CREF<QUAD> a ,CREF<QUAD> mask) const = 0 ;
	virtual BYTE binary (CREF<BYTE> a) const = 0 ;
	virtual WORD binary (CREF<WORD> a) const = 0 ;
	virtual CHAR binary (CREF<CHAR> a) const = 0 ;
	virtual QUAD binary (CREF<QUAD> a) const = 0 ;
	virtual INDEX pow_bit (CREF<LENGTH> nth) const = 0 ;
	virtual LENGTH popcount (CREF<BYTE> a) const = 0 ;
	virtual LENGTH lowcount (CREF<BYTE> a) const = 0 ;
} ;

class ByteProc implement OfThis<UniqueRef<ByteProcLayout>> {
public:
	static CREF<ByteProc> instance () {
		return keep[TYPE<ByteProc>::expr] (ByteProcHolder::instance ()) ;
	}

	static BYTE split_low (CREF<WORD> a) {
		return ByteProcHolder::hold (instance ())->split_low (a) ;
	}

	static WORD split_low (CREF<CHAR> a) {
		return ByteProcHolder::hold (instance ())->split_low (a) ;
	}

	static CHAR split_low (CREF<QUAD> a) {
		return ByteProcHolder::hold (instance ())->split_low (a) ;
	}

	static BYTE split_high (CREF<WORD> a) {
		return ByteProcHolder::hold (instance ())->split_high (a) ;
	}

	static WORD split_high (CREF<CHAR> a) {
		return ByteProcHolder::hold (instance ())->split_high (a) ;
	}

	static CHAR split_high (CREF<QUAD> a) {
		return ByteProcHolder::hold (instance ())->split_high (a) ;
	}

	static WORD merge (CREF<BYTE> high_ ,CREF<BYTE> low_) {
		return ByteProcHolder::hold (instance ())->merge (high_ ,low_) ;
	}

	static CHAR merge (CREF<WORD> high_ ,CREF<WORD> low_) {
		return ByteProcHolder::hold (instance ())->merge (high_ ,low_) ;
	}

	static QUAD merge (CREF<CHAR> high_ ,CREF<CHAR> low_) {
		return ByteProcHolder::hold (instance ())->merge (high_ ,low_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static ARG1 reverse (CREF<ARG1> a) {
		return ByteProcHolder::hold (instance ())->reverse (a) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static BOOL any_bit (CREF<ARG1> a ,CREF<ARG2> mask) {
		return ByteProcHolder::hold (instance ())->any_bit (a ,ARG1 (mask)) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static BOOL all_bit (CREF<ARG1> a ,CREF<ARG2> mask) {
		return ByteProcHolder::hold (instance ())->all_bit (a ,ARG1 (mask)) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static ARG1 binary (CREF<ARG1> a) {
		return ByteProcHolder::hold (instance ())->binary (a) ;
	}

	static INDEX pow_bit (CREF<LENGTH> nth) {
		return ByteProcHolder::hold (instance ())->pow_bit (nth) ;
	}

	static LENGTH popcount (CREF<BYTE> a) {
		return ByteProcHolder::hold (instance ())->popcount (a) ;
	}

	static LENGTH lowcount (CREF<BYTE> a) {
		return ByteProcHolder::hold (instance ())->lowcount (a) ;
	}
} ;

struct IntegerLayout {
	RefBuffer<BYTE> mInteger ;
	LENGTH mWidth ;

public:
	implicit IntegerLayout () noexcept {
		mWidth = 0 ;
	}
} ;

struct IntegerHolder implement Interface {
	imports VFat<IntegerHolder> hold (VREF<IntegerLayout> that) ;
	imports CFat<IntegerHolder> hold (CREF<IntegerLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<IntegerLayout> that) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual VAL64 fetch () const = 0 ;
	virtual void store (CREF<VAL64> item) = 0 ;
	virtual BOOL equal (CREF<IntegerLayout> that) const = 0 ;
	virtual FLAG compr (CREF<IntegerLayout> that) const = 0 ;
	virtual void visit (VREF<VisitorBinder> visitor) const = 0 ;
	virtual IntegerLayout sadd (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout ssub (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout smul (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout sdiv (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout smod (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout sabs () const = 0 ;
	virtual IntegerLayout minus () const = 0 ;
	virtual IntegerLayout lshift (CREF<LENGTH> scale) const = 0 ;
	virtual IntegerLayout rshift (CREF<LENGTH> scale) const = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
} ;

class Integer implement IntegerLayout {
protected:
	using IntegerLayout::mInteger ;

public:
	implicit Integer () = default ;

	explicit Integer (CREF<VAL64> item) {
		IntegerHolder::hold (thiz)->initialize (SIZE_OF<VAL64>::expr) ;
		IntegerHolder::hold (thiz)->store (item) ;
	}

	static CREF<Integer> zero () {
		return memorize ([&] () {
			return Integer (0) ;
		}) ;
	}

	implicit Integer (CREF<Integer> that) {
		IntegerHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<Integer> operator= (CREF<Integer> that) {
		return assign (thiz ,that) ;
	}

	implicit Integer (RREF<Integer> that) = default ;

	forceinline VREF<Integer> operator= (RREF<Integer> that) = default ;

	Integer clone () const {
		return move (thiz) ;
	}

	LENGTH size () const {
		return IntegerHolder::hold (thiz)->size () ;
	}

	VAL64 fetch () const {
		return IntegerHolder::hold (thiz)->fetch () ;
	}

	forceinline operator VAL64 () const {
		return fetch () ;
	}

	void store (CREF<VAL64> item) {
		return IntegerHolder::hold (thiz)->store (item) ;
	}

	BOOL equal (CREF<Integer> that) const {
		return IntegerHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Integer> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Integer> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Integer> that) const {
		return IntegerHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Integer> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Integer> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Integer> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Integer> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const {
		return IntegerHolder::hold (thiz)->visit (visitor) ;
	}

	Integer sadd (CREF<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator+ (CREF<Integer> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CREF<Integer> that) {
		thiz = sadd (that) ;
	}

	Integer ssub (CREF<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator- (CREF<Integer> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CREF<Integer> that) {
		thiz = ssub (that) ;
	}

	Integer smul (CREF<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator* (CREF<Integer> that) const {
		return smul (that) ;
	}

	forceinline void operator*= (CREF<Integer> that) {
		thiz = smul (that) ;
	}

	Integer sdiv (CREF<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->sdiv (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator/ (CREF<Integer> that) const {
		return sdiv (that) ;
	}

	forceinline void operator/= (CREF<Integer> that) {
		thiz = sdiv (that) ;
	}

	Integer smod (CREF<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->smod (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator% (CREF<Integer> that) const {
		return smod (that) ;
	}

	forceinline void operator%= (CREF<Integer> that) {
		thiz = smod (that) ;
	}

	Integer sabs () const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	Integer minus () const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator- () const {
		return minus () ;
	}

	Integer lshift (CREF<LENGTH> scale) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->lshift (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator<< (CREF<LENGTH> scale) const {
		return lshift (scale) ;
	}

	Integer rshift (CREF<LENGTH> scale) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->rshift (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator>> (CREF<LENGTH> scale) const {
		return rshift (scale) ;
	}

	void increase () {
		return IntegerHolder::hold (thiz)->increase () ;
	}

	forceinline void operator++ (int) {
		return increase () ;
	}

	void decrease () {
		return IntegerHolder::hold (thiz)->decrease () ;
	}

	forceinline void operator-- (int) {
		return decrease () ;
	}
} ;

struct JetNode ;
using JetEvalFunction = Function<VREF<JetNode> ,CREF<WrapperLayout>> ;

struct JetNode {
	FLT64 mFX ;
	FLT64 mEX ;
	RefBuffer<FLT64> mDX ;
	INDEX mSlot ;
	JetEvalFunction mEval ;
	Ref<JetNode> mFake ;
	Ref<JetNode> mThat ;
} ;

struct JetLayout {
	Ref<JetNode> mThis ;
} ;

struct JetHolder implement Interface {
	imports VFat<JetHolder> hold (VREF<JetLayout> that) ;
	imports CFat<JetHolder> hold (CREF<JetLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_ ,CREF<FLT64> item) = 0 ;
	virtual void initialize (CREF<LENGTH> size_ ,CREF<FLT64> item ,CREF<INDEX> slot) = 0 ;
	virtual FLT64 fx () const = 0 ;
	virtual FLT64 ex () const = 0 ;
	virtual FLT64 dx (CREF<INDEX> slot) const = 0 ;
	virtual void once (CREF<WrapperLayout> params) const = 0 ;
	virtual JetLayout sadd (CREF<JetLayout> that) const = 0 ;
	virtual JetLayout ssub (CREF<JetLayout> that) const = 0 ;
	virtual JetLayout smul (CREF<JetLayout> that) const = 0 ;
	virtual JetLayout sdiv (CREF<JetLayout> that) const = 0 ;
	virtual JetLayout inverse () const = 0 ;
	virtual JetLayout ssqrt () const = 0 ;
	virtual JetLayout scbrt () const = 0 ;
	virtual JetLayout spow (CREF<VAL32> that) const = 0 ;
	virtual JetLayout shypot (CREF<JetLayout> that) const = 0 ;
	virtual JetLayout sabs () const = 0 ;
	virtual JetLayout minus () const = 0 ;
	virtual JetLayout ssin () const = 0 ;
	virtual JetLayout scos () const = 0 ;
	virtual JetLayout stan () const = 0 ;
	virtual JetLayout sasin () const = 0 ;
	virtual JetLayout sacos () const = 0 ;
	virtual JetLayout satan (CREF<JetLayout> that) const = 0 ;
	virtual JetLayout sexp () const = 0 ;
	virtual JetLayout slog () const = 0 ;
} ;

template <class A>
class Jet implement JetLayout {
protected:
	using JetLayout::mThis ;

public:
	implicit Jet () = default ;

	implicit Jet (CREF<FLT64> item) {
		JetHolder::hold (thiz)->initialize (A::expr ,item) ;
	}

	explicit Jet (CREF<FLT64> item ,CREF<INDEX> slot) {
		JetHolder::hold (thiz)->initialize (A::expr ,item ,slot) ;
	}

	FLT64 fx () const {
		return JetHolder::hold (thiz)->fx () ;
	}

	FLT64 ex () const {
		return JetHolder::hold (thiz)->ex () ;
	}

	FLT64 dx (CREF<INDEX> slot) const {
		return JetHolder::hold (thiz)->dx (slot) ;
	}

	template <class...ARG1 ,class = REQUIRE<ENUM_ALL<IS_SAME<FLT64 ,ARG1>...>>>
	void once (CREF<ARG1>...params) const {
		require (ENUM_EQUAL<RANK_OF<TYPE<ARG1...>> ,A>) ;
		return JetHolder::hold (thiz)->once (MakeWrapper (params...)) ;
	}

	template <class...ARG1 ,class = REQUIRE<ENUM_ALL<IS_SAME<FLT64 ,ARG1>...>>>
	forceinline void operator() (CREF<ARG1>...params) const {
		return once (params...) ;
	}

	Jet sadd (CREF<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator+ (CREF<Jet> that) const {
		return sadd (that) ;
	}

	forceinline VREF<Jet> operator+= (CREF<Jet> that) {
		thiz = sadd (that) ;
	}

	Jet ssub (CREF<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator- (CREF<Jet> that) const {
		return ssub (that) ;
	}

	forceinline VREF<Jet> operator-= (CREF<Jet> that) {
		thiz = ssub (that) ;
	}

	Jet smul (CREF<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator* (CREF<Jet> that) const {
		return smul (that) ;
	}

	forceinline VREF<Jet> operator*= (CREF<Jet> that) {
		thiz = smul (that) ;
	}

	Jet sdiv (CREF<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->sdiv (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator/ (CREF<Jet> that) const {
		return sdiv (that) ;
	}

	forceinline VREF<Jet> operator/= (CREF<Jet> that) {
		thiz = sdiv (that) ;
	}

	Jet inverse () const {
		JetLayout ret = JetHolder::hold (thiz)->inverse () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet ssqrt () const {
		JetLayout ret = JetHolder::hold (thiz)->ssqrt () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet scbrt () const {
		JetLayout ret = JetHolder::hold (thiz)->scbrt () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet spow (CREF<VAL32> that) const {
		JetLayout ret = JetHolder::hold (thiz)->spow (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet shypot (CREF<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->shypot (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet sabs () const {
		JetLayout ret = JetHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet minus () const {
		JetLayout ret = JetHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator- () const {
		return minus () ;
	}

	Jet ssin () const {
		JetLayout ret = JetHolder::hold (thiz)->ssin () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet scos () const {
		JetLayout ret = JetHolder::hold (thiz)->scos () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet stan () const {
		JetLayout ret = JetHolder::hold (thiz)->stan () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet sasin () const {
		JetLayout ret = JetHolder::hold (thiz)->sasin () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet sacos () const {
		JetLayout ret = JetHolder::hold (thiz)->sacos () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet satan (CREF<JetLayout> that) const {
		JetLayout ret = JetHolder::hold (thiz)->satan (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet sexp () const {
		JetLayout ret = JetHolder::hold (thiz)->sexp () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet slog () const {
		JetLayout ret = JetHolder::hold (thiz)->slog () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}
} ;

struct HashProcLayout ;

struct HashProcHolder implement Interface {
	imports CREF<OfThis<UniqueRef<HashProcLayout>>> instance () ;
	imports VFat<HashProcHolder> hold (VREF<HashProcLayout> that) ;
	imports CFat<HashProcHolder> hold (CREF<HashProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_) const = 0 ;
	virtual CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<CHAR> val) const = 0 ;
	virtual QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_) const = 0 ;
	virtual QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<QUAD> val) const = 0 ;
	virtual BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_) const = 0 ;
	virtual BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<BYTE> val) const = 0 ;
	virtual WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_) const = 0 ;
	virtual WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<WORD> val) const = 0 ;
} ;

class HashProc implement OfThis<UniqueRef<HashProcLayout>> {
public:
	static CREF<HashProc> instance () {
		return keep[TYPE<HashProc>::expr] (HashProcHolder::instance ()) ;
	}

	static CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_) {
		return HashProcHolder::hold (instance ())->fnvhash32 (src ,size_) ;
	}

	static CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<CHAR> val) {
		return HashProcHolder::hold (instance ())->fnvhash32 (src ,size_ ,val) ;
	}

	static QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_) {
		return HashProcHolder::hold (instance ())->fnvhash64 (src ,size_) ;
	}

	static QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<QUAD> val) {
		return HashProcHolder::hold (instance ())->fnvhash64 (src ,size_ ,val) ;
	}

	static BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_) {
		return HashProcHolder::hold (instance ())->crchash8 (src ,size_) ;
	}

	static BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<BYTE> val) {
		return HashProcHolder::hold (instance ())->crchash8 (src ,size_ ,val) ;
	}

	static WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_) {
		return HashProcHolder::hold (instance ())->crchash16 (src ,size_) ;
	}

	static WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<WORD> val) {
		return HashProcHolder::hold (instance ())->crchash16 (src ,size_ ,val) ;
	}
} ;
} ;