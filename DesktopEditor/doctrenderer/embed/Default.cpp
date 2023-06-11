#include "Default.h"

#include "./ZipEmbed.h"
#include "./MemoryStreamEmbed.h"
#include "./TextMeasurerEmbed.h"
#include "./HashEmbed.h"

namespace NSJSBase
{
	void CreateDefaults()
	{
		CJSContext::Embed<CZipEmbed>();
		CJSContext::Embed<CMemoryStreamEmbed>();
		CJSContext::Embed<CTextMeasurerEmbed>();
		CJSContext::Embed<CHashEmbed>();
	}
}
