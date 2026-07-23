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

#include "../../../TxtFile/Source/TxtXmlFile.h"
#include "../../../OOXML/Binary/Document/DocWrapper/DocxSerializer.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 txt2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		params.m_bMacro = false; 

		CTxtXmlFile txtFile;
		std::wstring xml_options = params.getXmlOptionsFrom();
		return txtFile.txt_LoadFromFile(sFrom, sTo, xml_options);
	}
	_UINT32 docx_dir2txt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		CTxtXmlFile txtFile;
		return txtFile.txt_SaveToFile(sTo, sFrom, params.getXmlOptionsTo());
	}

	_UINT32 txt2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sResultDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sResultDocxDir);

		CTxtXmlFile txtFile;
		params.m_bMacro = false;

		_UINT32 nRes = txtFile.txt_LoadFromFile(sFrom, sResultDocxDir, params.getXmlOptionsFrom());
		if (SUCCEEDED_X2T(nRes))
		{
			BinDocxRW::CDocxSerializer m_oCDocxSerializer;
			nRes = m_oCDocxSerializer.saveToFile(sTo, sResultDocxDir, convertParams.m_sTempDir) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
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
