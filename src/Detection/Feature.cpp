#include "export.h"

namespace CSC3DCV {
class FeatureImplHolder implement FeatureHolder {
protected:

public:
	void initialize () override {

	}

	void execute () override {

	}
} ;

exports DLLEXTERN AutoRef<FeatureHolder> FeatureHolder::create () {
	return AutoRef<FeatureImplHolder>::make () ;
}
} ;
