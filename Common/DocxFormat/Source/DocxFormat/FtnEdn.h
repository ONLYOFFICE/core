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
	class CFtnEdn : public WritingElement
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
			for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )
					delete m_arrItems[nIndex];

				m_arrItems[nIndex] = NULL;
			}

			m_arrItems.RemoveAll();
		}
	public:

		const CFtnEdn& operator =(const XmlUtils::CXmlNode& oNode)
		{
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}

		//const CFtnEdn& operator =(const CFtnEdn& oOther)
		//{
		//	Clear();

		//	m_eType = oOther.m_eType;
		//	m_oId   = oOther.m_oId;
		//	m_oType = oOther.m_oType;

		//	for ( int nIndex = 0; nIndex < oOther.m_arrItems.GetSize(); nIndex++ )
		//	{
		//		WritingElement *pItem = oOther.m_arrItems[nIndex];
		//		WritingElement *pNewItem = NULL;

		//		/*if ( et_w_altChunk == pItem->getType() )
		//			pNewItem = new Logic::AltChunk( (Logic::AltChunk*)pItem );
		//		else */if ( et_w_bookmarkEnd == pItem->getType() )
		//			pNewItem = new Logic::CBookmarkEnd( (Logic::CBookmarkEnd*)pItem );
		//		else if ( et_w_bookmarkStart == pItem->getType() )
		//			pNewItem = new Logic::CBookmarkStart( (Logic::CBookmarkStart*)pItem );
		//		else if ( et_w_commentRangeEnd == pItem->getType() )
		//			pNewItem = new Logic::CCommentRangeEnd( (Logic::CCommentRangeEnd*)pItem );
		//		else if ( et_w_commentRangeStart == pItem->getType() )
		//			pNewItem = new Logic::CCommentRangeStart( (Logic::CCommentRangeStart*)pItem );
		//		//else if ( et_w_customXml == pItem->getType() )
		//		//	pNewItem = new Logic::CCustomXml( (Logic::CCustomXml*)pItem );
		//		else if ( et_w_customXmlDelRangeEnd == pItem->getType() )
		//			pNewItem = new Logic::CCustomXmlDelRangeEnd( (Logic::CCustomXmlDelRangeEnd*)pItem );
		//		else if ( et_w_customXmlDelRangeStart == pItem->getType() )
		//			pNewItem = new Logic::CCustomXmlDelRangeStart( (Logic::CCustomXmlDelRangeStart*)pItem );
		//		else if ( et_w_customXmlInsRangeEnd == pItem->getType() )
		//			pNewItem = new Logic::CCustomXmlInsRangeEnd( (Logic::CCustomXmlInsRangeEnd*)pItem );
		//		else if ( et_w_customXmlInsRangeStart == pItem->getType() )
		//			pNewItem = new Logic::CCustomXmlInsRangeStart( (Logic::CCustomXmlInsRangeStart*)pItem );
		//		else if ( et_w_customXmlMoveFromRangeEnd == pItem->getType() ) 
		//			pNewItem = new Logic::CCustomXmlMoveFromRangeEnd( (Logic::CCustomXmlMoveFromRangeEnd*)pItem );
		//		else if ( et_w_customXmlMoveFromRangeStart == pItem->getType() )
		//			pNewItem = new Logic::CCustomXmlMoveFromRangeStart( (Logic::CCustomXmlMoveFromRangeStart*)pItem );
		//		else if ( et_w_customXmlMoveToRangeEnd == pItem->getType() ) 
		//			pNewItem = new Logic::CCustomXmlMoveToRangeEnd( (Logic::CCustomXmlMoveToRangeEnd*)pItem );
		//		else if ( et_w_customXmlMoveToRangeStart == pItem->getType() )
		//			pNewItem = new Logic::CCustomXmlMoveToRangeStart( (Logic::CCustomXmlMoveToRangeStart*)pItem );
		//		//else if ( et_w_del == pItem->getType() )
		//		//	pNewItem = new Logic::CDel( (Logic::CDel*)pItem );
		//		//else if ( et_w_ins == pItem->getType() )
		//		//	pNewItem = new Logic::CIns( (Logic::CIns*)pItem );
		//		//else if ( et_w_moveFrom == pItem->getType() )
		//		//	pNewItem = new Logic::CMoveFrom( (Logic::CMoveFrom*)pItem );
		//		else if ( et_w_moveFromRangeEnd == pItem->getType() )
		//			pNewItem = new Logic::CMoveToRangeEnd( (Logic::CMoveToRangeEnd*)pItem );
		//		else if ( et_w_moveFromRangeStart == pItem->getType() )
		//			pNewItem = new Logic::CMoveToRangeStart( (Logic::CMoveToRangeStart*)pItem );
		//		//else if ( et_w_moveTo == pItem->getType() )
		//		//	pNewItem = new Logic::CMoveTo( (Logic::CMoveTo*)pItem );
		//		else if ( et_w_moveToRangeEnd == pItem->getType() )
		//			pNewItem = new Logic::CMoveToRangeEnd( (Logic::CMoveToRangeEnd*)pItem );
		//		else if ( et_w_moveToRangeStart == pItem->getType() )
		//			pNewItem = new Logic::CMoveToRangeStart( (Logic::CMoveToRangeStart*)pItem );
		//		//else if ( et_w_oMath == pItem->getType() )
		//		//	pNewItem = new Logic::COMath( (Logic::COMath*)pItem );
		//		//else if ( et_w_oMathPara == pItem->getType() )
		//		//	pNewItem = new Logic::COMathPara( (Logic::COMathPara*)pItem );
		//		else if ( et_w_p == pItem->getType() )
		//			pNewItem = new Logic::CParagraph( (Logic::CParagraph*)pItem );
		//		else if ( et_w_permEnd == pItem->getType() )
		//			pNewItem = new Logic::CPermEnd( (Logic::CPermEnd*)pItem );
		//		else if ( et_w_permStart == pItem->getType() )
		//			pNewItem = new Logic::CPermStart( (Logic::CPermStart*)pItem );
		//		else if ( et_w_proofErr == pItem->getType() )
		//			pNewItem = new Logic::CProofErr( (Logic::CProofErr*)pItem );
		//		else if ( et_w_sdt == pItem->getType() )
		//			pNewItem = new Logic::CSdt( (Logic::CSdt*)pItem );
		//		else if ( et_w_tbl == pItem->getType() )
		//			pNewItem = new Logic::CTbl( (Logic::CTbl*)pItem );

		//		if ( pItem )
		//			m_arrItems.Add( pItem );
		//	}
		//	return *this;
		//}
		void Clear()
		{
			m_eType = et_Unknown;

			m_oId.reset();
			m_oType.reset();

			for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )
					delete m_arrItems[nIndex];

				m_arrItems[nIndex] = NULL;
			}

			m_arrItems.RemoveAll();
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
							m_arrItems.Add( pItem );
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

			for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
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
			m_arrItems.Add( (WritingElement*)pPara );
		}

	public:

		OOX::EElementType                        m_eType; // <w:footnote> или <w:endnote>

		// Attributes
		nullable<SimpleTypes::CDecimalNumber<> > m_oId;
		nullable<SimpleTypes::CFtnEdn<>        > m_oType;

		// Childs
		CSimpleArray<WritingElement*           > m_arrItems;
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