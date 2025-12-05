#include "docbuilder_CDocBuilder.h"

#include <string>

#include "docbuilder.h"
// for wchar_t <=> char conversion
#include "../../../../common/File.h"

using namespace NSDoctRenderer;

static std::wstring wstringFromJavaString(JNIEnv* env, jstring jstr)
{
	const char* strUtf = env->GetStringUTFChars(jstr, nullptr);
	std::wstring wstr = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)strUtf, (LONG)strlen(strUtf));
	env->ReleaseStringUTFChars(jstr, strUtf);
	return wstr;
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilder_c_1Create(JNIEnv* env, jclass cls)
{
	return reinterpret_cast<jlong>(new CDocBuilder());
}

JNIEXPORT void JNICALL Java_docbuilder_CDocBuilder_c_1Destroy(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	delete pSelf;
}

JNIEXPORT jint JNICALL Java_docbuilder_CDocBuilder_c_1OpenFile(JNIEnv* env, jclass cls, jlong self, jstring path, jstring params)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strPath = wstringFromJavaString(env, path);
	std::wstring strParams = wstringFromJavaString(env, params);
	return (jint)pSelf->OpenFile(strPath.c_str(), strParams.c_str());
}

JNIEXPORT jboolean JNICALL Java_docbuilder_CDocBuilder_c_1CreateFileByType(JNIEnv* env, jclass cls, jlong self, jint type)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	return (jboolean)pSelf->CreateFile((int)type);
}

JNIEXPORT jboolean JNICALL Java_docbuilder_CDocBuilder_c_1CreateFileByExtension(JNIEnv* env, jclass cls, jlong self, jstring extension)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strExtension = wstringFromJavaString(env, extension);
	return (jboolean)pSelf->CreateFile(strExtension.c_str());
}

JNIEXPORT void JNICALL Java_docbuilder_CDocBuilder_c_1SetTmpFolder(JNIEnv* env, jclass cls, jlong self, jstring folder)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strFolder = wstringFromJavaString(env, folder);
	pSelf->SetTmpFolder(strFolder.c_str());
}

JNIEXPORT jint JNICALL Java_docbuilder_CDocBuilder_c_1SaveFileByType(JNIEnv* env, jclass cls, jlong self, jint type, jstring path)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strPath = wstringFromJavaString(env, path);
	return (jint)pSelf->SaveFile((int)type, strPath.c_str());
}

JNIEXPORT jint JNICALL Java_docbuilder_CDocBuilder_c_1SaveFileByTypeWithParams(JNIEnv* env, jclass cls, jlong self, jint type, jstring path, jstring params)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strPath = wstringFromJavaString(env, path);
	std::wstring strParams = wstringFromJavaString(env, params);
	return (jint)pSelf->SaveFile((int)type, strPath.c_str(), strParams.c_str());
}

JNIEXPORT jint JNICALL Java_docbuilder_CDocBuilder_c_1SaveFileByExtension(JNIEnv* env, jclass cls, jlong self, jstring extension, jstring path)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strExtension = wstringFromJavaString(env, extension);
	std::wstring strPath = wstringFromJavaString(env, path);
	return (jint)pSelf->SaveFile(strExtension.c_str(), strPath.c_str());
}

JNIEXPORT jint JNICALL Java_docbuilder_CDocBuilder_c_1SaveFileByExtensionWithParams(JNIEnv* env, jclass cls, jlong self, jstring extension, jstring path, jstring params)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strExtension = wstringFromJavaString(env, extension);
	std::wstring strPath = wstringFromJavaString(env, path);
	std::wstring strParams = wstringFromJavaString(env, params);
	return (jint)pSelf->SaveFile(strExtension.c_str(), strPath.c_str(), strParams.c_str());
}

JNIEXPORT void JNICALL Java_docbuilder_CDocBuilder_c_1CloseFile(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	pSelf->CloseFile();
}

JNIEXPORT jboolean JNICALL Java_docbuilder_CDocBuilder_c_1ExecuteCommand(JNIEnv* env, jclass cls, jlong self, jstring command)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strCommand = wstringFromJavaString(env, command);
	return (jboolean)pSelf->ExecuteCommand(strCommand.c_str());
}

JNIEXPORT jboolean JNICALL Java_docbuilder_CDocBuilder_c_1ExecuteCommandWithRetValue(JNIEnv* env, jclass cls, jlong self, jstring command, jlong retValue)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strCommand = wstringFromJavaString(env, command);
	CDocBuilderValue* pRetValue = reinterpret_cast<CDocBuilderValue*>(retValue);
	return (jboolean)pSelf->ExecuteCommand(strCommand.c_str(), pRetValue);
}

JNIEXPORT jboolean JNICALL Java_docbuilder_CDocBuilder_c_1Run(JNIEnv* env, jclass cls, jlong self, jstring path)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strPath = wstringFromJavaString(env, path);
	return (jboolean)pSelf->Run(strPath.c_str());
}

JNIEXPORT jboolean JNICALL Java_docbuilder_CDocBuilder_c_1RunText(JNIEnv* env, jclass cls, jlong self, jstring commands)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	const char* strUtfCommands = env->GetStringUTFChars(commands, nullptr);
	jboolean result = (jboolean)pSelf->RunTextA(strUtfCommands);
	env->ReleaseStringUTFChars(commands, strUtfCommands);
	return result;
}

JNIEXPORT void JNICALL Java_docbuilder_CDocBuilder_c_1SetProperty(JNIEnv* env, jclass cls, jlong self, jstring param, jstring value)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	const char* strUtfParam = env->GetStringUTFChars(param, nullptr);
	std::wstring strValue = wstringFromJavaString(env, value);
	pSelf->SetProperty(strUtfParam, strValue.c_str());
	env->ReleaseStringUTFChars(param, strUtfParam);
}

JNIEXPORT void JNICALL Java_docbuilder_CDocBuilder_c_1WriteData(JNIEnv* env, jclass cls, jlong self, jstring path, jstring data, jboolean append)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	std::wstring strPath = wstringFromJavaString(env, path);
	std::wstring strData = wstringFromJavaString(env, data);
	pSelf->WriteData(strPath.c_str(), strData.c_str(), (bool)append);
}

JNIEXPORT jboolean JNICALL Java_docbuilder_CDocBuilder_c_1IsSaveWithDoctrendererMode(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	return (jboolean)pSelf->IsSaveWithDoctrendererMode();
}

JNIEXPORT jstring JNICALL Java_docbuilder_CDocBuilder_c_1GetVersion(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	char* strUtfVersion = pSelf->GetVersion();
	jstring jstrVersion = env->NewStringUTF(strUtfVersion);
	delete[] strUtfVersion;
	return jstrVersion;
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilder_c_1GetContext(JNIEnv* env, jclass cls, jlong self)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	return reinterpret_cast<jlong>(new CDocBuilderContext(pSelf->GetContext()));
}

JNIEXPORT jlong JNICALL Java_docbuilder_CDocBuilder_c_1GetContextWithEnterParam(JNIEnv* env, jclass cls, jlong self, jboolean enterContext)
{
	CDocBuilder* pSelf = reinterpret_cast<CDocBuilder*>(self);
	return reinterpret_cast<jlong>(new CDocBuilderContext(pSelf->GetContext((bool)enterContext)));
}

JNIEXPORT void JNICALL Java_docbuilder_CDocBuilder_c_1Initialize(JNIEnv* env, jclass cls)
{
	CDocBuilder::Initialize();
}

JNIEXPORT void JNICALL Java_docbuilder_CDocBuilder_c_1InitializeWithDirectory(JNIEnv* env, jclass cls, jstring directory)
{
	std::wstring strDirectory = wstringFromJavaString(env, directory);
	CDocBuilder::Initialize(strDirectory.c_str());
}

JNIEXPORT void JNICALL Java_docbuilder_CDocBuilder_c_1Dispose(JNIEnv* env, jclass cls)
{
	CDocBuilder::Dispose();
}
