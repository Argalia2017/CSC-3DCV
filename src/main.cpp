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
	if ifdo (TRUE) {
		Singleton<Console>::instance ().start () ;
	}
	if ifdo (TRUE) {
		auto rax = Calibration () ;
		rax.execute () ;
	}
	return 0 ;
}