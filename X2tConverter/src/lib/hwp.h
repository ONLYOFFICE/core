#ifndef HWP_H
#define HWP_H

#include "../../../HwpFile/HWPFile.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 hwp_file2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams, bool bIsXmlFormat, bool bConvertToDir)
	{
		CHWPFile oFile;

		oFile.SetTempDirectory(convertParams.m_sTempDir);

		params.m_bMacro = false;
		if (((bIsXmlFormat && !oFile.OpenHWPX(sFrom)) ||
		     (!bIsXmlFormat && !oFile.OpenHWP(sFrom))) ||
		    ((bConvertToDir && !oFile.ConvertToOOXML_Dir(sTo)) ||
		     (!bConvertToDir && !oFile.ConvertToOOXML(sTo))))
			return AVS_FILEUTILS_ERROR_CONVERT;

		return 0;
	}

	_UINT32 hwp2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, false, false);
	}

	_UINT32 hwp2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, false, true);
	}

	_UINT32 hwpx2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, true, false);
	}

	_UINT32 hwpx2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, true, true);
	}
}

#endif // HWP_H
