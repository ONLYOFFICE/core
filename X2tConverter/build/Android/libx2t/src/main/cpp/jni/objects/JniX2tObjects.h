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

#ifndef JNI_X2T_OBJECTS_H
#define JNI_X2T_OBJECTS_H

#include <JniBaseObjects.h>

class EncodingJniObject : public JniBaseObjects {

    public:

        static constexpr const char * CLASS_PATH_ENCODINGS = "lib/x2t/data/Encoding";

    public:

        EncodingJniObject() {
            m_sClassPackage = CLASS_PATH_ENCODINGS;
            m_sConstructorSignature.append("(II")
                    .append("L").append(CLASS_PATH_STRING).append(";")
                    .append("L").append(CLASS_PATH_STRING).append(";")
                    .append(")V");
        }

        jobject toJniObject(JNIEnv * jEnv, int index, int code, const char * name, const char * displayName) {
            jstring jName = JniBaseObjects::charToJString(jEnv, name);
            jstring jDisplayName = JniBaseObjects::charToJString(jEnv, displayName);

            jobject jObject = JniBaseObjects::toJniObject(jEnv, index, code, jName, jDisplayName);

            DELETE_LOCAL_REF(jEnv, jName);
            DELETE_LOCAL_REF(jEnv, jDisplayName);
            return jObject;
        }
};

class EncodingsJniObject : public JniBaseObjects {

    protected:

        EncodingJniObject m_jEncodingObject;

    public:

        EncodingsJniObject() {}

    public:

        bool init(JNIEnv *jEnv) override {
            return m_jEncodingObject.init(jEnv);
        }

        void destroy(JNIEnv *jEnv) override {
            m_jEncodingObject.destroy(jEnv);
        }

        jobjectArray toJniObject(JNIEnv *jEnv, const NSUnicodeConverter::EncodindId encodings[]) {
            const int encodingsSize = sizeof(NSUnicodeConverter::Encodings)/sizeof(NSUnicodeConverter::EncodindId);
            jobjectArray jArrayObj = jEnv->NewObjectArray(encodingsSize, m_jEncodingObject.getClass(), NULL);

            for(int i = 0; i < encodingsSize; i++) {
                jobject jObject = m_jEncodingObject.toJniObject(jEnv, encodings[i].Index, encodings[i].WindowsCodePage, encodings[i].Name, encodings[i].DisplayName);
                jEnv->SetObjectArrayElement(jArrayObj, i, jObject);
                DELETE_LOCAL_REF(jEnv, jObject);
            }
            return jArrayObj;
        }
};


class JniHashMap: public JniBaseObjects
{
    public:

        static constexpr const char * CLASS_PATH_HASH_MAP = "java/util/HashMap";

    public:
        jobject m_oHashMap;
        jmethodID m_mPut;

        JniHashMap() {
            m_sClassPackage = CLASS_PATH_HASH_MAP;
            m_sConstructorSignature = "()V";
            m_oHashMap = NULL;
            m_mPut = NULL;
        }
    public:

        bool init(JNIEnv *jEnv) override {
            bool res = JniBaseObjects::init(jEnv);
            if(res)
                m_oHashMap = JniBaseObjects::toJniObject(jEnv);
            return res;
        }
        jobject toJniObject(JNIEnv * jEnv) {
            return m_oHashMap;
        }

        void put(JNIEnv * jEnv, const char* key, int value)
        {
            if(m_oHashMap == NULL)
                return;
            if(m_mPut == NULL)
                m_mPut = jEnv->GetMethodID(getClass(), "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

            auto jkey = charToJString(jEnv, key);
            auto jvalue = wStringToJString(jEnv,to_wstring(value));

            if (m_mPut != NULL) {
                jEnv->CallObjectMethod(m_oHashMap, m_mPut, jkey, jvalue);
            }

            DELETE_LOCAL_REF(jEnv, jkey)
            DELETE_LOCAL_REF(jEnv, jvalue)

        }
};

#endif
