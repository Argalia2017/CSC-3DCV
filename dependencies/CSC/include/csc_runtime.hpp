#pragma once

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

struct TimeLayout implement ThisLayout<Box<TimeImplLayout ,TimeImplStorage>> {} ;

struct TimeHolder implement Interface {
	imports VFat<TimeHolder> hold (VREF<TimeLayout> that) ;
	imports CFat<TimeHolder> hold (CREF<TimeLayout> that) ;

	virtual void initialize (CREF<LENGTH> milliseconds_) = 0 ;
	virtual void initialize (CREF<TimeCalendar> calendar_) = 0 ;
	virtual void initialize (CREF<TimeLayout> that) = 0 ;
	virtual Ref<TimeImplLayout> borrow () const = 0 ;
	virtual LENGTH megaseconds () const = 0 ;
	virtual LENGTH kiloseconds () const = 0 ;
	virtual LENGTH seconds () const = 0 ;
	virtual LENGTH milliseconds () const = 0 ;
	virtual LENGTH microseconds () const = 0 ;
	virtual LENGTH nanoseconds () const = 0 ;
	virtual TimeCalendar calendar () const = 0 ;
	virtual TimeLayout sadd (CREF<TimeLayout> that) const = 0 ;
	virtual TimeLayout ssub (CREF<TimeLayout> that) const = 0 ;
} ;

class Time implement TimeLayout {
protected:
	using TimeLayout::mThis ;

public:
	implicit Time () = default ;

	explicit Time (CREF<LENGTH> milliseconds_) {
		TimeHolder::hold (thiz)->initialize (milliseconds_) ;
	}

	explicit Time (CREF<TimeCalendar> calendar_) {
		TimeHolder::hold (thiz)->initialize (calendar_) ;
	}

	implicit Time (CREF<Time> that) {
		TimeHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<Time> operator= (CREF<Time> that) {
		return assign (thiz ,that) ;
	}

	implicit Time (RREF<Time> that) = default ;

	forceinline VREF<Time> operator= (RREF<Time> that) = default ;

	Ref<TimeImplLayout> borrow () const {
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

struct MakeTimeHolder implement Interface {
	imports VFat<MakeTimeHolder> hold (VREF<TimeLayout> that) ;
	imports CFat<MakeTimeHolder> hold (CREF<TimeLayout> that) ;

	virtual void CurrentTime_initialize () = 0 ;
} ;

inline Time CurrentTime () {
	Time ret ;
	MakeTimeHolder::hold (ret)->CurrentTime_initialize () ;
	return move (ret) ;
}

struct RuntimeProcLayout implement ThisLayout<AutoRefLayout> {} ;

struct RuntimeProcHolder implement Interface {
	imports VFat<RuntimeProcHolder> hold (VREF<RuntimeProcLayout> that) ;
	imports CFat<RuntimeProcHolder> hold (CREF<RuntimeProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH thread_concurrency () const = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void thread_sleep (CREF<Time> time) const = 0 ;
	virtual void thread_yield () const = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual void process_exit () const = 0 ;
	virtual FLAG random_seed () const = 0 ;
	virtual String<STR> working_path () const = 0 ;
	virtual String<STR> module_path () const = 0 ;
	virtual String<STR> module_name () const = 0 ;
} ;

class RuntimeProc implement RuntimeProcLayout {
protected:
	using RuntimeProcLayout::mThis ;

public:
	imports CREF<RuntimeProc> instance () {
		return memorize ([&] () {
			RuntimeProc ret ;
			RuntimeProcHolder::hold (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports LENGTH thread_concurrency () {
		return RuntimeProcHolder::hold (instance ())->thread_concurrency () ;
	}

	imports FLAG thread_uid () {
		return RuntimeProcHolder::hold (instance ())->thread_uid () ;
	}

	imports void thread_sleep (CREF<Time> time) {
		return RuntimeProcHolder::hold (instance ())->thread_sleep (time) ;
	}

	imports void thread_yield () {
		return RuntimeProcHolder::hold (instance ())->thread_yield () ;
	}

	imports FLAG process_uid () {
		return RuntimeProcHolder::hold (instance ())->process_uid () ;
	}

	imports void process_exit () {
		return RuntimeProcHolder::hold (instance ())->process_exit () ;
	}

	imports FLAG random_seed () {
		return RuntimeProcHolder::hold (instance ())->random_seed () ;
	}

	imports String<STR> working_path () {
		return RuntimeProcHolder::hold (instance ())->working_path () ;
	}

	imports String<STR> module_path () {
		return RuntimeProcHolder::hold (instance ())->module_path () ;
	}

	imports String<STR> module_name () {
		return RuntimeProcHolder::hold (instance ())->module_name () ;
	}
} ;

struct AtomicImplLayout ;
using AtomicImplStorage = Storage<SIZE_OF<VAL> ,ALIGN_OF<VAL>> ;

struct AtomicLayout implement ThisLayout<Box<AtomicImplLayout ,AtomicImplStorage>> {} ;

struct AtomicHolder implement Interface {
	imports VFat<AtomicHolder> hold (VREF<AtomicLayout> that) ;
	imports CFat<AtomicHolder> hold (CREF<AtomicLayout> that) ;

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
protected:
	using AtomicLayout::mThis ;

public:
	implicit Atomic () = default ;

	implicit Atomic (CREF<typeof (NULL)>) {
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

	void operator++ (int) const {
		return increase () ;
	}

	void decrease () const {
		return AtomicHolder::hold (thiz)->decrease () ;
	}

	void operator-- (int) const {
		return decrease () ;
	}
} ;

struct MutexImplLayout ;

struct MutexLayout implement ThisLayout<AutoRef<MutexImplLayout>> {} ;

struct MutexHolder implement Interface {
	imports VFat<MutexHolder> hold (VREF<MutexLayout> that) ;
	imports CFat<MutexHolder> hold (CREF<MutexLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Ref<MutexImplLayout> borrow () const = 0 ;
	virtual void enter () const = 0 ;
	virtual void leave () const = 0 ;
} ;

class Mutex implement MutexLayout {
protected:
	using MutexLayout::mThis ;

public:
	implicit Mutex () = default ;

	implicit Mutex (CREF<typeof (NULL)>) {
		MutexHolder::hold (thiz)->initialize () ;
	}

	Ref<MutexImplLayout> borrow () const {
		return MutexHolder::hold (thiz)->borrow () ;
	}

	void enter () const {
		return MutexHolder::hold (thiz)->enter () ;
	}

	void leave () const {
		return MutexHolder::hold (thiz)->leave () ;
	}
} ;

struct MakeMutexHolder implement Interface {
	imports VFat<MakeMutexHolder> hold (VREF<MutexLayout> that) ;
	imports CFat<MakeMutexHolder> hold (CREF<MutexLayout> that) ;

	virtual void RecursiveMutex_initialize () = 0 ;
	virtual void SharedMutex_initialize () = 0 ;
	virtual void UniqueMutex_initialize () = 0 ;
} ;

inline Mutex RecursiveMutex () {
	Mutex ret ;
	MakeMutexHolder::hold (ret)->RecursiveMutex_initialize () ;
	return move (ret) ;
}

inline Mutex SharedMutex () {
	Mutex ret ;
	MakeMutexHolder::hold (ret)->SharedMutex_initialize () ;
	return move (ret) ;
}

inline Mutex UniqueMutex () {
	Mutex ret ;
	MakeMutexHolder::hold (ret)->UniqueMutex_initialize () ;
	return move (ret) ;
}

struct SharedLockImplLayout ;
using SharedLockImplStorage = Storage<ENUM_MUL<SIZE_OF<VAL> ,RANK4> ,ALIGN_OF<VAL>> ;

struct SharedLockLayout implement ThisLayout<Box<SharedLockImplLayout ,SharedLockImplStorage>> {} ;

struct SharedLockHolder implement Interface {
	imports VFat<SharedLockHolder> hold (VREF<SharedLockLayout> that) ;
	imports CFat<SharedLockHolder> hold (CREF<SharedLockLayout> that) ;

	virtual void initialize (CREF<Mutex> mutex) = 0 ;
	virtual BOOL busy () const = 0 ;
	virtual void enter () const = 0 ;
	virtual void leave () const = 0 ;
} ;

class SharedLock implement SharedLockLayout {
protected:
	using SharedLockLayout::mThis ;

public:
	implicit SharedLock () = default ;

	explicit SharedLock (CREF<Mutex> mutex) {
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

struct UniqueLockLayout implement ThisLayout<Box<UniqueLockImplLayout ,UniqueLockImplStorage>> {} ;

struct UniqueLockHolder implement Interface {
	imports VFat<UniqueLockHolder> hold (VREF<UniqueLockLayout> that) ;
	imports CFat<UniqueLockHolder> hold (CREF<UniqueLockLayout> that) ;

	virtual void initialize (CREF<Mutex> mutex) = 0 ;
	virtual void wait () = 0 ;
	virtual void wait (CREF<Time> time) = 0 ;
	virtual void notify () = 0 ;
	virtual void yield () = 0 ;
} ;

class UniqueLock implement UniqueLockLayout {
protected:
	using UniqueLockLayout::mThis ;

public:
	implicit UniqueLock () = default ;

	explicit UniqueLock (CREF<Mutex> mutex) {
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

struct ThreadFriend implement Interface {
	virtual void friend_execute (CREF<INDEX> slot) = 0 ;
} ;

template <class A>
class ThreadFriendBinder implement Fat<ThreadFriend ,A> {
public:
	imports VFat<ThreadFriend> create (VREF<A> that) {
		return VFat<ThreadFriend> (ThreadFriendBinder () ,that) ;
	}

	void friend_execute (CREF<INDEX> slot) override {
		thiz.fake.friend_execute (slot) ;
	}
} ;

struct ThreadImplLayout ;

struct ThreadLayout implement ThisLayout<AutoRef<ThreadImplLayout>> {} ;

struct ThreadHolder implement Interface {
	imports VFat<ThreadHolder> hold (VREF<ThreadLayout> that) ;
	imports CFat<ThreadHolder> hold (CREF<ThreadLayout> that) ;

	virtual void initialize (RREF<Ref<ThreadFriend>> executor ,CREF<INDEX> slot) = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void start () = 0 ;
	virtual void stop () = 0 ;
} ;

class Thread implement ThreadLayout {
protected:
	using ThreadLayout::mThis ;

public:
	implicit Thread () = default ;

	explicit Thread (RREF<Ref<ThreadFriend>> executor ,CREF<INDEX> slot) {
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

struct ProcessLayout implement ThisLayout<AutoRef<ProcessImplLayout>> {} ;

struct ProcessHolder implement Interface {
	imports VFat<ProcessHolder> hold (VREF<ProcessLayout> that) ;
	imports CFat<ProcessHolder> hold (CREF<ProcessLayout> that) ;

	virtual void initialize (CREF<FLAG> uid) = 0 ;
	virtual void initialize (CREF<RefBuffer<BYTE>> snapshot_) = 0 ;
	virtual BOOL equal (CREF<ProcessLayout> that) const = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual RefBuffer<BYTE> snapshot () const = 0 ;
} ;

class Process implement ProcessLayout {
protected:
	using ProcessLayout::mThis ;

public:
	implicit Process () = default ;

	explicit Process (CREF<FLAG> uid) {
		ProcessHolder::hold (thiz)->initialize (uid) ;
	}

	explicit Process (CREF<RefBuffer<BYTE>> snapshot_) {
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

struct ModuleImplLayout ;

struct ModuleLayout implement ThisLayout<AutoRef<ModuleImplLayout>> {} ;

struct ModuleHolder implement Interface {
	imports VFat<ModuleHolder> hold (VREF<ModuleLayout> that) ;
	imports CFat<ModuleHolder> hold (CREF<ModuleLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual FLAG load (CREF<String<STR>> name) = 0 ;
	virtual String<STR> error () const = 0 ;
} ;

class Module implement ModuleLayout {
protected:
	using ModuleLayout::mThis ;

public:
	implicit Module () = default ;

	explicit Module (CREF<String<STR>> file) {
		ModuleHolder::hold (thiz)->initialize (file) ;
	}

	FLAG load (CREF<String<STR>> name) {
		return ModuleHolder::hold (thiz)->load (name) ;
	}

	String<STR> error () const {
		return ModuleHolder::hold (thiz)->error () ;
	}
} ;

struct SystemImplLayout ;

struct SystemLayout implement ThisLayout<AutoRef<SystemImplLayout>> {} ;

struct SystemHolder implement Interface {
	imports VFat<SystemHolder> hold (VREF<SystemLayout> that) ;
	imports CFat<SystemHolder> hold (CREF<SystemLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_locale (CREF<String<STR>> name) const = 0 ;
	virtual FLAG execute (CREF<String<STR>> command) const = 0 ;
} ;

class System implement SystemLayout {
protected:
	using SystemLayout::mThis ;

public:
	implicit System () = default ;

	void set_locale (CREF<String<STR>> name) const {
		return SystemHolder::hold (thiz)->set_locale (name) ;
	}

	FLAG execute (CREF<String<STR>> command) const {
		return SystemHolder::hold (thiz)->execute (command) ;
	}
} ;

struct RandomImplLayout ;

struct RandomLayout implement ThisLayout<AutoRef<RandomImplLayout>> {} ;

struct RandomHolder implement Interface {
	imports VFat<RandomHolder> hold (VREF<RandomLayout> that) ;
	imports CFat<RandomHolder> hold (CREF<RandomLayout> that) ;

	virtual void initialize (CREF<FLAG> seed) = 0 ;
	virtual FLAG seed () const = 0 ;
	virtual INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const = 0 ;
	virtual Array<INDEX> random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_) const = 0 ;
	virtual void random_shuffle (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> result) const = 0 ;
	virtual BitSet random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_) const = 0 ;
	virtual void random_pick (CREF<LENGTH> length_ ,CREF<LENGTH> size_ ,VREF<BitSet> result) const = 0 ;
	virtual BOOL random_draw (CREF<FLT64> possibility) const = 0 ;
	virtual FLT64 random_normal () const = 0 ;
} ;

class Random implement RandomLayout {
protected:
	using RandomLayout::mThis ;

public:
	implicit Random () = default ;

	explicit Random (CREF<FLAG> seed) {
		RandomHolder::hold (thiz)->initialize (seed) ;
	}

	FLAG seed () const {
		return RandomHolder::hold (thiz)->seed () ;
	}

	INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const {
		return RandomHolder::hold (thiz)->random_value (lb ,rb) ;
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

struct SingletonProcImplLayout ;

struct SingletonProcLayout implement ThisLayout<AutoRef<SingletonProcImplLayout>> {} ;

struct SingletonProcHolder implement Interface {
	imports VFat<SingletonProcHolder> hold (VREF<SingletonProcLayout> that) ;
	imports CFat<SingletonProcHolder> hold (CREF<SingletonProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual QUAD abi_reserve () const = 0 ;
	virtual QUAD ctx_reserve () const = 0 ;
	virtual FLAG load (CREF<Clazz> clazz) const = 0 ;
	virtual void save (CREF<Clazz> clazz ,CREF<FLAG> addr) const = 0 ;
} ;

class SingletonProc implement SingletonProcLayout {
protected:
	using SingletonProcLayout::mThis ;

public:
	imports CREF<SingletonProc> instance () {
		return memorize ([&] () {
			SingletonProc ret ;
			SingletonProcHolder::hold (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports QUAD abi_reserve () {
		return SingletonProcHolder::hold (instance ())->abi_reserve () ;
	}

	imports QUAD ctx_reserve () {
		return SingletonProcHolder::hold (instance ())->ctx_reserve () ;
	}

	imports FLAG load (CREF<Clazz> clazz) {
		return SingletonProcHolder::hold (instance ())->load (clazz) ;
	}

	imports void save (CREF<Clazz> clazz ,CREF<FLAG> addr) {
		return SingletonProcHolder::hold (instance ())->save (clazz ,addr) ;
	}
} ;

template <class A>
class Singleton implement Proxy {
public:
	imports CREF<A> instance () {
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
} ;

struct GlobalImplLayout ;

struct GlobalLayout {
	SharedRef<GlobalImplLayout> mThis ;
	INDEX mIndex ;
	Clazz mClazz ;
} ;

struct GlobalHolder implement Interface {
	imports VFat<GlobalHolder> hold (VREF<GlobalLayout> that) ;
	imports CFat<GlobalHolder> hold (CREF<GlobalLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CREF<Slice> name ,CREF<Unknown> holder ,CREF<GlobalLayout> root) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual AutoRef<Pointer> fetch () const = 0 ;
	virtual void store (RREF<AutoRef<Pointer>> item) const = 0 ;
} ;

class GlobalRoot implement GlobalLayout {
public:
	imports CREF<GlobalRoot> instance () {
		return memorize ([&] () {
			GlobalRoot ret ;
			GlobalHolder::hold (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}
} ;

template <class A>
class GlobalUnknownBinder implement UnknownFriend {
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

template <class A ,class B = Singleton<GlobalRoot>>
class Global implement GlobalLayout {
protected:
	using GlobalLayout::mThis ;
	using GlobalLayout::mIndex ;

public:
	implicit Global () = default ;

	explicit Global (CREF<Slice> name) {
		GlobalHolder::hold (thiz)->initialize (name ,GlobalUnknownBinder<A> () ,B::instance ()) ;
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

	void store (RREF<A> item) const {
		auto rax = AutoRef<A>::make (move (item)) ;
		return GlobalHolder::hold (thiz)->store (move (rax)) ;
	}
} ;

struct PathImplLayout ;

struct PathLayout implement ThisLayout<Ref<PathImplLayout>> {} ;

struct PathHolder implement Interface {
	imports VFat<PathHolder> hold (VREF<PathLayout> that) ;
	imports CFat<PathHolder> hold (CREF<PathLayout> that) ;

	virtual void initialize (RREF<String<STR>> pathname) = 0 ;
	virtual void initialize (CREF<PathLayout> that) = 0 ;
	virtual String<STR> fetch () const = 0 ;
	virtual PathLayout root () const = 0 ;
	virtual PathLayout child (CREF<Slice> name) const = 0 ;
	virtual PathLayout child (CREF<Format> name) const = 0 ;
	virtual PathLayout child (CREF<String<STR>> name) const = 0 ;
	virtual Array<PathLayout> list () const = 0 ;
	virtual Array<PathLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<PathLayout> that) const = 0 ;
	virtual BOOL is_file () const = 0 ;
	virtual BOOL is_dire () const = 0 ;
	virtual BOOL is_link () const = 0 ;
	virtual Deque<String<STR>> decouple () const = 0 ;
	virtual String<STR> path () const = 0 ;
	virtual String<STR> name () const = 0 ;
	virtual String<STR> stem () const = 0 ;
	virtual String<STR> extension () const = 0 ;
} ;

class Path implement PathLayout {
protected:
	using PathLayout::mThis ;

public:
	implicit Path () = default ;

	explicit Path (CREF<String<STR>> pathname) {
		PathHolder::hold (thiz)->initialize (move (pathname)) ;
	}

	explicit Path (RREF<String<STR>> pathname) {
		PathHolder::hold (thiz)->initialize (move (pathname)) ;
	}

	implicit Path (CREF<Path> that) {
		PathHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<Path> operator= (CREF<Path> that) {
		return assign (thiz ,that) ;
	}

	implicit Path (RREF<Path> that) = default ;

	forceinline VREF<Path> operator= (RREF<Path> that) = default ;

	String<STR> fetch () const {
		return PathHolder::hold (thiz)->fetch () ;
	}

	forceinline operator String<STR> () const {
		return fetch () ;
	}

	PathLayout root () const {
		PathLayout ret = PathHolder::hold (thiz)->root () ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CREF<Slice> name) const {
		PathLayout ret = PathHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CREF<Format> name) const {
		PathLayout ret = PathHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CREF<String<STR>> name) const {
		PathLayout ret = PathHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Array<Path> list () const {
		ArrayLayout ret = PathHolder::hold (thiz)->list () ;
		return move (keep[TYPE<Array<Path>>::expr] (ret)) ;
	}

	Array<Path> list (CREF<LENGTH> size_) const {
		ArrayLayout ret = PathHolder::hold (thiz)->list (size_) ;
		return move (keep[TYPE<Array<Path>>::expr] (ret)) ;
	}

	BOOL equal (CREF<Path> that) const {
		return PathHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Path> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Path> that) const {
		return (!equal (that)) ;
	}

	BOOL is_file () const {
		return PathHolder::hold (thiz)->is_file () ;
	}

	BOOL is_dire () const {
		return PathHolder::hold (thiz)->is_dire () ;
	}

	BOOL is_link () const {
		return PathHolder::hold (thiz)->is_link () ;
	}

	Deque<String<STR>> decouple () const {
		return PathHolder::hold (thiz)->decouple () ;
	}

	String<STR> path () const {
		return PathHolder::hold (thiz)->path () ;
	}

	String<STR> name () const {
		return PathHolder::hold (thiz)->name () ;
	}

	String<STR> stem () const {
		return PathHolder::hold (thiz)->stem () ;
	}

	String<STR> extension () const {
		return PathHolder::hold (thiz)->extension () ;
	}
} ;

struct FileProcImplLayout ;

struct FileProcLayout implement ThisLayout<AutoRef<FileProcImplLayout>> {} ;

struct FileProcHolder implement Interface {
	imports VFat<FileProcHolder> hold (VREF<FileProcLayout> that) ;
	imports CFat<FileProcHolder> hold (CREF<FileProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual RefBuffer<BYTE> load_file (CREF<String<STR>> file) const = 0 ;
	virtual void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> item) const = 0 ;
	virtual Ref<RefBuffer<BYTE>> load_asset (CREF<String<STR>> file) const = 0 ;
	virtual void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const = 0 ;
	virtual void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const = 0 ;
	virtual void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const = 0 ;
	virtual void erase_file (CREF<String<STR>> file) const = 0 ;
	virtual void build_dire (CREF<String<STR>> dire) const = 0 ;
	virtual void clear_dire (CREF<String<STR>> dire) const = 0 ;
	virtual void erase_dire (CREF<String<STR>> dire) const = 0 ;
	virtual BOOL lock_dire (CREF<String<STR>> dire) const = 0 ;
} ;

class FileProc implement FileProcLayout {
protected:
	using FileProcLayout::mThis ;

public:
	imports CREF<FileProc> instance () {
		return memorize ([&] () {
			FileProc ret ;
			FileProcHolder::hold (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports RefBuffer<BYTE> load_file (CREF<String<STR>> file) {
		return FileProcHolder::hold (instance ())->load_file (file) ;
	}

	imports void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> item) {
		return FileProcHolder::hold (instance ())->save_file (file ,item) ;
	}

	imports Ref<RefBuffer<BYTE>> load_asset (CREF<String<STR>> file) {
		return FileProcHolder::hold (instance ())->load_asset (file) ;
	}

	imports void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) {
		return FileProcHolder::hold (instance ())->copy_file (dst ,src) ;
	}

	imports void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) {
		return FileProcHolder::hold (instance ())->move_file (dst ,src) ;
	}

	imports void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) {
		return FileProcHolder::hold (instance ())->link_file (dst ,src) ;
	}

	imports void erase_file (CREF<String<STR>> file) {
		return FileProcHolder::hold (instance ())->erase_file (file) ;
	}

	imports void build_dire (CREF<String<STR>> dire) {
		return FileProcHolder::hold (instance ())->build_dire (dire) ;
	}

	imports void clear_dire (CREF<String<STR>> dire) {
		return FileProcHolder::hold (instance ())->clear_dire (dire) ;
	}

	imports void erase_dire (CREF<String<STR>> dire) {
		return FileProcHolder::hold (instance ())->erase_dire (dire) ;
	}

	imports BOOL lock_dire (CREF<String<STR>> dire) {
		return FileProcHolder::hold (instance ())->lock_dire (dire) ;
	}
} ;

struct StreamFileImplLayout ;

struct StreamFileLayout implement ThisLayout<AutoRef<StreamFileImplLayout>> {} ;

struct StreamFileHolder implement Interface {
	imports VFat<StreamFileHolder> hold (VREF<StreamFileLayout> that) ;
	imports CFat<StreamFileHolder> hold (CREF<StreamFileLayout> that) ;

	virtual void initialize (RREF<String<STR>> file) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w (CREF<LENGTH> size_) = 0 ;
	virtual void open_a () = 0 ;
	virtual LENGTH file_size () const = 0 ;
	virtual void read (VREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void write (CREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

class StreamFile implement StreamFileLayout {
protected:
	using StreamFileLayout::mThis ;

public:
	implicit StreamFile () = default ;

	explicit StreamFile (CREF<String<STR>> file) {
		StreamFileHolder::hold (thiz)->initialize (move (file)) ;
	}

	explicit StreamFile (RREF<String<STR>> file) {
		StreamFileHolder::hold (thiz)->initialize (move (file)) ;
	}

	void open_r () {
		return StreamFileHolder::hold (thiz)->open_r () ;
	}

	void open_w (CREF<LENGTH> size_) {
		return StreamFileHolder::hold (thiz)->open_w (size_) ;
	}

	void open_a () {
		return StreamFileHolder::hold (thiz)->open_a () ;
	}

	LENGTH file_size () const {
		return StreamFileHolder::hold (thiz)->file_size () ;
	}

	void read (VREF<RefBuffer<BYTE>> item) {
		return StreamFileHolder::hold (thiz)->read (item) ;
	}

	void write (CREF<RefBuffer<BYTE>> item) {
		return StreamFileHolder::hold (thiz)->write (item) ;
	}

	void flush () {
		return StreamFileHolder::hold (thiz)->flush () ;
	}
} ;

struct StreamFileByteWriterLayout implement ByteWriter {
	StreamFile mFile ;
	RefBuffer<BYTE> mFileBuffer ;
} ;

struct StreamFileByteWriterHolder implement Interface {
	imports VFat<StreamFileByteWriterHolder> hold (VREF<StreamFileByteWriterLayout> that) ;
	imports CFat<StreamFileByteWriterHolder> hold (CREF<StreamFileByteWriterLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual void close () = 0 ;
} ;

class StreamFileByteWriter implement StreamFileByteWriterLayout {
protected:
	using StreamFileByteWriterLayout::mFile ;
	using StreamFileByteWriterLayout::mFileBuffer ;

public:
	implicit StreamFileByteWriter () = delete ;

	explicit StreamFileByteWriter (CREF<String<STR>> file) {
		StreamFileByteWriterHolder::hold (thiz)->initialize (file) ;
	}

	implicit ~StreamFileByteWriter () noexcept {
		close () ;
	}

	implicit StreamFileByteWriter (CREF<StreamFileByteWriter> that) = default ;

	forceinline VREF<StreamFileByteWriter> operator= (CREF<StreamFileByteWriter> that) = default ;

	implicit StreamFileByteWriter (RREF<StreamFileByteWriter> that) = default ;

	forceinline VREF<StreamFileByteWriter> operator= (RREF<StreamFileByteWriter> that) = default ;

	void close () {
		StreamFileByteWriterHolder::hold (thiz)->close () ;
	}
} ;

struct StreamFileTextWriterLayout implement TextWriter {
	StreamFile mFile ;
	RefBuffer<BYTE> mFileBuffer ;
} ;

struct StreamFileTextWriterHolder implement Interface {
	imports VFat<StreamFileTextWriterHolder> hold (VREF<StreamFileTextWriterLayout> that) ;
	imports CFat<StreamFileTextWriterHolder> hold (CREF<StreamFileTextWriterLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual void close () = 0 ;
} ;

class StreamFileTextWriter implement StreamFileTextWriterLayout {
protected:
	using StreamFileTextWriterLayout::mFile ;
	using StreamFileTextWriterLayout::mFileBuffer ;

public:
	implicit StreamFileTextWriter () = delete ;

	explicit StreamFileTextWriter (CREF<String<STR>> file) {
		StreamFileTextWriterHolder::hold (thiz)->initialize (file) ;
	}

	implicit ~StreamFileTextWriter () noexcept {
		close () ;
	}

	implicit StreamFileTextWriter (CREF<StreamFileTextWriter> that) = default ;

	forceinline VREF<StreamFileTextWriter> operator= (CREF<StreamFileTextWriter> that) = default ;

	implicit StreamFileTextWriter (RREF<StreamFileTextWriter> that) = default ;

	forceinline VREF<StreamFileTextWriter> operator= (RREF<StreamFileTextWriter> that) = default ;

	void close () {
		StreamFileTextWriterHolder::hold (thiz)->close () ;
	}
} ;

struct BufferFileImplLayout ;

struct BufferFileLayout implement ThisLayout<AutoRef<BufferFileImplLayout>> {} ;

struct BufferFileHolder implement Interface {
	imports VFat<BufferFileHolder> hold (VREF<BufferFileLayout> that) ;
	imports CFat<BufferFileHolder> hold (CREF<BufferFileLayout> that) ;

	virtual void initialize (RREF<String<STR>> file) = 0 ;
	virtual void set_block_step (CREF<LENGTH> step_) = 0 ;
	virtual void set_cache_size (CREF<LENGTH> size_) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w (CREF<LENGTH> size_) = 0 ;
	virtual void open_a () = 0 ;
	virtual LENGTH file_size () const = 0 ;
	virtual void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

class BufferFile implement BufferFileLayout {
protected:
	using BufferFileLayout::mThis ;

public:
	implicit BufferFile () = default ;

	explicit BufferFile (CREF<String<STR>> file) {
		BufferFileHolder::hold (thiz)->initialize (move (file)) ;
	}

	explicit BufferFile (RREF<String<STR>> file) {
		BufferFileHolder::hold (thiz)->initialize (move (file)) ;
	}

	void set_block_step (CREF<LENGTH> step_) {
		return BufferFileHolder::hold (thiz)->set_block_step (step_) ;
	}

	void set_cache_size (CREF<LENGTH> size_) {
		return BufferFileHolder::hold (thiz)->set_cache_size (size_) ;
	}

	void open_r () {
		return BufferFileHolder::hold (thiz)->open_r () ;
	}

	void open_w (CREF<LENGTH> size_) {
		return BufferFileHolder::hold (thiz)->open_w (size_) ;
	}

	void open_a () {
		return BufferFileHolder::hold (thiz)->open_a () ;
	}

	LENGTH file_size () const {
		return BufferFileHolder::hold (thiz)->file_size () ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) {
		return BufferFileHolder::hold (thiz)->read (index ,item) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) {
		return BufferFileHolder::hold (thiz)->write (index ,item) ;
	}

	void flush () {
		return BufferFileHolder::hold (thiz)->flush () ;
	}
} ;

struct ConsoleOption {
	enum {
		All ,
		NoPrint ,
		NoFatal ,
		NoError ,
		NoWarn ,
		NoInfo ,
		NoDebug ,
		NoTrace ,
		ETC
	} ;
} ;

struct ConsoleImplLayout ;

struct ConsoleLayout implement ThisLayout<SharedRef<ConsoleImplLayout>> {} ;

struct ConsoleHolder implement Interface {
	imports VFat<ConsoleHolder> hold (VREF<ConsoleLayout> that) ;
	imports CFat<ConsoleHolder> hold (CREF<ConsoleLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_option (CREF<Just<ConsoleOption>> option) const = 0 ;
	virtual void print (CREF<Format> msg) const = 0 ;
	virtual void fatal (CREF<Format> msg) const = 0 ;
	virtual void error (CREF<Format> msg) const = 0 ;
	virtual void warn (CREF<Format> msg) const = 0 ;
	virtual void info (CREF<Format> msg) const = 0 ;
	virtual void debug (CREF<Format> msg) const = 0 ;
	virtual void trace (CREF<Format> msg) const = 0 ;
	virtual void open (CREF<String<STR>> dire) const = 0 ;
	virtual void start () const = 0 ;
	virtual void stop () const = 0 ;
	virtual void pause () const = 0 ;
	virtual void clear () const = 0 ;
} ;

class Console implement ConsoleLayout {
protected:
	using ConsoleLayout::mThis ;

public:
	imports CREF<Console> instance () {
		return memorize ([&] () {
			Console ret ;
			ConsoleHolder::hold (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	void set_option (CREF<Just<ConsoleOption>> option) const {
		return ConsoleHolder::hold (thiz)->set_option (option) ;
	}

	template <class...ARG1>
	void print (CREF<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->print (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void fatal (CREF<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->fatal (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void error (CREF<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->error (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void warn (CREF<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->warn (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void info (CREF<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->info (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void debug (CREF<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->debug (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void trace (CREF<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->trace (PrintFormat (params...)) ;
	}

	void open (CREF<String<STR>> dire) const {
		return ConsoleHolder::hold (thiz)->open (dire) ;
	}

	void start () const {
		return ConsoleHolder::hold (thiz)->start () ;
	}

	void stop () const {
		return ConsoleHolder::hold (thiz)->stop () ;
	}

	void pause () const {
		return ConsoleHolder::hold (thiz)->pause () ;
	}

	void clear () const {
		return ConsoleHolder::hold (thiz)->clear () ;
	}
} ;
} ;