#include "export.h"

namespace CSC3DCV {
struct BoardImplLayout {
	ImageShape mBoardShape ;
	Just<BoardType> mBoardType ;
} ;

class BoardImplHolder final implement Fat<BoardHolder ,BoardLayout> {
public:
	void initialize () override {
		fake.mThis = AutoRef<BoardImplLayout>::make () ;
	}

	void set_board_width (CREF<ImageShape> shape) override {
		fake.mThis->mBoardShape = shape ;
	}

	void set_board_type (CREF<Just<BoardType>> type) override {
		fake.mThis->mBoardType = type ;
	}

	Array<Point2F> detect (CREF<Image<Color3B>> image) override {
		Array<Point2F> ret ;
		unimplemented () ;
		return move (ret) ;
	}
} ;

exports VFat<BoardHolder> BoardHolder::hold (VREF<BoardLayout> that) {
	return VFat<BoardHolder> (BoardImplHolder () ,that) ;
}

exports CFat<BoardHolder> BoardHolder::hold (CREF<BoardLayout> that) {
	return CFat<BoardHolder> (BoardImplHolder () ,that) ;
}
} ;
