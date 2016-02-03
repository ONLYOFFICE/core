
#include "ConvertXls2Xlsx.h"
#include "XlsConverter.h"

#include "../../../Common/OfficeFileErrorDescription.h"


long ConvertXls2Xlsx(const std::wstring & srcFile, const std::wstring & dstPath, const ProgressCallback* pCallBack)
{
	XlsConverter converter(srcFile, dstPath,  pCallBack);

	if (converter.isError())
	{
		if (converter.is_older_version)
			return  AVS_MSFILE_ERROR_OLDER;
		else return AVS_ERROR_FILEFORMAT;
	}

	converter.convertDocument();
	
	converter.write();

	return 0;

}