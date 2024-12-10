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
} ;

class WorkThreadImpl implement WorkThreadImplLayout {
private:
	using THREAD_QUEUE_SIZE = ENUM<65536> ;

protected:
	using WorkThreadImplLayout::mThreadMutex ;
	using WorkThreadImplLayout::mThreadFlag ;
	using WorkThreadImplLayout::mThread ;
	using WorkThreadImplLayout::mThreadJoin ;
	using WorkThreadImplLayout::mThreadQueue ;
	using WorkThreadImplLayout::mThreadLoadLength ;
	using WorkThreadImplLayout::mThreadFunc ;
	using WorkThreadImplLayout::mItemQueue ;
	using WorkThreadImplLayout::mItemLoadLength ;

public:
	implicit WorkThreadImpl () = default ;

	implicit ~WorkThreadImpl () noexcept {
		stop () ;
	}

	void initialize () {
		mThreadMutex = UniqueMutex () ;
		mThreadFlag = ThreadFlag::Preparing ;
		const auto r1x = RuntimeProc::thread_concurrency () ;
		set_thread_size (r1x) ;
		set_queue_size (r1x) ;
	}

	void set_thread_size (CREF<LENGTH> size_) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mThread = Array<Thread> (size_) ;
		mThreadJoin = BitSet (size_) ;
		mThreadQueue = Array<IndexIterator> (size_) ;
		mThreadLoadLength = Array<LENGTH> (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mItemQueue.empty ()) ;
		mItemQueue = Deque<IndexIterator> (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThread.size () > 0) ;
		mThreadLoadLength.fill (0) ;
		mItemLoadLength = 0 ;
		mThreadFlag = ThreadFlag::Running ;
		mThreadFunc = func ;
		const auto r1x = Ref<VFat<ThreadFriend>>::make (ThreadFriendBinder<WorkThreadImpl>::hold (thiz)) ;
		for (auto &&i : mThread.range ()) {
			mThread[i] = Thread (r1x.share () ,i) ;
			mThread[i].start () ;
		}
	}

	void friend_execute (CREF<INDEX> slot) {
		try {
			while (TRUE) {
				if ifdo (TRUE) {
					if (mThreadQueue[slot].good ())
						discard ;
					poll (slot) ;
				}
				INDEX ix = mThreadQueue[slot].peek () ;
				mThreadFunc (ix) ;
				mThreadQueue[slot].next () ;
			}
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (mThreadMutex) ;
		mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	void poll (CREF<INDEX> slot) {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (!mItemQueue.empty ())
				break ;
			if ifdo (TRUE) {
				if (mThreadJoin[slot])
					discard ;
				mThreadJoin.add (slot) ;
				rax.notify () ;
			}
			rax.wait () ;
		}
		mThreadJoin.erase (slot) ;
		mItemLoadLength -= mThreadLoadLength[slot] ;
		INDEX ix = mItemQueue.head () ;
		const auto r2x = inline_alignas (mItemLoadLength ,mThread.size ()) / mThread.size () ;
		const auto r3x = MathProc::max_of (r2x / 2 ,LENGTH (1)) ;
		const auto r4x = MathProc::min_of (r3x ,mItemQueue[ix].length ()) ;
		mThreadLoadLength[slot] = r4x ;
		mItemLoadLength += mThreadLoadLength[slot] ;
		auto &&rbx = keep[TYPE<IndexIteratorLayout>::expr] (mItemQueue[ix]) ;
		mThreadQueue[slot] = IndexIterator (rbx.mBegin ,rbx.mBegin + r4x) ;
		mItemQueue[ix] = IndexIterator (rbx.mBegin + r4x ,rbx.mEnd) ;
		if ifdo (TRUE) {
			if (mItemQueue[ix].good ())
				discard ;
			mItemQueue.take () ;
		}
		rax.notify () ;
	}

	void post (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
		if (begin_ >= end_)
			return ;
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		const auto r1x = IndexIterator (begin_ ,end_) ;
		mItemQueue.add (r1x) ;
		mItemLoadLength += r1x.length () ;
		rax.notify () ;
	}

	void join () {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (mItemQueue.empty ())
				if (mThreadJoin.length () >= mThread.length ())
					break ;
			rax.wait () ;
		}
	}

	BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (mItemQueue.empty () == 0)
				if (mThreadJoin.length () >= mThread.length ())
					break ;
			auto rbx = FALSE ;
			rax = UniqueLock () ;
			predicate (rbx) ;
			rax = UniqueLock (mThreadMutex) ;
			if (!rbx)
				return FALSE ;
			rax.wait (interval) ;
		}
		return TRUE ;
	}

	void stop () {
		crash () ;
		for (auto &&i : mThread.range ())
			mThread[i].stop () ;
		mThread = Array<Thread> () ;
		mThreadFunc = Function<CREF<INDEX>> () ;
		mThreadFlag = ThreadFlag::Preparing ;
		mThreadJoin = BitSet () ;
		mThreadQueue = Array<IndexIterator> () ;
		mThreadLoadLength = Array<LENGTH> () ;
		mItemQueue = Deque<IndexIterator> () ;
	}
} ;

class WorkThreadImplHolder final implement Fat<WorkThreadHolder ,WorkThreadLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<WorkThreadImpl>::make () ;
		ptr (fake).initialize () ;
	}

	static VREF<WorkThreadImpl> ptr (CREF<WorkThreadLayout> layout) {
		return keep[TYPE<WorkThreadImpl>::expr] (layout.mThis.self) ;
	}

	void set_thread_size (CREF<LENGTH> size_) const override {
		return ptr (fake).set_thread_size (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) const override {
		return ptr (fake).set_queue_size (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) const override {
		return ptr (fake).start (func) ;
	}

	void post (CREF<INDEX> begin_ ,CREF<INDEX> end_) const override {
		return ptr (fake).post (begin_ ,end_) ;
	}

	void join () const override {
		return ptr (fake).join () ;
	}

	BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) const override {
		return ptr (fake).join (interval ,predicate) ;
	}

	void stop () const override {
		return ptr (fake).stop () ;
	}
} ;

exports VFat<WorkThreadHolder> WorkThreadHolder::hold (VREF<WorkThreadLayout> that) {
	return VFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

exports CFat<WorkThreadHolder> WorkThreadHolder::hold (CREF<WorkThreadLayout> that) {
	return CFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

struct CalcThreadImplLayout {
	Mutex mThreadMutex ;
	Just<ThreadFlag> mThreadFlag ;
	BOOL mSuspendFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Function<CREF<CalcSolution> ,VREF<CalcSolution>> mThreadFunc ;
	Array<CalcSolution> mThreadSolution ;
	Array<CalcSolution> mSearchSolution ;
	CalcSolution mBestSolution ;
	BOOL mNewSolution ;
} ;

class CalcThreadImpl implement CalcThreadImplLayout {
protected:
	using CalcThreadImplLayout::mThreadMutex ;
	using CalcThreadImplLayout::mThreadFlag ;
	using CalcThreadImplLayout::mSuspendFlag ;
	using CalcThreadImplLayout::mThread ;
	using CalcThreadImplLayout::mThreadJoin ;
	using CalcThreadImplLayout::mThreadFunc ;
	using CalcThreadImplLayout::mThreadSolution ;
	using CalcThreadImplLayout::mSearchSolution ;
	using CalcThreadImplLayout::mBestSolution ;
	using CalcThreadImplLayout::mNewSolution ;

public:
	implicit CalcThreadImpl () = default ;

	implicit ~CalcThreadImpl () noexcept {
		stop () ;
	}

	void initialize () {
		mThreadMutex = UniqueMutex () ;
		mThreadFlag = ThreadFlag::Preparing ;
		const auto r1x = RuntimeProc::thread_concurrency () ;
		set_thread_size (r1x) ;
	}

	void set_thread_size (CREF<LENGTH> size_) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThreadSolution.size () == 0) ;
		mThread = Array<Thread> (size_) ;
		mThreadJoin = BitSet (size_) ;
		mSearchSolution = Array<CalcSolution> (size_) ;
		mBestSolution.mIndex = NONE ;
		mBestSolution.mError = infinity ;
		mNewSolution = FALSE ;
	}

	void set_base_input (CREF<BitSet> base) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThread.size () > 0) ;
		mThreadSolution = Array<CalcSolution> (mThread.size ()) ;
		for (auto &&i : mThreadSolution.range ()) {
			mThreadSolution[i].mIndex = NONE ;
			mThreadSolution[i].mError = infinity ;
			mThreadSolution[i].mInput = base ;
		}
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThread.size () > 0) ;
		mThreadFlag = ThreadFlag::Running ;
		mSuspendFlag = FALSE ;
		mThreadFunc = func ;
		const auto r1x = Ref<VFat<ThreadFriend>>::make (ThreadFriendBinder<CalcThreadImpl>::hold (thiz)) ;
		for (auto &&i : mThread.range ()) {
			mThread[i] = Thread (r1x.share () ,i) ;
			mThread[i].start () ;
		}
	}

	void friend_execute (CREF<INDEX> slot) {
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
		auto rax = UniqueLock (mThreadMutex) ;
		mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	void search_new (CREF<INDEX> slot) {
		while (TRUE) {
			wait_solution (slot) ;
			while (TRUE) {
				wait_suspend (slot) ;
				mThreadFunc (mThreadSolution[slot] ,mSearchSolution[slot]) ;
				if (mSearchSolution[slot].mIndex == NONE)
					break ;
				if (mSearchSolution[slot].mError < mThreadSolution[slot].mError)
					break ;
			}
			if (mSearchSolution[slot].mIndex != NONE)
				break ;
		}
	}

	void search_xor (CREF<INDEX> slot) {
		while (TRUE) {
			const auto r1x = accept_solution (slot) ;
			if (r1x)
				break ;
			mThreadFunc (mThreadSolution[slot] ,mSearchSolution[slot]) ;
			if (mSearchSolution[slot].mIndex == NONE)
				break ;
			if (mSearchSolution[slot].mError >= mThreadSolution[slot].mError)
				break ;
		}
	}

	BitSet bitset_xor (CREF<BitSet> bitset1 ,CREF<BitSet> bitset2) const {
		if (bitset1.size () == 0)
			return bitset2 ;
		if (bitset2.size () == 0)
			return bitset1 ;
		return bitset1 ^ bitset2 ;
	}

	void wait_solution (CREF<INDEX> slot) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		if (mThreadSolution[slot].mIndex == mBestSolution.mIndex)
			return ;
		mThreadSolution[slot] = mBestSolution ;
	}

	BOOL accept_solution (CREF<INDEX> slot) {
		auto rax = UniqueLock (mThreadMutex) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mThreadSolution[slot].mIndex != mBestSolution.mIndex)
				if (mSearchSolution[slot].mError >= mBestSolution.mError)
					discard ;
			mBestSolution.mIndex++ ;
			mBestSolution.mError = mSearchSolution[slot].mError ;
			mBestSolution.mInput = mSearchSolution[slot].mInput ;
			mNewSolution = TRUE ;
			rax.notify () ;
		}
		if ifdo (act) {
			assume (mBestSolution.mIndex != NONE) ;
			const auto r1x = bitset_xor (mThreadSolution[slot].mInput ,mSearchSolution[slot].mInput) ;
			mThreadSolution[slot].mIndex = mBestSolution.mIndex ;
			mThreadSolution[slot].mError = mBestSolution.mError ;
			mThreadSolution[slot].mInput = bitset_xor (mBestSolution.mInput ,r1x) ;
			return TRUE ;
		}
		return FALSE ;
	}

	void wait_suspend (CREF<INDEX> slot) {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (!mSuspendFlag)
				break ;
			if ifdo (TRUE) {
				if (mThreadJoin[slot])
					discard ;
				mThreadJoin.add (slot) ;
				rax.notify () ;
			}
			rax.wait () ;
		}
		mThreadJoin.erase (slot) ;
	}

	BOOL ready () const {
		Scope<Mutex> anonymous (mThreadMutex) ;
		return mNewSolution ;
	}

	CalcSolution poll () {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (mNewSolution)
				break ;
			rax.wait () ;
		}
		mNewSolution = FALSE ;
		return mBestSolution ;
	}

	void suspend () {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		mSuspendFlag = TRUE ;
		rax.notify () ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (!mSuspendFlag)
				break ;
			if (mThreadJoin.length () >= mThread.length ())
				break ;
			rax.wait () ;
		}
	}

	void resume () {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		mSuspendFlag = FALSE ;
		rax.notify () ;
	}

	void stop () {
		crash () ;
		for (auto &&i : mThread.range ())
			mThread[i].stop () ;
		mThread = Array<Thread> () ;
		mThreadFunc = Function<CREF<CalcSolution> ,VREF<CalcSolution>> () ;
		mThreadFlag = ThreadFlag::Preparing ;
		mThreadJoin = BitSet () ;
		mThreadSolution = Array<CalcSolution> () ;
		mSearchSolution = Array<CalcSolution> () ;
		mBestSolution = CalcSolution () ;
	}
} ;

class CalcThreadImplHolder final implement Fat<CalcThreadHolder ,CalcThreadLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<CalcThreadImpl>::make () ;
		ptr (fake).initialize () ;
	}

	static VREF<CalcThreadImpl> ptr (CREF<CalcThreadLayout> layout) {
		return keep[TYPE<CalcThreadImpl>::expr] (layout.mThis.self) ;
	}

	void set_thread_size (CREF<LENGTH> size_) const override {
		return ptr (fake).set_thread_size (size_) ;
	}

	void set_base_input (CREF<BitSet> base) const override {
		return ptr (fake).set_base_input (base) ;
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const override {
		return ptr (fake).start (func) ;
	}

	BOOL ready () const override {
		return ptr (fake).ready () ;
	}

	CalcSolution poll () const override {
		return ptr (fake).poll () ;
	}

	void suspend () const override {
		return ptr (fake).suspend () ;
	}

	void resume () const override {
		return ptr (fake).resume () ;
	}

	void stop () const override {
		return ptr (fake).stop () ;
	}
} ;

exports VFat<CalcThreadHolder> CalcThreadHolder::hold (VREF<CalcThreadLayout> that) {
	return VFat<CalcThreadHolder> (CalcThreadImplHolder () ,that) ;
}

exports CFat<CalcThreadHolder> CalcThreadHolder::hold (CREF<CalcThreadLayout> that) {
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
} ;

class PromiseImpl implement PromiseImplLayout {
protected:
	using PromiseImplLayout::mThreadMutex ;
	using PromiseImplLayout::mThreadFlag ;
	using PromiseImplLayout::mThread ;
	using PromiseImplLayout::mThreadFunc ;
	using PromiseImplLayout::mRunningFunc ;
	using PromiseImplLayout::mItem ;
	using PromiseImplLayout::mException ;
	using PromiseImplLayout::mRetryFlag ;

public:
	implicit PromiseImpl () = default ;

	implicit ~PromiseImpl () noexcept {
		stop () ;
	}

	void initialize () {
		mThreadMutex = UniqueMutex () ;
		mThreadFlag = ThreadFlag::Preparing ;
		set_retry (FALSE) ;
	}

	void set_retry (CREF<BOOL> flag) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mRetryFlag = flag ;
	}

	void start () {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThread.size () == 0) ;
		mThreadFlag = ThreadFlag::Running ;
		mThreadFunc = Function<> () ;
		mItem = NULL ;
		mException = NULL ;
		rax.notify () ;
	}

	void start (CREF<Function<>> func) {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mThreadFlag = ThreadFlag::Running ;
		mThreadFunc = func ;
		mItem = NULL ;
		mException = NULL ;
		if ifdo (TRUE) {
			if (mThread.size () > 0)
				discard ;
			mThread = Array<Thread> (1) ;
			const auto r1x = Ref<VFat<ThreadFriend>>::make (ThreadFriendBinder<PromiseImpl>::hold (thiz)) ;
			for (auto &&i : mThread.range ()) {
				mThread[i] = Thread (r1x.share () ,0) ;
				mThread[i].start () ;
			}
		}
		rax.notify () ;
	}

	void friend_execute (CREF<INDEX> slot) {
		while (TRUE) {
			try {
				mRunningFunc = mThreadFunc ;
				mRunningFunc () ;
			} catch (CREF<Exception> e) {
				rethrow (e) ;
			}
			const auto r2x = wait_future () ;
			if (!r2x)
				break ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (mThreadMutex) ;
		mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	BOOL wait_future () {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			if (mThreadFlag != ThreadFlag::Finishing)
				break ;
			rax.wait () ;
		}
		if (mThreadFlag == ThreadFlag::Closing)
			return FALSE ;
		if ifdo (TRUE) {
			if (mRetryFlag)
				discard ;
			mThreadFlag = ThreadFlag::Preparing ;
			rax.notify () ;
		}
		while (TRUE) {
			if (mThreadFlag != ThreadFlag::Preparing)
				break ;
			rax.wait () ;
		}
		if (mThreadFlag == ThreadFlag::Closing)
			return FALSE ;
		return TRUE ;
	}

	void post (RREF<AutoRef<Pointer>> item) {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		assume (mItem == NULL) ;
		mItem = Box<AutoRef<Pointer>>::make (move (item)) ;
		mThreadFlag = ThreadFlag::Finishing ;
		rax.notify () ;
	}

	void rethrow (CREF<Exception> e) {
		auto rax = UniqueLock (mThreadMutex) ;
		if (mThreadFlag != ThreadFlag::Running)
			return ;
		if (mException != NULL)
			return ;
		mException = Box<Exception>::make (e) ;
		mThreadFlag = ThreadFlag::Finishing ;
		rax.notify () ;
	}

	BOOL ready () const {
		Scope<Mutex> anonymous (mThreadMutex) ;
		if (mThreadFlag == ThreadFlag::Finishing)
			return TRUE ;
		return FALSE ;
	}

	BOOL running () const {
		Scope<Mutex> anonymous (mThreadMutex) ;
		if (mThreadFlag == ThreadFlag::Running)
			return TRUE ;
		if (mThreadFlag == ThreadFlag::Finishing)
			return TRUE ;
		return FALSE ;
	}

	AutoRef<Pointer> poll () {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			if (mThreadFlag != ThreadFlag::Running)
				break ;
			rax.wait () ;
		}
		assume (mThreadFlag != ThreadFlag::Closing) ;
		if ifdo (TRUE) {
			if (mException == NULL)
				discard ;
			mException->raise () ;
		}
		AutoRef<Pointer> ret ;
		if ifdo (TRUE) {
			if (mItem == NULL)
				discard ;
			ret = move (mItem.self) ;
			mItem = NULL ;
		}
		mThreadFlag = ThreadFlag::Preparing ;
		rax.notify () ;
		return move (ret) ;
	}

	void future () {
		auto rax = UniqueLock (mThreadMutex) ;
		if (mThreadFlag == ThreadFlag::Preparing)
			return ;
		assume (mThreadFlag == ThreadFlag::Finishing) ;
		mThreadFlag = ThreadFlag::Preparing ;
		rax.notify () ;
	}

	void stop () {
		crash () ;
		for (auto &&i : mThread.range ())
			mThread[i].stop () ;
		mThread = Array<Thread> () ;
		mThreadFlag = ThreadFlag::Preparing ;
		mThreadFunc = Function<> () ;
		mItem = NULL ;
		mException = NULL ;
	}
} ;

class PromiseImplHolder final implement Fat<PromiseHolder ,PromiseLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<PromiseImpl>::make () ;
		ptr (fake).initialize () ;
	}

	static VREF<PromiseImpl> ptr (CREF<PromiseLayout> layout) {
		return keep[TYPE<PromiseImpl>::expr] (layout.mThis.self) ;
	}

	void set_retry (CREF<BOOL> flag) const override {
		return ptr (fake).set_retry (flag) ;
	}

	void start () const override {
		return ptr (fake).start () ;
	}

	void start (CREF<Function<>> func) const override {
		return ptr (fake).start (func) ;
	}

	void post (RREF<AutoRef<Pointer>> item) const override {
		return ptr (fake).post (move (item)) ;
	}

	void rethrow (CREF<Exception> e) const override {
		return ptr (fake).rethrow (e) ;
	}

	BOOL ready () const override {
		return ptr (fake).ready () ;
	}

	BOOL running () const override {
		return ptr (fake).running () ;
	}

	AutoRef<Pointer> poll () const override {
		return ptr (fake).poll () ;
	}

	void future () const override {
		return ptr (fake).future () ;
	}

	void stop () const override {
		return ptr (fake).stop () ;
	}
} ;

exports VFat<PromiseHolder> PromiseHolder::hold (VREF<PromiseLayout> that) {
	return VFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}

exports CFat<PromiseHolder> PromiseHolder::hold (CREF<PromiseLayout> that) {
	return CFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}
} ;