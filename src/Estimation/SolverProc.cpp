#include "export.h"

namespace CSC3DCV {
struct SolverProcLayout {} ;

class SolverProcImplHolder final implement Fat<SolverProcHolder ,SolverProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Buffer3<FLT64> encode_rotation (CREF<Matrix> mat_r) const override {
		Buffer3<FLT64> ret ;
		const auto r1x = Quaternion (mat_r) ;
		const auto r2x = r1x.vector () ;
		ret[0] = r2x[0] ;
		ret[1] = r2x[1] ;
		ret[2] = r2x[2] ;
		return move (ret) ;
	}

	Matrix decode_rotation (CREF<Buffer3<FLT64>> mat_r) const override {
		const auto r1x = Vector (mat_r[0] ,mat_r[1] ,mat_r[2] ,0) ;
		const auto r2x = Quaternion (r1x) ;
		return r2x.matrix () ;
	}
} ;

exports CREF<OfThis<SharedRef<SolverProcLayout>>> SolverProcHolder::instance () {
	return memorize ([&] () {
		OfThis<SharedRef<SolverProcLayout>> ret ;
		ret.mThis = SharedRef<SolverProcLayout>::make () ;
		SolverProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<SolverProcHolder> SolverProcHolder::hold (VREF<SolverProcLayout> that) {
	return VFat<SolverProcHolder> (SolverProcImplHolder () ,that) ;
}

exports CFat<SolverProcHolder> SolverProcHolder::hold (CREF<SolverProcLayout> that) {
	return CFat<SolverProcHolder> (SolverProcImplHolder () ,that) ;
}
} ;