#pragma once

#include "../util.h"

namespace CSC3DCV {
struct PlaneSplittingLayout ;

struct PlaneSplittingHolder implement Interface {
	imports OfThis<AutoRef<PlaneSplittingLayout>> create () ;
	imports VFat<PlaneSplittingHolder> hold (VREF<PlaneSplittingLayout> that) ;
	imports CFat<PlaneSplittingHolder> hold (CREF<PlaneSplittingLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

class PlaneSplitting implement OfThis<AutoRef<PlaneSplittingLayout>> {
public:
	implicit PlaneSplitting () = default ;

	explicit PlaneSplitting (CREF<typeof (NULL)>) {
		mThis = PlaneSplittingHolder::create () ;
		PlaneSplittingHolder::hold (thiz)->initialize () ;
	}

	void execute () {
		return PlaneSplittingHolder::hold (thiz)->execute () ;
	}
} ;

struct PlaneRenderingLayout ;

struct PlaneRenderingHolder implement Interface {
	imports OfThis<AutoRef<PlaneRenderingLayout>> create () ;
	imports VFat<PlaneRenderingHolder> hold (VREF<PlaneRenderingLayout> that) ;
	imports CFat<PlaneRenderingHolder> hold (CREF<PlaneRenderingLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

class PlaneRendering implement OfThis<AutoRef<PlaneRenderingLayout>> {
public:
	implicit PlaneRendering () = default ;

	explicit PlaneRendering (CREF<typeof (NULL)>) {
		mThis = PlaneRenderingHolder::create () ;
		PlaneRenderingHolder::hold (thiz)->initialize () ;
	}

	void execute () {
		return PlaneRenderingHolder::hold (thiz)->execute () ;
	}
} ;
} ;