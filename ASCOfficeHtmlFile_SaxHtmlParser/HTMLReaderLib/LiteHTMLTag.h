#pragma once

#include "LiteHTMLCommon.h"
#include "LiteHTMLAttributes.h"

#pragma warning(push, 4)
#pragma warning (disable : 4290)	// C++ Exception Specification ignored
#pragma warning (disable : 4127)	// Disable warning C4127: conditional expression is constant

enum TTagParsingMode {TPM_NORMAL, TPM_SCRIPT, TPM_TEXTAREA, TPM_STYLE};

class CLiteHTMLTag
{
// Construction/Destruction
public:
	CLiteHTMLTag()
		: m_pcollAttr(NULL)
		, m_bIsInline (FALSE)
	{ }

	/**
	 * @param bCopy - if true, this CLiteHTMLTag makes a copy of the 
	 * CLiteHTMLAttributes pointer. if false, this constructor takes 
	 * ownership of the encapsulated CLiteHTMLAttributes pointer.
	 */
	CLiteHTMLTag(CLiteHTMLTag &rSource, bool bCopy = false)
		:m_pcollAttr(NULL)
	{
		m_strTagName = rSource.m_strTagName.MakeLower();
		m_bIsInline = rSource.m_bIsInline;
		m_aTexts.Copy (rSource.m_aTexts);

		if (!bCopy)
		{
			m_pcollAttr = rSource.m_pcollAttr;
			rSource.m_pcollAttr = NULL;
		}
		else if (rSource.m_pcollAttr != NULL)
		{
			m_pcollAttr = new CLiteHTMLAttributes(*(rSource.m_pcollAttr), true);
		}
	}

	virtual ~CLiteHTMLTag()
		{ SAFE_DELETE_POINTER(m_pcollAttr); }

// Attributes
public:
	CString getTagName(void) const
	{ return (m_strTagName); }

	CLiteHTMLAttributes* getAttributes(void)
	{ 
		return (m_pcollAttr);  
	}
	void setRawAttributes(CLiteHTMLAttributes* pAttrs)
	{ 
		SAFE_DELETE_POINTER(m_pcollAttr);
		m_pcollAttr = pAttrs;
	}

	void addText (const CString& aText)
	{
		m_aTexts.Add (aText);
	}
	size_t getTextsCount ()
	{
		return m_aTexts.GetCount ();
	}
	void clearTexts ()
	{
		m_aTexts.RemoveAll ();
	}
	CString getTextAt (size_t aIndex)
	{
		if (aIndex < m_aTexts.GetCount())
			return m_aTexts [aIndex];

		return _T("");
	}

// Parsing Helpers
public:
	// parses tag information from the given string
	UINT parseFromStr(LPCTSTR lpszString, 
					  bool &bIsOpeningTag, 
					  bool &bIsClosingTag, 
					  TTagParsingMode &aTagParsingMode,
					  long aLength,
					  bool bParseAttrib = true);
	BOOL IsTagInline() const;

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
	BOOL				m_bIsInline;
	CAtlArray<CString>		m_aTexts;
};

//
inline BOOL CLiteHTMLTag::IsTagInline() const
{
	return m_bIsInline;
}
inline UINT CLiteHTMLTag::parseFromStr(LPCTSTR lpszString, 
									   bool &bIsOpeningTag, 
									   bool &bIsClosingTag, 
									   TTagParsingMode &aTagParsingMode,
									   long aLength,	// sizeof string
									   bool bParseAttrib /* = true */)
{
	ATLASSERT(AtlIsValidString(lpszString));

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
		ATLASSERT(strTagName.IsEmpty());
		ATLASSERT(pcollAttr == NULL);
		ATLASSERT(!bClosingTag);
		nRetVal = (UINT)(lpszBegin - lpszString);
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
			ATLASSERT(lpszEnd != lpszBegin);
			
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
	strTagName = CString(lpszBegin, int (lpszEnd - lpszBegin));

	//if is any other other than /script
	if (TPM_NORMAL != aTagParsingMode)
	{
		if (!bClosingTag)
			return 0U;

		if ((TPM_SCRIPT == aTagParsingMode) && (0 != strTagName.CompareNoCase(_T("script"))))
			return 0U;

		if ((TPM_STYLE == aTagParsingMode) && (0 != strTagName.CompareNoCase(_T("style"))))
			return 0U;

		if ((TPM_TEXTAREA == aTagParsingMode) && (0 != strTagName.CompareNoCase(_T("textarea"))))
			return 0U;
	}

	//fix: rem white spaces till the end </tagname > or <tagname >
	while (::_istspace(*lpszEnd))
	lpszEnd = ::_tcsinc(lpszEnd);

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

		// skip tag's final 'new line' char
		if (*lpszEnd == _T('\n'))
			::_tcsinc(lpszEnd);

		ATLASSERT(strTagName.GetLength());
		ATLASSERT(pcollAttr == NULL);
		nRetVal = UINT (lpszEnd - lpszString);
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
			ATLASSERT(pcollAttr == NULL);
			// instantiate collection ...
			if ((pcollAttr = new CLiteHTMLAttributes) == NULL)
			{
				ATLTRACE2("(Error) CLiteHTMLTag::parseFromStr: Out of memory.\n");
				return (0U);
			}

			// ... and delegate parsing process
			// bring size to function (don't use strlen!), it's faster.
			nTemp = pcollAttr->parseFromStr(lpszBegin, lpszBegin - lpszString + aLength);
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
			ATLASSERT(bOpeningTag);
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

	nRetVal = UINT (lpszEnd - lpszString);
	goto LUpdateAndExit;	// just to show the flow-of-control

LUpdateAndExit:
	bIsClosingTag = bClosingTag;
	bIsOpeningTag = bOpeningTag;
	m_strTagName = strTagName.MakeLower();
	m_strTagName.TrimLeft();	m_strTagName.TrimRight();	// just-in-case!
	SAFE_DELETE_POINTER(m_pcollAttr);
	m_pcollAttr = pcollAttr;
	pcollAttr = NULL;

	m_bIsInline = bClosingTag & bOpeningTag;
	return (nRetVal);
}

#pragma warning (default : 4290)	// C++ Exception Specification ignored
#pragma warning (default : 4127)	// Disable warning C4127: conditional expression is constant
#pragma warning(pop)

