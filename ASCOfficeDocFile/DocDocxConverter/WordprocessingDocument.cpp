/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/common/Directory.h"

#include "../../Common/DocxFormat/Source/DocxFormat/App.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Core.h"
#include "../../Common/DocxFormat/Source/DocxFormat/ContentTypes.h"

namespace ImageHelper
{
    struct __BITMAPINFOHEADER
    {
        _UINT32     biSize;
        _INT32      biWidth;
        _INT32      biHeight;
        _UINT16     biPlanes;
        _UINT16     biBitCount;
        _UINT32     biCompression;
        _UINT32     biSizeImage;
        _INT32      biXPelsPerMeter;
        _INT32      biYPelsPerMeter;
        _UINT32     biClrUsed;
        _UINT32     biClrImportant;
    };

    struct __BITMAPCOREHEADER
    {
        _UINT32     bcSize;                 /* used to get to color table */
        _UINT16     bcWidth;
        _UINT16     bcHeight;
        _UINT16     bcPlanes;
        _UINT16     bcBitCount;
    };

	inline Global::_BlipType SaveImageToFileFromDIB(unsigned char* data, int size, const std::wstring& file_name)//without ext
	{
		Global::_BlipType result = Global::msoblipERROR;

		CBgraFrame oFrame;
		int offset = 0, biSizeImage = 0;

		__BITMAPINFOHEADER * header = (__BITMAPINFOHEADER*)data;
		if (!header) return result;

		result = Global::msoblipDIB;

		if (header->biWidth > 100000 || header->biHeight > 100000 || header->biSize != 40)
		{
			__BITMAPCOREHEADER * header_core = (__BITMAPCOREHEADER *)data;
			if (header_core->bcSize != 12)
			{
				result = Global::msoblipWMF;
			}
			else
			{
				offset = 12; //sizeof(BITMAPCOREHEADER)			
			
				oFrame.put_Height	(header_core->bcHeight );
				oFrame.put_Width	(header_core->bcWidth );
				
				int sz_bitmap = header_core->bcHeight * header_core->bcWidth * header_core->bcBitCount/ 8;
				
				//if (header_core->bcWidth % 2 != 0 && sz_bitmap < size - offset)
				//	header_core->bcWidth++;
				///???? todooo непонятно .. в biff5 нужно флипать картинку, в biff8 не ясно ( - 
				
				int stride =  -(size - offset) / header_core->bcHeight;
				oFrame.put_Stride	(stride/*header_core->bcBitCount * header_core->bcWidth /8 */);

				biSizeImage = size - offset;
				
				if (-stride >= header_core->bcWidth && header_core->bcBitCount >=24 )
				{
					result = Global::msoblipPNG;
				}
			}
		}
		else
		{
			offset = 40; //sizeof(BITMAPINFOHEADER)

			oFrame.put_Height	(header->biHeight );
			oFrame.put_Width	(header->biWidth );
			
			int sz_bitmap = header->biHeight * header->biWidth * header->biBitCount/ 8;
			
			//if (header->biWidth % 2 != 0 && sz_bitmap < size -offset)
			//	header->biWidth++;
			
			int stride = -(size - offset) / header->biHeight;

			if (-stride >= header->biWidth && header->biBitCount >= 24)
			{
				result = Global::msoblipPNG;
			}
			oFrame.put_Stride	(stride/*header->biBitCount * header->biWidth /8*/);
			
			biSizeImage = header->biSizeImage > 0 ? header->biSizeImage : (size - offset);
		}
		
//------------------------------------------------------------------------------------------

		if (result == Global::msoblipPNG)
		{
			oFrame.put_Data((unsigned char*)data + offset);
			
            if (!oFrame.SaveFile(file_name + L".png", 4/*CXIMAGE_FORMAT_PNG*/))
				result = Global::msoblipERROR;

			oFrame.put_Data(NULL);
		}
		else if (result == Global::msoblipWMF)
		{
			NSFile::CFileBinary file;
            if (file.CreateFileW(file_name + L".wmf"))
			{
				file.WriteFile((BYTE*)data, size);
				file.CloseFile();
			}
		}
		else if (biSizeImage > 0)
		{
			NSFile::CFileBinary file;
            if (file.CreateFileW(file_name + L".bmp"))
			{
                _UINT16 vtType		= 0x4D42;				file.WriteFile((BYTE*)&vtType,	2);
                _UINT32 dwLen		= biSizeImage;			file.WriteFile((BYTE*)&dwLen,	4);
                _UINT32 dwRes		= 0;					file.WriteFile((BYTE*)&dwRes,	4);
                _UINT32 dwOffset	= 2;					file.WriteFile((BYTE*)&dwOffset, 4);
			
				file.WriteFile((BYTE*)data, size);
				file.CloseFile();
			}
		}
		return result;
	}
}

namespace DocFileFormat
{
	WordprocessingDocument::WordprocessingDocument(const std::wstring & _pathOutput, const WordDocument* _docFile) : 
		OpenXmlPackage( _docFile ), 
        FontTableXML( L"" ),	DocumentXML( L"" ),
        StyleSheetXML( L"" ),	FootnotesXML ( L"" ),	NumberingXML( L"" ),
        CommentsXML( L"" ),     SettingsXML( L"" ),     CommandTableXML ( L"" )
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
			pApp->SetApplication(L"ONLYOFFICE");
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
        SaveToFile(pathWord, std::wstring( L"settings.xml" ),		SettingsXML );
        SaveToFile(pathWord, std::wstring( L"customizations.xml" ), CommandTableXML );

		if (!ImagesList.empty())
		{
            std::wstring pathMedia = pathWord + FILE_SEPARATOR_STR + L"media";
 
            NSDirectory::CreateDirectory(pathMedia);

			int i = 1;

			for (std::list<ImageFileStructure>::iterator iter = ImagesList.begin(); iter != ImagesList.end(); ++iter)
			{
				unsigned char* bytes = NULL;
				bytes = new unsigned char[iter->data.size()];
				if (bytes) 
				{
					copy(iter->data.begin(), iter->data.end(), bytes);

					if (Global::msoblipDIB == iter->blipType)
					{//user_manual_v52.doc						
                        std::wstring file_name = pathMedia + FILE_SEPARATOR_STR + L"image" + FormatUtils::IntToWideString(i++);
						iter->blipType = ImageHelper::SaveImageToFileFromDIB(bytes, iter->data.size(), file_name);
					}
					else
					{
                        SaveToFile(pathMedia, std::wstring(L"image" ) + FormatUtils::IntToWideString(i++) + iter->ext, (void*)bytes, (unsigned int)iter->data.size());
					}
					
					RELEASEARRAYOBJECTS(bytes);
				}
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
					SaveEmbeddedObject(fileName, iter->data);
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
