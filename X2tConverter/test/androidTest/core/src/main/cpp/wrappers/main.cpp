#include <jni.h>
#include <ASCConverters.h>
#include <cextracttools.h>
#include "JniUtils.h"


using namespace std;
using namespace NExtractTools;
using namespace JniUtils;

extern "C" {

    /*
     * Test
     * */
    JNIEXPORT jstring JNICALL Java_com_onlyoffice_core_Core_test(JNIEnv* env, jobject thiz, jstring jstr) {
        string newStr = jstringToString(env, jstr);
        newStr.append("\nJNI changes!");
        return env->NewStringUTF(newStr.c_str());
    }

    /*
     * Docs
     * */
    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_docx2doctbin(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                      jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return docx2doct_bin(wFrom, wTo, wTemp, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_docx2doct(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                   jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return docx2doct(wFrom, wTo, wTemp, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_doctbin2docx(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo, jstring temp, 
                                                                      jboolean isFromChanges, jstring theme, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        wstring wTheme = jstringToWString(env, theme);
        return doct_bin2docx(wFrom, wTo, wTemp, isFromChanges, wTheme, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_doct2docx(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo, jstring temp,
                                                                   jboolean isFromChanges, jstring theme, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        wstring wTheme = jstringToWString(env, theme);
        return doct2docx(wFrom, wTo, wTemp, isFromChanges, wTheme, params);
    }


    /*
     * Xls
     * */
    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_xlsx2xlstbin(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                      jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return xlsx2xlst_bin(wFrom, wTo, wTemp, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_xlsx2xlst(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                   jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return xlsx2xlst(wFrom, wTo, wTemp, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_xlstbin2xlsx(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo, jstring temp,
                                                                      jboolean isFromChanges, jstring theme, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        wstring wTheme = jstringToWString(env, theme);
        return xlst_bin2xlsx(wFrom, wTo, wTemp, isFromChanges, wTheme, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_xlst2xlsx(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo, jstring temp,
                                                                   jboolean isFromChanges, jstring theme, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        wstring wTheme = jstringToWString(env, theme);
        return xlst2xlsx(wFrom, wTo, wTemp, isFromChanges, wTheme, params);
    }


    /*
     * Ppt
     * */
    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_pptx2ppttbin(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                      jstring temp, jobject jInputParams) {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return pptx2pptt_bin(wFrom, wTo, wTemp, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_pptx2pptt(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                   jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return pptx2pptt(wFrom, wTo, wTemp, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_ppttbin2pptx(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo, jstring temp,
                                                                      jboolean isFromChanges, jstring theme, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        wstring wTheme = jstringToWString(env, theme);
        return pptt_bin2pptx(wFrom, wTo, wTemp, isFromChanges, wTheme, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_pptt2pptx(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo, jstring temp,
                                                                   jboolean isFromChanges, jstring theme, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        wstring wTheme = jstringToWString(env, theme);
        return pptt2pptx(wFrom, wTo, wTemp, isFromChanges, wTheme, params);
    }


    /*
     * Csv
     * */
    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_csv2xlst(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                  jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        params.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return csv2xlst(wFrom, wTo, wTemp, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_csv2xlsx(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                  jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        params.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return csv2xlsx(wFrom, wTo, wTemp, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_csv2xlstbin(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo, jobject jInputParams) {
        InputParams params;
        initInputParams(env, jInputParams, params);
        params.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        return csv2xlst_bin(wFrom, wTo, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_xlst2csv(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                  jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return xlst2csv(wFrom, wTo, wTemp, params);
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_xlsx2csv(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                  jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return xlsx2csv(wFrom, wTo, wTemp, params);
    }

    /*
     * Txt
     * */
    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_txt2doctbin(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo,
                                                                     jstring temp, jobject jInputParams)
    {
        InputParams params;
        initInputParams(env, jInputParams, params);
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        wstring wTemp = jstringToWString(env, temp);
        return txt2doct_bin(wFrom, wTo, wTemp, params);
    }

    /*
     * Zip
     * */
    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_dir2zip(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo) {
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        return dir2zip(jstringToWString(env, jFrom), jstringToWString(env, jTo));
    }

    JNIEXPORT jint JNICALL Java_com_onlyoffice_core_Core_zip2dir(JNIEnv* env, jobject thiz, jstring jFrom, jstring jTo) {
        wstring wFrom = jstringToWString(env, jFrom);
        wstring wTo = jstringToWString(env, jTo);
        return zip2dir(jstringToWString(env, jFrom), jstringToWString(env, jTo));
    }

}

