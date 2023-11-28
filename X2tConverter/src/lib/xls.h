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

			return oCXlsxSerializer.saveToFile(sTo, sResultXlsxDir, params.getXmlOptions());
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
