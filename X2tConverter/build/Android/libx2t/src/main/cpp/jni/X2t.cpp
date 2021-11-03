#include <jni.h>
#include <jni/utils/JniExceptionsUtils.h>
#include "../../../../../../../../UnicodeConverter/UnicodeConverter_Encodings.h"
#include <jni/objects/JniX2tObjects.h>
#include "../../../../../../../../Common/OfficeFileFormats.h"
#include "../../../../../../../X2tConverter/src/dylib/x2t.h"
#include "../../../../../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../../../../../../DesktopEditor/fontengine/FontsAssistant.h"

#define JNI_FUNC(RTYPE, NAME) JNIEXPORT RTYPE JNICALL Java_lib_x2t_X2t_##NAME

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
    * OfficeFileFormats
    * */
    JNI_FUNC(jobject, getOfficeFileFormats)(JNIEnv* env, jclass type) {
        TRY_JNI(env, {
            JniHashMap jjniHashMap;
            if (jjniHashMap.init(env)) {
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_UNKNOWN", AVS_OFFICESTUDIO_FILE_UNKNOWN);

                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT", AVS_OFFICESTUDIO_FILE_DOCUMENT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF", AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM", AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX", AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC", AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT", AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF", AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT", AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML", AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT", AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB", AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2", AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI", AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM", AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX", AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM", AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT", AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT", AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC_FLAT", AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC_FLAT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER", AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER);

                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION", AVS_OFFICESTUDIO_FILE_PRESENTATION);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX", AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT", AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP", AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX", AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM", AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM", AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX", AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM", AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT", AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP", AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP);

                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET", AVS_OFFICESTUDIO_FILE_SPREADSHEET);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX", AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS", AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS", AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV", AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM", AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX", AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM", AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT", AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS", AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS);

                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SVG", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SVG);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRCanvas", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRCanvas);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDFA", AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDFA);

                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE",AVS_OFFICESTUDIO_FILE_IMAGE );
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_JPG", AVS_OFFICESTUDIO_FILE_IMAGE_JPG);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_TIFF", AVS_OFFICESTUDIO_FILE_IMAGE_TIFF);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_TGA", AVS_OFFICESTUDIO_FILE_IMAGE_TGA);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_GIF", AVS_OFFICESTUDIO_FILE_IMAGE_GIF);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_PNG", AVS_OFFICESTUDIO_FILE_IMAGE_PNG);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_EMF", AVS_OFFICESTUDIO_FILE_IMAGE_EMF);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_WMF", AVS_OFFICESTUDIO_FILE_IMAGE_WMF);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_BMP", AVS_OFFICESTUDIO_FILE_IMAGE_BMP);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_CR2", AVS_OFFICESTUDIO_FILE_IMAGE_CR2);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_PCX", AVS_OFFICESTUDIO_FILE_IMAGE_PCX);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_RAS", AVS_OFFICESTUDIO_FILE_IMAGE_RAS);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_PSD", AVS_OFFICESTUDIO_FILE_IMAGE_PSD);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_IMAGE_ICO", AVS_OFFICESTUDIO_FILE_IMAGE_ICO);

                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER", AVS_OFFICESTUDIO_FILE_OTHER);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE", AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO", AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER_HTMLZIP", AVS_OFFICESTUDIO_FILE_OTHER_HTMLZIP);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER_OLD_DOCUMENT", AVS_OFFICESTUDIO_FILE_OTHER_OLD_DOCUMENT);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER_OLD_PRESENTATION", AVS_OFFICESTUDIO_FILE_OTHER_OLD_PRESENTATION);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER_OLD_DRAWING", AVS_OFFICESTUDIO_FILE_OTHER_OLD_DRAWING);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER_TEAMLAB_INNER", AVS_OFFICESTUDIO_FILE_OTHER_TEAMLAB_INNER);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER_JSON", AVS_OFFICESTUDIO_FILE_OTHER_JSON);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_OTHER_ZIP", AVS_OFFICESTUDIO_FILE_OTHER_ZIP);

                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_TEAMLAB", AVS_OFFICESTUDIO_FILE_TEAMLAB);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY", AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY", AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY", AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY);

                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CANVAS", AVS_OFFICESTUDIO_FILE_CANVAS);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CANVAS_WORD", AVS_OFFICESTUDIO_FILE_CANVAS_WORD);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET", AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION", AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION);
                jjniHashMap.put(env, "AVS_OFFICESTUDIO_FILE_CANVAS_PDF", AVS_OFFICESTUDIO_FILE_CANVAS_PDF);

                jobject res = jjniHashMap.toJniObject(env);
                jjniHashMap.destroy(env);
                return res;
            }
            return NULL;
        })
    }

    /*
    * setFonts
    * */
    JNI_FUNC(void, setFonts)(JNIEnv* env, jclass type, jobjectArray fontsPaths, jstring cacheFontsPath) {
        TRY_JNI(env, {
            ASC::CFontsAssistant oFontsAssistant(JniBaseObjects::objectArrayToVectorWString(env, fontsPaths), JniBaseObjects::jstringToWString(env, cacheFontsPath));
            oFontsAssistant.Check();
        })
    }

    /*
     * convert
     * */
    JNI_FUNC(jint, convertFile)(JNIEnv* env, jclass type, jstring jPathXmlFile) {
        //TRY_JNI(env, {
            auto argXml = JniBaseObjects::jstringToString(env, jPathXmlFile);
            x2tchar *argv[2] = { (x2tchar *)"", (x2tchar *)argXml.c_str() };
            int retVal = 0;
            retVal = X2T_Convert(2, argv);
            return retVal;
        //})
    }

    /*
     * setIcuDataPath
     * */
    JNI_FUNC(void, setIcuDataPath)(JNIEnv* env, jclass type, jstring jIcuDatPath) {
        TRY_JNI(env, {
            auto icuDataPath = JniBaseObjects::jstringToWString(env, jIcuDatPath);
            NSUnicodeConverter::CUnicodeConverter::setIcuDataPath(icuDataPath);
        })
    }


}

