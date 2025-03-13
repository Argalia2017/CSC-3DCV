#include "export.h"

namespace CSC3DCV {
struct ToolProcImplLayout {} ;

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

	Array<FLT64> flatten (CREF<Matrix> matrix) const override {
		Array<FLT64> ret = Array<FLT64> (16) ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			INDEX ix = i.mX + i.mY * 4 ;
			ret[ix] = matrix[i] ;
		}
		return move (ret) ;
	}
} ;

exports CREF<ToolProcLayout> ToolProcHolder::instance () {
	return memorize ([&] () {
		ToolProcLayout ret ;
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