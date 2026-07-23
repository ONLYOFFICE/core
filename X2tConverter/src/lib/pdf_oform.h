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

#include "../../../Common/OfficeFileFormatDefines.h"
#include "./pdf_image.h"

namespace NExtractTools
{
	inline std::wstring GetMetaTag()
	{
		return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(g_format_oform_pdf_meta_tag,
															 (LONG)strlen(g_format_oform_pdf_meta_tag));

	}

	_UINT32 pdfoform2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTempDocxInjected = combinePath(convertParams.m_sTempDir, L"meta.docx");
		std::wstring sTempDocx = sTempDocxInjected;
		_UINT32 nRes = pdfoform2docx(sFrom, sTempDocx, params, convertParams);

		if (SUCCEEDED_X2T(nRes))
		{
			COfficeFileFormatChecker OfficeFileFormatChecker;
			if (OfficeFileFormatChecker.isOfficeFile(sTempDocxInjected))
			{
				if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
				{
					sTempDocx = combinePath(convertParams.m_sTempDir, L"uncrypt_file.oox");
					nRes = mscrypt2oox(sTempDocxInjected, sTempDocx, params, convertParams);
				}
				else if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO)
				{
					sTempDocx = combinePath(convertParams.m_sTempDir, L"uncrypt_file.oox");
					nRes = mitcrypt2oox(sTempDocxInjected, sTempDocx, params, convertParams);
				}
			}
		}

		if (SUCCEEDED_X2T(nRes))
		{
			COfficeUtils oOfficeUtils(NULL);
			if (S_OK == oOfficeUtils.ExtractToDirectory(sTempDocx, sTo, NULL, 0))
				return 0;

			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		return nRes;
	}
	_UINT32 docx_dir2pdfoform(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sDocxMeta = combinePath(convertParams.m_sTempDir, L"meta.docx");

		_UINT32 nRes = dir2zipMscrypt(sFrom, sDocxMeta, params, convertParams);

		if (!SUCCEEDED_X2T(nRes))
			return nRes;

		std::wstring sDoctDir = combinePath(convertParams.m_sTempDir, L"doct_pdf_unpacked");
		std::wstring sDoctMeta = combinePath(sDoctDir, L"Editor.bin");
		NSDirectory::CreateDirectory(sDoctDir);

		nRes = docx_dir2doct_bin(sFrom, sDoctMeta, params, convertParams);

		if (!SUCCEEDED_X2T(nRes))
			return nRes;

		convertParams.m_sPdfOformMetaName = GetMetaTag();
		convertParams.m_sPdfOformMetaData = sDocxMeta;

		nRes = doct_bin2pdf(NSDoctRenderer::DoctRendererFormat::DOCT, sDoctMeta, sTo, params, convertParams);

		convertParams.m_sPdfOformMetaName = L"";
		convertParams.m_sPdfOformMetaData = L"";

		return nRes;
	}

	_UINT32 pdfoform2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sDocxDir);

		_UINT32 nRes = pdfoform2docx_dir(sFrom, sDocxDir, params, convertParams);
		if (SUCCEEDED_X2T(nRes))
		{
			return docx_dir2doct_bin(sDocxDir, sTo, params, convertParams);
		}

		return nRes;
	}

	_UINT32 doct_bin2pdfoform(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sDocxMeta = combinePath(convertParams.m_sTempDir, L"meta.docx");

		_UINT32 nRes = doct_bin2docx(sFrom, sDocxMeta, params, convertParams);

		if (!SUCCEEDED_X2T(nRes))
			return nRes;

		convertParams.m_sPdfOformMetaName = GetMetaTag();
		convertParams.m_sPdfOformMetaData = sDocxMeta;

		nRes = doct_bin2pdf(NSDoctRenderer::DoctRendererFormat::DOCT, sFrom, sTo, params, convertParams);

		convertParams.m_sPdfOformMetaName = L"";
		convertParams.m_sPdfOformMetaData = L"";

		return nRes;
	}

	_UINT32 pdfoform2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		BYTE* pData = NULL;
		DWORD dwDataSize = 0;

		std::wstring sMetaName = GetMetaTag();
		CPdfFile::GetMetaData(sFrom, sMetaName, &pData, dwDataSize);

		if (NULL == pData)
			return AVS_FILEUTILS_ERROR_CONVERT;

		_UINT32 nRes = 0;
		NSFile::CFileBinary oFile;
		if (oFile.CreateFile(sTo))
		{
			bool bIsOk = oFile.WriteFile(pData, dwDataSize);
			oFile.CloseFile();
			nRes = bIsOk ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}

		RELEASEARRAYOBJECTS(pData);
		return nRes;
	}
	_UINT32 docx2pdfoform(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::ooxml2format(sFrom, sTo, params, convertParams, L"docx", docx_dir2pdfoform);
	}

	_UINT32 pdfoform2doct(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", pdfoform2doct_bin);
	}

	_UINT32 doct2pdfoform(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::oot2format(sFrom, sTo, params, convertParams, L"doct", doct_bin2pdfoform);
	}
}
