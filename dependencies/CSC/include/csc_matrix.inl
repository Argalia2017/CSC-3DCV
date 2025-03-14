#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_matrix.hpp"

namespace CSC {
class VectorImplHolder final implement Fat<VectorHolder ,VectorLayout> {
public:
	void initialize (CREF<Buffer<FLT64 ,RANK4>> that) override {
		self.mVector[0] = that[0] ;
		self.mVector[1] = that[1] ;
		self.mVector[2] = that[2] ;
		self.mVector[3] = that[3] ;
	}

	void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) override {
		self.mVector[0] = x ;
		self.mVector[1] = y ;
		self.mVector[2] = z ;
		self.mVector[3] = w ;
	}

	void initialize (CREF<Pixel> point) override {
		self.mVector[0] = FLT64 (point.mX) + FLT64 (0.5) ;
		self.mVector[1] = FLT64 (point.mY) + FLT64 (0.5) ;
		self.mVector[2] = 0 ;
		self.mVector[3] = 1 ;
	}

	void initialize (CREF<Point2F> point) override {
		self.mVector[0] = FLT64 (point.mX) ;
		self.mVector[1] = FLT64 (point.mY) ;
		self.mVector[2] = 0 ;
		self.mVector[3] = 1 ;
	}

	void initialize (CREF<Point3F> point) override {
		self.mVector[0] = FLT64 (point.mX) ;
		self.mVector[1] = FLT64 (point.mY) ;
		self.mVector[2] = FLT64 (point.mZ) ;
		self.mVector[3] = 1 ;
	}

	VREF<FLT64> at (CREF<INDEX> y) leftvalue override {
		return self.mVector[y] ;
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue override {
		return self.mVector[y] ;
	}

	static INDEX mm (CREF<INDEX> x ,CREF<INDEX> y) {
		return x + y * 4 ;
	}

	BOOL equal (CREF<VectorLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_equal (self.mVector[i] ,that.mVector[i]) ;
			if (!r1x)
				return r1x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<VectorLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_compr (self.mVector[i] ,that.mVector[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		for (auto &&i : iter (0 ,4)) {
			inline_visit (visitor ,self.mVector[i]) ;
		}
		visitor.leave () ;
	}

	VectorLayout sadd (CREF<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = self.mVector[i] + that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout ssub (CREF<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = self.mVector[i] - that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout smul (CREF<FLT64> scale) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = self.mVector[i] * scale ;
		}
		return move (ret) ;
	}

	VectorLayout sdiv (CREF<FLT64> scale) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = self.mVector[i] / scale ;
		}
		return move (ret) ;
	}

	FLT64 dot (CREF<VectorLayout> that) const override {
		FLT64 ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += self.mVector[i] * that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout smul (CREF<MatrixLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			auto rax = FLT64 (0) ;
			rax += self.mVector[0] * that.mMatrix[mm (i ,0)] ;
			rax += self.mVector[1] * that.mMatrix[mm (i ,1)] ;
			rax += self.mVector[2] * that.mMatrix[mm (i ,2)] ;
			rax += self.mVector[3] * that.mMatrix[mm (i ,3)] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	VectorLayout cross (CREF<VectorLayout> that) const override {
		assert (self.mVector[3] == 0) ;
		assert (that.mVector[3] == 0) ;
		VectorLayout ret ;
		ret.mVector[0] = self.mVector[1] * that.mVector[2] - self.mVector[2] * that.mVector[1] ;
		ret.mVector[1] = self.mVector[2] * that.mVector[0] - self.mVector[0] * that.mVector[2] ;
		ret.mVector[2] = self.mVector[0] * that.mVector[1] - self.mVector[1] * that.mVector[0] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	VectorLayout sabs () const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = MathProc::abs (self.mVector[i]) ;
		}
		return move (ret) ;
	}

	VectorLayout minus () const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = -self.mVector[i] ;
		}
		return move (ret) ;
	}

	FLT64 magnitude () const override {
		assert (self.mVector[3] == 0) ;
		FLT64 ret = 0 ;
		ret += MathProc::square (self.mVector[0]) ;
		ret += MathProc::square (self.mVector[1]) ;
		ret += MathProc::square (self.mVector[2]) ;
		ret = MathProc::sqrt (ret) ;
		return move (ret) ;
	}

	VectorLayout normalize () const override {
		VectorLayout ret ;
		const auto r1x = MathProc::inverse (magnitude ()) ;
		ret.mVector[0] = self.mVector[0] * r1x ;
		ret.mVector[1] = self.mVector[1] * r1x ;
		ret.mVector[2] = self.mVector[2] * r1x ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	VectorLayout projection () const override {
		VectorLayout ret ;
		const auto r1x = MathProc::inverse (self.mVector[3]) ;
		ret.mVector[0] = self.mVector[0] * r1x ;
		ret.mVector[1] = self.mVector[1] * r1x ;
		ret.mVector[2] = self.mVector[2] * r1x ;
		ret.mVector[3] = 1 ;
		return move (ret) ;
	}

	VectorLayout homogenize () const override {
		VectorLayout ret ;
		ret.mVector[0] = self.mVector[0] ;
		ret.mVector[1] = self.mVector[1] ;
		ret.mVector[2] = self.mVector[2] ;
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
			self.mMatrix[mm (i.mX ,i.mY)] = that[ix] ;
		}
	}

	void initialize (CREF<VectorLayout> x ,CREF<VectorLayout> y ,CREF<VectorLayout> z ,CREF<VectorLayout> w) override {
		for (auto &&i : iter (0 ,4)) {
			self.mMatrix[mm (0 ,i)] = x.mVector[i] ;
			self.mMatrix[mm (1 ,i)] = y.mVector[i] ;
			self.mMatrix[mm (2 ,i)] = z.mVector[i] ;
			self.mMatrix[mm (3 ,i)] = w.mVector[i] ;
		}
	}

	VREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue override {
		assert (inline_between (x ,0 ,4)) ;
		assert (inline_between (y ,0 ,4)) ;
		return self.mMatrix[mm (x ,y)] ;
	}

	CREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (inline_between (x ,0 ,4)) ;
		assert (inline_between (y ,0 ,4)) ;
		return self.mMatrix[mm (x ,y)] ;
	}

	static INDEX mm (CREF<INDEX> x ,CREF<INDEX> y) {
		return x + y * 4 ;
	}

	BOOL equal (CREF<MatrixLayout> that) const override {
		for (auto &&i : iter (0 ,16)) {
			const auto r1x = inline_equal (self.mMatrix[i] ,that.mMatrix[i]) ;
			if (!r1x)
				return r1x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<MatrixLayout> that) const override {
		for (auto &&i : iter (0 ,16)) {
			const auto r1x = inline_compr (self.mMatrix[i] ,that.mMatrix[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		for (auto &&i : iter (0 ,16)) {
			inline_visit (visitor ,self.mMatrix[i]) ;
		}
		visitor.leave () ;
	}

	MatrixLayout sadd (CREF<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = self.mMatrix[i] + that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout ssub (CREF<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = self.mMatrix[i] - that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout smul (CREF<FLT64> scale) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = self.mMatrix[i] * scale ;
		}
		return move (ret) ;
	}

	MatrixLayout sdiv (CREF<FLT64> scale) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = self.mMatrix[i] / scale ;
		}
		return move (ret) ;
	}

	VectorLayout smul (CREF<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			auto rax = FLT64 (0) ;
			rax += self.mMatrix[mm (0 ,i)] * that.mVector[0] ;
			rax += self.mMatrix[mm (1 ,i)] * that.mVector[1] ;
			rax += self.mMatrix[mm (2 ,i)] * that.mVector[2] ;
			rax += self.mMatrix[mm (3 ,i)] * that.mVector[3] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout smul (CREF<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			auto rax = FLT64 (0) ;
			rax += self.mMatrix[mm (0 ,i.mY)] * that.mMatrix[mm (i.mX ,0)] ;
			rax += self.mMatrix[mm (1 ,i.mY)] * that.mMatrix[mm (i.mX ,1)] ;
			rax += self.mMatrix[mm (2 ,i.mY)] * that.mMatrix[mm (i.mX ,2)] ;
			rax += self.mMatrix[mm (3 ,i.mY)] * that.mMatrix[mm (i.mX ,3)] ;
			ret.mMatrix[mm (i.mX ,i.mY)] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout sabs () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = MathProc::abs (self.mMatrix[i]) ;
		}
		return move (ret) ;
	}

	MatrixLayout minus () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = -self.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout transpose () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			ret.mMatrix[mm (i.mY ,i.mX)] = self.mMatrix[mm (i.mX ,i.mY)] ;
		}
		return move (ret) ;
	}

	MatrixLayout triangular () const override {
		MatrixLayout ret = self ;
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
			ret.mMatrix[mm (i.mX ,i.mY)] = self.mMatrix[mm (i.mX ,i.mY)] ;
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
			const auto r1x = self.mMatrix[mm (jy ,iy)] * self.mMatrix[mm (jz ,iz)] - self.mMatrix[mm (jz ,iy)] * self.mMatrix[mm (jy ,iz)] ;
			const auto r2x = self.mMatrix[mm (jy ,ix)] * self.mMatrix[mm (jz ,iz)] - self.mMatrix[mm (jz ,ix)] * self.mMatrix[mm (jy ,iz)] ;
			const auto r3x = self.mMatrix[mm (jy ,ix)] * self.mMatrix[mm (jz ,iy)] - self.mMatrix[mm (jz ,ix)] * self.mMatrix[mm (jy ,iy)] ;
			const auto r4x = self.mMatrix[mm (jx ,ix)] * r1x ;
			const auto r5x = self.mMatrix[mm (jx ,iy)] * r2x ;
			const auto r6x = self.mMatrix[mm (jx ,iz)] * r3x ;
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
			ret += self.mMatrix[mm (i ,i)] ;
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
		self = move (ret) ;
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
		self = move (ret) ;
	}

	void make_RotationMatrix (CREF<FLAG> axis ,CREF<FLT64> angle) override {
		Matrix ret = Matrix::identity () ;
		const auto r1x = MathProc::cos (angle) ;
		const auto r2x = MathProc::sin (angle) ;
		INDEX ix = axis ;
		INDEX iy = (ix + 1) % 3 ;
		INDEX iz = (iy + 1) % 3 ;
		ret[iy][iy] = r1x ;
		ret[iz][iy] = +r2x ;
		ret[iy][iz] = -r2x ;
		ret[iz][iz] = r1x ;
		self = move (ret) ;
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
		self = move (ret) ;
	}

	void make_TranslationMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) override {
		Matrix ret = Matrix::identity () ;
		ret[0][3] = x ;
		ret[1][3] = y ;
		ret[2][3] = z ;
		self = move (ret) ;
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
		self = move (ret) ;
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
		self = move (ret) ;
	}

	void make_ViewMatrix (CREF<Vector> x ,CREF<Vector> y) override {
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = (r1x ^ r2x).normalize () ;
		const auto r4x = (r3x ^ r1x).normalize () ;
		const auto r5x = Vector::axis_w () ;
		self = Matrix (r1x ,r4x ,r3x ,r5x) ;
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
			self = MatrixHolder::hold (self)->smul (r5x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::XZY)
				discard ;
			const auto r6x = Vector::axis_x () ;
			const auto r7x = -Vector::axis_z () ;
			const auto r8x = Vector::axis_y () ;
			const auto r9x = Vector::axis_w () ;
			const auto r10x = Matrix (r6x ,r7x ,r8x ,r9x) ;
			self = MatrixHolder::hold (self)->smul (r10x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::YXZ)
				discard ;
			const auto r11x = Vector::axis_y () ;
			const auto r12x = Vector::axis_x () ;
			const auto r13x = -Vector::axis_z () ;
			const auto r14x = Vector::axis_w () ;
			const auto r15x = Matrix (r11x ,r12x ,r13x ,r14x) ;
			self = MatrixHolder::hold (self)->smul (r15x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::YZX)
				discard ;
			const auto r16x = Vector::axis_z () ;
			const auto r17x = Vector::axis_x () ;
			const auto r18x = Vector::axis_y () ;
			const auto r19x = Vector::axis_w () ;
			const auto r20x = Matrix (r16x ,r17x ,r18x ,r19x) ;
			self = MatrixHolder::hold (self)->smul (r20x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::ZXY)
				discard ;
			const auto r21x = Vector::axis_y () ;
			const auto r22x = Vector::axis_z () ;
			const auto r23x = Vector::axis_x () ;
			const auto r24x = Vector::axis_w () ;
			const auto r25x = Matrix (r21x ,r22x ,r23x ,r24x) ;
			self = MatrixHolder::hold (self)->smul (r25x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::ZYX)
				discard ;
			const auto r26x = -Vector::axis_z () ;
			const auto r27x = Vector::axis_y () ;
			const auto r28x = Vector::axis_x () ;
			const auto r29x = Vector::axis_w () ;
			const auto r30x = Matrix (r26x ,r27x ,r28x ,r29x) ;
			self = MatrixHolder::hold (self)->smul (r30x) ;
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
		self = move (ret) ;
	}

	void make_SymmetryMatrix (CREF<Vector> x ,CREF<Vector> y) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = x ;
		const auto r2x = y ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			ret[i] = r1x[i.mY] * r2x[i.mX] ;
		}
		self = move (ret) ;
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
		self = move (ret) ;
	}
} ;

exports VFat<MakeMatrixHolder> MakeMatrixHolder::hold (VREF<MatrixLayout> that) {
	return VFat<MakeMatrixHolder> (MakeMatrixImplHolder () ,that) ;
}

exports CFat<MakeMatrixHolder> MakeMatrixHolder::hold (CREF<MatrixLayout> that) {
	return CFat<MakeMatrixHolder> (MakeMatrixImplHolder () ,that) ;
}

template class External<MatrixProcHolder ,MatrixProcLayout> ;

exports CREF<OfThis<UniqueRef<MatrixProcLayout>>> MatrixProcHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<MatrixProcLayout>> ret ;
		External<MatrixProcHolder ,MatrixProcLayout>::declare ().create (ret.mThis) ;
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
		self.mDuplexMatrix[0] = that ;
		self.mDuplexMatrix[1] = self.mDuplexMatrix[0].inverse () ;
		if ifdo (TRUE) {
			if (self.mDuplexMatrix[0][3][0] != 0)
				discard ;
			if (self.mDuplexMatrix[0][3][1] != 0)
				discard ;
			if (self.mDuplexMatrix[0][3][2] != 0)
				discard ;
			if (self.mDuplexMatrix[0][3][3] != 1)
				discard ;
			self.mDuplexMatrix[1][3][3] = 1 ;
		}
	}

	DuplexMatrixLayout inverse () const override {
		DuplexMatrixLayout ret ;
		ret.mDuplexMatrix[0] = self.mDuplexMatrix[1] ;
		ret.mDuplexMatrix[1] = self.mDuplexMatrix[0] ;
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
		self.mQuaternion[0] = x ;
		self.mQuaternion[1] = y ;
		self.mQuaternion[2] = z ;
		self.mQuaternion[3] = w ;
		normalized () ;
	}

	void initialize (CREF<Vector> that) override {
		const auto r1x = that.magnitude () ;
		const auto r2x = that.normalize () ;
		const auto r3x = MathProc::sin (r1x / 2) ;
		const auto r4x = MathProc::cos (r1x / 2) ;
		self.mQuaternion[0] = r2x[0] * r3x ;
		self.mQuaternion[1] = r2x[1] * r3x ;
		self.mQuaternion[2] = r2x[2] * r3x ;
		self.mQuaternion[3] = r4x ;
		normalized () ;
	}

	void initialize (CREF<Matrix> that) override {
		const auto r1x = that.homogenize () ;
		auto act = TRUE ;
		if ifdo (act) {
			const auto r2x = 1 + r1x.trace () ;
			if (r2x < 1)
				discard ;
			const auto r3x = MathProc::sqrt (r2x) ;
			const auto r4x = MathProc::inverse (r3x) ;
			self.mQuaternion[0] = (r1x[2][1] - r1x[1][2]) * r4x ;
			self.mQuaternion[1] = (r1x[0][2] - r1x[2][0]) * r4x ;
			self.mQuaternion[2] = (r1x[1][0] - r1x[0][1]) * r4x ;
			self.mQuaternion[3] = r3x ;
		}
		if ifdo (act) {
			const auto r5x = 1 + r1x[0][0] - r1x[1][1] - r1x[2][2] ;
			if (r5x < 1)
				discard ;
			const auto r6x = MathProc::sqrt (r5x) ;
			const auto r7x = MathProc::inverse (r6x) ;
			self.mQuaternion[0] = r6x ;
			self.mQuaternion[1] = (r1x[1][0] + r1x[0][1]) * r7x ;
			self.mQuaternion[2] = (r1x[0][2] + r1x[2][0]) * r7x ;
			self.mQuaternion[3] = (r1x[2][1] - r1x[1][2]) * r7x ;
		}
		if ifdo (act) {
			const auto r8x = 1 - r1x[0][0] + r1x[1][1] - r1x[2][2] ;
			if (r8x < 1)
				discard ;
			const auto r9x = MathProc::sqrt (r8x) ;
			const auto r10x = MathProc::inverse (r9x) ;
			self.mQuaternion[0] = (r1x[1][0] + r1x[0][1]) * r10x ;
			self.mQuaternion[1] = r9x ;
			self.mQuaternion[2] = (r1x[2][1] + r1x[1][2]) * r10x ;
			self.mQuaternion[3] = (r1x[0][2] - r1x[2][0]) * r10x ;
		}
		if ifdo (act) {
			const auto r11x = 1 - r1x[0][0] - r1x[1][1] + r1x[2][2] ;
			if (r11x < 1)
				discard ;
			const auto r12x = MathProc::sqrt (r11x) ;
			const auto r13x = MathProc::inverse (r12x) ;
			self.mQuaternion[0] = (r1x[0][2] + r1x[2][0]) * r13x ;
			self.mQuaternion[1] = (r1x[2][1] + r1x[1][2]) * r13x ;
			self.mQuaternion[2] = r12x ;
			self.mQuaternion[3] = (r1x[1][0] - r1x[0][1]) * r13x ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
		normalized () ;
	}

	void initialize (CREF<EulerAngle> that) override {
		const auto r1x = RotationMatrix (0 ,that.mPitch) ;
		const auto r2x = RotationMatrix (1 ,that.mYaw) ;
		const auto r3x = RotationMatrix (2 ,that.mRoll) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::XYZ)
				discard ;
			//@info: We are using intrinsic rotation
			//@info: First, rotation RXi = RX
			//@info: Second, rotation RYi = RXi * RY * RXi.inverse
			//@info: Third, rotation RZi = (RYi * RXi) * RZ * (RYi * RXi).inverse
			//@info: Finally, rotation RZi * RYi * RXi = RX * RY * RZ
			//@info: So, in the XYZ rotation order, RX is the first rotation, but RZ is applied last
			const auto r4x = r1x * r2x * r3x ;
			initialize (r4x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::XZY)
				discard ;
			const auto r5x = r1x * r3x * r2x ;
			initialize (r5x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::YXZ)
				discard ;
			const auto r6x = r2x * r1x * r3x ;
			initialize (r6x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::YZX)
				discard ;
			const auto r7x = r2x * r3x * r1x ;
			initialize (r7x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::ZXY)
				discard ;
			const auto r8x = r3x * r1x * r2x ;
			initialize (r8x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::ZYX)
				discard ;
			const auto r9x = r3x * r2x * r1x ;
			initialize (r9x) ;
		}
	}

	void normalized () {
		const auto r1x = MathProc::square (self.mQuaternion[0]) ;
		const auto r2x = MathProc::square (self.mQuaternion[1]) ;
		const auto r3x = MathProc::square (self.mQuaternion[2]) ;
		const auto r4x = MathProc::square (self.mQuaternion[3]) ;
		const auto r5x = MathProc::sqrt (r1x + r2x + r3x + r4x) ;
		const auto r6x = MathProc::inverse (r5x) ;
		self.mQuaternion[0] *= r6x ;
		self.mQuaternion[1] *= r6x ;
		self.mQuaternion[2] *= r6x ;
		self.mQuaternion[3] *= r6x ;
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue override {
		return self.mQuaternion[y] ;
	}

	BOOL equal (CREF<QuaternionLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_equal (self.mQuaternion[i] ,that.mQuaternion[i]) ;
			if (!r1x)
				return r1x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<QuaternionLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_compr (self.mQuaternion[i] ,that.mQuaternion[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		for (auto &&i : iter (0 ,4)) {
			inline_visit (visitor ,self.mQuaternion[i]) ;
		}
		visitor.leave () ;
	}

	QuaternionLayout smul (CREF<QuaternionLayout> that) const override {
		QuaternionLayout ret ;
		const auto r1x = axis (self) ;
		const auto r2x = axis (that) ;
		const auto r3x = self.mQuaternion[3] ;
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
		const auto r1x = axis (self).magnitude () ;
		const auto r2x = self.mQuaternion[3] ;
		return MathProc::atan (r1x ,r2x) * 2 ;
	}

	Vector vector () const override {
		const auto r1x = axis (self).normalize () ;
		const auto r2x = angle () ;
		return r1x * r2x ;
	}

	Matrix matrix () const override {
		const auto r1x = axis (self).normalize () ;
		const auto r2x = angle () ;
		return RotationMatrix (r1x ,r2x) ;
	}

	EulerAngle euler (CREF<Just<ViewMatrixOption>> type) const override {
		EulerAngle ret ;
		const auto r1x = matrix () ;
		const auto r2x = DiagMatrix (+1 ,-1 ,-1) ;
		const auto r3x = DiagMatrix (-1 ,+1 ,-1) ;
		const auto r4x = DiagMatrix (-1 ,-1 ,+1) ;
		const auto r5x = Buffer3<Matrix> ({r2x ,r3x ,r4x}) ;
		auto rax = Buffer3<Matrix> () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (type != ViewMatrixOption::XYZ)
				discard ;
			rax = euler_decompose (r1x ,0 ,1 ,2) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[0] = rax[0] * r5x[0] ;
				rax[1] = r5x[0] * rax[1] ;
			}
			if ifdo (TRUE) {
				if (rax[1][1][1] >= 0)
					discard ;
				rax[1] = rax[1] * r5x[2] ;
				rax[2] = r5x[2] * rax[2] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::XZY)
				discard ;
			rax = euler_decompose (r1x ,0 ,2 ,1) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[0] = rax[0] * r5x[0] ;
				rax[2] = r5x[0] * rax[2] ;
			}
			if ifdo (TRUE) {
				if (rax[2][2][2] >= 0)
					discard ;
				rax[2] = rax[2] * r5x[1] ;
				rax[1] = r5x[1] * rax[1] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::YXZ)
				discard ;
			rax = euler_decompose (r1x ,1 ,0 ,2) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[1] = rax[1] * r5x[1] ;
				rax[0] = r5x[1] * rax[0] ;
				rax[0] = rax[0] * r5x[2] ;
				rax[2] = r5x[2] * rax[2] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::YZX)
				discard ;
			rax = euler_decompose (r1x ,1 ,2 ,0) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[2] = rax[2] * r5x[0] ;
				rax[0] = r5x[0] * rax[0] ;
			}
			if ifdo (TRUE) {
				if (rax[2][2][2] >= 0)
					discard ;
				rax[1] = rax[1] * r5x[1] ;
				rax[2] = r5x[1] * rax[2] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::ZXY)
				discard ;
			rax = euler_decompose (r1x ,2 ,0 ,1) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[2] = rax[2] * r5x[2] ;
				rax[0] = r5x[2] * rax[0] ;
				rax[0] = rax[0] * r5x[1] ;
				rax[1] = r5x[1] * rax[1] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::ZYX)
				discard ;
			rax = euler_decompose (r1x ,2 ,1 ,0) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[1] = rax[1] * r5x[0] ;
				rax[0] = r5x[0] * rax[0] ;
			}
			if ifdo (TRUE) {
				if (rax[1][1][1] >= 0)
					discard ;
				rax[2] = rax[2] * r5x[2] ;
				rax[1] = r5x[2] * rax[1] ;
			}
		}
		assert (MathProc::abs (rax[0][0][0] - 1) < FLT32_EPS) ;
		assert (MathProc::abs (rax[1][1][1] - 1) < FLT32_EPS) ;
		assert (MathProc::abs (rax[2][2][2] - 1) < FLT32_EPS) ;
		ret.mType = type ;
		ret.mPitch = MathProc::atan (rax[0][2][1] ,rax[0][1][1]) ;
		ret.mYaw = MathProc::atan (rax[1][0][2] ,rax[1][2][2]) ;
		ret.mRoll = MathProc::atan (rax[2][1][0] ,rax[2][0][0]) ;
		return move (ret) ;
	}

	Buffer3<Matrix> euler_decompose (CREF<Matrix> rot ,CREF<INDEX> x ,CREF<INDEX> y ,CREF<INDEX> z) const {
		Buffer3<Matrix> ret ;
		const auto r1x = Buffer3<Vector> ({
			Vector::axis_x () ,
			Vector::axis_y () ,
			Vector::axis_z ()}) ;
		const auto r2x = rot * r1x[z] ;
		const auto r3x = rotate_angle (r2x ,r1x[x] ,r1x[y]) ;
		const auto r4x = -r3x.fetch () ;
		ret[x] = RotationMatrix (x ,r4x) ;
		const auto r5x = ret[x].transpose () * rot ;
		const auto r6x = MathProc::atan (r5x[x][z] ,r5x[z][z]) * rotate_sign (y ,z) ;
		ret[y] = RotationMatrix (y ,r6x) ;
		const auto r7x = ret[y].transpose () * r5x ;
		const auto r8x = MathProc::atan (r7x[y][x] ,r7x[x][x]) * rotate_sign (z ,x) ;
		ret[z] = RotationMatrix (z ,r8x) ;
		const auto r9x = ret[z].transpose () * r7x - Matrix::identity () ;
		for (auto &&j : iter (0 ,4 ,0 ,4)) {
			assert (MathProc::abs (r9x[j]) < FLT32_EPS) ;
		}
		return move (ret) ;
	}

	FLT64 rotate_sign (CREF<INDEX> y ,CREF<INDEX> z) const {
		if (y == 0)
			if (z == 2)
				return -1 ;
		if (y == 1)
			if (z == 0)
				return -1 ;
		if (y == 2)
			if (z == 1)
				return -1 ;
		return +1 ;
	}

	Optional<FLT64> rotate_angle (CREF<Vector> dir ,CREF<Vector> rot_axis ,CREF<Vector> normal) const {
		const auto r1x = CrossProductMatrix (rot_axis) ;
		const auto r2x = r1x * r1x ;
		const auto r3x = normal * (Matrix::identity () + r2x) * dir ;
		const auto r4x = normal * r1x * dir ;
		const auto r5x = -(normal * r2x * dir) ;
		const auto r6x = MathProc::inverse (MathProc::hypot (r4x ,r5x)) ;
		const auto r7x = r3x * r6x ;
		if (MathProc::abs (r7x) >= 1)
			return Optional<FLT64>::error (1) ;
		const auto r8x = r4x * r6x ;
		const auto r9x = r5x * r6x ;
		const auto r10x = MathProc::atan (r8x ,r9x) ;
		const auto r11x = MathProc::acos (-r7x) ;
		const auto r12x = r10x - r11x ;
		return ortho_angle (r12x) ;
	}

	FLT64 ortho_angle (CREF<FLT64> x) const {
		const auto r1x = MathProc::floor (x ,MATH_PI * 2) ;
		const auto r2x = x - r1x - MATH_PI ;
		if (r2x <= -MATH_PI / 2)
			return r2x + MATH_PI ;
		if (r2x >= +MATH_PI / 2)
			return r2x - MATH_PI ;
		return r2x ;
	}
} ;

exports VFat<QuaternionHolder> QuaternionHolder::hold (VREF<QuaternionLayout> that) {
	return VFat<QuaternionHolder> (QuaternionImplHolder () ,that) ;
}

exports CFat<QuaternionHolder> QuaternionHolder::hold (CREF<QuaternionLayout> that) {
	return CFat<QuaternionHolder> (QuaternionImplHolder () ,that) ;
}

template class External<LinearProcHolder ,LinearProcLayout> ;

exports CREF<OfThis<UniqueRef<LinearProcLayout>>> LinearProcHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<LinearProcLayout>> ret ;
		External<LinearProcHolder ,LinearProcLayout>::declare ().create (ret.mThis) ;
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

template class External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> ;

exports OfThis<AutoRef<PointCloudKDTreeLayout>> PointCloudKDTreeHolder::create () {
	OfThis<AutoRef<PointCloudKDTreeLayout>> ret ;
	External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout>::declare ().create (ret.mThis) ;
	return move (ret) ;
}

exports VFat<PointCloudKDTreeHolder> PointCloudKDTreeHolder::hold (VREF<PointCloudKDTreeLayout> that) {
	return VFat<PointCloudKDTreeHolder> (External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout>::declare () ,that) ;
}

exports CFat<PointCloudKDTreeHolder> PointCloudKDTreeHolder::hold (CREF<PointCloudKDTreeLayout> that) {
	return CFat<PointCloudKDTreeHolder> (External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout>::declare () ,that) ;
}

class PointCloudImplHolder final implement Fat<PointCloudHolder ,PointCloudLayout> {
public:
	void initialize (RREF<Ref<Array<Point2F>>> that) override {
		self.mRank = 2 ;
		auto &&rax = keep[TYPE<Ref<Array<Point2F>>>::expr] (Pointer::from (self.mPointCloud)) ;
		rax = move (that) ;
		self.mWorld = Matrix::identity () ;
	}

	void initialize (RREF<Ref<Array<Point3F>>> that) override {
		self.mRank = 3 ;
		auto &&rax = keep[TYPE<Ref<Array<Point3F>>>::expr] (Pointer::from (self.mPointCloud)) ;
		rax = move (that) ;
		self.mWorld = Matrix::identity () ;
	}

	LENGTH size () const override {
		return self.mPointCloud->size () ;
	}

	void get (CREF<INDEX> index ,VREF<Vector> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mRank != 2)
				discard ;
			item = Vector (keep[TYPE<Point2F>::expr] (self.mPointCloud.deref[index])) ;
			item = self.mWorld * item ;
		}
		if ifdo (act) {
			if (self.mRank != 3)
				discard ;
			item = Vector (keep[TYPE<Point3F>::expr] (self.mPointCloud.deref[index])) ;
			item = self.mWorld * item ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	Vector center () const override {
		Vector ret = Vector::zero () ;
		auto rax = Vector () ;
		for (auto &&i : self.mPointCloud->range ()) {
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
			for (auto &&i : self.mPointCloud->range ()) {
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
		for (auto &&i : self.mPointCloud->range ()) {
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
		ret.mRank = self.mRank ;
		ret.mPointCloud = self.mPointCloud ;
		ret.mWorld *= mat ;
		return move (ret) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor) const override {
		auto &&rax = keep[TYPE<OfThis<AutoRef<PointCloudKDTreeLayout>>>::expr] (self.mKDTree.deref) ;
		if ifdo (TRUE) {
			if (rax.mThis.exist ())
				discard ;
			PointCloudKDTreeHolder::hold (rax)->initialize (self.mPointCloud.deref) ;
		}
		return PointCloudKDTreeHolder::hold (rax)->search (center ,neighbor) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor ,CREF<FLT64> radius) const override {
		auto &&rax = keep[TYPE<OfThis<AutoRef<PointCloudKDTreeLayout>>>::expr] (self.mKDTree.deref) ;
		if ifdo (TRUE) {
			if (rax.mThis.exist ())
				discard ;
			PointCloudKDTreeHolder::hold (rax)->initialize (self.mPointCloud.deref) ;
		}
		return PointCloudKDTreeHolder::hold (rax)->search (center ,neighbor ,radius) ;
	}
} ;

exports VFat<PointCloudHolder> PointCloudHolder::hold (VREF<PointCloudLayout> that) {
	return VFat<PointCloudHolder> (PointCloudImplHolder () ,that) ;
}

exports CFat<PointCloudHolder> PointCloudHolder::hold (CREF<PointCloudLayout> that) {
	return CFat<PointCloudHolder> (PointCloudImplHolder () ,that) ;
}
} ;