#include "export.h"

namespace CSC3DCV {
struct FeatureLayout {

} ;

class FeatureImplHolder final implement Fat<FeatureHolder ,FeatureLayout> {
public:
	void initialize () override {

	}

	void execute () override {

	}
} ;

exports OfThis<AutoRef<FeatureLayout>> FeatureHolder::create () {
	OfThis<AutoRef<FeatureLayout>> ret ;
	ret.mThis = AutoRef<FeatureLayout>::make () ;
	return move (ret) ;
}

exports VFat<FeatureHolder> FeatureHolder::hold (VREF<FeatureLayout> that) {
	return VFat<FeatureHolder> (FeatureImplHolder () ,that) ;
}

exports CFat<FeatureHolder> FeatureHolder::hold (CREF<FeatureLayout> that) {
	return CFat<FeatureHolder> (FeatureImplHolder () ,that) ;
}
} ;
