#include "export.h"

namespace CSC3DCV {
struct DisplayLayout {

} ;

class DisplayImplHolder final implement Fat<DisplayHolder ,DisplayLayout> {
public:
	void initialize () override {

	}

	void execute () override {

	}
} ;

exports OfThis<AutoRef<DisplayLayout>> DisplayHolder::create () {
	OfThis<AutoRef<DisplayLayout>> ret ;
	ret.mThis = AutoRef<DisplayLayout>::make () ;
	return move (ret) ;
}

exports VFat<DisplayHolder> DisplayHolder::hold (VREF<DisplayLayout> that) {
	return VFat<DisplayHolder> (DisplayImplHolder () ,that) ;
}

exports CFat<DisplayHolder> DisplayHolder::hold (CREF<DisplayLayout> that) {
	return CFat<DisplayHolder> (DisplayImplHolder () ,that) ;
}
} ;
