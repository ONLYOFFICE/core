#ifndef HWP_H
#define HWP_H

#include "../../../HwpFile/HWPFile.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 hwp2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		CHWPFile oFile(sFrom);

		oFile.SetTempDirectory(convertParams.m_sTempDir);

		if (!oFile.Open() || !oFile.ConvertToOOXML(sTo))
		{
			oFile.Close();
			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		return 0;
	}

	_UINT32 hwp2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		CHWPFile oFile(sFrom);

		if (!oFile.Open() || !oFile.ConvertToOOXML_Dir(sTo))
		{
			oFile.Close();
			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		return 0;
	}
}

#endif // HWP_H
