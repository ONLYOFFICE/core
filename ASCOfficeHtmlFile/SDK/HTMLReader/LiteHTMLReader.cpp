/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLReader.cpp - CLiteHTMLReader implementation
 *	
 *	Written By Gurmeet S. Kochar <gomzygotit@hotmail.com>
 *	Copyright (c) 2004. All rights reserved.
 *
 *	This code may be used in compiled form in any way you desire 
 *	(including commercial use). The code may be redistributed 
 *	unmodified by any means PROVIDING it is not sold for profit 
 *	without the authors written consent, and providing that this 
 *	notice and the authors name and all copyright notices remains 
 *	intact. However, this file and the accompanying source code may 
 *	not be hosted on a website or bulletin board without the authors 
 *	written permission.
 *	
 *	This file is provided "AS IS" with no expressed or implied warranty.
 *	The author accepts no liability for any damage/loss of business that
 *	this product may cause.
 *
 *	Although it is not necessary, but if you use this code in any of 
 *	your application (commercial or non-commercial), please INFORM me 
 *	so that I may know how useful this library is. This will encourage 
 *	me to keep updating it.
 */
#include "stdafx.h"
#include "LiteHTMLReader.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif	// _DEBUG

#ifndef __LITEHTMLENTITYRESOLVER_H__
#	include "LiteHTMLEntityResolver.h"
#endif	// !__LITEHTMLENTITYRESOLVER_H__

#pragma warning(push, 4)
UINT CLiteHTMLReader::parseDocument(void)
{
	ASSERT(m_lpszBuffer != NULL);

	bool	bAbort = false;			// continue parsing or abort?
	bool	bIsClosingTag = false;	// tag parsed is a closing tag?
	bool	bIsOpeningTag = false;	// tag parsed is an opening tag?
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
					if (!parseTag(oTag, bIsOpeningTag, bIsClosingTag))
					{
						++dwCharDataLen;

						// manually advance buffer position
						// because the last call to UngetChar()
						// moved it back one character
						ch = ReadChar();

						break;
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
 *
 * @return number of TCHARs successfully parsed
 * @since 1.0
 * @author Gurmeet S. Kochar
 */
UINT CLiteHTMLReader::Read(LPCTSTR lpszString)
{
	ASSERT(AfxIsValidString(lpszString));

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
 * @return number of TCHARs successfully parsed
 * @since 1.0
 * @author Gurmeet S. Kochar
 */
UINT CLiteHTMLReader::ReadFile(HANDLE hFile)
{
	ASSERT(hFile != INVALID_HANDLE_VALUE);
	ASSERT(::GetFileType(hFile) == FILE_TYPE_DISK);

	HANDLE	hFileMap;
	LPCTSTR	lpsz;
	UINT	nRetVal;

	// determine file size
	m_dwBufLen = ::GetFileSize(hFile, NULL);
	if (m_dwBufLen == INVALID_FILE_SIZE)
	{
		TRACE1("(Error) CLiteHTMLReader::Read:"
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
		TRACE1("(Error) CLiteHTMLReader::Read:"
				" CreateFileMapping() failed;"
				" GetLastError() returns 0x%08x.\n", ::GetLastError());
		goto LError;
	}

	// map the entire file into the address-space of the application
	lpsz = (LPCTSTR)::MapViewOfFile(hFileMap, FILE_MAP_READ, 0L, 0L, 0L);
	if (lpsz == NULL)
	{
		TRACE1("(Error) CLiteHTMLReader::Read:"
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
		VERIFY(::UnmapViewOfFile(lpsz));
	if (hFileMap)
		VERIFY(::CloseHandle(hFileMap));
	return (nRetVal);
}

#pragma warning(pop)
