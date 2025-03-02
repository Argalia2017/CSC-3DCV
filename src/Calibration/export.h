#include "../util.h"

namespace CSC3DCV {
struct CameraViewLayout {
	String<STR> mName ;
	String<STR> mGroup ;
	Set<INDEX> mUsePose ;
	ImageWidth mWidth ;
	DuplexMatrix mMatK ;
	BOOL mConstMatK ;
	Array<FLT64> mDist ;
	BOOL mConstDist ;
	DuplexMatrix mMatV ;
	BOOL mConstMatV ;
	FLT64 mBaseLine ;
	FLT64 mRelative ;
	INDEX mParam1 ;
	INDEX mParam2 ;
	INDEX mParam3 ;
} ;

struct CameraViewHolder implement Interface {
	imports VFat<CameraViewHolder> hold (VREF<CameraViewLayout> that) ;
	imports CFat<CameraViewHolder> hold (CREF<CameraViewLayout> that) ;

	virtual void initialize () = 0 ;
} ;

class CameraView implement CameraViewLayout {
public:
	implicit CameraView () = default ;

	explicit CameraView (CREF<typeof (NULL)>) {
		CameraViewHolder::hold (thiz)->initialize () ;
	}
} ;

struct CameraPoseLayout {
	String<STR> mName ;
	String<STR> mGroup ;
	Color3B mColor ;
	Set<INDEX> mUseView ;
	INDEX mLFrame ;
	INDEX mRFrame ;
	DuplexMatrix mMatV ;
	BOOL mUsingMatV ;
	FLT64 mWeight ;
	Matrix mMatH ;
	BOOL mUsingMatH ;
	NormalError mError ;
	INDEX mParam4 ;
} ;

struct CameraPoseHolder implement Interface {
	imports VFat<CameraPoseHolder> hold (VREF<CameraPoseLayout> that) ;
	imports CFat<CameraPoseHolder> hold (CREF<CameraPoseLayout> that) ;

	virtual void initialize () = 0 ;
} ;

class CameraPose implement CameraPoseLayout {
public:
	implicit CameraPose () = default ;

	explicit CameraPose (CREF<typeof (NULL)>) {
		CameraPoseHolder::hold (thiz)->initialize () ;
	}
} ;

struct CameraFrameLayout {
	INDEX mFPose ;
	INDEX mFView ;
	INDEX mFTime ;
	String<STR> mImageFile ;
	BOOL mUndistortion ;
	Array<Point2F> mOriginPoint ;
	Array<Point2F> mUndistPoint ;
	Image<FLT32> mDepth ;
} ;

struct CameraFrameHolder implement Interface {
	imports VFat<CameraFrameHolder> hold (VREF<CameraFrameLayout> that) ;
	imports CFat<CameraFrameHolder> hold (CREF<CameraFrameLayout> that) ;

	virtual void initialize () = 0 ;
} ;

class CameraFrame implement CameraFrameLayout {
public:
	implicit CameraFrame () = default ;

	explicit CameraFrame (CREF<typeof (NULL)>) {
		CameraFrameHolder::hold (thiz)->initialize () ;
	}
} ;

struct CameraBlockLayout {
	INDEX mFPose ;
	Set<INDEX> mUseFrame ;
	Array<Point3F> mPoint ;
	Array<FLT32> mRadius ;
	INDEX mParam5 ;
} ;

struct CameraBlockHolder implement Interface {
	imports VFat<CameraBlockHolder> hold (VREF<CameraBlockLayout> that) ;
	imports CFat<CameraBlockHolder> hold (CREF<CameraBlockLayout> that) ;

	virtual void initialize () = 0 ;
} ;

class CameraBlock implement CameraBlockLayout {
public:
	implicit CameraBlock () = default ;

	explicit CameraBlock (CREF<typeof (NULL)>) {
		CameraBlockHolder::hold (thiz)->initialize () ;
	}
} ;

struct CameraOptimizerImplLayout ;

struct CameraOptimizerLayout implement OfThis<AutoRef<CameraOptimizerImplLayout>> {} ;

struct CameraOptimizerHolder implement Interface {
	imports VFat<CameraOptimizerHolder> hold (VREF<CameraOptimizerLayout> that) ;
	imports CFat<CameraOptimizerHolder> hold (CREF<CameraOptimizerLayout> that) ;

	virtual void initialize () = 0 ;
} ;

class CameraOptimizer implement CameraOptimizerLayout {
public:
	implicit CameraOptimizer () = default ;

	explicit CameraOptimizer (CREF<typeof (NULL)>) {
		CameraOptimizerHolder::hold (thiz)->initialize () ;
	}
} ;

struct CalibrationImplLayout ;

struct CalibrationLayout implement OfThis<AutoRef<CalibrationImplLayout>> {} ;

struct CalibrationHolder implement Interface {
	imports VFat<CalibrationHolder> hold (VREF<CalibrationLayout> that) ;
	imports CFat<CalibrationHolder> hold (CREF<CalibrationLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

class Calibration implement CalibrationLayout {
public:
	implicit Calibration () = default ;

	explicit Calibration (CREF<typeof (NULL)>) {
		CalibrationHolder::hold (thiz)->initialize () ;
	}

	void execute () {
		return CalibrationHolder::hold (thiz)->execute () ;
	}
} ;
} ;