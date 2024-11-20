#pragma once

#ifndef __CSC_BASIC__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_basic.hpp"

#include "csc_end.h"
#include <mutex>
#include "csc_begin.h"

namespace CSC {
class OptionalImplHolder implement Fat<OptionalHolder ,OptionalLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		store (item) ;
	}

	void initialize (CREF<FLAG> code) override {
		fake.mCode = code ;
	}

	BOOL exist () const override {
		return BoxHolder::hold (fake.mValue.self)->exist () ;
	}

	FLAG code () const override {
		return fake.mCode ;
	}

	void fetch (VREF<BoxLayout> item) const override {
		assume (exist ()) ;
		BoxHolder::hold (item)->acquire (fake.mValue.self) ;
		BoxHolder::hold (fake.mValue.self)->release () ;
	}

	void store (VREF<BoxLayout> item) const override {
		assume (!exist ()) ;
		BoxHolder::hold (fake.mValue.self)->acquire (item) ;
		BoxHolder::hold (item)->release () ;
	}
} ;

exports VFat<OptionalHolder> OptionalHolder::hold (VREF<OptionalLayout> that) {
	return VFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

exports CFat<OptionalHolder> OptionalHolder::hold (CREF<OptionalLayout> that) {
	return CFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

struct FunctionImplLayout {
	Pin<BoxLayout> mValue ;
} ;

class FunctionImplHolder implement Fat<FunctionHolder ,FunctionLayout> {
public:
	void initialize (RREF<BoxLayout> item ,CREF<Unknown> holder) override {
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<FunctionImplLayout> () ,r1x ,1) ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (raw ())->release () ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		BoxHolder::hold (item)->release () ;
	}

	void initialize (CREF<FunctionLayout> that) override {
		fake.mThis = that.mThis.share () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return fake.mThis->mValue.self ;
	}

	LENGTH rank () const override {
		if (fake.mThis == NULL)
			return 0 ;
		const auto r1x = RFat<ReflectInvoke> (BoxHolder::hold (raw ())->unknown ()) ;
		return r1x->rank () ;
	}

	void invoke (CREF<WrapperLayout> params) const override {
		if (fake.mThis == NULL)
			return ;
		const auto r1x = RFat<ReflectInvoke> (BoxHolder::hold (raw ())->unknown ()) ;
		return r1x->invoke (BoxHolder::hold (raw ())->self ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::hold (VREF<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

exports CFat<FunctionHolder> FunctionHolder::hold (CREF<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

struct AutoRefImplLayout {
	Clazz mClazz ;
	Pin<BoxLayout> mValue ;
} ;

class AutoRefImplHolder implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		assert (!exist ()) ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<AutoRefImplLayout> () ,holder ,1) ;
		auto &&rax = fake.mThis.leak () ;
		ClazzHolder::hold (rax.mClazz)->initialize (holder) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		const auto r1x = RFat<ReflectCreate> (holder) ;
		r1x->create (BoxHolder::hold (raw ())->self ,1) ;
		fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
	}

	void initialize (RREF<BoxLayout> item ,CREF<Clazz> clazz_) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<AutoRefImplLayout> () ,r1x ,1) ;
		auto &&rax = fake.mThis.leak () ;
		rax.mClazz = clazz_ ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (!BoxHolder::hold (raw ())->exist ())
			return ;
		BoxHolder::hold (raw ())->destroy () ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue.self ;
	}

	Clazz clazz () const override {
		if (!exist ())
			return Clazz () ;
		return fake.mThis->mClazz ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	VREF<Pointer> rebind (CREF<Clazz> clazz_) leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		return Pointer::from (fake) ;
	}

	CREF<Pointer> rebind (CREF<Clazz> clazz_) const leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		return Pointer::from (fake) ;
	}

	AutoRefLayout recast (CREF<ReflectRecast> simple) override {
		AutoRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		ret.mPointer = simple.recast (fake.mPointer) ;
		return move (ret) ;
	}
} ;

exports VFat<AutoRefHolder> AutoRefHolder::hold (VREF<AutoRefLayout> that) {
	return VFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::hold (CREF<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

struct SharedRefMutexLayout {
	FLAG mHandle ;

public:
	implicit SharedRefMutexLayout () noexcept {
		mHandle = ZERO ;
	}
} ;

struct SharedRefMutexHolder implement Interface {
	imports FLAG instance () ;
	imports VFat<SharedRefMutexHolder> hold (VREF<SharedRefMutexLayout> that) ;
	imports CFat<SharedRefMutexHolder> hold (CREF<SharedRefMutexLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void enter () const = 0 ;
	virtual void leave () const = 0 ;
} ;

class SharedRefMutex implement SharedRefMutexLayout {
protected:
	using SharedRefMutexLayout::mHandle ;

public:
	implicit SharedRefMutex () = default ;

	implicit SharedRefMutex (CREF<typeof (NULL)>) {
		mHandle = SharedRefMutexHolder::instance () ;
		SharedRefMutexHolder::hold (thiz)->initialize () ;
	}

	void enter () const {
		return SharedRefMutexHolder::hold (thiz)->enter () ;
	}

	void leave () const {
		return SharedRefMutexHolder::hold (thiz)->leave () ;
	}
} ;

struct SharedRefMutexImplLayout {
	Box<std::recursive_mutex> mMutex ;

public:
	imports CREF<Pin<SharedRefMutexImplLayout>> instance () {
		return memorize ([&] () {
			return Pin<SharedRefMutexImplLayout> () ;
		}) ;
	}
} ;

class SharedRefMutexImplHolder implement Fat<SharedRefMutexHolder ,SharedRefMutexLayout> {
public:
	void initialize () override {
		ptr (fake).mMutex.remake () ;
	}

	imports VREF<SharedRefMutexImplLayout> ptr (CREF<SharedRefMutexLayout> layout) {
		return SharedRefMutexImplLayout::instance ().self ;
	}

	void enter () const override {
		return ptr (fake).mMutex->lock () ;
	}

	void leave () const override {
		return ptr (fake).mMutex->unlock () ;
	}
} ;

exports FLAG SharedRefMutexHolder::instance () {
	return inline_vptr (SharedRefMutexImplHolder ()) ;
}

exports VFat<SharedRefMutexHolder> SharedRefMutexHolder::hold (VREF<SharedRefMutexLayout> that) {
	assert (that.mHandle != ZERO) ;
	auto &&rax = keep[TYPE<SharedRefMutexImplHolder>::expr] (Pointer::from (that.mHandle)) ;
	return VFat<SharedRefMutexHolder> (rax ,that) ;
}

exports CFat<SharedRefMutexHolder> SharedRefMutexHolder::hold (CREF<SharedRefMutexLayout> that) {
	assert (that.mHandle != ZERO) ;
	auto &&rax = keep[TYPE<SharedRefMutexImplHolder>::expr] (Pointer::from (that.mHandle)) ;
	return CFat<SharedRefMutexHolder> (rax ,that) ;
}

struct RefImplLayout ;

struct SharedRefImplLayout {
	SharedRefMutex mMutex ;
	Pin<LENGTH> mCounter ;
	Pin<BoxLayout> mValue ;
} ;

class SharedRefImplHolder implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<SharedRefImplLayout> () ,r1x ,1) ;
		auto &&rax = fake.mThis.leak () ;
		rax.mMutex = SharedRefMutex (NULL) ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		fake.mThis->mCounter.self++ ;
		fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
	}

	void initialize (CREF<SharedRefLayout> that) override {
		assert (!exist ()) ;
		if ifdo (TRUE) {
			if (that.mThis == NULL)
				discard ;
			Scope<SharedRefMutex> anonymous (that.mThis->mMutex) ;
			fake.mThis = that.mThis.share () ;
			fake.mThis->mCounter.self++ ;
			fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
		}
	}

	void initialize (CREF<Unknown> holder ,VREF<Pointer> pointer) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = address (pointer) - inline_alignas (SIZE_OF<SharedRefImplLayout>::expr ,r1x->type_align ()) ;
		auto &&rax = keep[TYPE<SharedRefImplLayout>::expr] (Pointer::make (r2x)) ;
		const auto r3x = BoxHolder::hold (rax.mValue.self)->unknown () ;
		if (r3x != holder)
			return ;
		Scope<SharedRefMutex> anonymous (rax.mMutex) ;
		if (rax.mCounter.self <= 0)
			return ;
		const auto r4x = r2x - inline_alignas (SIZE_OF<RefImplLayout>::expr ,ALIGN_OF<SharedRefImplLayout>::expr) ;
		auto &&rbx = keep[TYPE<RefImplLayout>::expr] (Pointer::make (r4x)) ;
		if (rbx.mCounter.self <= 0)
			return ;
		auto &&rcx = keep[TYPE<RefLayout>::expr] (fake.mThis) ;
		rcx.mHandle = address (rbx) ;
		rcx.mPointer = address (rax) ;
		rbx.mCounter.self++ ;
		fake.mThis->mCounter.self++ ;
		fake.mPointer = address (pointer) ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (!BoxHolder::hold (raw ())->exist ())
			return ;
		Scope<SharedRefMutex> anonymous (fake.mThis->mMutex) ;
		const auto r1x = --fake.mThis->mCounter.self ;
		if (r1x > 0)
			return ;
		BoxHolder::hold (raw ())->destroy () ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue.self ;
	}

	LENGTH counter () const override {
		if (!exist ())
			return 0 ;
		Scope<SharedRefMutex> anonymous (fake.mThis->mMutex) ;
		return fake.mThis->mCounter.self ;
	}

	VREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	SharedRefLayout recast (CREF<ReflectRecast> simple) override {
		SharedRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		ret.mPointer = simple.recast (fake.mPointer) ;
		return move (ret) ;
	}
} ;

exports VFat<SharedRefHolder> SharedRefHolder::hold (VREF<SharedRefLayout> that) {
	return VFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

exports CFat<SharedRefHolder> SharedRefHolder::hold (CREF<SharedRefLayout> that) {
	return CFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

struct UniqueRefImplLayout {
	Function<VREF<Pointer>> mOwner ;
	Pin<BoxLayout> mValue ;
} ;

class UniqueRefImplHolder implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (RREF<BoxLayout> item ,CREF<FunctionLayout> dtor) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<UniqueRefImplLayout> () ,r1x ,1) ;
		auto &&rax = fake.mThis.leak () ;
		rax.mOwner = move (keep[TYPE<Function<VREF<Pointer>>>::expr] (dtor)) ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (!BoxHolder::hold (raw ())->exist ())
			return ;
		fake.mThis->mOwner (BoxHolder::hold (raw ())->self) ;
		BoxHolder::hold (raw ())->destroy () ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	UniqueRefLayout recast (CREF<ReflectRecast> simple) override {
		UniqueRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		ret.mPointer = simple.recast (fake.mPointer) ;
		return move (ret) ;
	}
} ;

exports VFat<UniqueRefHolder> UniqueRefHolder::hold (VREF<UniqueRefLayout> that) {
	return VFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::hold (CREF<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

struct RefBufferImplLayout {
	LENGTH mWidth ;
	Pin<BoxLayout> mValue ;
} ;

class RefBufferImplHolder implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		if (exist ())
			return ;
		fake.mHolder = inline_vptr (holder) ;
		fake.mBuffer = ZERO ;
		fake.mSize = 0 ;
		fake.mStep = 0 ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		fake.mHolder = inline_vptr (holder) ;
		const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r1x ,size_) ;
		BoxHolder::hold (raw ())->initialize (r1x) ;
		fake.mBuffer = address (BoxHolder::hold (raw ())->self) ;
		fake.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (r1x) ;
		fake.mStep = r2x->type_size () ;
		const auto r3x = RFat<ReflectCreate> (r1x) ;
		r3x->create (self ,size_) ;
		auto &&rax = fake.mThis.leak () ;
		rax.mWidth = size_ ;
	}

	void initialize (CREF<Unknown> holder ,RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		fake.mHolder = inline_vptr (holder) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r1x ,1) ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		auto &&rax = fake.mThis.leak () ;
		rax.mWidth = USED ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (fake.mThis == NULL)
			return ;
		if (fake.mThis->mWidth == USED)
			return ;
		const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
		const auto r2x = RFat<ReflectDestroy> (r1x) ;
		r2x->destroy (self ,fake.mThis->mWidth) ;
		BoxHolder::hold (raw ())->release () ;
	}

	BOOL exist () const override {
		return fake.mHolder != ZERO ;
	}

	Unknown unknown () const override {
		assert (exist ()) ;
		return Unknown (fake.mHolder) ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue.self ;
	}

	LENGTH size () const override {
		if (!exist ())
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if (!exist ())
			return 0 ;
		return fake.mStep ;
	}

	VREF<Pointer> self_m () leftvalue override {
		return Pointer::make (fake.mBuffer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		return Pointer::make (fake.mBuffer) ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	void resize (CREF<LENGTH> size_) override {
		const auto r1x = size () ;
		if (size_ <= r1x)
			return ;
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			assert (fake.mThis->mWidth != USED) ;
		}
		auto rax = Ref<RefBufferImplLayout> () ;
		const auto r2x = RFat<ReflectElement> (unknown ())->element () ;
		RefHolder::hold (rax)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r2x ,size_) ;
		const auto r3x = RFat<ReflectSize> (r2x) ;
		const auto r4x = r3x->type_size () * r1x ;
		auto &&rbx = rax.leak () ;
		BoxHolder::hold (rbx.mValue.self)->initialize (r2x) ;
		const auto r5x = address (BoxHolder::hold (rbx.mValue.self)->self) ;
		inline_memcpy (Pointer::make (r5x) ,self ,r4x) ;
		const auto r6x = r5x + r4x ;
		const auto r7x = RFat<ReflectCreate> (r2x) ;
		r7x->create (Pointer::make (r6x) ,size_ - r1x) ;
		rbx.mWidth = size_ ;
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			BoxHolder::hold (raw ())->release () ;
		}
		swap (fake.mThis ,rax) ;
		fake.mBuffer = r5x ;
		fake.mSize = size_ ;
		fake.mStep = r3x->type_size () ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::hold (VREF<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::hold (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

class FarBufferImplHolder implement Fat<FarBufferHolder ,FarBufferLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectElement> (holder)->element () ;
		RefHolder::hold (fake.mThis)->initialize (r1x ,r1x ,0) ;
		fake.mBuffer = address (fake.mThis.self) ;
		fake.mIndex = NONE ;
		fake.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (holder) ;
		fake.mStep = r2x->type_size () ;
	}

	void use_getter (CREF<FunctionLayout> getter) override {
		fake.mGetter = move (keep[TYPE<Function<CREF<INDEX> ,VREF<Pointer>>>::expr] (getter)) ;
	}

	void use_setter (CREF<FunctionLayout> setter) override {
		fake.mSetter = move (keep[TYPE<Function<CREF<INDEX> ,CREF<Pointer>>>::expr] (setter)) ;
	}

	BOOL exist () const override {
		return fake.mThis.exist () ;
	}

	Unknown unknown () const override {
		return fake.mThis.unknown () ;
	}

	LENGTH size () const override {
		if (!exist ())
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if (!exist ())
			return 0 ;
		return fake.mStep ;
	}

	VREF<Pointer> self_m () leftvalue {
		return Pointer::make (fake.mBuffer) ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		update_sync (index) ;
		return self ;
	}

	void update_sync (CREF<INDEX> index) {
		if (fake.mIndex == index)
			return ;
		refresh () ;
		fake.mGetter (index ,self) ;
		fake.mIndex = index ;
	}

	void refresh () override {
		if (fake.mIndex == NONE)
			return ;
		fake.mSetter (fake.mIndex ,self) ;
		fake.mIndex = NONE ;
	}
} ;

exports VFat<FarBufferHolder> FarBufferHolder::hold (VREF<FarBufferLayout> that) {
	return VFat<FarBufferHolder> (FarBufferImplHolder () ,that) ;
}

exports CFat<FarBufferHolder> FarBufferHolder::hold (CREF<FarBufferLayout> that) {
	return CFat<FarBufferHolder> (FarBufferImplHolder () ,that) ;
}

class AllocatorImplHolder implement Fat<AllocatorHolder ,AllocatorLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		if (exist ())
			return ;
		RefBufferHolder::hold (fake.mAllocator)->initialize (holder) ;
		const auto r1x = RFat<ReflectTuple> (holder) ;
		fake.mOffset = r1x->tuple_m2nd () ;
		fake.mRest = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		RefBufferHolder::hold (fake.mAllocator)->initialize (holder ,size_) ;
		const auto r1x = RFat<ReflectTuple> (holder) ;
		fake.mOffset = r1x->tuple_m2nd () ;
		fake.mRest = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		const auto r1x = inline_min (fake.mRest ,size ()) ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			if (xbt (fake ,i).mNext != USED)
				continue ;
			r2x->destroy (fake.mAllocator.at (i) ,1) ;
		}
	}

	BOOL exist () const override {
		return fake.mAllocator.exist () ;
	}

	Unknown unknown () const override {
		return fake.mAllocator.unknown () ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mAllocator.raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mAllocator.raw () ;
	}

	void clear () override {
		destroy () ;
		fake.mRest = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}

	LENGTH size () const override {
		return fake.mAllocator.size () ;
	}

	LENGTH step () const override {
		return fake.mAllocator.step () ;
	}

	LENGTH length () const override {
		if (!exist ())
			return 0 ;
		return fake.mLength ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (used (index)) ;
		return fake.mAllocator.at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (used (index)) ;
		return fake.mAllocator.at (index) ;
	}

	VREF<Pointer> bt (CREF<INDEX> index) leftvalue override {
		assert (used (index)) ;
		return Pointer::from (xbt (fake ,index)) ;
	}

	CREF<Pointer> bt (CREF<INDEX> index) const leftvalue override {
		assert (used (index)) ;
		return Pointer::from (xbt (fake ,index)) ;
	}

	imports VREF<AllocatorNode> xbt (VREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		const auto r1x = address (layout.mAllocator.at (index)) + layout.mOffset ;
		return Pointer::make (r1x) ;
	}

	imports CREF<AllocatorNode> xbt (CREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		const auto r1x = address (layout.mAllocator.at (index)) + layout.mOffset ;
		return Pointer::make (r1x) ;
	}

	INDEX alloc () override {
		check_resize () ;
		INDEX ret = fake.mFree ;
		fake.mFree = xbt (fake ,ret).mNext ;
		const auto r1x = RFat<ReflectCreate> (unknown ()) ;
		r1x->create (fake.mAllocator.at (ret) ,1) ;
		xbt (fake ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	INDEX alloc (RREF<BoxLayout> item) override {
		check_resize () ;
		INDEX ret = fake.mFree ;
		fake.mFree = xbt (fake ,ret).mNext ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_size () ;
		inline_memcpy (fake.mAllocator.at (ret) ,BoxHolder::hold (item)->self ,r2x) ;
		inline_memset (BoxHolder::hold (item)->self ,r2x) ;
		xbt (fake ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	void free (CREF<INDEX> index) override {
		const auto r1x = index ;
		assert (used (r1x)) ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		r2x->destroy (fake.mAllocator.at (r1x) ,1) ;
		xbt (fake ,r1x).mNext = fake.mFree ;
		fake.mFree = r1x ;
		fake.mLength-- ;
	}

	BOOL used (CREF<INDEX> index) const override {
		if (!inline_between (index ,0 ,fake.mRest))
			return FALSE ;
		const auto r1x = xbt (fake ,index).mNext ;
		return r1x == USED ;
	}

	void resize (CREF<LENGTH> size_) override {
		RefBufferHolder::hold (fake.mAllocator)->resize (size_) ;
	}

	void check_resize () {
		assert (exist ()) ;
		if ifdo (TRUE) {
			if (fake.mFree != NONE)
				discard ;
			if ifdo (TRUE) {
				if (fake.mRest < size ())
					discard ;
				const auto r1x = inline_max (fake.mRest * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
				resize (r1x) ;
			}
			fake.mFree = fake.mRest ;
			xbt (fake ,fake.mFree).mNext = NONE ;
			fake.mRest++ ;
		}
	}
} ;

exports VFat<AllocatorHolder> AllocatorHolder::hold (VREF<AllocatorLayout> that) {
	return VFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}

exports CFat<AllocatorHolder> AllocatorHolder::hold (CREF<AllocatorLayout> that) {
	return CFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}
} ;