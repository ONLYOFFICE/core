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

#include "../../../OOXML/Binary/Document/DocWrapper/DocxSerializer.h"
#include "common.h"

namespace NExtractTools
{
	// docx <=> doct
	_UINT32 docx2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedDOCX = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedDOCX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCX, NULL, 0))
		{
			// check crypt
			COfficeFileFormatChecker OfficeFileFormatChecker;
			if (OfficeFileFormatChecker.isOfficeFile(sFrom))
			{
				if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
					return mscrypt2oot_bin(sFrom, sTo, params, convertParams);
				else if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO)
					return mitcrypt2oot_bin(sFrom, sTo, params, convertParams);
				else
				{
					if (create_if_empty(sFrom, sTo, L"DOCY;v10;0;"))
						return 0;
				}
				// zip currupt ... check for empty dir output  ?????
				if (NSDirectory::GetFilesCount(sTempUnpackedDOCX, true) < 4)
					return AVS_FILEUTILS_ERROR_CONVERT;
			}
			else
				return AVS_FILEUTILS_ERROR_CONVERT;
		}

		convertParams.m_sTempParamOOXMLFile = sFrom;
		return docx_dir2doct_bin(sTempUnpackedDOCX, sTo, params, convertParams);
	}
	_UINT32 docx_dir2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = S_OK;
		if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX) && !convertParams.m_sTempParamOOXMLFile.empty())
		{
			std::wstring sToDir = NSDirectory::GetFolderPath(sTo);
			nRes = CopyOOXOrigin(sToDir, sFrom, L"origin.docx", convertParams.m_sTempParamOOXMLFile);
		}
		else
		{
			// Save to file (from temp dir)
			BinDocxRW::CDocxSerializer m_oCDocxSerializer;

			m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
			m_oCDocxSerializer.setFontDir(params.getFontPath());

			// bool bRes = m_oCDocxSerializer.saveToFile (sResDoct, sSrcDocx, sTemp);
			nRes = m_oCDocxSerializer.saveToFile(sTo, sFrom, params.getXmlOptions(), convertParams.m_sTempDir) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		// clear tmp param for this method
		convertParams.m_sTempParamOOXMLFile = L"";
		return nRes;
	}
	_UINT32 docx2doct(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", docx2doct_bin);
	}
	_UINT32 docx_dir2doct(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", docx_dir2doct_bin);
	}
	_UINT32 doct_bin2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sResultDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sResultDocxDir);

		convertParams.m_sTempResultOOXMLDirectory = sResultDocxDir;
		_UINT32 nRes = doct_bin2docx_dir(sFrom, sTo, params, convertParams);

		if (SUCCEEDED_X2T(nRes) && params.m_nFormatTo)
		{
			if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == *params.m_nFormatTo)
			{
				std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
				std::wstring sCTTo;
				switch (*params.m_nFormatTo)
				{
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
					sCTTo = _T("application/vnd.ms-word.document.macroEnabled.main+xml");
					break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
					sCTTo = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml");
					break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
					sCTTo = _T("application/vnd.ms-word.template.macroEnabledTemplate.main+xml");
					break;
				}
				nRes = replaceContentType(sResultDocxDir, sCTFrom, sCTTo);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == *params.m_nFormatTo)
			{
			  // std::wstring sCT = L"<Default Extension=\"oform\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.oform\"/>";
			  // nRes = addContentType(sResultDocxDir, sCT);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == *params.m_nFormatTo)
			{
			  // std::wstring sCT = L"<Default Extension=\"docxf\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.docxf\"/>";
			  // nRes = addContentType(sResultDocxDir, sCT);
			}
		}
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sResultDocxDir, sTo, params, convertParams);
		}

		return nRes;
	}
	_UINT32 doct_bin2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		std::wstring sTargetBin;
		if (params.getFromChanges())
		{
			params.setFromChanges(false);
			nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sTargetBin, params, convertParams);
		}
		else
			sTargetBin = sFrom;

		BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		m_oCDocxSerializer.setOFormEnabled(params.m_nFormatTo &&
										  (*params.m_nFormatTo == AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM ||
										   *params.m_nFormatTo == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF ||
										   *params.m_nFormatTo == AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF));
		m_oCDocxSerializer.setMacroEnabled(params.m_bMacro);
		m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
		m_oCDocxSerializer.setFontDir(params.getFontPath());

		std::wstring sXmlOptions;
		std::wstring sThemePath; // will be filled by 'CreateDocxFolders' method
		std::wstring sMediaPath; // will be filled by 'CreateDocxFolders' method
		std::wstring sEmbedPath; // will be filled by 'CreateDocxFolders' method

		m_oCDocxSerializer.CreateDocxFolders(convertParams.m_sTempResultOOXMLDirectory, sThemePath, sMediaPath, sEmbedPath);

		if (SUCCEEDED_X2T(nRes))
		{
			nRes = m_oCDocxSerializer.loadFromFile(sTargetBin, convertParams.m_sTempResultOOXMLDirectory, sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
			params.m_bMacro = m_oCDocxSerializer.getMacroEnabled();
		}
		// удаляем EditorWithChanges, потому что он не в Temp
		if (sFrom != sTargetBin)
			NSFile::CFileBinary::Remove(sTargetBin);

		convertParams.m_sTempResultOOXMLDirectory = L"";
		return nRes;
	}
	_UINT32 doct2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::oot2format(sFrom, sTo, params, convertParams, L"doct", doct_bin2docx);
	}

	// docx flat
	_UINT32 docxflat2doct(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", docxflat2doct_bin);
	}
	_UINT32 docxflat2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// Save to file (from temp dir)
		BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
		m_oCDocxSerializer.setFontDir(params.getFontPath());

		_UINT32 nRes = m_oCDocxSerializer.saveToFile(sTo, sFrom, params.getXmlOptions(), convertParams.m_sTempDir) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}

	_UINT32 docxflat2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedDOCX = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedDOCX);

		BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		_UINT32 nRes = 0;
		if (m_oCDocxSerializer.convertFlat(sFrom, sTempUnpackedDOCX, params.m_bMacro, convertParams.m_sTempDir))
		{
			params.changeFormatFromPost(*params.m_nFormatFrom, params.m_bMacro);
			nRes = dir2zipMscrypt(sTempUnpackedDOCX, *params.m_sFileTo, params, convertParams);
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}

	// packageooxml
	_UINT32 package2ooxml(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"ooxml", package2ooxml_dir);
	}
	_UINT32 package2bin_t(const std::wstring &sFrom, const std::wstring &sTo, InputParams &params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"bin_t", package2bin);
	}
	_UINT32 package2bin(const std::wstring &sFrom, const std::wstring &sTo, InputParams &params, ConvertParams& convertParams)
	{
		std::wstring sResultOoxmlDir = combinePath(convertParams.m_sTempDir, L"ooxml_unpacked");
		NSDirectory::CreateDirectory(sResultOoxmlDir);

		_UINT32 nRes = package2ooxml_dir(sFrom, sResultOoxmlDir, params, convertParams);

		if (SUCCEEDED_X2T(nRes))
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;

			COfficeFileFormatChecker OfficeFileFormatChecker;

			if (OfficeFileFormatChecker.isOOXFormatFile(sResultOoxmlDir, true))
			{
				switch (OfficeFileFormatChecker.nFileType)
				{
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
					nRes = docx_dir2doct_bin(sResultOoxmlDir, sTo, params, convertParams);
					break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:

				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB:
					convertParams.m_bTempIsXmlOptions = true;
					nRes = xlsx_dir2xlst_bin(sResultOoxmlDir, sTo, params, convertParams);
					break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
					nRes = pptx_dir2pptt_bin(sResultOoxmlDir, sTo, params, convertParams);
					break;
				default:
					break;
				}
			}
		}

		return nRes;
	}
	_UINT32 package2ooxml_dir(const std::wstring &sFrom, const std::wstring &sTo, InputParams &params, ConvertParams& convertParams)
	{
		params.m_bMacro = false;

		BinDocxRW::CDocxSerializer m_oCDocxSerializer;
		_UINT32 nRes = m_oCDocxSerializer.unpackageFile(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}

	// docm/dotx
	_UINT32 dotm2docm(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"docm", dotm2docm_dir);
	}
	_UINT32 dotm2docm_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml_replace_content_type(sFrom, sTo, params, convertParams,
														  L"application/vnd.ms-word.template.macroEnabledTemplate.main+xml",
														  L"application/vnd.ms-word.document.macroEnabled.main+xml");
	}
	_UINT32 dotx2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"docx", dotx2docx_dir);
	}
	_UINT32 dotx2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml_replace_content_type(sFrom, sTo, params, convertParams,
														  L"application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml",
														  L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
	}
	_UINT32 docm2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"docx", docm2docx_dir);
	}
	_UINT32 docm2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxmlm2ooml_dir(sFrom, sTo, params, convertParams, NSCommon::OOXML_DOCUMENT_TYPE::Word, NSCommon::OOXML_DOCUMENT_SUBTYPE::Main);
	}
	_UINT32 dotm2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"docx", dotm2docx_dir);
	}
	_UINT32 dotm2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxmlm2ooml_dir(sFrom, sTo, params, convertParams, NSCommon::OOXML_DOCUMENT_TYPE::Word, NSCommon::OOXML_DOCUMENT_SUBTYPE::Template);
	}
}
