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
		ret.mThis = External<FileProcHolder ,FileProcLayout>::create () ;
		FileProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FileProcHolder> FileProcHolder::hold (VREF<FileProcImplLayout> that) {
	return VFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::declare () ,that) ;
}

exports CFat<FileProcHolder> FileProcHolder::hold (CREF<FileProcImplLayout> that) {
	return CFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::declare () ,that) ;
}

template class External<StreamFileHolder ,StreamFileLayout> ;

exports StreamFileLayout StreamFileHolder::create () {
	StreamFileLayout ret ;
	ret.mThis = External<StreamFileHolder ,StreamFileLayout>::create () ;
	return move (ret) ;
}

exports VFat<StreamFileHolder> StreamFileHolder::hold (VREF<StreamFileImplLayout> that) {
	return VFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::declare () ,that) ;
}

exports CFat<StreamFileHolder> StreamFileHolder::hold (CREF<StreamFileImplLayout> that) {
	return CFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::declare () ,that) ;
}

struct StreamFileByteWriterImplLayout {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mFileBuffer ;
	ByteWriter mFileWriter ;

public:
	implicit StreamFileByteWriterImplLayout () = default ;

	implicit ~StreamFileByteWriterImplLayout () noexcept {
		StreamFileByteWriterHolder::hold (thiz)->flush () ;
	}
} ;

class StreamFileByteWriterImplHolder final implement Fat<StreamFileByteWriterHolder ,StreamFileByteWriterImplLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mStreamFile = StreamFile (file) ;
		fake.mStreamFile.open_w (0) ;
		fake.mFileBuffer = RefBuffer<BYTE> (STREAMFILE_CHUNK_STEP::expr) ;
		set_writer () ;
	}

	void set_writer () {
		fake.mFileWriter = ByteWriter (Ref<RefBuffer<BYTE>>::reference (fake.mFileBuffer)) ;
		auto &&rax = fake ;
		fake.mFileWriter.use_overflow ([&] (VREF<ByteWriter> writer) {
			rax.mStreamFile.write (rax.mFileBuffer) ;
			rax.mFileWriter.reset () ;
		}) ;
	}

	VREF<ByteWriter> self_m () leftvalue override {
		return fake.mFileWriter ;
	}

	void flush () override {
		const auto r1x = fake.mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (fake.mFileBuffer.self) ;
		fake.mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		fake.mFileWriter.reset () ;
		fake.mStreamFile.flush () ;
	}
} ;

exports StreamFileByteWriterLayout StreamFileByteWriterHolder::create () {
	StreamFileByteWriterLayout ret ;
	ret.mThis = AutoRef<StreamFileByteWriterImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (VREF<StreamFileByteWriterImplLayout> that) {
	return VFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

exports CFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (CREF<StreamFileByteWriterImplLayout> that) {
	return CFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

struct StreamFileTextWriterImplLayout {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mFileBuffer ;
	TextWriter mFileWriter ;

public:
	implicit StreamFileTextWriterImplLayout () = default ;

	implicit ~StreamFileTextWriterImplLayout () noexcept {
		StreamFileTextWriterHolder::hold (thiz)->flush () ;
	}
} ;

class StreamFileTextWriterImplHolder final implement Fat<StreamFileTextWriterHolder ,StreamFileTextWriterImplLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mStreamFile = StreamFile (file) ;
		fake.mStreamFile.open_w (0) ;
		fake.mFileBuffer = RefBuffer<BYTE> (STREAMFILE_CHUNK_STEP::expr) ;
		set_writer () ;
	}

	void set_writer () {
		fake.mFileWriter = TextWriter (Ref<RefBuffer<BYTE>>::reference (fake.mFileBuffer)) ;
		auto &&rax = fake ;
		fake.mFileWriter.use_overflow ([&] (VREF<TextWriter> writer) {
			rax.mStreamFile.write (rax.mFileBuffer) ;
			rax.mFileWriter.reset () ;
		}) ;
	}

	VREF<TextWriter> self_m () leftvalue override {
		return fake.mFileWriter ;
	}

	void flush () override {
		const auto r1x = fake.mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (fake.mFileBuffer.self) ;
		fake.mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		fake.mFileWriter.reset () ;
		fake.mStreamFile.flush () ;
	}
} ;

exports StreamFileTextWriterLayout StreamFileTextWriterHolder::create () {
	StreamFileTextWriterLayout ret ;
	ret.mThis = AutoRef<StreamFileTextWriterImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (VREF<StreamFileTextWriterImplLayout> that) {
	return VFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

exports CFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (CREF<StreamFileTextWriterImplLayout> that) {
	return CFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

template class External<BufferFileHolder ,BufferFileLayout> ;

exports BufferFileLayout BufferFileHolder::create () {
	BufferFileLayout ret ;
	ret.mThis = External<BufferFileHolder ,BufferFileLayout>::create () ;
	return move (ret) ;
}

exports VFat<BufferFileHolder> BufferFileHolder::hold (VREF<BufferFileImplLayout> that) {
	return VFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::declare () ,that) ;
}

exports CFat<BufferFileHolder> BufferFileHolder::hold (CREF<BufferFileImplLayout> that) {
	return CFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::declare () ,that) ;
}

template class External<UartFileHolder ,UartFileLayout> ;

exports UartFileLayout UartFileHolder::create () {
	UartFileLayout ret ;
	ret.mThis = External<UartFileHolder ,UartFileLayout>::create () ;
	return move (ret) ;
}

exports VFat<UartFileHolder> UartFileHolder::hold (VREF<UartFileImplLayout> that) {
	return VFat<UartFileHolder> (External<UartFileHolder ,UartFileLayout>::declare () ,that) ;
}

exports CFat<UartFileHolder> UartFileHolder::hold (CREF<UartFileImplLayout> that) {
	return CFat<UartFileHolder> (External<UartFileHolder ,UartFileLayout>::declare () ,that) ;
}

template class External<ConsoleHolder ,ConsoleLayout> ;

exports CREF<ConsoleLayout> ConsoleHolder::instance () {
	return memorize ([&] () {
		ConsoleLayout ret ;
		ret.mThis = External<ConsoleHolder ,ConsoleLayout>::create () ;
		ConsoleHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ConsoleHolder> ConsoleHolder::hold (VREF<ConsoleImplLayout> that) {
	return VFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::declare () ,that) ;
}

exports CFat<ConsoleHolder> ConsoleHolder::hold (CREF<ConsoleImplLayout> that) {
	return CFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::declare () ,that) ;
}
} ;