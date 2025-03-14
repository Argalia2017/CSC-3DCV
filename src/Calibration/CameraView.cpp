#include "export.h"

#include "../Estimation/export.h"

#include "csc_end.h"
#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include "csc_begin.h"

namespace CSC3DCV {
struct CameraViewContext {
	Box<ceres::Problem> mProblem ;
	Array<FLT64> mVariable ;
	ceres::Solver::Options mOptions ;
	ceres::Solver::Summary mSummary ;
} ;

class CameraViewImplHolder final implement Fat<CameraViewHolder ,CameraViewLayout> {
public:
	void sfm_view_mat_k (CREF<ModelFrame> frame1 ,CREF<ModelFrame> frame2) override {
		auto &&view1 = self[frame1.mView1] ;
		auto &&view2 = self[frame2.mView1] ;
		const auto r14x = Vector (FLT64 (view1.mShape.mCX) ,FLT64 (view1.mShape.mCY) ,0 ,2).projection () ;
		const auto r15x = Vector (FLT64 (view2.mShape.mCX) ,FLT64 (view2.mShape.mCY) ,0 ,2).projection () ;
		if ifdo (TRUE) {
			if (self.mContext.exist ())
				discard ;
			self.mContext = AutoRef<CameraViewContext>::make () ;
			self.mContext->mProblem = Box<ceres::Problem>::make () ;
			self.mContext->mVariable = Array<FLT64> (8) ;
		}
		auto &&ctx = self.mContext.deref ;
		view1.mParam1 = 0 ;
		view1.mParam2 = 1 ;
		view2.mParam1 = 4 ;
		view2.mParam2 = 5 ;
		if ifdo (TRUE) {
			const auto view1_focal = FLT64 (1000) ;
			const auto view2_focal = FLT64 (1000) ;
			ctx.mVariable[view1.mParam1] = view1_focal ;
			ctx.mVariable[view2.mParam1] = view2_focal ;
			ctx.mProblem->AddParameterBlock ((&ctx.mVariable[view1.mParam1]) ,1) ;
			ctx.mProblem->AddParameterBlock ((&ctx.mVariable[view2.mParam1]) ,1) ;
		}
		if ifdo (TRUE) {
			const auto r2x = Matrix::identity () ;
			const auto r3x = Matrix::identity () ;
			const auto r4x = SolverProc::encode_rotation (r2x) ;
			const auto r5x = SolverProc::encode_rotation (r3x) ;
			ctx.mVariable[view1.mParam2 + 0] = r4x[0] ;
			ctx.mVariable[view1.mParam2 + 1] = r4x[1] ;
			ctx.mVariable[view1.mParam2 + 2] = r4x[2] ;
			ctx.mVariable[view2.mParam2 + 0] = r5x[0] ;
			ctx.mVariable[view2.mParam2 + 1] = r5x[1] ;
			ctx.mVariable[view2.mParam2 + 2] = r5x[2] ;
			ctx.mProblem->AddParameterBlock ((&ctx.mVariable[view1.mParam2]) ,3) ;
			ctx.mProblem->AddParameterBlock ((&ctx.mVariable[view2.mParam2]) ,3) ;
		}
		const auto r6x = new ceres::TrivialLoss () ;
		for (auto &&i : frame1.mOriginPoint.range ()) {
			const auto r7x = Vector (frame1.mOriginPoint[i]) - r14x ;
			const auto r8x = Vector (frame2.mOriginPoint[i]) - r15x ;
			const auto r9x = ResidualErrorFocal::create (r7x.xyz () ,r8x.xyz ()) ;
			const auto view1_focal = (&ctx.mVariable[view1.mParam1]) ;
			const auto view2_focal = (&ctx.mVariable[view2.mParam1]) ;
			const auto view1_rot = (&ctx.mVariable[view1.mParam2]) ;
			const auto view2_rot = (&ctx.mVariable[view2.mParam2]) ;
			ctx.mProblem->AddResidualBlock (r9x ,r6x ,view1_focal ,view2_focal ,view1_rot ,view2_rot) ;
		}
		ctx.mOptions.minimizer_progress_to_stdout = TRUE ;
		ctx.mOptions.logging_type = ceres::LoggingType::SILENT ;
		ceres::Solve (ctx.mOptions ,(&ctx.mProblem.deref) ,(&ctx.mSummary)) ;
		assume (ctx.mSummary.initial_cost >= 0) ;
		assume (ctx.mSummary.final_cost >= 0) ;
		if ifdo (TRUE) {
			const auto view1_focal = ctx.mVariable[view1.mParam1] ;
			const auto view2_focal = ctx.mVariable[view2.mParam1] ;
			view1.mMatK = PerspectiveMatrix (view1_focal ,view1_focal ,r14x[0] ,r14x[1]) ;
			view2.mMatK = PerspectiveMatrix (view2_focal ,view2_focal ,r15x[0] ,r15x[1]) ;
		}
		const auto r11x = view1.mMatK[0][0][0] ;
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mFocus = $2")) (view1.mName ,r11x)) ;
	}

	struct ResidualErrorFocal {
		Point2F mPoint1 ;
		Point2F mPoint2 ;

		explicit ResidualErrorFocal (CREF<Point2F> point1 ,CREF<Point2F> point2)
			:mPoint1 (point1) ,mPoint2 (point2) {}

		template <typename T>
		BOOL operator() (const T *view1_focal ,const T *view2_focal ,const T *view1_rot ,const T *view2_rot ,T *residuals) const {
			auto rax = Buffer3<T> () ;
			rax[0] = T (mPoint1.mX) ;
			rax[1] = T (mPoint1.mY) ;
			rax[2] = T (view1_focal[0]) ;
			const auto r1x = ceres::sqrt (rax[0] * rax[0] + rax[1] * rax[1] + rax[2] * rax[2]) ;
			rax[0] /= r1x ;
			rax[1] /= r1x ;
			rax[2] /= r1x ;
			const auto r2x = rax ;
			ceres::AngleAxisRotatePoint (view1_rot ,(&r2x[0]) ,(&rax[0])) ;
			auto rbx = Buffer3<T> () ;
			rbx[0] = T (mPoint2.mX) ;
			rbx[1] = T (mPoint2.mY) ;
			rbx[2] = T (view2_focal[0]) ;
			const auto r3x = ceres::sqrt (rbx[0] * rbx[0] + rbx[1] * rbx[1] + rbx[2] * rbx[2]) ;
			rbx[0] /= r3x ;
			rbx[1] /= r3x ;
			rbx[2] /= r3x ;
			const auto r4x = rbx ;
			ceres::AngleAxisRotatePoint (view2_rot ,(&r4x[0]) ,(&rbx[0])) ;
			const auto r5x = rax[2] * rbx[0] - rax[0] * rbx[2] ;
			const auto r6x = rax[0] * rbx[1] - rax[1] * rbx[0] ;
			residuals[0] = r5x * rax[1] + r6x * rax[2] ;
			residuals[1] = r5x * rbx[1] + r6x * rbx[2] ;
			return TRUE ;
		}

		static INDEX num_residuals () {
			return 2 ;
		}

		static DEF<ceres::CostFunction *> create (CREF<Point2F> point1 ,CREF<Point2F> point2) {
			const auto r1x = new ResidualErrorFocal (point1 ,point2) ;
			return new ceres::AutoDiffCostFunction<ResidualErrorFocal ,2 ,1 ,1 ,3 ,3> (r1x) ;
		}
	} ;

	void sfm_view_mat_v (CREF<ModelFrame> frame1 ,CREF<ModelFrame> frame2) override {
		auto &&view1 = self[frame1.mView1] ;
		auto &&view2 = self[frame2.mView1] ;
		const auto r1x = invoke ([&] () {
			Array<Point3F> ret = Array<Point3F> (frame1.mOriginPoint.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r2x = Vector (frame1.mOriginPoint[i]) ;
				const auto r3x = (view1.mMatK[1] * r2x).normalize () ;
				ret[i] = r3x.xyz () ;
			}
			return move (ret) ;
		}) ;
		const auto r4x = invoke ([&] () {
			Array<Point3F> ret = Array<Point3F> (frame2.mOriginPoint.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r5x = Vector (frame2.mOriginPoint[i]) ;
				const auto r6x = (view2.mMatK[1] * r5x).normalize () ;
				ret[i] = r6x.xyz () ;
			}
			return move (ret) ;
		}) ;
		const auto r7x = PointCloud (Ref<Array<Point3F>>::reference (r1x)).svd_matrix () ;
		const auto r8x = PointCloud (Ref<Array<Point3F>>::reference (r4x)).svd_matrix () ;
		const auto r9x = DuplexMatrix (Matrix::identity ()) ;
		const auto r10x = DuplexMatrix (Matrix::identity ()) ;
		const auto r11x = invoke ([&] () {
			Image<FLT64> ret = Image<FLT64> (9 ,INDEX (r1x.size ())) ;
			for (auto &&i : iter (0 ,r1x.size ())) {
				const auto r12x = r9x[1] * Vector (r1x[i]) ;
				const auto r13x = r10x[1] * Vector (r4x[i]) ;
				if ifdo (TRUE) {
					INDEX ix = i ;
					ret[ix][0] = r12x[0] * r13x[0] ;
					ret[ix][1] = r12x[0] * r13x[1] ;
					ret[ix][2] = r12x[0] * r13x[2] ;
					ret[ix][3] = r12x[1] * r13x[0] ;
					ret[ix][4] = r12x[1] * r13x[1] ;
					ret[ix][5] = r12x[1] * r13x[2] ;
					ret[ix][6] = r12x[2] * r13x[0] ;
					ret[ix][7] = r12x[2] * r13x[1] ;
					ret[ix][8] = r12x[2] * r13x[2] ;
				}
			}
			return move (ret) ;
		}) ;
		const auto r14x = Image<FLT64> (LinearProc::solve_lsm (r11x)) ;
		const auto r15x = invoke ([&] () {
			Matrix ret = Matrix::zero () ;
			ret[0][0] = r14x[0][0] ;
			ret[0][1] = r14x[1][0] ;
			ret[0][2] = r14x[2][0] ;
			ret[1][0] = r14x[3][0] ;
			ret[1][1] = r14x[4][0] ;
			ret[1][2] = r14x[5][0] ;
			ret[2][0] = r14x[6][0] ;
			ret[2][1] = r14x[7][0] ;
			ret[2][2] = r14x[8][0] ;
			return move (ret) ;
		}) ;
		const auto r16x = r9x[1].transpose () * r15x * r10x[1] ;
		const auto r17x = r16x ;
		const auto r18x = decompose_epipolar (r1x ,r4x ,r17x) ;
		const auto r19x = r18x.mT * Vector::axis_w () ;
		const auto r20x = r19x.homogenize ().normalize () ;
		const auto r21x = r20x * view1.mBaseLine ;
		const auto r22x = TranslationMatrix (r21x) ;
		const auto r23x = r22x * r18x.mR ;
		view2.mMatV = r23x ;
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mBaseLine = $2")) (view1.mName ,view1.mBaseLine)) ;
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mTranslation = [$2 ,$3 ,$4]")) (view2.mName ,r19x[0] ,r19x[1] ,r19x[2])) ;
	}

	KRTResult decompose_epipolar (CREF<Array<Point3F>> point1 ,CREF<Array<Point3F>> point2 ,CREF<Matrix> mat_e) const {
		KRTResult ret ;
		const auto r1x = invoke ([&] () {
			Array<Buffer2<FLT64>> ret = Array<Buffer2<FLT64>> (4) ;
			ret[0][0] = -1 ;
			ret[0][1] = -1 ;
			ret[1][0] = +1 ;
			ret[1][1] = -1 ;
			ret[2][0] = -1 ;
			ret[2][1] = +1 ;
			ret[3][0] = +1 ;
			ret[3][1] = +1 ;
			return move (ret) ;
		}) ;
		const auto r2x = MatrixProc::solve_svd (mat_e) ;
		auto rax = FLT64 (0) ;
		for (auto &&i : iter (0 ,r1x.size ())) {
			const auto r3x = invoke ([&] () {
				Matrix ret = Matrix::identity () ;
				ret[0][0] = 0 ;
				ret[0][1] = -r1x[i][0] ;
				ret[1][0] = r1x[i][0] ;
				ret[1][1] = 0 ;
				return move (ret) ;
			}) ;
			const auto r4x = r2x.mU * Vector::axis_z () * r1x[i][1] ;
			const auto r5x = r2x.mU * r3x * r2x.mV.transpose () ;
			const auto r6x = MathProc::inverse (r5x.det ()) ;
			const auto r7x = r5x * DiagMatrix (r6x ,r6x ,r6x) ;
			const auto r8x = invoke ([&] () {
				FLT64 ret = 0 ;
				for (auto &&i : iter (0 ,point1.size ())) {
					const auto r9x = Vector (point1[i]).homogenize () ;
					const auto r10x = Vector (point2[i]).homogenize () ;
					const auto r11x = -(r7x * r10x).normalize () ;
					const auto r12x = Matrix (r9x ,r11x ,r9x ^ r11x ,Vector::axis_w ()) ;
					const auto r13x = r12x.inverse () * r4x ;
					if (r13x[0] <= 0)
						continue ;
					if (r13x[1] <= 0)
						continue ;
					ret += 1 ;
				}
				return move (ret) ;
			}) ;
			if (rax >= r8x)
				continue ;
			ret.mK = CrossProductMatrix (r4x) ;
			ret.mT = TranslationMatrix (r4x) ;
			ret.mR = r7x ;
			rax = r8x ;
		}
		assume (rax > 0) ;
		return move (ret) ;
	}
} ;

exports VFat<CameraViewHolder> CameraViewHolder::hold (VREF<CameraViewLayout> that) {
	return VFat<CameraViewHolder> (CameraViewImplHolder () ,that) ;
}

exports CFat<CameraViewHolder> CameraViewHolder::hold (CREF<CameraViewLayout> that) {
	return CFat<CameraViewHolder> (CameraViewImplHolder () ,that) ;
}
} ;
