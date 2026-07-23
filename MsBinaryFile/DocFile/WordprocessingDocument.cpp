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

#include "WordprocessingDocument.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/SystemUtils.h"

#include "../../OOXML/DocxFormat/App.h"
#include "../../OOXML/DocxFormat/Core.h"
#include "../../OOXML/DocxFormat/ContentTypes.h"

namespace DocFileFormat
{
	WordprocessingDocument::WordprocessingDocument(const std::wstring & _pathOutput, const WordDocument* _docFile) : 
		OpenXmlPackage( _docFile )
	{
		m_strOutputPath = _pathOutput;
	}

	WordprocessingDocument::~WordprocessingDocument()
	{
		_isSectionPageBreak = 0;
	}

	_UINT32 WordprocessingDocument::SaveDocument(bool &bMacros)
	{
		std::wstring pathWord = m_strOutputPath + FILE_SEPARATOR_STR + L"word" ;
		NSDirectory::CreateDirectory( pathWord );

		if (bMacros && docFile->GetStorage()->isDirectory(L"Macros"))
		{
			std::wstring sVbaProjectFile = pathWord + FILE_SEPARATOR_STR + L"vbaProject.bin";

			POLE::Storage *storageVbaProject = new POLE::Storage(sVbaProjectFile.c_str());

			if ((storageVbaProject) && (storageVbaProject->open(true, true)))
			{			
				docFile->GetStorage()->copy(0, L"Macros/", storageVbaProject, false);

				storageVbaProject->close();
				delete storageVbaProject;

				RegisterDocumentMacros();
				RegisterVbaProject();	
			}
			else bMacros = false;
		}
		else 
			bMacros = false;

		if (!bMacros)
		{
			RegisterDocument();
		}
		OOX::CContentTypes oContentTypes;
		OOX::CPath pathDocProps = m_strOutputPath + FILE_SEPARATOR_STR + L"docProps";
		NSDirectory::CreateDirectory(pathDocProps.GetPath());
		
		OOX::CPath DocProps = std::wstring(L"docProps");

		if (docFile->m_sXmlApp.empty())
		{
			OOX::CApp* pApp = new OOX::CApp(NULL);
			if (pApp)
			{
				std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
				if (sApplication.empty())
					sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;

#if defined(INTVER)
				std::string s = VALUE2STR(INTVER);
				sApplication += L"/" + std::wstring(s.begin(), s.end());
#endif
				pApp->m_sApplication = sApplication;
				pApp->SetDefaults();

				pApp->write(pathDocProps + FILE_SEPARATOR_STR + L"app.xml", DocProps, oContentTypes);
				delete pApp;
			}
		}
		else
		{
			SaveToFile(pathDocProps.GetPath(), L"app.xml", docFile->m_sXmlApp);
		}
		if (docFile->m_sXmlCore.empty())
		{
			OOX::CCore* pCore = new OOX::CCore(NULL);
			if (pCore)
			{
				pCore->SetCreator(L"");
				pCore->SetLastModifiedBy(L"");
				pCore->write(pathDocProps + FILE_SEPARATOR_STR + L"core.xml", DocProps, oContentTypes);
				delete pCore;
			}
		}
		else
		{
			SaveToFile(pathDocProps.GetPath(), L"core.xml", docFile->m_sXmlCore);
		}
		RegisterDocPr();

		WritePackage();

		//Write main content. (word directory)

        SaveToFile(pathWord, L"document.xml",			DocumentXML );
        SaveToFile(pathWord, L"fontTable.xml",			FontTableXML );
        SaveToFile(pathWord, L"styles.xml",				StyleSheetXML );
        SaveToFile(pathWord, L"footnotes.xml",			FootnotesXML );
        SaveToFile(pathWord, L"endnotes.xml",			EndnotesXML );
        SaveToFile(pathWord, L"numbering.xml",			NumberingXML );
        SaveToFile(pathWord, L"comments.xml",			CommentsXML );
		SaveToFile(pathWord, L"commentsExtended.xml",	CommentsExtendedXML );
        SaveToFile(pathWord, L"settings.xml",			SettingsXML );
        SaveToFile(pathWord, L"customizations.xml",		CommandTableXML );

		if (!ImagesList.empty())
		{
            std::wstring pathMedia = pathWord + FILE_SEPARATOR_STR + L"media";
 
            NSDirectory::CreateDirectory(pathMedia);

			int i = 1;

			for (std::list<ImageFileStructure>::iterator iter = ImagesList.begin(); iter != ImagesList.end(); ++iter)
			{
				SaveToFile(pathMedia, std::wstring(L"image" ) + FormatUtils::IntToWideString(i++) + iter->ext, (void*)iter->data.get(), iter->size);
			}
		}

		if (!OleObjectsList.empty())
		{
            std::wstring pathObjects = pathWord + FILE_SEPARATOR_STR + L"embeddings" ;
            NSDirectory::CreateDirectory( pathObjects );

			int i = 1;
			for (std::list<OleObjectFileStructure>::iterator iter = OleObjectsList.begin(); iter != OleObjectsList.end(); ++iter)
			{
                std::wstring fileName = pathObjects + FILE_SEPARATOR_STR  + L"oleObject" + FormatUtils::IntToWideString(i++) + iter->ext;

				if (iter->data.second > 0)
				{
					SaveEmbeddedObject(fileName, *iter);
				}
				else
				{
					SaveOLEObject( fileName, *iter );	
				}
			}
		}

		int headersCount = 0;
		int footersCount = 0;

		for (std::list<std::wstring>::iterator iter = HeaderXMLList.begin(); iter != HeaderXMLList.end(); ++iter)
		{
            SaveToFile(pathWord, ( L"header" + FormatUtils::IntToWideString(++headersCount) + L".xml"), *iter);
		}

		for (std::list<std::wstring>::iterator iter = FooterXMLList.begin(); iter != FooterXMLList.end(); ++iter)
		{
            SaveToFile(pathWord, ( L"footer" + FormatUtils::IntToWideString(++footersCount) + L".xml" ), *iter);
		}
		return 0;
	}
}
