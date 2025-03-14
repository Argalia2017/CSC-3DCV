#include "export.h"

#include "../Estimation/export.h"

#include "csc_end.h"
#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include "csc_begin.h"

namespace CSC3DCV {
struct Problem_sfm_view_mat_k {
	Box<ceres::Problem> mProblem ;
	Array<FLT64> mVariable ;
	ceres::Solver::Options mOptions ;
	ceres::Solver::Summary mSummary ;
} ;

struct Problem_sfm_view_mat_v {
	Box<ceres::Problem> mProblem ;
	Array<FLT64> mVariable ;
	ceres::Solver::Options mOptions ;
	ceres::Solver::Summary mSummary ;
} ;

class CameraViewImplHolder final implement Fat<CameraViewHolder ,CameraViewLayout> {
public:
	AutoRef<Problem_sfm_view_mat_k> sfm_view_mat_k_problem () override {
		AutoRef<Problem_sfm_view_mat_k> ret = AutoRef<Problem_sfm_view_mat_k>::make () ;
		ret->mProblem = Box<ceres::Problem>::make () ;
		ret->mVariable = Array<FLT64> (1 + 3 + 3) ;
		return move (ret) ;
	}

	void sfm_view_mat_k (CREF<CameraFrame> frame1 ,CREF<CameraFrame> frame2 ,VREF<Problem_sfm_view_mat_k> ctx) override {
		assume (frame1.mView1 == frame2.mView1) ;
		const auto r1x = Vector (FLT64 (self.mShape.mCX) ,FLT64 (self.mShape.mCY) ,0 ,2).projection () ;
		if ifdo (TRUE) {
			self.mParam1 = 0 ;
			self.mParam2 = 1 ;
			self.mParam3 = 4 ;
		}
		if ifdo (TRUE) {
			const auto view_focal = FLT64 (1000) ;
			ctx.mVariable[self.mParam1] = view_focal ;
			ctx.mProblem->AddParameterBlock ((&ctx.mVariable[self.mParam1]) ,1) ;
		}
		if ifdo (TRUE) {
			const auto r3x = Matrix::identity () ;
			const auto r4x = Matrix::identity () ;
			const auto r5x = SolverProc::encode_rotation (r3x) ;
			const auto r6x = SolverProc::encode_rotation (r4x) ;
			ctx.mVariable[self.mParam2 + 0] = r5x[0] ;
			ctx.mVariable[self.mParam2 + 1] = r5x[1] ;
			ctx.mVariable[self.mParam2 + 2] = r5x[2] ;
			ctx.mProblem->AddParameterBlock ((&ctx.mVariable[self.mParam2]) ,3) ;
			ctx.mVariable[self.mParam3 + 0] = r6x[0] ;
			ctx.mVariable[self.mParam3 + 1] = r6x[1] ;
			ctx.mVariable[self.mParam3 + 2] = r6x[2] ;
			ctx.mProblem->AddParameterBlock ((&ctx.mVariable[self.mParam3]) ,3) ;
		}
		const auto r7x = new ceres::TrivialLoss () ;
		for (auto &&i : frame1.mOriginPoint.range ()) {
			const auto r8x = Vector (frame1.mOriginPoint[i]) - r1x ;
			const auto r9x = Vector (frame1.mOriginPoint[i]) - r1x ;
			const auto r10x = ResidualErrorFocal::create (r8x.xyz () ,r9x.xyz ()) ;
			const auto view_focal = (&ctx.mVariable[self.mParam1]) ;
			const auto view1_rot = (&ctx.mVariable[self.mParam2]) ;
			const auto view2_rot = (&ctx.mVariable[self.mParam3]) ;
			ctx.mProblem->AddResidualBlock (r10x ,r7x ,view_focal ,view1_rot ,view2_rot) ;
		}
		ctx.mOptions.minimizer_progress_to_stdout = TRUE ;
		ctx.mOptions.logging_type = ceres::LoggingType::SILENT ;
		ceres::Solve (ctx.mOptions ,(&ctx.mProblem.deref) ,(&ctx.mSummary)) ;
		assume (ctx.mSummary.initial_cost >= 0) ;
		assume (ctx.mSummary.final_cost >= 0) ;
		if ifdo (TRUE) {
			const auto view_focal = ctx.mVariable[self.mParam1] ;
			const auto r11x = PerspectiveMatrix (view_focal ,view_focal ,r1x[0] ,r1x[1]) ;
			self.mMatK = r11x ;
		}
		Singleton<Console>::instance ().info (Format (slice ("mView[$1].mFocus = $2")) (self.mName ,self.mMatK[0][0])) ;
	}

	struct ResidualErrorFocal {
		Point2F mPoint1 ;
		Point2F mPoint2 ;

		explicit ResidualErrorFocal (CREF<Point2F> point1 ,CREF<Point2F> point2)
			:mPoint1 (point1) ,mPoint2 (point2) {}

		template <typename T>
		BOOL operator() (const T *view_focal ,const T *view1_rot ,const T *view2_rot ,T *residuals) const {
			auto rax = Buffer3<T> () ;
			rax[0] = T (mPoint1.mX) ;
			rax[1] = T (mPoint1.mY) ;
			rax[2] = T (view_focal[0]) ;
			const auto r1x = ceres::sqrt (rax[0] * rax[0] + rax[1] * rax[1] + rax[2] * rax[2]) ;
			rax[0] /= r1x ;
			rax[1] /= r1x ;
			rax[2] /= r1x ;
			const auto r2x = rax ;
			ceres::AngleAxisRotatePoint (view1_rot ,(&r2x[0]) ,(&rax[0])) ;
			auto rbx = Buffer3<T> () ;
			rbx[0] = T (mPoint2.mX) ;
			rbx[1] = T (mPoint2.mY) ;
			rbx[2] = T (view_focal[0]) ;
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
			return new ceres::AutoDiffCostFunction<ResidualErrorFocal ,2 ,1 ,3 ,3> (r1x) ;
		}
	} ;

	void sfm_view_mat_v (CREF<CameraFrame> frame1 ,CREF<CameraFrame> frame2) override {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_view_mat_v")) ;
		const auto r1x = invoke ([&] () {
			Array<Point3F> ret = Array<Point3F> (frame1.mOriginPoint.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r2x = Vector (frame1.mOriginPoint[i]) ;
				const auto r3x = (mView[view1].mMatK[1] * r2x).normalize () ;
				ret[i] = r3x.xyz () ;
			}
			return move (ret) ;
		}) ;
		const auto r4x = invoke ([&] () {
			Array<Point3F> ret = Array<Point3F> (frame2.mOriginPoint.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r5x = Vector (frame2.mOriginPoint[i]) ;
				const auto r6x = (mView[view2].mMatK[1] * r5x).normalize () ;
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
		const auto r14x = Image<FLT64> (Solver::solve_lsm (r11x)) ;
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
		const auto r18x = Solver::decompose_epipolar (r1x ,r4x ,r17x) ;
		const auto r19x = r18x.mT * Vector::axis_w () ;
		const auto r20x = r19x.homogenize ().normalize () ;
		const auto r21x = r20x * mView[view1].mBaseLine ;
		const auto r22x = TranslationMatrix (r21x) ;
		const auto r23x = r22x * r18x.mR ;
		mView[view1].mMatV = Matrix::identity () ;
		mView[view2].mMatV = r23x ;
		Singleton<Console>::instance ().info (Format (slice ("mView[$1].mBaseLine = $2")) (mView[view1].mName ,mView[view1].mBaseLine)) ;
		Singleton<Console>::instance ().info (Format (slice ("mView[$1].mTranslation = [$2 ,$3 ,$4]")) (mView[view2].mName ,r19x[0] ,r19x[1] ,r19x[2])) ;
	}
} ;

exports VFat<CameraViewHolder> CameraViewHolder::hold (VREF<CameraViewLayout> that) {
	return VFat<CameraViewHolder> (CameraViewImplHolder () ,that) ;
}

exports CFat<CameraViewHolder> CameraViewHolder::hold (CREF<CameraViewLayout> that) {
	return CFat<CameraViewHolder> (CameraViewImplHolder () ,that) ;
}
} ;
