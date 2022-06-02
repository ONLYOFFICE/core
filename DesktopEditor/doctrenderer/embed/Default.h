#ifndef _BUILD_NATIVE_DEFAULT_EMBED_H_
#define _BUILD_NATIVE_DEFAULT_EMBED_H_

#include "./GraphicsEmbed.h"
#include "./ZipEmbed.h"
#include "./TextMeasurerEmbed.h"

namespace NSJSBase
{
    static void CreateDefaults(JSSmart<CJSContext>& context)
    {
        CGraphicsEmbed::CreateObjectInContext("CreateNativeGraphics", context);
        CZipEmbed::CreateObjectInContext("CreateNativeZip", context);
        CTextMeasurerEmbed::CreateObjectInContext("CreateNativeTextMeasurer", context);
    }
}

#endif // _BUILD_NATIVE_DEFAULT_EMBED_H_
