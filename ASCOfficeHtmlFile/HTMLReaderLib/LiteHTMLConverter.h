#pragma once
#include "LiteXHTMLWriter.h"
#include "LiteHTMLReader.h"
#include "HTMLTagWrapper.h"

class ILiteHTMLConverterEvent
{
public:
	virtual void OnProgress (double aProcent, bool &aAbort);
};

// flags
#define ECR_GENERAL					0x00000000
#define ECR_ALWAYSCLOSED			0x00000001
#define ECR_FORBIDDEN				0x00000002
#define ECR_HASNT_CHILDREN_TAG		0x00000004
#define ECR_EMULATE_HASTEXT			0x00000008


class CLiteHTMLConverter: public ILiteHTMLReaderEvents
{
protected:
	CLiteHTMLReader m_oReader;
	ILiteHTMLConverterEvent *pProgressEvent;
	IXHTMLWriter	*pXHTMLWriter;
	bool m_bAutoInlineClosing;

	typedef DWORD ECloseRules;
	struct STagInfo
	{
		//enum ECloseRules {ECR_GENERAL, ECR_MAYBEUNCLOSED, ECR_ALWAYSUNCLOSED, ECR_ALWAYSCLOSED, ECR_FORBIDDEN, ECR_HASNT_CHILDREN_TAG, ECR_EMULATE_HASTEXT} m_eCloseRules;
		ECloseRules m_eCloseRules;

	};
	typedef CMap<CString, LPCTSTR, STagInfo, STagInfo>	CTagsInfo;
	static CTagsInfo	m_mTagsInfo;	// collection of named colors
	
	CArray<CHTMLTagWrapper *> m_aOpenedTag;

public:
	CLiteHTMLConverter ()
		: pProgressEvent (NULL)
		, pXHTMLWriter (NULL)
		, m_bAutoInlineClosing (false)
	{
		InitTagInfo ();
	}
	virtual ~CLiteHTMLConverter ()
	{

	}
	HRESULT ToXHTML (HANDLE hHtmlFile, IXHTMLWriter *pWriter)
	{
		// both input and output files are opened
		HRESULT hRes = S_OK;
		
		// parser
		CLiteHTMLReader oReader;
		oReader.setEventHandler (this);

		// Store writer
		pXHTMLWriter = pWriter;
		

		// Start parsing
		UINT uiOpened = oReader.ReadFile (hHtmlFile);
		return hRes;
	}
protected:
	
	bool CheckEmulateHasText (CLiteHTMLTag *aLastUnclosedTag)
	{
		BOOL bRes = false;
		STagInfo rValue;

		if (TRUE == m_mTagsInfo.Lookup (aLastUnclosedTag->getTagName(), rValue))
		{
			if (rValue.m_eCloseRules & ECR_EMULATE_HASTEXT)
				return true;
		}
		return bRes;
	}
	bool CheckHasNoChildrenTags (CLiteHTMLTag *aLastUnclosedTag)
	{
		BOOL bRes = false;
		STagInfo rValue;

		if (TRUE == m_mTagsInfo.Lookup (aLastUnclosedTag->getTagName(), rValue))
		{
			if (rValue.m_eCloseRules & ECR_HASNT_CHILDREN_TAG)
				return true;
		}
		return bRes;
	}
	bool CheckForbiddenTag (CLiteHTMLTag *aLastUnclosedTag)
	{
		BOOL bRes = false;
		STagInfo rValue;

		if (TRUE == m_mTagsInfo.Lookup (aLastUnclosedTag->getTagName(), rValue))
		{
			if (rValue.m_eCloseRules & ECR_FORBIDDEN)
				return true;
		}
		return bRes;
	}
	bool CheckAutoInlineClosing (CLiteHTMLTag *aLastUnclosedTag)
	{
		// check auto inline closing
		// TODO: check avrious tags ("br", "meta", ...)

		BOOL bRes = false;
		STagInfo rValue;

		if (TRUE == m_mTagsInfo.Lookup (aLastUnclosedTag->getTagName(), rValue))
		{
			if (rValue.m_eCloseRules & ECR_ALWAYSCLOSED)
				return true;
		}
		return bRes;
	}
	virtual void BeginParse(DWORD dwAppData, bool &bAbort)
	{
		UNUSED_ALWAYS(dwAppData);
		TRACE (_T("CLiteHTMLConverter::BeginParse\n"));
		bAbort = false;

		if (pXHTMLWriter)
			pXHTMLWriter->BeginFile ();

		if (pProgressEvent)
			pProgressEvent->OnProgress (0, bAbort);

	}

	virtual void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		UNUSED_ALWAYS(pTag);
		UNUSED_ALWAYS(dwAppData);
		bAbort = false;

		ATLTRACE2 ("CLiteHTMLConverter::StartTag: %s %s\n", pTag->getTagName(), pTag->IsTagInline() ? "(inline)" : "");
		
		// Check: if previous tags must be closed
		CHTMLTagWrapper *pLastTag = NULL;
		const size_t szRecords = m_aOpenedTag.GetCount();
		if (0 != szRecords)
		{
			pLastTag = m_aOpenedTag.GetAt(szRecords - 1);
			if ((NULL != pLastTag) 
				&& (CheckAutoInlineClosing (pLastTag->Tag()) || CheckHasNoChildrenTags (pLastTag->Tag()))
				)
			{
				// previous tag must be closed
				if (pXHTMLWriter)
					pXHTMLWriter->EndTag (pLastTag);

				// remove last unclosed tag from tags quene
				m_aOpenedTag.RemoveAt (szRecords - 1);
				delete pLastTag;
				pLastTag = NULL;
			}
		}

		// Add unclosed tag to the opened tag quene
		CHTMLTagWrapper* pTagWrapper = new CHTMLTagWrapper(pTag);

		if (!CheckForbiddenTag (pTag))
		{
			// Write started tag
			if (pXHTMLWriter)
				pXHTMLWriter->StartTag (pTagWrapper, m_aOpenedTag.IsEmpty() ? NULL : m_aOpenedTag.GetAt (m_aOpenedTag.GetCount() - 1));
		}

		if (CheckEmulateHasText (pTag))
		{
			// emulate having text
			if (pXHTMLWriter)
				pXHTMLWriter->Characters (pTagWrapper, "");
		}

		m_aOpenedTag.Add (pTagWrapper);

		// Fire event
		if (pProgressEvent)
			pProgressEvent->OnProgress (0, bAbort);
	}

	virtual void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		UNUSED_ALWAYS(pTag);
		UNUSED_ALWAYS(dwAppData);
		bAbort = false;

		ATLTRACE2 ("CLiteHTMLConverter::EndTag: %s\n", pTag->getTagName());

		// Check unclosed tags
		size_t nLastTag = m_aOpenedTag.GetCount() - 1;
		while (nLastTag > 0)
		{
			//
			CHTMLTagWrapper* pOpenedTag = m_aOpenedTag.GetAt (nLastTag);
			if (NULL != pOpenedTag)
			{
				//
				if (pOpenedTag->Tag()->getTagName() == pTag->getTagName())
				{
					// ok, it's a normal situation. removing last tag and closing it in the writer
					if (!CheckForbiddenTag (pOpenedTag->Tag()))
					{
						if (pXHTMLWriter)
							pXHTMLWriter->EndTag (pOpenedTag);	// XHTMLWriter->EndTag (pOpenedTag);
					}
					
					m_aOpenedTag.RemoveAt (nLastTag);

					delete pOpenedTag;
					pOpenedTag = NULL;
					break;
				}
				else 
				{
					// we must to close a last opened tag, because 
					ATLTRACE2 ("Converter: EndTag(): force closing last opened tag (%s)\n", pOpenedTag->Tag()->getTagName());
					if (!CheckForbiddenTag (pOpenedTag->Tag()))
					{
						if (pXHTMLWriter)
							pXHTMLWriter->EndTag (pOpenedTag);
					}
					m_aOpenedTag.RemoveAt (nLastTag);

					delete pOpenedTag;
					pOpenedTag = NULL;
				}
			}
			else
			{
				ATLTRACE2 ("Converter: EndTag(): m_aOpenedTag.GetAt() returns NULL");
				break;
			}

			nLastTag--;
		}


		if (pProgressEvent)
			pProgressEvent->OnProgress (0, bAbort);
	}
	
	virtual void Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
	{
		UNUSED_ALWAYS(rText);
		UNUSED_ALWAYS(dwAppData);
		bAbort = false;

		ATLTRACE2 ("CLiteHTMLConverter::Characters: %s\n", rText);

		// give last opened tag and add text to tag
		if (!m_aOpenedTag.IsEmpty())
		{
			CHTMLTagWrapper *pTag = m_aOpenedTag.GetAt (m_aOpenedTag.GetCount() - 1);
			if (NULL == pTag)
			{
				ATLTRACE2 ("CLiteHTMLConverter::Characters: %s : fail to get last opened tag\n", rText);
				m_aOpenedTag.RemoveAt (m_aOpenedTag.GetCount() - 1);
				return;
			}

			// attach text to tag
			//pTag->Tag()->addText (rText);
			if (!CheckAutoInlineClosing (pTag->Tag()) && !CheckForbiddenTag (pTag->Tag()))
			{
				if (pXHTMLWriter)
					pXHTMLWriter->Characters (pTag, rText);
			}
			
		}
		else
		{
			ATLTRACE2 ("CLiteHTMLConverter::Characters: %s : there are no opened tag\n", rText);
		}

		if (pProgressEvent)
			pProgressEvent->OnProgress (0, bAbort);
	}
	
	virtual void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
	{
		UNUSED_ALWAYS(rComment);
		UNUSED_ALWAYS(dwAppData);
		bAbort = false;

		ATLTRACE2 ("CLiteHTMLConverter::Comment: %s\n", rComment);
		
		// emty comments

		if (pProgressEvent)
			pProgressEvent->OnProgress (0, bAbort);
	}

	virtual void EndParse(DWORD dwAppData, bool bIsAborted)
	{
		UNUSED_ALWAYS(dwAppData);
		UNUSED_ALWAYS(bIsAborted);

		ATLTRACE2 ("CLiteHTMLConverter::EndParse()\n");

		// Close all unclosed tags
		while (m_aOpenedTag.GetCount() > 0)
		{
			//
			const size_t nLastTag  = m_aOpenedTag.GetCount() - 1;

			CHTMLTagWrapper* pOpenedTag = m_aOpenedTag.GetAt (nLastTag);
			if (NULL != pOpenedTag)
			{
					// we must to close a last opened tag, because 
					ATLTRACE2 ("Converter: EndParse(): force closing last opened tag (%s)\n", pOpenedTag->Tag()->getTagName());
					if (pXHTMLWriter)
						pXHTMLWriter->EndTag (pOpenedTag);
					m_aOpenedTag.RemoveAt (nLastTag);

					delete pOpenedTag;
					pOpenedTag = NULL;
			}
			else
			{
				TRACE (_T("Converter: EndParse(): m_aOpenedTag.GetAt() returns NULL"));
			}
		}

		if (pProgressEvent)
			pProgressEvent->OnProgress (100.0, bIsAborted);
	}

public:
	// Event Handler
	void SetEventHandler (ILiteHTMLConverterEvent *aEventHandler)
	{
		pProgressEvent = aEventHandler;
	}
	ILiteHTMLConverterEvent *GetEventHandler (void) const
	{
		return pProgressEvent;
	}
	void SetWriter (IXHTMLWriter *aXHTMLWriter)
	{
		pXHTMLWriter = aXHTMLWriter;
	}
	IXHTMLWriter *GetWriter ()
	{
		return pXHTMLWriter;
	}

protected:
	void InitTagInfo ()
	{
		// tag parsing rules
		STagInfo sTagInfo;

		// meta
		sTagInfo.m_eCloseRules = ECR_ALWAYSCLOSED;
		m_mTagsInfo.SetAt ("meta", sTagInfo);


		// link
		sTagInfo.m_eCloseRules = ECR_ALWAYSCLOSED;
		m_mTagsInfo.SetAt ("link", sTagInfo);

		// br
		sTagInfo.m_eCloseRules = ECR_ALWAYSCLOSED;
		m_mTagsInfo.SetAt ("br", sTagInfo);

		// script
		sTagInfo.m_eCloseRules = ECR_FORBIDDEN;
		m_mTagsInfo.SetAt ("script", sTagInfo);

		// style
		sTagInfo.m_eCloseRules = ECR_HASNT_CHILDREN_TAG | ECR_EMULATE_HASTEXT;
		m_mTagsInfo.SetAt ("style", sTagInfo);

		// iframe
		sTagInfo.m_eCloseRules = ECR_HASNT_CHILDREN_TAG | ECR_EMULATE_HASTEXT;
		m_mTagsInfo.SetAt ("iframe", sTagInfo);
		
		// div		
		sTagInfo.m_eCloseRules = ECR_EMULATE_HASTEXT; // emulate having text (<tag attr="attr1"></tag> insteadof <tag attr="attr1"/> form)
		m_mTagsInfo.SetAt ("div", sTagInfo);
		m_mTagsInfo.SetAt ("p", sTagInfo);
		m_mTagsInfo.SetAt ("a", sTagInfo);

		// input
		sTagInfo.m_eCloseRules = ECR_ALWAYSCLOSED | ECR_HASNT_CHILDREN_TAG;
		m_mTagsInfo.SetAt ("input", sTagInfo);

	}
	// 
};