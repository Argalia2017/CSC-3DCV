#include "../util.h"

namespace CSC3DCV {
struct FeatureHolder implement Interface {
	imports DLLEXTERN AutoRef<FeatureHolder> create () ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

struct FeatureLayout implement ThisLayout<AutoRef<FeatureHolder>> {} ;

class Feature implement FeatureLayout {
public:
	using FeatureLayout::mThis ;

public:
	implicit Feature () = default ;

	explicit Feature (CREF<typeof (NULL)>) {
		mThis = FeatureHolder::create () ;
		mThis->initialize () ;
	}

	void execute () {
		return mThis->execute () ;
	}
} ;
} ;