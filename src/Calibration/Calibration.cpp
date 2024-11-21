#include "export.h"

namespace CSC3DCV {
class CalibrationImplHolder implement CalibrationHolder {
protected:
	CameraView mCameraView ;
	CameraPose mCameraPose ;
	CameraBlock mCameraBlock ;

public:
	void initialize () override {

	}

	void execute () override {

	}
} ;

exports DLLEXTERN AutoRef<CalibrationHolder> DLLAPI CalibrationHolder::create () {
	return AutoRef<CalibrationImplHolder>::make () ;
}
} ;
