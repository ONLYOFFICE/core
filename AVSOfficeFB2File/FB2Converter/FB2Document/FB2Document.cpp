#include "FB2Document.h"
#include "../Common.h"
#include "./../msxml/XML.h"

namespace FB2Converter
{
	FB2Document::FB2Document()
		:m_pXmlDoc( L"Msxml2.DOMDocument")
	{
		m_strMeta = _T("");
	}


	bool FB2Document::Read(const CString filepath)
	{
		m_strMeta = _T("");
		try
		{
			if (m_pXmlDoc->load(static_cast<_bstr_t>(filepath)) == VARIANT_TRUE)
			{
				MSXML2::IXMLDOMNodePtr description = m_pXmlDoc->selectSingleNode(L"FictionBook/description");
				if (NULL != description)
				{
					MSXML2::IXMLDOMNodePtr title = description->selectSingleNode(L"title-info");
					if(NULL != title)
					{
						MSXML2::IXMLDOMNodePtr author = title->selectSingleNode(L"author");
						if(NULL != author)
						{
							MSXML2::IXMLDOMNodePtr lastName = author->selectSingleNode(L"last-name");
							MSXML2::IXMLDOMNodePtr firstName = author->selectSingleNode(L"first-name");
							MSXML2::IXMLDOMNodePtr middleName = author->selectSingleNode(L"middle-name");
							if(NULL != lastName || NULL != firstName || NULL != middleName)
							{
								m_strMeta += _T("<author>");
								if(NULL != lastName)
									m_strMeta += CorrectString2(CString(lastName->text.GetBSTR()));
								if(NULL != firstName)
									m_strMeta += _T(" ") + CorrectString2(CString(firstName->text.GetBSTR()));
								if(NULL != middleName)
									m_strMeta += _T(" ") + CorrectString2(CString(middleName->text.GetBSTR()));
								m_strMeta += _T("</author>");
							}
						}
						MSXML2::IXMLDOMNodePtr booktitle = title->selectSingleNode(L"book-title");
						if(NULL != booktitle)
						{
							m_strMeta += _T("<name>") + CorrectString2(CString(booktitle->text.GetBSTR())) + _T("</name>");
						}
						MSXML2::IXMLDOMNodePtr annotation = title->selectSingleNode(L"annotation");
						if(NULL != annotation)
						{
							m_strMeta += _T("<annotation>") + CorrectString2(CString(annotation->text.GetBSTR())) + _T("</annotation>");
						}
						MSXML2::IXMLDOMNodePtr coverpage = title->selectSingleNode(L"coverpage");
						if(NULL != annotation)
						{
							MSXML2::IXMLDOMNodePtr coverpageimage = coverpage->selectSingleNode(L"image");
							if(NULL != coverpageimage && _T("") != XML::getAttr(coverpageimage, L"href"))
								m_strMeta += _T("<coverpage>1</coverpage>");
						}
					}
					MSXML2::IXMLDOMNodePtr publishinfo = description->selectSingleNode(L"publish-info");
					if(NULL != publishinfo)
					{
						MSXML2::IXMLDOMNodePtr publisher = publishinfo->selectSingleNode(L"publisher");
						if(NULL != publisher)
							m_strMeta += (_T("<publisher>") + CorrectString2(CString(publisher->text.GetBSTR())) + _T("</publisher>"));
					}
				}
			}
		}
		catch(...)
		{
			m_strMeta = _T("");
		}
		if(_T("") != m_strMeta)
		{
			m_strMeta = _T("<meta>") + m_strMeta + _T("</meta>");
		}
		return false;
	}


	bool FB2Document::Write(const CString filepath)
	{
		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath)) == S_OK)
			return true;
		return false;
	}
}