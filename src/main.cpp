#include "export.h"

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