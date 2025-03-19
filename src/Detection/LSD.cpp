#include "export.h"

#include <csc_end.h>
#include "lsd.h"
#include <csc_begin.h>

namespace CSC3DCV {
struct LSDLayout {

} ;

class LSDImplHolder final implement Fat<LSDHolder ,LSDLayout> {
public:
	void initialize () override {

	}

	void process (CREF<Image<Color3B>> image) override {

	}

	Array<Line2F> detect () override {
		Array<Line2F> ret ;

		return move (ret) ;
	}
} ;

exports OfThis<AutoRef<LSDLayout>> LSDHolder::create () {
	OfThis<AutoRef<LSDLayout>> ret ;
	ret.mThis = AutoRef<LSDLayout>::make () ;
	return move (ret) ;
}

exports VFat<LSDHolder> LSDHolder::hold (VREF<LSDLayout> that) {
	return VFat<LSDHolder> (LSDImplHolder () ,that) ;
}

exports CFat<LSDHolder> LSDHolder::hold (CREF<LSDLayout> that) {
	return CFat<LSDHolder> (LSDImplHolder () ,that) ;
}
} ;
