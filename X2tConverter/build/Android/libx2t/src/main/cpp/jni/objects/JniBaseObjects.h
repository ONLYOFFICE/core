/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#ifndef JNI_BASE_OBJECTS_H
#define JNI_BASE_OBJECTS_H

#include <jni.h>
#include <string>
#include <JniLogUtils.h>
#include <vector>
#include "../../../../../../../../../DesktopEditor/common/File.h"

#define DELETE_LOCAL_REF(ENV, REFERENCE)                                             \
    if (REFERENCE != NULL && !ENV->IsSameObject(REFERENCE, NULL)) {                  \
        ENV->DeleteLocalRef(REFERENCE);                                              \
    }

#define DELETE_WEAK_GLOBAL_REF(ENV, REFERENCE)                                       \
    if (REFERENCE != NULL && !ENV->IsSameObject(REFERENCE, NULL)) {                  \
        ENV->DeleteWeakGlobalRef(REFERENCE);                                         \
    }


class JniBaseObjects {

    public:

        static constexpr const char * CLASS_PATH_STRING = "java/lang/String";


    protected:

        jclass m_jClazz;

        jmethodID m_jConstructorId;

        std::string m_sClassPackage;

        std::string m_sConstructorSignature;


    public:

        JniBaseObjects() {
            m_jClazz = NULL;
            m_jConstructorId = NULL;
        }

        virtual ~JniBaseObjects() {}


    protected:

        virtual bool initClass(JNIEnv * jEnv) {
            if (m_jClazz != NULL) {
                LOGD("JniBaseObjects::initClass() - class %s already init!", m_sClassPackage.c_str());
                return true;
            }

            if (m_sClassPackage.empty()) {
                LOGE("JniBaseObjects::initClass() - must be set java path to package in \"m_sJavaPackage\"!");
                return false;
            }

            const char * path = m_sClassPackage.c_str();
            jclass clazz = jEnv->FindClass(path);
            if (clazz == NULL) {
                LOGE("JniBaseObjects::initClass() - can't find class %s", path);
                return false;
            }

            m_jClazz = reinterpret_cast<jclass>(jEnv->NewWeakGlobalRef(clazz));
            return true;
        }

        virtual bool initConstructor(JNIEnv * jEnv) {
            if (m_jClazz == NULL) {
                LOGD("JniBaseObjects::initConstructor() - class %s must be init first!", m_sClassPackage.c_str());
                return false;
            }

            if (m_sConstructorSignature.empty()) {
                LOGE("JniBaseObjects::initConstructor() - must be set constructor signature in \"m_sConstructorSignature\"!");
                return false;
            }

            if (m_jConstructorId != NULL) {
                LOGD("JniBaseObjects::initConstructor() - method with signature %s already init!", m_sConstructorSignature.c_str());
                return true;
            }

            m_jConstructorId = jEnv->GetMethodID(m_jClazz, "<init>", m_sConstructorSignature.c_str());
            if (m_jConstructorId == NULL) {
                LOGE("JniBaseObjects::initConstructor() - can't get object constructor for class!");
                return false;
            }

            return true;
        }


    public:

        virtual const jclass getClass() {
            return m_jClazz;
        }

        virtual const jmethodID getConstructor() {
            return m_jConstructorId;
        }

        virtual bool init(JNIEnv * jEnv) {
            return initClass(jEnv) && initConstructor(jEnv);
        }

        virtual void destroy(JNIEnv * jEnv) {
            DELETE_WEAK_GLOBAL_REF(jEnv, m_jClazz)
        }

        virtual jobject toJniObject(JNIEnv *jEnv, ...) {
            if (m_jClazz == NULL || m_jConstructorId == NULL) {
                LOGE("JniBaseObjects::toJniObject() - \"m_jClazz\" and \"m_jConstructorId\" must be init before method use!");
                return NULL;
            }

            va_list args;
            va_start(args, m_jConstructorId);
            jobject jObject = jEnv->NewObjectV(m_jClazz, m_jConstructorId, args);
            va_end(args);

            if (jObject == NULL) {
                LOGE("JniBaseObjects::toJniObject() - can't create object!");
                return NULL;
            }

            return jObject;
        }


    public:

        static std::string jstringToString(JNIEnv* env, jstring jstr) {
            jboolean isCopy;
            const char * charPath = env->GetStringUTFChars(jstr, &isCopy);
            const int size = env->GetStringUTFLength(jstr);
            const std::string str(charPath, (size_t)size);
            env->ReleaseStringUTFChars(jstr, charPath);
            return str;
        }

        static std::string* jstringToPString(JNIEnv* env, jstring jstr) {
            jboolean isCopy;
            const char * charPath = env->GetStringUTFChars(jstr, &isCopy);
            const int size = env->GetStringUTFLength(jstr);
            std::string* pStr = new std::string(charPath, (size_t)size);
            env->ReleaseStringUTFChars(jstr, charPath);
            return pStr;
        }

        static std::wstring jstringToWString(JNIEnv* env, jstring jstr) {
            jboolean isCopy;
            const char * charPath = env->GetStringUTFChars(jstr, &isCopy);
            const int size = env->GetStringUTFLength(jstr);
            std::wstring pStr = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)charPath, (LONG)size);
            env->ReleaseStringUTFChars(jstr, charPath);
            return pStr;
        }

        static jstring wStringToJString(JNIEnv* env, const std::wstring& wstr) {
            std::string sTmp = U_TO_UTF8(wstr);
            return stringToJString(env, sTmp);
        }

        static jstring stringToJString(JNIEnv* env, const std::string& str) {
            return env->NewStringUTF(str.c_str());
        }

        static jstring charToJString(JNIEnv* env, const char * str) {
            return env->NewStringUTF(str);
        }

        static jbyteArray charToJByteArray(JNIEnv *jEnv, u_char *data, int length) {
            jbyteArray jArray = jEnv->NewByteArray(length);
            jEnv->SetByteArrayRegion(jArray, 0, length, reinterpret_cast<jbyte*>(data));
            return jArray;
        }

        static jintArray intToJIntArray(JNIEnv *jEnv, uint *data, int length) {
            jintArray jArray = jEnv->NewIntArray(length);
            jEnv->SetIntArrayRegion(jArray, 0, length, reinterpret_cast<jint*>(data));
            return jArray;
        }

        static jobjectArray wStringToObjectArray(JNIEnv *jEnv, std::wstring *data, int length) {
            jobjectArray jObjectArray = NULL;
            jsize len = length;

            jObjectArray = jEnv->NewObjectArray(len, (jEnv)->FindClass(CLASS_PATH_STRING), NULL);

            for (jsize i = 0; i < len; ++i) {
                (jEnv)->SetObjectArrayElement(jObjectArray, i,
                                              JniBaseObjects::wStringToJString(jEnv, data[i]));
            }

            return jObjectArray;
        }

        static vector<std::wstring> objectArrayToVectorWString(JNIEnv *jEnv, jobjectArray stringArray) {

            vector<std::wstring>			stringArr;

            int stringCount = jEnv->GetArrayLength(stringArray);

            for (int i=0; i<stringCount; i++) {
                jstring string = (jstring) (jEnv->GetObjectArrayElement(stringArray, i));
                stringArr.push_back(jstringToWString(jEnv, string));
            }

            return stringArr;
        }
};


#endif
