#include "export.h"

namespace CSC3DCV {
class CameraBlockImplHolder implement CameraBlockHolder {
protected:

public:
	void initialize () override {

	}


} ;

DLLEXPORT
exports AutoRef<CameraBlockHolder> CameraBlockHolder::create () {
	return AutoRef<CameraBlockImplHolder>::make () ;
}
} ;
