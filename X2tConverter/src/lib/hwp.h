#ifndef HWP_H
#define HWP_H

#include "../../../HwpFile/HWPFile.h"
#include "common.h"

namespace NExtractTools
{
	template <typename OpenMethod>
	_UINT32 hwp_file2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams, OpenMethod openMethod, bool bConvertToDir)
	{
		CHWPFile oFile;

		oFile.SetTempDirectory(convertParams.m_sTempDir);

		params.m_bMacro = false;
		
		if (!openMethod(oFile, sFrom) ||
		    (bConvertToDir && !oFile.ConvertToOOXML_Dir(sTo)) ||
		    (!bConvertToDir && !oFile.ConvertToOOXML(sTo)))
			return AVS_FILEUTILS_ERROR_CONVERT;

		return 0;
	}

	_UINT32 hwp2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, [](CHWPFile& oFile, const std::wstring& sFrom){ return oFile.OpenHWP(sFrom); }, false);
	}

	_UINT32 hwp2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, [](CHWPFile& oFile, const std::wstring& sFrom){ return oFile.OpenHWP(sFrom); }, true);
	}

	_UINT32 hwpx2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, [](CHWPFile& oFile, const std::wstring& sFrom){ return oFile.OpenHWPX(sFrom); }, false);
	}

	_UINT32 hwpx2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, [](CHWPFile& oFile, const std::wstring& sFrom){ return oFile.OpenHWPX(sFrom); }, true);
	}

	_UINT32 hwpml2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, [](CHWPFile& oFile, const std::wstring& sFrom){ return oFile.OpenHWPML(sFrom); }, false);
	}

	_UINT32 hwpml2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return hwp_file2docx(sFrom, sTo, params, convertParams, [](CHWPFile& oFile, const std::wstring& sFrom){ return oFile.OpenHWPML(sFrom); }, true);
	}
}

#endif // HWP_H
