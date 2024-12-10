#pragma once

#ifndef __CSC_FILE__
#define __CSC_FILE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
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
	virtual String<STR> absolute () const = 0 ;
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

	Path root () const {
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

	String<STR> absolute () const {
		return PathHolder::hold (thiz)->absolute () ;
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

struct FileProcLayout implement ThisLayout<Ref<FileProcImplLayout>> {} ;

struct FileProcHolder implement Interface {
	imports CREF<FileProcLayout> instance () ;
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
	static CREF<FileProc> instance () {
		return keep[TYPE<FileProc>::expr] (FileProcHolder::instance ()) ;
	}

	static RefBuffer<BYTE> load_file (CREF<String<STR>> file) {
		return FileProcHolder::hold (instance ())->load_file (file) ;
	}

	static void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> item) {
		return FileProcHolder::hold (instance ())->save_file (file ,item) ;
	}

	static Ref<RefBuffer<BYTE>> load_asset (CREF<String<STR>> file) {
		return FileProcHolder::hold (instance ())->load_asset (file) ;
	}

	static void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) {
		return FileProcHolder::hold (instance ())->copy_file (dst ,src) ;
	}

	static void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) {
		return FileProcHolder::hold (instance ())->move_file (dst ,src) ;
	}

	static void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) {
		return FileProcHolder::hold (instance ())->link_file (dst ,src) ;
	}

	static void erase_file (CREF<String<STR>> file) {
		return FileProcHolder::hold (instance ())->erase_file (file) ;
	}

	static void build_dire (CREF<String<STR>> dire) {
		return FileProcHolder::hold (instance ())->build_dire (dire) ;
	}

	static void clear_dire (CREF<String<STR>> dire) {
		return FileProcHolder::hold (instance ())->clear_dire (dire) ;
	}

	static void erase_dire (CREF<String<STR>> dire) {
		return FileProcHolder::hold (instance ())->erase_dire (dire) ;
	}

	static BOOL lock_dire (CREF<String<STR>> dire) {
		return FileProcHolder::hold (instance ())->lock_dire (dire) ;
	}
} ;

struct StreamFileImplLayout ;

struct StreamFileLayout implement ThisLayout<AutoRef<StreamFileImplLayout>> {} ;

struct StreamFileHolder implement Interface {
	imports VFat<StreamFileHolder> hold (VREF<StreamFileLayout> that) ;
	imports CFat<StreamFileHolder> hold (CREF<StreamFileLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w (CREF<LENGTH> size_) = 0 ;
	virtual void open_a () = 0 ;
	virtual LENGTH file_size () const = 0 ;
	virtual void read (VREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void write (CREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

using STREAMFILE_BUF_SIZE = ENUM<65536> ;

class StreamFile implement StreamFileLayout {
protected:
	using StreamFileLayout::mThis ;

public:
	implicit StreamFile () = default ;

	explicit StreamFile (CREF<String<STR>> file) {
		StreamFileHolder::hold (thiz)->initialize (file) ;
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

struct StreamFileByteWriterImplLayout ;

struct StreamFileByteWriterLayout implement ThisLayout<AutoRef<StreamFileByteWriterImplLayout>> {} ;

struct StreamFileByteWriterHolder implement Interface {
	imports VFat<StreamFileByteWriterHolder> hold (VREF<StreamFileByteWriterLayout> that) ;
	imports CFat<StreamFileByteWriterHolder> hold (CREF<StreamFileByteWriterLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual VREF<ByteWriter> self_m () leftvalue = 0 ;
	virtual CREF<ByteWriter> self_m () const leftvalue = 0 ;
	virtual void flush () = 0 ;
} ;

class StreamFileByteWriter implement StreamFileByteWriterLayout {
protected:
	using StreamFileByteWriterLayout::mThis ;

public:
	implicit StreamFileByteWriter () = default ;

	explicit StreamFileByteWriter (CREF<String<STR>> file) {
		StreamFileByteWriterHolder::hold (thiz)->initialize (file) ;
	}

	VREF<ByteWriter> self_m () leftvalue {
		return StreamFileByteWriterHolder::hold (thiz)->self ;
	}

	forceinline operator VREF<ByteWriter> () leftvalue {
		return self ;
	}

	CREF<ByteWriter> self_m () const leftvalue {
		return StreamFileByteWriterHolder::hold (thiz)->self ;
	}

	forceinline operator CREF<ByteWriter> () const leftvalue {
		return self ;
	}

	void flush () {
		StreamFileByteWriterHolder::hold (thiz)->flush () ;
	}
} ;

struct StreamFileTextWriterImplLayout ;

struct StreamFileTextWriterLayout implement ThisLayout<AutoRef<StreamFileTextWriterImplLayout>> {} ;

struct StreamFileTextWriterHolder implement Interface {
	imports VFat<StreamFileTextWriterHolder> hold (VREF<StreamFileTextWriterLayout> that) ;
	imports CFat<StreamFileTextWriterHolder> hold (CREF<StreamFileTextWriterLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual VREF<TextWriter> self_m () leftvalue = 0 ;
	virtual CREF<TextWriter> self_m () const leftvalue = 0 ;
	virtual void flush () = 0 ;
} ;

class StreamFileTextWriter implement StreamFileTextWriterLayout {
protected:
	using StreamFileTextWriterLayout::mThis ;

public:
	implicit StreamFileTextWriter () = default ;

	explicit StreamFileTextWriter (CREF<String<STR>> file) {
		StreamFileTextWriterHolder::hold (thiz)->initialize (file) ;
	}

	VREF<TextWriter> self_m () leftvalue {
		return StreamFileTextWriterHolder::hold (thiz)->self ;
	}

	forceinline operator VREF<TextWriter> () leftvalue {
		return self ;
	}

	CREF<TextWriter> self_m () const leftvalue {
		return StreamFileTextWriterHolder::hold (thiz)->self ;
	}

	forceinline operator CREF<TextWriter> () const leftvalue {
		return self ;
	}

	void flush () {
		StreamFileTextWriterHolder::hold (thiz)->flush () ;
	}
} ;

struct BufferFileImplLayout ;

struct BufferFileLayout implement ThisLayout<AutoRef<BufferFileImplLayout>> {} ;

struct BufferFileHolder implement Interface {
	imports VFat<BufferFileHolder> hold (VREF<BufferFileLayout> that) ;
	imports CFat<BufferFileHolder> hold (CREF<BufferFileLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
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
		BufferFileHolder::hold (thiz)->initialize (file) ;
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

struct UartFileImplLayout ;

struct UartFileLayout implement ThisLayout<AutoRef<UartFileImplLayout>> {} ;

struct UartFileHolder implement Interface {
	imports VFat<UartFileHolder> hold (VREF<UartFileLayout> that) ;
	imports CFat<UartFileHolder> hold (CREF<UartFileLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_port_name (CREF<String<STR>> name) = 0 ;
	virtual void set_port_rate (CREF<LENGTH> rate) = 0 ;
	virtual void set_ring_size (CREF<LENGTH> size_) = 0 ;
	virtual void open () = 0 ;
	virtual void read (VREF<RefBuffer<BYTE>> buffer ,CREF<INDEX> offset ,CREF<LENGTH> size_) = 0 ;
} ;

class UartFile implement UartFileLayout {
protected:
	using UartFileLayout::mThis ;

public:
	implicit UartFile () = default ;

	implicit UartFile (CREF<typeof (NULL)>) {
		UartFileHolder::hold (thiz)->initialize () ;
	}

	void set_port_name (CREF<String<STR>> name) {
		return UartFileHolder::hold (thiz)->set_port_name (name) ;
	}

	void set_port_rate (CREF<LENGTH> rate) {
		return UartFileHolder::hold (thiz)->set_port_rate (rate) ;
	}

	void set_ring_size (CREF<LENGTH> size_) {
		return UartFileHolder::hold (thiz)->set_ring_size (size_) ;
	}

	void open () {
		return UartFileHolder::hold (thiz)->open () ;
	}

	void read (VREF<RefBuffer<BYTE>> buffer ,CREF<INDEX> offset ,CREF<LENGTH> size_) {
		return UartFileHolder::hold (thiz)->read (buffer ,offset ,size_) ;
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
	imports CREF<ConsoleLayout> instance () ;
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
	static CREF<Console> instance () {
		return keep[TYPE<Console>::expr] (ConsoleHolder::instance ()) ;
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