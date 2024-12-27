#include "export.h"

namespace CSC3DCV {
struct CalibrationImplLayout {
	List<CameraView> mView ;
	List<CameraPose> mPose ;
	List<CameraFrame> mFrame ;
	List<CameraBlock> mBlock ;
} ;

class CalibrationImplHolder final implement Fat<CalibrationHolder ,CalibrationLayout> {
public:
	void initialize () override {
		
	}

	void execute () override {

	}
} ;

exports VFat<CalibrationHolder> CalibrationHolder::hold (VREF<CalibrationLayout> that) {
	return VFat<CalibrationHolder> (CalibrationImplHolder () ,that) ;
}

exports CFat<CalibrationHolder> CalibrationHolder::hold (CREF<CalibrationLayout> that) {
	return CFat<CalibrationHolder> (CalibrationImplHolder () ,that) ;
}
} ;
