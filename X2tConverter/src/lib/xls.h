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

#include "../../../MsBinaryFile/XlsFile/Converter/ConvertXls2Xlsx.h"
#include "../../../OOXML/Binary/Document/DocWrapper/XlsxSerializer.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 xls2xlsm_dir_macro(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams, const bool& bIsMacro)
	{
		params.m_bMacro = bIsMacro;

		int lcid = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

		_UINT32 nRes = ConvertXls2Xlsx(sFrom, sTo, params.getPassword(), params.getFontPath(), convertParams.m_sTempDir, lcid, params.m_bMacro);

		nRes = processEncryptionError(nRes, sFrom, params);
		return nRes;
	}
	_UINT32 xls2xlsx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return xls2xlsm_dir_macro(sFrom, sTo, params, convertParams, false);
	}
	_UINT32 xls2xlsm_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return xls2xlsm_dir_macro(sFrom, sTo, params, convertParams, true);
	}

	_UINT32 xls2xlst_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sResultXlsxDir = combinePath(convertParams.m_sTempDir, L"xlsx_unpacked");
		NSDirectory::CreateDirectory(sResultXlsxDir);

		params.m_bMacro = true;

		int lcid = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

		_UINT32 nRes = ConvertXls2Xlsx(sFrom, sResultXlsxDir, params.getPassword(), params.getFontPath(), convertParams.m_sTempDir, lcid, params.m_bMacro);

		nRes = processEncryptionError(nRes, sFrom, params);
		if (SUCCEEDED_X2T(nRes))
		{
			BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

			oCXlsxSerializer.setFontDir(params.getFontPath());

			return oCXlsxSerializer.saveToFile(sTo, sResultXlsxDir, params.getXmlOptionsFrom());
		}
		return nRes;
	}

	_UINT32 xls2xlsx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"xlsx", xls2xlsx_dir);
	}
	_UINT32 xls2xlsm(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"xlsm", xls2xlsm_dir);
	}

	_UINT32 xls2xlst(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"xlst", xls2xlst_bin);
	}
}
