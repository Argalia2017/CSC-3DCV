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
struct CoroutineBinder implement Interface {
	virtual void before () = 0 ;
	virtual BOOL tick (CREF<FLT64> deltatime) = 0 ;
	virtual BOOL idle () = 0 ;
	virtual void after () = 0 ;
	virtual void execute () = 0 ;
} ;

template <class A>
class FriendCoroutineBinder final implement Fat<CoroutineBinder ,A> {
public:
	static VFat<CoroutineBinder> hold (VREF<A> that) {
		return VFat<CoroutineBinder> (FriendCoroutineBinder () ,that) ;
	}

	void before () override {
		return thiz.self.before () ;
	}

	BOOL tick (CREF<FLT64> deltatime) override {
		return thiz.self.tick (deltatime) ;
	}

	BOOL idle () override {
		return thiz.self.idle () ;
	}

	void after () override {
		return thiz.self.after () ;
	}

	void execute () override {
		thiz.self.before () ;
		while (TRUE) {
			while (thiz.self.tick (0)) ;
			if (!thiz.self.idle ())
				break ;
		}
		thiz.self.after () ;
	}
} ;

struct WorkThreadLayout ;

struct WorkThreadHolder implement Interface {
	imports OfThis<SharedRef<WorkThreadLayout>> create () ;
	imports VFat<WorkThreadHolder> hold (VREF<WorkThreadLayout> that) ;
	imports CFat<WorkThreadHolder> hold (CREF<WorkThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) = 0 ;
	virtual void set_queue_size (CREF<LENGTH> size_) = 0 ;
	virtual void start (CREF<Function<CREF<INDEX>>> func) = 0 ;
	virtual void friend_execute (CREF<INDEX> slot) = 0 ;
	virtual void post (CREF<INDEX> begin_ ,CREF<INDEX> end_) = 0 ;
	virtual void join () = 0 ;
	virtual BOOL join (CREF<Time> interval) = 0 ;
	virtual void stop () = 0 ;
} ;

class WorkThread implement OfThis<SharedRef<WorkThreadLayout>> {
public:
	implicit WorkThread () = default ;

	implicit WorkThread (CREF<typeof (NULL)>) {
		mThis = WorkThreadHolder::create () ;
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

struct CalcThreadLayout ;

struct CalcThreadHolder implement Interface {
	imports OfThis<SharedRef<CalcThreadLayout>> create () ;
	imports VFat<CalcThreadHolder> hold (VREF<CalcThreadLayout> that) ;
	imports CFat<CalcThreadHolder> hold (CREF<CalcThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) = 0 ;
	virtual void set_start_input (CREF<BitSet> input ,CREF<FLT64> factor) = 0 ;
	virtual void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) = 0 ;
	virtual void friend_execute (CREF<INDEX> slot) = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual CalcSolution poll () = 0 ;
	virtual void suspend () = 0 ;
	virtual void resume () = 0 ;
	virtual void stop () = 0 ;
} ;

class CalcThread implement OfThis<SharedRef<CalcThreadLayout>> {
public:
	implicit CalcThread () = default ;

	implicit CalcThread (CREF<typeof (NULL)>) {
		mThis = CalcThreadHolder::create () ;
		CalcThreadHolder::hold (thiz)->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const {
		return CalcThreadHolder::hold (thiz)->set_thread_size (size_) ;
	}

	void set_start_input (CREF<BitSet> input ,CREF<FLT64> factor) const {
		return CalcThreadHolder::hold (thiz)->set_start_input (input ,factor) ;
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

struct PromiseLayout ;

struct PromiseHolder implement Interface {
	imports OfThis<SharedRef<PromiseLayout>> create () ;
	imports VFat<PromiseHolder> hold (VREF<PromiseLayout> that) ;
	imports CFat<PromiseHolder> hold (CREF<PromiseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_retry (CREF<BOOL> flag) = 0 ;
	virtual void start () = 0 ;
	virtual void start (CREF<Function<>> func) = 0 ;
	virtual void friend_execute (CREF<INDEX> slot) = 0 ;
	virtual void post (RREF<AutoRef<Pointer>> item) = 0 ;
	virtual void rethrow (CREF<Exception> e) = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual BOOL running () const = 0 ;
	virtual AutoRef<Pointer> poll () = 0 ;
	virtual void future () = 0 ;
	virtual void stop () = 0 ;
} ;

template <class A>
class Promise implement OfThis<SharedRef<PromiseLayout>> {
public:
	implicit Promise () = default ;

	implicit Promise (CREF<typeof (NULL)>) {
		mThis = PromiseHolder::create () ;
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
		return move (rax.rebind (TYPE<A>::expr).deref) ;
	}

	void future () const {
		return PromiseHolder::hold (thiz)->future () ;
	}

	void stop () const {
		return PromiseHolder::hold (thiz)->stop () ;
	}
} ;
} ;