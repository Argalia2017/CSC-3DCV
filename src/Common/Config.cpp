#include "export.h"

namespace CSC3DCV {
struct ConfigProcImplLayout {
	Path mDataPath ;
} ;

class ConfigProcImplHolder final implement Fat<ConfigProcHolder ,ConfigProcLayout> {
public:
	void initialize () override {
		Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
		fake.mThis = SharedRef<ConfigProcImplLayout>::make () ;
	}

	void set_data_dire (CREF<String<STR>> dire) const override {
		fake.mThis->mDataPath = Path (dire) ;
		const auto r1x = FileProc::lock_dire (fake.mThis->mDataPath) ;
		assume (r1x) ;
		Global<Path> (slice ("mDataPath")).store (copy (fake.mThis->mDataPath)) ;
	}
} ;

exports CREF<ConfigProcLayout> ConfigProcHolder::instance () {
	return memorize ([&] () {
		ConfigProcLayout ret ;
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