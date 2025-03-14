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
struct PathTree {
	String<STR> mPathName ;
	Deque<INDEX> mSeparator ;
} ;

class PathImplHolder final implement Fat<PathHolder ,PathLayout> {
public:
	void initialize (RREF<String<STR>> pathname) override {
		self.mThis = Ref<PathTree>::make () ;
		self.mThis->mPathName = move (pathname) ;
		self.mThis->mSeparator.add (NONE) ;
		const auto r1x = self.mThis->mPathName.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			if (!is_separator (self.mThis->mPathName[i]))
				continue ;
			self.mThis->mSeparator.add (i) ;
			self.mThis->mPathName[i] = STR ('/') ;
		}
		self.mThis->mSeparator.add (r1x) ;
		if ifdo (TRUE) {
			if (r1x == 0)
				discard ;
			INDEX ix = self.mThis->mSeparator[self.mThis->mSeparator.length () - 2] ;
			if (ix != r1x - 1)
				discard ;
			self.mThis->mPathName.trunc (ix) ;
			self.mThis->mSeparator.pop () ;
		}
		if ifdo (TRUE) {
			if (self.mThis->mSeparator.length () != 2)
				discard ;
			INDEX ix = self.mThis->mSeparator[0] + 1 ;
			INDEX iy = self.mThis->mSeparator[1] ;
			if (!is_root (self.mThis->mPathName.segment (ix ,iy)))
				discard ;
			self.mThis->mPathName = String<STR>::make (self.mThis->mPathName ,slice ("/") ,slice (".")) ;
			self.mThis->mSeparator.add (iy + 2) ;
		}
		assume (self.mThis->mSeparator.length () >= 2) ;
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
		if (self.mThis == NULL)
			return String<STR>::zero () ;
		return self.mThis->mPathName ;
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
			me = opendir (self.mThis->mPathName) ;
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
			me = opendir (self.mThis->mPathName) ;
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
		const auto r1x = inline_compr (self.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!self.mThis.exist ())
			return FALSE ;
		return self.mThis->mPathName == that.mThis->mPathName ;
	}

	BOOL is_file () const override {
		if (self.mThis == NULL)
			return FALSE ;
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = std::stat (self.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	BOOL is_dire () const override {
		if (self.mThis == NULL)
			return FALSE ;
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = std::stat (self.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (!S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	BOOL is_link () const override {
		if (self.mThis == NULL)
			return FALSE ;
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = std::lstat (self.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (!S_ISLNK (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	PathLayout symbolic () const override {
		PathLayout ret = self ;
		if ifdo (TRUE) {
			if (!is_link ())
				discard ;
			auto rax = String<STR>::make () ;
			const auto r1x = INDEX (readlink (self.mThis->mPathName ,rax ,csc_size_t (rax.size ()))) ;
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
		const auto r1x = self.mThis->mSeparator.length () ;
		Deque<String<STR>> ret = Deque<String<STR>> (r1x) ;
		for (auto &&i : iter (0 ,r1x - 1)) {
			INDEX ix = self.mThis->mSeparator[i] + 1 ;
			INDEX iy = self.mThis->mSeparator[i + 1] ;
			const auto r2x = self.mThis->mPathName.segment (ix ,iy) ;
			ret.add (r2x) ;
		}
		return move (ret) ;
	}

	String<STR> path () const override {
		const auto r1x = self.mThis->mSeparator.length () ;
		INDEX ix = self.mThis->mSeparator[0] + 1 ;
		INDEX iy = self.mThis->mSeparator[r1x - 2] + 1 ;
		return self.mThis->mPathName.segment (ix ,iy) ;
	}

	String<STR> name () const override {
		const auto r1x = self.mThis->mSeparator.length () ;
		INDEX ix = self.mThis->mSeparator[r1x - 2] + 1 ;
		INDEX iy = self.mThis->mSeparator[r1x - 1] ;
		return self.mThis->mPathName.segment (ix ,iy) ;
	}

	String<STR> stem () const override {
		const auto r1x = self.mThis->mSeparator.length () ;
		INDEX ix = self.mThis->mSeparator[r1x - 2] + 1 ;
		INDEX iy = find_last_dot_word () ;
		return self.mThis->mPathName.segment (ix ,iy) ;
	}

	String<STR> extension () const override {
		const auto r1x = self.mThis->mSeparator.length () ;
		INDEX ix = find_last_dot_word () ;
		INDEX iy = self.mThis->mSeparator[r1x - 1] ;
		return self.mThis->mPathName.segment (ix ,iy) ;
	}

	INDEX find_last_dot_word () const {
		const auto r1x = self.mThis->mSeparator.length () ;
		INDEX ix = self.mThis->mSeparator[r1x - 1] - 1 ;
		while (TRUE) {
			if (ix <= 0)
				break ;
			if (self.mThis->mPathName[ix] == STRU32 ('.'))
				return ix ;
			if (!StreamProc::is_word (self.mThis->mPathName[ix]))
				break ;
			ix-- ;
		}
		return self.mThis->mSeparator[r1x - 1] ;
	}
} ;

static const auto mPathExternal = External<PathHolder ,PathLayout> (PathImplHolder ()) ;

struct FileProcLayout {
	Mutex mMutex ;
	Pin<List<UniqueRef<String<STR>>>> mLockDirectory ;
} ;

class FileProcImplHolder final implement Fat<FileProcHolder ,FileProcLayout> {
private:
	using FILEPROC_RETRY_TIME = RANK3 ;

public:
	void create (VREF<UniqueRef<FileProcLayout>> that) const override {
		that = UniqueRef<FileProcLayout>::make () ;
	}

	void initialize () override {
		self.mMutex = NULL ;
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
		Scope<Mutex> anonymous (self.mMutex) ;
		auto rax = UniqueRef<String<STR>> ([&] (VREF<String<STR>> me) {
			me = file ;
			FileProc::save_file (me ,snapshot_) ;
		} ,[&] (VREF<String<STR>> me) {
			FileProc::erase_file (me) ;
		}) ;
		auto rbx = List<UniqueRef<String<STR>>> () ;
		self.mLockDirectory.get (rbx) ;
		rbx.add (move (rax)) ;
		self.mLockDirectory.set (rbx) ;
	}
} ;

static const auto mFileProcExternal = External<FileProcHolder ,FileProcLayout> (FileProcImplHolder ()) ;

struct StreamFileLayout {
	String<STR> mFile ;
	UniqueRef<HFILEPIPE> mReadPipe ;
	UniqueRef<HFILEPIPE> mWritePipe ;
	VAL64 mFileSize ;
	VAL64 mRead ;
	VAL64 mWrite ;
	BOOL mShortRead ;
	LENGTH mShortSize ;
} ;

class StreamFileImplHolder final implement Fat<StreamFileHolder ,StreamFileLayout> {
public:
	void create (VREF<AutoRef<StreamFileLayout>> that) const override {
		that = AutoRef<StreamFileLayout>::make () ;
	}

	void initialize (CREF<String<STR>> file) override {
		self.mFile = move (file) ;
		self.mFileSize = 0 ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
		self.mShortRead = FALSE ;
		self.mShortSize = 0 ;
	}

	void set_short_read (CREF<BOOL> flag) override {
		self.mShortRead = flag ;
	}

	void open_r () override {
		assert (!self.mReadPipe.exist ()) ;
		assert (!self.mWritePipe.exist ()) ;
		self.mReadPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = file_size (self.mReadPipe) ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!self.mReadPipe.exist ()) ;
		assert (!self.mWritePipe.exist ()) ;
		self.mWritePipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = size_ ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
	}

	void open_a () override {
		assert (!self.mReadPipe.exist ()) ;
		assert (!self.mWritePipe.exist ()) ;
		self.mReadPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDONLY) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		self.mWritePipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = file_size (self.mReadPipe) ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
		if ifdo (TRUE) {
			const auto r5x = VAL64 (lseek64 (self.mWritePipe ,0 ,SEEK_END)) ;
			if (r5x <= 0)
				discard ;
			self.mWrite += r5x ;
		}
	}

	LENGTH file_size () const override {
		assume (self.mFileSize < VAL32_MAX) ;
		return LENGTH (self.mFileSize) ;
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
		return self.mShortSize ;
	}

	void read (VREF<RefBuffer<BYTE>> item) override {
		assert (self.mReadPipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			auto rbx = csc_size_t (rax) ;
			rbx = std::read (self.mReadPipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				discard ;
			assume (self.mShortRead) ;
		}
		self.mShortSize = r1x - rax ;
		self.mRead += self.mShortSize ;
	}

	void write (CREF<RefBuffer<BYTE>> item) override {
		assert (self.mWritePipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (self.mWritePipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				discard ;
			assume (self.mShortRead) ;
		}
		self.mShortSize = r1x - rax ;
		self.mWrite += self.mShortSize ;
	}

	void flush () override {
		if (!self.mWritePipe.exist ())
			return ;
		fsync (self.mWritePipe) ;
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

struct BufferFileLayout {
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

class BufferFileImplHolder final implement Fat<BufferFileHolder ,BufferFileLayout> {
private:
	using BUFFERFILE_BLOCK_STEP = ENUM<1024> ;
	using BUFFERFILE_CHUNK_STEP = ENUM<4194304> ;
	using BUFFERFILE_HEADER_STEP = ENUM<65536> ;

public:
	void create (VREF<AutoRef<BufferFileLayout>> that) const override {
		that = AutoRef<BufferFileLayout>::make () ;
	}

	void initialize (CREF<String<STR>> file) override {
		self.mFile = move (file) ;
		self.mFileSize = 0 ;
		self.mFileMapFlag = 0 ;
		set_block_step (BUFFERFILE_BLOCK_STEP::expr) ;
		set_cache_size (1) ;
	}

	void set_block_step (CREF<LENGTH> step_) override {
		self.mBlockStep = step_ ;
		self.mChunkStep = BUFFERFILE_CHUNK_STEP::expr ;
	}

	void set_cache_size (CREF<LENGTH> size_) override {
		assert (size_ > 0) ;
		self.mCacheSet = Set<VAL64> (size_) ;
		self.mCacheList = List<BufferFileChunk> (size_) ;
		self.mCacheTimer = 0 ;
	}

	void open_r () override {
		assert (!self.mPipe.exist ()) ;
		assert (!self.mMapping.exist ()) ;
		self.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = file_size (self.mPipe) ;
		self.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = HANDLE (self.mFile.deref) ;
		} ,[&] (VREF<HANDLE> me) {
			noop () ;
		}) ;
		self.mMapping.depend (self.mPipe) ;
		self.mFileMapFlag = csc_enum_t (PROT_READ) ;
		read_header () ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!self.mPipe.exist ()) ;
		assert (!self.mMapping.exist ()) ;
		self.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		const auto r3x = self.mChunkStep / self.mBlockStep ;
		const auto r4x = (size_ + r3x - 1) / r3x ;
		self.mFileSize = BUFFERFILE_HEADER_STEP::expr + r4x * self.mChunkStep ;
		self.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r5x = ftruncate64 (self.mPipe ,self.mFileSize) ;
			assume (r5x == 0) ;
			me = HANDLE (self.mFile.deref) ;
		} ,[&] (VREF<HANDLE> me) {
			noop () ;
		}) ;
		self.mMapping.depend (self.mPipe) ;
		self.mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		write_header () ;
	}

	void open_a () override {
		assert (!self.mPipe.exist ()) ;
		assert (!self.mMapping.exist ()) ;
		assume (self.mHeader != NULL) ;
		self.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = self.mHeader->mFileSize ;
		self.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r3x = ftruncate64 (self.mPipe ,self.mFileSize) ;
			assume (r3x == 0) ;
			me = HANDLE (self.mFile.deref) ;
		} ,[&] (VREF<HANDLE> me) {
			noop () ;
		}) ;
		self.mMapping.depend (self.mPipe) ;
		self.mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		read_header () ;
	}

	void read_header () {
		assert (self.mHeader == NULL) ;
		self.mHeader = Box<BufferFileHeader>::make () ;
		auto rax = ByteReader (borrow_header ()) ;
		rax >> slice ("CSC_BufferFile") ;
		rax >> GAP ;
		rax >> self.mHeader->mFileEndian ;
		assume (self.mHeader->mFileEndian == QUAD_ENDIAN) ;
		rax >> GAP ;
		rax >> self.mHeader->mFileSize ;
		assume (self.mHeader->mFileSize == self.mFileSize) ;
		rax >> GAP ;
		rax >> self.mHeader->mBlockSize ;
		const auto r1x = self.mChunkStep / self.mBlockStep ;
		assume (self.mHeader->mBlockSize == r1x) ;
		rax >> GAP ;
		rax >> self.mHeader->mBlockStep ;
		assume (self.mHeader->mBlockStep == self.mBlockStep) ;
		rax >> GAP ;
		rax >> self.mHeader->mBlockLength ;
		rax >> GAP ;
		rax >> self.mHeader->mChunkSize ;
		assume (self.mHeader->mChunkSize >= 0) ;
		rax >> GAP ;
		rax >> self.mHeader->mChunkStep ;
		assume (self.mHeader->mChunkStep == self.mChunkStep) ;
		rax >> GAP ;
		rax >> self.mHeader->mChunkLength ;
		rax >> GAP ;
	}

	void write_header () {
		if ifdo (TRUE) {
			if (self.mHeader != NULL)
				discard ;
			self.mHeader = Box<BufferFileHeader>::make () ;
			self.mHeader->mFileEndian = QUAD_ENDIAN ;
			self.mHeader->mFileSize = self.mFileSize ;
			self.mHeader->mBlockSize = self.mChunkStep / self.mBlockStep ;
			self.mHeader->mBlockStep = self.mBlockStep ;
			self.mHeader->mBlockLength = 0 ;
			self.mHeader->mChunkSize = (self.mFileSize - BUFFERFILE_HEADER_STEP::expr) / self.mChunkStep ;
			self.mHeader->mChunkStep = self.mChunkStep ;
			self.mHeader->mChunkLength = 0 ;
		}
		auto rax = ByteWriter (borrow_header ()) ;
		rax << slice ("CSC_BufferFile") ;
		rax << GAP ;
		rax << self.mHeader->mFileEndian ;
		rax << GAP ;
		rax << self.mHeader->mFileSize ;
		rax << GAP ;
		rax << self.mHeader->mBlockSize ;
		rax << GAP ;
		rax << self.mHeader->mBlockStep ;
		rax << GAP ;
		rax << self.mHeader->mBlockLength ;
		rax << GAP ;
		rax << self.mHeader->mChunkSize ;
		rax << GAP ;
		rax << self.mHeader->mChunkStep ;
		rax << GAP ;
		rax << self.mHeader->mChunkLength ;
		rax << GAP ;
		flush () ;
	}

	Ref<RefBuffer<BYTE>> borrow_header () {
		INDEX ix = mmap_cache (0 ,BUFFERFILE_HEADER_STEP::expr) ;
		const auto r1x = self.mCacheList[ix].mBlock->m1st ;
		const auto r2x = BUFFERFILE_HEADER_STEP::expr ;
		return Ref<RefBuffer<BYTE>>::make (RefBuffer<BYTE>::reference (r1x ,r2x)) ;
	}

	LENGTH file_size () const override {
		assume (self.mFileSize < VAL32_MAX) ;
		return LENGTH (self.mFileSize) ;
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
		assert (self.mPipe.exist ()) ;
		assert (inline_between (index ,0 ,LENGTH (self.mHeader->mBlockSize))) ;
		assert (item.size () == self.mHeader->mBlockStep) ;
		const auto r1x = index / self.mHeader->mBlockSize ;
		const auto r2x = index % self.mHeader->mBlockSize * self.mHeader->mBlockStep ;
		const auto r3x = BUFFERFILE_HEADER_STEP::expr + r1x * self.mHeader->mChunkStep ;
		INDEX ix = mmap_cache (r3x ,LENGTH (self.mHeader->mChunkStep)) ;
		const auto r4x = self.mCacheList[ix].mBlock->m1st + LENGTH (r2x) ;
		inline_memcpy (Pointer::from (item.deref) ,Pointer::make (r4x) ,LENGTH (self.mHeader->mBlockStep)) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) override {
		assert (self.mPipe.exist ()) ;
		assert (inline_between (index ,0 ,LENGTH (self.mHeader->mBlockSize))) ;
		assert (item.size () == self.mHeader->mBlockStep) ;
		const auto r1x = index / self.mHeader->mBlockSize ;
		const auto r2x = index % self.mHeader->mBlockSize * self.mHeader->mBlockStep ;
		const auto r3x = BUFFERFILE_HEADER_STEP::expr + r1x * self.mHeader->mChunkStep ;
		INDEX ix = mmap_cache (r3x ,LENGTH (self.mHeader->mChunkStep)) ;
		const auto r4x = self.mCacheList[ix].mBlock->m1st + LENGTH (r2x) ;
		inline_memcpy (Pointer::make (r4x) ,Pointer::from (item.deref) ,LENGTH (self.mHeader->mBlockStep)) ;
	}

	INDEX mmap_cache (CREF<VAL64> index ,CREF<LENGTH> size_) {
		INDEX ret = self.mCacheSet.map (index) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			update_overflow () ;
			ret = self.mCacheList.insert () ;
			self.mCacheSet.add (index ,ret) ;
			self.mCacheList[ret].mIndex = index ;
			self.mCacheList[ret].mBlock = UniqueRef<Tuple<FLAG ,FLAG>> ([&] (VREF<Tuple<FLAG ,FLAG>> me) {
				const auto r1x = mmap64 (NULL ,size_ ,self.mFileMapFlag ,MAP_SHARED ,self.mPipe ,index) ;
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
		self.mCacheList[ret].mCacheTime = self.mCacheTimer ;
		self.mCacheTimer++ ;
		if ifdo (TRUE) {
			if (self.mCacheTimer < VAL32_MAX)
				discard ;
			for (auto &&i : self.mCacheList.range ())
				self.mCacheList[i].mCacheTime = 0 ;
			self.mCacheList[ret].mCacheTime = 1 ;
			self.mCacheTimer = 2 ;
		}
		return move (ret) ;
	}

	void update_overflow () {
		if (!self.mCacheList.full ())
			return ;
		const auto r1x = invoke ([&] () {
			INDEX ret = NONE ;
			auto rax = VAL64 () ;
			for (auto &&i : self.mCacheList.range ()) {
				if (ret != NONE)
					if (rax >= self.mCacheList[i].mCacheTime)
						continue ;
				ret = i ;
				rax = self.mCacheList[i].mCacheTime ;
			}
			return move (ret) ;
		}) ;
		assert (r1x != NONE) ;
		self.mCacheSet.erase (self.mCacheList[r1x].mIndex) ;
		self.mCacheList.remove (r1x) ;
	}

	void flush () override {
		if (!self.mPipe.exist ())
			return ;
		self.mCacheSet.clear () ;
		self.mCacheList.clear () ;
		self.mCacheTimer = 0 ;
		fsync (self.mPipe) ;
	}
} ;

static const auto mBufferFileExternal = External<BufferFileHolder ,BufferFileLayout> (BufferFileImplHolder ()) ;

struct UartFileLayout {
	String<STR> mPortName ;
	LENGTH mPortRate ;
	UniqueRef<HFILEPIPE> mPipe ;
	TERMIOS_INFO mSerialStat ;
	RefBuffer<BYTE> mRingBuffer ;
	INDEX mRingRead ;
} ;

class UartFileImplHolder final implement Fat<UartFileHolder ,UartFileLayout> {
private:
	void create (VREF<AutoRef<UartFileLayout>> that) const override {
		that = AutoRef<UartFileLayout>::make () ;
	}

	void initialize () override {
		self.mPortRate = 0 ;
	}

	void set_port_name (CREF<String<STR>> name) override {
		self.mPortName = name ;
	}

	void set_port_rate (CREF<LENGTH> rate) override {
		self.mPortRate = rate ;
	}

	void set_ring_size (CREF<LENGTH> size_) override {
		self.mRingBuffer = RefBuffer<BYTE> (size_) ;
		self.mRingRead = 0 ;
	}

	void open () override {
		assert (self.mPortName.length () > 0) ;
		assert (self.mRingBuffer.size () > 0) ;
		self.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (O_RDWR | O_NOCTTY | O_SYNC) ;
			me = std::open (self.mPortName ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = tcgetattr (self.mPipe ,(&self.mSerialStat)) ;
		assume (r2x != 0) ;
		cfsetospeed ((&self.mSerialStat) ,VAL32 (self.mPortRate)) ;
		cfsetispeed ((&self.mSerialStat) ,VAL32 (self.mPortRate)) ;
		self.mSerialStat.c_cflag = (self.mSerialStat.c_cflag & ~CSIZE) | CS8 ;
		self.mSerialStat.c_iflag &= ~IGNBRK ;
		self.mSerialStat.c_lflag = 0 ;
		self.mSerialStat.c_oflag = 0 ;
		self.mSerialStat.c_cc[VMIN] = 0 ;
		self.mSerialStat.c_cc[VTIME] = 5 ;
		self.mSerialStat.c_iflag &= ~(IXON | IXOFF | IXANY) ;
		self.mSerialStat.c_cflag |= (CLOCAL | CREAD) ;
		self.mSerialStat.c_cflag &= ~(PARENB | PARODD) ;
		self.mSerialStat.c_cflag &= ~CSTOPB ;
		self.mSerialStat.c_cflag &= ~CRTSCTS ;
		const auto r3x = tcsetattr (self.mPipe ,TCSANOW ,(&self.mSerialStat)) ;
		assume (r3x != 0) ;
	}

	void read (VREF<RefBuffer<BYTE>> buffer ,CREF<INDEX> offset ,CREF<LENGTH> size_) override {
		for (auto &&i : iter (0 ,size_)) {
			buffer[offset + i] = self.mRingBuffer[self.mRingRead] ;
			self.mRingRead++ ;
			if ifdo (TRUE) {
				if (self.mRingRead < self.mRingBuffer.size ())
					discard ;
				auto rax = self.mRingBuffer.size () ;
				while (TRUE) {
					auto rbx = csc_size_t (rax) ;
					rbx = std::read (self.mPipe ,self.mRingBuffer ,rbx) ;
					assume (rbx >= 0) ;
					rax -= rbx ;
					if (rax == 0)
						break ;
					RuntimeProc::thread_yield () ;
				}
				self.mRingRead = 0 ;
			}
		}
	}
} ;

static const auto mUartFileExternal = External<UartFileHolder ,UartFileLayout> (UartFileImplHolder ()) ;

struct ConsoleLayout {
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

class ConsoleImplHolder final implement Fat<ConsoleHolder ,ConsoleLayout> {
public:
	void create (VREF<SharedRef<ConsoleLayout>> that) const override {
		that = SharedRef<ConsoleLayout>::make () ;
	}

	void initialize () override {
		self.mMutex = NULL ;
		self.mOption = BitSet (ConsoleOption::ETC) ;
		self.mLogBuffer = String<STR> (STREAMFILE_CHUNK_STEP::expr) ;
		self.mLogWriter = TextWriter (self.mLogBuffer.borrow ()) ;
		self.mCommand = NULL ;
	}

	void set_option (CREF<Just<ConsoleOption>> option) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (option != ConsoleOption::All)
				discard ;
			self.mOption.clear () ;
		}
		if ifdo (act) {
			self.mOption.add (option) ;
		}
	}

	void log (CREF<String<STR>> tag ,CREF<Format> msg) {
		self.mLogWriter << CLS ;
		self.mLogWriter << slice ("[") ;
		const auto r1x = CurrentTime () ;
		const auto r2x = r1x.calendar () ;
		self.mLogWriter << AlignedText (r2x.mHour ,2) ;
		self.mLogWriter << slice (":") ;
		self.mLogWriter << AlignedText (r2x.mMinute ,2) ;
		self.mLogWriter << slice (":") ;
		self.mLogWriter << AlignedText (r2x.mSecond ,2) ;
		self.mLogWriter << slice ("][") ;
		self.mLogWriter << tag ;
		self.mLogWriter << slice ("] : ") ;
		self.mLogWriter << msg ;
		self.mLogWriter << GAP ;
		self.mLogWriter << EOS ;
	}

	void print (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoPrint])
			return ;
		self.mLogWriter << CLS ;
		self.mLogWriter << msg ;
		self.mLogWriter << EOS ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("%s")) ;
			std::printf (r1x.deref ,self.mLogBuffer.deref) ;
		}
	}

	void fatal (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoFatal])
			return ;
		log (slice ("Fatal") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;34m%s\033[0m")) ;
			std::printf (r1x.deref ,self.mLogBuffer.deref) ;
		}
	}

	void error (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoError])
			return ;
		log (slice ("Error") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;31m%s\033[0m")) ;
			std::printf (r1x.deref ,self.mLogBuffer.deref) ;
		}
	}

	void warn (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoWarn])
			return ;
		log (slice ("Warn") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;33m%s\033[0m")) ;
			std::printf (r1x.deref ,self.mLogBuffer.deref) ;
		}
	}

	void info (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoInfo])
			return ;
		log (slice ("Info") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;32m%s\033[0m")) ;
			std::printf (r1x.deref ,self.mLogBuffer.deref) ;
		}
	}

	void debug (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoDebug])
			return ;
		log (slice ("Debug") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;36m%s\033[0m")) ;
			std::printf (r1x.deref ,self.mLogBuffer.deref) ;
		}
	}

	void trace (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoTrace])
			return ;
		log (slice ("Trace") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;37m%s\033[0m")) ;
			std::printf (r1x.deref ,self.mLogBuffer.deref) ;
		}
	}

	void open (CREF<String<STR>> dire) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		self.mLogFile = Path (dire).child (slice ("console.log")) ;
		self.mOldLogFile = Path (dire).child (slice ("console.old.log")) ;
		FileProc::erase_file (self.mOldLogFile) ;
		FileProc::move_file (self.mOldLogFile ,self.mLogFile) ;
		self.mLogStreamFile = StreamFile (self.mLogFile) ;
		self.mLogStreamFile.open_w (0) ;
		self.mLogWriter << CLS ;
		self.mLogWriter << BOM ;
		self.mLogWriter << EOS ;
		log_file () ;
	}

	void log_file () {
		if (self.mLogFile.length () == 0)
			return ;
		const auto r1x = FLAG (self.mLogBuffer.deref) ;
		const auto r2x = (self.mLogWriter.length () - 1) * SIZE_OF<STR>::expr ;
		self.mLogStreamFile.write (RefBuffer<BYTE>::reference (r1x ,r2x)) ;
	}

	void show () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mConsole.exist ())
			return ;
		self.mConsole = UniqueRef<HANDLE>::make (stderr) ;
	}

	void hide () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		self.mConsole = UniqueRef<HANDLE>::make () ;
	}

	void pause () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		const auto r1x = String<STR> (slice ("%s\n")) ;
		const auto r2x = String<STR> (slice ("press any key to continue...")) ;
		std::printf (r1x.deref ,r2x.deref) ;
		const auto r3x = std::getchar () ;
		noop (r3x) ;
	}

	void clear () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		self.mCommand.execute (slice ("clear")) ;
	}
} ;

static const auto mConsoleExternal = External<ConsoleHolder ,ConsoleLayout> (ConsoleImplHolder ()) ;
} ;