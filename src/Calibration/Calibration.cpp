#include "export.h"

#include "../Common/export.h"
#include "../Detection/export.h"

namespace CSC3DCV {
struct CalibrationImplLayout {
	Path mDataPath ;
	Random mRandom ;
	WorkThread mWorkThread ;
	INDEX mLFrameView ;
	INDEX mRFrameView ;
	ArrayList<CameraView> mView ;
	Set<String<STR>> mViewNameSet ;
	ArrayList<CameraPose> mPose ;
	Set<String<STR>> mPoseNameSet ;
	ArrayList<CameraFrame> mFrame ;
	Set<Pixel> mFramePixelSet ;
	ArrayList<CameraBlock> mBlock ;
	Board mBoard ;
	Feature mFeature ;
} ;

class CalibrationImplHolder final implement Fat<CalibrationHolder ,CalibrationImplLayout> {
public:
	void initialize () override {
		fake.mDataPath = Global<Path> (slice ("mDataPath")).fetch () ;
		fake.mRandom = CurrentRandom () ;
		fake.mWorkThread = WorkThread (NULL) ;
	}

	void execute () override {
		load_data_json () ;
		work_frame_detection () ;
		work_sfm_view_mat_k () ;
		work_sfm_view_mat_v () ;
		work_sfm_pose_mat_v () ;
		work_bundle_adjustment () ;
	}

	void load_data_json () {
		const auto r1x = fake.mDataPath.child (slice ("data.json")) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!r1x.is_file ())
				discard ;
			const auto r2x = JsonParser (FileProc::load_file (r1x)) ;
			load_calib_json_view (r2x) ;
			load_calib_json_pose (r2x) ;
		}
		if ifdo (act) {
			const auto r1x = fake.mDataPath.child (slice ("image")).list () ;
			auto rax = Regex (slice ("(\w+)_(\w+)")) ;
			for (auto &&i : r1x) {
				if (i.extension () != slice (".bmp"))
					continue ;
				const auto r2x = i.stem () ;
				const auto r3x = rax.search (Ref<String<STR>>::reference (r2x) ,0) ;
				if (r3x == NONE)
					continue ;
				const auto r4x = String<STR> (rax.match (1)) ;
				const auto r5x = String<STR> (rax.match (2)) ;
				INDEX ix = insert_new_pose (r4x ,slice ("image")) ;
				INDEX iy = insert_new_view (r5x ,slice ("image")) ;
				fake.mPose[ix].mUseView.add (iy) ;
				fake.mView[iy].mUsePose.add (ix) ;
			}
		}
		const auto r10x = Array<INDEX>::make (fake.mViewNameSet.range ()) ;
		assume (r10x.length () >= 2) ;
		fake.mLFrameView = r10x[0] ;
		fake.mRFrameView = r10x[1] ;
		for (auto &&i : fake.mPose.range ()) {
			for (auto &&j : fake.mPose[i].mUseView) {
				INDEX ix = insert_new_frame (i ,j) ;
				if ifdo (TRUE) {
					if (j != fake.mLFrameView)
						discard ;
					fake.mPose[i].mLFrame = ix ;
				}
				if ifdo (TRUE) {
					if (j != fake.mRFrameView)
						discard ;
					fake.mPose[i].mRFrame = ix ;
				}
			}
		}
		fake.mFrame.remap () ;
	}

	void load_calib_json_view (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mView")).list () ;
		fake.mView = ArrayList<CameraView> (r1x.length ()) ;
		for (auto &&i : fake.mView.range ()) {
			fake.mView[i].mName = r1x[i].child (slice ("mName")).parse (String<STR> ()) ;
			fake.mView[i].mGroup = r1x[i].child (slice ("mGroup")).parse (String<STR> ()) ;
			fake.mView[i].mUsePose = Set<INDEX> () ;
			const auto r2x = r1x[i].child (slice ("mWidth")) ;
			fake.mView[i].mWidth.mCX = r2x.child (0).parse (ZERO) ;
			fake.mView[i].mWidth.mCY = r2x.child (1).parse (ZERO) ;
			const auto r3x = r1x[i].child (slice ("mMatK")).parse (FLT64 (0) ,4) ;
			fake.mView[i].mMatK = PerspectiveMatrix (r3x[0] ,r3x[1] ,r3x[3] ,r3x[4]) ;
			fake.mView[i].mConstMatK = TRUE ;
			fake.mView[i].mDist = r1x[i].child (slice ("mDist")).parse (FLT64 (0) ,5) ;
			fake.mView[i].mConstMatV = TRUE ;
			fake.mView[i].mBaseLine = r1x[i].child (slice ("mBaseLine")).parse (FLT64 (0)) ;
			fake.mView[i].mRelative = r1x[i].child (slice ("mRelative")).parse (FLT64 (0)) ;
		}
		fake.mView.remap () ;
	}

	void load_calib_json_pose (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mPose")).list () ;
		fake.mPose = ArrayList<CameraPose> (r1x.length ()) ;
		for (auto &&i : fake.mPose.range ()) {
			fake.mPose[i].mName = r1x[i].child (slice ("mName")).parse (String<STR> ()) ;
			fake.mPose[i].mGroup = r1x[i].child (slice ("mGroup")).parse (String<STR> ()) ;
			const auto r2x = r1x[i].child (slice ("mColor")).parse (String<STR> ()) ;
			fake.mPose[i].mColor = StringParse<Color3B>::make (r2x) ;
			fake.mPose[i].mUseView = Set<INDEX> () ;
			fake.mPose[i].mLFrame = NONE ;
			fake.mPose[i].mRFrame = NONE ;
			const auto r3x = r1x[i].child (slice ("mMatV")).parse (FLT64 (0) ,7) ;
			fake.mPose[i].mMatV = AffineMatrix (r3x) ;
			fake.mPose[i].mUsingMatV = TRUE ;
			fake.mPose[i].mWeight = 1 ;
			const auto r4x = r1x[i].child (slice ("mMatH")).parse (FLT64 (0) ,16) ;
			fake.mPose[i].mMatH = AffineMatrix (r4x) ;
			fake.mPose[i].mUsingMatH = TRUE ;
			const auto r5x = r1x[i].child (slice ("mError")) ;
			fake.mPose[i].mError.mMaxError = r5x.child (0).parse (FLT64 (0)) ;
			fake.mPose[i].mError.mAvgError = r5x.child (1).parse (FLT64 (0)) ;
			fake.mPose[i].mError.mStdError = r5x.child (2).parse (FLT64 (0)) ;
		}
		fake.mPose.remap () ;
	}

	INDEX insert_new_view (CREF<String<STR>> name ,CREF<String<STR>> group) {
		INDEX ret = fake.mViewNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			ret = fake.mView.insert () ;
			fake.mView[ret].mName = name ;
			fake.mView[ret].mGroup = group ;
			fake.mView[ret].mUsePose = Set<INDEX> () ;
			fake.mView[ret].mWidth.mCX = ZERO ;
			fake.mView[ret].mWidth.mCY = ZERO ;
			fake.mView[ret].mConstMatK = FALSE ;
			fake.mView[ret].mConstMatV = FALSE ;
			fake.mView[ret].mBaseLine = 1 ;
			fake.mView[ret].mRelative = 1 ;
		}
		return move (ret) ;
	}

	INDEX insert_new_pose (CREF<String<STR>> name ,CREF<String<STR>> group) {
		INDEX ret = fake.mPoseNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			ret = fake.mPose.insert () ;
			fake.mPose[ret].mName = name ;
			fake.mPose[ret].mGroup = group ;
			fake.mPose[ret].mColor = random_color (fake.mRandom) ;
			fake.mPose[ret].mUseView = Set<INDEX> () ;
			fake.mPose[ret].mLFrame = NONE ;
			fake.mPose[ret].mRFrame = NONE ;
			fake.mPose[ret].mUsingMatV = FALSE ;
			fake.mPose[ret].mWeight = 1 ;
			fake.mPose[ret].mUsingMatH = FALSE ;
		}
		return move (ret) ;
	}

	inline Color3B random_color (CREF<Random> random) {
		Color3B ret ;
		ret.mB = BYTE (random.random_value (0 ,255)) ;
		ret.mG = BYTE (random.random_value (0 ,255)) ;
		ret.mR = BYTE (random.random_value (0 ,255)) ;
		return move (ret) ;
	}

	INDEX insert_new_frame (CREF<INDEX> pose1 ,CREF<INDEX> view1) {
		const auto r1x = Pixel ({pose1 ,view1}) ;
		INDEX ret = fake.mFramePixelSet.map (r1x) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			ret = fake.mFrame.insert () ;
			fake.mFrame[ret].mPose1 = pose1 ;
			fake.mFrame[ret].mView1 = view1 ;
			fake.mFrame[ret].mTime1 = 0 ;
			const auto r10x = Format (slice ("$1_$2.bmp")) ;
			const auto r11x = fake.mDataPath.child (r10x (fake.mPose[pose1].mName ,fake.mView[view1].mName)) ;
			fake.mFrame[ret].mImageFile = r11x ;
			fake.mFrame[ret].mImage = Image<Color3B> () ;
			fake.mFrame[ret].mDepth = Image<FLT32> () ;
			fake.mFrame[ret].mUndistortion = FALSE ;
			fake.mFrame[ret].mOriginPoint = Array<Point2F> () ;
			fake.mFrame[ret].mUndistPoint = Array<Point2F> () ;
		}
		return move (ret) ;
	}

	void work_frame_detection () {
		work_frame_with_board () ;
		work_frame_with_phase () ;
		work_frame_with_superpoint () ;
		work_frame_undistortion () ;
	}

	void work_frame_with_board () {

	}

	void work_frame_with_phase () {

	}

	void work_frame_with_superpoint () {
		fake.mFeature = Feature (NULL) ;
		if ifdo (TRUE) {
			fake.mWorkThread.start ([&] (CREF<INDEX> frame1) {
				fake.mFrame[frame1].mImage = ImageProc::load_image (fake.mFrame[frame1].mImageFile) ;
				fake.mFrame[frame1].mKeypoint = mFeature.detect (fake.mFrame[frame1].mImage) ;
			}) ;
			fake.mWorkThread.post (0 ,fake.mFrame.length ()) ;
			fake.mWorkThread.join () ;
			fake.mWorkThread.stop () ;
		}
		if ifdo (TRUE) {
			fake.mWorkThread.start ([&] (CREF<INDEX> pose1) {
				INDEX ix = fake.mPose[pose1].mLFrame ;
				INDEX iy = fake.mPose[pose1].mRFrame ;
				if (ix == NONE)
					return ;
				if (iy == NONE)
					return ;
				fake.mFrame[ix].mOriginPoint = mFeature.match (fake.mFrame[ix].mKeypoint ,fake.mFrame[iy].mKeypoint) ;
			}) ;
			fake.mWorkThread.post (0 ,fake.mPose.length ()) ;
			fake.mWorkThread.join () ;
			fake.mWorkThread.stop () ;
		}
		for (auto &&i : fake.mFrame.range ()) {
			assume (fake.mFrame[i].mOriginPoint.size () > 0) ;
		}
	}

	void work_frame_undistortion () {
		if ifdo (TRUE) {
			fake.mWorkThread.start ([&] (CREF<INDEX> frame1) {
				INDEX ix = fake.mFrame[frame1].mView1 ;
				if (!fake.mView[ix].mConstMatK)
					return ;
				if (!fake.mView[ix].mConstDist)
					return ;
				if (fake.mFrame[frame1].mUndistortion)
					return ;
				for (auto &&i : fake.mFrame[frame1].mOriginPoint.range ()) {
					const auto r1x = Vector (fake.mFrame[frame1].mOriginPoint[i]) ;
					const auto r2x = Solver::undistortion (fake.mView[ix].mMatK ,fake.mView[ix].mDist ,r1x) ;
					fake.mFrame[frame1].mUndistPoint[i] = r2x.xyz () ;
				}
				fake.mFrame[frame1].mUndistortion = TRUE ;
			}) ;
			fake.mWorkThread.post (0 ,fake.mFrame.length ()) ;
			fake.mWorkThread.join () ;
			fake.mWorkThread.stop () ;
		}
	}

	void save_data_json () {
		const auto r1x = fake.mDataPath.child (slice ("data.json")) ;
		auto mWriter = StreamFileTextWriter (r1x) ;
		auto mComma = Comma (slice ("\t") ,slice (",") ,slice ("\r\n")) ;
		mWriter.self << slice ("{") ;
		mComma++ ;
		if ifdo (TRUE) {
			mWriter.self << mComma ;
			mWriter.self << slice ("\"mView\":[") ;
			mComma++ ;
			for (auto &&i : fake.mView.range ()) {
				mWriter.self << mComma ;
				mWriter.self << slice ("{") ;
				mComma++ ;
				if ifdo (TRUE) {
					mWriter.self << mComma ;
					mWriter.self << slice ("\"id\":") << i ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mName\":") << slice ("\"") << fake.mView[i].mName << slice ("\"") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mGroup\":") << slice ("\"") << fake.mView[i].mGroup << slice ("\"") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mWidth\":[") ;
					mComma++ ;
					mComma.tight () ;
					mWriter.self << mComma ;
					mWriter.self << fake.mView[i].mWidth.mCX ;
					mWriter.self << mComma ;
					mWriter.self << fake.mView[i].mWidth.mCY ;
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mMatK\":[") ;
					mComma++ ;
					mComma.tight () ;
					const auto r2x = Solver::encode_matrix (fake.mView[i].mMatK[0]) ;
					for (auto &&j : iter (0 ,r2x.size ())) {
						mWriter.self << mComma ;
						mWriter.self << r2x[j] ;
					}
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mConstMatK\":") << fake.mView[i].mConstMatK ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mDist\":[") ;
					mComma++ ;
					mComma.tight () ;
					for (auto &&j : fake.mView[i].mDist) {
						mWriter.self << mComma ;
						mWriter.self << j ;
					}
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mConstDist\":") << fake.mView[i].mConstDist ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mMatV\":[") ;
					mComma++ ;
					mComma.tight () ;
					const auto r3x = Solver::encode_matrix (fake.mView[i].mMatV[0]) ;
					for (auto &&j : iter (0 ,r3x.size ())) {
						mWriter.self << mComma ;
						mWriter.self << r3x[j] ;
					}
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mConstMatV\":") << fake.mView[i].mConstMatV ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mBaseLine\":") << fake.mView[i].mBaseLine ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mRelative\":") << fake.mView[i].mRelative ;
				}
				mComma-- ;
				mWriter.self << mComma ;
				mWriter.self << slice ("}") ;
			}
			mComma-- ;
			mWriter.self << mComma ;
			mWriter.self << slice ("]") ;
			mWriter.self << mComma ;
			mWriter.self << slice ("\"mPose\":[") ;
			mComma++ ;
			for (auto &&i : fake.mPose.range ()) {
				mWriter.self << mComma ;
				mWriter.self << slice ("{") ;
				mComma++ ;
				if ifdo (TRUE) {
					mWriter.self << mComma ;
					mWriter.self << slice ("\"id\":") << i ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mName\":") << slice ("\"") << fake.mPose[i].mName << slice ("\"") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mGroup\":") << slice ("\"") << fake.mPose[i].mGroup << slice ("\"") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mColor\":[") ;
					mComma++ ;
					mComma.tight () ;
					mWriter.self << mComma ;
					mWriter.self << VAL32 (fake.mPose[i].mColor.mR) ;
					mWriter.self << mComma ;
					mWriter.self << VAL32 (fake.mPose[i].mColor.mG) ;
					mWriter.self << mComma ;
					mWriter.self << VAL32 (fake.mPose[i].mColor.mB) ;
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mUseView\":[") ;
					mComma++ ;
					mComma.tight () ;
					for (auto &&j : fake.mPose[i].mUseView) {
						mWriter.self << mComma ;
						mWriter.self << j ;
					}
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mMatV\":[") ;
					mComma++ ;
					mComma.tight () ;
					const auto r4x = Solver::encode_matrix (fake.mPose[i].mMatV[0]) ;
					for (auto &&j : iter (0 ,r4x.size ())) {
						mWriter.self << mComma ;
						mWriter.self << r4x[j] ;
					}
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mUsingMatV\":") << fake.mPose[i].mUsingMatV ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mWeight\":") << fake.mPose[i].mWeight ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mMatH\":[") ;
					mComma++ ;
					mComma.tight () ;
					const auto r5x = Solver::encode_matrix (fake.mPose[i].mMatH) ;
					for (auto &&j : iter (0 ,r5x.size ())) {
						mWriter.self << mComma ;
						mWriter.self << r5x[j] ;
					}
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mUsingMatH\":") << fake.mPose[i].mUsingMatH ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mError\":[") ;
					mComma++ ;
					mComma.tight () ;
					mWriter.self << fake.mPose[i].mError.mMaxError ;
					mWriter.self << mComma ;
					mWriter.self << fake.mPose[i].mError.mAvgError ;
					mWriter.self << mComma ;
					mWriter.self << fake.mPose[i].mError.mStdError ;
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
				}
				mComma-- ;
				mWriter.self << mComma ;
				mWriter.self << slice ("}") ;
			}
			mComma-- ;
			mWriter.self << mComma ;
			mWriter.self << slice ("]") ;
		}
		mComma-- ;
		mWriter.self << mComma ;
		mWriter.self << slice ("}") ;
		mWriter.flush () ;
	}
} ;

exports VFat<CalibrationHolder> CalibrationHolder::hold (VREF<CalibrationLayout> that) {
	return VFat<CalibrationHolder> (CalibrationImplHolder () ,that.mThis.self) ;
}

exports CFat<CalibrationHolder> CalibrationHolder::hold (CREF<CalibrationLayout> that) {
	return CFat<CalibrationHolder> (CalibrationImplHolder () ,that.mThis.self) ;
}
} ;
