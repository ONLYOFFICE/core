#include "../PointerEmbed.h"
#include "../../js_internal/v8/v8_base.h"

v8::Handle<v8::ObjectTemplate> CreatePointerTemplate(v8::Isolate* isolate)
{
    v8::EscapableHandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    return handle_scope.Escape(result);
}

JSSmart<CJSValue> CPointerEmbedObject::createObject()
{
    v8::Isolate* isolate = CV8Worker::GetCurrent();
    //v8::HandleScope scope(isolate);

    v8::Handle<v8::ObjectTemplate> pointerTemplate = v8::ObjectTemplate::New(isolate);
    pointerTemplate->SetInternalFieldCount(1);

    v8::Local<v8::Object> obj = pointerTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetInternalField(0, v8::External::New(CV8Worker::GetCurrent(), this));

    NSJSBase::CJSEmbedObjectPrivate::CreateWeaker(obj);

    CJSValueV8* returnValue = new CJSValueV8();
    returnValue->value = obj;

    JSSmart<CJSValue> ret = returnValue;
    return ret;
}
