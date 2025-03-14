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

inline namespace {
using HFILEPIPE = HANDLE ;
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
			self.mThis->mPathName[i] = STR ('\\') ;
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
			self.mThis->mPathName = String<STR>::make (self.mThis->mPathName ,slice ("\\") ,slice (".")) ;
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

	String<STR> fetch () const override {
		if (self.mThis == NULL)
			return String<STR>::zero () ;
		return self.mThis->mPathName ;
	}

	PathLayout child (CREF<Slice> name) const override {
		return Path (String<STR>::make (fetch () ,slice ("\\") ,name)) ;
	}

	PathLayout child (CREF<Format> name) const override {
		return Path (String<STR>::make (fetch () ,slice ("\\") ,name)) ;
	}

	PathLayout child (CREF<String<STR>> name) const override {
		return Path (String<STR>::make (fetch () ,slice ("\\") ,name)) ;
	}

	Array<PathLayout> list () const override {
		auto rax = WIN32_FIND_DATA () ;
		const auto r1x = String<STR>::make (self.mThis->mPathName ,slice ("\\") ,slice ("*.*")) ;
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
		const auto r1x = String<STR>::make (self.mThis->mPathName ,slice ("\\") ,slice ("*.*")) ;
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
		const auto r1x = CHAR (GetFileAttributes (self.mThis->mPathName)) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return (!ByteProc::any_bit (r1x ,FILE_ATTRIBUTE_DIRECTORY)) ;
	}

	BOOL is_dire () const override {
		if (self.mThis == NULL)
			return FALSE ;
		const auto r1x = CHAR (GetFileAttributes (self.mThis->mPathName)) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return ByteProc::any_bit (r1x ,FILE_ATTRIBUTE_DIRECTORY) ;
	}

	BOOL is_link () const override {
		if (self.mThis == NULL)
			return FALSE ;
		const auto r1x = CHAR (GetFileAttributes (self.mThis->mPathName)) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return ByteProc::any_bit (r1x ,FILE_ATTRIBUTE_REPARSE_POINT) ;
	}

	PathLayout symbolic () const override {
		PathLayout ret = self ;
		if ifdo (TRUE) {
			if (!is_link ())
				discard ;
			auto rax = String<STR>::make () ;
			const auto r1x = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
				me = CreateFile (self.mThis->mPathName ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_FLAG_BACKUP_SEMANTICS ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[&] (VREF<HFILEPIPE> me) {
				CloseHandle (me) ;
			}) ;
			GetFinalPathNameByHandle (r1x ,rax ,csc_enum_t (rax.size ()) ,FILE_NAME_OPENED) ;
			if ifdo (TRUE) {
				const auto r2x = rax.length () ;
				if (r2x < 4)
					discard ;
				const auto r3x = rax.segment (0 ,4) ;
				if (r3x != slice ("\\\\?\\"))
					discard ;
				rax = String<STR> (rax.segment (4 ,r2x)) ;
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
			me = CreateFile (file ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
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
		const auto r1x = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			me = CreateFile (file ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
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
			const auto r1x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (self.mFile ,GENERIC_READ ,r1x ,NULL ,OPEN_EXISTING ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			CloseHandle (me) ;
		}) ;
		self.mFileSize = file_size (self.mReadPipe) ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!self.mReadPipe.exist ()) ;
		assert (!self.mWritePipe.exist ()) ;
		self.mWritePipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (self.mFile ,GENERIC_WRITE ,r1x ,NULL ,CREATE_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			CloseHandle (me) ;
		}) ;
		self.mFileSize = size_ ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
	}

	void open_a () override {
		assert (!self.mReadPipe.exist ()) ;
		assert (!self.mWritePipe.exist ()) ;
		self.mReadPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (self.mFile ,GENERIC_READ ,r1x ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			CloseHandle (me) ;
		}) ;
		self.mWritePipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r2x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (self.mFile ,GENERIC_WRITE ,r2x ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			CloseHandle (me) ;
		}) ;
		self.mFileSize = file_size (self.mReadPipe) ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
		if ifdo (TRUE) {
			auto rax = LONG (0) ;
			const auto r3x = SetFilePointer (self.mWritePipe ,0 ,(&rax) ,FILE_END) ;
			const auto r4x = VAL64 (ByteProc::merge (CHAR (rax) ,CHAR (r3x))) ;
			if (r4x <= 0)
				discard ;
			self.mWrite += r4x ;
		}
	}

	LENGTH file_size () const override {
		assume (self.mFileSize < VAL32_MAX) ;
		return LENGTH (self.mFileSize) ;
	}

	VAL64 file_size (CREF<HANDLE> handle) const {
		auto rax = Buffer2<csc_enum_t> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = VAL64 (ByteProc::merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
		assume (r1x >= 0) ;
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
			auto rbx = csc_enum_t (rax) ;
			const auto r2x = ReadFile (self.mReadPipe ,(&item[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r2x) ;
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
			auto rbx = csc_enum_t (rax) ;
			const auto r2x = WriteFile (self.mWritePipe ,(&item[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r2x) ;
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
		FlushFileBuffers (self.mWritePipe) ;
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
			me = CreateFile (self.mFile ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			CloseHandle (me) ;
		}) ;
		self.mFileSize = file_size (self.mPipe) ;
		self.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (ByteProc::split_high (QUAD (self.mFileSize))) ;
			const auto r2x = csc_enum_t (ByteProc::split_low (QUAD (self.mFileSize))) ;
			me = CreateFileMapping (self.mPipe ,NULL ,PAGE_READONLY ,r1x ,r2x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		self.mMapping.depend (self.mPipe) ;
		self.mFileMapFlag = csc_enum_t (FILE_MAP_READ) ;
		read_header () ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!self.mPipe.exist ()) ;
		assert (!self.mMapping.exist ()) ;
		self.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (GENERIC_READ | GENERIC_WRITE) ;
			me = CreateFile (self.mFile ,r1x ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = self.mChunkStep / self.mBlockStep ;
		const auto r3x = (size_ + r2x - 1) / r2x ;
		self.mFileSize = BUFFERFILE_HEADER_STEP::expr + r3x * self.mChunkStep ;
		self.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r4x = csc_enum_t (ByteProc::split_high (QUAD (self.mFileSize))) ;
			const auto r5x = csc_enum_t (ByteProc::split_low (QUAD (self.mFileSize))) ;
			me = CreateFileMapping (self.mPipe ,NULL ,PAGE_READWRITE ,r4x ,r5x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		self.mMapping.depend (self.mPipe) ;
		self.mFileMapFlag = csc_enum_t (FILE_MAP_READ | FILE_MAP_WRITE) ;
		write_header () ;
	}

	void open_a () override {
		assert (!self.mPipe.exist ()) ;
		assert (!self.mMapping.exist ()) ;
		assume (self.mHeader != NULL) ;
		self.mPipe = UniqueRef<HFILEPIPE> ([&] (VREF<HFILEPIPE> me) {
			const auto r1x = csc_enum_t (GENERIC_READ | GENERIC_WRITE) ;
			me = CreateFile (self.mFile ,r1x ,0 ,NULL ,OPEN_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			CloseHandle (me) ;
		}) ;
		self.mFileSize = self.mHeader->mFileSize ;
		self.mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (ByteProc::split_high (QUAD (self.mFileSize))) ;
			const auto r2x = csc_enum_t (ByteProc::split_low (QUAD (self.mFileSize))) ;
			me = CreateFileMapping (self.mPipe ,NULL ,PAGE_READWRITE ,r1x ,r2x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		self.mMapping.depend (self.mPipe) ;
		self.mFileMapFlag = csc_enum_t (FILE_MAP_READ | FILE_MAP_WRITE) ;
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
			self.mHeader->mChunkStep = self.mChunkStep ;
			self.mHeader->mChunkSize = (self.mFileSize - BUFFERFILE_HEADER_STEP::expr) / self.mChunkStep ;
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

	VAL64 file_size (CREF<HANDLE> handle) const {
		auto rax = Buffer2<csc_enum_t> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = VAL64 (ByteProc::merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
		assume (r1x >= 0) ;
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
				const auto r1x = csc_enum_t (ByteProc::split_high (QUAD (index))) ;
				const auto r2x = csc_enum_t (ByteProc::split_low (QUAD (index))) ;
				const auto r3x = MapViewOfFile (self.mMapping ,self.mFileMapFlag ,r1x ,r2x ,size_) ;
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
		FlushFileBuffers (self.mPipe) ;
	}
} ;

static const auto mBufferFileExternal = External<BufferFileHolder ,BufferFileLayout> (BufferFileImplHolder ()) ;

struct UartFileLayout {
	String<STR> mPortName ;
	LENGTH mPortRate ;
	UniqueRef<HFILEPIPE> mPipe ;
	DCB mSerialParams ;
	COMSTAT mSerialStat ;
	csc_enum_t mSerialError ;
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
			me = CreateFile (self.mPortName ,GENERIC_READ | GENERIC_WRITE ,0 ,0 ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,0) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HFILEPIPE> me) {
			CloseHandle (me) ;
		}) ;
		inline_memset (self.mSerialParams) ;
		self.mSerialParams.DCBlength = csc_enum_t (SIZE_OF<DCB>::expr) ;
		GetCommState (self.mPipe ,(&self.mSerialParams)) ;
		self.mSerialParams.BaudRate = csc_enum_t (self.mPortRate) ;
		self.mSerialParams.ByteSize = 8 ;
		self.mSerialParams.StopBits = ONESTOPBIT ;
		self.mSerialParams.Parity = NOPARITY ;
		SetCommState (self.mPipe ,(&self.mSerialParams)) ;
		ClearCommError (self.mPipe ,(&self.mSerialError) ,(&self.mSerialStat)) ;
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
					auto rbx = csc_enum_t (rax) ;
					const auto r1x = ReadFile (self.mPipe ,self.mRingBuffer ,rbx ,(&rbx) ,NULL) ;
					assume (r1x) ;
					rax -= LENGTH (rbx) ;
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
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (self.mConsole ,r1x) ;
			auto rax = csc_enum_t (self.mLogWriter.length () - 1) ;
			WriteConsole (self.mConsole ,self.mLogBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void fatal (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoFatal])
			return ;
		log (slice ("Fatal") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (self.mConsole ,r1x) ;
			auto rax = csc_enum_t (self.mLogWriter.length () - 1) ;
			WriteConsole (self.mConsole ,self.mLogBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void error (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoError])
			return ;
		log (slice ("Error") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (self.mConsole ,r1x) ;
			auto rax = csc_enum_t (self.mLogWriter.length () - 1) ;
			WriteConsole (self.mConsole ,self.mLogBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void warn (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoWarn])
			return ;
		log (slice ("Warn") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (self.mConsole ,r1x) ;
			auto rax = csc_enum_t (self.mLogWriter.length () - 1) ;
			WriteConsole (self.mConsole ,self.mLogBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void info (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoInfo])
			return ;
		log (slice ("Info") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (self.mConsole ,r1x) ;
			auto rax = csc_enum_t (self.mLogWriter.length () - 1) ;
			WriteConsole (self.mConsole ,self.mLogBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void debug (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoDebug])
			return ;
		log (slice ("Debug") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (self.mConsole ,r1x) ;
			auto rax = csc_enum_t (self.mLogWriter.length () - 1) ;
			WriteConsole (self.mConsole ,self.mLogBuffer ,rax ,(&rax) ,NULL) ;
		}
	}

	void trace (CREF<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoTrace])
			return ;
		log (slice ("Trace") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (self.mConsole ,r1x) ;
			auto rax = csc_enum_t (self.mLogWriter.length () - 1) ;
			WriteConsole (self.mConsole ,self.mLogBuffer ,rax ,(&rax) ,NULL) ;
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
			if (self.mConsole.deref != INVALID_HANDLE_VALUE)
				return ;
		self.mConsole = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			AllocConsole () ;
			me = GetStdHandle (STD_OUTPUT_HANDLE) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			FreeConsole () ;
		}) ;
	}

	void hide () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		self.mConsole = UniqueRef<HANDLE>::make (INVALID_HANDLE_VALUE) ;
	}

	void pause () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if ifdo (TRUE) {
			const auto r1x = GetConsoleWindow () ;
			if (r1x == NULL)
				discard ;
			FlashWindow (r1x ,TRUE) ;
		}
		const auto r2x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
		SetConsoleTextAttribute (self.mConsole ,r2x) ;
		self.mCommand.execute (slice ("pause")) ;
	}

	void clear () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
		SetConsoleTextAttribute (self.mConsole ,r1x) ;
		self.mCommand.execute (slice ("cls")) ;
	}
} ;

static const auto mConsoleExternal = External<ConsoleHolder ,ConsoleLayout> (ConsoleImplHolder ()) ;
} ;