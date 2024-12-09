#include "util.h"

#include "Common/export.h"
#include "Detection/export.h"
#include "Calibration/export.h"
#include "Rendering/export.h"
#include "Registration/export.h"
#include "Meshing/export.h"
#include "System/export.h"

using namespace CSC3DCV ;

int main () {
	Singleton<Console>::instance ().start () ;
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().info (slice ("library_name = ") ,RuntimeProc::library_name ()) ;
	Singleton<Console>::instance ().info (slice ("heap_address = ") ,QUAD (address (Heap::instance ()))) ;

	GlobalRoot::instance ().shutdown () ;
	return 0 ;
}