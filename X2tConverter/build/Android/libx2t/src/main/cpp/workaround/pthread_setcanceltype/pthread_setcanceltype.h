#ifndef DOCUMENTS_ANDROID_PTHREAD_SETCANCELTYPE_H
#define DOCUMENTS_ANDROID_PTHREAD_SETCANCELTYPE_H

#include <pthread.h>
#include <errno.h>

#define PTHREAD_CANCEL_ASYNCHRONOUS 0x00000020
#define PTHREAD_CANCEL_DEFERRED     0x00000000

#define PTHREAD_ATTR_FLAG_CANCEL_HANDLER 0x00000040
#define PTHREAD_ATTR_FLAG_CANCEL_PENDING 0x00000008
#define PTHREAD_ATTR_FLAG_CANCEL_ENABLE 0x00000010
#define PTHREAD_ATTR_FLAG_CANCEL_ASYNCRONOUS 0x00000020

#define __BIONIC_DLERROR_BUFFER_SIZE 508


#ifdef __cplusplus
extern "C" {
#endif

    struct pthread_internal_t {
        struct pthread_internal_t* next;
        struct pthread_internal_t* prev;

        pid_t tid;

        void** tls;

        volatile pthread_attr_t attr;

        __pthread_cleanup_t* cleanup_stack;

        void* (*start_routine)(void*);
        void* start_routine_arg;
        void* return_value;

        void* alternate_signal_stack;

        char dlerror_buffer[__BIONIC_DLERROR_BUFFER_SIZE];

        pthread_mutex_t cancel_lock;
    };

    int __pthread_do_cancel (struct pthread_internal_t* p);

    void pthread_init(void);

    int pthread_setcanceltype(int type, int *oldtype);

#ifdef __cplusplus
}
#endif

#endif
