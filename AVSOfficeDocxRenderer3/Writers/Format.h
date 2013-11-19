#pragma once
#include "../Common/FontManager.h"
#include "../Logic/DocxPage.h"
#include "TempManager.h"

namespace NSDocx
{
	using namespace NSCommon;

	class CMetadata
	{
	public:
		CString						m_strAuthor;
		CString						m_strName;
		CString						m_strPublisher;
		CString						m_strLanguage;

		CString						m_strCreatorProgram;

		CString						m_strCoverPath;

		CAtlList<IDocxItem*>		m_listAnnotation;

	public:
		CMetadata() : m_listAnnotation()
		{
			Clear();
		}
		AVSINLINE void Clear()
		{
			m_strAuthor			= _T("");
			m_strName			= _T("");
			m_strPublisher		= _T("");
			m_strLanguage		= _T("en");

			m_strCreatorProgram	= _T("Online Media Technologies Ltd.");

			m_strCoverPath		= _T("");

			POSITION pos = m_listAnnotation.GetHeadPosition();
			while (NULL != pos)
			{
				IDocxItem* pItem = m_listAnnotation.GetNext(pos);
				RELEASEOBJECT(pItem);
			}
			m_listAnnotation.RemoveAll();
		}
		AVSINLINE void ClearNoAttack()
		{
			m_listAnnotation.RemoveAll();
		}

		~CMetadata()
		{
			Clear();
		}

		CMetadata(const CMetadata& oSrc)
		{
			*this = oSrc;
		}
		CMetadata& operator=(const CMetadata& oSrc)
		{
			Clear();
			m_strAuthor			= oSrc.m_strAuthor;
			m_strName			= oSrc.m_strName;
			m_strPublisher		= oSrc.m_strPublisher;
			m_strLanguage		= oSrc.m_strLanguage;

			m_strCreatorProgram	= oSrc.m_strCreatorProgram;

			m_strCoverPath		= oSrc.m_strCoverPath;

			m_listAnnotation.AddHeadList(&oSrc.m_listAnnotation);
		}
	};

	class CSection
	{
	public:
		CAtlList<IDocxItem*>	m_arTitle;
		CAtlList<IDocxItem*>	m_arItems;

	public:
		CSection() : m_arTitle(), m_arItems()
		{
		}
		~CSection()
		{
			Clear();			
		}

		void Clear()
		{
			POSITION pos = m_arItems.GetHeadPosition();
			while (pos != NULL)
			{
				IDocxItem* pItem = m_arItems.GetNext(pos);
				RELEASEOBJECT(pItem);
			}
			m_arItems.RemoveAll();

			pos = m_arTitle.GetHeadPosition();
			while (pos != NULL)
			{
				IDocxItem* pItem = m_arTitle.GetNext(pos);
				RELEASEOBJECT(pItem);
			}
			m_arTitle.RemoveAll();
		}
		
		CSection(const CSection& oSrc)
		{
			*this = oSrc;
		}
		CSection& operator=(const CSection& oSrc)
		{
			Clear();
			
			m_arItems.AddHeadList(&oSrc.m_arItems);
			m_arTitle.AddHeadList(&oSrc.m_arTitle);

			return *this;
		}
	};

	class CNotes
	{
	public:
		CAtlList<IDocxItem*>	m_listItems;

	public:
		CNotes() : m_listItems()
		{
		}
		~CNotes()
		{
			Clear();			
		}

		void Clear()
		{
			POSITION pos = m_listItems.GetHeadPosition();
			while (pos != NULL)
			{
				IDocxItem* pItem = m_listItems.GetNext(pos);
				RELEASEOBJECT(pItem);
			}
			m_listItems.RemoveAll();
		}
		
		CNotes(const CNotes& oSrc)
		{
			*this = oSrc;
		}
		CNotes& operator=(const CNotes& oSrc)
		{
			Clear();
			
			m_listItems.AddHeadList(&oSrc.m_listItems);

			return *this;
		}
	};

	class CFormat
	{
	public:
		CMetadata			m_oMeta;
		CAtlList<CSection>	m_listSections;
		CNotes				m_oNotes;

		CTempManager		m_oTempManager;
		CImageManager		m_oImageManager;

	public:
		CFormat() : m_oMeta(), m_oNotes(), m_listSections(), m_oTempManager(), m_oImageManager()
		{
		}
		~CFormat()
		{
		}

		AVSINLINE void Clear()
		{
			m_listSections.RemoveAll();
			m_oNotes.Clear();

			m_oImageManager.NewDocument();
		}

		void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
			Clear();

			XmlUtils::CXmlNode oNodeMeta;
			if (oNode.GetNode(_T("metadata"), oNodeMeta))
			{
				m_oMeta.m_strAuthor			= oNodeMeta.GetValue(_T("author"));
				m_oMeta.m_strName			= oNodeMeta.GetValue(_T("name"));
				m_oMeta.m_strPublisher		= oNodeMeta.GetValue(_T("publisher"));
				m_oMeta.m_strCreatorProgram	= oNodeMeta.GetValue(_T("creator"));
				m_oMeta.m_strLanguage		= oNodeMeta.GetValue(_T("language"));
				m_oMeta.m_strCoverPath		= oNodeMeta.GetValue(_T("coverpath"));

				XmlUtils::CXmlNode oNodeAnno;
				if (oNodeMeta.GetNode(_T("annotation"), oNodeAnno))
				{
					LoadElements(oNodeAnno, m_oMeta.m_listAnnotation);
				}
			}

			XmlUtils::CXmlNodes oNodeSections;
			if (oNode.GetNodes(_T("sections"), oNodeSections))
			{
				int nCount = oNodeSections.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNodeSection;
					oNodeSections.GetAt(0, oNodeSection);

					CSection oSection;

					XmlUtils::CXmlNode oTitle;
					if (oNodeSection.GetNode(_T("title"), oTitle))
						LoadElements(oTitle, oSection.m_arTitle);

					XmlUtils::CXmlNode oElems;
					if (oNodeSection.GetNode(_T("items"), oElems))
						LoadElements(oElems, oSection.m_arItems);

					m_listSections.AddTail(oSection);					
				}
			}

			XmlUtils::CXmlNode oNodeNotes;
			if (oNode.GetNode(_T("notes"), oNodeNotes))
			{
				LoadElements(oNodeNotes, m_oNotes.m_listItems);
			}
		}

		CString SaveToXml()
		{
			CString strXml = _T("<ebook>");

			// metadata
			strXml += (_T("<author>") + m_oMeta.m_strAuthor + _T("</author>"));
			strXml += (_T("<name>") + m_oMeta.m_strAuthor + _T("</name>"));
			strXml += (_T("<publisher>") + m_oMeta.m_strAuthor + _T("</publisher>"));
			strXml += (_T("<creator>") + m_oMeta.m_strAuthor + _T("</creator>"));
			strXml += (_T("<language>") + m_oMeta.m_strAuthor + _T("</language>"));
			strXml += (_T("<coverpath>") + m_oMeta.m_strAuthor + _T("</coverpath>"));

			strXml += _T("<annotation>");
			POSITION pos = m_oMeta.m_listAnnotation.GetHeadPosition();
			while (NULL != pos)
			{
				IDocxItem* pItem = m_oMeta.m_listAnnotation.GetNext(pos);
				strXml += pItem->SaveToXml();
			}
			strXml += _T("</annotation>");

			// items
			strXml += _T("<sections>");
			pos = m_listSections.GetHeadPosition();
			while (NULL != pos)
			{
				CSection& oSection = m_listSections.GetNext(pos);
				strXml += _T("<section><title>");

				POSITION pos_ = oSection.m_arTitle.GetHeadPosition();
				while (NULL != pos_)
				{
					IDocxItem* pItem = oSection.m_arTitle.GetNext(pos_);
					strXml += pItem->SaveToXml();					
				}

				strXml += _T("</title><items>");

				pos_ = oSection.m_arItems.GetHeadPosition();
				while (NULL != pos_)
				{
					IDocxItem* pItem = oSection.m_arItems.GetNext(pos_);
					strXml += pItem->SaveToXml();					
				}

				strXml += _T("</items></section>");
			}
			strXml += _T("</sections>");

			// notes
			strXml += _T("<notes>");
			pos = m_oNotes.m_listItems.GetHeadPosition();
			while (NULL != pos)
			{
				IDocxItem* pItem = m_oNotes.m_listItems.GetNext(pos);
				pItem->SaveToXml();
			}
			strXml += _T("</notes>");

			strXml += _T("</ebook>");
			return strXml;
		}

		void LoadFromFile(CString strFile)
		{
			XmlUtils::CXmlNode oNode;
			if (oNode.FromXmlFile2(strFile))
				ReadFromXml(oNode);
		}
		void SaveToFile(CString strFile)
		{
			CFile oFile;
			oFile.CreateFile(strFile);
			oFile.WriteStringUTF8(SaveToXml());
			oFile.CloseFile();
		}

		static IDocxItem* CreateElementFromNode(XmlUtils::CXmlNode& oNode)
		{
			CString strName = oNode.GetName();
			IDocxItem* pItem = NULL;
			if (_T("p") == strName)
				pItem = new CDocxParagraph();
			else if (_T("num") == strName)
				pItem = new CDocxNumbering();
			else if (_T("table") == strName)
				pItem = new CDocxTable();
			else if (_T("graphic") == strName)
				pItem = new CDocxGraphicObject();

			if (NULL != pItem)
				pItem->ReadFromXml(oNode);

			return pItem;
		}
		static void LoadElements(XmlUtils::CXmlNode& oNode, CAtlList<IDocxItem*>& oArray)
		{
			XmlUtils::CXmlNodes oNodes;
			if (oNode.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode node;
					oNodes.GetAt(i, node);
					IDocxItem* pItem = CreateElementFromNode(node);
					if (NULL != pItem)
						oArray.AddTail(pItem);
				}
			}
		}
	};
}