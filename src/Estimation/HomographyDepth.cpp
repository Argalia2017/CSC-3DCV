#include "export.h"

#include "../Estimation/export.h"

#include <csc_end.h>
#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif

#include <opencv2/opencv.hpp>
#include <csc_begin.h>

namespace CSC3DCV {
struct HomographyDepthLayout {
	Path mDataPath ;
	DuplexMatrix mLViewMatK ;
	DuplexMatrix mLViewMatV ;
	Image<Color3B> mLViewImage ;
	DuplexMatrix mRViewMatK ;
	DuplexMatrix mRViewMatV ;
	Image<Color3B> mRViewImage ;
	DuplexMatrix mPoseMatV ;
	Image<Color3B> mRemapImage ;
	Vector mPlaneNormal ;
	Vector mPlaneCenter ;
	BOOL mWKeyDown ;
	BOOL mSKeyDown ;
	BOOL mAKeyDown ;
	BOOL mDKeyDown ;
	BOOL mQKeyDown ;
	BOOL mEKeyDown ;
} ;

static Ref<HomographyDepthLayout> mCurrentLayout ;

class HomographyDepthImplHolder final implement Fat<HomographyDepthHolder ,HomographyDepthLayout> {
public:
	void initialize () override {
		self.mDataPath = Global<Path> (slice ("mDataPath")).fetch () ;
	}

	void execute () override {
		Singleton<Console>::instance ().info () ;
		Singleton<Console>::instance ().info (slice ("HomographyDepth::execute")) ;
		read_test_json () ;
		sync_first_plane () ;
		sync_keyboard_hook () ;
		while (TRUE) {
			work_manual_plane () ;
			sync_display () ;
		}
	}

	void read_test_json () {
		const auto r1x = self.mDataPath.child (slice ("test.json")) ;
		const auto r2x = JsonParser (FileProc::load_file (r1x)) ;
		if ifdo (TRUE) {
			const auto r3x = r2x.child (slice ("mLView")) ;
			const auto r4x = r3x.child (slice ("mMatK")).parse (FLT64 (0) ,16) ;
			self.mLViewMatK = SolverProc::decode_matrix (r4x) ;
			const auto r5x = r3x.child (slice ("mMatV")).parse (FLT64 (0) ,16) ;
			self.mLViewMatV = SolverProc::decode_matrix (r5x) ;
			const auto r6x = r3x.child (slice ("mImageFile")).parse (String<STR> ()) ;
			self.mLViewImage = ImageProc::load_image (r6x) ;
		}
		if ifdo (TRUE) {
			const auto r7x = r2x.child (slice ("mRView")) ;
			const auto r8x = r7x.child (slice ("mMatK")).parse (FLT64 (0) ,16) ;
			self.mRViewMatK = SolverProc::decode_matrix (r8x) ;
			const auto r9x = r7x.child (slice ("mMatV")).parse (FLT64 (0) ,16) ;
			self.mRViewMatV = SolverProc::decode_matrix (r9x) ;
			const auto r10x = r7x.child (slice ("mImageFile")).parse (String<STR> ()) ;
			self.mRViewImage = ImageProc::load_image (r10x) ;
		}
		if ifdo (TRUE) {
			const auto r11x = r2x.child (slice ("mFPose")) ;
			const auto r12x = r11x.child (slice ("mMatV")).parse (FLT64 (0) ,16) ;
			self.mPoseMatV = SolverProc::decode_matrix (r12x) ;
		}
	}

	void sync_first_plane () {
		self.mPlaneNormal = self.mPoseMatV[1] * Vector::axis_z () ;
		self.mPlaneCenter = self.mPoseMatV[1] * Vector::axis_w () ;
		assume (self.mLViewImage.shape () == self.mRViewImage.shape ()) ;
		self.mRemapImage = ImageProc::make_image (self.mLViewImage.shape ()) ;
	}

	void sync_keyboard_hook () {
		mCurrentLayout = Ref<HomographyDepthLayout>::reference (self) ;
		const auto r1x = SetWindowsHookEx (WH_KEYBOARD_LL ,keyboard_callback ,GetModuleHandle (NULL) ,0);
		assume (r1x != 0) ;
	}

	static LRESULT CALLBACK keyboard_callback (int nCode ,WPARAM wParam ,LPARAM lParam) {
		if (nCode >= 0) {
			KBDLLHOOKSTRUCT *kb = (KBDLLHOOKSTRUCT *) lParam;
			if (kb->vkCode == 'W') {
				mCurrentLayout->mWKeyDown = wParam == WM_KEYDOWN ;
			}
			if (kb->vkCode == 'S') {
				mCurrentLayout->mSKeyDown = wParam == WM_KEYDOWN ;
			}
			if (kb->vkCode == 'A') {
				mCurrentLayout->mAKeyDown = wParam == WM_KEYDOWN ;
			}
			if (kb->vkCode == 'D') {
				mCurrentLayout->mDKeyDown = wParam == WM_KEYDOWN ;
			}
			if (kb->vkCode == 'Q') {
				mCurrentLayout->mQKeyDown = wParam == WM_KEYDOWN ;
			}
			if (kb->vkCode == 'E') {
				mCurrentLayout->mEKeyDown = wParam == WM_KEYDOWN ;
			}
		}
		return CallNextHookEx (NULL ,nCode ,wParam ,lParam);
	}

	void work_manual_plane () {
		if ifdo (TRUE) {
			if (!self.mQKeyDown)
				discard ;
			self.mPlaneCenter[2] += 10 ;
		}
		if ifdo (TRUE) {
			if (!self.mEKeyDown)
				discard ;
			self.mPlaneCenter[2] -= 10 ;
		}
		const auto r1x = FLT64 (1) * MATH_R ;
		if ifdo (TRUE) {
			if (!self.mWKeyDown)
				discard ;
			const auto r2x = RotationMatrix (Vector::axis_x () ,-r1x) ;
			self.mPlaneNormal = r2x * self.mPlaneNormal ;
		}
		if ifdo (TRUE) {
			if (!self.mSKeyDown)
				discard ;
			const auto r3x = RotationMatrix (Vector::axis_x () ,+r1x) ;
			self.mPlaneNormal = r3x * self.mPlaneNormal ;
		}
		if ifdo (TRUE) {
			if (!self.mAKeyDown)
				discard ;
			const auto r4x = RotationMatrix (Vector::axis_y () ,+r1x) ;
			self.mPlaneNormal = r4x * self.mPlaneNormal ;
		}
		if ifdo (TRUE) {
			if (!self.mDKeyDown)
				discard ;
			const auto r5x = RotationMatrix (Vector::axis_y () ,-r1x) ;
			self.mPlaneNormal = r5x * self.mPlaneNormal ;
		}
	}

	void sync_display () {
		if ifdo (TRUE) {
			const auto r1x = self.mRemapImage.shape () ;
			const auto r2x = self.mPlaneNormal ;
			const auto r3x = self.mPlaneCenter ;
			const auto r4x = self.mRViewMatV[0] * Vector::axis_w () ;
			const auto r5x = self.mLViewMatV[0] * Vector::axis_w () ;
			const auto r6x = r2x * (r3x - r5x) ;
			const auto r7x = DiagMatrix (r6x ,r6x ,r6x) + SymmetryMatrix (r5x - r4x ,r2x) ;
			const auto r8x = self.mRViewMatV[1] * r7x * self.mLViewMatV[0] ;
			const auto r9x = r8x.homogenize () + Matrix::axis_w () ;
			const auto r10x = self.mRViewMatK[0] * r9x * self.mLViewMatK[1] ;
#pragma omp parallel for num_threads (12)
			for (INDEX i = 0 ; i < r1x.mCY ; i++) {
				for (auto &&j : iter (0 ,r1x.mCX)) {
					const auto r11x = Pixel ({j ,i}) ;
					const auto r12x = Vector (r11x) ;
					const auto r13x = (r10x * r12x).projection () ;
					const auto r14x = ImageProc::sampler (self.mRViewImage ,r13x[0] ,r13x[1]) ;
					const auto r15x = alpha_blend (self.mLViewImage[r11x] ,0.5 ,r14x ,0.5) ;
					self.mRemapImage[r11x] = r15x ;
				}
			}
		}
		auto rax = ImageProc::peek_image (self.mRemapImage ,TYPE<cv::Mat>::expr) ;
		cv::imshow ("rax" ,rax) ;
		cv::waitKey (10) ;
	}

	Color3B alpha_blend (CREF<Color3B> c1 ,CREF<FLT64> f1 ,CREF<Color3B> c2 ,CREF<FLT64> f2) const {
		Color3B ret ;
		ret.mB = BYTE (VAL32 (c1.mB) * f1 + VAL32 (c2.mB) * f2) ;
		ret.mG = BYTE (VAL32 (c1.mG) * f1 + VAL32 (c2.mG) * f2) ;
		ret.mR = BYTE (VAL32 (c1.mR) * f1 + VAL32 (c2.mR) * f2) ;
		return move (ret) ;
	}
} ;

exports OfThis<AutoRef<HomographyDepthLayout>> HomographyDepthHolder::create () {
	OfThis<AutoRef<HomographyDepthLayout>> ret ;
	ret.mThis = AutoRef<HomographyDepthLayout>::make () ;
	return move (ret) ;
}

exports VFat<HomographyDepthHolder> HomographyDepthHolder::hold (VREF<HomographyDepthLayout> that) {
	return VFat<HomographyDepthHolder> (HomographyDepthImplHolder () ,that) ;
}

exports CFat<HomographyDepthHolder> HomographyDepthHolder::hold (CREF<HomographyDepthLayout> that) {
	return CFat<HomographyDepthHolder> (HomographyDepthImplHolder () ,that) ;
}
} ;
