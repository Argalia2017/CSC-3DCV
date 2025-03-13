#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
struct HeapMutexLayout {
	FLAG mHolder ;

public:
	implicit HeapMutexLayout () noexcept {
		mHolder = ZERO ;
	}
} ;

struct HeapMutexHolder implement Interface {
	imports CREF<HeapMutexLayout> instance () ;
	imports VFat<HeapMutexHolder> hold (VREF<HeapMutexLayout> that) ;
	imports CFat<HeapMutexHolder> hold (CREF<HeapMutexLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void enter () const = 0 ;
	virtual void leave () const = 0 ;
} ;

class HeapMutex implement HeapMutexLayout {
protected:
	using HeapMutexLayout::mHolder ;

public:
	static CREF<HeapMutex> instance () {
		return keep[TYPE<HeapMutex>::expr] (HeapMutexHolder::instance ()) ;
	}

	void enter () const {
		return HeapMutexHolder::hold (thiz)->enter () ;
	}

	void leave () const {
		return HeapMutexHolder::hold (thiz)->leave () ;
	}
} ;

struct OptionalLayout {
	FLAG mCode ;
	Pin<BoxLayout> mValue ;

public:
	implicit OptionalLayout () noexcept {
		mCode = 0 ;
	}

	implicit OptionalLayout (CREF<OptionalLayout> that) = delete ;

	forceinline VREF<OptionalLayout> operator= (CREF<OptionalLayout> that) = delete ;

	implicit OptionalLayout (RREF<OptionalLayout> that) noexcept :OptionalLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<OptionalLayout> operator= (RREF<OptionalLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct OptionalHolder implement Interface {
	imports VFat<OptionalHolder> hold (VREF<OptionalLayout> that) ;
	imports CFat<OptionalHolder> hold (CREF<OptionalLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<FLAG> code) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual FLAG code () const = 0 ;
	virtual void get (VREF<BoxLayout> item) const = 0 ;
	virtual void set (VREF<BoxLayout> item) const = 0 ;
} ;

template <class A>
class Optional implement OptionalLayout {
protected:
	using OptionalLayout::mCode ;
	using OptionalLayout::mValue ;
	Union<A> mStorage ;

public:
	implicit Optional () = default ;

	implicit Optional (CREF<A> that) :Optional (move (that)) {}

	implicit Optional (RREF<A> that) {
		auto rax = Box<A>::make (move (that)) ;
		OptionalHolder::hold (thiz)->initialize (move (rax)) ;
	}

	static Optional error (CREF<FLAG> code) {
		Optional ret ;
		OptionalHolder::hold (ret)->initialize (code) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return OptionalHolder::hold (thiz)->exist () ;
	}

	FLAG code () const {
		return OptionalHolder::hold (thiz)->code () ;
	}

	A fetch () const {
		auto rax = Box<A> () ;
		OptionalHolder::hold (thiz)->get (rax) ;
		return move (rax.self) ;
	}

	forceinline operator A () const {
		return fetch () ;
	}

	void once (CREF<A> item) const {
		once (move (item)) ;
	}

	void once (RREF<A> item) const {
		if (exist ())
			return ;
		auto rax = Box<A>::make (move (item)) ;
		OptionalHolder::hold (thiz)->set (rax) ;
	}

	template <class ARG1>
	forceinline CREF<Optional> operator() (RREF<ARG1> item) const {
		once (move (item)) ;
		return thiz ;
	}

	template <class ARG1>
	void then (CREF<ARG1> func) const {
		if (!exist ())
			return ;
		auto rax = Box<A> () ;
		OptionalHolder::hold (thiz)->get (rax) ;
		func (rax.self) ;
		OptionalHolder::hold (thiz)->set (rax) ;
	}
} ;

struct BufferLayout {} ;

template <class A ,class B>
class Buffer implement BufferLayout {
private:
	using BUFFER = Tuple<CONDITIONAL<ENUM_GT_ZERO<B> ,ARR<A ,B> ,Storage<RANK1 ,ALIGN_OF<A>>>> ;

protected:
	BUFFER mBuffer ;

public:
	implicit Buffer () = default ;

	template <class...ARG1 ,class = REQUIRE<ENUM_EQUAL<RANK_OF<TYPE<ARG1...>> ,B>>>
	implicit Buffer (XREF<ARG1>...initval) {
		auto &&rax = keep[TYPE<BUFFER>::expr] (Pointer::from (mBuffer)) ;
		rax = BUFFER {A (initval)...} ;
	}

	LENGTH size () const {
		return B::expr ;
	}

	LENGTH step () const {
		return SIZE_OF<A>::expr ;
	}

	VREF<ARR<A>> self_m () leftvalue {
		return Pointer::from (mBuffer) ;
	}

	forceinline operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return Pointer::from (mBuffer) ;
	}

	forceinline operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	forceinline BOOL operator== (CREF<Buffer> that) = delete ;

	forceinline BOOL operator!= (CREF<Buffer> that) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		return self[index] ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		return self[index] ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}
} ;

template <class A>
using Buffer1 = Buffer<A ,RANK1> ;
template <class A>
using Buffer2 = Buffer<A ,RANK2> ;
template <class A>
using Buffer3 = Buffer<A ,RANK3> ;
template <class A>
using Buffer4 = Buffer<A ,RANK4> ;
template <class A>
using Buffer5 = Buffer<A ,RANK5> ;
template <class A>
using Buffer6 = Buffer<A ,RANK6> ;
template <class A>
using Buffer7 = Buffer<A ,RANK7> ;
template <class A>
using Buffer8 = Buffer<A ,RANK8> ;
template <class A>
using Buffer9 = Buffer<A ,RANK9> ;
template <class A>
using BufferX = Buffer<A ,RANKX> ;

struct WrapperLayout {
	LENGTH mRank ;
} ;

template <class...A>
class Wrapper implement WrapperLayout {
private:
	using RANK = RANK_OF<TYPE<A...>> ;
	using ITEM = REMOVE_CVR<TYPE_M1ST_ITEM<TYPE<A... ,Pointer>>> ;

protected:
	using WrapperLayout::mRank ;
	Buffer<FLAG ,RANK> mWrapper ;

public:
	implicit Wrapper () = delete ;

	explicit Wrapper (CREF<Buffer<FLAG ,RANK>> that) {
		mRank = RANK::expr ;
		mWrapper = that ;
	}

	LENGTH rank () const {
		return mRank ;
	}

	CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
		assert (inline_between (index ,0 ,mRank)) ;
		return Pointer::make (mWrapper.self[index]) ;
	}

	forceinline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	template <class ARG1>
	void invoke (CREF<ARG1> func) const {
		return invoke_impl (func ,TYPE_SENQUENCE<RANK>::expr) ;
	}

	template <class ARG1 ,class...ARG2>
	forceinline void invoke_impl (CREF<ARG1> func ,TYPE<ARG2...>) const {
		//@fatal: GCC is so bad
		return func (keep[TYPE<XREF<A>>::expr] (Pointer::make (mWrapper.self[ARG2::expr]))...) ;
	}

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> func) const {
		return invoke (func) ;
	}
} ;

template <class...ARG1>
inline Wrapper<CREF<ARG1>...> MakeWrapper (CREF<ARG1>...params) {
	using R1X = RANK_OF<TYPE<ARG1...>> ;
	return Wrapper<CREF<ARG1>...> (Buffer<FLAG ,R1X> ({address (params)...})) ;
}

struct ReflectInvoke implement Interface {
	virtual LENGTH rank () const = 0 ;
	virtual void invoke (CREF<Pointer> func ,CREF<WrapperLayout> params) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 200 ;
	}
} ;

template <class A>
class ReflectInvokeBinder implement ReflectInvoke {
public:
	LENGTH rank () const override {
		return RANK_OF<FUNCTION_PARAMS<A>>::expr ;
	}

	void invoke (CREF<Pointer> func ,CREF<WrapperLayout> params) const override {
		assert (params.mRank == rank ()) ;
		auto &&rax = wrapper_from (FUNCTION_PARAMS<A>::expr ,params) ;
		return rax (keep[TYPE<A>::expr] (func)) ;
	}

	template <class...ARG1>
	static CREF<Wrapper<ARG1...>> wrapper_from (TYPE<ARG1...> ,CREF<WrapperLayout> params) {
		return Pointer::from (params) ;
	}
} ;

struct FunctionImplLayout ;

struct FunctionLayout {
	Ref<FunctionImplLayout> mThis ;
} ;

struct FunctionHolder implement Interface {
	imports VFat<FunctionHolder> hold (VREF<FunctionLayout> that) ;
	imports CFat<FunctionHolder> hold (CREF<FunctionLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item ,CREF<Unknown> holder) = 0 ;
	virtual LENGTH rank () const = 0 ;
	virtual void invoke (CREF<WrapperLayout> params) const = 0 ;
} ;

template <class A>
class FunctionUnknownBinder implement ReflectUnknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectInvokeBinder<A>::expr)
			return inline_vptr (ReflectInvokeBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class...A>
class Function implement FunctionLayout {
protected:
	using FunctionLayout::mThis ;

public:
	implicit Function () = default ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_EXTEND<FunctionLayout ,ARG1>>>>
	implicit Function (RREF<ARG1> that) {
		using R1X = FUNCTION_RETURN<ARG1> ;
		using R2X = FUNCTION_PARAMS<ARG1> ;
		require (IS_VOID<R1X>) ;
		require (IS_SAME<R2X ,TYPE<A...>>) ;
		auto rax = Box<ARG1>::make (move (that)) ;
		FunctionHolder::hold (thiz)->initialize (move (rax) ,FunctionUnknownBinder<ARG1> ()) ;
	}

	LENGTH rank () const {
		return FunctionHolder::hold (thiz)->rank () ;
	}

	void invoke (XREF<A>...params) const {
		return FunctionHolder::hold (thiz)->invoke (MakeWrapper (params...)) ;
	}

	forceinline void operator() (XREF<A>...params) const {
		return invoke (keep[TYPE<XREF<A>>::expr] (params)...) ;
	}
} ;

struct ReflectRecast implement Interface {
	virtual FLAG recast (CREF<FLAG> layout) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 201 ;
	}
} ;

template <class A ,class B>
class ReflectRecastBinder implement ReflectRecast {
public:
	FLAG recast (CREF<FLAG> layout) const override {
		auto &&rax = keep[TYPE<B>::expr] (Pointer::make (layout)) ;
		return recast_impl (PHX ,TYPE<A>::expr ,rax) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_EXTEND<ARG1 ,ARG2>>>
	forceinline FLAG recast_impl (CREF<typeof (PH3)> ,TYPE<ARG1> ,CREF<ARG2> b) const {
		return FLAG (PTR<CREF<ARG1>> (&b)) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_VIRTUAL<ARG1 ,ARG2>>>
	forceinline FLAG recast_impl (CREF<typeof (PH2)> ,TYPE<ARG1> ,CREF<ARG2> b) const {
		return address (b.self) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_SAME<ARG1 ,Pointer>>>
	forceinline FLAG recast_impl (CREF<typeof (PH1)> ,TYPE<ARG1> ,CREF<ARG2> b) const {
		return address (b) ;
	}
} ;

struct AutoRefImplLayout ;

struct AutoRefLayout {
	Ref<AutoRefImplLayout> mThis ;
	FLAG mLayout ;

public:
	implicit AutoRefLayout () noexcept {
		mLayout = ZERO ;
	}

	implicit ~AutoRefLayout () noexcept ;

	implicit AutoRefLayout (CREF<AutoRefLayout> that) = delete ;

	forceinline VREF<AutoRefLayout> operator= (CREF<AutoRefLayout> that) = delete ;

	implicit AutoRefLayout (RREF<AutoRefLayout> that) noexcept :AutoRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<AutoRefLayout> operator= (RREF<AutoRefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct AutoRefHolder implement Interface {
	imports VFat<AutoRefHolder> hold (VREF<AutoRefLayout> that) ;
	imports CFat<AutoRefHolder> hold (CREF<AutoRefLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (RREF<BoxLayout> item ,CREF<Clazz> clazz_) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual VREF<Pointer> rebind (CREF<Clazz> clazz_) leftvalue = 0 ;
	virtual CREF<Pointer> rebind (CREF<Clazz> clazz_) const leftvalue = 0 ;
	virtual AutoRefLayout recast (CREF<Unknown> simple) = 0 ;
} ;

inline AutoRefLayout::~AutoRefLayout () noexcept {
	AutoRefHolder::hold (thiz)->destroy () ;
}

template <class A>
class AutoRef implement AutoRefLayout {
protected:
	using AutoRefLayout::mThis ;
	using AutoRefLayout::mLayout ;

public:
	implicit AutoRef () = default ;

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,A>>>
	implicit AutoRef (RREF<AutoRef<ARG1>> that) :AutoRef (that.recast (TYPE<A>::expr)) {}

	explicit AutoRef (CREF<Unknown> holder) {
		require (IS_SAME<A ,Pointer>) ;
		AutoRefHolder::hold (thiz)->initialize (holder) ;
	}

	template <class...ARG1>
	static AutoRef make (XREF<ARG1>...initval) {
		AutoRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		AutoRefHolder::hold (ret)->initialize (move (rax) ,Clazz (TYPE<A>::expr)) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return AutoRefHolder::hold (thiz)->exist () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return AutoRefHolder::hold (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return AutoRefHolder::hold (thiz)->raw () ;
	}

	Clazz clazz () const {
		return AutoRefHolder::hold (thiz)->clazz () ;
	}

	VREF<A> self_m () leftvalue {
		return AutoRefHolder::hold (thiz)->self ;
	}

	forceinline operator VREF<A> () leftvalue {
		return self ;
	}

	forceinline PTR<VREF<A>> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return AutoRefHolder::hold (thiz)->self ;
	}

	forceinline operator CREF<A> () const leftvalue {
		return self ;
	}

	forceinline PTR<CREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	forceinline BOOL operator== (CREF<AutoRef> that) = delete ;

	forceinline BOOL operator!= (CREF<AutoRef> that) = delete ;

	template <class ARG1>
	VREF<AutoRef<ARG1>> rebind (TYPE<ARG1>) leftvalue {
		return AutoRefHolder::hold (thiz)->rebind (Clazz (TYPE<ARG1>::expr)) ;
	}

	template <class ARG1>
	CREF<AutoRef<ARG1>> rebind (TYPE<ARG1>) const leftvalue {
		return AutoRefHolder::hold (thiz)->rebind (Clazz (TYPE<ARG1>::expr)) ;
	}

	template <class ARG1>
	AutoRef<ARG1> recast (TYPE<ARG1>) {
		using R1X = SimpleUnknownBinder<ReflectRecastBinder<ARG1 ,A>> ;
		AutoRefLayout ret = AutoRefHolder::hold (thiz)->recast (R1X ()) ;
		return move (keep[TYPE<AutoRef<ARG1>>::expr] (ret)) ;
	}
} ;

struct SharedRefImplLayout ;

struct SharedRefLayout {
	Ref<SharedRefImplLayout> mThis ;
	FLAG mLayout ;

public:
	implicit SharedRefLayout () noexcept {
		mLayout = ZERO ;
	}

	implicit ~SharedRefLayout () noexcept ;

	implicit SharedRefLayout (CREF<SharedRefLayout> that) = delete ;

	forceinline VREF<SharedRefLayout> operator= (CREF<SharedRefLayout> that) = delete ;

	implicit SharedRefLayout (RREF<SharedRefLayout> that) noexcept :SharedRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<SharedRefLayout> operator= (RREF<SharedRefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct SharedRefHolder implement Interface {
	imports VFat<SharedRefHolder> hold (VREF<SharedRefLayout> that) ;
	imports CFat<SharedRefHolder> hold (CREF<SharedRefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<SharedRefLayout> that) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<FLAG> layout) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual LENGTH counter () const = 0 ;
	virtual VREF<Pointer> self_m () const leftvalue = 0 ;
	virtual SharedRefLayout recast (CREF<Unknown> simple) = 0 ;
} ;

inline SharedRefLayout::~SharedRefLayout () noexcept {
	SharedRefHolder::hold (thiz)->destroy () ;
}

template <class A>
class SharedRef implement SharedRefLayout {
protected:
	using SharedRefLayout::mThis ;
	using SharedRefLayout::mLayout ;

public:
	implicit SharedRef () = default ;

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,A>>>
	implicit SharedRef (RREF<SharedRef<ARG1>> that) :SharedRef (that.recast (TYPE<A>::expr)) {}

	template <class...ARG1>
	static SharedRef make (XREF<ARG1>...initval) {
		SharedRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		SharedRefHolder::hold (ret)->initialize (move (rax)) ;
		return move (ret) ;
	}

	static SharedRef intrusive (VREF<A> that) {
		SharedRef ret ;
		SharedRefHolder::hold (ret)->initialize (BoxUnknownBinder<A> () ,address (that)) ;
		return move (ret) ;
	}

	implicit SharedRef (CREF<SharedRef> that) {
		SharedRefHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<SharedRef> operator= (CREF<SharedRef> that) {
		return assign (thiz ,that) ;
	}

	implicit SharedRef (RREF<SharedRef> that) = default ;

	forceinline VREF<SharedRef> operator= (RREF<SharedRef> that) = default ;

	BOOL exist () const {
		return SharedRefHolder::hold (thiz)->exist () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return SharedRefHolder::hold (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return SharedRefHolder::hold (thiz)->raw () ;
	}

	LENGTH counter () const {
		return SharedRefHolder::hold (thiz)->counter () ;
	}

	VREF<A> self_m () const leftvalue {
		return SharedRefHolder::hold (thiz)->self ;
	}

	forceinline operator VREF<A> () const leftvalue {
		return self ;
	}

	forceinline PTR<VREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	forceinline BOOL operator== (CREF<SharedRef> that) = delete ;

	forceinline BOOL operator!= (CREF<SharedRef> that) = delete ;

	template <class ARG1>
	SharedRef<ARG1> recast (TYPE<ARG1>) {
		using R1X = SimpleUnknownBinder<ReflectRecastBinder<ARG1 ,A>> ;
		SharedRefLayout ret = SharedRefHolder::hold (thiz)->recast (R1X ()) ;
		return move (keep[TYPE<SharedRef<ARG1>>::expr] (ret)) ;
	}
} ;

struct UniqueRefImplLayout ;

struct UniqueRefLayout {
	Ref<UniqueRefImplLayout> mThis ;
	FLAG mLayout ;

public:
	implicit UniqueRefLayout () noexcept {
		mLayout = ZERO ;
	}

	implicit ~UniqueRefLayout () noexcept ;

	implicit UniqueRefLayout (CREF<UniqueRefLayout> that) = delete ;

	forceinline VREF<UniqueRefLayout> operator= (CREF<UniqueRefLayout> that) = delete ;

	implicit UniqueRefLayout (RREF<UniqueRefLayout> that) noexcept :UniqueRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<UniqueRefLayout> operator= (RREF<UniqueRefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct UniqueRefHolder implement Interface {
	imports VFat<UniqueRefHolder> hold (VREF<UniqueRefLayout> that) ;
	imports CFat<UniqueRefHolder> hold (CREF<UniqueRefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void destroy () = 0 ;
	virtual void use_owner (CREF<Function<VREF<Pointer>>> owner) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual void depend (VREF<UniqueRefLayout> that) const = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual UniqueRefLayout recast (CREF<Unknown> simple) = 0 ;
} ;

inline UniqueRefLayout::~UniqueRefLayout () noexcept {
	UniqueRefHolder::hold (thiz)->destroy () ;
}

template <class A>
class UniqueRef implement UniqueRefLayout {
protected:
	using UniqueRefLayout::mThis ;
	using UniqueRefLayout::mLayout ;

public:
	implicit UniqueRef () = default ;

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,A>>>
	implicit UniqueRef (RREF<UniqueRef<ARG1>> that) :UniqueRef (that.recast (TYPE<A>::expr)) {}

	template <class ARG1 ,class ARG2>
	explicit UniqueRef (RREF<ARG1> ctor ,RREF<ARG2> dtor) {
		auto rax = Box<A>::make () ;
		auto rbx = Function<VREF<A>> (move (dtor)) ;
		UniqueRefHolder::hold (thiz)->initialize (move (rax)) ;
		ctor (BoxHolder::hold (raw ())->self) ;
		UniqueRefHolder::hold (thiz)->use_owner (Pointer::from (rbx)) ;
	}

	template <class...ARG1>
	static UniqueRef make (XREF<ARG1>...initval) {
		UniqueRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		UniqueRefHolder::hold (ret)->initialize (move (rax)) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return UniqueRefHolder::hold (thiz)->exist () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return UniqueRefHolder::hold (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return UniqueRefHolder::hold (thiz)->raw () ;
	}

	CREF<A> self_m () const leftvalue {
		return UniqueRefHolder::hold (thiz)->self ;
	}

	forceinline operator CREF<A> () const leftvalue {
		return self ;
	}

	forceinline PTR<CREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	forceinline BOOL operator== (CREF<UniqueRef> that) = delete ;

	forceinline BOOL operator!= (CREF<UniqueRef> that) = delete ;

	void depend (VREF<UniqueRefLayout> that) const {
		return UniqueRefHolder::hold (thiz)->depend (that) ;
	}

	template <class ARG1>
	UniqueRef<ARG1> recast (TYPE<ARG1>) {
		using R1X = SimpleUnknownBinder<ReflectRecastBinder<ARG1 ,A>> ;
		UniqueRefLayout ret = UniqueRefHolder::hold (thiz)->recast (R1X ()) ;
		return move (keep[TYPE<UniqueRef<ARG1>>::expr] (ret)) ;
	}
} ;

struct ReflectElement implement Interface {
	virtual Unknown element () const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 202 ;
	}
} ;

template <class A>
class ReflectElementBinder implement ReflectElement {
public:
	Unknown element () const override {
		return RefUnknownBinder<A> () ;
	}
} ;

struct RefBufferImplLayout ;

struct RefBufferLayout {
	Ref<RefBufferImplLayout> mThis ;
	FLAG mHolder ;
	FLAG mBuffer ;
	LENGTH mSize ;
	LENGTH mStep ;

public:
	implicit RefBufferLayout () noexcept {
		mHolder = ZERO ;
		mBuffer = ZERO ;
		mSize = 0 ;
		mStep = 0 ;
	}

	implicit ~RefBufferLayout () noexcept ;

	implicit RefBufferLayout (CREF<RefBufferLayout> that) = delete ;

	forceinline VREF<RefBufferLayout> operator= (CREF<RefBufferLayout> that) = delete ;

	implicit RefBufferLayout (RREF<RefBufferLayout> that) noexcept :RefBufferLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<RefBufferLayout> operator= (RREF<RefBufferLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct RefBufferHolder implement Interface {
	imports VFat<RefBufferHolder> hold (VREF<RefBufferLayout> that) ;
	imports CFat<RefBufferHolder> hold (CREF<RefBufferLayout> that) ;

	virtual void prepare (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<SliceLayout> buffer) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<SliceLayout> buffer ,RREF<BoxLayout> item) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual BOOL fixed () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
} ;

inline RefBufferLayout::~RefBufferLayout () noexcept {
	RefBufferHolder::hold (thiz)->destroy () ;
}

template <class A>
class BufferUnknownBinder implement ReflectUnknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_vptr (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class RefBufferRealLayout implement RefBufferLayout {
public:
	implicit RefBufferRealLayout () noexcept {
		noop (PTR<VREF<A>> (NULL)) ;
	}
} ;

template <class A>
class RefBuffer implement RefBufferRealLayout<A> {
protected:
	using RefBufferRealLayout<A>::mThis ;
	using RefBufferRealLayout<A>::mHolder ;
	using RefBufferRealLayout<A>::mBuffer ;
	using RefBufferRealLayout<A>::mSize ;
	using RefBufferRealLayout<A>::mStep ;

public:
	implicit RefBuffer () = default ;

	explicit RefBuffer (CREF<LENGTH> size_) {
		RefBufferHolder::hold (thiz)->initialize (BufferUnknownBinder<A> () ,size_) ;
	}

	static RefBuffer reference (CREF<FLAG> buffer ,CREF<LENGTH> size_) {
		RefBuffer ret ;
		const auto r1x = Slice (buffer ,size_ ,SIZE_OF<A>::expr) ;
		RefBufferHolder::hold (ret)->initialize (BufferUnknownBinder<A> () ,r1x) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return RefBufferHolder::hold (thiz)->exist () ;
	}

	BOOL fixed () const {
		return RefBufferHolder::hold (thiz)->fixed () ;
	}

	Unknown unknown () const {
		return RefBufferHolder::hold (thiz)->unknown () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return RefBufferHolder::hold (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return RefBufferHolder::hold (thiz)->raw () ;
	}

	LENGTH size () const {
		return RefBufferHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return RefBufferHolder::hold (thiz)->step () ;
	}

	VREF<ARR<A>> self_m () leftvalue {
		return RefBufferHolder::hold (thiz)->self ;
	}

	forceinline operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return RefBufferHolder::hold (thiz)->self ;
	}

	forceinline operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	forceinline BOOL operator== (CREF<RefBuffer> that) = delete ;

	forceinline BOOL operator!= (CREF<RefBuffer> that) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return RefBufferHolder::hold (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return RefBufferHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		RefBufferHolder::hold (thiz)->prepare (BufferUnknownBinder<A> ()) ;
		return RefBufferHolder::hold (thiz)->resize (size_) ;
	}
} ;

struct FarBufferLayout {
	Ref<Pointer> mThis ;
	Function<CREF<INDEX> ,VREF<Pointer>> mGetter ;
	Function<CREF<INDEX> ,CREF<Pointer>> mSetter ;
	LENGTH mSize ;
	LENGTH mStep ;
	INDEX mIndex ;
} ;

struct FarBufferHolder implement Interface {
	imports VFat<FarBufferHolder> hold (VREF<FarBufferLayout> that) ;
	imports CFat<FarBufferHolder> hold (CREF<FarBufferLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual void use_getter (CREF<Function<CREF<INDEX> ,VREF<Pointer>>> getter) = 0 ;
	virtual void use_setter (CREF<Function<CREF<INDEX> ,CREF<Pointer>>> setter) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual void refresh () = 0 ;
} ;

template <class A>
class FarBufferRealLayout implement FarBufferLayout {
public:
	implicit FarBufferRealLayout () noexcept {
		noop (PTR<VREF<A>> (NULL)) ;
	}
} ;

template <class A>
class FarBuffer implement FarBufferRealLayout<A> {
protected:
	using FarBufferRealLayout<A>::mThis ;
	using FarBufferRealLayout<A>::mGetter ;
	using FarBufferRealLayout<A>::mSetter ;
	using FarBufferRealLayout<A>::mSize ;
	using FarBufferRealLayout<A>::mStep ;
	using FarBufferRealLayout<A>::mIndex ;

public:
	implicit FarBuffer () = default ;

	explicit FarBuffer (CREF<LENGTH> size_) {
		FarBufferHolder::hold (thiz)->initialize (BufferUnknownBinder<A> () ,size_) ;
	}

	void use_getter (CREF<Function<CREF<INDEX> ,VREF<A>>> getter) {
		return FarBufferHolder::hold (thiz)->use_getter (Pointer::from (getter)) ;
	}

	void use_setter (CREF<Function<CREF<INDEX> ,CREF<A>>> setter) {
		return FarBufferHolder::hold (thiz)->use_setter (Pointer::from (setter)) ;
	}

	BOOL exist () const {
		return FarBufferHolder::hold (thiz)->exist () ;
	}

	Unknown unknown () const {
		return FarBufferHolder::hold (thiz)->unknown () ;
	}

	LENGTH size () const {
		return FarBufferHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return FarBufferHolder::hold (thiz)->step () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return FarBufferHolder::hold (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	void refresh () {
		return FarBufferHolder::hold (thiz)->refresh () ;
	}
} ;

template <class A ,class B>
struct TupleNode implement Tuple<Union<A> ,B> {} ;

struct AllocatorNode {
	INDEX mNext ;
} ;

struct AllocatorLayout {
	RefBuffer<Pointer> mAllocator ;
	LENGTH mOffset ;
	LENGTH mWidth ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	implicit AllocatorLayout () noexcept {
		mOffset = 0 ;
		mWidth = 0 ;
		mLength = 0 ;
		mFree = 0 ;
	}

	implicit ~AllocatorLayout () noexcept ;

	implicit AllocatorLayout (CREF<AllocatorLayout> that) = delete ;

	forceinline VREF<AllocatorLayout> operator= (CREF<AllocatorLayout> that) = delete ;

	implicit AllocatorLayout (RREF<AllocatorLayout> that) noexcept :AllocatorLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<AllocatorLayout> operator= (RREF<AllocatorLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct AllocatorHolder implement Interface {
	imports VFat<AllocatorHolder> hold (VREF<AllocatorLayout> that) ;
	imports CFat<AllocatorHolder> hold (CREF<AllocatorLayout> that) ;

	virtual void prepare (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual VREF<Pointer> bt (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> bt (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX alloc () = 0 ;
	virtual INDEX alloc (RREF<BoxLayout> item) = 0 ;
	virtual void free (CREF<INDEX> index) = 0 ;
	virtual BOOL used (CREF<INDEX> index) const = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
} ;

inline AllocatorLayout::~AllocatorLayout () noexcept {
	AllocatorHolder::hold (thiz)->destroy () ;
}

template <class A ,class B>
class AllocatorUnknownBinder implement ReflectUnknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = TupleNode<A ,B> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_vptr (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_vptr (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A ,class B>
class AllocatorRealLayout implement AllocatorLayout {
public:
	implicit AllocatorRealLayout () noexcept {
		noop (RefBuffer<TupleNode<A ,B>> ()) ;
	}
} ;

using ALLOCATOR_MIN_SIZE = ENUM<256> ;

template <class A ,class B>
class Allocator implement AllocatorRealLayout<A ,B> {
private:
	require (IS_TRIVIAL<B>) ;

protected:
	using AllocatorRealLayout<A ,B>::mAllocator ;
	using AllocatorRealLayout<A ,B>::mOffset ;
	using AllocatorRealLayout<A ,B>::mWidth ;
	using AllocatorRealLayout<A ,B>::mLength ;
	using AllocatorRealLayout<A ,B>::mFree ;

public:
	implicit Allocator () = default ;

	explicit Allocator (CREF<LENGTH> size_) {
		AllocatorHolder::hold (thiz)->initialize (AllocatorUnknownBinder<A ,B> () ,size_) ;
	}

	BOOL exist () const {
		return AllocatorHolder::hold (thiz)->exist () ;
	}

	Unknown unknown () const {
		return AllocatorHolder::hold (thiz)->unknown () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return AllocatorHolder::hold (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return AllocatorHolder::hold (thiz)->raw () ;
	}

	void clear () {
		return AllocatorHolder::hold (thiz)->clear () ;
	}

	LENGTH size () const {
		return AllocatorHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return AllocatorHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return AllocatorHolder::hold (thiz)->length () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return AllocatorHolder::hold (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return AllocatorHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	VREF<B> bt (CREF<INDEX> index) leftvalue {
		return AllocatorHolder::hold (thiz)->bt (index) ;
	}

	CREF<B> bt (CREF<INDEX> index) const leftvalue {
		return AllocatorHolder::hold (thiz)->bt (index) ;
	}

	INDEX alloc () {
		return AllocatorHolder::hold (thiz)->alloc () ;
	}

	INDEX alloc (RREF<BoxLayout> item) {
		return AllocatorHolder::hold (thiz)->alloc (move (item)) ;
	}

	void free (CREF<INDEX> index) {
		return AllocatorHolder::hold (thiz)->free (index) ;
	}

	BOOL used (CREF<INDEX> index) const {
		return AllocatorHolder::hold (thiz)->used (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		AllocatorHolder::hold (thiz)->prepare (AllocatorUnknownBinder<A ,B> ()) ;
		return AllocatorHolder::hold (thiz)->resize (size_) ;
	}
} ;
} ;
