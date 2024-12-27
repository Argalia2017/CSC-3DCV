#include "export.h"

namespace CSC3DCV {
class CameraBlockImplHolder final implement Fat<CameraBlockHolder ,CameraBlockLayout> {
public:
	void initialize () override {

	}


} ;

exports VFat<CameraBlockHolder> CameraBlockHolder::hold (VREF<CameraBlockLayout> that) {
	return VFat<CameraBlockHolder> (CameraBlockImplHolder () ,that) ;
}

exports CFat<CameraBlockHolder> CameraBlockHolder::hold (CREF<CameraBlockLayout> that) {
	return CFat<CameraBlockHolder> (CameraBlockImplHolder () ,that) ;
}
} ;
