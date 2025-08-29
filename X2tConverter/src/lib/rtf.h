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

#include "../../../RtfFile/Format/ConvertationManager.h"
#include "../../../OOXML/Binary/Document/DocWrapper/DocxSerializer.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 rtf2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		params.m_bMacro = false;
		
		RtfConvertationManager rtfConvert;

		rtfConvert.m_sTempFolder = convertParams.m_sTempDir;
		rtfConvert.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;
        if (params.m_sDefaultFontName != NULL)
            rtfConvert.m_sDefaultFontName = *params.m_sDefaultFontName;
        if (params.m_nDefaultFontSize != NULL)
        {
            rtfConvert.m_nDefaultFontSize = *params.m_nDefaultFontSize;
        }
        else
        {
            rtfConvert.m_nDefaultFontSize = NULL;
        }
		return 0 == rtfConvert.ConvertRtfToOOX(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 docx_dir2rtf(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// docx folder to rtf
		RtfConvertationManager rtfConvert;
		rtfConvert.m_sTempFolder = convertParams.m_sTempDir;
		rtfConvert.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

		if (rtfConvert.ConvertOOXToRtf(sTo, sFrom) == 0)
			return 0;
		return AVS_FILEUTILS_ERROR_CONVERT;
	}

	_UINT32 rtf2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		params.m_bMacro = false;

		std::wstring sResultDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sResultDocxDir);

		RtfConvertationManager rtfConvert;
		rtfConvert.m_sTempFolder = convertParams.m_sTempDir;
		rtfConvert.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

		if (rtfConvert.ConvertRtfToOOX(sFrom, sResultDocxDir) == 0)
		{
			BinDocxRW::CDocxSerializer m_oCDocxSerializer;

			m_oCDocxSerializer.setFontDir(params.getFontPath());

			std::wstring sXmlOptions;
			_UINT32 res = m_oCDocxSerializer.saveToFile(sTo, sResultDocxDir, sXmlOptions, convertParams.m_sTempDir) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

			return res;
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 doct_bin2rtf(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		std::wstring sResultDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sResultDocxDir);

		std::wstring sTargetBin;
		if (params.getFromChanges())
		{
			params.setFromChanges(false);
			nRes = apply_changes(sFrom, L"", NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sTargetBin, params, convertParams);
		}
		else
			sTargetBin = sFrom;

		BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		m_oCDocxSerializer.setFontDir(params.getFontPath());

		std::wstring sXmlOptions = _T("");
		std::wstring sThemePath; // will be filled by 'CreateDocxFolders' method
		std::wstring sMediaPath; // will be filled by 'CreateDocxFolders' method
		std::wstring sEmbedPath; // will be filled by 'CreateDocxFolders' method

		m_oCDocxSerializer.CreateDocxFolders(sResultDocxDir, sThemePath, sMediaPath, sEmbedPath);

		if (SUCCEEDED_X2T(nRes))
		{
			nRes = m_oCDocxSerializer.loadFromFile(sTargetBin, sResultDocxDir, sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
			if (SUCCEEDED_X2T(nRes))
			{
				// docx folder to rtf
				RtfConvertationManager rtfConvert;

				rtfConvert.m_sTempFolder = convertParams.m_sTempDir;
				rtfConvert.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

				nRes = rtfConvert.ConvertOOXToRtf(sTo, sResultDocxDir);
			}
		}
		// удаляем EditorWithChanges, потому что он не в Temp
		if (sFrom != sTargetBin)
			NSFile::CFileBinary::Remove(sTargetBin);
		return nRes;
	}

	// no dir method
	_UINT32 rtf2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"docx", rtf2docx_dir);
	}
	_UINT32 docx2rtf(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2format(sFrom, sTo, params, convertParams, L"docx", docx_dir2rtf);
	}

	_UINT32 rtf2doct(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", rtf2doct_bin);
	}	
	_UINT32 doct2rtf(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::oot2format(sFrom, sTo, params, convertParams, L"doct", doct_bin2rtf);
	}
}
