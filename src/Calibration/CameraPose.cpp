#include "export.h"

namespace CSC3DCV {
class CameraPoseImplHolder final implement Fat<CameraPoseHolder ,CameraPoseLayout> {
public:
	void sfm_pose_mat_v (CREF<ModelFrame> frame1) override {

	}
} ;

exports VFat<CameraPoseHolder> CameraPoseHolder::hold (VREF<CameraPoseLayout> that) {
	return VFat<CameraPoseHolder> (CameraPoseImplHolder () ,that) ;
}

exports CFat<CameraPoseHolder> CameraPoseHolder::hold (CREF<CameraPoseLayout> that) {
	return CFat<CameraPoseHolder> (CameraPoseImplHolder () ,that) ;
}
} ;
