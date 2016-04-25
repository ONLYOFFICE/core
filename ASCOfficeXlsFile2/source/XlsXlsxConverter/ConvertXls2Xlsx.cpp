
#include "ConvertXls2Xlsx.h"
#include "XlsConverter.h"

#include "../../../Common/OfficeFileErrorDescription.h"


long ConvertXls2Xlsx(const std::wstring & srcFile, const std::wstring & dstPath, const std::wstring & password, const std::wstring & fontsPath, const ProgressCallback* pCallBack)
{
	XlsConverter converter(srcFile, dstPath, password, fontsPath, pCallBack);

	if (converter.isError())
	{
		if (converter.is_older_version)
			return  AVS_MSFILE_ERROR_OLDER;
		if (converter.is_encrypted)
		{
			if (password.empty())	return  AVS_ERROR_DRM;
			else					return  AVS_ERROR_PASSWORD;
		}
		else return AVS_ERROR_FILEFORMAT;
	}

	converter.convertDocument();
	
	converter.write();

	return 0;

}