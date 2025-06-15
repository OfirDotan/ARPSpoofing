#ifndef COMMON_H
#define COMMON_H

#define FAIL_AND_CLEANUP do { status = -1; goto l_cleanup; } while(0)

#define CHECK_SIZE_OR_FAIL(size, expected) \
    do { \
        if ((size) != (expected)) { \
            FAIL_AND_CLEANUP; \
        } \
    } while(0)

#define CHECK_STATUS(status) \
    do { \
        if ((status) < 0) { \
            FAIL_AND_CLEANUP; \
        } \
    } while(0)

#endif