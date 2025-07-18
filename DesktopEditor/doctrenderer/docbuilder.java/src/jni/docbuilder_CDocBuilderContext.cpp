#include "docbuilder_CDocBuilderContext.h"

#include "docbuilder.h"

using namespace NSDoctRenderer;

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1Create(JNIEnv* env, jclass cls)
{
	return reinterpret_cast<jlong>(new CDocBuilderContext());
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1Copy(JNIEnv* env, jclass cls, jlong other)
{
	CDocBuilderContext* pOther = reinterpret_cast<CDocBuilderContext*>(other);
	return reinterpret_cast<jlong>(new CDocBuilderContext(*pOther));
}

JNIEXPORT void JNICALL Java_docbuilder_CDocBuilderContext_c_1Destroy(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderContext* pSelf = reinterpret_cast<CDocBuilderContext*>(self);
	delete pSelf;
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateUndefined(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderContext* pSelf = reinterpret_cast<CDocBuilderContext*>(self);
	return reinterpret_cast<jlong>(new CDocBuilderValue(pSelf->CreateUndefined()));
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateNull(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderContext* pSelf = reinterpret_cast<CDocBuilderContext*>(self);
	return reinterpret_cast<jlong>(new CDocBuilderValue(pSelf->CreateNull()));
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateObject(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderContext* pSelf = reinterpret_cast<CDocBuilderContext*>(self);
	return reinterpret_cast<jlong>(new CDocBuilderValue(pSelf->CreateObject()));
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateArray(JNIEnv* env, jclass cls, jlong self, jint length)
{
	CDocBuilderContext* pSelf = reinterpret_cast<CDocBuilderContext*>(self);
	return reinterpret_cast<jlong>(new CDocBuilderValue(pSelf->CreateArray((int)length)));
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1GetGlobal(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderContext* pSelf = reinterpret_cast<CDocBuilderContext*>(self);
	return reinterpret_cast<jlong>(new CDocBuilderValue(pSelf->GetGlobal()));
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilderContext_c_1CreateScope(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderContext* pSelf = reinterpret_cast<CDocBuilderContext*>(self);
	return reinterpret_cast<jlong>(new CDocBuilderContextScope(pSelf->CreateScope()));
}

JNIEXPORT jboolean JNICALL Java_docbuilder_CDocBuilderContext_c_1IsError(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderContext* pSelf = reinterpret_cast<CDocBuilderContext*>(self);
	return (jboolean)pSelf->IsError();
}
