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
