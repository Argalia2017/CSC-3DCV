#include "../util.h"

namespace CSC3DCV {
struct CameraViewHolder implement Interface {	
	imports DLLEXTERN AutoRef<CameraViewHolder> create () ;

	virtual void initialize () = 0 ;
} ;

struct CameraViewLayout implement ThisLayout<AutoRef<CameraViewHolder>> {} ;

class CameraView implement CameraViewLayout {
public:
	using CameraViewLayout::mThis ;

public:
	implicit CameraView () = default ;

	explicit CameraView (CREF<typeof (NULL)>) {
		mThis = CameraViewHolder::create () ;
		mThis->initialize () ;
	}
} ;

struct CameraPoseHolder implement Interface {
	imports DLLEXTERN AutoRef<CameraPoseHolder> create () ;

	virtual void initialize () = 0 ;
} ;

struct CameraPoseLayout implement ThisLayout<AutoRef<CameraPoseHolder>> {} ;

class CameraPose implement CameraPoseLayout {
public:
	using CameraPoseLayout::mThis ;

public:
	implicit CameraPose () = default ;

	explicit CameraPose (CREF<typeof (NULL)>) {
		mThis = CameraPoseHolder::create () ;
		mThis->initialize () ;
	}
} ;

struct CameraBlockHolder implement Interface {	
	imports DLLEXTERN AutoRef<CameraBlockHolder> create () ;

	virtual void initialize () = 0 ;
} ;

struct CameraBlockLayout implement ThisLayout<AutoRef<CameraBlockHolder>> {} ;

class CameraBlock implement CameraBlockLayout {
public:
	using CameraBlockLayout::mThis ;

public:
	implicit CameraBlock () = default ;

	explicit CameraBlock (CREF<typeof (NULL)>) {
		mThis = CameraBlockHolder::create () ;
		mThis->initialize () ;
	}
} ;

struct CameraOptimizerHolder implement Interface {
	imports DLLEXTERN AutoRef<CameraOptimizerHolder> create () ;

	virtual void initialize () = 0 ;
} ;

struct CameraOptimizerLayout implement ThisLayout<AutoRef<CameraOptimizerHolder>> {} ;

class CameraOptimizer implement CameraOptimizerLayout {
public:
	using CameraOptimizerLayout::mThis ;

public:
	implicit CameraOptimizer () = default ;

	explicit CameraOptimizer (CREF<typeof (NULL)>) {
		mThis = CameraOptimizerHolder::create () ;
		mThis->initialize () ;
	}
} ;

struct CalibrationHolder implement Interface {	
	imports DLLEXTERN AutoRef<CalibrationHolder> create () ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

struct CalibrationLayout implement ThisLayout<AutoRef<CalibrationHolder>> {} ;

class Calibration implement CalibrationLayout {
public:
	using CalibrationLayout::mThis ;

public:
	implicit Calibration () = default ;

	explicit Calibration (CREF<typeof (NULL)>) {
		mThis = CalibrationHolder::create () ;
		mThis->initialize () ;
	}

	void execute () {
		return mThis->execute () ;
	}
} ;
} ;