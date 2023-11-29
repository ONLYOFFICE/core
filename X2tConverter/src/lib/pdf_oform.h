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
		std::wstring sTempDocx = combinePath(convertParams.m_sTempDir, L"meta.docx");
		_UINT32 nRes = pdfoform2docx(sFrom, sTempDocx, params, convertParams);

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
