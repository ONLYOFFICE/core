#pragma once
#include "../Common/FontManager.h"
#include "Text.h"

namespace NSDocx
{
	class IDocxItem
	{
	public:
		enum EDocxItemType
		{
			ebitParagraph	= 0,
			ebitTable		= 1,
			ebitGraphic		= 2,
			ebitNumbering	= 3
		};

	public:
		EDocxItemType	m_eType;
		CDoubleRect		m_rcBounds;
		bool m_bIsPageBreakAfter;
		virtual ~IDocxItem() 
		{
		}

	public:
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)			= 0;
		virtual CString SaveToXml()									= 0;
	};

	class CDocxTable : public IDocxItem
	{
	public:
		CString m_strTableXml;

	public:
		CDocxTable()
		{
			m_eType = ebitTable;
			m_strTableXml = _T("");
			m_bIsPageBreakAfter = false;
		}
		virtual ~CDocxTable()
		{
		}

		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
			m_strTableXml = oNode.GetXml();
		}
		virtual CString SaveToXml()
		{			
			return m_strTableXml;
		}
	};

	class CDocxGraphicObject : public IDocxItem
	{
	public:
		NSCommon::CImageInfo m_oInfo;
		LONG	m_lWrapMode;

	public:
		CDocxGraphicObject()
		{
			m_eType		= ebitGraphic;
			m_lWrapMode	= 0;
			m_bIsPageBreakAfter = false;
		}
		virtual ~CDocxGraphicObject()
		{
		}

	public:
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
			m_lWrapMode = oNode.ReadAttributeInt(_T("wrap"));
			m_rcBounds.left		= 0;
			m_rcBounds.top		= 0;
			m_rcBounds.right	= oNode.ReadAttributeDouble(_T("width"));
			m_rcBounds.bottom	= oNode.ReadAttributeDouble(_T("height"));
			m_oInfo.m_lID		= oNode.ReadAttributeInt(_T("id"));
			m_oInfo.m_eType		= (_T("jpg") == oNode.GetAttribute(_T("type"))) ? NSCommon::itJPG : NSCommon::itPNG;
		}
		virtual CString SaveToXml()
		{
			CString strRet = _T("");
			strRet.Format(_T("<graphic wrap=\"%d\" x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" id=\"%d\" type=\"%s\"/>"), 
				m_lWrapMode, 0, 0, m_rcBounds.GetWidth(), m_rcBounds.GetHeight(), m_oInfo.m_lID,
				(NSCommon::itJPG == m_oInfo.m_eType) ? _T("jpg") : _T("png"));
			return strRet;
		}
	};

	class CDocxParagraph : public IDocxItem
	{
	public:
		class CSpan
		{
		public:
			// если имадж есть - то этот спан - имадж. и никакого текста нету!!!
			CImageInfo				m_oImageInfo;
			double					m_dImageWidth;
			double					m_dImageHeight;

			double					m_dX;
			double					m_dWidthSpan;

			double					m_dBaselineOffset;

			NSStructures::CFont		m_oFont;
			NSStructures::CBrush	m_oBrush;
			CTextItem				m_oText;

		public:
			CSpan() : m_oImageInfo(), m_oFont(), m_oBrush(), m_oText(10)
			{
				m_dImageWidth		= 0;
				m_dImageHeight		= 0;

				m_dX				= 0;
				m_dWidthSpan		= 0;

				m_dBaselineOffset	= 0;
			}
			~CSpan()
			{
			}
			CSpan(const CSpan& oSrc)
			{
				*this = oSrc;
			}
			CSpan& operator=(const CSpan& oSrc)
			{
				m_oImageInfo	= oSrc.m_oImageInfo;
				m_dImageWidth	= oSrc.m_dImageWidth;
				m_dImageHeight	= oSrc.m_dImageHeight;

				m_dX			= oSrc.m_dX;
				m_dWidthSpan	= oSrc.m_dWidthSpan;
				m_dBaselineOffset = oSrc.m_dBaselineOffset;

				m_oFont			= oSrc.m_oFont;
				m_oBrush		= oSrc.m_oBrush;
				m_oText			= oSrc.m_oText;

				return *this;
			}

			CString ToXml()
			{
				if (-1 != m_oImageInfo.m_lID)
				{
					CString strRet = _T("");
					strRet.Format(_T("<image id=\"%d\" type=\"%d\"/>"), m_oImageInfo.m_lID, (int)m_oImageInfo.m_eType);
					return strRet;
				}
				CString strRet = _T("");
				strRet.Format(_T("<span baselineoffset=\"%d\">"), (LONG)m_dBaselineOffset);
				strRet += m_oFont.ToXmlString();
				strRet += m_oBrush.ToXmlString();
				strRet += m_oText.GetCString();
				strRet += _T("</span>");
				return strRet;
			}
			void FromXml(XmlUtils::CXmlNode& oNode)
			{
				if (_T("image") == oNode.GetName())
				{
					m_oImageInfo.m_lID		= oNode.ReadAttributeInt(_T("id"));
					m_oImageInfo.m_eType	= (NSCommon::ImageType)oNode.ReadAttributeInt(_T("type"));
					return;
				}
				
				m_dBaselineOffset = oNode.ReadAttributeDouble(_T("baselineoffset"));
				XmlUtils::CXmlNodes oNodes;
				if (oNode.GetChilds(oNodes))
				{
					int nCount = oNodes.GetCount();

					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNodeS;
						oNodes.GetAt(i, oNodeS);

						CString sName = oNodeS.GetName();

						if (_T("#text") == sName)
						{
							m_oText += (CString)oNodeS.GetXml();
						}
						else if (_T("font") == sName)
						{
							m_oFont.FromXmlNode(oNodeS);
						}
						else if (_T("brush") == sName)
						{
							m_oBrush.FromXmlNode(oNodeS);
						}
					}
				}		
			}
		};
		class CLine
		{
		public:
			// если имадж есть - то это Image линия. И никаких спанов быть не может
			CImageInfo			m_oImageInfo;
			CAtlList<CSpan>		m_listSpans;

			double				m_dTop;
			double				m_dBaselinePos;

		public:
			CLine() : m_listSpans(), m_oImageInfo()
			{
				m_dTop			= -1;
				m_dBaselinePos	= -1;
			}
			~CLine()
			{
			}
			CLine(const CLine& oSrc)
			{
				*this = oSrc;
			}
			CLine& operator=(const CLine& oSrc)
			{
				m_dTop			= oSrc.m_dTop;
				m_dBaselinePos	= oSrc.m_dBaselinePos;

				m_oImageInfo	= oSrc.m_oImageInfo;
				m_listSpans.RemoveAll();
				
				POSITION pos = oSrc.m_listSpans.GetHeadPosition();
				while (NULL != pos)
				{
					CSpan oSpan = oSrc.m_listSpans.GetNext(pos);
					m_listSpans.AddTail(oSpan);
				}

				return *this;
			}
			AVSINLINE bool CheckTitle()
			{
				if (-1 != m_oImageInfo.m_lID)
					return false;

				POSITION pos = m_listSpans.GetHeadPosition();
				while (NULL != pos)
				{
					CSpan& oSpan = m_listSpans.GetNext(pos);
					if (oSpan.m_dBaselineOffset != 0)
						return false;
				}
				return true;
			}			
		};

	public:
		LONG				m_lTextAlign;
		LONG				m_lStyle;
		LONG				m_lNumberingLevel;
	
		bool				m_bIsComplete;
		CAtlList<CLine>		m_arLines;

	public:
		CDocxParagraph() : m_arLines()
		{
			m_eType			= ebitParagraph;
			m_lTextAlign	= 0;
			m_lStyle		= 0;
			m_lNumberingLevel	= 0;

			m_bIsPageBreakAfter = false;

			m_bIsComplete	= true;
		}
		~CDocxParagraph()
		{			
		}
		CDocxParagraph(const CDocxParagraph& oSrc)
		{
			m_eType			= ebitParagraph;
			*this = oSrc;
		}
		CDocxParagraph& operator=(const CDocxParagraph& oSrc)
		{
			m_lTextAlign	= oSrc.m_lTextAlign;
			m_lStyle		= oSrc.m_lStyle;
			m_lNumberingLevel = oSrc.m_lNumberingLevel;
			
			m_arLines.RemoveAll();
			POSITION pos = oSrc.m_arLines.GetHeadPosition();
			while (NULL != pos)
			{
				CLine oLine = oSrc.m_arLines.GetNext(pos);
				m_arLines.AddTail(oLine);
			}
			m_bIsComplete = oSrc.m_bIsComplete;

			return *this;
		}

		AVSINLINE void Clear()
		{
			m_lTextAlign	= 0;
			m_lStyle		= 0;
			m_lNumberingLevel = 0;

			m_arLines.RemoveAll();
		}

		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
			Clear();

			m_lTextAlign		= oNode.ReadAttributeInt(_T("align"));
			m_lStyle			= oNode.ReadAttributeInt(_T("style"));
			m_lNumberingLevel	= oNode.ReadAttributeInt(_T("level"));

			m_arLines.AddTail();
			CLine& oLine = m_arLines.GetTail();

			XmlUtils::CXmlNodes oNodes;
			if (oNode.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					oLine.m_listSpans.AddTail();
					CSpan& oSpan = oLine.m_listSpans.GetTail();

					XmlUtils::CXmlNode oNodeSpan;
					oNodes.GetAt(i, oNodeSpan);
					oSpan.FromXml(oNodeSpan);
				}
			}
		}
		virtual CString SaveToXml()
		{
			CString strHead = _T("");
			strHead.Format(_T("<p align=\"%d\" style=\"%d\" level=\"%d\">"), m_lTextAlign, m_lStyle, m_lNumberingLevel);

			CString strPar = strHead;

			CSpan* pSpan = NULL;

			POSITION posLine = m_arLines.GetHeadPosition();
			while (NULL != posLine)
			{
				CLine& oLine = m_arLines.GetNext(posLine);

				POSITION posSpan = oLine.m_listSpans.GetHeadPosition();
				while (NULL != posSpan)
				{
					CSpan& oSpan = oLine.m_listSpans.GetNext(posSpan);
					if ((NULL == posSpan) && (NULL != posLine))
						oSpan.m_oText.CheckLastSpanLine();

					if (NULL == pSpan)
						pSpan = &oSpan;
					else if (pSpan->m_oFont.IsEqual(&oSpan.m_oFont) && pSpan->m_oBrush.IsEqual(&oSpan.m_oBrush)
						&& (oSpan.m_oImageInfo.m_lID == -1) && (pSpan->m_oImageInfo.m_lID == -1))
					{
						pSpan->m_oText += oSpan.m_oText;
					}
					else
					{
						strPar += pSpan->ToXml();
						pSpan = &oSpan;
					}
				}
			}

			if (NULL != pSpan)
			{
				strPar += pSpan->ToXml();
			}

			strPar += _T("</p>");
			return strPar;
		}

		CString GetAllText()
		{
			CString strText = _T("");
			POSITION posLine = m_arLines.GetHeadPosition();
			while (NULL != posLine)
			{
				NSDocx::CDocxParagraph::CLine& oLine = m_arLines.GetNext(posLine);

				POSITION posSpan = oLine.m_listSpans.GetHeadPosition();
				while (NULL != posSpan)
				{
					NSDocx::CDocxParagraph::CSpan& oSpan = oLine.m_listSpans.GetNext(posSpan);
					if ((NULL == posSpan) && (NULL != posLine))
						oSpan.m_oText.CheckLastSpanLine();

					if (-1 != oSpan.m_oImageInfo.m_lID)
						continue;

					strText += oSpan.m_oText.GetCString();
				}
			}
			return strText;
		}

		AVSINLINE bool CheckTitle()
		{
			POSITION pos = m_arLines.GetHeadPosition();
			while (NULL != pos)
			{
				CLine& oLine = m_arLines.GetNext(pos);
				if (!oLine.CheckTitle())
					return false;
			}
			return true;
		}

		AVSINLINE void CheckShadows()
		{
			POSITION posLine = m_arLines.GetHeadPosition();
			while (NULL != posLine)
			{
				NSDocx::CDocxParagraph::CLine& oLine = m_arLines.GetNext(posLine);
				NSDocx::CDocxParagraph::CSpan* pSpan = NULL;

				POSITION posSpan = oLine.m_listSpans.GetHeadPosition();
				while (NULL != posSpan)
				{
					POSITION posOld = posSpan;
					NSDocx::CDocxParagraph::CSpan& oSpan = oLine.m_listSpans.GetNext(posSpan);
					if ((NULL == posSpan) && (NULL != posLine))
						oSpan.m_oText.CheckLastSpanLine();

					if (NULL == pSpan)
						pSpan = &oSpan;
					else if (oSpan.m_oText.IsEqualLast(pSpan->m_oText, ((pSpan->m_dX + pSpan->m_dWidthSpan - oSpan.m_dX) > 1) ? TRUE : FALSE))
					{
						// это тень!
						oLine.m_listSpans.RemoveAt(posOld);
						continue;
					}					
				}
			}
		}
	};

	class CDocxNumbering : public IDocxItem
	{
	public:
		CAtlArray<CDocxParagraph>	m_arParagraphs;
		LONG						m_lType;

	public:
		CDocxNumbering() : m_arParagraphs()
		{
			m_lType = ebitNumbering;
			m_bIsPageBreakAfter = false;
		}
		CDocxNumbering(const CDocxNumbering& oSrc)
		{
			m_lType = ebitNumbering;
			*this = oSrc;
		}
		CDocxNumbering& operator=(const CDocxNumbering& oSrc)
		{
			m_lType			= oSrc.m_lType;
			m_arParagraphs.Copy(oSrc.m_arParagraphs);
			return *this;
		}
		virtual ~CDocxNumbering()
		{
		}

		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
			m_arParagraphs.RemoveAll();

			m_lType = oNode.ReadAttributeInt(_T("type"));
			XmlUtils::CXmlNodes oParagraphs;
			if (oNode.GetNodes(_T("p"), oParagraphs))
			{
				int nCount = oParagraphs.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oMem;
					oParagraphs.GetAt(i, oMem);
					m_arParagraphs.Add();
					m_arParagraphs[m_arParagraphs.GetCount() - 1].ReadFromXml(oMem);
				}
			}
		}
		virtual CString SaveToXml()
		{
			CString strXml = _T("");
			strXml.Format(_T("<num type=\"%d\">"), m_lType);

			size_t nCount = m_arParagraphs.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				strXml += m_arParagraphs[i].SaveToXml();
			}
			strXml += _T("</num>");
			return strXml;
		}
	};
}