#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"

namespace CSC {
class Pointer implement Proxy {
public:
	static VREF<Pointer> make (CREF<FLAG> that) {
		return *(reinterpret_cast<PTR<VREF<Pointer>>> (that)) ;
	}

	template <class ARG1>
	static VREF<Pointer> from (VREF<ARG1> that) {
		return reinterpret_cast<VREF<Pointer>> (that) ;
	}

	template <class ARG1>
	static CREF<Pointer> from (CREF<ARG1> that) {
		return reinterpret_cast<CREF<Pointer>> (that) ;
	}

	template <class ARG1>
	static RREF<Pointer> from (RREF<ARG1> that) = delete ;

	template <class ARG1>
	forceinline operator VREF<ARG1> () leftvalue {
		return reinterpret_cast<VREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator CREF<ARG1> () leftvalue {
		return reinterpret_cast<CREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator ARG1 () leftvalue = delete ;

	template <class ARG1>
	forceinline operator CREF<ARG1> () const leftvalue {
		return reinterpret_cast<CREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator ARG1 () const leftvalue = delete ;
} ;

struct FUNCTION_noop {
	forceinline void operator() () const noexcept {
		return ;
	}

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> a) const noexcept {
		return ;
	}
} ;

static constexpr auto noop = FUNCTION_noop () ;

struct FUNCTION_address {
	template <class ARG1>
	forceinline FLAG operator() (CREF<ARG1> a) const noexcept {
		return FLAG (&Pointer::from (a)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

struct FUNCTION_inline_ifdo {
	forceinline BOOL operator() (CREF<BOOL> flag) const noexcept {
		return FALSE ;
	}

	forceinline BOOL operator() (VREF<BOOL> flag) const noexcept {
		flag = FALSE ;
		return FALSE ;
	}
} ;

static constexpr auto inline_ifdo = FUNCTION_inline_ifdo () ;

struct FUNCTION_inline_unittest {
	imports BOOL invoke () ;

	forceinline BOOL operator() () const noexcept {
		return invoke () ;
	}
} ;

static constexpr auto inline_unittest = FUNCTION_inline_unittest () ;

struct FUNCTION_inline_abort {
	imports void invoke () ;

	forceinline void operator() () const noexcept {
		return invoke () ;
	}
} ;

static constexpr auto inline_abort = FUNCTION_inline_abort () ;

struct FUNCTION_inline_watch {
	imports void invoke (VREF<Pointer> src) ;

	template <class ARG1 ,class ARG2>
	forceinline void operator() (TYPE<ARG1> ,XREF<ARG2> src) const noexcept {
		static volatile auto mInstance = (&src) ;
		return invoke (Pointer::make (FLAG (mInstance))) ;
	}
} ;

static constexpr auto inline_watch = FUNCTION_inline_watch () ;

struct FUNCTION_inline_type_name {
	imports FLAG invoke (CREF<Pointer> squalor ,CREF<FLAG> func_) ;

	forceinline FLAG operator() (CREF<Pointer> squalor ,CREF<FLAG> func_) const noexcept {
		return invoke (squalor ,func_) ;
	}
} ;

static constexpr auto inline_type_name = FUNCTION_inline_type_name () ;

struct FUNCTION_inline_list_pair {
	imports Tuple<FLAG ,FLAG> invoke (CREF<Pointer> squalor ,CREF<LENGTH> step_) ;

	forceinline Tuple<FLAG ,FLAG> operator() (CREF<Pointer> squalor ,CREF<LENGTH> step_) const noexcept {
		return invoke (squalor ,step_) ;
	}
} ;

static constexpr auto inline_list_pair = FUNCTION_inline_list_pair () ;

struct FUNCTION_inline_memset {
	imports void invoke (VREF<Pointer> dst ,CREF<LENGTH> size_) ;

	forceinline void operator() (VREF<Pointer> dst ,CREF<LENGTH> size_) const noexcept {
		return __macro_memset (dst ,size_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline void operator() (VREF<ARG1> dst) const noexcept {
		return thiz (Pointer::from (dst) ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto inline_memset = FUNCTION_inline_memset () ;

struct FUNCTION_inline_memcpy {
	imports void invoke (VREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) ;

	forceinline void operator() (VREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) const noexcept {
		return __macro_memcpy (dst ,src ,size_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline void operator() (VREF<ARG1> dst ,CREF<ARG1> src) const noexcept {
		return thiz (Pointer::from (dst) ,Pointer::from (src) ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto inline_memcpy = FUNCTION_inline_memcpy () ;

struct FUNCTION_inline_memcmp {
	imports FLAG invoke (CREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) ;

	forceinline FLAG operator() (CREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) const noexcept {
		return __macro_memcmp (dst ,src ,size_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline FLAG operator() (CREF<ARG1> dst ,CREF<ARG1> src) const noexcept {
		return thiz (Pointer::from (dst) ,Pointer::from (src) ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto inline_memcmp = FUNCTION_inline_memcmp () ;

template <class...>
trait FUNCTION_keep_impl_HELP ;

template <class A>
struct FUNCTION_keep_impl0 {
	template <class ARG1>
	forceinline VREF<A> operator() (VREF<ARG1> a) const noexcept {
		return static_cast<VREF<A>> (a) ;
	}

	template <class ARG1>
	forceinline CREF<A> operator() (CREF<ARG1> a) const noexcept {
		return static_cast<CREF<A>> (a) ;
	}

	template <class ARG1>
	forceinline RREF<A> operator() (RREF<ARG1> a) const noexcept {
		return static_cast<RREF<A>> (a) ;
	}
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,ORDINARY>>> {
	using RET = FUNCTION_keep_impl0<A> ;
} ;

template <class A>
struct FUNCTION_keep_impl1 {
	template <class ARG1>
	forceinline VREF<A> operator() (VREF<ARG1> a) const noexcept {
		return static_cast<VREF<A>> (a) ;
	}

	template <class ARG1>
	forceinline VREF<A> operator() (CREF<ARG1> a) const noexcept = delete ;

	template <class ARG1>
	forceinline VREF<A> operator() (RREF<ARG1> a) const noexcept = delete ;
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,VARIABLE>>> {
	using RET = FUNCTION_keep_impl1<A> ;
} ;

template <class A>
struct FUNCTION_keep_impl2 {
	template <class ARG1>
	forceinline CREF<A> operator() (VREF<ARG1> a) const noexcept {
		return static_cast<CREF<A>> (a) ;
	}

	template <class ARG1>
	forceinline CREF<A> operator() (CREF<ARG1> a) const noexcept {
		return static_cast<CREF<A>> (a) ;
	}

	template <class ARG1>
	forceinline CREF<A> operator() (RREF<ARG1> a) const noexcept {
		return static_cast<CREF<A>> (a) ;
	}
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,CONSTANT>>> {
	using RET = FUNCTION_keep_impl2<A> ;
} ;

template <class A>
struct FUNCTION_keep_impl3 {
	template <class ARG1>
	forceinline RREF<A> operator() (VREF<ARG1> a) const noexcept {
		return static_cast<RREF<A>> (a) ;
	}

	template <class ARG1>
	forceinline RREF<A> operator() (CREF<ARG1> a) const noexcept = delete ;

	template <class ARG1>
	forceinline RREF<A> operator() (RREF<ARG1> a) const noexcept {
		return static_cast<RREF<A>> (a) ;
	}
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,REGISTER>>> {
	using RET = FUNCTION_keep_impl3<A> ;
} ;

template <class A>
using FUNCTION_keep_impl = typename FUNCTION_keep_impl_HELP<REMOVE_CVR<A> ,REFLECT_REF<A> ,ALWAYS>::RET ;

struct FUNCTION_keep {
	template <class ARG1>
	forceinline consteval FUNCTION_keep_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNCTION_keep_impl<ARG1> () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_move {
	template <class ARG1>
	forceinline RREF<ARG1> operator() (VREF<ARG1> a) const noexcept {
		return static_cast<RREF<ARG1>> (a) ;
	}

	template <class ARG1>
	forceinline ARG1 operator() (CREF<ARG1> a) const {
		require (IS_CLONEABLE<ARG1>) ;
		return static_cast<CREF<ARG1>> (a) ;
	}

	template <class ARG1>
	forceinline RREF<ARG1> operator() (RREF<ARG1> a) const noexcept {
		return static_cast<RREF<ARG1>> (a) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_swap {
	template <class ARG1>
	forceinline void operator() (VREF<ARG1> a ,VREF<ARG1> b) const noexcept {
		//@warn: no class should depend on its address
		require (IS_DEFAULT<ARG1>) ;
		auto rax = Union<ARG1> () ;
		inline_memcpy (Pointer::from (rax) ,Pointer::from (a) ,SIZE_OF<ARG1>::expr) ;
		inline_memcpy (Pointer::from (a) ,Pointer::from (b) ,SIZE_OF<ARG1>::expr) ;
		inline_memcpy (Pointer::from (b) ,Pointer::from (rax) ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

struct FUNCTION_assign {
	template <class ARG1>
	forceinline VREF<ARG1> operator() (VREF<ARG1> a ,VREF<ARG1> b) const noexcept {
		if (address (a) == address (b))
			return a ;
		a.~ARG1 () ;
		new (csc_placement_new_t {(&a)}) ARG1 (move (b)) ;
		return a ;
	}

	template <class ARG1>
	forceinline VREF<ARG1> operator() (VREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		if (address (a) == address (b))
			return a ;
		a.~ARG1 () ;
		new (csc_placement_new_t {(&a)}) ARG1 (move (b)) ;
		return a ;
	}

	template <class ARG1>
	forceinline VREF<ARG1> operator() (VREF<ARG1> a ,RREF<ARG1> b) const noexcept = delete ;
} ;

static constexpr auto assign = FUNCTION_assign () ;

struct FUNCTION_replace {
	template <class ARG1 ,class ARG2>
	forceinline void operator() (VREF<ARG1> a ,CREF<ARG1> expect ,CREF<ARG2> item) const noexcept {
		if (a != expect)
			return ;
		a = ARG1 (item) ;
	}
} ;

static constexpr auto replace = FUNCTION_replace () ;

struct FUNCTION_invoke {
	template <class ARG1>
	forceinline FUNCTION_RETURN<ARG1> operator() (RREF<ARG1> func) const {
		return func () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

template <class A>
struct FUNCTION_bitwise_impl {
	template <class ARG1>
	forceinline A operator() (CREF<ARG1> a) const noexcept {
		require (IS_TRIVIAL<A>) ;
		using R1X = A ;
		using R2X = CONDITIONAL<IS_SAME<ARG1 ,Pointer> ,A ,ARG1> ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		A ret ;
		inline_memcpy (Pointer::from (ret) ,Pointer::from (a) ,SIZE_OF<R1X>::expr) ;
		return move (ret) ;
	}
} ;

struct FUNCTION_bitwise {
	template <class ARG1>
	forceinline consteval FUNCTION_bitwise_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNCTION_bitwise_impl<ARG1> () ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_memorize {
	template <class ARG1>
	forceinline CREF<FUNCTION_RETURN<ARG1>> operator() (CREF<ARG1> func) const {
		static const auto mInstance = func () ;
		return mInstance ;
	}
} ;

static constexpr auto memorize = FUNCTION_memorize () ;

struct FUNCTION_inline_vptr {
	forceinline FLAG operator() (CREF<Interface> binder) const noexcept {
		return bitwise[TYPE<FLAG>::expr] (binder) ;
	}
} ;

static constexpr auto inline_vptr = FUNCTION_inline_vptr () ;

struct FUNCTION_inline_min {
	forceinline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a <= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto inline_min = FUNCTION_inline_min () ;

struct FUNCTION_inline_max {
	forceinline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a >= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto inline_max = FUNCTION_inline_max () ;

struct FUNCTION_inline_between {
	forceinline BOOL operator() (CREF<VAL> val ,CREF<VAL> begin_ ,CREF<VAL> end_) const noexcept {
		if (val < begin_)
			return FALSE ;
		if (val >= end_)
			return FALSE ;
		return TRUE ;
	}
} ;

static constexpr auto inline_between = FUNCTION_inline_between () ;

struct FUNCTION_inline_alignas {
	forceinline VAL operator() (CREF<VAL> val ,CREF<VAL> align) const noexcept {
		assert (align > 0) ;
		const auto r1x = val / align * align ;
		if (r1x == val)
			return r1x ;
		return r1x + align ;
	}
} ;

static constexpr auto inline_alignas = FUNCTION_inline_alignas () ;

struct FUNCTION_inline_clone {
	template <class ARG1>
	forceinline void operator() (VREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		return clone_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLONEABLE<ARG1>>>
	forceinline void clone_impl (CREF<typeof (PH3)> ,VREF<ARG1> a ,CREF<ARG1> b) const {
		assign (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_CLONE<ARG1>>>
	forceinline void clone_impl (CREF<typeof (PH2)> ,VREF<ARG1> a ,CREF<ARG1> b) const {
		a = b.clone () ;
	}

	template <class ARG1>
	forceinline void clone_impl (CREF<typeof (PH1)> ,VREF<ARG1> a ,CREF<ARG1> b) const {
		assert (FALSE) ;
	}
} ;

static constexpr auto inline_clone = FUNCTION_inline_clone () ;

struct FUNCTION_inline_equal {
	template <class ARG1>
	forceinline BOOL operator() (CREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		return equal_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_EQUALABLE<ARG1>>>
	forceinline BOOL equal_impl (CREF<typeof (PH4)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a == b ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_EQUAL<ARG1>>>
	forceinline BOOL equal_impl (CREF<typeof (PH3)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a.equal (b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline BOOL equal_impl (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return inline_memcmp (a ,b) == ZERO ;
	}

	template <class ARG1>
	forceinline BOOL equal_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		assert (FALSE) ;
		return FALSE ;
	}
} ;

static constexpr auto inline_equal = FUNCTION_inline_equal () ;

struct FUNCTION_inline_compr {
	template <class ARG1>
	forceinline FLAG operator() (CREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		return compr_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_COMPRABLE<ARG1>>>
	forceinline FLAG compr_impl (CREF<typeof (PH4)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		if (a < b)
			return NONE ;
		if (a > b)
			return IDEN ;
		return ZERO ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_COMPR<ARG1>>>
	forceinline FLAG compr_impl (CREF<typeof (PH3)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a.compr (b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline FLAG compr_impl (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return inline_memcmp (a ,b) ;
	}

	template <class ARG1>
	forceinline FLAG compr_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		assert (FALSE) ;
		return ZERO ;
	}
} ;

static constexpr auto inline_compr = FUNCTION_inline_compr () ;

struct FUNCTION_inline_visit {
	template <class ARG1>
	forceinline void operator() (VREF<VisitorBinder> visitor ,CREF<ARG1> a) const noexcept {
		return visit_impl (PHX ,visitor ,a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline void visit_impl (CREF<typeof (PH3)> ,VREF<VisitorBinder> visitor ,CREF<ARG1> a) const {
		const auto r1x = bitwise[TYPE<BYTE_BASE<ARG1>>::expr] (a) ;
		visitor.push (r1x) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_VISIT<ARG1>>>
	forceinline void visit_impl (CREF<typeof (PH2)> ,VREF<VisitorBinder> visitor ,CREF<ARG1> a) const {
		return a.visit (visitor) ;
	}

	template <class ARG1>
	forceinline void visit_impl (CREF<typeof (PH1)> ,VREF<VisitorBinder> visitor ,CREF<ARG1> a) const {
		assert (FALSE) ;
	}
} ;

static constexpr auto inline_visit = FUNCTION_inline_visit () ;

struct IndexIteratorLayout {
	INDEX mBegin ;
	INDEX mEnd ;
	INDEX mPeek ;

public:
	implicit IndexIteratorLayout () noexcept {
		mBegin = 0 ;
		mEnd = 0 ;
		mPeek = 0 ;
	}
} ;

class IndexIterator implement IndexIteratorLayout {
protected:
	using IndexIteratorLayout::mBegin ;
	using IndexIteratorLayout::mEnd ;
	using IndexIteratorLayout::mPeek ;

public:
	implicit IndexIterator () = default ;

	explicit IndexIterator (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
		mBegin = begin_ ;
		mEnd = inline_max (begin_ ,end_) ;
		mPeek = mBegin ;
	}

	LENGTH length () const {
		return mEnd - mBegin ;
	}

	IndexIterator begin () const {
		return thiz ;
	}

	IndexIterator end () const {
		return thiz ;
	}

	BOOL good () const {
		return mPeek != mEnd ;
	}

	forceinline BOOL operator== (CREF<IndexIterator>) const {
		return (!good ()) ;
	}

	forceinline BOOL operator!= (CREF<IndexIterator>) const {
		return good () ;
	}

	CREF<INDEX> peek () const leftvalue {
		return mPeek ;
	}

	forceinline CREF<INDEX> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek++ ;
	}

	forceinline void operator++ () {
		next () ;
	}
} ;

struct Pixel {
	INDEX mX ;
	INDEX mY ;
} ;

struct PixelIteratorLayout {
	Pixel mBegin ;
	Pixel mEnd ;
	Pixel mPeek ;

public:
	implicit PixelIteratorLayout () noexcept {
		mBegin.mX = 0 ;
		mBegin.mY = 0 ;
		mEnd.mX = 0 ;
		mEnd.mY = 0 ;
		mPeek.mX = 0 ;
		mPeek.mY = 0 ;
	}
} ;

class PixelIterator implement PixelIteratorLayout {
protected:
	using PixelIteratorLayout::mBegin ;
	using PixelIteratorLayout::mEnd ;
	using PixelIteratorLayout::mPeek ;

public:
	implicit PixelIterator () = default ;

	explicit PixelIterator (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) {
		mBegin.mX = begin_x ;
		mBegin.mY = begin_y ;
		mEnd.mX = inline_max (begin_x ,end_x) ;
		mEnd.mY = inline_max (begin_y ,end_y) ;
		mPeek = mBegin ;
		if (length () > 0)
			return ;
		mPeek = mEnd ;
	}

	LENGTH length () const {
		return (mEnd.mX - mBegin.mX) * (mEnd.mY - mBegin.mY) ;
	}

	PixelIterator begin () const {
		return thiz ;
	}

	PixelIterator end () const {
		return thiz ;
	}

	BOOL good () const {
		return mPeek.mY != mEnd.mY ;
	}

	forceinline BOOL operator== (CREF<PixelIterator>) const {
		return (!good ()) ;
	}

	forceinline BOOL operator!= (CREF<PixelIterator>) const {
		return good () ;
	}

	CREF<Pixel> peek () const leftvalue {
		return mPeek ;
	}

	forceinline CREF<Pixel> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek.mX++ ;
		if (mPeek.mX < mEnd.mX)
			return ;
		mPeek.mX = 0 ;
		mPeek.mY++ ;
	}

	forceinline void operator++ () {
		next () ;
	}
} ;

struct FUNCTION_iter {
	forceinline IndexIterator operator() (CREF<INDEX> begin_ ,CREF<INDEX> end_) const noexcept {
		return IndexIterator (begin_ ,end_) ;
	}

	forceinline PixelIterator operator() (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) const noexcept {
		return PixelIterator (begin_x ,end_x ,begin_y ,end_y) ;
	}
} ;

static constexpr auto iter = FUNCTION_iter () ;

struct FatLayout {
	FLAG mHolder ;
	FLAG mLayout ;
} ;

template <class A ,class B>
class Fat implement A {
private:
	require (IS_INTERFACE<A>) ;

protected:
	FLAG mLayout ;

public:
	VREF<B> fake_m () leftvalue {
		return Pointer::make (mLayout) ;
	}

	CREF<B> fake_m () const leftvalue {
		return Pointer::make (mLayout) ;
	}
} ;

template <class A>
class VFat implement FatLayout {
private:
	require (IS_INTERFACE<A>) ;

protected:
	using FatLayout::mHolder ;
	using FatLayout::mLayout ;

public:
	implicit VFat () = delete ;

	template <class ARG1 ,class ARG2>
	explicit VFat (CREF<ARG1> holder ,VREF<ARG2> that) {
		require (IS_EXTEND<A ,ARG1>) ;
		require (ENUM_EQUAL<SIZE_OF<ARG1> ,SIZE_OF<VFat>>) ;
		require (ENUM_EQUAL<ALIGN_OF<ARG1> ,ALIGN_OF<VFat>>) ;
		using R1X = typeof (nullof (ARG1).fake) ;
		require (IS_SAME<R1X ,ARG2>) ;
		mHolder = inline_vptr (holder) ;
		mLayout = address (that) ;
	}

	VREF<A> self_m () const {
		return Pointer::from (const_cast<VREF<VFat>> (thiz)) ;
	}

	forceinline PTR<VREF<A>> operator-> () const {
		return (&self) ;
	}
} ;

template <class A>
class CFat implement FatLayout {
private:
	require (IS_INTERFACE<A>) ;

protected:
	using FatLayout::mHolder ;
	using FatLayout::mLayout ;

public:
	implicit CFat () = delete ;

	template <class ARG1 ,class ARG2>
	explicit CFat (CREF<ARG1> holder ,CREF<ARG2> that) {
		require (IS_EXTEND<A ,ARG1>) ;
		require (ENUM_EQUAL<SIZE_OF<ARG1> ,SIZE_OF<CFat>>) ;
		require (ENUM_EQUAL<ALIGN_OF<ARG1> ,ALIGN_OF<CFat>>) ;
		using R1X = typeof (nullof (ARG1).fake) ;
		require (IS_SAME<R1X ,ARG2>) ;
		mHolder = inline_vptr (holder) ;
		mLayout = address (that) ;
	}

	CREF<A> self_m () const {
		return Pointer::from (const_cast<CREF<CFat>> (thiz)) ;
	}

	forceinline PTR<CREF<A>> operator-> () const {
		return (&self) ;
	}
} ;

template <class A>
class RFat implement FatLayout {
private:
	require (IS_INTERFACE<A>) ;

protected:
	using FatLayout::mHolder ;
	using FatLayout::mLayout ;

public:
	implicit RFat () = delete ;

	explicit RFat (CREF<ReflectUnknown> unknown) {
		mHolder = unknown.reflect (A::expr) ;
		assert (mHolder != ZERO) ;
		mLayout = ZERO ;
	}

	CREF<A> self_m () const {
		return Pointer::from (const_cast<CREF<RFat>> (thiz)) ;
	}

	forceinline PTR<CREF<A>> operator-> () const {
		return (&self) ;
	}
} ;

template <class A>
struct SimpleUnknownBinder implement ReflectUnknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == A::expr)
			return inline_vptr (A ()) ;
		return ZERO ;
	}
} ;

class Unknown {
protected:
	FLAG mHolder ;

public:
	implicit Unknown () = delete ;

	explicit Unknown (CREF<FLAG> that) {
		mHolder = that ;
	}

	implicit Unknown (CREF<ReflectUnknown> that) {
		mHolder = inline_vptr (that) ;
	}

	forceinline operator CREF<ReflectUnknown> () const {
		return Pointer::from (thiz) ;
	}
} ;

struct ReflectSize implement Interface {
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 100 ;
	}
} ;

template <class A>
class ReflectSizeBinder implement ReflectSize {
public:
	LENGTH type_size () const override {
		return SIZE_OF<A>::expr ;
	}

	LENGTH type_align () const override {
		return ALIGN_OF<A>::expr ;
	}
} ;

struct ReflectCreate implement Interface {
	virtual void create (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 101 ;
	}
} ;

template <class A>
class ReflectCreateBinder implement ReflectCreate {
private:
	require (IS_DEFAULT<A>) ;

public:
	void create (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept override {
		if (MACRO_IS_TRIVIAL_CONSTRUCTIBLE<A>::expr)
			return ;
		auto &&rax = keep[TYPE<ARR<A>>::expr] (a) ;
		for (auto &&i : iter (0 ,size_)) {
			new (csc_placement_new_t {(&rax[i])}) A () ;
		}
	}
} ;

struct ReflectDestroy implement Interface {
	virtual void destroy (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 102 ;
	}
} ;

template <class A>
class ReflectDestroyBinder implement ReflectDestroy {
public:
	void destroy (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept override {
		if (MACRO_IS_TRIVIAL_DESTRUCTIBLE<A>::expr)
			return ;
		auto &&rax = keep[TYPE<ARR<A>>::expr] (a) ;
		for (auto &&i : iter (0 ,size_)) {
			rax[i].~A () ;
		}
	}
} ;

struct ReflectAssign implement Interface {
	virtual void assign (VREF<Pointer> a) const noexcept = 0 ;
	virtual void assign (VREF<Pointer> a ,VREF<Pointer> b) const noexcept = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 103 ;
	}
} ;

template <class A>
class ReflectAssignBinder implement ReflectAssign {
public:
	void assign (VREF<Pointer> a) const noexcept override {
		auto rax = A () ;
		assign (a ,Pointer::from (rax)) ;
	}

	void assign (VREF<Pointer> a ,VREF<Pointer> b) const noexcept override {
		CSC::assign (keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}
} ;

template <class A>
struct ReflectFriend implement Interface {
	virtual VFat<A> hold (VREF<Pointer> a) const = 0 ;
	virtual CFat<A> hold (CREF<Pointer> a) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 104 ;
	}
} ;

template <class A ,class B>
class ReflectFriendBinder implement ReflectFriend<A> {
public:
	VFat<A> hold (VREF<Pointer> a) const override {
		return B::hold (keep[TYPE<A>::expr] (a)) ;
	}

	CFat<A> hold (CREF<Pointer> a) const override {
		return B::hold (keep[TYPE<A>::expr] (a)) ;
	}
} ;

struct ReflectTuple implement Interface {
	virtual LENGTH tuple_m1st () const = 0 ;
	virtual LENGTH tuple_m2nd () const = 0 ;
	virtual LENGTH tuple_m3rd () const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 105 ;
	}
} ;

template <class A>
class ReflectTupleBinder implement ReflectTuple {
public:
	LENGTH tuple_m1st () const override {
		return tuple_m1st_impl (PHX ,TYPE<A>::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_M1ST<ARG1>>>
	forceinline LENGTH tuple_m1st_impl (CREF<typeof (PH2)> ,TYPE<ARG1>) const {
		return address (nullof (A).m1st) - address (nullof (A)) ;
	}

	template <class ARG1>
	forceinline LENGTH tuple_m1st_impl (CREF<typeof (PH1)> ,TYPE<ARG1>) const {
		assert (FALSE) ;
		return 0 ;
	}

	LENGTH tuple_m2nd () const override {
		return tuple_m2nd_impl (PHX ,TYPE<A>::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_M2ND<ARG1>>>
	forceinline LENGTH tuple_m2nd_impl (CREF<typeof (PH2)> ,TYPE<ARG1>) const {
		return address (nullof (A).m2nd) - address (nullof (A)) ;
	}

	template <class ARG1>
	forceinline LENGTH tuple_m2nd_impl (CREF<typeof (PH1)> ,TYPE<ARG1>) const {
		assert (FALSE) ;
		return 0 ;
	}

	LENGTH tuple_m3rd () const override {
		return tuple_m3rd_impl (PHX ,TYPE<A>::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_M3RD<ARG1>>>
	forceinline LENGTH tuple_m3rd_impl (CREF<typeof (PH2)> ,TYPE<ARG1>) const {
		return address (nullof (A).m3rd) - address (nullof (A)) ;
	}

	template <class ARG1>
	forceinline LENGTH tuple_m3rd_impl (CREF<typeof (PH1)> ,TYPE<ARG1>) const {
		assert (FALSE) ;
		return 0 ;
	}
} ;

struct BoxLayout {
	FLAG mHolder ;

public:
	implicit BoxLayout () noexcept {
		mHolder = ZERO ;
	}

	implicit ~BoxLayout () noexcept ;

	implicit BoxLayout (CREF<BoxLayout> that) = delete ;

	forceinline VREF<BoxLayout> operator= (CREF<BoxLayout> that) = delete ;

	implicit BoxLayout (RREF<BoxLayout> that) noexcept :BoxLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<BoxLayout> operator= (RREF<BoxLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct BoxHolder implement Interface {
	imports VFat<BoxHolder> hold (VREF<BoxLayout> that) ;
	imports CFat<BoxHolder> hold (CREF<BoxLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual void remake (CREF<Unknown> holder) = 0 ;
	virtual void acquire (CREF<BoxLayout> that) = 0 ;
	virtual void release () = 0 ;
} ;

inline BoxLayout::~BoxLayout () noexcept {
	BoxHolder::hold (thiz)->destroy () ;
}

template <class A>
class BoxUnknownBinder implement ReflectUnknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A ,class B = A>
class Box implement BoxLayout {
protected:
	using BoxLayout::mHolder ;
	Union<B> mStorage ;

public:
	implicit Box () = default ;

	implicit Box (CREF<typeof (NULL)>) {
		noop () ;
	}

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,B>>>
	implicit Box (RREF<Box<A ,ARG1>> that) {
		require (ENUM_EQUAL<SIZE_OF<B> ,SIZE_OF<ARG1>>) ;
		require (ENUM_EQUAL<ALIGN_OF<B> ,ALIGN_OF<ARG1>>) ;
		acquire (that) ;
		that.release () ;
	}

	template <class...ARG1>
	static Box make (XREF<ARG1>...initval) {
		Box ret ;
		ret.remake (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return BoxHolder::hold (thiz)->exist () ;
	}

	forceinline BOOL operator== (CREF<typeof (NULL)>) const {
		return (!exist ()) ;
	}

	forceinline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	Unknown unknown () const {
		return BoxHolder::hold (thiz)->unknown () ;
	}

	VREF<A> self_m () leftvalue {
		return BoxHolder::hold (thiz)->self ;
	}

	forceinline PTR<VREF<A>> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return BoxHolder::hold (thiz)->self ;
	}

	forceinline PTR<CREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	template <class...ARG1>
	void remake (XREF<ARG1>...initval) {
		if (exist ())
			return ;
		new (csc_placement_new_t {(&mStorage)}) A (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		BoxHolder::hold (thiz)->initialize (BoxUnknownBinder<A> ()) ;
	}

	void acquire (CREF<BoxLayout> that) {
		return BoxHolder::hold (thiz)->acquire (that) ;
	}

	void release () {
		return BoxHolder::hold (thiz)->release () ;
	}
} ;

struct PinLayout {} ;

template <class A>
class Pin implement PinLayout {
protected:
	mutable Union<A> mStorage ;

public:
	implicit Pin () noexcept {
		mStorage = zeroize () ;
	}

	implicit ~Pin () noexcept {
		const auto r1x = zeroize () ;
		if (inline_memcmp (mStorage ,r1x) == 0)
			return ;
		auto &&rax = keep[TYPE<A>::expr] (self) ;
		rax.~A () ;
		mStorage = r1x ;
	}

	static Union<A> zeroize () {
		Union<A> ret ;
		inline_memset (ret) ;
		return move (ret) ;
	}

	implicit Pin (CREF<Pin> that) :Pin () {
		noop () ;
	}

	forceinline VREF<Pin> operator= (CREF<Pin> that) {
		return thiz ;
	}

	implicit Pin (RREF<Pin> that) noexcept :Pin () {
		noop () ;
	}

	forceinline VREF<Pin> operator= (RREF<Pin> that) noexcept {
		return thiz ;
	}

	VREF<Pointer> self_m () const leftvalue {
		return Pointer::from (mStorage) ;
	}

	forceinline operator VREF<Pointer> () const leftvalue {
		return self ;
	}

	void get (VREF<A> item) const {
		auto &&rax = keep[TYPE<A>::expr] (self) ;
		item = move (rax) ;
	}

	void set (VREF<A> item) const {
		auto &&rax = keep[TYPE<A>::expr] (self) ;
		rax = move (item) ;
	}
} ;

struct RefLayout {
	FLAG mHandle ;
	FLAG mLayout ;

public:
	implicit RefLayout () noexcept {
		mHandle = ZERO ;
		mLayout = ZERO ;
	}

	implicit ~RefLayout () noexcept ;

	implicit RefLayout (CREF<RefLayout> that) = delete ;

	forceinline VREF<RefLayout> operator= (CREF<RefLayout> that) = delete ;

	implicit RefLayout (RREF<RefLayout> that) noexcept :RefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<RefLayout> operator= (RREF<RefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct RefHolder implement Interface {
	imports VFat<RefHolder> hold (VREF<RefLayout> that) ;
	imports CFat<RefHolder> hold (CREF<RefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<RefLayout> that) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<Unknown> extend ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<FLAG> layout) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual BOOL exclusive () const = 0 ;
} ;

inline RefLayout::~RefLayout () noexcept {
	RefHolder::hold (thiz)->destroy () ;
}

template <class A>
class RefUnknownBinder implement ReflectUnknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_vptr (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Ref implement RefLayout {
protected:
	using RefLayout::mHandle ;
	using RefLayout::mLayout ;

public:
	implicit Ref () = default ;

	implicit Ref (CREF<typeof (NULL)>) {
		noop () ;
	}

	template <class...ARG1>
	static Ref make (XREF<ARG1>...initval) {
		Ref ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		RefHolder::hold (ret)->initialize (move (rax)) ;
		return move (ret) ;
	}

	static Ref reference (VREF<A> that) {
		Ref ret ;
		ret.mHandle = VARIABLE::expr ;
		ret.mLayout = address (that) ;
		return move (ret) ;
	}

	static Ref reference (CREF<A> that) {
		Ref ret ;
		ret.mHandle = CONSTANT::expr ;
		ret.mLayout = address (that) ;
		return move (ret) ;
	}

	static Ref reference (RREF<A> that) = delete ;

	implicit Ref (CREF<Ref> that) {
		RefHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<Ref> operator= (CREF<Ref> that) {
		return assign (thiz ,that) ;
	}

	implicit Ref (RREF<Ref> that) = default ;

	forceinline VREF<Ref> operator= (RREF<Ref> that) = default ;

	Ref share () const {
		return move (thiz) ;
	}

	BOOL exist () const {
		return RefHolder::hold (thiz)->exist () ;
	}

	forceinline BOOL operator== (CREF<typeof (NULL)>) const {
		return (!exist ()) ;
	}

	forceinline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	Unknown unknown () const {
		return RefHolder::hold (thiz)->unknown () ;
	}

	VREF<A> self_m () leftvalue {
		return RefHolder::hold (thiz)->self ;
	}

	forceinline PTR<VREF<A>> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return RefHolder::hold (thiz)->self ;
	}

	forceinline PTR<CREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	BOOL exclusive () const {
		return RefHolder::hold (thiz)->exclusive () ;
	}
} ;

struct HeapLayout {
	FLAG mHolder ;

public:
	implicit HeapLayout () noexcept {
		mHolder = ZERO ;
	}
} ;

struct HeapHolder implement Interface {
	imports CREF<HeapLayout> instance () ;
	imports VFat<HeapHolder> hold (VREF<HeapLayout> that) ;
	imports CFat<HeapHolder> hold (CREF<HeapLayout> that) ;

	virtual void initialize () = 0 ;
	virtual INDEX stack () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual FLAG alloc (CREF<LENGTH> size_) const = 0 ;
	virtual void free (CREF<FLAG> layout) const = 0 ;
} ;

class Heap implement HeapLayout {
protected:
	using HeapLayout::mHolder ;

public:
	static CREF<Heap> instance () {
		return keep[TYPE<Heap>::expr] (HeapHolder::instance ()) ;
	}

	INDEX stack () const {
		return HeapHolder::hold (thiz)->stack () ;
	}

	LENGTH length () const {
		return HeapHolder::hold (thiz)->length () ;
	}

	FLAG alloc (CREF<LENGTH> size_) const {
		return HeapHolder::hold (thiz)->alloc (size_) ;
	}

	void free (CREF<FLAG> layout) const {
		return HeapHolder::hold (thiz)->free (layout) ;
	}
} ;

struct KeyBaseLayout {
	FLAG mHandle ;

public:
	implicit KeyBaseLayout () noexcept {
		mHandle = ZERO ;
	}

	implicit ~KeyBaseLayout () noexcept ;

	implicit KeyBaseLayout (CREF<KeyBaseLayout> that) = delete ;

	forceinline VREF<KeyBaseLayout> operator= (CREF<KeyBaseLayout> that) = delete ;

	implicit KeyBaseLayout (RREF<KeyBaseLayout> that) noexcept :KeyBaseLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<KeyBaseLayout> operator= (RREF<KeyBaseLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct KeyBaseHolder implement Interface {
	imports VFat<KeyBaseHolder> hold (VREF<KeyBaseLayout> that) ;
	imports CFat<KeyBaseHolder> hold (CREF<KeyBaseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CREF<KeyBaseLayout> root ,CREF<FLAG> layout) = 0 ;
	virtual void destroy () = 0 ;
	virtual INDEX get_index () const = 0 ;
	virtual INDEX get_check () const = 0 ;
	virtual void set_key (CREF<INDEX> index) = 0 ;
	virtual void set_def () = 0 ;
	virtual VREF<Pointer> lock (CREF<INDEX> check) leftvalue = 0 ;
	virtual FLAG spwan () const = 0 ;
	virtual FLAG spwan (CREF<INDEX> index) const = 0 ;
} ;

inline KeyBaseLayout::~KeyBaseLayout () noexcept {
	KeyBaseHolder::hold (thiz)->destroy () ;
}

template <class A>
struct KeyRootHolder implement Interface {
	imports CREF<KeyBaseLayout> instance () ;
} ;

template <class A>
class KeyBase implement KeyBaseLayout {
protected:
	using KeyBaseLayout::mHandle ;

public:
	implicit KeyBase () noexcept {
		auto &&rax = keep[TYPE<A>::expr] (thiz) ;
		KeyBaseHolder::hold (thiz)->initialize (KeyRootHolder<A>::instance () ,address (rax)) ;
	}

	INDEX get_index () const {
		return KeyBaseHolder::hold (thiz)->get_index () ;
	}

	INDEX get_check () const {
		return KeyBaseHolder::hold (thiz)->get_check () ;
	}

	void set_key (CREF<INDEX> index) {
		return KeyBaseHolder::hold (thiz)->set_key (index) ;
	}

	void set_def () {
		return KeyBaseHolder::hold (thiz)->set_def () ;
	}
} ;

struct KeyLayout {
	mutable FLAG mHandle ;
	INDEX mCheck ;

public:
	implicit KeyLayout () noexcept {
		mHandle = ZERO ;
		mCheck = 0 ;
	}
} ;

template <class A>
class Key implement KeyLayout {
protected:
	using KeyLayout::mHandle ;
	using KeyLayout::mCheck ;

public:
	implicit Key () = default ;

	implicit Key (VREF<KeyBase<A>> node_) {
		mHandle = KeyBaseHolder::hold (node_)->spwan () ;
		mCheck = KeyBaseHolder::hold (node ())->get_check () ;
	}

	explicit Key (CREF<INDEX> index) {
		mHandle = KeyBaseHolder::hold (KeyRootHolder<A>::instance ())->spwan (index) ;
		mCheck = KeyBaseHolder::hold (node ())->get_check () ;
	}

	BOOL equal (CREF<Key> that) const {
		return inline_equal (address (thiz.self) ,address (that.self)) ;
	}

	forceinline BOOL operator== (CREF<Key> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Key> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Key> that) const {
		return inline_compr (address (thiz.self) ,address (that.self)) ;
	}

	forceinline BOOL operator< (CREF<Key> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Key> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Key> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Key> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const {
		visitor.enter () ;
		inline_visit (visitor ,address (thiz.self)) ;
		visitor.leave () ;
	}

	VREF<A> self_m () const leftvalue {
		return KeyBaseHolder::hold (node ())->lock (mCheck) ;
	}

	forceinline PTR<VREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

private:
	VREF<KeyBase<A>> node () const leftvalue {
		return Pointer::from (mHandle) ;
	}
} ;

struct SliceLayout {
	FLAG mBuffer ;
	LENGTH mSize ;
	LENGTH mStep ;

public:
	implicit SliceLayout () noexcept {
		mBuffer = ZERO ;
		mSize = 0 ;
		mStep = 0 ;
	}
} ;

struct SliceHolder implement Interface {
	imports VFat<SliceHolder> hold (VREF<SliceLayout> that) ;
	imports CFat<SliceHolder> hold (CREF<SliceLayout> that) ;

	virtual void initialize (CREF<FLAG> buffer ,CREF<LENGTH> size_ ,CREF<LENGTH> step_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<STRU32> item) const = 0 ;
	virtual BOOL equal (CREF<SliceLayout> that) const = 0 ;
	virtual FLAG compr (CREF<SliceLayout> that) const = 0 ;
	virtual void visit (VREF<VisitorBinder> visitor) const = 0 ;
	virtual SliceLayout eos () const = 0 ;
} ;

using SLICE_MAX_SIZE = ENUM<4096> ;

class Slice implement SliceLayout {
protected:
	using SliceLayout::mBuffer ;
	using SliceLayout::mSize ;
	using SliceLayout::mStep ;

public:
	implicit Slice () = default ;

	template <class ARG1 ,class = REQUIRE<IS_TEXT<ARRAY_ITEM<ARG1>>>>
	explicit Slice (CREF<ARG1> that) {
		const auto r1x = address (that) ;
		const auto r2x = ENUM_DEC<ARRAY_SIZE<ARG1>>::expr ;
		const auto r3x = SIZE_OF<ARRAY_ITEM<ARG1>>::expr ;
		SliceHolder::hold (thiz)->initialize (r1x ,r2x ,r3x) ;
	}

	explicit Slice (CREF<FLAG> buffer ,CREF<LENGTH> size_ ,CREF<LENGTH> step_) {
		SliceHolder::hold (thiz)->initialize (buffer ,size_ ,step_) ;
	}

	LENGTH size () const {
		return SliceHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return SliceHolder::hold (thiz)->step () ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const {
		return SliceHolder::hold (thiz)->get (index ,item) ;
	}

	forceinline STRU32 operator[] (CREF<INDEX> index) const {
		STRU32 ret ;
		get (index ,ret) ;
		return move (ret) ;
	}

	BOOL equal (CREF<Slice> that) const {
		return SliceHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Slice> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Slice> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Slice> that) const {
		return SliceHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Slice> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Slice> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Slice> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Slice> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const {
		return SliceHolder::hold (thiz)->visit (visitor) ;
	}

	Slice eos () const {
		SliceLayout ret = SliceHolder::hold (thiz)->eos () ;
		return move (keep[TYPE<Slice>::expr] (ret)) ;
	}
} ;

struct ExceptionLayout {
	Slice mWhat ;
	Slice mFunc ;
	Slice mFile ;
	Slice mLine ;
} ;

struct ExceptionHolder implement Interface {
	imports VFat<ExceptionHolder> hold (VREF<ExceptionLayout> that) ;
	imports CFat<ExceptionHolder> hold (CREF<ExceptionLayout> that) ;

	virtual void initialize (CREF<Slice> what_ ,CREF<Slice> func_ ,CREF<Slice> file_ ,CREF<Slice> line_) = 0 ;
	virtual Slice what () const = 0 ;
	virtual Slice func () const = 0 ;
	virtual Slice file () const = 0 ;
	virtual Slice line () const = 0 ;
	virtual void event () const = 0 ;
	virtual void raise () const = 0 ;
} ;

class Exception implement ExceptionLayout {
protected:
	using ExceptionLayout::mWhat ;
	using ExceptionLayout::mFunc ;
	using ExceptionLayout::mFile ;
	using ExceptionLayout::mLine ;

public:
	implicit Exception () = default ;

	explicit Exception (CREF<Slice> what_) {
		ExceptionHolder::hold (thiz)->initialize (what_ ,Slice () ,Slice () ,Slice ()) ;
	}

	explicit Exception (CREF<Slice> what_ ,CREF<Slice> func_ ,CREF<Slice> file_ ,CREF<Slice> line_) {
		ExceptionHolder::hold (thiz)->initialize (what_ ,func_ ,file_ ,line_) ;
	}

	Slice what () const {
		return ExceptionHolder::hold (thiz)->what () ;
	}

	Slice func () const {
		return ExceptionHolder::hold (thiz)->func () ;
	}

	Slice file () const {
		return ExceptionHolder::hold (thiz)->file () ;
	}

	Slice line () const {
		return ExceptionHolder::hold (thiz)->line () ;
	}

	void event () const {
		return ExceptionHolder::hold (thiz)->event () ;
	}

	void raise () const {
		return ExceptionHolder::hold (thiz)->raise () ;
	}
} ;

struct FUNCTION_unimplemented {
	forceinline void operator() () const {
		assume (FALSE) ;
	}
} ;

static constexpr auto unimplemented = FUNCTION_unimplemented () ;

template <class A>
struct OfThis {
	A mThis ;

public:
	forceinline operator A () rightvalue {
		return move (mThis) ;
	}

	using VREF_ITEM = VREF<typeof (nullof (A).self)> ;
	using CREF_ITEM = decltype (keep[TYPE<CREF<A>>::expr] (nullof (A)).self) ;

	XREF<VREF_ITEM> self_m () leftvalue {
		return Pointer::make (address (mThis.self)) ;
	}

	forceinline operator XREF<VREF_ITEM> () leftvalue {
		return self ;
	}

	XREF<CREF_ITEM> self_m () const leftvalue {
		return mThis.self ;
	}

	forceinline operator XREF<CREF_ITEM> () const leftvalue {
		return self ;
	}
} ;

template <class...>
trait UNWARP_SELF_HELP ;

template <class A ,class OTHERWISE>
trait UNWARP_SELF_HELP<A ,OTHERWISE> {
	using RET = A ;
} ;

template <class A>
trait UNWARP_SELF_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).self)>>> {
	using RET = typeof (nullof (A).self) ;
} ;

template <class A>
using UNWARP_SELF = typename UNWARP_SELF_HELP<A ,ALWAYS>::RET ;

struct FriendExternal implement Interface {
	virtual void create (VREF<Pointer> a) const = 0 ;
} ;

template <class A>
class FriendExternalBinder implement Fat<FriendExternal ,A> {
public:
	void create (VREF<Pointer> a) const override {
		using R1X = typeof (A::mThis) ;
		auto &&rax = keep[TYPE<A>::expr] (a) ;
		rax.mThis = R1X::make () ;
	}
} ;

struct ExternalLayout {
	Pin<FatLayout> mImplHolder ;
	Pin<FatLayout> mImplLayout ;
} ;

template <class A ,class B ,class C = UNWARP_SELF<B>>
class External implement ExternalLayout {
public:
	implicit External () = default ;

	template <class ARG1>
	explicit External (CREF<ARG1> holder) :External (holder ,FriendExternalBinder<B> ()) {}

	template <class ARG1 ,class ARG2>
	explicit External (CREF<ARG1> holder ,CREF<ARG2> layout) {
		require (IS_EXTEND<Fat<A ,C> ,ARG1>) ;
		require (ENUM_EQUAL<SIZE_OF<ARG1> ,SIZE_OF<FatLayout>>) ;
		require (ENUM_EQUAL<ALIGN_OF<ARG1> ,ALIGN_OF<FatLayout>>) ;
		require (IS_EXTEND<Fat<FriendExternal ,B> ,ARG2>) ;
		require (ENUM_EQUAL<SIZE_OF<ARG2> ,SIZE_OF<FatLayout>>) ;
		require (ENUM_EQUAL<ALIGN_OF<ARG2> ,ALIGN_OF<FatLayout>>) ;
		inline_memcpy (instance ().mImplHolder ,Pointer::from (holder) ,SIZE_OF<FatLayout>::expr) ;
		inline_memcpy (instance ().mImplLayout ,Pointer::from (layout) ,SIZE_OF<FatLayout>::expr) ;
	}

	static CREF<Fat<A ,C>> declare () {
		return instance ().mImplHolder.self ;
	}

	static B create () {
		B ret ;
		auto &&rax = VFat<FriendExternal> (instance ().mImplLayout.self) ;
		rax->create (Pointer::from (ret)) ;
		return move (ret) ;
	}

private:
	static CREF<External> instance () ;
} ;

template <class A ,class B ,class C>
inline CREF<External<A ,B ,C>> External<A ,B ,C>::instance () {
	return memorize ([&] () {
		return External<A ,B ,C> () ;
	}) ;
}

struct ReflectClone implement Interface {
	virtual void clone (VREF<Pointer> a ,CREF<Pointer> b) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 110 ;
	}
} ;

template <class A>
class ReflectCloneBinder implement ReflectClone {
public:
	void clone (VREF<Pointer> a ,CREF<Pointer> b) const override {
		return inline_clone (keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}
} ;

struct ReflectEqual implement Interface {
	virtual BOOL equal (CREF<Pointer> a ,CREF<Pointer> b) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 111 ;
	}
} ;

template <class A>
class ReflectEqualBinder implement ReflectEqual {
public:
	BOOL equal (CREF<Pointer> a ,CREF<Pointer> b) const override {
		return inline_equal (keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}
} ;

struct ReflectCompr implement Interface {
	virtual FLAG compr (CREF<Pointer> a ,CREF<Pointer> b) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 112 ;
	}
} ;

template <class A>
class ReflectComprBinder implement ReflectCompr {
public:
	FLAG compr (CREF<Pointer> a ,CREF<Pointer> b) const override {
		return inline_compr (keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}
} ;

struct ReflectVisit implement Interface {
	virtual void visit (VREF<VisitorBinder> visitor ,CREF<Pointer> a) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 113 ;
	}
} ;

template <class A>
class ReflectVisitBinder implement ReflectVisit {
public:
	void visit (VREF<VisitorBinder> visitor ,CREF<Pointer> a) const override {
		return inline_visit (visitor ,keep[TYPE<A>::expr] (a)) ;
	}
} ;

struct ReflectGuid implement Interface {
	virtual FLAG type_guid () const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 114 ;
	}
} ;

template <class A>
class ReflectGuidBinder implement ReflectGuid {
public:
	FLAG type_guid () const override {
		return memorize ([&] () {
			return inline_vptr (thiz) ;
		}) ;
	}
} ;

struct ReflectName implement Interface {
	virtual Slice type_name () const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 115 ;
	}
} ;

template <class A>
class ReflectNameBinder implement ReflectName {
public:
	Slice type_name () const override {
		const auto r1x = inline_type_name (Pointer::from (thiz) ,address (__macro_type_rtti)) ;
		return Slice (r1x ,SLICE_MAX_SIZE::expr ,1).eos () ;
	}
} ;

struct ClazzImplLayout ;

struct ClazzLayout {
	Ref<ClazzImplLayout> mThis ;
} ;

struct ClazzHolder implement Interface {
	imports VFat<ClazzHolder> hold (VREF<ClazzLayout> that) ;
	imports CFat<ClazzHolder> hold (CREF<ClazzLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_guid () const = 0 ;
	virtual Slice type_name () const = 0 ;
	virtual BOOL equal (CREF<ClazzLayout> that) const = 0 ;
	virtual FLAG compr (CREF<ClazzLayout> that) const = 0 ;
	virtual void visit (VREF<VisitorBinder> visitor) const = 0 ;
} ;

template <class A>
class ClazzUnknownBinder implement ReflectUnknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectGuidBinder<A>::expr)
			return inline_vptr (ReflectGuidBinder<A> ()) ;
		if (uuid == ReflectNameBinder<A>::expr)
			return inline_vptr (ReflectNameBinder<A> ()) ;
		return ZERO ;
	}
} ;

class Clazz implement ClazzLayout {
protected:
	using ClazzLayout::mThis ;

public:
	implicit Clazz () = default ;

	template <class ARG1>
	explicit Clazz (TYPE<ARG1>) {
		ClazzHolder::hold (thiz)->initialize (ClazzUnknownBinder<ARG1> ()) ;
	}

	LENGTH type_size () const {
		return ClazzHolder::hold (thiz)->type_size () ;
	}

	LENGTH type_align () const {
		return ClazzHolder::hold (thiz)->type_align () ;
	}

	FLAG type_guid () const {
		return ClazzHolder::hold (thiz)->type_guid () ;
	}

	Slice type_name () const {
		return ClazzHolder::hold (thiz)->type_name () ;
	}

	BOOL equal (CREF<Clazz> that) const {
		return ClazzHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Clazz> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Clazz> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Clazz> that) const {
		return ClazzHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Clazz> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Clazz> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Clazz> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Clazz> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const {
		return ClazzHolder::hold (thiz)->visit (visitor) ;
	}
} ;

struct ScopeLayout {
	FLAG mLayout ;

public:
	implicit ScopeLayout () noexcept {
		mLayout = ZERO ;
	}
} ;

template <class A>
class Scope implement ScopeLayout {
protected:
	using ScopeLayout::mLayout ;

public:
	implicit Scope () = delete ;

	explicit Scope (CREF<A> that) {
		const auto r1x = address (that) ;
		auto &&rax = keep[TYPE<CREF<A>>::expr] (Pointer::make (r1x)) ;
		rax.enter () ;
		mLayout = r1x ;
	}

	explicit Scope (RREF<A> that) = delete ;

	implicit ~Scope () noexcept {
		if (mLayout == ZERO)
			return ;
		auto &&rax = keep[TYPE<CREF<A>>::expr] (Pointer::make (mLayout)) ;
		rax.leave () ;
		mLayout = ZERO ;
	}

	implicit Scope (CREF<Scope> that) = delete ;

	forceinline VREF<Scope> operator= (CREF<Scope> that) = delete ;

	implicit Scope (RREF<Scope> that) = delete ;

	forceinline VREF<Scope> operator= (RREF<Scope> that) = delete ;
} ;
} ;