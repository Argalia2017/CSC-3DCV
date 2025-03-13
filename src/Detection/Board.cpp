#include "export.h"

#include <csc_end.h>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

namespace CSC3DCV {
inline Point2F cvt_csc_point2f (CREF<cv::Point2f> a) {
	return Point2F ({FLT32 (a.x) ,FLT32 (a.y)}) ;
}

inline cv::Size cvt_cv_size (CREF<ImageShape> a) {
	return cv::Size (int (a.mCX) ,int (a.mCY)) ;
}

struct BoardImplLayout {
	ImageShape mBoardShape ;
	Just<BoardType> mBoardType ;
	FLT64 mBoardBaselineX ;
	FLT64 mBoardBaselineY ;
} ;

class BoardImplHolder final implement Fat<BoardHolder ,BoardLayout> {
public:
	void initialize () override {
		fake.mThis = AutoRef<BoardImplLayout>::make () ;
	}

	void set_board_width (CREF<ImageShape> shape) override {
		fake.mThis->mBoardShape = shape ;
	}

	void set_board_type (CREF<Just<BoardType>> type) override {
		fake.mThis->mBoardType = type ;
	}

	void set_board_baseline (CREF<FLT64> baseline_x ,CREF<FLT64> baseline_y) override {
		fake.mThis->mBoardBaselineX = baseline_x ;
		fake.mThis->mBoardBaselineY = baseline_y ;
	}

	Array<Point3F> extract () const {
		Array<Point3F> ret = Array<Point3F> (fake.mThis->mBoardShape.size ()) ;
		const auto r1x = DiagMatrix (fake.mThis->mBoardBaselineX ,fake.mThis->mBoardBaselineY ,1 ,1) ;
		for (auto &&i : iter (0 ,fake.mThis->mBoardShape.mCX ,0 ,fake.mThis->mBoardShape.mCY)) {
			INDEX ix = i.mX + i.mY * fake.mThis->mBoardShape.mCX ;
			const auto r2x = r1x * Vector (i) ;
			ret[ix] = r2x.xyz () ;
		}
		return move (ret) ;
	}

	Optional<Array<Point2F>> detect (CREF<Image<Color3B>> image) override {
		auto mImage = ImageProc::peek_image (image ,TYPE<cv::Mat>::expr) ;
		assert (!mImage.empty ()) ;
		const auto r1x = FLT32 (1) ;
		const auto r2x = VAL32 (mImage.cols * MathProc::inverse (r1x)) ;
		const auto r3x = VAL32 (mImage.rows * MathProc::inverse (r1x)) ;
		cv::resize (mImage ,mImage ,cv::Size (r2x ,r3x)) ;
		cv::cvtColor (mImage ,mImage ,cv::COLOR_BGR2GRAY) ;
		const auto r4x = invoke ([&] ()->Optional<std::vector<cv::Point2f>> {
			std::vector<cv::Point2f> ret ;
			const auto r5x = detect_grid (mImage ,ret) ;
			if (!r5x)
				return Optional<std::vector<cv::Point2f>>::error (1) ;
			return move (ret) ;
		}) ;
		if (!r4x.exist ())
			return Optional<Array<Point2F>>::error (r4x.code ()) ;
		const auto r6x = r4x.fetch () ;
		Array<Point2F> ret = Array<Point2F> (fake.mThis->mBoardShape.size ()) ;
		if (r6x[0].x < r6x[1].x) {
			for (auto &&i : iter (0 ,r6x.size ())) {
				INDEX ix = i ;
				ret[ix] = cvt_csc_point2f (r6x[i]) ;
				ret[ix].mX *= r1x ;
				ret[ix].mY *= r1x ;
			}
		} else {
			for (auto &&i : iter (0 ,r6x.size ())) {
				INDEX ix = INDEX (r6x.size ()) - 1 - i ;
				ret[ix] = cvt_csc_point2f (r6x[i]) ;
				ret[ix].mX *= r1x ;
				ret[ix].mY *= r1x ;
			}
		}
		return move (ret) ;
	}

	BOOL detect_grid (CREF<cv::Mat> image ,VREF<std::vector<cv::Point2f>> result) const {
		const auto r1x = invoke ([&] () {
			cv::SimpleBlobDetector::Params ret ;
			ret.thresholdStep = 10 ;
			ret.minThreshold = 120 ;
			ret.maxThreshold = 220 ;
			ret.filterByColor = TRUE ;
			ret.blobColor = 0 ;
			ret.filterByArea = TRUE ;
			ret.minArea = 10 ;
			ret.maxArea = 50 ;
			return move (ret) ;
		}) ;
		const auto r2x = cv::SimpleBlobDetector::create (r1x) ;
		const auto r3x = csc_enum_t (cv::CALIB_CB_SYMMETRIC_GRID | cv::CALIB_CB_CLUSTERING) ;
		if (fake.mThis->mBoardType == BoardType::CIRCLE)
			return cv::findCirclesGrid (image ,cvt_cv_size (fake.mThis->mBoardShape) ,result ,r3x) ;
		if (fake.mThis->mBoardType == BoardType::CHESS)
			return cv::findChessboardCorners (image ,cvt_cv_size (fake.mThis->mBoardShape) ,result ,r3x) ;
		return FALSE ;
	}
} ;

exports VFat<BoardHolder> BoardHolder::hold (VREF<BoardLayout> that) {
	return VFat<BoardHolder> (BoardImplHolder () ,that) ;
}

exports CFat<BoardHolder> BoardHolder::hold (CREF<BoardLayout> that) {
	return CFat<BoardHolder> (BoardImplHolder () ,that) ;
}
} ;
