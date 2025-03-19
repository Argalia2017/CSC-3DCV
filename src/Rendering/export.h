#include "../util.h"

namespace CSC3DCV {
struct DisplayLayout ;

struct DisplayHolder implement Interface {
	imports OfThis<AutoRef<DisplayLayout>> create () ;
	imports VFat<DisplayHolder> hold (VREF<DisplayLayout> that) ;
	imports CFat<DisplayHolder> hold (CREF<DisplayLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

class Display implement OfThis<AutoRef<DisplayLayout>> {
public:
	implicit Display () = default ;

	explicit Display (CREF<typeof (NULL)>) {
		mThis = DisplayHolder::create () ;
		DisplayHolder::hold (thiz)->initialize () ;
	}

	void execute () {
		return DisplayHolder::hold (thiz)->execute () ;
	}
} ;
} ;