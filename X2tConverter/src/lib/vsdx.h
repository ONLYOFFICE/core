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

#include "../../../OOXML/Binary/Document/DocWrapper/VsdxSerializer.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 vsdx2vsdt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"vsdt", vsdx2vsdt_bin);
	}
	_UINT32 vsdx2vsdt_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// Extract vsdx to temp directory
		std::wstring sTempUnpackedVSDX = combinePath(convertParams.m_sTempDir, L"vsdx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedVSDX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedVSDX, NULL, 0))
		{
			//check crypt
			COfficeFileFormatChecker OfficeFileFormatChecker;
			if (OfficeFileFormatChecker.isOfficeFile(sFrom))
			{
				if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
				{
					return mscrypt2oot_bin(sFrom, sTo, params, convertParams);
				}
				else if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO)
					return mitcrypt2oot_bin(sFrom, sTo, params, convertParams);
				else
				{
					if (create_if_empty(sFrom, sTo, L"VSDY;v10;0;"))
						return 0;
					return AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
			else
				return AVS_FILEUTILS_ERROR_CONVERT;
		}

		convertParams.m_bTempIsXmlOptions = true;
		convertParams.m_sTempParamOOXMLFile = sFrom;
		return vsdx_dir2vsdt_bin(sTempUnpackedVSDX, sTo, params, convertParams);
	}
	_UINT32 vsdx_dir2vsdt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"vsdt", vsdx_dir2vsdt_bin);
	}
	_UINT32 vsdx_dir2vsdt_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = S_OK;
		std::wstring sToDir = NSDirectory::GetFolderPath(sTo);
		if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_DRAW_VSDX) && !convertParams.m_sTempParamOOXMLFile.empty())
		{
			nRes = CopyOOXOrigin(sToDir, sFrom, L"origin.vsdx", convertParams.m_sTempParamOOXMLFile);
		}
		else
		{
			BinVsdxRW::CVsdxSerializer oVsdxSerializer;

			// Save to file (from temp dir)
			oVsdxSerializer.setIsNoBase64(params.getIsNoBase64());
			oVsdxSerializer.setFontDir(params.getFontPath());

			nRes = oVsdxSerializer.saveToFile(sTo, sFrom);
		}

		convertParams.m_sTempParamOOXMLFile = L"";
		convertParams.m_bTempIsXmlOptions = false;
		return nRes;
	}
	_UINT32 vsdt_bin2vsdx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// Extract vsdx to temp directory
		std::wstring sResultVsdxDir = combinePath(convertParams.m_sTempDir, L"vsdx_unpacked");
		NSDirectory::CreateDirectory(sResultVsdxDir);

		convertParams.m_sTempResultOOXMLDirectory = sResultVsdxDir;
		_UINT32 nRes = vsdt_bin2vsdx_dir(sFrom, sTo, params, convertParams);

		if (SUCCEEDED_X2T(nRes) && params.m_nFormatTo)
		{
			if (AVS_OFFICESTUDIO_FILE_DRAW_VSTX == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DRAW_VSTM == *params.m_nFormatTo)
			{
				std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
				std::wstring sCTTo;
				switch (*params.m_nFormatTo)
				{
				case AVS_OFFICESTUDIO_FILE_DRAW_VSTX:
					sCTTo = _T("application/vnd.ms-visio.template.main+xml");
					break;
				case AVS_OFFICESTUDIO_FILE_DRAW_VSTM:
					sCTFrom = _T("application/vnd.ms-visio.drawing.macroEnabled.main+xml");
					sCTTo = _T("application/vnd.ms-visio.template.macroEnabled.main+xml");
					break;
				}
				nRes = replaceContentType(sResultVsdxDir, sCTFrom, sCTTo);
			}
		}
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sResultVsdxDir, sTo, params, convertParams);
		}
		return nRes;
	}
	_UINT32 vsdt2vsdx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::oot2format(sFrom, sTo, params, convertParams, L"vsdt", vsdt_bin2vsdx);
	}
	_UINT32 vsdt_bin2vsdx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;

		std::wstring sTargetBin;
		if (params.getFromChanges())
		{
			params.setFromChanges(false);
			nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::VSDT, sTargetBin, params, convertParams);
		}
		else
			sTargetBin = sFrom;

		BinVsdxRW::CVsdxSerializer oVsdxSerializer;

		oVsdxSerializer.setMacroEnabled(params.m_bMacro);
		oVsdxSerializer.setIsNoBase64(params.getIsNoBase64());
		oVsdxSerializer.setFontDir(params.getFontPath());

		std::wstring sMediaPath;
		std::wstring sEmbedPath; 

		oVsdxSerializer.CreateVsdxFolders(convertParams.m_sTempResultOOXMLDirectory, sMediaPath, sEmbedPath);

		if (SUCCEEDED_X2T(nRes))
		{
			nRes = oVsdxSerializer.loadFromFile(sTargetBin, convertParams.m_sTempResultOOXMLDirectory, sMediaPath, sEmbedPath);
			params.m_bMacro = oVsdxSerializer.getMacroEnabled();
		}
		// удаляем EditorWithChanges, потому что он не в Temp
		if (sFrom != sTargetBin)
			NSFile::CFileBinary::Remove(sTargetBin);

		convertParams.m_sTempResultOOXMLDirectory = L"";
		return nRes;
	}

}
