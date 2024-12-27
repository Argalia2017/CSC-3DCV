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
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <termios.h>

#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include "csc_begin.h"

#include "csc_end.h"
#include <cstdio>
#include "csc_begin.h"

inline namespace {
using HMODULE = CSC::csc_pointer_t ;
using HANDLE = CSC::csc_pointer_t ;
using HFILE = int ;
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
		auto rax = PathImplLayout () ;
		rax.mPathName = move (pathname) ;
		rax.mSeparator.add (NONE) ;
		const auto r1x = rax.mPathName.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			if (!is_separator (rax.mPathName[i]))
				continue ;
			rax.mSeparator.add (i) ;
			rax.mPathName[i] = STR ('/') ;
		}
		rax.mSeparator.add (r1x) ;
		if ifdo (TRUE) {
			if (r1x == 0)
				discard ;
			INDEX ix = rax.mSeparator[rax.mSeparator.length () - 2] ;
			if (ix != r1x - 1)
				discard ;
			rax.mPathName.trunc (ix) ;
			rax.mSeparator.pop () ;
		}
		if ifdo (TRUE) {
			if (rax.mSeparator.length () != 2)
				discard ;
			INDEX ix = rax.mSeparator[0] + 1 ;
			INDEX iy = rax.mSeparator[1] ;
			if (!is_root (rax.mPathName ,ix ,iy))
				discard ;
			rax.mPathName = String<STR>::make (rax.mPathName ,slice ("/") ,slice (".")) ;
			rax.mSeparator.add (iy + 2) ;
		}
		assume (rax.mSeparator.length () >= 2) ;
		fake.mThis = Ref<PathImplLayout>::make (move (rax)) ;
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

	void initialize (CREF<PathLayout> that) override {
		fake.mThis = that.mThis.share () ;
	}

	String<STR> fetch () const override {
		if (fake.mThis == NULL)
			return String<STR>::zero () ;
		return fake.mThis->mPathName ;
	}

	PathLayout child (CREF<Slice> name) const override {
		if (fake.mThis == NULL)
			return Path (name) ;
		return Path (String<STR>::make (fake.mThis->mPathName ,slice ("/") ,name)) ;
	}

	PathLayout child (CREF<Format> name) const override {
		if (fake.mThis == NULL)
			return Path (String<STR>::make (name)) ;
		return Path (String<STR>::make (fake.mThis->mPathName ,slice ("/") ,name)) ;
	}

	PathLayout child (CREF<String<STR>> name) const override {
		if (fake.mThis == NULL)
			return Path (name) ;
		return Path (String<STR>::make (fake.mThis->mPathName ,slice ("/") ,name)) ;
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
		if (fake.mThis == NULL)
			if (that.mThis == NULL)
				return TRUE ;
		if (fake.mThis == NULL)
			return FALSE ;
		if (that.mThis == NULL)
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
		PathLayout ret ;
		PathHolder::hold (ret)->initialize (fake) ;
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
					if (!is_root (rax[0] ,0 ,rax[0].length ()))
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

	BOOL is_root (CREF<String<STR>> str ,CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
		if (str.size () == 0)
			return TRUE ;
		const auto r1x = Slice (address (str[begin_]) ,end_ - begin_ ,SIZE_OF<STR>::expr) ;
		if (r1x.size () == 0)
			return TRUE ;
		if (r1x.size () != 2)
			return FALSE ;
		if (StreamProc::is_alpha (r1x[0]))
			if (r1x[1] == STRU32 (':'))
				return TRUE ;
		return FALSE ;
	}

	Deque<String<STR>> decouple () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		Deque<String<STR>> ret = Deque<String<STR>> (r1x) ;
		for (auto &&i : iter (0 ,r1x - 1)) {
			INDEX ix = fake.mThis->mSeparator[i] + 1 ;
			INDEX iy = fake.mThis->mSeparator[i + 1] ;
			const auto r2x = Slice (address (fake.mThis->mPathName[ix]) ,iy - ix ,SIZE_OF<STR>::expr) ;
			ret.add (r2x) ;
		}
		return move (ret) ;
	}

	String<STR> path () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		INDEX ix = fake.mThis->mSeparator[0] + 1 ;
		INDEX iy = fake.mThis->mSeparator[r1x - 2] + 1 ;
		return Slice (address (fake.mThis->mPathName[ix]) ,iy - ix ,SIZE_OF<STR>::expr) ;
	}

	String<STR> name () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		INDEX ix = fake.mThis->mSeparator[r1x - 2] + 1 ;
		INDEX iy = fake.mThis->mSeparator[r1x - 1] ;
		return Slice (address (fake.mThis->mPathName[ix]) ,iy - ix ,SIZE_OF<STR>::expr) ;
	}

	String<STR> stem () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		INDEX ix = fake.mThis->mSeparator[r1x - 2] + 1 ;
		INDEX iy = find_last_dot_word () ;
		return Slice (address (fake.mThis->mPathName[ix]) ,iy - ix ,SIZE_OF<STR>::expr) ;
	}

	String<STR> extension () const override {
		const auto r1x = fake.mThis->mSeparator.length () ;
		INDEX ix = find_last_dot_word () ;
		INDEX iy = fake.mThis->mSeparator[r1x - 1] ;
		return Slice (address (fake.mThis->mPathName[ix]) ,iy - ix ,SIZE_OF<STR>::expr) ;
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

class FileProcImplHolder final implement Fat<FileProcHolder ,FileProcLayout> {
private:
	using FILEPROC_RETRY_TIME = RANK3 ;

public:
	void initialize () override {
		auto rax = FileProcImplLayout () ;
		rax.mMutex = NULL ;
		fake.mThis = Ref<FileProcImplLayout>::make (move (rax)) ;
	}

	RefBuffer<BYTE> load_file (CREF<String<STR>> file) const override {
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = std::open (file ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
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

	VAL64 file_size (CREF<HFILE> handle) const {
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
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r2x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r3x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (file ,r2x ,r3x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
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
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = std::open (src ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (dst ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
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
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		auto rax = UniqueRef<String<STR>> ([&] (VREF<String<STR>> me) {
			me = file ;
			FileProc::save_file (me ,snapshot_) ;
		} ,[&] (VREF<String<STR>> me) {
			FileProc::erase_file (me) ;
		}) ;
		auto rbx = List<UniqueRef<String<STR>>> () ;
		fake.mThis->mLockDirectory.get (rbx) ;
		rbx.add (move (rax)) ;
		fake.mThis->mLockDirectory.set (rbx) ;
	}
} ;

static const auto mFileProcExternal = External<FileProcHolder ,FileProcLayout> (FileProcImplHolder ()) ;

struct StreamFileImplLayout {
	String<STR> mFile ;
	UniqueRef<HFILE> mReadPipe ;
	UniqueRef<HFILE> mWritePipe ;
	VAL64 mFileSize ;
	VAL64 mRead ;
	VAL64 mWrite ;
	BOOL mShortRead ;
	LENGTH mShortSize ;
} ;

class StreamFileImplHolder final implement Fat<StreamFileHolder ,StreamFileLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mThis = AutoRef<StreamFileImplLayout>::make () ;
		fake.mThis->mFile = move (file) ;
		fake.mThis->mFileSize = 0 ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
		fake.mThis->mShortRead = FALSE ;
		fake.mThis->mShortSize = 0 ;
	}

	void set_short_read (CREF<BOOL> flag) override {
		fake.mThis->mShortRead = flag ;
	}

	void open_r () override {
		assert (!fake.mThis->mReadPipe.exist ()) ;
		assert (!fake.mThis->mWritePipe.exist ()) ;
		fake.mThis->mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mReadPipe) ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!fake.mThis->mReadPipe.exist ()) ;
		assert (!fake.mThis->mWritePipe.exist ()) ;
		fake.mThis->mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = size_ ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
	}

	void open_a () override {
		assert (!fake.mThis->mReadPipe.exist ()) ;
		assert (!fake.mThis->mWritePipe.exist ()) ;
		fake.mThis->mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDONLY) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mReadPipe) ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
		if ifdo (TRUE) {
			const auto r5x = VAL64 (lseek64 (fake.mThis->mWritePipe ,0 ,SEEK_END)) ;
			if (r5x <= 0)
				discard ;
			fake.mThis->mWrite += r5x ;
		}
	}

	LENGTH file_size () const override {
		assume (fake.mThis->mFileSize < VAL32_MAX) ;
		return LENGTH (fake.mThis->mFileSize) ;
	}

	VAL64 file_size (CREF<HFILE> handle) const {
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
		return fake.mThis->mShortSize ;
	}

	void read (VREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mReadPipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			auto rbx = csc_size_t (rax) ;
			rbx = std::read (fake.mThis->mReadPipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				discard ;
			assume (fake.mThis->mShortRead) ;
		}
		fake.mThis->mShortSize = r1x - rax ;
		fake.mThis->mRead += fake.mThis->mShortSize ;
	}

	void write (CREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mWritePipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (fake.mThis->mWritePipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				discard ;
			assume (fake.mThis->mShortRead) ;
		}
		fake.mThis->mShortSize = r1x - rax ;
		fake.mThis->mWrite += fake.mThis->mShortSize ;
	}

	void flush () override {
		if (!fake.mThis->mWritePipe.exist ())
			return ;
		fsync (fake.mThis->mWritePipe) ;
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
	UniqueRef<HFILE> mPipe ;
	UniqueRef<LENGTH> mMapping ;
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
	using BLOCK_STEP_SIZE = ENUM<1024> ;
	using CHUNK_STEP_SIZE = ENUM<4194304> ;
	using HEADER_SIZE = ENUM<65536> ;

public:
	void initialize (CREF<String<STR>> file) override {
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
		fake.mThis->mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mPipe) ;
		fake.mThis->mMapping = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = 0 ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		fake.mThis->mMapping.depend (fake.mThis->mPipe) ;
		fake.mThis->mFileMapFlag = csc_enum_t (PROT_READ) ;
		read_header () ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!fake.mThis->mPipe.exist ()) ;
		assert (!fake.mThis->mMapping.exist ()) ;
		fake.mThis->mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r3x = fake.mThis->mChunkStep / fake.mThis->mBlockStep ;
		const auto r4x = (size_ + r3x - 1) / r3x ;
		fake.mThis->mFileSize = HEADER_SIZE::expr + r4x * fake.mThis->mChunkStep ;
		fake.mThis->mMapping = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = ftruncate64 (fake.mThis->mPipe ,fake.mThis->mFileSize) ;
			assume (me == 0) ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		fake.mThis->mMapping.depend (fake.mThis->mPipe) ;
		fake.mThis->mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		write_header () ;
	}

	void open_a () override {
		assert (!fake.mThis->mPipe.exist ()) ;
		assert (!fake.mThis->mMapping.exist ()) ;
		assume (fake.mThis->mHeader != NULL) ;
		fake.mThis->mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = fake.mThis->mHeader->mFileSize ;
		fake.mThis->mMapping = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = ftruncate64 (fake.mThis->mPipe ,fake.mThis->mFileSize) ;
			assume (me == 0) ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		fake.mThis->mMapping.depend (fake.mThis->mPipe) ;
		fake.mThis->mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		read_header () ;
	}

	void read_header () {
		assert (fake.mThis->mHeader == NULL) ;
		fake.mThis->mHeader = Box<BufferFileHeader>::make () ;
		auto rax = ByteReader (borrow_header ()) ;
		rax >> slice ("CSC_BufferFile") ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mFileEndian ;
		assume (fake.mThis->mHeader->mFileEndian == QUAD_ENDIAN) ;
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
			fake.mThis->mHeader->mFileEndian = QUAD_ENDIAN ;
			fake.mThis->mHeader->mFileSize = fake.mThis->mFileSize ;
			fake.mThis->mHeader->mBlockSize = fake.mThis->mChunkStep / fake.mThis->mBlockStep ;
			fake.mThis->mHeader->mBlockStep = fake.mThis->mBlockStep ;
			fake.mThis->mHeader->mBlockLength = 0 ;
			fake.mThis->mHeader->mChunkSize = (fake.mThis->mFileSize - HEADER_SIZE::expr) / fake.mThis->mChunkStep ;
			fake.mThis->mHeader->mChunkStep = fake.mThis->mChunkStep ;
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

	LENGTH file_size () const override {
		assume (fake.mThis->mFileSize < VAL32_MAX) ;
		return LENGTH (fake.mThis->mFileSize) ;
	}

	VAL64 file_size (CREF<HFILE> handle) const {
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
				const auto r1x = mmap64 (NULL ,size_ ,fake.mThis->mFileMapFlag ,MAP_SHARED ,fake.mThis->mPipe ,index) ;
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
		fsync (fake.mThis->mPipe) ;
	}
} ;

static const auto mBufferFileExternal = External<BufferFileHolder ,BufferFileLayout> (BufferFileImplHolder ()) ;

struct UartFileImplLayout {
	String<STR> mPortName ;
	LENGTH mPortRate ;
	UniqueRef<HFILE> mPipe ;
	TERMIOS_INFO mSerialStat ;
	RefBuffer<BYTE> mRingBuffer ;
	INDEX mRingRead ;
} ;

class UartFileImplHolder final implement Fat<UartFileHolder ,UartFileLayout> {
private:
	void initialize () override {
		fake.mThis = AutoRef<UartFileImplLayout>::make () ;
		fake.mThis->mPortRate = 0 ;
	}

	void set_port_name (CREF<String<STR>> name) override {
		fake.mThis->mPortName = name ;
	}

	void set_port_rate (CREF<LENGTH> rate) override {
		fake.mThis->mPortRate = rate ;
	}

	void set_ring_size (CREF<LENGTH> size_) override {
		fake.mThis->mRingBuffer = RefBuffer<BYTE> (size_) ;
		fake.mThis->mRingRead = 0 ;
	}

	void open () override {
		assert (fake.mThis->mPortName.length () > 0) ;
		assert (fake.mThis->mRingBuffer.size () > 0) ;
		fake.mThis->mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_RDWR | O_NOCTTY | O_SYNC) ;
			me = std::open (fake.mThis->mPortName ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = tcgetattr (fake.mThis->mPipe ,(&fake.mThis->mSerialStat)) ;
		assume (r2x != 0) ;
		cfsetospeed ((&fake.mThis->mSerialStat) ,VAL32 (fake.mThis->mPortRate)) ;
		cfsetispeed ((&fake.mThis->mSerialStat) ,VAL32 (fake.mThis->mPortRate)) ;
		fake.mThis->mSerialStat.c_cflag = (fake.mThis->mSerialStat.c_cflag & ~CSIZE) | CS8 ;
		fake.mThis->mSerialStat.c_iflag &= ~IGNBRK ;
		fake.mThis->mSerialStat.c_lflag = 0 ;
		fake.mThis->mSerialStat.c_oflag = 0 ;
		fake.mThis->mSerialStat.c_cc[VMIN] = 0 ;
		fake.mThis->mSerialStat.c_cc[VTIME] = 5 ;
		fake.mThis->mSerialStat.c_iflag &= ~(IXON | IXOFF | IXANY) ;
		fake.mThis->mSerialStat.c_cflag |= (CLOCAL | CREAD) ;
		fake.mThis->mSerialStat.c_cflag &= ~(PARENB | PARODD) ;
		fake.mThis->mSerialStat.c_cflag &= ~CSTOPB ;
		fake.mThis->mSerialStat.c_cflag &= ~CRTSCTS ;
		const auto r3x = tcsetattr (fake.mThis->mPipe ,TCSANOW ,(&fake.mThis->mSerialStat)) ;
		assume (r3x != 0) ;
	}

	void read (VREF<RefBuffer<BYTE>> buffer ,CREF<INDEX> offset ,CREF<LENGTH> size_) override {
		for (auto &&i : iter (0 ,size_)) {
			buffer[offset + i] = fake.mThis->mRingBuffer[fake.mThis->mRingRead] ;
			fake.mThis->mRingRead++ ;
			if ifdo (TRUE) {
				if (fake.mThis->mRingRead < fake.mThis->mRingBuffer.size ())
					discard ;
				auto rax = fake.mThis->mRingBuffer.size () ;
				while (TRUE) {
					auto rbx = csc_size_t (rax) ;
					rbx = std::read (fake.mThis->mPipe ,fake.mThis->mRingBuffer ,rbx) ;
					assume (rbx >= 0) ;
					rax -= rbx ;
					if (rax == 0)
						break ;
					RuntimeProc::thread_yield () ;
				}
				fake.mThis->mRingRead = 0 ;
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

class ConsoleImplHolder final implement Fat<ConsoleHolder ,ConsoleLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<ConsoleImplLayout>::make () ;
		fake.mThis->mMutex = NULL ;
		fake.mThis->mOption = BitSet (ConsoleOption::ETC) ;
		fake.mThis->mLogBuffer = String<STR> (STREAMFILE_BUF_SIZE::expr) ;
		fake.mThis->mLogWriter = TextWriter (fake.mThis->mLogBuffer.borrow ()) ;
		fake.mThis->mCommand = NULL ;
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
		fake.mThis->mLogWriter << CLS ;
		fake.mThis->mLogWriter << slice ("[") ;
		const auto r1x = CurrentTime () ;
		const auto r2x = r1x.calendar () ;
		fake.mThis->mLogWriter << AlignedText (r2x.mHour ,2) ;
		fake.mThis->mLogWriter << slice (":") ;
		fake.mThis->mLogWriter << AlignedText (r2x.mMinute ,2) ;
		fake.mThis->mLogWriter << slice (":") ;
		fake.mThis->mLogWriter << AlignedText (r2x.mSecond ,2) ;
		fake.mThis->mLogWriter << slice ("][") ;
		fake.mThis->mLogWriter << tag ;
		fake.mThis->mLogWriter << slice ("] : ") ;
		fake.mThis->mLogWriter << msg ;
		fake.mThis->mLogWriter << GAP ;
		fake.mThis->mLogWriter << EOS ;
	}

	void print (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoPrint])
			return ;
		fake.mThis->mLogWriter << CLS ;
		fake.mThis->mLogWriter << msg ;
		fake.mThis->mLogWriter << EOS ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("%s")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void fatal (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoFatal])
			return ;
		log (slice ("Fatal") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;34m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void error (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoError])
			return ;
		log (slice ("Error") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;31m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void warn (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoWarn])
			return ;
		log (slice ("Warn") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;33m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void info (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoInfo])
			return ;
		log (slice ("Info") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;32m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void debug (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoDebug])
			return ;
		log (slice ("Debug") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;36m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void trace (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoTrace])
			return ;
		log (slice ("Trace") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STR> (slice ("\033[1;37m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void open (CREF<String<STR>> dire) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		fake.mThis->mLogFile = Path (dire).child (slice ("console.log")) ;
		fake.mThis->mOldLogFile = Path (dire).child (slice ("console.old.log")) ;
		FileProc::erase_file (fake.mThis->mOldLogFile) ;
		FileProc::move_file (fake.mThis->mOldLogFile ,fake.mThis->mLogFile) ;
		fake.mThis->mLogStreamFile = StreamFile (fake.mThis->mLogFile) ;
		fake.mThis->mLogStreamFile.open_w (0) ;
		fake.mThis->mLogWriter << CLS ;
		fake.mThis->mLogWriter << BOM ;
		fake.mThis->mLogWriter << EOS ;
		log_file () ;
	}

	void log_file () const {
		if (fake.mThis->mLogFile.length () == 0)
			return ;
		const auto r1x = FLAG (fake.mThis->mLogBuffer.self) ;
		const auto r2x = (fake.mThis->mLogWriter.length () - 1) * SIZE_OF<STR>::expr ;
		fake.mThis->mLogStreamFile.write (RefBuffer<BYTE>::reference (r1x ,r2x)) ;
	}

	void start () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mConsole.exist ())
			return ;
		fake.mThis->mConsole = UniqueRef<HANDLE>::make (stderr) ;
	}

	void stop () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		fake.mThis->mConsole = UniqueRef<HANDLE>::make () ;
	}

	void pause () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		const auto r1x = String<STR> (slice ("%s\n")) ;
		const auto r2x = String<STR> (slice ("press any key to continue...")) ;
		std::printf (r1x.self ,r2x.self) ;
		const auto r3x = std::getchar () ;
		noop (r3x) ;
	}

	void clear () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		fake.mThis->mCommand.execute (slice ("clear")) ;
	}
} ;

static const auto mConsoleExternal = External<ConsoleHolder ,ConsoleLayout> (ConsoleImplHolder ()) ;
} ;