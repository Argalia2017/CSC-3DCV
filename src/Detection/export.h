#include "../util.h"

namespace CSC3DCV {
struct BoardType {
	enum {
		CHESS ,
		CHESS_INV ,
		CIRCLE ,
		CIRCLE_INV ,
		ETC
	} ;
} ;

struct BoardImplLayout ;
struct BoardLayout implement OfThis<AutoRef<BoardImplLayout>> {} ;

struct BoardHolder implement Interface {
	imports VFat<BoardHolder> hold (VREF<BoardLayout> that) ;
	imports CFat<BoardHolder> hold (CREF<BoardLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_board_width (CREF<ImageShape> shape) = 0 ;
	virtual void set_board_type (CREF<Just<BoardType>> type) = 0 ;
	virtual void set_board_baseline (CREF<FLT64> baseline_x ,CREF<FLT64> baseline_y) = 0 ;
	virtual Array<Point3F> extract () const = 0 ;
	virtual Optional<Array<Point2F>> detect (CREF<Image<Color3B>> image) = 0 ;
} ;

class Board implement BoardLayout {
public:
	implicit Board () = default ;

	explicit Board (CREF<typeof (NULL)>) {
		BoardHolder::hold (thiz)->initialize () ;
	}

	void set_board_width (CREF<ImageShape> shape) {
		return BoardHolder::hold (thiz)->set_board_width (shape) ;
	}

	void set_board_type (CREF<Just<BoardType>> type) {
		return BoardHolder::hold (thiz)->set_board_type (type) ;
	}

	void set_board_baseline (CREF<FLT64> baseline_x ,CREF<FLT64> baseline_y) {
		return BoardHolder::hold (thiz)->set_board_baseline (baseline_x ,baseline_y) ;
	}

	Array<Point3F> extract () const {
		return BoardHolder::hold (thiz)->extract () ;
	}

	Optional<Array<Point2F>> detect (CREF<Image<Color3B>> image) {
		return BoardHolder::hold (thiz)->detect (image) ;
	}
} ;

struct FeatureImplLayout ;
struct FeatureLayout implement OfThis<AutoRef<FeatureImplLayout>> {} ;

struct FeatureHolder implement Interface {
	imports VFat<FeatureHolder> hold (VREF<FeatureLayout> that) ;
	imports CFat<FeatureHolder> hold (CREF<FeatureLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

class Feature implement FeatureLayout {
public:
	implicit Feature () = default ;

	explicit Feature (CREF<typeof (NULL)>) {
		FeatureHolder::hold (thiz)->initialize () ;
	}

	void execute () {
		return FeatureHolder::hold (thiz)->execute () ;
	}
} ;
} ;