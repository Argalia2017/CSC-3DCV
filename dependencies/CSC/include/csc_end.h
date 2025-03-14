
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

#ifdef csc_push_macro
#pragma pop_macro ("TRUE")
#pragma pop_macro ("FALSE")
#pragma pop_macro ("NULL")
#pragma pop_macro ("ZERO")
#pragma pop_macro ("IDEN")
#pragma pop_macro ("NONE")
#pragma pop_macro ("USED")
#undef csc_push_macro
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (push)
#define NOISY_CODE_ANALYSIS_WARNINGS 4661 4819 6201 6255 6269 6294 26439 26450 26495 26813 26820
#pragma warning (disable : NOISY_CODE_ANALYSIS_WARNINGS)
#endif

#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wdeprecated"
#endif

#ifdef __CSC_COMPILER_CLANG__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wdeprecated"
#endif