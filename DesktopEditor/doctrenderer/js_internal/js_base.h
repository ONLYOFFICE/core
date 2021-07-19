#ifndef _CORE_EXT_JS_BASE_H_
#define _CORE_EXT_JS_BASE_H_

#include "../../common/File.h"
#include "../../common/Array.h"
#include "../../../Common/DocxFormat/Source/Base/SmartPtr.h"

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
        virtual double toDouble()           = 0;
        virtual std::string toStringA()     = 0;
        virtual std::wstring toStringW()    = 0;

        virtual CJSObject* toObject()       = 0;
        virtual CJSArray* toArray()         = 0;
        virtual CJSTypedArray* toTypedArray() = 0;
        virtual CJSFunction* toFunction()   = 0;
    };

    class CJSEmbedObject
    {
    public:
        CJSEmbedObject() {}
        virtual ~CJSEmbedObject() {}

    public:
        virtual void* getObject() { return NULL; }
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

    class CJSTypedArray : public CJSValue
    {
    public:
        CJSTypedArray(BYTE* data = NULL, int count = 0) {}
        virtual ~CJSTypedArray() {}

        virtual int getCount()          = 0;
        virtual const BYTE* getData()   = 0;
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

    public:
        static CJSValue* createUndefined();
        static CJSValue* createNull();
        static CJSValue* createBool(const bool& value);
        static CJSValue* createInt(const int& value);
        static CJSValue* createDouble(const double& value);
        static CJSValue* createString(const char* value, const int& length = -1);
        static CJSValue* createString(const std::string& value);
        static CJSValue* createString(const std::wstring& value);

        static CJSObject* createObject();

        static CJSArray* createArray(const int& count);

        static CJSTypedArray* createUint8Array(BYTE* data = NULL, int count = 0);

    public:
        static CJSContext* GetCurrent();

    public:
        static void ExternalInitialize();
        static void ExternalDispose();
        static bool IsSupportNativeTypedArrays();
    };
}

#endif // _CORE_EXT_JS_BASE_H_
