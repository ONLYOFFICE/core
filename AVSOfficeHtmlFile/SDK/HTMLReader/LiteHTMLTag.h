/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLTag.h - Defines CLiteHTMLTag
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
 *
 *	HISTORY:
 *
 *	Version 1.0				Gurmeet S. Kochar (GSK)
 *	Mar 18, 2004			First release version.
 */
#ifndef __LITEHTMLTAG_H__
#define __LITEHTMLTAG_H__

/*
 * Conditional Includes
 */
#ifndef __LITEHTMLCOMMON_H__
#	include "LiteHTMLCommon.h"
#endif	// !__LITEHTMLCOMMON_H__

#ifndef __LITEHTMLATTRIBUTES_H__
#	include "LiteHTMLAttributes.h"
#endif	// !__LITEHTMLATTRIBUTES_H__

#pragma warning(push, 4)
#pragma warning (disable : 4290)	// C++ Exception Specification ignored

/**
 * CLiteHTMLTag
 *
 * @version 1.0 (March 18, 2004)
 * @author Gurmeet S. Kochar
 */
class CLiteHTMLTag
{
// Construction/Destruction
public:
	CLiteHTMLTag()
		: m_pcollAttr(NULL)
	{ }

	/**
	 * @param bCopy - if true, this CLiteHTMLTag makes a copy of the 
	 * CLiteHTMLAttributes pointer. if false, this constructor takes 
	 * ownership of the encapsulated CLiteHTMLAttributes pointer.
	 *
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLTag(CLiteHTMLTag &rSource, bool bCopy = false) throw(CMemoryException)
		:m_pcollAttr(NULL)
	{
		m_strTagName = rSource.m_strTagName;
		if (!bCopy)
		{
			m_pcollAttr = rSource.m_pcollAttr;
			rSource.m_pcollAttr = NULL;
		}
		else if (rSource.m_pcollAttr != NULL)
		{
			/** DEEP COPY BEGIN */
			TRY
			{
				m_pcollAttr = new CLiteHTMLAttributes(*(rSource.m_pcollAttr), true);
			}
			CATCH(CMemoryException, pEx)
			{
				THROW_LAST();
			}
			END_CATCH
			/** DEEP COPY END */
		}
	}

	virtual ~CLiteHTMLTag()
		{ SAFE_DELETE_POINTER(m_pcollAttr); }

// Attributes
public:
	/**
	 * @return name of the tag
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CString getTagName(void) const
		{ return (m_strTagName); }

	/**
	 * @return pointer to the attribute collection
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	const CLiteHTMLAttributes* getAttributes(void) const
		{ return (m_pcollAttr);  }

// Parsing Helpers
public:
	// parses tag information from the given string
	UINT parseFromStr(LPCTSTR lpszString, 
					  bool &bIsOpeningTag, 
					  bool &bIsClosingTag, 
					  bool bParseAttrib = true);

// Data Members
private:
	/**
	 * collection of CLiteHTMLElemAttr objects
	 * @since 1.0
	 */
	CLiteHTMLAttributes	*m_pcollAttr;

	/**
	 * tag name of the element
	 * @since 1.0
	 */
	CString				m_strTagName;
};

/**
 * CLiteHTMLTag::parseFromStr
 * 
 * @param rStr - string to parse
 * @param bIsOpeningTag - receives true if the tag parsed 
 *        is a opening tag, false otherwise.
 * @param bIsClosingTag - receives true if the tag parsed 
 *        is a closing tag, false otherwise.
 * @param bParseAttrib - determines whether attribute/value 
 *        pairs should be parsed. Default is true.
 *
 * @return number of characters successfully parsed
 * @since 1.0
 * @author Gurmeet S. Kochar
 */
inline UINT CLiteHTMLTag::parseFromStr(LPCTSTR lpszString, 
									   bool &bIsOpeningTag, 
									   bool &bIsClosingTag, 
									   bool bParseAttrib /* = true */)
{
	ASSERT(AfxIsValidString(lpszString));

	bool				bClosingTag = false;
	bool				bOpeningTag = false;
	CLiteHTMLAttributes	*pcollAttr = NULL;
	CString				strTagName;
	UINT				nRetVal = 0U, 
						nTemp = 0U;
	LPCTSTR				lpszBegin = lpszString;
	LPCTSTR				lpszEnd = NULL;

	// skip leading white-space characters
	while (::_istspace(*lpszBegin))
		lpszBegin = ::_tcsinc(lpszBegin);

	// HTML tags always begin with a less-than symbol
	if (*lpszBegin != _T('<'))
		return (0U);
	
	// skip tag's opening delimeter '<'
	lpszBegin = ::_tcsinc(lpszBegin);

	// optimization for empty opening tags
	if (*lpszBegin == _T('>'))
	{
		ASSERT(strTagName.IsEmpty());
		ASSERT(pcollAttr == NULL);
		ASSERT(!bClosingTag);
		nRetVal = lpszBegin - lpszString;
		goto LUpdateAndExit;
	}

	// tag names always begin with an alphabet
	if (!::_istalpha(*lpszBegin))
	{
		bClosingTag = (*lpszBegin == _T('/'));
		if (bClosingTag)
			lpszBegin = ::_tcsinc(lpszBegin);
		else
			return (0U);
	}

	bOpeningTag = !bClosingTag;
	lpszEnd = lpszBegin;
	do
	{
		// tag name may contain letters (a-z, A-Z), digits (0-9), 
		// underscores '_', hyphen '-', colons ':', and periods '.'
		if ( (!::_istalnum(*lpszEnd)) && 
			 (*lpszEnd != _T('-')) && (*lpszEnd != _T(':')) && 
			 (*lpszEnd != _T('_')) && (*lpszEnd != _T('.')) )
		{
			ASSERT(lpszEnd != lpszBegin);
			
			// only white-space characters, a null-character, a 
			// greater-than symbol, or a forward-slash can break 
			// a tag name
			if (*lpszEnd == NULL || ::_istspace(*lpszEnd) || 
				*lpszEnd == _T('>') || 
				(*lpszEnd == _T('/') && (!bClosingTag)) )
			{
				break;
			}

			return (0U);	// any other character will fail parsing process
		}

		lpszEnd = ::_tcsinc(lpszEnd);
	}
	while(true);

	// store tag name for later use
	strTagName = CString(lpszBegin, lpszEnd - lpszBegin);

	// is this a closing tag?
	if (bClosingTag)
	{
		// in a closing tag, there can be only one symbol after 
		// tag name, i.e., the tag end delimeter '>'. Anything 
		// else will result in parsing failure
		if (*lpszEnd != _T('>'))
			return (0U);

		// skip tag's ending delimeter
		lpszEnd = ::_tcsinc(lpszEnd);

		ASSERT(strTagName.GetLength());
		ASSERT(pcollAttr == NULL);
		nRetVal = lpszEnd - lpszString;
		goto LUpdateAndExit;
	}

	// tag contains attribute/value pairs?
	if (*lpszEnd != _T('>'))
	{
		lpszBegin = lpszEnd;
		lpszEnd = NULL;

		// skip white-space characters after tag name
		while (::_istspace(*lpszBegin))
			lpszBegin = ::_tcsinc(lpszBegin);

		nTemp = 0U;
		if (bParseAttrib)	// parse attribute/value pairs?
		{
			ASSERT(pcollAttr == NULL);
			// instantiate collection ...
			if ((pcollAttr = new CLiteHTMLAttributes) == NULL)
			{
				TRACE0("(Error) CLiteHTMLTag::parseFromStr: Out of memory.\n");
				return (0U);
			}

			// ... and delegate parsing process
			nTemp = pcollAttr->parseFromStr(lpszBegin);
		}

		if (nTemp == 0)	// attribute/value pair parsing is disabled? 
						//	- OR - 
						// attribute/value pairs could not be parsed?
		{
			SAFE_DELETE_POINTER(pcollAttr);
			if ((lpszEnd = ::_tcsstr(lpszBegin, _T("/>"))) == NULL)
			{
				if ((lpszEnd = ::_tcschr(lpszBegin, _T('>'))) == NULL)
					return (0U);
			}
		}
		else
		{
			lpszEnd = lpszBegin + nTemp;

			// skip white-space after attribute/value pairs
			while (::_istspace(*lpszEnd))
				lpszEnd = ::_tcsinc(lpszEnd);
			
			// tag's ending delimeter could not be found?
			if (*lpszEnd == NULL)
			{
				SAFE_DELETE_POINTER(pcollAttr);
				return (0U);
			}
		}

		// a tag like this one: <BR/>
		if (*lpszEnd == _T('/'))
		{
			ASSERT(bOpeningTag);
			bClosingTag = true;
			lpszEnd = ::_tcsinc(lpszEnd);
		}
	}

	// HTML tags always end with a greater-than '>' symbol
	if (*lpszEnd != _T('>'))
	{
		SAFE_DELETE_POINTER(pcollAttr);
		return (0U);
	}
	else
		lpszEnd = ::_tcsinc(lpszEnd);

	nRetVal = lpszEnd - lpszString;
	goto LUpdateAndExit;	// just to show the flow-of-control

LUpdateAndExit:
	bIsClosingTag = bClosingTag;
	bIsOpeningTag = bOpeningTag;
	m_strTagName = strTagName;
	m_strTagName.TrimLeft();	m_strTagName.TrimRight();	// just-in-case!
	SAFE_DELETE_POINTER(m_pcollAttr);
	m_pcollAttr = pcollAttr;
	pcollAttr = NULL;

	return (nRetVal);
}

#pragma warning (default : 4290)	// C++ Exception Specification ignored
#pragma warning(pop)

#endif	// !__LITEHTMLTAG_H__
