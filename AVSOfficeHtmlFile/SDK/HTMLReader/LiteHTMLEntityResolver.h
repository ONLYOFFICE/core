/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLEntityResolver.h - Defines CLiteHTMLEntityResolver
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
 *	Mar 21, 2004			First release version.
 */
#ifndef __LITEHTMLENTITYRESOLVER_H__
#define __LITEHTMLENTITYRESOLVER_H__

/*
 * Conditional Includes
 */
#ifndef __AFXTEMPL_H__
#	include <afxtempl.h>
#endif	// !__AFXTEMPL_H__

#ifndef __LITEHTMLCOMMON_H__
#	include "LiteHTMLCommon.h"
#endif	// !__LITEHTMLCOMMON_H__

#pragma warning(push, 4)

/**
 * CLiteHTMLEntityResolver
 * This is a utility class that helps in resolving the character 
 * and numeric entity references.
 *
 * @version 1.0 (Mar 17, 2004)
 * @author Gurmeet S. Kochar
 */
class CLiteHTMLEntityResolver
{
private:
	class CCharEntityRefs : public CMap<CString, LPCTSTR, TCHAR, TCHAR>
	{
	public:
		CCharEntityRefs() : CMap<CString, LPCTSTR, TCHAR, TCHAR>(55)
		{
			/** C0 Controls and Basic Latin */
			(*this)[_T("quot")]		= _T('\x22');
			(*this)[_T("amp")]		= _T('\x26');
			(*this)[_T("apos")]		= _T('\x27');
			(*this)[_T("lt")]		= _T('\x3C');
			(*this)[_T("gt")]		= _T('\x3E');
			/** ISO 8859-1 (Latin-1) characters */
			(*this)[_T("nbsp")]		= _T('\xA0');
			(*this)[_T("iexcl")]	= _T('\xA1');
			(*this)[_T("cent")]		= _T('\xA2');
			(*this)[_T("pound")]	= _T('\xA3');
			(*this)[_T("current")]	= _T('\xA4');
			(*this)[_T("yen")]		= _T('\xA5');
			(*this)[_T("brvbar")]	= _T('\xA6');
			(*this)[_T("sect")]		= _T('\xA7');
			(*this)[_T("uml")]		= _T('\xA8');
			(*this)[_T("copy")]		= _T('\xA9');
			(*this)[_T("ordf")]		= _T('\xAA');
			(*this)[_T("laquo")]	= _T('\xAB');
			(*this)[_T("not")]		= _T('\xAC');
			(*this)[_T("shy")]		= _T('\xAD');
			(*this)[_T("reg")]		= _T('\xAE');
			(*this)[_T("macr")]		= _T('\xAF');
			(*this)[_T("deg")]		= _T('\xB0');
			(*this)[_T("plusmn")]	= _T('\xB1');
			(*this)[_T("sup2")]		= _T('\xB2');
			(*this)[_T("sup3")]		= _T('\xB3');
			(*this)[_T("acute")]	= _T('\xB4');
			(*this)[_T("micro")]	= _T('\xB5');
			(*this)[_T("para")]		= _T('\xB6');
			(*this)[_T("middot")]	= _T('\xB7');
			(*this)[_T("cedil")]	= _T('\xB8');
			(*this)[_T("sup1")]		= _T('\xB9');
			(*this)[_T("ordm")]		= _T('\xBA');
			(*this)[_T("raquo")]	= _T('\xBB');
			(*this)[_T("frac14")]	= _T('\xBC');
			(*this)[_T("frac12")]	= _T('\xBD');
			(*this)[_T("frac34")]	= _T('\xBE');
			(*this)[_T("iquest")]	= _T('\xBF');
			(*this)[_T("Agrave")]	= _T('\xC0');
			(*this)[_T("Aacute")]	= _T('\xC1');
			(*this)[_T("Acirc")]	= _T('\xC2');
			(*this)[_T("Atilde")]	= _T('\xC3');
			(*this)[_T("Auml")]		= _T('\xC4');
			(*this)[_T("Aring")]	= _T('\xC5');
			(*this)[_T("AElig")]	= _T('\xC6');
			(*this)[_T("Ccedil")]	= _T('\xC7');
			(*this)[_T("Egrave")]	= _T('\xC8');
			(*this)[_T("Eacute")]	= _T('\xC9');
			(*this)[_T("Ecirc")]	= _T('\xCA');
			(*this)[_T("Euml")]		= _T('\xCB');
			(*this)[_T("Igrave")]	= _T('\xCC');
			(*this)[_T("Iacute")]	= _T('\xCD');
			(*this)[_T("Icirc")]	= _T('\xCE');
			(*this)[_T("Iuml")]		= _T('\xCF');
			(*this)[_T("ETH")]		= _T('\xD0');
			(*this)[_T("Ntilde")]	= _T('\xD1');
			(*this)[_T("Ograve")]	= _T('\xD2');
			(*this)[_T("Oacute")]	= _T('\xD3');
			(*this)[_T("Ocirc")]	= _T('\xD4');
			(*this)[_T("Otilde")]	= _T('\xD5');
			(*this)[_T("Ouml")]		= _T('\xD6');
			(*this)[_T("times")]	= _T('\xD7');
			(*this)[_T("Oslash")]	= _T('\xD8');
			(*this)[_T("Ugrave")]	= _T('\xD9');
			(*this)[_T("Uacute")]	= _T('\xDA');
			(*this)[_T("Ucirc")]	= _T('\xDB');
			(*this)[_T("Uuml")]		= _T('\xDC');
			(*this)[_T("Yacute")]	= _T('\xDD');
			(*this)[_T("THORN")]	= _T('\xDE');
			(*this)[_T("szlig")]	= _T('\xDF');
			(*this)[_T("agrave")]	= _T('\xE0');
			(*this)[_T("aacute")]	= _T('\xE1');
			(*this)[_T("acirc")]	= _T('\xE2');
			(*this)[_T("atilde")]	= _T('\xE3');
			(*this)[_T("auml")]		= _T('\xE4');
			(*this)[_T("aring")]	= _T('\xE5');
			(*this)[_T("aelig")]	= _T('\xE6');
			(*this)[_T("ccedil")]	= _T('\xE7');
			(*this)[_T("egrave")]	= _T('\xE8');
			(*this)[_T("eacute")]	= _T('\xE9');
			(*this)[_T("ecirc")]	= _T('\xEA');
			(*this)[_T("euml")]		= _T('\xEB');
			(*this)[_T("igrave")]	= _T('\xEC');
			(*this)[_T("iacute")]	= _T('\xED');
			(*this)[_T("icirc")]	= _T('\xEE');
			(*this)[_T("iuml")]		= _T('\xEF');
			(*this)[_T("eth")]		= _T('\xF0');
			(*this)[_T("ntilde")]	= _T('\xF1');
			(*this)[_T("ograve")]	= _T('\xF2');
			(*this)[_T("oacute")]	= _T('\xF3');
			(*this)[_T("ocirc")]	= _T('\xF4');
			(*this)[_T("otilde")]	= _T('\xF5');
			(*this)[_T("ouml")]		= _T('\xF6');
			(*this)[_T("divide")]	= _T('\xF7');
			(*this)[_T("oslash")]	= _T('\xF8');
			(*this)[_T("ugrave")]	= _T('\xF9');
			(*this)[_T("uacute")]	= _T('\xFA');
			(*this)[_T("ucirc")]	= _T('\xFB');
			(*this)[_T("uuml")]		= _T('\xFC');
			(*this)[_T("yacute")]	= _T('\xFD');
			(*this)[_T("thorn")]	= _T('\xFE');
			(*this)[_T("yuml")]		= _T('\xFF');
		}
	};

// Constructors
public:
	CLiteHTMLEntityResolver()	{ }

// Operations
public:
	/**
	 * Resolves a character or numeric entity reference.
	 *
	 * @param rStr - entity to resolve
	 * @param ch - variable that will receive the result
	 *
	 * @return number of TCHARs used to resolve entity reference
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	static UINT resolveEntity(LPCTSTR lpszEntity, TCHAR &chSubst)
	{
		ASSERT(m_CharEntityRefs.GetCount());
		ASSERT(AfxIsValidString(lpszEntity));

		LPCTSTR	lpszBegin = lpszEntity, 
				lpszEnd = ::_tcschr(lpszEntity, _T(';'));
		TCHAR	chTemp = 0;
		
		// entity references always end with a semi-colon ';'
		if (lpszEnd == NULL)
			return (0);

		// skip leading white-space characters
		while (::_istspace(*lpszBegin))
			lpszBegin = ::_tcsinc(lpszBegin);
		
		// remaining string (including semi-colon) 
		// must be at least 4 characters in length
		if (lpszEnd - lpszBegin < 3)
			return (0U);

		// entity references always begin with an ampersand '&' symbol
		if (*lpszBegin != _T('&'))
			return (0U);
		lpszBegin = ::_tcsinc(lpszBegin);

		// numeric (decimal or hexadecimal) entity reference?
		if (*lpszBegin == _T('#'))
		{
			lpszBegin = ::_tcsinc(lpszBegin);
			chTemp = *lpszBegin;
			int	radix = (::_istdigit(chTemp) ? 10 : 
						(chTemp == _T('x') || 
							chTemp == _T('X') ? 16 : 0));
			if (radix)
			{
				if (radix == 16)
					lpszBegin = ::_tcsinc(lpszBegin);
				
				unsigned long	ulNum = ::_tcstoul(lpszBegin, NULL, radix);
				chSubst = (TCHAR)ulNum;
				lpszEnd = ::_tcsinc(lpszEnd);
				return (lpszEnd - lpszEntity);
			}
		}

		// character entity reference?
		else
		{
			CString	strKey(lpszBegin, lpszEnd - lpszBegin);

			// because some character entity references are 
			// case-sensitive, we must fix them manually
			if (!strKey.CompareNoCase(_T("eth")) || 
				!strKey.CompareNoCase(_T("thorn")))
			{
				if (::_istupper(strKey[0]))
					strKey.MakeUpper();
				else
					strKey.MakeLower();
			}
			else if (!strKey.CompareNoCase(_T("Oslash")))
			{
				strKey.MakeLower();
				strKey.SetAt(0, _T('O'));
			}
			else if (!strKey.CompareNoCase(_T("AElig")))
			{
				strKey.MakeLower();
				strKey.SetAt(0, _T('A'));
				strKey.SetAt(1, _T('E'));
			}
			else
			{
				CString	strT = strKey.Mid(1);
				strKey.MakeLower();
				if (strT.CompareNoCase(_T("grave")) == 0 || 
					strT.CompareNoCase(_T("acute")) == 0 || 
					strT.CompareNoCase(_T("circ"))  == 0 || 
					strT.CompareNoCase(_T("uml"))   == 0 || 
					strT.CompareNoCase(_T("tilde")) == 0 || 
					strT.CompareNoCase(_T("cedil")) == 0 || 
					strT.CompareNoCase(_T("ring"))  == 0)
				{
					strKey.SetAt(0, strT[0]);
				}
			}
			
			// is this a known entity reference?
			if (m_CharEntityRefs.Lookup(strKey, chTemp))
			{
				chSubst = chTemp;
				lpszEnd = ::_tcsinc(lpszEnd);
				return (lpszEnd - lpszEntity);
			}
		}

		return (0U);
	}

// Data Members
private:
	static CCharEntityRefs	m_CharEntityRefs;
};
#pragma warning(pop)

#endif	// !__LITEHTMLENTITYRESOLVER_H__
