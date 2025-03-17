#include "../util.h"

namespace CSC3DCV {
struct CameraView {
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
	FLT64 mBaseline ;
	FLT64 mRelative ;
	INDEX mParam1 ;
	INDEX mParam2 ;
	INDEX mParam3 ;
} ;

struct CameraPose {
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

struct CameraFrame {
	INDEX mPose1 ;
	INDEX mView1 ;
	INDEX mBlock1 ;
	String<STR> mImageFile ;
	String<STR> mDepthFile ;
	Array<Point2F> mPoint2D ;
	Array<FLT32> mPointDepth ;
	Array<Vector> mPointRay ;
} ;

struct CameraBlock {
	INDEX mTime1 ;
	Set<INDEX> mUseFrame ;
	Array<Point3F> mPoint3D ;
	DuplexMatrix mMatP ;
	BOOL mUsingMatP ;
	Line3F mPlaneBound ;
	INDEX mParam5 ;
} ;

struct PoseGraphLayout {
	ArrayList<CameraView> mView ;
	Set<String<STR>> mViewNameSet ;
	ArrayList<CameraPose> mPose ;
	Set<String<STR>> mPoseNameSet ;
	ArrayList<CameraFrame> mFrame ;
	Set<Pixel> mFramePixelSet ;
	ArrayList<CameraBlock> mBlock ;
} ;

struct PoseGraphHolder implement Interface {
	imports VFat<PoseGraphHolder> hold (VREF<PoseGraphLayout> that) ;
	imports CFat<PoseGraphHolder> hold (CREF<PoseGraphLayout> that) ;
} ;

class PoseGraph implement PoseGraphLayout {
public:
	implicit PoseGraph () = default ;
} ;

struct CalibrationLayout ;

struct CalibrationHolder implement Interface {
	imports OfThis<AutoRef<CalibrationLayout>> create () ;
	imports VFat<CalibrationHolder> hold (VREF<CalibrationLayout> that) ;
	imports CFat<CalibrationHolder> hold (CREF<CalibrationLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

class Calibration implement OfThis<AutoRef<CalibrationLayout>> {
public:
	implicit Calibration () = default ;

	explicit Calibration (CREF<typeof (NULL)>) {
		mThis = CalibrationHolder::create () ;
		CalibrationHolder::hold (thiz)->initialize () ;
	}

	void execute () {
		return CalibrationHolder::hold (thiz)->execute () ;
	}
} ;
} ;