#include "docbuilder_CDocBuilderValue.h"

#include <wchar.h>

#include "docbuilder.h"
// for wchar_t <=> char conversion
#include "../../../../common/File.h"

using namespace NSDoctRenderer;

jlong Java_docbuilder_CDocBuilderValue_c_1Create(JNIEnv* env, jclass cls)
{
	return reinterpret_cast<jlong>(new CDocBuilderValue());
}

jlong Java_docbuilder_CDocBuilderValue_c_1Copy(JNIEnv* env, jclass cls, jlong other)
{
	CDocBuilderValue* pOther = reinterpret_cast<CDocBuilderValue*>(other);
	return reinterpret_cast<jlong>(new CDocBuilderValue(*pOther));
}

void Java_docbuilder_CDocBuilderValue_c_1Destroy(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	delete pSelf;
}

jboolean Java_docbuilder_CDocBuilderValue_c_1IsEmpty(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->IsEmpty();
}

void Java_docbuilder_CDocBuilderValue_c_1Clear(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	pSelf->Clear();
}

jboolean Java_docbuilder_CDocBuilderValue_c_1IsNull(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->IsNull();
}

jboolean Java_docbuilder_CDocBuilderValue_c_1IsUndefined(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->IsUndefined();
}

jboolean Java_docbuilder_CDocBuilderValue_c_1IsInt(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->IsInt();
}

jboolean Java_docbuilder_CDocBuilderValue_c_1IsDouble(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->IsDouble();
}

jboolean Java_docbuilder_CDocBuilderValue_c_1IsString(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->IsString();
}

jboolean Java_docbuilder_CDocBuilderValue_c_1IsFunction(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->IsFunction();
}

jboolean Java_docbuilder_CDocBuilderValue_c_1IsObject(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->IsObject();
}

jboolean Java_docbuilder_CDocBuilderValue_c_1IsArray(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->IsArray();
}

jint Java_docbuilder_CDocBuilderValue_c_1GetLength(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jint)pSelf->GetLength();
}

jboolean Java_docbuilder_CDocBuilderValue_c_1ToBool(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jboolean)pSelf->ToBool();
}

jint Java_docbuilder_CDocBuilderValue_c_1ToInt(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jint)pSelf->ToInt();
}

jdouble Java_docbuilder_CDocBuilderValue_c_1ToDouble(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	return (jdouble)pSelf->ToDouble();
}

jstring Java_docbuilder_CDocBuilderValue_c_1ToString(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CString strValue = pSelf->ToString();
	std::string strUtfData = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(strValue.c_str(), (LONG)wcslen(strValue.c_str()));
	return env->NewStringUTF(strUtfData.c_str());
}

jlong Java_docbuilder_CDocBuilderValue_c_1GetProperty(JNIEnv* env, jclass cls, jlong self, jstring name)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	const char* strUtfName = env->GetStringUTFChars(name, nullptr);
	CDocBuilderValue* pValue = new CDocBuilderValue(pSelf->Get(strUtfName));
	env->ReleaseStringUTFChars(name, strUtfName);
	return reinterpret_cast<jlong>(pValue);
}

jlong Java_docbuilder_CDocBuilderValue_c_1GetByIndex(JNIEnv* env, jclass cls, jlong self, jint index)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CDocBuilderValue* pValue = new CDocBuilderValue(pSelf->Get((int)index));
	return reinterpret_cast<jlong>(pValue);
}

void Java_docbuilder_CDocBuilderValue_c_1SetProperty(JNIEnv* env, jclass cls, jlong self, jstring name, jlong value)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CDocBuilderValue* pValue = reinterpret_cast<CDocBuilderValue*>(value);
	const char* strUtfName = env->GetStringUTFChars(name, nullptr);
	std::wstring strName = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)strUtfName, (LONG)strlen(strUtfName));
	pSelf->Set(strName.c_str(), *pValue);
	env->ReleaseStringUTFChars(name, strUtfName);
}

void Java_docbuilder_CDocBuilderValue_c_1SetByIndex(JNIEnv* env, jclass cls, jlong self, jint index, jlong value)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CDocBuilderValue* pValue = reinterpret_cast<CDocBuilderValue*>(value);
	pSelf->Set((int)index, *pValue);
}

jlong Java_docbuilder_CDocBuilderValue_c_1CreateWithBool(JNIEnv* env, jclass cls, jboolean value)
{
	return reinterpret_cast<jlong>(new CDocBuilderValue((bool)value));
}

jlong Java_docbuilder_CDocBuilderValue_c_1CreateWithInt(JNIEnv* env, jclass cls, jint value)
{
	return reinterpret_cast<jlong>(new CDocBuilderValue((int)value));
}

jlong Java_docbuilder_CDocBuilderValue_c_1CreateWithDouble(JNIEnv* env, jclass cls, jdouble value)
{
	return reinterpret_cast<jlong>(new CDocBuilderValue((double)value));
}

jlong Java_docbuilder_CDocBuilderValue_c_1CreateWithString(JNIEnv* env, jclass cls, jstring str)
{
	const char* strUtf = env->GetStringUTFChars(str, nullptr);
	CDocBuilderValue* pValue = new CDocBuilderValue(strUtf);
	env->ReleaseStringUTFChars(str, strUtf);
	return reinterpret_cast<jlong>(pValue);
}

jlong Java_docbuilder_CDocBuilderValue_c_1CreateUndefined(JNIEnv* env, jclass cls)
{
	return reinterpret_cast<jlong>(new CDocBuilderValue(CDocBuilderValue::CreateUndefined()));
}

jlong Java_docbuilder_CDocBuilderValue_c_1CreateNull(JNIEnv* env, jclass cls)
{
	return reinterpret_cast<jlong>(new CDocBuilderValue(CDocBuilderValue::CreateNull()));
}

jlong Java_docbuilder_CDocBuilderValue_c_1CreateArray(JNIEnv* env, jclass cls, jint length)
{
	return reinterpret_cast<jlong>(new CDocBuilderValue(CDocBuilderValue::CreateArray((int)length)));
}

jlong Java_docbuilder_CDocBuilderValue_c_1Call0(JNIEnv* env, jclass cls, jlong self, jstring name)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	const char* strUtfName = env->GetStringUTFChars(name, nullptr);
	CDocBuilderValue* pReturnValue = new CDocBuilderValue(pSelf->Call(strUtfName));
	env->ReleaseStringUTFChars(name, strUtfName);
	return reinterpret_cast<jlong>(pReturnValue);
}

jlong Java_docbuilder_CDocBuilderValue_c_1Call1(JNIEnv* env, jclass cls, jlong self, jstring name, jlong p1)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CDocBuilderValue* pParam1 = reinterpret_cast<CDocBuilderValue*>(p1);
	const char* strUtfName = env->GetStringUTFChars(name, nullptr);
	CDocBuilderValue* pReturnValue = new CDocBuilderValue(pSelf->Call(strUtfName, *pParam1));
	env->ReleaseStringUTFChars(name, strUtfName);
	return reinterpret_cast<jlong>(pReturnValue);
}

jlong Java_docbuilder_CDocBuilderValue_c_1Call2(JNIEnv* env, jclass cls, jlong self, jstring name, jlong p1, jlong p2)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CDocBuilderValue* pParam1 = reinterpret_cast<CDocBuilderValue*>(p1);
	CDocBuilderValue* pParam2 = reinterpret_cast<CDocBuilderValue*>(p2);
	const char* strUtfName = env->GetStringUTFChars(name, nullptr);
	CDocBuilderValue* pReturnValue = new CDocBuilderValue(pSelf->Call(strUtfName, *pParam1, *pParam2));
	env->ReleaseStringUTFChars(name, strUtfName);
	return reinterpret_cast<jlong>(pReturnValue);
}

jlong Java_docbuilder_CDocBuilderValue_c_1Call3(JNIEnv* env, jclass cls, jlong self, jstring name, jlong p1, jlong p2, jlong p3)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CDocBuilderValue* pParam1 = reinterpret_cast<CDocBuilderValue*>(p1);
	CDocBuilderValue* pParam2 = reinterpret_cast<CDocBuilderValue*>(p2);
	CDocBuilderValue* pParam3 = reinterpret_cast<CDocBuilderValue*>(p3);
	const char* strUtfName = env->GetStringUTFChars(name, nullptr);
	CDocBuilderValue* pReturnValue = new CDocBuilderValue(pSelf->Call(strUtfName, *pParam1, *pParam2, *pParam3));
	env->ReleaseStringUTFChars(name, strUtfName);
	return reinterpret_cast<jlong>(pReturnValue);
}

jlong Java_docbuilder_CDocBuilderValue_c_1Call4(JNIEnv* env, jclass cls, jlong self, jstring name, jlong p1, jlong p2, jlong p3, jlong p4)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CDocBuilderValue* pParam1 = reinterpret_cast<CDocBuilderValue*>(p1);
	CDocBuilderValue* pParam2 = reinterpret_cast<CDocBuilderValue*>(p2);
	CDocBuilderValue* pParam3 = reinterpret_cast<CDocBuilderValue*>(p3);
	CDocBuilderValue* pParam4 = reinterpret_cast<CDocBuilderValue*>(p4);
	const char* strUtfName = env->GetStringUTFChars(name, nullptr);
	CDocBuilderValue* pReturnValue = new CDocBuilderValue(pSelf->Call(strUtfName, *pParam1, *pParam2, *pParam3, *pParam4));
	env->ReleaseStringUTFChars(name, strUtfName);
	return reinterpret_cast<jlong>(pReturnValue);
}

jlong Java_docbuilder_CDocBuilderValue_c_1Call5(JNIEnv* env, jclass cls, jlong self, jstring name, jlong p1, jlong p2, jlong p3, jlong p4, jlong p5)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CDocBuilderValue* pParam1 = reinterpret_cast<CDocBuilderValue*>(p1);
	CDocBuilderValue* pParam2 = reinterpret_cast<CDocBuilderValue*>(p2);
	CDocBuilderValue* pParam3 = reinterpret_cast<CDocBuilderValue*>(p3);
	CDocBuilderValue* pParam4 = reinterpret_cast<CDocBuilderValue*>(p4);
	CDocBuilderValue* pParam5 = reinterpret_cast<CDocBuilderValue*>(p5);
	const char* strUtfName = env->GetStringUTFChars(name, nullptr);
	CDocBuilderValue* pReturnValue = new CDocBuilderValue(pSelf->Call(strUtfName, *pParam1, *pParam2, *pParam3, *pParam4, *pParam5));
	env->ReleaseStringUTFChars(name, strUtfName);
	return reinterpret_cast<jlong>(pReturnValue);
}

jlong Java_docbuilder_CDocBuilderValue_c_1Call6(JNIEnv* env, jclass cls, jlong self, jstring name, jlong p1, jlong p2, jlong p3, jlong p4, jlong p5, jlong p6)
{
	CDocBuilderValue* pSelf = reinterpret_cast<CDocBuilderValue*>(self);
	CDocBuilderValue* pParam1 = reinterpret_cast<CDocBuilderValue*>(p1);
	CDocBuilderValue* pParam2 = reinterpret_cast<CDocBuilderValue*>(p2);
	CDocBuilderValue* pParam3 = reinterpret_cast<CDocBuilderValue*>(p3);
	CDocBuilderValue* pParam4 = reinterpret_cast<CDocBuilderValue*>(p4);
	CDocBuilderValue* pParam5 = reinterpret_cast<CDocBuilderValue*>(p5);
	CDocBuilderValue* pParam6 = reinterpret_cast<CDocBuilderValue*>(p6);
	const char* strUtfName = env->GetStringUTFChars(name, nullptr);
	CDocBuilderValue* pReturnValue = new CDocBuilderValue(pSelf->Call(strUtfName, *pParam1, *pParam2, *pParam3, *pParam4, *pParam5, *pParam6));
	env->ReleaseStringUTFChars(name, strUtfName);
	return reinterpret_cast<jlong>(pReturnValue);
}
