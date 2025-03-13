﻿#pragma once

#ifndef __CSC_RUNTIME__
#define __CSC_RUNTIME__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
struct TimeCalendar {
	LENGTH mYear ;
	LENGTH mMonth ;
	LENGTH mDay ;
	LENGTH mWDay ;
	LENGTH mYDay ;
	LENGTH mHour ;
	LENGTH mMinute ;
	LENGTH mSecond ;
} ;

struct TimeImplLayout ;
using TimeImplStorage = Storage<ENUM<8> ,ENUM<8>> ;
struct TimeLayout implement OfThis<Box<TimeImplLayout ,TimeImplStorage>> {} ;

struct TimeHolder implement Interface {
	imports TimeLayout create () ;
	imports VFat<TimeHolder> hold (VREF<TimeImplLayout> that) ;
	imports CFat<TimeHolder> hold (CREF<TimeImplLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CREF<LENGTH> milliseconds_) = 0 ;
	virtual void initialize (CREF<TimeCalendar> calendar_) = 0 ;
	virtual void initialize (CREF<TimeImplLayout> that) = 0 ;
	virtual Ref<TimeImplLayout> borrow () const leftvalue = 0 ;
	virtual LENGTH megaseconds () const = 0 ;
	virtual LENGTH kiloseconds () const = 0 ;
	virtual LENGTH seconds () const = 0 ;
	virtual LENGTH milliseconds () const = 0 ;
	virtual LENGTH microseconds () const = 0 ;
	virtual LENGTH nanoseconds () const = 0 ;
	virtual TimeCalendar calendar () const = 0 ;
	virtual TimeLayout sadd (CREF<TimeImplLayout> that) const = 0 ;
	virtual TimeLayout ssub (CREF<TimeImplLayout> that) const = 0 ;
} ;

class Time implement TimeLayout {
public:
	implicit Time () = default ;

	explicit Time (CREF<LENGTH> milliseconds_) {
		mThis = TimeHolder::create () ;
		TimeHolder::hold (thiz)->initialize (milliseconds_) ;
	}

	explicit Time (CREF<TimeCalendar> calendar_) {
		mThis = TimeHolder::create () ;
		TimeHolder::hold (thiz)->initialize (calendar_) ;
	}

	implicit Time (CREF<Time> that) {
		if (!that.mThis.exist ())
			return ;
		mThis = TimeHolder::create () ;
		TimeHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<Time> operator= (CREF<Time> that) {
		return assign (thiz ,that) ;
	}

	implicit Time (RREF<Time> that) = default ;

	forceinline VREF<Time> operator= (RREF<Time> that) = default ;

	Ref<TimeImplLayout> borrow () const leftvalue {
		return TimeHolder::hold (thiz)->borrow () ;
	}

	LENGTH megaseconds () const {
		return TimeHolder::hold (thiz)->megaseconds () ;
	}

	LENGTH kiloseconds () const {
		return TimeHolder::hold (thiz)->kiloseconds () ;
	}

	LENGTH seconds () const {
		return TimeHolder::hold (thiz)->seconds () ;
	}

	LENGTH milliseconds () const {
		return TimeHolder::hold (thiz)->milliseconds () ;
	}

	LENGTH microseconds () const {
		return TimeHolder::hold (thiz)->microseconds () ;
	}

	LENGTH nanoseconds () const {
		return TimeHolder::hold (thiz)->nanoseconds () ;
	}

	TimeCalendar calendar () const {
		return TimeHolder::hold (thiz)->calendar () ;
	}

	Time sadd (CREF<Time> that) const {
		TimeLayout ret = TimeHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Time>::expr] (ret)) ;
	}

	Time operator+ (CREF<Time> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CREF<Time> that) {
		thiz = sadd (that) ;
	}

	Time ssub (CREF<Time> that) const {
		TimeLayout ret = TimeHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Time>::expr] (ret)) ;
	}

	Time operator- (CREF<Time> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CREF<Time> that) {
		thiz = ssub (that) ;
	}
} ;

inline Time CurrentTime () {
	Time ret ;
	ret.mThis = TimeHolder::create () ;
	TimeHolder::hold (ret)->initialize () ;
	return move (ret) ;
}

struct RuntimeProcImplLayout ;
struct RuntimeProcLayout implement OfThis<UniqueRef<RuntimeProcImplLayout>> {} ;

struct RuntimeProcHolder implement Interface {
	imports CREF<RuntimeProcLayout> instance () ;
	imports VFat<RuntimeProcHolder> hold (VREF<RuntimeProcImplLayout> that) ;
	imports CFat<RuntimeProcHolder> hold (CREF<RuntimeProcImplLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH thread_concurrency () const = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void thread_sleep (CREF<Time> time) const = 0 ;
	virtual void thread_yield () const = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual void process_exit () const = 0 ;
	virtual String<STR> library_file () const = 0 ;
	virtual String<STR> library_main () const = 0 ;
} ;

class RuntimeProc implement RuntimeProcLayout {
public:
	static CREF<RuntimeProc> instance () {
		return keep[TYPE<RuntimeProc>::expr] (RuntimeProcHolder::instance ()) ;
	}

	static LENGTH thread_concurrency () {
		return RuntimeProcHolder::hold (instance ())->thread_concurrency () ;
	}

	static FLAG thread_uid () {
		return RuntimeProcHolder::hold (instance ())->thread_uid () ;
	}

	static void thread_sleep (CREF<Time> time) {
		return RuntimeProcHolder::hold (instance ())->thread_sleep (time) ;
	}

	static void thread_yield () {
		return RuntimeProcHolder::hold (instance ())->thread_yield () ;
	}

	static FLAG process_uid () {
		return RuntimeProcHolder::hold (instance ())->process_uid () ;
	}

	static void process_exit () {
		return RuntimeProcHolder::hold (instance ())->process_exit () ;
	}

	static String<STR> library_file () {
		return RuntimeProcHolder::hold (instance ())->library_file () ;
	}

	static String<STR> library_main () {
		return RuntimeProcHolder::hold (instance ())->library_main () ;
	}
} ;

struct AtomicImplLayout ;
using AtomicImplStorage = Storage<SIZE_OF<VAL> ,ALIGN_OF<VAL>> ;
struct AtomicLayout implement OfThis<Box<AtomicImplLayout ,AtomicImplStorage>> {} ;

struct AtomicHolder implement Interface {
	imports AtomicLayout create () ;
	imports VFat<AtomicHolder> hold (VREF<AtomicImplLayout> that) ;
	imports CFat<AtomicHolder> hold (CREF<AtomicImplLayout> that) ;

	virtual void initialize () = 0 ;
	virtual VAL fetch () const = 0 ;
	virtual void store (CREF<VAL> item) const = 0 ;
	virtual VAL exchange (CREF<VAL> item) const = 0 ;
	virtual BOOL change (VREF<VAL> expect ,CREF<VAL> item) const = 0 ;
	virtual void replace (CREF<VAL> expect ,CREF<VAL> item) const = 0 ;
	virtual void increase () const = 0 ;
	virtual void decrease () const = 0 ;
} ;

class Atomic implement AtomicLayout {
public:
	implicit Atomic () = default ;

	implicit Atomic (CREF<typeof (NULL)>) {
		mThis = AtomicHolder::create () ;
		AtomicHolder::hold (thiz)->initialize () ;
	}

	VAL fetch () const {
		return AtomicHolder::hold (thiz)->fetch () ;
	}

	forceinline operator VAL () const {
		return fetch () ;
	}

	void store (CREF<VAL> item) const {
		return AtomicHolder::hold (thiz)->store (item) ;
	}

	VAL exchange (CREF<VAL> item) const {
		return AtomicHolder::hold (thiz)->exchange (item) ;
	}

	BOOL change (VREF<VAL> expect ,CREF<VAL> item) const {
		return AtomicHolder::hold (thiz)->change (expect ,item) ;
	}

	void replace (CREF<VAL> expect ,CREF<VAL> item) const {
		return AtomicHolder::hold (thiz)->replace (expect ,item) ;
	}

	void increase () const {
		return AtomicHolder::hold (thiz)->increase () ;
	}

	forceinline void operator++ (int) const {
		return increase () ;
	}

	void decrease () const {
		return AtomicHolder::hold (thiz)->decrease () ;
	}

	forceinline void operator-- (int) const {
		return decrease () ;
	}
} ;

struct MutexImplLayout ;
struct MutexLayout implement OfThis<SharedRef<MutexImplLayout>> {} ;

struct MutexHolder implement Interface {
	imports MutexLayout create () ;
	imports VFat<MutexHolder> hold (VREF<MutexImplLayout> that) ;
	imports CFat<MutexHolder> hold (CREF<MutexImplLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Ref<MutexImplLayout> borrow () leftvalue = 0 ;
	virtual BOOL done () = 0 ;
	virtual void enter () = 0 ;
	virtual void leave () = 0 ;
} ;

class Mutex implement MutexLayout {
public:
	implicit Mutex () = default ;

	implicit Mutex (CREF<typeof (NULL)>) {
		mThis = MutexHolder::create () ;
		MutexHolder::hold (thiz)->initialize () ;
	}

	Ref<MutexImplLayout> borrow () const leftvalue {
		return MutexHolder::hold (thiz)->borrow () ;
	}

	BOOL done () const {
		return MutexHolder::hold (thiz)->done () ;
	}

	void enter () const {
		return MutexHolder::hold (thiz)->enter () ;
	}

	void leave () const {
		return MutexHolder::hold (thiz)->leave () ;
	}
} ;

struct MakeMutexHolder implement Interface {
	imports VFat<MakeMutexHolder> hold (VREF<MutexImplLayout> that) ;
	imports CFat<MakeMutexHolder> hold (CREF<MutexImplLayout> that) ;

	virtual void make_OnceMutex () = 0 ;
	virtual void make_SharedMutex () = 0 ;
	virtual void make_UniqueMutex () = 0 ;
} ;

inline Mutex OnceMutex () {
	Mutex ret ;
	ret.mThis = MutexHolder::create () ;
	MakeMutexHolder::hold (ret)->make_OnceMutex () ;
	return move (ret) ;
}

inline Mutex SharedMutex () {
	Mutex ret ;
	ret.mThis = MutexHolder::create () ;
	MakeMutexHolder::hold (ret)->make_SharedMutex () ;
	return move (ret) ;
}

inline Mutex UniqueMutex () {
	Mutex ret ;
	ret.mThis = MutexHolder::create () ;
	MakeMutexHolder::hold (ret)->make_UniqueMutex () ;
	return move (ret) ;
}

struct SharedLockImplLayout ;
using SharedLockImplStorage = Storage<ENUM_MUL<SIZE_OF<VAL> ,RANK4> ,ALIGN_OF<VAL>> ;
struct SharedLockLayout implement OfThis<Box<SharedLockImplLayout ,SharedLockImplStorage>> {} ;

struct SharedLockHolder implement Interface {
	imports SharedLockLayout create () ;
	imports VFat<SharedLockHolder> hold (VREF<SharedLockImplLayout> that) ;
	imports CFat<SharedLockHolder> hold (CREF<SharedLockImplLayout> that) ;

	virtual void initialize (CREF<Mutex> mutex) = 0 ;
	virtual BOOL busy () const = 0 ;
	virtual void enter () const = 0 ;
	virtual void leave () const = 0 ;
} ;

class SharedLock implement SharedLockLayout {
public:
	implicit SharedLock () = default ;

	explicit SharedLock (CREF<Mutex> mutex) {
		mThis = SharedLockHolder::create () ;
		SharedLockHolder::hold (thiz)->initialize (mutex) ;
	}

	BOOL busy () const {
		return SharedLockHolder::hold (thiz)->busy () ;
	}

	void enter () const {
		return SharedLockHolder::hold (thiz)->enter () ;
	}

	void leave () const {
		return SharedLockHolder::hold (thiz)->leave () ;
	}
} ;

struct UniqueLockImplLayout ;
using UniqueLockImplStorage = Storage<ENUM_MUL<SIZE_OF<VAL> ,RANK4> ,ALIGN_OF<VAL>> ;
struct UniqueLockLayout implement OfThis<Box<UniqueLockImplLayout ,UniqueLockImplStorage>> {} ;

struct UniqueLockHolder implement Interface {
	imports UniqueLockLayout create () ;
	imports VFat<UniqueLockHolder> hold (VREF<UniqueLockImplLayout> that) ;
	imports CFat<UniqueLockHolder> hold (CREF<UniqueLockImplLayout> that) ;

	virtual void initialize (CREF<Mutex> mutex) = 0 ;
	virtual void wait () = 0 ;
	virtual void wait (CREF<Time> time) = 0 ;
	virtual void notify () = 0 ;
	virtual void yield () = 0 ;
} ;

class UniqueLock implement UniqueLockLayout {
public:
	implicit UniqueLock () = default ;

	explicit UniqueLock (CREF<Mutex> mutex) {
		mThis = UniqueLockHolder::create () ;
		UniqueLockHolder::hold (thiz)->initialize (mutex) ;
	}

	void wait () {
		return UniqueLockHolder::hold (thiz)->wait () ;
	}

	void wait (CREF<Time> time) {
		return UniqueLockHolder::hold (thiz)->wait (time) ;
	}

	void notify () {
		return UniqueLockHolder::hold (thiz)->notify () ;
	}

	void yield () {
		return UniqueLockHolder::hold (thiz)->yield () ;
	}
} ;

struct ThreadBinder implement Interface {
	virtual void friend_execute (CREF<INDEX> slot) = 0 ;
} ;

template <class A>
class FriendThreadBinder final implement Fat<ThreadBinder ,A> {
public:
	static VFat<ThreadBinder> hold (VREF<A> that) {
		return VFat<ThreadBinder> (FriendThreadBinder () ,that) ;
	}

	void friend_execute (CREF<INDEX> slot) override {
		thiz.fake.friend_execute (slot) ;
	}
} ;

struct ThreadImplLayout ;
struct ThreadLayout implement OfThis<AutoRef<ThreadImplLayout>> {} ;

struct ThreadHolder implement Interface {
	imports ThreadLayout create () ;
	imports VFat<ThreadHolder> hold (VREF<ThreadImplLayout> that) ;
	imports CFat<ThreadHolder> hold (CREF<ThreadImplLayout> that) ;

	virtual void initialize (RREF<Box<VFat<ThreadBinder>>> executor ,CREF<INDEX> slot) = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void start () = 0 ;
	virtual void stop () = 0 ;
} ;

class Thread implement ThreadLayout {
public:
	implicit Thread () = default ;

	explicit Thread (RREF<Box<VFat<ThreadBinder>>> executor ,CREF<INDEX> slot) {
		mThis = ThreadHolder::create () ;
		ThreadHolder::hold (thiz)->initialize (move (executor) ,slot) ;
	}

	FLAG thread_uid () const {
		return ThreadHolder::hold (thiz)->thread_uid () ;
	}

	void start () {
		return ThreadHolder::hold (thiz)->start () ;
	}

	void stop () {
		return ThreadHolder::hold (thiz)->stop () ;
	}
} ;

struct ProcessImplLayout ;
struct ProcessLayout implement OfThis<AutoRef<ProcessImplLayout>> {} ;

struct ProcessHolder implement Interface {
	imports ProcessLayout create () ;
	imports VFat<ProcessHolder> hold (VREF<ProcessImplLayout> that) ;
	imports CFat<ProcessHolder> hold (CREF<ProcessImplLayout> that) ;

	virtual void initialize (CREF<FLAG> uid) = 0 ;
	virtual void initialize (CREF<RefBuffer<BYTE>> snapshot_) = 0 ;
	virtual BOOL equal (CREF<ProcessImplLayout> that) const = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual RefBuffer<BYTE> snapshot () const = 0 ;
} ;

class Process implement ProcessLayout {
public:
	implicit Process () = default ;

	explicit Process (CREF<FLAG> uid) {
		mThis = ProcessHolder::create () ;
		ProcessHolder::hold (thiz)->initialize (uid) ;
	}

	explicit Process (CREF<RefBuffer<BYTE>> snapshot_) {
		mThis = ProcessHolder::create () ;
		ProcessHolder::hold (thiz)->initialize (snapshot_) ;
	}

	BOOL equal (CREF<Process> that) const {
		return ProcessHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Process> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Process> that) const {
		return (!equal (that)) ;
	}

	FLAG process_uid () const {
		return ProcessHolder::hold (thiz)->process_uid () ;
	}

	RefBuffer<BYTE> snapshot () const {
		return ProcessHolder::hold (thiz)->snapshot () ;
	}
} ;

struct LibraryImplLayout ;
struct LibraryLayout implement OfThis<AutoRef<LibraryImplLayout>> {} ;

struct LibraryHolder implement Interface {
	imports LibraryLayout create () ;
	imports VFat<LibraryHolder> hold (VREF<LibraryImplLayout> that) ;
	imports CFat<LibraryHolder> hold (CREF<LibraryImplLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual String<STR> library_file () const = 0 ;
	virtual FLAG load (CREF<String<STR>> name) = 0 ;
	virtual String<STR> error () const = 0 ;
} ;

class Library implement LibraryLayout {
public:
	implicit Library () = default ;

	explicit Library (CREF<String<STR>> file) {
		mThis = LibraryHolder::create () ;
		LibraryHolder::hold (thiz)->initialize (file) ;
	}

	String<STR> library_file () const {
		return LibraryHolder::hold (thiz)->library_file () ;
	}

	FLAG load (CREF<String<STR>> name) {
		return LibraryHolder::hold (thiz)->load (name) ;
	}

	String<STR> error () const {
		return LibraryHolder::hold (thiz)->error () ;
	}
} ;

struct SystemImplLayout ;
struct SystemLayout implement OfThis<AutoRef<SystemImplLayout>> {} ;

struct SystemHolder implement Interface {
	imports SystemLayout create () ;
	imports VFat<SystemHolder> hold (VREF<SystemImplLayout> that) ;
	imports CFat<SystemHolder> hold (CREF<SystemImplLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_locale (CREF<String<STR>> name) = 0 ;
	virtual void execute (CREF<String<STR>> command) const = 0 ;
} ;

class System implement SystemLayout {
public:
	implicit System () = default ;

	implicit System (CREF<typeof (NULL)>) {
		mThis = SystemHolder::create () ;
		SystemHolder::hold (thiz)->initialize () ;
	}

	void set_locale (CREF<String<STR>> name) {
		return SystemHolder::hold (thiz)->set_locale (name) ;
	}

	void execute (CREF<String<STR>> command) const {
		return SystemHolder::hold (thiz)->execute (command) ;
	}
} ;

struct RandomImplLayout ;
struct RandomLayout implement OfThis<SharedRef<RandomImplLayout>> {} ;

struct RandomHolder implement Interface {
	imports RandomLayout create () ;
	imports VFat<RandomHolder> hold (VREF<RandomImplLayout> that) ;
	imports CFat<RandomHolder> hold (CREF<RandomImplLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CREF<FLAG> seed) = 0 ;
	virtual FLAG seed () const = 0 ;
	virtual VAL32 random_value (CREF<VAL32> min_ ,CREF<VAL32> max_) = 0 ;
	virtual VAL64 random_value (CREF<VAL64> min_ ,CREF<VAL64> max_) = 0 ;
	virtual FLT64 random_float (CREF<FLT64> scale) = 0 ;
	virtual Array<INDEX> random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_) = 0 ;
	virtual void random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> result) = 0 ;
	virtual BitSet random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_) = 0 ;
	virtual void random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<BitSet> result) = 0 ;
	virtual BOOL random_draw (CREF<FLT64> possibility) = 0 ;
	virtual FLT64 random_normal () = 0 ;
} ;

class Random implement RandomLayout {
public:
	implicit Random () = default ;

	explicit Random (CREF<FLAG> seed) {
		mThis = RandomHolder::create () ;
		RandomHolder::hold (thiz)->initialize (seed) ;
	}

	FLAG seed () const {
		return RandomHolder::hold (thiz)->seed () ;
	}

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	ARG1 random_value (CREF<ARG1> min_ ,CREF<ARG1> max_) const {
		return RandomHolder::hold (thiz)->random_value (min_ ,max_) ;
	}

	FLT64 random_float (CREF<FLT64> scale) const {
		return RandomHolder::hold (thiz)->random_float (scale) ;
	}

	Array<INDEX> random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_) const {
		return RandomHolder::hold (thiz)->random_shuffle (length_ ,size_) ;
	}

	void random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> result) const {
		return RandomHolder::hold (thiz)->random_shuffle (length_ ,size_ ,result) ;
	}

	BitSet random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_) const {
		return RandomHolder::hold (thiz)->random_pick (length_ ,size_) ;
	}

	void random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<BitSet> result) const {
		return RandomHolder::hold (thiz)->random_pick (length_ ,size_ ,result) ;
	}

	BOOL random_draw (CREF<FLT64> possibility) const {
		return RandomHolder::hold (thiz)->random_draw (possibility) ;
	}

	FLT64 random_normal () const {
		return RandomHolder::hold (thiz)->random_normal () ;
	}
} ;

inline Random CurrentRandom () {
	Random ret ;
	ret.mThis = RandomHolder::create () ;
	RandomHolder::hold (ret)->initialize () ;
	return move (ret) ;
}

struct SingletonProcImplLayout ;
struct SingletonProcLayout implement OfThis<UniqueRef<SingletonProcImplLayout>> {} ;

struct SingletonProcHolder implement Interface {
	imports CREF<SingletonProcLayout> instance () ;
	imports VFat<SingletonProcHolder> hold (VREF<SingletonProcImplLayout> that) ;
	imports CFat<SingletonProcHolder> hold (CREF<SingletonProcImplLayout> that) ;

	virtual void initialize () = 0 ;
	virtual QUAD abi_reserve () const = 0 ;
	virtual QUAD ctx_reserve () const = 0 ;
	virtual FLAG load (CREF<Clazz> clazz) const = 0 ;
	virtual void save (CREF<Clazz> clazz ,CREF<FLAG> layout) const = 0 ;
} ;

class SingletonProc implement SingletonProcLayout {
public:
	static CREF<SingletonProc> instance () {
		return keep[TYPE<SingletonProc>::expr] (SingletonProcHolder::instance ()) ;
	}

	static QUAD abi_reserve () {
		return SingletonProcHolder::hold (instance ())->abi_reserve () ;
	}

	static QUAD ctx_reserve () {
		return SingletonProcHolder::hold (instance ())->ctx_reserve () ;
	}

	static FLAG load (CREF<Clazz> clazz) {
		return SingletonProcHolder::hold (instance ())->load (clazz) ;
	}

	static void save (CREF<Clazz> clazz ,CREF<FLAG> layout) {
		return SingletonProcHolder::hold (instance ())->save (clazz ,layout) ;
	}
} ;

template <class A>
class Singleton implement Proxy {
public:
	static CREF<A> instance () ;
} ;

template <class A>
inline CREF<A> Singleton<A>::instance () {
	return memorize ([&] () {
		const auto r1x = Clazz (TYPE<A>::expr) ;
		auto rax = SingletonProc::load (r1x) ;
		if ifdo (TRUE) {
			if (rax != ZERO)
				discard ;
			rax = address (A::instance ()) ;
			SingletonProc::save (r1x ,rax) ;
			rax = SingletonProc::load (r1x) ;
		}
		auto &&rbx = keep[TYPE<A>::expr] (Pointer::make (rax)) ;
		return Ref<A>::reference (rbx) ;
	}).self ;
}

struct GlobalImplLayout ;

struct GlobalLayout {
	Ref<GlobalImplLayout> mThis ;
	INDEX mIndex ;
	Clazz mClazz ;
} ;

struct GlobalHolder implement Interface {
	imports CREF<GlobalLayout> instance () ;
	imports VFat<GlobalHolder> hold (VREF<GlobalLayout> that) ;
	imports CFat<GlobalHolder> hold (CREF<GlobalLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CREF<Slice> name ,CREF<Unknown> holder) = 0 ;
	virtual void startup () const = 0 ;
	virtual void shutdown () const = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual AutoRef<Pointer> fetch () const = 0 ;
	virtual void store (RREF<AutoRef<Pointer>> item) const = 0 ;
} ;

class GlobalProc implement GlobalLayout {
public:
	static CREF<GlobalProc> instance () {
		return keep[TYPE<GlobalProc>::expr] (GlobalHolder::instance ()) ;
	}

	static void startup () {
		return GlobalHolder::hold (instance ())->startup () ;
	}

	static void shutdown () {
		return GlobalHolder::hold (instance ())->shutdown () ;
	}
} ;

template <class A>
class GlobalUnknownBinder implement ReflectUnknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_vptr (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectGuidBinder<A>::expr)
			return inline_vptr (ReflectGuidBinder<A> ()) ;
		if (uuid == ReflectNameBinder<A>::expr)
			return inline_vptr (ReflectNameBinder<A> ()) ;
		if (uuid == ReflectCloneBinder<A>::expr)
			return inline_vptr (ReflectCloneBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Global implement GlobalLayout {
protected:
	using GlobalLayout::mThis ;
	using GlobalLayout::mIndex ;
	using GlobalLayout::mClazz ;

public:
	implicit Global () = default ;

	explicit Global (CREF<Slice> name) {
		GlobalHolder::hold (thiz)->initialize (name ,GlobalUnknownBinder<A> ()) ;
	}

	BOOL exist () const {
		return GlobalHolder::hold (thiz)->exist () ;
	}

	A fetch () const {
		auto rax = GlobalHolder::hold (thiz)->fetch () ;
		return move (rax.rebind (TYPE<A>::expr).self) ;
	}

	forceinline operator A () const {
		return fetch () ;
	}

	void store (CREF<A> item) const {
		store (move (item)) ;
	}

	void store (RREF<A> item) const {
		auto rax = AutoRef<A>::make (move (item)) ;
		return GlobalHolder::hold (thiz)->store (move (rax)) ;
	}
} ;
} ;