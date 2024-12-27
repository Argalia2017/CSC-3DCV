#pragma once

#ifndef __CSC_FILE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_file.hpp"

#include "csc_end.h"
#include <cstdio>
#include "csc_begin.h"

namespace CSC {
template class External<PathHolder ,PathLayout> ;

exports VFat<PathHolder> PathHolder::hold (VREF<PathLayout> that) {
	return VFat<PathHolder> (External<PathHolder ,PathLayout>::declare () ,that) ;
}

exports CFat<PathHolder> PathHolder::hold (CREF<PathLayout> that) {
	return CFat<PathHolder> (External<PathHolder ,PathLayout>::declare () ,that) ;
}

template class External<FileProcHolder ,FileProcLayout> ;

exports CREF<FileProcLayout> FileProcHolder::instance () {
	return memorize ([&] () {
		FileProcLayout ret ;
		FileProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FileProcHolder> FileProcHolder::hold (VREF<FileProcLayout> that) {
	return VFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::declare () ,that) ;
}

exports CFat<FileProcHolder> FileProcHolder::hold (CREF<FileProcLayout> that) {
	return CFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::declare () ,that) ;
}

template class External<StreamFileHolder ,StreamFileLayout> ;

exports VFat<StreamFileHolder> StreamFileHolder::hold (VREF<StreamFileLayout> that) {
	return VFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::declare () ,that) ;
}

exports CFat<StreamFileHolder> StreamFileHolder::hold (CREF<StreamFileLayout> that) {
	return CFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::declare () ,that) ;
}

struct StreamFileByteWriterImplLayout {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mFileBuffer ;
	ByteWriter mFileWriter ;
} ;

class StreamFileByteWriterImpl implement StreamFileByteWriterImplLayout {
protected:
	using StreamFileByteWriterImplLayout::mStreamFile ;
	using StreamFileByteWriterImplLayout::mFileBuffer ;
	using StreamFileByteWriterImplLayout::mFileWriter ;

public:
	implicit StreamFileByteWriterImpl () = default ;

	implicit ~StreamFileByteWriterImpl () noexcept {
		flush () ;
	}

	void initialize (CREF<String<STR>> file) {
		mStreamFile = StreamFile (file) ;
		mStreamFile.open_w (0) ;
		mFileBuffer = RefBuffer<BYTE> (STREAMFILE_BUF_SIZE::expr) ;
		set_writer () ;
	}

	void set_writer () {
		mFileWriter = ByteWriter (Ref<RefBuffer<BYTE>>::reference (mFileBuffer)) ;
		mFileWriter.use_overflow ([&] (VREF<ByteWriter> writer) {
			mStreamFile.write (mFileBuffer) ;
			mFileWriter.reset () ;
		}) ;
	}

	void flush () {
		const auto r1x = mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (mFileBuffer.self) ;
		mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		mFileWriter.reset () ;
		mStreamFile.flush () ;
	}
} ;

class StreamFileByteWriterImplHolder final implement Fat<StreamFileByteWriterHolder ,StreamFileByteWriterLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mThis = AutoRef<StreamFileByteWriterImpl>::make () ;
		ptr (fake).initialize (file) ;
	}

	static VREF<StreamFileByteWriterImpl> ptr (VREF<StreamFileByteWriterLayout> layout) {
		return keep[TYPE<StreamFileByteWriterImpl>::expr] (layout.mThis.self) ;
	}

	VREF<ByteWriter> self_m () leftvalue override {
		return fake.mThis->mFileWriter ;
	}

	void flush () override {
		return ptr (fake).flush () ;
	}
} ;

exports VFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (VREF<StreamFileByteWriterLayout> that) {
	return VFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

exports CFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (CREF<StreamFileByteWriterLayout> that) {
	return CFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

struct StreamFileTextWriterImplLayout {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mFileBuffer ;
	TextWriter mFileWriter ;
} ;

class StreamFileTextWriterImpl implement StreamFileTextWriterImplLayout {
protected:
	using StreamFileTextWriterImplLayout::mStreamFile ;
	using StreamFileTextWriterImplLayout::mFileBuffer ;
	using StreamFileTextWriterImplLayout::mFileWriter ;

public:
	implicit StreamFileTextWriterImpl () = default ;

	implicit ~StreamFileTextWriterImpl () noexcept {
		flush () ;
	}

	void initialize (CREF<String<STR>> file) {
		mStreamFile = StreamFile (file) ;
		mStreamFile.open_w (0) ;
		mFileBuffer = RefBuffer<BYTE> (STREAMFILE_BUF_SIZE::expr) ;
		set_writer () ;
	}

	void set_writer () {
		mFileWriter = TextWriter (Ref<RefBuffer<BYTE>>::reference (mFileBuffer)) ;
		mFileWriter.use_overflow ([&] (VREF<TextWriter> writer) {
			mStreamFile.write (mFileBuffer) ;
			mFileWriter.reset () ;
		}) ;
	}

	void flush () {
		const auto r1x = mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (mFileBuffer.self) ;
		mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		mFileWriter.reset () ;
		mStreamFile.flush () ;
	}
} ;

class StreamFileTextWriterImplHolder final implement Fat<StreamFileTextWriterHolder ,StreamFileTextWriterLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mThis = AutoRef<StreamFileTextWriterImpl>::make () ;
		ptr (fake).initialize (file) ;
	}

	static VREF<StreamFileTextWriterImpl> ptr (VREF<StreamFileTextWriterLayout> layout) {
		return keep[TYPE<StreamFileTextWriterImpl>::expr] (layout.mThis.self) ;
	}

	VREF<TextWriter> self_m () leftvalue override {
		return fake.mThis->mFileWriter ;
	}

	void flush () override {
		return ptr (fake).flush () ;
	}
} ;

exports VFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (VREF<StreamFileTextWriterLayout> that) {
	return VFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

exports CFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (CREF<StreamFileTextWriterLayout> that) {
	return CFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

template class External<BufferFileHolder ,BufferFileLayout> ;

exports VFat<BufferFileHolder> BufferFileHolder::hold (VREF<BufferFileLayout> that) {
	return VFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::declare () ,that) ;
}

exports CFat<BufferFileHolder> BufferFileHolder::hold (CREF<BufferFileLayout> that) {
	return CFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::declare () ,that) ;
}

template class External<UartFileHolder ,UartFileLayout> ;

exports VFat<UartFileHolder> UartFileHolder::hold (VREF<UartFileLayout> that) {
	return VFat<UartFileHolder> (External<UartFileHolder ,UartFileLayout>::declare () ,that) ;
}

exports CFat<UartFileHolder> UartFileHolder::hold (CREF<UartFileLayout> that) {
	return CFat<UartFileHolder> (External<UartFileHolder ,UartFileLayout>::declare () ,that) ;
}

template class External<ConsoleHolder ,ConsoleLayout> ;

exports CREF<ConsoleLayout> ConsoleHolder::instance () {
	return memorize ([&] () {
		ConsoleLayout ret ;
		ConsoleHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ConsoleHolder> ConsoleHolder::hold (VREF<ConsoleLayout> that) {
	return VFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::declare () ,that) ;
}

exports CFat<ConsoleHolder> ConsoleHolder::hold (CREF<ConsoleLayout> that) {
	return CFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::declare () ,that) ;
}
} ;