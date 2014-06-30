#include "stdafx.h"
#include "WordprocessingDocument.h"

#include "GdiplusEx.h"

namespace ImageHelper
{
	inline static int CompareStrings (const WCHAR* str1, const WCHAR* str2)
	{
		CString cstr1; cstr1 = str1;
		CString cstr2; cstr2 = str2;

		if (cstr1 == cstr2)
			return 0;

		return 1;
	}
	inline static void GetEncoderCLSID (const WCHAR* pFormat, CLSID* pClsid)
	{
		// variables
		UINT nEncoders = 0;
		UINT nSize = 0;
		Gdiplus::ImageCodecInfo* pImageCodecInfo = 0;

		// retrieve encoders info
		Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

		// check for valid encoders
		if (!nSize)
			throw 0;

		// create encoders info structure of necessary size
		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(nSize));

		// check for valid encoder
		if (!pImageCodecInfo)
			throw 0;

		// retrieve all encoders
		Gdiplus::GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

		// locate necessary encoder
		for (UINT nEncoder = 0; nEncoder < nEncoders; ++nEncoder)
		{
			// compare MIME strings
			if (CompareStrings(pImageCodecInfo[nEncoder].MimeType, pFormat) == 0)
			{
				// save CLSID
				*pClsid = pImageCodecInfo[nEncoder].Clsid;

				// clear memory
				free(pImageCodecInfo);

				// all ok
				return;
			}    
		}

		// clear memory
		free(pImageCodecInfo);

		// codec not found
		throw 0;
	}
	inline bool SaveImageToFileFromDIB(unsigned char* buffer, const std::wstring& file)
	{
		const BITMAPFILEHEADER* bmfh	=	(BITMAPFILEHEADER*)buffer;
		const BITMAPINFO* info			=	(BITMAPINFO*)buffer;

		if (NULL != bmfh && NULL != info)
		{
			HDC hdc = GetDC(NULL);
			if (hdc)
			{
				HBITMAP hbm = ::CreateDIBitmap(hdc, &info->bmiHeader, CBM_INIT, (buffer + sizeof(BITMAPINFO)), info, DIB_RGB_COLORS);
				if (hbm)
				{
					Gdiplus::Bitmap oBitmap (hbm, NULL);
					if (Gdiplus::Ok == oBitmap.GetLastStatus()) 
					{
						CLSID guid;
						GetEncoderCLSID (L"image/png", &guid);

						if (Gdiplus::Ok == oBitmap.Save (file.c_str(), &guid))
						{
							oBitmap.Save(file.c_str(), &guid);

							DeleteObject(hbm);
							ReleaseDC(NULL, hdc);

							return (Gdiplus::Ok == oBitmap.GetLastStatus());
						}						
					}

					DeleteObject(hbm);
				}

				ReleaseDC(NULL, hdc);
			}
		}

		return false;
	}
}

namespace DocFileFormat
{
	WordprocessingDocument::WordprocessingDocument(const WCHAR* _fileName, const WordDocument* _docFile) : OpenXmlPackage( _docFile ), FontTableXML( _T( "" ) ), DocumentXML( _T( "" ) ), 
		StyleSheetXML( _T( "" ) ), FootnotesXML ( _T( "" ) ), NumberingXML( _T( "" ) ),
		CommentsXML( _T( "" ) ), SettingsXML( _T( "" ) ), CommandTableXML ( _T( "" ) )
	{
		m_strFileName = std::wstring(_fileName);
	}

	WordprocessingDocument::~WordprocessingDocument()
	{
	}

	void WordprocessingDocument::SaveDocument()
	{
		GetOutputDir();

		size_t fileNameSize = m_strFileName.length() + 1;
		char *Buf = new char[fileNameSize];
		size_t convertedCout;
		wcstombs_s(&convertedCout, Buf, fileNameSize, m_strFileName.c_str(), fileNameSize);

#ifdef CREATE_ZIPPED_DOCX

		this->zf = zipOpen( Buf, 0 );

#endif  // CREATE_ZIPPED_DOCX

		RELEASEARRAYOBJECTS(Buf);

#ifdef CREATE_UNZIPPED_XMLS

		CreateDirectory( (m_strOutputPath + wstring( _T( "\\word" ) ) ).c_str(), NULL );

#endif //CREATE_UNZIPPED_XMLS

#ifdef CREATE_ZIPPED_DOCX
		WritePackage(this->zf);
		
		//Write main content. (word directory)
		
		SaveToFile(m_strOutputPath, wstring( _T( "word/document.xml" ) ), this->zf, this->DocumentXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/fontTable.xml" ) ), this->zf, this->FontTableXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/styles.xml" ) ), this->zf, this->StyleSheetXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/footnotes.xml" ) ), this->zf, this->FootnotesXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/endnotes.xml" ) ), this->zf, this->EndnotesXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/numbering.xml" ) ), this->zf, this->NumberingXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/comments.xml" ) ), this->zf, this->CommentsXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/settings.xml" ) ), this->zf, this->SettingsXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/customizations.xml" ) ), this->zf, this->CommandTableXML );
#else
		WritePackage();

		//Write main content. (word directory)

		SaveToFile(m_strOutputPath, wstring( _T( "word/document.xml" ) ), this->DocumentXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/fontTable.xml" ) ), this->FontTableXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/styles.xml" ) ), this->StyleSheetXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/footnotes.xml" ) ), this->FootnotesXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/endnotes.xml" ) ), this->EndnotesXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/numbering.xml" ) ), this->NumberingXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/comments.xml" ) ), this->CommentsXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/settings.xml" ) ), this->SettingsXML );
		SaveToFile(m_strOutputPath, wstring( _T( "word/customizations.xml" ) ), this->CommandTableXML );

#endif  // CREATE_ZIPPED_DOCX

		if (!this->ImagesList.empty())
		{
#ifdef CREATE_UNZIPPED_XMLS
			CreateDirectory((m_strOutputPath + wstring( _T( "\\word\\media" ) ) ).c_str(), NULL);
#endif //CREATE_UNZIPPED_XMLS

			int i = 1;

			for (list<ImageFileStructure>::iterator iter = ImagesList.begin(); iter != ImagesList.end(); ++iter)
			{
				byte* bytes = NULL;
				bytes = new byte[iter->data.size()];
				if (bytes) 
				{
					copy(iter->data.begin(), iter->data.end(), bytes);

					if (Global::msoblipDIB == iter->blipType)
					{
						std::wstring file = m_strOutputPath + std::wstring(_T("\\word\\media\\image")) + FormatUtils::IntToWideString(i++) + iter->ext;
						ImageHelper::SaveImageToFileFromDIB(bytes, file);
					}
					else
					{

#ifdef CREATE_ZIPPED_DOCX
						SaveToFile(m_strOutputPath, ( wstring( _T( "\\word\\media\\image" ) ) + FormatUtils::IntToWideString(i++) + iter->ext), this->zf, (void*)bytes, (unsigned int)iter->data.size());
#else
						SaveToFile(m_strOutputPath, ( wstring( _T( "\\word\\media\\image" ) ) + FormatUtils::IntToWideString(i++) + iter->ext), (void*)bytes, (unsigned int)iter->data.size());
#endif // CREATE_ZIPPED_DOCX						
					}
					
					RELEASEARRAYOBJECTS(bytes);
				}
			}
		}

		if (!this->OleObjectsList.empty())
		{
#ifdef CREATE_UNZIPPED_XMLS
			CreateDirectory((m_strOutputPath + wstring( _T( "\\word\\embeddings" ) ) ).c_str(), NULL);
#endif //CREATE_UNZIPPED_XMLS

			int i = 1;
			for (list<OleObjectFileStructure>::iterator iter = this->OleObjectsList.begin(); iter != this->OleObjectsList.end(); ++iter)
			{
				std::wstring fileName = ( this->m_strOutputPath + wstring( _T( "\\word\\embeddings\\oleObject" ) ) + FormatUtils::IntToWideString(i++) + iter->ext);

#ifdef CREATE_ZIPPED_DOCX
	#error NOTE: OLE OBJECTS SAVING NOT IMPLEMENTED 
#else
				SaveOLEObject( fileName, *iter );	
#endif // CREATE_ZIPPED_DOCX
			}
		}

		int headersCount = 0;
		int footersCount = 0;

		for (list<wstring>::iterator iter = this->HeaderXMLList.begin(); iter != this->HeaderXMLList.end(); ++iter)
		{
			SaveToFile(m_strOutputPath, ( wstring( _T( "word/header" ) ) + FormatUtils::IntToWideString(++headersCount) + wstring( _T( ".xml" ) ) ), *iter);
		}

		for (list<wstring>::iterator iter = this->FooterXMLList.begin(); iter != this->FooterXMLList.end(); ++iter)
		{
			SaveToFile(m_strOutputPath, ( wstring( _T( "word/footer" ) ) + FormatUtils::IntToWideString(++footersCount) + wstring( _T( ".xml" ) ) ), *iter);
		}

#ifdef CREATE_ZIPPED_DOCX

		int err = zipClose(this->zf, NULL);

#endif // CREATE_ZIPPED_DOCX
	}
}