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
#ifdef __CSC_SYSTEM_LINUX__
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <sys/mman.h>
#include <sys/syscall.h>
#endif

#include <cstdlib>
#include <thread>
#include "csc_begin.h"

inline namespace {
using HMODULE = CSC::csc_pointer_t ;
using HANDLE = CSC::csc_pointer_t ;
using HFILEPIPE = int ;
} ;

namespace CSC {
struct RuntimeProcLayout {} ;

class RuntimeProcImplHolder final implement Fat<RuntimeProcHolder ,RuntimeProcLayout> {
public:
	void create (VREF<UniqueRef<RuntimeProcLayout>> that) const override {
		that = UniqueRef<RuntimeProcLayout>::make () ;
	}

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
		const auto r1x = String<STR> (slice ("/proc/deref/exe")) ;
		const auto r2x = INDEX (readlink (r1x ,ret ,csc_size_t (ret.size ()))) ;
		ret.trunc (r2x) ;
		return move (ret) ;
	}
} ;

static const auto mRuntimeProcExternal = External<RuntimeProcHolder ,RuntimeProcLayout> (RuntimeProcImplHolder ()) ;

struct ProcessLayout {
	FLAG mUid ;
	QUAD mProcessCode ;
	QUAD mProcessTime ;
} ;

class ProcessImplHolder final implement Fat<ProcessHolder ,ProcessLayout> {
private:
	using PROCESS_SNAPSHOT_STEP = ENUM<128> ;

public:
	void create (VREF<AutoRef<ProcessLayout>> that) const override {
		that = AutoRef<ProcessLayout>::make () ;
	}

	void initialize (CREF<FLAG> uid) override {
		self.mUid = uid ;
		const auto r1x = load_proc_file (uid) ;
		self.mProcessCode = process_code (r1x ,uid) ;
		self.mProcessTime = process_time (r1x ,uid) ;
	}

	String<STRU8> load_proc_file (CREF<FLAG> uid) const {
		String<STRU8> ret = String<STRU8>::make () ;
		ret.fill (STRU32 (0X00)) ;
		const auto r1x = String<STR>::make (Format (slice ("/proc/$1/stat")) (uid)) ;
		try {
			auto rax = StreamFile (r1x) ;
			rax.open_r () ;
			auto rbx = ret.borrow () ;
			rax.set_short_read (TRUE) ;
			rax.read (rbx.deref) ;
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
		self.mUid = 0 ;
		try {
			assume (snapshot_.size () == PROCESS_SNAPSHOT_STEP::expr) ;
			auto rax = ByteReader (Ref<RefBuffer<BYTE>>::reference (snapshot_)) ;
			rax >> slice ("CSC_Process") ;
			rax >> GAP ;
			const auto r1x = rax.poll (TYPE<VAL64>::expr) ;
			self.mUid = FLAG (r1x) ;
			rax >> GAP ;
			rax >> self.mProcessCode ;
			rax >> GAP ;
			rax >> self.mProcessTime ;
			rax >> GAP ;
			rax >> EOS ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
	}

	BOOL equal (CREF<ProcessLayout> that) const override {
		const auto r1x = inline_equal (self.mUid ,that.mUid) ;
		if (!r1x)
			return r1x ;
		const auto r2x = inline_equal (self.mProcessCode ,that.mProcessCode) ;
		if (!r2x)
			return r2x ;
		const auto r3x = inline_equal (self.mProcessTime ,that.mProcessTime) ;
		if (!r3x)
			return r3x ;
		return TRUE ;
	}

	FLAG process_uid () const override {
		return self.mUid ;
	}

	RefBuffer<BYTE> snapshot () const override {
		RefBuffer<BYTE> ret = RefBuffer<BYTE> (PROCESS_SNAPSHOT_STEP::expr) ;
		auto rax = ByteWriter (Ref<RefBuffer<BYTE>>::reference (ret)) ;
		if ifdo (TRUE) {
			rax << slice ("CSC_Process") ;
			rax << GAP ;
			rax << VAL64 (self.mUid) ;
			rax << GAP ;
			rax << self.mProcessCode ;
			rax << GAP ;
			rax << self.mProcessTime ;
			rax << GAP ;
			rax << EOS ;
		}
		return move (ret) ;
	}
} ;

static const auto mProcessExternal = External<ProcessHolder ,ProcessLayout> (ProcessImplHolder ()) ;

struct LibraryLayout {
	String<STR> mFile ;
	UniqueRef<HMODULE> mLibrary ;
	FLAG mLastError ;
} ;

class LibraryImplHolder final implement Fat<LibraryHolder ,LibraryLayout> {
public:
	void create (VREF<AutoRef<LibraryLayout>> that) const override {
		that = AutoRef<LibraryLayout>::make () ;
	}

	void initialize (CREF<String<STR>> file) override {
		self.mFile = move (file) ;
		assert (self.mFile.length () > 0) ;
		self.mLibrary = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
			const auto r1x = csc_enum_t (RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NODELETE) ;
			const auto r2x = csc_enum_t (r1x | RTLD_NOLOAD) ;
			me = dlopen (self.mFile ,r2x) ;
			if (me != NULL)
				return ;
			me = dlopen (self.mFile ,r1x) ;
			if (me != NULL)
				return ;
			self.mLastError = FLAG (errno) ;
			assume (FALSE) ;
		} ,[&] (VREF<HMODULE> me) {
			noop () ;
		}) ;
	}

	String<STR> library_file () const override {
		return self.mFile ;
	}

	FLAG load (CREF<String<STR>> name) override {
		assert (name.length () > 0) ;
		FLAG ret = FLAG (dlsym (self.mLibrary ,name)) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			self.mLastError = FLAG (errno) ;
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

struct SingletonRoot {
	Mutex mMutex ;
	Pin<Set<Clazz>> mClazzSet ;
} ;

struct SingletonPipe {
	QUAD mReserve1 ;
	QUAD mAddress1 ;
	QUAD mReserve2 ;
	QUAD mAddress2 ;
	QUAD mReserve3 ;
} ;

struct SingletonProcLayout {
	FLAG mUid ;
	String<STR> mName ;
	UniqueRef<HANDLE> mMapping ;
	SingletonPipe mLocal ;
	Ref<SingletonRoot> mThis ;

public:
	implicit SingletonProcLayout () = default ;

	implicit ~SingletonProcLayout () noexcept {
		if (mThis == NULL)
			return ;
		mThis->~SingletonRoot () ;
	}
} ;

class SingletonProcImplHolder final implement Fat<SingletonProcHolder ,SingletonProcLayout> {
public:
	void create (VREF<UniqueRef<SingletonProcLayout>> that) const override {
		that = UniqueRef<SingletonProcLayout>::make () ;
	}

	void initialize () override {
		self.mUid = RuntimeProc::process_uid () ;
		self.mName = String<STR>::make (slice ("/CSC_Singleton_") ,self.mUid) ;
		inline_memset (self.mLocal) ;
		link_pipe () ;
	}

	static VREF<SingletonRoot> root_ptr (CREF<SingletonProcLayout> that) {
		return memorize ([&] () {
			return Pin<SingletonRoot> () ;
		}).deref ;
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
			const auto r1x = FLAG (self.mLocal.mAddress1) ;
			assume (r1x != ZERO) ;
			auto &&rax = keep[TYPE<SingletonRoot>::expr] (Pointer::make (r1x)) ;
			self.mThis = Ref<SingletonRoot>::reference (rax) ;
		}
	}

	void init_pipe () {
		if (self.mMapping.exist ())
			return ;
		self.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_EXCL) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			const auto r3x = shm_open (self.mName ,r1x ,r2x) ;
			assume (r3x != NONE) ;
			const auto r4x = ftruncate (r3x ,SIZE_OF<SingletonPipe>::expr) ;
			assume (r4x == 0) ;
			me = HANDLE (self.mName.deref) ;
		} ,[&] (VREF<HANDLE> me) {
			shm_unlink (DEF<const char *> (me)) ;
		}) ;
		auto &&rax = keep[TYPE<SingletonRoot>::expr] (root_ptr (self)) ;
		rax.mMutex = NULL ;
		self.mLocal.mReserve1 = QUAD (self.mUid) ;
		self.mLocal.mAddress1 = QUAD (address (rax)) ;
		self.mLocal.mReserve2 = abi_reserve () ;
		self.mLocal.mAddress2 = QUAD (address (rax)) ;
		self.mLocal.mReserve3 = ctx_reserve () ;
	}

	void load_pipe () {
		const auto r1x = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			me = shm_open (self.mName ,O_RDONLY ,0) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			noop () ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = mmap (NULL ,SIZE_OF<SingletonPipe>::expr ,PROT_READ ,MAP_SHARED ,r1x ,0) ;
			replace (me ,MAP_FAILED ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			munmap (me ,SIZE_OF<SingletonPipe>::expr) ;
		}) ;
		const auto r3x = FLAG (r2x.deref) ;
		auto rax = SingletonPipe () ;
		inline_memcpy (Pointer::from (rax) ,Pointer::make (r3x) ,SIZE_OF<SingletonPipe>::expr) ;
		assume (rax.mReserve1 == QUAD (self.mUid)) ;
		assume (rax.mAddress1 != QUAD (0X00)) ;
		assume (rax.mAddress1 == rax.mAddress2) ;
		assume (rax.mReserve2 == abi_reserve ()) ;
		assume (rax.mReserve3 == ctx_reserve ()) ;
		self.mLocal = rax ;
	}

	void save_pipe () {
		const auto r1x = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			me = shm_open (self.mName ,O_RDWR ,0) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			noop () ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = mmap (NULL ,SIZE_OF<SingletonPipe>::expr ,PROT_WRITE ,MAP_SHARED ,r1x ,0) ;
			replace (me ,MAP_FAILED ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			munmap (me ,SIZE_OF<SingletonPipe>::expr) ;
		}) ;
		const auto r3x = FLAG (r2x.deref) ;
		auto rax = self.mLocal ;
		assume (rax.mReserve1 == QUAD (self.mUid)) ;
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
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		auto rax = Set<Clazz> () ;
		self.mThis->mClazzSet.get (rax) ;
		FLAG ret = rax.map (clazz) ;
		self.mThis->mClazzSet.set (rax) ;
		replace (ret ,NONE ,ZERO) ;
		return move (ret) ;
	}

	void save (CREF<Clazz> clazz ,CREF<FLAG> layout) const override {
		assert (layout != ZERO) ;
		assert (layout != NONE) ;
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		assume (self.mThis.exist ()) ;
		auto rax = Set<Clazz> () ;
		self.mThis->mClazzSet.get (rax) ;
		rax.add (clazz ,layout) ;
		self.mThis->mClazzSet.set (rax) ;
	}
} ;

static const auto mSingletonProcExternal = External<SingletonProcHolder ,SingletonProcLayout> (SingletonProcImplHolder ()) ;
} ;