#ifndef _CORE_EXT_JS_BASE_H_
#define _CORE_EXT_JS_BASE_H_

#include "../../common/File.h"
#include "../../common/Array.h"
#include "../../../OOXML/Base/SmartPtr.h"
#include "../../graphics/BaseThread.h"

#define JS_VALUE_EXIST(value) (value.is_init() && !value->isNull() && !value->isUndefined())
#define JS_IS_VALUE_ARRAY(value) (value.is_init() && !value->isNull() && !value->isUndefined() && value->isArray())

template <class T>
using JSSmart = NSCommon::smart_ptr<T>;

namespace NSJSBase
{
    class CJSObject;
    class CJSArray;
    class CJSTypedArray;
    class CJSFunction;

    class CJSValue
    {
    public:
        CJSValue() {}
        virtual ~CJSValue() {}

        virtual bool isUndefined()          = 0;
        virtual bool isNull()               = 0;
        virtual bool isBool()               = 0;
        virtual bool isNumber()             = 0;
        virtual bool isString()             = 0;
        virtual bool isArray()              = 0;
        virtual bool isTypedArray()         = 0;
        virtual bool isObject()             = 0;
        virtual bool isFunction()           = 0;
        virtual bool isEmpty()              = 0;

        virtual void doUndefined()          = 0;
        virtual void doNull()               = 0;

        virtual bool toBool()               = 0;
        virtual int toInt32()               = 0;
        virtual unsigned int toUInt32()     = 0;
        virtual double toDouble()           = 0;
        virtual std::string toStringA()     = 0;
        virtual std::wstring toStringW()    = 0;

        virtual CJSObject* toObject()       = 0;
        virtual CJSArray* toArray()         = 0;
        virtual CJSTypedArray* toTypedArray() = 0;
        virtual CJSFunction* toFunction()   = 0;

        virtual JSSmart<CJSObject> toObjectSmart() { return toObject(); }
    };

    class CJSEmbedObjectPrivateBase
    {
    public:
        CJSEmbedObjectPrivateBase() {}
        virtual ~CJSEmbedObjectPrivateBase() {}
    };

    class CJSEmbedObject
    {
    public:
        CJSEmbedObject() { embed_native_internal = NULL; }
        virtual ~CJSEmbedObject() { RELEASEOBJECT(embed_native_internal); }

    public:
        virtual void* getObject() { return NULL; }

    protected:
        CJSEmbedObjectPrivateBase* embed_native_internal;

        friend class CJSEmbedObjectPrivateBase;
        friend class CJSEmbedObjectPrivate;
    };

    class CJSObject : public CJSValue
    {
    public:
        CJSObject() {}
        virtual ~CJSObject() {}

        virtual CJSValue* get(const char* name)                 = 0;
        virtual void set(const char* name, CJSValue* value)     = 0;

        virtual void set(const char* name, const int& value)    = 0;
        virtual void set(const char* name, const double& value) = 0;

        virtual CJSEmbedObject* getNative()                     = 0;

        virtual JSSmart<CJSValue> call_func(const char* name, const int argc = 0, JSSmart<CJSValue> argv[] = NULL) = 0;
        virtual JSSmart<CJSValue> toValue()                     = 0;
    };

    class CJSArray : public CJSValue
    {
    public:
        CJSArray() {}
        virtual ~CJSArray() {}

        virtual int getCount()                                  = 0;
        virtual JSSmart<CJSValue> get(const int& index)         = 0;
        virtual void set(const int& index, CJSValue* value)     = 0;
        virtual void set(const int& index, const bool& value)   = 0;
        virtual void set(const int& index, const int& value)    = 0;
        virtual void set(const int& index, const double& value) = 0;

        virtual void add(CJSValue* value)                       = 0;
        virtual void add_null()                                 = 0;
        virtual void add_undefined()                            = 0;
        virtual void add_bool(const bool& value)                = 0;
        virtual void add_byte(const BYTE& value)                = 0;
        virtual void add_int(const int& value)                  = 0;
        virtual void add_double(const double& value)            = 0;
        virtual void add_stringa(const std::string& value)      = 0;
        virtual void add_string(const std::wstring& value)      = 0;
        virtual JSSmart<CJSValue> toValue()                     = 0;
    };

    namespace NSAllocator
    {
        unsigned char* Alloc(const size_t& size);
        void Free(unsigned char* data, const size_t& size);
    }

    class CJSDataBuffer
    {
    public:
        BYTE* Data;
        size_t Len;

        bool IsExternalize;

    public:
        BYTE* Copy()
        {
            if (0 == Len)
                return NULL;
            BYTE* pMem = new BYTE[Len];
            if (!pMem)
                return NULL;

            memcpy(pMem, Data, Len);
            return pMem;
        }
        CJSDataBuffer()
        {
            Data = NULL;
            Len = 0;
            IsExternalize = false;
        }
        void Free()
        {
            if (Data)
            {
                NSAllocator::Free(Data, Len);
                Data = NULL;
            }
        }
    };

    class CJSTypedArray : public CJSValue
    {
    public:
        CJSTypedArray(BYTE* data = NULL, int count = 0) {}
        virtual ~CJSTypedArray() {}

        virtual int getCount()          = 0;
        virtual CJSDataBuffer getData() = 0;
        virtual JSSmart<CJSValue> toValue() = 0;
    };

    class CJSFunction : public CJSValue
    {
    public:
        CJSFunction() {}
        virtual ~CJSFunction() {}

        virtual CJSValue* Call(CJSValue* recv, int argc, JSSmart<CJSValue> argv[]) = 0;
    };

    class CJSTryCatch
    {
    public:
        CJSTryCatch() {}
        virtual ~CJSTryCatch() {}
        virtual bool Check() = 0;
    };

    class CJSIsolateScope
    {
    public:
        CJSIsolateScope()
        {
        }
        virtual ~CJSIsolateScope()
        {
        }
    };

    class CJSContextScope
    {
    public:
        CJSContextScope()
        {
        }
        virtual ~CJSContextScope()
        {
        }
    };

    class CJSLocalScope
    {
    public:
        CJSLocalScope()
        {
        }
        virtual ~CJSLocalScope()
        {
        }
    };

    class CJSContextPrivate;
    class CJSContext
    {
    public:
        CJSContextPrivate* m_internal;

    public:
        CJSContext();
        ~CJSContext();

        void Initialize();
        void Dispose();

        CJSTryCatch* GetExceptions();

        void CreateContext();
        void CreateGlobalForContext();
        CJSObject* GetGlobal();

        CJSIsolateScope* CreateIsolateScope();
        CJSContextScope* CreateContextScope();
        CJSLocalScope* CreateLocalScope();

        JSSmart<CJSValue> runScript(const std::string& script, JSSmart<CJSTryCatch> exception = NULL, const std::wstring& scriptPath = std::wstring(L""));
        CJSValue* JSON_Parse(const char* json_content);
        void MoveToThread(ASC_THREAD_ID* id = NULL);

    public:
        static CJSValue* createUndefined();
        static CJSValue* createNull();
        static CJSValue* createBool(const bool& value);
        static CJSValue* createInt(const int& value);
        static CJSValue* createUInt(const unsigned int& value);
        static CJSValue* createDouble(const double& value);
        static CJSValue* createString(const char* value, const int& length = -1);
        static CJSValue* createString(const wchar_t* value, const int& length = -1);
        static CJSValue* createString(const std::string& value);
        static CJSValue* createString(const std::wstring& value);

        static CJSObject* createObject();

        static CJSArray* createArray(const int& count);

        static CJSTypedArray* createUint8Array(BYTE* data = NULL, int count = 0, const bool& isExternalize = true);

        static CJSValue* createUint8Array(const std::wstring& sFilePath)
        {
            NSFile::CFileBinary oFileBinary;
            if (oFileBinary.OpenFile(sFilePath))
            {
                long nFileSize = oFileBinary.GetFileSize();
                BYTE* pData = NSAllocator::Alloc((size_t)nFileSize);
                DWORD dwSizeRead;
                if (oFileBinary.ReadFile(pData, (DWORD)nFileSize, dwSizeRead))
                {
                    return CJSContext::createUint8Array(pData, (int)nFileSize, false);
                }
                NSAllocator::Free(pData, (size_t)nFileSize);
            }
            return CJSContext::createNull();
        }

    public:
        static CJSContext* GetCurrent();

    public:
        static void ExternalInitialize(const std::wstring& sDirectory);
        static void ExternalDispose();
        static bool IsSupportNativeTypedArrays();
    };
}

#endif // _CORE_EXT_JS_BASE_H_
