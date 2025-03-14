#include "export.h"

#include "../Common/export.h"
#include "../Detection/export.h"
#include "../Estimation/export.h"

namespace CSC3DCV {
struct CalibrationLayout {
	Path mDataPath ;
	Random mRandom ;
	WorkThread mWorkThread ;
	CameraView mView ;
	CameraPose mPose ;
	CameraFrame mFrame ;
	CameraBlock mBlock ;
	Board mBoard ;
	String<STR> mExtension ;
	Time mBeginTime ;
	Time mEndTime ;
} ;

class CalibrationImplHolder final implement Fat<CalibrationHolder ,CalibrationLayout> {
public:
	void initialize () override {
		self.mDataPath = Global<Path> (slice ("mDataPath")).fetch () ;
		self.mRandom = CurrentRandom () ;
		self.mWorkThread = WorkThread (NULL) ;
		self.mBoard = Board (NULL) ;
		self.mExtension = slice (".bmp") ;
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
		self.mView = CameraView () ;
		self.mPose = CameraPose () ;
		self.mFrame = CameraFrame () ;
		const auto r1x = self.mDataPath.child (slice ("data.json")) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!r1x.is_file ())
				discard ;
			const auto r2x = JsonParser (FileProc::load_file (r1x)) ;
			load_data_json_info (r2x) ;
			load_data_json_view (r2x) ;
			load_data_json_pose (r2x) ;
			self.mView.remap () ;
			self.mPose.remap () ;
		}
		if ifdo (act) {
			if ifdo (TRUE) {
				self.mBoard.set_board_shape (ImageShape (0 ,0 ,15 ,5 ,0)) ;
				self.mBoard.set_board_type (BoardType::CIRCLE) ;
				const auto r3x = FLT64 (54.75) ;
				self.mBoard.set_board_baseline (r3x ,r3x) ;
				self.mExtension = slice (".bmp") ;
			}
			const auto r4x = self.mDataPath.child (slice ("image")).list () ;
			auto rax = Regex (slice ("(\\w+)_(\\w+)")) ;
			for (auto &&i : r4x) {
				if (i.extension () != self.mExtension)
					continue ;
				const auto r5x = i.stem () ;
				const auto r6x = rax.search (Ref<String<STR>>::reference (r5x) ,0) ;
				if (r6x == NONE)
					continue ;
				const auto r7x = String<STR> (rax.match (1)) ;
				const auto r8x = String<STR> (rax.match (2)) ;
				INDEX ix = insert_new_pose (r7x ,slice ("image")) ;
				INDEX iy = insert_new_view (r8x ,slice ("image")) ;
				self.mPose[ix].mUseView.add (iy) ;
				self.mView[iy].mUsePose.add (ix) ;
			}
			self.mView.remap () ;
			self.mPose.remap () ;
		}
		if ifdo (TRUE) {
			const auto r9x = Array<INDEX>::make (self.mView.mViewNameSet.range ()) ;
			assume (r9x.length () >= 2) ;
			for (auto &&i : self.mPose.range ()) {
				for (auto &&j : self.mPose[i].mUseView) {
					INDEX ix = insert_new_frame (i ,j) ;
					if ifdo (TRUE) {
						if (j != r9x[0])
							discard ;
						self.mPose[i].mFrame1 = ix ;
					}
					if ifdo (TRUE) {
						if (j != r9x[1])
							discard ;
						self.mPose[i].mFrame2 = ix ;
					}
				}
			}
			self.mFrame.remap () ;
		}
	}

	void load_data_json_info (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mInfo")) ;
		const auto r2x = r1x.child (slice ("mShape")).child (0).parse (ZERO) ;
		const auto r3x = r1x.child (slice ("mShape")).child (1).parse (ZERO) ;
		const auto r4x = r1x.child (slice ("mType")).parse (String<STR> ()) ;
		const auto r5x = r1x.child (slice ("mBaseLine")).child (0).parse (FLT64 (0)) ;
		const auto r6x = r1x.child (slice ("mBaseLine")).child (1).parse (FLT64 (0)) ;
		self.mBoard.set_board_shape (ImageShape (0 ,0 ,r2x ,r3x ,0)) ;
		assume (r4x == slice ("CIRCLE")) ;
		self.mBoard.set_board_type (BoardType::CIRCLE) ;
		self.mBoard.set_board_baseline (r5x ,r6x) ;
		self.mExtension = r1x.child (slice ("mExtension")).parse (String<STR> ()) ;
	}

	void load_data_json_view (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mView")).list () ;
		for (auto &&i : self.mView.range ()) {
			self.mView[i].mName = r1x[i].child (slice ("mName")).parse (String<STR> ()) ;
			self.mView[i].mGroup = r1x[i].child (slice ("mGroup")).parse (String<STR> ()) ;
			self.mView[i].mUsePose = Set<INDEX> () ;
			const auto r2x = r1x[i].child (slice ("mShape")) ;
			self.mView[i].mShape.mCX = r2x.child (0).parse (ZERO) ;
			self.mView[i].mShape.mCY = r2x.child (1).parse (ZERO) ;
			const auto r3x = r1x[i].child (slice ("mMatK")).parse (FLT64 (0) ,4) ;
			self.mView[i].mMatK = PerspectiveMatrix (r3x[0] ,r3x[1] ,r3x[3] ,r3x[4]) ;
			self.mView[i].mConstMatK = TRUE ;
			self.mView[i].mDist = r1x[i].child (slice ("mDist")).parse (FLT64 (0) ,5) ;
			self.mView[i].mConstMatV = TRUE ;
			self.mView[i].mBaseLine = r1x[i].child (slice ("mBaseLine")).parse (FLT64 (0)) ;
			self.mView[i].mRelative = r1x[i].child (slice ("mRelative")).parse (FLT64 (0)) ;
		}
	}

	void load_data_json_pose (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mPose")).list () ;
		for (auto &&i : self.mPose.range ()) {
			self.mPose[i].mName = r1x[i].child (slice ("mName")).parse (String<STR> ()) ;
			self.mPose[i].mGroup = r1x[i].child (slice ("mGroup")).parse (String<STR> ()) ;
			const auto r2x = r1x[i].child (slice ("mColor")).parse (VAL32 (0) ,3) ;
			self.mPose[i].mColor.mR = BYTE (r2x[0]) ;
			self.mPose[i].mColor.mG = BYTE (r2x[1]) ;
			self.mPose[i].mColor.mB = BYTE (r2x[2]) ;
			self.mPose[i].mUseView = Set<INDEX> () ;
			self.mPose[i].mFrame1 = NONE ;
			self.mPose[i].mFrame2 = NONE ;
			const auto r3x = r1x[i].child (slice ("mMatV")).parse (FLT64 (0) ,16) ;
			self.mPose[i].mMatV = AffineMatrix (r3x) ;
			self.mPose[i].mUsingMatV = TRUE ;
			self.mPose[i].mWeight = 1 ;
			const auto r4x = r1x[i].child (slice ("mMatH")).parse (FLT64 (0) ,16) ;
			self.mPose[i].mMatH = AffineMatrix (r4x) ;
			self.mPose[i].mUsingMatH = TRUE ;
			const auto r5x = r1x[i].child (slice ("mError")) ;
			self.mPose[i].mError.mMaxError = r5x.child (0).parse (FLT64 (0)) ;
			self.mPose[i].mError.mAvgError = r5x.child (1).parse (FLT64 (0)) ;
			self.mPose[i].mError.mStdError = r5x.child (2).parse (FLT64 (0)) ;
		}
	}

	INDEX insert_new_view (CREF<String<STR>> name ,CREF<String<STR>> group) {
		INDEX ret = self.mView.mViewNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			ret = self.mView.insert () ;
			self.mView.mViewNameSet.add (name ,ret) ;
			self.mView[ret].mName = name ;
			self.mView[ret].mGroup = group ;
			self.mView[ret].mUsePose = Set<INDEX> () ;
			self.mView[ret].mShape.mCX = ZERO ;
			self.mView[ret].mShape.mCY = ZERO ;
			self.mView[ret].mConstMatK = FALSE ;
			self.mView[ret].mConstMatV = FALSE ;
			self.mView[ret].mBaseLine = 1 ;
			self.mView[ret].mRelative = 1 ;
		}
		return move (ret) ;
	}

	INDEX insert_new_pose (CREF<String<STR>> name ,CREF<String<STR>> group) {
		INDEX ret = self.mPose.mPoseNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			ret = self.mPose.insert () ;
			self.mPose.mPoseNameSet.add (name ,ret) ;
			self.mPose[ret].mName = name ;
			self.mPose[ret].mGroup = group ;
			self.mPose[ret].mColor = ToolProc::random_color (self.mRandom) ;
			self.mPose[ret].mUseView = Set<INDEX> () ;
			self.mPose[ret].mFrame1 = NONE ;
			self.mPose[ret].mFrame2 = NONE ;
			self.mPose[ret].mUsingMatV = FALSE ;
			self.mPose[ret].mWeight = 1 ;
			self.mPose[ret].mUsingMatH = FALSE ;
		}
		return move (ret) ;
	}

	INDEX insert_new_frame (CREF<INDEX> pose1 ,CREF<INDEX> view1) {
		const auto r1x = Pixel ({pose1 ,view1}) ;
		INDEX ret = self.mFrame.mFramePixelSet.map (r1x) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			ret = self.mFrame.insert () ;
			self.mFrame.mFramePixelSet.add (r1x ,ret) ;
			self.mFrame[ret].mPose1 = pose1 ;
			self.mFrame[ret].mView1 = view1 ;
			self.mFrame[ret].mTime1 = 0 ;
			const auto r2x = Format (slice ("$1_$2$3")) ;
			const auto r3x = self.mDataPath.child (self.mPose[pose1].mGroup).child (r2x (self.mPose[pose1].mName ,self.mView[view1].mName ,self.mExtension)) ;
			self.mFrame[ret].mImageFile = r3x ;
			self.mFrame[ret].mImage = Image<Color3B> () ;
			self.mFrame[ret].mDepth = Image<FLT32> () ;
			self.mFrame[ret].mUndistortion = FALSE ;
			self.mFrame[ret].mOriginPoint = Array<Point2F> () ;
			self.mFrame[ret].mUndistPoint = Array<Point2F> () ;
		}
		return move (ret) ;
	}

	void work_board_detection () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_board_detection")) ;
		if ifdo (TRUE) {
			self.mBlock = CameraBlock () ;
			INDEX ix = self.mBlock.insert () ;
			self.mBlock[ix].mTime1 = ix ;
			for (auto &&i : self.mFrame.range ()) {
				self.mBlock[ix].mUseFrame.add (i) ;
			}
			self.mBlock[ix].mPoint = self.mBoard.extract () ;
			self.mBlock.remap () ;
		}
		for (auto &&i : self.mFrame.range ()) {
			self.mFrame[i].mImage = ImageProc::load_image (self.mFrame[i].mImageFile) ;
			self.mFrame[i].mOriginPoint = self.mBoard.detect (self.mFrame[i].mImage) ;
			self.mFrame[i].mImage = Image<Color3B> () ;
		}
	}

	void work_sfm_view_mat_k () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_sfm_view_mat_k")) ;
		assume (self.mView.length () > 0) ;
		for (auto &&i : self.mView.range ()) {
			if (self.mView[i].mConstMatK)
				continue ;
			const auto r1x = Array<INDEX>::make (self.mView[i].mUsePose) ;
			assume (r1x.length () >= 1) ;
			INDEX ix = self.mPose[r1x[0]].mFrame1 ;
			INDEX iy = self.mPose[r1x[0]].mFrame2 ;
			assume (ix != NONE) ;
			assume (iy != NONE) ;
			self.mView.sfm_view_mat_k (self.mFrame[ix] ,self.mFrame[iy]) ;
		}
	}

	void work_sfm_view_mat_v () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_sfm_view_mat_v")) ;
		for (auto &&i : self.mPose.range ()) {
			INDEX jx = self.mFrame[self.mPose[i].mFrame1].mView1 ;
			if ifdo (TRUE) {
				if (self.mView[jx].mConstMatV)
					discard ;
				self.mView[jx].mMatV = Matrix::identity () ;
			}
			INDEX ix = self.mFrame.mFramePixelSet.map (Pixel ({i ,jx})) ;
			assume (ix != NONE) ;
			for (auto &&j : self.mPose[i].mUseView) {
				if (self.mView[j].mConstMatV)
					continue ;
				if (j == jx)
					continue ;
				INDEX iy = self.mFrame.mFramePixelSet.map (Pixel ({i ,j})) ;
				assume (iy != NONE) ;
				self.mView.sfm_view_mat_v (self.mFrame[ix] ,self.mFrame[iy]) ;
				self.mView[j].mMatV = self.mView[jx].mMatV[0] * self.mView[j].mMatV[0] ;
			}
		}
	}

	void work_sfm_pose_mat_v () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_sfm_pose_mat_v")) ;
		assume (self.mPose.length () > 0) ;
		for (auto &&i : self.mPose.range ()) {
			INDEX ix = self.mPose[i].mFrame1 ;
			self.mPose.sfm_pose_mat_v (self.mFrame[ix]) ;
		}
	}

	void work_bundle_adjustment () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_bundle_adjustment")) ;
	}

	void work_undistortion () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_undistortion")) ;
		for (auto &&i : self.mFrame.range ()) {
		
		}
	}

	void save_data_json () {
		const auto r1x = self.mDataPath.child (slice ("data.json")) ;
		auto mWriter = StreamFileTextWriter (r1x) ;
		auto mComma = Comma (slice ("\t") ,slice (",") ,slice ("\r\n")) ;
		mWriter.deref << slice ("{") ;
		mComma++ ;
		if ifdo (TRUE) {
			mWriter.deref << mComma ;
			mWriter.deref << slice ("\"mInfo\":{") ;
			mComma++ ;
			if ifdo (TRUE) {
			}
			mComma-- ;
			mWriter.deref << mComma ;
			mWriter.deref << slice ("}") ;
			mWriter.deref << mComma ;
			mWriter.deref << slice ("\"mView\":[") ;
			mComma++ ;
			for (auto &&i : self.mView.range ()) {
				mWriter.deref << mComma ;
				mWriter.deref << slice ("{") ;
				mComma++ ;
				if ifdo (TRUE) {
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"id\":") << i ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mName\":") << slice ("\"") << self.mView[i].mName << slice ("\"") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mGroup\":") << slice ("\"") << self.mView[i].mGroup << slice ("\"") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mShape\":[") ;
					mComma++ ;
					mComma.tight () ;
					mWriter.deref << mComma ;
					mWriter.deref << self.mView[i].mShape.mCX ;
					mWriter.deref << mComma ;
					mWriter.deref << self.mView[i].mShape.mCY ;
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mMatK\":[") ;
					mComma++ ;
					mComma.tight () ;
					const auto r2x = ToolProc::flatten (self.mView[i].mMatK[0]) ;
					for (auto &&j : r2x.range ()) {
						mWriter.deref << mComma ;
						mWriter.deref << r2x[j] ;
					}
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mConstMatK\":") << self.mView[i].mConstMatK ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mDist\":[") ;
					mComma++ ;
					mComma.tight () ;
					for (auto &&j : self.mView[i].mDist) {
						mWriter.deref << mComma ;
						mWriter.deref << j ;
					}
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mConstDist\":") << self.mView[i].mConstDist ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mMatV\":[") ;
					mComma++ ;
					mComma.tight () ;
					const auto r3x = ToolProc::flatten (self.mView[i].mMatV[0]) ;
					for (auto &&j : r3x.range ()) {
						mWriter.deref << mComma ;
						mWriter.deref << r3x[j] ;
					}
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mConstMatV\":") << self.mView[i].mConstMatV ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mBaseLine\":") << self.mView[i].mBaseLine ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mRelative\":") << self.mView[i].mRelative ;
				}
				mComma-- ;
				mWriter.deref << mComma ;
				mWriter.deref << slice ("}") ;
			}
			mComma-- ;
			mWriter.deref << mComma ;
			mWriter.deref << slice ("]") ;
			mWriter.deref << mComma ;
			mWriter.deref << slice ("\"mPose\":[") ;
			mComma++ ;
			for (auto &&i : self.mPose.range ()) {
				mWriter.deref << mComma ;
				mWriter.deref << slice ("{") ;
				mComma++ ;
				if ifdo (TRUE) {
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"id\":") << i ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mName\":") << slice ("\"") << self.mPose[i].mName << slice ("\"") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mGroup\":") << slice ("\"") << self.mPose[i].mGroup << slice ("\"") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mColor\":[") ;
					mComma++ ;
					mComma.tight () ;
					mWriter.deref << mComma ;
					mWriter.deref << VAL32 (self.mPose[i].mColor.mR) ;
					mWriter.deref << mComma ;
					mWriter.deref << VAL32 (self.mPose[i].mColor.mG) ;
					mWriter.deref << mComma ;
					mWriter.deref << VAL32 (self.mPose[i].mColor.mB) ;
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mUseView\":[") ;
					mComma++ ;
					mComma.tight () ;
					for (auto &&j : self.mPose[i].mUseView) {
						mWriter.deref << mComma ;
						mWriter.deref << j ;
					}
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mMatV\":[") ;
					mComma++ ;
					mComma.tight () ;
					const auto r4x = ToolProc::flatten (self.mPose[i].mMatV[0]) ;
					for (auto &&j : r4x.range ()) {
						mWriter.deref << mComma ;
						mWriter.deref << r4x[j] ;
					}
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mUsingMatV\":") << self.mPose[i].mUsingMatV ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mWeight\":") << self.mPose[i].mWeight ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mMatH\":[") ;
					mComma++ ;
					mComma.tight () ;
					const auto r5x = ToolProc::flatten (self.mPose[i].mMatH) ;
					for (auto &&j : r5x.range ()) {
						mWriter.deref << mComma ;
						mWriter.deref << r5x[j] ;
					}
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mUsingMatH\":") << self.mPose[i].mUsingMatH ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"mError\":[") ;
					mComma++ ;
					mComma.tight () ;
					mWriter.deref << mComma ;
					mWriter.deref << self.mPose[i].mError.mMaxError ;
					mWriter.deref << mComma ;
					mWriter.deref << self.mPose[i].mError.mAvgError ;
					mWriter.deref << mComma ;
					mWriter.deref << self.mPose[i].mError.mStdError ;
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
				}
				mComma-- ;
				mWriter.deref << mComma ;
				mWriter.deref << slice ("}") ;
			}
			mComma-- ;
			mWriter.deref << mComma ;
			mWriter.deref << slice ("]") ;
		}
		mComma-- ;
		mWriter.deref << mComma ;
		mWriter.deref << slice ("}") ;
		mWriter.flush () ;
	}
} ;

exports OfThis<AutoRef<CalibrationLayout>> CalibrationHolder::create () {
	OfThis<AutoRef<CalibrationLayout>> ret ;
	ret.mThis = AutoRef<CalibrationLayout>::make () ;
	return move (ret) ;
}

exports VFat<CalibrationHolder> CalibrationHolder::hold (VREF<CalibrationLayout> that) {
	return VFat<CalibrationHolder> (CalibrationImplHolder () ,that) ;
}

exports CFat<CalibrationHolder> CalibrationHolder::hold (CREF<CalibrationLayout> that) {
	return CFat<CalibrationHolder> (CalibrationImplHolder () ,that) ;
}
} ;
