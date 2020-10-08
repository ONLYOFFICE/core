#include <string>
#include <iostream>

#include "V8CanvasContext.h"
#include "../../../Common/3dParty/v8/v8/include/v8.h"
#include "../../../Common/3dParty/v8/v8/include/libplatform/libplatform.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Types.h"

std::string to_string(const v8::Local<v8::Value>& v)
{
    v8::String::Utf8Value data(v);
    const char* p = *data;
    if (p == NULL) return std::string();
    return std::string(p);
}

class CUser
{
private:
    std::string m_sName;
public:
    CUser() {}
    void SetName(const std::string& sName) { m_sName = sName; }
    std::string GetName() { return m_sName; }
};

CUser* unwrap_User(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CUser*>(field->Value());
}

void GetName(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CUser* pUser = unwrap_User(args.This());
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), pUser->GetName().data()));
}

void SetName(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CUser* pUser = unwrap_User(args.This());
    pUser->SetName(to_string(args[0]));
}

void nameget(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CUser* pUser = unwrap_User(info.Holder());
    info.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), pUser->GetName().data()));
}

void nameset(v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CUser* pUser = unwrap_User(info.Holder());
    pUser->SetName(to_string(value));
    info.GetReturnValue().Set(value);
}

v8::Handle<v8::ObjectTemplate> CreateUserTemplate(v8::Isolate* isolate)
{
    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    v8::Isolate* current = v8::Isolate::GetCurrent();

    // свойства
    result->SetAccessor(v8::String::NewFromUtf8(current, "name"), nameget, nameset);

    // методы
    result->Set(current, "GetName", v8::FunctionTemplate::New(current, GetName));
    result->Set(current, "SetName", v8::FunctionTemplate::New(current, SetName));

    return result;
}

void CreateUser(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Handle<v8::ObjectTemplate> oUser = CreateUserTemplate(isolate);
    CUser* pUser = new CUser();

    v8::Local<v8::Object> obj = oUser->NewInstance();
    obj->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), pUser));

    args.GetReturnValue().Set(obj);
}

CV8CanvasContext::CV8CanvasContext()
{
    m_pAllocator = NULL;
    m_platform = v8::platform::CreateDefaultPlatform();

    v8::V8::InitializePlatform(m_platform);
    v8::V8::Initialize();
    v8::V8::InitializeICU();
}

CV8CanvasContext::~CV8CanvasContext()
{
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete m_platform;
    if (m_pAllocator)
        delete m_pAllocator;
}

HRESULT CV8CanvasContext::Run(const std::wstring& sPath)
{
    // Читаем скрипт
    std::string sCommands;
    bool bRet = NSFile::CFileBinary::ReadAllTextUtf8A(sPath, sCommands);
    if (!bRet)
        return S_FALSE;

    // Isolate
    v8::Isolate::CreateParams create_params;
    m_pAllocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    create_params.array_buffer_allocator = m_pAllocator;
    v8::Isolate* isolate = v8::Isolate::New(create_params);

    {
        v8::Isolate::Scope isolate_scope(isolate);
        v8::Locker isolate_locker(isolate);
        v8::HandleScope handle_scope(isolate);

        v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
        global->Set(isolate, "CreateUser", v8::FunctionTemplate::New(isolate, CreateUser));

        v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global);
        v8::Context::Scope context_scope(context);

        v8::TryCatch try_catch(isolate);
        v8::Local<v8::String> sSource = v8::String::NewFromUtf8(isolate, sCommands.data());
        v8::Local<v8::Script> oScript = v8::Script::Compile(context, sSource).ToLocalChecked();
        if(try_catch.HasCaught())
        {
            std::cout << to_string(try_catch.Exception()) << std::endl;
            return S_FALSE;
        }
        v8::Local<v8::Value> oRes = oScript->Run(context).ToLocalChecked();
        if(try_catch.HasCaught())
        {
            std::cout << to_string(try_catch.Exception()) << std::endl;
            return S_FALSE;
        }

        std::cout << to_string(oRes) << std::endl;
    }

    isolate->Dispose();
    return S_OK;
}
