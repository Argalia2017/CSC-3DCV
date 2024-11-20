#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_runtime.hpp"

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <exception>
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

class TimeImplHolder implement Fat<TimeHolder ,TimeLayout> {
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

	Ref<TimeImplLayout> borrow () const override {
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

class MakeTimeImplHolder implement Fat<MakeTimeHolder ,TimeLayout> {
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

exports VFat<RuntimeProcHolder> RuntimeProcHolder::hold (VREF<RuntimeProcLayout> that) {
	return VFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::instance () ,that) ;
}

exports CFat<RuntimeProcHolder> RuntimeProcHolder::hold (CREF<RuntimeProcLayout> that) {
	return CFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::instance () ,that) ;
}

struct AtomicImplLayout {
	Pin<std::atomic<VAL>> mValue ;
} ;

class AtomicImplHolder implement Fat<AtomicHolder ,AtomicLayout> {
public:
	void initialize () override {
		fake.mThis = Box<AtomicImplLayout>::make () ;
		fake.mThis->mValue.self = 0 ;
	}

	VAL fetch () const override {
		if (fake.mThis == NULL)
			return 0 ;
		return fake.mThis->mValue->load (std::memory_order_relaxed) ;
	}

	void store (CREF<VAL> item) const override {
		return fake.mThis->mValue->store (item ,std::memory_order_relaxed) ;
	}

	VAL exchange (CREF<VAL> item) const override {
		return fake.mThis->mValue->exchange (item ,std::memory_order_relaxed) ;
	}

	BOOL change (VREF<VAL> expect ,CREF<VAL> item) const override {
		return fake.mThis->mValue->compare_exchange_weak (expect ,item ,std::memory_order_relaxed) ;
	}

	void replace (CREF<VAL> expect ,CREF<VAL> item) const override {
		auto rax = expect ;
		fake.mThis->mValue->compare_exchange_strong (rax ,item ,std::memory_order_relaxed) ;
	}

	void increase () const override {
		fake.mThis->mValue->fetch_add (1 ,std::memory_order_relaxed) ;
	}

	void decrease () const override {
		fake.mThis->mValue->fetch_sub (1 ,std::memory_order_relaxed) ;
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
		Mutex ,
		Recursive ,
		Shared ,
		Unique ,
		ETC
	} ;
} ;

struct MutexImplLayout {
	Just<MutexType> mType ;
	Pin<Box<std::mutex>> mMutex ;
	Pin<Box<std::recursive_mutex>> mRecursive ;
	Atomic mShared ;
	Pin<Box<std::condition_variable>> mUnique ;
} ;

class MutexImplHolder implement Fat<MutexHolder ,MutexLayout> {
public:
	void initialize () override {
		fake.mThis = AutoRef<MutexImplLayout>::make () ;
		fake.mThis->mType = MutexType::Mutex ;
		fake.mThis->mMutex->remake () ;
	}

	Ref<MutexImplLayout> borrow () const override {
		assert (fake.mThis.exist ()) ;
		return Ref<MutexImplLayout>::reference (fake.mThis.self) ;
	}

	void enter () const override {
		if (!fake.mThis.exist ())
			return ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mThis->mType != MutexType::Recursive)
				discard ;
			fake.mThis->mRecursive->self.lock () ;
		}
		if ifdo (act) {
			fake.mThis->mMutex->self.lock () ;
		}
	}

	void leave () const override {
		if (!fake.mThis.exist ())
			return ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mThis->mType != MutexType::Recursive)
				discard ;
			fake.mThis->mRecursive->self.unlock () ;
		}
		if ifdo (act) {
			fake.mThis->mMutex->self.unlock () ;
		}
	}
} ;

exports VFat<MutexHolder> MutexHolder::hold (VREF<MutexLayout> that) {
	return VFat<MutexHolder> (MutexImplHolder () ,that) ;
}

exports CFat<MutexHolder> MutexHolder::hold (CREF<MutexLayout> that) {
	return CFat<MutexHolder> (MutexImplHolder () ,that) ;
}

class MakeMutexImplHolder implement Fat<MakeMutexHolder ,MutexLayout> {
public:
	void RecursiveMutex_initialize () override {
		fake.mThis = AutoRef<MutexImplLayout>::make () ;
		fake.mThis->mType = MutexType::Recursive ;
		fake.mThis->mRecursive->remake () ;
	}

	void SharedMutex_initialize () override {
		fake.mThis = AutoRef<MutexImplLayout>::make () ;
		fake.mThis->mType = MutexType::Shared ;
		fake.mThis->mShared = NULL ;
	}

	void UniqueMutex_initialize () override {
		fake.mThis = AutoRef<MutexImplLayout>::make () ;
		fake.mThis->mType = MutexType::Unique ;
		fake.mThis->mMutex->remake () ;
		fake.mThis->mUnique->remake () ;
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
	imports VREF<SharedAtomicMutex> from (VREF<Atomic> that) {
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

class SharedLockImplHolder implement Fat<SharedLockHolder ,SharedLockLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mThis = Box<SharedLockImplLayout>::make () ;
		fake.mThis->mMutex = mutex.borrow () ;
		assert (fake.mThis->mMutex->mType == MutexType::Shared) ;
		shared_enter () ;
		auto &&rax = fake.mThis->mMutex.leak () ;
		fake.mThis->mLock = std::unique_lock<SharedAtomicMutex> (SharedAtomicMutex::from (rax.mShared)) ;
	}

	void shared_enter () {
		if ifdo (TRUE) {
			auto rax = fake.mThis->mMutex->mShared.fetch () ;
			while (TRUE) {
				rax = MathProc::abs (rax) ;
				const auto r1x = fake.mThis->mMutex->mShared.change (rax ,rax + 1) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
		std::atomic_thread_fence (std::memory_order_acquire) ;
	}

	BOOL busy () const override {
		return fake.mThis->mMutex->mShared.fetch () != IDEN ;
	}

	void enter () const override {
		fake.mThis->mMutex->mShared.decrease () ;
		fake.mThis->mMutex->mMutex->self.lock () ;
		if ifdo (TRUE) {
			auto rax = ZERO ;
			while (TRUE) {
				rax = ZERO ;
				const auto r1x = fake.mThis->mMutex->mShared.change (rax ,NONE) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
	}

	void leave () const override {
		std::atomic_thread_fence (std::memory_order_release) ;
		fake.mThis->mMutex->mShared.replace (NONE ,IDEN) ;
		fake.mThis->mMutex->mMutex->self.unlock () ;
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

class UniqueLockImplHolder implement Fat<UniqueLockHolder ,UniqueLockLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mThis = Box<UniqueLockImplLayout>::make () ;
		fake.mThis->mMutex = mutex.borrow () ;
		assert (fake.mThis->mMutex->mType == MutexType::Unique) ;
		fake.mThis->mLock = std::unique_lock<std::mutex> (fake.mThis->mMutex->mMutex->self) ;
	}

	void wait () override {
		fake.mThis->mMutex->mUnique->self.wait (fake.mThis->mLock) ;
	}

	void wait (CREF<Time> time) override {
		const auto r1x = time.borrow () ;
		fake.mThis->mMutex->mUnique->self.wait_for (fake.mThis->mLock ,r1x->mTime) ;
	}

	void notify () override {
		fake.mThis->mMutex->mUnique->self.notify_all () ;
	}

	void yield () override {
		fake.mThis->mLock = std::unique_lock<std::mutex> () ;
		std::this_thread::yield () ;
		fake.mThis->mLock = std::unique_lock<std::mutex> (fake.mThis->mMutex->mMutex->self) ;
	}
} ;

exports VFat<UniqueLockHolder> UniqueLockHolder::hold (VREF<UniqueLockLayout> that) {
	return VFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

exports CFat<UniqueLockHolder> UniqueLockHolder::hold (CREF<UniqueLockLayout> that) {
	return CFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

struct ThreadImplLayout {
	Ref<ThreadFriend> mExecutor ;
	FLAG mUid ;
	INDEX mSlot ;
	std::thread mThread ;
} ;

class ThreadImplHolder implement Fat<ThreadHolder ,ThreadLayout> {
public:
	void initialize (RREF<Ref<ThreadFriend>> executor ,CREF<INDEX> slot) override {
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
		fake.mThis->mThread = std::thread ([&] () {
			rax.mUid = RuntimeProc::thread_uid () ;
			auto &&rbx = rax.mExecutor.leak () ;
			rbx.friend_execute (rax.mSlot) ;
		}) ;
	}

	void stop () override {
		if (!fake.mThis.exist ())
			return ;
		fake.mThis->mThread.join () ;
		fake.mThis->mThread = std::thread () ;
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
	return VFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::instance () ,that) ;
}

exports CFat<ProcessHolder> ProcessHolder::hold (CREF<ProcessLayout> that) {
	return CFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::instance () ,that) ;
}

template class External<ModuleHolder ,ModuleLayout> ;

exports VFat<ModuleHolder> ModuleHolder::hold (VREF<ModuleLayout> that) {
	return VFat<ModuleHolder> (External<ModuleHolder ,ModuleLayout>::instance () ,that) ;
}

exports CFat<ModuleHolder> ModuleHolder::hold (CREF<ModuleLayout> that) {
	return CFat<ModuleHolder> (External<ModuleHolder ,ModuleLayout>::instance () ,that) ;
}

class SystemImplHolder implement Fat<SystemHolder ,SystemLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void set_locale (CREF<String<STR>> name) const override {
		const auto r1x = StringProc::stra_from_strs (name) ;
		std::setlocale (LC_ALL ,r1x) ;
	}

	FLAG execute (CREF<String<STR>> command) const override {
		const auto r1x = StringProc::stra_from_strs (command) ;
		return FLAG (std::system (r1x)) ;
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
	Pin<std::mt19937_64> mRandom ;
	SharedRef<RandomNormal> mNormal ;
} ;

class RandomImplHolder implement Fat<RandomHolder ,RandomLayout> {
public:
	void initialize (CREF<FLAG> seed) override {
		fake.mThis = AutoRef<RandomImplLayout>::make () ;
		fake.mThis->mSeed = seed ;
		fake.mThis->mRandom.self = std::mt19937_64 (seed) ;
		fake.mThis->mNormal = SharedRef<RandomNormal>::make () ;
		fake.mThis->mNormal->mOdd = FALSE ;
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
		const auto r2x = VAL64 (random_byte () & QUAD (VAL64_MAX)) ;
		return INDEX (r2x % r1x + lb) ;
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
			if (fake.mThis->mNormal->mOdd)
				discard ;
			const auto r1x = random_value (1 ,10000) ;
			const auto r2x = FLT64 (r1x) * MathProc::inverse (FLT64 (10000)) ;
			const auto r3x = random_value (0 ,10000) ;
			const auto r4x = FLT64 (r3x) * MathProc::inverse (FLT64 (10000)) ;
			const auto r5x = MathProc::sqrt (FLT64 (-2) * MathProc::log (r2x)) ;
			const auto r6x = MATH_PI * 2 * r4x ;
			fake.mThis->mNormal->mNX = r5x * MathProc::cos (r6x) ;
			fake.mThis->mNormal->mNY = r5x * MathProc::sin (r6x) ;
			fake.mThis->mNormal->mOdd = TRUE ;
			return fake.mThis->mNormal->mNX ;
		}
		fake.mThis->mNormal->mOdd = FALSE ;
		return fake.mThis->mNormal->mNY ;
	}
} ;

exports VFat<RandomHolder> RandomHolder::hold (VREF<RandomLayout> that) {
	return VFat<RandomHolder> (RandomImplHolder () ,that) ;
}

exports CFat<RandomHolder> RandomHolder::hold (CREF<RandomLayout> that) {
	return CFat<RandomHolder> (RandomImplHolder () ,that) ;
}

template class External<SingletonProcHolder ,SingletonProcLayout> ;

exports VFat<SingletonProcHolder> SingletonProcHolder::hold (VREF<SingletonProcLayout> that) {
	return VFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::instance () ,that) ;
}

exports CFat<SingletonProcHolder> SingletonProcHolder::hold (CREF<SingletonProcLayout> that) {
	return CFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::instance () ,that) ;
}

#ifdef __CSC_SYSTEM_WINDOWS__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		noop () ;
	}
} ;
#endif

static constexpr auto dump_memory_leaks = FUNCTION_dump_memory_leaks () ;

struct GlobalNode {
	FLAG mHolder ;
	AutoRef<Pointer> mValue ;
} ;

struct GlobalImplLayout {
	Mutex mMutex ;
	Set<Slice> mGlobalNameSet ;
	List<GlobalNode> mGlobalList ;
} ;

class GlobalImplHolder implement Fat<GlobalHolder ,GlobalLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<GlobalImplLayout>::make () ;
		fake.mThis->mMutex = RecursiveMutex () ;
		fake.mIndex = NONE ;
		dump_memory_leaks () ;
	}

	void initialize (CREF<Slice> name ,CREF<Unknown> holder ,CREF<GlobalLayout> root) override {
		fake.mThis = root.mThis ;
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

exports VFat<GlobalHolder> GlobalHolder::hold (VREF<GlobalLayout> that) {
	return VFat<GlobalHolder> (GlobalImplHolder () ,that) ;
}

exports CFat<GlobalHolder> GlobalHolder::hold (CREF<GlobalLayout> that) {
	return CFat<GlobalHolder> (GlobalImplHolder () ,that) ;
}

template class External<PathHolder ,PathLayout> ;

exports VFat<PathHolder> PathHolder::hold (VREF<PathLayout> that) {
	return VFat<PathHolder> (External<PathHolder ,PathLayout>::instance () ,that) ;
}

exports CFat<PathHolder> PathHolder::hold (CREF<PathLayout> that) {
	return CFat<PathHolder> (External<PathHolder ,PathLayout>::instance () ,that) ;
}

template class External<FileProcHolder ,FileProcLayout> ;

exports VFat<FileProcHolder> FileProcHolder::hold (VREF<FileProcLayout> that) {
	return VFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::instance () ,that) ;
}

exports CFat<FileProcHolder> FileProcHolder::hold (CREF<FileProcLayout> that) {
	return CFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::instance () ,that) ;
}

template class External<StreamFileHolder ,StreamFileLayout> ;

exports VFat<StreamFileHolder> StreamFileHolder::hold (VREF<StreamFileLayout> that) {
	return VFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::instance () ,that) ;
}

exports CFat<StreamFileHolder> StreamFileHolder::hold (CREF<StreamFileLayout> that) {
	return CFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::instance () ,that) ;
}

class StreamFileByteWriterImplHolder implement Fat<StreamFileByteWriterHolder ,StreamFileByteWriterLayout> {
private:
	using STREAM_FILE_BUFFER_SIZE = ENUM<4096> ;

public:
	void initialize (CREF<String<STR>> file) override {
		fake.mFile = StreamFile (file) ;
		fake.mFile.open_w (0) ;
		fake.mFileBuffer = RefBuffer<BYTE> (STREAM_FILE_BUFFER_SIZE::expr) ;
		set_writer (fake) ;
	}

	void set_writer (VREF<ByteWriter> writer) {
		writer = ByteWriter (Ref<RefBuffer<BYTE>>::reference (fake.mFileBuffer)) ;
		fake.use_overflow ([&] (VREF<ByteWriter> writer) {
			auto &&rax = keep[TYPE<StreamFileByteWriterLayout>::expr] (writer) ;
			rax.mFile.write (rax.mFileBuffer) ;
			rax.reset () ;
		}) ;
	}

	void close () override {
		const auto r1x = fake.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (fake.mFileBuffer.self) ;
		fake.mFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		fake.reset () ;
		fake.mFile.flush () ;
	}
} ;

exports VFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (VREF<StreamFileByteWriterLayout> that) {
	return VFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

exports CFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (CREF<StreamFileByteWriterLayout> that) {
	return CFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

class StreamFileTextWriterImplHolder implement Fat<StreamFileTextWriterHolder ,StreamFileTextWriterLayout> {
private:
	using STREAM_FILE_BUFFER_SIZE = ENUM<4096> ;

public:
	void initialize (CREF<String<STR>> file) override {
		fake.mFile = StreamFile (file) ;
		fake.mFile.open_w (0) ;
		fake.mFileBuffer = RefBuffer<BYTE> (STREAM_FILE_BUFFER_SIZE::expr) ;
		set_writer (fake) ;
	}

	void set_writer (VREF<TextWriter> writer) {
		writer = TextWriter (Ref<RefBuffer<BYTE>>::reference (fake.mFileBuffer)) ;
		fake.use_overflow ([&] (VREF<TextWriter> writer) {
			auto &&rax = keep[TYPE<StreamFileTextWriterLayout>::expr] (writer) ;
			rax.mFile.write (rax.mFileBuffer) ;
			rax.reset () ;
		}) ;
	}

	void close () override {
		const auto r1x = fake.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (fake.mFileBuffer.self) ;
		fake.mFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		fake.reset () ;
		fake.mFile.flush () ;
	}
} ;

exports VFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (VREF<StreamFileTextWriterLayout> that) {
	return VFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

exports CFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (CREF<StreamFileTextWriterLayout> that) {
	return CFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

template class External<BufferFileHolder ,BufferFileLayout> ;

exports VFat<BufferFileHolder> BufferFileHolder::hold (VREF<BufferFileLayout> that) {
	return VFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::instance () ,that) ;
}

exports CFat<BufferFileHolder> BufferFileHolder::hold (CREF<BufferFileLayout> that) {
	return CFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::instance () ,that) ;
}

template class External<ConsoleHolder ,ConsoleLayout> ;

exports VFat<ConsoleHolder> ConsoleHolder::hold (VREF<ConsoleLayout> that) {
	return VFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::instance () ,that) ;
}

exports CFat<ConsoleHolder> ConsoleHolder::hold (CREF<ConsoleLayout> that) {
	return CFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::instance () ,that) ;
}
} ;