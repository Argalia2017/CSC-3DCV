#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_matrix.hpp"

#include "csc_end.h"
#ifdef __CSC_COMPILER_MSVC__
#define EIGEN_HAS_STD_RESULT_OF 0
//@warn: actually eigen do not have 'EIGEN_HAS_STD_NEGATORS' ,you need merge 'csc_matrix.eigen.fix.h' to 'Eigen/src/Core/functors/StlFunctors.h'
#define EIGEN_HAS_STD_NEGATORS 0
#endif

#include <Eigen/Dense>
#include "csc_begin.h"

namespace CSC {
struct MatrixProcImplLayout {} ;

class MatrixProcImplHolder final implement Fat<MatrixProcHolder ,MatrixProcImplLayout> {
public:
	void initialize () override {
		noop () ;
	}

	TRSResult solve_trs (CREF<Matrix> a) const override {
		TRSResult ret ;
		const auto r1x = a * Vector::axis_x () ;
		const auto r2x = a * Vector::axis_y () ;
		const auto r3x = a * Vector::axis_z () ;
		const auto r4x = a * Vector::axis_w () ;
		const auto r5x = Matrix (r1x.normalize () ,r2x.normalize () ,r3x.normalize () ,Vector::axis_w ()) ;
		ret.mT = TranslationMatrix (r4x) ;
		ret.mR = Quaternion (r5x).matrix () ;
		ret.mS = DiagMatrix (r1x.magnitude () ,r2x.magnitude () ,r3x.magnitude ()) ;
		return move (ret) ;
	}

	KRTResult solve_krt (CREF<Matrix> a) const override {
		KRTResult ret ;
		ret.mK = a.homogenize () ;
		ret.mR = Matrix::identity () ;
		ret.mT = Matrix::identity () ;
		auto rax = TRUE ;
		while (TRUE) {
			rax = FALSE ;
			if ifdo (TRUE) {
				if (MathProc::inverse (ret.mK[1][0]) == 0)
					discard ;
				const auto r1x = ret.mK[1][0] ;
				const auto r2x = ret.mK[1][1] ;
				const auto r3x = MathProc::inverse (Vector (r1x ,r2x ,0 ,0).magnitude ()) ;
				const auto r4x = invoke ([&] () {
					Matrix ret = Matrix::identity () ;
					ret[0][0] = r2x * r3x ;
					ret[1][1] = ret[0][0] ;
					ret[0][1] = r1x * r3x ;
					ret[1][0] = -ret[0][1] ;
					return move (ret) ;
				}) ;
				ret.mK = ret.mK * r4x ;
				ret.mR = r4x.transpose () * ret.mR ;
				rax = TRUE ;
			}
			if ifdo (TRUE) {
				if (MathProc::inverse (ret.mK[2][0]) == 0)
					discard ;
				const auto r5x = ret.mK[2][0] ;
				const auto r6x = ret.mK[2][2] ;
				const auto r7x = MathProc::inverse (Vector (r5x ,r6x ,0 ,0).magnitude ()) ;
				const auto r8x = invoke ([&] () {
					Matrix ret = Matrix::identity () ;
					ret[0][0] = r6x * r7x ;
					ret[2][2] = ret[0][0] ;
					ret[0][2] = r5x * r7x ;
					ret[2][0] = -ret[0][2] ;
					return move (ret) ;
				}) ;
				ret.mK = ret.mK * r8x ;
				ret.mR = r8x.transpose () * ret.mR ;
				rax = TRUE ;
			}
			if ifdo (TRUE) {
				if (MathProc::inverse (ret.mK[2][1]) == 0)
					discard ;
				const auto r9x = ret.mK[2][1] ;
				const auto r10x = ret.mK[2][2] ;
				const auto r11x = MathProc::inverse (Vector (r9x ,r10x ,0 ,0).magnitude ()) ;
				const auto r12x = invoke ([&] () {
					Matrix ret = Matrix::identity () ;
					ret[1][1] = r10x * r11x ;
					ret[2][2] = ret[1][1] ;
					ret[1][2] = r9x * r11x ;
					ret[2][1] = -ret[1][2] ;
					return move (ret) ;
				}) ;
				ret.mK = ret.mK * r12x ;
				ret.mR = r12x.transpose () * ret.mR ;
				rax = TRUE ;
			}
			if (!rax)
				break ;
		}
		return move (ret) ;
	}

	SVDResult solve_svd (CREF<Matrix> a) const override {
		SVDResult ret ;
		const auto r1x = cvt_eigen_matrix (a) ;
		auto rax = Eigen::JacobiSVD<Eigen::Matrix4d> (r1x ,Eigen::ComputeFullU | Eigen::ComputeFullV) ;
		ret.mU = cvt_csc_matrix (rax.matrixU ()) ;
		const auto r2x = Eigen::Vector4d (rax.singularValues ()) ;
		ret.mS = DiagMatrix (r2x[0] ,r2x[1] ,r2x[2] ,r2x[3]) ;
		ret.mV = cvt_csc_matrix (rax.matrixV ()) ;
		return move (ret) ;
	}

	Eigen::Matrix4d cvt_eigen_matrix (CREF<Matrix> a) const {
		Eigen::Matrix4d ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4))
			ret (i.mY ,i.mX) = a[i] ;
		return move (ret) ;
	}

	Matrix cvt_csc_matrix (CREF<Eigen::Matrix4d> a) const {
		Matrix ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			ret[i] = a (i.mY ,i.mX) ;
			assume (!isnan (ret[i])) ;
		}
		return move (ret) ;
	}
} ;

static const auto mMatrixProcExternal = External<MatrixProcHolder ,MatrixProcLayout> (MatrixProcImplHolder ()) ;

struct LinearProcImplLayout {} ;

class LinearProcImplHolder final implement Fat<LinearProcHolder ,LinearProcImplLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Image<FLT64> solve_lsm (CREF<Image<FLT64>> a) const override {
		Image<FLT64> ret = Image<FLT64> (1 ,a.cx ()) ;
		const auto r1x = cvt_eigen_matrix (a) ;
		const auto r2x = Eigen::ComputeFullV ;
		auto rax = Eigen::JacobiSVD<Eigen::MatrixXd> (r1x ,r2x) ;
		INDEX ix = MathProc::min_of (INDEX (rax.rank ()) ,a.cx () - 1) ;
		assume (ix >= 0) ;
		const auto r3x = cvt_csc_matrix (rax.matrixV ()) ;
		for (auto &&i : iter (0 ,ret.cy ()))
			ret[i][0] = r3x[i][ix] ;
		return move (ret) ;
	}

	Image<FLT64> solve_lsm (CREF<Image<FLT64>> a ,CREF<Image<FLT64>> b) const override {
		const auto r1x = cvt_eigen_matrix (a) ;
		const auto r2x = cvt_eigen_matrix (b) ;
		const auto r3x = Eigen::MatrixXd (r1x.transpose () * r1x) ;
		const auto r4x = Eigen::MatrixXd (r1x.transpose () * r2x) ;
		const auto r5x = Eigen::ComputeThinU | Eigen::ComputeThinV ;
		auto rax = Eigen::JacobiSVD<Eigen::MatrixXd> (r3x ,r5x) ;
		const auto r6x = rax.solve (r4x) ;
		return cvt_csc_matrix (r6x) ;
	}

	Image<FLT64> solve_inv (CREF<Image<FLT64>> a) const override {
		const auto r1x = cvt_eigen_matrix (a) ;
		const auto r2x = Eigen::MatrixXd (r1x.completeOrthogonalDecomposition ().pseudoInverse ()) ;
		return cvt_csc_matrix (r2x) ;
	}

	Eigen::MatrixXd cvt_eigen_matrix (CREF<Image<FLT64>> a) const {
		Eigen::MatrixXd ret = Eigen::MatrixXd (a.cy () ,a.cx ()) ;
		for (auto &&i : a.range ())
			ret (i.mY ,i.mX) = a[i] ;
		return move (ret) ;
	}

	Image<FLT64> cvt_csc_matrix (CREF<Eigen::MatrixXd> a) const {
		Image<FLT64> ret = Image<FLT64> (a.cols () ,a.rows ()) ;
		for (auto &&i : ret.range ())
			ret[i] = a (i.mY ,i.mX) ;
		return move (ret) ;
	}
} ;

static const auto mLinearProcExternal = External<LinearProcHolder ,LinearProcLayout> (LinearProcImplHolder ()) ;
} ;