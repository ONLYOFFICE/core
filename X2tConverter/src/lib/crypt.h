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

#include "../../../OfficeCryptReader/source/ECMACryptFile.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 mscrypt2oot(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sResultOotDir        = combinePath(convertParams.m_sTempDir, L"oot_unpacked");
		std::wstring sResultOotFileEditor = combinePath(sResultOotDir, L"Editor.bin");
		NSDirectory::CreateDirectory(sResultOotDir);

		_UINT32 nRes = mscrypt2oot_bin(sFrom, sResultOotFileEditor, params, convertParams);
		if (SUCCEEDED_X2T(nRes))
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultOotDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}

		return nRes;
	}
	_UINT32 mscrypt2oot_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		//decrypt to temp file
		std::wstring password = params.getPassword();
		std::wstring sResultDecryptFile = combinePath(convertParams.m_sTempDir, L"uncrypt_file.oox");

		ECMACryptFile cryptReader;
		bool bDataIntegrity = false;

		if (cryptReader.DecryptOfficeFile(sFrom, sResultDecryptFile, password, bDataIntegrity) == false)
		{
			if (password.empty())
				return AVS_FILEUTILS_ERROR_CONVERT_DRM;
			else
				return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}

		if (bDataIntegrity == false)
		{
		  // было несанкционированое вешательство в файл
		}

		COfficeFileFormatChecker OfficeFileFormatChecker;

		if (OfficeFileFormatChecker.isOfficeFile(sResultDecryptFile))
		{
			switch (OfficeFileFormatChecker.nFileType)
			{
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM:
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF:
			{
				return docx2doct_bin(sResultDecryptFile, sTo, params, convertParams);
			}
			break;
			case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
			case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
			case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
			case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
			case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB:
			{
				const std::wstring &sXmlOptions = params.getXmlOptions();
				return xlsx2xlst_bin(sResultDecryptFile, sTo, params, convertParams);
			}
			break;
			case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
			case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
			case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
			case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
			case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
			case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
			{
				return pptx2pptt_bin(sResultDecryptFile, sTo, params, convertParams);
			}
			break;
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_FLAT:
			{
				return docxflat2doct_bin(sResultDecryptFile, sTo, params, convertParams);
			}
			break;
			case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_FLAT:
			{
				const std::wstring &sXmlOptions = params.getXmlOptions();
				return xlsxflat2xlst_bin(sResultDecryptFile, sTo, params, convertParams);
			}
			break;
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_PACKAGE:
			case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_PACKAGE:
			case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX_PACKAGE:
			{
				return package2bin(sResultDecryptFile, sTo, params, convertParams);
			}
			break;
			}
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 mitcrypt2oox(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// todo
		return AVS_FILEUTILS_ERROR_CONVERT_DRM_UNSUPPORTED;
	}
	_UINT32 mitcrypt2oot_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// todo
		return AVS_FILEUTILS_ERROR_CONVERT_DRM_UNSUPPORTED;
	}
	_UINT32 mscrypt2oox(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring password = params.getPassword();

		ECMACryptFile cryptReader;
		bool bDataIntegrity = false;

		if (cryptReader.DecryptOfficeFile(sFrom, sTo, password, bDataIntegrity) == false)
		{
			if (password.empty())
				return AVS_FILEUTILS_ERROR_CONVERT_DRM;
			else
				return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}

		if (bDataIntegrity == false)
		{
		  // было несанкционированое вешательство в файл
		}

		return 0;
	}
	_UINT32 oox2mscrypt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring password = params.getSavePassword();
		std::wstring documentID = params.getDocumentID();

		ECMACryptFile cryptReader;

		if (cryptReader.EncryptOfficeFile(sFrom, sTo, password, documentID) == false)
		{
			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		return 0;
	}
	_UINT32 fromMscrypt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring password = params.getPassword();
		std::wstring sResultDecryptFile = combinePath(convertParams.m_sTempDir, L"uncrypt_file.oox");

		_UINT32 nRes = mscrypt2oox(sFrom, sResultDecryptFile, params, convertParams);

		if (!SUCCEEDED_X2T(nRes) && password.empty()) // qiaoshemei1 (1).xlsx - запрокченный xlsx
		{
			if (!params.getDontSaveAdditional())
			{
				copyOrigin(sFrom, sTo);
			}
			return AVS_FILEUTILS_ERROR_CONVERT_DRM;
		}
		nRes = processEncryptionError(nRes, sFrom, params);
		if (SUCCEEDED_X2T(nRes))
		{
			COfficeFileFormatChecker OfficeFileFormatChecker;

			if (OfficeFileFormatChecker.isOfficeFile(sResultDecryptFile))
			{
				params.changeFormatFromPrev(OfficeFileFormatChecker.nFileType);
				switch (OfficeFileFormatChecker.nFileType)
				{
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF:
				{
					return fromDocument(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, params, convertParams);
				}
				break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB:
				{
					return fromSpreadsheet(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX, params, convertParams);
				}
				break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
				{
					return fromPresentation(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX, params, convertParams);
				}
				break;
				case AVS_OFFICESTUDIO_FILE_DRAW_VSDX:
				case AVS_OFFICESTUDIO_FILE_DRAW_VSDM:
				case AVS_OFFICESTUDIO_FILE_DRAW_VSTX:
				case AVS_OFFICESTUDIO_FILE_DRAW_VSTM:
				{
					return fromDraw(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_DRAW_VSDX, params, convertParams);
				}
				break;
				}
			}
		}
		return nRes;
	}
	_UINT32 fromMitcrypt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// todo
		return AVS_FILEUTILS_ERROR_CONVERT_DRM_UNSUPPORTED;
	}
}
