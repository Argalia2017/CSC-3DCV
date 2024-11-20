#pragma once

#ifndef __CSC_STREAM__
#define __CSC_STREAM__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"

namespace CSC {
struct StreamProcImplLayout ;

struct StreamProcLayout implement ThisLayout<AutoRef<StreamProcImplLayout>> {} ;

struct StreamProcHolder implement Interface {
	imports VFat<StreamProcHolder> hold (VREF<StreamProcLayout> that) ;
	imports CFat<StreamProcHolder> hold (CREF<StreamProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BOOL big_endian () const = 0 ;
	virtual BOOL is_blank (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_space (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_endline (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_punct (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_alpha (CREF<STRU32> str) const = 0 ;
	virtual STRU32 alpha_lower (CREF<STRU32> str) const = 0 ;
	virtual STRU32 alpha_upper (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_digit (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_hex_digit (CREF<STRU32> str) const = 0 ;
	virtual INDEX hex_from_str (CREF<STRU32> str) const = 0 ;
	virtual STRU32 str_from_hex (CREF<INDEX> hex) const = 0 ;
	virtual BOOL is_word (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_ctrl (CREF<STRU32> str) const = 0 ;
	virtual STRU32 word_from_ctrl (CREF<STRU32> str) const = 0 ;
	virtual STRU32 ctrl_from_word (CREF<STRU32> str) const = 0 ;
} ;

class StreamProc implement StreamProcLayout {
protected:
	using StreamProcLayout::mThis ;

public:
	imports CREF<StreamProc> instance () {
		return memorize ([&] () {
			StreamProc ret ;
			StreamProcHolder::hold (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports BOOL big_endian () {
		return StreamProcHolder::hold (instance ())->big_endian () ;
	}

	imports BOOL is_blank (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->is_blank (str) ;
	}

	imports BOOL is_space (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->is_space (str) ;
	}

	imports BOOL is_endline (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->is_endline (str) ;
	}

	imports BOOL is_punct (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->is_punct (str) ;
	}

	imports BOOL is_alpha (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->is_alpha (str) ;
	}

	imports STRU32 alpha_lower (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->alpha_lower (str) ;
	}

	imports STRU32 alpha_upper (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->alpha_upper (str) ;
	}

	imports BOOL is_digit (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->is_digit (str) ;
	}

	imports BOOL is_hex_digit (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->is_hex_digit (str) ;
	}

	imports INDEX hex_from_str (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->hex_from_str (str) ;
	}

	imports STRU32 str_from_hex (CREF<INDEX> hex) {
		return StreamProcHolder::hold (instance ())->str_from_hex (hex) ;
	}

	imports BOOL is_word (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->is_word (str) ;
	}

	imports BOOL is_ctrl (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->is_ctrl (str) ;
	}

	imports STRU32 word_from_ctrl (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->word_from_ctrl (str) ;
	}

	imports STRU32 ctrl_from_word (CREF<STRU32> str) {
		return StreamProcHolder::hold (instance ())->ctrl_from_word (str) ;
	}
} ;

static constexpr auto CLS = TYPE<PlaceHolder<RANK1>>::expr ;
static constexpr auto BOM = TYPE<PlaceHolder<RANK2>>::expr ;
static constexpr auto GAP = TYPE<PlaceHolder<RANK3>>::expr ;
static constexpr auto EOS = TYPE<PlaceHolder<RANK4>>::expr ;

struct ReaderFriend ;

template <class...>
trait HAS_FRIEND_READ_HELP ;

template <class A ,class OTHERWISE>
trait HAS_FRIEND_READ_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_FRIEND_READ_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).friend_read (nullof (ReaderFriend)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_FRIEND_READ = typename HAS_FRIEND_READ_HELP<A ,ALWAYS>::RET ;

struct ReaderFriend implement Interface {
	virtual BOOL good () const = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void read (VREF<BOOL> item) = 0 ;
	virtual void read (VREF<VAL32> item) = 0 ;
	virtual void read (VREF<VAL64> item) = 0 ;
	virtual void read (VREF<FLT32> item) = 0 ;
	virtual void read (VREF<FLT64> item) = 0 ;
	virtual void read (VREF<BYTE> item) = 0 ;
	virtual void read (VREF<WORD> item) = 0 ;
	virtual void read (VREF<CHAR> item) = 0 ;
	virtual void read (VREF<QUAD> item) = 0 ;
	virtual void read (VREF<STRU32> item) = 0 ;
	virtual void read (CREF<Slice> item) = 0 ;
	virtual void read (VREF<String<STRA>> item) = 0 ;
	virtual void read (VREF<String<STRW>> item) = 0 ;
	virtual void read (VREF<String<STRU8>> item) = 0 ;
	virtual void read (VREF<String<STRU16>> item) = 0 ;
	virtual void read (VREF<String<STRU32>> item) = 0 ;
	virtual void read (CREF<typeof (CLS)> item) = 0 ;
	virtual void read (CREF<typeof (BOM)> item) = 0 ;
	virtual void read (CREF<typeof (GAP)> item) = 0 ;
	virtual void read (CREF<typeof (EOS)> item) = 0 ;

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (VREF<ARG1> item) {
		item.friend_read (thiz) ;
	}
} ;

template <class A>
class ReaderFriendBinder implement Fat<ReaderFriend ,A> {
public:
	imports VFat<ReaderFriend> create (VREF<A> that) {
		return VFat<ReaderFriend> (ReaderFriendBinder () ,that) ;
	}

	BOOL good () const override {
		return thiz.fake.good () ;
	}

	void backup () override {
		return thiz.fake.backup () ;
	}

	void recover () override {
		return thiz.fake.recover () ;
	}

	void read (VREF<BOOL> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<VAL32> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<VAL64> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<FLT32> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<FLT64> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<BYTE> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<WORD> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<CHAR> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<QUAD> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<STRU32> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<Slice> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRA>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRW>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRU8>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRU16>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRU32>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<typeof (CLS)> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<typeof (BOM)> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<typeof (GAP)> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<typeof (EOS)> item) override {
		return thiz.fake.read (item) ;
	}
} ;

struct ByteReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	Function<VREF<ByteReaderLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct ByteReaderHolder implement Interface {
	imports VFat<ByteReaderHolder> hold (VREF<ByteReaderLayout> that) ;
	imports CFat<ByteReaderHolder> hold (CREF<ByteReaderLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CREF<FunctionLayout> overflow) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void read (VREF<BOOL> item) = 0 ;
	virtual void read (VREF<VAL32> item) = 0 ;
	virtual void read (VREF<VAL64> item) = 0 ;
	virtual void read (VREF<FLT32> item) = 0 ;
	virtual void read (VREF<FLT64> item) = 0 ;
	virtual void read (VREF<BYTE> item) = 0 ;
	virtual void read (VREF<WORD> item) = 0 ;
	virtual void read (VREF<CHAR> item) = 0 ;
	virtual void read (VREF<QUAD> item) = 0 ;
	virtual void read (VREF<STRU32> item) = 0 ;
	virtual void read (CREF<Slice> item) = 0 ;
	virtual void read (VREF<String<STRA>> item) = 0 ;
	virtual void read (VREF<String<STRW>> item) = 0 ;
	virtual void read (VREF<String<STRU8>> item) = 0 ;
	virtual void read (VREF<String<STRU16>> item) = 0 ;
	virtual void read (VREF<String<STRU32>> item) = 0 ;
	virtual void read (CREF<typeof (CLS)> item) = 0 ;
	virtual void read (CREF<typeof (BOM)> item) = 0 ;
	virtual void read (CREF<typeof (GAP)> item) = 0 ;
	virtual void read (CREF<typeof (EOS)> item) = 0 ;
} ;

class ByteReader implement ByteReaderLayout {
protected:
	using ByteReaderLayout::mStream ;
	using ByteReaderLayout::mRead ;
	using ByteReaderLayout::mWrite ;
	using ByteReaderLayout::mBackupRead ;
	using ByteReaderLayout::mBackupWrite ;

public:
	implicit ByteReader () = default ;

	explicit ByteReader (RREF<Ref<RefBuffer<BYTE>>> stream) {
		ByteReaderHolder::hold (thiz)->initialize (move (stream)) ;
	}

	void use_overflow (CREF<Function<VREF<ByteReader>>> overflow) {
		return ByteReaderHolder::hold (thiz)->use_overflow (overflow) ;
	}

	LENGTH size () const {
		return ByteReaderHolder::hold (thiz)->size () ;
	}

	LENGTH length () const {
		return ByteReaderHolder::hold (thiz)->length () ;
	}

	void reset () {
		return ByteReaderHolder::hold (thiz)->reset () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
		return ByteReaderHolder::hold (thiz)->reset (read_ ,write_) ;
	}

	BOOL good () const {
		return ByteReaderHolder::hold (thiz)->good () ;
	}

	void backup () {
		return ByteReaderHolder::hold (thiz)->backup () ;
	}

	void recover () {
		return ByteReaderHolder::hold (thiz)->recover () ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VREF<BOOL> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<BOOL> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL32> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<VAL32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL64> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<VAL64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT32> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<FLT32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT64> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<FLT64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<BYTE> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<BYTE> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<WORD> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<WORD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<CHAR> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<CHAR> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<QUAD> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<QUAD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<STRU32> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<STRU32> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<Slice> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRA>> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRA>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRW>> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRW>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU8>> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRU8>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU16>> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRU16>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU32>> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRU32>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (CLS)> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<typeof (CLS)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (BOM)> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<typeof (BOM)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (GAP)> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<typeof (GAP)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (EOS)> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<typeof (EOS)> item) {
		read (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (VREF<ARG1> item) {
		item.friend_read (ReaderFriendBinder<ByteReader>::create (thiz).self) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	forceinline VREF<ByteReader> operator>> (VREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct TextReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	Function<VREF<TextReaderLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct TextReaderHolder implement Interface {
	imports VFat<TextReaderHolder> hold (VREF<TextReaderLayout> that) ;
	imports CFat<TextReaderHolder> hold (CREF<TextReaderLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CREF<FunctionLayout> overflow) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void read (VREF<BOOL> item) = 0 ;
	virtual void read (VREF<VAL32> item) = 0 ;
	virtual void read (VREF<VAL64> item) = 0 ;
	virtual void read (VREF<FLT32> item) = 0 ;
	virtual void read (VREF<FLT64> item) = 0 ;
	virtual void read (VREF<BYTE> item) = 0 ;
	virtual void read (VREF<WORD> item) = 0 ;
	virtual void read (VREF<CHAR> item) = 0 ;
	virtual void read (VREF<QUAD> item) = 0 ;
	virtual void read (VREF<STRU32> item) = 0 ;
	virtual void read (CREF<Slice> item) = 0 ;
	virtual void read (VREF<String<STRA>> item) = 0 ;
	virtual void read (VREF<String<STRW>> item) = 0 ;
	virtual void read (VREF<String<STRU8>> item) = 0 ;
	virtual void read (VREF<String<STRU16>> item) = 0 ;
	virtual void read (VREF<String<STRU32>> item) = 0 ;
	virtual void read (CREF<typeof (CLS)> item) = 0 ;
	virtual void read (CREF<typeof (BOM)> item) = 0 ;
	virtual void read (CREF<typeof (GAP)> item) = 0 ;
	virtual void read (CREF<typeof (EOS)> item) = 0 ;
} ;

class TextReader implement TextReaderLayout {
protected:
	using TextReaderLayout::mStream ;
	using TextReaderLayout::mRead ;
	using TextReaderLayout::mWrite ;
	using TextReaderLayout::mBackupRead ;
	using TextReaderLayout::mBackupWrite ;

public:
	implicit TextReader () = default ;

	explicit TextReader (RREF<Ref<RefBuffer<BYTE>>> stream) {
		TextReaderHolder::hold (thiz)->initialize (move (stream)) ;
	}

	void use_overflow (CREF<Function<VREF<TextReader>>> overflow) {
		return TextReaderHolder::hold (thiz)->use_overflow (overflow) ;
	}

	LENGTH size () const {
		return TextReaderHolder::hold (thiz)->size () ;
	}

	LENGTH length () const {
		return TextReaderHolder::hold (thiz)->length () ;
	}

	void reset () {
		return TextReaderHolder::hold (thiz)->reset () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
		return TextReaderHolder::hold (thiz)->reset (read_ ,write_) ;
	}

	BOOL good () const {
		return TextReaderHolder::hold (thiz)->good () ;
	}

	void backup () {
		return TextReaderHolder::hold (thiz)->backup () ;
	}

	void recover () {
		return TextReaderHolder::hold (thiz)->recover () ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VREF<BOOL> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<BOOL> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL32> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<VAL32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL64> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<VAL64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT32> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<FLT32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT64> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<FLT64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<BYTE> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<BYTE> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<WORD> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<WORD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<CHAR> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<CHAR> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<QUAD> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<QUAD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<STRU32> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<STRU32> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<Slice> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRA>> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRA>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRW>> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRW>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU8>> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRU8>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU16>> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRU16>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU32>> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRU32>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (CLS)> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<typeof (CLS)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (BOM)> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<typeof (BOM)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (GAP)> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<typeof (GAP)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (EOS)> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<typeof (EOS)> item) {
		read (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (VREF<ARG1> item) {
		item.friend_read (ReaderFriendBinder<TextReader>::create (thiz).self) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	forceinline VREF<TextReader> operator>> (VREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct WriterFriend ;

template <class...>
trait HAS_FRIEND_WRITE_HELP ;

template <class A ,class OTHERWISE>
trait HAS_FRIEND_WRITE_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_FRIEND_WRITE_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).friend_write (nullof (WriterFriend)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_FRIEND_WRITE = typename HAS_FRIEND_WRITE_HELP<A ,ALWAYS>::RET ;

struct WriterFriend implement Interface {
	virtual BOOL good () const = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void write (CREF<BOOL> item) = 0 ;
	virtual void write (CREF<VAL32> item) = 0 ;
	virtual void write (CREF<VAL64> item) = 0 ;
	virtual void write (CREF<FLT32> item) = 0 ;
	virtual void write (CREF<FLT64> item) = 0 ;
	virtual void write (CREF<BYTE> item) = 0 ;
	virtual void write (CREF<WORD> item) = 0 ;
	virtual void write (CREF<CHAR> item) = 0 ;
	virtual void write (CREF<QUAD> item) = 0 ;
	virtual void write (CREF<STRU32> item) = 0 ;
	virtual void write (CREF<Slice> item) = 0 ;
	virtual void write (CREF<String<STRA>> item) = 0 ;
	virtual void write (CREF<String<STRW>> item) = 0 ;
	virtual void write (CREF<String<STRU8>> item) = 0 ;
	virtual void write (CREF<String<STRU16>> item) = 0 ;
	virtual void write (CREF<String<STRU32>> item) = 0 ;
	virtual void write (CREF<typeof (BOM)> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (CREF<ARG1> item) {
		item.friend_write (thiz) ;
	}
} ;

template <class A>
class WriterFriendBinder implement Fat<WriterFriend ,A> {
public:
	imports VFat<WriterFriend> create (VREF<A> that) {
		return VFat<WriterFriend> (WriterFriendBinder () ,that) ;
	}

	BOOL good () const override {
		return thiz.fake.good () ;
	}

	void backup () override {
		return thiz.fake.backup () ;
	}

	void recover () override {
		return thiz.fake.recover () ;
	}

	void write (CREF<BOOL> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<VAL32> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<VAL64> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<FLT32> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<FLT64> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<BYTE> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<WORD> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<CHAR> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<QUAD> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<STRU32> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<Slice> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRA>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRW>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRU8>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRU16>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRU32>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<typeof (BOM)> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<typeof (CLS)> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<typeof (GAP)> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<typeof (EOS)> item) override {
		return thiz.fake.write (item) ;
	}
} ;

struct ByteWriterLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	Function<VREF<ByteWriterLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct ByteWriterHolder implement Interface {
	imports VFat<ByteWriterHolder> hold (VREF<ByteWriterLayout> that) ;
	imports CFat<ByteWriterHolder> hold (CREF<ByteWriterLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CREF<FunctionLayout> overflow) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void write (CREF<BOOL> item) = 0 ;
	virtual void write (CREF<VAL32> item) = 0 ;
	virtual void write (CREF<VAL64> item) = 0 ;
	virtual void write (CREF<FLT32> item) = 0 ;
	virtual void write (CREF<FLT64> item) = 0 ;
	virtual void write (CREF<BYTE> item) = 0 ;
	virtual void write (CREF<WORD> item) = 0 ;
	virtual void write (CREF<CHAR> item) = 0 ;
	virtual void write (CREF<QUAD> item) = 0 ;
	virtual void write (CREF<STRU32> item) = 0 ;
	virtual void write (CREF<Slice> item) = 0 ;
	virtual void write (CREF<String<STRA>> item) = 0 ;
	virtual void write (CREF<String<STRW>> item) = 0 ;
	virtual void write (CREF<String<STRU8>> item) = 0 ;
	virtual void write (CREF<String<STRU16>> item) = 0 ;
	virtual void write (CREF<String<STRU32>> item) = 0 ;
	virtual void write (CREF<typeof (BOM)> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;
} ;

class ByteWriter implement ByteWriterLayout {
protected:
	using ByteWriterLayout::mStream ;
	using ByteWriterLayout::mRead ;
	using ByteWriterLayout::mWrite ;
	using ByteWriterLayout::mBackupRead ;
	using ByteWriterLayout::mBackupWrite ;

public:
	implicit ByteWriter () = default ;

	explicit ByteWriter (RREF<Ref<RefBuffer<BYTE>>> stream) {
		ByteWriterHolder::hold (thiz)->initialize (move (stream)) ;
	}

	void use_overflow (CREF<Function<VREF<ByteWriter>>> overflow) {
		return ByteWriterHolder::hold (thiz)->use_overflow (overflow) ;
	}

	LENGTH size () const {
		return ByteWriterHolder::hold (thiz)->size () ;
	}

	LENGTH length () const {
		return ByteWriterHolder::hold (thiz)->length () ;
	}

	void reset () {
		return ByteWriterHolder::hold (thiz)->reset () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
		return ByteWriterHolder::hold (thiz)->reset (read_ ,write_) ;
	}

	BOOL good () const {
		return ByteWriterHolder::hold (thiz)->good () ;
	}

	void backup () {
		return ByteWriterHolder::hold (thiz)->backup () ;
	}

	void recover () {
		return ByteWriterHolder::hold (thiz)->recover () ;
	}

	void write (CREF<BOOL> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<BOOL> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<VAL32> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<VAL32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<VAL64> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<VAL64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<FLT32> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<FLT32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<FLT64> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<FLT64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<BYTE> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<BYTE> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<WORD> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<WORD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<CHAR> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<CHAR> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<QUAD> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<QUAD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<STRU32> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<STRU32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<Slice> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRA>> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRA>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRW>> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRW>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU8>> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRU8>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU16>> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRU16>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU32>> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRU32>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (BOM)> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<typeof (BOM)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (CLS)> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<typeof (CLS)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (GAP)> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<typeof (GAP)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (EOS)> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<typeof (EOS)> item) {
		write (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (CREF<ARG1> item) {
		item.friend_write (WriterFriendBinder<ByteWriter>::create (thiz).self) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	forceinline VREF<ByteWriter> operator<< (CREF<ARG1> item) {
		write (item) ;
		return thiz ;
	}
} ;

struct TextWriterLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	Function<VREF<TextWriterLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct TextWriterHolder implement Interface {
	imports VFat<TextWriterHolder> hold (VREF<TextWriterLayout> that) ;
	imports CFat<TextWriterHolder> hold (CREF<TextWriterLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CREF<FunctionLayout> overflow) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void write (CREF<BOOL> item) = 0 ;
	virtual void write (CREF<VAL32> item) = 0 ;
	virtual void write (CREF<VAL64> item) = 0 ;
	virtual void write (CREF<FLT32> item) = 0 ;
	virtual void write (CREF<FLT64> item) = 0 ;
	virtual void write (CREF<BYTE> item) = 0 ;
	virtual void write (CREF<WORD> item) = 0 ;
	virtual void write (CREF<CHAR> item) = 0 ;
	virtual void write (CREF<QUAD> item) = 0 ;
	virtual void write (CREF<STRU32> item) = 0 ;
	virtual void write (CREF<Slice> item) = 0 ;
	virtual void write (CREF<String<STRA>> item) = 0 ;
	virtual void write (CREF<String<STRW>> item) = 0 ;
	virtual void write (CREF<String<STRU8>> item) = 0 ;
	virtual void write (CREF<String<STRU16>> item) = 0 ;
	virtual void write (CREF<String<STRU32>> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (BOM)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;
} ;

class TextWriter implement TextWriterLayout {
protected:
	using TextWriterLayout::mStream ;
	using TextWriterLayout::mRead ;
	using TextWriterLayout::mWrite ;
	using TextWriterLayout::mBackupRead ;
	using TextWriterLayout::mBackupWrite ;

public:
	implicit TextWriter () = default ;

	explicit TextWriter (RREF<Ref<RefBuffer<BYTE>>> stream) {
		TextWriterHolder::hold (thiz)->initialize (move (stream)) ;
	}

	void use_overflow (CREF<Function<VREF<TextWriter>>> overflow) {
		return TextWriterHolder::hold (thiz)->use_overflow (overflow) ;
	}

	LENGTH size () const {
		return TextWriterHolder::hold (thiz)->size () ;
	}

	LENGTH length () const {
		return TextWriterHolder::hold (thiz)->length () ;
	}

	void reset () {
		return TextWriterHolder::hold (thiz)->reset () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
		return TextWriterHolder::hold (thiz)->reset (read_ ,write_) ;
	}

	BOOL good () const {
		return TextWriterHolder::hold (thiz)->good () ;
	}

	void backup () {
		return TextWriterHolder::hold (thiz)->backup () ;
	}

	void recover () {
		return TextWriterHolder::hold (thiz)->recover () ;
	}

	void write (CREF<BOOL> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<BOOL> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<VAL32> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<VAL32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<VAL64> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<VAL64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<FLT32> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<FLT32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<FLT64> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<FLT64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<BYTE> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<BYTE> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<WORD> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<WORD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<CHAR> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<CHAR> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<QUAD> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<QUAD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<STRU32> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<STRU32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<Slice> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRA>> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRA>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRW>> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRW>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU8>> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRU8>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU16>> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRU16>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU32>> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRU32>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (CLS)> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<typeof (CLS)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (BOM)> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<typeof (BOM)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (GAP)> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<typeof (GAP)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (EOS)> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<typeof (EOS)> item) {
		write (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (CREF<ARG1> item) {
		item.friend_write (WriterFriendBinder<TextWriter>::create (thiz).self) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	forceinline VREF<TextWriter> operator<< (CREF<ARG1> item) {
		write (item) ;
		return thiz ;
	}
} ;

template <class A>
class StringParse implement Proxy {
public:
	imports A make (CREF<Slice> text) {
		return make (String<STR> (text)) ;
	}

	template <class ARG1>
	imports A make (CREF<String<ARG1>> text) {
		A ret ;
		auto rax = TextReader (text.borrow ()) ;
		rax >> ret ;
		rax >> EOS ;
		return move (ret) ;
	}
} ;

template <class A>
class StringBuild implement Proxy {
public:
	template <class...ARG1>
	imports String<A> make (CREF<ARG1>...params) {
		String<A> ret = String<A> (SLICE_MAX_SIZE::expr) ;
		auto rax = TextWriter (ret.borrow ()) ;
		make_impl (rax ,params...) ;
		rax << EOS ;
		return move (ret) ;
	}

	imports forceinline void make_impl (VREF<TextWriter> writer) {
		noop () ;
	}

	template <class ARG1 ,class...ARG2>
	imports forceinline void make_impl (VREF<TextWriter> writer ,CREF<ARG1> params1 ,CREF<ARG2>...params2) {
		writer << params1 ;
		make_impl (writer ,params2...) ;
	}
} ;

struct FormatFriend implement Interface {
	virtual void friend_write (VREF<WriterFriend> writer) const = 0 ;
} ;

template <class A>
class FormatFriendBinder implement Fat<FormatFriend ,A> {
public:
	imports CFat<FormatFriend> create (CREF<A> that) {
		return CFat<FormatFriend> (FormatFriendBinder () ,that) ;
	}

	void friend_write (VREF<WriterFriend> writer) const override {
		writer.write (thiz.fake) ;
	}
} ;

struct FormatLayout {
	Slice mFormat ;
	Pin<BufferX<FatLayout>> mParams ;
	Pin<INDEX> mWrite ;

public:
	implicit FormatLayout () = default ;

	implicit FormatLayout (CREF<FormatLayout> that) = delete ;

	forceinline VREF<FormatLayout> operator= (CREF<FormatLayout> that) = delete ;

	implicit FormatLayout (RREF<FormatLayout> that) noexcept :FormatLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<FormatLayout> operator= (RREF<FormatLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct FormatHolder implement Interface {
	imports VFat<FormatHolder> hold (VREF<FormatLayout> that) ;
	imports CFat<FormatHolder> hold (CREF<FormatLayout> that) ;

	virtual void initialize (CREF<Slice> format) = 0 ;
	virtual void friend_write (VREF<WriterFriend> writer) const = 0 ;
	virtual void once (CREF<WrapperLayout> params) const = 0 ;
} ;

class Format implement FormatLayout {
protected:
	using FormatLayout::mFormat ;
	using FormatLayout::mParams ;
	using FormatLayout::mWrite ;

public:
	implicit Format () = default ;

	explicit Format (CREF<Slice> format) {
		FormatHolder::hold (thiz)->initialize (format) ;
	}

	void friend_write (VREF<WriterFriend> writer) const {
		return FormatHolder::hold (thiz)->friend_write (writer) ;
	}

	template <class...ARG1>
	void once (CREF<ARG1>...params) const {
		return FormatHolder::hold (thiz)->once (MakeWrapper (FormatFriendBinder<ARG1>::create (params)...)) ;
	}

	template <class...ARG1>
	CREF<Format> operator() (CREF<ARG1>...params) const {
		once (params...) ;
		return thiz ;
	}
} ;

inline CREF<Format> PrintFormat (CREF<Format> params) {
	return params ;
}

template <class...ARG1>
inline Format PrintFormat (CREF<ARG1>...params) {
	Format ret = Format (slice ("${0}")) ;
	ret (params...) ;
	return move (ret) ;
}

struct StreamTextProcLayout implement ThisLayout<AutoRefLayout> {} ;

struct StreamTextProcHolder implement Interface {
	imports VFat<StreamTextProcHolder> hold (VREF<StreamTextProcLayout> that) ;
	imports CFat<StreamTextProcHolder> hold (CREF<StreamTextProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void read_keyword (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) const = 0 ;
	virtual void read_scalar (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) const = 0 ;
	virtual void read_escape (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) const = 0 ;
	virtual void write_escape (VREF<WriterFriend> writer ,CREF<String<STRU8>> item) const = 0 ;
	virtual void read_blank (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) const = 0 ;
	virtual void read_endline (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) const = 0 ;
	virtual void write_aligned (VREF<WriterFriend> writer ,CREF<VAL64> number ,CREF<LENGTH> align) const = 0 ;
} ;

class StreamTextProc implement StreamTextProcLayout {
protected:
	using StreamTextProcLayout::mThis ;

public:
	imports CREF<StreamTextProc> instance () {
		return memorize ([&] () {
			StreamTextProc ret ;
			StreamTextProcHolder::hold (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports void read_keyword (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) {
		return StreamTextProcHolder::hold (instance ())->read_keyword (reader ,item) ;
	}

	imports void read_scalar (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) {
		return StreamTextProcHolder::hold (instance ())->read_scalar (reader ,item) ;
	}

	imports void read_escape (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) {
		return StreamTextProcHolder::hold (instance ())->read_escape (reader ,item) ;
	}

	imports void write_escape (VREF<WriterFriend> writer ,CREF<String<STRU8>> item) {
		return StreamTextProcHolder::hold (instance ())->write_escape (writer ,item) ;
	}

	imports void read_blank (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) {
		return StreamTextProcHolder::hold (instance ())->read_blank (reader ,item) ;
	}

	imports void read_endline (VREF<ReaderFriend> reader ,VREF<String<STRU8>> item) {
		return StreamTextProcHolder::hold (instance ())->read_endline (reader ,item) ;
	}

	imports void write_aligned (VREF<WriterFriend> writer ,CREF<VAL64> number ,CREF<LENGTH> align) {
		return StreamTextProcHolder::hold (instance ())->write_aligned (writer ,number ,align) ;
	}
} ;

class KeywordText implement Proxy {
protected:
	String<STRU8> mThat ;

public:
	imports VREF<KeywordText> from (VREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	template <class ARG1>
	void friend_read (VREF<ARG1> reader) {
		return StreamTextProc::read_keyword (ReaderFriendBinder<ARG1>::create (reader) ,mThat) ;
	}
} ;

class ScalarText implement Proxy {
protected:
	String<STRU8> mThat ;

public:
	imports VREF<ScalarText> from (VREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	template <class ARG1>
	void friend_read (VREF<ARG1> reader) {
		return StreamTextProc::read_scalar (ReaderFriendBinder<ARG1>::create (reader) ,mThat) ;
	}
} ;

class EscapeText implement Proxy {
protected:
	String<STRU8> mThat ;

public:
	imports VREF<EscapeText> from (VREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	imports CREF<EscapeText> from (CREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	imports CREF<EscapeText> from (RREF<String<STRU8>> that) = delete ;

	template <class ARG1>
	void friend_read (VREF<ARG1> reader) {
		return StreamTextProc::read_escape (ReaderFriendBinder<ARG1>::create (reader) ,mThat) ;
	}

	template <class ARG1>
	void friend_write (VREF<ARG1> writer) const {
		return StreamTextProc::write_escape (WriterFriendBinder<ARG1>::create (writer) ,mThat) ;
	}
} ;

class BlankText implement Proxy {
protected:
	String<STRU8> mThat ;

public:
	imports VREF<BlankText> from (VREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	template <class ARG1>
	void friend_read (VREF<ARG1> reader) {
		return StreamTextProc::read_blank (ReaderFriendBinder<ARG1>::create (reader) ,mThat) ;
	}
} ;

class EndlineText implement Proxy {
protected:
	String<STRU8> mThat ;

public:
	imports VREF<EndlineText> from (VREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	template <class ARG1>
	void friend_read (VREF<ARG1> reader) {
		return StreamTextProc::read_endline (ReaderFriendBinder<ARG1>::create (reader) ,mThat) ;
	}
} ;

struct AlignedTextLayout {
	VAL64 mNumber ;
	LENGTH mAlign ;
} ;

class AlignedText implement AlignedTextLayout {
protected:
	using AlignedTextLayout::mNumber ;
	using AlignedTextLayout::mAlign ;

public:
	implicit AlignedText () = delete ;

	explicit AlignedText (CREF<VAL64> number ,CREF<LENGTH> align) {
		mNumber = number ;
		mAlign = align ;
	}

	template <class ARG1>
	void friend_write (VREF<ARG1> writer) const {
		return StreamTextProc::write_aligned (WriterFriendBinder<ARG1>::create (writer) ,mNumber ,mAlign) ;
	}
} ;

struct CommaImplLayout ;

struct CommaLayout implement ThisLayout<SharedRef<CommaImplLayout>> {} ;

struct CommaHolder implement Interface {
	imports VFat<CommaHolder> hold (VREF<CommaLayout> that) ;
	imports CFat<CommaHolder> hold (CREF<CommaLayout> that) ;

	virtual void initialize (CREF<Slice> indent ,CREF<Slice> comma ,CREF<Slice> endline) = 0 ;
	virtual void friend_write (VREF<WriterFriend> writer) const = 0 ;
	virtual void increase () const = 0 ;
	virtual void decrease () const = 0 ;
	virtual void tight () const = 0 ;
} ;

class Comma implement CommaLayout {
protected:
	using CommaLayout::mThis ;

public:
	implicit Comma () = default ;

	explicit Comma (CREF<Slice> indent ,CREF<Slice> comma ,CREF<Slice> endline) {
		CommaHolder::hold (thiz)->initialize (indent ,comma ,endline) ;
	}

	void friend_write (VREF<WriterFriend> writer) const {
		return CommaHolder::hold (thiz)->friend_write (writer) ;
	}

	void increase () const {
		return CommaHolder::hold (thiz)->increase () ;
	}

	forceinline void operator++ (int) const {
		return increase () ;
	}

	void decrease () const {
		return CommaHolder::hold (thiz)->decrease () ;
	}

	forceinline void operator-- (int) const {
		return decrease () ;
	}

	void tight () const {
		return CommaHolder::hold (thiz)->tight () ;
	}
} ;
} ;