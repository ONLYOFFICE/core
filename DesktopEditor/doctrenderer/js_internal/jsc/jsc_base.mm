#import "jsc_base.h"

using namespace NSJSBase;

JSContext* NSJSBase::CJSContextPrivate::g_lockedContext = nil;

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
    return true; // TODO!!!
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

    void CJSContext::Initialize(bool bIsGlobal)
    {
        m_internal->context = [[JSContext alloc] init];
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

    CJSValue* CJSContext::JSON_Parse(const char *sTmp)
    {
        // TODO:
        return CJSContext::createUndefined();
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
#if 1
        NSLog(@"%@", pExсeption);
#endif
        return true;
    }
}
