#include "export.h"

namespace CSC3DCV {
class ConfigImplHolder implement ConfigHolder {
protected:

public:
	void initialize () override {

	}
} ;

exports DLLEXTERN AutoRef<ConfigHolder> DLLAPI ConfigHolder::create () {
	return AutoRef<ConfigImplHolder>::make () ;
}
} ;
