/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

namespace ImageHelper
{
	struct __BITMAPINFOHEADER 
	{
		DWORD      biSize;
		LONG       biWidth;
		LONG       biHeight;
		WORD       biPlanes;
		WORD       biBitCount;
		DWORD      biCompression;
		DWORD      biSizeImage;
		LONG       biXPelsPerMeter;
		LONG       biYPelsPerMeter;
		DWORD      biClrUsed;
		DWORD      biClrImportant;
	};

	struct __BITMAPCOREHEADER
	{
        DWORD   bcSize;                 /* used to get to color table */
        WORD    bcWidth;
        WORD    bcHeight;
        WORD    bcPlanes;
        WORD    bcBitCount;
	};

	inline Global::_BlipType SaveImageToFileFromDIB(unsigned char* data, int size, const std::wstring& file_name)//without ext
	{
		Global::_BlipType result = Global::msoblipERROR;

		CBgraFrame oFrame;
		int offset = 0, biSizeImage = 0;

		__BITMAPINFOHEADER * header = (__BITMAPINFOHEADER*)data;
		if (!header) return result;

		result == Global::msoblipDIB;

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
			
			if (!oFrame.SaveFile(file_name + _T(".png"), 4/*CXIMAGE_FORMAT_PNG*/))
				result = Global::msoblipERROR;

			oFrame.put_Data(NULL);
		}
		else if (result == Global::msoblipWMF)
		{
			NSFile::CFileBinary file;
			if (file.CreateFileW(file_name + _T(".wmf")))
			{
				file.WriteFile((BYTE*)data, size);
				file.CloseFile();
			}
		}
		else if (biSizeImage > 0)
		{
			NSFile::CFileBinary file;
			if (file.CreateFileW(file_name + _T(".bmp")))
			{
				WORD vtType		= 0x4D42;				file.WriteFile((BYTE*)&vtType,	2);
				DWORD dwLen		= biSizeImage;			file.WriteFile((BYTE*)&dwLen,	4);
				DWORD dwRes		= 0;					file.WriteFile((BYTE*)&dwRes,	4);
				DWORD dwOffset	= 2;					file.WriteFile((BYTE*)&dwOffset, 4);
			
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
		FontTableXML( _T( "" ) ),	DocumentXML( _T( "" ) ), 
		StyleSheetXML( _T( "" ) ),	FootnotesXML ( _T( "" ) ),	NumberingXML( _T( "" ) ),
		CommentsXML( _T( "" ) ),	SettingsXML( _T( "" ) ),	CommandTableXML ( _T( "" ) )
	{
		m_strOutputPath = _pathOutput;
	}

	WordprocessingDocument::~WordprocessingDocument()
	{
	}

	void WordprocessingDocument::SaveDocument()
	{
		std::wstring pathWord = m_strOutputPath + FILE_SEPARATOR_STR + _T( "word" ) ;
		//OOX::CPath pathWord = CString(m_strOutputPath.c_str()) + FILE_SEPARATOR_STR + _T( "word" );
		//FileSystem::Directory::CreateDirectory( pathWord.GetPath() );
        NSDirectory::CreateDirectory( pathWord );

		WritePackage();

		//Write main content. (word directory)

		SaveToFile(pathWord, std::wstring( _T("document.xml" ) ),		DocumentXML );
		SaveToFile(pathWord, std::wstring( _T( "fontTable.xml" ) ),	FontTableXML );
		SaveToFile(pathWord, std::wstring( _T( "styles.xml" ) ),		StyleSheetXML );
		SaveToFile(pathWord, std::wstring( _T( "footnotes.xml" ) ),	FootnotesXML );
		SaveToFile(pathWord, std::wstring( _T( "endnotes.xml" ) ),		EndnotesXML );
		SaveToFile(pathWord, std::wstring( _T( "numbering.xml" ) ),	NumberingXML );
		SaveToFile(pathWord, std::wstring( _T( "comments.xml" ) ),		CommentsXML );
		SaveToFile(pathWord, std::wstring( _T( "settings.xml" ) ),		SettingsXML );
		SaveToFile(pathWord, std::wstring( _T( "customizations.xml" ) ),CommandTableXML );

		if (!ImagesList.empty())
		{
			std::wstring pathMedia = pathWord + FILE_SEPARATOR_STR + _T( "media" );
			//OOX::CPath pathMedia = pathWord + FILE_SEPARATOR_STR + _T( "media" );
			//FileSystem::Directory::CreateDirectory( pathMedia.GetPath() );

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
						std::wstring file_name = pathMedia + FILE_SEPARATOR_STR + _T("image") + FormatUtils::IntToWideString(i++);
						iter->blipType = ImageHelper::SaveImageToFileFromDIB(bytes, iter->data.size(), file_name);
					}
					else
					{
						SaveToFile(pathMedia, std::wstring(_T("image" )) + FormatUtils::IntToWideString(i++) + iter->ext, (void*)bytes, (unsigned int)iter->data.size());
					}
					
					RELEASEARRAYOBJECTS(bytes);
				}
			}
		}

		if (!OleObjectsList.empty())
		{
			std::wstring pathObjects = pathWord + FILE_SEPARATOR_STR + _T( "embeddings") ;
			//OOX::CPath pathObjects = pathWord + FILE_SEPARATOR_STR + _T( "embeddings" );
			//FileSystem::Directory::CreateDirectory( pathObjects.GetPath());
            NSDirectory::CreateDirectory( pathObjects );

			int i = 1;
			for (std::list<OleObjectFileStructure>::iterator iter = OleObjectsList.begin(); iter != OleObjectsList.end(); ++iter)
			{
				std::wstring fileName = pathObjects + FILE_SEPARATOR_STR  + _T( "oleObject" ) + FormatUtils::IntToWideString(i++) + iter->ext;

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
			SaveToFile(pathWord, ( std::wstring( _T( "header" ) ) + FormatUtils::IntToWideString(++headersCount) + std::wstring( _T( ".xml" ) ) ), *iter);
		}

		for (std::list<std::wstring>::iterator iter = FooterXMLList.begin(); iter != FooterXMLList.end(); ++iter)
		{
			SaveToFile(pathWord, ( std::wstring( _T( "footer" ) ) + FormatUtils::IntToWideString(++footersCount) + std::wstring( _T( ".xml" ) ) ), *iter);
		}
	}
}
