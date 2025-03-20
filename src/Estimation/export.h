#pragma once

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
	virtual Array<FLT64> encode_matrix (CREF<Matrix> mat_a) const = 0 ;
	virtual Matrix decode_matrix (CREF<Array<FLT64>> mat_a) const = 0 ;
	virtual Matrix solve_pnp (CREF<Array<Point2F>> point1 ,CREF<Array<Point3F>> point2 ,CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist) const = 0 ;
	virtual Vector redistortion (CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist ,CREF<Vector> point) const = 0 ;
	virtual Vector undistortion (CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist ,CREF<Vector> point) const = 0 ;
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

	static Array<FLT64> encode_matrix (CREF<Matrix> mat_a) {
		return SolverProcHolder::hold (instance ())->encode_matrix (mat_a) ;
	}

	static Matrix decode_matrix (CREF<Array<FLT64>> mat_a) {
		return SolverProcHolder::hold (instance ())->decode_matrix (mat_a) ;
	}

	static Matrix solve_pnp (CREF<Array<Point2F>> point1 ,CREF<Array<Point3F>> point2 ,CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist) {
		return SolverProcHolder::hold (instance ())->solve_pnp (point1 ,point2 ,mat_k ,dist) ;
	}

	static Vector redistortion (CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist ,CREF<Vector> point) {
		return SolverProcHolder::hold (instance ())->redistortion (mat_k ,dist ,point) ;
	}

	static Vector undistortion (CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist ,CREF<Vector> point) {
		return SolverProcHolder::hold (instance ())->undistortion (mat_k ,dist ,point) ;
	}
} ;

struct HomographyDepthLayout ;

struct HomographyDepthHolder implement Interface {
	imports OfThis<AutoRef<HomographyDepthLayout>> create () ;
	imports VFat<HomographyDepthHolder> hold (VREF<HomographyDepthLayout> that) ;
	imports CFat<HomographyDepthHolder> hold (CREF<HomographyDepthLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

class HomographyDepth implement OfThis<AutoRef<HomographyDepthLayout>> {
public:
	implicit HomographyDepth () = default ;

	explicit HomographyDepth (CREF<typeof (NULL)>) {
		mThis = HomographyDepthHolder::create () ;
		HomographyDepthHolder::hold (thiz)->initialize () ;
	}

	void execute () {
		return HomographyDepthHolder::hold (thiz)->execute () ;
	}
} ;
} ;