#include "export.h"

#include <csc_end.h>
extern "C" {
#include "lsd.h"
} ;
#include <csc_begin.h>

namespace CSC3DCV {
struct LSDLayout {
	ImageShape mShape ;
	Array<FLT64> mGrayImage ;
	UniqueRef<FLAG> mBuffer ;
} ;

class LSDImplHolder final implement Fat<LSDHolder ,LSDLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void process (CREF<Image<Color3B>> image) override {
		self.mShape = image.shape () ;
		if ifdo (TRUE) {
			if (self.mGrayImage.size () == self.mShape.size ())
				discard ;
			self.mGrayImage = Array<FLT64> (self.mShape.size ()) ;
		}
		const auto r1x = Vector (0.114 ,0.587 ,0.299 ,0) ;
		for (auto &&i : image.range ()) {
			INDEX ix = i.mX + i.mY * self.mShape.mCX ;
			const auto r2x = image[i] ;
			const auto r3x = Vector (FLT64 (r2x.mB) ,FLT64 (r2x.mG) ,FLT64 (r2x.mR) ,0) ;
			self.mGrayImage[ix] = r1x * r3x ;
		}
	}

	Array<Line2F> detect () override {
		auto rax = int (0) ;
		const auto r1x = VAL32 (self.mShape.mCX) ;
		const auto r2x = VAL32 (self.mShape.mCY) ;
		self.mBuffer = UniqueRef<FLAG> ([&] (VREF<FLAG> me) {
			me = FLAG (lsd ((&rax) ,(&self.mGrayImage[0]) ,r1x ,r2x)) ;
		} ,[&] (VREF<FLAG> me) {
			lsd_cleanup (csc_pointer_t (me)) ;
		}) ;
		const auto r3x = self.mBuffer.deref ;
		Array<Line2F> ret = Array<Line2F> (rax) ;
		for (auto &&i : ret.range ()) {
			auto &&rbx = keep[TYPE<Buffer7<FLT64>>::expr] (Pointer::make (r3x + i * 56)) ;
			ret[i].mMin.mX = FLT32 (rbx[0]) ;
			ret[i].mMin.mY = FLT32 (rbx[1]) ;
			ret[i].mMax.mX = FLT32 (rbx[2]) ;
			ret[i].mMax.mY = FLT32 (rbx[3]) ;
		}
		return move (ret) ;
	}
} ;

exports OfThis<AutoRef<LSDLayout>> LSDHolder::create () {
	OfThis<AutoRef<LSDLayout>> ret ;
	ret.mThis = AutoRef<LSDLayout>::make () ;
	return move (ret) ;
}

exports VFat<LSDHolder> LSDHolder::hold (VREF<LSDLayout> that) {
	return VFat<LSDHolder> (LSDImplHolder () ,that) ;
}

exports CFat<LSDHolder> LSDHolder::hold (CREF<LSDLayout> that) {
	return CFat<LSDHolder> (LSDImplHolder () ,that) ;
}
} ;
