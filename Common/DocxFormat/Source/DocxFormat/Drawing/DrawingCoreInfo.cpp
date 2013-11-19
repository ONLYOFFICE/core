#include "DrawingCoreInfo.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CNonVisualPictureProperties 20.1.2.2.7;20.2.2.2 (Part 1)
		//--------------------------------------------------------------------------------	
		void CNonVisualPictureProperties::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_eType = et_Unknown;

			// TO DO : Реализовать
		}
		void CNonVisualPictureProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_eType = et_Unknown;

			CWCharWrapper sName = oReader.GetName();
			if ( _T("a:cNvPicPr") == sName )
				m_eType = et_a_cNvPicPr;
			else if ( _T("p:cNvPicPr") == sName )
				m_eType = et_p_cNvPicPr;
			else if ( _T("pic:cNvPicPr") == sName )
				m_eType = et_pic_cNvPicPr;
			else
				return;

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				sName = oReader.GetName();
				if ( _T("a:picLocks") == sName )
					m_oPicLocks = oReader;
				else if ( _T("a:extLst") == sName )
					m_oExtLst = oReader;
			}
		}
		CString CNonVisualPictureProperties::toXML() const
		{
			CString sResult;

			if ( et_a_cNvPicPr == m_eType )
				sResult = _T("<a:cNvPicPr ");
			else if ( et_p_cNvPicPr == m_eType )
				sResult = _T("<p:cNvPicPr ");
			else if ( et_pic_cNvPicPr == m_eType )
				sResult = _T("<pic:cNvPicPr ");
			else
				return _T("");

			sResult += _T("preferRelativeResize=\"") + m_oPreferRelativeResize.ToString() + _T("\">");

			if ( m_oPicLocks.IsInit() )
				sResult += m_oPicLocks->toXML();

			if ( m_oExtLst.IsInit() )
				sResult += m_oExtLst->toXML();

			if ( et_a_cNvPicPr == m_eType )
				sResult += _T("</a:cNvPicPr>");
			else if ( et_p_cNvPicPr == m_eType )
				sResult += _T("</p:cNvPicPr>");
			else if ( et_pic_cNvPicPr == m_eType )
				sResult += _T("</pic:cNvPicPr>");

			return sResult;
		}
		//--------------------------------------------------------------------------------
		// CNonVisualDrawingProps 20.1.2.2.8;20.2.2.3;20.4.2.5 (Part 1)
		//--------------------------------------------------------------------------------	
		void CNonVisualDrawingProps::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_eType = et_Unknown;

			// TO DO: Реализовать
		}
		void CNonVisualDrawingProps::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_eType = et_Unknown;
			CWCharWrapper sName = oReader.GetName();

			if ( _T("wp:docPr") == sName )
				m_eType = et_wp_docPr;
			else if ( _T("p:cNvPr") == sName )
				m_eType = et_p_cNvPr;
			else if ( _T("pic:cNvPr") == sName )
				m_eType = et_pic_cNvPr;
			else
				return;

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				sName = oReader.GetName();
				if ( _T("a:extLst") == sName )
					m_oExtLst = oReader;
				else if ( _T("a:hlinkClick") == sName )
					m_oHlinkClick = oReader;
				else if ( _T("a:hlinkHover") == sName )
					m_oHlinkHover = oReader;
			}
		}
		CString CNonVisualDrawingProps::toXML() const
		{
			CString sResult = _T("<wp:docPr ");

			if ( et_wp_docPr == m_eType )
				sResult = _T("<wp:docPr ");
			else if ( et_p_cNvPr == m_eType )
				sResult += _T("<p:cNvPr ");
			else if ( et_pic_cNvPr == m_eType )
				sResult = _T("<pic:cNvPr ");
			else
				return _T("");

			if ( m_sDescr.IsInit()  )
			{
				sResult += _T("descr=\"");
				sResult += m_sDescr->GetString();
				sResult += _T("\" ");
			}
			if ( m_oHidden.IsInit() ) sResult += _T("hidden=\"") + m_oHidden->ToString() + _T("\" ");
			if ( m_oId.IsInit()     ) sResult += _T("id=\"")     + m_oId->ToString()     + _T("\" ");
			if ( m_sName.IsInit()   )
			{
				sResult += _T("name=\"");
				sResult += m_sName->GetString();
				sResult += _T("\" ");
			}
			if ( m_sTitle.IsInit()  )
			{
				sResult += _T("title=\"");
				sResult += m_sTitle->GetString();
				sResult += _T("\" ");
			}

			sResult += _T(">");

			if ( m_oExtLst.IsInit() )
				sResult += m_oExtLst->toXML();
			if ( m_oHlinkClick.IsInit() )
				sResult += m_oHlinkClick->toXML();
			if ( m_oHlinkHover.IsInit() )
				sResult += m_oHlinkHover->toXML();

			if ( et_wp_docPr == m_eType )
				sResult += _T("</wp:docPr>");
			else if ( et_p_cNvPr == m_eType )
				sResult += _T("</p:cNvPr>");
			else if ( et_pic_cNvPr == m_eType )
				sResult += _T("</pic:cNvPr>");

			return sResult;
		}
		//--------------------------------------------------------------------------------
		// CHyperlink 20.1.2.2.23;21.1.2.3.5 (Part 1)
		//--------------------------------------------------------------------------------	
		void CHyperlink::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать
		}
		void CHyperlink::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			// Определим тип текущей ноды
			CWCharWrapper sName = oReader.GetName();
			if ( _T("a:hlinkClick") == sName  )
				m_eType = et_a_hlinkClick;
			else if ( _T("a:hlinkHover") == sName )
				m_eType = et_a_hlinkHover;


			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				if ( _T("a:extLst") == sName )
					m_oExtLst = oReader;
				else if ( _T("a:snd") == sName )
					m_oSnd = oReader;
			}
		}
		CString CHyperlink::toXML() const
		{
			CString sResult;

			if ( et_a_hlinkClick == m_eType )
				sResult = _T("<a:hlinkClick ");
			else if ( et_a_hlinkHover == m_eType )
				sResult = _T("<a:hlinkHover ");
			else
				return _T("");

			if ( m_sAction.IsInit()         )
			{
				sResult += _T("action=\"");
				sResult += m_sAction->GetString();
				sResult += _T("\" ");
			}

			if ( m_oEndSnd.IsInit()         ) sResult += _T("endSnd=\"")         + m_oEndSnd->ToString()         + _T("\" ");
			if ( m_oHighlightClick.IsInit() ) sResult += _T("highlightClick=\"") + m_oHighlightClick->ToString() + _T("\" ");
			if ( m_oHistory.IsInit()        ) sResult += _T("history=\"")        + m_oHistory->ToString()        + _T("\" ");
			if ( m_oId.IsInit()             ) sResult += _T("r:id=\"")           + m_oId->ToString()             + _T("\" ");

			if ( m_sInvalidUrl.IsInit()     )
			{
				sResult += _T("invalidUrl=\"");
				sResult += m_sInvalidUrl->GetString();
				sResult += _T("\" ");
			}
			if ( m_sTgtFrame.IsInit()       )
			{
				sResult += _T("tgtFrame=\"");
				sResult += m_sTgtFrame->GetString();
				sResult += _T("\" ");
			}
			if ( m_sTooltip.IsInit()        )
			{
				sResult += _T("tooltip=\"");
				sResult += m_sTooltip->GetString();
				sResult += _T("\" ");
			}

			sResult += _T(">");

			if ( m_oExtLst.IsInit() )
				sResult += m_oExtLst->toXML();

			if ( m_oSnd.IsInit() )
				sResult += m_oSnd->toXML();

			if ( et_a_hlinkClick == m_eType )
				sResult = _T("</a:hlinkClick>");
			else if ( et_a_hlinkHover == m_eType )
				sResult = _T("</a:hlinkHover>");

			return sResult;
		}
	} // Drawing
} // OOX