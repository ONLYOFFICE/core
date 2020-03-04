#include <jni.h>
#include <ASCConverters.h>
#include <cextracttools.h>
#include <UnicodeConverter_Encodings.h>
#include <jni/utils/JniExceptionsUtils.h>
#include <jni/objects/JniX2tObjects.h>
#include <jni/JniX2tParams.h>

#define JNI_FUNC(RTYPE, NAME) JNIEXPORT RTYPE JNICALL Java_lib_x2t_X2t_##NAME

using namespace NExtractTools;

extern "C" {

    /*
     * Encoding
     * */
    JNI_FUNC(jobjectArray, getEncodings)(JNIEnv* env, jclass type) {
        TRY_JNI(env, {
            EncodingsJniObject jJniEncodings;
            if (jJniEncodings.init(env)) {
                jobjectArray jEncodings = jJniEncodings.toJniObject(env, NSUnicodeConverter::Encodings);
                jJniEncodings.destroy(env);
                return jEncodings;
            }
            return NULL;
        })
    }


    /*
     * Odf
     * */
    JNI_FUNC(jint, odf2oox)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return odf2oox(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }


    /*
     * Docs
     * */
    JNI_FUNC(jint, docx2doctbin)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return docx2doct_bin(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, docx2doct)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return docx2doct(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, docx2odt)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return docx2odt(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, docxdir2txt)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return docx_dir2txt(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, docx2rtf)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return docx2rtf(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, doctbin2docx)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return doct_bin2docx(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, *params.m_bFromChanges, *params.m_sThemeDir, params);
        })
    }

    JNI_FUNC(jint, doct2docx)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return doct2docx(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, *params.m_bFromChanges, *params.m_sThemeDir, params);
        })
    }

    JNI_FUNC(jint, rtf2docx)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return rtf2docx(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, txt2docx)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return txt2docx(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, rtf2doctbin)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return rtf2doct_bin(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, doc2doctbin)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return doc2doct_bin(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }


    /*
     * Xls
     * */
    JNI_FUNC(jint, xlsx2xlstbin)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return xlsx2xlst_bin(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, xlsx2xlst)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return xlsx2xlst(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, xlsx2ods)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return xlsx2ods(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, xlstbin2xlsx)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return xlst_bin2xlsx(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, *params.m_bFromChanges, *params.m_sThemeDir, params);
        })
    }

    JNI_FUNC(jint, xlst2xlsx)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return xlst2xlsx(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, *params.m_bFromChanges, *params.m_sThemeDir, params);
        })
    }

    JNI_FUNC(jint, xls2xlstbin)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return xls2xlst_bin(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }


    /*
     * Ppt
     * */
    JNI_FUNC(jint, pptx2ppttbin)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return pptx2pptt_bin(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, pptx2pptt)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return pptx2pptt(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, ppttbin2pptx)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return pptt_bin2pptx(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, *params.m_bFromChanges, *params.m_sThemeDir, params);
        })
    }

    JNI_FUNC(jint, pptt2pptx)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return pptt2pptx(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, *params.m_bFromChanges, *params.m_sThemeDir, params);
        })
    }

    JNI_FUNC(jint, pptx2odp)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return pptx2odp(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, pptx2DirOdp)(JNIEnv* env, jclass type, jobject jInputParams, jboolean isTemplate) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return pptx_dir2odp(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params, isTemplate);
        })
    }

    JNI_FUNC(jint, ppt2ppttbin)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return ppt2pptt_bin(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }


    /*
     * Csv
     * */
    JNI_FUNC(jint, csv2xlst)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return csv2xlst(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, csv2xlsx)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return csv2xlsx(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, csv2xlstbin)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            params.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV);
            return csv2xlst_bin(*params.m_sFileFrom, *params.m_sFileTo, params);
        })
    }

    JNI_FUNC(jint, xlst2csv)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return xlst2csv(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    JNI_FUNC(jint, xlsx2csv)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return xlsx2csv(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    /*
     * Txt
     * */
    JNI_FUNC(jint, txt2doctbin)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return txt2doct_bin(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, params);
        })
    }

    /*
     * Zip
     * */
    JNI_FUNC(jint, dir2zip)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return dir2zip(*params.m_sFileFrom, *params.m_sFileTo);
        })
    }

    JNI_FUNC(jint, zip2dir)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return zip2dir(*params.m_sFileFrom, *params.m_sFileTo);
        })
    }

    /*
     * Bin
     * */
    JNI_FUNC(jint, bin2pdf)(JNIEnv* env, jclass type, jobject jInputParams) {
        TRY_JNI(env, {
            InputParams params = getInputParams(env, jInputParams);
            return bin2pdf(*params.m_sFileFrom, *params.m_sFileTo, *params.m_sTempDir, *params.m_bFromChanges, *params.m_sThemeDir, params);
        })
    }

}

