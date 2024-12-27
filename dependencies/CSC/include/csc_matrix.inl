#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_matrix.hpp"

namespace CSC {
class VectorImplHolder final implement Fat<VectorHolder ,VectorLayout> {
public:
	void initialize (CREF<Buffer<FLT64 ,RANK4>> that) override {
		fake.mVector[0] = that[0] ;
		fake.mVector[1] = that[1] ;
		fake.mVector[2] = that[2] ;
		fake.mVector[3] = that[3] ;
	}

	void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) override {
		fake.mVector[0] = x ;
		fake.mVector[1] = y ;
		fake.mVector[2] = z ;
		fake.mVector[3] = w ;
	}

	void initialize (CREF<Pixel> point) override {
		fake.mVector[0] = FLT64 (point.mX) + FLT64 (0.5) ;
		fake.mVector[1] = FLT64 (point.mY) + FLT64 (0.5) ;
		fake.mVector[2] = 0 ;
		fake.mVector[3] = 1 ;
	}

	void initialize (CREF<Point2F> point) override {
		fake.mVector[0] = FLT64 (point.mX) ;
		fake.mVector[1] = FLT64 (point.mY) ;
		fake.mVector[2] = 0 ;
		fake.mVector[3] = 1 ;
	}

	void initialize (CREF<Point3F> point) override {
		fake.mVector[0] = FLT64 (point.mX) ;
		fake.mVector[1] = FLT64 (point.mY) ;
		fake.mVector[2] = FLT64 (point.mZ) ;
		fake.mVector[3] = 1 ;
	}

	VREF<FLT64> at (CREF<INDEX> y) leftvalue override {
		return fake.mVector[y] ;
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue override {
		return fake.mVector[y] ;
	}

	static INDEX mm (CREF<INDEX> x ,CREF<INDEX> y) {
		return x + y * 4 ;
	}

	BOOL equal (CREF<VectorLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_equal (fake.mVector[i] ,that.mVector[i]) ;
			if (!r1x)
				return r1x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<VectorLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_compr (fake.mVector[i] ,that.mVector[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorFriend> visitor) const override {
		visitor.enter () ;
		for (auto &&i : iter (0 ,4)) {
			inline_visit (visitor ,fake.mVector[i]) ;
		}
		visitor.leave () ;
	}

	VectorLayout sadd (CREF<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] + that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout ssub (CREF<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] - that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout smul (CREF<FLT64> scale) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] * scale ;
		}
		return move (ret) ;
	}

	VectorLayout sdiv (CREF<FLT64> scale) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] / scale ;
		}
		return move (ret) ;
	}

	FLT64 dot (CREF<VectorLayout> that) const override {
		FLT64 ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += fake.mVector[i] * that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout smul (CREF<MatrixLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			auto rax = FLT64 (0) ;
			rax += fake.mVector[0] * that.mMatrix[mm (i ,0)] ;
			rax += fake.mVector[1] * that.mMatrix[mm (i ,1)] ;
			rax += fake.mVector[2] * that.mMatrix[mm (i ,2)] ;
			rax += fake.mVector[3] * that.mMatrix[mm (i ,3)] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	VectorLayout cross (CREF<VectorLayout> that) const override {
		assert (fake.mVector[3] == 0) ;
		assert (that.mVector[3] == 0) ;
		VectorLayout ret ;
		ret.mVector[0] = fake.mVector[1] * that.mVector[2] - fake.mVector[2] * that.mVector[1] ;
		ret.mVector[1] = fake.mVector[2] * that.mVector[0] - fake.mVector[0] * that.mVector[2] ;
		ret.mVector[2] = fake.mVector[0] * that.mVector[1] - fake.mVector[1] * that.mVector[0] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	VectorLayout sabs () const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = MathProc::abs (fake.mVector[i]) ;
		}
		return move (ret) ;
	}

	VectorLayout minus () const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = -fake.mVector[i] ;
		}
		return move (ret) ;
	}

	FLT64 magnitude () const override {
		assert (fake.mVector[3] == 0) ;
		FLT64 ret = 0 ;
		ret += MathProc::square (fake.mVector[0]) ;
		ret += MathProc::square (fake.mVector[1]) ;
		ret += MathProc::square (fake.mVector[2]) ;
		ret = MathProc::sqrt (ret) ;
		return move (ret) ;
	}

	VectorLayout normalize () const override {
		VectorLayout ret ;
		const auto r1x = MathProc::inverse (magnitude ()) ;
		ret.mVector[0] = fake.mVector[0] * r1x ;
		ret.mVector[1] = fake.mVector[1] * r1x ;
		ret.mVector[2] = fake.mVector[2] * r1x ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	VectorLayout projection () const override {
		VectorLayout ret ;
		const auto r1x = MathProc::inverse (fake.mVector[3]) ;
		ret.mVector[0] = fake.mVector[0] * r1x ;
		ret.mVector[1] = fake.mVector[1] * r1x ;
		ret.mVector[2] = fake.mVector[2] * r1x ;
		ret.mVector[3] = 1 ;
		return move (ret) ;
	}

	VectorLayout homogenize () const override {
		VectorLayout ret ;
		ret.mVector[0] = fake.mVector[0] ;
		ret.mVector[1] = fake.mVector[1] ;
		ret.mVector[2] = fake.mVector[2] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}
} ;

exports VFat<VectorHolder> VectorHolder::hold (VREF<VectorLayout> that) {
	return VFat<VectorHolder> (VectorImplHolder () ,that) ;
}

exports CFat<VectorHolder> VectorHolder::hold (CREF<VectorLayout> that) {
	return CFat<VectorHolder> (VectorImplHolder () ,that) ;
}

class MatrixImplHolder final implement Fat<MatrixHolder ,MatrixLayout> {
public:
	void initialize (CREF<Buffer<FLT64 ,ENUM<16>>> that) override {
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			INDEX ix = i.mY * 4 + i.mX ;
			fake.mMatrix[mm (i.mX ,i.mY)] = that[ix] ;
		}
	}

	void initialize (CREF<VectorLayout> x ,CREF<VectorLayout> y ,CREF<VectorLayout> z ,CREF<VectorLayout> w) override {
		for (auto &&i : iter (0 ,4)) {
			fake.mMatrix[mm (0 ,i)] = x.mVector[i] ;
			fake.mMatrix[mm (1 ,i)] = y.mVector[i] ;
			fake.mMatrix[mm (2 ,i)] = z.mVector[i] ;
			fake.mMatrix[mm (3 ,i)] = w.mVector[i] ;
		}
	}

	VREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue override {
		assert (inline_between (x ,0 ,4)) ;
		assert (inline_between (y ,0 ,4)) ;
		return fake.mMatrix[mm (x ,y)] ;
	}

	CREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (inline_between (x ,0 ,4)) ;
		assert (inline_between (y ,0 ,4)) ;
		return fake.mMatrix[mm (x ,y)] ;
	}

	static INDEX mm (CREF<INDEX> x ,CREF<INDEX> y) {
		return x + y * 4 ;
	}

	BOOL equal (CREF<MatrixLayout> that) const override {
		for (auto &&i : iter (0 ,16)) {
			const auto r1x = inline_equal (fake.mMatrix[i] ,that.mMatrix[i]) ;
			if (!r1x)
				return r1x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<MatrixLayout> that) const override {
		for (auto &&i : iter (0 ,16)) {
			const auto r1x = inline_compr (fake.mMatrix[i] ,that.mMatrix[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorFriend> visitor) const override {
		visitor.enter () ;
		for (auto &&i : iter (0 ,16)) {
			inline_visit (visitor ,fake.mMatrix[i]) ;
		}
		visitor.leave () ;
	}

	MatrixLayout sadd (CREF<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] + that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout ssub (CREF<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] - that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout smul (CREF<FLT64> scale) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] * scale ;
		}
		return move (ret) ;
	}

	MatrixLayout sdiv (CREF<FLT64> scale) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] / scale ;
		}
		return move (ret) ;
	}

	VectorLayout smul (CREF<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			auto rax = FLT64 (0) ;
			rax += fake.mMatrix[mm (0 ,i)] * that.mVector[0] ;
			rax += fake.mMatrix[mm (1 ,i)] * that.mVector[1] ;
			rax += fake.mMatrix[mm (2 ,i)] * that.mVector[2] ;
			rax += fake.mMatrix[mm (3 ,i)] * that.mVector[3] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout smul (CREF<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			auto rax = FLT64 (0) ;
			rax += fake.mMatrix[mm (0 ,i.mY)] * that.mMatrix[mm (i.mX ,0)] ;
			rax += fake.mMatrix[mm (1 ,i.mY)] * that.mMatrix[mm (i.mX ,1)] ;
			rax += fake.mMatrix[mm (2 ,i.mY)] * that.mMatrix[mm (i.mX ,2)] ;
			rax += fake.mMatrix[mm (3 ,i.mY)] * that.mMatrix[mm (i.mX ,3)] ;
			ret.mMatrix[mm (i.mX ,i.mY)] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout sabs () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = MathProc::abs (fake.mMatrix[i]) ;
		}
		return move (ret) ;
	}

	MatrixLayout minus () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = -fake.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout transpose () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			ret.mMatrix[mm (i.mY ,i.mX)] = fake.mMatrix[mm (i.mX ,i.mY)] ;
		}
		return move (ret) ;
	}

	MatrixLayout triangular () const override {
		MatrixLayout ret = fake ;
		for (auto &&i : iter (0 ,4)) {
			if ifdo (TRUE) {
				INDEX ix = find_abs_max_row (ret ,i) ;
				if (ix == i)
					discard ;
				for (auto &&j : iter (i ,4)) {
					const auto r1x = -ret.mMatrix[mm (j ,i)] ;
					ret.mMatrix[mm (j ,i)] = ret.mMatrix[mm (j ,ix)] ;
					ret.mMatrix[mm (j ,ix)] = r1x ;
				}
			}
			const auto r2x = MathProc::inverse (ret.mMatrix[mm (i ,i)]) ;
			if (r2x == 0)
				continue ;
			for (auto &&j : iter (i + 1 ,4)) {
				const auto r3x = ret.mMatrix[mm (i ,j)] * r2x ;
				for (auto &&k : iter (i + 1 ,4))
					ret.mMatrix[mm (k ,j)] -= r3x * ret.mMatrix[mm (k ,i)] ;
				ret.mMatrix[mm (i ,j)] = 0 ;
			}
		}
		return move (ret) ;
	}

	INDEX find_abs_max_row (CREF<MatrixLayout> that ,CREF<INDEX> x) const {
		INDEX ret = NONE ;
		auto rax = FLT64 () ;
		for (auto &&i : iter (x ,4)) {
			const auto r1x = MathProc::abs (that.mMatrix[mm (x ,i)]) ;
			if (ret != NONE)
				if (rax >= r1x)
					continue ;
			ret = i ;
			rax = r1x ;
		}
		return move (ret) ;
	}

	MatrixLayout homogenize () const override {
		MatrixLayout ret = Matrix::zero () ;
		for (auto &&i : iter (0 ,3 ,0 ,3)) {
			ret.mMatrix[mm (i.mX ,i.mY)] = fake.mMatrix[mm (i.mX ,i.mY)] ;
		}
		return move (ret) ;
	}

	FLT64 det () const override {
		const auto r1x = triangular () ;
		FLT64 ret = 1 ;
		for (auto &&i : iter (0 ,4))
			ret *= r1x.mMatrix[mm (i ,i)] ;
		return move (ret) ;
	}

	MatrixLayout adjoint () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			INDEX ix = LENGTH (i.mY == 0) ;
			INDEX iy = LENGTH (i.mY == 1) + ix + 1 ;
			INDEX iz = LENGTH (i.mY == 2) + iy + 1 ;
			INDEX jx = LENGTH (i.mX == 0) ;
			INDEX jy = LENGTH (i.mX == 1) + jx + 1 ;
			INDEX jz = LENGTH (i.mX == 2) + jy + 1 ;
			const auto r1x = fake.mMatrix[mm (jy ,iy)] * fake.mMatrix[mm (jz ,iz)] - fake.mMatrix[mm (jz ,iy)] * fake.mMatrix[mm (jy ,iz)] ;
			const auto r2x = fake.mMatrix[mm (jy ,ix)] * fake.mMatrix[mm (jz ,iz)] - fake.mMatrix[mm (jz ,ix)] * fake.mMatrix[mm (jy ,iz)] ;
			const auto r3x = fake.mMatrix[mm (jy ,ix)] * fake.mMatrix[mm (jz ,iy)] - fake.mMatrix[mm (jz ,ix)] * fake.mMatrix[mm (jy ,iy)] ;
			const auto r4x = fake.mMatrix[mm (jx ,ix)] * r1x ;
			const auto r5x = fake.mMatrix[mm (jx ,iy)] * r2x ;
			const auto r6x = fake.mMatrix[mm (jx ,iz)] * r3x ;
			const auto r7x = (r4x - r5x + r6x) * adjoint_sign (i.mY + i.mX) ;
			ret.mMatrix[mm (i.mY ,i.mX)] = r7x ;
		}
		return move (ret) ;
	}

	FLT64 adjoint_sign (CREF<INDEX> index) const {
		if (index % 2 == 0)
			return 1 ;
		return -1 ;
	}

	MatrixLayout inverse () const override {
		const auto r1x = MathProc::inverse (det ()) ;
		assume (r1x != 0) ;
		MatrixLayout ret = adjoint () ;
		ret = MatrixHolder::hold (ret)->smul (r1x) ;
		return move (ret) ;
	}

	FLT64 trace () const override {
		FLT64 ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += fake.mMatrix[mm (i ,i)] ;
		}
		return move (ret) ;
	}
} ;

exports VFat<MatrixHolder> MatrixHolder::hold (VREF<MatrixLayout> that) {
	return VFat<MatrixHolder> (MatrixImplHolder () ,that) ;
}

exports CFat<MatrixHolder> MatrixHolder::hold (CREF<MatrixLayout> that) {
	return CFat<MatrixHolder> (MatrixImplHolder () ,that) ;
}

class MakeMatrixImplHolder final implement Fat<MakeMatrixHolder ,MatrixLayout> {
public:
	void make_DiagMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) override {
		Matrix ret = Matrix::zero () ;
		ret[0][0] = x ;
		ret[1][1] = y ;
		ret[2][2] = z ;
		ret[3][3] = w ;
		fake = move (ret) ;
	}

	void make_ShearMatrix (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = z.normalize () ;
		const auto r4x = r1x * r2x ;
		const auto r5x = r1x * r3x ;
		const auto r6x = r2x * r3x ;
		const auto r7x = MathProc::sqrt (1 - MathProc::square (r4x)) ;
		const auto r8x = (r6x - r4x * r5x) * MathProc::inverse (r7x) ;
		const auto r9x = MathProc::sqrt (1 - MathProc::square (r5x) - MathProc::square (r8x)) ;
		ret[0][0] = 1 ;
		ret[0][1] = r4x ;
		ret[1][1] = r7x ;
		ret[0][2] = r5x ;
		ret[1][2] = r8x ;
		ret[2][3] = r9x ;
		ret[3][3] = 1 ;
		fake = move (ret) ;
	}

	void make_RotationMatrix (CREF<Vector> normal ,CREF<FLT64> angle) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = normal.normalize () ;
		const auto r2x = MathProc::cos (angle) ;
		const auto r3x = r1x * MathProc::sin (angle) ;
		const auto r4x = r1x * (1 - r2x) ;
		ret[0][0] = r1x[0] * r4x[0] + r2x ;
		ret[1][0] = r1x[1] * r4x[0] + r3x[2] ;
		ret[2][0] = r1x[2] * r4x[0] - r3x[1] ;
		ret[0][1] = r1x[0] * r4x[1] - r3x[2] ;
		ret[1][1] = r1x[1] * r4x[1] + r2x ;
		ret[2][1] = r1x[2] * r4x[1] + r3x[0] ;
		ret[0][2] = r1x[0] * r4x[2] + r3x[1] ;
		ret[1][2] = r1x[1] * r4x[2] - r3x[0] ;
		ret[2][2] = r1x[2] * r4x[2] + r2x ;
		ret[3][3] = 1 ;
		fake = move (ret) ;
	}

	void make_TranslationMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) override {
		Matrix ret = Matrix::identity () ;
		ret[0][3] = x ;
		ret[1][3] = y ;
		ret[2][3] = z ;
		fake = move (ret) ;
	}

	void make_PerspectiveMatrix (CREF<FLT64> fx ,CREF<FLT64> fy ,CREF<FLT64> wx ,CREF<FLT64> wy) override {
		assert (fx > 0) ;
		assert (fy > 0) ;
		Matrix ret = Matrix::zero () ;
		ret[0][0] = fx ;
		ret[1][1] = fy ;
		ret[0][2] = wx ;
		ret[1][2] = wy ;
		ret[3][2] = 1 ;
		ret[2][3] = 1 ;
		fake = move (ret) ;
	}

	void make_ProjectionMatrix (CREF<Vector> normal ,CREF<Vector> center ,CREF<Vector> light) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = normal.normalize () ;
		const auto r2x = center * r1x ;
		const auto r3x = light.normalize () ;
		const auto r4x = r1x * r3x ;
		ret[0][0] = r4x - r1x[0] * r3x[0] ;
		ret[1][0] = -r1x[0] * r3x[1] ;
		ret[2][0] = -r1x[0] * r3x[2] ;
		ret[0][1] = -r1x[1] * r3x[0] ;
		ret[1][1] = r4x - r1x[1] * r3x[1] ;
		ret[2][1] = -r1x[1] * r3x[2] ;
		ret[0][2] = -r1x[2] * r3x[0] ;
		ret[1][2] = -r1x[2] * r3x[1] ;
		ret[2][2] = r4x - r1x[2] * r3x[2] ;
		ret[0][3] = r2x * r3x[0] ;
		ret[1][3] = r2x * r3x[1] ;
		ret[2][3] = r2x * r3x[2] ;
		ret[3][3] = r4x ;
		fake = move (ret) ;
	}

	void make_ViewMatrix (CREF<Vector> x ,CREF<Vector> y) override {
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = (r1x ^ r2x).normalize () ;
		const auto r4x = (r3x ^ r1x).normalize () ;
		const auto r5x = Vector::axis_w () ;
		fake = Matrix (r1x ,r4x ,r3x ,r5x) ;
	}

	void make_ViewMatrix (CREF<Vector> x ,CREF<Vector> y ,CREF<Just<ViewMatrixOption>> option) override {
		make_ViewMatrix (x ,y) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (option != ViewMatrixOption::XYZ)
				discard ;
			const auto r1x = Vector::axis_x () ;
			const auto r2x = Vector::axis_y () ;
			const auto r3x = Vector::axis_z () ;
			const auto r4x = Vector::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			fake = MatrixHolder::hold (fake)->smul (r5x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::XZY)
				discard ;
			const auto r6x = Vector::axis_x () ;
			const auto r7x = -Vector::axis_z () ;
			const auto r8x = Vector::axis_y () ;
			const auto r9x = Vector::axis_w () ;
			const auto r10x = Matrix (r6x ,r7x ,r8x ,r9x) ;
			fake = MatrixHolder::hold (fake)->smul (r10x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::YXZ)
				discard ;
			const auto r11x = Vector::axis_y () ;
			const auto r12x = Vector::axis_x () ;
			const auto r13x = -Vector::axis_z () ;
			const auto r14x = Vector::axis_w () ;
			const auto r15x = Matrix (r11x ,r12x ,r13x ,r14x) ;
			fake = MatrixHolder::hold (fake)->smul (r15x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::YZX)
				discard ;
			const auto r16x = Vector::axis_z () ;
			const auto r17x = Vector::axis_x () ;
			const auto r18x = Vector::axis_y () ;
			const auto r19x = Vector::axis_w () ;
			const auto r20x = Matrix (r16x ,r17x ,r18x ,r19x) ;
			fake = MatrixHolder::hold (fake)->smul (r20x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::ZXY)
				discard ;
			const auto r21x = Vector::axis_y () ;
			const auto r22x = Vector::axis_z () ;
			const auto r23x = Vector::axis_x () ;
			const auto r24x = Vector::axis_w () ;
			const auto r25x = Matrix (r21x ,r22x ,r23x ,r24x) ;
			fake = MatrixHolder::hold (fake)->smul (r25x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::ZYX)
				discard ;
			const auto r26x = -Vector::axis_z () ;
			const auto r27x = Vector::axis_y () ;
			const auto r28x = Vector::axis_x () ;
			const auto r29x = Vector::axis_w () ;
			const auto r30x = Matrix (r26x ,r27x ,r28x ,r29x) ;
			fake = MatrixHolder::hold (fake)->smul (r30x) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	void make_CrossProductMatrix (CREF<Vector> xyz) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = xyz.homogenize () ;
		ret[1][0] = r1x[2] ;
		ret[2][0] = -r1x[1] ;
		ret[0][1] = -r1x[2] ;
		ret[2][1] = r1x[0] ;
		ret[0][2] = r1x[1] ;
		ret[1][2] = -r1x[0] ;
		fake = move (ret) ;
	}

	void make_SymmetryMatrix (CREF<Vector> x ,CREF<Vector> y) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = x ;
		const auto r2x = y ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			ret[i] = r1x[i.mY] * r2x[i.mX] ;
		}
		fake = move (ret) ;
	}

	void make_AffineMatrix (CREF<Array<FLT64>> a) override {
		Matrix ret = Matrix::identity () ;
		if ifdo (TRUE) {
			if (a.length () < 3)
				discard ;
			ret *= TranslationMatrix (a[0] ,a[1] ,a[2]) ;
		}
		if ifdo (TRUE) {
			if (a.length () < 7)
				discard ;
			ret *= Quaternion (a[4] ,a[5] ,a[6] ,a[7]).matrix () ;
		}
		if ifdo (TRUE) {
			if (a.length () < 10)
				discard ;
			ret *= DiagMatrix (a[8] ,a[9] ,a[10]) ;
		}
		fake = move (ret) ;
	}
} ;

exports VFat<MakeMatrixHolder> MakeMatrixHolder::hold (VREF<MatrixLayout> that) {
	return VFat<MakeMatrixHolder> (MakeMatrixImplHolder () ,that) ;
}

exports CFat<MakeMatrixHolder> MakeMatrixHolder::hold (CREF<MatrixLayout> that) {
	return CFat<MakeMatrixHolder> (MakeMatrixImplHolder () ,that) ;
}

template class External<MatrixProcHolder ,MatrixProcLayout> ;

exports CREF<MatrixProcLayout> MatrixProcHolder::instance () {
	return memorize ([&] () {
		MatrixProcLayout ret ;
		MatrixProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<MatrixProcHolder> MatrixProcHolder::hold (VREF<MatrixProcLayout> that) {
	return VFat<MatrixProcHolder> (External<MatrixProcHolder ,MatrixProcLayout>::declare () ,that) ;
}

exports CFat<MatrixProcHolder> MatrixProcHolder::hold (CREF<MatrixProcLayout> that) {
	return CFat<MatrixProcHolder> (External<MatrixProcHolder ,MatrixProcLayout>::declare () ,that) ;
}

class DuplexMatrixImplHolder final implement Fat<DuplexMatrixHolder ,DuplexMatrixLayout> {
public:
	void initialize (CREF<Matrix> that) override {
		fake.mDuplexMatrix[0] = that ;
		fake.mDuplexMatrix[1] = fake.mDuplexMatrix[0].inverse () ;
		if ifdo (TRUE) {
			if (fake.mDuplexMatrix[0][3][0] != 0)
				discard ;
			if (fake.mDuplexMatrix[0][3][1] != 0)
				discard ;
			if (fake.mDuplexMatrix[0][3][2] != 0)
				discard ;
			if (fake.mDuplexMatrix[0][3][3] != 1)
				discard ;
			fake.mDuplexMatrix[1][3][3] = 1 ;
		}
	}

	DuplexMatrixLayout inverse () const override {
		DuplexMatrixLayout ret ;
		ret.mDuplexMatrix[0] = fake.mDuplexMatrix[1] ;
		ret.mDuplexMatrix[1] = fake.mDuplexMatrix[0] ;
		return move (ret) ;
	}
} ;

exports VFat<DuplexMatrixHolder> DuplexMatrixHolder::hold (VREF<DuplexMatrixLayout> that) {
	return VFat<DuplexMatrixHolder> (DuplexMatrixImplHolder () ,that) ;
}

exports CFat<DuplexMatrixHolder> DuplexMatrixHolder::hold (CREF<DuplexMatrixLayout> that) {
	return CFat<DuplexMatrixHolder> (DuplexMatrixImplHolder () ,that) ;
}

class QuaternionImplHolder final implement Fat<QuaternionHolder ,QuaternionLayout> {
public:
	void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) override {
		fake.mQuaternion[0] = x ;
		fake.mQuaternion[1] = y ;
		fake.mQuaternion[2] = z ;
		fake.mQuaternion[3] = w ;
		normalized () ;
	}

	void initialize (CREF<VectorLayout> that) override {
		const auto r1x = move (keep[TYPE<Vector>::expr] (that)) ;
		const auto r2x = r1x.magnitude () ;
		const auto r3x = r1x.normalize () ;
		const auto r4x = MathProc::sin (r2x / 2) ;
		const auto r5x = MathProc::cos (r2x / 2) ;
		fake.mQuaternion[0] = r3x[0] * r4x ;
		fake.mQuaternion[1] = r3x[1] * r4x ;
		fake.mQuaternion[2] = r3x[2] * r4x ;
		fake.mQuaternion[3] = r5x ;
		normalized () ;
	}

	void initialize (CREF<MatrixLayout> that) override {
		const auto r1x = move (keep[TYPE<Matrix>::expr] (that)) ;
		const auto r2x = r1x.homogenize () ;
		auto act = TRUE ;
		if ifdo (act) {
			const auto r3x = 1 + r2x.trace () ;
			if (r3x < 1)
				discard ;
			const auto r4x = MathProc::sqrt (r3x) ;
			const auto r5x = MathProc::inverse (r4x) ;
			fake.mQuaternion[0] = (r2x[2][1] - r2x[1][2]) * r5x ;
			fake.mQuaternion[1] = (r2x[0][2] - r2x[2][0]) * r5x ;
			fake.mQuaternion[2] = (r2x[1][0] - r2x[0][1]) * r5x ;
			fake.mQuaternion[3] = r4x ;
		}
		if ifdo (act) {
			const auto r6x = 1 + r2x[0][0] - r2x[1][1] - r2x[2][2] ;
			if (r6x < 1)
				discard ;
			const auto r7x = MathProc::sqrt (r6x) ;
			const auto r8x = MathProc::inverse (r7x) ;
			fake.mQuaternion[0] = r7x ;
			fake.mQuaternion[1] = (r2x[1][0] + r2x[0][1]) * r8x ;
			fake.mQuaternion[2] = (r2x[0][2] + r2x[2][0]) * r8x ;
			fake.mQuaternion[3] = (r2x[2][1] - r2x[1][2]) * r8x ;
		}
		if ifdo (act) {
			const auto r9x = 1 - r2x[0][0] + r2x[1][1] - r2x[2][2] ;
			if (r9x < 1)
				discard ;
			const auto r10x = MathProc::sqrt (r9x) ;
			const auto r11x = MathProc::inverse (r10x) ;
			fake.mQuaternion[0] = (r2x[1][0] + r2x[0][1]) * r11x ;
			fake.mQuaternion[1] = r10x ;
			fake.mQuaternion[2] = (r2x[2][1] + r2x[1][2]) * r11x ;
			fake.mQuaternion[3] = (r2x[0][2] - r2x[2][0]) * r11x ;
		}
		if ifdo (act) {
			const auto r12x = 1 - r2x[0][0] - r2x[1][1] + r2x[2][2] ;
			if (r12x < 1)
				discard ;
			const auto r13x = MathProc::sqrt (r12x) ;
			const auto r14x = MathProc::inverse (r13x) ;
			fake.mQuaternion[0] = (r2x[0][2] + r2x[2][0]) * r14x ;
			fake.mQuaternion[1] = (r2x[2][1] + r2x[1][2]) * r14x ;
			fake.mQuaternion[2] = r13x ;
			fake.mQuaternion[3] = (r2x[1][0] - r2x[0][1]) * r14x ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
		normalized () ;
	}

	void normalized () {
		const auto r1x = MathProc::square (fake.mQuaternion[0]) ;
		const auto r2x = MathProc::square (fake.mQuaternion[1]) ;
		const auto r3x = MathProc::square (fake.mQuaternion[2]) ;
		const auto r4x = MathProc::square (fake.mQuaternion[3]) ;
		const auto r5x = MathProc::sqrt (r1x + r2x + r3x + r4x) ;
		const auto r6x = MathProc::inverse (r5x) ;
		fake.mQuaternion[0] *= r6x ;
		fake.mQuaternion[1] *= r6x ;
		fake.mQuaternion[2] *= r6x ;
		fake.mQuaternion[3] *= r6x ;
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue override {
		return fake.mQuaternion[y] ;
	}

	BOOL equal (CREF<QuaternionLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_equal (fake.mQuaternion[i] ,that.mQuaternion[i]) ;
			if (!r1x)
				return r1x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<QuaternionLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_compr (fake.mQuaternion[i] ,that.mQuaternion[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorFriend> visitor) const override {
		visitor.enter () ;
		for (auto &&i : iter (0 ,4)) {
			inline_visit (visitor ,fake.mQuaternion[i]) ;
		}
		visitor.leave () ;
	}

	QuaternionLayout smul (CREF<QuaternionLayout> that) const override {
		QuaternionLayout ret ;
		const auto r1x = axis (fake) ;
		const auto r2x = axis (that) ;
		const auto r3x = fake.mQuaternion[3] ;
		const auto r4x = that.mQuaternion[3] ;
		const auto r5x = r3x * r2x + r4x * r1x + (r1x ^ r2x) ;
		const auto r6x = r3x * r4x - r1x * r2x ;
		ret.mQuaternion[0] = r5x[0] ;
		ret.mQuaternion[1] = r5x[1] ;
		ret.mQuaternion[2] = r5x[2] ;
		ret.mQuaternion[3] = r6x ;
		return move (ret) ;
	}

	static Vector axis (CREF<QuaternionLayout> q) {
		return Vector (q.mQuaternion[0] ,q.mQuaternion[1] ,q.mQuaternion[2] ,0) ;
	}

	FLT64 angle () const {
		const auto r1x = axis (fake).magnitude () ;
		const auto r2x = fake.mQuaternion[3] ;
		return MathProc::atan (r1x ,r2x) * 2 ;
	}

	VectorLayout vector () const override {
		const auto r1x = axis (fake).normalize () ;
		const auto r2x = angle () ;
		return r1x * r2x ;
	}

	MatrixLayout matrix () const override {
		const auto r1x = axis (fake).normalize () ;
		const auto r2x = angle () ;
		return RotationMatrix (r1x ,r2x) ;
	}
} ;

exports VFat<QuaternionHolder> QuaternionHolder::hold (VREF<QuaternionLayout> that) {
	return VFat<QuaternionHolder> (QuaternionImplHolder () ,that) ;
}

exports CFat<QuaternionHolder> QuaternionHolder::hold (CREF<QuaternionLayout> that) {
	return CFat<QuaternionHolder> (QuaternionImplHolder () ,that) ;
}

template class External<LinearProcHolder ,LinearProcLayout> ;

exports CREF<LinearProcLayout> LinearProcHolder::instance () {
	return memorize ([&] () {
		LinearProcLayout ret ;
		LinearProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<LinearProcHolder> LinearProcHolder::hold (VREF<LinearProcLayout> that) {
	return VFat<LinearProcHolder> (External<LinearProcHolder ,LinearProcLayout>::declare () ,that) ;
}

exports CFat<LinearProcHolder> LinearProcHolder::hold (CREF<LinearProcLayout> that) {
	return CFat<LinearProcHolder> (External<LinearProcHolder ,LinearProcLayout>::declare () ,that) ;
}

class PointCloudImplHolder final implement Fat<PointCloudHolder ,PointCloudLayout> {
public:
	void initialize (RREF<Ref<Array<Point2F>>> that) override {
		fake.mRank = 2 ;
		auto &&rax = keep[TYPE<Ref<Array<Point2F>>>::expr] (Pointer::from (fake.mPointCloud)) ;
		rax = move (that) ;
		fake.mWorld = Matrix::identity () ;
	}

	void initialize (RREF<Ref<Array<Point3F>>> that) override {
		fake.mRank = 3 ;
		auto &&rax = keep[TYPE<Ref<Array<Point3F>>>::expr] (Pointer::from (fake.mPointCloud)) ;
		rax = move (that) ;
		fake.mWorld = Matrix::identity () ;
	}

	LENGTH size () const override {
		return fake.mPointCloud->size () ;
	}

	void get (CREF<INDEX> index ,VREF<Vector> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mRank != 2)
				discard ;
			item = Vector (keep[TYPE<Point2F>::expr] (fake.mPointCloud.self[index])) ;
			item = fake.mWorld * item ;
		}
		if ifdo (act) {
			if (fake.mRank != 3)
				discard ;
			item = Vector (keep[TYPE<Point3F>::expr] (fake.mPointCloud.self[index])) ;
			item = fake.mWorld * item ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	Vector center () const override {
		Vector ret = Vector::zero () ;
		auto rax = Vector () ;
		for (auto &&i : fake.mPointCloud->range ()) {
			get (i ,rax) ;
			ret += rax ;
		}
		ret = ret.projection () ;
		return move (ret) ;
	}

	Matrix svd_matrix () const override {
		const auto r1x = center () ;
		const auto r2x = invoke ([&] () {
			Matrix ret = Matrix::zero () ;
			auto rax = Vector () ;
			for (auto &&i : fake.mPointCloud->range ()) {
				get (i ,rax) ;
				const auto r3x = (rax - r1x).normalize () ;
				ret[0][0] += MathProc::square (r3x[0]) ;
				ret[0][1] += r3x[0] * r3x[1] ;
				ret[0][2] += r3x[0] * r3x[2] ;
				ret[1][1] += MathProc::square (r3x[1]) ;
				ret[1][2] += r3x[1] * r3x[2] ;
				ret[2][2] += MathProc::square (r3x[2]) ;
			}
			ret[1][0] = ret[0][1] ;
			ret[2][0] = ret[0][2] ;
			ret[2][1] = ret[1][2] ;
			return move (ret) ;
		}) ;
		const auto r4x = MatrixProc::solve_svd (r2x) ;
		const auto r5x = TranslationMatrix (r1x) ;
		const auto r6x = DiagMatrix (sqrt_fix (r4x.mS[0][0]) ,sqrt_fix (r4x.mS[1][1]) ,sqrt_fix (r4x.mS[2][2])) ;
		const auto r7x = r5x * r4x.mV * r6x ;
		return r7x ;
	}

	FLT64 sqrt_fix (CREF<FLT64> a) const {
		const auto r1x = FLT64 (FLT32_EPS) / 2 ;
		return MathProc::sqrt (MathProc::max_of (a ,r1x)) ;
	}

	Matrix box_matrix () const override {
		const auto r1x = bound () ;
		const auto r2x = invoke ([&] () {
			Vector ret = Vector::zero () ;
			INDEX ix = (size () - 1) / 2 ;
			INDEX iy = size () / 2 ;
			auto rax = Vector () ;
			get (ix ,rax) ;
			ret += rax ;
			get (iy ,rax) ;
			ret += rax ;
			ret = ret.projection () ;
			return move (ret) ;
		}) ;
		const auto r3x = TranslationMatrix (r2x) ;
		const auto r4x = DiagMatrix (r1x.mMax.mX - r1x.mMin.mX ,r1x.mMax.mY - r1x.mMin.mY ,1) ;
		return r3x * r4x ;
	}

	Line3F bound () const override {
		Line3F ret ;
		ret.mMin.mX = +infinity ;
		ret.mMin.mY = +infinity ;
		ret.mMin.mZ = +infinity ;
		ret.mMax.mX = -infinity ;
		ret.mMax.mY = -infinity ;
		ret.mMax.mZ = -infinity ;
		auto rax = Vector () ;
		for (auto &&i : fake.mPointCloud->range ()) {
			get (i ,rax) ;
			ret.mMin.mX = MathProc::min_of (ret.mMin.mX ,FLT32 (rax[0])) ;
			ret.mMin.mY = MathProc::min_of (ret.mMin.mY ,FLT32 (rax[1])) ;
			ret.mMin.mZ = MathProc::min_of (ret.mMin.mZ ,FLT32 (rax[2])) ;
			ret.mMax.mX = MathProc::max_of (ret.mMax.mX ,FLT32 (rax[0])) ;
			ret.mMax.mY = MathProc::max_of (ret.mMax.mY ,FLT32 (rax[1])) ;
			ret.mMax.mZ = MathProc::max_of (ret.mMax.mZ ,FLT32 (rax[2])) ;
		}
		return move (ret) ;
	}

	PointCloudLayout smul (CREF<Matrix> mat) const override {
		PointCloudLayout ret ;
		ret.mRank = fake.mRank ;
		ret.mPointCloud = fake.mPointCloud.share () ;
		ret.mWorld *= mat ;
		return move (ret) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor) const override {
		Array<INDEX> ret ;
		unimplemented () ;
		return move (ret) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor ,CREF<FLT64> radius) const override {
		Array<INDEX> ret ;
		unimplemented () ;
		return move (ret) ;
	}
} ;

exports VFat<PointCloudHolder> PointCloudHolder::hold (VREF<PointCloudLayout> that) {
	return VFat<PointCloudHolder> (PointCloudImplHolder () ,that) ;
}

exports CFat<PointCloudHolder> PointCloudHolder::hold (CREF<PointCloudLayout> that) {
	return CFat<PointCloudHolder> (PointCloudImplHolder () ,that) ;
}
} ;