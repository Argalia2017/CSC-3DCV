#pragma once

#ifndef __CSC_MATRIX__
#define __CSC_MATRIX__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_image.hpp"

namespace CSC {
struct Point2F {
	FLT32 mX ;
	FLT32 mY ;
} ;

struct Point3F {
	FLT32 mX ;
	FLT32 mY ;
	FLT32 mZ ;
} ;

struct Line2F {
	Point2F mMin ;
	Point2F mMax ;
} ;

struct Line3F {
	Point3F mMin ;
	Point3F mMax ;
} ;

template <class A>
class XYZProxy {
protected:
	XREF<A> mThat ;

public:
	implicit XYZProxy () = delete ;

	explicit XYZProxy (XREF<A> that) :mThat (that) {}

	forceinline operator Point2F () rightvalue {
		Point2F ret ;
		ret.mX = FLT32 (mThat.at (0)) ;
		ret.mY = FLT32 (mThat.at (1)) ;
		return move (ret) ;
	}

	forceinline operator Point3F () rightvalue {
		Point3F ret ;
		ret.mX = FLT32 (mThat.at (0)) ;
		ret.mY = FLT32 (mThat.at (1)) ;
		ret.mZ = FLT32 (mThat.at (2)) ;
		return move (ret) ;
	}
} ;

struct VectorLayout ;
struct MatrixLayout ;
struct QuaternionLayout ;

struct VectorLayout {
	Buffer<FLT64 ,RANK4> mVector ;
} ;

struct VectorHolder implement Interface {
	imports VFat<VectorHolder> hold (VREF<VectorLayout> that) ;
	imports CFat<VectorHolder> hold (CREF<VectorLayout> that) ;

	virtual void initialize (CREF<Buffer<FLT64 ,RANK4>> that) = 0 ;
	virtual void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) = 0 ;
	virtual void initialize (CREF<Pixel> point) = 0 ;
	virtual void initialize (CREF<Point2F> point) = 0 ;
	virtual void initialize (CREF<Point3F> point) = 0 ;
	virtual VREF<FLT64> at (CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<FLT64> at (CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<VectorLayout> that) const = 0 ;
	virtual FLAG compr (CREF<VectorLayout> that) const = 0 ;
	virtual void visit (VREF<VisitorFriend> visitor) const = 0 ;
	virtual VectorLayout sadd (CREF<VectorLayout> that) const = 0 ;
	virtual VectorLayout ssub (CREF<VectorLayout> that) const = 0 ;
	virtual VectorLayout smul (CREF<FLT64> scale) const = 0 ;
	virtual VectorLayout sdiv (CREF<FLT64> scale) const = 0 ;
	virtual FLT64 dot (CREF<VectorLayout> that) const = 0 ;
	virtual VectorLayout smul (CREF<MatrixLayout> that) const = 0 ;
	virtual VectorLayout cross (CREF<VectorLayout> that) const = 0 ;
	virtual VectorLayout sabs () const = 0 ;
	virtual VectorLayout minus () const = 0 ;
	virtual FLT64 magnitude () const = 0 ;
	virtual VectorLayout normalize () const = 0 ;
	virtual VectorLayout projection () const = 0 ;
	virtual VectorLayout homogenize () const = 0 ;
} ;

class Vector implement VectorLayout {
protected:
	using VectorLayout::mVector ;

public:
	implicit Vector () = default ;

	explicit Vector (CREF<Buffer<FLT64 ,RANK4>> that) {
		VectorHolder::hold (thiz)->initialize (that) ;
	}

	explicit Vector (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) {
		VectorHolder::hold (thiz)->initialize (x ,y ,z ,w) ;
	}

	explicit Vector (CREF<Pixel> point) {
		VectorHolder::hold (thiz)->initialize (point) ;
	}

	explicit Vector (CREF<Point2F> point) {
		VectorHolder::hold (thiz)->initialize (point) ;
	}

	explicit Vector (CREF<Point3F> point) {
		VectorHolder::hold (thiz)->initialize (point) ;
	}

	static CREF<Vector> zero () {
		return memorize ([&] () {
			return Vector (0 ,0 ,0 ,0) ;
		}) ;
	}

	static CREF<Vector> axis_x () {
		return memorize ([&] () {
			return Vector (1 ,0 ,0 ,0) ;
		}) ;
	}

	static CREF<Vector> axis_y () {
		return memorize ([&] () {
			return Vector (0 ,1 ,0 ,0) ;
		}) ;
	}

	static CREF<Vector> axis_z () {
		return memorize ([&] () {
			return Vector (0 ,0 ,1 ,0) ;
		}) ;
	}

	static CREF<Vector> axis_w () {
		return memorize ([&] () {
			return Vector (0 ,0 ,0 ,1) ;
		}) ;
	}

	XYZProxy<CREF<Vector>> xyz () const leftvalue {
		return XYZProxy<CREF<Vector>> (thiz) ;
	}

	VREF<FLT64> at (CREF<INDEX> y) leftvalue {
		return VectorHolder::hold (thiz)->at (y) ;
	}

	forceinline VREF<FLT64> operator[] (CREF<INDEX> y) leftvalue {
		return at (y) ;
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue {
		return VectorHolder::hold (thiz)->at (y) ;
	}

	forceinline CREF<FLT64> operator[] (CREF<INDEX> y) const leftvalue {
		return at (y) ;
	}

	BOOL equal (CREF<Vector> that) const {
		return VectorHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Vector> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Vector> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Vector> that) const {
		return VectorHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Vector> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Vector> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Vector> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Vector> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<VisitorFriend> visitor) const {
		return VectorHolder::hold (thiz)->visit (visitor) ;
	}

	Vector sadd (CREF<Vector> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator+ (CREF<Vector> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CREF<Vector> that) {
		thiz = sadd (that) ;
	}

	Vector ssub (CREF<Vector> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator- (CREF<Vector> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CREF<Vector> that) {
		thiz = ssub (that) ;
	}

	Vector smul (CREF<FLT64> scale) const {
		VectorLayout ret = VectorHolder::hold (thiz)->smul (scale) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator* (CREF<FLT64> scale) const {
		return smul (scale) ;
	}

	forceinline friend Vector operator* (CREF<FLT64> scale ,CREF<Vector> that) {
		return that.smul (scale) ;
	}

	forceinline void operator*= (CREF<FLT64> scale) {
		thiz = smul (scale) ;
	}

	Vector sdiv (CREF<FLT64> scale) const {
		VectorLayout ret = VectorHolder::hold (thiz)->sdiv (scale) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator/ (CREF<FLT64> scale) const {
		return sdiv (scale) ;
	}

	forceinline void operator/= (CREF<FLT64> scale) {
		thiz = sdiv (scale) ;
	}

	FLT64 dot (CREF<Vector> that) const {
		return VectorHolder::hold (thiz)->dot (that) ;
	}

	forceinline FLT64 operator* (CREF<Vector> that) const {
		return dot (that) ;
	}

	Vector smul (CREF<MatrixLayout> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator* (CREF<MatrixLayout> that) const {
		return smul (that) ;
	}

	Vector cross (CREF<Vector> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->cross (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator^ (CREF<Vector> that) const {
		return cross (that) ;
	}

	forceinline void operator^= (CREF<Vector> that) {
		thiz = cross (that) ;
	}

	Vector sabs () const {
		VectorLayout ret = VectorHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Vector minus () const {
		VectorLayout ret = VectorHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator- () const {
		return minus () ;
	}

	FLT64 magnitude () const {
		return VectorHolder::hold (thiz)->magnitude () ;
	}

	Vector normalize () const {
		VectorLayout ret = VectorHolder::hold (thiz)->normalize () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Vector projection () const {
		VectorLayout ret = VectorHolder::hold (thiz)->projection () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Vector homogenize () const {
		VectorLayout ret = VectorHolder::hold (thiz)->homogenize () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}
} ;

struct MatrixLayout {
	Buffer<FLT64 ,ENUM<16>> mMatrix ;
} ;

struct MatrixHolder implement Interface {
	imports VFat<MatrixHolder> hold (VREF<MatrixLayout> that) ;
	imports CFat<MatrixHolder> hold (CREF<MatrixLayout> that) ;

	virtual void initialize (CREF<Buffer<FLT64 ,ENUM<16>>> that) = 0 ;
	virtual void initialize (CREF<VectorLayout> x ,CREF<VectorLayout> y ,CREF<VectorLayout> z ,CREF<VectorLayout> w) = 0 ;
	virtual VREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<MatrixLayout> that) const = 0 ;
	virtual FLAG compr (CREF<MatrixLayout> that) const = 0 ;
	virtual void visit (VREF<VisitorFriend> visitor) const = 0 ;
	virtual MatrixLayout sadd (CREF<MatrixLayout> that) const = 0 ;
	virtual MatrixLayout ssub (CREF<MatrixLayout> that) const = 0 ;
	virtual MatrixLayout smul (CREF<FLT64> scale) const = 0 ;
	virtual MatrixLayout sdiv (CREF<FLT64> scale) const = 0 ;
	virtual VectorLayout smul (CREF<VectorLayout> that) const = 0 ;
	virtual MatrixLayout smul (CREF<MatrixLayout> that) const = 0 ;
	virtual MatrixLayout sabs () const = 0 ;
	virtual MatrixLayout minus () const = 0 ;
	virtual MatrixLayout transpose () const = 0 ;
	virtual MatrixLayout triangular () const = 0 ;
	virtual MatrixLayout homogenize () const = 0 ;
	virtual FLT64 det () const = 0 ;
	virtual MatrixLayout adjoint () const = 0 ;
	virtual MatrixLayout inverse () const = 0 ;
	virtual FLT64 trace () const = 0 ;
} ;

class Matrix implement MatrixLayout {
protected:
	using MatrixLayout::mMatrix ;

public:
	implicit Matrix () = default ;

	explicit Matrix (CREF<Buffer<FLT64 ,ENUM<16>>> that) {
		MatrixHolder::hold (thiz)->initialize (that) ;
	}

	explicit Matrix (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z ,CREF<Vector> w) {
		MatrixHolder::hold (thiz)->initialize (x ,y ,z ,w) ;
	}

	static CREF<Matrix> zero () {
		return memorize ([&] () {
			const auto r1x = Vector::zero () ;
			return Matrix (r1x ,r1x ,r1x ,r1x) ;
		}) ;
	}

	static CREF<Matrix> identity () {
		return memorize ([&] () {
			const auto r1x = Vector::axis_x () ;
			const auto r2x = Vector::axis_y () ;
			const auto r3x = Vector::axis_z () ;
			const auto r4x = Vector::axis_w () ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}) ;
	}

	static CREF<Matrix> axis_w () {
		return memorize ([&] () {
			const auto r1x = Vector::zero () ;
			const auto r2x = Vector::axis_w () ;
			return Matrix (r1x ,r1x ,r1x ,r2x) ;
		}) ;
	}

	VREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
		return MatrixHolder::hold (thiz)->at (x ,y) ;
	}

	VREF<FLT64> at (CREF<Pixel> index) leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline VREF<FLT64> operator[] (CREF<Pixel> index) leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<VREF<Matrix>> operator[] (CREF<INDEX> y) leftvalue {
		return RowProxy<VREF<Matrix>> (thiz ,y) ;
	}

	CREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
		return MatrixHolder::hold (thiz)->at (x ,y) ;
	}

	CREF<FLT64> at (CREF<Pixel> index) const leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline CREF<FLT64> operator[] (CREF<Pixel> index) const leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<CREF<Matrix>> operator[] (CREF<INDEX> y) const leftvalue {
		return RowProxy<CREF<Matrix>> (thiz ,y) ;
	}

	BOOL equal (CREF<Matrix> that) const {
		return MatrixHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Matrix> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Matrix> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Matrix> that) const {
		return MatrixHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Matrix> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Matrix> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Matrix> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Matrix> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<VisitorFriend> visitor) const {
		return MatrixHolder::hold (thiz)->visit (visitor) ;
	}

	Matrix sadd (CREF<Matrix> that) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator+ (CREF<Matrix> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CREF<Matrix> that) {
		thiz = sadd (that) ;
	}

	Matrix ssub (CREF<Matrix> that) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator- (CREF<Matrix> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CREF<Matrix> that) {
		thiz = ssub (that) ;
	}

	Matrix smul (CREF<FLT64> scale) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->smul (scale) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator* (CREF<FLT64> scale) const {
		return smul (scale) ;
	}

	forceinline friend Matrix operator* (CREF<FLT64> scale ,CREF<Matrix> that) {
		return that.smul (scale) ;
	}

	forceinline void operator*= (CREF<FLT64> scale) {
		thiz = smul (scale) ;
	}

	Matrix sdiv (CREF<FLT64> scale) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->sdiv (scale) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator/ (CREF<FLT64> scale) const {
		return sdiv (scale) ;
	}

	forceinline void operator/= (CREF<FLT64> scale) {
		thiz = sdiv (scale) ;
	}

	Vector smul (CREF<Vector> that) const {
		VectorLayout ret = MatrixHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator* (CREF<Vector> that) const {
		return smul (that) ;
	}

	Matrix smul (CREF<Matrix> that) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator* (CREF<Matrix> that) const {
		return smul (that) ;
	}

	forceinline void operator*= (CREF<Matrix> that) {
		thiz = smul (that) ;
	}

	Matrix sabs () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix minus () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator- () const {
		return minus () ;
	}

	Matrix transpose () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->transpose () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix triangular () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->triangular () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix homogenize () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->homogenize () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	FLT64 det () const {
		return MatrixHolder::hold (thiz)->det () ;
	}

	Matrix adjoint () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->adjoint () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix inverse () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->inverse () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	FLT64 trace () const {
		return MatrixHolder::hold (thiz)->trace () ;
	}
} ;

struct ViewMatrixOption {
	enum {
		XYZ ,
		XZY ,
		YXZ ,
		YZX ,
		ZXY ,
		ZYX ,
		ETC
	} ;
} ;

struct MakeMatrixHolder implement Interface {
	imports VFat<MakeMatrixHolder> hold (VREF<MatrixLayout> that) ;
	imports CFat<MakeMatrixHolder> hold (CREF<MatrixLayout> that) ;

	virtual void make_DiagMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) = 0 ;
	virtual void make_ShearMatrix (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) = 0 ;
	virtual void make_RotationMatrix (CREF<Vector> normal ,CREF<FLT64> angle) = 0 ;
	virtual void make_TranslationMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) = 0 ;
	virtual void make_PerspectiveMatrix (CREF<FLT64> fx ,CREF<FLT64> fy ,CREF<FLT64> wx ,CREF<FLT64> wy) = 0 ;
	virtual void make_ProjectionMatrix (CREF<Vector> normal ,CREF<Vector> center ,CREF<Vector> light) = 0 ;
	virtual void make_ViewMatrix (CREF<Vector> vx ,CREF<Vector> vy) = 0 ;
	virtual void make_ViewMatrix (CREF<Vector> vx ,CREF<Vector> vy ,CREF<Just<ViewMatrixOption>> option) = 0 ;
	virtual void make_CrossProductMatrix (CREF<Vector> xyz) = 0 ;
	virtual void make_SymmetryMatrix (CREF<Vector> x ,CREF<Vector> y) = 0 ;
	virtual void make_AffineMatrix (CREF<Array<FLT64>> a) = 0 ;
} ;

inline Matrix DiagMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_DiagMatrix (x ,y ,z ,1) ;
	return move (ret) ;
}

inline Matrix DiagMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_DiagMatrix (x ,y ,z ,w) ;
	return move (ret) ;
}

inline Matrix ShearMatrix (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ShearMatrix (x ,y ,z) ;
	return move (ret) ;
}

inline Matrix RotationMatrix (CREF<Vector> normal ,CREF<FLT64> angle) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_RotationMatrix (normal ,angle) ;
	return move (ret) ;
}

inline Matrix TranslationMatrix (CREF<Vector> xyz) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_TranslationMatrix (xyz[0] ,xyz[1] ,xyz[2]) ;
	return move (ret) ;
}

inline Matrix TranslationMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_TranslationMatrix (x ,y ,z) ;
	return move (ret) ;
}

inline Matrix PerspectiveMatrix (CREF<FLT64> fx ,CREF<FLT64> fy ,CREF<FLT64> wx ,CREF<FLT64> wy) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_PerspectiveMatrix (fx ,fy ,wx ,wy) ;
	return move (ret) ;
}

inline Matrix ProjectionMatrix (CREF<Vector> normal ,CREF<Vector> center ,CREF<Vector> light) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ProjectionMatrix (normal ,center ,light) ;
	return move (ret) ;
}

inline Matrix ViewMatrixXYZ (CREF<Vector> x ,CREF<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (x ,y ,ViewMatrixOption::XYZ) ;
	return move (ret) ;
}

inline Matrix ViewMatrixXZY (CREF<Vector> x ,CREF<Vector> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (x ,z ,ViewMatrixOption::XZY) ;
	return move (ret) ;
}

inline Matrix ViewMatrixYXZ (CREF<Vector> y ,CREF<Vector> x) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (y ,x ,ViewMatrixOption::YXZ) ;
	return move (ret) ;
}

inline Matrix ViewMatrixYZX (CREF<Vector> y ,CREF<Vector> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (y ,z ,ViewMatrixOption::YZX) ;
	return move (ret) ;
}

inline Matrix ViewMatrixZXY (CREF<Vector> z ,CREF<Vector> x) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (z ,x ,ViewMatrixOption::ZXY) ;
	return move (ret) ;
}

inline Matrix ViewMatrixZYX (CREF<Vector> z ,CREF<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (z ,y ,ViewMatrixOption::ZYX) ;
	return move (ret) ;
}

inline Matrix CrossProductMatrix (CREF<Vector> xyz) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_CrossProductMatrix (xyz) ;
	return move (ret) ;
}

inline Matrix SymmetryMatrix (CREF<Vector> x ,CREF<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_SymmetryMatrix (x ,y) ;
	return move (ret) ;
}

inline Matrix AffineMatrix (CREF<Array<FLT64>> a) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_AffineMatrix (a) ;
	return move (ret) ;
}

struct TRSResult {
	Matrix mR ;
	Matrix mT ;
	Matrix mS ;
} ;

struct KRTResult {
	Matrix mK ;
	Matrix mR ;
	Matrix mT ;
} ;

struct SVDResult {
	Matrix mU ;
	Matrix mS ;
	Matrix mV ;
} ;

struct MatrixProcLayout implement ThisLayout<RefLayout> {} ;

struct MatrixProcHolder implement Interface {
	imports CREF<MatrixProcLayout> instance () ;
	imports VFat<MatrixProcHolder> hold (VREF<MatrixProcLayout> that) ;
	imports CFat<MatrixProcHolder> hold (CREF<MatrixProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual TRSResult solve_trs (CREF<Matrix> a) const = 0 ;
	virtual KRTResult solve_krt (CREF<Matrix> a) const = 0 ;
	virtual SVDResult solve_svd (CREF<Matrix> a) const = 0 ;
} ;

class MatrixProc implement MatrixProcLayout {
protected:
	using MatrixProcLayout::mThis ;

public:
	static CREF<MatrixProc> instance () {
		return keep[TYPE<MatrixProc>::expr] (MatrixProcHolder::instance ()) ;
	}

	static TRSResult solve_trs (CREF<Matrix> a) {
		return MatrixProcHolder::hold (instance ())->solve_trs (a) ;
	}

	static KRTResult solve_krt (CREF<Matrix> a) {
		return MatrixProcHolder::hold (instance ())->solve_krt (a) ;
	}

	static SVDResult solve_svd (CREF<Matrix> a) {
		return MatrixProcHolder::hold (instance ())->solve_svd (a) ;
	}
} ;

struct DuplexMatrixLayout {
	Buffer2<Matrix> mDuplexMatrix ;
} ;

struct DuplexMatrixHolder implement Interface {
	imports VFat<DuplexMatrixHolder> hold (VREF<DuplexMatrixLayout> that) ;
	imports CFat<DuplexMatrixHolder> hold (CREF<DuplexMatrixLayout> that) ;

	virtual void initialize (CREF<Matrix> that) = 0 ;
	virtual DuplexMatrixLayout inverse () const = 0 ;
} ;

class DuplexMatrix implement DuplexMatrixLayout {
protected:
	using DuplexMatrixLayout::mDuplexMatrix ;

public:
	implicit DuplexMatrix () = default ;

	implicit DuplexMatrix (CREF<Matrix> that) {
		DuplexMatrixHolder::hold (thiz)->initialize (that) ;
	}

	CREF<Matrix> at (CREF<INDEX> index) const leftvalue {
		return mDuplexMatrix[index] ;
	}

	forceinline CREF<Matrix> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	DuplexMatrix inverse () const {
		DuplexMatrixLayout ret = DuplexMatrixHolder::hold (thiz)->inverse () ;
		return move (keep[TYPE<DuplexMatrix>::expr] (ret)) ;
	}
} ;

struct QuaternionLayout {
	Buffer<FLT64 ,RANK4> mQuaternion ;
} ;

struct QuaternionHolder implement Interface {
	imports VFat<QuaternionHolder> hold (VREF<QuaternionLayout> that) ;
	imports CFat<QuaternionHolder> hold (CREF<QuaternionLayout> that) ;

	virtual void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) = 0 ;
	virtual void initialize (CREF<VectorLayout> that) = 0 ;
	virtual void initialize (CREF<MatrixLayout> that) = 0 ;
	virtual CREF<FLT64> at (CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<QuaternionLayout> that) const = 0 ;
	virtual FLAG compr (CREF<QuaternionLayout> that) const = 0 ;
	virtual void visit (VREF<VisitorFriend> visitor) const = 0 ;
	virtual QuaternionLayout smul (CREF<QuaternionLayout> that) const = 0 ;
	virtual VectorLayout vector () const = 0 ;
	virtual MatrixLayout matrix () const = 0 ;
} ;

class Quaternion implement QuaternionLayout {
protected:
	using QuaternionLayout::mQuaternion ;

public:
	implicit Quaternion () = default ;

	explicit Quaternion (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) {
		QuaternionHolder::hold (thiz)->initialize (x ,y ,z ,w) ;
	}

	explicit Quaternion (CREF<Vector> that) {
		QuaternionHolder::hold (thiz)->initialize (that) ;
	}

	explicit Quaternion (CREF<Matrix> that) {
		QuaternionHolder::hold (thiz)->initialize (that) ;
	}

	static CREF<Quaternion> identity () {
		return memorize ([&] () {
			return Quaternion (0 ,0 ,0 ,1) ;
		}) ;
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue {
		return QuaternionHolder::hold (thiz)->at (y) ;
	}

	forceinline CREF<FLT64> operator[] (CREF<INDEX> y) const leftvalue {
		return at (y) ;
	}

	BOOL equal (CREF<Quaternion> that) const {
		return QuaternionHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Quaternion> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Quaternion> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Quaternion> that) const {
		return QuaternionHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Quaternion> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Quaternion> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Quaternion> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Quaternion> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<VisitorFriend> visitor) const {
		return QuaternionHolder::hold (thiz)->visit (visitor) ;
	}

	Quaternion smul (CREF<Quaternion> that) const {
		QuaternionLayout ret = QuaternionHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Quaternion>::expr] (ret)) ;
	}

	forceinline Quaternion operator* (CREF<Quaternion> that) const {
		return smul (that) ;
	}

	forceinline void operator*= (CREF<Quaternion> that) {
		thiz = smul (that) ;
	}

	Vector vector () const {
		VectorLayout ret = QuaternionHolder::hold (thiz)->vector () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Matrix matrix () const {
		MatrixLayout ret = QuaternionHolder::hold (thiz)->matrix () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}
} ;

struct LinearProcLayout implement ThisLayout<RefLayout> {} ;

struct LinearProcHolder implement Interface {
	imports CREF<LinearProcLayout> instance () ;
	imports VFat<LinearProcHolder> hold (VREF<LinearProcLayout> that) ;
	imports CFat<LinearProcHolder> hold (CREF<LinearProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Image<FLT64> solve_lsm (CREF<Image<FLT64>> a) const = 0 ;
	virtual Image<FLT64> solve_lsm (CREF<Image<FLT64>> a ,CREF<Image<FLT64>> b) const = 0 ;
	virtual Image<FLT64> solve_inv (CREF<Image<FLT64>> a) const = 0 ;
} ;

class LinearProc implement LinearProcLayout {
protected:
	using LinearProcLayout::mThis ;

public:
	static CREF<LinearProc> instance () {
		return keep[TYPE<LinearProc>::expr] (LinearProcHolder::instance ()) ;
	}

	static Image<FLT64> solve_lsm (CREF<Image<FLT64>> a) {
		return LinearProcHolder::hold (instance ())->solve_lsm (a) ;
	}

	static Image<FLT64> solve_lsm (CREF<Image<FLT64>> a ,CREF<Image<FLT64>> b) {
		return LinearProcHolder::hold (instance ())->solve_lsm (a ,b) ;
	}

	static Image<FLT64> solve_inv (CREF<Image<FLT64>> a) {
		return LinearProcHolder::hold (instance ())->solve_inv (a) ;
	}
} ;

struct PointCloudLayout {
	LENGTH mRank ;
	Ref<Array<Pointer>> mPointCloud ;
	Matrix mWorld ;
} ;

struct PointCloudHolder implement Interface {
	imports VFat<PointCloudHolder> hold (VREF<PointCloudLayout> that) ;
	imports CFat<PointCloudHolder> hold (CREF<PointCloudLayout> that) ;

	virtual void initialize (RREF<Ref<Array<Point2F>>> that) = 0 ;
	virtual void initialize (RREF<Ref<Array<Point3F>>> that) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<Vector> item) const = 0 ;
	virtual Vector center () const = 0 ;
	virtual Matrix svd_matrix () const = 0 ;
	virtual Matrix box_matrix () const = 0 ;
	virtual Line3F bound () const = 0 ;
	virtual PointCloudLayout smul (CREF<Matrix> mat) const = 0 ;
	virtual Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor) const = 0 ;
	virtual Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor ,CREF<FLT64> radius) const = 0 ;
} ;

class PointCloud implement PointCloudLayout {
protected:
	using PointCloudLayout::mRank ;
	using PointCloudLayout::mPointCloud ;
	using PointCloudLayout::mWorld ;

public:
	implicit PointCloud () = default ;

	explicit PointCloud (CREF<Array<Point2F>> that) {
		PointCloudHolder::hold (thiz)->initialize (Ref<Array<Point2F>>::make (move (that))) ;
	}

	explicit PointCloud (CREF<Array<Point3F>> that) {
		PointCloudHolder::hold (thiz)->initialize (Ref<Array<Point3F>>::make (move (that))) ;
	}

	explicit PointCloud (RREF<Ref<Array<Point2F>>> that) {
		PointCloudHolder::hold (thiz)->initialize (move (that)) ;
	}

	explicit PointCloud (RREF<Ref<Array<Point3F>>> that) {
		PointCloudHolder::hold (thiz)->initialize (move (that)) ;
	}

	LENGTH size () const {
		return PointCloudHolder::hold (thiz)->size () ;
	}

	void get (CREF<INDEX> index ,VREF<Vector> item) const {
		return PointCloudHolder::hold (thiz)->get (index ,item) ;
	}

	forceinline Vector operator[] (CREF<INDEX> index) const {
		Vector ret ;
		get (index ,ret) ;
		return move (ret) ;
	}

	Vector center () const {
		return PointCloudHolder::hold (thiz)->center () ;
	}

	Matrix svd_matrix () const {
		return PointCloudHolder::hold (thiz)->svd_matrix () ;
	}

	Matrix box_matrix () const {
		return PointCloudHolder::hold (thiz)->box_matrix () ;
	}

	Line3F bound () const {
		return PointCloudHolder::hold (thiz)->bound () ;
	}

	PointCloud smul (CREF<Matrix> mat) const {
		PointCloudLayout ret = PointCloudHolder::hold (thiz)->smul (mat) ;
		return move (keep[TYPE<PointCloud>::expr] (ret)) ;
	}

	forceinline PointCloud operator* (CREF<Matrix> mat) const {
		return smul (mat) ;
	}

	forceinline friend PointCloud operator* (CREF<Matrix> mat ,CREF<PointCloud> that) {
		return that.smul (mat.transpose ()) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor) const {
		return PointCloudHolder::hold (thiz)->search (center ,neighbor) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor ,CREF<FLT64> radius) const {
		return PointCloudHolder::hold (thiz)->search (center ,neighbor ,radius) ;
	}
} ;
} ;