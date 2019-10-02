/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

#include "WordprocessingDocument.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/SystemUtils.h"

#include "../../Common/DocxFormat/Source/DocxFormat/App.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Core.h"
#include "../../Common/DocxFormat/Source/DocxFormat/ContentTypes.h"

namespace DocFileFormat
{
	WordprocessingDocument::WordprocessingDocument(const std::wstring & _pathOutput, const WordDocument* _docFile) : 
		OpenXmlPackage( _docFile )
	{
		m_strOutputPath = _pathOutput;
	}

	WordprocessingDocument::~WordprocessingDocument()
	{
	}

	void WordprocessingDocument::SaveDocument(bool &bMacros)
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
				//output_document->get_xl_files().add_vba_project();
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
		OOX::CPath pathDocProps = m_strOutputPath + FILE_SEPARATOR_STR + _T("docProps");
		NSDirectory::CreateDirectory(pathDocProps.GetPath());
		
		OOX::CPath DocProps = std::wstring(_T("docProps"));

		OOX::CApp* pApp = new OOX::CApp(NULL);
		if (pApp)
		{
			std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
			if (sApplication.empty())
				sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
			pApp->SetApplication(sApplication);
	#if defined(INTVER)
			pApp->SetAppVersion(VALUE2STR(INTVER));
	#endif
			pApp->SetDocSecurity(0);
			pApp->SetScaleCrop(false);
			pApp->SetLinksUpToDate(false);
			pApp->SetSharedDoc(false);
			pApp->SetHyperlinksChanged(false);
			
			pApp->write(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"), DocProps, oContentTypes);
			delete pApp;
		}				
		OOX::CCore* pCore = new OOX::CCore(NULL);
		if (pCore)
		{
			pCore->SetCreator(_T(""));
			pCore->SetLastModifiedBy(_T(""));
			pCore->write(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"), DocProps, oContentTypes);
			delete pCore;
		} 
		RegisterDocPr();

		WritePackage();

		//Write main content. (word directory)

        SaveToFile(pathWord, std::wstring( L"document.xml" ),		DocumentXML );
        SaveToFile(pathWord, std::wstring( L"fontTable.xml" ),      FontTableXML );
        SaveToFile(pathWord, std::wstring( L"styles.xml" ),         StyleSheetXML );
        SaveToFile(pathWord, std::wstring( L"footnotes.xml" ),      FootnotesXML );
        SaveToFile(pathWord, std::wstring( L"endnotes.xml" ),		EndnotesXML );
        SaveToFile(pathWord, std::wstring( L"numbering.xml" ),      NumberingXML );
        SaveToFile(pathWord, std::wstring( L"comments.xml" ),		CommentsXML );
		SaveToFile(pathWord, std::wstring( L"commentsExtended.xml"),CommentsExtendedXML );
        SaveToFile(pathWord, std::wstring( L"settings.xml" ),		SettingsXML );
        SaveToFile(pathWord, std::wstring( L"customizations.xml" ), CommandTableXML );

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

				if (!iter->data.empty())
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
            SaveToFile(pathWord, ( std::wstring( L"header" ) + FormatUtils::IntToWideString(++headersCount) + std::wstring( L".xml" ) ), *iter);
		}

		for (std::list<std::wstring>::iterator iter = FooterXMLList.begin(); iter != FooterXMLList.end(); ++iter)
		{
            SaveToFile(pathWord, ( std::wstring( L"footer" ) + FormatUtils::IntToWideString(++footersCount) + std::wstring( L".xml" ) ), *iter);
		}
	}
}
