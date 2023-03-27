#include "PointerEmbed.h"
#include <memory>

namespace NSPointerObjectDeleters
{
    void EmptyDeleter(void* data)
    {
        // nothing
    }
    void FreeDeleter(void* data)
    {
        free(data);
    }
}

CPointerEmbedObject::CPointerEmbedObject(void* data, NSPointerObjectDeleters::PointerObjectDeleter deleter)
{
    Data = data;
    Deleter = deleter;
}

void CPointerEmbedObject::Free()
{
    if (Data && Deleter)
        Deleter(Data);
    Data = nullptr;
}

CPointerEmbedObject::~CPointerEmbedObject()
{
    Free();
}
