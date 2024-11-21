#include "export.h"

namespace CSC3DCV {
class CameraViewImplHolder implement CameraViewHolder {
protected:

public:
	void initialize () override {

	}


} ;

exports DLLEXTERN AutoRef<CameraViewHolder> DLLAPI CameraViewHolder::create () {
	return AutoRef<CameraViewImplHolder>::make () ;
}
} ;
