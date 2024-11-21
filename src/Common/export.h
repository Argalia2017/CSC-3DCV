#include "../util.h"

namespace CSC3DCV {
struct ConfigHolder implement Interface {	
	imports DLLEXTERN AutoRef<ConfigHolder> DLLAPI create () ;

	virtual void initialize () = 0 ;
} ;

struct ConfigLayout implement ThisLayout<AutoRef<ConfigHolder>> {} ;

class Config implement ConfigLayout {
public:
	using ConfigLayout::mThis ;

public:
	implicit Config () = default ;

	explicit Config (CREF<typeof (NULL)>) {
		mThis = ConfigHolder::create () ;
		mThis->initialize () ;
	}
} ;
} ;