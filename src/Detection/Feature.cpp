#include "export.h"

namespace CSC3DCV {
class FeatureImplHolder final implement Fat<FeatureHolder ,FeatureLayout> {
public:
	void initialize () override {

	}

	void execute () override {

	}
} ;

exports VFat<FeatureHolder> FeatureHolder::hold (VREF<FeatureLayout> that) {
	return VFat<FeatureHolder> (FeatureImplHolder () ,that) ;
}

exports CFat<FeatureHolder> FeatureHolder::hold (CREF<FeatureLayout> that) {
	return CFat<FeatureHolder> (FeatureImplHolder () ,that) ;
}
} ;
