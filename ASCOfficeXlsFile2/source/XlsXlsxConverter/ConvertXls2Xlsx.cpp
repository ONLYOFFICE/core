
#include "ConvertXls2Xlsx.h"
#include "XlsConverter.h"

#include "../../../Common/OfficeFileErrorDescription.h"


long ConvertXls2Xlsx(const std::wstring & srcFile, const std::wstring & dstPath, const ProgressCallback* pCallBack)
{
	XlsConverter converter(srcFile, pCallBack);

	converter.convertDocument();
	
	converter.write(dstPath);

	return 0;

}