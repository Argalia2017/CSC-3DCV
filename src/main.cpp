#include "util.h"

#include "Common/export.h"
#include "Detection/export.h"
#include "Calibration/export.h"
#include "Rendering/export.h"
#include "Registration/export.h"
#include "Meshing/export.h"
#include "System/export.h"

#include <csc_end.h>
#include <exception>
#include <csc_begin.h>

using namespace CSC3DCV ;

int main (int argc ,DEF<char **> argv) {
	Singleton<Console>::instance ().start () ;
	Singleton<Console>::instance ().open (slice (".")) ;
	try {
		Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
		assume (argc >= 2) ;
		const auto r1x = Path (Slice (FLAG (argv[1]) ,SLICE_MAX_SIZE::expr ,1)).absolute () ;
		Singleton<Console>::instance ().info (slice ("working_path = ") ,r1x.fetch ()) ;
		ConfigProc::instance ().set_data_dire (r1x) ;
		watch (r1x) ;
		watch (r1x) ;
		watch (r1x) ;
	} catch (CREF<Exception> e) {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().error (slice ("ERROR")) ;
		const auto r1x = Format (slice ("assume : $1 at $2 in $3, $4")) ;
		Singleton<Console>::instance ().error (r1x (e.what () ,e.func () ,e.file () ,e.line ())) ;
		Singleton<Console>::instance ().trace () ;
	}
	GlobalRoot::instance ().shutdown () ;
	return 0 ;
}