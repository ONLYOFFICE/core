#ifndef JNI_X2T_PARAMS_H
#define JNI_X2T_PARAMS_H

#include <jni.h>
#include <JniBaseObjects.h>
#include <ASCConverters.h>
#include <cextracttools.h>

using namespace NExtractTools;

#define GetFieldId(NAME, TYPE) \
    env->GetFieldID(inputParams, #NAME, #TYPE)

#define SetStringInputParam(FIELD, VAL)                                                     \
    if (FIELD != NULL) {                                                                    \
        jstring jStr = (jstring) env->GetObjectField(jInputParams, FIELD);                  \
        if (!env->IsSameObject(jStr, NULL)) {                                               \
            VAL = new std::wstring(JniBaseObjects::jstringToWString(env, jStr));            \
        } else {                                                                            \
            LOGE("getInputParams() - no java object \"%s\"", #FIELD);                       \
        }                                                                                   \
    } else {                                                                                \
        LOGE("getInputParams() - no java field \"%s\"", #FIELD);                            \
    }

#define SetIntInputParam(FIELD, VAL)                                             \
    if (FIELD != NULL) {                                                         \
        jint jInt = (jint) env->GetIntField(jInputParams, FIELD);                \
        VAL = new int((int) jInt);                                               \
    } else {                                                                     \
        LOGE("getInputParams() - no java field \"%s\"", #FIELD);                 \
    }

#define SetBoolInputParam(FIELD, VAL)                                            \
    if (FIELD != NULL) {                                                         \
        jboolean jBool = (jboolean) env->GetBooleanField(jInputParams, FIELD);   \
        VAL = new bool((bool) jBool);                                            \
    } else {                                                                     \
        LOGE("getInputParams() - no java field \"%s\"", #FIELD);                 \
    }


static InputParams getInputParams(JNIEnv *env, jobject jInputParams) {
    InputParams params;

    if (env->IsSameObject(jInputParams, NULL) == JNI_FALSE) {
        jclass inputParams = env->GetObjectClass(jInputParams);

        jfieldID fromField = GetFieldId(from, Ljava/lang/String;);
        jfieldID toField = GetFieldId(to, Ljava/lang/String;);
        jfieldID tempField = GetFieldId(temp, Ljava/lang/String;);
        jfieldID themeField = GetFieldId(theme, Ljava/lang/String;);
        jfieldID fontsDirField = GetFieldId(fontsDir, Ljava/lang/String;);
        jfieldID fontsJsField = GetFieldId(fontsJs, Ljava/lang/String;);
        jfieldID passwordField = GetFieldId(password, Ljava/lang/String;);
        jfieldID delimiterCharField = GetFieldId(delimiterChar, Ljava/lang/String;);

        jfieldID delimiterCodeField = GetFieldId(delimiterCode, I);
        jfieldID encodingField = GetFieldId(encoding, I);
        jfieldID formatFromField = GetFieldId(formatFrom, I);
        jfieldID formatToField = GetFieldId(formatTo, I);

        jfieldID isNoBase64Field = GetFieldId(isNoBase64, Z);
        jfieldID isFromChangeField = GetFieldId(isFromChange, Z);
        jfieldID isPaidField = GetFieldId(isPaid, Z);

        SetStringInputParam(fromField, params.m_sFileFrom);
        SetStringInputParam(toField, params.m_sFileTo);
        SetStringInputParam(tempField, params.m_sTempDir);
        SetStringInputParam(themeField, params.m_sThemeDir);
        SetStringInputParam(fontsDirField, params.m_sFontDir);
        SetStringInputParam(fontsJsField, params.m_sAllFontsPath);
        SetStringInputParam(passwordField, params.m_sPassword);
        SetStringInputParam(delimiterCharField, params.m_sCsvDelimiterChar);

        SetIntInputParam(delimiterCodeField, params.m_nCsvDelimiter);
        SetIntInputParam(encodingField, params.m_nCsvTxtEncoding);
        SetIntInputParam(formatFromField, params.m_nFormatFrom);
        SetIntInputParam(formatToField, params.m_nFormatTo);

        SetBoolInputParam(isNoBase64Field, params.m_bIsNoBase64);
        SetBoolInputParam(isFromChangeField, params.m_bFromChanges);
        SetBoolInputParam(isPaidField, params.m_bPaid);
    }

    return params;
}

#endif
