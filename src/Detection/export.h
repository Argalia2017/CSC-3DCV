#pragma once

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

struct BoardLayout ;

struct BoardHolder implement Interface {
	imports OfThis<AutoRef<BoardLayout>> create () ;
	imports VFat<BoardHolder> hold (VREF<BoardLayout> that) ;
	imports CFat<BoardHolder> hold (CREF<BoardLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_board_shape (CREF<ImageShape> shape) = 0 ;
	virtual void set_board_type (CREF<Just<BoardType>> type) = 0 ;
	virtual void set_board_baseline (CREF<Vector> baseline) = 0 ;
	virtual Array<Point3F> extract () const = 0 ;
	virtual Optional<Array<Point2F>> detect (CREF<Image<Color3B>> image) = 0 ;
} ;

class Board implement OfThis<AutoRef<BoardLayout>> {
public:
	implicit Board () = default ;

	explicit Board (CREF<typeof (NULL)>) {
		mThis = BoardHolder::create () ;
		BoardHolder::hold (thiz)->initialize () ;
	}

	void set_board_shape (CREF<ImageShape> shape) {
		return BoardHolder::hold (thiz)->set_board_shape (shape) ;
	}

	void set_board_type (CREF<Just<BoardType>> type) {
		return BoardHolder::hold (thiz)->set_board_type (type) ;
	}

	void set_board_baseline (CREF<Vector> baseline) {
		return BoardHolder::hold (thiz)->set_board_baseline (baseline) ;
	}

	Array<Point3F> extract () const {
		return BoardHolder::hold (thiz)->extract () ;
	}

	Optional<Array<Point2F>> detect (CREF<Image<Color3B>> image) {
		return BoardHolder::hold (thiz)->detect (image) ;
	}
} ;

struct FeatureLayout ;

struct FeatureHolder implement Interface {
	imports OfThis<AutoRef<FeatureLayout>> create () ;
	imports VFat<FeatureHolder> hold (VREF<FeatureLayout> that) ;
	imports CFat<FeatureHolder> hold (CREF<FeatureLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void process (CREF<Image<Color3B>> image) = 0 ;
} ;

class Feature implement OfThis<AutoRef<FeatureLayout>> {
public:
	implicit Feature () = default ;

	explicit Feature (CREF<typeof (NULL)>) {
		mThis = FeatureHolder::create () ;
		FeatureHolder::hold (thiz)->initialize () ;
	}

	void process (CREF<Image<Color3B>> image) {
		return FeatureHolder::hold (thiz)->process (image) ;
	}
} ;

struct LSDLayout ;

struct LSDHolder implement Interface {
	imports OfThis<AutoRef<LSDLayout>> create () ;
	imports VFat<LSDHolder> hold (VREF<LSDLayout> that) ;
	imports CFat<LSDHolder> hold (CREF<LSDLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void process (CREF<Image<Color3B>> image) = 0 ;
	virtual Array<Line2F> detect () = 0 ;
} ;

class LSD implement OfThis<AutoRef<LSDLayout>> {
public:
	implicit LSD () = default ;

	explicit LSD (CREF<typeof (NULL)>) {
		mThis = LSDHolder::create () ;
		LSDHolder::hold (thiz)->initialize () ;
	}

	void process (CREF<Image<Color3B>> image) {
		return LSDHolder::hold (thiz)->process (image) ;
	}

	Array<Line2F> detect () {
		return LSDHolder::hold (thiz)->detect () ;
	}
} ;
} ;