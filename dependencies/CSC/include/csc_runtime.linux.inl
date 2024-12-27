#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_runtime.hpp"

#ifdef __CSC_CONFIG_STRW__
#error "∑(っ°Д° ;)っ : unsupported"
#endif

#include "csc_end.h"
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <sys/mman.h>
#include <sys/syscall.h>
#include "csc_begin.h"

#include "csc_end.h"
#include <cstdlib>
#include <thread>
#include "csc_begin.h"

inline namespace {
using HMODULE = CSC::csc_pointer_t ;
using HANDLE = CSC::csc_pointer_t ;
using HFILE = int ;
} ;

namespace CSC {
class RuntimeProcImplHolder final implement Fat<RuntimeProcHolder ,RuntimeProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	LENGTH thread_concurrency () const override {
		return std::thread::hardware_concurrency () ;
	}

	FLAG thread_uid () const override {
		return FLAG (syscall (SYS_gettid)) ;
	}

	void thread_sleep (CREF<Time> time) const override {
		const auto r1x = time.borrow () ;
		std::this_thread::sleep_for (r1x->mTime) ;
	}

	void thread_yield () const override {
		std::this_thread::yield () ;
	}

	FLAG process_uid () const override {
		return FLAG (syscall (SYS_getpid)) ;
	}

	void process_exit () const override {
		std::quick_exit (0) ;
	}

	String<STR> library_file () const override {
		auto &&rax = keep[TYPE<HeapLayout>::expr] (Heap::instance ()) ;
		const auto r1x = csc_pointer_t (rax.mHolder) ;
		auto rbx = Dl_info () ;
		const auto r2x = dladdr (r1x ,(&rbx)) ;
		assume (r2x != ZERO) ;
		const auto r3x = Slice (FLAG (rbx.dli_fname) ,SLICE_MAX_SIZE::expr ,1).eos () ;
		return String<STR> (r3x) ;
	}

	String<STR> library_main () const override {
		String<STR> ret = String<STR>::make () ;
		const auto r1x = String<STR> (slice ("/proc/self/exe")) ;
		const auto r2x = INDEX (readlink (r1x ,ret ,csc_size_t (ret.size ()))) ;
		ret.trunc (r2x) ;
		return move (ret) ;
	}
} ;

static const auto mRuntimeProcExternal = External<RuntimeProcHolder ,RuntimeProcLayout> (RuntimeProcImplHolder ()) ;

struct ProcessImplLayout {
	FLAG mUid ;
	QUAD mProcessCode ;
	QUAD mProcessTime ;
} ;

class ProcessImplHolder final implement Fat<ProcessHolder ,ProcessLayout> {
private:
	using PROCESS_SNAPSHOT_SIZE = ENUM<128> ;

public:
	void initialize (CREF<FLAG> uid) override {
		fake.mThis = AutoRef<ProcessImplLayout>::make () ;
		fake.mThis->mUid = uid ;
		const auto r1x = load_proc_file (uid) ;
		fake.mThis->mProcessCode = process_code (r1x ,uid) ;
		fake.mThis->mProcessTime = process_time (r1x ,uid) ;
	}

	String<STRU8> load_proc_file (CREF<FLAG> uid) const {
		String<STRU8> ret = String<STRU8>::make () ;
		ret.fill (STRU32 (0X00)) ;
		const auto r1x = String<STR>::make (Format (slice ("/proc/$1/stat")) (uid)) ;
		try {
			auto rax = StreamFile (r1x) ;
			rax.open_r () ;
			auto rbx = ret.borrow () ;
			auto &&rcx = keep[TYPE<RefBuffer<BYTE>>::expr] (rbx.pin ().self) ;
			rax.set_short_read (TRUE) ;
			rax.read (rcx) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
			ret.clear () ;
		}
		return move (ret) ;
	}

	QUAD process_code (CREF<String<STRU8>> info ,CREF<FLAG> uid) const {
		return QUAD (getpgid (pid_t (uid))) ;
	}

	QUAD process_time (CREF<String<STRU8>> info ,CREF<FLAG> uid) const {
		if (info.length () == 0)
			return QUAD (0X00) ;
		auto rax = TextReader (info.borrow ()) ;
		auto rbx = String<STRU8> () ;
		rax >> GAP ;
		rax >> BlankText::from (rbx) ;
		const auto r1x = StringParse<VAL64>::make (rbx) ;
		assume (r1x == uid) ;
		rax >> GAP ;
		rax >> slice ("(") ;
		while (TRUE) {
			const auto r2x = rax.poll (TYPE<STRU32>::expr) ;
			if (r2x == STRU32 (0X00))
				break ;
			if (r2x == STRU32 (')'))
				break ;
			assume (r2x != STRU32 ('(')) ;
		}
		rax >> GAP ;
		rax >> BlankText::from (rbx) ;
		assume (rbx.length () == 1) ;
		for (auto &&i : iter (0 ,18)) {
			noop (i) ;
			rax >> GAP ;
			rax >> BlankText::from (rbx) ;
		}
		rax >> GAP ;
		rax >> BlankText::from (rbx) ;
		const auto r3x = StringParse<VAL64>::make (rbx) ;
		return QUAD (r3x) ;
	}

	void initialize (CREF<RefBuffer<BYTE>> snapshot_) override {
		fake.mThis = AutoRef<ProcessImplLayout>::make () ;
		fake.mThis->mUid = 0 ;
		try {
			assume (snapshot_.size () == PROCESS_SNAPSHOT_SIZE::expr) ;
			auto rax = ByteReader (Ref<RefBuffer<BYTE>>::reference (snapshot_)) ;
			rax >> slice ("CSC_Process") ;
			rax >> GAP ;
			const auto r1x = rax.poll (TYPE<VAL64>::expr) ;
			fake.mThis->mUid = FLAG (r1x) ;
			rax >> GAP ;
			rax >> fake.mThis->mProcessCode ;
			rax >> GAP ;
			rax >> fake.mThis->mProcessTime ;
			rax >> GAP ;
			rax >> EOS ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
	}

	BOOL equal (CREF<ProcessLayout> that) const override {
		const auto r1x = inline_equal (fake.mThis->mUid ,that.mThis->mUid) ;
		if (!r1x)
			return r1x ;
		const auto r2x = inline_equal (fake.mThis->mProcessCode ,that.mThis->mProcessCode) ;
		if (!r2x)
			return r2x ;
		const auto r3x = inline_equal (fake.mThis->mProcessTime ,that.mThis->mProcessTime) ;
		if (!r3x)
			return r3x ;
		return TRUE ;
	}

	FLAG process_uid () const override {
		return fake.mThis->mUid ;
	}

	RefBuffer<BYTE> snapshot () const override {
		RefBuffer<BYTE> ret = RefBuffer<BYTE> (PROCESS_SNAPSHOT_SIZE::expr) ;
		auto rax = ByteWriter (Ref<RefBuffer<BYTE>>::reference (ret)) ;
		if ifdo (TRUE) {
			rax << slice ("CSC_Process") ;
			rax << GAP ;
			rax << VAL64 (fake.mThis->mUid) ;
			rax << GAP ;
			rax << fake.mThis->mProcessCode ;
			rax << GAP ;
			rax << fake.mThis->mProcessTime ;
			rax << GAP ;
			rax << EOS ;
		}
		return move (ret) ;
	}
} ;

static const auto mProcessExternal = External<ProcessHolder ,ProcessLayout> (ProcessImplHolder ()) ;

struct LibraryImplLayout {
	String<STR> mFile ;
	UniqueRef<HMODULE> mLibrary ;
	FLAG mLastError ;
} ;

class LibraryImplHolder final implement Fat<LibraryHolder ,LibraryLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mThis = AutoRef<LibraryImplLayout>::make () ;
		fake.mThis->mFile = move (file) ;
		assert (fake.mThis->mFile.length () > 0) ;
		fake.mThis->mLibrary = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
			const auto r1x = csc_enum_t (RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NODELETE) ;
			const auto r2x = csc_enum_t (r1x | RTLD_NOLOAD) ;
			me = dlopen (fake.mThis->mFile ,r2x) ;
			if (me != NULL)
				return ;
			me = dlopen (fake.mThis->mFile ,r1x) ;
			if (me != NULL)
				return ;
			fake.mThis->mLastError = FLAG (errno) ;
			assume (FALSE) ;
		} ,[&] (VREF<HMODULE> me) {
			noop () ;
		}) ;
	}

	String<STR> library_file () const override {
		return fake.mThis->mFile ;
	}

	FLAG load (CREF<String<STR>> name) override {
		assert (name.length () > 0) ;
		FLAG ret = FLAG (dlsym (fake.mThis->mLibrary ,name)) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			fake.mThis->mLastError = FLAG (errno) ;
			assume (FALSE) ;
		}
		return move (ret) ;
	}

	String<STR> error () const override {
		String<STR> ret = String<STR>::make () ;
		const auto r1x = FLAG (dlerror ()) ;
		assume (r1x != ZERO) ;
		const auto r2x = Slice (r1x ,SLICE_MAX_SIZE::expr ,1).eos () ;
		ret = String<STR>::make (Format (slice ("LastError = $1 : $2")) (r1x ,r2x)) ;
		return move (ret) ;
	}
} ;

static const auto mLibraryExternal = External<LibraryHolder ,LibraryLayout> (LibraryImplHolder ()) ;

struct SingletonLayout {
	Mutex mMutex ;
	Pin<Set<Clazz>> mClazzSet ;
} ;

class SingletonRoot implement Pin<SingletonLayout> {
public:
	imports CREF<SingletonRoot> instance () ;
} ;

exports CREF<SingletonRoot> SingletonRoot::instance () {
	return memorize ([&] () {
		return SingletonRoot () ;
	}) ;
}

struct SingletonPipe {
	QUAD mReserve1 ;
	QUAD mAddress1 ;
	QUAD mReserve2 ;
	QUAD mAddress2 ;
	QUAD mReserve3 ;
} ;

struct SingletonProcImplLayout {
	FLAG mUid ;
	String<STR> mName ;
	UniqueRef<Tuple<HFILE ,String<STR>>> mPipe ;
	SingletonPipe mLocal ;
	Ref<SingletonLayout> mRoot ;

public:
	implicit SingletonProcImplLayout () = default ;

	implicit ~SingletonProcImplLayout () noexcept {
		if (mRoot == NULL)
			return ;
		mRoot.pin ().~Pin () ;
	}
} ;

class SingletonProcImplHolder final implement Fat<SingletonProcHolder ,SingletonProcLayout> {
public:
	void initialize () override {
		fake.mThis = AutoRef<SingletonProcImplLayout>::make () ;
		fake.mThis->mUid = RuntimeProc::process_uid () ;
		fake.mThis->mName = String<STR>::make (slice ("/CSC_Singleton_") ,fake.mThis->mUid) ;
		inline_memset (fake.mThis->mLocal) ;
		link_pipe () ;
	}

	void link_pipe () {
		auto act = TRUE ;
		if ifdo (act) {
			try {
				load_pipe () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (act) {
			try {
				init_pipe () ;
				save_pipe () ;
				load_pipe () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (TRUE) {
			const auto r1x = FLAG (fake.mThis->mLocal.mAddress1) ;
			assume (r1x != ZERO) ;
			auto &&rax = keep[TYPE<SingletonLayout>::expr] (Pointer::make (r1x)) ;
			fake.mThis->mRoot = Ref<SingletonLayout>::reference (rax) ;
		}
	}

	void init_pipe () {
		if (fake.mThis->mPipe.exist ())
			return ;
		fake.mThis->mPipe = UniqueRef<Tuple<HFILE ,String<STR>>> ([&] (VREF<Tuple<HFILE ,String<STR>>> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_EXCL) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me.m1st = shm_open (fake.mThis->mName ,r1x ,r2x) ;
			assume (me.m1st != NONE) ;
			me.m2nd = fake.mThis->mName ;
		} ,[&] (VREF<Tuple<HFILE ,String<STR>>> me) {
			shm_unlink (me.m2nd) ;
		}) ;
		const auto r3x = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = ftruncate (fake.mThis->mPipe->m1st ,SIZE_OF<SingletonPipe>::expr) ;
			assume (me == 0) ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		const auto r4x = address (SingletonRoot::instance ().self) ;
		auto &&rax = keep[TYPE<SingletonLayout>::expr] (Pointer::make (r4x)) ;
		rax.mMutex = NULL ;
		fake.mThis->mLocal.mReserve1 = QUAD (fake.mThis->mUid) ;
		fake.mThis->mLocal.mAddress1 = QUAD (r4x) ;
		fake.mThis->mLocal.mReserve2 = abi_reserve () ;
		fake.mThis->mLocal.mAddress2 = QUAD (r4x) ;
		fake.mThis->mLocal.mReserve3 = ctx_reserve () ;
	}

	void load_pipe () {
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = shm_open (fake.mThis->mName ,O_RDONLY ,0) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			noop () ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = mmap (NULL ,SIZE_OF<SingletonPipe>::expr ,PROT_READ ,MAP_SHARED ,r1x ,0) ;
			replace (me ,MAP_FAILED ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			munmap (me ,SIZE_OF<SingletonPipe>::expr) ;
		}) ;
		const auto r3x = FLAG (r2x.self) ;
		auto rax = SingletonPipe () ;
		inline_memcpy (Pointer::from (rax) ,Pointer::make (r3x) ,SIZE_OF<SingletonPipe>::expr) ;
		assume (rax.mReserve1 == QUAD (fake.mThis->mUid)) ;
		assume (rax.mAddress1 != QUAD (0X00)) ;
		assume (rax.mAddress1 == rax.mAddress2) ;
		assume (rax.mReserve2 == abi_reserve ()) ;
		assume (rax.mReserve3 == ctx_reserve ()) ;
		fake.mThis->mLocal = rax ;
	}

	void save_pipe () {
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = shm_open (fake.mThis->mName ,O_RDWR ,0) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			noop () ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = mmap (NULL ,SIZE_OF<SingletonPipe>::expr ,PROT_WRITE ,MAP_SHARED ,r1x ,0) ;
			replace (me ,MAP_FAILED ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			munmap (me ,SIZE_OF<SingletonPipe>::expr) ;
		}) ;
		const auto r3x = FLAG (r2x.self) ;
		auto rax = fake.mThis->mLocal ;
		assume (rax.mReserve1 == QUAD (fake.mThis->mUid)) ;
		assume (rax.mAddress1 != QUAD (0X00)) ;
		assume (rax.mAddress1 == rax.mAddress2) ;
		assume (rax.mReserve2 == abi_reserve ()) ;
		assume (rax.mReserve3 == ctx_reserve ()) ;
		inline_memcpy (Pointer::make (r3x) ,Pointer::from (rax) ,SIZE_OF<SingletonPipe>::expr) ;
	}

	QUAD abi_reserve () const override {
		QUAD ret = QUAD (0X00) ;
#ifdef __CSC_VER_DEBUG__
		ret |= QUAD (0X00000001) ;
#elif defined __CSC_VER_UNITTEST__
		ret |= QUAD (0X00000002) ;
#elif defined __CSC_VER_RELEASE__
		ret |= QUAD (0X00000003) ;
#endif
#ifndef __CSC_COMPILER_NVCC__
#ifdef __CSC_COMPILER_MSVC__
		ret |= QUAD (0X00000010) ;
#elif defined __CSC_COMPILER_GNUC__
		ret |= QUAD (0X00000020) ;
#elif defined __CSC_COMPILER_CLANG__
		ret |= QUAD (0X00000030) ;
#endif
#else
		ret |= QUAD (0X00000040) ;
#endif
#ifdef __CSC_SYSTEM_WINDOWS__
		ret |= QUAD (0X00000100) ;
#elif defined __CSC_SYSTEM_LINUX__
		ret |= QUAD (0X00000200) ;
#endif
#ifdef __CSC_PLATFORM_X86__
		ret |= QUAD (0X00001000) ;
#elif defined __CSC_PLATFORM_X64__
		ret |= QUAD (0X00002000) ;
#elif defined __CSC_PLATFORM_ARM__
		ret |= QUAD (0X00003000) ;
#elif defined __CSC_PLATFORM_ARM64__
		ret |= QUAD (0X00004000) ;
#endif
#ifdef __CSC_CONFIG_VAL32__
		ret |= QUAD (0X00010000) ;
#elif defined __CSC_CONFIG_VAL64__
		ret |= QUAD (0X00020000) ;
#endif
#ifdef __CSC_CONFIG_STRA__
		ret |= QUAD (0X00100000) ;
#elif defined __CSC_CONFIG_STRW__
		ret |= QUAD (0X00200000) ;
#endif
		return move (ret) ;
	}

	QUAD ctx_reserve () const override {
		return QUAD (0X0FEDCBA987654321) ;
	}

	FLAG load (CREF<Clazz> clazz) const override {
		Scope<Mutex> anonymous (fake.mThis->mRoot->mMutex) ;
		auto rax = Set<Clazz> () ;
		fake.mThis->mRoot->mClazzSet.get (rax) ;
		FLAG ret = rax.map (clazz) ;
		fake.mThis->mRoot->mClazzSet.set (rax) ;
		replace (ret ,NONE ,ZERO) ;
		return move (ret) ;
	}

	void save (CREF<Clazz> clazz ,CREF<FLAG> pointer) const override {
		assert (pointer != ZERO) ;
		assert (pointer != NONE) ;
		Scope<Mutex> anonymous (fake.mThis->mRoot->mMutex) ;
		assume (fake.mThis->mPipe.exist ()) ;
		auto rax = Set<Clazz> () ;
		fake.mThis->mRoot->mClazzSet.get (rax) ;
		rax.add (clazz ,pointer) ;
		fake.mThis->mRoot->mClazzSet.set (rax) ;
	}
} ;

static const auto mSingletonProcExternal = External<SingletonProcHolder ,SingletonProcLayout> (SingletonProcImplHolder ()) ;
} ;