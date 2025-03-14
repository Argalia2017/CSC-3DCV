#include "export.h"

namespace CSC3DCV {
struct ToolProcLayout {} ;

class ToolProcImplHolder final implement Fat<ToolProcHolder ,ToolProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Color3B random_color (CREF<Random> random) const override {
		Color3B ret ;
		ret.mB = BYTE (random.random_value (0 ,255)) ;
		ret.mG = BYTE (random.random_value (0 ,255)) ;
		ret.mR = BYTE (random.random_value (0 ,255)) ;
		return move (ret) ;
	}

	String<STR> format_time (CREF<Time> time) const override {
		const auto r1x = time.milliseconds () ;
		const auto r2x = r1x / 60000 ;
		const auto r3x = r1x % 60000 / 1000 ;
		const auto r4x = AlignedText (r1x % 1000 ,3) ;
		return String<STR>::make (r2x ,slice ("m") ,r3x ,slice (".") ,r4x ,slice ("s")) ;
	}
} ;

exports CREF<OfThis<SharedRef<ToolProcLayout>>> ToolProcHolder::instance () {
	return memorize ([&] () {
		OfThis<SharedRef<ToolProcLayout>> ret ;
		ret.mThis = SharedRef<ToolProcLayout>::make () ;
		ToolProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ToolProcHolder> ToolProcHolder::hold (VREF<ToolProcLayout> that) {
	return VFat<ToolProcHolder> (ToolProcImplHolder () ,that) ;
}

exports CFat<ToolProcHolder> ToolProcHolder::hold (CREF<ToolProcLayout> that) {
	return CFat<ToolProcHolder> (ToolProcImplHolder () ,that) ;
}
} ;