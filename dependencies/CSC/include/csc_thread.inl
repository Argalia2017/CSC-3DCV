#pragma once

#ifndef __CSC_THREAD__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_thread.hpp"

namespace CSC {
struct ThreadFlag {
	enum {
		Preparing ,
		Running ,
		Finishing ,
		Closing ,
		ETC
	} ;
} ;

struct WorkThreadImplLayout {
	Mutex mThreadMutex ;
	Just<ThreadFlag> mThreadFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Array<IndexIterator> mThreadQueue ;
	Array<LENGTH> mThreadLoadLength ;
	Function<CREF<INDEX>> mThreadFunc ;
	Deque<IndexIterator> mItemQueue ;
	LENGTH mItemLoadLength ;

public:
	implicit WorkThreadImplLayout () = default ;

	implicit ~WorkThreadImplLayout () noexcept {
		WorkThreadHolder::hold (thiz)->stop () ;
	}

	void friend_execute (CREF<INDEX> slot) {
		WorkThreadHolder::hold (thiz)->friend_execute (slot) ;
	}
} ;

class WorkThreadImplHolder final implement Fat<WorkThreadHolder ,WorkThreadImplLayout> {
private:
	using THREAD_QUEUE_SIZE = ENUM<65536> ;

public:
	void initialize () override {
		fake.mThreadMutex = UniqueMutex () ;
		fake.mThreadFlag = ThreadFlag::Preparing ;
		const auto r1x = RuntimeProc::thread_concurrency () ;
		set_thread_size (r1x) ;
		set_queue_size (r1x) ;
	}

	void set_thread_size (CREF<LENGTH> size_) override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Preparing) ;
		fake.mThread = Array<Thread> (size_) ;
		fake.mThreadJoin = BitSet (size_) ;
		fake.mThreadQueue = Array<IndexIterator> (size_) ;
		fake.mThreadLoadLength = Array<LENGTH> (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) override {
		assert (size_ > 0) ;
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Preparing) ;
		assume (fake.mItemQueue.empty ()) ;
		fake.mItemQueue = Deque<IndexIterator> (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Preparing) ;
		assume (fake.mThread.size () > 0) ;
		fake.mThreadLoadLength.fill (0) ;
		fake.mItemLoadLength = 0 ;
		fake.mThreadFlag = ThreadFlag::Running ;
		fake.mThreadFunc = func ;
		const auto r1x = FriendThreadBinder<WorkThreadImplLayout>::hold (fake) ;
		for (auto &&i : fake.mThread.range ()) {
			fake.mThread[i] = Thread (Box<VFat<ThreadBinder>>::make (r1x) ,i) ;
			fake.mThread[i].start () ;
		}
	}

	void friend_execute (CREF<INDEX> slot) override {
		try {
			while (TRUE) {
				poll (slot) ;
				for (auto &&i : fake.mThreadQueue[slot]) {
					fake.mThreadFunc (i) ;
				}
			}
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		fake.mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	void poll (CREF<INDEX> slot) {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Running)
				break ;
			if (!fake.mItemQueue.empty ())
				break ;
			if ifdo (TRUE) {
				if (fake.mThreadJoin[slot])
					discard ;
				fake.mThreadJoin.add (slot) ;
				rax.notify () ;
			}
			rax.wait () ;
		}
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
		fake.mThreadJoin.erase (slot) ;
		fake.mItemLoadLength -= fake.mThreadLoadLength[slot] ;
		INDEX ix = fake.mItemQueue.head () ;
		const auto r1x = inline_alignas (fake.mItemLoadLength ,fake.mThread.size ()) / fake.mThread.size () ;
		const auto r2x = MathProc::max_of (r1x / 2 ,LENGTH (1)) ;
		const auto r3x = MathProc::min_of (r2x ,fake.mItemQueue[ix].length ()) ;
		fake.mThreadLoadLength[slot] = r3x ;
		auto &&rbx = keep[TYPE<IndexIteratorLayout>::expr] (fake.mItemQueue[ix]) ;
		fake.mThreadQueue[slot] = IndexIterator (rbx.mBegin ,rbx.mBegin + r3x) ;
		fake.mItemQueue[ix] = IndexIterator (rbx.mBegin + r3x ,rbx.mEnd) ;
		if ifdo (TRUE) {
			if (fake.mItemQueue[ix].good ())
				discard ;
			fake.mItemQueue.take () ;
		}
		rax.notify () ;
	}

	void post (CREF<INDEX> begin_ ,CREF<INDEX> end_) override {
		if (begin_ >= end_)
			return ;
		auto rax = UniqueLock (fake.mThreadMutex) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Running)
				break ;
			if (!fake.mItemQueue.full ())
				break ;
			rax.wait () ;
		}
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
		const auto r1x = IndexIterator (begin_ ,end_) ;
		fake.mItemQueue.add (r1x) ;
		fake.mItemLoadLength += r1x.length () ;
		rax.notify () ;
	}

	void join () override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Running)
				break ;
			if (fake.mItemQueue.empty ())
				if (fake.mThreadJoin.length () >= fake.mThread.length ())
					break ;
			rax.wait () ;
		}
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
	}

	BOOL join (CREF<Time> interval) override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		auto rbx = TRUE ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Running)
				break ;
			if (fake.mItemQueue.empty () == 0)
				if (fake.mThreadJoin.length () >= fake.mThread.length ())
					break ;
			if (!rbx)
				return FALSE ;
			rbx = FALSE ;
			rax.wait (interval) ;
		}
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
		return TRUE ;
	}

	void stop () override {
		crash () ;
		for (auto &&i : fake.mThread.range ())
			fake.mThread[i].stop () ;
		fake.mThread = Array<Thread> () ;
		fake.mThreadFunc = Function<CREF<INDEX>> () ;
		fake.mThreadFlag = ThreadFlag::Preparing ;
		fake.mThreadJoin = BitSet () ;
		fake.mThreadQueue = Array<IndexIterator> () ;
		fake.mThreadLoadLength = Array<LENGTH> () ;
		fake.mItemQueue = Deque<IndexIterator> () ;
	}
} ;

exports WorkThreadLayout WorkThreadHolder::create () {
	WorkThreadLayout ret ;
	ret.mThis = SharedRef<WorkThreadImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<WorkThreadHolder> WorkThreadHolder::hold (VREF<WorkThreadImplLayout> that) {
	return VFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

exports CFat<WorkThreadHolder> WorkThreadHolder::hold (CREF<WorkThreadImplLayout> that) {
	return CFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

struct CalcThreadImplLayout {
	Mutex mThreadMutex ;
	Just<ThreadFlag> mThreadFlag ;
	BOOL mSuspendFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Function<CREF<CalcSolution> ,VREF<CalcSolution>> mThreadFunc ;
	Array<FLT64> mConfidence ;
	FLT64 mConfidencePow ;
	FLT64 mConfidenceFator ;
	Array<CalcSolution> mThreadSolution ;
	Array<CalcSolution> mSearchSolution ;
	CalcSolution mBestSolution ;
	BOOL mNewSolution ;

public:
	implicit CalcThreadImplLayout () = default ;

	implicit ~CalcThreadImplLayout () noexcept {
		CalcThreadHolder::hold (thiz)->stop () ;
	}

	void friend_execute (CREF<INDEX> slot) {
		CalcThreadHolder::hold (thiz)->friend_execute (slot) ;
	}
} ;

class CalcThreadImplHolder final implement Fat<CalcThreadHolder ,CalcThreadImplLayout> {
public:
	void initialize () override {
		fake.mThreadMutex = UniqueMutex () ;
		fake.mThreadFlag = ThreadFlag::Preparing ;
		const auto r1x = RuntimeProc::thread_concurrency () ;
		set_thread_size (r1x) ;
	}

	void set_thread_size (CREF<LENGTH> size_) override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Preparing) ;
		assume (fake.mThreadSolution.size () == 0) ;
		fake.mThread = Array<Thread> (size_) ;
		fake.mThreadJoin = BitSet (size_) ;
		fake.mSearchSolution = Array<CalcSolution> (size_) ;
		fake.mThreadSolution = Array<CalcSolution> (size_) ;
		fake.mNewSolution = FALSE ;
	}

	void set_start_input (CREF<BitSet> input ,CREF<FLT64> factor) override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Preparing) ;
		assume (fake.mThread.size () > 0) ;
		fake.mBestSolution.mIteration = ZERO ;
		fake.mBestSolution.mAvgError = infinity ;
		fake.mBestSolution.mStdError = 0 ;
		fake.mBestSolution.mInput = input ;
		for (auto &&i : fake.mThreadSolution.range ())
			fake.mThreadSolution[i].mIteration = NONE ;
		fake.mConfidence = Array<FLT64> (1000) ;
		fake.mConfidencePow = 0.9545 ;
		fake.mConfidenceFator = MathProc::clamp (factor ,FLT64 (0) ,FLT64 (1)) ;
		fake.mConfidence[0] = 1 ;
		for (auto &&i : iter (1 ,fake.mConfidence.length ())) {
			fake.mConfidence[i] = fake.mConfidencePow * fake.mConfidence[i - 1] ;
		}
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Preparing) ;
		assume (fake.mThread.size () > 0) ;
		fake.mThreadFlag = ThreadFlag::Running ;
		fake.mSuspendFlag = FALSE ;
		fake.mThreadFunc = func ;
		const auto r1x = FriendThreadBinder<CalcThreadImplLayout>::hold (fake) ;
		for (auto &&i : fake.mThread.range ()) {
			fake.mThread[i] = Thread (Box<VFat<ThreadBinder>>::make (r1x) ,i) ;
			fake.mThread[i].start () ;
		}
	}

	void friend_execute (CREF<INDEX> slot) override {
		try {
			while (TRUE) {
				search_new (slot) ;
				search_xor (slot) ;
			}
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		fake.mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	void search_new (CREF<INDEX> slot) {
		while (TRUE) {
			wait_solution (slot) ;
			while (TRUE) {
				wait_suspend (slot) ;
				fake.mSearchSolution[slot].mIteration = NONE ;
				fake.mThreadFunc (fake.mThreadSolution[slot] ,fake.mSearchSolution[slot]) ;
				if (fake.mSearchSolution[slot].mIteration == NONE)
					break ;
				if (is_better (fake.mSearchSolution[slot] ,fake.mThreadSolution[slot]))
					break ;
			}
			if (fake.mSearchSolution[slot].mIteration != NONE)
				break ;
		}
	}

	void search_xor (CREF<INDEX> slot) {
		while (TRUE) {
			const auto r1x = accept_solution (slot) ;
			if (r1x)
				break ;
			fake.mSearchSolution[slot].mIteration = fake.mThreadSolution[slot].mIteration ;
			fake.mThreadFunc (fake.mThreadSolution[slot] ,fake.mSearchSolution[slot]) ;
			if (fake.mSearchSolution[slot].mIteration == NONE)
				break ;
			if (!is_better (fake.mSearchSolution[slot] ,fake.mThreadSolution[slot]))
				break ;
		}
	}

	BOOL is_better (CREF<CalcSolution> a ,CREF<CalcSolution> b) const {
		const auto r1x = INDEX (FLT64 (a.mIteration) * fake.mConfidenceFator) ;
		INDEX ix = MathProc::clamp (r1x ,ZERO ,fake.mConfidence.length ()) ;
		const auto r2x = a.mAvgError - a.mStdError * fake.mConfidence[ix] ;
		const auto r3x = b.mAvgError - b.mStdError * fake.mConfidence[ix] ;
		if (r2x < r3x)
			return TRUE ;
		return FALSE ;
	}

	void wait_solution (CREF<INDEX> slot) {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		if (fake.mBestSolution.mIteration != NONE)
			if (fake.mThreadSolution[slot].mIteration == fake.mBestSolution.mIteration)
				return ;
		fake.mThreadSolution[slot] = fake.mBestSolution ;
	}

	BOOL accept_solution (CREF<INDEX> slot) {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mThreadSolution[slot].mIteration != fake.mBestSolution.mIteration)
				if (is_better (fake.mSearchSolution[slot] ,fake.mBestSolution))
					discard ;
			assert (fake.mSearchSolution[slot].mInput.size () > 0) ;
			fake.mBestSolution = move (fake.mSearchSolution[slot]) ;
			fake.mNewSolution = TRUE ;
			rax.notify () ;
			return TRUE ;
		}
		if ifdo (act) {
			assume (fake.mBestSolution.mIteration != NONE) ;
			const auto r1x = bitset_xor (fake.mThreadSolution[slot].mInput ,fake.mSearchSolution[slot].mInput) ;
			fake.mThreadSolution[slot] = move (fake.mSearchSolution[slot]) ;
			fake.mSearchSolution[slot].mAvgError = infinity ;
			fake.mSearchSolution[slot].mStdError = 0 ;
			fake.mSearchSolution[slot].mInput = bitset_xor (fake.mBestSolution.mInput ,r1x) ;
		}
		return FALSE ;
	}

	BitSet bitset_xor (CREF<BitSet> bitset1 ,CREF<BitSet> bitset2) const {
		if (bitset1.size () == 0)
			return bitset2 ;
		if (bitset2.size () == 0)
			return bitset1 ;
		return bitset1 ^ bitset2 ;
	}

	void wait_suspend (CREF<INDEX> slot) {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Running)
				break ;
			if (!fake.mSuspendFlag)
				break ;
			if ifdo (TRUE) {
				if (fake.mThreadJoin[slot])
					discard ;
				fake.mThreadJoin.add (slot) ;
				rax.notify () ;
			}
			rax.wait () ;
		}
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
		fake.mThreadJoin.erase (slot) ;
	}

	BOOL ready () const override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		return fake.mNewSolution ;
	}

	CalcSolution poll () override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Running)
				break ;
			if (fake.mNewSolution)
				break ;
			rax.wait () ;
		}
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
		fake.mNewSolution = FALSE ;
		return fake.mBestSolution ;
	}

	void suspend () override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
		fake.mSuspendFlag = TRUE ;
		rax.notify () ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Running)
				break ;
			if (!fake.mSuspendFlag)
				break ;
			if (fake.mThreadJoin.length () >= fake.mThread.length ())
				break ;
			rax.wait () ;
		}
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
	}

	void resume () override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
		fake.mSuspendFlag = FALSE ;
		rax.notify () ;
	}

	void stop () override {
		crash () ;
		for (auto &&i : fake.mThread.range ())
			fake.mThread[i].stop () ;
		fake.mThread = Array<Thread> () ;
		fake.mThreadFunc = Function<CREF<CalcSolution> ,VREF<CalcSolution>> () ;
		fake.mThreadFlag = ThreadFlag::Preparing ;
		fake.mThreadJoin = BitSet () ;
		fake.mThreadSolution = Array<CalcSolution> () ;
		fake.mSearchSolution = Array<CalcSolution> () ;
		fake.mBestSolution = CalcSolution () ;
	}
} ;

exports CalcThreadLayout CalcThreadHolder::create () {
	CalcThreadLayout ret ;
	ret.mThis = SharedRef<CalcThreadImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<CalcThreadHolder> CalcThreadHolder::hold (VREF<CalcThreadImplLayout> that) {
	return VFat<CalcThreadHolder> (CalcThreadImplHolder () ,that) ;
}

exports CFat<CalcThreadHolder> CalcThreadHolder::hold (CREF<CalcThreadImplLayout> that) {
	return CFat<CalcThreadHolder> (CalcThreadImplHolder () ,that) ;
}

struct PromiseImplLayout {
	Mutex mThreadMutex ;
	Just<ThreadFlag> mThreadFlag ;
	Array<Thread> mThread ;
	Function<> mThreadFunc ;
	Function<> mRunningFunc ;
	Box<AutoRef<Pointer>> mItem ;
	Box<Exception> mException ;
	BOOL mRetryFlag ;

public:
	implicit PromiseImplLayout () = default ;

	implicit ~PromiseImplLayout () noexcept {
		PromiseHolder::hold (thiz)->stop () ;
	}

	void friend_execute (CREF<INDEX> slot) {
		PromiseHolder::hold (thiz)->friend_execute (slot) ;
	}
} ;

class PromiseImplHolder final implement Fat<PromiseHolder ,PromiseImplLayout> {
public:
	void initialize () override {
		fake.mThreadMutex = UniqueMutex () ;
		fake.mThreadFlag = ThreadFlag::Preparing ;
		set_retry (FALSE) ;
	}

	void set_retry (CREF<BOOL> flag) override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Preparing) ;
		fake.mRetryFlag = flag ;
	}

	void start () override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Preparing) ;
		assume (fake.mThread.size () == 0) ;
		fake.mThreadFlag = ThreadFlag::Running ;
		fake.mThreadFunc = Function<> () ;
		fake.mItem = NULL ;
		fake.mException = NULL ;
		rax.notify () ;
	}

	void start (CREF<Function<>> func) override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Preparing) ;
		fake.mThreadFlag = ThreadFlag::Running ;
		fake.mThreadFunc = func ;
		fake.mItem = NULL ;
		fake.mException = NULL ;
		if ifdo (TRUE) {
			if (fake.mThread.size () > 0)
				discard ;
			fake.mThread = Array<Thread> (1) ;
			const auto r1x = FriendThreadBinder<PromiseImplLayout>::hold (fake) ;
			for (auto &&i : fake.mThread.range ()) {
				fake.mThread[i] = Thread (Box<VFat<ThreadBinder>>::make (r1x) ,0) ;
				fake.mThread[i].start () ;
			}
		}
		rax.notify () ;
	}

	void friend_execute (CREF<INDEX> slot) override {
		while (TRUE) {
			try {
				fake.mRunningFunc = fake.mThreadFunc ;
				fake.mRunningFunc () ;
			} catch (CREF<Exception> e) {
				rethrow (e) ;
			}
			const auto r1x = wait_future () ;
			if (!r1x)
				break ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		fake.mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	BOOL wait_future () {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Finishing)
				break ;
			rax.wait () ;
		}
		if (fake.mThreadFlag == ThreadFlag::Closing)
			return FALSE ;
		if ifdo (TRUE) {
			if (fake.mRetryFlag)
				discard ;
			fake.mThreadFlag = ThreadFlag::Preparing ;
			rax.notify () ;
		}
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Preparing)
				break ;
			rax.wait () ;
		}
		if (fake.mThreadFlag == ThreadFlag::Closing)
			return FALSE ;
		return TRUE ;
	}

	void post (RREF<AutoRef<Pointer>> item) override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadFlag::Running) ;
		assume (fake.mItem == NULL) ;
		fake.mItem = Box<AutoRef<Pointer>>::make (move (item)) ;
		fake.mThreadFlag = ThreadFlag::Finishing ;
		rax.notify () ;
	}

	void rethrow (CREF<Exception> e) override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		if (fake.mThreadFlag != ThreadFlag::Running)
			return ;
		if (fake.mException != NULL)
			return ;
		fake.mException = Box<Exception>::make (e) ;
		fake.mThreadFlag = ThreadFlag::Finishing ;
		rax.notify () ;
	}

	BOOL ready () const override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		if (fake.mThreadFlag == ThreadFlag::Finishing)
			return TRUE ;
		return FALSE ;
	}

	BOOL running () const override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		if (fake.mThreadFlag == ThreadFlag::Running)
			return TRUE ;
		if (fake.mThreadFlag == ThreadFlag::Finishing)
			return TRUE ;
		return FALSE ;
	}

	AutoRef<Pointer> poll () override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadFlag::Running)
				break ;
			rax.wait () ;
		}
		assume (fake.mThreadFlag != ThreadFlag::Closing) ;
		if ifdo (TRUE) {
			if (fake.mException == NULL)
				discard ;
			fake.mException->raise () ;
		}
		AutoRef<Pointer> ret ;
		if ifdo (TRUE) {
			if (fake.mItem == NULL)
				discard ;
			ret = move (fake.mItem.self) ;
			fake.mItem = NULL ;
		}
		fake.mThreadFlag = ThreadFlag::Preparing ;
		rax.notify () ;
		return move (ret) ;
	}

	void future () override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		if (fake.mThreadFlag == ThreadFlag::Preparing)
			return ;
		assume (fake.mThreadFlag == ThreadFlag::Finishing) ;
		fake.mThreadFlag = ThreadFlag::Preparing ;
		rax.notify () ;
	}

	void stop () override {
		crash () ;
		for (auto &&i : fake.mThread.range ())
			fake.mThread[i].stop () ;
		fake.mThread = Array<Thread> () ;
		fake.mThreadFlag = ThreadFlag::Preparing ;
		fake.mThreadFunc = Function<> () ;
		fake.mItem = NULL ;
		fake.mException = NULL ;
	}
} ;

exports PromiseLayout PromiseHolder::create () {
	PromiseLayout ret ;
	ret.mThis = SharedRef<PromiseImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<PromiseHolder> PromiseHolder::hold (VREF<PromiseImplLayout> that) {
	return VFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}

exports CFat<PromiseHolder> PromiseHolder::hold (CREF<PromiseImplLayout> that) {
	return CFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}
} ;