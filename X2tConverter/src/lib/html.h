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

#include "../../../Common/3dParty/pole/pole.h"
#include "../../../EpubFile/CEpubFile.h"
#include "../../../Fb2File/Fb2File.h"
#include "../../../HtmlFile2/htmlfile2.h"
#include "../../../Common/3dParty/md/md2html.h"
#include "common.h"

namespace NExtractTools
{
	_UINT32 html2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sDocxDir);
		_UINT32 nRes = html2docx_dir(sFrom, sDocxDir, params, convertParams);
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = (S_OK == docx_dir2doct_bin(sDocxDir, sTo, params, convertParams)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	_UINT32 html_zip2doct_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
		NSDirectory::CreateDirectory(sDocxDir);
		_UINT32 nRes = html_zip2docx_dir(sFrom, sDocxDir, params, convertParams);
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = (S_OK == docx_dir2doct_bin(sDocxDir, sTo, params, convertParams)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	_UINT32 html2doct(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", html2doct_bin);
	}
	_UINT32 html_zip2doct(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"doct", html_zip2doct_bin);
	}
	_UINT32 html2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"docx", html2docx_dir);
	}
	_UINT32 html_zip2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"docx", html_zip2docx_dir);
	}
	_UINT32 html_array2docx_dir(const std::vector<std::wstring> &arFiles, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		params.m_bMacro = false;

		CHtmlFile2 oFile;
		oFile.SetTempDirectory(convertParams.m_sTempDir);
		return (S_OK == oFile.ConvertHTML2OOXML(arFiles, sTo)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 html2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::vector<std::wstring> arFiles;
		arFiles.push_back(sFrom);
		return html_array2docx_dir(arFiles, sTo, params, convertParams);
	}
	_UINT32 html_zip2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		std::vector<std::wstring> arFiles;

		POLE::Storage storage(sFrom.c_str());
		if (storage.open())
		{
			POLE::Stream stream(&storage, L"WordDocument");

			POLE::uint64 size_stream = stream.size();
			unsigned char *buffer = new unsigned char[size_stream];
			if (buffer)
			{
				stream.read(buffer, size_stream);
				std::wstring sTempHtml = combinePath(convertParams.m_sTempDir, L"tempHtml.html");

				NSFile::CFileBinary file;

				if (file.CreateFileW(sTempHtml))
				{
					file.WriteFile(buffer, (DWORD)size_stream);
					file.CloseFile();

					arFiles.push_back(sTempHtml);
				}
				delete[] buffer;
			}
		}
		else // in zip
		{
		}
		return 0 == html_array2docx_dir(arFiles, sTo, params, convertParams) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	// mht
	_UINT32 mht2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		CHtmlFile2 oFile;
		oFile.SetTempDirectory(convertParams.m_sTempDir);
		return (S_OK == oFile.ConvertMHT2OOXML(sFrom, sTo)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 mht2md(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		CHtmlFile2 oFile;
		oFile.SetTempDirectory(convertParams.m_sTempDir);
		return (S_OK == oFile.ConvertMHT2Markdown(sFrom, sTo)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	// epub
	_UINT32 epub2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		CEpubFile oFile;
		oFile.SetTempDirectory(convertParams.m_sTempDir);
		return (S_OK == oFile.Convert(sFrom, sTo, false)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 fb2docx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		CFb2File fb2File;
		fb2File.SetTmpDirectory(convertParams.m_sTempDir);
		return S_OK == fb2File.Open(sFrom, sTo) ? S_OK : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 fb2docx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"docx", fb2docx_dir);
	}

	// doct_bin => html
	_UINT32 doct_bin2html_internal(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		if (params.getFromChanges())
			params.setFromChanges(false);

		std::wstring sFileFromDir = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = combinePath(sFileFromDir, L"media");
		std::wstring sHtmlFile = combinePath(convertParams.m_sTempDir, L"index.html");
		if (!NSDirectory::Exists(sImagesDirectory))
			NSDirectory::CreateDirectory(sImagesDirectory);

		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
		std::wstring sXml = getDoctXml(NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT,
									   NSDoctRenderer::DoctRendererFormat::FormatFile::HTML,
									   sFrom, sHtmlFile, sImagesDirectory, convertParams.m_sThemesDir, -1, L"", params);

		std::wstring sResult;
		oDoctRenderer.Execute(sXml, sResult);
		
		if (sResult.find(L"error") != std::wstring::npos)
		{
			std::wcerr << L"DoctRenderer:" << sResult << std::endl;
			return AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			if (!params.getDontSaveAdditional())
			{
				//create changes.zip next to result file
				std::wstring sBinDir = NSDirectory::GetFolderPath(sFrom);
				std::wstring sChangesDir = sBinDir + FILE_SEPARATOR_STR + L"changes";
				copyImagesFromChanges(&oDoctRenderer, sImagesDirectory, sChangesDir, NSDirectory::GetFolderPath(*params.m_sFileTo));
			}
		}
		return nRes;
	}

	// doct_bin -> epub
	_UINT32 doct_bin2epub(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = doct_bin2html_internal(sFrom, sTo, params, convertParams);
		if (0 != nRes)
			return nRes;

		std::wstring sHtmlFile = combinePath(convertParams.m_sTempDir, L"index.html");

		CEpubFile oFile;
		std::wstring sEpubTemp = combinePath(convertParams.m_sTempDir, L"tmp");
		NSDirectory::CreateDirectory(sEpubTemp);
		oFile.SetTempDirectory(sEpubTemp);

		if (S_FALSE == oFile.FromHtml(sHtmlFile, sTo, params.m_sTitle ? *params.m_sTitle : L""))
			nRes = AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}
	// doct_bin -> fb2
	_UINT32 doct_bin2fb(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = doct_bin2html_internal(sFrom, sTo, params, convertParams);
		if (0 != nRes)
			return nRes;

		std::wstring sHtmlFile = combinePath(convertParams.m_sTempDir, L"index.html");

		CFb2File fb2File;
		fb2File.SetTmpDirectory(convertParams.m_sTempDir);
		if (S_FALSE == fb2File.FromHtml(sHtmlFile, sTo, params.m_sTitle ? *params.m_sTitle : L""))
			nRes = AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}
	// doct_bin -> html
	_UINT32 doct_bin2html(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = doct_bin2html_internal(sFrom, sTo, params, convertParams);
		if (0 != nRes)
			return nRes;

		std::wstring sHtmlFile = combinePath(convertParams.m_sTempDir, L"index.html");

		if (!NSFile::CFileBinary::Copy(sHtmlFile, sTo))
			nRes = AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}
	// doct_bin -> html_zip
	_UINT32 doct_bin2html_zip(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = doct_bin2html_internal(sFrom, sTo, params, convertParams);
		if (0 != nRes)
			return nRes;

		std::wstring sHtmlFile = combinePath(convertParams.m_sTempDir, L"index.html");

		COfficeUtils oZip;
		if (S_FALSE == oZip.CompressFileOrDirectory(sHtmlFile, sTo))
			nRes = AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}
	// md -> html
	_UINT32 md2html(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return Md::ConvertMdFileToHtml(sFrom, sTo) ? S_OK : AVS_FILEUTILS_ERROR_CONVERT;
	}

	// html -> markdown
	_UINT32 html2md(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		CHtmlFile2 oFile;
		oFile.SetTempDirectory(convertParams.m_sTempDir);
		return (S_OK == oFile.ConvertHTML2Markdown(sFrom, sTo)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
}
