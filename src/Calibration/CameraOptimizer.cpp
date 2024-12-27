#include "export.h"

namespace CSC3DCV {
class CameraOptimizerImplHolder final implement Fat<CameraOptimizerHolder ,CameraOptimizerLayout> {
public:
	void initialize () override {

	}


} ;

exports VFat<CameraOptimizerHolder> CameraOptimizerHolder::hold (VREF<CameraOptimizerLayout> that) {
	return VFat<CameraOptimizerHolder> (CameraOptimizerImplHolder () ,that) ;
}

exports CFat<CameraOptimizerHolder> CameraOptimizerHolder::hold (CREF<CameraOptimizerLayout> that) {
	return CFat<CameraOptimizerHolder> (CameraOptimizerImplHolder () ,that) ;
}
} ;
