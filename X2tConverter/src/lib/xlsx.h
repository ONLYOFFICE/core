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

#include "../../../OOXML/XlsbFormat/Xlsb.h"
#include "../../../OOXML/Binary/Document/DocWrapper/XlsxSerializer.h"
#include "../../../OOXML/Binary/Sheets/Reader/XMLReader/XMLReader.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/GlobalWorkbookInfo.h"

#include "common.h"

namespace NExtractTools
{
	_UINT32 xlsx2xlst(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"xlst", xlsx2xlst_bin);
	}
	_UINT32 xlsx2xlst_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// Extract xlsx to temp directory
		std::wstring sTempUnpackedXLSX = combinePath(convertParams.m_sTempDir, L"xlsx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedXLSX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
		{
			//check crypt
			COfficeFileFormatChecker OfficeFileFormatChecker;
			if (OfficeFileFormatChecker.isOfficeFile(sFrom))
			{
				if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
				{
					// test protect
					bool isOldPassword = params.hasPassword();
					const std::wstring sOldPassword = params.getPassword();

					if (isOldPassword)
						delete params.m_sPassword;
					params.m_sPassword = new std::wstring(L"VelvetSweatshop");

					_UINT32 nRes = mscrypt2oot_bin(sFrom, sTo, params, convertParams);
					if (SUCCEEDED_X2T(nRes))
					{
						return nRes;
					}
					else
					{
						delete params.m_sPassword;
						if (isOldPassword)
							params.m_sPassword = new std::wstring(sOldPassword);
						return mscrypt2oot_bin(sFrom, sTo, params, convertParams);
					}
				}
				else if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO)
					return mitcrypt2oot_bin(sFrom, sTo, params, convertParams);
				else
				{
					if (create_if_empty(sFrom, sTo, L"XLSY;v10;0;"))
						return 0;
					return AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
			else
				return AVS_FILEUTILS_ERROR_CONVERT;
		}

		convertParams.m_bTempIsXmlOptions = true;
		convertParams.m_sTempParamOOXMLFile = sFrom;
		return xlsx_dir2xlst_bin(sTempUnpackedXLSX, sTo, params, convertParams);
	}
	_UINT32 xlsxflat2xlst(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"xlst", xlsxflat2xlst_bin);
	}
	_UINT32 xlsxflat2xlst_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		oCXlsxSerializer.setFontDir(params.getFontPath());

		return oCXlsxSerializer.saveToFile(sTo, sFrom, params.getXmlOptions());
	}
	_UINT32 xlsxflat2xlsx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"xlsx", xlsxflat2xlsx_dir);
	}
	_UINT32 xlsxflat2xlsx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		params.m_bMacro = false;
		BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		oCXlsxSerializer.setFontDir(params.getFontPath());
		oCXlsxSerializer.setMacroEnabled(params.m_bMacro);

		_UINT32 nRes = oCXlsxSerializer.xml2Xlsx(sFrom, sTo, params.getXmlOptions());

		return nRes;
	}
	_UINT32 xlsx_dir2xlst(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"xlst", xlsx_dir2xlst_bin);
	}
	_UINT32 xlsx_dir2xlst_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = S_OK;
		std::wstring sToDir = NSDirectory::GetFolderPath(sTo);
		if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX) && !convertParams.m_sTempParamOOXMLFile.empty())
		{
			nRes = CopyOOXOrigin(sToDir, sFrom, L"origin.xlsx", convertParams.m_sTempParamOOXMLFile);
		}
		else
		{
			BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

			// Save to file (from temp dir)
			oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
			oCXlsxSerializer.setFontDir(params.getFontPath());

			nRes = oCXlsxSerializer.saveToFile(sTo, sFrom, convertParams.m_bTempIsXmlOptions ? params.getXmlOptions() : L"");
		}

		convertParams.m_sTempParamOOXMLFile = L"";
		convertParams.m_bTempIsXmlOptions = false;
		return nRes;
	}
	_UINT32 xlsx_dir2xlsb_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		const OOX::CPath oox_path(sFrom);

		OOX::Spreadsheet::CXlsb oXlsb;
		oXlsb.m_bWriteToXlsb = true;
		oXlsb.Read(oox_path);
		oXlsb.LinkTables();
		oXlsb.PrepareRichStr();

		OOX::CContentTypes oContentTypes;
		oXlsb.SetPropForWriteSheet(sTo, oContentTypes);
		oXlsb.WriteSheetData();
		_UINT32 nRes = oXlsb.WriteBin(sTo, oContentTypes) ? S_OK : AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}
	_UINT32 xlsx_dir2xls_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = S_OK;
		const OOX::CPath oox_path(sFrom);
		{
			OOX::Spreadsheet::CXlsb oXlsx;
			oXlsx.m_bWriteToXlsb = true;
			oXlsx.Read(oox_path);
			oXlsx.PrepareHlinks();
			oXlsx.PrepareRichStr();
			nRes = oXlsx.WriteXLS(sTo)? S_OK : AVS_FILEUTILS_ERROR_CONVERT;;
		}
		return nRes;
	}
	_UINT32 xlst_bin2xlsb_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{

		_UINT32 nRes = 0;
		
		std::wstring sTargetBin;
		if (params.getFromChanges())
		{
			params.setFromChanges(false);
			nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sTargetBin, params, convertParams);
		}
		else
			sTargetBin = sFrom;
		
		std::wstring sTempUnpackedXLSB = convertParams.m_sTempResultOOXMLDirectory;

        convertParams.m_sTempResultOOXMLDirectory = sTempUnpackedXLSB;
        nRes = xlst_bin2xlsx_dir(sTargetBin, sTempUnpackedXLSB, params, convertParams);

		// удаляем EditorWithChanges, потому что он не в Temp
		if (sFrom != sTargetBin)
			NSFile::CFileBinary::Remove(sTargetBin);
		return nRes;
	}
	_UINT32 xlst_bin2xlsb(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// Extract xlsx to temp directory
		std::wstring sResultXlsxDir = combinePath(convertParams.m_sTempDir, L"xlsb_unpacked");
		NSDirectory::CreateDirectory(sResultXlsxDir);

		convertParams.m_sTempResultOOXMLDirectory = sResultXlsxDir;
		_UINT32 nRes = xlst_bin2xlsb_dir(sFrom, sTo, params, convertParams);

		if (SUCCEEDED_X2T(nRes) && params.m_nFormatTo)
		{
			nRes = dir2zipMscrypt(sResultXlsxDir, sTo, params, convertParams);
		}
		return nRes;
	}
	_UINT32 xlst_bin2xlsx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// Extract xlsx to temp directory
		std::wstring sResultXlsxDir = combinePath(convertParams.m_sTempDir, L"xlsx_unpacked");
		NSDirectory::CreateDirectory(sResultXlsxDir);

		convertParams.m_sTempResultOOXMLDirectory = sResultXlsxDir;
		_UINT32 nRes = xlst_bin2xlsx_dir(sFrom, sTo, params, convertParams);

		if (SUCCEEDED_X2T(nRes) && params.m_nFormatTo)
		{
			if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == *params.m_nFormatTo)
			{
				std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml");
				std::wstring sCTTo;
				switch (*params.m_nFormatTo)
				{
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
					sCTTo = _T("application/vnd.ms-excel.sheet.macroEnabled.main+xml");
					break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
					sCTTo = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml");
					break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
					sCTTo = _T("application/vnd.ms-excel.template.macroEnabled.main+xml");
					break;
				}
				nRes = replaceContentType(sResultXlsxDir, sCTFrom, sCTTo);
			}
		}
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sResultXlsxDir, sTo, params, convertParams);
		}
		return nRes;
	}
	_UINT32 xlst_bin2xlsx_dir(const std::wstring& sFrom, const std::wstring &sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;

		std::wstring sTargetBin;
		if (params.getFromChanges())
		{
			params.setFromChanges(false);
			nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sTargetBin, params, convertParams);
		}
		else
			sTargetBin = sFrom;

		BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setMacroEnabled(params.m_bMacro);
		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		oCXlsxSerializer.setFontDir(params.getFontPath());

        std::wstring sXmlOptions = params.getXmlOptions();
		std::wstring sMediaPath; // will be filled by 'CreateXlsxFolders' method
		std::wstring sEmbedPath; // will be filled by 'CreateXlsxFolders' method

		oCXlsxSerializer.CreateXlsxFolders(sXmlOptions, convertParams.m_sTempResultOOXMLDirectory, sMediaPath, sEmbedPath);

		if (SUCCEEDED_X2T(nRes))
		{
			nRes = oCXlsxSerializer.loadFromFile(sTargetBin, convertParams.m_sTempResultOOXMLDirectory, sXmlOptions, sMediaPath, sEmbedPath);
			params.m_bMacro = oCXlsxSerializer.getMacroEnabled();
		}
		// удаляем EditorWithChanges, потому что он не в Temp
		if (sFrom != sTargetBin)
			NSFile::CFileBinary::Remove(sTargetBin);

		convertParams.m_sTempResultOOXMLDirectory = L"";
		return nRes;
	}
	_UINT32 xlst2xlsx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::oot2format(sFrom, sTo, params, convertParams, L"xlst", xlst_bin2xlsx);
	}
	_UINT32 xlst2xlsb(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::oot2format(sFrom, sTo, params, convertParams, L"xlst", xlst_bin2xlsb);
	}
	_UINT32 xltx2xlsx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"xlsx", xltx2xlsx_dir);
	}
	_UINT32 xltx2xlsx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		params.m_bMacro = false;
		return NSCommon::ooxml2ooxml_replace_content_type(sFrom, sTo, params, convertParams,
														  L"application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml",
														  L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml");
	}
	_UINT32 xltm2xlsm(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"xlsm", xltm2xlsm_dir);
	}
	_UINT32 xltm2xlsm_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml_replace_content_type(sFrom, sTo, params, convertParams,
														  L"application/vnd.ms-excel.template.macroEnabled.main+xml",
														  L"application/vnd.ms-excel.sheet.macroEnabled.main+xml");
	}
	_UINT32 xlsm2xlsx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"xlsx", xlsm2xlsx_dir);
	}
	_UINT32 xlsm2xlsx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxmlm2ooml_dir(sFrom, sTo, params, convertParams, NSCommon::OOXML_DOCUMENT_TYPE::Sheet, NSCommon::OOXML_DOCUMENT_SUBTYPE::Main);
	}
	_UINT32 xltm2xlsx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"xlsx", xltm2xlsx_dir);
	}
	_UINT32 xltm2xlsx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxmlm2ooml_dir(sFrom, sTo, params, convertParams, NSCommon::OOXML_DOCUMENT_TYPE::Sheet, NSCommon::OOXML_DOCUMENT_SUBTYPE::Template);
	}
	_UINT32 xlsb2xlsx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedXLSB = combinePath(convertParams.m_sTempDir, L"xlsb_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedXLSB);

		COfficeUtils oCOfficeUtils(NULL);
		_UINT32 nRes = oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSB, NULL, 0);
		if (SUCCEEDED_X2T(nRes))
		{
			OOX::Spreadsheet::CXlsb oXlsb;
			oXlsb.ReadNative(OOX::CPath(sTempUnpackedXLSB));
			oXlsb.PrepareSi();
			oXlsb.PrepareTableFormula();

			OOX::CContentTypes oContentTypes;
			oXlsb.SetPropForWriteSheet(sTo, oContentTypes);
			oXlsb.ReadSheetData();

			params.m_bMacro = params.m_bMacro & (oXlsb.m_pVbaProject || oXlsb.GetGlobalinfo()->bMacrosExist);
			nRes = oXlsb.WriteNative(sTo, oContentTypes) ? S_OK : AVS_FILEUTILS_ERROR_CONVERT;
			if(!params.m_bMacro)
			{
				nRes = NSCommon::ooxmlm_dir2ooxml_dir(sFrom, sTo, params, convertParams, NSCommon::OOXML_DOCUMENT_TYPE::Sheet, NSCommon::OOXML_DOCUMENT_SUBTYPE::Main);
				if (0 != nRes)
					return nRes;
			}

		}
		return nRes;
	}

	_UINT32 xlsx_dir2xlsb(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
    {
		return NSCommon::ooxml2ooxml(sFrom, sTo, params, convertParams, L"xlsb", xlsx_dir2xlsb_dir);
    }
	_UINT32 xlsx2xlsb(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempUnpackedXLSX = combinePath(convertParams.m_sTempDir, L"xlsx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedXLSX);

		COfficeUtils oCOfficeUtils(NULL);
		_UINT32 nRes = oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0);
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = xlsx_dir2xlsb(sTempUnpackedXLSX, sTo, params, convertParams);
		}
		return nRes;
	}
	_UINT32 xlsx2xls(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2format(sFrom, sTo, params, convertParams, L"xls", xlsx_dir2xls_dir);
	}
	_UINT32 xml2xlsx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"xlsx", xml2xlsx_dir);
	}
	_UINT32 xml2xlsx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		OOX::Spreadsheet::CXlsx oXlsx;

		XMLReader reader = {};
        _INT32 lcid = -1;
        if(params.m_nLcid != nullptr)
            lcid = *params.m_nLcid;

        reader.Read2(sFrom, oXlsx, lcid);

		oXlsx.PrepareToWrite();

		OOX::CContentTypes oContentTypes;
		auto nRes = oXlsx.Write(sTo, oContentTypes) ? S_OK : AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}
}
