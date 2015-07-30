
#include "ConvertXls2Xlsx.h"
#include "XlsConverter.h"

#include "../../../Common/OfficeFileErrorDescription.h"


long ConvertXls2Xlsx(const std::wstring & srcFile, const std::wstring & dstPath, const ProgressCallback* pCallBack)
{
	XlsConverter converter(srcFile, dstPath,  pCallBack);

	converter.convertDocument();
	
	converter.write();

	return 0;

}