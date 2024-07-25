#include "docbuilder_utils_Utils.h"

#include <stdlib.h>
#include <string>

void Java_docbuilder_utils_Utils_addToPath(JNIEnv* env, jclass cls, jstring directory)
{
#ifdef _WIN32
	const char* strUtfDirectory = env->GetStringUTFChars(directory, nullptr);
	std::string sPath(getenv("PATH"));
	sPath += ';';
	sPath += strUtfDirectory;
	_putenv_s("PATH", sPath.c_str());
	env->ReleaseStringUTFChars(directory, strUtfDirectory);
#endif
}
