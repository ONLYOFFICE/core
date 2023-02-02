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
		CFldSimple::CFldSimple(XmlUtils::CXmlNode &oNode) : WritingElementWithChilds<>(NULL)
		{
			fromXML( oNode );
		}
		CFldSimple::CFldSimple(XmlUtils::CXmlLiteReader& oReader) : WritingElementWithChilds<>(NULL)
		{
			fromXML( oReader );
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
							pItem = new CBdo( oItem );
						else if ( _T("w:bookmarkEnd") == sName )
							pItem = new CBookmarkEnd( oItem );
						else if ( _T("w:bookmarkStart") == sName )
							pItem = new CBookmarkStart( oItem );
						else if ( _T("w:commentRangeEnd") == sName )
							pItem = new CCommentRangeEnd( oItem );
						else if ( _T("w:commentRangeStart") == sName )
							pItem = new CCommentRangeStart( oItem );
						//else if ( _T("w:customXml") == sName )
						//	pItem = new CCustomXml( oItem );
						else if ( _T("w:customXmlDelRangeEnd") == sName )
							pItem = new CCustomXmlDelRangeEnd( oItem );
						else if ( _T("w:customXmlDelRangeStart") == sName )
							pItem = new CCustomXmlDelRangeStart( oItem );
						else if ( _T("w:customXmlInsRangeEnd") == sName )
							pItem = new CCustomXmlInsRangeEnd( oItem );
						else if ( _T("w:customXmlInsRangeStart") == sName )
							pItem = new CCustomXmlInsRangeStart( oItem );
						else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveFromRangeEnd( oItem );
						else if ( _T("w:customXmlMoveFromRangeStart") == sName )
							pItem = new CCustomXmlMoveFromRangeStart( oItem );
						else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveToRangeEnd( oItem );
						else if ( _T("w:customXmlMoveToRangeStart") == sName )
							pItem = new CCustomXmlMoveToRangeStart( oItem );
						//else if ( _T("w:del") == sName )
						//	pItem = new CDel( oItem );
						else if ( _T("w:dir") == sName )
							pItem = new CDir( oItem );
						else if ( _T("w:fldSimple") == sName )
							pItem = new CFldSimple( oItem );
						else if ( _T("w:hyperlink") == sName )
							pItem = new CHyperlink( oItem );
						//else if ( _T("w:ins") == sName )
						//	pItem = new CIns( oItem );
						//else if ( _T("w:moveFrom") == sName )
						//	pItem = new CMoveFrom( oItem );
						else if ( _T("w:moveFromRangeEnd") == sName )
							pItem = new CMoveFromRangeEnd( oItem );
						else if ( _T("w:moveFromRangeStart") == sName )
							pItem = new CMoveFromRangeStart( oItem );
						//else if ( _T("w:moveTo") == sName )
						//	pItem = new CMoveTo( oItem );
						else if ( _T("w:moveToRangeEnd") == sName )
							pItem = new CMoveToRangeEnd( oItem );
						else if ( _T("w:moveToRangeStart") == sName )
							pItem = new CMoveToRangeStart( oItem );
						else if ( _T("m:oMath") == sName )
							pItem = new COMath( oItem );
						else if ( _T("m:oMathPara") == sName )
							pItem = new COMathPara( oItem );
						else if ( _T("w:permEnd") == sName )
							pItem = new CPermEnd( oItem );
						else if ( _T("w:permStart") == sName )
							pItem = new CPermStart( oItem );
						else if ( _T("w:proofErr") == sName )
							pItem = new CProofErr( oItem );
						else if ( _T("w:r") == sName )
							pItem = new CRun( oItem );
						else if ( _T("w:sdt") == sName )
							pItem = new CSdt( oItem );
						else if ( _T("w:smartTag") == sName )
							pItem = new CSmartTag( oItem );
						//else if ( _T("w:subDoc") == sName )
						//	pItem = new CSubDoc( oItem );
						else if ( _T("w:ffData") == sName )
							m_oFFData = new CFFData( oItem );

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

			OOX::Document* document = WritingElement::m_pMainDocument;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:bdo") == sName )
					pItem = new CBdo(document);
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new CBookmarkEnd(document);
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new CBookmarkStart(document);
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new CCommentRangeEnd(document);
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new CCommentRangeStart(document);
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new CCustomXmlDelRangeEnd(document);
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new CCustomXmlDelRangeStart(document);
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new CCustomXmlInsRangeEnd(document);
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new CCustomXmlInsRangeStart(document);
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveFromRangeEnd(document);
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new CCustomXmlMoveFromRangeStart(document);
				else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveToRangeEnd(document);
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new CCustomXmlMoveToRangeStart(document);
				else if ( _T("w:del") == sName )
					pItem = new CDel(document);
				else if ( _T("w:dir") == sName )
					pItem = new CDir(document);
				else if ( _T("w:fldSimple") == sName )
					pItem = new CFldSimple(document);
				else if ( _T("w:hyperlink") == sName )
					pItem = new CHyperlink(document);
				else if ( _T("w:ins") == sName )
					pItem = new CIns(document);
				else if ( _T("w:moveFrom") == sName )
					pItem = new CMoveFrom(document);
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveFromRangeEnd(document);
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveFromRangeStart(document);
				else if ( _T("w:moveTo") == sName )
					pItem = new CMoveTo(document);
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new CMoveToRangeEnd(document);
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new CMoveToRangeStart(document);
				else if ( _T("m:oMath") == sName )
					pItem = new COMath(document);
				else if ( _T("m:oMathPara") == sName )
					pItem = new COMathPara(document);
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd(document);
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart(document);
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr(document);
				else if ( _T("w:r") == sName )
					pItem = new CRun(document, this);
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt(document);
				else if ( _T("w:smartTag") == sName )
					pItem = new CSmartTag(document);
				//else if ( _T("w:subDoc") == sName )
				//	pItem = new CSubDoc( oReader );
				else if ( _T("w:ffData") == sName )
					m_oFFData = new CFFData();

				if (pItem)
				{
					pItem->fromXML(oReader);
					m_arrItems.push_back(pItem);
				}
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
