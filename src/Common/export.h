#include "../util.h"

namespace CSC3DCV {
struct ConfigHolder implement Interface {	
	imports DLLEXTERN AutoRef<ConfigHolder> create () ;

	virtual void initialize () = 0 ;
} ;

struct ConfigLayout implement ThisLayout<AutoRef<ConfigHolder>> {} ;

class Config implement ConfigLayout {
public:
	using ConfigLayout::mThis ;

public:
	explicit Config () {
		mThis = ConfigHolder::create () ;
		mThis->initialize () ;
	}
} ;
} ;