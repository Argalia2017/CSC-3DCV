#include "export.h"

namespace CSC3DCV {
class CameraOptimizerImplHolder implement CameraOptimizerHolder {
protected:

public:
	void initialize () override {

	}


} ;

DLLEXPORT
exports AutoRef<CameraOptimizerHolder> CameraOptimizerHolder::create () {
	return AutoRef<CameraOptimizerImplHolder>::make () ;
}
} ;
