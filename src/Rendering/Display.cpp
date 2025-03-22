#include "export.h"

#include "../Detection/export.h"

#include <csc_end.h>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

namespace CSC3DCV {
struct DisplayLayout {
	Path mVideoPath ;
	Path mFramePath ;
	Ref<cv::VideoCapture> mVideoCapture ;
	Image<Color3B> mCurrImage ;
	Image<Color3B> mDisplayImage ;
	LSD mLSDEngine ;
	Array<Line2F> mCurrLine ;
} ;

class DisplayImplHolder final implement Fat<DisplayHolder ,DisplayLayout> {
public:
	void initialize () override {
		self.mVideoPath = Path (slice ("D:/Documents/C++/csc_3dcv/data/5")) ;
		self.mLSDEngine = LSD (NULL) ;
	}

	void execute () override {
		self.mFramePath = self.mVideoPath.child (slice ("frame")) ;
		FileProc::build_dire (self.mFramePath) ;
		const auto r1x = self.mVideoPath.child (slice ("0001-0254.mp4")) ;
		const auto r2x = StringProc::stra_from_strs (r1x) ;
		self.mVideoCapture = Ref<cv::VideoCapture>::make () ;
		self.mVideoCapture.deref = cv::VideoCapture ((&r2x[0]) ,cv::CAP_ANY) ;
		assume (self.mVideoCapture->isOpened ()) ;
		auto rax = Box<Pin<cv::Mat>>::make () ;
		auto mFrame = ZERO ;
		while (TRUE) {
			const auto r3x = self.mVideoCapture->read (keep[TYPE<cv::Mat>::expr] (rax->deref)) ;
			if (!r3x)
				break ;
			self.mCurrImage = ImageProc::make_image (move (rax)) ;
			self.mLSDEngine.process (self.mCurrImage) ;
			self.mCurrLine = self.mLSDEngine.detect () ;
			if ifdo (TRUE) {
				if (self.mDisplayImage.shape () == self.mCurrImage.shape ())
					discard ;
				self.mDisplayImage = ImageProc::make_image (self.mCurrImage.shape ()) ;
			}
			if ifdo (TRUE) {
				const auto r9x = self.mFramePath.child (Format (slice ("$1.txt")) (AlignedText (mFrame ,5))) ;
				auto mWriter = StreamFileTextWriter (r9x) ;
				mWriter.deref << self.mCurrLine.length () << GAP ;
				for (auto &&i : self.mCurrLine.range ()) {
					mWriter.deref << i << slice (" ") ;
					mWriter.deref << self.mCurrLine[i].mMin.mX << slice (" ") ;
					mWriter.deref << self.mCurrLine[i].mMin.mY << slice (" ") ;
					mWriter.deref << self.mCurrLine[i].mMax.mX << slice (" ") ;
					mWriter.deref << self.mCurrLine[i].mMax.mY << slice (" ") ;
					mWriter.deref << GAP ;
				}
				mWriter.flush () ;
			}
			if ifdo (TRUE) {
				self.mDisplayImage.fill (COLOR_BLACK) ;
				auto rbx = ImageProc::peek_image (self.mDisplayImage ,TYPE<cv::Mat>::expr) ;
				const auto r4x = cv::Scalar (0 ,0 ,255 ,255) ;
				for (auto &&i : self.mCurrLine) {
					const auto r5x = cv::Point2i (VAL32 (i.mMin.mX) ,VAL32 (i.mMin.mY)) ;
					const auto r6x = cv::Point2i (VAL32 (i.mMax.mX) ,VAL32 (i.mMax.mY)) ;
					cv::line (rbx ,r5x ,r6x ,r4x ,VAL32 (2) ,cv::LINE_8) ;
				}
				const auto r7x = self.mFramePath.child (Format (slice ("$1.bmp")) (AlignedText (mFrame ,5))) ;
				const auto r8x = StringProc::stra_from_strs (r7x) ;
				cv::imwrite ((&r8x[0]) ,rbx) ;
				cv::imshow ("rbx" ,rbx) ;
				cv::waitKey (1) ;
			}
			mFrame++ ;
		}
	}
} ;

exports OfThis<AutoRef<DisplayLayout>> DisplayHolder::create () {
	OfThis<AutoRef<DisplayLayout>> ret ;
	ret.mThis = AutoRef<DisplayLayout>::make () ;
	return move (ret) ;
}

exports VFat<DisplayHolder> DisplayHolder::hold (VREF<DisplayLayout> that) {
	return VFat<DisplayHolder> (DisplayImplHolder () ,that) ;
}

exports CFat<DisplayHolder> DisplayHolder::hold (CREF<DisplayLayout> that) {
	return CFat<DisplayHolder> (DisplayImplHolder () ,that) ;
}
} ;
