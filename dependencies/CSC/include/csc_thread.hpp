#pragma once

#ifndef __CSC_THREAD__
#define __CSC_THREAD__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
struct CoroutineFriend implement Interface {
	virtual void before () = 0 ;
	virtual BOOL tick () = 0 ;
	virtual void after () = 0 ;
} ;

template <class A>
class CoroutineFriendBinder final implement Fat<CoroutineFriend ,A> {
public:
	static VFat<CoroutineFriend> hold (VREF<A> that) {
		return VFat<CoroutineFriend> (CoroutineFriendBinder () ,that) ;
	}

	void before () override {
		return thiz.fake.before () ;
	}

	BOOL tick () override {
		return thiz.fake.tick () ;
	}

	void after () override {
		return thiz.fake.after () ;
	}
} ;

struct WorkThreadImplLayout ;

struct WorkThreadLayout implement ThisLayout<SharedRef<WorkThreadImplLayout>> {} ;

struct WorkThreadHolder implement Interface {
	imports VFat<WorkThreadHolder> hold (VREF<WorkThreadLayout> that) ;
	imports CFat<WorkThreadHolder> hold (CREF<WorkThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) const = 0 ;
	virtual void set_queue_size (CREF<LENGTH> size_) const = 0 ;
	virtual void start (CREF<Function<CREF<INDEX>>> func) const = 0 ;
	virtual void post (CREF<INDEX> begin_ ,CREF<INDEX> end_) const = 0 ;
	virtual void join () const = 0 ;
	virtual BOOL join (CREF<Time> interval) const = 0 ;
	virtual void stop () const = 0 ;
} ;

class WorkThread implement WorkThreadLayout {
protected:
	using WorkThreadLayout::mThis ;

public:
	implicit WorkThread () = default ;

	implicit WorkThread (CREF<typeof (NULL)>) {
		WorkThreadHolder::hold (thiz)->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const {
		return WorkThreadHolder::hold (thiz)->set_thread_size (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) const {
		return WorkThreadHolder::hold (thiz)->set_queue_size (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) const {
		return WorkThreadHolder::hold (thiz)->start (func) ;
	}

	void post (CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
		return WorkThreadHolder::hold (thiz)->post (begin_ ,end_) ;
	}

	void join () const {
		return WorkThreadHolder::hold (thiz)->join () ;
	}

	BOOL join (CREF<Time> interval) const {
		return WorkThreadHolder::hold (thiz)->join (interval) ;
	}

	void stop () const {
		return WorkThreadHolder::hold (thiz)->stop () ;
	}
} ;

struct CalcSolution {
	INDEX mIteration ;
	FLT64 mAvgError ;
	FLT64 mStdError ;
	BitSet mInput ;
} ;

struct CalcThreadImplLayout ;

struct CalcThreadLayout implement ThisLayout<SharedRef<CalcThreadImplLayout>> {} ;

struct CalcThreadHolder implement Interface {
	imports VFat<CalcThreadHolder> hold (VREF<CalcThreadLayout> that) ;
	imports CFat<CalcThreadHolder> hold (CREF<CalcThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) const = 0 ;
	virtual void set_base_input (CREF<BitSet> base) const = 0 ;
	virtual void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual CalcSolution poll () const = 0 ;
	virtual void suspend () const = 0 ;
	virtual void resume () const = 0 ;
	virtual void stop () const = 0 ;
} ;

class CalcThread implement CalcThreadLayout {
protected:
	using CalcThreadLayout::mThis ;

public:
	implicit CalcThread () = default ;

	implicit CalcThread (CREF<typeof (NULL)>) {
		CalcThreadHolder::hold (thiz)->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const {
		return CalcThreadHolder::hold (thiz)->set_thread_size (size_) ;
	}

	void set_base_input (CREF<BitSet> base) const {
		return CalcThreadHolder::hold (thiz)->set_base_input (base) ;
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const {
		return CalcThreadHolder::hold (thiz)->start (func) ;
	}

	BOOL ready () const {
		return CalcThreadHolder::hold (thiz)->ready () ;
	}

	CalcSolution poll () const {
		return CalcThreadHolder::hold (thiz)->poll () ;
	}

	void suspend () const {
		return CalcThreadHolder::hold (thiz)->suspend () ;
	}

	void resume () const {
		return CalcThreadHolder::hold (thiz)->resume () ;
	}

	void stop () const {
		return CalcThreadHolder::hold (thiz)->stop () ;
	}
} ;

struct PromiseImplLayout ;

struct PromiseLayout implement ThisLayout<SharedRef<PromiseImplLayout>> {} ;

struct PromiseHolder implement Interface {
	imports VFat<PromiseHolder> hold (VREF<PromiseLayout> that) ;
	imports CFat<PromiseHolder> hold (CREF<PromiseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_retry (CREF<BOOL> flag) const = 0 ;
	virtual void start () const = 0 ;
	virtual void start (CREF<Function<>> func) const = 0 ;
	virtual void post (RREF<AutoRef<Pointer>> item) const = 0 ;
	virtual void rethrow (CREF<Exception> e) const = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual BOOL running () const = 0 ;
	virtual AutoRef<Pointer> poll () const = 0 ;
	virtual void future () const = 0 ;
	virtual void stop () const = 0 ;
} ;

template <class A>
class Promise implement PromiseLayout {
protected:
	using PromiseLayout::mThis ;

public:
	implicit Promise () = default ;

	implicit Promise (CREF<typeof (NULL)>) {
		PromiseHolder::hold (thiz)->initialize () ;
	}

	void set_retry (CREF<BOOL> flag) const {
		return PromiseHolder::hold (thiz)->set_retry (flag) ;
	}

	void start () const {
		return PromiseHolder::hold (thiz)->start () ;
	}

	void start (CREF<Function<>> func) const {
		return PromiseHolder::hold (thiz)->start (func) ;
	}

	void post (CREF<A> item) const {
		return post (move (item)) ;
	}

	void post (RREF<A> item) const {
		auto rax = AutoRef<A>::make (move (item)) ;
		return PromiseHolder::hold (thiz)->post (move (rax)) ;
	}

	void rethrow (CREF<Exception> e) const {
		return PromiseHolder::hold (thiz)->rethrow (e) ;
	}

	BOOL ready () const {
		return PromiseHolder::hold (thiz)->ready () ;
	}

	BOOL running () const {
		return PromiseHolder::hold (thiz)->running () ;
	}

	Optional<A> poll () const {
		auto rax = PromiseHolder::hold (thiz)->poll () ;
		if (!rax.exist ())
			return Optional<A>::error (1) ;
		return move (rax.rebind (TYPE<A>::expr).self) ;
	}

	void future () const {
		return PromiseHolder::hold (thiz)->future () ;
	}

	void stop () const {
		return PromiseHolder::hold (thiz)->stop () ;
	}
} ;
} ;