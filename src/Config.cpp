#include "export.h"

namespace CSC3DCV {
class ConfigImplHolder implement ConfigHolder {
protected:

public:
	void initialize () override {

	}
} ;

exports AutoRef<ConfigHolder> ConfigHolder::create () {
	return AutoRef<ConfigImplHolder>::make () ;
}
} ;
