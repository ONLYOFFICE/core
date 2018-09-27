#ifndef DOCUMENTS_ANDROID_PTHREAD_SETCANCELTYPE_H
#define DOCUMENTS_ANDROID_PTHREAD_SETCANCELTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#define PTHREAD_CANCEL_ASYNCHRONOUS 0

int pthread_setcanceltype(int type, int *oldtype);

#ifdef __cplusplus
}
#endif

#endif //DOCUMENTS_ANDROID_PTHREAD_SETCANCELTYPE_H
