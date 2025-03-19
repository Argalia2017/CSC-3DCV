#include "export.h"

#include "../Detection/export.h"

#include <csc_end.h>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

namespace CSC3DCV {
struct DisplayLayout {
	Path mVideoPath ;
	Ref<cv::VideoCapture> mVideoCapture ;
	Image<Color3B> mCurrFrame ;
	LSD mLSDEngine ;
	Array<Line2F> mCurrLine ;
} ;

class DisplayImplHolder final implement Fat<DisplayHolder ,DisplayLayout> {
public:
	void initialize () override {
		self.mVideoPath = Path (slice ("D:/Documents/C++/csc_3dcv/data/5/input.avi")) ;
		self.mLSDEngine = LSD (NULL) ;
	}

	void execute () override {
		const auto r1x = StringProc::stra_from_strs (self.mVideoPath.fetch ()) ;
		self.mVideoCapture = Ref<cv::VideoCapture>::make () ;
		self.mVideoCapture.deref = cv::VideoCapture ((&r1x[0]) ,cv::CAP_ANY) ;
		assume (self.mVideoCapture->isOpened ()) ;
		auto rax = Box<Pin<cv::Mat>>::make () ;
		while (TRUE) {
			const auto r2x = self.mVideoCapture->read (keep[TYPE<cv::Mat>::expr] (rax->deref)) ;
			if (!r2x)
				break ;
			self.mCurrFrame = ImageProc::make_image (move (rax)) ;
			self.mLSDEngine.process (self.mCurrFrame) ;
			self.mCurrLine = self.mLSDEngine.detect () ;
			if ifdo (TRUE) {
				auto rbx = ImageProc::peek_image (self.mCurrFrame ,TYPE<cv::Mat>::expr) ;
				const auto r3x = cv::Scalar (0 ,0 ,255 ,255) ;
				for (auto &&i : self.mCurrLine) {
					const auto r4x = cv::Point2i (VAL32 (i.mMin.mX) ,VAL32 (i.mMin.mY)) ;
					const auto r5x = cv::Point2i (VAL32 (i.mMax.mX) ,VAL32 (i.mMax.mY)) ;
					cv::line (rbx ,r4x ,r5x ,r3x ,VAL32 (2) ,cv::LINE_8) ;
				}
				cv::imshow ("rbx" ,rbx) ;
				cv::waitKey (10) ;
			}
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
