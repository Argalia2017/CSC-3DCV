
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (pop)
#endif

#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __CSC_COMPILER_CLANG__
#pragma clang diagnostic pop
#endif

#undef implicit
#undef exports
#undef imports
#undef forceinline
#undef leftvalue
#undef rightvalue
#undef thiz
#undef deref
#undef self
#undef expr
#undef trait
#undef implement
#undef require
#undef anonymous
#undef slice
#undef assert
#undef assume
#undef watch
#undef ifdo
#undef discard
#undef typeof
#undef nullof

#define implicit
#define exports __macro_exports
#define imports __macro_imports
#define forceinline __macro_forceinline
#define leftvalue &
#define rightvalue &&
#define thiz (*this)
#define deref deref_m ()
#define self self_m ()
#define expr expr_m ()
#define trait struct
#define implement :public
#define require __macro_require
#define anonymous __macro_anonymous
#define slice __macro_slice
#define assert __macro_assert
#define assume __macro_assume
#define watch __macro_watch
#define ifdo __macro_ifdo
#define discard break
#define typeof __macro_typeof
#define nullof __macro_nullof

#ifdef NULL
#define csc_push_macro
#pragma push_macro ("TRUE")
#pragma push_macro ("FALSE")
#pragma push_macro ("NULL")
#pragma push_macro ("ZERO")
#pragma push_macro ("IDEN")
#pragma push_macro ("NONE")
#pragma push_macro ("USED")
#undef TRUE
#undef FALSE
#undef NULL
#undef ZERO
#undef IDEN
#undef NONE
#undef USED
#endif