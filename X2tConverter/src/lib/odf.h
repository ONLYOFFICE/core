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

#include "../../../OdfFile/Reader/Converter/ConvertOO2OOX.h"
#include "../../../OdfFile/Writer/Converter/Oox2OdfConverter.h"
#include "../../../OOXML/Binary/Document/DocWrapper/DocxSerializer.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 odf2oox(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedOox = combinePath(convertParams.m_sTempDir, L"oox_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedOox);

		_UINT32 nRes = odf2oox_dir(sFrom, sTempUnpackedOox, params, convertParams);

		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sTempUnpackedOox, sTo, params, convertParams);
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
			if (create_if_empty(sFrom, sTo, L""))
				nRes = 0;
		}
		return nRes;
	}
	_UINT32 odf2oox_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;

		std::wstring sTempUnpackedOdf = combinePath(convertParams.m_sTempDir, L"odf_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedOdf);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
		{
			nRes = ConvertODF2OOXml(sTempUnpackedOdf, sTo, params.getFontPath(), convertParams.m_sTempDir, params.getPassword());

			params.m_bMacro = false; // todooo ������� ��������� �������� odf
			nRes = processEncryptionError(nRes, sFrom, params);
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	_UINT32 odf2oot(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", odf2oot_bin);
	}
	_UINT32 odf2oot_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedOdf = combinePath(convertParams.m_sTempDir, L"odf_unpacked");
		std::wstring sTempUnpackedOox = combinePath(convertParams.m_sTempDir, L"oox_unpacked");

		NSDirectory::CreateDirectory(sTempUnpackedOdf);

		_UINT32 nRes = 0;

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
		{
			NSDirectory::CreateDirectory(sTempUnpackedOox);

			nRes = ConvertODF2OOXml(sTempUnpackedOdf, sTempUnpackedOox, params.getFontPath(), convertParams.m_sTempDir, params.getPassword());
			
			params.m_bMacro = false; // todooo ������� ��������� �������� odf
			
			nRes = processEncryptionError(nRes, sFrom, params);
			if (SUCCEEDED_X2T(nRes))
			{
				COfficeFileFormatChecker OfficeFileFormatChecker;

				if (OfficeFileFormatChecker.isOOXFormatFile(sTempUnpackedOox, true))
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
						return docx_dir2doct_bin(sTempUnpackedOox, sTo, params, convertParams);
					}
					break;
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB:
					{
						const std::wstring &sXmlOptions = params.getXmlOptions();
						convertParams.m_bTempIsXmlOptions = false;
						return xlsx_dir2xlst_bin(sTempUnpackedOox, sTo, params, convertParams);
					}
					break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
					{
						return pptx_dir2pptt_bin(sTempUnpackedOox, sTo, params, convertParams);
					}
					break;
					default:
					{
						nRes = AVS_FILEUTILS_ERROR_CONVERT;
					}
					break;
					}
				}
			}
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
			if (create_if_empty(sFrom, sTo, L"DOCY;v10;0;"))
				nRes = 0;
		}
		return nRes;
	}

	_UINT32 odf_flat2oox(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"oox", odf_flat2oox_dir);
	}
	_UINT32 odf_flat2oox_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = ConvertODF2OOXml(sFrom, sTo, params.getFontPath(), convertParams.m_sTempDir, params.getPassword());
		params.m_bMacro = false; // todooo ������� ��������� �������� odf
		
		nRes = processEncryptionError(nRes, sFrom, params);
		return nRes;
	}
	_UINT32 odf_flat2oot(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", odf_flat2oot_bin);
	}
	_UINT32 odf_flat2oot_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedOox = combinePath(convertParams.m_sTempDir, L"oox_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedOox);

		_UINT32 nRes = ConvertODF2OOXml(sFrom, sTempUnpackedOox, params.getFontPath(), convertParams.m_sTempDir, params.getPassword());
		params.m_bMacro = false; // todooo ������� ��������� �������� odf

		nRes = processEncryptionError(nRes, sFrom, params);
		if (SUCCEEDED_X2T(nRes))
		{
			BinDocxRW::CDocxSerializer m_oCDocxSerializer;

			m_oCDocxSerializer.setFontDir(params.getFontPath());

			nRes = m_oCDocxSerializer.saveToFile(sTo, sTempUnpackedOox, params.getXmlOptions(), convertParams.m_sTempDir) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}

		return nRes;
	}

	// docx => odt
	_UINT32 docx2odt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedDOCX = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedDOCX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCX, NULL, 0))
		{
			convertParams.m_bIsTemplate = false;
			return docx_dir2odt(sTempUnpackedDOCX, sTo, params, convertParams); // add Template ????
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 docx_dir2odt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedODT = combinePath(convertParams.m_sTempDir, L"odt_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedODT);

		Oox2Odf::Converter converter(sFrom, L"text", params.getFontPath(), convertParams.m_bIsTemplate, convertParams.m_sTempDir);

		_UINT32 nRes = 0;
		try
		{
			std::wstring password = params.getSavePassword();
			std::wstring documentID = params.getDocumentID();

			converter.convert();
			converter.write(sTempUnpackedODT, convertParams.m_sTempDir, password, documentID);

			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODT, sTo, false, password.empty() ? Z_DEFLATED : 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		catch (...)
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}

	// xlsx => ods
	_UINT32 xlsx2ods(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedXLSX = combinePath(convertParams.m_sTempDir, L"xlsx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedXLSX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
		{
			convertParams.m_bIsTemplate = false;
			return xlsx_dir2ods(sTempUnpackedXLSX, sTo, params, convertParams); // add Template ???
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 xlsx_dir2ods(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedODS = combinePath(convertParams.m_sTempDir, L"ods_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedODS);

		Oox2Odf::Converter converter(sFrom, L"spreadsheet", params.getFontPath(), convertParams.m_bIsTemplate, convertParams.m_sTempDir);

		_UINT32 nRes = 0;

		std::wstring password = params.getSavePassword();
		std::wstring documentID = params.getDocumentID();

		converter.convert();
		converter.write(sTempUnpackedODS, convertParams.m_sTempDir, password, documentID);

		COfficeUtils oCOfficeUtils(NULL);
		nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODS, sTo, false, password.empty() ? Z_DEFLATED : 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}

	// pptx => odp
	_UINT32 pptx2odp(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedPPTX = combinePath(convertParams.m_sTempDir, L"pptx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedPPTX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTX, NULL, 0))
		{
			convertParams.m_bIsTemplate = false;
			return pptx_dir2odp(sTempUnpackedPPTX, sTo, params, convertParams); // add template ???
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	// pptx_dir -> odp
	_UINT32 pptx_dir2odp(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedODP = combinePath(convertParams.m_sTempDir, L"odp_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedODP);

		Oox2Odf::Converter converter(sFrom, L"presentation", params.getFontPath(), convertParams.m_bIsTemplate, convertParams.m_sTempDir);

		_UINT32 nRes = 0;
		try
		{
			std::wstring password = params.getSavePassword();
			std::wstring documentID = params.getDocumentID();

			converter.convert();
			converter.write(sTempUnpackedODP, convertParams.m_sTempDir, password, documentID);

			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODP, sTo, false, password.empty() ? Z_DEFLATED : 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		catch (...)
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}

	_UINT32 otf2odf(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedOdf = combinePath(convertParams.m_sTempDir, L"odf_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedOdf);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
			return AVS_FILEUTILS_ERROR_CONVERT;

		_UINT32 nRes = ConvertOTF2ODF(sTempUnpackedOdf);
		if (SUCCEEDED_X2T(nRes))
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedOdf, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}

	_UINT32 docxflat2odt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedODT = combinePath(convertParams.m_sTempDir, L"odt_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedODT);

		Oox2Odf::Converter converter(sFrom, L"text", params.getFontPath(), false, convertParams.m_sTempDir);

		_UINT32 nRes = 0;
		try
		{
			std::wstring password = params.getSavePassword();
			std::wstring documentID = params.getDocumentID();

			converter.convert();
			converter.write(sTempUnpackedODT, convertParams.m_sTempDir, password, documentID);

			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODT, sTo, false, password.empty() ? Z_DEFLATED : 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		catch (...)
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
}
