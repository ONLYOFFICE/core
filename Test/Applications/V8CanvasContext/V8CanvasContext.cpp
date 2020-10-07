#include <string>
#include <iostream>

#include "V8CanvasContext.h"
#include "../../../Common/3dParty/v8/v8/include/v8.h"
#include "../../../Common/3dParty/v8/v8/include/libplatform/libplatform.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Types.h"

// #include "v8.h"
// #include "libplatform/libplatform.h"

std::string to_string(v8::Isolate* isolate, const v8::Local<v8::Context>& context, const v8::Local<v8::Value>& v)
{
    v8::Local<v8::String> sRes = v->ToString(context).FromMaybe(v8::Local<v8::String>());
    if(sRes.IsEmpty()) return std::string();
    v8::String::Utf8Value data(sRes);
    const char* p = *data;
    if (p == NULL) return std::string();
    return std::string(p);
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

    v8::Isolate::Scope isolate_cope(isolate);
    v8::Locker isolate_locker(isolate);

    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::Context::Scope context_scope(context);

    v8::TryCatch try_catch(isolate);
    v8::Local<v8::String> sSource = v8::String::NewFromUtf8(isolate, sCommands.data());
    v8::Local<v8::Script> oScript = v8::Script::Compile(context, sSource).ToLocalChecked();
    if(try_catch.HasCaught())
    {
        std::cout << to_string(isolate, context, try_catch.Exception()) << std::endl;
        return S_FALSE;
    }
    v8::Local<v8::Value> oRes = oScript->Run(context).ToLocalChecked();
    if(try_catch.HasCaught())
    {
        std::cout << to_string(isolate, context, try_catch.Exception()) << std::endl;
        return S_FALSE;
    }

    std::cout << to_string(isolate, context, oRes) << std::endl;
    return S_OK;
}
