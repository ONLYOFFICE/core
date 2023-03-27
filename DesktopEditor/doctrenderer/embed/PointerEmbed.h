#ifndef _BUILD_NATIVE_POINTER_EMBED_H_
#define _BUILD_NATIVE_POINTER_EMBED_H_

#include "../js_internal/js_base.h"

namespace NSPointerObjectDeleters
{
    using PointerObjectDeleter = void (*)(void* data);

    void EmptyDeleter(void* data);
    void FreeDeleter(void* data);
}

#define POINTER_DELETER(CLASS_NAME, NAME) void NAME(void* data) { CLASS_NAME* p = (CLASS_NAME*)data; delete data; }

using namespace NSJSBase;
class CPointerEmbedObject : public CJSEmbedObject
{
public:
    void* Data;

private:
    NSPointerObjectDeleters::PointerObjectDeleter Deleter;

public:
    CPointerEmbedObject(void* data, NSPointerObjectDeleters::PointerObjectDeleter = nullptr);
    void Free();
    virtual ~CPointerEmbedObject();

    virtual void* getObject() { return Data; }

    JSSmart<CJSValue> createObject();
};

#endif // _BUILD_NATIVE_POINTER_EMBED_H_
