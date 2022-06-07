#ifndef _BUILD_NATIVE_DEFAULT_EMBED_H_
#define _BUILD_NATIVE_DEFAULT_EMBED_H_

#include "./ZipEmbed.h"
#include "./TextMeasurerEmbed.h"
#include "./MemoryStreamEmbed.h"

namespace NSJSBase
{
    static void CreateDefaults(JSSmart<CJSContext>& context)
    {
        CZipEmbed::CreateObjectInContext("CreateNativeZip", context);
        CTextMeasurerEmbed::CreateObjectInContext("CreateNativeTextMeasurer", context);
        CMemoryStreamEmbed::CreateObjectInContext("CreateNativeMemoryStream", context);
    }
}

#endif // _BUILD_NATIVE_DEFAULT_EMBED_H_
