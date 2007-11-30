#ifndef _UMACRO_H_
#define _UMACRO_H_

#define MACRO_FIRST(a, ...) a
#define MACRO_REST(a, ...) __VA_ARGS__
#define MACRO_CALL(a, b) a b
#define MACRO_OK(exp) exp
#define MACRO_TEST(exp) MACRO_OK
#define MACRO_TESTEND
#define MACRO_END(...)
#define MACRO_ARG(...) MACRO_OK

#define MACRO_LOOP(mac, i, ...) MACRO_LOOP_##i(mac, __VA_ARGS__)
#define MACRO_LOOP_20(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_19(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_19(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_18(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_18(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_17(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_17(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_16(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_16(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_15(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_15(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_14(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_14(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_13(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_13(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_12(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_12(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_11(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_11(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_10(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_10(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_9(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_9(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_8(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_8(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_7(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_7(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_6(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_6(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_5(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_5(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_4(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_4(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_3(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_3(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_2(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_2(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__)) MACRO_LOOP_1(mac, MACRO_REST(__VA_ARGS__))
#define MACRO_LOOP_1(mac, ...) MACRO_CALL(mac, MACRO_FIRST(__VA_ARGS__))

#endif //_UMACRO_H_
