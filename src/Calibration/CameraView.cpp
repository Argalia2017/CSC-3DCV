#include "export.h"

namespace CSC3DCV {
class CameraViewImplHolder final implement Fat<CameraViewHolder ,CameraViewLayout> {
public:

} ;

exports VFat<CameraViewHolder> CameraViewHolder::hold (VREF<CameraViewLayout> that) {
	return VFat<CameraViewHolder> (CameraViewImplHolder () ,that) ;
}

exports CFat<CameraViewHolder> CameraViewHolder::hold (CREF<CameraViewLayout> that) {
	return CFat<CameraViewHolder> (CameraViewImplHolder () ,that) ;
}
} ;
