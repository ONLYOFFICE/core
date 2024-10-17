#include "docbuilder_CDocBuilderContextScope.h"

#include "docbuilder.h"

using namespace NSDoctRenderer;

jlong Java_docbuilder_CDocBuilderContextScope_c_1Create(JNIEnv* env, jclass cls)
{
	return reinterpret_cast<jlong>(new CDocBuilderContextScope());
}

jlong Java_docbuilder_CDocBuilderContextScope_c_1Copy(JNIEnv* env, jclass cls, jlong other)
{
	CDocBuilderContextScope* pOther = reinterpret_cast<CDocBuilderContextScope*>(other);
	return reinterpret_cast<jlong>(new CDocBuilderContextScope(*pOther));
}

void Java_docbuilder_CDocBuilderContextScope_c_1Destroy(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderContextScope* pSelf = reinterpret_cast<CDocBuilderContextScope*>(self);
	delete pSelf;
}

void Java_docbuilder_CDocBuilderContextScope_c_1Close(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderContextScope* pSelf = reinterpret_cast<CDocBuilderContextScope*>(self);
	pSelf->Close();
}
