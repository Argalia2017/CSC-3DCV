#include "export.h"

#include "../Common/export.h"
#include "../Detection/export.h"
#include "../Estimation/export.h"

#include "csc_end.h"
#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include "csc_begin.h"

namespace CSC3DCV {
struct CalibrationLayout {
	Path mDataPath ;
	Random mRandom ;
	WorkThread mWorkThread ;
	ArrayList<CameraView> mView ;
	Set<String<STR>> mViewNameSet ;
	ArrayList<CameraPose> mPose ;
	Set<String<STR>> mPoseNameSet ;
	ArrayList<CameraFrame> mFrame ;
	Set<Pixel> mFramePixelSet ;
	ArrayList<CameraBlock> mBlock ;
	Array<Line2F> mMapping ;
	Board mBoard ;
	ImageShape mBoardShape ;
	String<STR> mBoardType ;
	Vector mBoardBaseline ;
	Array<String<STR>> mImageGroup ;
	String<STR> mImageExtension ;
	Box<ceres::Problem> mProblem ;
	Array<FLT64> mFunc ;
	ceres::Solver::Options mOptions ;
	ceres::Solver::Summary mSummary ;
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
		self.mBoardShape.mCX = 15 ;
		self.mBoardShape.mCY = 5 ;
		self.mBoardType = slice ("CIRCLE") ;
		self.mBoardBaseline = Vector (54.75 ,54.75 ,1 ,0) ;
		self.mImageGroup = Array<String<STR>> ({slice ("image")}) ;
		self.mImageExtension = slice (".bmp") ;
	}

	void execute () override {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("Calibration::execute")) ;
		self.mBeginTime = CurrentTime () ;
		load_data_json () ;
		work_build_frame () ;
		work_build_block () ;
		work_sfm_view_mat_k () ;
		work_sfm_view_mat_v () ;
		work_sfm_pose_mat_v () ;
		work_sfm_global_ba (TRUE ,FALSE ,TRUE ,TRUE ,FALSE) ;
		work_sfm_global_ba (FALSE ,TRUE ,TRUE ,TRUE ,FALSE) ;
		work_refine_pose () ;
		work_sfm_global_ba (FALSE ,FALSE ,FALSE ,TRUE ,TRUE) ;
		work_sfm_pose_weight () ;
		work_mapping_block () ;
		save_data_json () ;
		save_pointcloud_ply () ;
		save_camera_ply () ;
		self.mEndTime = CurrentTime () ;
		const auto r1x = ToolProc::format_time (self.mEndTime - self.mBeginTime) ;
		Singleton<Console>::instance ().warn (slice ("time_cost = ") ,r1x) ;
	}

	void load_data_json () {
		self.mView = ArrayList<CameraView> () ;
		self.mPose = ArrayList<CameraPose> () ;
		self.mFrame = ArrayList<CameraFrame> () ;
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
			for (auto &&i : self.mImageGroup) {
				const auto r3x = self.mDataPath.child (i).list () ;
				auto rax = Regex (slice ("(\\w+)_(\\w+)")) ;
				for (auto &&j : r3x) {
					if (j.extension () != self.mImageExtension)
						continue ;
					const auto r4x = j.stem () ;
					const auto r5x = rax.search (Ref<String<STR>>::reference (r4x) ,0) ;
					if (r5x == NONE)
						continue ;
					const auto r6x = String<STR> (rax.match (1)) ;
					const auto r7x = String<STR> (rax.match (2)) ;
					INDEX ix = insert_new_pose (r6x ,i) ;
					INDEX iy = insert_new_view (r7x ,i) ;
					self.mPose[ix].mUseView.add (iy) ;
					self.mView[iy].mUsePose.add (ix) ;
				}
			}
			self.mView.remap () ;
			self.mPose.remap () ;
		}
	}

	void load_data_json_info (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mInfo")) ;
		const auto r2x = r1x.child (slice ("mBoardShape")).parse (ZERO ,2) ;
		self.mBoardShape.mCX = r2x[0] ;
		self.mBoardShape.mCY = r2x[1] ;
		const auto r3x = r1x.child (slice ("mBoardBaseline")).parse (FLT64 (0) ,3) ;
		self.mBoardBaseline[0] = r3x[0] ;
		self.mBoardBaseline[1] = r3x[1] ;
		self.mBoardBaseline[2] = r3x[2] ;
		self.mBoardBaseline[3] = 0 ;
		self.mBoardType = r1x.child (slice ("mBoardType")).parse (String<STR> ()) ;
		assume (self.mBoardType == slice ("CIRCLE")) ;
		self.mImageExtension = r1x.child (slice ("mImageExtension")).parse (String<STR> ()) ;
	}

	void load_data_json_view (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mView")).list () ;
		for (auto &&i : r1x.range ()) {
			INDEX ix = self.mView.insert () ;
			const auto r2x = r1x[i].child (slice ("mName")).parse (String<STR> ()) ;
			self.mView[ix].mName = r2x ;
			self.mViewNameSet.add (r2x ,ix) ;
			self.mView[ix].mGroup = r1x[i].child (slice ("mGroup")).parse (String<STR> ()) ;
			const auto r3x = r1x[i].child (slice ("mUsePose")).list () ;
			self.mView[ix].mUsePose = Set<INDEX> (r3x.length ()) ;
			for (auto &&j : r3x) {
				const auto r4x = j.parse (NONE) ;
				self.mView[ix].mUsePose.add (r4x) ;
			}
			const auto r5x = r1x[i].child (slice ("mShape")) ;
			self.mView[ix].mShape.mCX = r5x.child (0).parse (ZERO) ;
			self.mView[ix].mShape.mCY = r5x.child (1).parse (ZERO) ;
			const auto r6x = r1x[i].child (slice ("mMatK")).parse (FLT64 (0) ,16) ;
			self.mView[ix].mMatK = SolverProc::decode_matrix (r6x) ;
			self.mView[ix].mConstMatK = TRUE ;
			self.mView[ix].mDist = r1x[i].child (slice ("mDist")).parse (FLT64 (0) ,5) ;
			self.mView[ix].mConstDist = TRUE ;
			const auto r7x = r1x[i].child (slice ("mMatV")).parse (FLT64 (0) ,16) ;
			self.mView[ix].mMatV = SolverProc::decode_matrix (r7x) ;
			self.mView[ix].mConstMatV = TRUE ;
			self.mView[ix].mBaseline = r1x[i].child (slice ("mBaseline")).parse (FLT64 (0)) ;
			self.mView[ix].mRelative = r1x[i].child (slice ("mRelative")).parse (FLT64 (0)) ;
		}
	}

	void load_data_json_pose (CREF<JsonParser> parser) {
		const auto r1x = parser.child (slice ("mPose")).list () ;
		for (auto &&i : r1x.range ()) {
			INDEX ix = self.mPose.insert () ;
			const auto r2x = r1x[i].child (slice ("mName")).parse (String<STR> ()) ;
			self.mPose[ix].mName = r2x ;
			self.mPoseNameSet.add (r2x ,ix) ;
			self.mPose[ix].mGroup = r1x[i].child (slice ("mGroup")).parse (String<STR> ()) ;
			const auto r3x = r1x[i].child (slice ("mColor")).parse (VAL32 (0) ,3) ;
			self.mPose[ix].mColor.mR = BYTE (r3x[0]) ;
			self.mPose[ix].mColor.mG = BYTE (r3x[1]) ;
			self.mPose[ix].mColor.mB = BYTE (r3x[2]) ;
			const auto r4x = r1x[i].child (slice ("mUseView")).list () ;
			self.mPose[ix].mUseView = Set<INDEX> (r4x.length ()) ;
			for (auto &&j : r4x) {
				const auto r5x = j.parse (NONE) ;
				self.mPose[ix].mUseView.add (r5x) ;
			}
			self.mPose[ix].mFrame1 = NONE ;
			self.mPose[ix].mFrame2 = NONE ;
			const auto r6x = r1x[i].child (slice ("mMatV")).parse (FLT64 (0) ,16) ;
			self.mPose[ix].mMatV = SolverProc::decode_matrix (r6x) ;
			self.mPose[ix].mUsingMatV = TRUE ;
			self.mPose[ix].mWeight = 1 ;
			const auto r7x = r1x[i].child (slice ("mMatH")).parse (FLT64 (0) ,16) ;
			self.mPose[ix].mMatH = SolverProc::decode_matrix (r7x) ;
			self.mPose[ix].mUsingMatH = FALSE ;
			const auto r8x = r1x[i].child (slice ("mError")) ;
			self.mPose[ix].mError.mMaxError = r8x.child (0).parse (FLT64 (0)) ;
			self.mPose[ix].mError.mAvgError = r8x.child (1).parse (FLT64 (0)) ;
			self.mPose[ix].mError.mStdError = r8x.child (2).parse (FLT64 (0)) ;
		}
	}

	void work_build_frame () {
		const auto r1x = Array<INDEX>::make (self.mView.range ()) ;
		assume (r1x.length () >= 2) ;
		for (auto &&i : self.mPose.range ()) {
			for (auto &&j : self.mPose[i].mUseView) {
				INDEX ix = insert_new_frame (i ,j) ;
				if ifdo (TRUE) {
					if (j != r1x[0])
						discard ;
					self.mPose[i].mFrame1 = ix ;
				}
				if ifdo (TRUE) {
					if (j != r1x[1])
						discard ;
					self.mPose[i].mFrame2 = ix ;
				}
				INDEX jx = self.mPose[i].mUseView.find (j) ;
				assert (jx != NONE) ;
				self.mPose[i].mUseView.set (jx ,ix) ;
				INDEX jy = self.mView[j].mUsePose.find (i) ;
				assert (jy != NONE) ;
				self.mView[j].mUsePose.set (jy ,ix) ;
			}
		}
		self.mFrame.remap () ;
	}

	INDEX insert_new_view (CREF<String<STR>> name ,CREF<String<STR>> group) {
		INDEX ret = self.mViewNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			ret = self.mView.insert () ;
			self.mViewNameSet.add (name ,ret) ;
			self.mView[ret].mName = name ;
			self.mView[ret].mGroup = group ;
			self.mView[ret].mUsePose = Set<INDEX> () ;
			self.mView[ret].mShape.mCX = 1920 ;
			self.mView[ret].mShape.mCY = 1080 ;
			self.mView[ret].mConstMatK = FALSE ;
			self.mView[ret].mDist = Array<FLT64> (5) ;
			self.mView[ret].mDist.fill (0) ;
			self.mView[ret].mConstMatV = FALSE ;
			self.mView[ret].mBaseline = 1 ;
			self.mView[ret].mRelative = 1 ;
		}
		return move (ret) ;
	}

	INDEX insert_new_pose (CREF<String<STR>> name ,CREF<String<STR>> group) {
		INDEX ret = self.mPoseNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			ret = self.mPose.insert () ;
			self.mPoseNameSet.add (name ,ret) ;
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
		INDEX ret = self.mFramePixelSet.map (r1x) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			ret = self.mFrame.insert () ;
			self.mFramePixelSet.add (r1x ,ret) ;
			self.mFrame[ret].mPose1 = pose1 ;
			self.mFrame[ret].mView1 = view1 ;
			self.mFrame[ret].mTime1 = 0 ;
			self.mFrame[ret].mUseBlock = Set<INDEX> () ;
			const auto r2x = Format (slice ("$1_$2$3")) ;
			const auto r3x = self.mDataPath.child (self.mPose[pose1].mGroup).child (r2x (self.mPose[pose1].mName ,self.mView[view1].mName ,self.mImageExtension)) ;
			self.mFrame[ret].mImageFile = r3x ;
			self.mFrame[ret].mImage = Image<Color3B> () ;
			self.mFrame[ret].mPoint2D = Array<Point2F> () ;
			self.mFrame[ret].mPointDepth = Array<FLT32> () ;
			self.mFrame[ret].mPointRay = Array<Vector> () ;
		}
		return move (ret) ;
	}

	void work_build_block () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_build_block")) ;
		self.mBoard.set_board_shape (self.mBoardShape) ;
		self.mBoard.set_board_type (BoardType::CIRCLE) ;
		self.mBoard.set_board_baseline (self.mBoardBaseline) ;
		if ifdo (TRUE) {
			self.mBlock = ArrayList<CameraBlock> () ;
			INDEX ix = self.mBlock.insert () ;
			self.mBlock[ix].mTime1 = ix ;
			self.mBlock[ix].mUseFrame = Set<INDEX> () ;
			self.mBlock[ix].mPoint3D = self.mBoard.extract () ;
			self.mBlock[ix].mMatP = Matrix::identity () ;
			self.mBlock[ix].mUsingMatP = FALSE ;
			self.mBlock.remap () ;
		}
		for (auto &&i : self.mFrame.range ()) {
			INDEX ix = self.mFrame[i].mView1 ;
			const auto r1x = self.mView[ix].mShape ;
			self.mFrame[i].mImage = ImageProc::load_image (self.mFrame[i].mImageFile) ;
			assume (self.mFrame[i].mImage.shape () == r1x) ;
			self.mFrame[i].mPoint2D = self.mBoard.detect (self.mFrame[i].mImage) ;
			self.mFrame[i].mPointDepth = Array<FLT32> (self.mFrame[i].mPoint2D.size ()) ;
			self.mFrame[i].mPointRay = Array<Vector> (self.mFrame[i].mPoint2D.size ()) ;
			self.mFrame[i].mImage = Image<Color3B> () ;
		}
	}

	void work_sfm_view_mat_k () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_sfm_view_mat_k")) ;
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
			work_sfm_focal_length (ix ,iy) ;
		}
	}

	void work_sfm_focal_length (CREF<INDEX> frame1 ,CREF<INDEX> frame2) {
		auto &&view1 = self.mView[self.mFrame[frame1].mView1] ;
		auto &&view2 = self.mView[self.mFrame[frame2].mView1] ;
		if ifdo (TRUE) {
			self.mProblem = Box<ceres::Problem>::make () ;
			INDEX ix = 0 ;
			view1.mParam1 = ix ;
			ix += 1 ;
			view1.mParam2 = ix ;
			ix += 3 ;
			view2.mParam1 = ix ;
			ix += 1 ;
			view2.mParam2 = ix ;
			ix += 3 ;
			if (ix <= self.mFunc.size ())
				discard ;
			self.mFunc = Array<FLT64> (ix) ;
		}
		const auto r1x = Vector (FLT64 (view1.mShape.mCX) ,FLT64 (view1.mShape.mCY) ,0 ,2).projection () ;
		const auto r2x = Vector (FLT64 (view2.mShape.mCX) ,FLT64 (view2.mShape.mCY) ,0 ,2).projection () ;
		if ifdo (TRUE) {
			const auto view1_focal = FLT64 (1000) ;
			const auto view2_focal = FLT64 (1000) ;
			self.mFunc[view1.mParam1] = view1_focal ;
			self.mFunc[view2.mParam1] = view2_focal ;
			self.mProblem->AddParameterBlock ((&self.mFunc[view1.mParam1]) ,1) ;
			self.mProblem->AddParameterBlock ((&self.mFunc[view2.mParam1]) ,1) ;
		}
		if ifdo (TRUE) {
			const auto r3x = Matrix::identity () ;
			const auto r4x = Matrix::identity () ;
			const auto r5x = SolverProc::encode_rotation (r3x) ;
			const auto r6x = SolverProc::encode_rotation (r4x) ;
			self.mFunc[view1.mParam2 + 0] = r5x[0] ;
			self.mFunc[view1.mParam2 + 1] = r5x[1] ;
			self.mFunc[view1.mParam2 + 2] = r5x[2] ;
			self.mFunc[view2.mParam2 + 0] = r6x[0] ;
			self.mFunc[view2.mParam2 + 1] = r6x[1] ;
			self.mFunc[view2.mParam2 + 2] = r6x[2] ;
			self.mProblem->AddParameterBlock ((&self.mFunc[view1.mParam2]) ,3) ;
			self.mProblem->AddParameterBlock ((&self.mFunc[view2.mParam2]) ,3) ;
		}
		const auto r7x = new ceres::TrivialLoss () ;
		for (auto &&i : self.mFrame[frame1].mPoint2D.range ()) {
			const auto r8x = Vector (self.mFrame[frame1].mPoint2D[i]) - r1x ;
			const auto r9x = Vector (self.mFrame[frame2].mPoint2D[i]) - r2x ;
			const auto r10x = ResidualErrorFocal::create (r8x.xyz () ,r9x.xyz ()) ;
			const auto view1_focal = (&self.mFunc[view1.mParam1]) ;
			const auto view2_focal = (&self.mFunc[view2.mParam1]) ;
			const auto view1_rot = (&self.mFunc[view1.mParam2]) ;
			const auto view2_rot = (&self.mFunc[view2.mParam2]) ;
			self.mProblem->AddResidualBlock (r10x ,r7x ,view1_focal ,view2_focal ,view1_rot ,view2_rot) ;
		}
		self.mOptions.minimizer_progress_to_stdout = TRUE ;
		self.mOptions.logging_type = ceres::LoggingType::SILENT ;
		ceres::Solve (self.mOptions ,(&self.mProblem.deref) ,(&self.mSummary)) ;
		assume (self.mSummary.initial_cost >= 0) ;
		assume (self.mSummary.final_cost >= 0) ;
		if ifdo (TRUE) {
			const auto view1_focal = self.mFunc[view1.mParam1] ;
			const auto view2_focal = self.mFunc[view2.mParam1] ;
			view1.mMatK = PerspectiveMatrix (view1_focal ,view1_focal ,r1x[0] ,r1x[1]) ;
			view2.mMatK = PerspectiveMatrix (view2_focal ,view2_focal ,r2x[0] ,r2x[1]) ;
		}
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mFocus = $2")) (view1.mName ,self.mFunc[view1.mParam1])) ;
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mFocus = $2")) (view2.mName ,self.mFunc[view2.mParam1])) ;
	}

	void work_sfm_view_mat_v () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_sfm_view_mat_v")) ;
		assume (self.mView.length () > 0) ;
		for (auto &&i : self.mPose.range ()) {
			INDEX jx = self.mFrame[self.mPose[i].mFrame1].mView1 ;
			if ifdo (TRUE) {
				if (self.mView[jx].mConstMatV)
					discard ;
				self.mView[jx].mMatV = Matrix::identity () ;
			}
			INDEX ix = self.mPose[i].mUseView.map (jx) ;
			for (auto &&j : self.mPose[i].mUseView) {
				if (self.mView[j].mConstMatV)
					continue ;
				if (j == jx)
					continue ;
				INDEX iy = self.mPose[i].mUseView.map (j) ;
				work_sfm_homography (ix ,iy) ;
				self.mView[j].mMatV = self.mView[jx].mMatV[0] * self.mView[j].mMatV[0] ;
			}
			check_epipolar_error (self.mPose[i].mFrame1 ,self.mPose[i].mFrame2) ;
		}
	}

	void work_sfm_epipolar (CREF<INDEX> frame1 ,CREF<INDEX> frame2) {
		auto &&view1 = self.mView[self.mFrame[frame1].mView1] ;
		auto &&view2 = self.mView[self.mFrame[frame2].mView1] ;
		const auto r1x = invoke ([&] () {
			Array<Vector> ret = Array<Vector> (self.mFrame[frame1].mPoint2D.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r2x = Vector (self.mFrame[frame1].mPoint2D[i]) ;
				const auto r3x = (view1.mMatK[1] * r2x).normalize () ;
				ret[i] = r3x ;
			}
			return move (ret) ;
		}) ;
		const auto r4x = invoke ([&] () {
			Array<Vector> ret = Array<Vector> (self.mFrame[frame2].mPoint2D.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r5x = Vector (self.mFrame[frame2].mPoint2D[i]) ;
				const auto r6x = (view2.mMatK[1] * r5x).normalize () ;
				ret[i] = r6x ;
			}
			return move (ret) ;
		}) ;
		const auto r7x = DuplexMatrix (Matrix::identity ()) ;
		const auto r8x = DuplexMatrix (Matrix::identity ()) ;
		const auto r9x = invoke ([&] () {
			Image<FLT64> ret = Image<FLT64> (9 ,LENGTH (r1x.size ())) ;
			for (auto &&i : iter (0 ,r1x.size ())) {
				const auto r10x = r7x[1] * r1x[i] ;
				const auto r11x = r8x[1] * r4x[i] ;
				if ifdo (TRUE) {
					INDEX ix = i ;
					ret[ix][0] = r10x[0] * r11x[0] ;
					ret[ix][1] = r10x[0] * r11x[1] ;
					ret[ix][2] = r10x[0] * r11x[2] ;
					ret[ix][3] = r10x[1] * r11x[0] ;
					ret[ix][4] = r10x[1] * r11x[1] ;
					ret[ix][5] = r10x[1] * r11x[2] ;
					ret[ix][6] = r10x[2] * r11x[0] ;
					ret[ix][7] = r10x[2] * r11x[1] ;
					ret[ix][8] = r10x[2] * r11x[2] ;
				}
			}
			return move (ret) ;
		}) ;
		const auto r12x = Image<FLT64> (LinearProc::solve_lsm (r9x)) ;
		const auto r13x = invoke ([&] () {
			Matrix ret = Matrix::zero () ;
			ret[0][0] = r12x[0][0] ;
			ret[0][1] = r12x[1][0] ;
			ret[0][2] = r12x[2][0] ;
			ret[1][0] = r12x[3][0] ;
			ret[1][1] = r12x[4][0] ;
			ret[1][2] = r12x[5][0] ;
			ret[2][0] = r12x[6][0] ;
			ret[2][1] = r12x[7][0] ;
			ret[2][2] = r12x[8][0] ;
			return move (ret) ;
		}) ;
		const auto r14x = r7x[1].transpose () * r13x * r8x[1] ;
		const auto r15x = MatrixProc::solve_svd (r14x) ;
		const auto r16x = invoke ([&] () {
			Array<KRTResult> ret = Array<KRTResult> (4) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r17x = MathProc::sign (ByteProc::any_bit (BYTE (i) ,BYTE (0X02))) ;
				const auto r18x = MathProc::sign (ByteProc::any_bit (BYTE (i) ,BYTE (0X01))) ;
				const auto r19x = invoke ([&] () {
					Matrix ret = Matrix::identity () ;
					ret[0][0] = 0 ;
					ret[0][1] = -r17x ;
					ret[1][0] = r17x ;
					ret[1][1] = 0 ;
					return move (ret) ;
				}) ;
				const auto r20x = r15x.mU * Vector::axis_z () * r18x ;
				const auto r21x = r15x.mU * r19x * r15x.mV.transpose () ;
				const auto r22x = MathProc::inverse (r21x.det ()) ;
				const auto r23x = r21x * DiagMatrix (r22x ,r22x ,r22x) ;
				ret[i].mK = CrossProductMatrix (r20x) ;
				ret[i].mT = TranslationMatrix (r20x) ;
				ret[i].mR = r23x ;
			}
			return move (ret) ;
		}) ;
		const auto r24x = triangulate_select (r1x ,r4x ,r16x ,view2.mBaseline) ;
		view2.mMatV = r24x.mT * r24x.mR ;
		const auto r25x = view2.mMatV[0] * Vector::axis_z () ;
		const auto r26x = view2.mMatV[0] * Vector::axis_w () ;
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mDirection = [$2 ,$3 ,$4]")) (view1.mName ,r25x[0] ,r25x[1] ,r25x[2])) ;
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mTranslation = [$2 ,$3 ,$4]")) (view2.mName ,r26x[0] ,r26x[1] ,r26x[2])) ;
	}

	void work_sfm_homography (CREF<INDEX> frame1 ,CREF<INDEX> frame2) {
		auto &&view1 = self.mView[self.mFrame[frame1].mView1] ;
		auto &&view2 = self.mView[self.mFrame[frame2].mView1] ;
		const auto r1x = PerspectiveMatrix (1 ,1 ,0 ,0) ;
		const auto r2x = invoke ([&] () {
			Array<Vector> ret = Array<Vector> (self.mFrame[frame1].mPoint2D.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r3x = Vector (self.mFrame[frame1].mPoint2D[i]) ;
				const auto r4x = view1.mMatK[1] * r3x ;
				ret[i] = r4x;
			}
			return move (ret) ;
		}) ;
		const auto r5x = invoke ([&] () {
			Array<Vector> ret = Array<Vector> (self.mFrame[frame2].mPoint2D.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r6x = Vector (self.mFrame[frame2].mPoint2D[i]) ;
				const auto r7x = view2.mMatK[1] * r6x ;
				ret[i] = r7x ;
			}
			return move (ret) ;
		}) ;
		const auto r8x = DuplexMatrix (r1x) ;
		const auto r9x = DuplexMatrix (r1x) ;
		const auto r10x = invoke ([&] () {
			Image<FLT64> ret = Image<FLT64> (9 ,LENGTH (r2x.size ()) * 2) ;
			for (auto &&i : iter (0 ,r2x.size ())) {
				const auto r11x = r8x[1] * r2x[i] ;
				const auto r12x = r9x[1] * r5x[i] ;
				if ifdo (TRUE) {
					INDEX ix = i * 2 + 0 ;
					ret[ix][0] = 0 ;
					ret[ix][1] = 0 ;
					ret[ix][2] = 0 ;
					ret[ix][3] = -r11x[3] * r12x[0] ;
					ret[ix][4] = -r11x[3] * r12x[1] ;
					ret[ix][5] = -r11x[3] * r12x[3] ;
					ret[ix][6] = r11x[1] * r12x[0] ;
					ret[ix][7] = r11x[1] * r12x[1] ;
					ret[ix][8] = r11x[1] * r12x[3] ;
				}
				if ifdo (TRUE) {
					INDEX ix = i * 2 + 1 ;
					ret[ix][0] = r11x[3] * r12x[0] ;
					ret[ix][1] = r11x[3] * r12x[1] ;
					ret[ix][2] = r11x[3] * r12x[3] ;
					ret[ix][3] = 0 ;
					ret[ix][4] = 0 ;
					ret[ix][5] = 0 ;
					ret[ix][6] = -r11x[0] * r12x[0] ;
					ret[ix][7] = -r11x[0] * r12x[1] ;
					ret[ix][8] = -r11x[0] * r12x[3] ;
				}
			}
			return move (ret) ;
		}) ;
		const auto r13x = Image<FLT64> (LinearProc::solve_lsm (r10x)) ;
		const auto r14x = invoke ([&] () {
			Matrix ret = Matrix::zero () ;
			ret[0][0] = r13x[0][0] ;
			ret[0][1] = r13x[1][0] ;
			ret[0][3] = r13x[2][0] ;
			ret[1][0] = r13x[3][0] ;
			ret[1][1] = r13x[4][0] ;
			ret[1][3] = r13x[5][0] ;
			ret[3][0] = r13x[6][0] ;
			ret[3][1] = r13x[7][0] ;
			ret[3][3] = r13x[8][0] ;
			return move (ret) ;
		}) ;
		const auto r15x = r8x[0] * r14x * r9x[1] ;
		const auto r16x = MatrixProc::solve_svd (r15x) ;
		const auto r17x = (r16x.mU * r16x.mV).det () ;
		assume (r17x > 0) ;
		const auto r18x = invoke ([&] () {
			Array<KRTResult> ret = Array<KRTResult> (4) ;
			const auto r19x = MathProc::sqrt (MathProc::square (r16x.mS[0][0]) - MathProc::square (r16x.mS[1][1])) ;
			const auto r20x = MathProc::sqrt (MathProc::square (r16x.mS[1][1]) - MathProc::square (r16x.mS[2][2])) ;
			const auto r21x = Vector (r19x ,0 ,r20x ,0).normalize () ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r22x = MathProc::sign (ByteProc::any_bit (BYTE (i) ,BYTE (0X02))) ;
				const auto r23x = MathProc::sign (ByteProc::any_bit (BYTE (i) ,BYTE (0X01))) ;
				const auto r24x = r21x[0] * r22x ;
				const auto r25x = r21x[2] * r23x ;
				const auto r26x = Vector (r24x ,0 ,-r25x ,0) ;
				ret[i].mT = TranslationMatrix (r16x.mU * r26x) ;
				const auto r27x = r24x * (r16x.mS[0][0] - r16x.mS[2][2]) ;
				const auto r28x = r16x.mS[0][0] - r26x[0] * r27x ;
				const auto r29x = r26x[2] * r27x ;
				const auto r30x = Vector (r28x ,0 ,r29x ,0) ;
				const auto r31x = (r16x.mS[0][0] - r16x.mS[2][2]) * MathProc::inverse (r30x.magnitude ()) ;
				const auto r32x = Vector (r24x ,0 ,+r25x ,0) * r31x ;
				ret[i].mK = TranslationMatrix (r16x.mV * r32x) ;
				const auto r33x = r30x.normalize () ;
				const auto r34x = invoke ([&] () {
					Matrix ret = Matrix::identity () ;
					ret[0][0] = r33x[0] ;
					ret[0][2] = +r33x[2] ;
					ret[2][0] = -r33x[2] ;
					ret[2][2] = r33x[0] ;
					return move (ret) ;
				}) ;
				ret[i].mR = r16x.mU * r34x * r16x.mV.transpose () ;
			}
			return move (ret) ;
		}) ;
		const auto r35x = triangulate_select (r2x ,r5x ,r18x ,view2.mBaseline) ;
		view2.mMatV = r35x.mT * r35x.mR ;
		const auto r36x = view2.mMatV[0] * Vector::axis_z () ;
		const auto r37x = view2.mMatV[0] * Vector::axis_w () ;
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mDirection = [$2 ,$3 ,$4]")) (view1.mName ,r36x[0] ,r36x[1] ,r36x[2])) ;
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mTranslation = [$2 ,$3 ,$4]")) (view2.mName ,r37x[0] ,r37x[1] ,r37x[2])) ;
		if ifdo (TRUE) {
			auto rax = NormalError () ;
			const auto r38x = r35x.mT * Vector::axis_w () ;
			const auto r39x = r35x.mK * Vector::axis_w () ;
			const auto r40x = r38x.homogenize ().normalize () ;
			const auto r41x = r39x.homogenize () ;
			const auto r42x = SymmetryMatrix (r40x ,r41x) + r35x.mR ;
			const auto r43x = view1.mMatK[0] ;
			const auto r44x = r43x * r42x ;
			for (auto &&j : r5x.range ()) {
				const auto r45x = (r44x * r5x[j]).projection () ;
				const auto r46x = (r43x * r2x[j]).projection () ;
				const auto r47x = (r45x - r46x).magnitude () ;
				rax += r47x ;
			}
			watch (rax) ;
			watch (rax) ;
		}
	}

	KRTResult triangulate_select (CREF<Array<Vector>> point1 ,CREF<Array<Vector>> point2 ,CREF<Array<KRTResult>> candiate ,CREF<FLT64> baseline) const {
		KRTResult ret ;
		auto rax = FLT64 (0) ;
		for (auto &&i : candiate.range ()) {
			const auto r1x = candiate[i].mT * Vector::axis_w () ;
			const auto r2x = r1x.homogenize ().normalize () * baseline ;
			const auto r3x = candiate[i].mR ;
			const auto r4x = invoke ([&] () {
				FLT64 ret = 0 ;
				for (auto &&i : point1.range ()) {
					const auto r5x = point1[i] ;
					const auto r6x = point2[i] ;
					const auto r7x = -(r3x * r6x).normalize () ;
					const auto r8x = Matrix (r5x ,r7x ,r5x ^ r7x ,Vector::axis_w ()) ;
					const auto r9x = r8x.inverse () * r1x ;
					if (r9x[0] <= 0)
						continue ;
					if (r9x[1] <= 0)
						continue ;
					ret += 1 ;
				}
				return move (ret) ;
			}) ;
			if (rax >= r4x)
				continue ;
			ret = candiate[i] ;
			ret.mT = TranslationMatrix (r2x) ;
			rax = r4x ;
		}
		assume (rax > 0) ;
		return move (ret) ;
	}

	void work_sfm_pose_mat_v () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_sfm_pose_mat_v")) ;
		assume (self.mPose.length () > 0) ;
		for (auto &&i : self.mPose.range ()) {
			INDEX ix = self.mPose[i].mFrame1 ;
			for (auto &&j : self.mBlock.range ()) {
				work_sfm_projection (ix ,j) ;
				self.mBlock[j].mUseFrame.add (ix) ;
				self.mFrame[ix].mUseBlock.add (j) ;
			}
			check_projection_error (ix) ;
			work_sfm_global_ba (FALSE ,FALSE ,FALSE ,TRUE ,FALSE) ;
			for (auto &&j : self.mBlock.range ()) {
				for (auto &&k : self.mPose[i].mUseView) {
					INDEX kx = self.mPose[i].mUseView.map (k) ;
					self.mBlock[j].mUseFrame.add (kx) ;
					self.mFrame[kx].mUseBlock.add (j) ;
				}
			}
			work_sfm_global_ba (FALSE ,FALSE ,FALSE ,FALSE ,FALSE) ;
			save_camera_ply () ;
			work_sfm_global_ba (FALSE ,FALSE ,TRUE ,TRUE ,FALSE) ;
		}
	}

	void work_sfm_projection (CREF<INDEX> frame1 ,CREF<INDEX> block1) {
		auto &&pose1 = self.mPose[self.mFrame[frame1].mPose1] ;
		auto &&view1 = self.mView[self.mFrame[frame1].mView1] ;
		const auto r1x = SolverProc::solve_pnp (self.mFrame[frame1].mPoint2D ,self.mBlock[block1].mPoint3D ,view1.mMatK ,view1.mDist) ;
		const auto r2x = r1x.inverse () * view1.mMatV[1] ;
		pose1.mMatV = r2x ;
		pose1.mUsingMatV = TRUE ;
	}

	void check_epipolar_error (CREF<INDEX> frame1 ,CREF<INDEX> frame2) {
		auto &&view1 = self.mView[self.mFrame[frame1].mView1] ;
		auto &&view2 = self.mView[self.mFrame[frame2].mView1] ;
		Singleton<Console>::instance ().trace (Format (slice ("epipolar_error[$1][$2]")) (view1.mName ,view2.mName)) ;
		auto rax = NormalError () ;
		const auto r1x = view1.mMatV[1] * view2.mMatV[0] ;
		const auto r2x = r1x.homogenize () + Matrix::axis_w () ;
		const auto r3x = r1x * Vector::axis_w () ;
		const auto r4x = r3x.homogenize ().normalize () ;
		const auto r5x = CrossProductMatrix (r4x) * r2x ;
		for (auto &&j : self.mFrame[frame1].mPoint2D.range ()) {
			const auto r6x = Vector (self.mFrame[frame1].mPoint2D[j]) ;
			const auto r7x = Vector (self.mFrame[frame2].mPoint2D[j]) ;
			const auto r8x = (view1.mMatK[1] * r6x).normalize () ;
			const auto r9x = (view2.mMatK[1] * r7x).normalize () ;
			const auto r10x = r8x * r5x * r9x ;
			const auto r11x = abs (r10x) ;
			rax += r11x ;
		}
		Singleton<Console>::instance ().debug (slice ("mMaxError = ") ,rax.mMaxError) ;
		Singleton<Console>::instance ().debug (slice ("mAvgError = ") ,rax.mAvgError) ;
		Singleton<Console>::instance ().debug (slice ("mStdError = ") ,rax.mStdError) ;
		Singleton<Console>::instance ().trace () ;
	}

	void check_projection_error (CREF<INDEX> frame1) {
		auto &&pose1 = self.mPose[self.mFrame[frame1].mPose1] ;
		auto &&view1 = self.mView[self.mFrame[frame1].mView1] ;
		Singleton<Console>::instance ().trace (Format (slice ("projection_error[$1]")) (pose1.mName)) ;
		auto rax = NormalError () ;
		for (auto &&i : self.mFrame[frame1].mUseBlock) {
			for (auto &&j : self.mBlock[i].mPoint3D.range ()) {
				const auto r1x = Vector (self.mBlock[i].mPoint3D[j]) ;
				const auto r2x = Vector (self.mFrame[frame1].mPoint2D[j]) ;
				const auto r3x = pose1.mMatV[1] * r1x ;
				const auto r4x = view1.mMatV[1] * r3x ;
				const auto r5x = (view1.mMatK[0] * r4x).projection () ;
				const auto r6x = (r5x - r2x).magnitude () ;
				rax += r6x ;
			}
		}
		Singleton<Console>::instance ().debug (slice ("mMaxError = ") ,rax.mMaxError) ;
		Singleton<Console>::instance ().debug (slice ("mAvgError = ") ,rax.mAvgError) ;
		Singleton<Console>::instance ().debug (slice ("mStdError = ") ,rax.mStdError) ;
		Singleton<Console>::instance ().trace () ;
	}

	void check_homography_error (CREF<INDEX> view1 ,CREF<INDEX> view2) {
		Singleton<Console>::instance ().trace (Format (slice ("homography_error[$1][$2]")) (self.mView[view1].mName ,self.mView[view2].mName)) ;
		auto rax = NormalError () ;
		for (auto &&i : self.mPose.range ()) {
			if (!self.mPose[i].mUsingMatH)
				continue ;
			if (!self.mPose[i].mUseView.contain (view1))
				continue ;
			if (!self.mPose[i].mUseView.contain (view2))
				continue ;
			auto rbx = NormalError () ;
			INDEX ix = self.mPose[i].mUseView.map (view1) ;
			INDEX iy = self.mPose[i].mUseView.map (view2) ;
			for (auto &&j : self.mFrame[ix].mPointRay.range ()) {
				const auto r1x = Vector (self.mFrame[ix].mPointRay[j]) ;
				const auto r2x = Vector (self.mFrame[iy].mPointRay[j]) ;
				const auto r3x = (self.mView[view1].mMatK[0] * r1x).projection () ;
				const auto r4x = (self.mView[view2].mMatK[0] * r2x).projection () ;
				const auto r5x = (self.mPose[i].mMatH * r4x).projection () ;
				const auto r6x = (r5x - r3x).magnitude () ;
				rax += r6x ;
				rbx += r6x ;
			}
			self.mPose[i].mError = rbx ;
		}
		Singleton<Console>::instance ().debug (slice ("mMaxError = ") ,rax.mMaxError) ;
		Singleton<Console>::instance ().debug (slice ("mAvgError = ") ,rax.mAvgError) ;
		Singleton<Console>::instance ().debug (slice ("mStdError = ") ,rax.mStdError) ;
		Singleton<Console>::instance ().trace () ;
	}

	void work_refine_pose () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_refine_pose")) ;
		INDEX ix = 0 ;
		const auto r1x = self.mView[ix].mMatV ;
		auto rax = Set<INDEX> () ;
		for (auto &&i : self.mPose.range ()) {
			if (!self.mPose[i].mUsingMatV)
				continue ;
			if (!self.mPose[i].mUseView.contain (ix))
				continue ;
			const auto r2x = self.mPose[i].mMatV[0] * r1x[0] ;
			self.mPose[i].mMatV = r2x ;
			for (auto &&j : self.mPose[i].mUseView) {
				rax.add (j) ;
			}
		}
		for (auto &&i : rax) {
			const auto r3x = r1x[1] * self.mView[i].mMatV[0] ;
			self.mView[i].mMatV = r3x ;
		}
	}

	void work_sfm_global_ba (CREF<BOOL> opt_view_mat_k ,CREF<BOOL> opt_view_dist ,CREF<BOOL> opt_view_mat_v ,CREF<BOOL> opt_pose_mat_v ,CREF<BOOL> opt_point_3d) {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_sfm_global_ba")) ;
		if ifdo (TRUE) {
			self.mProblem = Box<ceres::Problem>::make () ;
			INDEX ix = 0 ;
			for (auto &&i : self.mView.range ()) {
				self.mView[i].mParam1 = ix ;
				ix += 4 ;
				self.mView[i].mParam2 = ix ;
				ix += 5 ;
				self.mView[i].mParam3 = ix ;
				ix += 6 ;
			}
			for (auto &&i : self.mPose.range ()) {
				self.mPose[i].mParam4 = ix ;
				ix += 6 ;
			}
			for (auto &&i : self.mBlock.range ()) {
				self.mBlock[i].mParam5 = ix ;
				ix += self.mBlock[i].mPoint3D.length () * 3 ;
			}
			if (ix <= self.mFunc.size ())
				discard ;
			self.mFunc = Array<FLT64> (ix) ;
		}
		for (auto &&i : self.mView.range ()) {
			const auto view_mat_k_fx = self.mView[i].mMatK[0][0][0] ;
			const auto view_mat_k_fy = self.mView[i].mMatK[0][1][1] ;
			const auto view_mat_k_cx = self.mView[i].mMatK[0][0][2] ;
			const auto view_mat_k_cy = self.mView[i].mMatK[0][1][2] ;
			self.mFunc[self.mView[i].mParam1 + 0] = view_mat_k_fx ;
			self.mFunc[self.mView[i].mParam1 + 1] = view_mat_k_fy ;
			self.mFunc[self.mView[i].mParam1 + 2] = view_mat_k_cx ;
			self.mFunc[self.mView[i].mParam1 + 3] = view_mat_k_cy ;
			self.mProblem->AddParameterBlock ((&self.mFunc[self.mView[i].mParam1]) ,4) ;
			if (opt_view_mat_k)
				if (!self.mView[i].mConstMatK)
					continue ;
			self.mProblem->SetParameterBlockConstant ((&self.mFunc[self.mView[i].mParam1])) ;
		}
		for (auto &&i : self.mView.range ()) {
			const auto view_dist_k1 = self.mView[i].mDist[0] ;
			const auto view_dist_k2 = self.mView[i].mDist[1] ;
			const auto view_dist_t1 = self.mView[i].mDist[2] ;
			const auto view_dist_t2 = self.mView[i].mDist[3] ;
			const auto view_dist_k3 = self.mView[i].mDist[4] ;
			self.mFunc[self.mView[i].mParam2 + 0] = view_dist_k1 ;
			self.mFunc[self.mView[i].mParam2 + 1] = view_dist_k2 ;
			self.mFunc[self.mView[i].mParam2 + 2] = view_dist_t1 ;
			self.mFunc[self.mView[i].mParam2 + 3] = view_dist_t2 ;
			self.mFunc[self.mView[i].mParam2 + 4] = view_dist_k3 ;
			self.mProblem->AddParameterBlock ((&self.mFunc[self.mView[i].mParam2]) ,5) ;
			if (opt_view_dist)
				if (!self.mView[i].mConstDist)
					continue ;
			self.mProblem->SetParameterBlockConstant ((&self.mFunc[self.mView[i].mParam2])) ;
		}
		for (auto &&i : self.mView.range ()) {
			const auto r1x = SolverProc::encode_rotation (self.mView[i].mMatV[1]) ;
			const auto r2x = self.mView[i].mMatV[1] * Vector::axis_w () ;
			const auto view_mat_v_r1 = r1x[0] ;
			const auto view_mat_v_r2 = r1x[1] ;
			const auto view_mat_v_r3 = r1x[2] ;
			const auto view_mat_v_t1 = r2x[0] ;
			const auto view_mat_v_t2 = r2x[1] ;
			const auto view_mat_v_t3 = r2x[2] ;
			self.mFunc[self.mView[i].mParam3 + 0] = view_mat_v_r1 ;
			self.mFunc[self.mView[i].mParam3 + 1] = view_mat_v_r2 ;
			self.mFunc[self.mView[i].mParam3 + 2] = view_mat_v_r3 ;
			self.mFunc[self.mView[i].mParam3 + 3] = view_mat_v_t1 ;
			self.mFunc[self.mView[i].mParam3 + 4] = view_mat_v_t2 ;
			self.mFunc[self.mView[i].mParam3 + 5] = view_mat_v_t3 ;
			self.mProblem->AddParameterBlock ((&self.mFunc[self.mView[i].mParam3]) ,6) ;
			if (opt_view_mat_v)
				if (!self.mView[i].mConstMatV)
					continue ;
			self.mProblem->SetParameterBlockConstant ((&self.mFunc[self.mView[i].mParam3])) ;
		}
		for (auto &&i : self.mPose.range ()) {
			const auto r3x = SolverProc::encode_rotation (self.mPose[i].mMatV[1]) ;
			const auto r4x = self.mPose[i].mMatV[1] * Vector::axis_w () ;
			const auto view_mat_v_r1 = r3x[0] ;
			const auto view_mat_v_r2 = r3x[1] ;
			const auto view_mat_v_r3 = r3x[2] ;
			const auto view_mat_v_t1 = r4x[0] ;
			const auto view_mat_v_t2 = r4x[1] ;
			const auto view_mat_v_t3 = r4x[2] ;
			self.mFunc[self.mPose[i].mParam4 + 0] = view_mat_v_r1 ;
			self.mFunc[self.mPose[i].mParam4 + 1] = view_mat_v_r2 ;
			self.mFunc[self.mPose[i].mParam4 + 2] = view_mat_v_r3 ;
			self.mFunc[self.mPose[i].mParam4 + 3] = view_mat_v_t1 ;
			self.mFunc[self.mPose[i].mParam4 + 4] = view_mat_v_t2 ;
			self.mFunc[self.mPose[i].mParam4 + 5] = view_mat_v_t3 ;
			self.mProblem->AddParameterBlock ((&self.mFunc[self.mPose[i].mParam4]) ,6) ;
			if (opt_pose_mat_v)
				continue ;
			self.mProblem->SetParameterBlockConstant ((&self.mFunc[self.mPose[i].mParam4])) ;
		}
		const auto r5x = new ceres::TrivialLoss () ;
		for (auto &&i : self.mBlock.range ()) {
			for (auto &&j : self.mBlock[i].mPoint3D.range ()) {
				const auto r6x = self.mBlock[i].mParam5 + j * 3 ;
				self.mFunc[r6x + 0] = self.mBlock[i].mPoint3D[j].mX ;
				self.mFunc[r6x + 1] = self.mBlock[i].mPoint3D[j].mY ;
				self.mFunc[r6x + 2] = self.mBlock[i].mPoint3D[j].mZ ;
				for (auto &&k : self.mBlock[i].mUseFrame) {
					INDEX ix = self.mFrame[k].mPose1 ;
					INDEX jx = self.mFrame[k].mView1 ;
					const auto r7x = self.mFrame[k].mPoint2D[j] ;
					const auto r8x = self.mPose[ix].mWeight ;
					const auto r9x = ResidualErrorPinhole::create (r7x ,r8x) ;
					const auto view_mat_k = (&self.mFunc[self.mView[jx].mParam1]) ;
					const auto view_dist = (&self.mFunc[self.mView[jx].mParam2]) ;
					const auto view_mat_v = (&self.mFunc[self.mView[jx].mParam3]) ;
					const auto pose_mat_v = (&self.mFunc[self.mPose[ix].mParam4]) ;
					const auto point2 = (&self.mFunc[r6x]) ;
					self.mProblem->AddResidualBlock (r9x ,r5x ,view_mat_k ,view_dist ,view_mat_v ,pose_mat_v ,point2) ;
					if (opt_point_3d)
						continue ;
					self.mProblem->SetParameterBlockConstant (point2) ;
				}
			}
		}
		self.mOptions.minimizer_progress_to_stdout = TRUE ;
		self.mOptions.logging_type = ceres::LoggingType::SILENT ;
		ceres::Solve (self.mOptions ,(&self.mProblem.deref) ,(&self.mSummary)) ;
		Singleton<Console>::instance ().debug (slice ("initial_cost = ") ,self.mSummary.initial_cost) ;
		Singleton<Console>::instance ().debug (slice ("final_cost = ") ,self.mSummary.final_cost) ;
		assume (self.mSummary.initial_cost >= 0) ;
		assume (self.mSummary.final_cost >= 0) ;
		for (auto &&i : self.mView.range ()) {
			const auto view_mat_k_fx = self.mFunc[self.mView[i].mParam1 + 0] ;
			const auto view_mat_k_fy = self.mFunc[self.mView[i].mParam1 + 1] ;
			const auto view_mat_k_cx = self.mFunc[self.mView[i].mParam1 + 2] ;
			const auto view_mat_k_cy = self.mFunc[self.mView[i].mParam1 + 3] ;
			const auto r10x = PerspectiveMatrix (view_mat_k_fx ,view_mat_k_fy ,view_mat_k_cx ,view_mat_k_cy) ;
			self.mView[i].mMatK = r10x ;
		}
		for (auto &&i : self.mView.range ()) {
			const auto view_dist_k1 = self.mFunc[self.mView[i].mParam2 + 0] ;
			const auto view_dist_k2 = self.mFunc[self.mView[i].mParam2 + 1] ;
			const auto view_dist_t1 = self.mFunc[self.mView[i].mParam2 + 2] ;
			const auto view_dist_t2 = self.mFunc[self.mView[i].mParam2 + 3] ;
			const auto view_dist_k3 = self.mFunc[self.mView[i].mParam2 + 4] ;
			self.mView[i].mDist[0] = view_dist_k1 ;
			self.mView[i].mDist[1] = view_dist_k2 ;
			self.mView[i].mDist[2] = view_dist_t1 ;
			self.mView[i].mDist[3] = view_dist_t2 ;
			self.mView[i].mDist[4] = view_dist_k3 ;
		}
		for (auto &&i : self.mView.range ()) {
			const auto view_mat_v_r1 = self.mFunc[self.mView[i].mParam3 + 0] ;
			const auto view_mat_v_r2 = self.mFunc[self.mView[i].mParam3 + 1] ;
			const auto view_mat_v_r3 = self.mFunc[self.mView[i].mParam3 + 2] ;
			const auto view_mat_v_t1 = self.mFunc[self.mView[i].mParam3 + 3] ;
			const auto view_mat_v_t2 = self.mFunc[self.mView[i].mParam3 + 4] ;
			const auto view_mat_v_t3 = self.mFunc[self.mView[i].mParam3 + 5] ;
			const auto r11x = SolverProc::decode_rotation ({view_mat_v_r1 ,view_mat_v_r2 ,view_mat_v_r3}) ;
			const auto r12x = TranslationMatrix (view_mat_v_t1 ,view_mat_v_t2 ,view_mat_v_t3) ;
			self.mView[i].mMatV = r12x * r11x ;
			self.mView[i].mMatV = self.mView[i].mMatV.inverse () ;
		}
		for (auto &&i : self.mPose.range ()) {
			const auto pose_mat_v_r1 = self.mFunc[self.mPose[i].mParam4 + 0] ;
			const auto pose_mat_v_r2 = self.mFunc[self.mPose[i].mParam4 + 1] ;
			const auto pose_mat_v_r3 = self.mFunc[self.mPose[i].mParam4 + 2] ;
			const auto pose_mat_v_t1 = self.mFunc[self.mPose[i].mParam4 + 3] ;
			const auto pose_mat_v_t2 = self.mFunc[self.mPose[i].mParam4 + 4] ;
			const auto pose_mat_v_t3 = self.mFunc[self.mPose[i].mParam4 + 5] ;
			const auto r13x = SolverProc::decode_rotation ({pose_mat_v_r1 ,pose_mat_v_r2 ,pose_mat_v_r3}) ;
			const auto r14x = TranslationMatrix (pose_mat_v_t1 ,pose_mat_v_t2 ,pose_mat_v_t3) ;
			self.mPose[i].mMatV = r14x * r13x ;
			self.mPose[i].mMatV = self.mPose[i].mMatV.inverse () ;
		}
		for (auto &&i : self.mPose.range ()) {
			if (self.mPose[i].mFrame1 == NONE)
				continue ;
			if (self.mPose[i].mFrame2 == NONE)
				continue ;
			INDEX ix = self.mFrame[self.mPose[i].mFrame1].mView1 ;
			INDEX iy = self.mFrame[self.mPose[i].mFrame2].mView1 ;
			const auto r15x = self.mPose[i].mMatV[1] * Vector::axis_z () ;
			const auto r16x = -(r15x * self.mPose[i].mMatV[1] * Vector::axis_w ()) ;
			const auto r17x = self.mView[ix].mMatV[0] * Vector::axis_w () ;
			const auto r18x = self.mView[iy].mMatV[0] * Vector::axis_w () ;
			const auto r19x = -(r15x * r18x + r16x) ;
			const auto r20x = DiagMatrix (r19x ,r19x ,r19x ,0) + SymmetryMatrix (r18x - r17x ,r15x) ;
			const auto r21x = self.mView[ix].mMatV[1] * r20x * self.mView[iy].mMatV[0] ;
			const auto r22x = r21x.homogenize () + Matrix::axis_w () ;
			self.mPose[i].mMatH = self.mView[ix].mMatK[0] * r22x * self.mView[iy].mMatK[1] ;
			self.mPose[i].mUsingMatH = TRUE ;
			const auto r23x = (r17x - r18x).magnitude () ;
			self.mView[ix].mRelative = r23x ;
			self.mView[iy].mRelative = r23x ;
		}
		for (auto &&i : self.mBlock.range ()) {
			for (auto &&j : self.mBlock[i].mPoint3D.range ()) {
				const auto r24x = self.mBlock[i].mParam5 + j * 3 ;
				self.mBlock[i].mPoint3D[j].mX = FLT32 (self.mFunc[r24x + 0]) ;
				self.mBlock[i].mPoint3D[j].mY = FLT32 (self.mFunc[r24x + 1]) ;
				self.mBlock[i].mPoint3D[j].mZ = FLT32 (self.mFunc[r24x + 2]) ;
				const auto r25x = Vector (self.mBlock[i].mPoint3D[j]) ;
				for (auto &&k : self.mBlock[i].mUseFrame) {
					INDEX ix = self.mFrame[k].mPose1 ;
					INDEX jx = self.mFrame[k].mView1 ;
					const auto r26x = self.mView[jx].mMatV[1] * self.mPose[ix].mMatV[1] ;
					const auto r27x = (r26x * r25x).homogenize ().magnitude () ;
					self.mFrame[k].mPointDepth[j] = FLT32 (r27x) ;
				}
			}
		}
		for (auto &&i : self.mFrame.range ()) {
			INDEX ix = self.mFrame[i].mView1 ;
			for (auto &&j : self.mFrame[i].mPoint2D.range ()) {
				const auto r28x = Vector (self.mFrame[i].mPoint2D[j]) ;
				const auto r29x = SolverProc::undistortion (self.mView[ix].mMatK ,self.mView[ix].mDist ,r28x) ;
				const auto r30x = (self.mView[ix].mMatK[1] * r29x).normalize () ;
				self.mFrame[i].mPointRay[j] = r30x ;
			}
		}
	}

	void work_sfm_pose_weight () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_sfm_pose_weight")) ;
		const auto r1x = Array<INDEX>::make (self.mView.range ()) ;
		assume (r1x.length () >= 2) ;
		check_homography_error (r1x[0] ,r1x[1]) ;
		const auto r2x = MathProc::inverse (FLT64 (2.0)) ;
		for (auto &&i : self.mPose.range ()) {
			const auto r3x = self.mPose[i].mError ;
			const auto r4x = (r3x.mAvgError + r3x.mStdError) * r2x ;
			const auto r5x = MathProc::max_of (r4x ,FLT64 (1)) ;
			const auto r6x = MathProc::log (r5x) ;
			const auto r7x = MathProc::exp (-r6x) ;
			self.mPose[i].mWeight = r7x ;
		}
	}

	void work_mapping_block () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_mapping_block")) ;
		self.mMapping = Array<Line2F> (self.mBlock.length ()) ;
		for (auto &&i : self.mBlock.range ()) {
			const auto r1x = PointCloud (Ref<Array<Point3F>>::reference (self.mBlock[i].mPoint3D)) ;
			const auto r2x = r1x.svd_matrix () ;
			const auto r3x = MatrixProc::solve_trs (r2x) ;
			self.mBlock[i].mMatP = r3x.mT * r3x.mR ;
			self.mBlock[i].mUsingMatP = TRUE ;
			const auto r4x = self.mBlock[i].mMatP[1] * r1x ;
			self.mBlock[i].mPlaneBound = r4x.bound () ;
			self.mBlock[i].mPlaneBound.mMin.mX -= FLT32 (self.mBoardBaseline[0]) ;
			self.mBlock[i].mPlaneBound.mMax.mX += FLT32 (self.mBoardBaseline[0]) ;
			self.mBlock[i].mPlaneBound.mMin.mY -= FLT32 (self.mBoardBaseline[1]) ;
			self.mBlock[i].mPlaneBound.mMax.mY += FLT32 (self.mBoardBaseline[1]) ;
			const auto r5x = self.mPose[0].mFrame1 ;
			assume (self.mBlock[i].mUseFrame.contain (r5x)) ;
			INDEX ix = self.mFrame[r5x].mPose1 ;
			INDEX jx = self.mFrame[r5x].mView1 ;
			self.mMapping[i].mMin.mX = +infinity ;
			self.mMapping[i].mMin.mY = +infinity ;
			self.mMapping[i].mMax.mX = -infinity ;
			self.mMapping[i].mMax.mY = -infinity ;
			const auto r6x = self.mView[jx].mMatK[0] * self.mView[jx].mMatV[1] ;
			const auto r7x = self.mPose[ix].mMatV[1] * self.mBlock[i].mMatP[0] ;
			const auto r8x = r6x * r7x ;
			const auto r9x = Vector (self.mBlock[i].mPlaneBound.mMin.mX ,self.mBlock[i].mPlaneBound.mMin.mY ,0 ,1) ;
			const auto r10x = Vector (self.mBlock[i].mPlaneBound.mMin.mX ,self.mBlock[i].mPlaneBound.mMax.mY ,0 ,1) ;
			const auto r11x = Vector (self.mBlock[i].mPlaneBound.mMax.mX ,self.mBlock[i].mPlaneBound.mMax.mY ,0 ,1) ;
			const auto r12x = Vector (self.mBlock[i].mPlaneBound.mMax.mX ,self.mBlock[i].mPlaneBound.mMin.mY ,0 ,1) ;
			const auto r13x = Buffer4<Vector> (r9x ,r10x ,r11x ,r12x) ;
			for (auto &&j : iter (0 ,r13x.size ())) {
				const auto r14x = (r8x * r13x[j]).projection () ;
				self.mMapping[i].mMin.mX = MathProc::min_of (self.mMapping[i].mMin.mX ,FLT32 (r14x[0])) ;
				self.mMapping[i].mMin.mY = MathProc::min_of (self.mMapping[i].mMin.mY ,FLT32 (r14x[1])) ;
				self.mMapping[i].mMax.mX = MathProc::max_of (self.mMapping[i].mMax.mX ,FLT32 (r14x[0])) ;
				self.mMapping[i].mMax.mY = MathProc::max_of (self.mMapping[i].mMax.mY ,FLT32 (r14x[1])) ;
			}
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
				mWriter.deref << mComma ;
				mWriter.deref << slice ("\"mBoardShape\":[") ;
				mComma++ ;
				mComma.tight () ;
				mWriter.deref << mComma ;
				mWriter.deref << self.mBoardShape.mCX ;
				mWriter.deref << mComma ;
				mWriter.deref << self.mBoardShape.mCY ;
				mComma-- ;
				mWriter.deref << mComma ;
				mWriter.deref << slice ("]") ;
				mWriter.deref << mComma ;
				mWriter.deref << slice ("\"mBoardBaseline\":[") ;
				mComma++ ;
				mComma.tight () ;
				mWriter.deref << mComma ;
				mWriter.deref << self.mBoardBaseline[0] ;
				mWriter.deref << mComma ;
				mWriter.deref << self.mBoardBaseline[1] ;
				mWriter.deref << mComma ;
				mWriter.deref << self.mBoardBaseline[2] ;
				mComma-- ;
				mWriter.deref << mComma ;
				mWriter.deref << slice ("]") ;
				mWriter.deref << mComma ;
				mWriter.deref << slice ("\"mBoardType\":") << slice ("\"") << self.mBoardType << slice ("\"")  ;
				mWriter.deref << mComma ;
				mWriter.deref << slice ("\"mImageGroup\":[") ;
				mComma++ ;
				for (auto &&i : self.mImageGroup.range ()) {
					mWriter.deref << mComma ;
					mWriter.deref << slice ("\"") << self.mImageGroup[i] << slice ("\"") ;
				}
				mComma-- ;
				mWriter.deref << mComma ;
				mWriter.deref << slice ("]") ;
				mWriter.deref << mComma ;
				mWriter.deref << slice ("\"mImageExtension\":") << slice ("\"") << self.mImageExtension << slice ("\"")  ;
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
					mWriter.deref << slice ("\"mUsePose\":[") ;
					mComma++ ;
					mComma.tight () ;
					for (auto &&j : self.mView[i].mUsePose) {
						mWriter.deref << mComma ;
						mWriter.deref << j ;
					}
					mComma-- ;
					mWriter.deref << mComma ;
					mWriter.deref << slice ("]") ;
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
					const auto r2x = SolverProc::encode_matrix (self.mView[i].mMatK[0]) ;
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
					const auto r3x = SolverProc::encode_matrix (self.mView[i].mMatV[0]) ;
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
					mWriter.deref << slice ("\"mBaseline\":") << self.mView[i].mBaseline ;
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
					const auto r4x = SolverProc::encode_matrix (self.mPose[i].mMatV[0]) ;
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
					const auto r5x = SolverProc::encode_matrix (self.mPose[i].mMatH) ;
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

	void save_pointcloud_ply () {
		const auto r1x = self.mDataPath.child (slice ("ply")) ;
		FileProc::build_dire (r1x) ;
		const auto r2x = r1x.child (slice ("pointcloud.ply")) ;
		auto mWriter = StreamFileTextWriter (r2x) ;
		mWriter.deref << slice ("ply") << GAP ;
		mWriter.deref << slice ("format ascii 1.0") << GAP ;
		//@mark
		const auto r3x = LENGTH (MathProc::ceil (self.mMapping[0].mMin.mX ,FLT32 (1))) ;
		const auto r4x = LENGTH (MathProc::ceil (self.mMapping[0].mMin.mY ,FLT32 (1))) ;
		const auto r5x = LENGTH (MathProc::ceil (self.mMapping[0].mMax.mX ,FLT32 (1))) ;
		const auto r6x = LENGTH (MathProc::ceil (self.mMapping[0].mMax.mY ,FLT32 (1))) ;
		const auto r7x = invoke ([&] () {
			ImageShape ret ;
			ret.mCX = r5x - r3x ;
			ret.mCY = r6x - r4x ;
			return move (ret) ;
		}) ;
		mWriter.deref << slice ("element vertex ") << r7x.size () << GAP ;
		mWriter.deref << slice ("property float x") << GAP ;
		mWriter.deref << slice ("property float y") << GAP ;
		mWriter.deref << slice ("property float z") << GAP ;
		mWriter.deref << slice ("property uchar red") << GAP ;
		mWriter.deref << slice ("property uchar green") << GAP ;
		mWriter.deref << slice ("property uchar blue") << GAP ;
		mWriter.deref << slice ("end_header") << GAP ;
		const auto r8x = self.mPose[0].mFrame1 ;
		self.mFrame[r8x].mImage = ImageProc::load_image (self.mFrame[r8x].mImageFile) ;
		INDEX ix = self.mFrame[r8x].mPose1 ;
		INDEX jx = self.mFrame[r8x].mView1 ;
		const auto r9x = self.mPose[jx].mMatV[0] * self.mView[jx].mMatV[0] * self.mView[jx].mMatK[1] ;
		const auto r10x = self.mBlock[0].mMatP[0] * Vector::axis_z () ;
		const auto r11x = self.mBlock[0].mMatP[0] * Vector::axis_w () ;
		const auto r12x = self.mPose[jx].mMatV[0] * self.mView[jx].mMatV[0] * Vector::axis_w () ;
		const auto r13x = r10x * (r11x - r12x) ;
		for (auto &&i : iter (0 ,r7x.mCX ,0 ,r7x.mCY)) {
			const auto r14x = Pixel ({r3x + i.mX ,r4x + i.mY}) ;
			const auto r15x = Vector (r14x) ;
			const auto r16x = SolverProc::undistortion (self.mView[jx].mMatK ,self.mView[jx].mDist ,r15x) ;
			const auto r17x = (r9x * r16x).normalize () ;
			const auto r18x = r10x * r17x ;
			const auto r19x = r13x * MathProc::inverse (r18x) ;
			const auto r20x = r17x * r19x + r12x ;
			const auto r21x = self.mFrame[r8x].mImage[r14x] ;
			mWriter.deref << r20x[0] << slice (" ") ;
			mWriter.deref << r20x[1] << slice (" ") ;
			mWriter.deref << r20x[2] << GAP ;
			mWriter.deref << VAL32 (r21x.mR) << slice (" ") ;
			mWriter.deref << VAL32 (r21x.mG) << slice (" ") ;
			mWriter.deref << VAL32 (r21x.mB) << GAP ;
		}
		mWriter.flush () ;
	}

	void save_camera_ply () {
		const auto r1x = self.mDataPath.child (slice ("ply")) ;
		FileProc::build_dire (r1x) ;
		for (auto &&i : self.mPose.range ()) {
			if (!self.mPose[i].mUsingMatV)
				continue ;
			for (auto &&j : self.mPose[i].mUseView) {
				INDEX ix = self.mPose[i].mUseView.map (j) ;
				const auto r2x = r1x.child (Format (slice ("$1_$2.ply")) (self.mPose[i].mName ,self.mView[j].mName)) ;
				auto mWriter = StreamFileTextWriter (r2x) ;
				mWriter.deref << slice ("ply") << GAP ;
				mWriter.deref << slice ("format ascii 1.0") << GAP ;
				mWriter.deref << slice ("element vertex ") << 5 + self.mFrame[ix].mPoint2D.size () << GAP ;
				mWriter.deref << slice ("property float x") << GAP ;
				mWriter.deref << slice ("property float y") << GAP ;
				mWriter.deref << slice ("property float z") << GAP ;
				mWriter.deref << slice ("property uchar red") << GAP ;
				mWriter.deref << slice ("property uchar green") << GAP ;
				mWriter.deref << slice ("property uchar blue") << GAP ;
				mWriter.deref << slice ("element edge ") << 8 << GAP ;
				mWriter.deref << slice ("property int vertex1") << GAP ;
				mWriter.deref << slice ("property int vertex2") << GAP ;
				mWriter.deref << slice ("end_header") << GAP ;
				const auto r3x = self.mPose[i].mMatV[0] * self.mView[j].mMatV[0] ;
				if ifdo (TRUE) {
					const auto r4x = r3x * Vector::axis_w () ;
					mWriter.deref << r4x[0] << slice (" ") ;
					mWriter.deref << r4x[1] << slice (" ") ;
					mWriter.deref << r4x[2] << GAP ;
					mWriter.deref << VAL32 (self.mPose[i].mColor.mR) << slice (" ") ;
					mWriter.deref << VAL32 (self.mPose[i].mColor.mG) << slice (" ") ;
					mWriter.deref << VAL32 (self.mPose[i].mColor.mB) << GAP ;
					const auto r5x = Vector (0 ,0 ,0 ,1) ;
					const auto r6x = Vector (FLT64 (self.mView[j].mShape.mCX) ,0 ,0 ,1) ;
					const auto r7x = Vector (FLT64 (self.mView[j].mShape.mCX) ,FLT64 (self.mView[j].mShape.mCY) ,0 ,1) ;
					const auto r8x = Vector (0 ,FLT64 (self.mView[j].mShape.mCY) ,0 ,1) ;
					const auto r9x = Buffer4<Vector> ({r5x ,r6x ,r7x ,r8x}) ;
					for (auto &&k : iter (0 ,r9x.size ())) {
						const auto r10x = self.mView[j].mMatK[1] * r9x[k] ;
						const auto r11x = self.mView[j].mRelative * MathProc::inverse (r10x[2]) ;
						const auto r12x = r10x * r11x + Vector::axis_w () ;
						const auto r13x = r3x * r12x ;
						mWriter.deref << r13x[0] << slice (" ") ;
						mWriter.deref << r13x[1] << slice (" ") ;
						mWriter.deref << r13x[2] << GAP ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mR) << slice (" ") ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mG) << slice (" ") ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mB) << GAP ;
					}
				}
				if ifdo (TRUE) {
					for (auto &&k : self.mFrame[ix].mPoint2D.range ()) {
						const auto r14x = self.mFrame[ix].mPointRay[k] ;
						const auto r15x = r14x * self.mFrame[ix].mPointDepth[k] + Vector::axis_w () ;
						const auto r16x = r3x * r15x ;
						mWriter.deref << r16x[0] << slice (" ") ;
						mWriter.deref << r16x[1] << slice (" ") ;
						mWriter.deref << r16x[2] << GAP ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mR) << slice (" ") ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mG) << slice (" ") ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mB) << GAP ;
					}
				}
				if ifdo (FALSE) {
					for (auto &&k : self.mFrame[ix].mPoint2D.range ()) {
						const auto r17x = self.mFrame[ix].mPointRay[k] ;
						const auto r18x = self.mView[j].mRelative * MathProc::inverse (r17x[2]) ;
						const auto r19x = r17x * r18x + Vector::axis_w () ;
						const auto r20x = r3x * r19x ;
						mWriter.deref << r20x[0] << slice (" ") ;
						mWriter.deref << r20x[1] << slice (" ") ;
						mWriter.deref << r20x[2] << GAP ;
						mWriter.deref << slice ("200 200 200") << GAP ;
					}
				}
				if ifdo (TRUE) {
					mWriter.deref << slice ("0 1") << GAP ;
					mWriter.deref << slice ("0 2") << GAP ;
					mWriter.deref << slice ("0 3") << GAP ;
					mWriter.deref << slice ("0 4") << GAP ;
					mWriter.deref << slice ("1 2") << GAP ;
					mWriter.deref << slice ("2 3") << GAP ;
					mWriter.deref << slice ("3 4") << GAP ;
					mWriter.deref << slice ("4 1") << GAP ;
				}
				if ifdo (FALSE) {
					for (auto &&k : self.mFrame[ix].mPoint2D.range ()) {
						const auto r21x = 5 + k ;
						mWriter.deref << 0 << slice (" ") ;
						mWriter.deref << r21x << GAP ;
					}
				}
				mWriter.flush () ;
			}
		}
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

	struct ResidualErrorPinhole {
		Point2F mPoint2D ;
		FLT64 mWeight ;

		explicit ResidualErrorPinhole (CREF<Point2F> point1 ,CREF<FLT64> weight)
			:mPoint2D (point1) ,mWeight (weight) {}

		template <typename T>
		BOOL operator() (const T *view_mat_k ,const T *view_dist ,const T *view_mat_v ,const T *pose_mat_v ,const T *point2 ,T *residuals) const {
			const auto pose_mat_v_r = (&pose_mat_v[0]) ;
			const auto pose_mat_v_t = (&pose_mat_v[3]) ;
			const auto view_mat_v_r = (&view_mat_v[0]) ;
			const auto view_mat_v_t = (&view_mat_v[3]) ;
			auto rax = Buffer3<T> () ;
			rax[0] = point2[0] ;
			rax[1] = point2[1] ;
			rax[2] = point2[2] ;
			const auto r1x = rax ;
			ceres::AngleAxisRotatePoint (pose_mat_v_r ,(&r1x[0]) ,(&rax[0])) ;
			rax[0] += pose_mat_v_t[0] ;
			rax[1] += pose_mat_v_t[1] ;
			rax[2] += pose_mat_v_t[2] ;
			const auto r2x = rax ;
			ceres::AngleAxisRotatePoint (view_mat_v_r ,(&r2x[0]) ,(&rax[0])) ;
			rax[0] += view_mat_v_t[0] ;
			rax[1] += view_mat_v_t[1] ;
			rax[2] += view_mat_v_t[2] ;
			if (rax[2] < T (0))
				return FALSE ;
			rax[0] /= rax[2] ;
			rax[1] /= rax[2] ;
			rax[2] = T (0) ;
			const auto view_dist_k1 = view_dist[0] ;
			const auto view_dist_k2 = view_dist[1] ;
			const auto view_dist_t1 = view_dist[2] ;
			const auto view_dist_t2 = view_dist[3] ;
			const auto view_dist_k3 = view_dist[4] ;
			const auto r3x = rax[0] * rax[0] + rax[1] * rax[1] ;
			const auto r4x = r3x * r3x ;
			const auto r5x = r4x * r3x ;
			const auto r6x = T (1) + view_dist_k1 * r3x + view_dist_k2 * r4x + view_dist_k3 * r5x ;
			const auto r7x = T (2) * rax[0] * rax[1] ;
			const auto r8x = T (2) * rax[0] * rax[0] + r3x ;
			const auto r9x = T (2) * rax[1] * rax[1] + r3x ;
			rax[0] = r6x * rax[0] + view_dist_t1 * r7x + view_dist_t2 * r8x ;
			rax[1] = r6x * rax[1] + view_dist_t1 * r9x + view_dist_t2 * r7x ;
			const auto view_mat_k_fx = view_mat_k[0] ;
			const auto view_mat_k_fy = view_mat_k[1] ;
			const auto view_mat_k_cx = view_mat_k[2] ;
			const auto view_mat_k_cy = view_mat_k[3] ;
			rax[0] = view_mat_k_fx * rax[0] + view_mat_k_cx ;
			rax[1] = view_mat_k_fy * rax[1] + view_mat_k_cy ;
			residuals[0] = (rax[0] - T (mPoint2D.mX)) * T (mWeight) ;
			residuals[1] = (rax[1] - T (mPoint2D.mY)) * T (mWeight) ;
			return TRUE ;
		}

		imports INDEX num_residuals () {
			return 2 ;
		}

		imports DEF<ceres::CostFunction *> create (CREF<Point2F> point1 ,CREF<FLT64> weight) {
			const auto r1x = new ResidualErrorPinhole (point1 ,weight) ;
			return new ceres::AutoDiffCostFunction<ResidualErrorPinhole ,2 ,4 ,5 ,6 ,6 ,3> (r1x) ;
		}
	} ;
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
