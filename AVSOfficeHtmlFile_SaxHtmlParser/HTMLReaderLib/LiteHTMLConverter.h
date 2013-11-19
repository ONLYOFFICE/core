#pragma once
//#include "LiteXHTMLWriter.h"
#include "IXHTMLWriter.h"
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
	bool m_bAddSpaceCharacter;
	bool m_bNewLineStarted;	// new line started flag

	typedef DWORD ECloseRules;
	struct STagInfo
	{
		//enum ECloseRules {ECR_GENERAL, ECR_MAYBEUNCLOSED, ECR_ALWAYSUNCLOSED, ECR_ALWAYSCLOSED, ECR_FORBIDDEN, ECR_HASNT_CHILDREN_TAG, ECR_EMULATE_HASTEXT} m_eCloseRules;
		ECloseRules m_eCloseRules;
	};
	typedef CAtlMap<CString, STagInfo>	CTagsInfo;
	static CTagsInfo	m_mTagsInfo;	// collection of named colors
	
	CAtlArray<CHTMLTagWrapper *> m_aOpenedTag;
	CHTMLTagWrapper *m_pTopTagWrapper;

public:
	CLiteHTMLConverter ()
		: pProgressEvent (NULL)
		, pXHTMLWriter (NULL)
		, m_bAutoInlineClosing (false)
		, m_bAddSpaceCharacter (false)
		, m_bNewLineStarted (true)
		, m_pTopTagWrapper (NULL)
	{
		InitTagInfo ();
	}
	virtual ~CLiteHTMLConverter ()
	{
		if (NULL != m_pTopTagWrapper)
		{
			m_pTopTagWrapper->DestroyChildren ();
			delete m_pTopTagWrapper;
			m_pTopTagWrapper = NULL;
		}
	}
	HRESULT ToXHTML (HANDLE hHtmlFile, IXHTMLWriter *pWriter, int aDefaultCodePage = CP_ACP);
	HRESULT ToXHTML (CStringW& aBuffer, IXHTMLWriter *pWriter);

protected:
	
	bool CheckEmulateHasText (CLiteHTMLTag *aLastUnclosedTag)
	{
		bool bRes = false;
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
		bool bRes = false;
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
		bool bRes = false;
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

		bool bRes = false;
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
		ATLTRACE2 (_T("CLiteHTMLConverter::BeginParse\n"));
		bAbort = false;

		if (pXHTMLWriter)
			pXHTMLWriter->BeginFile ();

		if (pProgressEvent)
			pProgressEvent->OnProgress (0, bAbort);

	}

	virtual void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		bAbort = false;

		CStringA sTagName;
		sTagName = pTag->getTagName();
		ATLTRACE2 ("CLiteHTMLConverter::StartTag: %s %s\n", sTagName, pTag->IsTagInline() ? "(inline)" : "");
		
		// Check: if previous tags must be closed
		size_t szRecords = m_aOpenedTag.GetCount();
		if (0 != szRecords)
		{
			CHTMLTagWrapper *pLastTag = m_aOpenedTag.GetAt(szRecords - 1);
			if ((NULL != pLastTag) 
				&& (CheckAutoInlineClosing (pLastTag->Tag()) || CheckHasNoChildrenTags (pLastTag->Tag()) || pLastTag->Tag()->IsTagInline())
				)
			{
				// previous tag must be closed
				if (pXHTMLWriter)
					pXHTMLWriter->EndTag (pLastTag);

				// remove last unclosed tag from tags quene
				m_aOpenedTag.RemoveAt (szRecords - 1);
				//delete pLastTag;
				//pLastTag = NULL;
			}
		}

		// check custom rules
		szRecords = m_aOpenedTag.GetCount();
		// 1. <li> <ul> <ol> tag wouldn't present inside of <li> tag
		if (0 == pTag->getTagName().CompareNoCase (_T("li")))
		{
			for (long nTag = (long) szRecords - 1; nTag >= 0; nTag--)
			{
				CHTMLTagWrapper *pTagItem = m_aOpenedTag.GetAt(nTag);
				if (NULL != pTagItem)
				{
					bool bFound = false;
					CString sTagItemName = pTagItem->Tag()->getTagName();
					if (0 == sTagItemName.CompareNoCase (_T("li")))
					{
						// current <li>-tag is situated inside of <li> tag
						// we must close parent <li> tag
						long nTagsToClose = szRecords - nTag;
						while (nTagsToClose > 0)
						{
							nTagsToClose--;

							CHTMLTagWrapper *pLastTag = m_aOpenedTag.GetAt (m_aOpenedTag.GetCount() - 1);
							
							if (pXHTMLWriter)
								pXHTMLWriter->EndTag (pLastTag);

							// remove last unclosed tag from tags quene
							m_aOpenedTag.RemoveAt (m_aOpenedTag.GetCount() - 1);
							//delete pLastTag;
							//pLastTag = NULL;
						}
						bFound = true;
						break;
					}
					else if (0 == sTagItemName.CompareNoCase (_T("ol")) || 0 == sTagItemName.CompareNoCase (_T("ul")))
					{
						bFound = true;
						break;
					}
					if (bFound)
						break;
				}
			}
		}

		// 2. <dt> tag wouldn't present inside of <dt> tag
		szRecords = m_aOpenedTag.GetCount();
		if (0 == pTag->getTagName().CompareNoCase (_T("dt")))
		{
			for (long nTag = (long) szRecords - 1; nTag >= 0; nTag--)
			{
				CHTMLTagWrapper *pTagItem = m_aOpenedTag.GetAt(nTag);
				if (NULL != pTagItem)
				{
					bool bFound = false;
					CString sTagItemName = pTagItem->Tag()->getTagName();
					if (0 == sTagItemName.CompareNoCase (_T("dt")))
					{
						// current <dt>-tag is situated inside of <dt> tag
						// we must close parent <dt> tag
						long nTagsToClose = szRecords - nTag;
						while (nTagsToClose > 0)
						{
							nTagsToClose--;

							CHTMLTagWrapper *pLastTag = m_aOpenedTag.GetAt (m_aOpenedTag.GetCount() - 1);
							
							if (pXHTMLWriter)
								pXHTMLWriter->EndTag (pLastTag);

							// remove last unclosed tag from tags quene
							m_aOpenedTag.RemoveAt (m_aOpenedTag.GetCount() - 1);
							//delete pLastTag;
							//pLastTag = NULL;
						}
						bFound = true;
						break;
					}
					else if (0 == sTagItemName.CompareNoCase (_T("dl")))
					{
						// dt must to be inside of <dl> tag
						bFound = true;
						break;
					}
					if (bFound)
						break;
				}
			}
		}
		// 3. <tbody> tag should be in the <table> tag
		szRecords = m_aOpenedTag.GetCount();
		if (0 == pTag->getTagName().CompareNoCase (_T("tbody")))
		{
			long nTagsToClose (0);
			for (long nTag = (long) szRecords - 1; nTag >= 0; nTag--)
			{
				CHTMLTagWrapper *pTagItem = m_aOpenedTag.GetAt(nTag);
				if (NULL != pTagItem)
				{
					bool bFound = false;
					CString sTagItemName = pTagItem->Tag()->getTagName();
					if (0 == sTagItemName.CompareNoCase (_T("table")))
					{
						break;
					}
					else if (0 == sTagItemName.CompareNoCase (_T("boby")))
					{
						nTagsToClose = 0;
						break;

					}
					else
					{
						// parent of <tbody> is not a <table>
						++nTagsToClose;
					}
				}
			}
			// close tags between (parent) tbody and table
			
			while (nTagsToClose > 0)
			{
				nTagsToClose--;
				CHTMLTagWrapper *pLastTag = m_aOpenedTag.GetAt (m_aOpenedTag.GetCount() - 1);
					
				if (pXHTMLWriter)
					pXHTMLWriter->EndTag (pLastTag);
				// remove last unclosed tag from tags quene
				m_aOpenedTag.RemoveAt (m_aOpenedTag.GetCount() - 1);
				//delete pLastTag;
				//pLastTag = NULL;
			}
		}
		// 4. clear m_bAddSpaceCharacter flag, if new tag must to caret-return.
		if (CHTMLTagWrapper::IsNewLineStarting (pTag->getTagName().MakeLower()))
		{
			m_bAddSpaceCharacter = false;
			m_bNewLineStarted = true;
		}
		else
		{
		}

		// Add unclosed tag to the opened tag quene
		CHTMLTagWrapper* pTagWrapper = new CHTMLTagWrapper(pTag);
		CHTMLTagWrapper* pParentTagWrapper = m_aOpenedTag.IsEmpty() ? NULL : m_aOpenedTag.GetAt (m_aOpenedTag.GetCount() - 1);

		if (NULL != pParentTagWrapper)
		{
			pParentTagWrapper->AddChild (pTagWrapper);
		}
		else
		{
			m_pTopTagWrapper = pTagWrapper;	// head of tags tree
		}
		
		pTagWrapper->setParentTag (pParentTagWrapper);

		if (!CheckForbiddenTag (pTag))
		{
			// Write started tag
			if (pXHTMLWriter)
				pXHTMLWriter->StartTag (pTagWrapper, pParentTagWrapper);
		}

		m_aOpenedTag.Add (pTagWrapper);

		// Fire event
		if (pProgressEvent)
			pProgressEvent->OnProgress (0, bAbort);
	}

	virtual void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		bAbort = false;

		CStringA sTagName;
		sTagName = pTag->getTagName();
		ATLTRACE2 ("CLiteHTMLConverter::EndTag: %s\n", sTagName);

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

					//delete pOpenedTag;
					//pOpenedTag = NULL;
					break;
				}
				else 
				{
					// we must to close a last opened tag, because 
					CStringA sTagLastName;
					sTagLastName = pOpenedTag->Tag()->getTagName();
					ATLTRACE2 ("Converter: EndTag(): force closing last opened tag (%s)\n", sTagLastName);
					if (0 == sTagLastName.CompareNoCase ("body") && 0 != sTagName.CompareNoCase("body"))
					{
						ATLTRACE2 ("Converter: EndTag(): force closing last opened tag, but it is the <body> tag! skipping.\n");
						break;
					}
					if (0 == sTagLastName.CompareNoCase ("html") && 0 != sTagName.CompareNoCase("html"))
					{
						ATLTRACE2 ("Converter: EndTag(): force closing last opened tag, but it is the <html> tag! skipping.\n");
						break;
					}

					if (!CheckForbiddenTag (pOpenedTag->Tag()))
					{
						if (pXHTMLWriter)
							pXHTMLWriter->EndTag (pOpenedTag);
					}
					m_aOpenedTag.RemoveAt (nLastTag);

					//delete pOpenedTag;
					//pOpenedTag = NULL;
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
		bAbort = false;

		CStringA sText;
		sText = rText;
		ATLTRACE2 ("CLiteHTMLConverter::Characters: %s\n", sText);

		// give last opened tag and add text to tag
		if (!m_aOpenedTag.IsEmpty())
		{
			CHTMLTagWrapper *pTag = m_aOpenedTag.GetAt (m_aOpenedTag.GetCount() - 1);
			if (NULL == pTag)
			{
				ATLTRACE2 ("CLiteHTMLConverter::Characters: %s : fail to get last opened tag\n", sText);
				m_aOpenedTag.RemoveAt (m_aOpenedTag.GetCount() - 1);
				return;
			}

			// attach text to tag
			//pTag->Tag()->addText (rText);
			bool cbAutoInlineClosing = CheckAutoInlineClosing (pTag->Tag());
			const bool cbForbidden = CheckForbiddenTag (pTag->Tag());

			if (cbAutoInlineClosing && !cbForbidden)	// this tag is "inline closed" - ex. '<br>' tag
			{
				// Attach a text to the parent tag, because it wouldn't has text
				if (m_aOpenedTag.GetCount() > 1)
				{
					for (int nParentTag = m_aOpenedTag.GetCount() - 2; nParentTag >= 0; --nParentTag)
					{
						CHTMLTagWrapper *pParentTag = m_aOpenedTag[nParentTag];
						const bool cbParentTagAutoInlineClosing = CheckAutoInlineClosing (pParentTag->Tag());
						const bool cbParentTagForbidden = CheckForbiddenTag (pParentTag->Tag());
						if (!cbParentTagAutoInlineClosing && !cbParentTagAutoInlineClosing)
						{
							pTag = pParentTag;	// we found parent tag fot text adding
							cbAutoInlineClosing = false;
							break;
						}
					}
				}
			}

			if (!cbAutoInlineClosing && !cbForbidden)
			{
				CString sTagName = pTag->Tag()->getTagName();
				pTag->Tag()->addText (rText);

				// check parent 'pre' tag
				CHTMLTagWrapper *pParentTag = pTag;
				bool bPreTag = false;
				while (NULL != pParentTag)
				{
					bPreTag = (0 == pParentTag->Tag()->getTagName().CompareNoCase(_T("pre")));
					
					if (bPreTag) 
						break;

					pParentTag = pParentTag->getParentTag();
				}


				if (bPreTag || 0 == sTagName.CompareNoCase (_T("textarea")))	// save all tabs, enters and speces for <pre> and <textarea> tags
				{
					if (pXHTMLWriter)
					{
						CString sResText = rText;
						sResText.Replace (_T("<"), _T("&lt;"));
						sResText.Replace (_T(">"), _T("&gt;"));
						pXHTMLWriter->Characters (pTag, sResText, false);
					}

					// this text already processed by 'pXHTMLWriter->Characters (pTag, rText);' command
				}
				else
				{	
					// format text for other tags
					CString sResultString;
					bool nNeedAddSpaceToBegin = m_bAddSpaceCharacter;

					for (int nChar = 0; nChar < rText.GetLength(); nChar++)
					{
						TCHAR tcChar = rText.GetAt(nChar);
						if (tcChar == '\r' 
							|| tcChar == ' ' 
							|| tcChar == '\t'
							|| tcChar == '\n')
						{

							m_bAddSpaceCharacter = true;
						}
						//else if (tcChar == '\n')
						//{
						//	// do nothing?
						//}
						else
						{
							if (m_bAddSpaceCharacter)
							{
								if (nNeedAddSpaceToBegin)
								{
									// add space from previous tags
									if (!m_bNewLineStarted)
									{
										/*
										if (NULL != pTag->getParentTag() && NULL != pXHTMLWriter)
										{
											CHTMLTagWrapper *pParent = pTagWrapper->getParentTag();
											int nTagIndex = pParent->GetChildIndex (pTagWrapper) - 1;
											CHTMLTagWrapper *pBranch = pParent->GetChildAt (nTagIndex);
											if (NULL == pBranch ||  0 == pBranch->Tag()->getTextsCount())
											{
												pTagWrapper->pXmlTagNode->appendChild (pTextPtr);
											}
											else
											{
												pBranch->pXmlTagNode->appendChild (pTextPtr);
											}
										}
										else
											sResultString = _T(" ") + sResultString;
										*/
										
										//pXHTMLWriter->Characters (pTag->getParentTag(), _T(" "), true);
										sResultString = _T(" ") + sResultString;
										
										/*if (NULL != pTag->getParentTag() && NULL != pXHTMLWriter)
											pXHTMLWriter->Characters (pTag->getParentTag(), _T(" "), true);
										else
											sResultString = _T(" ") + sResultString;
											*/
										
									}
									
									nNeedAddSpaceToBegin = false;
								}
								else
								{
									if (!m_bNewLineStarted)
									{
										sResultString += _T(" ");
									}
								}
								
								
								m_bAddSpaceCharacter = false;
							}
							sResultString += tcChar;
							m_bNewLineStarted = false;
						}
					}
					if (!sResultString.IsEmpty())
					{
						pTag->addUnprocessedText (sResultString);	// this text already processed by 'pXHTMLWriter->Characters (pTag, rText);' command
					}
					/*if (bInsertSpace)
					{
						sResultString += _T(" ");
						bInsertSpace = false;
					}*/
					
				}
			}
		}
		else
		{
			ATLTRACE2 ("CLiteHTMLConverter::Characters: %s : there are no opened tag\n", sText);
		}

		if (pProgressEvent)
			pProgressEvent->OnProgress (0, bAbort);
	}
	
	virtual void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
	{
		bAbort = false;

		CStringA sComment;
		sComment = rComment;
		ATLTRACE2 ("CLiteHTMLConverter::Comment: %s\n", sComment);
		
		// emty comments

		if (pProgressEvent)
			pProgressEvent->OnProgress (0, bAbort);
	}

	virtual void EndParse(DWORD dwAppData, bool bIsAborted)
	{
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
				
					CStringA sTagName;
					sTagName = pOpenedTag->Tag()->getTagName();
					ATLTRACE2 ("Converter: EndParse(): force closing last opened tag (%s)\n", sTagName);
					if (pXHTMLWriter)
						pXHTMLWriter->EndTag (pOpenedTag);
					m_aOpenedTag.RemoveAt (nLastTag);

					//delete pOpenedTag;
					//pOpenedTag = NULL;
			}
			else
			{
				ATLTRACE2 (_T("Converter: EndParse(): m_aOpenedTag.GetAt() returns NULL"));
			}
		}

		// destroy all tags
		if (NULL != m_pTopTagWrapper)
		{
			m_pTopTagWrapper->DestroyChildren ();
			delete m_pTopTagWrapper;
			m_pTopTagWrapper = NULL;
		}

		if (pProgressEvent)
			pProgressEvent->OnProgress (100.0, bIsAborted);

		if (pXHTMLWriter)
			pXHTMLWriter->EndFile();
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
		m_mTagsInfo.SetAt (_T("meta"), sTagInfo);


		// link
		sTagInfo.m_eCloseRules = ECR_ALWAYSCLOSED;
		m_mTagsInfo.SetAt (_T("link"), sTagInfo);

		// br
		sTagInfo.m_eCloseRules = ECR_ALWAYSCLOSED | ECR_HASNT_CHILDREN_TAG;
		m_mTagsInfo.SetAt (_T("br"), sTagInfo);

		// img
		sTagInfo.m_eCloseRules = ECR_ALWAYSCLOSED;
		m_mTagsInfo.SetAt (_T("img"), sTagInfo);

		// script
		sTagInfo.m_eCloseRules = ECR_FORBIDDEN;
		m_mTagsInfo.SetAt (_T("script"), sTagInfo);

		// style
		sTagInfo.m_eCloseRules = ECR_HASNT_CHILDREN_TAG | ECR_EMULATE_HASTEXT;
		m_mTagsInfo.SetAt (_T("style"), sTagInfo);

		// iframe
		sTagInfo.m_eCloseRules = ECR_HASNT_CHILDREN_TAG | ECR_EMULATE_HASTEXT;
		m_mTagsInfo.SetAt (_T("iframe"), sTagInfo);
		m_mTagsInfo.SetAt (_T("frame"), sTagInfo);
		
		// div		
		sTagInfo.m_eCloseRules = ECR_EMULATE_HASTEXT; // emulate having text (<tag attr="attr1"></tag> insteadof <tag attr="attr1"/> form)
		m_mTagsInfo.SetAt (_T("div"), sTagInfo);
		m_mTagsInfo.SetAt (_T("a"), sTagInfo);
		m_mTagsInfo.SetAt (_T("p"), sTagInfo);
		

		// input
		sTagInfo.m_eCloseRules = ECR_ALWAYSCLOSED | ECR_HASNT_CHILDREN_TAG;
		m_mTagsInfo.SetAt (_T("input"), sTagInfo);

		// hr
		sTagInfo.m_eCloseRules = ECR_ALWAYSCLOSED | ECR_HASNT_CHILDREN_TAG;
		m_mTagsInfo.SetAt (_T("hr"), sTagInfo);

	}
};