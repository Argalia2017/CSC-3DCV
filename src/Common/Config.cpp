#include "export.h"

namespace CSC3DCV {
class ConfigImplHolder implement ConfigHolder {
protected:

public:
	void initialize () override {
		Singleton<Console>::instance ().info (slice ("library_name = ") ,RuntimeProc::library_name ()) ;
		Singleton<Console>::instance ().info (slice ("heap_address = ") ,QUAD (address (Heap::instance ()))) ;
	}
} ;

exports DLLEXTERN AutoRef<ConfigHolder> ConfigHolder::create () {
	return AutoRef<ConfigImplHolder>::make () ;
}
} ;
