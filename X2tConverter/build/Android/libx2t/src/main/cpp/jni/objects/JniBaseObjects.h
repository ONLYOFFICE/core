#ifndef JNI_BASE_OBJECTS_H
#define JNI_BASE_OBJECTS_H

#include <jni.h>
#include <string>
#include <locale>
#include <codecvt>
#include <JniLogUtils.h>
#include <vector>

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
            const int size = env->GetStringLength(jstr);
            const std::string str(charPath, charPath + size);
            env->ReleaseStringUTFChars(jstr, charPath);
            return str;
        }

        static std::string* jstringToPString(JNIEnv* env, jstring jstr) {
            jboolean isCopy;
            const char * charPath = env->GetStringUTFChars(jstr, &isCopy);
            const int size = env->GetStringLength(jstr);
            std::string* pStr = new std::string(charPath, charPath + size);
            env->ReleaseStringUTFChars(jstr, charPath);
            return pStr;
        }

        static std::wstring jstringToWString(JNIEnv* env, jstring jstr) {
            jboolean isCopy;
            const char * cstr = env->GetStringUTFChars(jstr, &isCopy);
            const int size = env->GetStringLength(jstr);
            const std::wstring wstr = charsToWString(cstr);
            env->ReleaseStringUTFChars(jstr, cstr);
            return wstr;
        }

        static jstring wStringToJString(JNIEnv* env, const std::wstring wstr) {
            return env->NewStringUTF(wStringToString(wstr).c_str());
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

        static std::wstring charsToWString(const char * str) {
            using convert_typeX = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_typeX, wchar_t> converterX;
            return converterX.from_bytes(str);
        }

        static std::wstring stringToWString(const std::string & str) {
            using convert_typeX = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_typeX, wchar_t> converterX;
            return converterX.from_bytes(str);
        }

        static std::string wStringToString(const std::wstring & str) {
            using convert_typeX = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_typeX, wchar_t> converterX;
            return converterX.to_bytes(str);
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
