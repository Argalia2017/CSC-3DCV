#include "../util.h"

namespace CSC3DCV {
struct ToolProcLayout ;

struct ToolProcHolder implement Interface {
	imports CREF<OfThis<SharedRef<ToolProcLayout>>> instance () ;
	imports VFat<ToolProcHolder> hold (VREF<ToolProcLayout> that) ;
	imports CFat<ToolProcHolder> hold (CREF<ToolProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Color3B random_color (CREF<Random> random) const = 0 ;
	virtual String<STR> format_time (CREF<Time> time) const = 0 ;
	virtual Array<FLT64> flatten (CREF<Matrix> matrix) const = 0 ;
} ;

class ToolProc implement OfThis<SharedRef<ToolProcLayout>> {
public:
	static CREF<ToolProc> instance () {
		return keep[TYPE<ToolProc>::expr] (ToolProcHolder::instance ()) ;
	}

	static Color3B random_color (CREF<Random> random) {
		return ToolProcHolder::hold (instance ())->random_color (random) ;
	}

	static String<STR> format_time (CREF<Time> time) {
		return ToolProcHolder::hold (instance ())->format_time (time) ;
	}

	static Array<FLT64> flatten (CREF<Matrix> matrix) {
		return ToolProcHolder::hold (instance ())->flatten (matrix) ;
	}
} ;

struct ConfigProcLayout ;

struct ConfigProcHolder implement Interface {
	imports CREF<OfThis<SharedRef<ConfigProcLayout>>> instance () ;
	imports VFat<ConfigProcHolder> hold (VREF<ConfigProcLayout> that) ;
	imports CFat<ConfigProcHolder> hold (CREF<ConfigProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_data_dire (CREF<String<STR>> path) = 0 ;
} ;

class ConfigProc implement OfThis<SharedRef<ConfigProcLayout>> {
public:
	static CREF<ConfigProc> instance () {
		return keep[TYPE<ConfigProc>::expr] (ConfigProcHolder::instance ()) ;
	}

	static void set_data_dire (CREF<String<STR>> dire) {
		return ConfigProcHolder::hold (instance ())->set_data_dire (dire) ;
	}
} ;
} ;