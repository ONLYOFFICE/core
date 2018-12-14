#include <jni.h>
#include <string>
#include <cextracttools.h>

#ifndef DOCUMENTSNEW_JNIUTILS_H
#define DOCUMENTSNEW_JNIUTILS_H

using namespace std;
using namespace NExtractTools;

namespace JniUtils {

    unsigned int getLength(const char* str) {
        unsigned int lenght = 0;
        while (str[lenght++] != '\0');
        return lenght - 1;
    }

    wstring jstringToWString(JNIEnv* env, jstring jstr) {
        const char * charPath = env->GetStringUTFChars(jstr, JNI_FALSE);
        const int size = env->GetStringLength(jstr);
        const wstring wstr(charPath, charPath + size);
        return wstr;
    }

    string jstringToString(JNIEnv* env, jstring jstr) {
        const char * charPath = env->GetStringUTFChars(jstr, JNI_FALSE);
        const int size = env->GetStringLength(jstr);
        const string str(charPath, charPath + size);
        return str;
    }

    wstring charToWString(const char* str) {
        if (str != NULL) {
            wstring temp(str[0], str[getLength(str)]);
            return temp;
        }
        return NULL;
    }

    void initInputParams(JNIEnv *env, jobject jInputParams, InputParams &params) {
        if (env->IsSameObject(jInputParams, NULL) == JNI_FALSE) {
            jclass inputParams = env->GetObjectClass(jInputParams);
            jfieldID passwordField = env->GetFieldID(inputParams, "password", "Ljava/lang/String;");
            jfieldID delimiterCharField = env->GetFieldID(inputParams, "delimiterChar", "Ljava/lang/String;");
            jfieldID delimiterCodeField = env->GetFieldID(inputParams, "delimiterCode", "I");
            jfieldID encodingField = env->GetFieldID(inputParams, "encoding", "I");
            jfieldID formatFromField = env->GetFieldID(inputParams, "formatFrom", "I");
            jfieldID formatToField = env->GetFieldID(inputParams, "formatTo", "I");
            jfieldID isNoBase64Field = env->GetFieldID(inputParams, "isNoBase64", "I");

            if (passwordField != NULL) {
                jstring jPassword = (jstring) env->GetObjectField(jInputParams, passwordField);
                if (jPassword != NULL) {
                    params.m_sPassword = new wstring(jstringToWString(env, jPassword));
                }
            }

            if (delimiterCharField != NULL) {
                jstring jDelimiter = (jstring) env->GetObjectField(jInputParams, delimiterCharField);
                if (jDelimiter != NULL) {
                    params.m_sCsvDelimiterChar = new wstring(jstringToWString(env, jDelimiter));
                }
            }

            if (delimiterCodeField != NULL) {
                jint jDelimiter = env->GetIntField(jInputParams, delimiterCodeField);
                if (jDelimiter != NULL) {
                    params.m_nCsvDelimiter = new int((int) jDelimiter);
                }
            }

            if (encodingField != NULL) {
                jint jEncoding = env->GetIntField(jInputParams, encodingField);
                if (jEncoding != NULL) {
                    params.m_nCsvTxtEncoding = new int((int) jEncoding);
                }
            }

            if (formatFromField != NULL) {
                jint jFormat = env->GetIntField(jInputParams, formatFromField);
                if (jFormat != NULL) {
                    params.m_nFormatFrom = new int((int) jFormat);
                }
            }

            if (formatToField != NULL) {
                jint jFormat = env->GetIntField(jInputParams, formatToField);
                if (jFormat != NULL) {
                    params.m_nFormatTo = new int((int) jFormat);
                }
            }

            if (isNoBase64Field != NULL) {
                jint jIsNoBase64 = env->GetIntField(jInputParams, isNoBase64Field);
                if (jIsNoBase64 != NULL) {
                    params.m_bIsNoBase64 = new bool(jIsNoBase64 != 0);
                }
            }
        }
    }
}


#endif //DOCUMENTSNEW_JNIUTILS_H
