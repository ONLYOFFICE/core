#pragma once

#ifndef __AFXTEMPL_H__
#	include <afxtempl.h>
#endif	// !__AFXTEMPL_H__


#include "LiteHTMLReader.h"


#define TAG_STATE_CLOSED		0x00000000	// tag closed
#define TAG_STATE_OPENED		0x00000001	// tag opened
#define TAG_STATE_HAS_ATTR		0x00000002	// tag has attributes
#define TAG_STATE_HAS_TEXTS		0x00000004	// tag has text fields
#define TAG_STATE_HAS_CHILDTAG	0x00000008	// tag has child tags
#define TAG_STATE_PREFIX_CLOSED	0x00000010	// tag has opened prefix ("<....") instead of ("<....>")


class CHTMLTagWrapper 
{
protected:
	CLiteHTMLTag	*m_pTag;
	DWORD			m_dwState;

public:
	CHTMLTagWrapper ()
		: m_pTag (NULL)
		, m_dwState (TAG_STATE_CLOSED)
	{
	}
	CHTMLTagWrapper (CLiteHTMLTag *aTag)
		: m_pTag (NULL)
		, m_dwState (TAG_STATE_CLOSED)
	{
		m_pTag = new CLiteHTMLTag (*aTag, true);	// make a copy
	}
	CHTMLTagWrapper (const CHTMLTagWrapper& aTagWrapper)
		: m_pTag (NULL)
		, m_dwState (TAG_STATE_CLOSED)
	{
		if (&aTagWrapper == this)
			return;

		CopyFrom (aTagWrapper);
	}
	CHTMLTagWrapper (const CHTMLTagWrapper *aTagWrapper)
		: m_pTag (NULL)
		, m_dwState (TAG_STATE_CLOSED)
	{
		CopyFrom (*aTagWrapper);
	}
	virtual ~CHTMLTagWrapper ()
	{
		if (NULL != m_pTag)
		{
			delete m_pTag;
			m_pTag = NULL;
		}
	}

// operators
public:
	CHTMLTagWrapper& operator = (const CHTMLTagWrapper& aTagWrapper)
	{
		if (this == &aTagWrapper)
			return *this;

		CopyFrom (aTagWrapper);

		return *this;
	}
	CHTMLTagWrapper& operator = (CLiteHTMLTag *aTag)
	{
		if (NULL == aTag)
		{
			if (m_pTag)
				delete m_pTag;

			m_pTag = NULL;
		}
		else
			m_pTag = new CLiteHTMLTag (*aTag, true);	// make a copy

		m_dwState = TAG_STATE_CLOSED;	// default value
		return *this;
	}
protected:
	void CopyFrom (const CHTMLTagWrapper& aTagWrapper)
	{
		if (NULL == aTagWrapper.m_pTag)
		{
			if (m_pTag)
				delete m_pTag;

			m_pTag = NULL;
		}
		else
			m_pTag = new CLiteHTMLTag (*aTagWrapper.m_pTag, true);	// make a copy

		m_dwState = aTagWrapper.m_dwState;
	}
public:
	DWORD State () const
	{
		return m_dwState;
	}
	void State (const DWORD aState)
	{
		m_dwState = aState;
	}
	CLiteHTMLTag* Tag ()
	{
		return m_pTag;
	}
	void Tag (CLiteHTMLTag *aTag)
	{
		if (NULL == aTag)
		{
			if (m_pTag)
				delete m_pTag;

			m_pTag = NULL;
		}
		else
			m_pTag = new CLiteHTMLTag (*aTag, true);	// make a copy

		m_dwState = TAG_STATE_CLOSED;	// default value
	}

};
