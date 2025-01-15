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

#include "../../../OOXML/Binary/Sheets/Common/Common.h"
#include "../../../OOXML/Binary/Sheets/Reader/CSVReader.h"
#include "../../../OOXML/XlsxFormat/Xlsx.h"
#include "../../../OOXML/Binary/Document/DocWrapper/XlsxSerializer.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 csv2xlsx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		params.m_bMacro = false;

		OOX::Spreadsheet::CXlsx oXlsx;

		BYTE fileType;
		UINT nCodePage;
		std::wstring sDelimiter;
		BYTE saveFileType;
        _INT32 lcid = -1;
        
		SerializeCommon::ReadFileType(params.getXmlOptions(), fileType, nCodePage, sDelimiter, saveFileType, lcid);

		CSVReader csvReader;
        _UINT32 nRes = csvReader.Read(sFrom, oXlsx, nCodePage, sDelimiter, lcid);

		if (SUCCEEDED_X2T(nRes))
		{
			oXlsx.PrepareToWrite();

			OOX::CContentTypes oContentTypes;
				nRes = oXlsx.Write(sTo, oContentTypes) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}

		return nRes;
	}
	_UINT32 xlsx_dir2csv(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sResultXlstDir   = combinePath(convertParams.m_sTempDir, L"xlst_unpacked");
		std::wstring sResultXlstFileEditor = combinePath(sResultXlstDir, L"Editor.bin");

		NSDirectory::CreateDirectory(sResultXlstDir);

		BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		oCXlsxSerializer.setFontDir(params.getFontPath());

		std::wstring sXMLOptions = _T("<XmlOptions><fileOptions fileType=\"1\"/></XmlOptions>");
		_UINT32 nRes = oCXlsxSerializer.saveToFile(sResultXlstFileEditor, sFrom, sXMLOptions);
		if (SUCCEEDED_X2T(nRes))
		{
			std::wstring sMediaPath;
			std::wstring sEmbedPath;
            if(!params.m_nFormatTo)
            {
                    params.m_nFormatTo = new int;
                    *params.m_nFormatTo = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
            }
            sXMLOptions = params.getXmlOptions();//_T("<XmlOptions><fileOptions fileType=\"2\"/></XmlOptions>");

			nRes = oCXlsxSerializer.loadFromFile(sResultXlstFileEditor, sTo, sXMLOptions, sMediaPath, sEmbedPath);
		}

		return nRes;
	}

	_UINT32 csv2xlst_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		params.m_bMacro = false;

		// Save to file (from temp dir)
		BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		oCXlsxSerializer.setFontDir(params.getFontPath());

		if (!params.m_nFormatFrom)
			params.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_UNKNOWN);
		if (AVS_OFFICESTUDIO_FILE_UNKNOWN == *params.m_nFormatFrom)
			*params.m_nFormatFrom = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;

		return oCXlsxSerializer.saveToFile(sTo, sFrom, params.getXmlOptions());
	}
	_UINT32 xlst_bin2csv(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
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

		if (SUCCEEDED_X2T(nRes))
		{
			BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

			oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
			oCXlsxSerializer.setFontDir(params.getFontPath());

			std::wstring sResultCsvDir = combinePath(convertParams.m_sTempDir, L"csv_unpacked");

			NSDirectory::CreateDirectory(sResultCsvDir);
			std::wstring sMediaPath; // will be filled by 'CreateXlsxFolders' method
			std::wstring sEmbedPath; // will be filled by 'CreateXlsxFolders' method
			std::wstring sXmlOptions = params.getXmlOptions();

			oCXlsxSerializer.CreateXlsxFolders(sXmlOptions, sResultCsvDir, sMediaPath, sEmbedPath);

			nRes = oCXlsxSerializer.loadFromFile(sTargetBin, sTo, sXmlOptions, sMediaPath, sEmbedPath);
		}
		// удаляем EditorWithChanges, потому что он не в Temp
		if (sFrom != sTargetBin)
			NSFile::CFileBinary::Remove(sTargetBin);
		return nRes;
	}

	_UINT32 xlsx2csv(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2format(sFrom, sTo, params, convertParams, L"xlsx", xlsx_dir2csv);
	}
	_UINT32 csv2xlsx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"xlsx", csv2xlsx_dir);
	}

	_UINT32 csv2xlst(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"xlst", csv2xlst_bin);
	}
	// TODO: return NSCommon::oot2format(sFrom, sTo, params, convertParams, L"doct", doct_bin2rtf);
	_UINT32 xlst2csv(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sCSV = sTo;
		std::wstring sTempUnpackedXLST   = combinePath(convertParams.m_sTempDir, L"xlst_unpacked");
		std::wstring sTempXlstFileEditor = combinePath(sTempUnpackedXLST, L"Editor.bin");
		NSDirectory::CreateDirectory(sTempUnpackedXLST);

		// unzip xlst to folder
		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLST, NULL, 0))
			return AVS_FILEUTILS_ERROR_CONVERT;

		BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		oCXlsxSerializer.setFontDir(params.getFontPath());
		oCXlsxSerializer.setTempDir(convertParams.m_sTempDir);

		std::wstring sMediaPath;
		std::wstring sEmbedPath;

		params.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV);

		return oCXlsxSerializer.loadFromFile(sTempXlstFileEditor, sCSV, params.getXmlOptions(), sMediaPath, sEmbedPath);
	}
}
