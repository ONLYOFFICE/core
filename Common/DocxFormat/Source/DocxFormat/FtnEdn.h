/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
#include "Math/OMath.h"

// TO DO: Нехватающие классы:
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
		WritingElement_AdditionConstructors(CFtnEdn)
		CFtnEdn()
		{
			m_eType = et_Unknown;
		}
		virtual ~CFtnEdn()
		{
		}
		virtual void ClearItems()
		{
			m_eType = et_Unknown;

			m_oId.reset();
			m_oType.reset();

			WritingElementWithChilds::ClearItems();
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:footnote") == oNode.GetName() )
				m_eType = et_w_footnote;
			else if ( _T("w:endnote") == oNode.GetName() )
				m_eType = et_w_endnote;
			else 
				return;

			XmlMacroReadAttributeBase( oNode, _T("w:id"),   m_oId   );
			XmlMacroReadAttributeBase( oNode, _T("w:type"), m_oType );

			XmlUtils::CXmlNodes oChilds;
			if ( oNode.GetNodes( _T("*"), oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( _T("w:altChunk") == sName )
							pItem = new Logic::CAltChunk( oItem );
						else if ( _T("w:bookmarkEnd") == sName )
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
							pItem = new Logic::CMoveFromRangeEnd( oItem );
						else if ( _T("w:moveFromRangeStart") == sName )
							pItem = new Logic::CMoveFromRangeStart( oItem );
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
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring sName = oReader.GetName();
			if ( _T("w:footnote") == sName )
				m_eType = et_w_footnote;
			else if ( _T("w:endnote") == sName )
				m_eType = et_w_endnote;
			else
				return;

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:altChunk") == sName )
					pItem = new Logic::CAltChunk( oReader );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new Logic::CBookmarkEnd( oReader );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new Logic::CBookmarkStart( oReader );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new Logic::CCommentRangeEnd( oReader );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new Logic::CCommentRangeStart( oReader );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new Logic::CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new Logic::CCustomXmlDelRangeEnd( oReader );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new Logic::CCustomXmlDelRangeStart( oReader );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new Logic::CCustomXmlInsRangeEnd( oReader );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new Logic::CCustomXmlInsRangeStart( oReader );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName )
					pItem = new Logic::CCustomXmlMoveFromRangeEnd( oReader );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new Logic::CCustomXmlMoveFromRangeStart( oReader );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName )
					pItem = new Logic::CCustomXmlMoveToRangeEnd( oReader );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new Logic::CCustomXmlMoveToRangeStart( oReader );
				else if ( _T("w:del") == sName )
					pItem = new Logic::CDel( oReader );
				else if ( _T("w:ins") == sName )
					pItem = new Logic::CIns( oReader );
				else if ( _T("w:moveFrom") == sName )
					pItem = new Logic::CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new Logic::CMoveFromRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new Logic::CMoveFromRangeStart( oReader );
				else if ( _T("w:moveTo") == sName )
					pItem = new Logic::CMoveTo( oReader );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new Logic::CMoveToRangeEnd( oReader );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new Logic::CMoveToRangeStart( oReader );
				else if ( _T("m:oMath") == sName )
					pItem = new Logic::COMath( oReader );
				else if ( _T("m:oMathPara") == sName )
					pItem = new Logic::COMathPara( oReader );
				else if ( _T("w:p") == sName )
					pItem = new Logic::CParagraph( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new Logic::CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new Logic::CPermStart( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new Logic::CProofErr( oReader );
				else if ( _T("w:sdt") == sName )
					pItem = new Logic::CSdt( oReader );
				else if ( _T("w:tbl") == sName )
					pItem = new Logic::CTbl( oReader );

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if( oReader, _T("w:id"), m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:type"), m_oType )
			WritingElement_ReadAttributes_End( oReader )
		}
        virtual std::wstring toXML() const
		{
			std::wstring sResult;
			
			if ( m_eType == et_w_footnote )
				sResult = _T("<w:footnote ");
			else if ( m_eType == et_w_endnote )
				sResult = _T("<w:endnote ");
			else
				return _T("");

			ComplexTypes_WriteAttribute( _T("w:id=\""),   m_oId );
			ComplexTypes_WriteAttribute( _T("w:type=\""), m_oType );

			sResult += _T(">");

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if (  m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
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
		void AddParagraph(Logic::CParagraph *pPara)
		{
			m_arrItems.push_back( (WritingElement*)pPara );
		}
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

			XmlMacroReadAttributeBase( oNode, _T("w:id"),   m_oId   );
		}
		virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring sName = oReader.GetName();
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
        virtual std::wstring      toXML() const
		{
			std::wstring sResult;
			
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

			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
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
