#include "export.h"

#include "../Common/export.h"
#include "../Detection/export.h"
#include "../Estimation/export.h"

#include <csc_end.h>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

namespace CSC3DCV {
struct CameraBinoView {
	INDEX mView1 ;
	DuplexMatrix mMatK ;
	DuplexMatrix mMatV ;
	Image<Point2F> mRemap ;
	NormalError mError ;
} ;

struct BinocularLayout {
	Path mDataPath ;
	ArrayList<CameraView> mView ;
	Array<CameraBinoView> mBinoView ;
	Set<String<STR>> mViewNameSet ;
	ArrayList<CameraPose> mPose ;
	Set<String<STR>> mPoseNameSet ;
	Board mBoard ;
	ImageShape mBoardShape ;
	String<STR> mBoardType ;
	Vector mBoardBaseline ;
	String<STR> mImageGroup ;
	String<STR> mImageExtension ;
	Color3B mDefaultColor ;
} ;

class BinocularImplHolder final implement Fat<BinocularHolder ,BinocularLayout> {
public:
	void initialize () override {
		self.mDataPath = Global<Path> (slice ("mDataPath")).fetch () ;
		self.mBoard = Board (NULL) ;
		self.mBoardShape.mCX = 15 ;
		self.mBoardShape.mCY = 5 ;
		self.mBoardType = slice ("CIRCLE") ;
		self.mBoardBaseline = Vector (54.75 ,54.75 ,1 ,0) ;
		self.mImageGroup = slice ("image_undist") ;
		self.mImageExtension = slice (".bmp") ;
		self.mDefaultColor = Color3B ({BYTE (0X00) ,BYTE (0X00) ,BYTE (0X80)}) ;
	}

	void execute () override {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("Binocular::execute")) ;
		load_data_json () ;
		work_binocular_view () ;
		work_binocular_remap () ;
		check_binocular_error () ;
	}

	void load_data_json () {
		self.mView = ArrayList<CameraView> () ;
		self.mPose = ArrayList<CameraPose> () ;
		const auto r1x = self.mDataPath.child (slice ("data.json")) ;
		const auto r2x = JsonParser (FileProc::load_file (r1x)) ;
		load_data_json_view (r2x) ;
		load_data_json_pose (r2x) ;
		self.mView.remap () ;
		self.mPose.remap () ;
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
			self.mPose[ix].mUsingMatH = TRUE ;
			const auto r8x = r1x[i].child (slice ("mError")) ;
			self.mPose[ix].mError.mMaxError = r8x.child (0).parse (FLT64 (0)) ;
			self.mPose[ix].mError.mAvgError = r8x.child (1).parse (FLT64 (0)) ;
			self.mPose[ix].mError.mStdError = r8x.child (2).parse (FLT64 (0)) ;
		}
	}

	void work_binocular_view () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_binocular_view")) ;
		const auto r1x = Array<INDEX>::make (self.mView.range ()) ;
		assume (r1x.length () == 2) ;
		assume (self.mView[r1x[0]].mShape == self.mView[r1x[1]].mShape) ;
		const auto r2x = self.mView[r1x[0]].mMatV[0] * Vector::axis_w () ;
		const auto r3x = self.mView[r1x[1]].mMatV[0] * Vector::axis_w () ;
		const auto r4x = (r3x - r2x).normalize () ;
		const auto r5x = (self.mView[r1x[0]].mMatK[0][0][0] + self.mView[r1x[1]].mMatK[0][0][0]) / 2 ;
		const auto r6x = (self.mView[r1x[0]].mMatK[0][1][1] + self.mView[r1x[1]].mMatK[0][1][1]) / 2 ;
		const auto r7x = Vector (FLT64 (self.mView[r1x[0]].mShape.mCX) ,FLT64 (self.mView[r1x[0]].mShape.mCY) ,0 ,2).projection () ;
		self.mBinoView = Array<CameraBinoView> (self.mView.length ()) ;
		for (auto &&i : self.mView.range ()) {
			self.mBinoView[i].mView1 = i ;
			self.mBinoView[i].mMatK = PerspectiveMatrix (r5x ,r6x ,r7x[0] ,r7x[1]) ;
			self.mBinoView[i].mMatV = self.mView[i].mMatV ;
			self.mBinoView[i].mRemap = Image<Point2F> (self.mView[i].mShape) ;
		}
		for (auto &&i : iter (0 ,2)) {
			INDEX ix = r1x[i] ;
			const auto r8x = self.mView[ix].mMatV[0] * Vector::axis_w () ;
			const auto r9x = self.mView[ix].mMatV[0] * Vector::axis_z () ;
			const auto r10x = ViewMatrixZXY (r9x ,r4x) ;
			const auto r11x = TranslationMatrix (r8x) ;
			self.mBinoView[ix].mMatV = r11x * r10x ;
			const auto r12x = self.mBinoView[ix].mRemap.cy () ;
#pragma omp parallel for
			for (INDEX j = 0 ; j < r12x ; j++) {
				for (auto &&k : iter (0 ,self.mBinoView[ix].mRemap.cx ())) {
					const auto r13x = Pixel ({k ,j}) ;
					const auto r14x = Vector (r13x) ;
					const auto r15x = self.mBinoView[ix].mMatK[1] * r14x ;
					const auto r16x = self.mBinoView[ix].mMatV[0] * r15x ;
					const auto r17x = self.mView[ix].mMatV[1] * r16x ;
					const auto r18x = (self.mView[ix].mMatK[0] * r17x).projection () ;
					const auto r19x = SolverProc::redistortion (self.mView[ix].mMatK ,self.mView[ix].mDist ,r18x) ;
					self.mBinoView[ix].mRemap[r13x] = r19x.xyz () ;
				}
			}
		}
	}

	void work_binocular_remap () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_binocular_remap")) ;
		const auto r1x = self.mDataPath.child (self.mImageGroup) ;
		FileProc::build_dire (r1x) ;
		const auto r2x = self.mPose.length () ;
#pragma omp parallel for
		for (INDEX i = 0 ; i < r2x ; i++) {
			const auto r3x = Format (slice ("$1_$2$3")) ;
			const auto r4x = self.mDataPath.child (self.mPose[i].mGroup) ;
			for (auto &&j : self.mPose[i].mUseView) {
				const auto r5x = r4x.child (r3x (self.mPose[i].mName ,self.mView[j].mName ,self.mImageExtension)) ;
				const auto r6x = r1x.child (r3x (self.mPose[i].mName ,self.mView[j].mName ,self.mImageExtension)) ;
				const auto r7x = Image<Color3B> (ImageProc::load_image (r5x)) ;
				auto rax = Image<Color3B> (ImageProc::make_image (r7x.shape ())) ;
				rax.fill (self.mDefaultColor) ;
				const auto r8x = Vector (FLT64 (rax.cx ()) ,FLT64 (rax.cy ()) ,0 ,1) ;
				for (auto &&k : rax.range ()) {
					const auto r9x = self.mBinoView[j].mRemap[k] ;
					if (!(r9x.mX >= 0 && r9x.mX <= r8x[0]))
						continue ;
					if (!(r9x.mY >= 0 && r9x.mY <= r8x[1]))
						continue ;
					rax[k] = ImageProc::sampler (r7x ,r9x.mX ,r9x.mY) ;
				}
				ImageProc::save_image (r6x ,rax) ;
			}
		}
	}

	void check_binocular_error () {
		Singleton<Console>::instance ().trace (slice ("check_binocular_error")) ;
		self.mBoard.set_board_shape (self.mBoardShape) ;
		self.mBoard.set_board_type (BoardType::CIRCLE) ;
		self.mBoard.set_board_baseline (self.mBoardBaseline) ;
		const auto r1x = Array<INDEX>::make (self.mView.range ()) ;
		assume (r1x.length () == 2) ;
		INDEX ix = r1x[0] ;
		INDEX iy = r1x[1] ;
		self.mBinoView[ix].mError = NormalError () ;
		self.mBinoView[iy].mError = NormalError () ;
		const auto r2x = self.mDataPath.child (self.mImageGroup) ;
		const auto r3x = r2x.child (slice ("check")) ;
		FileProc::build_dire (r3x) ;
		const auto r4x = Format (slice ("$1_$2$3")) ;
		for (auto &&i : self.mPose.range ()) {
			const auto r5x = r2x.child (r4x (self.mPose[i].mName ,self.mView[ix].mName ,self.mImageExtension)) ;
			const auto r6x = r2x.child (r4x (self.mPose[i].mName ,self.mView[iy].mName ,self.mImageExtension)) ;
			auto rax = Image<Color3B> (ImageProc::load_image (r5x)) ;
			auto rbx = Image<Color3B> (ImageProc::load_image (r6x)) ;
			assume (rax.shape () == self.mView[ix].mShape) ;
			assume (rbx.shape () == self.mView[iy].mShape) ;
			const auto r7x = self.mBoard.detect (rax) ;
			const auto r8x = self.mBoard.detect (rbx) ;
			if (!r7x.exist ())
				continue ;
			if (!r8x.exist ())
				continue ;
			const auto r9x = r7x.fetch () ;
			const auto r10x = r8x.fetch () ;
			INDEX jx = NONE ;
			INDEX jy = NONE ;
			auto rcx = FLT64 (-infinity) ;
			auto rdx = FLT64 (+infinity) ;
			for (auto &&j : r9x.range ()) {
				const auto r11x = MathProc::abs (r9x[j].mY - r10x[j].mY) ;
				self.mBinoView[ix].mError += r11x ;
				self.mBinoView[iy].mError += r11x ;
				if ifdo (TRUE) {
					if (jx != NONE)
						if (rcx >= r11x)
							discard ;
					jx = j ;
					rcx = r11x ;
				}
				if ifdo (TRUE) {
					if (jy != NONE)
						if (rdx <= r11x)
							discard ;
					jy = j ;
					rdx = r11x ;
				}
			}
			if ifdo (TRUE) {
				const auto r12x = cv::Scalar (0 ,255 ,0 ,255) ;
				const auto r13x = cv::Scalar (0 ,0 ,255 ,255) ;
				if ifdo (TRUE) {
					auto rex = ImageProc::peek_image (rax ,TYPE<cv::Mat>::expr) ;
					for (auto &&k : Array<INDEX> ({jx ,jy})) {
						const auto r14x = cv::Point2f (0 ,r10x[k].mY) ;
						const auto r15x = cv::Point2f (FLT32 (rex.cols) - 1 ,r10x[k].mY) ;
						const auto r16x = cv::Point2f (r9x[k].mX ,r9x[k].mY) ;
						cv::line (rex ,r14x ,r15x ,r12x ,VAL32 (1) ,cv::LINE_AA) ;
						cv::circle (rex ,r16x ,VAL32 (2) ,r13x ,cv::FILLED) ;
					}
					const auto r17x = r3x.child (r4x (self.mPose[i].mName ,self.mView[ix].mName ,slice (".bmp"))) ;
					ImageProc::save_image (r17x ,rax) ;
				}
				if ifdo (TRUE) {
					auto rex = ImageProc::peek_image (rbx ,TYPE<cv::Mat>::expr) ;
					for (auto &&k : Array<INDEX> ({jx ,jy})) {
						const auto r18x = cv::Point2f (0 ,r9x[k].mY) ;
						const auto r19x = cv::Point2f (FLT32 (rex.cols) - 1 ,r9x[k].mY) ;
						const auto r20x = cv::Point2f (r10x[k].mX ,r10x[k].mY) ;
						cv::line (rex ,r18x ,r19x ,r12x ,VAL32 (1) ,cv::LINE_AA) ;
						cv::circle (rex ,r20x ,VAL32 (2) ,r13x ,cv::FILLED) ;
					}
					const auto r21x = r3x.child (r4x (self.mPose[i].mName ,self.mView[iy].mName ,slice (".bmp"))) ;
					ImageProc::save_image (r21x ,rbx) ;
				}
			}
		}
		Singleton<Console>::instance ().debug (slice ("mMaxError = ") ,self.mBinoView[ix].mError.mMaxError) ;
		Singleton<Console>::instance ().debug (slice ("mAvgError = ") ,self.mBinoView[ix].mError.mAvgError) ;
		Singleton<Console>::instance ().debug (slice ("mStdError = ") ,self.mBinoView[ix].mError.mStdError) ;
		Singleton<Console>::instance ().trace () ;
	}
} ;

exports OfThis<AutoRef<BinocularLayout>> BinocularHolder::create () {
	OfThis<AutoRef<BinocularLayout>> ret ;
	ret.mThis = AutoRef<BinocularLayout>::make () ;
	return move (ret) ;
}

exports VFat<BinocularHolder> BinocularHolder::hold (VREF<BinocularLayout> that) {
	return VFat<BinocularHolder> (BinocularImplHolder () ,that) ;
}

exports CFat<BinocularHolder> BinocularHolder::hold (CREF<BinocularLayout> that) {
	return CFat<BinocularHolder> (BinocularImplHolder () ,that) ;
}
} ;