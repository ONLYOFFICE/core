/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "../../../MsBinaryFile/DocFile/Main/DocFormatLib.h"
#include "../../../OOXML/Binary/Document/DocWrapper/DocxSerializer.h"
#include "common.h"

#ifdef CreateDirectory
#undef CreateDirectory
#endif

namespace NExtractTools
{
	_UINT32 doc2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		COfficeDocFile docFile;

		docFile.m_sTempFolder = convertParams.m_sTempDir;
		docFile.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

		params.m_bMacro = false;
		_UINT32 hRes = docFile.LoadFromFile(sFrom, sTo, params.getPassword(), params.m_bMacro);

		if (AVS_ERROR_DRM == hRes)
		{
			if (!params.getDontSaveAdditional())
			{
				copyOrigin(sFrom, *params.m_sFileTo);
			}
			return AVS_FILEUTILS_ERROR_CONVERT_DRM;
		}
		else if (AVS_ERROR_PASSWORD == hRes)
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}
		return 0 == hRes ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 docx_dir2doc(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return AVS_FILEUTILS_ERROR_CONVERT;
	}

	_UINT32 doc2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sResultDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sResultDocxDir);

		COfficeDocFile docFile;
		docFile.m_sTempFolder = convertParams.m_sTempDir;
		docFile.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

		params.m_bMacro = true;

		_UINT32 nRes = docFile.LoadFromFile(sFrom, sResultDocxDir, params.getPassword(), params.m_bMacro);

		nRes = processEncryptionError(nRes, sFrom, params);
		if (SUCCEEDED_X2T(nRes))
		{
			BinDocxRW::CDocxSerializer m_oCDocxSerializer;

			m_oCDocxSerializer.setFontDir(params.getFontPath());

			std::wstring xml_options = params.getXmlOptions();

			nRes =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, xml_options, convertParams.m_sTempDir) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}

	_UINT32 doc2docm_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		COfficeDocFile docFile;
		docFile.m_sTempFolder = convertParams.m_sTempDir;
		docFile.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

		params.m_bMacro = true;

		_UINT32 hRes = docFile.LoadFromFile(sFrom, sTo, params.getPassword(), params.m_bMacro);
		if (AVS_ERROR_DRM == hRes)
		{
			if (!params.getDontSaveAdditional())
			{
				copyOrigin(sFrom, *params.m_sFileTo);
			}
			hRes = AVS_FILEUTILS_ERROR_CONVERT_DRM;
		}
		else if (AVS_ERROR_PASSWORD == hRes)
		{
			hRes = AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}
		return hRes;
	}

	_UINT32 doc2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"docx", doc2docx_dir);
	}
	_UINT32 doc2doct(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", doc2doct_bin);
	}
	_UINT32 doc2docm(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"docx", doc2docx_dir, false);
	}
}
