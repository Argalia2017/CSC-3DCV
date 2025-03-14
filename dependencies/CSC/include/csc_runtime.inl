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

struct TimeLayout {
	std::chrono::system_clock::duration mTime ;
} ;

class TimeImplHolder final implement Fat<TimeHolder ,TimeLayout> {
public:
	void initialize () override {
		const auto r1x = std::chrono::system_clock::now () ;
		self.mTime = r1x.time_since_epoch () ;
	}

	void initialize (CREF<LENGTH> milliseconds_) override {
		const auto r1x = std::chrono::milliseconds (milliseconds_) ;
		self.mTime = std::chrono::duration_cast<std::chrono::system_clock::duration> (r1x) ;
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
		self.mTime = r6x.time_since_epoch () ;
	}

	void initialize (CREF<TimeLayout> that) override {
		self.mTime = that.mTime ;
	}

	Ref<TimeLayout> borrow () const leftvalue override {
		return Ref<TimeLayout>::reference (self) ;
	}

	LENGTH megaseconds () const override {
		using R1X = std::chrono::duration<csc_int64_t ,std::ratio<10000000>> ;
		const auto r1x = std::chrono::duration_cast<R1X> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH kiloseconds () const override {
		using R1X = std::chrono::duration<csc_int64_t ,std::ratio<1000>> ;
		const auto r1x = std::chrono::duration_cast<R1X> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH seconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::seconds> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH milliseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::milliseconds> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH microseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::microseconds> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH nanoseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::nanoseconds> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	TimeCalendar calendar () const override {
		TimeCalendar ret ;
		const auto r1x = std::chrono::system_clock::time_point (self.mTime) ;
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

	OfThis<Box<TimeLayout ,TimeStorage>> sadd (CREF<TimeLayout> that) const override {
		OfThis<Box<TimeLayout ,TimeStorage>> ret = TimeHolder::create () ;
		ret.mThis->mTime = self.mTime + that.mTime ;
		return move (ret) ;
	}

	OfThis<Box<TimeLayout ,TimeStorage>> ssub (CREF<TimeLayout> that) const override {
		OfThis<Box<TimeLayout ,TimeStorage>> ret = TimeHolder::create () ;
		ret.mThis->mTime = self.mTime - that.mTime ;
		return move (ret) ;
	}
} ;

exports OfThis<Box<TimeLayout ,TimeStorage>> TimeHolder::create () {
	OfThis<Box<TimeLayout ,TimeStorage>> ret ;
	ret.mThis = Box<TimeLayout>::make () ;
	return move (ret) ;
}

exports VFat<TimeHolder> TimeHolder::hold (VREF<TimeLayout> that) {
	return VFat<TimeHolder> (TimeImplHolder () ,that) ;
}

exports CFat<TimeHolder> TimeHolder::hold (CREF<TimeLayout> that) {
	return CFat<TimeHolder> (TimeImplHolder () ,that) ;
}

template class External<RuntimeProcHolder ,RuntimeProcLayout> ;

exports CREF<OfThis<UniqueRef<RuntimeProcLayout>>> RuntimeProcHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<RuntimeProcLayout>> ret ;
		External<RuntimeProcHolder ,RuntimeProcLayout>::declare ().create (ret.mThis) ;
		RuntimeProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<RuntimeProcHolder> RuntimeProcHolder::hold (VREF<RuntimeProcLayout> that) {
	return VFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::declare () ,that) ;
}

exports CFat<RuntimeProcHolder> RuntimeProcHolder::hold (CREF<RuntimeProcLayout> that) {
	return CFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::declare () ,that) ;
}

struct AtomicLayout {
	Pin<std::atomic<VAL>> mAtomic ;
} ;

class AtomicImplHolder final implement Fat<AtomicHolder ,AtomicLayout> {
public:
	void initialize () override {
		store (ZERO) ;
	}

	static VREF<std::atomic<VAL>> ptr (CREF<AtomicLayout> that) {
		return that.mAtomic.deref ;
	}

	VAL fetch () const override {
		return ptr (self).load (std::memory_order_relaxed) ;
	}

	void store (CREF<VAL> item) const override {
		return ptr (self).store (item ,std::memory_order_relaxed) ;
	}

	VAL exchange (CREF<VAL> item) const override {
		return ptr (self).exchange (item ,std::memory_order_relaxed) ;
	}

	BOOL change (VREF<VAL> expect ,CREF<VAL> item) const override {
		return ptr (self).compare_exchange_weak (expect ,item ,std::memory_order_relaxed) ;
	}

	void replace (CREF<VAL> expect ,CREF<VAL> item) const override {
		auto rax = expect ;
		ptr (self).compare_exchange_strong (rax ,item ,std::memory_order_relaxed) ;
	}

	void increase () const override {
		ptr (self).fetch_add (1 ,std::memory_order_relaxed) ;
	}

	void decrease () const override {
		ptr (self).fetch_sub (1 ,std::memory_order_relaxed) ;
	}
} ;

exports OfThis<Box<AtomicLayout ,AtomicStorage>> AtomicHolder::create () {
	OfThis<Box<AtomicLayout ,AtomicStorage>> ret ;
	ret.mThis = Box<AtomicLayout>::make () ;
	return move (ret) ;
}

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

struct MutexLayout {
	Just<MutexType> mType ;
	Box<std::mutex> mBasic ;
	Atomic mShared ;
	Box<std::condition_variable> mUnique ;
} ;

class MutexImplHolder final implement Fat<MutexHolder ,MutexLayout> {
public:
	void initialize () override {
		self.mType = MutexType::Basic ;
		self.mBasic.remake () ;
	}

	Ref<MutexLayout> borrow () leftvalue override {
		return Ref<MutexLayout>::reference (self) ;
	}

	BOOL done () override {
		if (self.mType == MutexType::OnceDone)
			return TRUE ;
		return FALSE ;
	}

	void enter () override {
		if (done ())
			return ;
		self.mBasic->lock () ;
	}

	void leave () override {
		if (done ())
			return ;
		replace (self.mType ,FLAG (MutexType::Once) ,MutexType::OnceDone) ;
		self.mBasic->unlock () ;
	}
} ;

exports OfThis<SharedRef<MutexLayout>> MutexHolder::create () {
	OfThis<SharedRef<MutexLayout>> ret ;
	ret.mThis = SharedRef<MutexLayout>::make () ;
	return move (ret) ;
}

exports VFat<MutexHolder> MutexHolder::hold (VREF<MutexLayout> that) {
	return VFat<MutexHolder> (MutexImplHolder () ,that) ;
}

exports CFat<MutexHolder> MutexHolder::hold (CREF<MutexLayout> that) {
	return CFat<MutexHolder> (MutexImplHolder () ,that) ;
}

class MakeMutexImplHolder final implement Fat<MakeMutexHolder ,MutexLayout> {
public:
	void make_OnceMutex () override {
		self.mType = MutexType::Once ;
		self.mBasic.remake () ;
	}

	void make_SharedMutex () override {
		self.mType = MutexType::Shared ;
		self.mBasic.remake () ;
		self.mShared = NULL ;
	}

	void make_UniqueMutex () override {
		self.mType = MutexType::Unique ;
		self.mBasic.remake () ;
		self.mUnique.remake () ;
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

struct SharedLockLayout {
	Ref<MutexLayout> mMutex ;
	std::unique_lock<SharedAtomicMutex> mLock ;
} ;

class SharedLockImplHolder final implement Fat<SharedLockHolder ,SharedLockLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		self.mMutex = mutex.borrow () ;
		assert (self.mMutex.exclusive ()) ;
		assert (ptr (self).mType == MutexType::Shared) ;
		shared_enter () ;
		self.mLock = std::unique_lock<SharedAtomicMutex> (SharedAtomicMutex::from (ptr (self).mShared)) ;
	}

	static VREF<MutexLayout> ptr (CREF<SharedLockLayout> that) {
		return Pointer::make (address (that.mMutex.deref)) ;
	}

	void shared_enter () {
		if ifdo (TRUE) {
			auto rax = ptr (self).mShared.fetch () ;
			while (TRUE) {
				rax = MathProc::abs (rax) ;
				const auto r1x = ptr (self).mShared.change (rax ,rax + 1) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
		std::atomic_thread_fence (std::memory_order_acquire) ;
	}

	BOOL busy () const override {
		return ptr (self).mShared.fetch () != IDEN ;
	}

	void enter () const override {
		ptr (self).mShared.decrease () ;
		ptr (self).mBasic->lock () ;
		if ifdo (TRUE) {
			auto rax = ZERO ;
			while (TRUE) {
				rax = ZERO ;
				const auto r1x = ptr (self).mShared.change (rax ,NONE) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
	}

	void leave () const override {
		std::atomic_thread_fence (std::memory_order_release) ;
		ptr (self).mShared.replace (NONE ,IDEN) ;
		ptr (self).mBasic->unlock () ;
	}
} ;

exports OfThis<Box<SharedLockLayout ,SharedLockStorage>> SharedLockHolder::create () {
	OfThis<Box<SharedLockLayout ,SharedLockStorage>> ret ;
	ret.mThis = Box<SharedLockLayout>::make () ;
	return move (ret) ;
}

exports VFat<SharedLockHolder> SharedLockHolder::hold (VREF<SharedLockLayout> that) {
	return VFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

exports CFat<SharedLockHolder> SharedLockHolder::hold (CREF<SharedLockLayout> that) {
	return CFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

struct UniqueLockLayout {
	Ref<MutexLayout> mMutex ;
	std::unique_lock<std::mutex> mLock ;
} ;

class UniqueLockImplHolder final implement Fat<UniqueLockHolder ,UniqueLockLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		self.mMutex = mutex.borrow () ;
		assert (self.mMutex.exclusive ()) ;
		assert (ptr (self).mType == MutexType::Unique) ;
		self.mLock = std::unique_lock<std::mutex> (ptr (self).mBasic.deref) ;
	}

	static VREF<MutexLayout> ptr (CREF<UniqueLockLayout> that) {
		return Pointer::make (address (that.mMutex.deref)) ;
	}

	void wait () override {
		ptr (self).mUnique->wait (self.mLock) ;
	}

	void wait (CREF<Time> time) override {
		const auto r1x = time.borrow () ;
		ptr (self).mUnique->wait_for (self.mLock ,r1x->mTime) ;
	}

	void notify () override {
		ptr (self).mUnique->notify_all () ;
	}

	void yield () override {
		self.mLock = std::unique_lock<std::mutex> () ;
		std::this_thread::yield () ;
		self.mLock = std::unique_lock<std::mutex> (ptr (self).mBasic.deref) ;
	}
} ;

exports OfThis<Box<UniqueLockLayout ,UniqueLockStorage>> UniqueLockHolder::create () {
	OfThis<Box<UniqueLockLayout ,UniqueLockStorage>> ret ;
	ret.mThis = Box<UniqueLockLayout>::make () ;
	return move (ret) ;
}

exports VFat<UniqueLockHolder> UniqueLockHolder::hold (VREF<UniqueLockLayout> that) {
	return VFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

exports CFat<UniqueLockHolder> UniqueLockHolder::hold (CREF<UniqueLockLayout> that) {
	return CFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

struct ThreadLayout {
	Box<VFat<ThreadBinder>> mExecutor ;
	FLAG mUid ;
	INDEX mSlot ;
	Box<std::thread> mThread ;

public:
	implicit ThreadLayout () = default ;

	implicit ~ThreadLayout () noexcept {
		assert (mThread == NULL) ;
	}
} ;

class ThreadImplHolder final implement Fat<ThreadHolder ,ThreadLayout> {
public:
	void initialize (RREF<Box<VFat<ThreadBinder>>> executor ,CREF<INDEX> slot) override {
		self.mExecutor = move (executor) ;
		self.mUid = ZERO ;
		self.mSlot = slot ;
	}

	FLAG thread_uid () const override {
		return self.mUid ;
	}

	void start () override {
		auto &&rax = self ;
		self.mThread = Box<std::thread>::make ([&] () {
			rax.mUid = RuntimeProc::thread_uid () ;
			rax.mExecutor.deref->friend_execute (rax.mSlot) ;
		}) ;
	}

	void stop () override {
		if (self.mThread == NULL)
			return ;
		self.mThread->join () ;
		self.mThread = NULL ;
	}
} ;

exports OfThis<AutoRef<ThreadLayout>> ThreadHolder::create () {
	OfThis<AutoRef<ThreadLayout>> ret ;
	ret.mThis = AutoRef<ThreadLayout>::make () ;
	return move (ret) ;
}

exports VFat<ThreadHolder> ThreadHolder::hold (VREF<ThreadLayout> that) {
	return VFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

exports CFat<ThreadHolder> ThreadHolder::hold (CREF<ThreadLayout> that) {
	return CFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

template class External<ProcessHolder ,ProcessLayout> ;

exports OfThis<AutoRef<ProcessLayout>> ProcessHolder::create () {
	OfThis<AutoRef<ProcessLayout>> ret ;
	External<ProcessHolder ,ProcessLayout>::declare ().create (ret.mThis) ;
	return move (ret) ;
}

exports VFat<ProcessHolder> ProcessHolder::hold (VREF<ProcessLayout> that) {
	return VFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::declare () ,that) ;
}

exports CFat<ProcessHolder> ProcessHolder::hold (CREF<ProcessLayout> that) {
	return CFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::declare () ,that) ;
}

template class External<LibraryHolder ,LibraryLayout> ;

exports OfThis<AutoRef<LibraryLayout>> LibraryHolder::create () {
	OfThis<AutoRef<LibraryLayout>> ret ;
	External<LibraryHolder ,LibraryLayout>::declare ().create (ret.mThis) ;
	return move (ret) ;
}

exports VFat<LibraryHolder> LibraryHolder::hold (VREF<LibraryLayout> that) {
	return VFat<LibraryHolder> (External<LibraryHolder ,LibraryLayout>::declare () ,that) ;
}

exports CFat<LibraryHolder> LibraryHolder::hold (CREF<LibraryLayout> that) {
	return CFat<LibraryHolder> (External<LibraryHolder ,LibraryLayout>::declare () ,that) ;
}

struct SystemLayout {
	std::locale mLocale ;
} ;

class SystemImplHolder final implement Fat<SystemHolder ,SystemLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void set_locale (CREF<String<STR>> name) override {
		const auto r1x = StringProc::stra_from_strs (name) ;
		self.mLocale = std::locale (r1x) ;
	}

	void execute (CREF<String<STR>> command) const override {
		const auto r1x = StringProc::stra_from_strs (command) ;
		const auto r2x = FLAG (std::system (r1x)) ;
		noop (r2x) ;
	}
} ;

exports OfThis<AutoRef<SystemLayout>> SystemHolder::create () {
	OfThis<AutoRef<SystemLayout>> ret ;
	ret.mThis = AutoRef<SystemLayout>::make () ;
	return move (ret) ;
}

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

struct RandomLayout {
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
		self.mSeed = seed ;
		self.mRandom.remake () ;
		self.mRandom.deref = std::mt19937_64 (seed) ;
		self.mNormal.mOdd = FALSE ;
	}

	FLAG seed () const override {
		return self.mSeed ;
	}

	QUAD random_byte () {
		return QUAD (self.mRandom.deref ()) ;
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
			if (self.mNormal.mOdd)
				discard ;
			const auto r1x = VAL64 (10000) ;
			const auto r2x = MathProc::min_of (random_float (r1x) + FLT64_EPS ,FLT64 (1)) ;
			const auto r3x = random_float (r1x) ;
			const auto r4x = MathProc::sqrt (FLT64 (-2) * MathProc::log (r2x)) ;
			const auto r5x = MATH_PI * 2 * r3x ;
			self.mNormal.mNX = r4x * MathProc::cos (r5x) ;
			self.mNormal.mNY = r4x * MathProc::sin (r5x) ;
			self.mNormal.mOdd = TRUE ;
			return self.mNormal.mNX ;
		}
		self.mNormal.mOdd = FALSE ;
		return self.mNormal.mNY ;
	}
} ;

exports OfThis<SharedRef<RandomLayout>> RandomHolder::create () {
	OfThis<SharedRef<RandomLayout>> ret ;
	ret.mThis = SharedRef<RandomLayout>::make () ;
	return move (ret) ;
}

exports VFat<RandomHolder> RandomHolder::hold (VREF<RandomLayout> that) {
	return VFat<RandomHolder> (RandomImplHolder () ,that) ;
}

exports CFat<RandomHolder> RandomHolder::hold (CREF<RandomLayout> that) {
	return CFat<RandomHolder> (RandomImplHolder () ,that) ;
}

template class External<SingletonProcHolder ,SingletonProcLayout> ;

exports CREF<OfThis<UniqueRef<SingletonProcLayout>>> SingletonProcHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<SingletonProcLayout>> ret ;
		External<SingletonProcHolder ,SingletonProcLayout>::declare ().create (ret.mThis) ;
		SingletonProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<SingletonProcHolder> SingletonProcHolder::hold (VREF<SingletonProcLayout> that) {
	return VFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::declare () ,that) ;
}

exports CFat<SingletonProcHolder> SingletonProcHolder::hold (CREF<SingletonProcLayout> that) {
	return CFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::declare () ,that) ;
}

struct GlobalNode {
	FLAG mHolder ;
	Pin<AutoRef<Pointer>> mValue ;
} ;

struct GlobalTree {
	Mutex mMutex ;
	Set<Slice> mGlobalNameSet ;
	List<GlobalNode> mGlobalList ;
	BOOL mFinalize ;
} ;

class GlobalImplHolder final implement Fat<GlobalHolder ,GlobalLayout> {
public:
	void initialize () override {
		self.mThis = Ref<GlobalTree>::make () ;
		self.mThis->mMutex = NULL ;
		self.mThis->mFinalize = FALSE ;
		self.mIndex = NONE ;
	}

	void initialize (CREF<Slice> name ,CREF<Unknown> holder) override {
		self.mThis = Singleton<GlobalProc>::instance ().mThis ;
		assert (!self.mThis->mFinalize) ;
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		INDEX ix = self.mThis->mGlobalNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = self.mThis->mGlobalList.insert () ;
			self.mThis->mGlobalNameSet.add (name ,ix) ;
			self.mThis->mGlobalList[ix].mHolder = inline_vptr (holder) ;
		}
		self.mIndex = ix ;
		ClazzHolder::hold (self.mClazz)->initialize (holder) ;
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
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		INDEX ix = self.mIndex ;
		auto &&rax = keep[TYPE<AutoRef<Pointer>>::expr] (self.mThis->mGlobalList[ix].mValue.deref) ;
		return self.mClazz == rax.clazz () ;
	}

	AutoRef<Pointer> fetch () const override {
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		INDEX ix = self.mIndex ;
		auto &&rax = keep[TYPE<AutoRef<Pointer>>::expr] (self.mThis->mGlobalList[ix].mValue.deref) ;
		assume (rax.exist ()) ;
		const auto r1x = Unknown (self.mThis->mGlobalList[ix].mHolder) ;
		AutoRef<Pointer> ret = AutoRef<Pointer> (r1x) ;
		const auto r2x = RFat<ReflectClone> (r1x) ;
		r2x->clone (ret.deref ,rax.deref) ;
		return move (ret) ;
	}

	void store (RREF<AutoRef<Pointer>> item) const override {
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		INDEX ix = self.mIndex ;
		auto &&rax = keep[TYPE<AutoRef<Pointer>>::expr] (self.mThis->mGlobalList[ix].mValue.deref) ;
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