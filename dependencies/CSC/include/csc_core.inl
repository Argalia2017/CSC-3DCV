#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_core.hpp"

#include "csc_end.h"
#ifdef __CSC_SYSTEM_WINDOWS__
#include <debugapi.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <malloc.h>
#include <typeinfo>
#include <initializer_list>
#include <atomic>
#include "csc_begin.h"

namespace CSC {
#ifdef __CSC_SYSTEM_WINDOWS__
struct FUNCTION_has_debugger {
	forceinline BOOL operator() () const noexcept {
		return IsDebuggerPresent () ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_has_debugger {
	forceinline BOOL operator() () const noexcept {
		return FALSE ;
	}
} ;
#endif

static constexpr auto has_debugger = FUNCTION_has_debugger () ;

exports BOOL FUNCTION_inline_unittest::invoke () {
	return memorize ([&] () {
		return has_debugger () ;
	}) ;
} ;

exports void FUNCTION_inline_abort::invoke () {
	std::abort () ;
} ;

exports void FUNCTION_inline_watch::invoke (VREF<Pointer> src) {
	//@warn: make side effect
	inline_memcpy (src ,src ,1) ;
}

#ifdef __CSC_CXX_RTTI__
struct FUNCTION_stl_type_name {
	forceinline FLAG operator() (CREF<Interface> squalor ,CREF<FLAG> func_) const noexcept {
		return FLAG (typeid (squalor).name ()) ;
	}
} ;
#endif

#ifndef __CSC_CXX_RTTI__
#ifdef __CSC_COMPILER_NVCC__
#pragma message "NVCC would not generate type_name without rtti"
#endif

struct FUNCTION_stl_type_name {
	forceinline FLAG operator() (CREF<Interface> squalor ,CREF<FLAG> func_) const noexcept {
		return func_ ;
	}
} ;
#endif

static constexpr auto stl_type_name = FUNCTION_stl_type_name () ;

exports FLAG FUNCTION_inline_type_name::invoke (CREF<Pointer> squalor ,CREF<FLAG> func_) {
	return stl_type_name (squalor ,func_) ;
}

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_stl_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) const noexcept {
		Tuple<FLAG ,FLAG> ret ;
		ret.m1st = FLAG (squalor.begin ()) ;
		ret.m2nd = FLAG (squalor.end ()) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_stl_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) const noexcept {
		Tuple<FLAG ,FLAG> ret ;
		ret.m1st = FLAG (squalor.begin ()) ;
		ret.m2nd = FLAG (squalor.begin ()) + LENGTH (squalor.size ()) * step_ ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_stl_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) const noexcept {
		Tuple<FLAG ,FLAG> ret ;
		ret.m1st = FLAG (squalor.begin ()) ;
		ret.m2nd = FLAG (squalor.end ()) ;
		return move (ret) ;
	}
} ;
#endif

static constexpr auto stl_list_pair = FUNCTION_stl_list_pair () ;

exports Tuple<FLAG ,FLAG> FUNCTION_inline_list_pair::invoke (CREF<Pointer> squalor ,CREF<LENGTH> step_) {
	return stl_list_pair (squalor ,step_) ;
}

exports void FUNCTION_inline_memset::invoke (VREF<Pointer> dst ,CREF<LENGTH> size_) {
	std::memset ((&dst) ,0 ,size_) ;
}

exports void FUNCTION_inline_memcpy::invoke (VREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) {
	std::memcpy ((&dst) ,(&src) ,size_) ;
}

exports FLAG FUNCTION_inline_memcmp::invoke (CREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) {
	return FLAG (std::memcmp ((&dst) ,(&src) ,size_)) ;
}

class BoxImplHolder final implement Fat<BoxHolder ,BoxLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		assert (!exist ()) ;
		fake.mHolder = inline_vptr (holder) ;
	}

	void destroy () override {
		if (fake.mHolder == ZERO)
			return ;
		const auto r1x = RFat<ReflectDestroy> (unknown ()) ;
		r1x->destroy (self ,1) ;
		fake.mHolder = ZERO ;
	}

	BOOL exist () const override {
		return fake.mHolder != ZERO ;
	}

	Unknown unknown () const override {
		assert (exist ()) ;
		return Unknown (fake.mHolder) ;
	}

	VREF<Pointer> self_m () leftvalue override {
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		//@warn: different from mStorage due to alignment
		const auto r2x = address (fake) + SIZE_OF<BoxLayout>::expr ;
		const auto r3x = inline_alignas (r2x ,r1x->type_align ()) ;
		return Pointer::make (r3x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		//@warn: different from mStorage due to alignment
		const auto r2x = address (fake) + SIZE_OF<BoxLayout>::expr ;
		const auto r3x = inline_alignas (r2x ,r1x->type_align ()) ;
		return Pointer::make (r3x) ;
	}

	void remake (CREF<Unknown> holder) override {
		if (exist ())
			return ;
		fake.mHolder = inline_vptr (holder) ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		inline_memset (self ,r1x->type_size ()) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		assert (!exist ()) ;
		if (that.mHolder == ZERO)
			return ;
		fake.mHolder = that.mHolder ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		inline_memcpy (self ,BoxHolder::hold (that)->self ,r1x->type_size ()) ;
	}

	void release () override {
		fake.mHolder = ZERO ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::hold (VREF<BoxLayout> that) {
	return VFat<BoxHolder> (BoxImplHolder () ,that) ;
}

exports CFat<BoxHolder> BoxHolder::hold (CREF<BoxLayout> that) {
	return CFat<BoxHolder> (BoxImplHolder () ,that) ;
}

struct RefImplLayout implement Proxy {
	Heap mHeap ;
	std::atomic<VAL> mCounter ;
	BoxLayout mValue ;
} ;

static constexpr auto REFIMPLLAYOUT_MIN_HANDLE = 8 ;

class RefImplHolder final implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (BoxHolder::hold (item)->unknown ()) ;
		const auto r2x = inline_max (r1x->type_align () - ALIGN_OF<RefImplLayout>::expr ,0) ;
		const auto r3x = SIZE_OF<RefImplLayout>::expr + r2x + r1x->type_size () ;
		const auto r4x = Heap::instance () ;
		fake.mHandle = r4x.alloc (r3x) ;
		assert (fake.mHandle >= REFIMPLLAYOUT_MIN_HANDLE) ;
		inline_memset (Pointer::make (fake.mHandle) ,SIZE_OF<RefImplLayout>::expr) ;
		ptr (fake).mHeap = r4x ;
		BoxHolder::hold (ptr (fake).mValue)->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		fake.mLayout = address (BoxHolder::hold (ptr (fake).mValue)->self) ;
		ptr (fake).mCounter = 1 ;
	}

	void initialize (CREF<RefLayout> that) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (that.mHandle < REFIMPLLAYOUT_MIN_HANDLE)
				discard ;
			const auto r1x = ++ptr (that).mCounter ;
			noop (r1x) ;
			assert (r1x >= 2) ;
			fake.mHandle = that.mHandle ;
			fake.mLayout = address (BoxHolder::hold (ptr (that).mValue)->self) ;
		}
		if ifdo (act) {
			fake.mHandle = that.mHandle ;
			fake.mLayout = that.mLayout ;
		}
	}

	void initialize (CREF<Unknown> holder ,CREF<Unknown> extend ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = RFat<ReflectSize> (extend) ;
		const auto r3x = inline_max (r1x->type_align () - ALIGN_OF<RefImplLayout>::expr ,0) ;
		const auto r4x = SIZE_OF<RefImplLayout>::expr + r3x + r1x->type_size () ;
		const auto r5x = inline_max (r2x->type_align () - r1x->type_align () ,0) ;
		const auto r6x = r4x + r5x + r2x->type_size () * size_ ;
		const auto r7x = Heap::instance () ;
		fake.mHandle = r7x.alloc (r6x) ;
		assert (fake.mHandle >= REFIMPLLAYOUT_MIN_HANDLE) ;
		inline_memset (Pointer::make (fake.mHandle) ,SIZE_OF<RefImplLayout>::expr) ;
		ptr (fake).mHeap = r7x ;
		BoxHolder::hold (ptr (fake).mValue)->initialize (holder) ;
		fake.mLayout = address (BoxHolder::hold (ptr (fake).mValue)->self) ;
		const auto r8x = RFat<ReflectCreate> (holder) ;
		r8x->create (self ,1) ;
		ptr (fake).mCounter = 1 ;
	}

	void initialize (CREF<Unknown> holder ,CREF<FLAG> layout) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_align () ;
		noop (r2x) ;
		assert (r2x <= ALIGN_OF<RefImplLayout>::expr) ;
		const auto r3x = layout - SIZE_OF<RefImplLayout>::expr ;
		auto &&rax = keep[TYPE<RefImplLayout>::expr] (Pointer::make (r3x)) ;
		assert (rax.mCounter > 0) ;
		fake.mHandle = r3x ;
		fake.mLayout = layout ;
		ptr (fake).mCounter++ ;
	}

	void destroy () override {
		if (fake.mLayout == ZERO)
			return ;
		if ifdo (TRUE) {
			if (fake.mHandle < REFIMPLLAYOUT_MIN_HANDLE)
				discard ;
			assert (fake.mLayout != ZERO) ;
			if ifdo (TRUE) {
				const auto r1x = --ptr (fake).mCounter ;
				if (r1x > 0)
					discard ;
				BoxHolder::hold (ptr (fake).mValue)->destroy () ;
				const auto r2x = ptr (fake).mHeap ;
				r2x.free (fake.mHandle) ;
			}
		}
		fake.mHandle = ZERO ;
		fake.mLayout = ZERO ;
	}

	static VREF<RefImplLayout> ptr (CREF<RefLayout> that) {
		return Pointer::make (that.mHandle) ;
	}

	BOOL exist () const override {
		return fake.mLayout != ZERO ;
	}

	Unknown unknown () const override {
		assert (exist ()) ;
		return BoxHolder::hold (ptr (fake).mValue)->unknown () ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mLayout) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mLayout) ;
	}

	BOOL exclusive () const override {
		assert (exist ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mHandle < REFIMPLLAYOUT_MIN_HANDLE)
				discard ;
			const auto r1x = ptr (fake).mCounter.load () ;
			return r1x == IDEN ;
		}
		if ifdo (act) {
			//@warn: reference instance are always unchecked except cv-qualifier
			if (fake.mHandle != VARIABLE::expr)
				discard ;
			return TRUE ;
		}
		return FALSE ;
	}
} ;

exports VFat<RefHolder> RefHolder::hold (VREF<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,that) ;
}

exports CFat<RefHolder> RefHolder::hold (CREF<RefLayout> that) {
	return CFat<RefHolder> (RefImplHolder () ,that) ;
}

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		noop () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		noop () ;
	}
} ;
#endif

static constexpr auto dump_memory_leaks = FUNCTION_dump_memory_leaks () ;

#ifdef __CSC_SYSTEM_WINDOWS__
struct FUNCTION_memsize {
	forceinline LENGTH operator() (CREF<csc_pointer_t> a) const {
		return LENGTH (_msize (a)) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_memsize {
	forceinline LENGTH operator() (CREF<csc_pointer_t> a) const {
		return LENGTH (malloc_usable_size (a)) ;
	}
} ;
#endif

static constexpr auto memsize = FUNCTION_memsize () ;

struct HeapRoot {
	Box<std::atomic<VAL>> mStack ;
	Box<std::atomic<VAL>> mLength ;
} ;

class HeapImplHolder final implement Fat<HeapHolder ,HeapLayout> {
public:
	void initialize () override {
		root_ptr (fake).mStack.remake () ;
		root_ptr (fake).mLength.remake () ;
		dump_memory_leaks () ;
	}

	static VREF<HeapRoot> root_ptr (CREF<HeapLayout> that) {
		return memorize ([&] () {
			return Pin<HeapRoot> () ;
		}).self ;
	}

	INDEX stack () const override {
		INDEX ret = root_ptr (fake).mStack.self++ ;
		if ifdo (TRUE) {
			if (ret >= 0)
				discard ;
			ret = ret & VAL_MAX ;
		}
		return move (ret) ;
	}

	LENGTH length () const override {
		return root_ptr (fake).mLength.self ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_ ,std::nothrow)) ;
		assume (ret != ZERO) ;
		const auto r1x = csc_pointer_t (ret) ;
		const auto r2x = memsize (r1x) ;
		root_ptr (fake).mLength.self += r2x ;
		return move (ret) ;
	}

	void free (CREF<FLAG> layout) const override {
		const auto r1x = csc_pointer_t (layout) ;
		const auto r2x = memsize (r1x) ;
		root_ptr (fake).mLength.self -= r2x ;
		operator delete (r1x ,std::nothrow) ;
	}
} ;

exports CREF<HeapLayout> HeapHolder::instance () {
	return memorize ([&] () {
		HeapLayout ret ;
		ret.mHolder = inline_vptr (HeapImplHolder ()) ;
		HeapHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<HeapHolder> HeapHolder::hold (VREF<HeapLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return VFat<HeapHolder> (rax ,that) ;
}

exports CFat<HeapHolder> HeapHolder::hold (CREF<HeapLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return CFat<HeapHolder> (rax ,that) ;
}

struct KeyNodeLayout implement Proxy {
	FLAG mHead ;
	FLAG mLayout ;
	INDEX mCheck ;
} ;

struct KeyHeadLayout implement Proxy {
	FLAG mRoot ;
	FLAG mL1Next ;
	FLAG mL2Next ;
	FLAG mL1Hash ;
	FLAG mL2Hash ;
	FLAG mBegin ;
	FLAG mEnd ;
} ;

struct KeyRootLayout implement Proxy {
	FLAG mHead ;
	FLAG mRefNode ;
	FLAG mDefNode ;
	Heap mHeap ;
	BOOL mFinalize ;
} ;

class KeyBaseImplHolder final implement Fat<KeyBaseHolder ,KeyBaseLayout> {
private:
	using KEYHEAD_LEVEL1_SIZE = ENUM<1024> ;
	using KEYHEAD_LEVEL2_SIZE = ENUM<64> ;

public:
	void initialize () override {
		if (fake.mHandle != ZERO)
			return ;
		auto rax = ZERO ;
		auto rbx = ZERO ;
		if ifdo (TRUE) {
			const auto r1x = Heap::instance () ;
			rax = r1x.alloc (SIZE_OF<KeyRootLayout>::expr) ;
			root_ptr (rax).mHeap = r1x ;
			root_ptr (rax).mFinalize = FALSE ;
			const auto r2x = 2 * SIZE_OF<KeyNodeLayout>::expr ;
			const auto r3x = SIZE_OF<KeyHeadLayout>::expr + r2x ;
			rbx = r1x.alloc (r3x) ;
			inline_memset (Pointer::make (rbx) ,r3x) ;
			const auto r4x = rbx + SIZE_OF<KeyHeadLayout>::expr ;
			const auto r5x = r4x + SIZE_OF<KeyNodeLayout>::expr ;
			head_ptr (rbx).mRoot = rax ;
			head_ptr (rbx).mL1Next = rbx ;
			head_ptr (rbx).mL2Next = rbx ;
			head_ptr (rbx).mL1Hash = NONE ;
			head_ptr (rbx).mL2Hash = NONE ;
			head_ptr (rbx).mBegin = r4x ;
			head_ptr (rbx).mEnd = r4x + r2x ;
			node_ptr (r4x).mHead = rbx ;
			node_ptr (r4x).mCheck = NONE ;
			node_ptr (r5x).mHead = rbx ;
			node_ptr (r5x).mCheck = NONE ;
			root_ptr (rax).mHead = rbx ;
			root_ptr (rax).mRefNode = r4x ;
			root_ptr (rax).mDefNode = r5x ;
		}
		fake.mHandle = root_ptr (rax).mRefNode ;
	}

	void initialize (CREF<KeyBaseLayout> root ,CREF<FLAG> layout) override {
		assert (fake.mHandle == ZERO) ;
		assert (address (fake) == layout) ;
		const auto r1x = node_ptr (root.mHandle).mHead ;
		const auto r2x = head_ptr (r1x).mRoot ;
		const auto r3x = root_ptr (r2x).mDefNode ;
		fake.mHandle = r3x ;
	}

	void destroy () override {
		if (fake.mHandle == ZERO)
			return ;
		const auto r1x = node_ptr (fake.mHandle).mHead ;
		const auto r2x = head_ptr (r1x).mRoot ;
		const auto r3x = root_ptr (r2x).mDefNode ;
		node_ptr (fake.mHandle).mLayout = node_ptr (r3x).mLayout ;
		node_ptr (fake.mHandle).mCheck++ ;
		if ifdo (TRUE) {
			const auto r4x = root_ptr (r2x).mRefNode ;
			if (fake.mHandle != r4x)
				discard ;
			root_ptr (r2x).mFinalize = TRUE ;
			remove_all_head (r2x) ;
			remove_root (r2x) ;
		}
		fake.mHandle = ZERO ;
	}

	static VREF<KeyNodeLayout> node_ptr (CREF<FLAG> handle) {
		return Pointer::make (handle) ;
	}

	static VREF<KeyHeadLayout> head_ptr (CREF<FLAG> handle) {
		return Pointer::make (handle) ;
	}

	static VREF<KeyRootLayout> root_ptr (CREF<FLAG> handle) {
		return Pointer::make (handle) ;
	}

	INDEX get_index () const override {
		if (fake.mHandle == ZERO)
			return NONE ;
		const auto r1x = node_ptr (fake.mHandle).mHead ;
		if (head_ptr (r1x).mL1Hash < 0)
			return NONE ;
		const auto r2x = (fake.mHandle - head_ptr (r1x).mBegin) / SIZE_OF<KeyNodeLayout>::expr ;
		const auto r3x = head_ptr (r1x).mL1Hash * KEYHEAD_LEVEL1_SIZE::expr ;
		return r3x + r2x ;
	}

	INDEX get_check () const override {
		if (fake.mHandle == ZERO)
			return ZERO ;
		return node_ptr (fake.mHandle).mCheck ;
	}

	void set_key (CREF<INDEX> index) override {
		assert (fake.mHandle != ZERO) ;
		const auto r1x = spwan (index) ;
		node_ptr (r1x).mLayout = address (fake) ;
		fake.mHandle = r1x ;
	}

	void set_def () override {
		assert (fake.mHandle != ZERO) ;
		const auto r1x = node_ptr (fake.mHandle).mHead ;
		const auto r2x = head_ptr (r1x).mRoot ;
		const auto r3x = root_ptr (r2x).mDefNode ;
		node_ptr (r3x).mLayout = address (fake) ;
		fake.mHandle = r3x ;
	}

	VREF<Pointer> lock (CREF<INDEX> check) leftvalue override {
		assert (fake.mHandle != ZERO) ;
		const auto r1x = node_ptr (fake.mHandle).mLayout ;
		const auto r2x = node_ptr (fake.mHandle).mCheck ;
		if ifdo (TRUE) {
			if (r2x >= 0)
				if (r2x == check)
					discard ;
			const auto r3x = node_ptr (fake.mHandle).mHead ;
			const auto r4x = head_ptr (r3x).mRoot ;
			const auto r5x = root_ptr (r4x).mDefNode ;
			fake.mHandle = r5x ;
			const auto r6x = node_ptr (fake.mHandle).mLayout ;
			return Pointer::make (r6x) ;
		}
		return Pointer::make (r1x) ;
	}

	FLAG spwan () const override {
		assert (fake.mHandle != ZERO) ;
		return fake.mHandle ;
	}

	FLAG spwan (CREF<INDEX> index) const override {
		assert (fake.mHandle != ZERO) ;
		const auto r1x = node_ptr (fake.mHandle).mHead ;
		const auto r2x = head_ptr (r1x).mRoot ;
		const auto r3x = root_ptr (r2x).mDefNode ;
		if (index < 0)
			return r3x ;
		assert (!root_ptr (r2x).mFinalize) ;
		const auto r4x = index / KEYHEAD_LEVEL1_SIZE::expr ;
		const auto r5x = index % KEYHEAD_LEVEL1_SIZE::expr ;
		const auto r6x = insert_head (r4x ,r2x) ;
		const auto r7x = head_ptr (r6x).mBegin + r5x * SIZE_OF<KeyNodeLayout>::expr ;
		if ifdo (TRUE) {
			if (node_ptr (r7x).mHead != ZERO)
				discard ;
			node_ptr (r7x).mHead = r6x ;
			node_ptr (r7x).mLayout = node_ptr (r3x).mLayout ;
		}
		return r7x ;
	}

	FLAG insert_head (CREF<FLAG> hash ,CREF<FLAG> root) const {
		const auto r1x = hash / KEYHEAD_LEVEL2_SIZE::expr ;
		const auto r2x = root_ptr (root).mHead ;
		auto rax = r2x ;
		while (TRUE) {
			const auto r3x = head_ptr (rax).mL2Next ;
			if (r3x == r2x)
				break ;
			if (head_ptr (r3x).mL2Hash >= r1x)
				break ;
			rax = r3x ;
		}
		auto rbx = rax ;
		while (TRUE) {
			const auto r4x = head_ptr (rbx).mL1Next ;
			if (r4x == r2x)
				break ;
			if (head_ptr (r4x).mL1Hash >= hash)
				break ;
			rbx = r4x ;
		}
		FLAG ret = head_ptr (rbx).mL1Next ;
		if ifdo (TRUE) {
			if (head_ptr (ret).mL1Hash == hash)
				discard ;
			const auto r5x = root_ptr (root).mHeap ;
			const auto r6x = KEYHEAD_LEVEL1_SIZE::expr * SIZE_OF<KeyNodeLayout>::expr ;
			const auto r7x = SIZE_OF<KeyHeadLayout>::expr + r6x ;
			ret = r5x.alloc (r7x) ;
			inline_memset (Pointer::make (ret) ,r7x) ;
			const auto r8x = ret + SIZE_OF<KeyHeadLayout>::expr ;
			head_ptr (ret).mRoot = root ;
			head_ptr (ret).mL1Next = head_ptr (rbx).mL1Next ;
			head_ptr (rbx).mL1Next = ret ;
			const auto r9x = head_ptr (rax).mL2Next ;
			auto act = TRUE ;
			if ifdo (act) {
				if (head_ptr (r9x).mL2Hash == r1x)
					discard ;
				head_ptr (ret).mL2Next = r9x ;
				head_ptr (rax).mL2Next = ret ;
			}
			if ifdo (act) {
				if (head_ptr (r9x).mL1Hash <= hash)
					discard ;
				head_ptr (ret).mL2Next = head_ptr (r9x).mL2Next ;
				head_ptr (rax).mL2Next = ret ;
			}
			if ifdo (act) {
				head_ptr (ret).mL2Next = head_ptr (r9x).mL2Next ;
			}
			head_ptr (ret).mL1Hash = hash ;
			head_ptr (ret).mL2Hash = r1x ;
			head_ptr (ret).mBegin = r8x ;
			head_ptr (ret).mEnd = r8x + r6x ;
		}
		return move (ret) ;
	}

	void remove_root (CREF<FLAG> root) {
		const auto r1x = root_ptr (root).mHeap ;
		r1x.free (root) ;
	}

	void remove_all_head (CREF<FLAG> root) {
		const auto r1x = root_ptr (root).mHead ;
		const auto r2x = root_ptr (root).mHeap ;
		auto rax = r1x ;
		while (TRUE) {
			const auto r3x = head_ptr (rax).mL1Next ;
			r2x.free (rax) ;
			rax = r3x ;
			if (rax == r1x)
				break ;
		}
	}
} ;

exports VFat<KeyBaseHolder> KeyBaseHolder::hold (VREF<KeyBaseLayout> that) {
	return VFat<KeyBaseHolder> (KeyBaseImplHolder () ,that) ;
}

exports CFat<KeyBaseHolder> KeyBaseHolder::hold (CREF<KeyBaseLayout> that) {
	return CFat<KeyBaseHolder> (KeyBaseImplHolder () ,that) ;
}

class SliceImplHolder final implement Fat<SliceHolder ,SliceLayout> {
public:
	void initialize (CREF<FLAG> buffer ,CREF<LENGTH> size_ ,CREF<LENGTH> step_) override {
		fake.mBuffer = buffer ;
		fake.mSize = size_ ;
		fake.mStep = step_ ;
	}

	LENGTH size () const override {
		return fake.mSize ;
	}

	LENGTH step () const override {
		return fake.mStep ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStep != SIZE_OF<STRU8>::expr)
				discard ;
			item = bitwise[TYPE<STRU8>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (fake.mStep != SIZE_OF<STRU16>::expr)
				discard ;
			item = bitwise[TYPE<STRU16>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (fake.mStep != SIZE_OF<STRU32>::expr)
				discard ;
			item = bitwise[TYPE<STRU32>::expr] (at (index)) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	BOOL equal (CREF<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::hold (that)->size () ;
		if (r1x != r2x)
			return FALSE ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			SliceHolder::hold (that)->get (i ,rbx) ;
			const auto r3x = inline_equal (rax ,rbx) ;
			if (!r3x)
				return r3x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::hold (that)->size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			SliceHolder::hold (that)->get (i ,rbx) ;
			const auto r4x = inline_compr (rax ,rbx) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		const auto r1x = size () ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			inline_visit (visitor ,rax) ;
		}
		visitor.leave () ;
	}

	SliceLayout eos () const override {
		SliceLayout ret = fake ;
		INDEX ix = 0 ;
		auto rax = STRU32 () ;
		while (TRUE) {
			if (ix >= fake.mSize)
				break ;
			get (ix ,rax) ;
			if (rax == STRU32 (0X00))
				break ;
			ix++ ;
		}
		ret.mSize = ix ;
		return move (ret) ;
	}
} ;

exports VFat<SliceHolder> SliceHolder::hold (VREF<SliceLayout> that) {
	return VFat<SliceHolder> (SliceImplHolder () ,that) ;
}

exports CFat<SliceHolder> SliceHolder::hold (CREF<SliceLayout> that) {
	return CFat<SliceHolder> (SliceImplHolder () ,that) ;
}

class ExceptionImplHolder final implement Fat<ExceptionHolder ,ExceptionLayout> {
public:
	void initialize (CREF<Slice> what_ ,CREF<Slice> func_ ,CREF<Slice> file_ ,CREF<Slice> line_) override {
		fake.mWhat = what_ ;
		fake.mFunc = func_ ;
		fake.mFile = file_ ;
		fake.mLine = line_ ;
	}

	Slice what () const override {
		return fake.mWhat ;
	}

	Slice func () const override {
		return fake.mFunc ;
	}

	Slice file () const override {
		return fake.mFile ;
	}

	Slice line () const override {
		return fake.mLine ;
	}

	void event () const override {
		unimplemented () ;
	}

	void raise () const override {
		auto &&rax = keep[TYPE<Exception>::expr] (fake) ;
		throw rax ;
	}
} ;

exports VFat<ExceptionHolder> ExceptionHolder::hold (VREF<ExceptionLayout> that) {
	return VFat<ExceptionHolder> (ExceptionImplHolder () ,that) ;
}

exports CFat<ExceptionHolder> ExceptionHolder::hold (CREF<ExceptionLayout> that) {
	return CFat<ExceptionHolder> (ExceptionImplHolder () ,that) ;
}

struct ClazzImplLayout {
	LENGTH mTypeSize ;
	LENGTH mTypeAlign ;
	FLAG mTypeGuid ;
	Slice mTypeName ;
} ;

class ClazzImplHolder final implement Fat<ClazzHolder ,ClazzLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		fake.mThis = Ref<ClazzImplLayout>::make () ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		fake.mThis->mTypeSize = r1x->type_size () ;
		fake.mThis->mTypeAlign = r1x->type_align () ;
		const auto r2x = RFat<ReflectGuid> (holder) ;
		fake.mThis->mTypeGuid = r2x->type_guid () ;
		const auto r3x = RFat<ReflectName> (holder) ;
		fake.mThis->mTypeName = r3x->type_name () ;
	}

	LENGTH type_size () const override {
		if (fake.mThis == NULL)
			return 0 ;
		return fake.mThis->mTypeSize ;
	}

	LENGTH type_align () const override {
		if (fake.mThis == NULL)
			return 0 ;
		return fake.mThis->mTypeAlign ;
	}

	FLAG type_guid () const override {
		if (fake.mThis == NULL)
			return ZERO ;
		return fake.mThis->mTypeGuid ;
	}

	Slice type_name () const override {
		if (fake.mThis == NULL)
			return Slice () ;
		return fake.mThis->mTypeName ;
	}

	BOOL equal (CREF<ClazzLayout> that) const override {
		if (type_size () != ClazzHolder::hold (that)->type_size ())
			return FALSE ;
		if (type_align () != ClazzHolder::hold (that)->type_align ())
			return FALSE ;
		if (type_guid () != 0)
			if (type_guid () == ClazzHolder::hold (that)->type_guid ())
				return TRUE ;
		return inline_equal (type_name () ,ClazzHolder::hold (that)->type_name ()) ;
	}

	FLAG compr (CREF<ClazzLayout> that) const override {
		if (type_guid () != 0)
			if (type_guid () == ClazzHolder::hold (that)->type_guid ())
				return ZERO ;
		return inline_compr (type_name () ,ClazzHolder::hold (that)->type_name ()) ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		inline_visit (visitor ,type_size ()) ;
		inline_visit (visitor ,type_align ()) ;
		inline_visit (visitor ,type_guid ()) ;
		inline_visit (visitor ,type_name ()) ;
		visitor.leave () ;
	}
} ;

exports VFat<ClazzHolder> ClazzHolder::hold (VREF<ClazzLayout> that) {
	return VFat<ClazzHolder> (ClazzImplHolder () ,that) ;
}

exports CFat<ClazzHolder> ClazzHolder::hold (CREF<ClazzLayout> that) {
	return CFat<ClazzHolder> (ClazzImplHolder () ,that) ;
}
} ;