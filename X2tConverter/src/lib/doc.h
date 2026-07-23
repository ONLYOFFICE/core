/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

			nRes =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, convertParams.m_sTempDir) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
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
