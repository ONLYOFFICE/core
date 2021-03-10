#ifndef _BUILD_NATIVE_CONTROL_JSC_BASE_H_
#define _BUILD_NATIVE_CONTROL_JSC_BASE_H_

#include "../js_base.h"

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#include <JavaScriptCore/JSTypedArray.h>
#include <JavaScriptCore/JSValueRef.h>

#import "../../../../DesktopEditor/common/Mac/NSString+StringUtils.h"

@protocol JSEmbedObjectProtocol
- (void*) getNative;
@end

namespace NSJSBase
{
    class CJSContextPrivate
    {
    public:
        JSContext* context;

        static bool g_oldVersion;
        static JSContext* g_lockedContext;

    public:
        CJSContextPrivate()
        {
            context = nil;
        }
        ~CJSContextPrivate()
        {
            context = nil;
        }
    };
}

static JSContext* _getCurrentContext()
{
    return NSJSBase::CJSContextPrivate::g_lockedContext;
}

namespace NSJSBase
{
    template<typename T>
    class CJSValueJSCTemplate : public T
    {
    public:
        JSValue* value;
        JSContext* context;

        CJSValueJSCTemplate()
        {
            value = nil;
            context = nil;
        }
        CJSValueJSCTemplate(JSValue* _value)
        {
            value = _value;
            context = _getCurrentContext();
            
            if (context == nil) {
                context = _value.context;
            }
        }
        
    public:

        virtual ~CJSValueJSCTemplate()
        {
            value = nil;
            context = nil;
        }

        virtual bool isUndefined();
        virtual bool isNull();
        virtual bool isBool();
        virtual bool isNumber();
        virtual bool isString();
        virtual bool isArray();
        virtual bool isTypedArray();
        virtual bool isObject();
        virtual bool isFunction();
        virtual bool isEmpty();

        virtual void doUndefined();
        virtual void doNull();
        virtual bool toBool();
        virtual int toInt32();
        virtual double toDouble();
        virtual std::string toStringA();
        virtual std::wstring toStringW();

        virtual CJSObject* toObject();
        virtual CJSArray* toArray();
        virtual CJSTypedArray* toTypedArray();
        virtual CJSFunction* toFunction();
    };

    typedef CJSValueJSCTemplate<CJSValue> CJSValueJSC;

    class CJSObjectJSC : public CJSValueJSCTemplate<CJSObject>
    {
    public:
        CJSObjectJSC()
        {
        }
        CJSObjectJSC(JSValue* _value) : CJSValueJSCTemplate<CJSObject>(_value)
        {
        }

        virtual ~CJSObjectJSC()
        {
            value = nil;
            context = nil;
        }

        virtual CJSValue* get(const char* name)
        {
            CJSValueJSC* _value = new CJSValueJSC();
            _value->value = [value valueForProperty:[[NSString alloc] initWithUTF8String:name]];
            _value->context = context;
            return _value;
        }

        virtual void set(const char* name, CJSValue* value_param)
        {
            CJSValueJSC* _value = static_cast<CJSValueJSC*>(value_param);
            [value setValue:_value->value forProperty:[[NSString alloc] initWithUTF8String:name]];
        }

        virtual void set(const char* name, const int& _value)
        {
            [value setValue:[JSValue valueWithInt32:_value inContext: context] forProperty:[[NSString alloc] initWithUTF8String:name]];
        }

        virtual void set(const char* name, const double& _value)
        {
            [value setValue:[JSValue valueWithDouble:_value inContext: context] forProperty:[[NSString alloc] initWithUTF8String:name]];
        }

        virtual CJSEmbedObject* getNative()
        {
            id _wrapper = [value toObject];
            if ([_wrapper conformsToProtocol:@protocol(JSEmbedObjectProtocol)])
            {
                return (CJSEmbedObject*)([_wrapper getNative]);
            }
            return NULL;
        }

        virtual JSSmart<CJSValue> call_func(const char* name, const int argc = 0, JSSmart<CJSValue> argv[] = NULL)
        {
            NSMutableArray* arr = nil;

            if (argc > 0)
            {
                arr = [[NSMutableArray alloc] init];
                for (int i = 0; i < argc; ++i)
                {
                    CJSValueJSC* _val = (CJSValueJSC*)argv[i].operator ->();
                    [arr addObject:_val->value];
                }
            }

            CJSValueJSC* _return = new CJSValueJSC();
            _return->context = context;
            _return->value = [value invokeMethod: [[NSString alloc] initWithUTF8String:name]
                                    withArguments: arr];

            return _return;
        }

        virtual JSSmart<CJSValue> toValue()
        {
            CJSValueJSC* _value = new CJSValueJSC();
            _value->value = value;
            //_value->value = [JSValue valueWithJSValueRef:[value JSValueRef] inContext:context];
            _value->context = context;
            return _value;
        }
    };

    class CJSArrayJSC : public CJSValueJSCTemplate<CJSArray>
    {
    public:
        int m_count;
    public:
        CJSArrayJSC()
        {
            m_count = 0;
        }
        virtual ~CJSArrayJSC()
        {
            value = nil;
            context = nil;
        }

        virtual int getCount()
        {
            JSValue* _ret = [value valueForProperty:@"length"];
            if (nil != _ret && NO == [_ret isUndefined])
                return [_ret toInt32];
            return 0;
        }

        virtual JSSmart<CJSValue> get(const int& index)
        {
            CJSValueJSC* _value = new CJSValueJSC();
            _value->value = [value valueAtIndex:(NSUInteger)index];
            _value->context = context;
            return _value;
        }

        virtual void set(const int& index, CJSValue* value_param)
        {
            CJSValueJSC* _value = static_cast<CJSValueJSC*>(value_param);
            [value setValue:_value->value atIndex:index];
        }

        virtual void add(CJSValue* value_param)
        {
            set(getCount(), value_param);
        }

        virtual void set(const int& index, const bool& _value)
        {
            [value setValue:[JSValue valueWithBool:_value inContext : context] atIndex:index];
        }

        virtual void set(const int& index, const int& _value)
        {
            [value setValue:[JSValue valueWithInt32:_value inContext : context] atIndex:index];
        }

        virtual void set(const int& index, const double& _value)
        {
            [value setValue:[JSValue valueWithDouble:_value inContext : context] atIndex:index];
        }

        virtual void add_null()
        {
            [value setValue:[JSValue valueWithNullInContext:context] atIndex:m_count++];
        }

        virtual void add_undefined()
        {
            [value setValue:nil atIndex:m_count++];
        }

        virtual void add_bool(const bool& _value)
        {
            [value setValue:[JSValue valueWithBool:_value inContext : context] atIndex:m_count++];
        }

        virtual void add_byte(const BYTE& _value)
        {
            [value setValue:[JSValue valueWithInt32:(int)_value inContext : context] atIndex:m_count++];
        }

        virtual void add_int(const int& _value)
        {
            [value setValue:[JSValue valueWithInt32:_value inContext : context] atIndex:m_count++];
        }

        virtual void add_double(const double& _value)
        {
            [value setValue:[JSValue valueWithDouble:_value inContext : context] atIndex:m_count++];
        }

        virtual void add_stringa(const std::string& _value)
        {
            [value setValue:[NSString stringWithAString:_value] atIndex:m_count++];
        }

        virtual void add_string(const std::wstring& _value)
        {
            [value setValue:[NSString stringWithWString:_value] atIndex:m_count++];
        }

        virtual JSSmart<CJSValue> toValue()
        {
            CJSValueJSC* _value = new CJSValueJSC();
            _value->value = value;
            _value->context = context;
            return _value;
        }
    };

    class CJSTypedArrayJSC : public CJSValueJSCTemplate<CJSTypedArray>
    {
    private:
        BYTE* data_old_version;
    public:
        CJSTypedArrayJSC(JSContext* _context, BYTE* data = NULL, int count = 0)
        {
            data_old_version = NULL;
            context = _context;
            if (0 >= count)
                return;
            if (!CJSContextPrivate::g_oldVersion)
            {
                JSObjectRef object = JSObjectMakeTypedArrayWithBytesNoCopy(context.JSGlobalContextRef,
                                                                           kJSTypedArrayTypeUint8Array,
                                                                           (void*)data, (size_t)count,
                                                                           data_destroy, nullptr, nullptr);
                if (object)
                {
                    value = [JSValue valueWithJSValueRef:object inContext:context];
                }
            }
            else
            {
                char* pDst = NULL;
                int nDstLen = 0;
                NSFile::CBase64Converter::Encode(data, count, pDst, nDstLen, NSBase64::B64_BASE64_FLAG_NOCRLF);

                std::string sCode = "jsc_fromBase64(\"" + std::string(pDst, nDstLen) + "\", " + std::to_string(count) + ");";
                RELEASEARRAYOBJECTS(pDst);
                value = [_context evaluateScript:[NSString stringWithAString:sCode]];
            }
        }
        virtual ~CJSTypedArrayJSC()
        {
            RELEASEARRAYOBJECTS(data_old_version);
            value = nil;
            context = nil;
        }

        static void data_destroy(void* bytes, void* deallocatorContext)
        {
            free(bytes);
        }

        virtual int getCount()
        {
            if (nil == value)
                return 0;
            if (!CJSContextPrivate::g_oldVersion)
            {
                JSObjectRef obj = JSValueToObject(context.JSGlobalContextRef, value.JSValueRef, NULL);
                return (int)JSObjectGetTypedArrayByteLength(context.JSGlobalContextRef, obj, NULL);
            }
            JSValue* _ret = [value valueForProperty:@"length"];
            if (nil != _ret && NO == [_ret isUndefined])
                return [_ret toInt32];
            return 0;
        }

        virtual const BYTE* getData()
        {
            if (!CJSContextPrivate::g_oldVersion)
            {
                JSObjectRef obj = JSValueToObject(context.JSGlobalContextRef, value.JSValueRef, NULL);
                return (BYTE*)JSObjectGetTypedArrayBytesPtr(context.JSGlobalContextRef, obj, NULL);
            }

            NSMutableArray* arr = [[NSMutableArray alloc] init];
            [arr addObject:value];

            JSValue* dataBase64 = [context[@"jsc_toBase64"] callWithArguments:arr];
            std::string sBase64Data = [[dataBase64 toString] stdstring];

            RELEASEARRAYOBJECTS(data_old_version);
            int nDstLen = 0;
            NSFile::CBase64Converter::Decode(sBase64Data.c_str(), (int)sBase64Data.length(), data_old_version, nDstLen);
            return data_old_version;
        }

        virtual JSSmart<CJSValue> toValue()
        {
            CJSValueJSC* _value = new CJSValueJSC();
            _value->value = value;
            _value->context = context;
            return _value;
        }
    };

    class CJSFunctionJSC : public CJSValueJSCTemplate<CJSFunction>
    {
    public:
        CJSFunctionJSC()
        {
        }
        virtual ~CJSFunctionJSC()
        {
            value = nil;
            context = nil;
        }
        
        virtual CJSValue* Call(CJSValue* recv, int argc, JSSmart<CJSValue> argv[])
        {
            NSMutableArray* arr = [[NSMutableArray alloc] init];
            for (int i = 0; i < argc; ++i)
            {
                CJSValueJSC* _val = (CJSValueJSC*)argv[i].operator ->();
                [arr addObject:_val->value];
            }

            CJSValueJSC* _return = new CJSValueJSC();
            _return->context = context;
            _return->value = [value callWithArguments:arr];

            return _return;
        }
    };

    template<typename T>
    CJSObject* CJSValueJSCTemplate<T>::toObject()
    {
        CJSObjectJSC* _value = new CJSObjectJSC();
        _value->value = value;
        _value->context = context;
        return _value;
    }

    template<typename T>
    CJSArray* CJSValueJSCTemplate<T>::toArray()
    {
        CJSArrayJSC* _value = new CJSArrayJSC();
        _value->value = value;
        _value->context = context;
        return _value;
    }

    template<typename T>
    CJSTypedArray* CJSValueJSCTemplate<T>::toTypedArray()
    {
        CJSTypedArrayJSC* _value = new CJSTypedArrayJSC(context);
        _value->value = value;
        _value->context = context;
        return _value;
    }

    template<typename T>
    CJSFunction* CJSValueJSCTemplate<T>::toFunction()
    {
        CJSFunctionJSC* _value = new CJSFunctionJSC();
        _value->value = value;
        _value->context = context;
        return _value;
    }
}

namespace NSJSBase
{
    // TRY - CATCH
    class CJSCTryCatch : public CJSTryCatch
    {
    public:
        JSContext* context;
        JSSmart<CJSContext> smartContext;

    public:
        CJSCTryCatch() : CJSTryCatch()
        {
            smartContext = CJSContext::GetCurrent();
            context = smartContext->m_internal->context;
        }
        virtual ~CJSCTryCatch()
        {
            context = nil;
        }

    public:
        virtual bool Check();
    };
}

inline JSSmart<NSJSBase::CJSValue> js_value(JSValue* _value)
{
    return new NSJSBase::CJSValueJSC(_value);
}
inline JSSmart<NSJSBase::CJSValue> js_object(JSValue* _value)
{
    return new NSJSBase::CJSObjectJSC(_value);
}
inline JSValue* js_return(JSSmart<NSJSBase::CJSValue> _value)
{
    if (!_value.is_init())
        return nil;
    NSJSBase::CJSValueJSC* _tmp = (NSJSBase::CJSValueJSC*)(_value.operator ->());
    return _tmp->value;
}

#define FUNCTION_WRAPPER_JS(NAME, NAME_EMBED)                                       \
    -(JSValue*) NAME                                                                \
    {                                                                               \
        return js_return(m_internal->NAME_EMBED());                                 \
    }

#define FUNCTION_WRAPPER_JS_1(NAME, NAME_EMBED)                                     \
    -(JSValue*) NAME:(JSValue*)p1                                                   \
    {                                                                               \
        return js_return(m_internal->NAME_EMBED(js_value(p1)));                     \
    }
#define FUNCTION_WRAPPER_JS_2(NAME, NAME_EMBED)                                     \
    -(JSValue*) NAME:(JSValue*)p1 : (JSValue*)p2                                    \
    {                                                                               \
        return js_return(m_internal->NAME_EMBED(js_value(p1), js_value(p2)));       \
    }
#define FUNCTION_WRAPPER_JS_3(NAME, NAME_EMBED)                                             \
    -(JSValue*) NAME:(JSValue*)p1 : (JSValue*)p2 : (JSValue*)p3                             \
    {                                                                                       \
        return js_return(m_internal->NAME_EMBED(js_value(p1), js_value(p2), js_value(p3))); \
    }
#define FUNCTION_WRAPPER_JS_4(NAME, NAME_EMBED)                                                             \
    -(JSValue*) NAME:(JSValue*)p1 : (JSValue*)p2 : (JSValue*)p3 : (JSValue*)p4                              \
    {                                                                                                       \
        return js_return(m_internal->NAME_EMBED(js_value(p1), js_value(p2), js_value(p3), js_value(p4)));   \
    }
#define FUNCTION_WRAPPER_JS_5(NAME, NAME_EMBED)                                                                             \
    -(JSValue*) NAME:(JSValue*)p1 : (JSValue*)p2 : (JSValue*)p3 : (JSValue*)p4 : (JSValue*)p5                               \
    {                                                                                                                       \
        return js_return(m_internal->NAME_EMBED(js_value(p1), js_value(p2), js_value(p3), js_value(p4), js_value(p5)));     \
    }
#define FUNCTION_WRAPPER_JS_6(NAME, NAME_EMBED)                                                                                         \
    -(JSValue*) NAME:(JSValue*)p1 : (JSValue*)p2 : (JSValue*)p3 : (JSValue*)p4 : (JSValue*)p5 : (JSValue*)p6                            \
    {                                                                                                                                   \
        return js_return(m_internal->NAME_EMBED(js_value(p1), js_value(p2), js_value(p3), js_value(p4), js_value(p5), js_value(p6)));   \
    }
#define FUNCTION_WRAPPER_JS_7(NAME, NAME_EMBED)                                                                                                         \
    -(JSValue*) NAME:(JSValue*)p1 : (JSValue*)p2 : (JSValue*)p3 : (JSValue*)p4 : (JSValue*)p5 : (JSValue*)p6 : (JSValue*)p7                             \
    {                                                                                                                                                   \
        return js_return(m_internal->NAME_EMBED(js_value(p1), js_value(p2), js_value(p3), js_value(p4), js_value(p5), js_value(p6), js_value(p7)));     \
    }
#define FUNCTION_WRAPPER_JS_8(NAME, NAME_EMBED)                                                                                                         \
    -(JSValue*) NAME:(JSValue*)p1 : (JSValue*)p2 : (JSValue*)p3 : (JSValue*)p4 : (JSValue*)p5 : (JSValue*)p6 : (JSValue*)p7 : (JSValue*)p8              \
    {                                                                                                                                                   \
        return js_return(m_internal->NAME_EMBED(js_value(p1), js_value(p2), js_value(p3), js_value(p4), js_value(p5), js_value(p6), js_value(p7), js_value(p8)));     \
    }

#if __has_feature(objc_arc)
#define EMBED_OBJECT_WRAPPER_METHODS(CLASS)     \
-(id) init                                      \
{                                               \
    self = [super init];                        \
    if (self)                                   \
        m_internal = new CLASS();               \
    return self;                                \
}                                               \
-(void) dealloc                                 \
{                                               \
    RELEASEOBJECT(m_internal);                  \
}                                               \
- (void*) getNative                             \
{                                               \
    return m_internal;                          \
}
#else
#define EMBED_OBJECT_WRAPPER_METHODS(CLASS)     \
-(id) init                                      \
{                                               \
    self = [super init];                        \
    if (self)                                   \
        m_internal = new CLASS();               \
    return self;                                \
}                                               \
-(void) dealloc                                 \
{                                               \
    RELEASEOBJECT(m_internal);                  \
    [super dealloc];                            \
}                                               \
- (void*) getNative                             \
{                                               \
    return m_internal;                          \
}
#endif

#endif // _BUILD_NATIVE_CONTROL_JSC_BASE_H_
