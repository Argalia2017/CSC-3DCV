#include "export.h"

#include "../Common/export.h"
#include "../Estimation/export.h"

#include <csc_end.h>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

namespace CSC3DCV {
struct CameraBiView {
	INDEX mView1 ;
	DuplexMatrix mMatK ;
	DuplexMatrix mMatV ;
	Image<Point2F> mRemap ;
} ;

struct BinocularLayout {
	Path mDataPath ;
	ArrayList<CameraView> mView ;
	Array<CameraBiView> mBiView ;
	Set<String<STR>> mViewNameSet ;
	ArrayList<CameraPose> mPose ;
	Set<String<STR>> mPoseNameSet ;
	String<STR> mImageExtension ;
} ;

class BinocularImplHolder final implement Fat<BinocularHolder ,BinocularLayout> {
public:
	void initialize () override {
		self.mDataPath = Global<Path> (slice ("mDataPath")).fetch () ;
		self.mImageExtension = slice (".bmp") ;
	}

	void execute () override {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("Binocular::execute")) ;
		load_data_json () ;
		work_binocular_view () ;
		work_binocular_remap () ;
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
		self.mBiView = Array<CameraBiView> (self.mView.length ()) ;
		for (auto &&i : self.mView.range ()) {
			self.mBiView[i].mView1 = i ;
			self.mBiView[i].mMatK = self.mView[i].mMatK ;
			self.mBiView[i].mMatV = self.mView[i].mMatV ;
			self.mBiView[i].mRemap = Image<Point2F> (self.mView[i].mShape) ;
		}
		const auto r1x = Array<INDEX>::make (self.mView.range ()) ;
		assume (r1x.length () == 2) ;
		const auto r2x = self.mBiView[r1x[0]].mMatV[0] * Vector::axis_w () ;
		const auto r3x = self.mBiView[r1x[1]].mMatV[0] * Vector::axis_w () ;
		const auto r4x = (r3x - r2x).normalize () ;
		for (auto &&i : iter (0 ,2)) {
			INDEX ix = r1x[i] ;
			const auto r5x = self.mBiView[ix].mMatV[0] * Vector::axis_w () ;
			const auto r6x = self.mBiView[ix].mMatV[0] * Vector::axis_z () ;
			const auto r7x = ViewMatrixZXY (r6x ,r4x) ;
			const auto r8x = TranslationMatrix (r5x) ;
			self.mBiView[ix].mMatV = r8x * r7x ;
			const auto r9x = self.mBiView[ix].mRemap.cy () ;
#pragma omp parallel for
			for (INDEX j = 0 ; j < r9x ; j++) {
				for (auto &&k : iter (0 ,self.mBiView[ix].mRemap.cx ())) {
					const auto r10x = Pixel ({k ,j}) ;
					const auto r11x = Vector (r10x) ;
					const auto r12x = self.mBiView[ix].mMatK[1] * r11x ;
					const auto r13x = self.mBiView[ix].mMatV[0] * r12x ;
					const auto r14x = self.mView[ix].mMatV[1] * r13x ;
					const auto r15x = (self.mView[ix].mMatK[0] * r14x).projection () ;
					const auto r16x = SolverProc::undistortion (self.mView[ix].mMatK ,self.mView[ix].mDist ,r15x) ;
					self.mBiView[ix].mRemap[r10x] = r16x.xyz () ;
				}
			}
		}
	}

	void work_binocular_remap () {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().trace (slice ("work_binocular_remap")) ;
		const auto r1x = self.mDataPath.child (slice ("image_undist")) ;
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
				for (auto &&k : rax.range ()) {
					const auto r8x = self.mBiView[j].mRemap[k] ;
					rax[k] = ImageProc::sampler (r7x ,r8x.mX ,r8x.mY) ;
				}
				ImageProc::save_image (r6x ,rax) ;
			}
		}
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