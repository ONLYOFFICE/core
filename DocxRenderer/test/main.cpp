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

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"

#include "../../PdfFile/PdfFile.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../DocxRenderer.h"
#include "../../Common/OfficeFileFormatChecker.h"

#include "TextCommandRenderer/TextCommandRenderer.h"

#include <fstream>

#ifdef TEST_FOR_HTML_RENDERER_TEXT
#include "../../DesktopEditor/graphics/pro/js/wasm/src/HTMLRendererText.h"
#endif

//#define LOAD_FILE_AS_BINARY
//#define TEST_XML_BOM

#ifdef TEST_XML_BOM
#include "../../DesktopEditor/xml/include/xmlutils.h"
#endif

int main(int argc, char *argv[])
{
#ifdef TEST_XML_BOM
	std::wstring sFileXmlSrc = L"PATH_TO_SRC_XML";
	std::wstring sFileXmlDst = L"PATH_TO_DST_XML";

	BYTE* pBufferXml = NULL;
	DWORD lBufferXmlLen = 0;
	NSFile::CFileBinary::ReadAllBytes(sFileXmlSrc, &pBufferXml, lBufferXmlLen);

	std::string sUtf8 = XmlUtils::GetUtf8FromFileContent(pBufferXml, lBufferXmlLen);
	std::wstring sUnicode = UTF8_TO_U(sUtf8);

	NSFile::CFileBinary::SaveToFile(sFileXmlDst, sUnicode, true);

	RELEASEARRAYOBJECTS(pBufferXml);
#endif

	CApplicationFontsWorker oWorker;
	oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
	oWorker.m_bIsNeedThumbnails = false;

    // oWorker.m_arAdditionalFolders.push_back(L"");

	if (!NSDirectory::Exists(oWorker.m_sDirectory))
		NSDirectory::CreateDirectory(oWorker.m_sDirectory);

	NSFonts::IApplicationFonts* pFonts = oWorker.Check();

	std::wstring sTempDir = NSFile::GetProcessDirectory() + L"/temp";
	std::wstring sTempDirOut = NSFile::GetProcessDirectory() + L"/temp/output";

	if (!NSDirectory::Exists(sTempDir))
		NSDirectory::CreateDirectory(sTempDir);
	if (!NSDirectory::Exists(sTempDirOut))
		NSDirectory::CreateDirectory(sTempDirOut);

	std::vector<std::wstring> sSourceFiles = NSDirectory::GetFiles(L"");
	//sSourceFiles.push_back(L"");

	std::wstring sTextDirOut = NSFile::GetProcessDirectory() + L"/output";
	if (!NSDirectory::Exists(sTextDirOut))
		NSDirectory::CreateDirectory(sTextDirOut);

	IOfficeDrawingFile* pReader = NULL;

	COfficeFileFormatChecker oChecker;
	int nFileType = 0;

	CDocxRenderer oDocxRenderer(pFonts);
	oDocxRenderer.SetTempFolder(sTempDirOut);

	for (size_t nIndex = 0; nIndex < sSourceFiles.size(); nIndex++)
	{
		// need to reset type so it's not always detected as OOXML (see checker).
		oChecker.nFileType = 0;
		if (oChecker.isOfficeFile(sSourceFiles[nIndex]))
		{
			nFileType = oChecker.nFileType;
			switch (nFileType)
			{
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
				pReader = new CPdfFile(pFonts);
				break;
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS:
				pReader = new CXpsFile(pFonts);
				break;
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU:
				pReader = new CDjVuFile(pFonts);
				break;
			default:
				break;
			}
		}

		if (!pReader)
			continue;

		pReader->SetTempDirectory(sTempDir);

#ifndef LOAD_FILE_AS_BINARY
		pReader->LoadFromFile(sSourceFiles[nIndex]);
#else
		BYTE* pFileBinary = NULL;
		DWORD nFileBinaryLen = 0;
		NSFile::CFileBinary::ReadAllBytes(sSourceFile, &pFileBinary, nFileBinaryLen);

		pReader->LoadFromMemory(pFileBinary, nFileBinaryLen);
#endif

#ifdef TEST_FOR_HTML_RENDERER_TEXT
		if (true)
		{
			int nPagesCount = pReader->GetPagesCount();

			NSHtmlRenderer::CHTMLRendererText oTextRenderer;
			for (int i = 0; i < nPagesCount; i++)
			{
				oTextRenderer.Init(pReader, 8);
				pReader->DrawPageOnRenderer(&oTextRenderer, i, NULL);
			}
		}
#else

		std::wstring sExtention = NSFile::GetFileExtention(sSourceFiles[nIndex]);
		std::wstring sFileNameWithExtention = NSFile::GetFileName(sSourceFiles[nIndex]);
		std::wstring sFileName = sFileNameWithExtention.substr(0, sFileNameWithExtention.size() - 1 - sExtention.size());
		std::wstring sDocx = L"/" + sFileName + L".docx";
		std::wstring sZip = L"/" + sFileName + L".zip";

		NSDocxRenderer::TextAssociationType taType;
		//taType = NSDocxRenderer::TextAssociationType::tatPlainLine;
		//taType = NSDocxRenderer::TextAssociationType::tatShapeLine;
		//taType = NSDocxRenderer::TextAssociationType::tatPlainParagraph;
		taType = NSDocxRenderer::TextAssociationType::tatParagraphToShape;

		NSDocxRenderer::IImageStorage* pExternalImagheStorage = NSDocxRenderer::CreateWasmImageStorage();
		//oDocxRenderer.SetExternalImageStorage(pExternalImagheStorage);

		oDocxRenderer.SetTextAssociationType(taType);
		oDocxRenderer.Convert(pReader, sTextDirOut+sDocx);

//		std::wstring test_txt_file = L"";
//		std::ofstream fin(test_txt_file);
//		auto shapes = oDocxRenderer.ScanPagePptx(pReader, 0);
//		for (auto& s : shapes)
//			fin << U_TO_UTF8(s);

		CTextCommandRenderer oTextCommandRenderer(pFonts);
		oTextCommandRenderer.Do(pReader);

#endif
		RELEASEOBJECT(pReader);
		RELEASEOBJECT(pExternalImagheStorage);
	}

	pFonts->Release();

#ifdef LOAD_FILE_AS_BINARY
	RELEASEARRAYOBJECTS(pFileBinary);
#endif

	return 0;
}
