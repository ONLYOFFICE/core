#include "stdafx.h"
#include "LiteHTMLReader.h"
#include <exception>
#include "LiteHTMLEntityResolver.h"
#include "UnicodeTextFile.h"


#pragma warning(push, 4)

// helper - delete specific tag from html string

void CLiteHTMLReader::DeleteComments (CString &sHtml)
{
	int iCommentStart (0);
	while (true)
	{
		iCommentStart = sHtml.Find (_T("<!--"), iCommentStart);
		if (-1 == iCommentStart)
			break;

		const int iCommentEnd = sHtml.Find (_T("-->"), iCommentStart + 4); // length of "<!--"
		if (-1 != iCommentEnd)
		{
			sHtml.Delete (iCommentStart, iCommentEnd - iCommentStart + 3);	// lenght of '-->'
		}
		else
		{
			sHtml.Delete (iCommentStart, sHtml.GetLength() - iCommentStart); // delete to end of file
		}
	}
}
void CLiteHTMLReader::DeleteTags (CString &sHtml, CString sTagName)
{
	const int sTagLength  = sTagName.GetLength();

	CString sTagPrefixOpened = _T("<");	
	sTagPrefixOpened += sTagName;		// "<tagname"

	CString sTagClosed = _T("</");
	sTagClosed += sTagName;
	sTagClosed += _T(">");

	int iScriptStart = 0;
	// delete script
	while (true)
	{
		iScriptStart = sHtml.Find (sTagPrefixOpened, iScriptStart);
		if (-1 == iScriptStart)
			break;

		const int iScriptEnd = sHtml.Find (sTagClosed, iScriptStart + sTagLength + 1); // length of "<" + tagLenth
		if (-1 == iScriptEnd)
		{
			const int iScriptPrefixEnd = sHtml.Find (_T(">"), iScriptStart + sTagLength + 1);
			if (-1 != iScriptPrefixEnd)
			{
				sHtml.Delete (iScriptStart, iScriptPrefixEnd - iScriptStart);
			}
			else
			{
				sHtml.Delete (iScriptStart, sTagLength + 1);	// end of a doc ?
			}
		}
		else
		{
			sHtml.Delete (iScriptStart, iScriptEnd - iScriptStart + sTagLength + 2 + 1); // length of "</" + tagLenth + ">"
		}
	}
}

UINT CLiteHTMLReader::parseDocument(void)
{
	ATLASSERT(m_lpszBuffer != NULL);

	bool	bAbort = false;			// continue parsing or abort?
	bool	bIsClosingTag = false;	// tag parsed is a closing tag?
	bool	bIsOpeningTag = false;	// tag parsed is an opening tag?
	bool	bInsideScript = false;

	CString	strCharacters;			// character data 
	CString	strComment;				// comment data
	CString	strT;					// temporary storage
	DWORD	dwCharDataStart = 0L;	// starting position of character data
	DWORD	dwCharDataLen = 0L;		// length of character data
	LONG	lTemp = 0L;				// temporary storage
	TCHAR	ch = 0;					// character at current buffer position
	CLiteHTMLTag	oTag;			// tag information

	if ( (!m_lpszBuffer) || (!m_dwBufLen) )
		return (0U);

	// reset seek pointer to beginning
	ResetSeekPointer();

	// notify event handler about parsing startup
	if (getEventNotify(notifyStartStop))
	{
		bAbort = false;
		m_pEventHandler->BeginParse(m_dwAppData, bAbort);
		if (bAbort)	goto LEndParse;
	}

	// skip leading white-space characters
	while (isWhiteSpace(ReadChar()))
		;
	
	TTagParsingMode aTagParsingMode = TPM_NORMAL;

	ch = UngetChar();
	while ((ch = ReadChar()) != NULL)
	{
		switch (ch)
		{

		// tag starting delimeter?
		case _T('<'):
			{
				UngetChar();
				
				strComment.Empty();
				if (!parseComment(strComment))
				{
					bIsOpeningTag = false;
					bIsClosingTag = false;

					

					if (!parseTag(oTag, bIsOpeningTag, bIsClosingTag, aTagParsingMode))
					{
						++dwCharDataLen;

						// manually advance buffer position
						// because the last call to UngetChar()
						// moved it back one character
						ch = ReadChar();

						break;
					}
					else
					{
						// check <script> tag			
						//WE ENTER IN SCRIPT MODE
						if (bIsOpeningTag && (TPM_NORMAL == aTagParsingMode))
						{
							if (0 == oTag.getTagName().CompareNoCase(_T("script")))
							{
								if (!oTag.IsTagInline())
								{
									aTagParsingMode = TPM_SCRIPT;
								}
							}
							else if (0 == oTag.getTagName().CompareNoCase(_T("style")))
							{
								if (!oTag.IsTagInline())
								{
									aTagParsingMode = TPM_STYLE;
								}
							}
							else if (0 == oTag.getTagName().CompareNoCase(_T("textarea")))
							{
								if (!oTag.IsTagInline())
								{
									aTagParsingMode = TPM_TEXTAREA;
								}
							}

						}

						if (bIsClosingTag && (TPM_NORMAL != aTagParsingMode))
						{
							if ((TPM_SCRIPT == aTagParsingMode) && (0 == oTag.getTagName().CompareNoCase(_T("script"))))
							{
								aTagParsingMode = TPM_NORMAL;
							}
							else if ((TPM_STYLE == aTagParsingMode) && (0 == oTag.getTagName().CompareNoCase(_T("style"))))
							{
								aTagParsingMode = TPM_NORMAL;
							}
							else if ((TPM_TEXTAREA == aTagParsingMode) && (0 == oTag.getTagName().CompareNoCase(_T("textarea"))))
							{
								aTagParsingMode = TPM_NORMAL;
							}
						}
					}
				}
				

				// clear pending notifications
				if ( (dwCharDataLen) || (strCharacters.GetLength()) )
				{
					strCharacters += CString(&m_lpszBuffer[dwCharDataStart], dwCharDataLen);
					NormalizeCharacters(strCharacters);
					
					if ( (strCharacters.GetLength()) && 
						 (getEventNotify(notifyCharacters)) )
					{
						bAbort = false;
						m_pEventHandler->Characters(strCharacters, m_dwAppData, bAbort);
						if (bAbort)	goto LEndParse;
					}

					strCharacters.Empty();
				}

				dwCharDataLen = 0L;
				dwCharDataStart = m_dwBufPos;

				if (strComment.GetLength())
				{
					if (getEventNotify(notifyComment))
					{
						bAbort = false;
						m_pEventHandler->Comment(strComment, m_dwAppData, bAbort);
						if (bAbort)	goto LEndParse;
					}
				}
				else
				{
					if ( (bIsOpeningTag) && (getEventNotify(notifyTagStart)) )
					{
						bAbort = false;
						m_pEventHandler->StartTag(&oTag, m_dwAppData, bAbort);
						if (bAbort)	goto LEndParse;
					}

					if ( (bIsClosingTag) && (getEventNotify(notifyTagEnd)) )
					{
						bAbort = false;
						m_pEventHandler->EndTag(&oTag, m_dwAppData, bAbort);
						if (bAbort)	goto LEndParse;
					}
				}

				break;
			}

		// entity reference beginning delimeter?
		case _T('&'):
			{
				UngetChar();

				lTemp = 0;
				if (m_bResolveEntities)
					lTemp = CLiteHTMLEntityResolver::resolveEntity(&m_lpszBuffer[m_dwBufPos], ch);
				
				if (lTemp)
				{
					wchar_t isSoftHyphen = L'­';
					if(ch == isSoftHyphen)
						strCharacters += CString(&m_lpszBuffer[dwCharDataStart], dwCharDataLen);
					else
						strCharacters += CString(&m_lpszBuffer[dwCharDataStart], dwCharDataLen) + ch;
					m_dwBufPos += lTemp;
					dwCharDataStart = m_dwBufPos;
					dwCharDataLen = 0L;
				}
				else
				{
					ch = ReadChar();
					++dwCharDataLen;
				}
				
				break;
			}
		
		// any other character
		default:
			{
				++dwCharDataLen;
				break;
			}
		}
	}

	// clear pending notifications
	if ( (dwCharDataLen) || (strCharacters.GetLength()) )
	{
		strCharacters += CString(&m_lpszBuffer[dwCharDataStart], dwCharDataLen) + ch;
		NormalizeCharacters(strCharacters);
		strCharacters.TrimRight();	// explicit trailing white-space removal

		if ( (strCharacters.GetLength()) && 
			 (getEventNotify(notifyCharacters)) )
		{
			bAbort = false;
			m_pEventHandler->Characters(strCharacters, m_dwAppData, bAbort);
			if (bAbort)	goto LEndParse;
		}
	}

LEndParse:
	// notify event handler about parsing completion
	if (getEventNotify(notifyStartStop))
		m_pEventHandler->EndParse(m_dwAppData, bAbort);

	m_lpszBuffer = NULL;
	m_dwBufLen = 0L;
	return (m_dwBufPos);
}

/**
 * CLiteHTMLReader::Read
 * The Read method parses an HTML document from an 
 * in-memory string buffer and raises events defined 
 * in ILiteHTMLReaderEvents to notify about variours 
 * elements.
 *
 * @param lpszString - string containing HTML text to parse
 */
UINT CLiteHTMLReader::Read(LPCTSTR lpszString)
{
	ATLASSERT(AtlIsValidString(lpszString));

	m_dwBufLen = ::_tcslen(lpszString);
	if (m_dwBufLen)
	{
		m_lpszBuffer = lpszString;
		return (parseDocument());
	}

	return (0U);
}

/**
 * CLiteHTMLReader::Read
 * This method is similar to the Read(LPCTSTR) method, 
 * except that, it accepts a file HANDLE instead of 
 * an in-memory string buffer containing HTML text.
 *
 * @param hFile - file handle
 *
 */
UINT CLiteHTMLReader::ReadFile(HANDLE hFile)
{
	ATLASSERT(hFile != INVALID_HANDLE_VALUE);
	ATLASSERT(::GetFileType(hFile) == FILE_TYPE_DISK);

	HANDLE	hFileMap;
	LPCTSTR	lpsz;
	UINT	nRetVal;

	// determine file size
	m_dwBufLen = ::GetFileSize(hFile, NULL);
	if (m_dwBufLen == INVALID_FILE_SIZE)
	{
		ATLTRACE2("(Error) CLiteHTMLReader::Read:"
				" GetFileSize() failed;"
				" GetLastError() returns 0x%08x.\n", ::GetLastError());
		goto LError;
	}

	// calculate length, in TCHARs, of the buffer
	m_dwBufLen /= sizeof(TCHAR);
	if (!m_dwBufLen)
		return (0U);

	// create a file-mapping object for the file
	hFileMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0L, 0L, NULL);
	if (hFileMap == NULL)
	{
		ATLTRACE2("(Error) CLiteHTMLReader::Read:"
				" CreateFileMapping() failed;"
				" GetLastError() returns 0x%08x.\n", ::GetLastError());
		goto LError;
	}

	// map the entire file into the address-space of the application
	lpsz = (LPCTSTR)::MapViewOfFile(hFileMap, FILE_MAP_READ, 0L, 0L, 0L);
	if (lpsz == NULL)
	{
		ATLTRACE2("(Error) CLiteHTMLReader::Read:"
				" MapViewOfFile() failed;"
				" GetLastError() returns 0x%08x.\n", ::GetLastError());
		goto LError;
	}

	m_lpszBuffer = lpsz;
	nRetVal = parseDocument();
	goto LCleanExit;

LError:
	nRetVal = 0U;
	m_dwBufLen = 0L;

LCleanExit:
	if (lpsz != NULL)
		ATLVERIFY(::UnmapViewOfFile(lpsz));
	if (hFileMap)
		ATLVERIFY(::CloseHandle(hFileMap));
	return (nRetVal);
}

TBOMType CharsetStringToInt (CStringA sCharset)
{
	TBOMType nBomType = TB_UNKNOWN;
	if (!sCharset.IsEmpty())
	{
		if (0 == sCharset.CompareNoCase("utf-8"))
			nBomType = TB_UTF8;
		else if (0 == sCharset.CompareNoCase("windows-1250"))	// central europe	// single byte
			nBomType = TB_WINDOWS1250;
		else if (0 == sCharset.CompareNoCase("windows-1251"))	// cyrillic
			nBomType = TB_WINDOWS1251;
		else if (0 == sCharset.CompareNoCase("windows-1252"))	// western european
			nBomType = TB_WINDOWS1252;
		else if (0 == sCharset.CompareNoCase("windows-1253"))	// greek
			nBomType = TB_WINDOWS1253;
		else if (0 == sCharset.CompareNoCase("windows-1254"))	// turkish
			nBomType = TB_WINDOWS1254;
		else if (0 == sCharset.CompareNoCase("windows-1255"))	// hebrew
			nBomType = TB_WINDOWS1255;
		else if (0 == sCharset.CompareNoCase("windows-1256"))	// arabic
			nBomType = TB_WINDOWS1256;
		else if (0 == sCharset.CompareNoCase("windows-1257"))	// baltic
			nBomType = TB_WINDOWS1257;
		else if (0 == sCharset.CompareNoCase("windows-1258"))	// vietnamese
			nBomType = TB_WINDOWS1258;
		else if (0 == sCharset.CompareNoCase("windows-874"))		// thai
			nBomType = TB_WINDOWS874;
		else if (0 == sCharset.CompareNoCase("koi8-r"))			// russian koi8-r
			nBomType = TB_KOI8R;
		else if (0 == sCharset.CompareNoCase("iso-8859-1"))			// iso-8859-1
			nBomType = TB_ISO8859_1;
		else if (0 == sCharset.CompareNoCase("shift_jis"))			// japan shit-jis
			nBomType = TB_SHIFT_JIS;
	}
	return nBomType;
}
// codepage detect;
TBOMType CLiteHTMLReader::DetectCodePage (HANDLE hFile, int& nBomSize)
{
	TBOMType nBomType (TB_UNKNOWN);
	BYTE *pBuffer = NULL;

	nBomSize = 0;

	try
	{
		// seek to begin of file
		if (INVALID_SET_FILE_POINTER == ::SetFilePointer (hFile, 0, NULL, FILE_BEGIN))
		{
			ATLTRACE2("(Error) CLiteHTMLReader::DetectCodePage:"
					" ::SetFilePointer failed;"
					" GetLastError() returns 0x%08x.\n", ::GetLastError());
			throw (UINT) -1;
		}

		nBomType = GetBomType (hFile, nBomSize);
		if (TB_UNKNOWN != nBomType)
		{
			return nBomType;
		}

		// charset is not detected by bom. find it from meta tag
		CStringA sCharset;

		if (INVALID_SET_FILE_POINTER == ::SetFilePointer (hFile, 0, NULL, FILE_BEGIN))
		{
			ATLTRACE2("(Error) CLiteHTMLReader::DetectCodePage:"
					" ::SetFilePointer failed;"
					" GetLastError() returns 0x%08x.\n", ::GetLastError());
			throw (UINT) -1;
		}

		// determine buffer size
		const DWORD dwFileSize = ::GetFileSize (hFile, NULL);
		const DWORD dwBufferSize = min (dwFileSize, 1 * 1024 * 1024);	// load 4 bytes
		CStringA sBuffer;
		pBuffer = reinterpret_cast <BYTE *> (sBuffer.GetBuffer(dwBufferSize));

		// checking multibyte codepage. loading 1 MB of data.
		DWORD dwRead (0);
		BOOL bReadRes = ::ReadFile (hFile, pBuffer, dwBufferSize, &dwRead, NULL);
		if ((0 == bReadRes) || (dwBufferSize != dwRead))
		{
			ATLTRACE2("(Error) CLiteHTMLReader::DetectCodePage:"
					" ::ReadFile(Buffer) failed;"
					" GetLastError() returns 0x%08x.\n", ::GetLastError());
			throw (UINT) -1;
		}
		sBuffer.ReleaseBuffer (dwBufferSize);
		sBuffer = sBuffer.MakeLower ();

		// seek to the beginning of file
		if (INVALID_SET_FILE_POINTER == ::SetFilePointer (hFile, 0, NULL, FILE_BEGIN))
		{
			ATLTRACE2("(Error) CLiteHTMLReader::DetectCodePage:"
					" ::SetFilePointer failed;"
					" GetLastError() returns 0x%08x.\n", ::GetLastError());
			throw (UINT) -1;
		}
		
		// find <meta> tag from beginning of file (usually it situated in the top of document)
		int nMetaStart = sBuffer.Find ("<meta");

		while (-1 != nMetaStart)
		{
			nMetaStart += 5;	// strlen ("<meta")

			// find closing '>'
			int nMetaEnd = sBuffer.Find (">", nMetaStart);	// strlen ("<meta") = 5
			//char *pMetaEnd = strstr (pMetaStart, ">");
			if (-1 == nMetaEnd)
			{
				// meta is not closed
				break;
			}
			nMetaEnd++; // strlen (">")

			// ok, let find charset
			CStringA sMetaContent = sBuffer.Mid (nMetaStart, nMetaEnd - nMetaStart);

			sMetaContent = sMetaContent.MakeLower ();
			const int nHttpEquiv = sMetaContent.Find ("http-equiv=");
			const int nContent = sMetaContent.Find ("content=");

			if (-1 != nHttpEquiv && -1 != nContent)
			{
				// check "http-equiv" attribute
				const bool bHttpEquivContentType = (-1 != sMetaContent.Find("content-type", nHttpEquiv + 11));
				// parse "content" attribute
				if (bHttpEquivContentType)
				{

					const int nCharset = sMetaContent.Find ("charset=", nContent + 8);
					if (-1 != nCharset)
					{
						const int nCharsetEnd = sMetaContent.Find ("\"", nCharset + 8);

						if (-1 != nCharsetEnd)
						{
							sCharset = sMetaContent.Mid (nCharset + 8, nCharsetEnd - nCharset - 8);
						}
					}
				}
			}

			if (!sCharset.IsEmpty())
			{
				nBomType = CharsetStringToInt (sCharset);
				break;
			}
			
			// find next meta tag
			nMetaStart = sBuffer.Find ("<meta", nMetaEnd);
		}

		if (TB_UNKNOWN == nBomType)
		{
			const int nXmlStart = sBuffer.Find ("<?");
			if (-1 != nXmlStart)
			{
				const int nXmlEnd = sBuffer.Find ('>', nXmlStart + 2);
				if (-1 != nXmlEnd)
				{
					// string like 'xml version='1.0' encoding="UTF-8"'
					CStringA sXmlNodeString = sBuffer.Mid (nXmlStart + 2, nXmlEnd - nXmlStart - 2);
					int nWordStart = sXmlNodeString.Find ("encoding");
					if (-1 != nWordStart)
					{
						nWordStart = sXmlNodeString.Find ('\"', nWordStart + 9);
						if (-1 != nWordStart)
						{
							sXmlNodeString = sXmlNodeString.Mid (nWordStart + 1);
							nWordStart = sXmlNodeString.Find ('\"');
							if (-1 != nWordStart)
							{
								sXmlNodeString = sXmlNodeString.Left (nWordStart);
								nBomType = CharsetStringToInt (sXmlNodeString);
							}
						}
					}
				}
			}
		}
	}
	catch (UINT eRes)
	{
		eRes;
		nBomType = TB_ERROR;
	}

	return nBomType;
}

//
UINT CLiteHTMLReader::ReadFile2 (HANDLE hFile, UINT aCodePage)
{
	ATLASSERT(hFile != INVALID_HANDLE_VALUE);
	ATLASSERT(::GetFileType(hFile) == FILE_TYPE_DISK);

	int nBomSize (0);
	TBOMType nCodePage = DetectCodePage (hFile, nBomSize);

	HANDLE	hFileMap (NULL);
	LPCSTR	lpsz (NULL);
	UINT	nRetVal (0);
	CString sHtml;

	try 
	{
		// determine file size
		m_dwBufLen = ::GetFileSize(hFile, NULL);
		if (m_dwBufLen == INVALID_FILE_SIZE)
		{
			ATLTRACE2("(Error) CLiteHTMLReader::Read:"
					" GetFileSize() failed;"
					" GetLastError() returns 0x%08x.\n", ::GetLastError());
			throw;
		}

		if (0 == m_dwBufLen)
			return (0U);

		// create a file-mapping object for the file
		hFileMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0L, 0L, NULL);
		if (hFileMap == NULL)
		{
			ATLTRACE2("(Error) CLiteHTMLReader::Read:"
					" CreateFileMapping() failed;"
					" GetLastError() returns 0x%08x.\n", ::GetLastError());
			throw;
		}

		// map the entire file into the address-space of the application
		lpsz = (LPCSTR)::MapViewOfFile(hFileMap, FILE_MAP_READ, 0L, 0L, 0L);
		if (lpsz == NULL)
		{
			ATLTRACE2("(Error) CLiteHTMLReader::Read:"
					" MapViewOfFile() failed;"
					" GetLastError() returns 0x%08x.\n", ::GetLastError());
			throw;
		}

		// skip BOM
		lpsz += nBomSize;
		m_dwBufLen -= nBomSize;
		

		bool bSimpleConvertToUnicode = true;
		switch (nCodePage)
		{
		case TB_UNKNOWN:
			//aCodePage = aCodePage; // do nothing (default code page passed through func params)
			break;
		case TB_UTF7:
			aCodePage = CP_UTF7;
			break;
		case TB_UTF8:
			aCodePage = CP_UTF8;
			break;
		case TB_WINDOWS1250:
			aCodePage = 1250;
			break;
		case TB_WINDOWS1251:
			aCodePage = 1251;
			break;
		case TB_WINDOWS1252:
		case TB_ISO8859_1:	// html5 says that
			aCodePage = 1252;
			break;
		case TB_WINDOWS1253:
			aCodePage = 1253;
			break;
		case TB_WINDOWS1254:
			aCodePage = 1254;
			break;
		case TB_WINDOWS1255:
			aCodePage = 1255;
			break;
		case TB_WINDOWS1256:
			aCodePage = 1256;
			break;
		case TB_WINDOWS1257:
			aCodePage = 1257;
			break;
		case TB_WINDOWS1258:
			aCodePage = 1258;
			break;
		case TB_WINDOWS874:
			aCodePage = 874;
			break;
		case TB_SHIFT_JIS:
			aCodePage = 932;
			break;
		default:
			// need complex conversion
			bSimpleConvertToUnicode = false;
			break;
		}

		if (bSimpleConvertToUnicode)
		{
			BOOL bRes = ConvertMultibyteToUnicode (aCodePage, lpsz, m_dwBufLen, sHtml);
			if (!bRes)
				throw;
		}
		else
		{
			if (TB_KOI8R == nCodePage)
			{
				BOOL bRes = ConvertKoi8RToUnicode (lpsz, m_dwBufLen, sHtml);
				if (!bRes)
					throw;
			}
			else if (TB_UTF16LE == nCodePage)
			{
				const int nSizeInChars = m_dwBufLen / sizeof (WCHAR);
				WCHAR *pwcBuffer = sHtml.GetBufferSetLength (nSizeInChars);
				if (NULL != pwcBuffer)
				{
					::memcpy (pwcBuffer, lpsz, nSizeInChars * sizeof (WCHAR));	// even quantity of chars
					sHtml.ReleaseBuffer ();
				}
			}
			else if (TB_UTF16BE == nCodePage)
			{
				const int nSizeInChars = m_dwBufLen / sizeof (WCHAR);
				WCHAR *pwcBuffer = sHtml.GetBufferSetLength (nSizeInChars);
				if (NULL != pwcBuffer)
				{
					for (int nChar = 0; nChar < nSizeInChars; nChar ++)
					{
						pwcBuffer[nChar] = lpsz[(nChar << 1) + 1] | (lpsz[nChar << 1] << 8);
					}
					sHtml.ReleaseBuffer ();
				}
			}
		}

		// delete script
		DeleteTags (sHtml, _T("script"));
		DeleteTags (sHtml, _T("SCRIPT"));
		DeleteComments (sHtml);


		m_lpszBuffer = sHtml.GetBuffer();
		nRetVal = parseDocument ();
		sHtml.ReleaseBuffer();
	}
	catch (...)
	{
		nRetVal = 0U;
		m_dwBufLen = 0L;
	}

	if (lpsz != NULL)
		ATLVERIFY(::UnmapViewOfFile(lpsz));
	if (hFileMap)
		ATLVERIFY(::CloseHandle(hFileMap));
	return (nRetVal);
}

// static
BOOL CLiteHTMLReader::ConvertKoi8RToUnicode (const LPCSTR aInput, const int aInputSize, CStringW &aOut) throw()
{
	ATLASSERT (aInput);
	ATLASSERT (aInputSize);

	// rfc-1489
	static const USHORT koi82unicode[128] = {
		 0x2500,0x2502,0x250c,0x2510,0x2514,0x2518,0x251c,0x2524,
		 0x252c,0x2534,0x253c,0x2580,0x2584,0x2588,0x258c,0x2590,
		 0x2591,0x2592,0x2593,0x2320,0x25a0,0x2219,0x221a,0x2248,
		 0x2264,0x2265,0x00a0,0x2321,0x00b0,0x00b2,0x00b7,0x00f7,
		 0x2550,0x2551,0x2552,0x0451,0x2553,0x2554,0x2555,0x2556,
		 0x2557,0x2558,0x2559,0x255a,0x255b,0x255c,0x255d,0x255e,
		 0x255f,0x2560,0x2561,0x0401,0x2562,0x2563,0x2564,0x2565,
		 0x2566,0x2567,0x2568,0x2569,0x256a,0x256b,0x256c,0x00a9,
		 0x044e,0x0430,0x0431,0x0446,0x0434,0x0435,0x0444,0x0433,
		 0x0445,0x0438,0x0439,0x043a,0x043b,0x043c,0x043d,0x043e,
		 0x043f,0x044f,0x0440,0x0441,0x0442,0x0443,0x0436,0x0432,
		 0x044c,0x044b,0x0437,0x0448,0x044d,0x0449,0x0447,0x044a,
		 0x042e,0x0410,0x0411,0x0426,0x0414,0x0415,0x0424,0x0413,
		 0x0425,0x0418,0x0419,0x041a,0x041b,0x041c,0x041d,0x041e,
		 0x041f,0x042f,0x0420,0x0421,0x0422,0x0423,0x0416,0x0412,
		 0x042c,0x042b,0x0417,0x0428,0x042d,0x0429,0x0427,0x042a
	};
	//aOut.Preallocate (aInputSize);
	WCHAR *pUnicodeBuffer = aOut.GetBufferSetLength (aInputSize);

	if (NULL == pUnicodeBuffer)
		return FALSE;

	for (int nChar = 0; nChar < aInputSize; nChar++)
	{
		const BYTE cKoi8Char = BYTE (aInput[nChar]); 
		const USHORT wcUnicodeChar = cKoi8Char < 128 ? cKoi8Char : koi82unicode [cKoi8Char - 128];
		pUnicodeBuffer[nChar] = (WCHAR) wcUnicodeChar;
		//aOut.SetAt(nChar, (WCHAR) wcUnicodeChar);
	}
	aOut.ReleaseBuffer ();
	
	return TRUE;
}
/*
// static
BOOL CLiteHTMLReader::ConvertMultibyteToUnicode (const UINT aCodePage, const LPCSTR aInput, const int aInputSize, CStringW &aOut) throw()
{
	ATLASSERT (aInput);
	ATLASSERT (aInputSize);

	const int nUnicodeSize = MultiByteToWideChar (aCodePage, 0, aInput, aInputSize, NULL, 0);

	WCHAR *pwStr = NULL;
	if (0 != nUnicodeSize)
	{
		try
		{
			pwStr = new WCHAR [nUnicodeSize];
		}
		catch (std::bad_alloc& ba)
		{
			ATLTRACE2("(Error) CLiteHTMLReader::ConvertMultibyteToUnicode:"
				"new[] failed;"
				" exception: %s\n", ba.what ());
		}
		if (NULL == pwStr)
		{
			ATLTRACE2("(Error) CLiteHTMLReader::ConvertMultibyteToUnicode:"
				" new failed;"
				" GetLastError() returns 0x%08x.\n", ::GetLastError());
			
			return FALSE;
		}

		const int nStrSize = ::MultiByteToWideChar (aCodePage, 0, aInput, aInputSize, pwStr, nUnicodeSize);
		ATLTRACE2 ("CLiteHTMLReader::ConvertMultibyteToUnicode(): %i bytes has been converted to unicode\n", nStrSize);
	}

	// Preprocess
	
	if (NULL != pwStr)
	{
		aOut = pwStr;
		delete [] pwStr;
		pwStr = NULL;
		return TRUE;
	}
	return FALSE;
}
*/
#pragma warning(pop)

