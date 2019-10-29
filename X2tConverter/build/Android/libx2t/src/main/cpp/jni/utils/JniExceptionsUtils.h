#ifndef JNI_EXCEPTIONS_UTILS_H
#define JNI_EXCEPTIONS_UTILS_H

#include <iostream>
#include <jni.h>
#include <JniLogUtils.h>

using namespace std;

#define THROW(ENV, CLASS, MESSAGE)                                    \
    jclass jc = ENV->FindClass(CLASS);                                \
    if (jc != NULL) {                                                 \
        ENV->ThrowNew(jc, MESSAGE);                                   \
    }

#define TRY_JNI(ENV, STUFF)                                           \
    try {                                                             \
        STUFF;                                                        \
    } catch (const std::bad_alloc& e){                                \
        THROW(ENV, "java/lang/OutOfMemoryError", e.what())            \
    } catch (const std::ios_base::failure& e) {                       \
        THROW(ENV, "java/io/IOException", e.what())                   \
    } catch (const std::exception& e) {                               \
        THROW(ENV, "java/lang/Exception", e.what())                   \
    } catch (...) {                                                   \
        THROW(ENV, "java/lang/Error", "Unidentified exception")       \
    }

#endif
