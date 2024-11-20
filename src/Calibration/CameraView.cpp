#include "export.h"

namespace CSC3DCV {
class CameraViewImplHolder implement CameraViewHolder {
protected:

public:
	void initialize () override {

	}


} ;

DLLEXPORT
exports AutoRef<CameraViewHolder> CameraViewHolder::create () {
	return AutoRef<CameraViewImplHolder>::make () ;
}
} ;
