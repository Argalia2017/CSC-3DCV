#include "../util.h"

namespace CSC3DCV {
struct CameraViewLayout {
	String<STR> mName ;
	String<STR> mGroup ;
	Set<INDEX> mUsePose ;
	ImageShape mShape ;
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
} ;

class CameraView implement CameraViewLayout {
public:
	implicit CameraView () = default ;
} ;

struct CameraPoseLayout {
	String<STR> mName ;
	String<STR> mGroup ;
	Color3B mColor ;
	Set<INDEX> mUseView ;
	INDEX mFrame1 ;
	INDEX mFrame2 ;
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
} ;

class CameraPose implement CameraPoseLayout {
public:
	implicit CameraPose () = default ;
} ;

struct CameraFrameLayout {
	INDEX mPose1 ;
	INDEX mView1 ;
	INDEX mTime1 ;
	String<STR> mImageFile ;
	Image<Color3B> mImage ;
	Image<FLT32> mDepth ;
	Array<FLT32> mKeypoint ;
	BOOL mUndistortion ;
	Array<Point2F> mOriginPoint ;
	Array<Point2F> mUndistPoint ;
} ;

struct CameraFrameHolder implement Interface {
	imports VFat<CameraFrameHolder> hold (VREF<CameraFrameLayout> that) ;
	imports CFat<CameraFrameHolder> hold (CREF<CameraFrameLayout> that) ;
} ;

class CameraFrame implement CameraFrameLayout {
public:
	implicit CameraFrame () = default ;
} ;

struct CameraBlockLayout {
	INDEX mTime1 ;
	Set<INDEX> mUseFrame ;
	Array<Point3F> mPoint ;
	Array<FLT32> mRadius ;
	INDEX mParam5 ;
} ;

struct CameraBlockHolder implement Interface {
	imports VFat<CameraBlockHolder> hold (VREF<CameraBlockLayout> that) ;
	imports CFat<CameraBlockHolder> hold (CREF<CameraBlockLayout> that) ;
} ;

class CameraBlock implement CameraBlockLayout {
public:
	implicit CameraBlock () = default ;
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