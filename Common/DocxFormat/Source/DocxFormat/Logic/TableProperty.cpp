#include "TableProperty.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// TblPrChange 
		//--------------------------------------------------------------------------------	
		CTblPrChange::CTblPrChange()
		{
			m_pTblPr.Init();
			m_pTblPr->m_bTblPrChange = true;
		}
		CTblPrChange::CTblPrChange(XmlUtils::CXmlNode& oNode)
		{
			m_pTblPr.Init();
			m_pTblPr->m_bTblPrChange = true;

			fromXML( oNode );
		}
		CTblPrChange::CTblPrChange(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pTblPr.Init();
			m_pTblPr->m_bTblPrChange = true;

			fromXML( oReader );
		}
		CTblPrChange::~CTblPrChange()
		{
		}
		const CTblPrChange& CTblPrChange::operator = (const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CTblPrChange& CTblPrChange::operator = (const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CTblPrChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:tblPrChange") != oNode.GetName() )
				return;

			oNode.ReadAttributeBase( _T("w:author"), m_sAuthor );
			oNode.ReadAttributeBase( _T("w:date"),   m_oDate );
			oNode.ReadAttributeBase( _T("w:id"),     m_oId );
			oNode.ReadAttributeBase( _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_tblPr;

			if ( m_pTblPr.IsInit() && oNode.GetNode( _T("w:tblPr"), oNode_tblPr ) )
				m_pTblPr->fromXML( oNode_tblPr );

		}
		void CTblPrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				if ( m_pTblPr.IsInit() && _T("w:tblPr") == sName )
					m_pTblPr->fromXML( oReader );
			}
		}
		CString CTblPrChange::toXML() const
		{			
			CString sResult = _T("<w:tblPrChange ");

			if ( m_sAuthor.IsInit() )
			{
				sResult += "w:author=\"";
				sResult += m_sAuthor->GetString();
				sResult += "\" ";
			}

			if ( m_oDate.IsInit() )
			{
				sResult += "w:date=\"";
				sResult += m_oDate->ToString();
				sResult += "\" ";
			}

			if ( m_oId.IsInit() )
			{
				sResult += "w:id=\"";
				sResult += m_oId->ToString();
				sResult += "\" ";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += "oouserid=\"";
				sResult += m_sUserId->GetString();
				sResult += "\" ";
			}

			sResult += _T(">");

			if ( m_pTblPr.IsInit() )
				sResult += m_pTblPr->toXML();

			sResult += _T("</w:tblPrChange>");

			return sResult;
		}
		EElementType CTblPrChange::getType() const
		{
			return et_w_tblPrChange;
		}
		void CTblPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("oouserid"), m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}
	} // namespace Logic
} // namespace OOX
namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// TrPrChange 
		//--------------------------------------------------------------------------------	
		CTrPrChange::CTrPrChange()
		{
			m_pTrPr.Init();
			m_pTrPr->m_bTrPrChange = true;
		}
		CTrPrChange::CTrPrChange(XmlUtils::CXmlNode& oNode)
		{
			m_pTrPr.Init();
			m_pTrPr->m_bTrPrChange = true;

			fromXML( oNode );
		}
		CTrPrChange::CTrPrChange(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pTrPr.Init();
			m_pTrPr->m_bTrPrChange = true;

			fromXML( oReader );
		}
		CTrPrChange::~CTrPrChange()
		{
		}
		const CTrPrChange& CTrPrChange::operator = (const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CTrPrChange& CTrPrChange::operator = (const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CTrPrChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:trPrChange") != oNode.GetName() )
				return;

			oNode.ReadAttributeBase( _T("w:author"), m_sAuthor );
			oNode.ReadAttributeBase( _T("w:date"),   m_oDate );
			oNode.ReadAttributeBase( _T("w:id"),     m_oId );
			oNode.ReadAttributeBase( _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_tblPr;

			if ( m_pTrPr.IsInit() && oNode.GetNode( _T("w:trPr"), oNode_tblPr ) )
				m_pTrPr->fromXML( oNode_tblPr );

		}
		void CTrPrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				if ( m_pTrPr.IsInit() && _T("w:trPr") == sName )
					m_pTrPr->fromXML( oReader );
			}
		}
		CString CTrPrChange::toXML() const
		{			
			CString sResult = _T("<w:trPrChange ");

			if ( m_sAuthor.IsInit() )
			{
				sResult += "w:author=\"";
				sResult += m_sAuthor->GetString();
				sResult += "\" ";
			}

			if ( m_oDate.IsInit() )
			{
				sResult += "w:date=\"";
				sResult += m_oDate->ToString();
				sResult += "\" ";
			}

			if ( m_oId.IsInit() )
			{
				sResult += "w:id=\"";
				sResult += m_oId->ToString();
				sResult += "\" ";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += "oouserid=\"";
				sResult += m_sUserId->GetString();
				sResult += "\" ";
			}

			sResult += _T(">");

			if ( m_pTrPr.IsInit() )
				sResult += m_pTrPr->toXML();

			sResult += _T("</w:trPrChange>");

			return sResult;
		}
		EElementType CTrPrChange::getType() const
		{
			return et_w_trPrChange;
		}

		void CTrPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}
	} // namespace Logic
} // namespace OOX

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// TcPrChange 
		//--------------------------------------------------------------------------------	
		CTcPrChange::CTcPrChange()
		{
			m_pTcPr.Init();
			m_pTcPr->m_bTcPrChange = true;
		}
		CTcPrChange::CTcPrChange(XmlUtils::CXmlNode& oNode)
		{
			m_pTcPr.Init();
			m_pTcPr->m_bTcPrChange = true;

			fromXML( oNode );
		}
		CTcPrChange::CTcPrChange(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pTcPr.Init();
			m_pTcPr->m_bTcPrChange = true;

			fromXML( oReader );
		}
		CTcPrChange::~CTcPrChange()
		{
		}
		const CTcPrChange& CTcPrChange::operator = (const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CTcPrChange& CTcPrChange::operator = (const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CTcPrChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:tcPrChange") != oNode.GetName() )
				return;

			oNode.ReadAttributeBase( _T("w:author"), m_sAuthor );
			oNode.ReadAttributeBase( _T("w:date"),   m_oDate );
			oNode.ReadAttributeBase( _T("w:id"),     m_oId );
			oNode.ReadAttributeBase( _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_tcPr;

			if ( m_pTcPr.IsInit() && oNode.GetNode( _T("w:tcPr"), oNode_tcPr ) )
				m_pTcPr->fromXML( oNode_tcPr );

		}
		void CTcPrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				if ( m_pTcPr.IsInit() && _T("w:tcPr") == sName )
					m_pTcPr->fromXML( oReader );
			}
		}
		CString CTcPrChange::toXML() const
		{			
			CString sResult = _T("<w:tcPrChange ");

			if ( m_sAuthor.IsInit() )
			{
				sResult += "w:author=\"";
				sResult += m_sAuthor->GetString();
				sResult += "\" ";
			}

			if ( m_oDate.IsInit() )
			{
				sResult += "w:date=\"";
				sResult += m_oDate->ToString();
				sResult += "\" ";
			}

			if ( m_oId.IsInit() )
			{
				sResult += "w:id=\"";
				sResult += m_oId->ToString();
				sResult += "\" ";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += "oouserid=\"";
				sResult += m_sUserId->GetString();
				sResult += "\" ";
			}

			sResult += _T(">");

			if ( m_pTcPr.IsInit() )
				sResult += m_pTcPr->toXML();

			sResult += _T("</w:tcPrChange>");

			return sResult;
		}
		EElementType CTcPrChange::getType() const
		{
			return et_w_tcPrChange;
		}
		void CTcPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}
	} // namespace Logic
} // namespace OOX