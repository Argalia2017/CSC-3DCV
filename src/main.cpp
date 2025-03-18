#include "util.h"

#include "Common/export.h"
#include "Localization/export.h"
#include "Rendering/export.h"

using namespace CSC3DCV ;

/*
1. 提前计算基线的长度
2. 实现多个标定板的结构
*/

exports int main (int argc ,DEF<char **> argv) {
	const auto r1x = Singleton<Console>::instance () ;
	r1x.show () ;
#ifdef __CSC_VER_RELEASE__
	try {
#endif
		r1x.trace () ;
		r1x.debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
		assume (argc >= 2) ;
		const auto r2x = Path (Slice (FLAG (argv[1]) ,SLICE_MAX_SIZE::expr ,1)).absolute ().fetch () ;
		r1x.debug (slice ("working_path = ") ,r2x) ;
		r1x.open (r2x) ;
		r1x.trace () ;
		ConfigProc::set_data_dire (r2x) ;
		const auto r3x = CurrentTime () ;
		if ifdo (TRUE) {
			auto rax = Calibration (NULL) ;
			rax.execute () ;
		}
		const auto r4x = CurrentTime () ;
		const auto r5x = ToolProc::format_time (r4x - r3x) ;
		r1x.trace () ;
		r1x.info (slice ("all done")) ;
		r1x.warn (slice ("time_cost = ") ,r5x) ;
		r1x.trace () ;
#ifdef __CSC_VER_RELEASE__
	} catch (CREF<Exception> e) {
		r1x.trace () ;
		r1x.error (slice ("ERROR")) ;
		const auto r6x = Format (slice ("assume : $1 at $2 in $3, $4")) ;
		r1x.error (r6x (e.what () ,e.func () ,e.file () ,e.line ())) ;
		r1x.trace () ;
	}
#endif
	GlobalProc::shutdown () ;
	return 0 ;
}