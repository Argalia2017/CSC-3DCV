#pragma once

#ifndef __CSC_STREAM__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_stream.hpp"

#include "csc_end.h"
#include <regex>
#include "csc_begin.h"

namespace CSC {
struct StreamProcImplLayout {
	Slice mBlankSlice ;
	Slice mPunctSlice ;
	Slice mAlphaSlice ;
	Slice mDigitSlice ;
	Slice mEscapeWordSlice ;
	Slice mEscapeCtrlSlice ;
} ;

class StreamProcImplHolder final implement Fat<StreamProcHolder ,StreamProcImplLayout> {
public:
	void initialize () override {
		fake.mBlankSlice = slice ("\b\t\n\v\f\r ") ;
		fake.mPunctSlice = slice ("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~") ;
		fake.mAlphaSlice = slice ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") ;
		fake.mDigitSlice = slice ("0123456789") ;
		fake.mEscapeWordSlice = slice ("\\/tvbrnf\'\"?u") ;
		fake.mEscapeCtrlSlice = slice ("\\/\t\v\b\r\n\f\'\"?\a") ;
	}

	BOOL big_endian () const override {
		const auto r1x = WORD (0X00FF) ;
		const auto r2x = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<WORD>>>::expr] (r1x) ;
		return r2x[0] == BYTE (0X00) ;
	}

	BOOL is_blank (CREF<STRU32> str) const override {
		const auto r1x = fake.mBlankSlice ;
		for (auto &&i : iter (0 ,r1x.size ())) {
			if (r1x[i] == str)
				return TRUE ;
		}
		return FALSE ;
	}

	BOOL is_space (CREF<STRU32> str) const override {
		if (str == STRU32 (' '))
			return TRUE ;
		if (str == STRU32 ('\t'))
			return TRUE ;
		if (str == STRU32 ('\b'))
			return TRUE ;
		return FALSE ;
	}

	BOOL is_endline (CREF<STRU32> str) const override {
		if (str == STRU32 ('\r'))
			return TRUE ;
		if (str == STRU32 ('\n'))
			return TRUE ;
		if (str == STRU32 ('\v'))
			return TRUE ;
		if (str == STRU32 ('\f'))
			return TRUE ;
		return FALSE ;
	}

	BOOL is_punct (CREF<STRU32> str) const override {
		const auto r1x = fake.mPunctSlice ;
		for (auto &&i : iter (0 ,r1x.size ())) {
			if (r1x[i] == str)
				return TRUE ;
		}
		return FALSE ;
	}

	BOOL is_hyphen (CREF<STRU32> str) const override {
		if (str == STRU32 ('-'))
			return TRUE ;
		if (str == STRU32 (':'))
			return TRUE ;
		if (str == STRU32 ('.'))
			return TRUE ;
		return FALSE ;
	}

	BOOL is_alpha (CREF<STRU32> str) const override {
		if (str >= STRU32 ('a'))
			if (str <= STRU32 ('z'))
				return TRUE ;
		if (str >= STRU32 ('A'))
			if (str <= STRU32 ('Z'))
				return TRUE ;
		return FALSE ;
	}

	STRU32 alpha_lower (CREF<STRU32> str) const override {
		if (str >= STRU32 ('A'))
			if (str <= STRU32 ('Z'))
				return str - STRU32 ('A') + STRU32 ('a') ;
		return str ;
	}

	STRU32 alpha_upper (CREF<STRU32> str) const override {
		if (str >= STRU32 ('a'))
			if (str <= STRU32 ('z'))
				return str - STRU32 ('a') + STRU32 ('A') ;
		return str ;
	}

	BOOL is_digit (CREF<STRU32> str) const override {
		if (str >= STRU32 ('0'))
			if (str <= STRU32 ('9'))
				return TRUE ;
		return FALSE ;
	}

	BOOL is_hex_digit (CREF<STRU32> str) const override {
		if (str >= STRU32 ('a'))
			if (str <= STRU32 ('f'))
				return TRUE ;
		if (str >= STRU32 ('A'))
			if (str <= STRU32 ('F'))
				return TRUE ;
		return FALSE ;
	}

	INDEX hex_from_str (CREF<STRU32> str) const override {
		if (is_digit (str))
			return INDEX (str - STRU32 ('0')) ;
		if (is_hex_digit (str))
			return INDEX (alpha_upper (str) - STRU32 ('A') + 10) ;
		assume (FALSE) ;
		return INDEX () ;
	}

	STRU32 str_from_hex (CREF<INDEX> hex) const override {
		if (inline_between (hex ,0 ,10))
			return STRU32 ('0') + STRU32 (hex) ;
		if (inline_between (hex ,10 ,16))
			return STRU32 ('A') + STRU32 (hex) - 10 ;
		assume (FALSE) ;
		return STRU32 () ;
	}

	BOOL is_word (CREF<STRU32> str) const override {
		if (is_alpha (str))
			return TRUE ;
		if (is_digit (str))
			return TRUE ;
		if (str == STRU32 ('_'))
			return TRUE ;
		return FALSE ;
	}

	BOOL is_ctrl (CREF<STRU32> str) const override {
		const auto r1x = fake.mEscapeCtrlSlice ;
		for (auto &&i : iter (0 ,r1x.size ())) {
			if (r1x[i] == str)
				return TRUE ;
		}
		return FALSE ;
	}

	STRU32 word_from_ctrl (CREF<STRU32> str) const override {
		const auto r1x = fake.mEscapeWordSlice ;
		const auto r2x = fake.mEscapeCtrlSlice ;
		for (auto &&i : iter (0 ,r2x.size ())) {
			if (r2x[i] == str)
				return r1x[i] ;
		}
		assume (FALSE) ;
		return STRU32 ('?') ;
	}

	STRU32 ctrl_from_word (CREF<STRU32> str) const override {
		const auto r1x = fake.mEscapeWordSlice ;
		const auto r2x = fake.mEscapeCtrlSlice ;
		for (auto &&i : iter (0 ,r1x.size ())) {
			if (r1x[i] == str)
				return r2x[i] ;
		}
		assume (FALSE) ;
		return STRU32 ('?') ;
	}
} ;

exports CREF<StreamProcLayout> StreamProcHolder::instance () {
	return memorize ([&] () {
		StreamProcLayout ret ;
		ret.mThis = UniqueRef<StreamProcImplLayout>::make () ;
		StreamProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<StreamProcHolder> StreamProcHolder::hold (VREF<StreamProcImplLayout> that) {
	return VFat<StreamProcHolder> (StreamProcImplHolder () ,that) ;
}

exports CFat<StreamProcHolder> StreamProcHolder::hold (CREF<StreamProcImplLayout> that) {
	return CFat<StreamProcHolder> (StreamProcImplHolder () ,that) ;
}

class ByteReaderImplHolder final implement Fat<ByteReaderHolder ,ByteReaderLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () == 1) ;
		fake.mStream = move (stream) ;
		fake.mDiffEndian = FALSE ;
		reset () ;
	}

	void use_overflow (CREF<Function<VREF<ByteReaderLayout>>> overflow) override {
		fake.mOverflow = overflow ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	LENGTH length () const override {
		return fake.mRead ;
	}

	void reset () override {
		fake.mRead = 0 ;
		fake.mWrite = fake.mStream->size () ;
	}

	void reset (CREF<StreamShape> shape) override {
		fake.mRead = shape.mRead ;
		fake.mWrite = shape.mWrite ;
	}

	StreamShape backup () const override {
		StreamShape ret ;
		ret.mRead = fake.mRead ;
		ret.mWrite = fake.mWrite ;
		return move (ret) ;
	}

	BOOL good () const override {
		return length () < size () ;
	}

	void read (VREF<BOOL> item) override {
		auto rax = BYTE_BASE<BOOL> () ;
		read (rax) ;
		item = bitwise[TYPE<BOOL>::expr] (rax) ;
	}

	void read (VREF<VAL32> item) override {
		auto rax = BYTE_BASE<VAL32> () ;
		read (rax) ;
		item = bitwise[TYPE<VAL32>::expr] (rax) ;
	}

	void read (VREF<VAL64> item) override {
		auto rax = BYTE_BASE<VAL64> () ;
		read (rax) ;
		item = bitwise[TYPE<VAL64>::expr] (rax) ;
	}

	void read (VREF<FLT32> item) override {
		auto rax = BYTE_BASE<FLT32> () ;
		read (rax) ;
		item = bitwise[TYPE<FLT32>::expr] (rax) ;
	}

	void read (VREF<FLT64> item) override {
		auto rax = BYTE_BASE<FLT64> () ;
		read (rax) ;
		item = bitwise[TYPE<FLT64>::expr] (rax) ;
	}

	void read (VREF<BYTE> item) override {
		if ifdo (TRUE) {
			if (fake.mRead < fake.mWrite)
				discard ;
			fake.mOverflow (fake) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mRead >= fake.mWrite)
				discard ;
			item = fake.mStream.self[fake.mRead] ;
			fake.mRead++ ;
		}
		if ifdo (act) {
			item = BYTE (0X00) ;
		}
	}

	void read (VREF<WORD> item) override {
		read_byte_impl (item) ;
	}

	void read (VREF<CHAR> item) override {
		read_byte_impl (item) ;
	}

	void read (VREF<QUAD> item) override {
		read_byte_impl (item) ;
	}

	template <class ARG1>
	forceinline void read_byte_impl (VREF<ARG1> item) {
		auto rax = Buffer<BYTE ,SIZE_OF<ARG1>> () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			read (rax[i]) ;
		}
		item = bitwise[TYPE<ARG1>::expr] (rax) ;
		if ifdo (TRUE) {
			if (!fake.mDiffEndian)
				discard ;
			item = ByteProc::reverse (item) ;
		}
	}

	void read (VREF<STRU32> item) override {
		item = 0 ;
		auto rax = BYTE () ;
		INDEX ix = 0 ;
		while (TRUE) {
			read (rax) ;
			const auto r1x = STRU32 (rax) ;
			if (r1x < STRU32 (0X80))
				break ;
			item |= STRU32 (rax & BYTE (0X7F)) << ix ;
			ix += 7 ;
		}
		item |= STRU32 (rax) << ix ;
	}

	void read (CREF<Slice> item) override {
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,item.size ())) {
			assume (inline_between (INDEX (item[i]) ,0 ,128)) ;
			read (rax) ;
			assume (rax == item[i]) ;
		}
	}

	void read (VREF<String<STRA>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRW>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU8>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU16>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU32>> item) override {
		read_string_impl (item) ;
	}

	template <class ARG1>
	forceinline void read_string_impl (VREF<String<ARG1>> item) {
		item.clear () ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			item[i] = ARG1 (rax) ;
		}
	}

	void read (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void read (CREF<typeof (BOM)>) override {
		fake.mDiffEndian = !fake.mDiffEndian ;
	}

	void read (CREF<typeof (GAP)>) override {
		auto rax = BYTE () ;
		read (rax) ;
		assume (rax == BYTE (0X5D)) ;
		read (rax) ;
		assume (rax == BYTE (0X5B)) ;
	}

	void read (CREF<typeof (EOS)>) override {
		auto rax = BYTE () ;
		while (TRUE) {
			if (fake.mRead >= fake.mWrite)
				break ;
			read (rax) ;
			assume (rax == BYTE (0X00)) ;
		}
	}
} ;

exports VFat<ByteReaderHolder> ByteReaderHolder::hold (VREF<ByteReaderLayout> that) {
	return VFat<ByteReaderHolder> (ByteReaderImplHolder () ,that) ;
}

exports CFat<ByteReaderHolder> ByteReaderHolder::hold (CREF<ByteReaderLayout> that) {
	return CFat<ByteReaderHolder> (ByteReaderImplHolder () ,that) ;
}

class TextReaderImplHolder final implement Fat<TextReaderHolder ,TextReaderLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () <= 4) ;
		fake.mStream = move (stream) ;
		fake.mDiffEndian = FALSE ;
		reset () ;
	}

	void use_overflow (CREF<Function<VREF<TextReaderLayout>>> overflow) override {
		fake.mOverflow = overflow ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	LENGTH length () const override {
		return fake.mRead ;
	}

	void reset () override {
		fake.mRead = 0 ;
		fake.mWrite = fake.mStream->size () ;
	}

	void reset (CREF<StreamShape> shape) override {
		fake.mRead = shape.mRead ;
		fake.mWrite = shape.mWrite ;
	}

	StreamShape backup () const override {
		StreamShape ret ;
		ret.mRead = fake.mRead ;
		ret.mWrite = fake.mWrite ;
		return move (ret) ;
	}

	BOOL good () const override {
		return length () < size () ;
	}

	void read (VREF<BOOL> item) override {
		auto rax = STRU32 () ;
		read (rax) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (rax != STRU32 ('t'))
				discard ;
			push (rax) ;
			read (slice ("true")) ;
			item = TRUE ;
		}
		if ifdo (act) {
			if (rax != STRU32 ('T'))
				discard ;
			push (rax) ;
			read (slice ("TRUE")) ;
			item = TRUE ;
		}
		if ifdo (act) {
			if (rax != STRU32 ('f'))
				discard ;
			push (rax) ;
			read (slice ("false")) ;
			item = FALSE ;
		}
		if ifdo (act) {
			if (rax != STRU32 ('F'))
				discard ;
			push (rax) ;
			read (slice ("FALSE")) ;
			item = FALSE ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	void read (VREF<VAL32> item) override {
		auto rax = VAL64 () ;
		read (rax) ;
		assume (rax >= VAL32_MIN) ;
		assume (rax <= VAL32_MAX) ;
		item = VAL32 (rax) ;
	}

	void read (VREF<VAL64> item) override {
		auto rax = STRU32 () ;
		read (rax) ;
		const auto r1x = BOOL (rax == STRU32 ('-')) ;
		if ifdo (TRUE) {
			if (rax != STRU32 ('-'))
				if (rax != STRU32 ('+'))
					discard ;
			read (rax) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			assume (StreamProc::is_digit (rax)) ;
			auto rbx = Notation () ;
			rbx.mRadix = 10 ;
			rbx.mPrecision = 0 ;
			rbx.mSign = FALSE ;
			rbx.mMantissa = 0 ;
			rbx.mDownflow = 0 ;
			rbx.mExponent = 0 ;
			read_value (rbx ,rax) ;
			assume (rbx.mMantissa >= 0) ;
			item = rbx.mMantissa ;
		}
		if ifdo (TRUE) {
			if (!r1x)
				discard ;
			item = -item ;
		}
		push (rax) ;
	}

	void read_value (VREF<Notation> fexp10 ,VREF<STRU32> top) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = FloatProc::value_precision () ;
		if ifdo (TRUE) {
			while (TRUE) {
				if (!StreamProc::is_digit (top))
					break ;
				if (fexp10.mPrecision > r1x - 1)
					break ;
				fexp10.mMantissa *= 10 ;
				fexp10.mMantissa += StreamProc::hex_from_str (top) ;
				fexp10.mPrecision++ ;
				read (top) ;
			}
			if ifdo (TRUE) {
				if (!StreamProc::is_digit (top))
					discard ;
				const auto r2x = fexp10.mMantissa * 10 + StreamProc::hex_from_str (top) ;
				if (r2x < 0)
					discard ;
				fexp10.mMantissa = r2x ;
				fexp10.mPrecision++ ;
				read (top) ;
			}
			while (TRUE) {
				if (!StreamProc::is_digit (top))
					break ;
				fexp10.mExponent++ ;
				read (top) ;
			}
		}
	}

	void read (VREF<FLT32> item) override {
		auto rax = FLT64 () ;
		read (rax) ;
		assume (rax >= FLT32_MIN) ;
		assume (rax <= FLT32_MAX) ;
		item = FLT32 (rax) ;
	}

	void read (VREF<FLT64> item) override {
		auto rax = STRU32 () ;
		read (rax) ;
		const auto r1x = BOOL (rax == STRU32 ('-')) ;
		if ifdo (TRUE) {
			if (rax != STRU32 ('-'))
				if (rax != STRU32 ('+'))
					discard ;
			read (rax) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			assume (StreamProc::is_digit (rax)) ;
			auto rbx = Notation () ;
			rbx.mRadix = 10 ;
			rbx.mPrecision = 0 ;
			rbx.mSign = FALSE ;
			rbx.mMantissa = 0 ;
			rbx.mDownflow = 0 ;
			rbx.mExponent = 0 ;
			read_float (rbx ,rax) ;
			assume (rbx.mMantissa >= 0) ;
			rbx = FloatProc::fexp2_from_fexp10 (rbx) ;
			item = FloatProc::encode (rbx) ;
		}
		if ifdo (TRUE) {
			if (!r1x)
				discard ;
			item = -item ;
		}
		push (rax) ;
	}

	void read_float (VREF<Notation> fexp10 ,VREF<STRU32> top) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = FloatProc::value_precision () ;
		read_value (fexp10 ,top) ;
		if ifdo (TRUE) {
			if (top != STRU32 ('.'))
				discard ;
			read (top) ;
			while (TRUE) {
				if (!StreamProc::is_digit (top))
					break ;
				if (fexp10.mPrecision > r1x - 1)
					break ;
				fexp10.mMantissa *= 10 ;
				fexp10.mMantissa += StreamProc::hex_from_str (top) ;
				fexp10.mExponent-- ;
				fexp10.mPrecision++ ;
				read (top) ;
			}
			while (TRUE) {
				if (!StreamProc::is_digit (top))
					break ;
				read (top) ;
			}
		}
		if ifdo (TRUE) {
			if (top != STRU32 ('e'))
				if (top != STRU32 ('E'))
					discard ;
			read (top) ;
			const auto r2x = BOOL (top == STRU32 ('-')) ;
			if ifdo (TRUE) {
				if (top != STRU32 ('-'))
					if (top != STRU32 ('+'))
						discard ;
				read (top) ;
			}
			assume (StreamProc::is_digit (top)) ;
			auto rbx = Notation () ;
			rbx.mRadix = 10 ;
			rbx.mPrecision = 0 ;
			rbx.mSign = r2x ;
			read_value (rbx ,top) ;
			assume (rbx.mExponent == 0) ;
			const auto r3x = invoke ([&] () {
				if (r2x)
					return -rbx.mMantissa ;
				return rbx.mMantissa ;
			}) ;
			fexp10.mExponent += r3x ;
		}
	}

	void read (VREF<BYTE> item) override {
		read_byte_impl (item) ;
	}

	void read (VREF<WORD> item) override {
		read_byte_impl (item) ;
	}

	void read (VREF<CHAR> item) override {
		read_byte_impl (item) ;
	}

	void read (VREF<QUAD> item) override {
		read_byte_impl (item) ;
	}

	template <class ARG1>
	forceinline void read_byte_impl (VREF<ARG1> item) {
		auto rax = STRU32 () ;
		if ifdo (TRUE) {
			read (rax) ;
			assume (rax == STRU32 ('0')) ;
			read (rax) ;
			if (rax == STRU32 ('x'))
				discard ;
			if (rax == STRU32 ('X'))
				discard ;
			assume (FALSE) ;
		}
		item = ARG1 (0X00) ;
		for (auto &&i : iter (0 ,SIZE_OF<ARG1>::expr)) {
			noop (i) ;
			read (rax) ;
			const auto r1x = ARG1 (StreamProc::hex_from_str (rax)) ;
			item = (item << 4) | r1x ;
			read (rax) ;
			const auto r2x = ARG1 (StreamProc::hex_from_str (rax)) ;
			item = (item << 4) | r2x ;
		}
	}

	void read (VREF<STRU32> item) override {
		if ifdo (TRUE) {
			if (fake.mRead < fake.mWrite)
				discard ;
			fake.mOverflow (fake) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU8>::expr)
				discard ;
			if (fake.mRead >= fake.mWrite)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU8>>::expr] (Pointer::from (fake.mStream.self)) ;
			item = rax[fake.mRead] ;
			fake.mRead++ ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU16>::expr)
				discard ;
			if (fake.mRead >= fake.mWrite)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU16>>::expr] (Pointer::from (fake.mStream.self)) ;
			item = rax[fake.mRead] ;
			fake.mRead++ ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU32>::expr)
				discard ;
			if (fake.mRead >= fake.mWrite)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU32>>::expr] (Pointer::from (fake.mStream.self)) ;
			item = rax[fake.mRead] ;
			fake.mRead++ ;
		}
		if ifdo (act) {
			item = STRU32 (0X00) ;
		}
	}

	void push (CREF<STRU32> item) {
		auto act = TRUE ;
		if ifdo (act) {
			if (item != STRU32 (0X00))
				discard ;
			if (fake.mRead < fake.mWrite)
				discard ;
			noop () ;
		}
		if ifdo (act) {
			fake.mRead-- ;
		}
	}

	void read (CREF<Slice> item) override {
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,item.size ())) {
			assume (inline_between (INDEX (item[i]) ,0 ,128)) ;
			read (rax) ;
			assume (rax == item[i]) ;
		}
	}

	void read (VREF<String<STRA>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRW>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU8>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU16>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU32>> item) override {
		read_string_impl (item) ;
	}

	template <class ARG1>
	forceinline void read_string_impl (VREF<String<ARG1>> item) {
		item.clear () ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			item[i] = ARG1 (rax) ;
		}
	}

	void read (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void read (CREF<typeof (BOM)>) override {
		auto rax = STRU32 () ;
		const auto r1x = backup () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU8>::expr)
				discard ;
			read (rax) ;
			if (rax != STRU32 (0XEF))
				discard ;
			read (rax) ;
			if (rax != STRU32 (0XBB))
				discard ;
			read (rax) ;
			if (rax != STRU32 (0XBF))
				discard ;
			noop (rax) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU16>::expr)
				discard ;
			read (rax) ;
			if (rax != STRU32 (0XFEFF))
				if (rax != STRU32 (0XFFFE))
					discard ;
			fake.mDiffEndian = rax != STRU32 (0XFEFF) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU32>::expr)
				discard ;
			read (rax) ;
			if (rax != STRU32 (0X0000FEFF))
				if (rax != STRU32 (0XFFFE0000))
					discard ;
			fake.mDiffEndian = rax != STRU32 (0X0000FEFF) ;
		}
		if ifdo (act) {
			reset (r1x) ;
		}
	}

	void read (CREF<typeof (GAP)>) override {
		auto rax = STRU32 () ;
		read (rax) ;
		while (TRUE) {
			if (rax == STRU32 (0X00))
				break ;
			if (!StreamProc::is_blank (rax))
				break ;
			read (rax) ;
		}
		push (rax) ;
	}

	void read (CREF<typeof (EOS)>) override {
		auto rax = STRU32 () ;
		read (rax) ;
		assume (rax == STRU32 (0X00)) ;
	}
} ;

exports VFat<TextReaderHolder> TextReaderHolder::hold (VREF<TextReaderLayout> that) {
	return VFat<TextReaderHolder> (TextReaderImplHolder () ,that) ;
}

exports CFat<TextReaderHolder> TextReaderHolder::hold (CREF<TextReaderLayout> that) {
	return CFat<TextReaderHolder> (TextReaderImplHolder () ,that) ;
}

class ByteWriterImplHolder final implement Fat<ByteWriterHolder ,ByteWriterLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () == 1) ;
		fake.mStream = move (stream) ;
		assert (fake.mStream.exclusive ()) ;
		fake.mDiffEndian = FALSE ;
		reset () ;
	}

	void use_overflow (CREF<Function<VREF<ByteWriterLayout>>> overflow) override {
		fake.mOverflow = overflow ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	LENGTH length () const override {
		return fake.mWrite ;
	}

	void reset () override {
		fake.mRead = fake.mStream->size () ;
		fake.mWrite = 0 ;
	}

	void reset (CREF<StreamShape> shape) override {
		fake.mRead = shape.mRead ;
		fake.mWrite = shape.mWrite ;
	}

	StreamShape backup () const override {
		StreamShape ret ;
		ret.mRead = fake.mRead ;
		ret.mWrite = fake.mWrite ;
		return move (ret) ;
	}

	BOOL good () const override {
		return length () < size () ;
	}

	void write (CREF<BOOL> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<BOOL>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<VAL32> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<VAL32>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<VAL64> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<VAL64>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<FLT32> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<FLT32>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<FLT64> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<FLT64>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<BYTE> item) override {
		if ifdo (TRUE) {
			if (fake.mWrite < fake.mRead)
				discard ;
			fake.mOverflow (fake) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<BYTE>>::expr] (Pointer::from (fake.mStream.self)) ;
			rax[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void write (CREF<WORD> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<CHAR> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<QUAD> item) override {
		write_byte_impl (item) ;
	}

	template <class ARG1>
	forceinline void write_byte_impl (CREF<ARG1> item) {
		auto rax = item ;
		if ifdo (TRUE) {
			if (!fake.mDiffEndian)
				discard ;
			rax = ByteProc::reverse (rax) ;
		}
		const auto r1x = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<ARG1>>>::expr] (rax) ;
		for (auto &&i : iter (0 ,r1x.size ())) {
			write (r1x[i]) ;
		}
	}

	void write (CREF<STRU32> item) override {
		auto rax = item ;
		while (TRUE) {
			if (rax < STRU32 (0X80))
				break ;
			const auto r1x = (BYTE (rax) & BYTE (0X7F)) | BYTE (0X80) ;
			write (r1x) ;
			rax = rax >> 7 ;
		}
		const auto r2x = BYTE (rax) ;
		write (r2x) ;
	}

	void write (CREF<Slice> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			assume (inline_between (INDEX (item[i]) ,0 ,128)) ;
			write (item[i]) ;
		}
	}

	void write (CREF<String<STRA>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRW>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU8>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU16>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU32>> item) override {
		write_string_impl (item) ;
	}

	template <class ARG1>
	forceinline void write_string_impl (CREF<String<ARG1>> item) {
		const auto r1x = item.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = STRU32 (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void write (CREF<typeof (BOM)>) override {
		fake.mDiffEndian = !fake.mDiffEndian ;
	}

	void write (CREF<typeof (GAP)>) override {
		write (BYTE (0X5D)) ;
		write (BYTE (0X5B)) ;
	}

	void write (CREF<typeof (EOS)>) override {
		while (TRUE) {
			if (fake.mWrite >= fake.mRead)
				break ;
			write (BYTE (0X00)) ;
		}
	}
} ;

exports VFat<ByteWriterHolder> ByteWriterHolder::hold (VREF<ByteWriterLayout> that) {
	return VFat<ByteWriterHolder> (ByteWriterImplHolder () ,that) ;
}

exports CFat<ByteWriterHolder> ByteWriterHolder::hold (CREF<ByteWriterLayout> that) {
	return CFat<ByteWriterHolder> (ByteWriterImplHolder () ,that) ;
}

struct WriteValueBuffer {
	Buffer<STRU8 ,ENUM<64>> mBuffer ;
	INDEX mWrite ;
} ;

class TextWriterImplHolder final implement Fat<TextWriterHolder ,TextWriterLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () <= 4) ;
		fake.mStream = move (stream) ;
		assert (fake.mStream.exclusive ()) ;
		fake.mDiffEndian = FALSE ;
		reset () ;
	}

	void use_overflow (CREF<Function<VREF<TextWriterLayout>>> overflow) override {
		fake.mOverflow = overflow ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	LENGTH length () const override {
		return fake.mWrite ;
	}

	void reset () override {
		fake.mRead = fake.mStream->size () ;
		fake.mWrite = 0 ;
	}

	void reset (CREF<StreamShape> shape) override {
		fake.mRead = shape.mRead ;
		fake.mWrite = shape.mWrite ;
	}

	StreamShape backup () const override {
		StreamShape ret ;
		ret.mRead = fake.mRead ;
		ret.mWrite = fake.mWrite ;
		return move (ret) ;
	}

	BOOL good () const override {
		return length () < size () ;
	}

	void write (CREF<BOOL> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (!item)
				discard ;
			write (slice ("true")) ;
		}
		if ifdo (act) {
			write (slice ("false")) ;
		}
	}

	void write (CREF<VAL32> item) override {
		if ifdo (TRUE) {
			if (item >= 0)
				discard ;
			write (STRU32 ('-')) ;
		}
		const auto r1x = VAL64 (MathProc::abs (item)) ;
		write (r1x) ;
	}

	void write (CREF<VAL64> item) override {
		if ifdo (TRUE) {
			if (item >= 0)
				discard ;
			write (STRU32 ('-')) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			auto rax = Notation () ;
			rax.mRadix = 10 ;
			rax.mSign = FALSE ;
			rax.mMantissa = MathProc::abs (item) ;
			rax.mDownflow = 0 ;
			rax.mExponent = 0 ;
			rax.mPrecision = LENGTH (log10p (rax.mMantissa)) ;
			auto rbx = WriteValueBuffer () ;
			rbx.mWrite = rbx.mBuffer.size () ;
			write_value (rax ,rbx) ;
			for (auto &&i : iter (rbx.mWrite ,rbx.mBuffer.size ()))
				write (STRU32 (rbx.mBuffer[i])) ;
		}
	}

	void write_value (VREF<Notation> fexp10 ,VREF<WriteValueBuffer> wvb) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = fexp10.mPrecision ;
		auto act = TRUE ;
		if ifdo (act) {
			//@info: case '0'
			if (fexp10.mMantissa != 0)
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
		}
		if ifdo (act) {
			//@info: case 'xxx'
			for (auto &&i : iter (0 ,r1x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
		}
		if ifdo (TRUE) {
			if (!fexp10.mSign)
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('-') ;
		}
	}

	void write (CREF<FLT32> item) override {
		const auto r1x = FLT64 (item) ;
		write (r1x) ;
	}

	void write (CREF<FLT64> item) override {
		if ifdo (TRUE) {
			if (item >= 0)
				discard ;
			write (STRU32 ('-')) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (!MathProc::is_inf (item))
				discard ;
			write (slice ("infinity")) ;
		}
		if ifdo (act) {
			auto rax = FloatProc::decode (MathProc::abs (item)) ;
			rax = FloatProc::fexp10_from_fexp2 (rax) ;
			rax.mPrecision = LENGTH (log10p (rax.mMantissa)) ;
			auto rbx = WriteValueBuffer () ;
			rbx.mWrite = rbx.mBuffer.size () ;
			write_float (rax ,rbx) ;
			for (auto &&i : iter (rbx.mWrite ,rbx.mBuffer.size ()))
				write (STRU32 (rbx.mBuffer[i])) ;
		}
	}

	void write_float (VREF<Notation> fexp10 ,VREF<WriteValueBuffer> wvb) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = FloatProc::float_precision () ;
		if ifdo (TRUE) {
			if (fexp10.mPrecision == 0)
				discard ;
			while (TRUE) {
				if (fexp10.mMantissa == 0)
					break ;
				if (fexp10.mMantissa % 10 != 0)
					break ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			const auto r2x = fexp10.mPrecision - r1x ;
			for (auto &&i : iter (0 ,r2x - 1)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			if (r2x <= 0)
				discard ;
			const auto r3x = MathProc::step (fexp10.mMantissa % 10 - 5) ;
			fexp10.mMantissa += r3x * 5 ;
			fexp10.mMantissa /= 10 ;
			fexp10.mExponent++ ;
			fexp10.mPrecision-- ;
		}
		const auto r4x = fexp10.mPrecision ;
		const auto r5x = LENGTH (fexp10.mExponent) ;
		auto act = TRUE ;
		if ifdo (act) {
			//@info: case '0'
			if (fexp10.mMantissa != 0)
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
		}
		if ifdo (act) {
			//@info: case 'x.xxxExxx'
			const auto r6x = r4x - 1 + r5x ;
			if (MathProc::abs (r6x) < r1x)
				discard ;
			auto rax = Notation () ;
			rax.mRadix = 10 ;
			rax.mSign = BOOL (r6x < 0) ;
			rax.mMantissa = MathProc::abs (r6x) ;
			rax.mDownflow = 0 ;
			rax.mExponent = 0 ;
			rax.mPrecision = LENGTH (log10p (rax.mMantissa)) ;
			write_value (rax ,wvb) ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('E') ;
			const auto r7x = inline_max (LENGTH (r4x - 1 - r1x) ,0) ;
			for (auto &&i : iter (0 ,r7x)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			INDEX ix = wvb.mWrite - 1 ;
			for (auto &&i : iter (r7x ,r4x - 1)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('0')) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('.') ;
			wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('.')) ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
			fexp10.mMantissa /= 10 ;
			fexp10.mExponent++ ;
			fexp10.mPrecision-- ;
		}
		if ifdo (act) {
			//@info: case 'xxx000'
			if (r5x < 0)
				discard ;
			for (auto &&i : iter (0 ,r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
			}
			for (auto &&i : iter (0 ,r4x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
		}
		if ifdo (act) {
			//@info: case 'xxx.xxx'
			if (r5x < 1 - r4x)
				discard ;
			if (r5x >= 0)
				discard ;
			const auto r8x = inline_max (LENGTH (-r5x - r1x) ,0) ;
			for (auto &&i : iter (0 ,r8x)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			INDEX ix = wvb.mWrite - 1 ;
			for (auto &&i : iter (r8x ,-r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('0')) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('.') ;
			wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('.')) ;
			for (auto &&i : iter (0 ,r4x + r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
		}
		if ifdo (act) {
			//@info: case '0.000xxx'
			if (r5x >= 1 - r4x)
				discard ;
			if (r5x >= 0)
				discard ;
			const auto r9x = inline_max (LENGTH (-r5x - r1x) ,ZERO) ;
			for (auto &&i : iter (0 ,r9x)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			INDEX ix = wvb.mWrite - 1 ;
			for (auto &&i : iter (r9x ,r4x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('0')) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			const auto r10x = inline_max (r9x ,r4x) ;
			for (auto &&i : iter (r10x ,-r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
				wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('0')) ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('.') ;
			wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('.')) ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
		}
	}

	VAL64 log10p (CREF<VAL64> a) const {
		VAL64 ret = 0 ;
		auto rax = a ;
		while (TRUE) {
			if (rax == 0)
				break ;
			ret++ ;
			rax /= 10 ;
		}
		return move (ret) ;
	}

	void write (CREF<BYTE> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<WORD> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<CHAR> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<QUAD> item) override {
		write_byte_impl (item) ;
	}

	template <class ARG1>
	forceinline void write_byte_impl (CREF<ARG1> item) {
		write (slice ("0X")) ;
		INDEX ix = SIZE_OF<ARG1>::expr * 8 ;
		for (auto &&i : iter (0 ,SIZE_OF<ARG1>::expr)) {
			noop (i) ;
			ix -= 4 ;
			const auto r1x = INDEX ((item >> ix) & ARG1 (0X0F)) ;
			write (StreamProc::str_from_hex (r1x)) ;
			ix -= 4 ;
			const auto r2x = INDEX ((item >> ix) & ARG1 (0X0F)) ;
			write (StreamProc::str_from_hex (r2x)) ;
		}
	}

	void write (CREF<STRU32> item) override {
		if ifdo (TRUE) {
			if (fake.mWrite < fake.mRead)
				discard ;
			fake.mOverflow (fake) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU8>::expr)
				discard ;
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU8>>::expr] (Pointer::from (fake.mStream.self)) ;
			rax[fake.mWrite] = STRU8 (item) ;
			fake.mWrite++ ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU16>::expr)
				discard ;
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU16>>::expr] (Pointer::from (fake.mStream.self)) ;
			rax[fake.mWrite] = STRU16 (item) ;
			fake.mWrite++ ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU32>::expr)
				discard ;
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU32>>::expr] (Pointer::from (fake.mStream.self)) ;
			rax[fake.mWrite] = STRU32 (item) ;
			fake.mWrite++ ;
		}
	}

	void write (CREF<Slice> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			assume (inline_between (INDEX (item[i]) ,0 ,128)) ;
			write (item[i]) ;
		}
	}

	void write (CREF<String<STRA>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRW>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU8>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU16>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU32>> item) override {
		write_string_impl (item) ;
	}

	template <class ARG1>
	forceinline void write_string_impl (CREF<String<ARG1>> item) {
		const auto r1x = item.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = STRU32 (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void write (CREF<typeof (BOM)>) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU8>::expr)
				discard ;
			write (STRU32 (0XEF)) ;
			write (STRU32 (0XBB)) ;
			write (STRU32 (0XBF)) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU16>::expr)
				discard ;
			write (STRU32 (0XFEFF)) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU32>::expr)
				discard ;
			write (STRU32 (0X0000FEFF)) ;
		}
	}

	void write (CREF<typeof (GAP)>) override {
		write (STRU32 ('\r')) ;
		write (STRU32 ('\n')) ;
	}

	void write (CREF<typeof (EOS)>) override {
		assume (fake.mWrite < fake.mRead) ;
		write (STRU32 (0X00)) ;
	}
} ;

exports VFat<TextWriterHolder> TextWriterHolder::hold (VREF<TextWriterLayout> that) {
	return VFat<TextWriterHolder> (TextWriterImplHolder () ,that) ;
}

exports CFat<TextWriterHolder> TextWriterHolder::hold (CREF<TextWriterLayout> that) {
	return CFat<TextWriterHolder> (TextWriterImplHolder () ,that) ;
}

class FormatImplHolder final implement Fat<FormatHolder ,FormatLayout> {
public:
	void initialize (CREF<Slice> format) override {
		fake.mFormat = format ;
	}

	void friend_write (VREF<WriterBinder> writer) const override {
		auto rax = FLAG (0) ;
		auto rbx = FatLayout () ;
		for (auto &&i : iter (0 ,fake.mFormat.size ())) {
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (fake.mFormat[i] != STRU32 ('$'))
					discard ;
				rax = FLAG (2) ;
			}
			if ifdo (act) {
				if (rax != FLAG (2))
					discard ;
				if (fake.mFormat[i] != STRU32 ('{'))
					discard ;
				rax = FLAG (1) ;
			}
			if ifdo (act) {
				if (rax != FLAG (2))
					discard ;
				if ifdo (TRUE) {
					const auto r1x = StreamProc::hex_from_str (fake.mFormat[i]) - 1 ;
					if (!inline_between (r1x ,0 ,fake.mWrite.self))
						discard ;
					fake.mParams[r1x].get (rbx) ;
					const auto r2x = keep[TYPE<CFat<FormatBinder>>::expr] (rbx) ;
					r2x->friend_write (writer) ;
				}
				rax = FLAG (0) ;
			}
			if ifdo (act) {
				if (rax != FLAG (1))
					discard ;
				if (fake.mFormat[i] != STRU32 ('0'))
					discard ;
				for (auto &&j : iter (0 ,fake.mWrite.self)) {
					fake.mParams[j].get (rbx) ;
					const auto r3x = keep[TYPE<CFat<FormatBinder>>::expr] (rbx) ;
					r3x->friend_write (writer) ;
				}
				rax = FLAG (3) ;
			}
			if ifdo (act) {
				if (rax != FLAG (1))
					discard ;
				if ifdo (TRUE) {
					const auto r4x = StreamProc::hex_from_str (fake.mFormat[i]) - 1 ;
					if (!inline_between (r4x ,0 ,fake.mWrite.self))
						discard ;
					fake.mParams[r4x].get (rbx) ;
					const auto r5x = keep[TYPE<CFat<FormatBinder>>::expr] (rbx) ;
					r5x->friend_write (writer) ;
				}
				rax = FLAG (3) ;
			}
			if ifdo (act) {
				if (rax != FLAG (3))
					discard ;
				assert (fake.mFormat[i] == STRU32 ('}')) ;
				rax = FLAG (0) ;
			}
			if ifdo (act) {
				assume (rax == FLAG (0)) ;
				writer.write (fake.mFormat[i]) ;
			}
		}
	}

	void once (CREF<WrapperLayout> params) const override {
		auto &&rax = keep[TYPE<Wrapper<FatLayout>>::expr] (params) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (0 ,params.mRank)) {
			auto rbx = rax[i] ;
			fake.mParams[ix].set (rbx) ;
			ix++ ;
		}
		fake.mWrite.set (ix) ;
	}
} ;

exports VFat<FormatHolder> FormatHolder::hold (VREF<FormatLayout> that) {
	return VFat<FormatHolder> (FormatImplHolder () ,that) ;
}

exports CFat<FormatHolder> FormatHolder::hold (CREF<FormatLayout> that) {
	return CFat<FormatHolder> (FormatImplHolder () ,that) ;
}

struct StreamTextProcImplLayout {} ;

class StreamTextProcImplHolder final implement Fat<StreamTextProcHolder ,StreamTextProcImplLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void read_keyword (VREF<ReaderBinder> reader ,VREF<String<STRU8>> item) const override {
		auto rax = STRU32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.backup () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			if (!StreamProc::is_word (rax))
				break ;
			rbx++ ;
			reader.read (rax) ;
			while (TRUE) {
				if (!StreamProc::is_word (rax))
					if (!StreamProc::is_hyphen (rax))
						break ;
				rbx++ ;
				reader.read (rax) ;
			}
		}
		reader.reset (r1x) ;
		item = String<STRU8> (rbx) ;
		reader.read (item) ;
	}

	void read_scalar (VREF<ReaderBinder> reader ,VREF<String<STRU8>> item) const override {
		auto rax = STRU32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.backup () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('+'))
					if (rax != STRU32 ('-'))
						discard ;
				rbx++ ;
				reader.read (rax) ;
			}
			while (TRUE) {
				if (!StreamProc::is_digit (rax))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
			if ifdo (TRUE) {
				if (rax != STRU32 ('.'))
					discard ;
				rbx++ ;
				reader.read (rax) ;
				while (TRUE) {
					if (!StreamProc::is_digit (rax))
						break ;
					rbx++ ;
					reader.read (rax) ;
				}
			}
			if (rax != STRU32 ('E'))
				if (rax != STRU32 ('e'))
					discard ;
			rbx++ ;
			reader.read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('+'))
					if (rax != STRU32 ('-'))
						discard ;
				rbx++ ;
				reader.read (rax) ;
			}
			while (TRUE) {
				if (!StreamProc::is_digit (rax))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
		}
		reader.reset (r1x) ;
		item = String<STRU8> (rbx) ;
		reader.read (item) ;
	}

	void read_escape (VREF<ReaderBinder> reader ,VREF<String<STRU8>> item) const override {
		auto rax = STRU32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.backup () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			assume (rax == STRU32 ('\"')) ;
			reader.read (rax) ;
			while (TRUE) {
				if (rax == STRU32 (0X00))
					break ;
				if (rax == STRU32 ('\"'))
					break ;
				if ifdo (TRUE) {
					if (rax != STRU32 ('\\'))
						discard ;
					reader.read (rax) ;
				}
				rbx++ ;
				reader.read (rax) ;
			}
			assume (rax == STRU32 ('\"')) ;
		}
		reader.reset (r1x) ;
		item = String<STRU8> (rbx) ;
		reader.read (rax) ;
		for (auto &&i : iter (0 ,rbx)) {
			reader.read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('\\'))
					discard ;
				reader.read (rax) ;
				rax = StreamProc::ctrl_from_word (rax) ;
			}
			item[i] = STRU8 (rax) ;
		}
		reader.read (rax) ;
	}

	void write_escape (VREF<WriterBinder> writer ,CREF<String<STRU8>> item) const override {
		writer.write (STRU32 ('\"')) ;
		for (auto &&i : item) {
			auto act = TRUE ;
			if ifdo (act) {
				if (!StreamProc::is_ctrl (i))
					discard ;
				const auto r1x = StreamProc::word_from_ctrl (i) ;
				writer.write (STRU32 ('\\')) ;
				writer.write (r1x) ;
			}
			if ifdo (act) {
				writer.write (STRU32 (i)) ;
			}
		}
		writer.write (STRU32 ('\"')) ;
	}

	void read_blank (VREF<ReaderBinder> reader ,VREF<String<STRU8>> item) const override {
		auto rax = STRU32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.backup () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			while (TRUE) {
				if (rax == STRU32 (0X00))
					break ;
				if (StreamProc::is_space (rax))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
		}
		reader.reset (r1x) ;
		item = String<STRU8> (rbx) ;
		reader.read (item) ;
	}

	void read_endline (VREF<ReaderBinder> reader ,VREF<String<STRU8>> item) const override {
		auto rax = STRU32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.backup () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			while (TRUE) {
				if (rax == STRU32 (0X00))
					break ;
				if (StreamProc::is_endline (rax))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
		}
		reader.reset (r1x) ;
		item = String<STRU8> (rbx) ;
		reader.read (item) ;
	}

	void write_aligned (VREF<WriterBinder> writer ,CREF<VAL64> number ,CREF<LENGTH> align) const override {
		auto rax = WriteValueBuffer () ;
		assert (inline_between (align ,0 ,rax.mBuffer.size ())) ;
		rax.mWrite = rax.mBuffer.size () ;
		auto rbx = MathProc::abs (number) ;
		for (auto &&i : iter (0 ,align)) {
			noop (i) ;
			rax.mWrite-- ;
			rax.mBuffer[rax.mWrite] = STRU8 (STRU32 ('0') + rbx % 10) ;
			rbx /= 10 ;
		}
		for (auto &&i : iter (rax.mWrite ,rax.mBuffer.size ())) {
			writer.write (STRU32 (rax.mBuffer[i])) ;
		}
	}
} ;

exports CREF<StreamTextProcLayout> StreamTextProcHolder::instance () {
	return memorize ([&] () {
		StreamTextProcLayout ret ;
		ret.mThis = UniqueRef<StreamTextProcImplLayout>::make () ;
		StreamTextProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<StreamTextProcHolder> StreamTextProcHolder::hold (VREF<StreamTextProcImplLayout> that) {
	return VFat<StreamTextProcHolder> (StreamTextProcImplHolder () ,that) ;
}

exports CFat<StreamTextProcHolder> StreamTextProcHolder::hold (CREF<StreamTextProcImplLayout> that) {
	return CFat<StreamTextProcHolder> (StreamTextProcImplHolder () ,that) ;
}

struct CommaImplLayout {
	Slice mIndent ;
	Slice mComma ;
	Slice mEndline ;
	LENGTH mDepth ;
	Deque<BOOL> mFirst ;
	LENGTH mTight ;
	LENGTH mLastTight ;
} ;

class CommaImplHolder final implement Fat<CommaHolder ,CommaImplLayout> {
public:
	void initialize (CREF<Slice> indent ,CREF<Slice> comma ,CREF<Slice> endline) override {
		fake.mIndent = indent ;
		fake.mComma = comma ;
		fake.mEndline = endline ;
		fake.mDepth = 0 ;
		fake.mTight = 255 ;
		fake.mLastTight = 0 ;
	}

	void friend_write (VREF<WriterBinder> writer) override {
		if ifdo (TRUE) {
			if (fake.mDepth >= fake.mTight + fake.mLastTight)
				discard ;
			writer.write (fake.mEndline) ;
		}
		if ifdo (TRUE) {
			if (fake.mFirst.empty ())
				discard ;
			INDEX ix = fake.mFirst.tail () ;
			if ifdo (TRUE) {
				if (fake.mFirst[ix])
					discard ;
				writer.write (fake.mComma) ;
			}
			fake.mFirst[ix] = FALSE ;
		}
		if ifdo (TRUE) {
			if (fake.mDepth >= fake.mTight + fake.mLastTight)
				discard ;
			for (auto &&i : iter (0 ,fake.mDepth)) {
				noop (i) ;
				writer.write (fake.mIndent) ;
			}
		}
		fake.mLastTight = 0 ;
	}

	void increase () override {
		fake.mDepth++ ;
		if ifdo (TRUE) {
			if (fake.mFirst.empty ())
				discard ;
			fake.mFirst[fake.mFirst.tail ()] = TRUE ;
		}
		fake.mFirst.add (TRUE) ;
	}

	void decrease () override {
		fake.mFirst.pop () ;
		fake.mDepth-- ;
		fake.mLastTight = fake.mTight - 256 ;
		fake.mTight = 255 ;
	}

	void tight () override {
		fake.mTight = inline_min (fake.mTight ,fake.mDepth) ;
	}
} ;

exports CommaLayout CommaHolder::create () {
	CommaLayout ret ;
	ret.mThis = SharedRef<CommaImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<CommaHolder> CommaHolder::hold (VREF<CommaImplLayout> that) {
	return VFat<CommaHolder> (CommaImplHolder () ,that) ;
}

exports CFat<CommaHolder> CommaHolder::hold (CREF<CommaImplLayout> that) {
	return CFat<CommaHolder> (CommaImplHolder () ,that) ;
}

struct RegexImplLayout {
	std::basic_regex<STR> mRegex ;
	std::match_results<PTR<CREF<STR>>> mMatch ;
	Ref<String<STR>> mText ;
} ;

class RegexImplHolder final implement Fat<RegexHolder ,RegexImplLayout> {
public:
	void initialize (CREF<String<STR>> format) override {
		fake.mRegex = std::basic_regex<STR> (format) ;
	}

	INDEX search (RREF<Ref<String<STR>>> text ,CREF<INDEX> offset) override {
		fake.mText = move (text) ;
		const auto r1x = (&fake.mText.self[offset]) ;
		const auto r2x = std::regex_search (r1x ,fake.mMatch ,fake.mRegex) ;
		if (!r2x)
			return NONE ;
		const auto r3x = FLAG (fake.mMatch[0].first) ;
		const auto r4x = (r3x - FLAG (r1x)) / SIZE_OF<STR>::expr ;
		return offset + r4x ;
	}

	Slice match (CREF<INDEX> index) const override {
		assert (!fake.mMatch.empty ()) ;
		assert (inline_between (index ,0 ,fake.mMatch.size ())) ;
		const auto r1x = FLAG (fake.mMatch[index].first) ;
		const auto r2x = FLAG (fake.mMatch[index].second) ;
		const auto r3x = (r2x - r1x) / SIZE_OF<STR>::expr ;
		return Slice (r1x ,r3x ,SIZE_OF<STR>::expr) ;
	}
} ;

exports RegexLayout RegexHolder::create () {
	RegexLayout ret ;
	ret.mThis = AutoRef<RegexImplLayout>::make () ;
	return move (ret) ;
}

exports VFat<RegexHolder> RegexHolder::hold (VREF<RegexImplLayout> that) {
	return VFat<RegexHolder> (RegexImplHolder () ,that) ;
}

exports CFat<RegexHolder> RegexHolder::hold (CREF<RegexImplLayout> that) {
	return CFat<RegexHolder> (RegexImplHolder () ,that) ;
}
} ;