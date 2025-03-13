#include "export.h"

#include "csc_end.h"
#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include "csc_begin.h"

namespace CSC3DCV {
class CameraViewImplHolder final implement Fat<CameraViewHolder ,CameraViewLayout> {
public:
	void sfm_view_mat_k (CREF<CameraFrame> frame1 ,CREF<CameraFrame> frame2) override {
		assume (frame1.mView1 == frame2.mView1) ;
		if (fake.mConstMatK)
			return ;
		const auto r1x = Vector (FLT64 (fake.mShape.mCX) ,FLT64 (fake.mShape.mCY) ,0 ,2).projection () ;
		auto mProblem = Box<ceres::Problem>::make () ;
		auto mVariable = Array<FLT64> (1 + 3 + 3) ;
		if ifdo (TRUE) {
			fake.mParam1 = 0 ;
			fake.mParam2 = 1 ;
			fake.mParam3 = 4 ;
		}
		if ifdo (TRUE) {
			const auto view_focal = FLT64 (1000) ;
			mVariable[fake.mParam1] = view_focal ;
			mProblem->AddParameterBlock ((&mVariable[fake.mParam1]) ,1) ;
		}
		if ifdo (TRUE) {
			const auto r3x = Matrix::identity () ;
			const auto r4x = Matrix::identity () ;
			const auto r5x = Solver::encode_rotation (r3x) ;
			const auto r6x = Solver::encode_rotation (r4x) ;
			mVariable[fake.mParam2 + 0] = r5x[0] ;
			mVariable[fake.mParam2 + 1] = r5x[1] ;
			mVariable[fake.mParam2 + 2] = r5x[2] ;
			mProblem->AddParameterBlock ((&mVariable[fake.mParam2]) ,3) ;
			mVariable[fake.mParam3 + 0] = r6x[0] ;
			mVariable[fake.mParam3 + 1] = r6x[1] ;
			mVariable[fake.mParam3 + 2] = r6x[2] ;
			mProblem->AddParameterBlock ((&mVariable[fake.mParam3]) ,3) ;
		}
		const auto r7x = new ceres::TrivialLoss () ;
		for (auto &&i : frame1.mOriginPoint.range ()) {
			const auto r8x = Vector (frame1.mOriginPoint[i]) - r1x ;
			const auto r9x = Vector (frame1.mOriginPoint[i]) - r1x ;
			const auto r10x = ResidualErrorFocal::create (r8x.xyz () ,r9x.xyz ()) ;
			const auto view_focal = (&mVariable[fake.mParam1]) ;
			const auto view1_rot = (&mVariable[fake.mParam2]) ;
			const auto view2_rot = (&mVariable[fake.mParam3]) ;
			mProblem->AddResidualBlock (r10x ,r7x ,view_focal ,view1_rot ,view2_rot) ;
		}
		mOptions.minimizer_progress_to_stdout = TRUE ;
		mOptions.logging_type = ceres::LoggingType::SILENT ;
		ceres::Solve (mOptions ,(&mProblem.self) ,(&mSummary)) ;
		assume (mSummary.initial_cost >= 0) ;
		assume (mSummary.final_cost >= 0) ;
		if ifdo (TRUE) {
			const auto view_focal = mVariable[fake.mParam1] ;
			const auto r11x = PerspectiveMatrix (view_focal ,view_focal ,r1x[0] ,r1x[1]) ;
			fake.mMatK = r11x ;
		}
		Singleton<Console>::instance ().info (Format (slice ("mView[$1].mFocus = $2")) (fake.mName ,mVariable[fake.mParam1])) ;
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
} ;

exports VFat<CameraViewHolder> CameraViewHolder::hold (VREF<CameraViewLayout> that) {
	return VFat<CameraViewHolder> (CameraViewImplHolder () ,that) ;
}

exports CFat<CameraViewHolder> CameraViewHolder::hold (CREF<CameraViewLayout> that) {
	return CFat<CameraViewHolder> (CameraViewImplHolder () ,that) ;
}
} ;
