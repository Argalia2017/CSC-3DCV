#pragma once

#ifndef __CSC_FILE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_file.hpp"

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require windows.h"
#endif

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
			rax.mPathName[i] = STR ('\\') ;
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
			rax.mPathName = String<STR>::make (rax.mPathName ,slice ("\\") ,slice (".")) ;
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
				rax.splice (ix ,slice ("\\")) ;
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
		return Path (String<STR>::make (fake.mThis->mPathName ,slice ("\\") ,name)) ;
	}

	PathLayout child (CREF<Format> name) const override {
		if (fake.mThis == NULL)
			return Path (String<STR>::make (name)) ;
		return Path (String<STR>::make (fake.mThis->mPathName ,slice ("\\") ,name)) ;
	}

	PathLayout child (CREF<String<STR>> name) const override {
		if (fake.mThis == NULL)
			return Path (name) ;
		return Path (String<STR>::make (fake.mThis->mPathName ,slice ("\\") ,name)) ;
	}

	Array<PathLayout> list () const override {
		auto rax = WIN32_FIND_DATA () ;
		const auto r1x = String<STR>::make (fake.mThis->mPathName ,slice ("\\") ,slice ("*.*")) ;
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
			const auto r3x = Slice (address (rax.cFileName) ,3 ,SIZE_OF<STR>::expr) ;
			noop (r3x) ;
			assert (r3x.eos () == slice (".")) ;
			FindNextFile (r2x ,(&rax)) ;
			assert (r3x.eos () == slice ("..")) ;
			while (TRUE) {
				const auto r4x = FindNextFile (r2x ,(&rax)) ;
				if (!r4x)
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
		const auto r1x = String<STR>::make (fake.mThis->mPathName ,slice ("\\") ,slice ("*.*")) ;
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
			const auto r3x = Slice (address (rax.cFileName) ,3 ,SIZE_OF<STR>::expr) ;
			noop (r3x) ;
			assert (r3x.eos () == slice (".")) ;
			FindNextFile (r2x ,(&rax)) ;
			assert (r3x.eos () == slice ("..")) ;
			while (TRUE) {
				const auto r4x = FindNextFile (r2x ,(&rax)) ;
				if (!r4x)
					break ;
				if (rbx.length () >= size_)
					break ;
				rbx.add (Slice (rax.cFileName)) ;
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
		const auto r1x = CHAR (GetFileAttributes (fake.mThis->mPathName)) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return (!ByteProc::any_bit (r1x ,FILE_ATTRIBUTE_DIRECTORY)) ;
	}

	BOOL is_dire () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		const auto r1x = CHAR (GetFileAttributes (fake.mThis->mPathName)) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return ByteProc::any_bit (r1x ,FILE_ATTRIBUTE_DIRECTORY) ;
	}

	BOOL is_link () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		const auto r1x = CHAR (GetFileAttributes (fake.mThis->mPathName)) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return ByteProc::any_bit (r1x ,FILE_ATTRIBUTE_REPARSE_POINT) ;
	}

	PathLayout symbolic () const override {
		PathLayout ret ;
		PathHolder::hold (ret)->initialize (fake) ;
		if ifdo (TRUE) {
			if (!is_link ())
				discard ;
			auto rax = String<STR>::make () ;
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile (fake.mThis->mPathName ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_FLAG_BACKUP_SEMANTICS ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[&] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			GetFinalPathNameByHandle (r1x ,rax ,csc_enum_t (rax.size ()) ,FILE_NAME_OPENED) ;
			if ifdo (TRUE) {
				const auto r2x = rax.length () ;
				if (r2x < 4)
					discard ;
				const auto r3x = Slice (address (rax[0]) ,4 ,SIZE_OF<STR>::expr) ;
				if (r3x != slice ("\\\\?\\"))
					discard ;
				rax = String<STR> (Slice (address (rax[4]) ,r2x - 4 ,SIZE_OF<STR>::expr)) ;
			}
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
					GetCurrentDirectory (csc_enum_t (rcx.size ()) ,rcx) ;
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
					GetCurrentDirectory (csc_enum_t (rcx.size ()) ,rcx) ;
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
		for (auto &&i : iter (0 ,FILEPROC_RETRY_TIME::expr)) {
			noop (i) ;
			auto rbx = csc_enum_t (rax) ;
			const auto r4x = ReadFile (r1x ,(&ret[r3x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r4x) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				break ;
		}
		assume (rax == 0) ;
		return move (ret) ;
	}

	VAL64 file_size (CREF<HANDLE> handle) const {
		auto rax = Buffer2<csc_enum_t> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = VAL64 (ByteProc::merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
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
		for (auto &&i : iter (0 ,FILEPROC_RETRY_TIME::expr)) {
			noop (i) ;
			auto rbx = csc_enum_t (rax) ;
			const auto r3x = WriteFile (r1x ,(&item[r2x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r3x) ;
			rax -= LENGTH (rbx) ;
			if (rax == 0)
				break ;
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
		if (!Path (src).is_file ())
			return ;
		const auto r1x = CreateSymbolicLink (dst ,src ,0) ;
		noop (r1x) ;
	}

	void erase_file (CREF<String<STR>> file) const override {
		if (!Path (file).is_file ())
			return ;
		const auto r1x = DeleteFile (file) ;
		noop (r1x) ;
	}

	void build_dire (CREF<String<STR>> dire) const override {
		const auto r1x = Path (dire).decouple () ;
		auto rax = Path () ;
		for (auto &&i : r1x.range ()) {
			rax = rax.child (r1x[i]) ;
			const auto r2x = rax.fetch () ;
			CreateDirectory (r2x ,NULL) ;
		}
	}

	void link_dire (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		if (!Path (src).is_dire ())
			return ;
		const auto r1x = CreateSymbolicLink (dst ,src ,SYMBOLIC_LINK_FLAG_DIRECTORY) ;
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
		const auto r1x = RemoveDirectory (dire) ;
		noop (r1x) ;
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
	UniqueRef<HANDLE> mReadPipe ;
	UniqueRef<HANDLE> mWritePipe ;
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
			const auto r4x = VAL64 (ByteProc::merge (CHAR (rax) ,CHAR (r3x))) ;
			if (r4x <= 0)
				discard ;
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
		const auto r1x = VAL64 (ByteProc::merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
		assume (r1x >= 0) ;
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
			auto rbx = csc_enum_t (rax) ;
			const auto r2x = ReadFile (fake.mThis->mReadPipe ,(&item[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r2x) ;
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
			auto rbx = csc_enum_t (rax) ;
			const auto r2x = WriteFile (fake.mThis->mWritePipe ,(&item[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r2x) ;
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
		FlushFileBuffers (fake.mThis->mWritePipe) ;
	}
} ;

static const auto mStreamFileExternal = External<StreamFileHolder ,StreamFileLayout> (StreamFileImplHolder ()) ;

struct BufferFileHeader {
	QUAD mFileEndian ;
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
		fake.mThis->mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFile (fake.mThis->mFile ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mPipe) ;
		fake.mThis->mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (ByteProc::split_high (QUAD (fake.mThis->mFileSize))) ;
			const auto r2x = csc_enum_t (ByteProc::split_low (QUAD (fake.mThis->mFileSize))) ;
			me = CreateFileMapping (fake.mThis->mPipe ,NULL ,PAGE_READONLY ,r1x ,r2x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mMapping.depend (fake.mThis->mPipe) ;
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
			const auto r4x = csc_enum_t (ByteProc::split_high (QUAD (fake.mThis->mFileSize))) ;
			const auto r5x = csc_enum_t (ByteProc::split_low (QUAD (fake.mThis->mFileSize))) ;
			me = CreateFileMapping (fake.mThis->mPipe ,NULL ,PAGE_READWRITE ,r4x ,r5x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mMapping.depend (fake.mThis->mPipe) ;
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
			const auto r1x = csc_enum_t (ByteProc::split_high (QUAD (fake.mThis->mFileSize))) ;
			const auto r2x = csc_enum_t (ByteProc::split_low (QUAD (fake.mThis->mFileSize))) ;
			me = CreateFileMapping (fake.mThis->mPipe ,NULL ,PAGE_READWRITE ,r1x ,r2x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mMapping.depend (fake.mThis->mPipe) ;
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

	LENGTH file_size () const override {
		assume (fake.mThis->mFileSize < VAL32_MAX) ;
		return LENGTH (fake.mThis->mFileSize) ;
	}

	VAL64 file_size (CREF<HANDLE> handle) const {
		auto rax = Buffer2<csc_enum_t> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = VAL64 (ByteProc::merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
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
				const auto r1x = csc_enum_t (ByteProc::split_high (QUAD (index))) ;
				const auto r2x = csc_enum_t (ByteProc::split_low (QUAD (index))) ;
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

static const auto mBufferFileExternal = External<BufferFileHolder ,BufferFileLayout> (BufferFileImplHolder ()) ;

struct UartFileImplLayout {
	String<STR> mPortName ;
	LENGTH mPortRate ;
	UniqueRef<HANDLE> mPipe ;
	DCB mSerialParams ;
	COMSTAT mSerialStat ;
	csc_enum_t mSerialError ;
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
		fake.mThis->mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFile (fake.mThis->mPortName ,GENERIC_READ | GENERIC_WRITE ,0 ,0 ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,0) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		inline_memset (fake.mThis->mSerialParams) ;
		fake.mThis->mSerialParams.DCBlength = csc_enum_t (SIZE_OF<DCB>::expr) ;
		GetCommState (fake.mThis->mPipe ,(&fake.mThis->mSerialParams)) ;
		fake.mThis->mSerialParams.BaudRate = csc_enum_t (fake.mThis->mPortRate) ;
		fake.mThis->mSerialParams.ByteSize = 8 ;
		fake.mThis->mSerialParams.StopBits = ONESTOPBIT ;
		fake.mThis->mSerialParams.Parity = NOPARITY ;
		SetCommState (fake.mThis->mPipe ,(&fake.mThis->mSerialParams)) ;
		ClearCommError (fake.mThis->mPipe ,(&fake.mThis->mSerialError) ,(&fake.mThis->mSerialStat)) ;
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
					auto rbx = csc_enum_t (rax) ;
					const auto r1x = ReadFile (fake.mThis->mPipe ,fake.mThis->mRingBuffer ,rbx ,(&rbx) ,NULL) ;
					assume (r1x) ;
					rax -= LENGTH (rbx) ;
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
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mLogWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mLogBuffer ,rax ,(&rax) ,NULL) ;
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
			auto rax = csc_enum_t (fake.mThis->mLogWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mLogBuffer ,rax ,(&rax) ,NULL) ;
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
			auto rax = csc_enum_t (fake.mThis->mLogWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mLogBuffer ,rax ,(&rax) ,NULL) ;
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
			auto rax = csc_enum_t (fake.mThis->mLogWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mLogBuffer ,rax ,(&rax) ,NULL) ;
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
			auto rax = csc_enum_t (fake.mThis->mLogWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mLogBuffer ,rax ,(&rax) ,NULL) ;
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
			auto rax = csc_enum_t (fake.mThis->mLogWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mLogBuffer ,rax ,(&rax) ,NULL) ;
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
			auto rax = csc_enum_t (fake.mThis->mLogWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,fake.mThis->mLogBuffer ,rax ,(&rax) ,NULL) ;
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
		fake.mThis->mCommand.execute (slice ("pause")) ;
	}

	void clear () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
		SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
		fake.mThis->mCommand.execute (slice ("cls")) ;
	}
} ;

static const auto mConsoleExternal = External<ConsoleHolder ,ConsoleLayout> (ConsoleImplHolder ()) ;
} ;