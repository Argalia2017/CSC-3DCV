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

struct MathProcLayout implement ThisLayout<RefLayout> {} ;

struct MathProcHolder implement Interface {
	imports CREF<MathProcLayout> instance () ;
	imports VFat<MathProcHolder> hold (VREF<MathProcLayout> that) ;
	imports CFat<MathProcHolder> hold (CREF<MathProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BOOL is_inf (CREF<FLT32> a) const = 0 ;
	virtual BOOL is_inf (CREF<FLT64> a) const = 0 ;
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
	virtual VAL32 clamp (CREF<VAL32> a ,CREF<VAL32> lb ,CREF<VAL32> rb) const = 0 ;
	virtual VAL64 clamp (CREF<VAL64> a ,CREF<VAL64> lb ,CREF<VAL64> rb) const = 0 ;
	virtual FLT32 clamp (CREF<FLT32> a ,CREF<FLT32> lb ,CREF<FLT32> rb) const = 0 ;
	virtual FLT64 clamp (CREF<FLT64> a ,CREF<FLT64> lb ,CREF<FLT64> rb) const = 0 ;
	virtual VAL32 lerp (CREF<FLT64> a ,CREF<VAL32> lb ,CREF<VAL32> rb) const = 0 ;
	virtual VAL64 lerp (CREF<FLT64> a ,CREF<VAL64> lb ,CREF<VAL64> rb) const = 0 ;
	virtual FLT32 cos (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cos (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sin (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sin (CREF<FLT64> a) const = 0 ;
	virtual FLT32 tan (CREF<FLT32> a) const = 0 ;
	virtual FLT64 tan (CREF<FLT64> a) const = 0 ;
	virtual FLT32 arccos (CREF<FLT32> a) const = 0 ;
	virtual FLT64 arccos (CREF<FLT64> a) const = 0 ;
	virtual FLT32 arcsin (CREF<FLT32> a) const = 0 ;
	virtual FLT64 arcsin (CREF<FLT64> a) const = 0 ;
	virtual FLT32 arctan (CREF<FLT32> y ,CREF<FLT32> x) const = 0 ;
	virtual FLT64 arctan (CREF<FLT64> y ,CREF<FLT64> x) const = 0 ;
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

class MathProc implement MathProcLayout {
protected:
	using MathProcLayout::mThis ;

public:
	static CREF<MathProc> instance () {
		return keep[TYPE<MathProc>::expr] (MathProcHolder::instance ()) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static BOOL is_inf (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->is_inf (a) ;
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
	static ARG1 clamp (CREF<ARG1> a ,CREF<ARG1> lb ,CREF<ARG1> rb) {
		return MathProcHolder::hold (instance ())->clamp (a ,lb ,rb) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	static ARG1 lerp (CREF<FLT64> a ,CREF<ARG1> lb ,CREF<ARG1> rb) {
		return MathProcHolder::hold (instance ())->lerp (a ,lb ,rb) ;
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
	static ARG1 arccos (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->arccos (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 arcsin (CREF<ARG1> a) {
		return MathProcHolder::hold (instance ())->arcsin (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 arctan (CREF<ARG1> y ,CREF<ARG1> x) {
		return MathProcHolder::hold (instance ())->arctan (y ,x) ;
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

	virtual void update (CREF<FLT64> error) = 0 ;
} ;

class NormalError implement NormalErrorLayout {
protected:
	using NormalErrorLayout::mCount ;
	using NormalErrorLayout::mMaxError ;
	using NormalErrorLayout::mAvgError ;
	using NormalErrorLayout::mStdError ;

public:
	implicit NormalError () = default ;

	CREF<NormalErrorLayout> layout () const leftvalue {
		return thiz ;
	}

	void update (CREF<FLT64> error) {
		return NormalErrorHolder::hold (thiz)->update (error) ;
	}

	forceinline void operator+= (CREF<FLT64> error) {
		return update (error) ;
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

struct FEXP2CacheLayout implement ThisLayout<RefLayout> {} ;

struct FEXP2CacheHolder implement Interface {
	imports CREF<FEXP2CacheLayout> instance () ;
	imports VFat<FEXP2CacheHolder> hold (VREF<FEXP2CacheLayout> that) ;
	imports CFat<FEXP2CacheHolder> hold (CREF<FEXP2CacheLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void get (CREF<VAL64> index ,VREF<Notation> item) const = 0 ;
} ;

class FEXP2Cache implement FEXP2CacheLayout {
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

struct FEXP10CacheLayout implement ThisLayout<RefLayout> {} ;

struct FEXP10CacheHolder implement Interface {
	imports CREF<FEXP10CacheLayout> instance () ;
	imports VFat<FEXP10CacheHolder> hold (VREF<FEXP10CacheLayout> that) ;
	imports CFat<FEXP10CacheHolder> hold (CREF<FEXP10CacheLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void get (CREF<VAL64> index ,VREF<Notation> item) const = 0 ;
} ;

class FEXP10Cache implement FEXP10CacheLayout {
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

struct FloatProcLayout implement ThisLayout<RefLayout> {} ;

struct FloatProcHolder implement Interface {
	imports CREF<FloatProcLayout> instance () ;
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

class FloatProc implement FloatProcLayout {
protected:
	using FloatProcLayout::mThis ;

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

struct ByteProcLayout implement ThisLayout<RefLayout> {} ;

struct ByteProcHolder implement Interface {
	imports CREF<ByteProcLayout> instance () ;
	imports VFat<ByteProcHolder> hold (VREF<ByteProcLayout> that) ;
	imports CFat<ByteProcHolder> hold (CREF<ByteProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BYTE bit_low (CREF<WORD> a) const = 0 ;
	virtual WORD bit_low (CREF<CHAR> a) const = 0 ;
	virtual CHAR bit_low (CREF<QUAD> a) const = 0 ;
	virtual BYTE bit_high (CREF<WORD> a) const = 0 ;
	virtual WORD bit_high (CREF<CHAR> a) const = 0 ;
	virtual CHAR bit_high (CREF<QUAD> a) const = 0 ;
	virtual WORD bit_merge (CREF<BYTE> high ,CREF<BYTE> low) const = 0 ;
	virtual CHAR bit_merge (CREF<WORD> high ,CREF<WORD> low) const = 0 ;
	virtual QUAD bit_merge (CREF<CHAR> high ,CREF<CHAR> low) const = 0 ;
	virtual BOOL bit_any (CREF<BYTE> curr ,CREF<BYTE> mask) const = 0 ;
	virtual BOOL bit_any (CREF<WORD> curr ,CREF<WORD> mask) const = 0 ;
	virtual BOOL bit_any (CREF<CHAR> curr ,CREF<CHAR> mask) const = 0 ;
	virtual BOOL bit_any (CREF<QUAD> curr ,CREF<QUAD> mask) const = 0 ;
	virtual BOOL bit_all (CREF<BYTE> curr ,CREF<BYTE> mask) const = 0 ;
	virtual BOOL bit_all (CREF<WORD> curr ,CREF<WORD> mask) const = 0 ;
	virtual BOOL bit_all (CREF<CHAR> curr ,CREF<CHAR> mask) const = 0 ;
	virtual BOOL bit_all (CREF<QUAD> curr ,CREF<QUAD> mask) const = 0 ;
	virtual BYTE bit_reverse (CREF<BYTE> a) const = 0 ;
	virtual WORD bit_reverse (CREF<WORD> a) const = 0 ;
	virtual CHAR bit_reverse (CREF<CHAR> a) const = 0 ;
	virtual QUAD bit_reverse (CREF<QUAD> a) const = 0 ;
	virtual INDEX bit_pow (CREF<LENGTH> nth) const = 0 ;
	virtual LENGTH popcount (CREF<BYTE> a) const = 0 ;
	virtual LENGTH lowcount (CREF<BYTE> a) const = 0 ;
} ;

class ByteProc implement ByteProcLayout {
protected:
	using ByteProcLayout::mThis ;

public:
	static CREF<ByteProc> instance () {
		return keep[TYPE<ByteProc>::expr] (ByteProcHolder::instance ()) ;
	}

	static BYTE bit_low (CREF<WORD> a) {
		return ByteProcHolder::hold (instance ())->bit_low (a) ;
	}

	static WORD bit_low (CREF<CHAR> a) {
		return ByteProcHolder::hold (instance ())->bit_low (a) ;
	}

	static CHAR bit_low (CREF<QUAD> a) {
		return ByteProcHolder::hold (instance ())->bit_low (a) ;
	}

	static BYTE bit_high (CREF<WORD> a) {
		return ByteProcHolder::hold (instance ())->bit_high (a) ;
	}

	static WORD bit_high (CREF<CHAR> a) {
		return ByteProcHolder::hold (instance ())->bit_high (a) ;
	}

	static CHAR bit_high (CREF<QUAD> a) {
		return ByteProcHolder::hold (instance ())->bit_high (a) ;
	}

	static WORD bit_merge (CREF<BYTE> high ,CREF<BYTE> low) {
		return ByteProcHolder::hold (instance ())->bit_merge (high ,low) ;
	}

	static CHAR bit_merge (CREF<WORD> high ,CREF<WORD> low) {
		return ByteProcHolder::hold (instance ())->bit_merge (high ,low) ;
	}

	static QUAD bit_merge (CREF<CHAR> high ,CREF<CHAR> low) {
		return ByteProcHolder::hold (instance ())->bit_merge (high ,low) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static BOOL bit_any (CREF<ARG1> curr ,CREF<ARG2> mask) {
		return ByteProcHolder::hold (instance ())->bit_any (curr ,ARG1 (mask)) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static BOOL bit_all (CREF<ARG1> curr ,CREF<ARG2> mask) {
		return ByteProcHolder::hold (instance ())->bit_all (curr ,ARG1 (mask)) ;
	}

	static BYTE bit_reverse (CREF<BYTE> a) {
		return ByteProcHolder::hold (instance ())->bit_reverse (a) ;
	}

	static WORD bit_reverse (CREF<WORD> a) {
		return ByteProcHolder::hold (instance ())->bit_reverse (a) ;
	}

	static CHAR bit_reverse (CREF<CHAR> a) {
		return ByteProcHolder::hold (instance ())->bit_reverse (a) ;
	}

	static QUAD bit_reverse (CREF<QUAD> a) {
		return ByteProcHolder::hold (instance ())->bit_reverse (a) ;
	}

	static INDEX bit_pow (CREF<LENGTH> nth) {
		return ByteProcHolder::hold (instance ())->bit_pow (nth) ;
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
} ;

struct IntegerHolder implement Interface {
	imports VFat<IntegerHolder> hold (VREF<IntegerLayout> that) ;
	imports CFat<IntegerHolder> hold (CREF<IntegerLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_ ,CREF<VAL64> item) = 0 ;
	virtual void initialize (CREF<IntegerLayout> that) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH precision () const = 0 ;
	virtual VAL64 fetch () const = 0 ;
	virtual void store (CREF<VAL64> item) = 0 ;
	virtual BOOL equal (CREF<IntegerLayout> that) const = 0 ;
	virtual FLAG compr (CREF<IntegerLayout> that) const = 0 ;
	virtual void visit (VREF<VisitorFriend> visitor) const = 0 ;
	virtual IntegerLayout sadd (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout ssub (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout smul (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout smul (CREF<VAL64> scale) const = 0 ;
	virtual IntegerLayout sdiv (CREF<VAL64> scale) const = 0 ;
	virtual IntegerLayout smod (CREF<VAL64> scale) const = 0 ;
	virtual IntegerLayout sabs () const = 0 ;
	virtual IntegerLayout minus () const = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
} ;

class Integer implement IntegerLayout {
protected:
	using IntegerLayout::mInteger ;

public:
	implicit Integer () = default ;

	explicit Integer (CREF<LENGTH> size_ ,CREF<VAL64> item) {
		IntegerHolder::hold (thiz)->initialize (size_ ,item) ;
	}

	implicit Integer (CREF<Integer> that) {
		IntegerHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<Integer> operator= (CREF<Integer> that) {
		return assign (thiz ,that) ;
	}

	implicit Integer (RREF<Integer> that) = default ;

	forceinline VREF<Integer> operator= (RREF<Integer> that) = default ;

	LENGTH size () const {
		return IntegerHolder::hold (thiz)->size () ;
	}

	LENGTH precision () const {
		return IntegerHolder::hold (thiz)->precision () ;
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

	void visit (VREF<VisitorFriend> visitor) const {
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

	Integer smul (CREF<VAL64> scale) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->smul (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator* (CREF<VAL64> scale) const {
		return smul (scale) ;
	}

	forceinline void operator*= (CREF<VAL64> scale) {
		thiz = smul (scale) ;
	}

	Integer sdiv (CREF<VAL64> scale) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->sdiv (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator/ (CREF<VAL64> scale) const {
		return sdiv (scale) ;
	}

	forceinline void operator/= (CREF<VAL64> scale) {
		thiz = sdiv (scale) ;
	}

	Integer smod (CREF<VAL64> scale) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->smod (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator% (CREF<VAL64> scale) const {
		return smod (scale) ;
	}

	forceinline void operator%= (CREF<VAL64> scale) {
		thiz = smod (scale) ;
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

struct HashProcLayout implement ThisLayout<RefLayout> {} ;

struct HashProcHolder implement Interface {
	imports CREF<HashProcLayout> instance () ;
	imports VFat<HashProcHolder> hold (VREF<HashProcLayout> that) ;
	imports CFat<HashProcHolder> hold (CREF<HashProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_) const = 0 ;
	virtual CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<CHAR> curr) const = 0 ;
	virtual QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_) const = 0 ;
	virtual QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<QUAD> curr) const = 0 ;
	virtual BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_) const = 0 ;
	virtual BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<BYTE> curr) const = 0 ;
	virtual WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_) const = 0 ;
	virtual WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<WORD> curr) const = 0 ;
} ;

class HashProc implement HashProcLayout {
public:
	static CREF<HashProc> instance () {
		return keep[TYPE<HashProc>::expr] (HashProcHolder::instance ()) ;
	}

	static CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_) {
		return HashProcHolder::hold (instance ())->fnvhash32 (src ,size_) ;
	}

	static CHAR fnvhash32 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<CHAR> curr) {
		return HashProcHolder::hold (instance ())->fnvhash32 (src ,size_ ,curr) ;
	}

	static QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_) {
		return HashProcHolder::hold (instance ())->fnvhash64 (src ,size_) ;
	}

	static QUAD fnvhash64 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<QUAD> curr) {
		return HashProcHolder::hold (instance ())->fnvhash64 (src ,size_ ,curr) ;
	}

	static BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_) {
		return HashProcHolder::hold (instance ())->crchash8 (src ,size_) ;
	}

	static BYTE crchash8 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<BYTE> curr) {
		return HashProcHolder::hold (instance ())->crchash8 (src ,size_ ,curr) ;
	}

	static WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_) {
		return HashProcHolder::hold (instance ())->crchash16 (src ,size_) ;
	}

	static WORD crchash16 (CREF<Pointer> src ,CREF<LENGTH> size_ ,CREF<WORD> curr) {
		return HashProcHolder::hold (instance ())->crchash16 (src ,size_ ,curr) ;
	}
} ;
} ;