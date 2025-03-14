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
	virtual Matrix solve_pnp (CREF<Array<Point2F>> point_2d ,CREF<Array<Point3F>> point_3d ,CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist) const = 0 ;
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

	static Matrix solve_pnp (CREF<Array<Point2F>> point_2d ,CREF<Array<Point3F>> point_3d ,CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist) {
		return SolverProcHolder::hold (instance ())->solve_pnp (point_2d ,point_3d ,mat_k ,dist) ;
	}
} ;
} ;