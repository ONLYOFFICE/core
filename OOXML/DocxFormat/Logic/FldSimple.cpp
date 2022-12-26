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
#include "Bdo.h"
#include "FldSimple.h"
#include "Annotations.h"
#include "Run.h"
#include "RunProperty.h"
#include "ParagraphProperty.h"
#include "Sdt.h"
#include "Hyperlink.h"
#include "SmartTag.h"
#include "Dir.h"
#include "../Math/oMathPara.h"
#include "../Math/OMath.h"

// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:moveFrom>
//        <w:moveTo>
//        <w:subDoc>


namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CFldSimple 17.16.19 (Part 1)
		//--------------------------------------------------------------------------------	

		CFldSimple::CFldSimple(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}		
		CFldSimple::~CFldSimple()
		{
		}
		const CFldSimple& CFldSimple::operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CFldSimple& CFldSimple::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CFldSimple::ClearItems()
		{
			m_oDirty.SetValue( SimpleTypes::onoffFalse );
			m_oFldLock.SetValue( SimpleTypes::onoffFalse );
			m_sInstr.reset();

			WritingElementWithChilds::ClearItems();
		}
		void CFldSimple::fromXML(XmlUtils::CXmlNode& oNode)
		{
            XmlMacroReadAttributeBase( oNode, _T("w:dirty"),   m_oDirty );
            XmlMacroReadAttributeBase( oNode, _T("w:fldLock"), m_oFldLock );
            XmlMacroReadAttributeBase( oNode, _T("w:instr"),   m_sInstr );

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

						if ( _T("w:bdo") == sName )
							AssignPtrXmlContent(pItem, CBdo, oItem)
						else if ( _T("w:bookmarkEnd") == sName )
							AssignPtrXmlContent(pItem, CBookmarkEnd, oItem)
						else if ( _T("w:bookmarkStart") == sName )
							AssignPtrXmlContent(pItem, CBookmarkStart, oItem)
						else if ( _T("w:commentRangeEnd") == sName )
							AssignPtrXmlContent(pItem, CCommentRangeEnd, oItem)
						else if ( _T("w:commentRangeStart") == sName )
							AssignPtrXmlContent(pItem, CCommentRangeStart, oItem)
						//else if ( _T("w:customXml") == sName )
						//	pItem = new CCustomXml( oItem );
						else if ( _T("w:customXmlDelRangeEnd") == sName )
							AssignPtrXmlContent(pItem, CCustomXmlDelRangeEnd, oItem)
						else if ( _T("w:customXmlDelRangeStart") == sName )
							AssignPtrXmlContent(pItem, CCustomXmlDelRangeStart, oItem)
						else if ( _T("w:customXmlInsRangeEnd") == sName )
							AssignPtrXmlContent(pItem, CCustomXmlInsRangeEnd, oItem)
						else if ( _T("w:customXmlInsRangeStart") == sName )
							AssignPtrXmlContent(pItem, CCustomXmlInsRangeStart, oItem)
						else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
							AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeEnd, oItem)
						else if ( _T("w:customXmlMoveFromRangeStart") == sName )
							AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeStart, oItem)
						else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
							AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeEnd, oItem)
						else if ( _T("w:customXmlMoveToRangeStart") == sName )
							AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeStart, oItem)
						//else if ( _T("w:del") == sName )
						//	pItem = new CDel( oItem );
						else if ( _T("w:dir") == sName )
							AssignPtrXmlContent(pItem, CDir, oItem)
						else if ( _T("w:fldSimple") == sName )
							AssignPtrXmlContent(pItem, CFldSimple, oItem)
						else if ( _T("w:hyperlink") == sName )
							AssignPtrXmlContent(pItem, CHyperlink, oItem)
						//else if ( _T("w:ins") == sName )
						//	pItem = new CIns( oItem );
						//else if ( _T("w:moveFrom") == sName )
						//	pItem = new CMoveFrom( oItem );
						else if ( _T("w:moveFromRangeEnd") == sName )
							AssignPtrXmlContent(pItem, CMoveFromRangeEnd, oItem)
						else if ( _T("w:moveFromRangeStart") == sName )
							AssignPtrXmlContent(pItem, CMoveFromRangeStart, oItem)
						//else if ( _T("w:moveTo") == sName )
						//	pItem = new CMoveTo( oItem );
						else if ( _T("w:moveToRangeEnd") == sName )
							AssignPtrXmlContent(pItem, CMoveToRangeEnd, oItem)
						else if ( _T("w:moveToRangeStart") == sName )
							AssignPtrXmlContent(pItem, CMoveToRangeStart, oItem)
						else if ( _T("m:oMath") == sName )
							AssignPtrXmlContent(pItem, COMath, oItem)
						else if ( _T("m:oMathPara") == sName )
							AssignPtrXmlContent(pItem, COMathPara, oItem)
						else if ( _T("w:permEnd") == sName )
							AssignPtrXmlContent(pItem, CPermEnd, oItem)
						else if ( _T("w:permStart") == sName )
							AssignPtrXmlContent(pItem, CPermStart, oItem)
						else if ( _T("w:proofErr") == sName )
							AssignPtrXmlContent(pItem, CProofErr, oItem)
						else if ( _T("w:r") == sName )
							AssignPtrXmlContent(pItem, CRun, oItem)
						else if ( _T("w:sdt") == sName )
							AssignPtrXmlContent(pItem, CSdt, oItem)
						else if ( _T("w:smartTag") == sName )
							AssignPtrXmlContent(pItem, CSmartTag, oItem)
						//else if ( _T("w:subDoc") == sName )
						//	pItem = new CSubDoc( oItem );
						else if ( _T("w:ffData") == sName )
						{
							m_oFFData = new CFFData();
							m_oFFData = oItem;
						}

						if ( pItem )
							m_arrItems.push_back( pItem );
					}
				}
			}
		}
		void CFldSimple::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:bdo") == sName )
					AssignPtrXmlContent(pItem, CBdo, oReader)
				else if ( _T("w:bookmarkEnd") == sName )
					AssignPtrXmlContent(pItem, CBookmarkEnd, oReader)
				else if ( _T("w:bookmarkStart") == sName )
					AssignPtrXmlContent(pItem, CBookmarkStart, oReader)
				else if ( _T("w:commentRangeEnd") == sName )
					AssignPtrXmlContent(pItem, CCommentRangeEnd, oReader)
				else if ( _T("w:commentRangeStart") == sName )
					AssignPtrXmlContent(pItem, CCommentRangeStart, oReader)
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					AssignPtrXmlContent(pItem, CCustomXmlDelRangeEnd, oReader)
				else if ( _T("w:customXmlDelRangeStart") == sName )
					AssignPtrXmlContent(pItem, CCustomXmlDelRangeStart, oReader)
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					AssignPtrXmlContent(pItem, CCustomXmlInsRangeEnd, oReader)
				else if ( _T("w:customXmlInsRangeStart") == sName )
					AssignPtrXmlContent(pItem, CCustomXmlInsRangeStart, oReader)
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
					AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeEnd, oReader)
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeStart, oReader)
				else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
					AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeEnd, oReader)
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeStart, oReader)
				else if ( _T("w:del") == sName )
					AssignPtrXmlContent(pItem, CDel, oReader)
				else if ( _T("w:dir") == sName )
					AssignPtrXmlContent(pItem, CDir, oReader)
				else if ( _T("w:fldSimple") == sName )
					AssignPtrXmlContent(pItem, CFldSimple, oReader)
				else if ( _T("w:hyperlink") == sName )
					AssignPtrXmlContent(pItem, CHyperlink, oReader)
				else if ( _T("w:ins") == sName )
					AssignPtrXmlContent(pItem, CIns, oReader)
				else if ( _T("w:moveFrom") == sName )
					AssignPtrXmlContent(pItem, CMoveFrom, oReader)
				else if ( _T("w:moveFromRangeEnd") == sName )
					AssignPtrXmlContent(pItem, CMoveFromRangeEnd, oReader)
				else if ( _T("w:moveFromRangeStart") == sName )
					AssignPtrXmlContent(pItem, CMoveFromRangeStart, oReader)
				else if ( _T("w:moveTo") == sName )
					AssignPtrXmlContent(pItem, CMoveTo, oReader)
				else if ( _T("w:moveToRangeEnd") == sName )
					AssignPtrXmlContent(pItem, CMoveToRangeEnd, oReader)
				else if ( _T("w:moveToRangeStart") == sName )
					AssignPtrXmlContent(pItem, CMoveToRangeStart, oReader)
				else if ( _T("m:oMath") == sName )
					AssignPtrXmlContent(pItem, COMath, oReader)
				else if ( _T("m:oMathPara") == sName )
					AssignPtrXmlContent(pItem, COMathPara, oReader)
				else if ( _T("w:permEnd") == sName )
					AssignPtrXmlContent(pItem, CPermEnd, oReader)
				else if ( _T("w:permStart") == sName )
					AssignPtrXmlContent(pItem, CPermStart, oReader)
				else if ( _T("w:proofErr") == sName )
					AssignPtrXmlContent(pItem, CProofErr, oReader)
				else if ( _T("w:r") == sName )
					AssignPtrXmlContent(pItem, CRun, oReader)
				else if ( _T("w:sdt") == sName )
					AssignPtrXmlContent(pItem, CSdt, oReader)
				else if ( _T("w:smartTag") == sName )
					AssignPtrXmlContent(pItem, CSmartTag, oReader)
				//else if ( _T("w:subDoc") == sName )
				//	pItem = new CSubDoc( oReader );
				else if ( _T("w:ffData") == sName )
				{
					m_oFFData = new CFFData();
					m_oFFData = oReader;
				}

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}
		std::wstring CFldSimple::toXML() const
		{
			std::wstring sResult = _T("<w:fldSimple ");

			sResult += _T("w:dirty=\"");
			sResult += m_oDirty.ToString();
			sResult += _T("\" ");
			sResult += _T("w:fldLock=\"");
			sResult += m_oFldLock.ToString();
			sResult += _T("\" ");

			if ( m_sInstr.IsInit() )
			{
				sResult += _T("w:instr=\"");
                sResult += m_sInstr.get2();
				sResult += _T("\" ");
			}

			sResult += _T(">");

			if (m_oFFData.IsInit())
			{
				sResult += m_oFFData->toXML();
			}

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += _T("</w:fldSimple>");

			return sResult;
		}
		EElementType CFldSimple::getType() const
		{
			return et_w_fldSimple;
		}
		void CFldSimple::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:dirty"),   m_oDirty )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:fldLock"), m_oFldLock )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:instr"),   m_sInstr )
			WritingElement_ReadAttributes_End( oReader )
		}

	} // namespace Logic
} // namespace OOX
