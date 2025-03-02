#pragma once

#ifndef __CSC_FILE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_file.hpp"

#ifdef __CSC_CONFIG_STRW__
#error "∑(っ°Д° ;)っ : unsupported"
#endif

#include "csc_end.h"
#ifdef __CSC_SYSTEM_LINUX__
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <termios.h>

#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#endif

#include <cstdio>
#include "csc_begin.h"

inline namespace {
using HMODULE = CSC::csc_pointer_t ;
using HANDLE = CSC::csc_pointer_t ;
using HFILEPIPE = int ;
using HDIR = CSC::DEF<DIR *> ;
using HDIRENT = CSC::DEF<dirent * > ;
using STAT_INFO = struct stat ;
using TERMIOS_INFO = struct termios ;
} ;

namespace std {
inline namespace {
using ::open ;
using ::close ;
using ::read ;
using ::write ;
using ::stat ;
using ::lstat ;
} ;
} ;

namespace CSC {
struct PathImplLayout {
	String<STR> mPathName ;
	Deque<INDEX> mSeparator ;
} ;

class PathImplHolder final implement Fat<PathHolder ,PathLayout> {
public:
	void initialize (RREF<String<STR>> pathname) override {
		fake.mThis = Ref<PathImplLayout>::make () ;
		fake.mThis->mPathName = move (pathname) ;
		fake.mThis->mSeparator.add (NONE) ;
		const auto r1x = fake.mThis->mPathName.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			if (!is_separator (fake.mThis->mPathName[i]))
				continue ;
			fake.mThis->mSeparator.add (i) ;
			fake.mThis->mPathName[i] = STR ('/') ;
		}
		fake.mThis->mSeparator.add (r1x) ;
		if ifdo (TRUE) {
			if (r1x == 0)
				discard ;
			INDEX ix = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 2] ;
			if (ix != r1x - 1)
				discard ;
			fake.mThis->mPathName.trunc (ix) ;
			fake.mThis->mSeparator.pop () ;
		}
		if ifdo (TRUE) {
			if (fake.mThis->mSeparator.length () != 2)
				discard ;
			INDEX ix = fake.mThis->mSeparator[0] + 1 ;
			INDEX iy = fake.mThis->mSeparator[1] ;
			if (!is_root (fake.mThis->mPathName.segment (ix ,iy)))
				discard ;
			fake.mThis->mPathName = String<STR>::make (fake.mThis->mPathName ,slice ("/") ,slice (".")) ;
			fake.mThis->mSeparator.add (iy + 2) ;
		}
		assume (fake.mThis->mSeparator.length () >= 2) ;
	}

	void initialize (CREF<Deque<String<STR>>> pathname) override {
		auto rax = String<STR>::make () ;
		INDEX ix = 0 ;
		for (auto &&i : pathname.range ()) {
			if ifdo (TRUE) {
				if (i == 0)
					discard ;
				rax.splice (ix ,slice ("/")) ;
				ix++ ;
			}
			rax.splice (ix ,pathname[i]) ;
			ix += pathname[i].length () ;
		}
		initialize (move (rax)) ;
	}

	BOOL is_separator (CREF<STRU32> str) const {
		if (str == STRU32 ('\\'))
			return TRUE ;
		if (str == STRU32 ('/'))
			return TRUE ;
		return FALSE ;
	}

	String<STR> fetch () const override {
		if (fake.mThis == NULL)
			return String<STR>::zero () ;
		return fake.mThis->mPathName ;
	}

	PathLayout child (CREF<Slice> name) const override {
		return Path (String<STR>::make (fetch () ,slice ("/") ,name)) ;
	}

	PathLayout child (CREF<Format> name) const override {
		return Path (String<STR>::make (fetch () ,slice ("/") ,name)) ;
	}

	PathLayout child (CREF<String<STR>> name) const override {
		return Path (String<STR>::make (fetch () ,slice ("/") ,name)) ;
	}

	Array<PathLayout> list () const override {
		auto rax = HDIRENT (NULL) ;
		const auto r1x = UniqueRef<HDIR> ([&] (VREF<HDIR> me) {
			me = opendir (fake.mThis->mPathName) ;
		} ,[&] (VREF<HDIR> me) {
			if (me == NULL)
				return ;
			closedir (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r1x == NULL)
				discard ;
			const auto r2x = Slice (address (rax->d_name) ,3 ,SIZE_OF<STR>::expr) ;
			noop (r2x) ;
			rax = readdir (r1x) ;
			assert (r2x.eos () == slice (".")) ;
			rax = readdir (r1x) ;
			assert (r2x.eos () == slice ("..")) ;
			while (TRUE) {
				rax = readdir (r1x) ;
				if (rax == NULL)
					break ;
				rbx.add (Slice (rax->d_name)) ;
			}
		}
		Array<PathLayout> ret = Array<PathLayout> (rbx.length ()) ;
		for (auto &&i : ret.range ())
			ret[i] = child (rbx[i]) ;
		return move (ret) ;
	}

	Array<PathLayout> list (CREF<LENGTH> size_) const override {
		auto rax = HDIRENT (NULL) ;
		const auto r1x = UniqueRef<HDIR> ([&] (VREF<HDIR> me) {
			me = opendir (fake.mThis->mPathName) ;
		} ,[&] (VREF<HDIR> me) {
			if (me == NULL)
				return ;
			closedir (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r1x == NULL)
				discard ;
			const auto r2x = Slice (address (rax->d_name) ,3 ,SIZE_OF<STR>::expr) ;
			noop (r2x) ;
			rax = readdir (r1x) ;
			assert (r2x.eos () == slice (".")) ;
			rax = readdir (r1x) ;
			assert (r2x.eos () == slice ("..")) ;
			while (TRUE) {
				rax = readdir (r1x) ;
				if (rax == NULL)
					break ;
				if (rbx.length () >= size_)
					break ;
				rbx.add (Slice (rax->d_name)) ;
			}
		}
		assume (rbx.length () == size_) ;
		Array<PathLayout> ret = Array<PathLayout> (size_) ;
		for (auto &&i : iter (0 ,size_))
			ret[i] = child (rbx[i]) ;
		return move (ret) ;
	}

	BOOL equal (CREF<PathLayout> that) const override {
		const auto r1x = inline_compr (fake.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!fake.mThis.exist ())
			return FALSE ;
		return fake.mThis->mPathName == that.mThis->mPathName ;
	}

	BOOL is_file () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = std::stat (fake.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	BOOL is_dire () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = std::stat (fake.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (!S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	BOOL is_link () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = std::lstat (fake.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (!S_ISLNK (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	PathLayout symbolic () const override {
		PathLayout ret = fake ;
		if ifdo (TRUE) {
			if (!is_link ())
				discard ;
			auto rax = String<STR>::make () ;
			const auto r1x = INDEX (readlink (fake.mThis->mPathName ,rax ,csc_size_t (rax.size ()))) ;
			rax.trunc (r1x) ;
			ret = Path (rax) ;
		}
		return move (ret) ;
	}

	PathLayout absolute () const override {
		auto rax = Deque<String<STR>> () ;
		auto rbx = decouple () ;
		while (TRUE) {
			if (rbx.empty ())
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rbx[0] != slice ("."))
					discard ;
				if ifdo (TRUE) {
					if (!rax.empty ())
						discard ;
					auto rcx = String<STR>::make () ;
					if ifdo (TRUE) {
						const auto r1x = getcwd (rcx ,csc_size_t (rcx.size ())) ;
						if (r1x != NULL)
							discard ;
						rcx.clear () ;
					}
					rax = Path (move (rcx)).decouple () ;
				}
				rbx.take () ;
			}
			if ifdo (act) {
				if (rbx[0] != slice (".."))
					discard ;
				if ifdo (TRUE) {
					if (!rax.empty ())
						discard ;
					auto rcx = String<STR>::make () ;
					if ifdo (TRUE) {
						const auto r1x = getcwd (rcx ,csc_size_t (rcx.size ())) ;
						if (r1x != NULL)
							discard ;
						rcx.clear () ;
					}
					rax = Path (move (rcx)).decouple () ;
				}
				if ifdo (TRUE) {
					if (rax.length () > 1)
						discard ;
					if (!is_root (rax[0].segment (0 ,rax[0].length ())))
						discard ;
					rax.add (String<STR>::zero ()) ;
				}
				rax.pop () ;
				rbx.take () ;
			}
			if ifdo (act) {
				rax.add (move (rbx[0])) ;
				rbx.take () ;
			}
		}
		return Path (rax) ;
	}

	BOOL is_root (CREF<Slice> str) const {
		if (str.size () == 0)
			return TRUE ;
		if (str.size () != 2)
			return FALSE ;
		if (StreamProc::is_alpha (str[0]))
			if (str[1] == STRU32 (':'))
				return TRUE ;
		return FALSE ;
	}

	Deque<String<STR>> decouple () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		Deque<String<STR>> ret = Deque<String<STR>> (r1x) ;
		for (auto &&i : iter (0 ,r1x - 1)) {
			INDEX ix = fake.mThis->mSeparator[i] + 1 ;
			INDEX iy = fake.mThis->mSeparator[i + 1] ;
			const auto r2x = fake.mThis->mPathName.segment (ix ,iy) ;
			ret.add (r2x) ;
		}
		return move (ret) ;
	}

	String<STR> path () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		INDEX ix = fake.mThis->mSeparator[0] + 1 ;
		INDEX iy = fake.mThis->mSeparator[r1x - 2] + 1 ;
		return fake.mThis->mPathName.segment (ix ,iy) ;
	}

	String<STR> name () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		INDEX ix = fake.mThis->mSeparator[r1x - 2] + 1 ;
		INDEX iy = fake.mThis->mSeparator[r1x - 1] ;
		return fake.mThis->mPathName.segment (ix ,iy) ;
	}

	String<STR> stem () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		INDEX ix = fake.mThis->mSeparator[r1x - 2] + 1 ;
		INDEX iy = find_last_dot_word () ;
		return fake.mThis->mPathName.segment (ix ,iy) ;
	}

	String<STR> extension () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		INDEX ix = find_last_dot_word () ;
		INDEX iy = fake.mThis->mSeparator[r1x - 1] ;
		return fake.mThis->mPathName.segment (ix ,iy) ;
	}

	INDEX find_last_dot_word () const {
		const auto r1x = fake.mThis->mSeparator.length () ;
		INDEX ix = fake.mThis->mSeparator[r1x - 1] - 1 ;
		while (TRUE) {
			if (ix <= 0)
				break ;
			if (fake.mThis->mPathName[ix] == STRU32 ('.'))
				return ix ;
			if (!StreamProc::is_word (fake.mThis->mPathName[ix]))
				break ;
			ix-- ;
		}
		return fake.mThis->mSeparator[r1x - 1] ;
	}
} ;

static const auto mPathExternal = External<PathHolder ,PathLayout> (PathImplHolder ()) ;

struct FileProcImplLayout {
	Mutex mMutex ;
	Pin<List<UniqueRef<String<STR>>>> mLockDirectory ;
} ;

class FileProcImplHolder final implement Fat<FileProcHolder ,FileProcImplLayout> {
private:
	using FILEPROC_RETRY_TIME = RANK3 ;

public:
	void initialize () override {
		fake.mMutex = NULL ;
	}

	RefBuffer<BYTE> load_file (CREF<String<STR>> file) const override {
		const auto r1x = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			me = std::open (file ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = file_size (r1x) ;
		assume (r2x < VAL32_MAX) ;
		const auto r3x = LENGTH (r2x) ;
		RefBuffer<BYTE> ret = RefBuffer<BYTE> (r3x) ;
		auto rax = r3x ;
		for (auto &&i : iter (0 ,FILEPROC_RETRY_TIME::expr)) {
			noop (i) ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::read (r1x ,(&ret[r3x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				break ;
		}
		assume (rax == 0) ;
		return move (ret) ;
	}

	VAL64 file_size (CREF<HFILEPIPE> handle) const {
		const auto r1x = VAL64 (lseek64 (handle ,0 ,SEEK_END)) ;
		const auto r2x = VAL64 (lseek64 (handle ,0 ,SEEK_SET)) ;
		//@warn: file in '/proc' is zero size
		if (r1x == NONE)
			return 0 ;
		if (r2x == NONE)
			return 0 ;
		assume (r1x >= 0) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> item) const override {
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r2x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r3x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (file ,r2x ,r3x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		const auto r4x = item.size () ;
		auto rax = r4x ;
		for (auto &&i : iter (0 ,FILEPROC_RETRY_TIME::expr)) {
			noop (i) ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (r1x ,(&item[r4x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				break ;
		}
		assume (rax == 0) ;
	}

	Ref<RefBuffer<BYTE>> load_asset (CREF<String<STR>> file) const override {
		const auto r1x = dlopen (NULL ,RTLD_LOCAL) ;
		assume (r1x != NULL) ;
		const auto r2x = String<STR>::make (file ,slice ("input_txt_start")) ;
		const auto r3x = String<STR>::make (file ,slice ("input_txt_end")) ;
		const auto r4x = FLAG (dlsym (r1x ,r2x)) ;
		assume (r4x != ZERO) ;
		const auto r5x = FLAG (dlsym (r1x ,r3x)) ;
		assume (r5x != ZERO) ;
		const auto r6x = r5x - r4x ;
		assume (r6x >= 0) ;
		return Ref<RefBuffer<BYTE>>::make (RefBuffer<BYTE>::reference (r4x ,r6x)) ;
	}

	void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			me = std::open (src ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (dst ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		const auto r5x = file_size (r1x) ;
		assume (r5x < VAL32_MAX) ;
		sendfile (r1x ,r2x ,NULL ,r5x) ;
	}

	void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = rename (src ,dst) ;
		noop (r1x) ;
	}

	void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		if (!Path (src).is_file ())
			return ;
		const auto r1x = symlink (src ,dst) ;
		noop (r1x) ;
	}

	void erase_file (CREF<String<STR>> file) const override {
		if (!Path (file).is_file ())
			return ;
		const auto r1x = unlink (file) ;
		noop (r1x) ;
	}

	void build_dire (CREF<String<STR>> dire) const override {
		const auto r1x = Path (dire).decouple () ;
		const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
		auto rax = Path () ;
		for (auto &&i : r1x.range ()) {
			rax = rax.child (r1x[i]) ;
			const auto r3x = rax.fetch () ;
			mkdir (r3x ,r2x) ;
		}
	}

	void link_dire (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		if (!Path (src).is_dire ())
			return ;
		const auto r1x = symlink (src ,dst) ;
		noop (r1x) ;
	}

	void clear_dire (CREF<String<STR>> dire) const override {
		auto rax = Deque<Tuple<Path ,BOOL>> () ;
		clear_dire_push (rax ,Path (dire)) ;
		while (TRUE) {
			if (rax.empty ())
				break ;
			INDEX ix = rax.tail () ;
			auto act = TRUE ;
			if ifdo (act) {
				if (!rax[ix].m2nd)
					discard ;
				rax[ix].m2nd = FALSE ;
				clear_dire_push (rax ,rax[ix].m1st) ;
			}
			if ifdo (act) {
				erase_dire (rax[ix].m1st) ;
				rax.pop () ;
			}
		}
	}

	void clear_dire_push (VREF<Deque<Tuple<Path ,BOOL>>> queue ,CREF<Path> dire) const {
		const auto r1x = dire.list () ;
		for (auto &&i : r1x) {
			auto act = TRUE ;
			if ifdo (act) {
				if (!i.is_file ())
					discard ;
				erase_file (i) ;
			}
			if ifdo (act) {
				if (!i.is_link ())
					discard ;
				erase_dire (i) ;
			}
			if ifdo (act) {
				if (!i.is_dire ())
					discard ;
				queue.add ({i ,TRUE}) ;
			}
		}
	}

	void erase_dire (CREF<String<STR>> dire) const override {
		if (!Path (dire).is_dire ())
			return ;
		const auto r1x = unlink (dire) ;
		noop (r1x) ;
		const auto r2x = rmdir (dire) ;
		noop (r2x) ;
	}

	BOOL lock_dire (CREF<String<STR>> dire) const override {
		const auto r1x = Path (dire).child (slice (".lockdirectory")) ;
		const auto r2x = Process (RuntimeProc::process_uid ()) ;
		if ifdo (TRUE) {
			if (!r1x.is_file ())
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
		Scope<Mutex> anonymous (fake.mMutex) ;
		auto rax = UniqueRef<String<STR>> ([&] (VREF<String<STR>> me) {
			me = file ;
			FileProc::save_file (me ,snapshot_) ;
		} ,[&] (VREF<String<STR>> me) {
			FileProc::erase_file (me) ;
		}) ;
		auto rbx = List<UniqueRef<String<STR>>> () ;
		fake.mLockDirectory.get (rbx) ;
		rbx.add (move (rax)) ;
		fake.mLockDirectory.set (rbx) ;
	}
} ;

static const auto mFileProcExternal = External<FileProcHolder ,FileProcLayout> (FileProcImplHolder ()) ;

struct StreamFileImplLayout {
	String<STR> mFile ;
	UniqueRef<HFILEPIPE> mReadPipe ;
	UniqueRef<HFILEPIPE> mWritePipe ;
	VAL64 mFileSize ;
	VAL64 mRead ;
	VAL64 mWrite ;
	BOOL mShortRead ;
	LENGTH mShortSize ;
} ;

class StreamFileImplHolder final implement Fat<StreamFileHolder ,StreamFileImplLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mFile = move (file) ;
		fake.mFileSize = 0 ;
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
		fake.mShortRead = FALSE ;
		fake.mShortSize = 0 ;
	}

	void set_short_read (CREF<BOOL> flag) override {
		fake.mShortRead = flag ;
	}

	void open_r () override {
		assert (!fake.mReadPipe.exist ()) ;
		assert (!fake.mWritePipe.exist ()) ;
		fake.mReadPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		fake.mFileSize = file_size (fake.mReadPipe) ;
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!fake.mReadPipe.exist ()) ;
		assert (!fake.mWritePipe.exist ()) ;
		fake.mWritePipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		fake.mFileSize = size_ ;
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
	}

	void open_a () override {
		assert (!fake.mReadPipe.exist ()) ;
		assert (!fake.mWritePipe.exist ()) ;
		fake.mReadPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDONLY) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		fake.mWritePipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mFile ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		fake.mFileSize = file_size (fake.mReadPipe) ;
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
		if ifdo (TRUE) {
			const auto r5x = VAL64 (lseek64 (fake.mWritePipe ,0 ,SEEK_END)) ;
			if (r5x <= 0)
				discard ;
			fake.mWrite += r5x ;
		}
	}

	LENGTH file_size () const override {
		assume (fake.mFileSize < VAL32_MAX) ;
		return LENGTH (fake.mFileSize) ;
	}

	VAL64 file_size (CREF<HFILEPIPE> handle) const {
		const auto r1x = VAL64 (lseek64 (handle ,0 ,SEEK_END)) ;
		const auto r2x = VAL64 (lseek64 (handle ,0 ,SEEK_SET)) ;
		//@warn: file in '/proc' is zero size
		if (r1x == NONE)
			return 0 ;
		if (r2x == NONE)
			return 0 ;
		assume (r1x >= 0) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	LENGTH short_size () const override {
		return fake.mShortSize ;
	}

	void read (VREF<RefBuffer<BYTE>> item) override {
		assert (fake.mReadPipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			auto rbx = csc_size_t (rax) ;
			rbx = std::read (fake.mReadPipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				discard ;
			assume (fake.mShortRead) ;
		}
		fake.mShortSize = r1x - rax ;
		fake.mRead += fake.mShortSize ;
	}

	void write (CREF<RefBuffer<BYTE>> item) override {
		assert (fake.mWritePipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (fake.mWritePipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				discard ;
			assume (fake.mShortRead) ;
		}
		fake.mShortSize = r1x - rax ;
		fake.mWrite += fake.mShortSize ;
	}

	void flush () override {
		if (!fake.mWritePipe.exist ())
			return ;
		fsync (fake.mWritePipe) ;
	}
} ;

static const auto mStreamFileExternal = External<StreamFileHolder ,StreamFileLayout> (StreamFileImplHolder ()) ;

struct BufferFileHeader {
	QUAD mFileEndian ;
	VAL64 mFileSize ;
	VAL64 mBlockSize ;
	VAL64 mBlockStep ;
	VAL64 mBlockLength ;
	VAL64 mChunkStep ;
	VAL64 mChunkSize ;
	VAL64 mChunkLength ;
} ;

struct BufferFileChunk {
	VAL64 mIndex ;
	VAL64 mCacheTime ;
	UniqueRef<Tuple<FLAG ,FLAG>> mBlock ;
} ;

struct BufferFileImplLayout {
	String<STR> mFile ;
	UniqueRef<HFILEPIPE> mPipe ;
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

class BufferFileImplHolder final implement Fat<BufferFileHolder ,BufferFileImplLayout> {
private:
	using BUFFERFILE_BLOCK_STEP = ENUM<1024> ;
	using BUFFERFILE_CHUNK_STEP = ENUM<4194304> ;
	using BUFFERFILE_HEADER_STEP = ENUM<65536> ;

public:
	void initialize (CREF<String<STR>> file) override {
		fake.mFile = move (file) ;
		fake.mFileSize = 0 ;
		fake.mFileMapFlag = 0 ;
		set_block_step (BUFFERFILE_BLOCK_STEP::expr) ;
		set_cache_size (1) ;
	}

	void set_block_step (CREF<LENGTH> step_) override {
		fake.mBlockStep = step_ ;
		fake.mChunkStep = BUFFERFILE_CHUNK_STEP::expr ;
	}

	void set_cache_size (CREF<LENGTH> size_) override {
		assert (size_ > 0) ;
		fake.mCacheSet = Set<VAL64> (size_) ;
		fake.mCacheList = List<BufferFileChunk> (size_) ;
		fake.mCacheTimer = 0 ;
	}

	void open_r () override {
		assert (!fake.mPipe.exist ()) ;
		assert (!fake.mMapping.exist ()) ;
		fake.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		fake.mFileSize = file_size (fake.mPipe) ;
		fake.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = HANDLE (fake.mFile.self) ;
		} ,[&] (VREF<HANDLE> me) {
			noop () ;
		}) ;
		fake.mMapping.depend (fake.mPipe) ;
		fake.mFileMapFlag = csc_enum_t (PROT_READ) ;
		read_header () ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!fake.mPipe.exist ()) ;
		assert (!fake.mMapping.exist ()) ;
		fake.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		const auto r3x = fake.mChunkStep / fake.mBlockStep ;
		const auto r4x = (size_ + r3x - 1) / r3x ;
		fake.mFileSize = BUFFERFILE_HEADER_STEP::expr + r4x * fake.mChunkStep ;
		fake.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r5x = ftruncate64 (fake.mPipe ,fake.mFileSize) ;
			assume (r5x == 0) ;
			me = HANDLE (fake.mFile.self) ;
		} ,[&] (VREF<HANDLE> me) {
			noop () ;
		}) ;
		fake.mMapping.depend (fake.mPipe) ;
		fake.mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		write_header () ;
	}

	void open_a () override {
		assert (!fake.mPipe.exist ()) ;
		assert (!fake.mMapping.exist ()) ;
		assume (fake.mHeader != NULL) ;
		fake.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		fake.mFileSize = fake.mHeader->mFileSize ;
		fake.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r3x = ftruncate64 (fake.mPipe ,fake.mFileSize) ;
			assume (r3x == 0) ;
			me = HANDLE (fake.mFile.self) ;
		} ,[&] (VREF<HANDLE> me) {
			noop () ;
		}) ;
		fake.mMapping.depend (fake.mPipe) ;
		fake.mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		read_header () ;
	}

	void read_header () {
		assert (fake.mHeader == NULL) ;
		fake.mHeader = Box<BufferFileHeader>::make () ;
		auto rax = ByteReader (borrow_header ()) ;
		rax >> slice ("CSC_BufferFile") ;
		rax >> GAP ;
		rax >> fake.mHeader->mFileEndian ;
		assume (fake.mHeader->mFileEndian == QUAD_ENDIAN) ;
		rax >> GAP ;
		rax >> fake.mHeader->mFileSize ;
		assume (fake.mHeader->mFileSize == fake.mFileSize) ;
		rax >> GAP ;
		rax >> fake.mHeader->mBlockSize ;
		const auto r1x = fake.mChunkStep / fake.mBlockStep ;
		assume (fake.mHeader->mBlockSize == r1x) ;
		rax >> GAP ;
		rax >> fake.mHeader->mBlockStep ;
		assume (fake.mHeader->mBlockStep == fake.mBlockStep) ;
		rax >> GAP ;
		rax >> fake.mHeader->mBlockLength ;
		rax >> GAP ;
		rax >> fake.mHeader->mChunkSize ;
		assume (fake.mHeader->mChunkSize >= 0) ;
		rax >> GAP ;
		rax >> fake.mHeader->mChunkStep ;
		assume (fake.mHeader->mChunkStep == fake.mChunkStep) ;
		rax >> GAP ;
		rax >> fake.mHeader->mChunkLength ;
		rax >> GAP ;
	}

	void write_header () {
		if ifdo (TRUE) {
			if (fake.mHeader != NULL)
				discard ;
			fake.mHeader = Box<BufferFileHeader>::make () ;
			fake.mHeader->mFileEndian = QUAD_ENDIAN ;
			fake.mHeader->mFileSize = fake.mFileSize ;
			fake.mHeader->mBlockSize = fake.mChunkStep / fake.mBlockStep ;
			fake.mHeader->mBlockStep = fake.mBlockStep ;
			fake.mHeader->mBlockLength = 0 ;
			fake.mHeader->mChunkSize = (fake.mFileSize - BUFFERFILE_HEADER_STEP::expr) / fake.mChunkStep ;
			fake.mHeader->mChunkStep = fake.mChunkStep ;
			fake.mHeader->mChunkLength = 0 ;
		}
		auto rax = ByteWriter (borrow_header ()) ;
		rax << slice ("CSC_BufferFile") ;
		rax << GAP ;
		rax << fake.mHeader->mFileEndian ;
		rax << GAP ;
		rax << fake.mHeader->mFileSize ;
		rax << GAP ;
		rax << fake.mHeader->mBlockSize ;
		rax << GAP ;
		rax << fake.mHeader->mBlockStep ;
		rax << GAP ;
		rax << fake.mHeader->mBlockLength ;
		rax << GAP ;
		rax << fake.mHeader->mChunkSize ;
		rax << GAP ;
		rax << fake.mHeader->mChunkStep ;
		rax << GAP ;
		rax << fake.mHeader->mChunkLength ;
		rax << GAP ;
		flush () ;
	}

	Ref<RefBuffer<BYTE>> borrow_header () {
		INDEX ix = mmap_cache (0 ,BUFFERFILE_HEADER_STEP::expr) ;
		const auto r1x = fake.mCacheList[ix].mBlock->m1st ;
		const auto r2x = BUFFERFILE_HEADER_STEP::expr ;
		return Ref<RefBuffer<BYTE>>::make (RefBuffer<BYTE>::reference (r1x ,r2x)) ;
	}

	LENGTH file_size () const override {
		assume (fake.mFileSize < VAL32_MAX) ;
		return LENGTH (fake.mFileSize) ;
	}

	VAL64 file_size (CREF<HFILEPIPE> handle) const {
		const auto r1x = VAL64 (lseek64 (handle ,0 ,SEEK_END)) ;
		const auto r2x = VAL64 (lseek64 (handle ,0 ,SEEK_SET)) ;
		//@warn: file in '/proc' is zero size
		if (r1x == NONE)
			return 0 ;
		if (r2x == NONE)
			return 0 ;
		assume (r1x >= 0) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) override {
		assert (fake.mPipe.exist ()) ;
		assert (inline_between (index ,0 ,LENGTH (fake.mHeader->mBlockSize))) ;
		assert (item.size () == fake.mHeader->mBlockStep) ;
		const auto r1x = index / fake.mHeader->mBlockSize ;
		const auto r2x = index % fake.mHeader->mBlockSize * fake.mHeader->mBlockStep ;
		const auto r3x = BUFFERFILE_HEADER_STEP::expr + r1x * fake.mHeader->mChunkStep ;
		INDEX ix = mmap_cache (r3x ,LENGTH (fake.mHeader->mChunkStep)) ;
		const auto r4x = fake.mCacheList[ix].mBlock->m1st + LENGTH (r2x) ;
		inline_memcpy (Pointer::from (item.self) ,Pointer::make (r4x) ,LENGTH (fake.mHeader->mBlockStep)) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) override {
		assert (fake.mPipe.exist ()) ;
		assert (inline_between (index ,0 ,LENGTH (fake.mHeader->mBlockSize))) ;
		assert (item.size () == fake.mHeader->mBlockStep) ;
		const auto r1x = index / fake.mHeader->mBlockSize ;
		const auto r2x = index % fake.mHeader->mBlockSize * fake.mHeader->mBlockStep ;
		const auto r3x = BUFFERFILE_HEADER_STEP::expr + r1x * fake.mHeader->mChunkStep ;
		INDEX ix = mmap_cache (r3x ,LENGTH (fake.mHeader->mChunkStep)) ;
		const auto r4x = fake.mCacheList[ix].mBlock->m1st + LENGTH (r2x) ;
		inline_memcpy (Pointer::make (r4x) ,Pointer::from (item.self) ,LENGTH (fake.mHeader->mBlockStep)) ;
	}

	INDEX mmap_cache (CREF<VAL64> index ,CREF<LENGTH> size_) {
		INDEX ret = fake.mCacheSet.map (index) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			update_overflow () ;
			ret = fake.mCacheList.insert () ;
			fake.mCacheSet.add (index ,ret) ;
			fake.mCacheList[ret].mIndex = index ;
			fake.mCacheList[ret].mBlock = UniqueRef<Tuple<FLAG ,FLAG>> ([&] (VREF<Tuple<FLAG ,FLAG>> me) {
				const auto r1x = mmap64 (NULL ,size_ ,fake.mFileMapFlag ,MAP_SHARED ,fake.mPipe ,index) ;
				assume (r1x != MAP_FAILED) ;
				me.m1st = FLAG (r1x) ;
				me.m2nd = me.m1st + size_ ;
			} ,[&] (VREF<Tuple<FLAG ,FLAG>> me) {
				const auto r2x = csc_pointer_t (me.m1st) ;
				const auto r3x = me.m2nd - me.m1st ;
				msync (r2x ,r3x ,MS_SYNC) ;
				munmap (r2x ,r3x) ;
			}) ;
		}
		fake.mCacheList[ret].mCacheTime = fake.mCacheTimer ;
		fake.mCacheTimer++ ;
		if ifdo (TRUE) {
			if (fake.mCacheTimer < VAL32_MAX)
				discard ;
			for (auto &&i : fake.mCacheList.range ())
				fake.mCacheList[i].mCacheTime = 0 ;
			fake.mCacheList[ret].mCacheTime = 1 ;
			fake.mCacheTimer = 2 ;
		}
		return move (ret) ;
	}

	void update_overflow () {
		if (!fake.mCacheList.full ())
			return ;
		const auto r1x = invoke ([&] () {
			INDEX ret = NONE ;
			auto rax = VAL64 () ;
			for (auto &&i : fake.mCacheList.range ()) {
				if (ret != NONE)
					if (rax >= fake.mCacheList[i].mCacheTime)
						continue ;
				ret = i ;
				rax = fake.mCacheList[i].mCacheTime ;
			}
			return move (ret) ;
		}) ;
		assert (r1x != NONE) ;
		fake.mCacheSet.erase (fake.mCacheList[r1x].mIndex) ;
		fake.mCacheList.remove (r1x) ;
	}

	void flush () override {
		if (!fake.mPipe.exist ())
			return ;
		fake.mCacheSet.clear () ;
		fake.mCacheList.clear () ;
		fake.mCacheTimer = 0 ;
		fsync (fake.mPipe) ;
	}
} ;

static const auto mBufferFileExternal = External<BufferFileHolder ,BufferFileLayout> (BufferFileImplHolder ()) ;

struct UartFileImplLayout {
	String<STR> mPortName ;
	LENGTH mPortRate ;
	UniqueRef<HFILEPIPE> mPipe ;
	TERMIOS_INFO mSerialStat ;
	RefBuffer<BYTE> mRingBuffer ;
	INDEX mRingRead ;
} ;

class UartFileImplHolder final implement Fat<UartFileHolder ,UartFileImplLayout> {
private:
	void initialize () override {
		fake.mPortRate = 0 ;
	}

	void set_port_name (CREF<String<STR>> name) override {
		fake.mPortName = name ;
	}

	void set_port_rate (CREF<LENGTH> rate) override {
		fake.mPortRate = rate ;
	}

	void set_ring_size (CREF<LENGTH> size_) override {
		fake.mRingBuffer = RefBuffer<BYTE> (size_) ;
		fake.mRingRead = 0 ;
	}

	void open () override {
		assert (fake.mPortName.length () > 0) ;
		assert (fake.mRingBuffer.size () > 0) ;
		fake.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_RDWR | O_NOCTTY | O_SYNC) ;
			me = std::open (fake.mPortName ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = tcgetattr (fake.mPipe ,(&fake.mSerialStat)) ;
		assume (r2x != 0) ;
		cfsetospeed ((&fake.mSerialStat) ,VAL32 (fake.mPortRate)) ;
		cfsetispeed ((&fake.mSerialStat) ,VAL32 (fake.mPortRate)) ;
		fake.mSerialStat.c_cflag = (fake.mSerialStat.c_cflag & ~CSIZE) | CS8 ;
		fake.mSerialStat.c_iflag &= ~IGNBRK ;
		fake.mSerialStat.c_lflag = 0 ;
		fake.mSerialStat.c_oflag = 0 ;
		fake.mSerialStat.c_cc[VMIN] = 0 ;
		fake.mSerialStat.c_cc[VTIME] = 5 ;
		fake.mSerialStat.c_iflag &= ~(IXON | IXOFF | IXANY) ;
		fake.mSerialStat.c_cflag |= (CLOCAL | CREAD) ;
		fake.mSerialStat.c_cflag &= ~(PARENB | PARODD) ;
		fake.mSerialStat.c_cflag &= ~CSTOPB ;
		fake.mSerialStat.c_cflag &= ~CRTSCTS ;
		const auto r3x = tcsetattr (fake.mPipe ,TCSANOW ,(&fake.mSerialStat)) ;
		assume (r3x != 0) ;
	}

	void read (VREF<RefBuffer<BYTE>> buffer ,CREF<INDEX> offset ,CREF<LENGTH> size_) override {
		for (auto &&i : iter (0 ,size_)) {
			buffer[offset + i] = fake.mRingBuffer[fake.mRingRead] ;
			fake.mRingRead++ ;
			if ifdo (TRUE) {
				if (fake.mRingRead < fake.mRingBuffer.size ())
					discard ;
				auto rax = fake.mRingBuffer.size () ;
				while (TRUE) {
					auto rbx = csc_size_t (rax) ;
					rbx = std::read (fake.mPipe ,fake.mRingBuffer ,rbx) ;
					assume (rbx >= 0) ;
					rax -= rbx ;
					if (rax == 0)
						break ;
					RuntimeProc::thread_yield () ;
				}
				fake.mRingRead = 0 ;
			}
		}
	}
} ;

static const auto mUartFileExternal = External<UartFileHolder ,UartFileLayout> (UartFileImplHolder ()) ;

struct ConsoleImplLayout {
	Mutex mMutex ;
	BitSet mOption ;
	UniqueRef<HANDLE> mConsole ;
	String<STR> mLogBuffer ;
	TextWriter mLogWriter ;
	String<STR> mLogFile ;
	String<STR> mOldLogFile ;
	StreamFile mLogStreamFile ;
	System mCommand ;
} ;

class ConsoleImplHolder final implement Fat<ConsoleHolder ,ConsoleImplLayout> {
public:
	void initialize () override {
		fake.mMutex = NULL ;
		fake.mOption = BitSet (ConsoleOption::ETC) ;
		fake.mLogBuffer = String<STR> (STREAMFILE_CHUNK_STEP::expr) ;
		fake.mLogWriter = TextWriter (fake.mLogBuffer.borrow ()) ;
		fake.mCommand = NULL ;
	}

	void set_option (CREF<Just<ConsoleOption>> option) override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (option != ConsoleOption::All)
				discard ;
			fake.mOption.clear () ;
		}
		if ifdo (act) {
			fake.mOption.add (option) ;
		}
	}

	void log (CREF<String<STR>> tag ,CREF<Format> msg) {
		fake.mLogWriter << CLS ;
		fake.mLogWriter << slice ("[") ;
		const auto r1x = CurrentTime () ;
		const auto r2x = r1x.calendar () ;
		fake.mLogWriter << AlignedText (r2x.mHour ,2) ;
		fake.mLogWriter << slice (":") ;
		fake.mLogWriter << AlignedText (r2x.mMinute ,2) ;
		fake.mLogWriter << slice (":") ;
		fake.mLogWriter << AlignedText (r2x.mSecond ,2) ;
		fake.mLogWriter << slice ("][") ;
		fake.mLogWriter << tag ;
		fake.mLogWriter << slice ("] : ") ;
		fake.mLogWriter << msg ;
		fake.mLogWriter << GAP ;
		fake.mLogWriter << EOS ;
	}

	void print (CREF<Format> msg) override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		if (fake.mOption[ConsoleOption::NoPrint])
			return ;
		fake.mLogWriter << CLS ;
		fake.mLogWriter << msg ;
		fake.mLogWriter << EOS ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("%s")) ;
			std::printf (r1x.self ,fake.mLogBuffer.self) ;
		}
	}

	void fatal (CREF<Format> msg) override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		if (fake.mOption[ConsoleOption::NoFatal])
			return ;
		log (slice ("Fatal") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;34m%s\033[0m")) ;
			std::printf (r1x.self ,fake.mLogBuffer.self) ;
		}
	}

	void error (CREF<Format> msg) override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		if (fake.mOption[ConsoleOption::NoError])
			return ;
		log (slice ("Error") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;31m%s\033[0m")) ;
			std::printf (r1x.self ,fake.mLogBuffer.self) ;
		}
	}

	void warn (CREF<Format> msg) override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		if (fake.mOption[ConsoleOption::NoWarn])
			return ;
		log (slice ("Warn") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;33m%s\033[0m")) ;
			std::printf (r1x.self ,fake.mLogBuffer.self) ;
		}
	}

	void info (CREF<Format> msg) override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		if (fake.mOption[ConsoleOption::NoInfo])
			return ;
		log (slice ("Info") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;32m%s\033[0m")) ;
			std::printf (r1x.self ,fake.mLogBuffer.self) ;
		}
	}

	void debug (CREF<Format> msg) override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		if (fake.mOption[ConsoleOption::NoDebug])
			return ;
		log (slice ("Debug") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;36m%s\033[0m")) ;
			std::printf (r1x.self ,fake.mLogBuffer.self) ;
		}
	}

	void trace (CREF<Format> msg) override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		if (fake.mOption[ConsoleOption::NoTrace])
			return ;
		log (slice ("Trace") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;37m%s\033[0m")) ;
			std::printf (r1x.self ,fake.mLogBuffer.self) ;
		}
	}

	void open (CREF<String<STR>> dire) override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		fake.mLogFile = Path (dire).child (slice ("console.log")) ;
		fake.mOldLogFile = Path (dire).child (slice ("console.old.log")) ;
		FileProc::erase_file (fake.mOldLogFile) ;
		FileProc::move_file (fake.mOldLogFile ,fake.mLogFile) ;
		fake.mLogStreamFile = StreamFile (fake.mLogFile) ;
		fake.mLogStreamFile.open_w (0) ;
		fake.mLogWriter << CLS ;
		fake.mLogWriter << BOM ;
		fake.mLogWriter << EOS ;
		log_file () ;
	}

	void log_file () {
		if (fake.mLogFile.length () == 0)
			return ;
		const auto r1x = FLAG (fake.mLogBuffer.self) ;
		const auto r2x = (fake.mLogWriter.length () - 1) * SIZE_OF<STR>::expr ;
		fake.mLogStreamFile.write (RefBuffer<BYTE>::reference (r1x ,r2x)) ;
	}

	void show () override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		if (fake.mConsole.exist ())
			return ;
		fake.mConsole = UniqueRef<HANDLE>::make (stderr) ;
	}

	void hide () override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		fake.mConsole = UniqueRef<HANDLE>::make () ;
	}

	void pause () override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		const auto r1x = String<STR> (slice ("%s\n")) ;
		const auto r2x = String<STR> (slice ("press any key to continue...")) ;
		std::printf (r1x.self ,r2x.self) ;
		const auto r3x = std::getchar () ;
		noop (r3x) ;
	}

	void clear () override {
		Scope<Mutex> anonymous (fake.mMutex) ;
		fake.mCommand.execute (slice ("clear")) ;
	}
} ;

static const auto mConsoleExternal = External<ConsoleHolder ,ConsoleLayout> (ConsoleImplHolder ()) ;
} ;