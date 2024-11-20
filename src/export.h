#include "util.h"

#include "Detection/export.h"
#include "Calibration/export.h"
#include "Rendering/export.h"
#include "Registration/export.h"
#include "Meshing/export.h"
#include "System/export.h"

namespace CSC3DCV {
struct ConfigHolder implement Interface {
	imports AutoRef<ConfigHolder> create () ;

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