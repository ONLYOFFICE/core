#include <malloc.h>

#include "../../../../GraphicsRenderer.h"

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

WASM_EXPORT void* Graphics_Malloc(unsigned int size)
{
    return ::malloc(size);
}
WASM_EXPORT void  Graphics_Free(void* p)
{
    if (p) ::free(p);
}
WASM_EXPORT void* Graphics_Create()
{
    return new CGraphicsRenderer();
}
WASM_EXPORT void Graphics_Destroy(void* p)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)p;
    delete pGraphics;
}

#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
int main()
{
    return 0;
}
#endif
