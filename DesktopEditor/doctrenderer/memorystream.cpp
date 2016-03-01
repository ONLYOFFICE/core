#include "memorystream.h"

// wrap_methods -------------
CMemoryStream* unwrap_memorystream(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CMemoryStream*>(field->Value());
}

void _ms_write_byte(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    BYTE arg = (BYTE)args[0]->Int32Value();
    pNative->WriteBYTE(arg);

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_write_bool(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    BYTE arg = (BYTE)args[0]->BooleanValue();
    pNative->WriteBYTE(arg ? 1 : 0);

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_write_long(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    LONG arg = (LONG)args[0]->Int32Value();
    pNative->WriteLONG(arg);

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_write_double(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    double arg = (double)args[0]->NumberValue();
    pNative->WriteLONG((LONG)(arg * 100000));

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_writestring1(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());
    v8::String::Value data(args[0]);
    pNative->WriteString((wchar_t*)*data, data.length());
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_writestring2(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());
    v8::String::Value data(args[0]);
    pNative->WriteString2((wchar_t*)*data, data.length());
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_copy(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    CMemoryStream* pNative2 = unwrap_memorystream(args[0]->ToObject());
    size_t pos = (size_t)args[1]->Uint32Value();
    size_t len = (size_t)args[2]->Uint32Value();

    pNative->Copy(pNative2, pos, len);

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_clearnoattack(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());
    pNative->ClearNoAttack();
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_pos(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CMemoryStream* pNative = unwrap_memorystream(info.Holder());
    info.GetReturnValue().Set(v8::Integer::New(v8::Isolate::GetCurrent(), pNative->GetSize()));
}

v8::Handle<v8::ObjectTemplate> CreateMemoryStreamTemplate(v8::Isolate* isolate)
{
    //v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New();
    result->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

    v8::Isolate* current = v8::Isolate::GetCurrent();

    // property
    result->SetAccessor(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "pos"), _ms_pos); // получить код ошибки

    // прописываем функции - методы объекта
    result->Set(v8::String::NewFromUtf8(current, "Copy"),			v8::FunctionTemplate::New(current, _ms_copy));
    result->Set(v8::String::NewFromUtf8(current, "ClearNoAttack"),	v8::FunctionTemplate::New(current, _ms_clearnoattack));
    result->Set(v8::String::NewFromUtf8(current, "WriteByte"),		v8::FunctionTemplate::New(current, _ms_write_byte));
    result->Set(v8::String::NewFromUtf8(current, "WriteBool"),		v8::FunctionTemplate::New(current, _ms_write_bool));
    result->Set(v8::String::NewFromUtf8(current, "WriteLong"),		v8::FunctionTemplate::New(current, _ms_write_long));
    result->Set(v8::String::NewFromUtf8(current, "WriteDouble"),	v8::FunctionTemplate::New(current, _ms_write_double));
    result->Set(v8::String::NewFromUtf8(current, "WriteString"),	v8::FunctionTemplate::New(current, _ms_writestring1));
    result->Set(v8::String::NewFromUtf8(current, "WriteString2"),	v8::FunctionTemplate::New(current, _ms_writestring2));

    // возвращаем временный хэндл хитрым образом, который переносит наш хэндл в предыдущий HandleScope и не дает ему
    // уничтожиться при уничтожении "нашего" HandleScope - handle_scope

    //return handle_scope.Close(result);
    return result;
}

