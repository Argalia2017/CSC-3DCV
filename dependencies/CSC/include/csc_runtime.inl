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
#ifdef __CSC_SYSTEM_WINDOWS__
struct FUNCTION_calendar_from_timepoint {
	forceinline std::tm operator() (CREF<std::time_t> time) const {
		std::tm ret ;
		inline_memset (ret) ;
		localtime_s ((&ret) ,(&time)) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_calendar_from_timepoint {
	forceinline std::tm operator() (CREF<std::time_t> time) const {
		const auto r1x = FLAG (std::localtime (&time)) ;
		return bitwise[TYPE<std::tm>::expr] (Pointer::make (r1x)) ;
	}
} ;
#endif

static constexpr auto calendar_from_timepoint = FUNCTION_calendar_from_timepoint () ;

struct TimeImplLayout {
	std::chrono::system_clock::duration mTime ;
} ;

class TimeImplHolder final implement Fat<TimeHolder ,TimeImplLayout> {
public:
	void initialize () override {
		const auto r1x = std::chrono::system_clock::now () ;
		fake.mTime = r1x.time_since_epoch () ;
	}

	void initialize (CREF<LENGTH> milliseconds_) override {
		const auto r1x = std::chrono::milliseconds (milliseconds_) ;
		fake.mTime = std::chrono::duration_cast<std::chrono::system_clock::duration> (r1x) ;
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
		fake.mTime = r6x.time_since_epoch () ;
	}

	void initialize (CREF<TimeImplLayout> that) override {
		fake.mTime = that.mTime ;
	}

	Ref<TimeImplLayout> borrow () const leftvalue override {
		return Ref<TimeImplLayout>::reference (fake) ;
	}

	LENGTH megaseconds () const override {
		using R1X = std::chrono::duration<csc_int64_t ,std::ratio<10000000>> ;
		const auto r1x = std::chrono::duration_cast<R1X> (fake.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH kiloseconds () const override {
		using R1X = std::chrono::duration<csc_int64_t ,std::ratio<1000>> ;
		const auto r1x = std::chrono::duration_cast<R1X> (fake.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH seconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::seconds> (fake.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH milliseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::milliseconds> (fake.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH microseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::microseconds> (fake.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH nanoseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::nanoseconds> (fake.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	TimeCalendar calendar () const override {
		TimeCalendar ret ;
		const auto r1x = std::chrono::system_clock::time_point (fake.mTime) ;
		const auto r2x = std::time_t (std::chrono::system_clock::to_time_t (r1x)) ;
		const auto r3x = calendar_from_timepoint (r2x) ;
		ret.mYear = LENGTH (r3x.tm_year) + 1900 ;
		ret.mMonth = LENGTH (r3x.tm_mon) + 1 ;
		ret.mDay = LENGTH (r3x.tm_mday) ;
		ret.mWDay = LENGTH (r3x.tm_wday) + 1 ;
		ret.mYDay = LENGTH (r3x.tm_yday) + 1 ;
		ret.mHour = LENGTH (r3x.tm_hour) ;
		ret.mMinute = LENGTH (r3x.tm_min) ;
		ret.mSecond = LENGTH (r3x.tm_sec) ;
		return move (ret) ;
	}

	TimeLayout sadd (CREF<TimeImplLayout> that) const override {
		TimeLayout ret = TimeHolder::create () ;
		ret.mThis->mTime = fake.mTime + that.mTime ;
		return move (ret) ;
	}

	TimeLayout ssub (CREF<TimeImplLayout> that) const override {
		TimeLayout ret = TimeHolder::create () ;
		ret.mThis->mTime = fake.mTime - that.mTime ;
		return move (ret) ;
	}
} ;

exports TimeLayout TimeHolder::create () {
	TimeLayout ret ;
	ret.mThis = Box<TimeImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<TimeHolder> TimeHolder::hold (VREF<TimeImplLayout> that) {
	return VFat<TimeHolder> (TimeImplHolder () ,that) ;
}

exports CFat<TimeHolder> TimeHolder::hold (CREF<TimeImplLayout> that) {
	return CFat<TimeHolder> (TimeImplHolder () ,that) ;
}

template class External<RuntimeProcHolder ,RuntimeProcLayout> ;

exports CREF<RuntimeProcLayout> RuntimeProcHolder::instance () {
	return memorize ([&] () {
		RuntimeProcLayout ret ;
		ret.mThis = External<RuntimeProcHolder ,RuntimeProcLayout>::create () ;
		RuntimeProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<RuntimeProcHolder> RuntimeProcHolder::hold (VREF<RuntimeProcImplLayout> that) {
	return VFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::declare () ,that) ;
}

exports CFat<RuntimeProcHolder> RuntimeProcHolder::hold (CREF<RuntimeProcImplLayout> that) {
	return CFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::declare () ,that) ;
}

struct AtomicImplLayout {
	Pin<std::atomic<VAL>> mAtomic ;
} ;

class AtomicImplHolder final implement Fat<AtomicHolder ,AtomicImplLayout> {
public:
	void initialize () override {
		store (ZERO) ;
	}

	static VREF<std::atomic<VAL>> ptr (CREF<AtomicImplLayout> that) {
		return that.mAtomic.self ;
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

exports AtomicLayout AtomicHolder::create () {
	AtomicLayout ret ;
	ret.mThis = Box<AtomicImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<AtomicHolder> AtomicHolder::hold (VREF<AtomicImplLayout> that) {
	return VFat<AtomicHolder> (AtomicImplHolder () ,that) ;
}

exports CFat<AtomicHolder> AtomicHolder::hold (CREF<AtomicImplLayout> that) {
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

class MutexImplHolder final implement Fat<MutexHolder ,MutexImplLayout> {
public:
	void initialize () override {
		fake.mType = MutexType::Basic ;
		fake.mBasic.remake () ;
	}

	Ref<MutexImplLayout> borrow () leftvalue override {
		return Ref<MutexImplLayout>::reference (fake) ;
	}

	BOOL done () override {
		if (fake.mType == MutexType::OnceDone)
			return TRUE ;
		return FALSE ;
	}

	void enter () override {
		if (done ())
			return ;
		fake.mBasic->lock () ;
	}

	void leave () override {
		if (done ())
			return ;
		replace (fake.mType ,FLAG (MutexType::Once) ,MutexType::OnceDone) ;
		fake.mBasic->unlock () ;
	}
} ;

exports MutexLayout MutexHolder::create () {
	MutexLayout ret ;
	ret.mThis = SharedRef<MutexImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<MutexHolder> MutexHolder::hold (VREF<MutexImplLayout> that) {
	return VFat<MutexHolder> (MutexImplHolder () ,that) ;
}

exports CFat<MutexHolder> MutexHolder::hold (CREF<MutexImplLayout> that) {
	return CFat<MutexHolder> (MutexImplHolder () ,that) ;
}

class MakeMutexImplHolder final implement Fat<MakeMutexHolder ,MutexImplLayout> {
public:
	void make_OnceMutex () override {
		fake.mType = MutexType::Once ;
		fake.mBasic.remake () ;
	}

	void make_SharedMutex () override {
		fake.mType = MutexType::Shared ;
		fake.mBasic.remake () ;
		fake.mShared = NULL ;
	}

	void make_UniqueMutex () override {
		fake.mType = MutexType::Unique ;
		fake.mBasic.remake () ;
		fake.mUnique.remake () ;
	}
} ;

exports VFat<MakeMutexHolder> MakeMutexHolder::hold (VREF<MutexImplLayout> that) {
	return VFat<MakeMutexHolder> (MakeMutexImplHolder () ,that) ;
}

exports CFat<MakeMutexHolder> MakeMutexHolder::hold (CREF<MutexImplLayout> that) {
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

class SharedLockImplHolder final implement Fat<SharedLockHolder ,SharedLockImplLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mMutex = mutex.borrow () ;
		assert (fake.mMutex.exclusive ()) ;
		assert (ptr (fake).mType == MutexType::Shared) ;
		shared_enter () ;
		fake.mLock = std::unique_lock<SharedAtomicMutex> (SharedAtomicMutex::from (ptr (fake).mShared)) ;
	}

	static VREF<MutexImplLayout> ptr (CREF<SharedLockImplLayout> that) {
		return Pointer::make (address (that.mMutex.self)) ;
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

exports SharedLockLayout SharedLockHolder::create () {
	SharedLockLayout ret ;
	ret.mThis = Box<SharedLockImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<SharedLockHolder> SharedLockHolder::hold (VREF<SharedLockImplLayout> that) {
	return VFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

exports CFat<SharedLockHolder> SharedLockHolder::hold (CREF<SharedLockImplLayout> that) {
	return CFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

struct UniqueLockImplLayout {
	Ref<MutexImplLayout> mMutex ;
	std::unique_lock<std::mutex> mLock ;
} ;

class UniqueLockImplHolder final implement Fat<UniqueLockHolder ,UniqueLockImplLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mMutex = mutex.borrow () ;
		assert (fake.mMutex.exclusive ()) ;
		assert (ptr (fake).mType == MutexType::Unique) ;
		fake.mLock = std::unique_lock<std::mutex> (ptr (fake).mBasic.self) ;
	}

	static VREF<MutexImplLayout> ptr (CREF<UniqueLockImplLayout> that) {
		return Pointer::make (address (that.mMutex.self)) ;
	}

	void wait () override {
		ptr (fake).mUnique->wait (fake.mLock) ;
	}

	void wait (CREF<Time> time) override {
		const auto r1x = time.borrow () ;
		ptr (fake).mUnique->wait_for (fake.mLock ,r1x->mTime) ;
	}

	void notify () override {
		ptr (fake).mUnique->notify_all () ;
	}

	void yield () override {
		fake.mLock = std::unique_lock<std::mutex> () ;
		std::this_thread::yield () ;
		fake.mLock = std::unique_lock<std::mutex> (ptr (fake).mBasic.self) ;
	}
} ;

exports UniqueLockLayout UniqueLockHolder::create () {
	UniqueLockLayout ret ;
	ret.mThis = Box<UniqueLockImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<UniqueLockHolder> UniqueLockHolder::hold (VREF<UniqueLockImplLayout> that) {
	return VFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

exports CFat<UniqueLockHolder> UniqueLockHolder::hold (CREF<UniqueLockImplLayout> that) {
	return CFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

struct ThreadImplLayout {
	Box<VFat<ThreadBinder>> mExecutor ;
	FLAG mUid ;
	INDEX mSlot ;
	Box<std::thread> mThread ;

public:
	implicit ThreadImplLayout () = default ;

	implicit ~ThreadImplLayout () noexcept {
		assert (mThread == NULL) ;
	}
} ;

class ThreadImplHolder final implement Fat<ThreadHolder ,ThreadImplLayout> {
public:
	void initialize (RREF<Box<VFat<ThreadBinder>>> executor ,CREF<INDEX> slot) override {
		fake.mExecutor = move (executor) ;
		fake.mUid = ZERO ;
		fake.mSlot = slot ;
	}

	FLAG thread_uid () const override {
		return fake.mUid ;
	}

	void start () override {
		auto &&rax = fake ;
		fake.mThread = Box<std::thread>::make ([&] () {
			rax.mUid = RuntimeProc::thread_uid () ;
			rax.mExecutor.self->friend_execute (rax.mSlot) ;
		}) ;
	}

	void stop () override {
		if (fake.mThread == NULL)
			return ;
		fake.mThread->join () ;
		fake.mThread = NULL ;
	}
} ;

exports ThreadLayout ThreadHolder::create () {
	ThreadLayout ret ;
	ret.mThis = AutoRef<ThreadImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<ThreadHolder> ThreadHolder::hold (VREF<ThreadImplLayout> that) {
	return VFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

exports CFat<ThreadHolder> ThreadHolder::hold (CREF<ThreadImplLayout> that) {
	return CFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

template class External<ProcessHolder ,ProcessLayout> ;

exports ProcessLayout ProcessHolder::create () {
	ProcessLayout ret ;
	ret.mThis = External<ProcessHolder ,ProcessLayout>::create () ;
	return move (ret) ;
}

exports VFat<ProcessHolder> ProcessHolder::hold (VREF<ProcessImplLayout> that) {
	return VFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::declare () ,that) ;
}

exports CFat<ProcessHolder> ProcessHolder::hold (CREF<ProcessImplLayout> that) {
	return CFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::declare () ,that) ;
}

template class External<LibraryHolder ,LibraryLayout> ;

exports LibraryLayout LibraryHolder::create () {
	LibraryLayout ret ;
	ret.mThis = External<LibraryHolder ,LibraryLayout>::create () ;
	return move (ret) ;
}

exports VFat<LibraryHolder> LibraryHolder::hold (VREF<LibraryImplLayout> that) {
	return VFat<LibraryHolder> (External<LibraryHolder ,LibraryLayout>::declare () ,that) ;
}

exports CFat<LibraryHolder> LibraryHolder::hold (CREF<LibraryImplLayout> that) {
	return CFat<LibraryHolder> (External<LibraryHolder ,LibraryLayout>::declare () ,that) ;
}

struct SystemImplLayout {
	std::locale mLocale ;
} ;

class SystemImplHolder final implement Fat<SystemHolder ,SystemImplLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void set_locale (CREF<String<STR>> name) override {
		const auto r1x = StringProc::stra_from_strs (name) ;
		fake.mLocale = std::locale (r1x) ;
	}

	void execute (CREF<String<STR>> command) const override {
		const auto r1x = StringProc::stra_from_strs (command) ;
		const auto r2x = FLAG (std::system (r1x)) ;
		noop (r2x) ;
	}
} ;

exports SystemLayout SystemHolder::create () {
	SystemLayout ret ;
	ret.mThis = AutoRef<SystemImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<SystemHolder> SystemHolder::hold (VREF<SystemImplLayout> that) {
	return VFat<SystemHolder> (SystemImplHolder () ,that) ;
}

exports CFat<SystemHolder> SystemHolder::hold (CREF<SystemImplLayout> that) {
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

class RandomImplHolder final implement Fat<RandomHolder ,RandomImplLayout> {
public:
	void initialize () override {
		const auto r1x = invoke (std::random_device ()) ;
		initialize (r1x) ;
	}

	void initialize (CREF<FLAG> seed) override {
		fake.mSeed = seed ;
		fake.mRandom.remake () ;
		fake.mRandom.self = std::mt19937_64 (seed) ;
		fake.mNormal.mOdd = FALSE ;
	}

	FLAG seed () const override {
		return fake.mSeed ;
	}

	QUAD random_byte () {
		return QUAD (fake.mRandom.self ()) ;
	}

	VAL32 random_value (CREF<VAL32> min_ ,CREF<VAL32> max_) override {
		assert (min_ <= max_) ;
		const auto r1x = VAL32 (max_) - VAL32 (min_) + 1 ;
		assert (r1x > 0) ;
		const auto r2x = VAL32 (random_byte ()) & VAL32_MAX ;
		const auto r3x = r2x % r1x + min_ ;
		return r3x ;
	}

	VAL64 random_value (CREF<VAL64> min_ ,CREF<VAL64> max_) override {
		assert (min_ <= max_) ;
		const auto r1x = VAL64 (max_) - VAL64 (min_) + 1 ;
		assert (r1x > 0) ;
		const auto r2x = VAL64 (random_byte ()) & VAL64_MAX ;
		const auto r3x = r2x % r1x + min_ ;
		return r3x ;
	}

	FLT64 random_float (CREF<FLT64> scale) override {
		const auto r1x = VAL64 (scale) ;
		assert (r1x > 0) ;
		const auto r2x = FLT64 (random_value (VAL64 (0) ,r1x)) ;
		const auto r3x = r2x * MathProc::inverse (FLT64 (r1x)) ;
		return r3x ;
	}

	Array<INDEX> random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_) override {
		Array<INDEX> ret = Array<INDEX>::make (iter (0 ,size_)) ;
		random_shuffle (length_ ,size_ ,ret) ;
		return move (ret) ;
	}

	void random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> result) override {
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

	BitSet random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_) override {
		BitSet ret = BitSet (size_) ;
		random_pick (length_ ,size_ ,ret) ;
		return move (ret) ;
	}

	void random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<BitSet> result) override {
		assert (length_ >= 0) ;
		assert (length_ <= size_) ;
		assert (result.size () == size_) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (length_ >= size_ / 2)
				discard ;
			result.clear () ;
			const auto r1x = random_shuffle (length_ ,size_) ;
			for (auto &&i : iter (0 ,length_))
				result.add (r1x[i]) ;
		}
		if ifdo (act) {
			result.fill (BYTE (0XFF)) ;
			const auto r2x = random_shuffle (size_ - length_ ,size_) ;
			for (auto &&i : iter (size_ - length_ ,size_))
				result.erase (r2x[i]) ;
		}
	}

	BOOL random_draw (CREF<FLT64> possibility) override {
		if (random_float (10000) < possibility)
			return TRUE ;
		return FALSE ;
	}

	FLT64 random_normal () override {
		if ifdo (TRUE) {
			if (fake.mNormal.mOdd)
				discard ;
			const auto r1x = VAL64 (10000) ;
			const auto r2x = MathProc::min_of (random_float (r1x) + FLT64_EPS ,FLT64 (1)) ;
			const auto r3x = random_float (r1x) ;
			const auto r4x = MathProc::sqrt (FLT64 (-2) * MathProc::log (r2x)) ;
			const auto r5x = MATH_PI * 2 * r3x ;
			fake.mNormal.mNX = r4x * MathProc::cos (r5x) ;
			fake.mNormal.mNY = r4x * MathProc::sin (r5x) ;
			fake.mNormal.mOdd = TRUE ;
			return fake.mNormal.mNX ;
		}
		fake.mNormal.mOdd = FALSE ;
		return fake.mNormal.mNY ;
	}
} ;

exports RandomLayout RandomHolder::create () {
	RandomLayout ret ;
	ret.mThis = SharedRef<RandomImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<RandomHolder> RandomHolder::hold (VREF<RandomImplLayout> that) {
	return VFat<RandomHolder> (RandomImplHolder () ,that) ;
}

exports CFat<RandomHolder> RandomHolder::hold (CREF<RandomImplLayout> that) {
	return CFat<RandomHolder> (RandomImplHolder () ,that) ;
}

template class External<SingletonProcHolder ,SingletonProcLayout> ;

exports CREF<SingletonProcLayout> SingletonProcHolder::instance () {
	return memorize ([&] () {
		SingletonProcLayout ret ;
		ret.mThis = External<SingletonProcHolder ,SingletonProcLayout>::create () ;
		SingletonProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<SingletonProcHolder> SingletonProcHolder::hold (VREF<SingletonProcImplLayout> that) {
	return VFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::declare () ,that) ;
}

exports CFat<SingletonProcHolder> SingletonProcHolder::hold (CREF<SingletonProcImplLayout> that) {
	return CFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::declare () ,that) ;
}

struct GlobalNode {
	FLAG mHolder ;
	Pin<AutoRef<Pointer>> mValue ;
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
		fake.mThis = Ref<GlobalImplLayout>::make () ;
		fake.mThis->mMutex = NULL ;
		fake.mThis->mFinalize = FALSE ;
		fake.mIndex = NONE ;
	}

	void initialize (CREF<Slice> name ,CREF<Unknown> holder) override {
		fake.mThis = Singleton<GlobalProc>::instance ().mThis ;
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
		auto rax = Singleton<GlobalProc>::instance ().mThis ;
		assume (!rax->mFinalize) ;
	}

	void shutdown () const override {
		auto rax = Singleton<GlobalProc>::instance ().mThis ;
		if (rax->mFinalize)
			return ;
		rax->mFinalize = TRUE ;
		rax->mGlobalNameSet.clear () ;
		rax->mGlobalList.clear () ;
	}

	BOOL exist () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		INDEX ix = fake.mIndex ;
		auto &&rax = keep[TYPE<AutoRef<Pointer>>::expr] (fake.mThis->mGlobalList[ix].mValue.self) ;
		return fake.mClazz == rax.clazz () ;
	}

	AutoRef<Pointer> fetch () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		INDEX ix = fake.mIndex ;
		auto &&rax = keep[TYPE<AutoRef<Pointer>>::expr] (fake.mThis->mGlobalList[ix].mValue.self) ;
		assume (rax.exist ()) ;
		const auto r1x = Unknown (fake.mThis->mGlobalList[ix].mHolder) ;
		AutoRef<Pointer> ret = AutoRef<Pointer> (r1x) ;
		const auto r2x = RFat<ReflectClone> (r1x) ;
		r2x->clone (ret.self ,rax.self) ;
		return move (ret) ;
	}

	void store (RREF<AutoRef<Pointer>> item) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		INDEX ix = fake.mIndex ;
		auto &&rax = keep[TYPE<AutoRef<Pointer>>::expr] (fake.mThis->mGlobalList[ix].mValue.self) ;
		assume (!rax.exist ()) ;
		rax = move (item) ;
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