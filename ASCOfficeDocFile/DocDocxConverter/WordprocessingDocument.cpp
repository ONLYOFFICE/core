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

#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "../../DesktopEditor/raster/BgraFrame.h"

namespace ImageHelper
{
	typedef struct ___tagBITMAPINFOHEADER 
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
	} ___BITMAPINFOHEADER;

	inline static int CompareStrings (const wchar_t* str1, const wchar_t* str2)
	{
		CString cstr1; cstr1 = str1;
		CString cstr2; cstr2 = str2;

		if (cstr1 == cstr2)
			return 0;

		return 1;
	}
	inline bool SaveImageToFileFromDIB(unsigned char* buffer, int size, const std::wstring& file)
	{
		bool result = false;
		const ___BITMAPINFOHEADER* info =	(___BITMAPINFOHEADER*)buffer;

		if (NULL != info && info->biSize == 40)
		{
			unsigned char* pBgraData = buffer + sizeof(___BITMAPINFOHEADER);
			
			int nWidth	= info->biWidth;
			int nHeight = info->biHeight;

			CBgraFrame oFrame;
			oFrame.put_Data		(pBgraData);
			oFrame.put_Width	(nWidth);
			oFrame.put_Height	(nHeight);

			int nStride = info->biSizeImage / nHeight;
			oFrame.put_Stride( -nStride );

			result = oFrame.SaveFile(file, 4);

			oFrame.put_Data(NULL);

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
		OOX::CPath pathWord = CString(m_strOutputPath.c_str()) + FILE_SEPARATOR_STR + _T( "word" );
		FileSystem::Directory::CreateDirectory( pathWord.GetPath() );


		WritePackage();

		//Write main content. (word directory)

		SaveToFile(string2std_string(pathWord.GetPath()), std::wstring( _T("document.xml" ) ),		DocumentXML );
		SaveToFile(string2std_string(pathWord.GetPath()), std::wstring( _T( "fontTable.xml" ) ),		FontTableXML );
		SaveToFile(string2std_string(pathWord.GetPath()), std::wstring( _T( "styles.xml" ) ),		StyleSheetXML );
		SaveToFile(string2std_string(pathWord.GetPath()), std::wstring( _T( "footnotes.xml" ) ),		FootnotesXML );
		SaveToFile(string2std_string(pathWord.GetPath()), std::wstring( _T( "endnotes.xml" ) ),		EndnotesXML );
		SaveToFile(string2std_string(pathWord.GetPath()), std::wstring( _T( "numbering.xml" ) ),		NumberingXML );
		SaveToFile(string2std_string(pathWord.GetPath()), std::wstring( _T( "comments.xml" ) ),		CommentsXML );
		SaveToFile(string2std_string(pathWord.GetPath()), std::wstring( _T( "settings.xml" ) ),		SettingsXML );
		SaveToFile(string2std_string(pathWord.GetPath()), std::wstring( _T( "customizations.xml" ) ),CommandTableXML );

		if (!ImagesList.empty())
		{
			OOX::CPath pathMedia = pathWord + FILE_SEPARATOR_STR + _T( "media" );
		
			FileSystem::Directory::CreateDirectory( pathMedia.GetPath() );

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
						std::wstring file_name = string2std_string(pathMedia.GetPath()) + FILE_SEPARATOR_STR + _T("image") + FormatUtils::IntToWideString(i++) + iter->ext;
						ImageHelper::SaveImageToFileFromDIB(bytes, iter->data.size(), file_name);
					}
					else
					{
						SaveToFile(string2std_string(pathMedia.GetPath()), std::wstring(_T("image" )) + FormatUtils::IntToWideString(i++) + iter->ext, (void*)bytes, (unsigned int)iter->data.size());
					}
					
					RELEASEARRAYOBJECTS(bytes);
				}
			}
		}

		if (!OleObjectsList.empty())
		{
			OOX::CPath pathObjects = pathWord + FILE_SEPARATOR_STR + _T( "embeddings" );
		
			FileSystem::Directory::CreateDirectory( pathObjects.GetPath());

			int i = 1;
			for (std::list<OleObjectFileStructure>::iterator iter = OleObjectsList.begin(); iter != OleObjectsList.end(); ++iter)
			{
				std::wstring fileName = string2std_string(pathObjects.GetPath()) + FILE_SEPARATOR_STR  + _T( "oleObject" ) + FormatUtils::IntToWideString(i++) + iter->ext;

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
			SaveToFile(string2std_string(pathWord.GetPath()), ( std::wstring( _T( "header" ) ) + FormatUtils::IntToWideString(++headersCount) + std::wstring( _T( ".xml" ) ) ), *iter);
		}

		for (std::list<std::wstring>::iterator iter = FooterXMLList.begin(); iter != FooterXMLList.end(); ++iter)
		{
			SaveToFile(string2std_string(pathWord.GetPath()), ( std::wstring( _T( "footer" ) ) + FormatUtils::IntToWideString(++footersCount) + std::wstring( _T( ".xml" ) ) ), *iter);
		}
	}
}
