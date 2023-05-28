#include "Default.h"

#include "./ZipEmbed.h"
//#include "./TextMeasurerEmbed.h"
//#include "./MemoryStreamEmbed.h"
#include "./HashEmbed.h"

namespace NSJSBase
{
	void CreateDefaults(JSSmart<CJSContext>& context)
	{
		context->Embed<CZipEmbed>();
//		CTextMeasurerEmbed::CreateObjectInContext("CreateNativeTextMeasurer", context);
//		CMemoryStreamEmbed::CreateObjectInContext("CreateNativeMemoryStream", context);
		context->Embed<CHashEmbed>();
	}
}
