#pragma once

#ifndef __CSC_STRING__
#define __CSC_STRING__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"

namespace CSC {
struct StringProcLayout ;

struct StringProcHolder implement Interface {
	imports CREF<OfThis<UniqueRef<StringProcLayout>>> instance () ;
	imports VFat<StringProcHolder> hold (VREF<StringProcLayout> that) ;
	imports CFat<StringProcHolder> hold (CREF<StringProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual String<STRA> stra_from_strw (CREF<String<STRW>> a) const = 0 ;
	virtual String<STRA> stra_from_strs (CREF<String<STR>> a) const = 0 ;
	virtual String<STRW> strw_from_stra (CREF<String<STRA>> a) const = 0 ;
	virtual String<STRW> strw_from_strs (CREF<String<STR>> a) const = 0 ;
	virtual String<STR> strs_from_stra (CREF<String<STRA>> a) const = 0 ;
	virtual String<STR> strs_from_strw (CREF<String<STRW>> a) const = 0 ;
	virtual String<STRU8> stru8_from_stru16 (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STRU8> stru8_from_stru32 (CREF<String<STRU32>> a) const = 0 ;
	virtual String<STRU16> stru16_from_stru8 (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STRU16> stru16_from_stru32 (CREF<String<STRU32>> a) const = 0 ;
	virtual String<STRU32> stru32_from_stru8 (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STRU32> stru32_from_stru16 (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STRUA> strua_from_stra (RREF<String<STRA>> a) const = 0 ;
	virtual String<STRA> stra_from_strua (RREF<String<STRUA>> a) const = 0 ;
	virtual String<STRUW> struw_from_strw (RREF<String<STRW>> a) const = 0 ;
	virtual String<STRW> strw_from_struw (RREF<String<STRUW>> a) const = 0 ;
	virtual String<STRA> stra_from_stru (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STRA> stra_from_stru (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STRA> stra_from_stru (CREF<String<STRU32>> a) const = 0 ;
	virtual String<STRW> strw_from_stru (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STRW> strw_from_stru (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STRW> strw_from_stru (CREF<String<STRU32>> a) const = 0 ;
	virtual String<STR> strs_from_stru (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STR> strs_from_stru (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STR> strs_from_stru (CREF<String<STRU32>> a) const = 0 ;
	virtual String<STRU8> stru8_from_struw (CREF<String<STRUW>> a) const = 0 ;
	virtual String<STRU16> stru16_from_struw (CREF<String<STRUW>> a) const = 0 ;
	virtual String<STRU32> stru32_from_struw (CREF<String<STRUW>> a) const = 0 ;
	virtual String<STRU8> stru8_from_strs (CREF<String<STRA>> a) const = 0 ;
	virtual String<STRU8> stru8_from_strs (CREF<String<STRW>> a) const = 0 ;
	virtual String<STRU16> stru16_from_strs (CREF<String<STRA>> a) const = 0 ;
	virtual String<STRU16> stru16_from_strs (CREF<String<STRW>> a) const = 0 ;
	virtual String<STRU32> stru32_from_strs (CREF<String<STRA>> a) const = 0 ;
	virtual String<STRU32> stru32_from_strs (CREF<String<STRW>> a) const = 0 ;
} ;

class StringProc implement OfThis<UniqueRef<StringProcLayout>> {
public:
	static CREF<StringProc> instance () {
		return keep[TYPE<StringProc>::expr] (StringProcHolder::instance ()) ;
	}

	static String<STRA> stra_from_strw (CREF<String<STRW>> a) {
		return StringProcHolder::hold (instance ())->stra_from_strw (a) ;
	}

	static String<STRA> stra_from_strs (CREF<String<STR>> a) {
		return StringProcHolder::hold (instance ())->stra_from_strs (a) ;
	}

	static String<STRW> strw_from_stra (CREF<String<STRA>> a) {
		return StringProcHolder::hold (instance ())->strw_from_stra (a) ;
	}

	static String<STRW> strw_from_strs (CREF<String<STR>> a) {
		return StringProcHolder::hold (instance ())->strw_from_strs (a) ;
	}

	static String<STR> strs_from_stra (CREF<String<STRA>> a) {
		return StringProcHolder::hold (instance ())->strs_from_stra (a) ;
	}

	static String<STR> strs_from_strw (CREF<String<STRW>> a) {
		return StringProcHolder::hold (instance ())->strs_from_strw (a) ;
	}

	static String<STRU8> stru8_from_stru16 (CREF<String<STRU16>> a) {
		return StringProcHolder::hold (instance ())->stru8_from_stru16 (a) ;
	}

	static String<STRU8> stru8_from_stru32 (CREF<String<STRU32>> a) {
		return StringProcHolder::hold (instance ())->stru8_from_stru32 (a) ;
	}

	static String<STRU16> stru16_from_stru8 (CREF<String<STRU8>> a) {
		return StringProcHolder::hold (instance ())->stru16_from_stru8 (a) ;
	}

	static String<STRU16> stru16_from_stru32 (CREF<String<STRU32>> a) {
		return StringProcHolder::hold (instance ())->stru16_from_stru32 (a) ;
	}

	static String<STRU32> stru32_from_stru8 (CREF<String<STRU8>> a) {
		return StringProcHolder::hold (instance ())->stru32_from_stru8 (a) ;
	}

	static String<STRU32> stru32_from_stru16 (CREF<String<STRU16>> a) {
		return StringProcHolder::hold (instance ())->stru32_from_stru16 (a) ;
	}

	static String<STRUA> strua_from_stra (RREF<String<STRA>> a) {
		return StringProcHolder::hold (instance ())->strua_from_stra (move (a)) ;
	}

	static String<STRA> stra_from_strua (RREF<String<STRUA>> a) {
		return StringProcHolder::hold (instance ())->stra_from_strua (move (a)) ;
	}

	static String<STRUW> struw_from_strw (RREF<String<STRW>> a) {
		return StringProcHolder::hold (instance ())->struw_from_strw (move (a)) ;
	}

	static String<STRW> strw_from_struw (RREF<String<STRUW>> a) {
		return StringProcHolder::hold (instance ())->strw_from_struw (move (a)) ;
	}

	static String<STRA> stra_from_stru (CREF<String<STRU8>> a) {
		return StringProcHolder::hold (instance ())->stra_from_stru (a) ;
	}

	static String<STRA> stra_from_stru (CREF<String<STRU16>> a) {
		return StringProcHolder::hold (instance ())->stra_from_stru (a) ;
	}

	static String<STRA> stra_from_stru (CREF<String<STRU32>> a) {
		return StringProcHolder::hold (instance ())->stra_from_stru (a) ;
	}

	static String<STRW> strw_from_stru (CREF<String<STRU8>> a) {
		return StringProcHolder::hold (instance ())->strw_from_stru (a) ;
	}

	static String<STRW> strw_from_stru (CREF<String<STRU16>> a) {
		return StringProcHolder::hold (instance ())->strw_from_stru (a) ;
	}

	static String<STRW> strw_from_stru (CREF<String<STRU32>> a) {
		return StringProcHolder::hold (instance ())->strw_from_stru (a) ;
	}

	static String<STR> strs_from_stru (CREF<String<STRU8>> a) {
		return StringProcHolder::hold (instance ())->strs_from_stru (a) ;
	}

	static String<STR> strs_from_stru (CREF<String<STRU16>> a) {
		return StringProcHolder::hold (instance ())->strs_from_stru (a) ;
	}

	static String<STR> strs_from_stru (CREF<String<STRU32>> a) {
		return StringProcHolder::hold (instance ())->strs_from_stru (a) ;
	}

	static String<STRU8> stru8_from_struw (CREF<String<STRUW>> a) {
		return StringProcHolder::hold (instance ())->stru8_from_struw (a) ;
	}

	static String<STRU16> stru16_from_struw (CREF<String<STRUW>> a) {
		return StringProcHolder::hold (instance ())->stru16_from_struw (a) ;
	}

	static String<STRU32> stru32_from_struw (CREF<String<STRUW>> a) {
		return StringProcHolder::hold (instance ())->stru32_from_struw (a) ;
	}

	static String<STRU8> stru8_from_strs (CREF<String<STRA>> a) {
		return StringProcHolder::hold (instance ())->stru8_from_strs (a) ;
	}

	static String<STRU8> stru8_from_strs (CREF<String<STRW>> a) {
		return StringProcHolder::hold (instance ())->stru8_from_strs (a) ;
	}

	static String<STRU16> stru16_from_strs (CREF<String<STRA>> a) {
		return StringProcHolder::hold (instance ())->stru16_from_strs (a) ;
	}

	static String<STRU16> stru16_from_strs (CREF<String<STRW>> a) {
		return StringProcHolder::hold (instance ())->stru16_from_strs (a) ;
	}

	static String<STRU32> stru32_from_strs (CREF<String<STRA>> a) {
		return StringProcHolder::hold (instance ())->stru32_from_strs (a) ;
	}

	static String<STRU32> stru32_from_strs (CREF<String<STRW>> a) {
		return StringProcHolder::hold (instance ())->stru32_from_strs (a) ;
	}
} ;

struct XmlParserTree ;

struct XmlParserLayout {
	Ref<XmlParserTree> mThis ;
	INDEX mIndex ;
} ;

struct XmlParserHolder implement Interface {
	imports VFat<XmlParserHolder> hold (VREF<XmlParserLayout> that) ;
	imports CFat<XmlParserHolder> hold (CREF<XmlParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<BYTE>> stream) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual XmlParserLayout root () const = 0 ;
	virtual XmlParserLayout parent () const = 0 ;
	virtual XmlParserLayout brother () const = 0 ;
	virtual XmlParserLayout child () const = 0 ;
	virtual XmlParserLayout child (CREF<INDEX> index) const = 0 ;
	virtual XmlParserLayout child (CREF<Slice> name) const = 0 ;
	virtual XmlParserLayout child (CREF<String<STRU8>> name) const = 0 ;
	virtual Array<XmlParserLayout> list () const = 0 ;
	virtual Array<XmlParserLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<XmlParserLayout> that) const = 0 ;
	virtual CREF<String<STRU8>> name () const leftvalue = 0 ;
	virtual BOOL parse (CREF<BOOL> def) const = 0 ;
	virtual VAL32 parse (CREF<VAL32> def) const = 0 ;
	virtual VAL64 parse (CREF<VAL64> def) const = 0 ;
	virtual FLT32 parse (CREF<FLT32> def) const = 0 ;
	virtual FLT64 parse (CREF<FLT64> def) const = 0 ;
	virtual String<STRA> parse (CREF<String<STRA>> def) const = 0 ;
	virtual String<STRW> parse (CREF<String<STRW>> def) const = 0 ;
	virtual String<STRU8> parse (CREF<String<STRU8>> def) const = 0 ;
	virtual String<STRU16> parse (CREF<String<STRU16>> def) const = 0 ;
	virtual String<STRU32> parse (CREF<String<STRU32>> def) const = 0 ;
	virtual Array<BOOL> parse (CREF<BOOL> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> parse (CREF<VAL32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> parse (CREF<VAL64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> parse (CREF<FLT32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> parse (CREF<FLT64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRA>> parse (CREF<String<STRA>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRW>> parse (CREF<String<STRW>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU8>> parse (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU16>> parse (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU32>> parse (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const = 0 ;
} ;

class XmlParser implement XmlParserLayout {
protected:
	using XmlParserLayout::mThis ;
	using XmlParserLayout::mIndex ;

public:
	implicit XmlParser () = default ;

	explicit XmlParser (CREF<RefBuffer<BYTE>> stream) {
		XmlParserHolder::hold (thiz)->initialize (stream) ;
	}

	BOOL exist () const {
		return XmlParserHolder::hold (thiz)->exist () ;
	}

	XmlParser root () const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->root () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser parent () const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->parent () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser brother () const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->brother () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child () const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CREF<INDEX> index) const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child (index) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CREF<Slice> name) const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CREF<String<STRU8>> name) const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	Array<XmlParser> list () const {
		ArrayLayout ret = XmlParserHolder::hold (thiz)->list () ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (ret)) ;
	}

	Array<XmlParser> list (CREF<LENGTH> size_) const {
		ArrayLayout ret = XmlParserHolder::hold (thiz)->list (size_) ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (ret)) ;
	}

	BOOL equal (CREF<XmlParser> that) const {
		return XmlParserHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<XmlParser> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<XmlParser> that) const {
		return (!equal (that)) ;
	}

	CREF<String<STRU8>> name () const leftvalue {
		return XmlParserHolder::hold (thiz)->name () ;
	}

	BOOL parse (CREF<BOOL> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	VAL32 parse (CREF<VAL32> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	VAL64 parse (CREF<VAL64> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	FLT32 parse (CREF<FLT32> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	FLT64 parse (CREF<FLT64> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRA> parse (CREF<String<STRA>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRW> parse (CREF<String<STRW>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU8> parse (CREF<String<STRU8>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU16> parse (CREF<String<STRU16>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU32> parse (CREF<String<STRU32>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	Array<BOOL> parse (CREF<BOOL> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<VAL32> parse (CREF<VAL32> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<VAL64> parse (CREF<VAL64> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<FLT32> parse (CREF<FLT32> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<FLT64> parse (CREF<FLT64> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRA>> parse (CREF<String<STRA>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRW>> parse (CREF<String<STRW>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU8>> parse (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU16>> parse (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU32>> parse (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}
} ;

struct JsonParserTree ;

struct JsonParserLayout {
	Ref<JsonParserTree> mThis ;
	INDEX mIndex ;
} ;

struct JsonParserHolder implement Interface {
	imports VFat<JsonParserHolder> hold (VREF<JsonParserLayout> that) ;
	imports CFat<JsonParserHolder> hold (CREF<JsonParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<BYTE>> stream) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual JsonParserLayout root () const = 0 ;
	virtual JsonParserLayout parent () const = 0 ;
	virtual JsonParserLayout brother () const = 0 ;
	virtual JsonParserLayout child () const = 0 ;
	virtual JsonParserLayout child (CREF<INDEX> index) const = 0 ;
	virtual JsonParserLayout child (CREF<Slice> name) const = 0 ;
	virtual JsonParserLayout child (CREF<String<STRU8>> name) const = 0 ;
	virtual Array<JsonParserLayout> list () const = 0 ;
	virtual Array<JsonParserLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<JsonParserLayout> that) const = 0 ;
	virtual CREF<String<STRU8>> name () const leftvalue = 0 ;
	virtual BOOL parse (CREF<BOOL> def) const = 0 ;
	virtual VAL32 parse (CREF<VAL32> def) const = 0 ;
	virtual VAL64 parse (CREF<VAL64> def) const = 0 ;
	virtual FLT32 parse (CREF<FLT32> def) const = 0 ;
	virtual FLT64 parse (CREF<FLT64> def) const = 0 ;
	virtual String<STRA> parse (CREF<String<STRA>> def) const = 0 ;
	virtual String<STRW> parse (CREF<String<STRW>> def) const = 0 ;
	virtual String<STRU8> parse (CREF<String<STRU8>> def) const = 0 ;
	virtual String<STRU16> parse (CREF<String<STRU16>> def) const = 0 ;
	virtual String<STRU32> parse (CREF<String<STRU32>> def) const = 0 ;
	virtual Array<BOOL> parse (CREF<BOOL> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> parse (CREF<VAL32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> parse (CREF<VAL64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> parse (CREF<FLT32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> parse (CREF<FLT64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRA>> parse (CREF<String<STRA>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRW>> parse (CREF<String<STRW>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU8>> parse (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU16>> parse (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU32>> parse (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const = 0 ;
} ;

class JsonParser implement JsonParserLayout {
protected:
	using JsonParserLayout::mThis ;
	using JsonParserLayout::mIndex ;

public:
	implicit JsonParser () = default ;

	explicit JsonParser (CREF<RefBuffer<BYTE>> stream) {
		JsonParserHolder::hold (thiz)->initialize (stream) ;
	}

	BOOL exist () const {
		return JsonParserHolder::hold (thiz)->exist () ;
	}

	JsonParser root () const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->root () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser parent () const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->parent () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser brother () const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->brother () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child () const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CREF<INDEX> index) const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child (index) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CREF<Slice> name) const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CREF<String<STRU8>> name) const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	Array<JsonParser> list () const {
		ArrayLayout ret = JsonParserHolder::hold (thiz)->list () ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (ret)) ;
	}

	Array<JsonParser> list (CREF<LENGTH> size_) const {
		ArrayLayout ret = JsonParserHolder::hold (thiz)->list (size_) ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (ret)) ;
	}

	BOOL equal (CREF<JsonParser> that) const {
		return JsonParserHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<JsonParser> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<JsonParser> that) const {
		return (!equal (that)) ;
	}

	CREF<String<STRU8>> name () const leftvalue {
		return JsonParserHolder::hold (thiz)->name () ;
	}

	BOOL parse (CREF<BOOL> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	VAL32 parse (CREF<VAL32> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	VAL64 parse (CREF<VAL64> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	FLT32 parse (CREF<FLT32> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	FLT64 parse (CREF<FLT64> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRA> parse (CREF<String<STRA>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRW> parse (CREF<String<STRW>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU8> parse (CREF<String<STRU8>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU16> parse (CREF<String<STRU16>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU32> parse (CREF<String<STRU32>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	Array<BOOL> parse (CREF<BOOL> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<VAL32> parse (CREF<VAL32> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<VAL64> parse (CREF<VAL64> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<FLT32> parse (CREF<FLT32> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<FLT64> parse (CREF<FLT64> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRA>> parse (CREF<String<STRA>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRW>> parse (CREF<String<STRW>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU8>> parse (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU16>> parse (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU32>> parse (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}
} ;

struct PlyParserTree ;

struct PlyParserGuide {
	INDEX mElement ;
	Deque<INDEX> mProperty ;
	INDEX mCol ;
	INDEX mRow ;
	INDEX mPlyBegin ;
	INDEX mPlyEnd ;
	INDEX mPlyIndex ;
	BOOL mPlyListMode ;
} ;

struct PlyParserLayout {
	Ref<PlyParserTree> mThis ;
	PlyParserGuide mGuide ;
} ;

struct PlyParserHolder implement Interface {
	imports VFat<PlyParserHolder> hold (VREF<PlyParserLayout> that) ;
	imports CFat<PlyParserHolder> hold (CREF<PlyParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<BYTE>> stream) = 0 ;
	virtual LENGTH element_size (CREF<Slice> element) const = 0 ;
	virtual LENGTH property_size (CREF<Slice> element ,CREF<Slice> property) const = 0 ;
	virtual void guide_new (CREF<Slice> element) = 0 ;
	virtual void guide_put (CREF<Slice> property) = 0 ;
	virtual void read (VREF<BOOL> item) = 0 ;
	virtual void read (VREF<VAL32> item) = 0 ;
	virtual void read (VREF<VAL64> item) = 0 ;
	virtual void read (VREF<FLT32> item) = 0 ;
	virtual void read (VREF<FLT64> item) = 0 ;
	virtual void read (VREF<BYTE> item) = 0 ;
	virtual void read (VREF<WORD> item) = 0 ;
	virtual void read (VREF<CHAR> item) = 0 ;
	virtual void read (VREF<QUAD> item) = 0 ;
} ;

class PlyParser implement PlyParserLayout {
protected:
	using PlyParserLayout::mThis ;
	using PlyParserLayout::mGuide ;

public:
	implicit PlyParser () = default ;

	explicit PlyParser (CREF<RefBuffer<BYTE>> stream) {
		PlyParserHolder::hold (thiz)->initialize (stream) ;
	}

	LENGTH element_size (CREF<Slice> element) const {
		return PlyParserHolder::hold (thiz)->element_size (element) ;
	}

	LENGTH property_size (CREF<Slice> element ,CREF<Slice> property) const {
		return PlyParserHolder::hold (thiz)->property_size (element ,property) ;
	}

	void guide_new (CREF<Slice> element) {
		return PlyParserHolder::hold (thiz)->guide_new (element) ;
	}

	void guide_put (CREF<Slice> property) {
		return PlyParserHolder::hold (thiz)->guide_put (property) ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VREF<BOOL> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<PlyParser> operator>> (VREF<BOOL> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL32> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<PlyParser> operator>> (VREF<VAL32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL64> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<PlyParser> operator>> (VREF<VAL64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT32> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<PlyParser> operator>> (VREF<FLT32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT64> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<PlyParser> operator>> (VREF<FLT64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<BYTE> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<PlyParser> operator>> (VREF<BYTE> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<WORD> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<PlyParser> operator>> (VREF<WORD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<CHAR> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<PlyParser> operator>> (VREF<CHAR> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<QUAD> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VREF<PlyParser> operator>> (VREF<QUAD> item) {
		read (item) ;
		return thiz ;
	}
} ;
} ;