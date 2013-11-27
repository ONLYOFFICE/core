#include "../HTMLReaderLib/LiteHTMLReader.h"
#include "../HTMLReaderLib/LiteHTMLConverter.h"
#include "Windows.h"

static const TCHAR *pcRootHtml = _T("<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">");
static const TCHAR *pcXmlSubtag = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
static const TCHAR *pcDoctypeSubtag = _T("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
static const TCHAR *pcHtmlTagStart = _T("<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">");
static const TCHAR *pcHtmlTagEnd = _T("</html>");
static const TCHAR *pcTagEnd = _T(">");


CLiteHTMLElemAttr::CNamedColors CLiteHTMLElemAttr::_namedColors(166 /* block size */);
CLiteHTMLConverter::CTagsInfo CLiteHTMLConverter::m_mTagsInfo(200 /* block size */);


class CXHTMLWriter: public IXHTMLWriter
{
private:
	CString m_sOutput;
	HANDLE m_hFile;
	DWORD	m_dwErrorCode;
	BOOL	m_bLastTagClosed;

public:
	CXHTMLWriter ()
		: m_hFile (NULL)
		, m_dwErrorCode (0)
		, m_bLastTagClosed (TRUE)
	{
	}
	virtual ~CXHTMLWriter ()
	{
		CloseOutput ();
	}
protected:

	// preprocess <img> tag
	BOOL PreprocessImg (CHTMLTagWrapper *pTagWrapper)
	{
		/*
		CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
		CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();
		
		for (int nAttr = 0; nAttr < pAttrs->getCount(); nAttr++)
		{

			CLiteHTMLElemAttr oAttr = pAttrs->getAttribute(nAttr);
			CString sAttrName = oAttr.getName();
			CString sAttrValue = oAttr.getValue();

			if (sAttrName.GetLength() <= 0 || sAttrName.Find(_T(":")) != -1 || sAttrValue.GetLength() <= 0)
				continue;

			sAttrName.MakeLower();

			if (sAttrName = _T("src"))
			{
				CString sValueEncode = sAttrValue;
				HtmlUtils::DecodeUrl (sAttrValue);
				CString sResultPath = _T("");
				CString sFromRelativePath = HtmlUtils::full_from_relative( (const std::wstring) m_sOutput.GetBuffer(), (const std::wstring) sAttrValue.GetBuffer() );

				if (HtmlUtils::FileExist (&sAttrValue))
				{
					sResultPath = sAttrValue;
				}
				else if (HtmlUtils::FileExist(&sFromRelativePath) )
				{
					sResultPath = sFromRelativePath;
				}
				else
				{
					CString sPath = sValueEncode;
					if ( m_bDownloadImages && ( -1 != sPath.Find( _T("www") ) || -1 != sPath.Find( _T("http") ) || -1 != sPath.Find( _T("ftp") ) ) ) 
					{
						CFileDownloader oDownloader (sPath, FALSE);
						oDownloader.StartWork (1);
						while ( oDownloader.IsRunned() )
						{
							::Sleep( 10 );
						}

						if ( oDownloader.IsFileDownloaded() )
						{
							sResultPath = oDownloader.GetFilePath().GetBuffer();
							m_arrImageFileName.Add( oDownloader.GetFilePath() );
						}
						else
							sResultPath = _T("");
					}
				}
				if (!sResultPath.IsEmpty())
				{
					// check width and height
					CSS::CHtmlCSS oTempCSS(_T(""), pAttrs->getValueFromName(_T("style")));
					
					CSS::CHtmlCSSProperty *pProperty = NULL;
					if (NULL != (pProperty = oTempCss.get_Property (CSS::ptWidth)))	// width property
					{
					}
				}
			}
		}
		*/
	}
	BOOL PreprocessA (CHTMLTagWrapper *pTagWrapper)
	{}
	BOOL PreprocessFrame (CHTMLTagWrapper *pTagWrapper)
	{}
	BOOL PreprocessIframe (CHTMLTagWrapper *pTagWrapper)
	{}
	BOOL ProcessTag (CHTMLTagWrapper *pTagWrapper)
	{
		// 
		ATLASSERT (pTagWrapper);

		CLiteHTMLTag *pLiteTag = pTagWrapper->Tag(); 
		
		// Preprocess specific tags (Step 1)
		CString sTagName = pLiteTag->getTagName();
		if (_T("img") == sTagName)
			PreprocessImg (pTagWrapper);
		else if (_T("a") == sTagName)
			PreprocessA (pTagWrapper);
		else if (_T("frame") == sTagName)
			PreprocessFrame (pTagWrapper);
		else if (_T("iframe") == sTagName)
			PreprocessIframe (pTagWrapper);

		// 

	}
protected:

	BOOL WriteMultiByte (const BYTE *aBuffer, DWORD aSize, UINT aCodePage)
	{
		if (NULL == m_hFile)
			return FALSE;
		
		// convert multibyte to widechar
		if (CP_UTF8 == aCodePage)
			return WriteUTF8 ((const char*) aBuffer, aSize);

		const int nWideCharSize = MultiByteToWideChar(aCodePage,         // code page
		  0,										// character-type options
		  (LPCSTR) aBuffer, // string to map
		  aSize,       // number of bytes in string
		  NULL,  // wide-character buffer
		  0        // size of buffer
		);

		if (0 == nWideCharSize ||
			ERROR_NO_UNICODE_TRANSLATION == nWideCharSize)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteMultiByte() failed! (MultiByteToWideChar 0x%x)", GetLastError());
			return FALSE;
		}
		wchar_t *aWideChars = new wchar_t [nWideCharSize];
		if (NULL == aWideChars)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteMultiByte() failed! (no memory 1)");
			return FALSE;
		}

		const int nMultiByteToWideCharResult = MultiByteToWideChar(aCodePage,         // code page
		  0,										// character-type options
		  (LPCSTR) aBuffer, // string to map
		  aSize,       // number of bytes in string
		  aWideChars,  // wide-character buffer
		  nWideCharSize// size of buffer
		);
		if (0 == nWideCharSize ||
			ERROR_NO_UNICODE_TRANSLATION == nWideCharSize)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteMultiByte() failed! (0x%x)", GetLastError());
			delete [] aWideChars;
			return FALSE;
		}

		// convert to UTF-8
		const int nUTF8Size = WideCharToMultiByte (CP_UTF8, 0, aWideChars, nWideCharSize, NULL, 0, NULL, NULL);
		if (0 == nUTF8Size || ERROR_INSUFFICIENT_BUFFER == nUTF8Size)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteMultiByte() failed! (WideCharToMultiByte)");
			delete [] aWideChars;
			return FALSE;
		}
		BYTE*  aUTF8String = new BYTE [nUTF8Size+1];
		if (NULL == aUTF8String)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteMultiByte() failed! (no memory 2)");
			delete [] aWideChars;
			return FALSE;
		}
		aUTF8String[nUTF8Size] = 0x00;
		const int nWideCharToUTF8Result = WideCharToMultiByte (CP_UTF8, 0, aWideChars, nWideCharSize, (LPSTR) aUTF8String, nUTF8Size, NULL, NULL);
		delete [] aWideChars;
		if (0 == nWideCharToUTF8Result || ERROR_INSUFFICIENT_BUFFER == nWideCharToUTF8Result)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteMultiByte() failed! (WideCharToMultiByte)");
			
			delete [] aUTF8String;
			return FALSE;
		}
		
		BOOL bRes = WriteUTF8 ((char *) aUTF8String, nUTF8Size);
		delete [] aUTF8String;
		return bRes;
	}
	BOOL WriteUTF8 (const char *aBuffer, DWORD aSize)
	{
		if (NULL == m_hFile)
			return FALSE;

		DWORD dwWritten (0);
		const BOOL bRes = ::WriteFile (m_hFile, aBuffer, aSize, &dwWritten, NULL);
		if (0 == bRes)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteUTF8() failed! (0x%x)", GetLastError());
		}
		return bRes;
	}
	BOOL WriteUnicode (CStringW aBuffer)
	{
		if (NULL == m_hFile)
			return FALSE;

		// convert to UTF-8
		LPCWSTR pBuffer = aBuffer.GetBuffer();
		const int nUTF8Size = WideCharToMultiByte (CP_UTF8, 0, pBuffer, -1, NULL, 0, NULL, NULL);
		if (0 == nUTF8Size || ERROR_INSUFFICIENT_BUFFER == nUTF8Size)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteUnicode() failed! (WideCharToMultiByte)");
			aBuffer.ReleaseBuffer();
			return FALSE;
		}
		BYTE*  aUTF8String = new BYTE [nUTF8Size+1];
		if (NULL == aUTF8String)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteUnicode() failed! (no memory 2)");
			aBuffer.ReleaseBuffer();
			return FALSE;
		}
		aUTF8String[nUTF8Size] = 0x00;
		const int nWideCharToUTF8Result = WideCharToMultiByte (CP_UTF8, 0, pBuffer, -1, (LPSTR) aUTF8String, nUTF8Size, NULL, NULL);
		if (0 == nWideCharToUTF8Result || ERROR_INSUFFICIENT_BUFFER == nWideCharToUTF8Result)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteUnicode() failed! (WideCharToMultiByte)");
			delete [] aUTF8String;
			aBuffer.ReleaseBuffer();
			return FALSE;
		}
		aBuffer.ReleaseBuffer();
		
		BOOL bRes = WriteUTF8 ((char *) aUTF8String, nUTF8Size);
		delete [] aUTF8String;
		return bRes;
	}
	BOOL WriteString (CString aBuffer)
	{
		if (NULL == m_hFile)
			return FALSE;

		BOOL bRes = TRUE;

		#ifdef UNICODE
			bRes = WriteUnicode (aBuffer);
		#else
			bRes = WriteMultiByte (aBuffer);
		#endif

		if (FALSE == bRes)
		{
			ATLTRACE2 ("CXHTMLWriter::WriteString() failed! (0x%x)", GetLastError());
		}
		return bRes;
	}

	virtual void CloseOutput ()
	{
		if (NULL != m_hFile)
		{
			CloseHandle (m_hFile);
			m_hFile = NULL;
		}
	}


	BOOL CloseTagPrefix (CHTMLTagWrapper *pPrevTagWrapper, bool aCloseTag = false)
	{
		DWORD dwState = pPrevTagWrapper->State ();
		
		BOOL bRes = TRUE;

		// a tag is already closed
		if (TAG_STATE_CLOSED & dwState)
			return TRUE;

		// a tag has child tags - we must only close a prefix
		if (TAG_STATE_PREFIX_CLOSED & dwState)
		{
		}
		else
		{
			if ((TAG_STATE_HAS_CHILDTAG & dwState) || (TAG_STATE_HAS_TEXTS & dwState))
			{
				// close prefix (">")
				dwState |= TAG_STATE_PREFIX_CLOSED;
				pPrevTagWrapper->State (dwState);
				bRes &= WriteString (pcTagEnd);
			}
		}

		// we need to close tag
		if (aCloseTag)
		{
			if ((TAG_STATE_HAS_CHILDTAG & dwState) || (TAG_STATE_HAS_TEXTS & dwState))
			{
				// write close tag
				CString sTagPostFix = _T("</");
				sTagPostFix += pPrevTagWrapper->Tag()->getTagName();
				sTagPostFix += pcTagEnd;

				bRes &= WriteString (sTagPostFix);
				dwState |= TAG_STATE_CLOSED;
			}
			else 
			{
				// inline close
				CString sTagPostFix = _T("/>");
				bRes &= WriteString (sTagPostFix);
				dwState |= TAG_STATE_CLOSED;
			}
		}


		// 
		return TRUE;
	}

	// IXHTMLWriter impl
	virtual DWORD ErrorCode ()
	{
		return m_dwErrorCode;
	}
	virtual void SetOutput (CString &aOut)
	{
		m_sOutput = aOut;
	}
	virtual BOOL BeginFile ()
	{
		// write strict xhtml
		m_dwErrorCode = 0;
		CloseOutput ();
		m_hFile = ::CreateFile (m_sOutput, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			ATLTRACE2 (_T("Can't open file!! (last error: 0x%x)"), GetLastError());
			m_hFile = NULL;
			m_dwErrorCode = 1;
			return FALSE;
		}

		// write header
		BOOL bRes = TRUE;
		bRes &= WriteString (pcXmlSubtag);
		bRes &= WriteString (pcDoctypeSubtag);
		bRes &= WriteString (pcHtmlTagStart);

		return bRes;
	}
	virtual BOOL StartTag (CHTMLTagWrapper *pTagWrapper, CHTMLTagWrapper *pPrevTagWrapper = NULL)
	{
		if (NULL == pTagWrapper)
			return FALSE;

		BOOL bRes = TRUE;

		// close last tag prefix
		if (NULL != pPrevTagWrapper)
		{
			// close prefix in prev tag or close tag
			DWORD dwState = pPrevTagWrapper->State ();
			dwState |= TAG_STATE_HAS_CHILDTAG;
			pPrevTagWrapper->State (dwState);
			CloseTagPrefix (pPrevTagWrapper);
		}

		CLiteHTMLTag *pTag = pTagWrapper->Tag();
		if (NULL == pTag)
		{
			return FALSE;
		}

		DWORD dwState = TAG_STATE_OPENED;
		
		// write tag prefix
		CString sTagPrefix = _T("<");
		sTagPrefix += pTag->getTagName();
		bRes &= WriteString (sTagPrefix);

		// write attributes
		const CLiteHTMLAttributes* pAttrs = pTag->getAttributes();
		if (NULL != pAttrs)
		{
			const int iAttrCount = pAttrs->getCount();
			if (iAttrCount > 0)
			{
				sTagPrefix = _T(" ");
				bRes &= WriteString (sTagPrefix);
				dwState |= TAG_STATE_HAS_ATTR;
			}
			for (int iAttr = 0; iAttr < iAttrCount; iAttr++)
			{
				// write attr
				CLiteHTMLElemAttr& oAttr = pAttrs->getAttribute(iAttr);
				CString& sAttrName = oAttr.getName();
				CString& sAttrValue = oAttr.getValue ();

				CString sAttrPair = sAttrName;
				sAttrPair += _T("=\"");
				sAttrPair += sAttrValue;
				sAttrPair += _T("\" ");
				bRes &= WriteString (sAttrPair);			

				// 
			}
		}
		pTagWrapper->State (dwState);
				
		return bRes;
	}
	virtual BOOL EndTag (CHTMLTagWrapper *pTagWrapper)
	{
		if (NULL == pTagWrapper)
			return FALSE;

		BOOL bRes = TRUE;

		/*
		CLiteHTMLTag *pTag = pTagWrapper->Tag();
		if (NULL == pTag)
		{
			return FALSE;
		}

		
		// write attributes
		size_t nTextsCount = pTag->getTextsCount();
		if (0 == nTextsCount && pTag->IsTagInline())
		{
			CString sTagPostFix = _T("/>\n");
			bRes &= WriteUTF8 (sTagPostFix, _tcslen (sTagPostFix));
			m_bLastTagClosed = TRUE;
			return bRes;
		}

		// WriteTexts
		for (int nText = 0; nText < nTextsCount; nText++)
		{
			CString sText = pTag->getTextAt (nText);
			if (sText == _T("92619"))
			{
				ATLTRACE2 ("trololo\n");
			}

			bRes &= WriteUTF8 (sText, _tcslen (sText));
		}
		*/

		// close tag
		bRes &= CloseTagPrefix (pTagWrapper, true);
		return bRes;
	}
	virtual BOOL EndFile ()
	{
		// TODO: check all tags closed

		// close html tag
		BOOL bRes = TRUE;
		//bRes &= WriteUTF8 (pcHtmlTagEnd, _tcslen (pcHtmlTagEnd));
		CloseOutput ();
		return bRes;
	}
	virtual void Characters (CHTMLTagWrapper *pTagWrapper, CString aText)
	{
		if (NULL == pTagWrapper)
			return;
		

		DWORD dwState = pTagWrapper->State ();
		dwState |= TAG_STATE_HAS_TEXTS;
		pTagWrapper->State (dwState);

		CloseTagPrefix (pTagWrapper);

		// write text
		if (!aText.IsEmpty())
			WriteString (aText);
	}
};



// test - convert html to xml
int _tmain (int argc, TCHAR *argv[ ], TCHAR *envp[ ])
{
	if (argc < 3)
	{
		ATLTRACE2 ("needed parameters not found!");
		return -1;
	}
	// remove "" from begin and end of string
	CString sInputFileName = argv[1];
	CString sOutputFileName = argv[2];


	// Opens input file for reading
	HANDLE hInputFile = ::CreateFile (sInputFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hInputFile)
	{
		ATLTRACE2 ("Can't open file!! (last error: 0x%x)", GetLastError());
		return -2;	
	}

	// Create converter object
	CLiteHTMLConverter oConverter;
	IXHTMLWriter *pWriter = new CXHTMLWriter ();
	pWriter->SetOutput (sOutputFileName);
	oConverter.ToXHTML (hInputFile, pWriter);

	CloseHandle (hInputFile);
	delete pWriter;


	return 0;
}