#import "jsc_base.h"
#include <iostream>

using namespace NSJSBase;

JSContext* NSJSBase::CJSContextPrivate::g_lockedContext = nil;
bool NSJSBase::CJSContextPrivate::g_oldVersion = false;

template<typename T>
bool CJSValueJSCTemplate<T>::isUndefined()
{
    return (value == nil) ? true : ([value isUndefined] == YES);
}
template<typename T>
bool CJSValueJSCTemplate<T>::isNull()
{
    return (value == nil) ? false : ([value isNull] == YES);
}
template<typename T>
bool CJSValueJSCTemplate<T>::isBool()
{
    return (value == nil) ? false : ([value isBoolean] == YES);
}
template<typename T>
bool CJSValueJSCTemplate<T>::isNumber()
{
    return (value == nil) ? false : ([value isNumber] == YES);
}
template<typename T>
bool CJSValueJSCTemplate<T>::isString()
{
    return (value == nil) ? false : ([value isString] == YES);
}
template<typename T>
bool CJSValueJSCTemplate<T>::isArray()
{
    return (value == nil) ? false : JSValueIsArray(context.JSGlobalContextRef, value.JSValueRef);
}
template<typename T>
bool CJSValueJSCTemplate<T>::isTypedArray()
{
    return (value == nil) ? false : (kJSTypedArrayTypeNone == JSValueGetTypedArrayType(context.JSGlobalContextRef, value.JSValueRef, NULL));
}
template<typename T>
bool CJSValueJSCTemplate<T>::isObject()
{
    return (value == nil) ? false : ([value isObject] == YES);
}
template<typename T>
bool CJSValueJSCTemplate<T>::isFunction()
{
    return isObject();
}
template<typename T>
bool CJSValueJSCTemplate<T>::isEmpty()
{
    return (value == nil) ? true : false;
}
template<typename T>
void CJSValueJSCTemplate<T>::doUndefined()
{
    value = [JSValue valueWithUndefinedInContext:context];
}
template<typename T>
void CJSValueJSCTemplate<T>::doNull()
{
    value = [JSValue valueWithNullInContext:context];
}
template<typename T>
bool CJSValueJSCTemplate<T>::toBool()
{
    return ([value toBool] == YES) ? true : false;
}
template<typename T>
int CJSValueJSCTemplate<T>::toInt32()
{
    return [value toInt32];
}
template<typename T>
double CJSValueJSCTemplate<T>::toDouble()
{
    return [value toDouble];
}
template<typename T>
std::string CJSValueJSCTemplate<T>::toStringA()
{
    return [[value toString] stdstring];
}
template<typename T>
std::wstring CJSValueJSCTemplate<T>::toStringW()
{
    return [[value toString] stdwstring];
}

namespace NSJSBase
{
    CJSContext::CJSContext()
    {
        m_internal = new CJSContextPrivate();
    }
    CJSContext::~CJSContext()
    {
        m_internal->context = nil;
        RELEASEOBJECT(m_internal);
    }

    CJSTryCatch* CJSContext::GetExceptions()
    {
        return new CJSCTryCatch();
    }

    void CJSContext::Initialize()
    {
        m_internal->context = [[JSContext alloc] init];

#ifndef _IOS
        if (@available(macOS 10.12, *))
        {
            // none
        }
        else
        {
            CJSContextPrivate::g_oldVersion = true;
            [m_internal->context evaluateScript:@"var Uint8Array=Array;function jsc_toBase64(r){for(var o=[\"A\",\"B\",\"C\",\"D\",\"E\",\"F\",\"G\",\"H\",\"I\",\"J\",\"K\",\"L\",\"M\",\"N\",\"O\",\"P\",\"Q\",\"R\",\"S\",\"T\",\"U\",\"V\",\"W\",\"X\",\"Y\",\"Z\",\"a\",\"b\",\"c\",\"d\",\"e\",\"f\",\"g\",\"h\",\"i\",\"j\",\"k\",\"l\",\"m\",\"n\",\"o\",\"p\",\"q\",\"r\",\"s\",\"t\",\"u\",\"v\",\"w\",\"x\",\"y\",\"z\",\"0\",\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\",\"8\",\"9\",\"+\",\"/\"],a=r.length,f=4*(a/3>>0),n=f/76>>0,t=19,v=0,e=[],i=\"\",s=0;s<=n;s++){s==n&&(t=f%76/4>>0);for(var u=0;u<t;u++){for(var c=0,h=0;h<3;h++)c|=r[0+v++],c<<=8;i=\"\";for(var A=0;A<4;A++){i+=o[c>>>26&255],c<<=6,c&=4294967295}e.push(i)}}if(n=a%3!=0?a%3+1:0){for(c=0,h=0;h<3;h++)h<a%3&&(c|=r[0+v++]),c<<=8;i=\"\";for(A=0;A<n;A++){i+=o[c>>>26&255],c<<=6}t=0!=n?4-n:0;for(u=0;u<t;u++)i+=\"=\";e.push(i)}return e.join(\"\")}function jsc_fromBase64(r,o){for(var a,f=r.length,n=0,t=new Array(void 0===o?f:o),v=t,e=0,i=0;e<f;){for(var s=0,u=0,c=0;c<4&&!(f<=e);c++){var h=65<=(a=r.charCodeAt(e++))&&a<=90?a-65:97<=a&&a<=122?a-71:48<=a&&a<=57?a+4:43==a?62:47==a?63:-1;-1!=h?(s<<=6,s|=h,u+=6):c--}for(s<<=24-u,i=u>>>3,c=0;c<i;c++)v[n++]=(16711680&s)>>>16,s<<=8}return t}\n"];
        }
#endif
    }
    void CJSContext::Dispose()
    {
        m_internal->context = nil;
        m_internal->g_lockedContext = nil;
        NSJSBase::CJSContextPrivate::g_lockedContext = nil;
    }

    void CJSContext::CreateContext()
    {
        // NONE
    }

    void CJSContext::CreateGlobalForContext()
    {
        // NONE
    }

    CJSObject* CJSContext::GetGlobal()
    {
        CJSObjectJSC* ret = new CJSObjectJSC();
        ret->context = m_internal->context;
        ret->value = [m_internal->context globalObject];
        return ret;
    }

    CJSIsolateScope* CJSContext::CreateIsolateScope()
    {
#if 0
        // FOR DEBUG
        [m_internal->context setExceptionHandler:^(JSContext *context, JSValue *value) {
            NSLog(@"%@", value);
        }];
#endif
        
        m_internal->g_lockedContext = m_internal->context;
        return new CJSIsolateScope();
    }

    CJSContextScope* CJSContext::CreateContextScope()
    {
        return new CJSContextScope();
    }

    CJSLocalScope* CJSContext::CreateLocalScope()
    {
        return new CJSLocalScope();
    }

    CJSValue* CJSContext::createUndefined()
    {
        CJSValueJSC* _value = new CJSValueJSC();
        _value->context = _getCurrentContext();
        _value->doUndefined();
        return _value;
    }

    CJSValue* CJSContext::createNull()
    {
        CJSValueJSC* _value = new CJSValueJSC();
        _value->context = _getCurrentContext();
        _value->doNull();
        return _value;
    }

    CJSValue* CJSContext::createBool(const bool& value)
    {
        CJSValueJSC* _value = new CJSValueJSC();
        _value->context = _getCurrentContext();
        _value->value = [JSValue valueWithBool:(value ? YES : NO) inContext:_value->context];
        return _value;
    }

    CJSValue* CJSContext::createInt(const int& value)
    {
        CJSValueJSC* _value = new CJSValueJSC();
        _value->context = _getCurrentContext();
        _value->value = [JSValue valueWithInt32:((int32_t) value) inContext:_value->context];
        return _value;
    }

    CJSValue* CJSContext::createDouble(const double& value)
    {
        CJSValueJSC* _value = new CJSValueJSC();
        _value->context = _getCurrentContext();
        _value->value = [JSValue valueWithDouble:value inContext:_value->context];
        return _value;
    }

    CJSValue* CJSContext::createString(const char* value, const int& len)
    {
        CJSValueJSC* _value = new CJSValueJSC();
        _value->context = _getCurrentContext();
        _value->value = [NSString stringWithUtf8Buffer:value
                                                length:(size_t)len];
        return _value;
    }

    CJSValue* CJSContext::createString(const std::string& value)
    {
        CJSValueJSC* _value = new CJSValueJSC();
        _value->context = _getCurrentContext();
        _value->value = [NSString stringWithAString:value];
        return _value;
    }

    CJSValue* CJSContext::createString(const std::wstring& value)
    {
        CJSValueJSC* _value = new CJSValueJSC();
        _value->context = _getCurrentContext();
        _value->value = [NSString stringWithWString:value];
        return _value;
    }

    CJSObject* CJSContext::createObject()
    {
        CJSObjectJSC* _value = new CJSObjectJSC();
        _value->context = _getCurrentContext();
        _value->value = [JSValue valueWithNewObjectInContext:_value->context];
        return _value;
    }

    CJSArray* CJSContext::createArray(const int& count)
    {
        CJSArrayJSC* _value = new CJSArrayJSC();
        _value->context = _getCurrentContext();
        _value->value = [JSValue valueWithNewArrayInContext:_value->context];
        return _value;
    }

    CJSTypedArray* CJSContext::createUint8Array(BYTE* data, int count)
    {
        JSContext* _current = _getCurrentContext();
        CJSTypedArrayJSC* _value = new CJSTypedArrayJSC(_current, data, count);
        return _value;
    }

    JSSmart<CJSValue> CJSContext::runScript(const std::string& script, JSSmart<CJSTryCatch> exception, const std::wstring& scriptPath)
    {
        CJSValueJSC* _value = new CJSValueJSC();
        _value->value = [m_internal->context evaluateScript:[NSString stringWithAString:script]];
        _value->context = m_internal->context;
        return _value;
    }

    CJSContext* CJSContext::GetCurrent()
    {
        CJSContext* ret = new CJSContext();
        ret->m_internal->context = _getCurrentContext();
        return ret;
    }

    void CJSContext::ExternalInitialize()
    {
    }
    void CJSContext::ExternalDispose()
    {
    }
    bool CJSContext::IsSupportNativeTypedArrays()
    {
        return (CJSContextPrivate::g_oldVersion == false) ? true : false;
    }

    CJSValue* CJSContext::JSON_Parse(const char *sTmp)
    {
        if (!sTmp)
            return CJSContext::createUndefined();

        NSString* sValue = [[NSString alloc] initWithUTF8String:sTmp];
        JSStringRef sValueRef = JSStringCreateWithCFString((__bridge CFStringRef)sValue);
        JSValueRef oValueJSRef = JSValueMakeFromJSONString(m_internal->context.JSGlobalContextRef, sValueRef);

        CJSValueJSC* _value = new CJSValueJSC();
        _value->value = [JSValue valueWithJSValueRef:oValueJSRef inContext: m_internal->context];
        _value->context = m_internal->context;
        return _value;
    }
}

namespace NSJSBase
{
    bool CJSCTryCatch::Check()
    {
        JSValue* exc = [context exception];
        if (exc == nil || [exc isNull] || [exc isUndefined])
            return false;
        
        NSString* pExсeption = [[context exception] toString];
        std::cerr << [pExсeption stdstring] << std::endl;
        NSLog(@"%@", pExсeption);
        return true;
    }
}
