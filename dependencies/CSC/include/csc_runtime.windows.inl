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
class RuntimeProcImplHolder implement Fat<RuntimeProcHolder ,RuntimeProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	LENGTH thread_concurrency () const override {
		return std::thread::hardware_concurrency () ;
	}

	FLAG thread_uid () const override {
		const auto r1x = std::this_thread::get_id () ;
		return FLAG (bitwise[TYPE<CHAR>::expr] (r1x)) ;
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

	FLAG random_seed () const override {
		return invoke (std::random_device ()) ;
	}

	String<STR> working_path () const override {
		String<STR> ret = String<STR>::make () ;
		GetCurrentDirectory (csc_enum_t (ret.size ()) ,ret) ;
		ret = Path (ret).fetch () ;
		return move (ret) ;
	}

	String<STR> module_path () const override {
		String<STR> ret = String<STR>::make () ;
		GetModuleFileName (NULL ,ret ,csc_enum_t (ret.size ())) ;
		ret = Path (ret).path () ;
		return move (ret) ;
	}

	String<STR> module_name () const override {
		String<STR> ret = String<STR>::make () ;
		GetModuleFileName (NULL ,ret ,csc_enum_t (ret.size ())) ;
		ret = Path (ret).name () ;
		return move (ret) ;
	}
} ;

static const auto mRuntimeProcExternal = External<RuntimeProcHolder ,RuntimeProcLayout>::declare (RuntimeProcImplHolder ()) ;

struct ProcessImplLayout {
	FLAG mUid ;
	QUAD mProcessCode ;
	QUAD mProcessTime ;
} ;

class ProcessImplHolder implement Fat<ProcessHolder ,ProcessLayout> {
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
		const auto r1x = invoke ([&] () {
			csc_enum_t ret ;
			if ifdo (TRUE) {
				const auto r2x = GetExitCodeProcess (handle ,(&ret)) ;
				if (r2x != ZERO)
					discard ;
				ret = 0 ;
			}
			return move (ret) ;
		}) ;
		return QUAD (r1x) ;
	}

	QUAD process_time (CREF<HANDLE> handle ,CREF<FLAG> uid) const {
		const auto r1x = invoke ([&] () {
			Buffer4<FILETIME> ret ;
			inline_memset (ret) ;
			GetProcessTimes (handle ,(&ret[0]) ,(&ret[1]) ,(&ret[2]) ,(&ret[3])) ;
			return move (ret) ;
		}) ;
		return ByteProc::bit_merge (CHAR (r1x[0].dwHighDateTime) ,CHAR (r1x[0].dwLowDateTime)) ;
	}

	void initialize (CREF<RefBuffer<BYTE>> snapshot_) override {
		fake.mThis = AutoRef<ProcessImplLayout>::make () ;
		fake.mThis->mUid = 0 ;
		try {
			assume (snapshot_.size () == PROCESS_SNAPSHOT_SIZE::expr) ;
			auto rax = ByteReader (Ref<RefBuffer<BYTE>>::reference (snapshot_)) ;
			rax >> slice ("CSC_Process") ;
			rax >> GAP ;
			const auto r1x = rax.poll (TYPE<VAL32>::expr) ;
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
		rax << slice ("CSC_Process") ;
		if ifdo (TRUE) {
			rax << GAP ;
			rax << VAL64 (fake.mThis->mUid) ;
			rax << GAP ;
			rax << fake.mThis->mProcessCode ;
			rax << GAP ;
			rax << fake.mThis->mProcessTime ;
		}
		rax << GAP ;
		rax << EOS ;
		return move (ret) ;
	}
} ;

static const auto mProcessExternal = External<ProcessHolder ,ProcessLayout>::declare (ProcessImplHolder ()) ;

struct ModuleImplLayout {
	UniqueRef<HMODULE> mModule ;
	FLAG mLastError ;
} ;

class ModuleImplHolder implement Fat<ModuleHolder ,ModuleLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		const auto r1x = Path (file).name () ;
		assert (r1x.length () > 0) ;
		fake.mThis = AutoRef<ModuleImplLayout>::make () ;
		fake.mThis->mModule = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
			me = GetModuleHandle (r1x) ;
			if (me != NULL)
				return ;
			me = LoadLibrary (file) ;
			if (me != NULL)
				return ;
			fake.mThis->mLastError = FLAG (GetLastError ()) ;
			assume (FALSE) ;
		} ,[&] (VREF<HMODULE> me) {
			noop () ;
		}) ;
	}

	FLAG load (CREF<String<STR>> name) override {
		assert (name.length () > 0) ;
		const auto r1x = StringProc::stra_from_strs (name) ;
		FLAG ret = FLAG (GetProcAddress (fake.mThis->mModule ,r1x)) ;
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

static const auto mModuleExternal = External<ModuleHolder ,ModuleLayout>::declare (ModuleImplHolder ()) ;

struct SingletonLayout {
	Mutex mMutex ;
	Pin<Set<Clazz>> mClazzSet ;

public:
	imports CREF<Pin<SingletonLayout>> instance () {
		return memorize ([&] () {
			return Pin<SingletonLayout> () ;
		}) ;
	}
} ;

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
	Ref<SingletonLayout> mRoot ;
} ;

class SingletonProcImplHolder implement Fat<SingletonProcHolder ,SingletonProcLayout> {
public:
	void initialize () override {
		fake.mThis = AutoRef<SingletonProcImplLayout>::make () ;
		fake.mThis->mUid = RuntimeProc::process_uid () ;
		fake.mThis->mName = String<STR>::make (slice ("CSC_Singleton_") ,fake.mThis->mUid) ;
		link_pipe () ;
	}

	void link_pipe () {
		auto rax = SingletonPipe () ;
		auto act = TRUE ;
		if ifdo (act) {
			try {
				init_pipe () ;
				rax = load_pipe () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (act) {
			try {
				save_pipe () ;
				rax = load_pipe () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (TRUE) {
			const auto r1x = FLAG (rax.mAddress1) ;
			assume (r1x != ZERO) ;
			if (address (fake.mThis->mRoot.self) == r1x)
				return ;
			auto &&rbx = keep[TYPE<SingletonLayout>::expr] (Pointer::make (r1x)) ;
			fake.mThis->mRoot = Ref<SingletonLayout>::reference (rbx) ;
		}
	}

	void init_pipe () {
		if (fake.mThis->mPipe.exist ())
			return ;
		fake.mThis->mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (SIZE_OF<SingletonPipe>::expr) ;
			me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,r1x ,fake.mThis->mName) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mRoot = Ref<SingletonLayout>::reference (SingletonLayout::instance ().self) ;
		auto &&rax = fake.mThis->mRoot.leak () ;
		rax.mMutex = RecursiveMutex () ;
	}

	SingletonPipe load_pipe () const {
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
		SingletonPipe ret ;
		inline_memcpy (Pointer::from (ret) ,Pointer::make (r3x) ,SIZE_OF<SingletonPipe>::expr) ;
		assume (ret.mReserve1 == QUAD (fake.mThis->mUid)) ;
		assume (ret.mReserve2 == abi_reserve ()) ;
		assume (ret.mReserve3 == ctx_reserve ()) ;
		assume (ret.mAddress1 == ret.mAddress2) ;
		return move (ret) ;
	}

	void save_pipe () const {
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
		auto rax = SingletonPipe () ;
		rax.mReserve1 = QUAD (fake.mThis->mUid) ;
		rax.mAddress1 = QUAD (address (fake.mThis->mRoot.self)) ;
		rax.mReserve2 = abi_reserve () ;
		rax.mAddress2 = QUAD (address (fake.mThis->mRoot.self)) ;
		rax.mReserve3 = ctx_reserve () ;
		inline_memcpy (Pointer::make (r3x) ,Pointer::from (rax) ,SIZE_OF<SingletonPipe>::expr) ;
	}

	QUAD abi_reserve () const {
		QUAD ret = QUAD (0X00) ;
#ifdef __CSC_VER_DEBUG__
		ret |= QUAD (0X00000001) ;
#elif defined __CSC_VER_UNITTEST__
		ret |= QUAD (0X00000001) ;
#elif defined __CSC_VER_RELEASE__
		ret |= QUAD (0X00000001) ;
#endif
#ifdef __CSC_COMPILER_MSVC__
		ret |= QUAD (0X00000010) ;
#elif defined __CSC_COMPILER_GNUC__
		ret |= QUAD (0X00000020) ;
#elif defined __CSC_COMPILER_CLANG__
		ret |= QUAD (0X00000040) ;
#endif
#ifdef __CSC_SYSTEM_WINDOWS__
		ret |= QUAD (0X00000100) ;
#elif defined __CSC_SYSTEM_LINUX__
		ret |= QUAD (0X00000200) ;
#endif
#ifdef __CSC_TARGET_EXE__
		ret |= QUAD (0X00001000) ;
#elif defined __CSC_TARGET_DLL__
		ret |= QUAD (0X00001000) ;
#elif defined __CSC_TARGET_LIB__
		ret |= QUAD (0X00001000) ;
#endif
#ifdef __CSC_PLATFORM_X86__
		ret |= QUAD (0X00010000) ;
#elif defined __CSC_PLATFORM_X64__
		ret |= QUAD (0X00020000) ;
#elif defined __CSC_PLATFORM_ARM__
		ret |= QUAD (0X00030000) ;
#elif defined __CSC_PLATFORM_ARM64__
		ret |= QUAD (0X00040000) ;
#endif
#ifdef __CSC_CONFIG_VAL32__
		ret |= QUAD (0X00100000) ;
#elif defined __CSC_CONFIG_VAL64__
		ret |= QUAD (0X00200000) ;
#endif
#ifdef __CSC_CONFIG_STRA__
		ret |= QUAD (0X01000000) ;
#elif defined __CSC_CONFIG_STRW__
		ret |= QUAD (0X02000000) ;
#endif
#ifdef __CSC_CXX_LATEST__
		ret |= QUAD (0X10000000) ;
#else
		ret |= QUAD (0X10000000) ;
#endif
		return move (ret) ;
	}

	QUAD ctx_reserve () const override {
		return QUAD (0XAAAABBBBCCCCDDDD) ;
	}

	FLAG load (CREF<Clazz> clazz) const override {
		Scope<Mutex> anonymous (fake.mThis->mRoot->mMutex) ;
		FLAG ret = fake.mThis->mRoot->mClazzSet->map (clazz) ;
		replace (ret ,NONE ,ZERO) ;
		return move (ret) ;
	}

	void save (CREF<Clazz> clazz ,CREF<FLAG> addr) const override {
		assert (addr != ZERO) ;
		assert (addr != NONE) ;
		Scope<Mutex> anonymous (fake.mThis->mRoot->mMutex) ;
		fake.mThis->mRoot->mClazzSet->add (clazz ,addr) ;
	}
} ;

static const auto mSingletonProcExternal = External<SingletonProcHolder ,SingletonProcLayout>::declare (SingletonProcImplHolder ()) ;

struct PathImplLayout {
	String<STR> mPathName ;
	Deque<INDEX> mSeparator ;
} ;

class PathImplHolder implement Fat<PathHolder ,PathLayout> {
public:
	void initialize (RREF<String<STR>> pathname) override {
		auto rax = PathImplLayout () ;
		rax.mPathName = move (pathname) ;
		rax.mSeparator.add (NONE) ;
		const auto r1x = rax.mPathName.length () ;
		assume (r1x > 0) ;
		for (auto &&i : iter (0 ,r1x)) {
			if (!is_separator (rax.mPathName[i]))
				continue ;
			rax.mSeparator.add (i) ;
			rax.mPathName[i] = STR ('/') ;
		}
		rax.mSeparator.add (r1x) ;
		if ifdo (TRUE) {
			if (rax.mSeparator[1] != 0)
				discard ;
			rax.mSeparator.take () ;
		}
		if ifdo (TRUE) {
			INDEX ix = rax.mSeparator[rax.mSeparator.length () - 1] ;
			if (ix != r1x - 1)
				discard ;
			rax.mPathName.trunc (ix) ;
			rax.mSeparator.pop () ;
		}
		fake.mThis = Ref<PathImplLayout>::make (move (rax)) ;
	}

	BOOL is_separator (CREF<STRU32> str) const {
		if (str == STRU32 ('\\'))
			return TRUE ;
		if (str == STRU32 ('/'))
			return TRUE ;
		return FALSE ;
	}

	void initialize (CREF<PathLayout> that) override {
		fake.mThis = that.mThis.share () ;
	}

	String<STR> fetch () const override {
		return fake.mThis->mPathName ;
	}

	PathLayout root () const override {
		PathLayout ret ;
		if ifdo (TRUE) {
			if (!StreamProc::is_alpha (fake.mThis->mPathName[0]))
				discard ;
			if (fake.mThis->mPathName[1] != STRU32 (':'))
				discard ;
			ret = Path (Slice (FLAG (fake.mThis->mPathName.self) ,2 ,SIZE_OF<STR>::expr)) ;
		}
		return move (ret) ;
	}

	PathLayout child (CREF<Slice> name) const override {
		PathLayout ret ;
		ret = Path (String<STR>::make (fake.mThis->mPathName ,slice ("/") ,name)) ;
		return move (ret) ;
	}

	PathLayout child (CREF<Format> name) const override {
		PathLayout ret ;
		ret = Path (String<STR>::make (fake.mThis->mPathName ,slice ("/") ,name)) ;
		return move (ret) ;
	}

	PathLayout child (CREF<String<STR>> name) const override {
		PathLayout ret ;
		ret = Path (String<STR>::make (fake.mThis->mPathName ,slice ("/") ,name)) ;
		return move (ret) ;
	}

	Array<PathLayout> list () const override {
		auto rax = WIN32_FIND_DATA () ;
		const auto r1x = String<STR>::make (fake.mThis->mPathName ,slice ("/") ,slice ("*.*")) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = FindFirstFile (r1x ,(&rax)) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			if (me == NULL)
				return ;
			FindClose (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r2x == NULL)
				discard ;
			assert (Slice (rax.cFileName).eos () == slice (".")) ;
			FindNextFile (r2x ,(&rax)) ;
			assert (Slice (rax.cFileName).eos () == slice ("..")) ;
			while (TRUE) {
				const auto r3x = FindNextFile (r2x ,(&rax)) ;
				if (!r3x)
					break ;
				rbx.add (Slice (rax.cFileName)) ;
			}
		}
		Array<PathLayout> ret = Array<PathLayout> (rbx.length ()) ;
		for (auto &&i : ret.range ())
			ret[i] = child (rbx[i]) ;
		return move (ret) ;
	}

	Array<PathLayout> list (CREF<LENGTH> size_) const override {
		auto rax = WIN32_FIND_DATA () ;
		const auto r1x = String<STR>::make (fake.mThis->mPathName ,slice ("/") ,slice ("*.*")) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = FindFirstFile (r1x ,(&rax)) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			if (me == NULL)
				return ;
			FindClose (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r2x == NULL)
				discard ;
			assert (Slice (rax.cFileName).eos () == slice (".")) ;
			FindNextFile (r2x ,(&rax)) ;
			assert (Slice (rax.cFileName).eos () == slice ("..")) ;
			while (TRUE) {
				const auto r3x = FindNextFile (r2x ,(&rax)) ;
				if (!r3x)
					break ;
				if (rbx.length () >= size_)
					break ;
				rbx.add (Slice (rax.cFileName)) ;
			}
		}
		Array<PathLayout> ret = Array<PathLayout> (size_) ;
		const auto r4x = inline_min (rbx.length () ,size_) ;
		for (auto &&i : iter (0 ,r4x))
			ret[i] = child (rbx[i]) ;
		for (auto &&i : iter (r4x ,size_))
			PathHolder::hold (ret[i])->initialize (fake) ;
		return move (ret) ;
	}

	BOOL equal (CREF<PathLayout> that) const override {
		return fake.mThis->mPathName == that.mThis->mPathName ;
	}

	BOOL is_file () const override {
		const auto r1x = CHAR (GetFileAttributes (fake.mThis->mPathName)) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return (!ByteProc::bit_any (r1x ,FILE_ATTRIBUTE_DIRECTORY)) ;
	}

	BOOL is_dire () const override {
		const auto r1x = CHAR (GetFileAttributes (fake.mThis->mPathName)) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return ByteProc::bit_any (r1x ,FILE_ATTRIBUTE_DIRECTORY) ;
	}

	BOOL is_link () const override {
		const auto r1x = CHAR (GetFileAttributes (fake.mThis->mPathName)) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return ByteProc::bit_any (r1x ,FILE_ATTRIBUTE_REPARSE_POINT) ;
	}

	Deque<String<STR>> decouple () const override {
		Deque<String<STR>> ret = Deque<String<STR>> (fake.mThis->mSeparator.length ()) ;
		for (auto &&i : iter (1 ,fake.mThis->mSeparator.length ())) {
			const auto r1x = fake.mThis->mSeparator[i - 1] + 1 ;
			const auto r2x = fake.mThis->mSeparator[i] ;
			const auto r3x = address (fake.mThis->mPathName.at (r1x)) ;
			const auto r4x = Slice (r3x ,r2x - r1x ,SIZE_OF<STR>::expr) ;
			ret.add (r4x) ;
		}
		return move (ret) ;
	}

	String<STR> path () const override {
		const auto r1x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 2] + 1 ;
		String<STR> ret = String<STR> (r1x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (0 ,r1x)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STR> name () const override {
		const auto r1x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 2] + 1 ;
		const auto r2x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 1] ;
		String<STR> ret = String<STR> (r2x - r1x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (r1x ,r2x)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STR> stem () const override {
		const auto r1x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 2] + 1 ;
		const auto r2x = find_last_dot_word () ;
		String<STR> ret = String<STR> (r2x - r1x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (r1x ,r2x)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STR> extension () const override {
		const auto r1x = find_last_dot_word () ;
		const auto r2x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 1] ;
		String<STR> ret = String<STR> (r2x - r1x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (r1x ,r2x)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	INDEX find_last_dot_word () const {
		INDEX ix = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 1] - 1 ;
		while (TRUE) {
			if (ix <= 0)
				break ;
			if (fake.mThis->mPathName[ix] == STRU32 ('.'))
				return ix ;
			if (!StreamProc::is_word (fake.mThis->mPathName[ix]))
				break ;
			ix-- ;
		}
		return fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 1] ;
	}
} ;

static const auto mPathExternal = External<PathHolder ,PathLayout>::declare (PathImplHolder ()) ;

struct FileProcImplLayout {
	Mutex mMutex ;
	Pin<List<UniqueRef<String<STR>>>> mLockDirectory ;
} ;

class FileProcImplHolder implement Fat<FileProcHolder ,FileProcLayout> {
public:
	void initialize () override {
		fake.mThis = AutoRef<FileProcImplLayout>::make () ;
		fake.mThis->mMutex = NULL ;
	}

	RefBuffer<BYTE> load_file (CREF<String<STR>> file) const override {
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFile (file ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = file_size (r1x) ;
		assume (r2x < VAL32_MAX) ;
		const auto r3x = LENGTH (r2x) ;
		RefBuffer<BYTE> ret = RefBuffer<BYTE> (r3x) ;
		auto rax = r3x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_enum_t (rax) ;
			const auto r4x = ReadFile (r1x ,(&ret[r3x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r4x) ;
			rax -= LENGTH (rbx) ;
		}
		assume (rax == 0) ;
		return move (ret) ;
	}

	VAL64 file_size (CREF<HANDLE> handle) const {
		auto rax = Buffer2<csc_enum_t> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = VAL64 (ByteProc::bit_merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
		assume (r1x >= 0) ;
		return r1x ;
	}

	void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> item) const override {
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFile (file ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = item.size () ;
		auto rax = r2x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_enum_t (rax) ;
			const auto r3x = WriteFile (r1x ,(&item[r2x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r3x) ;
			rax -= LENGTH (rbx) ;
		}
		assume (rax == 0) ;
	}

	Ref<RefBuffer<BYTE>> load_asset (CREF<String<STR>> file) const override {
		const auto r1x = String<STR> (slice (".bin")) ;
		const auto r2x = FindResource (NULL ,file ,r1x) ;
		assume (r2x != NULL) ;
		const auto r3x = LENGTH (SizeofResource (NULL ,r2x)) ;
		assume (r3x > 0) ;
		const auto r4x = LoadResource (NULL ,r2x) ;
		assume (r4x != NULL) ;
		const auto r5x = FLAG (LockResource (r4x)) ;
		return Ref<RefBuffer<BYTE>>::make (RefBuffer<BYTE>::reference (r5x ,r3x)) ;
	}

	void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = CopyFile (src ,dst ,TRUE) ;
		noop (r1x) ;
	}

	void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = MoveFile (src ,dst) ;
		noop (r1x) ;
	}

	void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = CreateHardLink (dst ,src ,NULL) ;
		noop (r1x) ;
	}

	void erase_file (CREF<String<STR>> file) const override {
		const auto r1x = DeleteFile (file) ;
		noop (r1x) ;
	}

	void build_dire (CREF<String<STR>> dire) const override {
		const auto r1x = Path (dire).decouple () ;
		auto rax = String<STR>::make () ;
		INDEX ix = 0 ;
		for (auto &&i : r1x) {
			rax.splice (ix ,i) ;
			ix = rax.length () ;
			CreateDirectory (rax ,NULL) ;
			rax.splice (ix ,slice ("/")) ;
			ix++ ;
		}
	}

	void clear_dire (CREF<String<STR>> dire) const override {
		auto rax = Deque<Tuple<Path ,BOOL>> () ;
		rax.add ({Path (dire) ,TRUE}) ;
		clear_dire_push (rax ,0) ;
		rax.take () ;
		while (TRUE) {
			if (rax.empty ())
				break ;
			INDEX ix = rax.tail () ;
			auto act = TRUE ;
			if ifdo (act) {
				if (!rax[ix].m2nd)
					discard ;
				rax[ix].m2nd = FALSE ;
				clear_dire_push (rax ,ix) ;
			}
			if ifdo (act) {
				erase_dire (rax[ix].m1st) ;
				rax.take () ;
			}
		}
	}

	void clear_dire_push (VREF<Deque<Tuple<Path ,BOOL>>> queue ,CREF<INDEX> curr) const {
		const auto r1x = queue[curr].m1st.list () ;
		for (auto &&i : r1x) {
			auto act = TRUE ;
			if ifdo (act) {
				if (!i.is_link ())
					discard ;
				erase_file (i) ;
			}
			if ifdo (act) {
				if (!i.is_file ())
					discard ;
				erase_file (i) ;
			}
			if ifdo (act) {
				if (!i.is_dire ())
					discard ;
				queue.add ({i ,TRUE}) ;
			}
		}
	}

	void erase_dire (CREF<String<STR>> dire) const override {
		const auto r1x = RemoveDirectory (dire) ;
		assume (r1x) ;
	}

	BOOL lock_dire (CREF<String<STR>> dire) const override {
		const auto r1x = Path (dire).child (slice (".lockdirectory")) ;
		const auto r2x = Process (RuntimeProc::process_uid ()) ;
		if ifdo (TRUE) {
			if (r1x.is_file ())
				discard ;
			const auto r3x = FileProc::load_file (r1x) ;
			const auto r4x = Process (r3x) ;
			const auto r5x = Process (r4x.process_uid ()) ;
			if (r4x != r5x)
				discard ;
			return r4x == r2x ;
		}
		lock_dire_push (r1x ,r2x.snapshot ()) ;
		return TRUE ;
	}

	void lock_dire_push (CREF<Path> file ,CREF<RefBuffer<BYTE>> snapshot_) const {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		auto rax = UniqueRef<String<STR>> ([&] (VREF<String<STR>> me) {
			me = file ;
			FileProc::save_file (me ,snapshot_) ;
		} ,[&] (VREF<String<STR>> me) {
			FileProc::erase_dire (me) ;
		}) ;
		fake.mThis->mLockDirectory->add (move (rax)) ;
	}
} ;

static const auto mFileProcExternal = External<FileProcHolder ,FileProcLayout>::declare (FileProcImplHolder ()) ;

struct StreamFileImplLayout {
	String<STR> mFile ;
	UniqueRef<HANDLE> mReadPipe ;
	UniqueRef<HANDLE> mWritePipe ;
	VAL64 mFileSize ;
	VAL64 mRead ;
	VAL64 mWrite ;
} ;

class StreamFileImplHolder implement Fat<StreamFileHolder ,StreamFileLayout> {
public:
	void initialize (RREF<String<STR>> file) override {
		fake.mThis = AutoRef<StreamFileImplLayout>::make () ;
		fake.mThis->mFile = move (file) ;
		fake.mThis->mFileSize = 0 ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
	}

	void open_r () override {
		assert (!fake.mThis->mReadPipe.exist ()) ;
		assert (!fake.mThis->mWritePipe.exist ()) ;
		fake.mThis->mReadPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (fake.mThis->mFile ,GENERIC_READ ,r1x ,NULL ,OPEN_EXISTING ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mReadPipe) ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!fake.mThis->mReadPipe.exist ()) ;
		assert (!fake.mThis->mWritePipe.exist ()) ;
		fake.mThis->mWritePipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (fake.mThis->mFile ,GENERIC_WRITE ,r1x ,NULL ,CREATE_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mFileSize = size_ ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
	}

	void open_a () override {
		assert (!fake.mThis->mReadPipe.exist ()) ;
		assert (!fake.mThis->mWritePipe.exist ()) ;
		fake.mThis->mReadPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (fake.mThis->mFile ,GENERIC_READ ,r1x ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mWritePipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r2x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (fake.mThis->mFile ,GENERIC_WRITE ,r2x ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mReadPipe) ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
		if ifdo (TRUE) {
			auto rax = LONG (0) ;
			const auto r3x = SetFilePointer (fake.mThis->mWritePipe ,0 ,(&rax) ,FILE_END) ;
			const auto r4x = VAL64 (ByteProc::bit_merge (CHAR (rax) ,CHAR (r3x))) ;
			if (r4x <= 0)
				return ;
			fake.mThis->mWrite += r4x ;
		}
	}

	LENGTH file_size () const override {
		assume (fake.mThis->mFileSize < VAL32_MAX) ;
		return LENGTH (fake.mThis->mFileSize) ;
	}

	VAL64 file_size (CREF<HANDLE> handle) const {
		auto rax = Buffer2<csc_enum_t> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = VAL64 (ByteProc::bit_merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
		assume (r1x >= 0) ;
		return r1x ;
	}

	void read (VREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mReadPipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_enum_t (rax) ;
			const auto r2x = ReadFile (fake.mThis->mReadPipe ,(&item[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r2x) ;
			rax -= LENGTH (rbx) ;
		}
		fake.mThis->mRead += r1x ;
	}

	void write (CREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mWritePipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_enum_t (rax) ;
			const auto r2x = WriteFile (fake.mThis->mWritePipe ,(&item[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r2x) ;
			rax -= LENGTH (rbx) ;
		}
		fake.mThis->mWrite += r1x ;
	}

	void flush () override {
		if (!fake.mThis->mWritePipe.exist ())
			return ;
		FlushFileBuffers (fake.mThis->mWritePipe) ;
	}
} ;

static const auto mStreamFileExternal = External<StreamFileHolder ,StreamFileLayout>::declare (StreamFileImplHolder ()) ;

struct BufferFileHeader {
	VAL64 mFileEndian ;
	VAL64 mFileSize ;
	VAL64 mBlockSize ;
	VAL64 mBlockStep ;
	VAL64 mBlockLength ;
	VAL64 mChunkSize ;
	VAL64 mChunkStep ;
	VAL64 mChunkLength ;
} ;

struct BufferFileChunk {
	VAL64 mIndex ;
	VAL64 mCacheTime ;
	UniqueRef<Tuple<FLAG ,FLAG>> mBlock ;
} ;

struct BufferFileImplLayout {
	String<STR> mFile ;
	UniqueRef<HANDLE> mPipe ;
	UniqueRef<HANDLE> mMapping ;
	VAL64 mFileSize ;
	VAL64 mBlockStep ;
	VAL64 mChunkStep ;
	csc_enum_t mFileMapFlag ;
	Box<BufferFileHeader> mHeader ;
	Set<VAL64> mCacheSet ;
	List<BufferFileChunk> mCacheList ;
	VAL64 mCacheTimer ;
} ;

class BufferFileImplHolder implement Fat<BufferFileHolder ,BufferFileLayout> {
private:
	using BLOCK_STEP_SIZE = ENUM<1024> ;
	using CHUNK_STEP_SIZE = ENUM<4194304> ;
	using HEADER_SIZE = ENUM<65536> ;

public:
	void initialize (RREF<String<STR>> file) override {
		fake.mThis = AutoRef<BufferFileImplLayout>::make () ;
		fake.mThis->mFile = move (file) ;
		fake.mThis->mFileSize = 0 ;
		fake.mThis->mFileMapFlag = 0 ;
		set_block_step (BLOCK_STEP_SIZE::expr) ;
		set_cache_size (1) ;
	}

	void set_block_step (CREF<LENGTH> step_) override {
		fake.mThis->mBlockStep = step_ ;
		fake.mThis->mChunkStep = CHUNK_STEP_SIZE::expr ;
	}

	void set_cache_size (CREF<LENGTH> size_) override {
		fake.mThis->mCacheSet = Set<VAL64> (size_) ;
		fake.mThis->mCacheList = List<BufferFileChunk> (size_) ;
		fake.mThis->mCacheTimer = 0 ;
	}

	void open_r () override {
		assert (!fake.mThis->mPipe.exist ()) ;
		assert (!fake.mThis->mMapping.exist ()) ;
		fake.mThis->mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFile (fake.mThis->mFile ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mPipe) ;
		fake.mThis->mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (ByteProc::bit_high (QUAD (fake.mThis->mFileSize))) ;
			const auto r2x = csc_enum_t (ByteProc::bit_low (QUAD (fake.mThis->mFileSize))) ;
			me = CreateFileMapping (fake.mThis->mPipe ,NULL ,PAGE_READONLY ,r1x ,r2x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mFileMapFlag = csc_enum_t (FILE_MAP_READ) ;
		read_header () ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!fake.mThis->mPipe.exist ()) ;
		assert (!fake.mThis->mMapping.exist ()) ;
		fake.mThis->mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (GENERIC_READ | GENERIC_WRITE) ;
			me = CreateFile (fake.mThis->mFile ,r1x ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = fake.mThis->mChunkStep / fake.mThis->mBlockStep ;
		const auto r3x = (size_ + r2x - 1) / r2x ;
		fake.mThis->mFileSize = HEADER_SIZE::expr + r3x * fake.mThis->mChunkStep ;
		fake.mThis->mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r4x = csc_enum_t (ByteProc::bit_high (QUAD (fake.mThis->mFileSize))) ;
			const auto r5x = csc_enum_t (ByteProc::bit_low (QUAD (fake.mThis->mFileSize))) ;
			me = CreateFileMapping (fake.mThis->mPipe ,NULL ,PAGE_READWRITE ,r4x ,r5x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mFileMapFlag = csc_enum_t (FILE_MAP_READ | FILE_MAP_WRITE) ;
		write_header () ;
	}

	void open_a () override {
		assert (!fake.mThis->mPipe.exist ()) ;
		assert (!fake.mThis->mMapping.exist ()) ;
		assume (fake.mThis->mHeader != NULL) ;
		fake.mThis->mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (GENERIC_READ | GENERIC_WRITE) ;
			me = CreateFile (fake.mThis->mFile ,r1x ,0 ,NULL ,OPEN_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mFileSize = fake.mThis->mHeader->mFileSize ;
		fake.mThis->mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (ByteProc::bit_high (QUAD (fake.mThis->mFileSize))) ;
			const auto r2x = csc_enum_t (ByteProc::bit_low (QUAD (fake.mThis->mFileSize))) ;
			me = CreateFileMapping (fake.mThis->mPipe ,NULL ,PAGE_READWRITE ,r1x ,r2x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mFileMapFlag = csc_enum_t (FILE_MAP_READ | FILE_MAP_WRITE) ;
		read_header () ;
	}

	void read_header () {
		assert (fake.mThis->mHeader == NULL) ;
		fake.mThis->mHeader = Box<BufferFileHeader>::make () ;
		auto rax = ByteReader (borrow_header ()) ;
		rax >> slice ("CSC_BufferFile") ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mFileEndian ;
		assume (fake.mThis->mHeader->mFileEndian == file_endian ()) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mFileSize ;
		assume (fake.mThis->mHeader->mFileSize == fake.mThis->mFileSize) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mBlockSize ;
		const auto r1x = fake.mThis->mChunkStep / fake.mThis->mBlockStep ;
		assume (fake.mThis->mHeader->mBlockSize == r1x) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mBlockStep ;
		assume (fake.mThis->mHeader->mBlockStep == fake.mThis->mBlockStep) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mBlockLength ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mChunkSize ;
		assume (fake.mThis->mHeader->mChunkSize >= 0) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mChunkStep ;
		assume (fake.mThis->mHeader->mChunkStep == fake.mThis->mChunkStep) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mChunkLength ;
		rax >> GAP ;
	}

	void write_header () {
		if ifdo (TRUE) {
			if (fake.mThis->mHeader != NULL)
				discard ;
			fake.mThis->mHeader = Box<BufferFileHeader>::make () ;
			fake.mThis->mHeader->mFileEndian = file_endian () ;
			fake.mThis->mHeader->mFileSize = fake.mThis->mFileSize ;
			fake.mThis->mHeader->mBlockSize = fake.mThis->mChunkStep / fake.mThis->mBlockStep ;
			fake.mThis->mHeader->mBlockStep = fake.mThis->mBlockStep ;
			fake.mThis->mHeader->mBlockLength = 0 ;
			fake.mThis->mHeader->mChunkStep = fake.mThis->mChunkStep ;
			fake.mThis->mHeader->mChunkSize = (fake.mThis->mFileSize - HEADER_SIZE::expr) / fake.mThis->mChunkStep ;
			fake.mThis->mHeader->mChunkLength = 0 ;
		}
		auto rax = ByteWriter (borrow_header ()) ;
		rax << slice ("CSC_BufferFile") ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mFileEndian ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mFileSize ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mBlockSize ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mBlockStep ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mBlockLength ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mChunkSize ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mChunkStep ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mChunkLength ;
		rax << GAP ;
		flush () ;
	}

	Ref<RefBuffer<BYTE>> borrow_header () {
		INDEX ix = mmap_cache (0 ,HEADER_SIZE::expr) ;
		const auto r1x = fake.mThis->mCacheList[ix].mBlock->m1st ;
		const auto r2x = HEADER_SIZE::expr ;
		return Ref<RefBuffer<BYTE>>::make (RefBuffer<BYTE>::reference (r1x ,r2x)) ;
	}

	VAL64 file_endian () const {
		const auto r1x = invoke ([&] () {
			auto rax = Buffer<BYTE ,SIZE_OF<CHAR>> () ;
			rax[0] = BYTE (0X00) ;
			rax[1] = BYTE (0X01) ;
			rax[2] = BYTE (0X02) ;
			rax[3] = BYTE (0X03) ;
			return bitwise[TYPE<CHAR>::expr] (rax) ;
		}) ;
		return VAL64 (r1x) ;
	}

	LENGTH file_size () const override {
		assume (fake.mThis->mFileSize < VAL32_MAX) ;
		return LENGTH (fake.mThis->mFileSize) ;
	}

	VAL64 file_size (CREF<HANDLE> handle) const {
		auto rax = Buffer2<csc_enum_t> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = VAL64 (ByteProc::bit_merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
		assume (r1x >= 0) ;
		return r1x ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mPipe.exist ()) ;
		assert (inline_between (index ,0 ,LENGTH (fake.mThis->mHeader->mBlockSize))) ;
		assert (item.size () == fake.mThis->mHeader->mBlockStep) ;
		const auto r1x = index / fake.mThis->mHeader->mBlockSize ;
		const auto r2x = index % fake.mThis->mHeader->mBlockSize * fake.mThis->mHeader->mBlockStep ;
		const auto r3x = HEADER_SIZE::expr + r1x * fake.mThis->mHeader->mChunkStep ;
		INDEX ix = mmap_cache (r3x ,LENGTH (fake.mThis->mHeader->mChunkStep)) ;
		const auto r4x = fake.mThis->mCacheList[ix].mBlock->m1st + LENGTH (r2x) ;
		inline_memcpy (Pointer::from (item.self) ,Pointer::make (r4x) ,LENGTH (fake.mThis->mHeader->mBlockStep)) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mPipe.exist ()) ;
		assert (inline_between (index ,0 ,LENGTH (fake.mThis->mHeader->mBlockSize))) ;
		assert (item.size () == fake.mThis->mHeader->mBlockStep) ;
		const auto r1x = index / fake.mThis->mHeader->mBlockSize ;
		const auto r2x = index % fake.mThis->mHeader->mBlockSize * fake.mThis->mHeader->mBlockStep ;
		const auto r3x = HEADER_SIZE::expr + r1x * fake.mThis->mHeader->mChunkStep ;
		INDEX ix = mmap_cache (r3x ,LENGTH (fake.mThis->mHeader->mChunkStep)) ;
		const auto r4x = fake.mThis->mCacheList[ix].mBlock->m1st + LENGTH (r2x) ;
		inline_memcpy (Pointer::make (r4x) ,Pointer::from (item.self) ,LENGTH (fake.mThis->mHeader->mBlockStep)) ;
	}

	INDEX mmap_cache (CREF<VAL64> index ,CREF<LENGTH> size_) {
		INDEX ret = fake.mThis->mCacheSet.map (index) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			update_overflow () ;
			ret = fake.mThis->mCacheList.insert () ;
			fake.mThis->mCacheSet.add (index ,ret) ;
			fake.mThis->mCacheList[ret].mIndex = index ;
			fake.mThis->mCacheList[ret].mBlock = UniqueRef<Tuple<FLAG ,FLAG>> ([&] (VREF<Tuple<FLAG ,FLAG>> me) {
				const auto r1x = csc_enum_t (ByteProc::bit_high (QUAD (index))) ;
				const auto r2x = csc_enum_t (ByteProc::bit_low (QUAD (index))) ;
				const auto r3x = MapViewOfFile (fake.mThis->mMapping ,fake.mThis->mFileMapFlag ,r1x ,r2x ,size_) ;
				assume (r3x != NULL) ;
				me.m1st = FLAG (r3x) ;
				me.m2nd = me.m1st + size_ ;
			} ,[&] (VREF<Tuple<FLAG ,FLAG>> me) {
				const auto r4x = csc_pointer_t (me.m1st) ;
				const auto r5x = me.m2nd - me.m1st ;
				FlushViewOfFile (r4x ,r5x) ;
				UnmapViewOfFile (r4x) ;
			}) ;
		}
		fake.mThis->mCacheList[ret].mCacheTime = fake.mThis->mCacheTimer ;
		fake.mThis->mCacheTimer++ ;
		if ifdo (TRUE) {
			if (fake.mThis->mCacheTimer < VAL32_MAX)
				discard ;
			for (auto &&i : fake.mThis->mCacheList.range ())
				fake.mThis->mCacheList[i].mCacheTime = 0 ;
			fake.mThis->mCacheList[ret].mCacheTime = 1 ;
			fake.mThis->mCacheTimer = 2 ;
		}
		return move (ret) ;
	}

	void update_overflow () {
		if (fake.mThis->mCacheList.length () < fake.mThis->mCacheList.size ())
			return ;
		const auto r1x = invoke ([&] () {
			INDEX ret = NONE ;
			auto rax = VAL64 () ;
			for (auto &&i : fake.mThis->mCacheList.range ()) {
				if (ret != NONE)
					if (rax >= fake.mThis->mCacheList[i].mCacheTime)
						continue ;
				ret = i ;
				rax = fake.mThis->mCacheList[i].mCacheTime ;
			}
			return move (ret) ;
		}) ;
		assert (r1x != NONE) ;
		fake.mThis->mCacheSet.erase (fake.mThis->mCacheList[r1x].mIndex) ;
		fake.mThis->mCacheList.remove (r1x) ;
	}

	void flush () override {
		if (!fake.mThis->mPipe.exist ())
			return ;
		fake.mThis->mCacheSet.clear () ;
		fake.mThis->mCacheList.clear () ;
		fake.mThis->mCacheTimer = 0 ;
		FlushFileBuffers (fake.mThis->mPipe) ;
	}
} ;

static const auto mBufferFileExternal = External<BufferFileHolder ,BufferFileLayout>::declare (BufferFileImplHolder ()) ;

struct ConsoleImplLayout {
	Mutex mMutex ;
	BitSet mOption ;
	UniqueRef<HANDLE> mConsole ;
	String<STR> mWriterBuffer ;
	TextWriter mWriter ;
	String<STR> mLogFile ;
	String<STR> mOldLogFile ;
	StreamFile mLogFileWriter ;
} ;

class ConsoleImplHolder implement Fat<ConsoleHolder ,ConsoleLayout> {
private:
	using CONSOLE_BUFFER_SIZE = ENUM<4194304> ;

public:
	void initialize () override {
		fake.mThis = SharedRef<ConsoleImplLayout>::make () ;
		fake.mThis->mMutex = RecursiveMutex () ;
		fake.mThis->mOption = BitSet (ConsoleOption::ETC) ;
		fake.mThis->mWriterBuffer = String<STR> (CONSOLE_BUFFER_SIZE::expr) ;
		fake.mThis->mWriter = TextWriter (fake.mThis->mWriterBuffer.borrow ()) ;
	}

	void set_option (CREF<Just<ConsoleOption>> option) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (option != ConsoleOption::All)
				discard ;
			fake.mThis->mOption.clear () ;
		}
		if ifdo (act) {
			fake.mThis->mOption.add (option) ;
		}
	}

	void log (CREF<String<STR>> tag ,CREF<Format> msg) const {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		fake.mThis->mWriter << CLS ;
		fake.mThis->mWriter << slice ("[") ;
		const auto r1x = CurrentTime () ;
		const auto r2x = r1x.calendar () ;
		fake.mThis->mWriter << AlignedText (r2x.mHour ,2) ;
		fake.mThis->mWriter << slice (":") ;
		fake.mThis->mWriter << AlignedText (r2x.mMinute ,2) ;
		fake.mThis->mWriter << slice (":") ;
		fake.mThis->mWriter << AlignedText (r2x.mSecond ,2) ;
		fake.mThis->mWriter << slice ("][") ;
		fake.mThis->mWriter << tag ;
		fake.mThis->mWriter << slice ("] : ") ;
		fake.mThis->mWriter << msg ;
		fake.mThis->mWriter << GAP ;
		fake.mThis->mWriter << EOS ;
	}

	void print (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoPrint])
			return ;
		log (slice ("Print") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mWriterBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void fatal (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoFatal])
			return ;
		log (slice ("Fatal") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mWriterBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void error (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoError])
			return ;
		log (slice ("Error") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mWriterBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void warn (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoWarn])
			return ;
		log (slice ("Warn") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mWriterBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void info (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoInfo])
			return ;
		log (slice ("Info") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mWriterBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void debug (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoDebug])
			return ;
		log (slice ("Debug") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mWriterBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void trace (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoTrace])
			return ;
		log (slice ("Trace") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mWriterBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void open (CREF<String<STR>> dire) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		fake.mThis->mLogFile = Path (dire).child (slice ("console.log")) ;
		fake.mThis->mOldLogFile = Path (dire).child (slice ("console.old.log")) ;
		FileProc::erase_file (fake.mThis->mOldLogFile) ;
		FileProc::move_file (fake.mThis->mOldLogFile ,fake.mThis->mLogFile) ;
		fake.mThis->mLogFileWriter = StreamFile (fake.mThis->mLogFile) ;
		fake.mThis->mLogFileWriter.open_w (0) ;
		fake.mThis->mWriter << CLS ;
		fake.mThis->mWriter << BOM ;
		fake.mThis->mWriter << EOS ;
		log_file () ;
	}

	void log_file () const {
		if (fake.mThis->mLogFile.length () == 0)
			return ;
		const auto r1x = FLAG (fake.mThis->mWriterBuffer.self) ;
		const auto r2x = (fake.mThis->mWriter.length () - 1) * SIZE_OF<STR>::expr ;
		fake.mThis->mLogFileWriter.write (RefBuffer<BYTE>::reference (r1x ,r2x)) ;
	}

	void start () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mConsole.exist ())
			if (fake.mThis->mConsole.self != INVALID_HANDLE_VALUE)
				return ;
		fake.mThis->mConsole = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			AllocConsole () ;
			me = GetStdHandle (STD_OUTPUT_HANDLE) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			FreeConsole () ;
		}) ;
	}

	void stop () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		fake.mThis->mConsole = UniqueRef<HANDLE>::make (INVALID_HANDLE_VALUE) ;
	}

	void pause () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if ifdo (TRUE) {
			const auto r1x = GetConsoleWindow () ;
			if (r1x == NULL)
				discard ;
			FlashWindow (r1x ,TRUE) ;
		}
		const auto r2x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
		SetConsoleTextAttribute (fake.mThis->mConsole ,r2x) ;
		const auto r3x = String<STRA> (slice ("pause")) ;
		const auto r4x = std::system (r3x) ;
		noop (r4x) ;
	}

	void clear () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
		SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
		const auto r2x = String<STRA> (slice ("cls")) ;
		const auto r3x = std::system (r2x) ;
		noop (r3x) ;
	}
} ;

static const auto mConsoleExternal = External<ConsoleHolder ,ConsoleLayout>::declare (ConsoleImplHolder ()) ;
} ;