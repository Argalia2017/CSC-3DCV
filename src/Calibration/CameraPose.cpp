#include "export.h"

namespace CSC3DCV {
class CameraPoseImplHolder implement CameraPoseHolder {
protected:

public:
	void initialize () override {

	}


} ;

DLLEXPORT
exports AutoRef<CameraPoseHolder> CameraPoseHolder::create () {
	return AutoRef<CameraPoseImplHolder>::make () ;
}
} ;
