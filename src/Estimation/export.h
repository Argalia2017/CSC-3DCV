#include "../util.h"

namespace CSC3DCV {
struct SolverProcLayout ;

struct SolverProcHolder implement Interface {
	imports CREF<OfThis<SharedRef<SolverProcLayout>>> instance () ;
	imports VFat<SolverProcHolder> hold (VREF<SolverProcLayout> that) ;
	imports CFat<SolverProcHolder> hold (CREF<SolverProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Buffer3<FLT64> encode_rotation (CREF<Matrix> mat_r) const = 0 ;
	virtual Matrix decode_rotation (CREF<Buffer3<FLT64>> mat_r) const = 0 ;
} ;

class SolverProc implement OfThis<SharedRef<SolverProcLayout>> {
public:
	static CREF<SolverProc> instance () {
		return keep[TYPE<SolverProc>::expr] (SolverProcHolder::instance ()) ;
	}

	static Buffer3<FLT64> encode_rotation (CREF<Matrix> mat_r) {
		return SolverProcHolder::hold (instance ())->encode_rotation (mat_r) ;
	}

	static Matrix decode_rotation (CREF<Buffer3<FLT64>> mat_r) {
		return SolverProcHolder::hold (instance ())->decode_rotation (mat_r) ;
	}
} ;
} ;