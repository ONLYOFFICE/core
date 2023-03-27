#include "../MemoryStreamEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSMemoryStream
{
    #define CURRENTWRAPPER CMemoryStreamEmbed

    FUNCTION_WRAPPER_V8_1(_ms_write_byte, WriteByte)
    FUNCTION_WRAPPER_V8_1(_ms_write_bool, WriteBool)
    FUNCTION_WRAPPER_V8_1(_ms_write_long, WriteLong)
    FUNCTION_WRAPPER_V8_1(_ms_write_double, WriteDouble)
    FUNCTION_WRAPPER_V8_1(_ms_write_double2, WriteDouble2)
    FUNCTION_WRAPPER_V8_1(_ms_writestringA, WriteStringA)
    FUNCTION_WRAPPER_V8_1(_ms_writestring1, WriteString)
    FUNCTION_WRAPPER_V8_1(_ms_writestring2, WriteString2)
    FUNCTION_WRAPPER_V8_3(_ms_copy, Copy)
    FUNCTION_WRAPPER_V8(_ms_clearnoattack, ClearNoAttack)

    void _ms_pos(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& args)
    {
        CMemoryStreamEmbed* _this = (CMemoryStreamEmbed*)unwrap_native(args.This());
        args.GetReturnValue().Set(v8::Integer::New(CV8Worker::GetCurrent(), _this->m_pInternal->GetSize()));
    }

    v8::Handle<v8::ObjectTemplate> CreateMemoryStreamTemplate(v8::Isolate* isolate)
    {
        v8::EscapableHandleScope handle_scope(isolate);

        v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(V8IsolateOneArg);
        result->SetInternalFieldCount(1);

        // property
        result->SetAccessor(CreateV8String(CV8Worker::GetCurrent(), "pos"), _ms_pos);

        NSV8Objects::Template_Set(result, "Copy",			_ms_copy);
        NSV8Objects::Template_Set(result, "ClearNoAttack",  _ms_clearnoattack);
        NSV8Objects::Template_Set(result, "WriteByte",		_ms_write_byte);
        NSV8Objects::Template_Set(result, "WriteBool",		_ms_write_bool);
        NSV8Objects::Template_Set(result, "WriteLong",		_ms_write_long);
        NSV8Objects::Template_Set(result, "WriteDouble",	_ms_write_double);
        NSV8Objects::Template_Set(result, "WriteDouble2",	_ms_write_double2);
        NSV8Objects::Template_Set(result, "WriteStringA",	_ms_writestringA);
        NSV8Objects::Template_Set(result, "WriteString",	_ms_writestring1);
        NSV8Objects::Template_Set(result, "WriteString2",	_ms_writestring2);

        return handle_scope.Escape(result);
    }

    void CreateNativeMemoryStream(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        v8::Handle<v8::ObjectTemplate> MemoryObjectTemplate = NSMemoryStream::CreateMemoryStreamTemplate(isolate);
        CMemoryStreamEmbed* pMemoryObject = new CMemoryStreamEmbed();

        v8::Local<v8::Object> obj = MemoryObjectTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
        obj->SetInternalField(0, v8::External::New(CV8Worker::GetCurrent(), pMemoryObject));

        NSJSBase::CJSEmbedObjectPrivate::CreateWeaker(obj);

        args.GetReturnValue().Set(obj);
    }
  }

void CMemoryStreamEmbed::CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context)
{
    v8::Isolate* current = CV8Worker::GetCurrent();
    context->m_internal->m_global->Set(current, name.c_str(), v8::FunctionTemplate::New(current, NSMemoryStream::CreateNativeMemoryStream));
}
