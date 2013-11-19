#pragma once

#ifndef __AFXTEMPL_H__
#	include <afxtempl.h>
#endif	// !__AFXTEMPL_H__


#include "LiteHTMLReader.h"
#include "HTMLTagWrapper.h"

class IXHTMLWriter
{
public:
	virtual BOOL BeginFile () = 0;
	virtual BOOL StartTag (CHTMLTagWrapper *pTag, CHTMLTagWrapper *pPrevTag) = 0;
	virtual BOOL EndTag (CHTMLTagWrapper *pTag) = 0;
	virtual void Characters (CHTMLTagWrapper *pTagWrapper, CString aText) = 0;
	virtual BOOL EndFile () = 0;
	virtual void SetOutput (CString &aOut) = 0;
	virtual DWORD ErrorCode () = 0;
};

static const char *pcRootHtml = "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">";
static const char *pcXmlSubtag = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
static const char *pcDoctypeSubtag = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">";
static const char *pcHtmlTagStart = "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">";
static const char *pcHtmlTagEnd = "</html>";
static const char *pcTagEnd = ">";



class CXHTMLWriter: public IXHTMLWriter
{
private:
	
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
				bRes &= WriteUTF8 (pcTagEnd, strlen (pcTagEnd));
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

				bRes &= WriteUTF8 (sTagPostFix, strlen (sTagPostFix));
				dwState |= TAG_STATE_CLOSED;
			}
			else 
			{
				// inline close
				CString sTagPostFix = _T("/>");
				bRes &= WriteUTF8 (sTagPostFix, strlen (sTagPostFix));
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
		//bRes &= WriteUTF8 (pcXmlSubtag, strlen (pcXmlSubtag));
		//bRes &= WriteUTF8 (pcDoctypeSubtag, strlen (pcDoctypeSubtag));
		//bRes &= WriteUTF8 (pcHtmlTagStart, strlen (pcHtmlTagStart));

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
		bRes &= WriteUTF8 (sTagPrefix, strlen (sTagPrefix));

		// write attributes
		const CLiteHTMLAttributes* pAttrs = pTag->getAttributes();
		if (NULL != pAttrs)
		{
			const int iAttrCount = pAttrs->getCount();
			if (iAttrCount > 0)
			{
				sTagPrefix = _T(" ");
				bRes &= WriteUTF8 (sTagPrefix, strlen (sTagPrefix));
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
				bRes &= WriteUTF8 (sAttrPair, strlen (sAttrPair));			

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
			bRes &= WriteUTF8 (sTagPostFix, strlen (sTagPostFix));
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

			bRes &= WriteUTF8 (sText, strlen (sText));
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
		//bRes &= WriteUTF8 (pcHtmlTagEnd, strlen (pcHtmlTagEnd));
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
			WriteUTF8 (aText, strlen (aText));
	}
};