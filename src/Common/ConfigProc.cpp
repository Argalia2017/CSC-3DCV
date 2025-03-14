#include "export.h"

namespace CSC3DCV {
struct ConfigProcLayout {
	Path mDataPath ;
} ;

class ConfigProcImplHolder final implement Fat<ConfigProcHolder ,ConfigProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void set_data_dire (CREF<String<STR>> dire) override {
		self.mDataPath = Path (dire) ;
		const auto r1x = FileProc::lock_dire (self.mDataPath) ;
		assume (r1x) ;
		Global<Path> (slice ("mDataPath")).store (self.mDataPath) ;
	}
} ;

exports CREF<OfThis<SharedRef<ConfigProcLayout>>> ConfigProcHolder::instance () {
	return memorize ([&] () {
		OfThis<SharedRef<ConfigProcLayout>> ret ;
		ret.mThis = SharedRef<ConfigProcLayout>::make () ;
		ConfigProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ConfigProcHolder> ConfigProcHolder::hold (VREF<ConfigProcLayout> that) {
	return VFat<ConfigProcHolder> (ConfigProcImplHolder () ,that) ;
}

exports CFat<ConfigProcHolder> ConfigProcHolder::hold (CREF<ConfigProcLayout> that) {
	return CFat<ConfigProcHolder> (ConfigProcImplHolder () ,that) ;
}
} ;