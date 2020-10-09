#include "graphics_wrapper.h"

double to_double(const v8::Local<v8::Value>& v)
{
    return v->ToNumber(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Number>())->Value();
}
bool   to_bool  (const v8::Local<v8::Value>& v)
{
    return v->ToBoolean(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Boolean>())->Value();
}
int    to_int   (const v8::Local<v8::Value>& v)
{
    return v->ToInt32(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Int32>())->Value();
}

CJSGraphics* unwrap_Graphics(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CJSGraphics*>(field->Value());
}

void init_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->init(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]));
}
void EndDraw_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->EndDraw();
}
void put_GlobalAlpha_w  (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->put_GlobalAlpha(to_bool(args[0]), to_int(args[1]));
}
void Start_GlobalAlpha_w(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->Start_GlobalAlpha();
}
void End_GlobalAlpha_w  (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->End_GlobalAlpha();
}
void p_color_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->p_color(to_int(args[0]), to_int(args[1]), to_int(args[2]), to_int(args[3]));
}
void p_width_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->p_width(to_int(args[0]));
}
void p_dash_w           (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->p_dash(args[0]);
}

void m_dDpiX_get_w    (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->m_dDpiX_get()));
}
void globalAlpha_get_w(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Int32::New(v8::Isolate::GetCurrent(), pGraphics->globalAlpha_get()));
}

v8::Handle<v8::ObjectTemplate> CreateGraphicsTemplate(v8::Isolate* isolate)
{
    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    v8::Isolate* current = v8::Isolate::GetCurrent();

    // свойства
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_dDpiX"), m_dDpiX_get_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "globalAlpha"), globalAlpha_get_w);

    // методы
    result->Set(current, "init",              v8::FunctionTemplate::New(current, init_w));
    result->Set(current, "EndDraw",           v8::FunctionTemplate::New(current, EndDraw_w));
    result->Set(current, "put_GlobalAlpha",   v8::FunctionTemplate::New(current, put_GlobalAlpha_w));
    result->Set(current, "Start_GlobalAlpha", v8::FunctionTemplate::New(current, Start_GlobalAlpha_w));
    result->Set(current, "End_GlobalAlpha",   v8::FunctionTemplate::New(current, End_GlobalAlpha_w));
    result->Set(current, "p_color",           v8::FunctionTemplate::New(current, p_color_w));
    result->Set(current, "p_width",           v8::FunctionTemplate::New(current, p_width_w));
    result->Set(current, "p_dash",            v8::FunctionTemplate::New(current, p_dash_w));

    return result;
}
void CreateGraphics(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Handle<v8::ObjectTemplate> oUser = CreateGraphicsTemplate(isolate);
    CJSGraphics* pGraphics = new CJSGraphics();

    v8::Local<v8::Object> obj = oUser->NewInstance();
    obj->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), pGraphics));

    args.GetReturnValue().Set(obj);
}
