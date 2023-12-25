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

#include "../../../TxtFile/Source/TxtXmlFile.h"
#include "../../../OOXML/Binary/Document/DocWrapper/DocxSerializer.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 txt2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		params.m_bMacro = false; 

		CTxtXmlFile txtFile;
		std::wstring xml_options = params.getXmlOptions();
		return txtFile.txt_LoadFromFile(sFrom, sTo, xml_options);
	}
	_UINT32 docx_dir2txt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		CTxtXmlFile txtFile;
		return txtFile.txt_SaveToFile(sTo, sFrom, params.getXmlOptions());
	}

	_UINT32 txt2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sResultDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sResultDocxDir);

		CTxtXmlFile txtFile;
		params.m_bMacro = false;

		_UINT32 nRes = txtFile.txt_LoadFromFile(sFrom, sResultDocxDir, params.getXmlOptions());
		if (SUCCEEDED_X2T(nRes))
		{
			BinDocxRW::CDocxSerializer m_oCDocxSerializer;
			nRes = m_oCDocxSerializer.saveToFile(sTo, sResultDocxDir, params.getXmlOptions(), convertParams.m_sTempDir) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}

	_UINT32 txt2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"docx", txt2docx_dir);
	}
	_UINT32 docx2txt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2format(sFrom, sTo, params, convertParams, L"docx", docx_dir2txt);
	}

	_UINT32 txt2doct(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", txt2doct_bin);
	}
}
