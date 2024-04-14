#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG
#	define __LOG(file, line, ...) ((void)0)
#else
void __LOG(const char *file, int line, const char* fmt, ...);
#endif

#ifndef __cplusplus
#	ifdef _STDBOOL_H
#		undef bool
#		undef true
#		undef false
#	endif
typedef enum {
	false = 0,
	true = 1
} bool;
#define null NULL
#else
#define null nullptr
#endif

#ifndef EPSILON
#define EPSILON 0.00001
#endif

#define logp(...) __LOG(__FILE__, __LINE__, __VA_ARGS__)
#define loge(...) __LOG(__FILE__, -__LINE__, __VA_ARGS__)
#define __assert(result, ...) ((result) ? (void) 0 : loge(__VA_ARGS__))

#define assert(__result, __expression, __msg, ...) __assert(__result, "Failed to assert '%s'. " __msg,  #__expression, __VA_ARGS__)

// pointer assertion
#define assert_null(__expression) do{const void* __pointer=__expression;__assert(__pointer==null, "Failed to assert that '%s' (=%p) is null", #__expression, __pointer);}while(false)
#define assert_not_null(__expression) do{const void* __pointer=__expression;__assert(__pointer!=null, "Failed to assert that '%s' is not null", #__expression);}while(false)

// boolean assertion
#define assert_true(__expression) do{bool __result=__expression;__assert(__result, "Failed to assert that '%s' is true", #__expression);}while(false)
#define assert_false(__expression) do{bool __result=!(__expression);__assert(__result, "Failed to assert that '%s' is false", #__expression);}while(false)

// string assertion
#define __assert_str(__a__, _s, __b__, _f) do{const char *_v1=__a__,*_v2=__b__;__assert(_f(_v1, _v2) _s 0, "Failed to assert that %s (=%s) %s %s (=$s) using %s", #__a__, _v1, #_s, #__b__, _v2, #_f);}while(false)
#define assert_str(__a__, _s, __b__) __assert_str(__a__, _s, __b__, strcmp)
#define assert_stri(__a__, _s, __b__) __assert_str(__a__, _s, __b__, strcasecmp)
#define assert_streq(__a__, __b__) __assert_str(__a__, ==, __b__, strcmp)
#define assert_strne(__a__, __b__) __assert_str(__a__, !=, __b__, strcmp)
#define assert_strieq(__a__, __b__) __assert_str(__a__, ==, __b__, strcasecmp)
#define assert_strine(__a__, __b__) __assert_str(__a__, !=, __b__, strcasecmp)

// integer assertion
#define assert_icmp(__a__, __cmp__, __x__) do{int _v1 = __a__, _v2 = __x__; assert(_v1 __cmp__ _v2, __a__ __cmp__ __x__, "%s=%d, %s=%d", #__a__, _v1, #__x__, _v2);}while(false)
#define assert_ieq(__actual, __expected) assert_icmp(__actual, ==, __expected)
#define assert_ine(__actual, __expected) assert_icmp(__actual, !=, __expected)
#define assert_ilt(__actual, __expected) assert_icmp(__actual, <, __expected)
#define assert_igt(__actual, __expected) assert_icmp(__actual, >, __expected)
#define assert_ile(__actual, __expected) assert_icmp(__actual, <=, __expected)
#define assert_ige(__actual, __expected) assert_icmp(__actual, >=, __expected)
#define assert_icmp2(__a__, __cmp1__, __b__, __cmp2__, __c__) do{int v1=__a__,v2=__b__,v3=__c__;assert(v1 __cmp1__ v2 && v1 __cmp2__ v3, __a__ __cmp1__ __b__ && __a__ __cmp2__ __c__, "%s (=%d), %s (=%d), %s (=%d)", #__a__, v1, #__b__, v2, #__c__, v3);}while(false)
#define assert_ibetweenii(__a__, __b__, __c__) assert_icmp2(__a__, >=, __b__, <=, __c__)
#define assert_ibetweenei(__a__, __b__, __c__) assert_icmp2(__a__, >, __b__, <=, __c__)
#define assert_ibetweenie(__a__, __b__, __c__) assert_icmp2(__a__, >=, __b__, <, __c__)
#define assert_ibetweenee(__a__, __b__, __c__) assert_icmp2(__a__, >, __b__, <, __c__)

// float assertion
#define assert_fclose(__actual, __expected) do{float _v1=__actual, _v2=__expected;__assert((_v1 - _v2)<EPSILON || (_v1 - _v2)>EPSILON, "Failed to assert that %s (=%f) is close to %s (%f)", #__actual, _v1, #__expected, _v2)}while(false)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LOG_H */