#include "../util.h"

namespace CSC3DCV {
struct FeatureImplLayout ;

struct FeatureLayout implement ThisLayout<AutoRef<FeatureImplLayout>> {} ;

struct FeatureHolder implement Interface {
	imports VFat<FeatureHolder> hold (VREF<FeatureLayout> that) ;
	imports CFat<FeatureHolder> hold (CREF<FeatureLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

class Feature implement FeatureLayout {
public:
	implicit Feature () = default ;

	explicit Feature (CREF<typeof (NULL)>) {
		FeatureHolder::hold (thiz)->initialize () ;
	}

	void execute () {
		return FeatureHolder::hold (thiz)->execute () ;
	}
} ;
} ;