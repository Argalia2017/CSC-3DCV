#include "export.h"

namespace CSC3DCV {
class CameraFrameImplHolder final implement Fat<CameraFrameHolder ,CameraFrameLayout> {
public:

} ;

exports VFat<CameraFrameHolder> CameraFrameHolder::hold (VREF<CameraFrameLayout> that) {
	return VFat<CameraFrameHolder> (CameraFrameImplHolder () ,that) ;
}

exports CFat<CameraFrameHolder> CameraFrameHolder::hold (CREF<CameraFrameLayout> that) {
	return CFat<CameraFrameHolder> (CameraFrameImplHolder () ,that) ;
}
} ;
