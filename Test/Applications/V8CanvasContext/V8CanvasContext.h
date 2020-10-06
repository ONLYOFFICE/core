#ifndef V8CANVASCONTEXT_H
#define V8CANVASCONTEXT_H

#ifndef V8CANVASCONTEXT_USE_DYNAMIC_LIBRARY
#define V8CANVASCONTEXT_DECL_EXPORT
#else
#include "../../../DesktopEditor/common/base_export.h"
#define V8CANVASCONTEXT_DECL_EXPORT Q_DECL_EXPORT
#endif

class V8CANVASCONTEXT_DECL_EXPORT CV8CanvasContext
{
public:
    CV8CanvasContext();
};

#endif // V8CANVASCONTEXT_H
