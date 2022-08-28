#ifndef __J_ASSERT_H__
#define __J_ASSERT_H__

#define J_CALL_AND_ASSIGNMENT_CHECK(func, assign, err, ret, ...) \
    do { \
    if (((assign) = (func)(__VA_ARGS__)) == (err)) \
	return (ret); \
    } while (0)

#define J_IF_GOTO_THEN_ELS(chk, err, ret, then, els)	\
    do { \
    if ((chk) == (err)) { \
    goto ret; \
    } else { \
    (then) = (els); \
    } \
    } while (0)

#endif
