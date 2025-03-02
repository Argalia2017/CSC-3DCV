#include "util.h"

#include "Common/export.h"
#include "Detection/export.h"
#include "Calibration/export.h"
#include "Rendering/export.h"
#include "Registration/export.h"
#include "Meshing/export.h"

using namespace CSC3DCV ;

inline String<STR> format_time (CREF<Time> time) {
	const auto r1x = time.milliseconds () ;
	const auto r2x = r1x / 60000 ;
	const auto r3x = r1x % 60000 / 1000 ;
	const auto r4x = AlignedText (r1x % 1000 ,3) ;
	return String<STR>::make (r2x ,slice ("m") ,r3x ,slice (".") ,r4x ,slice ("s")) ;
}

exports int main (int argc ,DEF<char **> argv) {
	const auto r1x = Singleton<Console>::instance () ;
	r1x.show () ;
	try {
		r1x.trace () ;
		r1x.info (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
		assume (argc >= 2) ;
		const auto r2x = Path (Slice (FLAG (argv[1]) ,SLICE_MAX_SIZE::expr ,1)).absolute ().fetch () ;
		r1x.info (slice ("working_path = ") ,r2x) ;
		r1x.open (r2x) ;
		r1x.trace () ;
		ConfigProc::set_data_dire (r2x) ;
		const auto r3x = CurrentTime () ;
		RuntimeProc::thread_sleep (Time (1000)) ;
		const auto r4x = CurrentTime () ;
		const auto r5x = format_time (r4x - r3x) ;
		r1x.trace () ;
		r1x.info (slice ("all done")) ;
		r1x.warn (slice ("time_cost = ") ,r5x) ;
		r1x.trace () ;
	} catch (CREF<Exception> e) {
		r1x.trace () ;
		r1x.error (slice ("ERROR")) ;
		const auto r6x = Format (slice ("assume : $1 at $2 in $3, $4")) ;
		r1x.error (r6x (e.what () ,e.func () ,e.file () ,e.line ())) ;
		r1x.trace () ;
	}
	GlobalProc::shutdown () ;
	return 0 ;
}