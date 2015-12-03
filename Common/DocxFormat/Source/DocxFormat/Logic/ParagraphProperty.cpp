#include "ParagraphProperty.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// PPrChange
		//--------------------------------------------------------------------------------
		CPPrChange::CPPrChange()
		{
			m_pParPr.Init();
			m_pParPr->m_bPPrChange = true;
		}
		CPPrChange::CPPrChange(XmlUtils::CXmlNode &oNode)
		{
			m_pParPr.Init();
			m_pParPr->m_bPPrChange = true;

			fromXML( oNode );
		}
		CPPrChange::CPPrChange(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pParPr.Init();
			m_pParPr->m_bPPrChange = true;

			fromXML( oReader );
		}
		const CPPrChange& CPPrChange::operator =(const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CPPrChange& CPPrChange::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		CPPrChange::~CPPrChange()
		{
		}

		void CPPrChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:pPrChange") != oNode.GetName() )
				return;

			oNode.ReadAttributeBase( _T("w:author"), m_sAuthor );
			oNode.ReadAttributeBase( _T("w:date"),   m_oDate );
			oNode.ReadAttributeBase( _T("w:id"),     m_oId );
			oNode.ReadAttributeBase( _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_pPr;

			if ( m_pParPr.IsInit() && oNode.GetNode( _T("w:pPr"), oNode_pPr ) )
				m_pParPr->fromXML( oNode_pPr );
		}

		void CPPrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				if ( _T("w:pPr") == sName )
					m_pParPr->fromXML( oReader );
			}
		}

		CString CPPrChange::toXML() const
		{
			CString sResult = _T("<w:pPrChange ");

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

			if ( m_pParPr.IsInit() )
				sResult += m_pParPr->toXML();

			sResult += _T("</w:pPrChange>");

			return sResult;
		}
		EElementType CPPrChange::getType() const
		{
			return et_w_pPrChange;
		}
		void CPPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),	 m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}
	} // Logic
} // OOX
