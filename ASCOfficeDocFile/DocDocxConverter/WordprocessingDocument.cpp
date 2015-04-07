
#include "WordprocessingDocument.h"

#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

namespace ImageHelper
{
	inline static int CompareStrings (const wchar_t* str1, const wchar_t* str2)
	{
		CString cstr1; cstr1 = str1;
		CString cstr2; cstr2 = str2;

		if (cstr1 == cstr2)
			return 0;

		return 1;
	}
	//inline static void GetEncoderCLSID (const wchar_t* pFormat, CLSID* pClsid)
	//{
	//	// variables
	//	UINT nEncoders = 0;
	//	UINT nSize = 0;
	//	Gdiplus::ImageCodecInfo* pImageCodecInfo = 0;

	//	// retrieve encoders info
	//	Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

	//	// check for valid encoders
	//	if (!nSize)
	//		throw 0;

	//	// create encoders info structure of necessary size
	//	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(nSize));

	//	// check for valid encoder
	//	if (!pImageCodecInfo)
	//		throw 0;

	//	// retrieve all encoders
	//	Gdiplus::GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

	//	// locate necessary encoder
	//	for (UINT nEncoder = 0; nEncoder < nEncoders; ++nEncoder)
	//	{
	//		// compare MIME strings
	//		if (CompareStrings(pImageCodecInfo[nEncoder].MimeType, pFormat) == 0)
	//		{
	//			// save CLSID
	//			*pClsid = pImageCodecInfo[nEncoder].Clsid;

	//			// clear memory
	//			free(pImageCodecInfo);

	//			// all ok
	//			return;
	//		}    
	//	}

	//	// clear memory
	//	free(pImageCodecInfo);

	//	// codec not found
	//	throw 0;
	//}
	//inline bool SaveImageToFileFromDIB(unsigned char* buffer, const std::wstring& file)
	//{
	//	const BITMAPFILEHEADER* bmfh	=	(BITMAPFILEHEADER*)buffer;
	//	const BITMAPINFO* info			=	(BITMAPINFO*)buffer;

	//	if (NULL != bmfh && NULL != info)
	//	{
	//		HDC hdc = GetDC(NULL);
	//		if (hdc)
	//		{
	//			HBITMAP hbm = ::CreateDIBitmap(hdc, &info->bmiHeader, CBM_INIT, (buffer + sizeof(BITMAPINFO)), info, DIB_RGB_COLORS);
	//			if (hbm)
	//			{
	//				Gdiplus::Bitmap oBitmap (hbm, NULL);
	//				if (Gdiplus::Ok == oBitmap.GetLastStatus()) 
	//				{
	//					CLSID guid;
	//					GetEncoderCLSID (L"image/png", &guid);

	//					if (Gdiplus::Ok == oBitmap.Save (file.c_str(), &guid))
	//					{
	//						oBitmap.Save(file.c_str(), &guid);

	//						DeleteObject(hbm);
	//						ReleaseDC(NULL, hdc);

	//						return (Gdiplus::Ok == oBitmap.GetLastStatus());
	//					}						
	//				}

	//				DeleteObject(hbm);
	//			}

	//			ReleaseDC(NULL, hdc);
	//		}
	//	}

	//	return false;
	//}
}

namespace DocFileFormat
{
	WordprocessingDocument::WordprocessingDocument(const wchar_t* _pathOutput, const WordDocument* _docFile) : OpenXmlPackage( _docFile ), FontTableXML( _T( "" ) ), DocumentXML( _T( "" ) ), 
		StyleSheetXML( _T( "" ) ), FootnotesXML ( _T( "" ) ), NumberingXML( _T( "" ) ),
		CommentsXML( _T( "" ) ), SettingsXML( _T( "" ) ), CommandTableXML ( _T( "" ) )
	{
		m_strOutputPath = std::wstring(_pathOutput);
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

		SaveToFile(string2std_string(pathWord.GetPath()), wstring( _T("document.xml" ) ),		DocumentXML );
		SaveToFile(string2std_string(pathWord.GetPath()), wstring( _T( "fontTable.xml" ) ),		FontTableXML );
		SaveToFile(string2std_string(pathWord.GetPath()), wstring( _T( "styles.xml" ) ),		StyleSheetXML );
		SaveToFile(string2std_string(pathWord.GetPath()), wstring( _T( "footnotes.xml" ) ),		FootnotesXML );
		SaveToFile(string2std_string(pathWord.GetPath()), wstring( _T( "endnotes.xml" ) ),		EndnotesXML );
		SaveToFile(string2std_string(pathWord.GetPath()), wstring( _T( "numbering.xml" ) ),		NumberingXML );
		SaveToFile(string2std_string(pathWord.GetPath()), wstring( _T( "comments.xml" ) ),		CommentsXML );
		SaveToFile(string2std_string(pathWord.GetPath()), wstring( _T( "settings.xml" ) ),		SettingsXML );
		SaveToFile(string2std_string(pathWord.GetPath()), wstring( _T( "customizations.xml" ) ),CommandTableXML );

		if (!ImagesList.empty())
		{
			OOX::CPath pathMedia = pathWord + FILE_SEPARATOR_STR + _T( "media" );
		
			FileSystem::Directory::CreateDirectory( pathMedia.GetPath() );

			int i = 1;

			for (list<ImageFileStructure>::iterator iter = ImagesList.begin(); iter != ImagesList.end(); ++iter)
			{
				unsigned char* bytes = NULL;
				bytes = new unsigned char[iter->data.size()];
				if (bytes) 
				{
					copy(iter->data.begin(), iter->data.end(), bytes);

					if (Global::msoblipDIB == iter->blipType)
					{
						std::wstring file = string2std_string(pathMedia.GetPath()) + FILE_SEPARATOR_STR + _T("image") + FormatUtils::IntToWideString(i++) + iter->ext;
						//ImageHelper::SaveImageToFileFromDIB(bytes, file);

						//todooo ... переписать универсально под any system
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
			for (list<OleObjectFileStructure>::iterator iter = OleObjectsList.begin(); iter != OleObjectsList.end(); ++iter)
			{
				std::wstring fileName = string2std_string(pathObjects.GetPath()) + FILE_SEPARATOR_STR  + _T( "oleObject" ) + FormatUtils::IntToWideString(i++) + iter->ext;

				SaveOLEObject( fileName, *iter );	
			}
		}

		int headersCount = 0;
		int footersCount = 0;

		for (list<wstring>::iterator iter = HeaderXMLList.begin(); iter != HeaderXMLList.end(); ++iter)
		{
			SaveToFile(string2std_string(pathWord.GetPath()), ( wstring( _T( "header" ) ) + FormatUtils::IntToWideString(++headersCount) + wstring( _T( ".xml" ) ) ), *iter);
		}

		for (list<wstring>::iterator iter = FooterXMLList.begin(); iter != FooterXMLList.end(); ++iter)
		{
			SaveToFile(string2std_string(pathWord.GetPath()), ( wstring( _T( "footer" ) ) + FormatUtils::IntToWideString(++footersCount) + wstring( _T( ".xml" ) ) ), *iter);
		}
	}
}
