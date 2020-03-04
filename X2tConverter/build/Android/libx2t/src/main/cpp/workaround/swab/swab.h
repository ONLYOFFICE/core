/*
 *  Included only for API >= 28
 *  <SDK>/ndk-bundle/sysroot/usr/include/unistd.h:296
 *  #if __ANDROID_API__ >= 28
 *      void swab(const void* __src, void* __dst, ssize_t __byte_count) __INTRODUCED_IN(28);
 *  #endif
 * */

#ifndef DOCUMENTS_SWAB_H
#define DOCUMENTS_SWAB_H

#ifdef __cplusplus
extern "C" {
#endif

void swab(void *from, void *to, int n);

#ifdef __cplusplus
}
#endif

#endif //DOCUMENTS_SWAB_H
