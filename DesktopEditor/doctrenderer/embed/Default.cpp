#include "Default.h"

#include "./ZipEmbed.h"
#include "./TextMeasurerEmbed.h"
#include "./MemoryStreamEmbed.h"

namespace NSJSBase
{
	void CreateDefaults(JSSmart<CJSContext>& context) {
		CZipEmbed::CreateObjectInContext("CreateNativeZip", context);
		CTextMeasurerEmbed::CreateObjectInContext("CreateNativeTextMeasurer", context);
		CMemoryStreamEmbed::CreateObjectInContext("CreateNativeMemoryStream", context);
	}
}
