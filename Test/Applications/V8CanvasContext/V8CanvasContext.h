#ifndef V8CANVASCONTEXT_H
#define V8CANVASCONTEXT_H

#include <string>

#include "../../../DesktopEditor/common/Types.h"
#include "../../../Common/3dParty/v8/v8/include/v8.h"
#include "../../../Common/3dParty/v8/v8/include/libplatform/libplatform.h"

#ifndef V8CANVASCONTEXT_USE_DYNAMIC_LIBRARY
#define V8CANVASCONTEXT_DECL_EXPORT
#else
#include "../../../DesktopEditor/common/base_export.h"
#define V8CANVASCONTEXT_DECL_EXPORT Q_DECL_EXPORT
#endif

class V8CANVASCONTEXT_DECL_EXPORT CV8CanvasContext
{
private:
    v8::ArrayBuffer::Allocator* m_pAllocator;
    v8::Platform* m_platform;

public:
    CV8CanvasContext();
    ~CV8CanvasContext();

    HRESULT Run(const std::wstring& sPath);
};

#endif // V8CANVASCONTEXT_H
