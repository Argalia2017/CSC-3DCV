#include "export.h"

#include "../Common/export.h"
#include "../Detection/export.h"

namespace CSC3DCV {
struct CalibrationImplLayout {
	Path mDataPath ;
	Random mRandom ;
	WorkThread mWorkThread ;
	INDEX mFrame1View ;
	INDEX mFrame2View ;
	ArrayList<CameraView> mView ;
	Set<String<STR>> mViewNameSet ;
	ArrayList<CameraPose> mPose ;
	Set<String<STR>> mPoseNameSet ;
	ArrayList<CameraFrame> mFrame ;
	Set<Pixel> mFramePixelSet ;
	ArrayList<CameraBlock> mBlock ;
	Board mBoard ;
} ;

class CalibrationImplHolder final implement Fat<CalibrationHolder ,CalibrationImplLayout> {
public:
	void initialize () override {
		fake.mDataPath = Global<Path> (slice ("mDataPath")).fetch () ;
		fake.mRandom = CurrentRandom () ;
		fake.mWorkThread = WorkThread (NULL) ;
		fake.mBoard = Board (NULL) ;
		fake.mBoard.set_board_width (ImageShape (0 ,0 ,15 ,5 ,0)) ;
		fake.mBoard.set_board_type (BoardType::CIRCLE) ;
		fake.mBoard.set_board_baseline (37.6 ,37.6) ;
	}

	void execute () override {
		load_data_json () ;
		work_board_detection () ;
		work_sfm_view_mat_k () ;
		work_sfm_view_mat_v () ;
		work_sfm_pose_mat_v () ;
		work_bundle_adjustment () ;
		work_undistortion () ;
		save_data_json () ;
	}

	void load_data_json () {
		fake.mView = ArrayList<CameraView> () ;
		fake.mPose = ArrayList<CameraPose> () ;
		fake.mFrame = ArrayList<CameraFrame> () ;
		const auto r1x = fake.mDataPath.child (slice ("data.json")) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!r1x.is_file ())
				discard ;
			const auto r2x = JsonParser (FileProc::load_file (r1x)) ;
			load_data_json_view (r2x) ;
			load_data_json_pose (r2x) ;
			fake.mView.remap () ;
			fake.mPose.remap () ;
		}
		if ifdo (act) {
			const auto r3x = fake.mDataPath.child (slice ("image")).list () ;
			auto rax = Regex (slice ("(\\w+)_(\\w+)")) ;
			for (auto &&i : r3x) {
				if (i.extension () != slice (".bmp"))
					continue ;
				const auto r4x = i.stem () ;
				const auto r5x = rax.search (Ref<String<STR>>::reference (r4x) ,0) ;
				if (r5x == NONE)
					continue ;
				const auto r6x = String<STR> (rax.match (1)) ;
				const auto r7x = String<STR> (rax.match (2)) ;
				INDEX ix = insert_new_pose (r6x ,slice ("image")) ;
				INDEX iy = insert_new_view (r7x ,slice ("image")) ;
				fake.mPose[ix].mUseView.add (iy) ;
				fake.mView[iy].mUsePose.add (ix) ;
			}
			fake.mView.remap () ;
			fake.mPose.remap () ;
		}
		if ifdo (TRUE) {
			const auto r8x = Array<INDEX>::make (fake.mViewNameSet.range ()) ;
			assume (r8x.length () >= 2) ;
			fake.mFrame1View = r8x[0] ;
			fake.mFrame2View = r8x[1] ;
			for (auto &&i : fake.mPose.range ()) {
				for (auto &&j : fake.mPose[i].mUseView) {
					INDEX ix = insert_new_frame (i ,j) ;
					if ifdo (TRUE) {
						if (j != fake.mFrame1View)
							discard ;
						fake.mPose[i].mFrame1 = ix ;
					}
					if ifdo (TRUE) {
						if (j != fake.mFrame2View)
							discard ;
						fake.mPose[i].mFrame2 = ix ;
					}
				}
			}
			fake.mFrame.remap () ;
		}
	}

	void load_data_json_view (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mView")).list () ;
		for (auto &&i : fake.mView.range ()) {
			fake.mView[i].mName = r1x[i].child (slice ("mName")).parse (String<STR> ()) ;
			fake.mView[i].mGroup = r1x[i].child (slice ("mGroup")).parse (String<STR> ()) ;
			fake.mView[i].mUsePose = Set<INDEX> () ;
			const auto r2x = r1x[i].child (slice ("mShape")) ;
			fake.mView[i].mShape.mCX = r2x.child (0).parse (ZERO) ;
			fake.mView[i].mShape.mCY = r2x.child (1).parse (ZERO) ;
			const auto r3x = r1x[i].child (slice ("mMatK")).parse (FLT64 (0) ,4) ;
			fake.mView[i].mMatK = PerspectiveMatrix (r3x[0] ,r3x[1] ,r3x[3] ,r3x[4]) ;
			fake.mView[i].mConstMatK = TRUE ;
			fake.mView[i].mDist = r1x[i].child (slice ("mDist")).parse (FLT64 (0) ,5) ;
			fake.mView[i].mConstMatV = TRUE ;
			fake.mView[i].mBaseLine = r1x[i].child (slice ("mBaseLine")).parse (FLT64 (0)) ;
			fake.mView[i].mRelative = r1x[i].child (slice ("mRelative")).parse (FLT64 (0)) ;
		}
	}

	void load_data_json_pose (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mPose")).list () ;
		for (auto &&i : fake.mPose.range ()) {
			fake.mPose[i].mName = r1x[i].child (slice ("mName")).parse (String<STR> ()) ;
			fake.mPose[i].mGroup = r1x[i].child (slice ("mGroup")).parse (String<STR> ()) ;
			const auto r2x = r1x[i].child (slice ("mColor")).parse (String<STR> ()) ;
			fake.mPose[i].mColor = StringParse<Color3B>::make (r2x) ;
			fake.mPose[i].mUseView = Set<INDEX> () ;
			fake.mPose[i].mFrame1 = NONE ;
			fake.mPose[i].mFrame2 = NONE ;
			const auto r3x = r1x[i].child (slice ("mMatV")).parse (FLT64 (0) ,16) ;
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
			fake.mView[ret].mShape.mCX = ZERO ;
			fake.mView[ret].mShape.mCY = ZERO ;
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
			fake.mPose[ret].mColor = ToolProc::random_color (fake.mRandom) ;
			fake.mPose[ret].mUseView = Set<INDEX> () ;
			fake.mPose[ret].mFrame1 = NONE ;
			fake.mPose[ret].mFrame2 = NONE ;
			fake.mPose[ret].mUsingMatV = FALSE ;
			fake.mPose[ret].mWeight = 1 ;
			fake.mPose[ret].mUsingMatH = FALSE ;
		}
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
			const auto r2x = Format (slice ("$1_$2.bmp")) ;
			const auto r3x = fake.mDataPath.child (r2x (fake.mPose[pose1].mName ,fake.mView[view1].mName)) ;
			fake.mFrame[ret].mImageFile = r3x ;
			fake.mFrame[ret].mImage = Image<Color3B> () ;
			fake.mFrame[ret].mDepth = Image<FLT32> () ;
			fake.mFrame[ret].mUndistortion = FALSE ;
			fake.mFrame[ret].mOriginPoint = Array<Point2F> () ;
			fake.mFrame[ret].mUndistPoint = Array<Point2F> () ;
		}
		return move (ret) ;
	}

	void work_board_detection () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_board_detection")) ;
		if ifdo (TRUE) {
			fake.mBlock = ArrayList<CameraBlock> () ;
			INDEX ix = fake.mBlock.insert () ;
			fake.mBlock[ix].mTime1 = ix ;
			for (auto &&i : fake.mFrame.range ()) {
				fake.mBlock[ix].mUseFrame.add (i) ;
			}
			fake.mBlock[ix].mPoint = fake.mBoard.extract () ;
			fake.mBlock.remap () ;
		}
		for (auto &&i : fake.mFrame.range ()) {
			fake.mFrame[i].mImage = ImageProc::load_image (fake.mFrame[i].mImageFile) ;
			fake.mFrame[i].mOriginPoint = fake.mBoard.detect (fake.mFrame[i].mImage) ;
			fake.mFrame[i].mImage = Image<Color3B> () ;
		}
	}

	void work_sfm_view_mat_k () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_sfm_view_mat_k")) ;
		for (auto &&i : fake.mView.range ()) {
			const auto r1x = Array<INDEX>::make (fake.mView[i].mUsePose) ;
			assume (r1x.length () >= 2) ;
			INDEX ix = fake.mFramePixelSet.map (Pixel ({r1x[0] ,i})) ;
			INDEX iy = fake.mFramePixelSet.map (Pixel ({r1x[1] ,i})) ;
			assume (ix != NONE) ;
			assume (iy != NONE) ;
			fake.mView[i].sfm_view_mat_k (fake.mFrame[ix] ,fake.mFrame[iy]) ;
		}
	}

	void work_sfm_view_mat_v () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_sfm_view_mat_v")) ;
	}

	void work_sfm_pose_mat_v () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_sfm_pose_mat_v")) ;
	}

	void work_bundle_adjustment () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_bundle_adjustment")) ;
	}

	void work_undistortion () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_undistortion")) ;
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
					mWriter.self << slice ("\"mShape\":[") ;
					mComma++ ;
					mComma.tight () ;
					mWriter.self << mComma ;
					mWriter.self << fake.mView[i].mShape.mCX ;
					mWriter.self << mComma ;
					mWriter.self << fake.mView[i].mShape.mCY ;
					mComma-- ;
					mWriter.self << mComma ;
					mWriter.self << slice ("]") ;
					mWriter.self << mComma ;
					mWriter.self << slice ("\"mMatK\":[") ;
					mComma++ ;
					mComma.tight () ;
					const auto r2x = ToolProc::flatten (fake.mView[i].mMatK[0]) ;
					for (auto &&j : r2x.range ()) {
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
					const auto r3x = ToolProc::flatten (fake.mView[i].mMatV[0]) ;
					for (auto &&j : r3x.range ()) {
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
					const auto r4x = ToolProc::flatten (fake.mPose[i].mMatV[0]) ;
					for (auto &&j : r4x.range ()) {
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
					const auto r5x = ToolProc::flatten (fake.mPose[i].mMatH) ;
					for (auto &&j : r5x.range ()) {
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
