#pragma once

#include "LiteHTMLTag.h"
#import <msxml6.dll>


#define TAG_STATE_CLOSED		0x00000000	// tag closed
#define TAG_STATE_OPENED		0x00000001	// tag opened
#define TAG_STATE_HAS_ATTR		0x00000002	// tag has attributes
#define TAG_STATE_HAS_TEXTS		0x00000004	// tag has text fields
#define TAG_STATE_HAS_CHILDTAG	0x00000008	// tag has child tags
#define TAG_STATE_PREFIX_CLOSED	0x00000010	// tag has opened prefix ("<....") instead of ("<....>")
#define TAG_STATE_SPECIAL		0x00000020	// tag is special - special tag handling

class CHTMLTagWrapper;
typedef CAtlArray<CHTMLTagWrapper *> CAtlTagWrapperArray;

class CHTMLTagWrapper 
{
protected:
	CLiteHTMLTag*			m_pTag;
	DWORD					m_dwState;
	CString					m_sUnprecessedText;
	CLiteHTMLElemStyleAttr	m_oStyle;
	CHTMLTagWrapper*		m_pParentTag;
	double					m_dFontSize;
	CAtlTagWrapperArray		m_aChildArray;


public:
	MSXML2::IXMLDOMNodePtr		pXmlTagNode; 

public:
	CHTMLTagWrapper ()
		: m_pTag (NULL)
		, m_dwState (TAG_STATE_CLOSED)
		, pXmlTagNode (NULL)
		, m_oStyle (CString())
		, m_pParentTag (NULL)
		, m_dFontSize (16.0)
	{
	}
	CHTMLTagWrapper (CLiteHTMLTag *aTag)
		: m_pTag (NULL)
		, m_dwState (TAG_STATE_CLOSED)
		, pXmlTagNode (NULL)
		, m_oStyle (CString())
		, m_pParentTag (NULL)
		, m_dFontSize (16.0)
	{
		m_pTag = new CLiteHTMLTag (*aTag, true);	// make a copy
		//m_spDomTag.CreateInstance ();
	}
	CHTMLTagWrapper (const CHTMLTagWrapper& aTagWrapper)
		: m_pTag (NULL)
		, m_dwState (TAG_STATE_CLOSED)
		, pXmlTagNode (NULL)
		, m_oStyle (CString())
		, m_pParentTag (NULL)
		, m_dFontSize (16.0)
	{
		if (&aTagWrapper == this)
			return;

		CopyFrom (aTagWrapper);
	}
	CHTMLTagWrapper (const CHTMLTagWrapper *aTagWrapper)
		: m_pTag (NULL)
		, m_dwState (TAG_STATE_CLOSED)
		, pXmlTagNode (NULL)
		, m_oStyle (CString())
		, m_pParentTag (NULL)
		, m_dFontSize (16.0)
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
		{
			m_pTag = new CLiteHTMLTag (*aTag, true);	// make a copy
		}

		m_dwState = TAG_STATE_CLOSED;	// default value
		m_sUnprecessedText.Empty();
		m_pParentTag = NULL;
		m_dFontSize = 16.0;
		DestroyChildren ();		

		return *this;
	}

public:
	// IMPORTANT: this method could be called explicitly !!!
	void DestroyChildren ()
	{
		if (!m_aChildArray.IsEmpty())
		{
			for (size_t nItem = 0; nItem < m_aChildArray.GetCount(); ++nItem)
			{
				if (NULL != m_aChildArray[nItem])
				{
					m_aChildArray[nItem]->DestroyChildren();
					delete m_aChildArray[nItem];
					m_aChildArray[nItem] = NULL;
				}
			}
			m_aChildArray.RemoveAll ();
		}
	}
	void AddChild (CHTMLTagWrapper* aTagWrapper)
	{
		m_aChildArray.Add (aTagWrapper);
	}
	int GetChildrenCount () const
	{
		m_aChildArray.GetCount ();
	}
	int GetChildIndex (const CHTMLTagWrapper* aTagWrapper)
	{
		int nIndex = -1;
		for (size_t nItem = 0; nItem < m_aChildArray.GetCount(); ++nItem)
		{
			if (aTagWrapper == m_aChildArray[nItem])
			{
				nIndex = nItem;
				break;
			}
		}
		return nIndex;
	}
	BOOL InsertChildAt (int aIndex, CHTMLTagWrapper* aTagWrapper)
	{
		if (aIndex < m_aChildArray.GetCount() && aIndex >= 0)
		{
			m_aChildArray.InsertAt (aIndex, aTagWrapper);
			return TRUE;
		}
		return FALSE;
	}
	CHTMLTagWrapper* GetChildAt (int aIndex)
	{
		if (aIndex < 0 && aIndex >= m_aChildArray.GetCount())
		{
			return NULL;
		}
		return m_aChildArray [aIndex];
	}
	CHTMLTagWrapper* DeleteChildAt (int aIndex)
	{
		if (aIndex < 0 && aIndex >= m_aChildArray.GetCount())
		{
			return NULL;
		}
		if (NULL != m_aChildArray [aIndex])
		{
			delete m_aChildArray [aIndex];
			m_aChildArray.RemoveAt (aIndex);
		}
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
		{
			m_pTag = new CLiteHTMLTag (*aTagWrapper.m_pTag, true);			// make a copy
		}

		m_dwState = aTagWrapper.m_dwState;
		pXmlTagNode = aTagWrapper.pXmlTagNode;
		m_sUnprecessedText = aTagWrapper.m_sUnprecessedText;
		m_oStyle = aTagWrapper.m_oStyle;
		m_pParentTag = aTagWrapper.m_pParentTag;
		m_dFontSize = aTagWrapper.m_dFontSize;

		m_aChildArray.RemoveAll ();
		if (!aTagWrapper.m_aChildArray.IsEmpty())
			m_aChildArray.Copy (aTagWrapper.m_aChildArray);

	}
	BOOL TestSingleSelector (const CString& aSelector) const
	{
		if (aSelector == _T("*"))
			return TRUE;

		const CString sType = m_pTag->getTagName();	// type

		CString sSelector = aSelector;
		if (!sSelector.IsEmpty() && sSelector[0] == '*')
		{
			// *selector
			sSelector.Delete(0);
			if (sSelector == '.')
				return TRUE;
		}

		if (0 == sType.CompareNoCase (sSelector))
			return TRUE;

		// check parentnings and siblings
		/////////////////////////////////
		if (-1 != sSelector.Find('>') && (NULL != m_pParentTag))	// a>b - b is child of a
		{
			CAtlArray<CString> aParentAndChild;
			CLiteHTMLElemStyleAttr::SymbolSeparatedValuesToArray('>', sSelector, aParentAndChild);

			// make 2 elements form array of (a>b>c>d) = ('a>b>c' (parent) and 'd' (this))
			if (aParentAndChild.GetCount() > 1)
			{
				CString sThisTagSelector = aParentAndChild[aParentAndChild.GetCount() - 1];	// last element
				CString sParentTagSelector;
				for (int nParent = 0; nParent < aParentAndChild.GetCount() - 2; ++nParent)
				{
					sParentTagSelector += aParentAndChild[nParent];
					sParentTagSelector += '>';
				}
				sParentTagSelector += aParentAndChild[aParentAndChild.GetCount() - 2];

				if (TestSingleSelector (sThisTagSelector) && m_pParentTag->TestSingleSelector (sParentTagSelector))
					return TRUE;
				else 
					return FALSE;
			}
			else 
				return FALSE;
		}
		else if (-1 != sSelector.Find('+') && (NULL != m_pParentTag)) // a+b - b is next sibling after a
		{
			const int nThisTagIndexInParent = m_pParentTag->GetChildIndex (this);
			
			if (nThisTagIndexInParent < 1)	// ok, this tag is not first in the parent's family
				return FALSE;

			// prev sibling:
			CHTMLTagWrapper *pPrevSiblingTag = m_pParentTag->GetChildAt (nThisTagIndexInParent - 1);
			if (NULL == pPrevSiblingTag)
				return FALSE;

			CAtlArray<CString> aPrevSiblingsAndThis;
			CLiteHTMLElemStyleAttr::SymbolSeparatedValuesToArray('+', sSelector, aPrevSiblingsAndThis);
			// make 2 elements form array of (a>b>c>d) = ('a>b>c' (parent) and 'd' (this))
			if (aPrevSiblingsAndThis.GetCount() > 1)
			{
				CString sThisTagSelector = aPrevSiblingsAndThis[aPrevSiblingsAndThis.GetCount() - 1];	// last element
				CString sSiblingsTagSelector;
				for (int nParent = 0; nParent < aPrevSiblingsAndThis.GetCount() - 2; ++nParent)
				{
					sSiblingsTagSelector += aPrevSiblingsAndThis[nParent];
					sSiblingsTagSelector += '+';
				}
				sSiblingsTagSelector += aPrevSiblingsAndThis[aPrevSiblingsAndThis.GetCount() - 2];

				if (TestSingleSelector (sThisTagSelector) && pPrevSiblingTag->TestSingleSelector (sSiblingsTagSelector))
					return TRUE;
				else 
					return FALSE;
			}
			else 
				return FALSE;
		}
		/////////////////////////////////

		// pseudo-classes
		const int nPseudoClassSelectorChar = sSelector.Find(':');
		if (-1 != nPseudoClassSelectorChar)
		{
			// Examples:
			// E:first-child	Matches element E when E is the first child of its parent.	The :first-child pseudo-class
			// E:link
			// E:visited	Matches element E if E is the source anchor of a hyperlink of which the target is not yet visited (:link) or already visited (:visited).	The link pseudo-classes
			// E:active
			// E:hover
			// E:focus	Matches E during certain user actions.	The dynamic pseudo-classes
			// E:lang(c)	Matches element of type E if it is in (human) language c (the document language specifies how language is determined).	The :lang() pseudo-class
			const CString sThisSelector = sSelector.Mid (0, nPseudoClassSelectorChar);
			

			if (0 == sType.CompareNoCase (sThisSelector))
			{
				const CString sSubSelector = sSelector.Mid (nPseudoClassSelectorChar + 1);

				if (0 == sSubSelector.CompareNoCase (_T("link")))
				{
					return TRUE;
				}
				else if (0 == sSubSelector.CompareNoCase (_T("first-child")) && 0 == m_pParentTag->GetChildIndex (this))
				{
					return TRUE;
				}
				else 
					return FALSE;
			}
		}
		//

		// check real single selector
		CLiteHTMLAttributes *pAttrs = m_pTag->getAttributes();
		if (NULL != pAttrs)
		{
			const int iClassAttrIndex = pAttrs->getIndexFromName(_T("class"));
			const int iIdAttrIndex = pAttrs->getIndexFromName(_T("id"));

			const CString sClass = (-1 == iClassAttrIndex) ? _T("") : pAttrs->getValue(iClassAttrIndex);
			const CString sId = (-1 == iIdAttrIndex) ? _T("") : pAttrs->getValue(iIdAttrIndex);

			
 
			if (!sId.IsEmpty())	// has id attribute?
			{
				const CString sSharp = _T("#");
				const CString sSharpId = sSharp + sId;
				const CString sTypeSharpClass = sType + sSharp + sId;

				if (0 == sSharpId.CompareNoCase (sSelector))
					return TRUE;
				if (0 == sTypeSharpClass.CompareNoCase (sSelector))
					return TRUE;


			}

			// class is space-separated array of classes! (ex.: class="headerBar cl_lightweight_topnav_slice") - we need to check all classes

			if (!sClass.IsEmpty())	// has class attribute?
			{
				CAtlArray<CString> aClassSelectors;
				CLiteHTMLElemStyleAttr::SpaceSeparatedValuesToArray (sClass, aClassSelectors);
				for (int nClassSelectorNumber = 0; nClassSelectorNumber < aClassSelectors.GetCount(); ++nClassSelectorNumber)
				{
					const CString sDot = _T(".");
					const CString sDotClass = sDot + aClassSelectors[nClassSelectorNumber];
					const CString sTypeDotClass = sType + sDot + aClassSelectors[nClassSelectorNumber];

					if (0 == sDotClass.CompareNoCase (sSelector))
						return TRUE;
					if (0 == sTypeDotClass.CompareNoCase (sSelector))
						return TRUE;
				}
			}
		}


		return FALSE;
	}

	BOOL Test (CAtlArray<CString> &aSelectors)
	{
		if (aSelectors.IsEmpty())
			return FALSE;
		
		// get last tag
		CString sSingleSelector = aSelectors [aSelectors.GetCount() - 1];
		
		if (TRUE == TestSingleSelector (sSingleSelector))
		{
			// ok, check other selectors
			aSelectors.RemoveAt (aSelectors.GetCount() - 1);	// delete last selector from array
			if (aSelectors.IsEmpty())
				return TRUE;
		}

		// may be parent equals this selector
		if (NULL == m_pParentTag)
			return FALSE;

		return m_pParentTag->Test (aSelectors);
	}
public:
	void setParentTag (CHTMLTagWrapper *aParent)
	{
		m_pParentTag = aParent;
		m_oStyle.SetParentFontSize ((NULL == m_pParentTag) ? 16.0 : m_pParentTag->getFontSize());
	}
	CHTMLTagWrapper *getParentTag ()
	{
		return m_pParentTag;
	}

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

	double getFontSize () const
	{
		return m_dFontSize;
	}
	void setFontSize (const double aFontSize)
	{
		m_dFontSize = aFontSize;
	}
	

	BOOL Test (const CString& aSelectors)
	{
		// TODO: impl 'test' - test CSS selectors
		// selector examples: 'type.class' , 'type.class child', '*', '.class', 'span[class=example]' etc.

		// omit universal selector '*.' and '*'
		if (NULL == m_pTag)
			return FALSE;

		// parse selector
		CAtlArray<CString> aParentSelectors;
		CLiteHTMLElemStyleAttr::SpaceSeparatedValuesToArray (aSelectors, aParentSelectors);
		if (aParentSelectors.GetCount() > 0)
		{
			
			// run throw parent hierarchy
			ATLTRACE2 (" ");

			// last item must to be equal to THIS tag
			CString sSingleSelector = aParentSelectors [aParentSelectors.GetCount() - 1];
			if (FALSE == TestSingleSelector (sSingleSelector))
				return FALSE;

			// ok, last selector equals this tag, check parent tags
			aParentSelectors.RemoveAt (aParentSelectors.GetCount() - 1);	// delete last selector from array
			if (aParentSelectors.IsEmpty())
				return TRUE;

			// check parent tags
			if (NULL != m_pParentTag)
			{
				return m_pParentTag->Test (aParentSelectors);
			}
		}
		
		// blank selector
		return FALSE;
	}
	BOOL set_attribute(const TCHAR* aAttr, const TCHAR* aValue)
	{
		if (m_pTag)
		{
			CLiteHTMLAttributes * pAttrs = m_pTag->getAttributes();
			if (pAttrs)
			{
				pAttrs->addAttribute (aAttr, aValue);
				return TRUE;
			}
		}
		return FALSE;
	}
	const TCHAR* get_attribute (const TCHAR* aAttr)
	{
		if (m_pTag)
		{
			CLiteHTMLAttributes * pAttrs = m_pTag->getAttributes();
			if (pAttrs)
			{
				return pAttrs->getAttribute(aAttr);
			}
		}
		return NULL;
	}

	CLiteHTMLElemStyleAttr& getStyle ()
	{
		return m_oStyle;
	}
	CString getStyleString ()
	{
		return m_oStyle.ToString ();
	}

	// unprocessed text
	CString getUnprocessedText () const
	{
		return m_sUnprecessedText;
	}
	void setUnprocessedText (CString &aText)
	{
		m_sUnprecessedText = aText;
	}
	void addUnprocessedText (CString &aText)
	{
		m_sUnprecessedText += aText;
	}
	bool isUnprocessedTextExists () const
	{
		return !m_sUnprecessedText.IsEmpty();
	}
	void clearUnprocessedText ()
	{
		m_sUnprecessedText.Empty();
	}
public:
	static BOOL IsNewLineStarting (CString &sTagName)
	{
		return (sTagName == _T("p")
			|| sTagName == _T("div")
			|| sTagName == _T("br")
			|| sTagName == _T("dd")
			|| sTagName == _T("dl")
			|| sTagName == _T("dir")
			|| sTagName == _T("dt")
			|| sTagName == _T("fieldset")
			|| sTagName == _T("form")
			|| sTagName == _T("frame")
			|| sTagName == _T("frameset")
			|| sTagName == _T("head")
			|| sTagName == _T("h1")
			|| sTagName == _T("h2")
			|| sTagName == _T("h3")
			|| sTagName == _T("h4")
			|| sTagName == _T("h5")
			|| sTagName == _T("h6")
			|| sTagName == _T("hr")
			|| sTagName == _T("html")
			|| sTagName == _T("iframe")
			|| sTagName == _T("input")
			|| sTagName == _T("li")
			|| sTagName == _T("ol")
			|| sTagName == _T("optgroup")
			|| sTagName == _T("option")
			|| sTagName == _T("select")
			|| sTagName == _T("table")
			|| sTagName == _T("thread")
			|| sTagName == _T("tfoot")
			|| sTagName == _T("tbody")
			|| sTagName == _T("th")
			|| sTagName == _T("tr")
			);
	}
	static BOOL IsNewLineStarting (CHTMLTagWrapper *pTagWrapper)
	{
		if (NULL == pTagWrapper || NULL == pTagWrapper->Tag())
			return FALSE;

		CString sTagName = pTagWrapper->Tag()->getTagName().MakeLower();
		return IsNewLineStarting(sTagName);		
	}
};
