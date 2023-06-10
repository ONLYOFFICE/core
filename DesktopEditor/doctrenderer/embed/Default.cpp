#include "Default.h"

#include "./ZipEmbed.h"
//#include "./TextMeasurerEmbed.h"
#include "./MemoryStreamEmbed.h"
#include "./HashEmbed.h"

namespace NSJSBase
{
	void CreateDefaults()
	{
		CJSContext::Embed<CZipEmbed>();
		CJSContext::Embed<CMemoryStreamEmbed>();
//		CTextMeasurerEmbed::CreateObjectInContext("CreateNativeTextMeasurer", context);
		CJSContext::Embed<CHashEmbed>();
	}
}
