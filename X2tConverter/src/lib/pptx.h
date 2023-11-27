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

#include "../../OOXML/PPTXFormat/DrawingConverter/ASCOfficePPTXFile.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 pptx2pptt_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// unzip pptx to temp folder
		std::wstring sTempUnpackedPPTX = combinePath(convertParams.m_sTempDir, L"pptx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedPPTX);

			   // unzip pptx to folder
		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTX, NULL, 0))
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
					if (create_if_empty(sFrom, sTo, L"PPTY;v10;0;"))
						return 0;
					return AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
			else
				return AVS_FILEUTILS_ERROR_CONVERT;
		}

		convertParams.m_sTempParamOOXMLFile = sFrom;
		return pptx_dir2pptt_bin(sTempUnpackedPPTX, sTo, params, convertParams);
	}
	_UINT32 pptx_dir2pptt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"pptt", pptx_dir2pptt_bin);
	}
	_UINT32 pptx_dir2pptt_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		std::wstring sToDir = NSDirectory::GetFolderPath(sTo);
		if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX) && !convertParams.m_sTempParamOOXMLFile.empty())
		{
			nRes = CopyOOXOrigin(sToDir, sFrom, L"origin.pptx", convertParams.m_sTempParamOOXMLFile);
		}
		else
		{
			// convert unzipped pptx to unzipped pptt
			CPPTXFile *pptx_file = new CPPTXFile();

			if (pptx_file)
			{
				pptx_file->SetIsNoBase64(params.getIsNoBase64());
				pptx_file->put_TempDirectory(convertParams.m_sTempDir);
				pptx_file->SetFontDir(params.getFontPath());
				nRes = (S_OK == pptx_file->OpenFileToPPTY(sFrom, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

				delete pptx_file;
			}
		}

		convertParams.m_sTempParamOOXMLFile = L"";
		return nRes;
	}
	_UINT32 pptx2pptt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"pptt", pptx2pptt_bin);
	}
	_UINT32 pptt_bin2pptx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sResultPptxDir = combinePath(convertParams.m_sTempDir, L"pptx_unpacked");
		NSDirectory::CreateDirectory(sResultPptxDir);

		convertParams.m_sTempResultOOXMLDirectory = sResultPptxDir;
		_UINT32 nRes = pptt_bin2pptx_dir(sFrom, sTo, params, convertParams);

		if (SUCCEEDED_X2T(nRes) && params.m_nFormatTo)
		{
			if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == *params.m_nFormatTo)
			{
				std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
				std::wstring sCTTo;
				switch (*params.m_nFormatTo)
				{
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
					sCTTo = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");
					break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
					sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml");
					break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
					sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml");
					break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
					sCTTo = _T("application/vnd.ms-powerpoint.template.macroEnabled.main+xml");
					break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
					sCTTo = _T("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml");
					break;
				}
				nRes = replaceContentType(sResultPptxDir, sCTFrom, sCTTo);
			}
		}
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sResultPptxDir, sTo, params, convertParams);
		}
		return nRes;
	}
	_UINT32 pptt_bin2pptx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;

		std::wstring sTargetBin;
		if (params.getFromChanges())
		{
			params.setFromChanges(false);
			nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT, sTargetBin, params, convertParams);
		}
		else
			sTargetBin = sFrom;

		CPPTXFile *pptx_file = new CPPTXFile();
		if (pptx_file)
		{
			pptx_file->SetMacroEnabled(params.m_bMacro);
			pptx_file->SetIsNoBase64(params.getIsNoBase64());
			pptx_file->SetFontDir(params.getFontPath());
			nRes = (S_OK == pptx_file->ConvertPPTYToPPTX(sTargetBin, convertParams.m_sTempResultOOXMLDirectory, convertParams.m_sThemesDir)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

			delete pptx_file;
		}
		// удаляем EditorWithChanges, потому что он не в Temp
		if (sFrom != sTargetBin)
			NSFile::CFileBinary::Remove(sTargetBin);

		convertParams.m_sTempResultOOXMLDirectory = L"";
		return nRes;
	}
	_UINT32 pptt2pptx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::oot2format(sFrom, sTo, params, convertParams, L"pptt", pptt_bin2pptx);
	}

	_UINT32 ppsx2pptx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"pptx", ppsx2pptx_dir);
	}
	_UINT32 ppsx2pptx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml_replace_content_type(sFrom, sTo, params, convertParams,
														  L"application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml",
														  L"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
	}
	_UINT32 potx2pptx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"pptx", potx2pptx_dir);
	}
	_UINT32 potx2pptx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml_replace_content_type(sFrom, sTo, params, convertParams,
														  L"application/vnd.openxmlformats-officedocument.presentationml.template.main+xml",
														  L"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
	}
	_UINT32 ppsm2pptx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"pptx", ppsm2pptx_dir);
	}
	_UINT32 ppsm2pptx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxmlm2ooml_dir(sFrom, sTo, params, convertParams, NSCommon::OOXML_DOCUMENT_TYPE::Slide, NSCommon::OOXML_DOCUMENT_SUBTYPE::Show);
	}
	_UINT32 potm2pptm(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"pptx", potm2pptm_dir);
	}
	_UINT32 potm2pptm_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml_replace_content_type(sFrom, sTo, params, convertParams,
														  L"application/vnd.ms-powerpoint.template.macroEnabled.main+xml",
														  L"application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");
	}
	_UINT32 ppsm2pptm(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"pptm", ppsm2pptm_dir);
	}
	_UINT32 ppsm2pptm_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml_replace_content_type(sFrom, sTo, params, convertParams,
														  L"application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml",
														  L"application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");
	}
	_UINT32 pptm2pptx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"pptm", pptm2pptx_dir);
	}
	_UINT32 pptm2pptx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxmlm2ooml_dir(sFrom, sTo, params, convertParams, NSCommon::OOXML_DOCUMENT_TYPE::Slide, NSCommon::OOXML_DOCUMENT_SUBTYPE::Main);
	}
	_UINT32 potm2pptx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"pptx", potm2pptx_dir);
	}
	_UINT32 potm2pptx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxmlm2ooml_dir(sFrom, sTo, params, convertParams, NSCommon::OOXML_DOCUMENT_TYPE::Slide, NSCommon::OOXML_DOCUMENT_SUBTYPE::Template);
	}

}
