#pragma once

#ifndef __CSC_STRING__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_string.hpp"

#include "csc_end.h"
#include <cstdlib>
#include <clocale>
#include "csc_begin.h"

namespace CSC {
template <class A>
struct FUNCTION_string_from_impl {
	template <class ARG1>
	forceinline CREF<String<A>> operator() (CREF<String<ARG1>> that) {
		assert (IS_SAME<A ,ARG1>::expr) ;
		return keep[TYPE<String<A>>::expr] (Pointer::from (that)) ;
	}

	template <class ARG1>
	forceinline RREF<String<A>> operator() (RREF<String<ARG1>> that) {
		assert (IS_SAME<A ,ARG1>::expr) ;
		return move (keep[TYPE<String<A>>::expr] (Pointer::from (that))) ;
	}
} ;

struct FUNCTION_string_from {
	template <class ARG1>
	forceinline consteval FUNCTION_string_from_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNCTION_string_from_impl<ARG1> () ;
	}
} ;

static constexpr auto string_from = FUNCTION_string_from () ;

#ifdef __CSC_SYSTEM_WINDOWS__
using locale_t = _locale_t ;

struct FUNCTION_string_locale {
	forceinline UniqueRef<locale_t> operator() () const {
		return UniqueRef<locale_t> ([&] (VREF<locale_t> me) {
			me = _create_locale (LC_CTYPE ,String<STRA>::zero ()) ;
		} ,[&] (VREF<locale_t> me) {
			_free_locale (me) ;
		}) ;
	}
} ;

struct FUNCTION_stra_from_strw {
	forceinline void operator() (VREF<String<STRA>> dst ,CREF<String<STRW>> src ,CREF<locale_t> loc) const {
		auto rax = csc_size_t (0) ;
		const auto r1x = _wcstombs_s_l ((&rax) ,dst ,dst.size () ,src ,_TRUNCATE ,loc) ;
		assume (r1x == 0) ;
		dst.trunc (rax) ;
	}
} ;

struct FUNCTION_strw_from_stra {
	forceinline void operator() (VREF<String<STRW>> dst ,CREF<String<STRA>> src ,CREF<locale_t> loc) const {
		auto rax = csc_size_t (0) ;
		const auto r1x = _mbstowcs_s_l ((&rax) ,dst ,dst.size () ,src ,_TRUNCATE ,loc) ;
		assume (r1x == 0) ;
		dst.trunc (rax) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_string_locale {
	forceinline UniqueRef<locale_t> operator() () const {
		return UniqueRef<locale_t> ([&] (VREF<locale_t> me) {
			me = newlocale (LC_CTYPE_MASK ,String<STRA>::zero () ,NULL) ;
		} ,[&] (VREF<locale_t> me) {
			freelocale (me) ;
		}) ;
	}
} ;

struct FUNCTION_stra_from_strw {
	forceinline void operator() (VREF<String<STRA>> dst ,CREF<String<STRW>> src ,CREF<locale_t> loc) const {
		auto rax = mbstate_t () ;
		inline_memset (rax) ;
		uselocale (loc) ;
		auto rbx = src.self ;
		const auto r1x = LENGTH (wcsrtombs (dst ,(&rbx) ,dst.size () ,(&rax))) ;
		assume (r1x >= 0) ;
		dst.trunc (r1x) ;
	}
} ;

struct FUNCTION_strw_from_stra {
	forceinline void operator() (VREF<String<STRW>> dst ,CREF<String<STRA>> src ,CREF<locale_t> loc) const {
		auto rax = mbstate_t () ;
		inline_memset (rax) ;
		uselocale (loc) ;
		auto rbx = src.self ;
		const auto r1x = LENGTH (mbsrtowcs (dst ,(&rbx) ,dst.size () ,(&rax))) ;
		assume (r1x >= 0) ;
		dst.trunc (r1x) ;
	}
} ;
#endif

static constexpr auto string_locale = FUNCTION_string_locale () ;
static constexpr auto stra_from_strw = FUNCTION_stra_from_strw () ;
static constexpr auto strw_from_stra = FUNCTION_strw_from_stra () ;

struct StringProcImplLayout {
	UniqueRef<locale_t> mStringLocale ;
} ;

class StringProcImplHolder final implement Fat<StringProcHolder ,StringProcLayout> {
public:
	void initialize () override {
		auto rax = StringProcImplLayout () ;
		rax.mStringLocale = string_locale () ;
		fake.mThis = Ref<StringProcImplLayout>::make (move (rax)) ;
	}

	String<STRA> stra_from_strw (CREF<String<STRW>> a) const override {
		String<STRA> ret = String<STRA> (a.length () * 2 + 1) ;
		CSC::stra_from_strw (ret ,a ,fake.mThis->mStringLocale) ;
		return move (ret) ;
	}

	String<STRA> stra_from_strs (CREF<String<STR>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STRA>::expr] (a) ;
		if (IS_SAME<STR ,STRW>::expr)
			return stra_from_strw (string_from[TYPE<STRW>::expr] (a)) ;
		assert (FALSE) ;
		return String<STRA> () ;
	}

	String<STRW> strw_from_stra (CREF<String<STRA>> a) const override {
		String<STRW> ret = String<STRW> (a.length () + 1) ;
		CSC::strw_from_stra (ret ,a ,fake.mThis->mStringLocale) ;
		return move (ret) ;
	}

	String<STRW> strw_from_strs (CREF<String<STR>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return strw_from_stra (string_from[TYPE<STRA>::expr] (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STRW>::expr] (a) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STR> strs_from_stra (CREF<String<STRA>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (a) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stra (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_strw (CREF<String<STRW>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_strw (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (a) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STRU8> stru8_from_stru16 (CREF<String<STRU16>> a) const override {
		String<STRU8> ret = String<STRU8> (a.length () * 3) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU16 (0X007F))
					discard ;
				ret[ix] = STRU8 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU16 (0X07FF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] < STRU16 (0XD800))
					discard ;
				if (a[i] > STRU16 (0XDBFF))
					discard ;
				rbx = STRU32 (a[i] & STRU16 (0X03FF)) ;
				rax = FLAG (1) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (1))
					discard ;
				if (a[i] < STRU16 (0XDC00))
					discard ;
				if (a[i] > STRU16 (0XDFFF))
					discard ;
				rbx = STRU32 (((rbx << 10) | (a[i] & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
				ret[ix] = (STRU8 (rbx >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
				ix++ ;
				ret[ix] = (STRU8 (rbx >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (rbx >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (rbx) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				rax = FLAG (0) ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == FLAG (0))
				discard ;
			ret[ix] = STRU8 (0X3F) ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU8> stru8_from_stru32 (CREF<String<STRU32>> a) const override {
		//@info: 1 bytes [0,0X7F] 0xxxxxxx
		//@info: 2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
		//@info: 3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
		//@info: 4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		String<STRU8> ret = String<STRU8> (a.length () * 6) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X0000007F))
					discard ;
				ret[ix] = STRU8 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X000007FF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X0000FFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X001FFFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X03FFFFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 24) & STRU8 (0X03)) | STRU8 (0XF8) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X7FFFFFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 30) & STRU8 (0X01)) | STRU8 (0XFC) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 24) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU16> stru16_from_stru8 (CREF<String<STRU8>> a) const override {
		String<STRU16> ret = String<STRU16> (a.length ()) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			if ifdo (TRUE) {
				auto act = TRUE ;
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0X7F))
						discard ;
					ret[ix] = STRU16 (a[i]) ;
					ix++ ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XDF))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X1F)) ;
					rax = FLAG (1) ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XEF))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X0F)) ;
					rax = FLAG (2) ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XF7))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X07)) ;
					rax = FLAG (3) ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XFB))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X03)) ;
					rax = FLAG (4) ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XFD))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X01)) ;
					rax = FLAG (5) ;
				}
				if ifdo (act) {
					if (rax != FLAG (1))
						discard ;
					if (a[i] > STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
					rax = FLAG (10) ;
				}
				if ifdo (act) {
					if (!inline_between (rax ,2 ,6))
						discard ;
					if (a[i] > STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
					rax-- ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = FLAG (99) ;
				}
			}
			if ifdo (TRUE) {
				if (rax != FLAG (10))
					discard ;
				auto act = TRUE ;
				if ifdo (act) {
					if (rbx > STRU32 (0X0000FFFF))
						discard ;
					ret[ix] = STRU16 (rbx) ;
					ix++ ;
					rax = FLAG (0) ;
				}
				if ifdo (act) {
					if (rbx > STRU32 (0X0010FFFF))
						discard ;
					rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
					ret[ix] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
					ix++ ;
					ret[ix] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
					ix++ ;
					rax = FLAG (0) ;
				}
				if ifdo (act) {
					if (rbx > STRU32 (0X7FFFFFFF))
						discard ;
					ret[ix] = STRU16 (0X3F) ;
					ix++ ;
					rax = FLAG (0) ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = FLAG (99) ;
				}
			}
		}
		if ifdo (TRUE) {
			if (rax == FLAG (0))
				discard ;
			ret[ix] = STRU16 (0X3F) ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU16> stru16_from_stru32 (CREF<String<STRU32>> a) const override {
		//@info: utf16 [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
		//@info: utf32 [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
		String<STRU16> ret = String<STRU16> (a.length () * 2) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X0000FFFF))
					discard ;
				ret[ix] = STRU16 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X0010FFFF))
					discard ;
				ret[ix] = (STRU16 ((a[i] - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
				ix++ ;
				ret[ix] = (STRU16 (a[i] - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X7FFFFFFF))
					discard ;
				ret[ix] = STRU16 (0X3F) ;
				ix++ ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU32> stru32_from_stru8 (CREF<String<STRU8>> a) const override {
		//@info: 1 bytes [0,0X7F] 0xxxxxxx
		//@info: 2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
		//@info: 3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
		//@info: 4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		String<STRU32> ret = String<STRU32> (a.length ()) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0X7F))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XDF))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X1F)) ;
				rax = FLAG (1) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XEF))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X0F)) ;
				rax = FLAG (2) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XF7))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X07)) ;
				rax = FLAG (3) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XFB))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X03)) ;
				rax = FLAG (4) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XFD))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X01)) ;
				rax = FLAG (5) ;
			}
			if ifdo (act) {
				if (rax != FLAG (1))
					discard ;
				if (a[i] > STRU8 (0XBF))
					discard ;
				rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
				ret[ix] = rbx ;
				ix++ ;
				rax = FLAG (0) ;
			}
			if ifdo (act) {
				if (!inline_between (rax ,2 ,6))
					discard ;
				if (a[i] > STRU8 (0XBF))
					discard ;
				rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
				rax-- ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == FLAG (0))
				discard ;
			ret[ix] = STRU32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU32> stru32_from_stru16 (CREF<String<STRU16>> a) const override {
		//@info: utf16 [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
		//@info: utf32 [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
		String<STRU32> ret = String<STRU32> (a.length ()) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU16 (0X07FF))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] < STRU16 (0XD800))
					discard ;
				if (a[i] > STRU16 (0XDBFF))
					discard ;
				rbx = STRU32 (a[i] & STRU16 (0X03FF)) ;
				rax = FLAG (1) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (1))
					discard ;
				if (a[i] < STRU16 (0XDC00))
					discard ;
				if (a[i] > STRU16 (0XDFFF))
					discard ;
				rbx = STRU32 (((rbx << 10) | (a[i] & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
				ret[ix] = rbx ;
				ix++ ;
				rax = FLAG (0) ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == FLAG (0))
				discard ;
			ret[ix] = STRU32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRUA> strua_from_stra (RREF<String<STRA>> a) const override {
		return move (keep[TYPE<String<STRUA>>::expr] (Pointer::from (a))) ;
	}

	String<STRA> stra_from_strua (RREF<String<STRUA>> a) const override {
		return move (keep[TYPE<String<STRA>>::expr] (Pointer::from (a))) ;
	}

	String<STRUW> struw_from_strw (RREF<String<STRW>> a) const override {
		return move (keep[TYPE<String<STRUW>>::expr] (Pointer::from (a))) ;
	}

	String<STRW> strw_from_struw (RREF<String<STRUW>> a) const override {
		return move (keep[TYPE<String<STRW>>::expr] (Pointer::from (a))) ;
	}

	String<STRA> stra_from_stru (CREF<String<STRU8>> a) const override {
		return stra_from_strw (strw_from_stru (a)) ;
	}

	String<STRA> stra_from_stru (CREF<String<STRU16>> a) const override {
		return stra_from_strw (strw_from_stru (a)) ;
	}

	String<STRA> stra_from_stru (CREF<String<STRU32>> a) const override {
		return stra_from_strw (strw_from_stru (a)) ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU8>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (move (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru16_from_stru8 (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru32_from_stru8 (a))) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU16>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru8_from_stru16 (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (move (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru32_from_stru16 (a))) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU32>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru8_from_stru32 (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru16_from_stru32 (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (move (a))) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU8>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU16>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU32>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STRU8> stru8_from_strw (CREF<String<STRW>> a) const override {
		const auto r1x = struw_from_strw (move (a)) ;
		if (IS_SAME<STRUW ,STRU8>::expr)
			return string_from[TYPE<STRU8>::expr] (r1x) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return stru8_from_stru16 (string_from[TYPE<STRU16>::expr] (r1x)) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return stru8_from_stru32 (string_from[TYPE<STRU32>::expr] (r1x)) ;
		assert (FALSE) ;
		return String<STRU8> () ;
	}

	String<STRU16> stru16_from_strw (CREF<String<STRW>> a) const override {
		const auto r1x = struw_from_strw (move (a)) ;
		if (IS_SAME<STRUW ,STRU8>::expr)
			return stru16_from_stru8 (string_from[TYPE<STRU8>::expr] (r1x)) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return string_from[TYPE<STRU16>::expr] (r1x) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return stru16_from_stru32 (string_from[TYPE<STRU32>::expr] (r1x)) ;
		assert (FALSE) ;
		return String<STRU16> () ;
	}

	String<STRU32> stru32_from_strw (CREF<String<STRW>> a) const override {
		const auto r1x = struw_from_strw (move (a)) ;
		if (IS_SAME<STRUW ,STRU8>::expr)
			return stru32_from_stru8 (string_from[TYPE<STRU8>::expr] (r1x)) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return stru32_from_stru16 (string_from[TYPE<STRU16>::expr] (r1x)) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return string_from[TYPE<STRU32>::expr] (r1x) ;
		assert (FALSE) ;
		return String<STRU32> () ;
	}
} ;

exports CREF<StringProcLayout> StringProcHolder::instance () {
	return memorize ([&] () {
		StringProcLayout ret ;
		StringProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<StringProcHolder> StringProcHolder::hold (VREF<StringProcLayout> that) {
	return VFat<StringProcHolder> (StringProcImplHolder () ,that) ;
}

exports CFat<StringProcHolder> StringProcHolder::hold (CREF<StringProcLayout> that) {
	return CFat<StringProcHolder> (StringProcImplHolder () ,that) ;
}

struct RegularReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	StreamShape mBackup ;
	Box<TextReader> mTextReader ;
	Deque<STRU32> mDeque ;
	STRU32 mTop ;
} ;

class RegularReader implement RegularReaderLayout {
protected:
	using RegularReaderLayout::mStream ;
	using RegularReaderLayout::mBackup ;
	using RegularReaderLayout::mTextReader ;
	using RegularReaderLayout::mDeque ;
	using RegularReaderLayout::mTop ;

public:
	implicit RegularReader () = default ;

	explicit RegularReader (RREF<Ref<RefBuffer<BYTE>>> stream ,CREF<LENGTH> ring_size) {
		mStream = move (stream) ;
		mBackup.mRead = 0 ;
		mBackup.mWrite = mStream->size () ;
		mDeque = Deque<STRU32> (ring_size) ;
	}

	void use_text () {
		mTextReader = Box<TextReader>::make (mStream.share ()) ;
		mTextReader->reset (mBackup) ;
		mDeque.clear () ;
		while (TRUE) {
			if (mDeque.length () >= mDeque.size ())
				break ;
			mTextReader.self >> mTop ;
			mDeque.add (mTop) ;
		}
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const {
		item = mDeque[index] ;
	}

	forceinline STRU32 operator[] (CREF<INDEX> index) const {
		STRU32 ret ;
		get (index ,ret) ;
		return move (ret) ;
	}

	template <class ARG1>
	void read (XREF<ARG1> item) {
		mTextReader->reset (mBackup) ;
		mTextReader.self >> item ;
		mBackup = mTextReader->backup () ;
		mDeque.clear () ;
		while (TRUE) {
			if (mDeque.length () >= mDeque.size ())
				break ;
			mTextReader.self >> mTop ;
			mDeque.add (mTop) ;
		}
	}

	template <class ARG1>
	forceinline VREF<RegularReader> operator>> (XREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}

	void next () {
		mTextReader.self >> mTop ;
		if ifdo (TRUE) {
			if (mTop == STRU32 (0X00))
				discard ;
			mBackup.mRead++ ;
		}
		mDeque.take () ;
		mDeque.add (mTop) ;
	}

	forceinline void operator++ (int) {
		next () ;
	}
} ;

class ScopeCounter implement Proxy {
private:
	using COUNTER_MAX_DEPTH = ENUM<256> ;

protected:
	Pin<LENGTH> mThat ;

public:
	static CREF<ScopeCounter> from (VREF<LENGTH> that) {
		return Pointer::from (that) ;
	}

	void enter () const {
		auto rax = LENGTH () ;
		mThat.get (rax) ;
		rax++ ;
		assume (rax < COUNTER_MAX_DEPTH::expr) ;
		mThat.set (rax) ;
	}

	void leave () const {
		auto rax = LENGTH () ;
		mThat.get (rax) ;
		rax-- ;
		mThat.set (rax) ;
	}
} ;

struct XmlParserNodeType {
	enum {
		Value ,
		Array ,
		Object ,
		ETC
	} ;
} ;

struct XmlParserNode {
	String<STRU8> mName ;
	String<STRU8> mValue ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	Just<XmlParserNodeType> mType ;
	INDEX mParent ;
	INDEX mBrother ;
	INDEX mChild ;
} ;

struct XmlParserImplLayout {
	Array<XmlParserNode> mTree ;
	INDEX mRoot ;
} ;

struct MakeXmlParserLayout {
	RegularReader mReader ;
	LENGTH mRecursiveCounter ;
	List<XmlParserNode> mTree ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	INDEX mLastIndex ;
	String<STRU8> mLastString ;
} ;

class MakeXmlParser implement MakeXmlParserLayout {
protected:
	using MakeXmlParserLayout::mReader ;
	using MakeXmlParserLayout::mRecursiveCounter ;
	using MakeXmlParserLayout::mTree ;
	using MakeXmlParserLayout::mArrayMap ;
	using MakeXmlParserLayout::mObjectMap ;
	using MakeXmlParserLayout::mLastIndex ;
	using MakeXmlParserLayout::mLastString ;

public:
	implicit MakeXmlParser () = default ;

	explicit MakeXmlParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mReader = RegularReader (move (stream) ,5) ;
		mReader.use_text () ;
		mRecursiveCounter = 0 ;
		mArrayMap = SortedMap<INDEX> (ALLOCATOR_MIN_SIZE::expr) ;
		mObjectMap = SortedMap<String<STRU8>> (ALLOCATOR_MIN_SIZE::expr) ;
	}

	XmlParserImplLayout poll () {
		XmlParserImplLayout ret ;
		ret.mTree = Array<XmlParserNode> (mTree.length ()) ;
		const auto r1x = Array<INDEX>::make (mTree.range ()) ;
		for (auto &&i : ret.mTree.range ()) {
			ret.mTree[i] = move (mTree[r1x[i]]) ;
			const auto r2x = ret.mTree[i].mArrayMap.length () ;
			ret.mTree[i].mArrayMap.remap () ;
			assume (ret.mTree[i].mArrayMap.length () == r2x) ;
			const auto r3x = ret.mTree[i].mObjectMap.length () ;
			ret.mTree[i].mObjectMap.remap () ;
			assume (ret.mTree[i].mObjectMap.length () == r3x) ;
		}
		ret.mRoot = NONE ;
		if ifdo (TRUE) {
			if (r1x.length () == 0)
				discard ;
			ret.mRoot = 0 ;
		}
		return move (ret) ;
	}

	void generate () {
		//@info: $0->$10 $9 $11
		//@info: $1->${keyword}
		//@info: $2->"${escape}"
		//@info: $3->$1 = $2
		//@info: $4->${eps}|$3 $4
		//@info: $5-><$1 $4 />|<$1 $4 > $8 </$1 >
		//@info: $6-><!--comment-->
		//@info: $7->${[^<>]+}
		//@info: $8->$5 $8|$6 $8|$7 $8
		//@info: $9->${eps}|$5
		//@info: $10->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		//@info: $11->${end}
		read_shift_e0 () ;
		assume (mLastIndex == 0) ;
	}

	void read_shift_e0 () {
		read_shift_e10 () ;
		mReader >> GAP ;
		read_shift_e9 () ;
		mReader >> GAP ;
		read_shift_e11 () ;
	}

	//@info: $1->${keyword}
	void read_shift_e1 () {
		mReader >> KeywordText::from (mLastString) ;
	}

	//@info: $2->"${escape}"
	void read_shift_e2 () {
		mReader >> EscapeText::from (mLastString) ;
	}

	//@info: $3->$1 = $2
	void read_shift_e3 (CREF<INDEX> curr) {
		INDEX ix = mTree.insert () ;
		read_shift_e1 () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mType = XmlParserNodeType::Value ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		mReader >> slice ("=") ;
		mReader >> GAP ;
		read_shift_e2 () ;
		mTree[ix].mValue = move (mLastString) ;
		mLastIndex = ix ;
	}

	//@info: $4->${eps}|$3 $4
	void read_shift_e4 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			if (!StreamProc::is_word (mReader[0]))
				break ;
			read_shift_e3 (curr) ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != NONE)
					discard ;
				ix = mLastIndex ;
				iy = mLastIndex ;
			}
			if ifdo (act) {
				mTree[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
			mTree[curr].mObjectMap.add (mTree[iy].mName ,iy) ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	//@info: $5-><$1 $4 />|<$1 $4 > $8 </$1 >
	void read_shift_e5 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		mReader >> slice ("<") ;
		INDEX ix = mTree.insert () ;
		read_shift_e1 () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mObjectMap = mObjectMap.share () ;
		mTree[ix].mType = XmlParserNodeType::Value ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		read_shift_e4 (ix) ;
		INDEX iy = mLastIndex ;
		mReader >> GAP ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mReader[0] != STRU32 ('>'))
				discard ;
			mReader++ ;
			mReader >> GAP ;
			mTree[ix].mArrayMap = mArrayMap.share () ;
			read_shift_e8 (ix ,iy) ;
			mTree[ix].mChild = mLastIndex ;
			mReader >> GAP ;
			mReader >> slice ("</") ;
			read_shift_e1 () ;
			assume (mTree[ix].mName == mLastString) ;
			mReader >> GAP ;
			mReader >> slice (">") ;
		}
		if ifdo (act) {
			mReader >> slice ("/>") ;
		}
		mLastIndex = ix ;
	}

	//@info: $6-><!--${comment}-->
	void read_shift_e6 () {
		mReader >> slice ("<!--") ;
		while (TRUE) {
			if (mReader[0] == STRU32 ('-'))
				if (mReader[1] == STRU32 ('-'))
					if (mReader[2] == STRU32 ('>'))
						break ;
			mReader++ ;
		}
		mReader >> slice ("-->") ;
	}

	//@info: $7->${[^<>]+}
	void read_shift_e7 (CREF<INDEX> curr) {
		auto rax = String<STRU8>::make () ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (mReader[0] == STRU32 ('<'))
				break ;
			if (mReader[0] == STRU32 ('>'))
				break ;
			assume (ix < rax.size ()) ;
			rax[ix] = STRU8 (mReader[0]) ;
			ix++ ;
			mReader++ ;
		}
		while (TRUE) {
			ix-- ;
			if (ix < 0)
				break ;
			if (!StreamProc::is_blank (rax[ix]))
				break ;
		}
		ix++ ;
		rax.trunc (ix) ;
		if ifdo (TRUE) {
			if (mTree[curr].mValue.size () > 0)
				discard ;
			mTree[curr].mValue = move (rax) ;
		}
		mTree[curr].mValue.splice (mTree[curr].mValue.length () ,rax) ;
	}

	//@info: $8->$5 $8|$6 $8|$7 $8
	void read_shift_e8 (CREF<INDEX> curr ,CREF<INDEX> first) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		INDEX ix = first ;
		INDEX iy = first ;
		while (TRUE) {
			auto act = TRUE ;
			if ifdo (TRUE) {
				if (!is_first_of_object ())
					discard ;
				read_shift_e5 (curr) ;
				if ifdo (act) {
					if (ix != NONE)
						discard ;
					ix = mLastIndex ;
					iy = mLastIndex ;
				}
				if ifdo (act) {
					mTree[iy].mBrother = mLastIndex ;
					iy = mLastIndex ;
				}
				const auto r1x = mTree[curr].mArrayMap.length () ;
				mTree[curr].mArrayMap.add (r1x ,iy) ;
				mTree[curr].mObjectMap.add (mTree[iy].mName ,iy) ;
			}
			if ifdo (act) {
				if (!is_first_of_comment ())
					discard ;
				read_shift_e6 () ;
			}
			mReader >> GAP ;
			if (mReader[0] == STRU32 ('<'))
				if (mReader[1] == STRU32 ('/'))
					break ;
			if ifdo (act) {
				read_shift_e7 (curr) ;
			}
		}
		mLastIndex = ix ;
	}

	BOOL is_first_of_comment () const {
		if (mReader[0] == STRU32 ('<'))
			if (mReader[1] == STRU32 ('!'))
				return TRUE ;
		return FALSE ;
	}

	BOOL is_first_of_object () const {
		if (mReader[0] == STRU32 ('<'))
			if (mReader[1] != STRU32 ('/'))
				return TRUE ;
		return FALSE ;
	}

	//@info: $9->${eps}|$5
	void read_shift_e9 () {
		INDEX ix = NONE ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (0X00))
				discard ;
			read_shift_e5 (NONE) ;
			ix = mLastIndex ;
		}
		mLastIndex = ix ;
	}

	//@info: $10->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
	void read_shift_e10 () {
		mReader >> BOM ;
		if (mReader[0] != STRU32 ('<'))
			return ;
		if (mReader[1] != STRU32 ('?'))
			return ;
		mReader++ ;
		mReader++ ;
		mReader >> slice ("xml") ;
		mReader >> GAP ;
		mReader >> slice ("version") ;
		mReader >> GAP ;
		mReader >> slice ("=") ;
		mReader >> GAP ;
		mReader >> slice ("\"1.0\"") ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 ('?'))
				discard ;
			mReader >> slice ("encoding") ;
			mReader >> GAP ;
			mReader >> slice ("=") ;
			mReader >> GAP ;
			mReader >> slice ("\"utf-8\"") ;
			mReader >> GAP ;
		}
		mReader >> slice ("?>") ;
	}

	//@info: $11->${end}
	void read_shift_e11 () {
		assume (mReader[0] == STRU32 (0X00)) ;
	}
} ;

class XmlParserImplHolder final implement Fat<XmlParserHolder ,XmlParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakeXmlParser (Ref<RefBuffer<BYTE>>::reference (stream)) ;
		rax.generate () ;
		fake.mThis = Ref<XmlParserImplLayout>::make (rax.poll ()) ;
		fake.mIndex = fake.mThis->mRoot ;
	}

	void initialize (CREF<XmlParserLayout> that) override {
		fake.mThis = that.mThis.share () ;
		fake.mIndex = that.mIndex ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mIndex == NONE)
			return FALSE ;
		return TRUE ;
	}

	XmlParserLayout root () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mRoot ;
		}
		return move (ret) ;
	}

	XmlParserLayout parent () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mParent ;
		}
		return move (ret) ;
	}

	XmlParserLayout brother () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	XmlParserLayout child () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mChild ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<INDEX> index) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<Slice> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<String<STRU8>> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list () const override {
		Array<XmlParserLayout> ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArrayMap.length () ;
			ret = Array<XmlParserLayout> (r1x) ;
			for (auto &&i : iter (0 ,r1x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (i) ;
			}
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list (CREF<LENGTH> size_) const override {
		Array<XmlParserLayout> ret = Array<XmlParserLayout> (size_) ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArrayMap.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : iter (0 ,r2x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (i) ;
			}
		}
		return move (ret) ;
	}

	BOOL equal (CREF<XmlParserLayout> that) const override {
		const auto r1x = inline_compr (fake.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!fake.mThis.exist ())
			return TRUE ;
		if (address (fake.mThis.self) != address (that.mThis.self))
			return FALSE ;
		if (fake.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CREF<String<STRU8>> name () const leftvalue override {
		assert (exist ()) ;
		return fake.mThis->mTree[fake.mIndex].mName ;
	}

	BOOL parse (CREF<BOOL> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<BOOL>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 parse (CREF<VAL32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL32>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 parse (CREF<VAL64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL64>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 parse (CREF<FLT32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT32>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 parse (CREF<FLT64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT64>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> parse (CREF<String<STRA>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stra_from_stru (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRW> parse (CREF<String<STRW>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::strw_from_stru (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU8> parse (CREF<String<STRU8>> def) const override {
		if (!exist ())
			return def ;
		try {
			return fake.mThis->mTree[fake.mIndex].mValue ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU16> parse (CREF<String<STRU16>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru16_from_stru8 (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU32> parse (CREF<String<STRU32>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru32_from_stru8 (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<BOOL> parse (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<VAL32> parse (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<VAL64> parse (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<FLT32> parse (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<FLT64> parse (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRA>> parse (CREF<String<STRA>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRW>> parse (CREF<String<STRW>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU8>> parse (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU16>> parse (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU32>> parse (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	template <class ARG1>
	forceinline Array<ARG1> parse_impl (CREF<ARG1> def ,CREF<LENGTH> size_) const {
		const auto r1x = list () ;
		assume (r1x.size () == size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : ret.range ()) {
			const auto r2x = XmlParserHolder::hold (fake)->child (i) ;
			ret[i] = XmlParserHolder::hold (r2x)->parse (def) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<XmlParserHolder> XmlParserHolder::hold (VREF<XmlParserLayout> that) {
	return VFat<XmlParserHolder> (XmlParserImplHolder () ,that) ;
}

exports CFat<XmlParserHolder> XmlParserHolder::hold (CREF<XmlParserLayout> that) {
	return CFat<XmlParserHolder> (XmlParserImplHolder () ,that) ;
}

struct JsonParserNodeType {
	enum {
		Value ,
		Array ,
		Object ,
		ETC
	} ;
} ;

struct JsonParserNode {
	String<STRU8> mName ;
	String<STRU8> mValue ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	Just<JsonParserNodeType> mType ;
	INDEX mParent ;
	INDEX mBrother ;
	INDEX mChild ;
} ;

struct JsonParserImplLayout {
	Array<JsonParserNode> mTree ;
	INDEX mRoot ;
} ;

struct MakeJsonParserLayout {
	RegularReader mReader ;
	LENGTH mRecursiveCounter ;
	List<JsonParserNode> mTree ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	INDEX mLastIndex ;
	String<STRU8> mLastString ;
} ;

class MakeJsonParser implement MakeJsonParserLayout {
protected:
	using MakeJsonParserLayout::mReader ;
	using MakeJsonParserLayout::mRecursiveCounter ;
	using MakeJsonParserLayout::mTree ;
	using MakeJsonParserLayout::mArrayMap ;
	using MakeJsonParserLayout::mObjectMap ;
	using MakeJsonParserLayout::mLastIndex ;
	using MakeJsonParserLayout::mLastString ;

public:
	implicit MakeJsonParser () = default ;

	explicit MakeJsonParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mReader = RegularReader (move (stream) ,5) ;
		mReader.use_text () ;
		mRecursiveCounter = 0 ;
		mArrayMap = SortedMap<INDEX> (ALLOCATOR_MIN_SIZE::expr) ;
		mObjectMap = SortedMap<String<STRU8>> (ALLOCATOR_MIN_SIZE::expr) ;
	}

	JsonParserImplLayout poll () {
		JsonParserImplLayout ret ;
		ret.mTree = Array<JsonParserNode> (mTree.length ()) ;
		const auto r1x = Array<INDEX>::make (mTree.range ()) ;
		for (auto &&i : ret.mTree.range ()) {
			ret.mTree[i] = move (mTree[r1x[i]]) ;
			const auto r2x = ret.mTree[i].mArrayMap.length () ;
			ret.mTree[i].mArrayMap.remap () ;
			assume (ret.mTree[i].mArrayMap.length () == r2x) ;
			const auto r3x = ret.mTree[i].mObjectMap.length () ;
			ret.mTree[i].mObjectMap.remap () ;
			assume (ret.mTree[i].mObjectMap.length () == r3x) ;
		}
		ret.mRoot = NONE ;
		if ifdo (TRUE) {
			if (r1x.length () == 0)
				discard ;
			ret.mRoot = 0 ;
		}
		return move (ret) ;
	}

	void generate () {
		//@info: $0->$11 $10 $12
		//@info: $1->${scalar}
		//@info: $2->true|false|null
		//@info: $3->"${escape}"
		//@info: $4->$1|$2|$3|$6|$9
		//@info: $5->$4|$4 , $5
		//@info: $6->[ ]|[ $5 ]
		//@info: $7->$3 : $4
		//@info: $8->$7|$7 , $8
		//@info: $9->{ }|{ $8 }
		//@info: $10->${eps}|$4
		//@info: $11->${eps}
		//@info: $12->${end}
		read_shift_e0 () ;
		assert (mLastIndex == 0) ;
	}

	//@info: $0->$11 $10 $12
	void read_shift_e0 () {
		read_shift_e11 () ;
		mReader >> GAP ;
		read_shift_e10 () ;
		mReader >> GAP ;
		read_shift_e12 () ;
	}

	//@info: $1->${scalar}
	void read_shift_e1 () {
		mReader >> ScalarText::from (mLastString) ;
	}

	//@info: $2->true|false|null
	void read_shift_e2 () {
		auto act = TRUE ;
		if ifdo (act) {
			if (mReader[0] != STRU32 ('t'))
				discard ;
			mReader >> slice ("true") ;
			mLastString = String<STRU8>::make (slice ("true")) ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('f'))
				discard ;
			mReader >> slice ("false") ;
			mLastString = String<STRU8>::make (slice ("false")) ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('n'))
				discard ;
			mReader >> slice ("null") ;
			mLastString = String<STRU8> () ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	//@info: $3->"${escape}"
	void read_shift_e3 () {
		mReader >> EscapeText::from (mLastString) ;
	}

	//@info: $4->$1|$2|$3|$6|$9
	void read_shift_e4 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		INDEX ix = NONE ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!is_first_of_number ())
				discard ;
			ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			read_shift_e1 () ;
			mTree[ix].mValue = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (!is_first_of_const ())
				discard ;
			ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			read_shift_e2 () ;
			mTree[ix].mValue = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('\"'))
				discard ;
			ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			read_shift_e3 () ;
			mTree[ix].mValue = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('['))
				discard ;
			read_shift_e6 (curr) ;
			ix = mLastIndex ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('{'))
				discard ;
			read_shift_e9 (curr) ;
			ix = mLastIndex ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
		mLastIndex = ix ;
	}

	BOOL is_first_of_number () const {
		if (mReader[0] == STRU32 ('+'))
			return TRUE ;
		if (mReader[0] == STRU32 ('-'))
			return TRUE ;
		if (mReader[0] >= STRU32 ('0'))
			if (mReader[0] <= STRU32 ('9'))
				return TRUE ;
		return FALSE ;
	}

	BOOL is_first_of_const () const {
		if (mReader[0] == STRU32 ('t'))
			return TRUE ;
		if (mReader[0] == STRU32 ('f'))
			return TRUE ;
		if (mReader[0] == STRU32 ('n'))
			return TRUE ;
		return FALSE ;
	}

	//@info: $5->$4|$4 , $5
	void read_shift_e5 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			read_shift_e4 (curr) ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != NONE)
					discard ;
				ix = mLastIndex ;
				iy = mLastIndex ;
			}
			if ifdo (act) {
				mTree[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
			const auto r1x = mTree[curr].mArrayMap.length () ;
			mTree[curr].mArrayMap.add (r1x ,iy) ;
			mReader >> GAP ;
			if (mReader[0] != STRU32 (','))
				break ;
			mReader++ ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	//@info: $6->[ ]|[ $5 ]
	void read_shift_e6 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		mReader >> slice ("[") ;
		INDEX ix = mTree.insert () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mArrayMap = mArrayMap.share () ;
		mTree[ix].mType = JsonParserNodeType::Array ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (']'))
				break ;
			read_shift_e5 (ix) ;
			mTree[ix].mChild = mLastIndex ;
			mReader >> GAP ;
		}
		mReader >> slice ("]") ;
		mLastIndex = ix ;
	}

	//@info: $7->$3 : $4
	void read_shift_e7 (CREF<INDEX> curr) {
		read_shift_e3 () ;
		mReader >> GAP ;
		mReader >> slice (":") ;
		mReader >> GAP ;
		read_shift_e4 (curr) ;
	}

	//@info: $8->$7|$7 , $8
	void read_shift_e8 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			read_shift_e7 (curr) ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != NONE)
					discard ;
				ix = mLastIndex ;
				iy = mLastIndex ;
			}
			if ifdo (act) {
				mTree[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
			const auto r1x = mTree[curr].mArrayMap.length () ;
			mTree[curr].mArrayMap.add (r1x ,iy) ;
			mTree[curr].mObjectMap.add (mTree[iy].mName ,iy) ;
			mReader >> GAP ;
			if (mReader[0] != STRU32 (','))
				break ;
			mReader++ ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	//@info: $9->{ }|{ $8 }
	void read_shift_e9 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		mReader >> slice ("{") ;
		INDEX ix = mTree.insert () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mArrayMap = mArrayMap.share () ;
		mTree[ix].mObjectMap = mObjectMap.share () ;
		mTree[ix].mType = JsonParserNodeType::Object ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 ('}'))
				discard ;
			read_shift_e8 (ix) ;
			mTree[ix].mChild = mLastIndex ;
			mReader >> GAP ;
		}
		mReader >> slice ("}") ;
		mLastIndex = ix ;
	}

	//@info: $10->${eps}|$4
	void read_shift_e10 () {
		INDEX ix = NONE ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (0X00))
				discard ;
			read_shift_e4 (NONE) ;
			ix = mLastIndex ;
		}
		mLastIndex = ix ;
	}

	//@info: $11->${eps}
	void read_shift_e11 () {
		mReader >> BOM ;
	}

	//@info: $12->${end}
	void read_shift_e12 () {
		assume (mReader[0] == STRU32 (0X00)) ;
	}
} ;

class JsonParserImplHolder final implement Fat<JsonParserHolder ,JsonParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakeJsonParser (Ref<RefBuffer<BYTE>>::reference (stream)) ;
		rax.generate () ;
		fake.mThis = Ref<JsonParserImplLayout>::make (rax.poll ()) ;
		fake.mIndex = fake.mThis->mRoot ;
	}

	void initialize (CREF<JsonParserLayout> that) override {
		fake.mThis = that.mThis.share () ;
		fake.mIndex = that.mIndex ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mIndex == NONE)
			return FALSE ;
		return TRUE ;
	}

	JsonParserLayout root () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mRoot ;
		}
		return move (ret) ;
	}

	JsonParserLayout parent () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mParent ;
		}
		return move (ret) ;
	}

	JsonParserLayout brother () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	JsonParserLayout child () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mChild ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<INDEX> index) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<Slice> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<String<STRU8>> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list () const override {
		Array<JsonParserLayout> ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArrayMap.length () ;
			ret = Array<JsonParserLayout> (r1x) ;
			for (auto &&i : iter (0 ,r1x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (i) ;
			}
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list (CREF<LENGTH> size_) const override {
		Array<JsonParserLayout> ret = Array<JsonParserLayout> (size_) ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArrayMap.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : iter (0 ,r2x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (i) ;
			}
		}
		return move (ret) ;
	}

	BOOL equal (CREF<JsonParserLayout> that) const override {
		const auto r1x = inline_compr (fake.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!fake.mThis.exist ())
			return TRUE ;
		if (address (fake.mThis.self) != address (that.mThis.self))
			return FALSE ;
		if (fake.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CREF<String<STRU8>> name () const leftvalue override {
		assert (exist ()) ;
		return fake.mThis->mTree[fake.mIndex].mName ;
	}

	BOOL parse (CREF<BOOL> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<BOOL>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 parse (CREF<VAL32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL32>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 parse (CREF<VAL64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL64>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 parse (CREF<FLT32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT32>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 parse (CREF<FLT64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT64>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> parse (CREF<String<STRA>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stra_from_stru (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRW> parse (CREF<String<STRW>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::strw_from_stru (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU8> parse (CREF<String<STRU8>> def) const override {
		if (!exist ())
			return def ;
		try {
			return fake.mThis->mTree[fake.mIndex].mValue ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU16> parse (CREF<String<STRU16>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru16_from_stru8 (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU32> parse (CREF<String<STRU32>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru32_from_stru8 (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<BOOL> parse (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<VAL32> parse (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<VAL64> parse (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<FLT32> parse (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<FLT64> parse (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRA>> parse (CREF<String<STRA>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRW>> parse (CREF<String<STRW>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU8>> parse (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU16>> parse (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU32>> parse (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	template <class ARG1>
	forceinline Array<ARG1> parse_impl (CREF<ARG1> def ,CREF<LENGTH> size_) const {
		const auto r1x = list () ;
		assume (r1x.size () == size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : ret.range ()) {
			const auto r2x = JsonParserHolder::hold (fake)->child (i) ;
			ret[i] = JsonParserHolder::hold (r2x)->parse (def) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<JsonParserHolder> JsonParserHolder::hold (VREF<JsonParserLayout> that) {
	return VFat<JsonParserHolder> (JsonParserImplHolder () ,that) ;
}

exports CFat<JsonParserHolder> JsonParserHolder::hold (CREF<JsonParserLayout> that) {
	return CFat<JsonParserHolder> (JsonParserImplHolder () ,that) ;
}

struct PlyParserDataType {
	enum {
		Bool ,
		Val32 ,
		Val64 ,
		Flt32 ,
		Flt64 ,
		Byte ,
		Word ,
		Char ,
		Quad ,
		Null ,
		ETC
	} ;
} ;

struct PlyParserProperty {
	String<STRU8> mName ;
	Just<PlyParserDataType> mType ;
	Just<PlyParserDataType> mListType ;
	LENGTH mListSize ;
	INDEX mPlyBegin ;
	INDEX mPlyEnd ;
} ;

struct PlyParserElement {
	String<STRU8> mName ;
	LENGTH mLineSize ;
	LENGTH mLineStep ;
	LENGTH mLineLength ;
	ArrayList<PlyParserProperty> mPropertyList ;
	Set<String<STRU8>> mPropertySet ;
	RefBuffer<BYTE> mPlyBuffer ;
	INDEX mPlyIndex ;
} ;

struct PlyParserImplLayout {
	String<STRU8> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<STRU8>> mElementSet ;
} ;

struct MakePlyParserLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	TextReader mTextReader ;
	ByteReader mByteReader ;
	String<STRU8> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<STRU8>> mElementSet ;
	BOOL mReverseFlag ;
	StreamShape mBodyBackup ;
	INDEX mLastIndex ;
	String<STRU8> mLastType ;
	String<STRU8> mLastString ;
	Set<String<STRU8>> mPropertyType ;
	Set<String<STRU8>> mPropertyListType ;
} ;

class MakePlyParser implement MakePlyParserLayout {
protected:
	using MakePlyParserLayout::mStream ;
	using MakePlyParserLayout::mTextReader ;
	using MakePlyParserLayout::mByteReader ;
	using MakePlyParserLayout::mFormat ;
	using MakePlyParserLayout::mElementList ;
	using MakePlyParserLayout::mElementSet ;
	using MakePlyParserLayout::mReverseFlag ;
	using MakePlyParserLayout::mLastIndex ;
	using MakePlyParserLayout::mLastType ;
	using MakePlyParserLayout::mLastString ;
	using MakePlyParserLayout::mPropertyType ;
	using MakePlyParserLayout::mPropertyListType ;

public:
	implicit MakePlyParser () = default ;

	explicit MakePlyParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mStream = move (stream) ;
		mPropertyType.add (slice ("bool") ,PlyParserDataType::Bool) ;
		mPropertyType.add (slice ("int") ,PlyParserDataType::Val32) ;
		mPropertyType.add (slice ("int64") ,PlyParserDataType::Val64) ;
		mPropertyType.add (slice ("float") ,PlyParserDataType::Flt32) ;
		mPropertyType.add (slice ("double") ,PlyParserDataType::Flt64) ;
		mPropertyType.add (slice ("uchar") ,PlyParserDataType::Byte) ;
		mPropertyType.add (slice ("ushort") ,PlyParserDataType::Word) ;
		mPropertyType.add (slice ("uint") ,PlyParserDataType::Char) ;
		mPropertyType.add (slice ("uint64") ,PlyParserDataType::Quad) ;
		mPropertyListType.add (slice ("uchar") ,PlyParserDataType::Byte) ;
		mPropertyListType.add (slice ("ushort") ,PlyParserDataType::Word) ;
		mPropertyListType.add (slice ("uint") ,PlyParserDataType::Char) ;
	}

	PlyParserImplLayout poll () {
		PlyParserImplLayout ret ;
		ret.mFormat = move (mFormat) ;
		ret.mElementList = move (mElementList) ;
		ret.mElementSet = move (mElementSet) ;
		return move (ret) ;
	}

	void generate () {
		read_header () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mFormat != slice ("ascii"))
				discard ;
			read_body_text () ;
		}
		if ifdo (act) {
			if (mFormat != slice ("binary_big_endian"))
				discard ;
			mReverseFlag = FALSE ;
			read_body_byte () ;
		}
		if ifdo (act) {
			if (mFormat != slice ("binary_little_endian"))
				discard ;
			mReverseFlag = TRUE ;
			read_body_byte () ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	void read_header () {
		mTextReader = TextReader (mStream.share ()) ;
		mTextReader >> BOM ;
		mTextReader >> slice ("ply") ;
		mTextReader >> GAP ;
		mTextReader >> slice ("format") ;
		mTextReader >> GAP ;
		mTextReader >> KeywordText::from (mLastString) ;
		mFormat = move (mLastString) ;
		mTextReader >> EndlineText::from (mLastString) ;
		mTextReader >> GAP ;
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			mTextReader >> KeywordText::from (mLastString) ;
			if (mLastString == slice ("end_header"))
				break ;
			mTextReader >> GAP ;
			auto act = TRUE ;
			if ifdo (act) {
				if (mLastString != slice ("element"))
					discard ;
				ix = mElementList.insert () ;
				mTextReader >> KeywordText::from (mLastString) ;
				mElementList[ix].mName = move (mLastString) ;
				mTextReader >> GAP ;
				mTextReader >> ScalarText::from (mLastString) ;
				const auto r1x = StringParse<LENGTH>::make (mLastString) ;
				assume (r1x >= 0) ;
				mElementList[ix].mLineSize = r1x ;
				mElementList[ix].mLineStep = 0 ;
				mElementList[ix].mLineLength = 0 ;
				mElementList[ix].mPropertyList = ArrayList<PlyParserProperty> (r1x) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("property"))
					discard ;
				assume (ix != NONE) ;
				mTextReader >> KeywordText::from (mLastType) ;
				mTextReader >> GAP ;
				if (mLastType != slice ("list"))
					discard ;
				iy = mElementList[ix].mPropertyList.insert () ;
				mTextReader >> KeywordText::from (mLastString) ;
				const auto r2x = mPropertyListType.map (mLastString) ;
				assume (r2x != NONE) ;
				mElementList[ix].mPropertyList[iy].mType = r2x ;
				mElementList[ix].mPropertyList[iy].mPlyBegin = mElementList[ix].mLineStep ;
				const auto r3x = ply_parser_data_type_size (r2x) ;
				mElementList[ix].mLineStep += r3x ;
				mElementList[ix].mPropertyList[iy].mPlyEnd = mElementList[ix].mLineStep ;
				mTextReader >> GAP ;
				mTextReader >> KeywordText::from (mLastString) ;
				const auto r4x = mPropertyType.map (mLastString) ;
				if ifdo (TRUE) {
					if (r4x == PlyParserDataType::Val32)
						discard ;
					if (r4x == PlyParserDataType::Val64)
						discard ;
					assume (FALSE) ;
				}
				mElementList[ix].mPropertyList[iy].mListType = r4x ;
				mElementList[ix].mPropertyList[iy].mListSize = 0 ;
				mTextReader >> GAP ;
				mTextReader >> KeywordText::from (mLastString) ;
				mElementList[ix].mPropertyList[iy].mName = move (mLastString) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("property"))
					discard ;
				assume (ix != NONE) ;
				iy = mElementList[ix].mPropertyList.insert () ;
				const auto r5x = mPropertyType.map (mLastType) ;
				assume (r5x != NONE) ;
				mElementList[ix].mPropertyList[iy].mType = r5x ;
				mElementList[ix].mPropertyList[iy].mPlyBegin = mElementList[ix].mLineStep ;
				const auto r6x = ply_parser_data_type_size (r5x) ;
				mElementList[ix].mLineStep += r6x ;
				mElementList[ix].mPropertyList[iy].mPlyEnd = mElementList[ix].mLineStep ;
				mTextReader >> GAP ;
				mElementList[ix].mPropertyList[iy].mListType = PlyParserDataType::Null ;
				mElementList[ix].mPropertyList[iy].mListSize = 0 ;
				mTextReader >> GAP ;
				mTextReader >> KeywordText::from (mLastString) ;
				mElementList[ix].mPropertyList[iy].mName = move (mLastString) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("comment"))
					discard ;
				mTextReader >> EndlineText::from (mLastString) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				assume (FALSE) ;
			}
		}
		if ifdo (TRUE) {
			auto rax = STRU32 () ;
			const auto r7x = mTextReader.backup () ;
			mTextReader.read (rax) ;
			if (rax == STRU32 ('\r'))
				discard ;
			mTextReader.reset (r7x) ;
		}
		if ifdo (TRUE) {
			auto rax = STRU32 () ;
			mTextReader.read (rax) ;
			assume (rax == STRU32 ('\n')) ;
			mBodyBackup = mTextReader.backup () ;
		}
		for (auto &&i : mElementList.range ()) {
			auto &&rax = mElementList[i] ;
			rax.mPropertyList.remap () ;
			rax.mPropertySet = Set<String<STRU8>> (rax.mPropertyList.length ()) ;
			for (auto &&j : rax.mPropertyList.range ())
				rax.mPropertySet.add (rax.mPropertyList[j].mName ,j) ;
		}
		if ifdo (TRUE) {
			mElementList.remap () ;
			mElementSet = Set<String<STRU8>> (mElementList.length ()) ;
			for (auto &&j : mElementList.range ())
				mElementSet.add (mElementList[j].mName ,j) ;
		}
		for (auto &&i : mElementList.range ()) {
			auto &&rax = mElementList[i] ;
			const auto r8x = rax.mLineSize * rax.mLineStep ;
			rax.mPlyBuffer = RefBuffer<BYTE> (r8x) ;
			rax.mPlyIndex = address (rax.mPlyBuffer[0]) ;
		}
	}

	LENGTH ply_parser_data_type_size (CREF<Just<PlyParserDataType>> type_) const {
		if (type_ == PlyParserDataType::Bool)
			return 1 ;
		if (type_ == PlyParserDataType::Val32)
			return 4 ;
		if (type_ == PlyParserDataType::Val64)
			return 8 ;
		if (type_ == PlyParserDataType::Flt32)
			return 4 ;
		if (type_ == PlyParserDataType::Flt64)
			return 8 ;
		if (type_ == PlyParserDataType::Byte)
			return 1 ;
		if (type_ == PlyParserDataType::Word)
			return 2 ;
		if (type_ == PlyParserDataType::Char)
			return 4 ;
		if (type_ == PlyParserDataType::Quad)
			return 8 ;
		return 0 ;
	}

	void read_body_text () {
		mTextReader = TextReader (mStream.share ()) ;
		mTextReader.reset (mBodyBackup) ;
		mTextReader >> GAP ;
		for (auto &&i : iter (0 ,mElementList.length ())) {
			for (auto &&j : iter (0 ,mElementList[i].mLineSize)) {
				for (auto &&k : iter (0 ,mElementList[i].mPropertyList.length ())) {
					read_body_text_item (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					read_body_text_list (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					mElementList[i].mLineLength++ ;
				}
			}
		}
		mTextReader >> GAP ;
		mTextReader >> EOS ;
	}

	void read_body_text_item (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			const auto r2x = mTextReader.poll (TYPE<FLT32>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r2x) ,SIZE_OF<FLT32>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT32>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			const auto r3x = mTextReader.poll (TYPE<FLT64>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r3x) ,SIZE_OF<FLT64>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT64>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Bool)
				discard ;
			const auto r4x = mTextReader.poll (TYPE<BOOL>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r4x) ,SIZE_OF<BOOL>::expr) ;
			element.mPlyIndex += SIZE_OF<BOOL>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			const auto r5x = mTextReader.poll (TYPE<VAL32>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r5x) ,SIZE_OF<VAL32>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL32>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			const auto r6x = mTextReader.poll (TYPE<VAL64>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r6x) ,SIZE_OF<VAL64>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL64>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			const auto r7x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r7x >= 0) ;
			const auto r8x = BYTE (r7x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r8x) ,SIZE_OF<BYTE>::expr) ;
			element.mPlyIndex += SIZE_OF<BYTE>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			const auto r9x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r9x >= 0) ;
			const auto r10x = WORD (r9x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r10x) ,SIZE_OF<WORD>::expr) ;
			element.mPlyIndex += SIZE_OF<WORD>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			const auto r11x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r11x >= 0) ;
			const auto r12x = CHAR (r11x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r12x) ,SIZE_OF<CHAR>::expr) ;
			element.mPlyIndex += SIZE_OF<CHAR>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			const auto r13x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r13x >= 0) ;
			const auto r14x = QUAD (r13x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r14x) ,SIZE_OF<QUAD>::expr) ;
			element.mPlyIndex += SIZE_OF<QUAD>::expr ;
			mTextReader >> GAP ;
		}
	}

	void read_body_text_list (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mListType ;
		if (r1x == PlyParserDataType::Null)
			return ;
		const auto r2x = address (element.mPlyBuffer[property.mPlyBegin]) ;
		const auto r3x = invoke ([&] () {
			if (property.mType == PlyParserDataType::Byte)
				return LENGTH (bitwise[TYPE<BYTE>::expr] (Pointer::make (r2x))) ;
			if (property.mType == PlyParserDataType::Word)
				return LENGTH (bitwise[TYPE<WORD>::expr] (Pointer::make (r2x))) ;
			if (property.mType == PlyParserDataType::Char)
				return LENGTH (bitwise[TYPE<CHAR>::expr] (Pointer::make (r2x))) ;
			assume (FALSE) ;
			return ZERO ;
		}) ;
		if ifdo (TRUE) {
			if (property.mListSize != 0)
				discard ;
			property.mListSize = r3x ;
			const auto r4x = ply_parser_data_type_size (property.mListType) ;
			const auto r5x = property.mPlyEnd ;
			for (auto &&i : iter (0 ,element.mPropertyList.length ())) {
				if ifdo (TRUE) {
					if (element.mPropertyList[i].mPlyBegin < r5x)
						discard ;
					element.mPropertyList[i].mPlyBegin += r4x ;
				}
				if ifdo (TRUE) {
					if (element.mPropertyList[i].mPlyEnd < r5x)
						discard ;
					element.mPropertyList[i].mPlyEnd += r4x ;
				}
			}
			element.mLineStep += r4x ;
			const auto r6x = element.mLineSize * element.mLineStep ;
			element.mPlyBuffer.resize (r6x) ;
		}
		assume (property.mListSize == r3x) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			mTextReader >> GAP ;
			for (auto &&i : iter (0 ,r3x)) {
				noop (i) ;
				const auto r7x = mTextReader.poll (TYPE<VAL32>::expr) ;
				inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r7x) ,SIZE_OF<VAL32>::expr) ;
				element.mPlyIndex += SIZE_OF<VAL32>::expr ;
				mTextReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			mTextReader >> GAP ;
			for (auto &&i : iter (0 ,r3x)) {
				noop (i) ;
				const auto r8x = mTextReader.poll (TYPE<VAL64>::expr) ;
				inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r8x) ,SIZE_OF<VAL64>::expr) ;
				element.mPlyIndex += SIZE_OF<VAL64>::expr ;
				mTextReader >> GAP ;
			}
		}
	}

	void read_body_byte () {
		mByteReader = ByteReader (mStream.share ()) ;
		mByteReader.reset (mBodyBackup) ;
		for (auto &&i : iter (0 ,mElementList.length ())) {
			for (auto &&j : iter (0 ,mElementList[i].mLineSize)) {
				for (auto &&k : iter (0 ,mElementList[i].mPropertyList.length ())) {
					read_body_byte_item (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					read_body_byte_list (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					mElementList[i].mLineLength++ ;
				}
			}
		}
		mByteReader >> EOS ;
	}

	void read_body_byte_item (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			const auto r2x = mByteReader.poll (TYPE<FLT32>::expr) ;
			const auto r3x = bitwise_reverse (r2x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r3x) ,SIZE_OF<FLT32>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT32>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			const auto r4x = mByteReader.poll (TYPE<FLT64>::expr) ;
			const auto r5x = bitwise_reverse (r4x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r5x) ,SIZE_OF<FLT64>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT64>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Bool)
				discard ;
			const auto r6x = mByteReader.poll (TYPE<BOOL>::expr) ;
			const auto r7x = bitwise_reverse (r6x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r7x) ,SIZE_OF<BOOL>::expr) ;
			element.mPlyIndex += SIZE_OF<BOOL>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			const auto r8x = mByteReader.poll (TYPE<VAL32>::expr) ;
			const auto r9x = bitwise_reverse (r8x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r9x) ,SIZE_OF<VAL32>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL32>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			const auto r10x = mByteReader.poll (TYPE<VAL64>::expr) ;
			const auto r11x = bitwise_reverse (r10x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r11x) ,SIZE_OF<VAL64>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL64>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			const auto r12x = mByteReader.poll (TYPE<BYTE>::expr) ;
			const auto r13x = bitwise_reverse (r12x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r13x) ,SIZE_OF<BYTE>::expr) ;
			element.mPlyIndex += SIZE_OF<BYTE>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			const auto r14x = mByteReader.poll (TYPE<WORD>::expr) ;
			const auto r15x = bitwise_reverse (r14x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r15x) ,SIZE_OF<WORD>::expr) ;
			element.mPlyIndex += SIZE_OF<WORD>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			const auto r16x = mByteReader.poll (TYPE<CHAR>::expr) ;
			const auto r17x = bitwise_reverse (r16x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r17x) ,SIZE_OF<CHAR>::expr) ;
			element.mPlyIndex += SIZE_OF<CHAR>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			const auto r18x = mByteReader.poll (TYPE<QUAD>::expr) ;
			const auto r19x = bitwise_reverse (r18x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r19x) ,SIZE_OF<QUAD>::expr) ;
			element.mPlyIndex += SIZE_OF<QUAD>::expr ;
		}
	}

	void read_body_byte_list (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mListType ;
		if (r1x == PlyParserDataType::Null)
			return ;
		const auto r2x = address (element.mPlyBuffer[property.mPlyBegin]) ;
		const auto r3x = invoke ([&] () {
			if (property.mType == PlyParserDataType::Byte)
				return LENGTH (bitwise[TYPE<BYTE>::expr] (Pointer::make (r2x))) ;
			if (property.mType == PlyParserDataType::Word)
				return LENGTH (bitwise[TYPE<WORD>::expr] (Pointer::make (r2x))) ;
			if (property.mType == PlyParserDataType::Char)
				return LENGTH (bitwise[TYPE<CHAR>::expr] (Pointer::make (r2x))) ;
			assume (FALSE) ;
			return ZERO ;
		}) ;
		if ifdo (TRUE) {
			if (property.mListSize != 0)
				discard ;
			property.mListSize = r3x ;
			const auto r4x = ply_parser_data_type_size (property.mListType) ;
			const auto r5x = property.mPlyEnd ;
			for (auto &&i : iter (0 ,element.mPropertyList.length ())) {
				if ifdo (TRUE) {
					if (element.mPropertyList[i].mPlyBegin < r5x)
						discard ;
					element.mPropertyList[i].mPlyBegin += r4x ;
				}
				if ifdo (TRUE) {
					if (element.mPropertyList[i].mPlyEnd < r5x)
						discard ;
					element.mPropertyList[i].mPlyEnd += r4x ;
				}
			}
			element.mLineStep += r4x ;
			const auto r6x = element.mLineSize * element.mLineStep ;
			element.mPlyBuffer.resize (r6x) ;
		}
		assume (property.mListSize == r3x) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			for (auto &&i : iter (0 ,r3x)) {
				noop (i) ;
				const auto r7x = mByteReader.poll (TYPE<VAL32>::expr) ;
				inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r7x) ,SIZE_OF<VAL32>::expr) ;
				element.mPlyIndex += SIZE_OF<VAL32>::expr ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			for (auto &&i : iter (0 ,r3x)) {
				noop (i) ;
				const auto r8x = mByteReader.poll (TYPE<VAL64>::expr) ;
				inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r8x) ,SIZE_OF<VAL64>::expr) ;
				element.mPlyIndex += SIZE_OF<VAL64>::expr ;
			}
		}
	}

	template <class ARG1>
	BYTE_BASE<ARG1> bitwise_reverse (CREF<ARG1> a) const {
		if (!mReverseFlag)
			return bitwise[TYPE<BYTE_BASE<ARG1>>::expr] (a) ;
		return ByteProc::bit_reverse (bitwise[TYPE<BYTE_BASE<ARG1>>::expr] (a)) ;
	}
} ;

class PlyParserImplHolder final implement Fat<PlyParserHolder ,PlyParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakePlyParser (Ref<RefBuffer<BYTE>>::reference (stream)) ;
		rax.generate () ;
		fake.mThis = Ref<PlyParserImplLayout>::make (rax.poll ()) ;
		fake.mGuide.mElementIndex = NONE ;
	}

	LENGTH element_size (CREF<Slice> element) const override {
		INDEX ix = fake.mThis->mElementSet.map (element) ;
		if (ix == NONE)
			return 0 ;
		return fake.mThis->mElementList[ix].mLineSize ;
	}

	LENGTH property_size (CREF<Slice> element ,CREF<Slice> property) const override {
		INDEX ix = fake.mThis->mElementSet.map (element) ;
		if (ix == NONE)
			return 0 ;
		INDEX jx = fake.mThis->mElementList[ix].mPropertySet.map (property) ;
		if (jx == NONE)
			return 0 ;
		return fake.mThis->mElementList[ix].mPropertyList[jx].mListSize ;
	}

	void guide_new (CREF<Slice> element) override {
		INDEX ix = fake.mThis->mElementSet.map (element) ;
		assume (ix != NONE) ;
		fake.mGuide.mElementIndex = ix ;
		fake.mGuide.mProperty.clear () ;
		fake.mGuide.mPropertyIndex = 0 ;
		fake.mGuide.mLineIndex = 0 ;
		fake.mGuide.mPlyIndex = NONE ;
	}

	void guide_put (CREF<Slice> property) override {
		INDEX ix = fake.mGuide.mElementIndex ;
		assume (ix != NONE) ;
		INDEX jx = fake.mThis->mElementList[ix].mPropertySet.map (property) ;
		assume (jx != NONE) ;
		assert (fake.mGuide.mPlyIndex == NONE) ;
		fake.mGuide.mProperty.add (jx) ;
	}

	void guide_jmp () {
		assert (fake.mGuide.mElementIndex != NONE) ;
		INDEX ix = fake.mGuide.mElementIndex ;
		INDEX jx = NONE ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mGuide.mPlyIndex != NONE)
				discard ;
			fake.mGuide.mLineIndex = 0 ;
			fake.mGuide.mPropertyIndex = 0 ;
			assume (fake.mGuide.mPropertyIndex < fake.mGuide.mProperty.length ()) ;
			assume (fake.mGuide.mLineIndex < fake.mThis->mElementList[ix].mLineSize) ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			const auto r1x = fake.mGuide.mLineIndex * fake.mThis->mElementList[ix].mLineStep ;
			const auto r2x = r1x + fake.mThis->mElementList[ix].mPropertyList[jx].mPlyBegin ;
			fake.mGuide.mPlyIndex = address (fake.mThis->mElementList[ix].mPlyBuffer[r2x]) ;
		}
		if ifdo (act) {
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			if (fake.mGuide.mPlyIndex >= fake.mThis->mElementList[ix].mPropertyList[jx].mPlyEnd)
				discard ;
		}
		if ifdo (act) {
			fake.mGuide.mPropertyIndex++ ;
			if (fake.mGuide.mPropertyIndex >= fake.mGuide.mProperty.length ())
				discard ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			const auto r3x = fake.mGuide.mLineIndex * fake.mThis->mElementList[ix].mLineStep ;
			const auto r4x = r3x + fake.mThis->mElementList[ix].mPropertyList[jx].mPlyBegin ;
			fake.mGuide.mPlyIndex = address (fake.mThis->mElementList[ix].mPlyBuffer[r4x]) ;
		}
		if ifdo (act) {
			fake.mGuide.mLineIndex++ ;
			fake.mGuide.mPropertyIndex = 0 ;
			if (fake.mGuide.mLineIndex >= fake.mThis->mElementList[ix].mLineSize)
				discard ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			const auto r5x = fake.mGuide.mLineIndex * fake.mThis->mElementList[ix].mLineStep ;
			const auto r6x = r5x + fake.mThis->mElementList[ix].mPropertyList[jx].mPlyBegin ;
			fake.mGuide.mPlyIndex = address (fake.mThis->mElementList[ix].mPlyBuffer[r6x]) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
		fake.mGuide.mPlyType = fake.mThis->mElementList[ix].mPropertyList[jx].mType ;
	}

	void read (VREF<BOOL> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Bool) ;
		item = bitwise[TYPE<BOOL>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<BOOL>::expr ;
	}

	void read (VREF<VAL32> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Val32) ;
		item = bitwise[TYPE<VAL32>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<VAL32>::expr ;
	}

	void read (VREF<VAL64> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Val64) ;
		item = bitwise[TYPE<VAL64>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<VAL64>::expr ;
	}

	void read (VREF<FLT32> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Flt32) ;
		item = bitwise[TYPE<FLT32>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<FLT32>::expr ;
	}

	void read (VREF<FLT64> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Flt64) ;
		item = bitwise[TYPE<FLT64>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<FLT64>::expr ;
	}

	void read (VREF<BYTE> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Byte) ;
		item = bitwise[TYPE<BYTE>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<BYTE>::expr ;
	}

	void read (VREF<WORD> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Word) ;
		item = bitwise[TYPE<WORD>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<WORD>::expr ;
	}

	void read (VREF<CHAR> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Char) ;
		item = bitwise[TYPE<CHAR>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<CHAR>::expr ;
	}

	void read (VREF<QUAD> item) override {
		guide_jmp () ;
		item = bitwise[TYPE<QUAD>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<QUAD>::expr ;
	}
} ;

exports VFat<PlyParserHolder> PlyParserHolder::hold (VREF<PlyParserLayout> that) {
	return VFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}

exports CFat<PlyParserHolder> PlyParserHolder::hold (CREF<PlyParserLayout> that) {
	return CFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}
} ;