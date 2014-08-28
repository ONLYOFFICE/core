#pragma once
#ifndef OOX_FTNEDN_INCLUDE_H_
#define OOX_FTNEDN_INCLUDE_H_

#include "../Base/Nullable.h"

#include "WritingElement.h"

#include "Logic/Annotations.h"
#include "Logic/Paragraph.h"
#include "Logic/Sdt.h"
#include "Logic/Table.h"
#include "Math/oMathPara.h"
#include "Math/oMath.h"

// TO DO: Нехватающие классы:
//        <w:altChunk>
//        <w:customXml>
//        <w:del>
//        <w:ins>
//        <w:moveFrom>
//        <w:moveTo>

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CFtnEdn 17.11.2 && 17.11.10 (Part 1)
	//--------------------------------------------------------------------------------	
	class CFtnEdn : public WritingElementWithChilds<>
	{
	public:
		CFtnEdn()
		{
			m_eType = et_Unknown;
		}
		CFtnEdn(const XmlUtils::CXmlNode& oNode)
		{
			m_eType = et_Unknown;
			fromXML( (XmlUtils::CXmlNode&)oNode );
		}
		virtual ~CFtnEdn()
		{
		}
	public:

		const CFtnEdn& operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();

			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		virtual void ClearItems()
		{
			m_eType = et_Unknown;

			m_oId.reset();
			m_oType.reset();

			WritingElementWithChilds::ClearItems();
		}

	public:

		virtual void         fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:footnote") == oNode.GetName() )
				m_eType = et_w_footnote;
			else if ( _T("w:endnote") == oNode.GetName() )
				m_eType = et_w_endnote;
			else 
				return;

			oNode.ReadAttributeBase( _T("w:id"),   m_oId   );
			oNode.ReadAttributeBase( _T("w:type"), m_oType );

			XmlUtils::CXmlNodes oChilds;
			if ( oNode.GetNodes( _T("*"), oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						CString sName = oItem.GetName();
						WritingElement *pItem = NULL;

						/*if ( _T("w:altChunk") == sName )
							pItem = new Logic::AltChunk( oItem );
						else */if ( _T("w:bookmarkEnd") == sName )
							pItem = new Logic::CBookmarkEnd( oItem );
						else if ( _T("w:bookmarkStart") == sName )
							pItem = new Logic::CBookmarkStart( oItem );
						else if ( _T("w:commentRangeEnd") == sName )
							pItem = new Logic::CCommentRangeEnd( oItem );
						else if ( _T("w:commentRangeStart") == sName )
							pItem = new Logic::CCommentRangeStart( oItem );
						//else if ( _T("w:customXml") == sName )
						//	pItem = new Logic::CCustomXml( oItem );
						else if ( _T("w:customXmlDelRangeEnd") == sName )
							pItem = new Logic::CCustomXmlDelRangeEnd( oItem );
						else if ( _T("w:customXmlDelRangeStart") == sName )
							pItem = new Logic::CCustomXmlDelRangeStart( oItem );
						else if ( _T("w:customXmlInsRangeEnd") == sName )
							pItem = new Logic::CCustomXmlInsRangeEnd( oItem );
						else if ( _T("w:customXmlInsRangeStart") == sName )
							pItem = new Logic::CCustomXmlInsRangeStart( oItem );
						else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
							pItem = new Logic::CCustomXmlMoveFromRangeEnd( oItem );
						else if ( _T("w:customXmlMoveFromRangeStart") == sName )
							pItem = new Logic::CCustomXmlMoveFromRangeStart( oItem );
						else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
							pItem = new Logic::CCustomXmlMoveToRangeEnd( oItem );
						else if ( _T("w:customXmlMoveToRangeStart") == sName )
							pItem = new Logic::CCustomXmlMoveToRangeStart( oItem );
						//else if ( _T("w:del") == sName )
						//	pItem = new Logic::CDel( oItem );
						//else if ( _T("w:ins") == sName )
						//	pItem = new Logic::CIns( oItem );
						//else if ( _T("w:moveFrom") == sName )
						//	pItem = new Logic::CMoveFrom( oItem );
						else if ( _T("w:moveFromRangeEnd") == sName )
							pItem = new Logic::CMoveToRangeEnd( oItem );
						else if ( _T("w:moveFromRangeStart") == sName )
							pItem = new Logic::CMoveToRangeStart( oItem );
						//else if ( _T("w:moveTo") == sName )
						//	pItem = new Logic::CMoveTo( oItem );
						else if ( _T("w:moveToRangeEnd") == sName )
							pItem = new Logic::CMoveToRangeEnd( oItem );
						else if ( _T("w:moveToRangeStart") == sName )
							pItem = new Logic::CMoveToRangeStart( oItem );
						else if ( _T("m:oMath") == sName )
							pItem = new Logic::COMath( oItem );
						else if ( _T("m:oMathPara") == sName )
							pItem = new Logic::COMathPara( oItem );
						else if ( _T("w:p") == sName )
							pItem = new Logic::CParagraph( oItem );
						else if ( _T("w:permEnd") == sName )
							pItem = new Logic::CPermEnd( oItem );
						else if ( _T("w:permStart") == sName )
							pItem = new Logic::CPermStart( oItem );
						else if ( _T("w:proofErr") == sName )
							pItem = new Logic::CProofErr( oItem );
						else if ( _T("w:sdt") == sName )
							pItem = new Logic::CSdt( oItem );
						else if ( _T("w:tbl") == sName )
							pItem = new Logic::CTbl( oItem );

						if ( pItem )
							m_arrItems.push_back( pItem );
					}
				}
			}
		}
        virtual CString      toXML() const
		{
			CString sResult;
			
			if ( m_eType == et_w_footnote )
				sResult = _T("<w:footnote ");
			else if ( m_eType == et_w_endnote )
				sResult = _T("<w:endnote ");
			else
				return _T("");

			ComplexTypes_WriteAttribute( _T("w:id=\""),   m_oId );
			ComplexTypes_WriteAttribute( _T("w:type=\""), m_oType );

			sResult += _T(">");

			for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )
				{
					sResult += m_arrItems[nIndex]->toXML();
				}
			}

			if ( m_eType == et_w_footnote )
				sResult += _T("</w:footnote>");
			else if ( m_eType == et_w_endnote )
				sResult += _T("</w:endnote>");

			return sResult;
		}
		virtual EElementType getType() const
		{
			return m_eType;
		}

	public:

		void AddParagraph(Logic::CParagraph *pPara)
		{
			m_arrItems.push_back( (WritingElement*)pPara );
		}

	public:

		OOX::EElementType                        m_eType; // <w:footnote> или <w:endnote>

		// Attributes
		nullable<SimpleTypes::CDecimalNumber<> > m_oId;
		nullable<SimpleTypes::CFtnEdn<>        > m_oType;

		// Childs
	};

	//--------------------------------------------------------------------------------
	// CFtnEdnSepRef 17.11.3 && 17.11.9 (Part 1)
	//--------------------------------------------------------------------------------	
	class CFtnEdnSepRef : public WritingElement
	{
	public:
		CFtnEdnSepRef()
		{
			m_eType = et_Unknown;
		}
		CFtnEdnSepRef(XmlUtils::CXmlNode& oNode)
		{
			m_eType = et_Unknown;
			fromXML( (XmlUtils::CXmlNode&)oNode );
		}
		CFtnEdnSepRef(XmlUtils::CXmlLiteReader& oReader)
		{
			m_eType = et_Unknown;
			fromXML( oReader );
		}
		virtual ~CFtnEdnSepRef()
		{
		}
	public:

		const CFtnEdnSepRef& operator =(const XmlUtils::CXmlNode& oNode)
		{
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}

		const CFtnEdnSepRef& operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}

	public:

		virtual void         fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:footnote") == oNode.GetName() )
				m_eType = et_w_footnote;
			else if ( _T("w:endnote") == oNode.GetName() )
				m_eType = et_w_endnote;
			else 
				return;

			oNode.ReadAttributeBase( _T("w:id"),   m_oId   );
		}
		virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			CWCharWrapper sName = oReader.GetName();
			if ( _T("w:footnote") == sName )
				m_eType = et_w_footnote;
			else if ( _T("w:endnote") == sName )
				m_eType = et_w_endnote;
			else 
				return;

			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
        virtual CString      toXML() const
		{
			CString sResult;
			
			if ( m_eType == et_w_footnote )
				sResult = _T("<w:footnote ");
			else if ( m_eType == et_w_endnote )
				sResult = _T("<w:endnote ");
			else
				return _T("");

			ComplexTypes_WriteAttribute( _T("w:id=\""),   m_oId );

			sResult += _T("/>");

			return sResult;
		}
		virtual EElementType getType() const
		{
			return m_eType;
		}

	private:

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.GetAttributesCount() <= 0 )
				return;

			if ( !oReader.MoveToFirstAttribute() )
				return;

			CWCharWrapper wsName = oReader.GetName();
			while( !wsName.IsNull() )
			{
				if ( _T("w:id") == wsName )
					m_oId = oReader.GetText();

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}

			oReader.MoveToElement();
		}

	public:

		OOX::EElementType                        m_eType; // <w:footnote> или <w:endnote>

		// Attributes
		nullable<SimpleTypes::CDecimalNumber<> > m_oId;
	};

} // namespace OOX

#endif // OOX_FTNEDN_INCLUDE_H_