/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class docbuilder_CDocBuilderContext */

#ifndef _Included_docbuilder_CDocBuilderContext
#define _Included_docbuilder_CDocBuilderContext
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_Create
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1Create
  (JNIEnv *, jclass);

/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_Copy
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1Copy
  (JNIEnv *, jclass, jlong);

/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_Destroy
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_docbuilder_CDocBuilderContext_c_1Destroy
  (JNIEnv *, jclass, jlong);

/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_CreateUndefined
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateUndefined
  (JNIEnv *, jclass, jlong);

/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_CreateNull
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateNull
  (JNIEnv *, jclass, jlong);

/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_CreateObject
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateObject
  (JNIEnv *, jclass, jlong);

/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_CreateArray
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateArray
  (JNIEnv *, jclass, jlong, jint);

/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_GetGlobal
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1GetGlobal
  (JNIEnv *, jclass, jlong);

/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_CreateScope
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateScope
  (JNIEnv *, jclass, jlong);

/*
 * Class:     docbuilder_CDocBuilderContext
 * Method:    c_IsError
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_docbuilder_CDocBuilderContext_c_1IsError
  (JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif
