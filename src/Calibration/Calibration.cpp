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
		self.mBoardBaseline = Vector (54.75 ,54.75 ,1 ,1) ;
		self.mImageGroup = Array<String<STR>> ({slice ("image")}) ;
		self.mImageExtension = slice (".bmp") ;
	}

	void execute () override {
		load_data_json () ;
		work_build_frame () ;
		work_board_detection () ;
		work_sfm_view_mat_k () ;
		work_sfm_view_mat_v () ;
		work_sfm_pose_mat_v () ;
		work_sfm_global_ba (TRUE ,FALSE ,TRUE ,TRUE ,TRUE) ;
		work_sfm_global_ba (FALSE ,TRUE ,TRUE ,TRUE ,TRUE) ;
		work_mapping_world () ;
		work_sfm_global_ba (FALSE ,FALSE ,FALSE ,FALSE ,TRUE) ;
		work_undistortion () ;
		save_data_json () ;
		save_pointcloud_ply () ;
		save_camera_ply () ;
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
		self.mBoardBaseline[3] = 1 ;
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
			self.mPose[ix].mUsingMatH = TRUE ;
			const auto r8x = r1x[i].child (slice ("mError")) ;
			self.mPose[ix].mError.mMaxError = r8x.child (0).parse (FLT64 (0)) ;
			self.mPose[ix].mError.mAvgError = r8x.child (1).parse (FLT64 (0)) ;
			self.mPose[ix].mError.mStdError = r8x.child (2).parse (FLT64 (0)) ;
		}
	}

	void work_build_frame () {
		const auto r1x = Array<INDEX>::make (self.mViewNameSet.range ()) ;
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
			self.mView[ret].mShape.mCX = ZERO ;
			self.mView[ret].mShape.mCY = ZERO ;
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
			const auto r2x = Format (slice ("$1_$2$3")) ;
			const auto r3x = self.mDataPath.child (self.mPose[pose1].mGroup).child (r2x (self.mPose[pose1].mName ,self.mView[view1].mName ,self.mImageExtension)) ;
			self.mFrame[ret].mImageFile = r3x ;
			self.mFrame[ret].mImage = Image<Color3B> () ;
			self.mFrame[ret].mPoint2D = Array<Point2F> () ;
			self.mFrame[ret].mPointDepth = Array<FLT32> () ;
			self.mFrame[ret].mPointRay = Array<Point3F> () ;
			self.mFrame[ret].mUsingPointRay = FALSE ;
		}
		return move (ret) ;
	}

	void work_board_detection () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_board_detection")) ;
		self.mBoard.set_board_shape (self.mBoardShape) ;
		self.mBoard.set_board_type (BoardType::CIRCLE) ;
		self.mBoard.set_board_baseline (self.mBoardBaseline) ;
		if ifdo (TRUE) {
			self.mBlock = ArrayList<CameraBlock> () ;
			INDEX ix = self.mBlock.insert () ;
			self.mBlock[ix].mTime1 = ix ;
			self.mBlock[ix].mUseFrame = Set<INDEX> () ;
			self.mBlock[ix].mPoint3D = self.mBoard.extract () ;
			self.mBlock[ix].mConstPoint = TRUE ;
			self.mBlock.remap () ;
		}
		for (auto &&i : self.mFrame.range ()) {
			self.mFrame[i].mImage = ImageProc::load_image (self.mFrame[i].mImageFile) ;
			self.mFrame[i].mPoint2D = self.mBoard.detect (self.mFrame[i].mImage) ;
			self.mFrame[i].mPointDepth = Array<FLT32> (self.mFrame[i].mPoint2D.size ()) ;
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
			work_sfm_view_mat_k (ix ,iy) ;
		}
	}

	void work_sfm_view_mat_k (CREF<INDEX> frame1 ,CREF<INDEX> frame2) {
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
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_sfm_view_mat_v")) ;
		for (auto &&i : self.mPose.range ()) {
			INDEX jx = self.mFrame[self.mPose[i].mFrame1].mView1 ;
			if ifdo (TRUE) {
				if (self.mView[jx].mConstMatV)
					discard ;
				self.mView[jx].mMatV = Matrix::identity () ;
			}
			INDEX ix = self.mPose[i].mUseView.map (jx) ;
			assume (ix != NONE) ;
			for (auto &&j : self.mPose[i].mUseView) {
				if (self.mView[j].mConstMatV)
					continue ;
				if (j == jx)
					continue ;
				INDEX iy = self.mPose[i].mUseView.map (j) ;
				assume (iy != NONE) ;
				work_sfm_view_mat_v (ix ,iy) ;
				self.mView[j].mMatV = self.mView[jx].mMatV[0] * self.mView[j].mMatV[0] ;
			}
		}
	}

	void work_sfm_view_mat_v (CREF<INDEX> frame1 ,CREF<INDEX> frame2) {
		auto &&view1 = self.mView[self.mFrame[frame1].mView1] ;
		auto &&view2 = self.mView[self.mFrame[frame2].mView1] ;
		const auto r1x = invoke ([&] () {
			Array<Point3F> ret = Array<Point3F> (self.mFrame[frame1].mPoint2D.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r2x = Vector (self.mFrame[frame1].mPoint2D[i]) ;
				const auto r3x = (view1.mMatK[1] * r2x).normalize () ;
				ret[i] = r3x.xyz () ;
			}
			return move (ret) ;
		}) ;
		const auto r4x = invoke ([&] () {
			Array<Point3F> ret = Array<Point3F> (self.mFrame[frame2].mPoint2D.size ()) ;
			for (auto &&i : iter (0 ,ret.size ())) {
				const auto r5x = Vector (self.mFrame[frame2].mPoint2D[i]) ;
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
		const auto r21x = r20x * view1.mBaseline ;
		const auto r22x = TranslationMatrix (r21x) ;
		const auto r23x = r22x * r18x.mR ;
		view2.mMatV = r23x ;
		Singleton<Console>::instance ().debug (Format (slice ("mView[$1].mBaseline = $2")) (view1.mName ,view1.mBaseline)) ;
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

	void work_sfm_pose_mat_v () {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_sfm_pose_mat_v")) ;
		assume (self.mPose.length () > 0) ;
		INDEX jx = 0 ;
		for (auto &&i : self.mPose.range ()) {
			INDEX ix = self.mPose[i].mFrame1 ;
			work_sfm_pose_mat_v (ix ,jx) ;
			self.mBlock[jx].mUseFrame.add (ix) ;
			work_sfm_global_ba (FALSE ,FALSE ,FALSE ,TRUE ,FALSE) ;
			work_sfm_global_ba (FALSE ,FALSE ,TRUE ,TRUE ,TRUE) ;
		}
	}

	void work_sfm_pose_mat_v (CREF<INDEX> frame1 ,CREF<INDEX> block1) {
		auto &&view1 = self.mView[self.mFrame[frame1].mView1] ;
		auto &&pose1 = self.mPose[self.mFrame[frame1].mPose1] ;
		const auto r1x = SolverProc::solve_pnp (self.mFrame[frame1].mPoint2D ,self.mBlock[block1].mPoint3D ,view1.mMatK ,view1.mDist) ;
		const auto r2x = r1x * view1.mMatV[1] ;
		pose1.mMatV = r2x ;
		pose1.mUsingMatV = TRUE ;
	}

	void work_mapping_world () {

	}

	void work_sfm_global_ba (CREF<BOOL> opt_mat_k ,CREF<BOOL> opt_dist ,CREF<BOOL> opt_view_mat_v ,CREF<BOOL> opt_pose_mat_v ,CREF<BOOL> opt_point_3d) {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("work_sfm_global_ba")) ;
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
			if (!self.mView[i].mConstMatK)
				if (opt_mat_k)
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
			if (!self.mView[i].mConstDist)
				if (opt_dist)
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
			if (!self.mView[i].mConstMatV)
				if (opt_view_mat_v)
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
					const auto point_3d = (&self.mFunc[self.mBlock[i].mParam5]) ;
					self.mProblem->AddResidualBlock (r9x ,r5x ,view_mat_k ,view_dist ,view_mat_v ,pose_mat_v ,point_3d) ;
					if (!self.mBlock[i].mConstPoint)
						if (opt_point_3d)
							continue ;
					self.mProblem->SetParameterBlockConstant (point_3d) ;
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
			if (!self.mPose[i].mUsingMatH)
				continue ;
			INDEX ix = self.mFrame[self.mPose[i].mFrame1].mView1 ;
			INDEX iy = self.mFrame[self.mPose[i].mFrame2].mView1 ;
			const auto r15x = self.mPose[i].mMatV[1] * Vector::axis_z () ;
			const auto r16x = r15x * self.mPose[i].mMatV[1] * Vector::axis_w () ;
			const auto r17x = self.mView[ix].mMatV[0] * Vector::axis_w () ;
			const auto r18x = self.mView[iy].mMatV[0] * Vector::axis_w () ;
			const auto r19x = MathProc::abs (r15x * r18x - r16x) ;
			const auto r20x = DiagMatrix (r19x ,r19x ,r19x ,0) + SymmetryMatrix (r18x - r17x ,r15x) ;
			const auto r21x = self.mView[ix].mMatV[1] * r20x * self.mView[iy].mMatV[0] ;
			const auto r22x = r21x.homogenize () + Matrix::axis_w () ;
			self.mPose[i].mMatH = self.mView[ix].mMatK[0] * r22x * self.mView[iy].mMatK[1] ;
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
			}
			for (auto &&j : self.mBlock[i].mUseFrame) {
				INDEX ix = self.mFrame[j].mPose1 ;
				INDEX jx = self.mFrame[j].mView1 ;
				const auto r25x = self.mPose[jx].mMatV[1] * self.mPose[ix].mMatV[1] ;
				for (auto &&k : self.mBlock[i].mPoint3D.range ()) {
					const auto r26x = Vector (self.mBlock[i].mPoint3D[k]) ;
					const auto r27x = (r25x * r26x).homogenize ().magnitude () ;
					self.mFrame[j].mPointDepth[k] = FLT32 (r27x) ;
				}
			}
		}
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
		const auto r3x = invoke ([&] () {
			LENGTH ret = 0 ;
			for (auto &&i : self.mBlock)
				ret += i.mPoint3D.length () ;
			return move (ret) ;
		}) ;
		mWriter.deref << slice ("element vertex ") << r3x << GAP ;
		mWriter.deref << slice ("property float x") << GAP ;
		mWriter.deref << slice ("property float y") << GAP ;
		mWriter.deref << slice ("property float z") << GAP ;
		mWriter.deref << slice ("property uchar red") << GAP ;
		mWriter.deref << slice ("property uchar green") << GAP ;
		mWriter.deref << slice ("property uchar blue") << GAP ;
		mWriter.deref << slice ("end_header") << GAP ;
		for (auto &&i : self.mBlock) {
			for (auto &&j : i.mPoint3D) {
				const auto r4x = Vector (j) ;
				mWriter.deref << r4x[0] << slice (" ") ;
				mWriter.deref << r4x[1] << slice (" ") ;
				mWriter.deref << r4x[2] << GAP ;
				mWriter.deref << 255 << slice (" ") ;
				mWriter.deref << 255 << slice (" ") ;
				mWriter.deref << 255 << GAP ;
			}
		}
		mWriter.flush () ;
	}

	void save_camera_ply () {
		const auto r1x = self.mDataPath.child (slice ("ply")) ;
		FileProc::build_dire (r1x) ;
		for (auto &&i : self.mPose.range ()) {
			for (auto &&j : self.mPose[i].mUseView) {
				INDEX ix = self.mPose[i].mUseView.map (j) ;
				assume (ix != NONE) ;
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
				const auto r4x = Vector (0 ,0 ,0 ,MathProc::inverse (self.mView[j].mBaseline)) ;
				if ifdo (TRUE) {
					const auto r5x = r3x * Vector::axis_w () ;
					mWriter.deref << r5x[0] << slice (" ") ;
					mWriter.deref << r5x[1] << slice (" ") ;
					mWriter.deref << r5x[2] << GAP ;
					mWriter.deref << VAL32 (self.mPose[i].mColor.mR) << slice (" ") ;
					mWriter.deref << VAL32 (self.mPose[i].mColor.mG) << slice (" ") ;
					mWriter.deref << VAL32 (self.mPose[i].mColor.mB) << GAP ;
					const auto r6x = Vector (0 ,0 ,0 ,1) ;
					const auto r7x = Vector (FLT64 (self.mView[j].mShape.mCX) ,0 ,0 ,1) ;
					const auto r8x = Vector (FLT64 (self.mView[j].mShape.mCX) ,FLT64 (self.mView[j].mShape.mCY) ,0 ,1) ;
					const auto r9x = Vector (0 ,FLT64 (self.mView[j].mShape.mCY) ,0 ,1) ;
					const auto r10x = Buffer4<Vector> ({r6x ,r7x ,r8x ,r9x}) ;
					for (auto &&k : iter (0 ,r10x.size ())) {
						const auto r11x = r10x[k] ;
						const auto r12x = self.mView[j].mMatK[1] * r11x ;
						const auto r13x = (r12x + r4x).projection () ;
						const auto r14x = r3x * r13x ;
						mWriter.deref << r14x[0] << slice (" ") ;
						mWriter.deref << r14x[1] << slice (" ") ;
						mWriter.deref << r14x[2] << GAP ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mR) << slice (" ") ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mG) << slice (" ") ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mB) << GAP ;
					}
				}
				if ifdo (TRUE) {
					for (auto &&k : iter (0 ,self.mFrame[ix].mPoint2D.size ())) {
						const auto r15x = Vector (self.mFrame[ix].mPoint2D[k]) ;
						const auto r16x = (self.mView[j].mMatK[1] * r15x).normalize () ;
						const auto r17x = r16x * self.mFrame[ix].mPointDepth[k] + Vector::axis_w () ;
						const auto r18x = r3x * r17x ;
						mWriter.deref << r18x[0] << slice (" ") ;
						mWriter.deref << r18x[1] << slice (" ") ;
						mWriter.deref << r18x[2] << GAP ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mR) << slice (" ") ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mG) << slice (" ") ;
						mWriter.deref << VAL32 (self.mPose[i].mColor.mB) << GAP ;
					}
				}
				if ifdo (FALSE) {
					for (auto &&k : iter (0 ,self.mFrame[ix].mPoint2D.size ())) {
						const auto r19x = Vector (self.mFrame[ix].mPoint2D[k]) ;
						const auto r20x = self.mView[j].mMatK[1] * r19x ;
						const auto r21x = (r20x + r4x).projection () ;
						const auto r22x = r3x * r21x ;
						mWriter.deref << r22x[0] << slice (" ") ;
						mWriter.deref << r22x[1] << slice (" ") ;
						mWriter.deref << r22x[2] << GAP ;
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
				if ifdo (TRUE) {
					for (auto &&k : iter (0 ,self.mFrame[ix].mPoint2D.size ())) {
						const auto r23x = 9 + k ;
						mWriter.deref << 0 << slice (" ") ;
						mWriter.deref << r23x << GAP ;
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

		explicit ResidualErrorPinhole (CREF<Point2F> point_2d ,CREF<FLT64> weight)
			:mPoint2D (point_2d) ,mWeight (weight) {}

		template <typename T>
		BOOL operator() (const T *view_mat_k ,const T *view_dist ,const T *view_mat_v ,const T *pose_mat_v ,const T *point_3d ,T *residuals) const {
			const auto pose_mat_v_r = (&pose_mat_v[0]) ;
			const auto pose_mat_v_t = (&pose_mat_v[3]) ;
			const auto view_mat_v_r = (&view_mat_v[0]) ;
			const auto view_mat_v_t = (&view_mat_v[3]) ;
			auto rax = Buffer3<T> () ;
			rax[0] = point_3d[0] ;
			rax[1] = point_3d[1] ;
			rax[2] = point_3d[2] ;
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

		imports DEF<ceres::CostFunction *> create (CREF<Point2F> point_2d ,CREF<FLT64> weight) {
			const auto r1x = new ResidualErrorPinhole (point_2d ,weight) ;
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
