#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_runtime.hpp"

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require windows.h"
#endif

#include "csc_end.h"
#include <cstdlib>
#include <thread>
#include "csc_begin.h"

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
		return FLAG (GetCurrentThreadId ()) ;
	}

	void thread_sleep (CREF<Time> time) const override {
		const auto r1x = time.borrow () ;
		std::this_thread::sleep_for (r1x->mTime) ;
	}

	void thread_yield () const override {
		std::this_thread::yield () ;
	}

	FLAG process_uid () const override {
		return FLAG (GetCurrentProcessId ()) ;
	}

	void process_exit () const override {
		std::quick_exit (0) ;
	}

	String<STR> library_file () const override {
		String<STR> ret = String<STR>::make () ;
		const auto r1x = invoke ([&] () {
			const auto r2x = csc_enum_t (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT) ;
			auto &&rax = keep[TYPE<HeapLayout>::expr] (Heap::instance ()) ;
			const auto r3x = LPTSTR (rax.mHolder) ;
			HMODULE ret = NULL ;
			const auto r4x = GetModuleHandleEx (r2x ,r3x ,(&ret)) ;
			assume (r4x) ;
			return move (ret) ;
		}) ;
		GetModuleFileName (r1x ,ret ,csc_enum_t (ret.size ())) ;
		return move (ret) ;
	}

	String<STR> library_main () const override {
		String<STR> ret = String<STR>::make () ;
		GetModuleFileName (NULL ,ret ,csc_enum_t (ret.size ())) ;
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
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = OpenProcess (PROCESS_QUERY_INFORMATION ,FALSE ,csc_enum_t (uid)) ;
		} ,[&] (VREF<HANDLE> me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
		}) ;
		fake.mThis->mProcessCode = process_code (r1x ,uid) ;
		fake.mThis->mProcessTime = process_time (r1x ,uid) ;
	}

	QUAD process_code (CREF<HANDLE> handle ,CREF<FLAG> uid) const {
		return QUAD (GetProcessVersion (csc_enum_t (uid))) ;
	}

	QUAD process_time (CREF<HANDLE> handle ,CREF<FLAG> uid) const {
		const auto r1x = invoke ([&] () {
			Buffer4<FILETIME> ret ;
			inline_memset (ret) ;
			GetProcessTimes (handle ,(&ret[0]) ,(&ret[1]) ,(&ret[2]) ,(&ret[3])) ;
			return move (ret) ;
		}) ;
		return ByteProc::merge (CHAR (r1x[0].dwHighDateTime) ,CHAR (r1x[0].dwLowDateTime)) ;
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
			me = GetModuleHandle (fake.mThis->mFile) ;
			if (me != NULL)
				return ;
			me = LoadLibrary (fake.mThis->mFile) ;
			if (me != NULL)
				return ;
			fake.mThis->mLastError = FLAG (GetLastError ()) ;
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
		const auto r1x = StringProc::stra_from_strs (name) ;
		FLAG ret = FLAG (GetProcAddress (fake.mThis->mLibrary ,r1x)) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			fake.mThis->mLastError = FLAG (GetLastError ()) ;
			assume (FALSE) ;
		}
		return move (ret) ;
	}

	String<STR> error () const override {
		String<STR> ret = String<STR>::make () ;
		const auto r1x = csc_enum_t (fake.mThis->mLastError) ;
		const auto r2x = csc_enum_t (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
		const auto r3x = csc_enum_t (ret.size ()) ;
		FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,r1x ,r2x ,ret ,r3x ,NULL) ;
		ret = String<STR>::make (Format (slice ("LastError = $1 : $2")) (FLAG (r1x) ,ret)) ;
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
	UniqueRef<HANDLE> mPipe ;
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
		fake.mThis->mName = String<STR>::make (slice ("CSC_Singleton_") ,fake.mThis->mUid) ;
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
		fake.mThis->mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (SIZE_OF<SingletonPipe>::expr) ;
			me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,r1x ,fake.mThis->mName) ;
			assume (me != NULL) ;
			const auto r2x = GetLastError () ;
			assume (r2x != ERROR_ALREADY_EXISTS) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r3x = address (SingletonRoot::instance ().self) ;
		auto &&rax = keep[TYPE<SingletonLayout>::expr] (Pointer::make (r3x)) ;
		rax.mMutex = NULL ;
		fake.mThis->mLocal.mReserve1 = QUAD (fake.mThis->mUid) ;
		fake.mThis->mLocal.mAddress1 = QUAD (r3x) ;
		fake.mThis->mLocal.mReserve2 = abi_reserve () ;
		fake.mThis->mLocal.mAddress2 = QUAD (r3x) ;
		fake.mThis->mLocal.mReserve3 = ctx_reserve () ;
	}

	void load_pipe () {
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = OpenFileMapping (FILE_MAP_READ ,FALSE ,fake.mThis->mName) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = MapViewOfFile (r1x ,FILE_MAP_READ ,0 ,0 ,SIZE_OF<SingletonPipe>::expr) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			UnmapViewOfFile (me) ;
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
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = OpenFileMapping (FILE_MAP_WRITE ,FALSE ,fake.mThis->mName) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = MapViewOfFile (r1x ,FILE_MAP_WRITE ,0 ,0 ,SIZE_OF<SingletonPipe>::expr) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			UnmapViewOfFile (me) ;
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