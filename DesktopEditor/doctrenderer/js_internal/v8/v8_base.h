﻿#ifndef _BUILD_NATIVE_CONTROL_V8_BASE_H_
#define _BUILD_NATIVE_CONTROL_V8_BASE_H_

#include "../js_base.h"
#include <iostream>

#include "v8.h"
#include "libplatform/libplatform.h"

#ifdef V8_VERSION_87_PLUS
#define kV8NormalString v8::NewStringType::kNormal
#define kV8ProduceCodeCache v8::ScriptCompiler::kEagerCompile
#define V8ContextFirstArg CV8Worker::GetCurrentContext(),
#define V8ContextOneArg CV8Worker::GetCurrentContext()
#define V8IsolateFirstArg CV8Worker::GetCurrent(),
#define V8IsolateOneArg CV8Worker::GetCurrent()
#define V8ToChecked ToChecked
#else
#define kV8NormalString v8::NewStringType::kNormal
#define kV8ProduceCodeCache v8::ScriptCompiler::kProduceCodeCache
#define V8ContextFirstArg CV8Worker::GetCurrentContext(),
#define V8ContextOneArg CV8Worker::GetCurrentContext()
#define V8IsolateFirstArg
#define V8IsolateOneArg CV8Worker::GetCurrent()
#ifdef V8_OS_XP
#define V8ToChecked FromJust
#else
#define V8ToChecked ToChecked
#endif
#endif

v8::Local<v8::String> CreateV8String(v8::Isolate* i, const char* str, const int& len = -1);
v8::Local<v8::String> CreateV8String(v8::Isolate* i, const std::string& str);

#ifdef __ANDROID__
    #include <JniLogUtils.h>
#endif

#ifdef V8_OS_XP
class MallocArrayBufferAllocator : public v8::ArrayBuffer::Allocator
{
public:
    virtual void* Allocate(size_t length)
    {
        void* ret = malloc(length);
        memset(ret, 0, length);
        return ret;
    }
    virtual void* AllocateUninitialized(size_t length)
    {
        return malloc(length);
    }
    virtual void Free(void* data, size_t length)
    {
        free(data);
    }
};
#endif

class CV8Initializer
{
private:
#ifdef V8_VERSION_87_PLUS
    std::unique_ptr<v8::Platform> m_platform;
#else
    v8::Platform* m_platform;
#endif
    v8::ArrayBuffer::Allocator* m_pAllocator;

public:
    CV8Initializer()
    {
        std::wstring sPrW = NSFile::GetProcessPath();
        std::string sPrA = U_TO_UTF8(sPrW);

        m_pAllocator = NULL;

    #ifndef V8_OS_XP
        v8::V8::InitializeICUDefaultLocation(sPrA.c_str());
        v8::V8::InitializeExternalStartupData(sPrA.c_str());
        #ifdef V8_VERSION_87_PLUS
        m_platform = v8::platform::NewDefaultPlatform();
        v8::V8::InitializePlatform(m_platform.get());
        #else
        m_platform = v8::platform::CreateDefaultPlatform();
        v8::V8::InitializePlatform(m_platform);
        #endif
        v8::V8::Initialize();
    #else
        m_platform = v8::platform::CreateDefaultPlatform();
        v8::V8::InitializePlatform(m_platform);
        v8::V8::Initialize();
        v8::V8::InitializeICU();
    #endif
    }
    ~CV8Initializer()
    {
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();
        #ifndef V8_VERSION_87_PLUS
        delete m_platform;
        #endif
        if (m_pAllocator)
            delete m_pAllocator;
    }

    v8::ArrayBuffer::Allocator* getAllocator()
    {
        return m_pAllocator;
    }

    v8::Isolate* CreateNew()
    {
        v8::Isolate::CreateParams create_params;
    #ifndef V8_OS_XP
        m_pAllocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    #else
        m_pAllocator = new MallocArrayBufferAllocator();
    #endif
        create_params.array_buffer_allocator = m_pAllocator;
        return v8::Isolate::New(create_params);
    }
};

class CV8Worker
{
private:
    static CV8Initializer* m_pInitializer;
    static bool m_bUseExternalInitialize;

public:
    CV8Worker() {}
    ~CV8Worker() {}

    static void Initialize()
    {
        if (NULL == m_pInitializer)
            m_pInitializer = new CV8Initializer();
    }

    static void Dispose()
    {
        if (NULL != m_pInitializer)
            delete m_pInitializer;
        m_pInitializer = NULL;
        m_bUseExternalInitialize = false;
    }

    static CV8Initializer* getInitializer()
    {
        if (NULL == m_pInitializer)
        {
            m_pInitializer = new CV8Initializer();
        }

        return CV8Worker::m_pInitializer;
    }

    static v8::Isolate* GetCurrent()
    {
        return v8::Isolate::GetCurrent();
    }
    static v8::Local<v8::Context> GetCurrentContext()
    {
        return v8::Isolate::GetCurrent()->GetCurrentContext();
    }

    static void SetUseExetralInitialize()
    {
        m_bUseExternalInitialize = true;
    }
    static bool IsUseExternalInitialize()
    {
        return m_bUseExternalInitialize;
    }
};

namespace NSJSBase
{

    template<typename V, typename B>
    class CJSValueV8Template : public B
    {
    public:
        v8::Local<V> value;

        CJSValueV8Template()
        {
        }

        CJSValueV8Template(const v8::Local<V>& _value)
        {
            value = _value;
        }

    public:

        virtual ~CJSValueV8Template()
        {
            value.Clear();
        }

        virtual bool isUndefined()
        {
            return value.IsEmpty() ? false : value->IsUndefined();
        }
        virtual bool isNull()
        {
            return value.IsEmpty() ? false : value->IsNull();
        }
        virtual bool isBool()
        {
            return value.IsEmpty() ? false : value->IsBoolean();
        }
        virtual bool isNumber()
        {
            return value.IsEmpty() ? false : value->IsNumber();
        }
        virtual bool isString()
        {
            return value.IsEmpty() ? false : value->IsString();
        }
        virtual bool isArray()
        {
            return value.IsEmpty() ? false : value->IsArray();
        }
        virtual bool isTypedArray()
        {
            return value.IsEmpty() ? false : value->IsTypedArray();
        }
        virtual bool isObject()
        {
            return value.IsEmpty() ? false : value->IsObject();
        }
        virtual bool isFunction()
        {
            return value.IsEmpty() ? false : value->IsFunction();
        }
        virtual bool isEmpty()
        {
            return value.IsEmpty();
        }

        virtual void doUndefined()
        {            
        }

        virtual void doNull()
        {
        }

        virtual bool toBool()
        {
            return false;
        }

        virtual int toInt32()
        {
            return 0;
        }

        virtual double toDouble()
        {
            return 0;
        }

        virtual std::string toStringA()
        {
            return "";
        }

        virtual std::wstring toStringW()
        {
            return L"";
        }

        virtual CJSObject* toObject();
        virtual CJSArray* toArray();
        virtual CJSTypedArray* toTypedArray();
        virtual CJSFunction* toFunction();
    };

    class CJSValueV8TemplatePrimitive : public CJSValueV8Template<v8::Value, CJSValue>
    {
    public:
        CJSValueV8TemplatePrimitive()
        {
        }
        CJSValueV8TemplatePrimitive(const v8::Local<v8::Value>& _value) : CJSValueV8Template<v8::Value, CJSValue>(_value)
        {
        }

        virtual ~CJSValueV8TemplatePrimitive()
        {
            value.Clear();
        }

        virtual void doUndefined()
        {
            value = v8::Undefined(CV8Worker::GetCurrent());
        }

        virtual void doNull()
        {
            value = v8::Null(CV8Worker::GetCurrent());
        }

        virtual bool toBool()
        {
#ifdef V8_VERSION_87_PLUS
            return value.IsEmpty() ? false : value->BooleanValue(V8IsolateOneArg);
#else
            return value.IsEmpty() ? false : value->BooleanValue(V8ContextOneArg).V8ToChecked();
#endif
        }

        virtual int toInt32()
        {
            return value.IsEmpty() ? 0 : value->Int32Value(V8ContextOneArg).V8ToChecked();
        }

        virtual double toDouble()
        {
            return value.IsEmpty() ? 0 : value->NumberValue(V8ContextOneArg).V8ToChecked();
        }

        virtual std::string toStringA()
        {
            if (value.IsEmpty())
                return "";

            v8::String::Utf8Value data(V8IsolateFirstArg value);
            if (NULL == *data)
                return "";

            return std::string((char*)(*data), data.length());
        }

        virtual std::wstring toStringW()
        {
            if (value.IsEmpty())
                return L"";

            v8::String::Utf8Value data(V8IsolateFirstArg value);
            if (NULL == *data)
                return L"";

            return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)(*data), data.length());
        }
    };

    typedef CJSValueV8TemplatePrimitive CJSValueV8;

    class CJSObjectV8 : public CJSValueV8Template<v8::Object, CJSObject>
    {
    public:
        CJSObjectV8()
        {
        }

        virtual ~CJSObjectV8()
        {
            value.Clear();
        }

        virtual CJSValue* get(const char* name)
        {
            CJSValueV8* _value = new CJSValueV8();
            v8::Local<v8::String> _name = CreateV8String(CV8Worker::GetCurrent(), name);
            _value->value = value->Get(V8ContextFirstArg _name).ToLocalChecked();
            return _value;
        }

        virtual void set(const char* name, CJSValue* value_param)
        {
            CJSValueV8* _value = static_cast<CJSValueV8*>(value_param);
            v8::Local<v8::String> _name = CreateV8String(CV8Worker::GetCurrent(), name);
            value->Set(V8ContextFirstArg _name, _value->value);
        }

        virtual void set(const char* name, const int& _value)
        {
            v8::Isolate* isolate = CV8Worker::GetCurrent();
            v8::Local<v8::String> _name = CreateV8String(CV8Worker::GetCurrent(), name);
            value->Set(V8ContextFirstArg _name, v8::Integer::New(isolate, _value));
        }

        virtual void set(const char* name, const double& _value)
        {
            v8::Isolate* isolate = CV8Worker::GetCurrent();
            v8::Local<v8::String> _name = CreateV8String(CV8Worker::GetCurrent(), name);
            value->Set(V8ContextFirstArg _name, v8::Number::New(isolate, _value));
        }

        virtual CJSEmbedObject* getNative()
        {
            v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(value->GetInternalField(0));
            return (CJSEmbedObject*)field->Value();
        }

        virtual JSSmart<CJSValue> call_func(const char* name, const int argc = 0, JSSmart<CJSValue> argv[] = NULL)
        {
            v8::Local<v8::String> _name = CreateV8String(CV8Worker::GetCurrent(), name);
            v8::Handle<v8::Value> _func = value->Get(V8ContextFirstArg _name).ToLocalChecked();

            CJSValueV8* _return = new CJSValueV8();
            if (_func->IsFunction())
            {
                v8::Handle<v8::Function> _funcN = v8::Handle<v8::Function>::Cast(_func);

                if (0 == argc)
                {
                    v8::MaybeLocal<v8::Value> retValue = _funcN->Call(V8ContextFirstArg value, 0, NULL);
                    if (!retValue.IsEmpty())
                        _return->value = retValue.ToLocalChecked();
                }
                else
                {
                    v8::Local<v8::Value>* args = new v8::Local<v8::Value>[argc];
                    for (int i = 0; i < argc; ++i)
                    {
                        CJSValueV8* _value_arg = static_cast<CJSValueV8*>(argv[i].operator ->());
                        args[i] = _value_arg->value;
                    }
                    v8::MaybeLocal<v8::Value> retValue = _funcN->Call(V8ContextFirstArg value, argc, args);
                    if (!retValue.IsEmpty())
                        _return->value = retValue.ToLocalChecked();
                    RELEASEARRAYOBJECTS(args);
                }
            }

            JSSmart<CJSValue> _ret = _return;
            return _ret;
        }

        virtual JSSmart<CJSValue> toValue()
        {
            CJSValueV8* _value = new CJSValueV8();
            _value->value = value;
            return _value;
        }
    };

    class CJSArrayV8 : public CJSValueV8Template<v8::Array, CJSArray>
    {
    public:
        int m_count;
    public:
        CJSArrayV8()
        {
            m_count = 0;
        }
        virtual ~CJSArrayV8()
        {
            value.Clear();
        }

        virtual int getCount()
        {
            return value->Length();
        }

        virtual JSSmart<CJSValue> get(const int& index)
        {
            CJSValueV8* _value = new CJSValueV8();
            _value->value = value->Get(V8ContextFirstArg index).ToLocalChecked();
            return _value;
        }

        virtual void set(const int& index, CJSValue* value_param)
        {
            CJSValueV8* _value = static_cast<CJSValueV8*>(value_param);
            value->Set(V8ContextFirstArg index, _value->value);
        }

        virtual void add(CJSValue* value_param)
        {
            CJSValueV8* _value = static_cast<CJSValueV8*>(value_param);
            value->Set(V8ContextFirstArg getCount(), _value->value);
        }

        virtual void set(const int& index, const bool& _value)
        {
            value->Set(V8ContextFirstArg index, v8::Boolean::New(CV8Worker::GetCurrent(), _value));
        }

        virtual void set(const int& index, const int& _value)
        {
            value->Set(V8ContextFirstArg index, v8::Integer::New(CV8Worker::GetCurrent(), _value));
        }

        virtual void set(const int& index, const double& _value)
        {
            value->Set(V8ContextFirstArg index, v8::Number::New(CV8Worker::GetCurrent(), _value));
        }

        virtual void add_null()
        {
            value->Set(V8ContextFirstArg m_count++, v8::Null(CV8Worker::GetCurrent()));
        }

        virtual void add_undefined()
        {
            value->Set(V8ContextFirstArg m_count++, v8::Undefined(CV8Worker::GetCurrent()));
        }

        virtual void add_bool(const bool& _value)
        {
            value->Set(V8ContextFirstArg m_count++, v8::Boolean::New(CV8Worker::GetCurrent(), _value));
        }

        virtual void add_byte(const BYTE& _value)
        {
            value->Set(V8ContextFirstArg m_count++, v8::Integer::New(CV8Worker::GetCurrent(), (int)_value));
        }

        virtual void add_int(const int& _value)
        {
            value->Set(V8ContextFirstArg m_count++, v8::Integer::New(CV8Worker::GetCurrent(), _value));
        }

        virtual void add_double(const double& _value)
        {
            value->Set(V8ContextFirstArg m_count++, v8::Number::New(CV8Worker::GetCurrent(), _value));
        }

        virtual void add_stringa(const std::string& _value)
        {
            value->Set(V8ContextFirstArg m_count++, CreateV8String(CV8Worker::GetCurrent(), _value));
        }

        virtual void add_string(const std::wstring& _value)
        {
            std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(_value);
            add_stringa(sReturn);
        }

        virtual JSSmart<CJSValue> toValue()
        {
            CJSValueV8* _value = new CJSValueV8();
            _value->value = value;
            return _value;
        }
    };

    class CJSTypedArrayV8 : public CJSValueV8Template<v8::Uint8Array, CJSTypedArray>
    {
    public:
        CJSTypedArrayV8(BYTE* data = NULL, int count = 0)
        {
            if (0 < count)
            {
                v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(CV8Worker::GetCurrent(), (void*)data, (size_t)count);
                value = v8::Uint8Array::New(_buffer, 0, (size_t)count);
            }
        }
        virtual ~CJSTypedArrayV8()
        {
            value.Clear();
        }

        virtual int getCount()
        {
            return (int)value->ByteLength();
        }

        virtual const BYTE* getData()
        {
            return (BYTE*)value->Buffer()->Externalize().Data();
        }

        virtual JSSmart<CJSValue> toValue()
        {
            CJSValueV8* _value = new CJSValueV8();
            _value->value = value;
            return _value;
        }
    };

    class CJSFunctionV8 : public CJSValueV8Template<v8::Function, CJSFunction>
    {
    public:
        CJSFunctionV8()
        {
        }
        virtual ~CJSFunctionV8()
        {
            value.Clear();
        }

        virtual CJSValue* Call(CJSValue* recv, int argc, JSSmart<CJSValue> argv[])
        {
            CJSValueV8* _value = static_cast<CJSValueV8*>(recv);
            CJSValueV8* _return = new CJSValueV8();
            if (0 == argc)
            {
                _return->value = value->Call(V8ContextFirstArg _value->value, 0, NULL).ToLocalChecked();
            }
            else
            {
                v8::Local<v8::Value>* args = new v8::Local<v8::Value>[argc];
                for (int i = 0; i < argc; ++i)
                {
                    CJSValueV8* _value_arg = static_cast<CJSValueV8*>(argv[i].operator ->());
                    args[i] = _value_arg->value;
                }
                _return->value = value->Call(V8ContextFirstArg _value->value, argc, args).ToLocalChecked();
                RELEASEARRAYOBJECTS(args);
            }
            return _return;
        }
    };

    template<typename V, typename B>
    CJSObject* CJSValueV8Template<V, B>::toObject()
    {
        CJSObjectV8* _value = new CJSObjectV8();
        _value->value = value->ToObject(V8ContextOneArg).ToLocalChecked();
        return _value;
    }

    template<typename V, typename B>
    CJSArray* CJSValueV8Template<V, B>::toArray()
    {
        CJSArrayV8* _value = new CJSArrayV8();
        _value->value = v8::Local<v8::Array>::Cast(value);
        return _value;
    }

    template<typename V, typename B>
    CJSTypedArray* CJSValueV8Template<V, B>::toTypedArray()
    {
        CJSTypedArrayV8* _value = new CJSTypedArrayV8();
        _value->value = v8::Local<v8::Uint8Array>::Cast(value);
        return _value;
    }

    template<typename V, typename B>
    CJSFunction* CJSValueV8Template<V, B>::toFunction()
    {
        CJSFunctionV8* _value = new CJSFunctionV8();
        _value->value = v8::Local<v8::Function>::Cast(value);
        return _value;
    }
}

namespace NSJSBase
{
    // TRY - CATCH
    class CV8TryCatch : public CJSTryCatch
    {
    private:
        v8::TryCatch try_catch;

    public:
        CV8TryCatch() : CJSTryCatch(), try_catch(V8IsolateOneArg)
        {
        }
        virtual ~CV8TryCatch()
        {
        }

    public:
        virtual bool Check()
        {
            if (try_catch.HasCaught())
            {
                int nLineNumber             = try_catch.Message()->GetLineNumber(V8ContextOneArg).V8ToChecked();

                JSSmart<CJSValueV8> _line = new CJSValueV8();
                _line->value = try_catch.Message()->GetSourceLine(V8ContextOneArg).ToLocalChecked();

                JSSmart<CJSValueV8> _exception = new CJSValueV8();
                _exception->value = try_catch.Message()->Get();

                std::string strCode        = _line->toStringA();
                std::string strException   = _exception->toStringA();

#ifndef __ANDROID__
                std::cerr << strException << std::endl;
#else
                LOGE("NSJSBase::CV8TryCatch::Check() - error:");
                LOGE(std::to_string(nLineNumber).c_str());
                LOGE(strCode.c_str());
                LOGE(strException.c_str());
#endif
                return true;
            }
            return false;
        }
    };
}

namespace NSJSBase
{
    class CJSContextPrivate
    {
    public:
        CV8Worker m_oWorker;
        v8::Isolate* m_isolate;

        v8::Local<v8::ObjectTemplate>   m_global;
        v8::Local<v8::Context>          m_context;

    public:
        CJSContextPrivate() : m_oWorker(), m_isolate(NULL)
        {
        }
    };
}

namespace NSV8Objects
{
    static void Template_Set(v8::Local<v8::ObjectTemplate>& obj, const char* name, v8::FunctionCallback callback)
    {
        v8::Isolate* current = CV8Worker::GetCurrent();
        obj->Set(CreateV8String(current, name), v8::FunctionTemplate::New(current, callback));
    }
}

inline NSJSBase::CJSEmbedObject* unwrap_native(const v8::Local<v8::Object>& value)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(value->GetInternalField(0));
    return (NSJSBase::CJSEmbedObject*)field->Value();
}
inline NSJSBase::CJSEmbedObject* unwrap_native2(const v8::Local<v8::Value>& value)
{
    v8::Local<v8::Object> _obj = value->ToObject(V8ContextOneArg).ToLocalChecked();
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(_obj->GetInternalField(0));
    return (NSJSBase::CJSEmbedObject*)field->Value();
}

inline JSSmart<NSJSBase::CJSValue> js_value(const v8::Local<v8::Value>& value)
{
    return new NSJSBase::CJSValueV8(value);
}
inline JSSmart<NSJSBase::CJSValue> js_object(const v8::Local<v8::Object>& value)
{
    NSJSBase::CJSObjectV8* _ret = new NSJSBase::CJSObjectV8();
    _ret->value = value;
    return _ret;
}
inline void js_return(const v8::FunctionCallbackInfo<v8::Value>& args, JSSmart<NSJSBase::CJSValue>& value)
{
    if (value.is_init())
    {
        NSJSBase::CJSValueV8* _value = (NSJSBase::CJSValueV8*)(value.operator ->());
        args.GetReturnValue().Set(_value->value);
    }
}
inline void js_return(const v8::PropertyCallbackInfo<v8::Value>& info, JSSmart<NSJSBase::CJSValue>& value)
{
    if (value.is_init())
    {
        NSJSBase::CJSValueV8* _value = (NSJSBase::CJSValueV8*)(value.operator ->());
        info.GetReturnValue().Set(_value->value);
    }
}

#define PROPERTY_GET(NAME, NAME_EMBED)                                                      \
    void NAME(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info) \
    {                                                                                       \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(info.Holder());              \
        JSSmart<CJSValue> ret = _this->NAME_EMBED();                                        \
        js_return(info, ret);                                                               \
    }

#define FUNCTION_WRAPPER_V8(NAME, NAME_EMBED)                                       \
    void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                      \
    {                                                                               \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());        \
        JSSmart<CJSValue> ret = _this->NAME_EMBED();                                \
        js_return(args, ret);                                                       \
    }

#define FUNCTION_WRAPPER_V8_1(NAME, NAME_EMBED)                                     \
    void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                      \
    {                                                                               \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());        \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]));               \
        js_return(args, ret);                                                       \
    }
#define FUNCTION_WRAPPER_V8_2(NAME, NAME_EMBED)                                             \
    void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                              \
    {                                                                                       \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]));    \
        js_return(args, ret);                                                               \
    }
#define FUNCTION_WRAPPER_V8_3(NAME, NAME_EMBED)                                                                 \
    void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                                                  \
    {                                                                                                           \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                                    \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]), js_value(args[2]));     \
        js_return(args, ret);                                                                                   \
    }
#define FUNCTION_WRAPPER_V8_4(NAME, NAME_EMBED)                                                                                 \
    void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                                                                  \
    {                                                                                                                           \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                                                    \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]), js_value(args[2]), js_value(args[3]));  \
        js_return(args, ret);                                                                                                   \
    }
#define FUNCTION_WRAPPER_V8_5(NAME, NAME_EMBED)                                                                                                     \
    void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                                                                                      \
    {                                                                                                                                               \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                                                                        \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]), js_value(args[2]), js_value(args[3]), js_value(args[4]));   \
        js_return(args, ret);                                                                                                                       \
    }
#define FUNCTION_WRAPPER_V8_6(NAME, NAME_EMBED)                                                                                                     \
    void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                                                                                      \
    {                                                                                                                                               \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                                                                        \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]), js_value(args[2]), js_value(args[3]), js_value(args[4]), js_value(args[5]));   \
        js_return(args, ret);                                                                                                                       \
    }
#define FUNCTION_WRAPPER_V8_7(NAME, NAME_EMBED)                                                                                                     \
void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                                                                                          \
    {                                                                                                                                               \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                                                                        \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]), js_value(args[2]), js_value(args[3]), js_value(args[4]),    \
                                                   js_value(args[5]), js_value(args[6]));                                                           \
        js_return(args, ret);                                                                                                                       \
    }
#define FUNCTION_WRAPPER_V8_8(NAME, NAME_EMBED)                                                                                                     \
void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                                                                                          \
    {                                                                                                                                               \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                                                                        \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]), js_value(args[2]), js_value(args[3]), js_value(args[4]),    \
                                                   js_value(args[5]), js_value(args[6]), js_value(args[7]));                                        \
        js_return(args, ret);                                                                                                                       \
    }
#define FUNCTION_WRAPPER_V8_9(NAME, NAME_EMBED)                                                                                                     \
void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                                                                                          \
    {                                                                                                                                               \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                                                                        \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]), js_value(args[2]), js_value(args[3]), js_value(args[4]),    \
                                                   js_value(args[5]), js_value(args[6]), js_value(args[7]), js_value(args[8]));                     \
        js_return(args, ret);                                                                                                                       \
    }
#define FUNCTION_WRAPPER_V8_10(NAME, NAME_EMBED)                                                                                                    \
    void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                                                                                      \
    {                                                                                                                                               \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                                                                        \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]), js_value(args[2]), js_value(args[3]), js_value(args[4]), js_value(args[5]), \
                                                  js_value(args[6]), js_value(args[7]), js_value(args[8]), js_value(args[9]));   \
        js_return(args, ret);                                                                                                                       \
    }
#define FUNCTION_WRAPPER_V8_13(NAME, NAME_EMBED)                                                                                                    \
    void NAME(const v8::FunctionCallbackInfo<v8::Value>& args)                                                                                      \
    {                                                                                                                                               \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(args.Holder());                                                                        \
        JSSmart<CJSValue> ret = _this->NAME_EMBED(js_value(args[0]), js_value(args[1]), js_value(args[2]), js_value(args[3]), js_value(args[4]), js_value(args[5]), \
                                                  js_value(args[6]), js_value(args[7]), js_value(args[8]), js_value(args[9]), js_value(args[10]), js_value(args[11]), \
                                                  js_value(args[12]));                                                                              \
        js_return(args, ret);                                                                                                                       \
    }

#endif // _BUILD_NATIVE_CONTROL_V8_BASE_H_
