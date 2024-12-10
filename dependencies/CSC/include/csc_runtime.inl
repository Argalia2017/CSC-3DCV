#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_runtime.hpp"

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <random>
#include "csc_begin.h"

namespace CSC {
#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_calendar_from_timepoint {
	forceinline std::tm operator() (CREF<std::time_t> time) const {
		std::tm ret ;
		inline_memset (ret) ;
		localtime_s ((&ret) ,(&time)) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_calendar_from_timepoint {
	forceinline std::tm operator() (CREF<std::time_t> time) const {
		std::tm ret ;
		const auto r1x = FLAG (std::localtime (&time)) ;
		inline_memcpy (Pointer::from (ret) ,Pointer::make (r1x) ,SIZE_OF<std::tm>::expr) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_calendar_from_timepoint {
	forceinline std::tm operator() (CREF<std::time_t> time) const {
		std::tm ret ;
		inline_memset (ret) ;
		localtime_s ((&ret) ,(&time)) ;
		return move (ret) ;
	}
} ;
#endif

static constexpr auto calendar_from_timepoint = FUNCTION_calendar_from_timepoint () ;

struct TimeImplLayout {
	std::chrono::system_clock::duration mTime ;
} ;

class TimeImplHolder final implement Fat<TimeHolder ,TimeLayout> {
public:
	void initialize (CREF<LENGTH> milliseconds_) override {
		fake.mThis = Box<TimeImplLayout>::make () ;
		const auto r1x = std::chrono::milliseconds (milliseconds_) ;
		fake.mThis->mTime = std::chrono::duration_cast<std::chrono::system_clock::duration> (r1x) ;
	}

	void initialize (CREF<TimeCalendar> calendar_) override {
		auto rax = std::tm () ;
		const auto r1x = calendar_.mYear - 1900 ;
		rax.tm_year = VAL32 (r1x * LENGTH (r1x > 0)) ;
		const auto r2x = calendar_.mMonth ;
		rax.tm_mon = VAL32 (r2x * LENGTH (r2x > 0)) ;
		rax.tm_mday = VAL32 (calendar_.mDay) ;
		const auto r3x = calendar_.mWDay - 1 ;
		rax.tm_wday = VAL32 (r3x * LENGTH (r3x > 0)) ;
		const auto r4x = calendar_.mYDay - 1 ;
		rax.tm_yday = VAL32 (r4x * LENGTH (r4x > 0)) ;
		rax.tm_hour = VAL32 (calendar_.mHour) ;
		rax.tm_min = VAL32 (calendar_.mMinute) ;
		rax.tm_sec = VAL32 (calendar_.mSecond) ;
		rax.tm_isdst = 0 ;
		const auto r5x = std::mktime (&rax) ;
		const auto r6x = std::chrono::system_clock::from_time_t (r5x) ;
		fake.mThis = Box<TimeImplLayout>::make () ;
		fake.mThis->mTime = r6x.time_since_epoch () ;
	}

	void initialize (CREF<TimeLayout> that) override {
		if (that.mThis == NULL)
			return ;
		fake.mThis = Box<TimeImplLayout>::make () ;
		fake.mThis->mTime = that.mThis->mTime ;
	}

	Ref<TimeImplLayout> borrow () const leftvalue override {
		assert (fake.mThis.exist ()) ;
		return Ref<TimeImplLayout>::reference (fake.mThis.self) ;
	}

	LENGTH megaseconds () const override {
		using R1X = std::chrono::duration<csc_int64_t ,std::ratio<10000000>> ;
		const auto r1x = std::chrono::duration_cast<R1X> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH kiloseconds () const override {
		using R1X = std::chrono::duration<csc_int64_t ,std::ratio<1000>> ;
		const auto r1x = std::chrono::duration_cast<R1X> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH seconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::seconds> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH milliseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::milliseconds> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH microseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::microseconds> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH nanoseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::nanoseconds> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	TimeCalendar calendar () const override {
		TimeCalendar ret ;
		const auto r1x = std::chrono::system_clock::time_point (fake.mThis->mTime) ;
		const auto r2x = std::time_t (std::chrono::system_clock::to_time_t (r1x)) ;
		const auto r3x = calendar_from_timepoint (r2x) ;
		ret.mYear = r3x.tm_year + 1900 ;
		ret.mMonth = r3x.tm_mon + 1 ;
		ret.mDay = r3x.tm_mday ;
		ret.mWDay = r3x.tm_wday + 1 ;
		ret.mYDay = r3x.tm_yday + 1 ;
		ret.mHour = r3x.tm_hour ;
		ret.mMinute = r3x.tm_min ;
		ret.mSecond = r3x.tm_sec ;
		return move (ret) ;
	}

	TimeLayout sadd (CREF<TimeLayout> that) const override {
		TimeLayout ret ;
		ret.mThis = Box<TimeImplLayout>::make () ;
		ret.mThis->mTime = fake.mThis->mTime + that.mThis->mTime ;
		return move (ret) ;
	}

	TimeLayout ssub (CREF<TimeLayout> that) const override {
		TimeLayout ret ;
		ret.mThis = Box<TimeImplLayout>::make () ;
		ret.mThis->mTime = fake.mThis->mTime - that.mThis->mTime ;
		return move (ret) ;
	}
} ;

exports VFat<TimeHolder> TimeHolder::hold (VREF<TimeLayout> that) {
	return VFat<TimeHolder> (TimeImplHolder () ,that) ;
}

exports CFat<TimeHolder> TimeHolder::hold (CREF<TimeLayout> that) {
	return CFat<TimeHolder> (TimeImplHolder () ,that) ;
}

class MakeTimeImplHolder final implement Fat<MakeTimeHolder ,TimeLayout> {
public:
	void CurrentTime_initialize () override {
		fake.mThis = Box<TimeImplLayout>::make () ;
		const auto r1x = std::chrono::system_clock::now () ;
		fake.mThis->mTime = r1x.time_since_epoch () ;
	}
} ;

exports VFat<MakeTimeHolder> MakeTimeHolder::hold (VREF<TimeLayout> that) {
	return VFat<MakeTimeHolder> (MakeTimeImplHolder () ,that) ;
}

exports CFat<MakeTimeHolder> MakeTimeHolder::hold (CREF<TimeLayout> that) {
	return CFat<MakeTimeHolder> (MakeTimeImplHolder () ,that) ;
}

template class External<RuntimeProcHolder ,RuntimeProcLayout> ;

exports CREF<RuntimeProcLayout> RuntimeProcHolder::instance () {
	return memorize ([&] () {
		RuntimeProcLayout ret ;
		RuntimeProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<RuntimeProcHolder> RuntimeProcHolder::hold (VREF<RuntimeProcLayout> that) {
	return VFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::linkage () ,that) ;
}

exports CFat<RuntimeProcHolder> RuntimeProcHolder::hold (CREF<RuntimeProcLayout> that) {
	return CFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::linkage () ,that) ;
}

struct AtomicImplLayout {
	Pin<std::atomic<VAL>> mAtomic ;
} ;

class AtomicImplHolder final implement Fat<AtomicHolder ,AtomicLayout> {
public:
	void initialize () override {
		fake.mThis = Box<AtomicImplLayout>::make () ;
		store (ZERO) ;
	}

	static VREF<std::atomic<VAL>> ptr (CREF<AtomicLayout> layout) {
		return layout.mThis->mAtomic.self ;
	}

	VAL fetch () const override {
		return ptr (fake).load (std::memory_order_relaxed) ;
	}

	void store (CREF<VAL> item) const override {
		return ptr (fake).store (item ,std::memory_order_relaxed) ;
	}

	VAL exchange (CREF<VAL> item) const override {
		return ptr (fake).exchange (item ,std::memory_order_relaxed) ;
	}

	BOOL change (VREF<VAL> expect ,CREF<VAL> item) const override {
		return ptr (fake).compare_exchange_weak (expect ,item ,std::memory_order_relaxed) ;
	}

	void replace (CREF<VAL> expect ,CREF<VAL> item) const override {
		auto rax = expect ;
		ptr (fake).compare_exchange_strong (rax ,item ,std::memory_order_relaxed) ;
	}

	void increase () const override {
		ptr (fake).fetch_add (1 ,std::memory_order_relaxed) ;
	}

	void decrease () const override {
		ptr (fake).fetch_sub (1 ,std::memory_order_relaxed) ;
	}
} ;

exports VFat<AtomicHolder> AtomicHolder::hold (VREF<AtomicLayout> that) {
	return VFat<AtomicHolder> (AtomicImplHolder () ,that) ;
}

exports CFat<AtomicHolder> AtomicHolder::hold (CREF<AtomicLayout> that) {
	return CFat<AtomicHolder> (AtomicImplHolder () ,that) ;
}

struct MutexType {
	enum {
		Basic ,
		Once ,
		OnceDone ,
		Shared ,
		Unique ,
		ETC
	} ;
} ;

struct MutexImplLayout {
	Just<MutexType> mType ;
	Box<std::mutex> mBasic ;
	Atomic mShared ;
	Box<std::condition_variable> mUnique ;
} ;

class MutexImplHolder final implement Fat<MutexHolder ,MutexLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<MutexImplLayout>::make () ;
		fake.mThis->mType = MutexType::Basic ;
		fake.mThis->mBasic.remake () ;
	}

	Ref<MutexImplLayout> borrow () const leftvalue override {
		assert (fake.mThis.exist ()) ;
		return Ref<MutexImplLayout>::reference (fake.mThis.self) ;
	}

	void enter () const override {
		if (done ())
			return ;
		fake.mThis->mBasic->lock () ;
	}

	void leave () const override {
		if (done ())
			return ;
		replace (fake.mThis->mType ,FLAG (MutexType::Once) ,MutexType::OnceDone) ;
		fake.mThis->mBasic->unlock () ;
	}

	BOOL done () const override {
		if (!fake.mThis.exist ())
			return TRUE ;
		if (fake.mThis->mType == MutexType::OnceDone)
			return TRUE ;
		return FALSE ;
	}
} ;

exports VFat<MutexHolder> MutexHolder::hold (VREF<MutexLayout> that) {
	return VFat<MutexHolder> (MutexImplHolder () ,that) ;
}

exports CFat<MutexHolder> MutexHolder::hold (CREF<MutexLayout> that) {
	return CFat<MutexHolder> (MutexImplHolder () ,that) ;
}

class MakeMutexImplHolder final implement Fat<MakeMutexHolder ,MutexLayout> {
public:
	void OnceMutex_initialize () override {
		fake.mThis = SharedRef<MutexImplLayout>::make () ;
		fake.mThis->mType = MutexType::Once ;
		fake.mThis->mBasic.remake () ;
	}

	void SharedMutex_initialize () override {
		fake.mThis = SharedRef<MutexImplLayout>::make () ;
		fake.mThis->mType = MutexType::Shared ;
		fake.mThis->mBasic.remake () ;
		fake.mThis->mShared = NULL ;
	}

	void UniqueMutex_initialize () override {
		fake.mThis = SharedRef<MutexImplLayout>::make () ;
		fake.mThis->mType = MutexType::Unique ;
		fake.mThis->mBasic.remake () ;
		fake.mThis->mUnique.remake () ;
	}
} ;

exports VFat<MakeMutexHolder> MakeMutexHolder::hold (VREF<MutexLayout> that) {
	return VFat<MakeMutexHolder> (MakeMutexImplHolder () ,that) ;
}

exports CFat<MakeMutexHolder> MakeMutexHolder::hold (CREF<MutexLayout> that) {
	return CFat<MakeMutexHolder> (MakeMutexImplHolder () ,that) ;
}

class SharedAtomicMutex implement Proxy {
protected:
	Atomic mThat ;

public:
	static VREF<SharedAtomicMutex> from (VREF<Atomic> that) {
		return Pointer::from (that) ;
	}

	void lock () {
		noop () ;
	}

	void unlock () {
		mThat.decrease () ;
	}
} ;

struct SharedLockImplLayout {
	Ref<MutexImplLayout> mMutex ;
	std::unique_lock<SharedAtomicMutex> mLock ;
} ;

class SharedLockImplHolder final implement Fat<SharedLockHolder ,SharedLockLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mThis = Box<SharedLockImplLayout>::make () ;
		fake.mThis->mMutex = mutex.borrow () ;
		assert (ptr (fake).mType == MutexType::Shared) ;
		shared_enter () ;
		fake.mThis->mLock = std::unique_lock<SharedAtomicMutex> (SharedAtomicMutex::from (ptr (fake).mShared)) ;
	}

	static VREF<MutexImplLayout> ptr (CREF<SharedLockLayout> layout) {
		return layout.mThis->mMutex.pin ().self ;
	}

	void shared_enter () {
		if ifdo (TRUE) {
			auto rax = ptr (fake).mShared.fetch () ;
			while (TRUE) {
				rax = MathProc::abs (rax) ;
				const auto r1x = ptr (fake).mShared.change (rax ,rax + 1) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
		std::atomic_thread_fence (std::memory_order_acquire) ;
	}

	BOOL busy () const override {
		return ptr (fake).mShared.fetch () != IDEN ;
	}

	void enter () const override {
		ptr (fake).mShared.decrease () ;
		ptr (fake).mBasic->lock () ;
		if ifdo (TRUE) {
			auto rax = ZERO ;
			while (TRUE) {
				rax = ZERO ;
				const auto r1x = ptr (fake).mShared.change (rax ,NONE) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
	}

	void leave () const override {
		std::atomic_thread_fence (std::memory_order_release) ;
		ptr (fake).mShared.replace (NONE ,IDEN) ;
		ptr (fake).mBasic->unlock () ;
	}
} ;

exports VFat<SharedLockHolder> SharedLockHolder::hold (VREF<SharedLockLayout> that) {
	return VFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

exports CFat<SharedLockHolder> SharedLockHolder::hold (CREF<SharedLockLayout> that) {
	return CFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

struct UniqueLockImplLayout {
	Ref<MutexImplLayout> mMutex ;
	std::unique_lock<std::mutex> mLock ;
} ;

class UniqueLockImplHolder final implement Fat<UniqueLockHolder ,UniqueLockLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mThis = Box<UniqueLockImplLayout>::make () ;
		fake.mThis->mMutex = mutex.borrow () ;
		assert (ptr (fake).mType == MutexType::Unique) ;
		fake.mThis->mLock = std::unique_lock<std::mutex> (ptr (fake).mBasic.self) ;
	}

	static VREF<MutexImplLayout> ptr (CREF<UniqueLockLayout> layout) {
		return layout.mThis->mMutex.pin ().self ;
	}

	void wait () override {
		ptr (fake).mUnique->wait (fake.mThis->mLock) ;
	}

	void wait (CREF<Time> time) override {
		const auto r1x = time.borrow () ;
		ptr (fake).mUnique->wait_for (fake.mThis->mLock ,r1x->mTime) ;
	}

	void notify () override {
		ptr (fake).mUnique->notify_all () ;
	}

	void yield () override {
		fake.mThis->mLock = std::unique_lock<std::mutex> () ;
		std::this_thread::yield () ;
		fake.mThis->mLock = std::unique_lock<std::mutex> (ptr (fake).mBasic.self) ;
	}
} ;

exports VFat<UniqueLockHolder> UniqueLockHolder::hold (VREF<UniqueLockLayout> that) {
	return VFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

exports CFat<UniqueLockHolder> UniqueLockHolder::hold (CREF<UniqueLockLayout> that) {
	return CFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

struct ThreadImplLayout {
	Ref<VFat<ThreadFriend>> mExecutor ;
	FLAG mUid ;
	INDEX mSlot ;
	Box<std::thread> mThread ;

public:
	implicit ThreadImplLayout () = default ;

	implicit ~ThreadImplLayout () noexcept {
		assert (mThread == NULL) ;
	}
} ;

class ThreadImplHolder final implement Fat<ThreadHolder ,ThreadLayout> {
public:
	void initialize (RREF<Ref<VFat<ThreadFriend>>> executor ,CREF<INDEX> slot) override {
		fake.mThis = AutoRef<ThreadImplLayout>::make () ;
		fake.mThis->mExecutor = move (executor) ;
		fake.mThis->mUid = ZERO ;
		fake.mThis->mSlot = slot ;
	}

	FLAG thread_uid () const override {
		return fake.mThis->mUid ;
	}

	void start () override {
		auto &&rax = fake.mThis.self ;
		fake.mThis->mThread = Box<std::thread>::make ([&] () {
			rax.mUid = RuntimeProc::thread_uid () ;
			rax.mExecutor->self.friend_execute (rax.mSlot) ;
		}) ;
	}

	void stop () override {
		if (!fake.mThis.exist ())
			return ;
		if (fake.mThis->mThread == NULL)
			return ;
		fake.mThis->mThread->join () ;
		fake.mThis->mThread = NULL ;
	}
} ;

exports VFat<ThreadHolder> ThreadHolder::hold (VREF<ThreadLayout> that) {
	return VFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

exports CFat<ThreadHolder> ThreadHolder::hold (CREF<ThreadLayout> that) {
	return CFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

template class External<ProcessHolder ,ProcessLayout> ;

exports VFat<ProcessHolder> ProcessHolder::hold (VREF<ProcessLayout> that) {
	return VFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::linkage () ,that) ;
}

exports CFat<ProcessHolder> ProcessHolder::hold (CREF<ProcessLayout> that) {
	return CFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::linkage () ,that) ;
}

template class External<LibraryHolder ,LibraryLayout> ;

exports VFat<LibraryHolder> LibraryHolder::hold (VREF<LibraryLayout> that) {
	return VFat<LibraryHolder> (External<LibraryHolder ,LibraryLayout>::linkage () ,that) ;
}

exports CFat<LibraryHolder> LibraryHolder::hold (CREF<LibraryLayout> that) {
	return CFat<LibraryHolder> (External<LibraryHolder ,LibraryLayout>::linkage () ,that) ;
}

struct SystemImplLayout {
	std::locale mLocale ;
} ;

class SystemImplHolder final implement Fat<SystemHolder ,SystemLayout> {
public:
	void initialize () override {
		fake.mThis = AutoRef<SystemImplLayout>::make () ;
	}

	void set_locale (CREF<String<STR>> name) override {
		const auto r1x = StringProc::stra_from_strs (name) ;
		fake.mThis->mLocale = std::locale (r1x) ;
	}

	void execute (CREF<String<STR>> command) const override {
		const auto r1x = StringProc::stra_from_strs (command) ;
		const auto r2x = FLAG (std::system (r1x)) ;
		noop (r2x) ;
	}
} ;

exports VFat<SystemHolder> SystemHolder::hold (VREF<SystemLayout> that) {
	return VFat<SystemHolder> (SystemImplHolder () ,that) ;
}

exports CFat<SystemHolder> SystemHolder::hold (CREF<SystemLayout> that) {
	return CFat<SystemHolder> (SystemImplHolder () ,that) ;
}

struct RandomNormal {
	BOOL mOdd ;
	FLT64 mNX ;
	FLT64 mNY ;
} ;

struct RandomImplLayout {
	FLAG mSeed ;
	Box<std::mt19937_64> mRandom ;
	RandomNormal mNormal ;
} ;

class RandomImplHolder final implement Fat<RandomHolder ,RandomLayout> {
public:
	void initialize () override {
		const auto r1x = invoke (std::random_device ()) ;
		initialize (r1x) ;
	}

	void initialize (CREF<FLAG> seed) override {
		fake.mThis = SharedRef<RandomImplLayout>::make () ;
		fake.mThis->mSeed = seed ;
		fake.mThis->mRandom.remake () ;
		fake.mThis->mRandom.self = std::mt19937_64 (seed) ;
		fake.mThis->mNormal.mOdd = FALSE ;
	}

	FLAG seed () const override {
		return fake.mThis->mSeed ;
	}

	QUAD random_byte () const {
		return QUAD (fake.mThis->mRandom.self ()) ;
	}

	INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const override {
		assert (lb <= rb) ;
		const auto r1x = VAL64 (rb) - VAL64 (lb) + 1 ;
		const auto r2x = VAL64 (random_byte ()) & VAL64_MAX ;
		const auto r3x = INDEX (r2x % r1x) + lb ;
		return r3x ;
	}

	Array<INDEX> random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_) const override {
		Array<INDEX> ret = Array<INDEX>::make (iter (0 ,size_)) ;
		random_shuffle (length_ ,size_ ,ret) ;
		return move (ret) ;
	}

	void random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> result) const override {
		assert (length_ >= 0) ;
		assert (length_ <= size_) ;
		assert (result.size () == size_) ;
		const auto r1x = result.size () - 1 ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= length_)
				break ;
			INDEX iy = random_value (ix ,r1x) ;
			swap (result[ix] ,result[iy]) ;
			ix++ ;
		}
	}

	BitSet random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_) const override {
		BitSet ret = BitSet (size_) ;
		random_pick (length_ ,size_ ,ret) ;
		return move (ret) ;
	}

	void random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<BitSet> result) const override {
		assert (length_ >= 0) ;
		assert (length_ <= size_) ;
		assert (result.size () == size_) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (length_ >= size_ / 2)
				discard ;
			const auto r1x = random_shuffle (length_ ,size_) ;
			for (auto &&i : iter (0 ,length_))
				result.add (r1x[i]) ;
		}
		if ifdo (act) {
			const auto r2x = random_shuffle (size_ - length_ ,size_) ;
			for (auto &&i : iter (size_ - length_ ,size_))
				result.add (r2x[i]) ;
		}
	}

	BOOL random_draw (CREF<FLT64> possibility) const override {
		const auto r1x = random_value (0 ,10000) ;
		const auto r2x = FLT64 (r1x) * MathProc::inverse (FLT64 (10000)) ;
		if (r2x < possibility)
			return TRUE ;
		return FALSE ;
	}

	FLT64 random_normal () const override {
		if ifdo (TRUE) {
			if (fake.mThis->mNormal.mOdd)
				discard ;
			const auto r1x = random_value (1 ,10000) ;
			const auto r2x = FLT64 (r1x) * MathProc::inverse (FLT64 (10000)) ;
			const auto r3x = random_value (0 ,10000) ;
			const auto r4x = FLT64 (r3x) * MathProc::inverse (FLT64 (10000)) ;
			const auto r5x = MathProc::sqrt (FLT64 (-2) * MathProc::log (r2x)) ;
			const auto r6x = MATH_PI * 2 * r4x ;
			fake.mThis->mNormal.mNX = r5x * MathProc::cos (r6x) ;
			fake.mThis->mNormal.mNY = r5x * MathProc::sin (r6x) ;
			fake.mThis->mNormal.mOdd = TRUE ;
			return fake.mThis->mNormal.mNX ;
		}
		fake.mThis->mNormal.mOdd = FALSE ;
		return fake.mThis->mNormal.mNY ;
	}
} ;

exports VFat<RandomHolder> RandomHolder::hold (VREF<RandomLayout> that) {
	return VFat<RandomHolder> (RandomImplHolder () ,that) ;
}

exports CFat<RandomHolder> RandomHolder::hold (CREF<RandomLayout> that) {
	return CFat<RandomHolder> (RandomImplHolder () ,that) ;
}

template class External<SingletonProcHolder ,SingletonProcLayout> ;

exports CREF<SingletonProcLayout> SingletonProcHolder::instance () {
	return memorize ([&] () {
		SingletonProcLayout ret ;
		SingletonProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<SingletonProcHolder> SingletonProcHolder::hold (VREF<SingletonProcLayout> that) {
	return VFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::linkage () ,that) ;
}

exports CFat<SingletonProcHolder> SingletonProcHolder::hold (CREF<SingletonProcLayout> that) {
	return CFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::linkage () ,that) ;
}

struct GlobalNode {
	FLAG mHolder ;
	AutoRef<Pointer> mValue ;
} ;

struct GlobalImplLayout {
	Mutex mMutex ;
	Set<Slice> mGlobalNameSet ;
	List<GlobalNode> mGlobalList ;
	BOOL mFinalize ;
} ;

class GlobalImplHolder final implement Fat<GlobalHolder ,GlobalLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<GlobalImplLayout>::make () ;
		fake.mThis->mMutex = NULL ;
		fake.mThis->mFinalize = FALSE ;
		fake.mIndex = NONE ;
	}

	void initialize (CREF<Slice> name ,CREF<Unknown> holder) override {
		fake.mThis = Singleton<GlobalRoot>::instance ().mThis ;
		assert (!fake.mThis->mFinalize) ;
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		INDEX ix = fake.mThis->mGlobalNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = fake.mThis->mGlobalList.insert () ;
			fake.mThis->mGlobalNameSet.add (name ,ix) ;
			fake.mThis->mGlobalList[ix].mHolder = inline_vptr (holder) ;
		}
		fake.mIndex = ix ;
		ClazzHolder::hold (fake.mClazz)->initialize (holder) ;
	}

	void startup () const override {
		auto &&rax = Singleton<GlobalRoot>::instance ().mThis.self ;
		assume (!rax.mFinalize) ;
	}

	void shutdown () const override {
		auto &&rax = Singleton<GlobalRoot>::instance ().mThis.self ;
		if (rax.mFinalize)
			return ;
		rax.mFinalize = TRUE ;
		rax.mGlobalNameSet.clear () ;
		rax.mGlobalList.clear () ;
	}

	BOOL exist () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		INDEX ix = fake.mIndex ;
		const auto r1x = fake.mClazz ;
		const auto r2x = fake.mThis->mGlobalList[ix].mValue.clazz () ;
		return r1x == r2x ;
	}

	AutoRef<Pointer> fetch () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		INDEX ix = fake.mIndex ;
		assume (fake.mThis->mGlobalList[ix].mValue.exist ()) ;
		const auto r1x = Unknown (fake.mThis->mGlobalList[ix].mHolder) ;
		AutoRef<Pointer> ret = AutoRef<Pointer> (r1x) ;
		const auto r2x = RFat<ReflectClone> (r1x) ;
		r2x->clone (ret ,fake.mThis->mGlobalList[ix].mValue) ;
		return move (ret) ;
	}

	void store (RREF<AutoRef<Pointer>> item) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		INDEX ix = fake.mIndex ;
		assume (!fake.mThis->mGlobalList[ix].mValue.exist ()) ;
		fake.mThis->mGlobalList[ix].mValue = move (item) ;
	}
} ;

exports CREF<GlobalLayout> GlobalHolder::instance () {
	return memorize ([&] () {
		GlobalLayout ret ;
		GlobalHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<GlobalHolder> GlobalHolder::hold (VREF<GlobalLayout> that) {
	return VFat<GlobalHolder> (GlobalImplHolder () ,that) ;
}

exports CFat<GlobalHolder> GlobalHolder::hold (CREF<GlobalLayout> that) {
	return CFat<GlobalHolder> (GlobalImplHolder () ,that) ;
}
} ;