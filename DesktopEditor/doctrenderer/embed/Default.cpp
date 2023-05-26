#include "Default.h"

//#include "./ZipEmbed.h"
//#include "./TextMeasurerEmbed.h"
//#include "./MemoryStreamEmbed.h"
#include "./HashEmbed.h"

namespace NSJSBase
{
	void CreateDefaults(JSSmart<CJSContext>& context)
	{
//		CZipEmbed::CreateObjectInContext("CreateNativeZip", context);
//		CTextMeasurerEmbed::CreateObjectInContext("CreateNativeTextMeasurer", context);
//		CMemoryStreamEmbed::CreateObjectInContext("CreateNativeMemoryStream", context);
		context->Embed<CHashEmbed>();
	}
}
