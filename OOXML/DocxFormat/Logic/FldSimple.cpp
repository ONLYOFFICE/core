/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "SectionProperty.h"
#include "Sdt.h"
#include "Hyperlink.h"
#include "SmartTag.h"
#include "Dir.h"
#include "FldChar.h"
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
            XmlMacroReadAttributeBase( oNode, L"w:dirty",   m_oDirty );
            XmlMacroReadAttributeBase( oNode, L"w:fldLock", m_oFldLock );
            XmlMacroReadAttributeBase( oNode, L"w:instr",   m_sInstr );

			std::vector<XmlUtils::CXmlNode> oChilds;
			if ( oNode.GetNodes( L"*", oChilds ) )
			{
				for ( size_t nIndex = 0; nIndex < oChilds.size(); nIndex++ )
				{
					XmlUtils::CXmlNode& oItem = oChilds[nIndex];
					if (oItem.IsValid())
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( L"w:bdo" == sName )
							AssignPtrXmlContent(pItem, CBdo, oItem)
						else if ( L"w:bookmarkEnd" == sName )
							AssignPtrXmlContent(pItem, CBookmarkEnd, oItem)
						else if ( L"w:bookmarkStart" == sName )
							AssignPtrXmlContent(pItem, CBookmarkStart, oItem)
						else if ( L"w:commentRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CCommentRangeEnd, oItem)
						else if ( L"w:commentRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCommentRangeStart, oItem)
						//else if ( L"w:customXml" == sName )
						//	pItem = new CCustomXml( oItem );
						else if ( L"w:customXmlDelRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlDelRangeEnd, oItem)
						else if ( L"w:customXmlDelRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlDelRangeStart, oItem)
						else if ( L"w:customXmlInsRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlInsRangeEnd, oItem)
						else if ( L"w:customXmlInsRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlInsRangeStart, oItem)
						else if ( L"w:customXmlMoveFromRangeEnd" == sName ) 
							AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeEnd, oItem)
						else if ( L"w:customXmlMoveFromRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeStart, oItem)
						else if ( L"w:customXmlMoveToRangeEnd" == sName ) 
							AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeEnd, oItem)
						else if ( L"w:customXmlMoveToRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeStart, oItem)
						//else if ( L"w:del" == sName )
						//	pItem = new CDel( oItem );
						else if ( L"w:dir" == sName )
							AssignPtrXmlContent(pItem, CDir, oItem)
						else if ( L"w:fldSimple" == sName )
							AssignPtrXmlContent(pItem, CFldSimple, oItem)
						else if ( L"w:hyperlink" == sName )
							AssignPtrXmlContent(pItem, CHyperlink, oItem)
						//else if ( L"w:ins" == sName )
						//	pItem = new CIns( oItem );
						//else if ( L"w:moveFrom" == sName )
						//	pItem = new CMoveFrom( oItem );
						else if ( L"w:moveFromRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CMoveFromRangeEnd, oItem)
						else if ( L"w:moveFromRangeStart" == sName )
							AssignPtrXmlContent(pItem, CMoveFromRangeStart, oItem)
						//else if ( L"w:moveTo" == sName )
						//	pItem = new CMoveTo( oItem );
						else if ( L"w:moveToRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CMoveToRangeEnd, oItem)
						else if ( L"w:moveToRangeStart" == sName )
							AssignPtrXmlContent(pItem, CMoveToRangeStart, oItem)
						else if ( L"m:oMath" == sName )
							AssignPtrXmlContent(pItem, COMath, oItem)
						else if ( L"m:oMathPara" == sName )
							AssignPtrXmlContent(pItem, COMathPara, oItem)
						else if ( L"w:permEnd" == sName )
							AssignPtrXmlContent(pItem, CPermEnd, oItem)
						else if ( L"w:permStart" == sName )
							AssignPtrXmlContent(pItem, CPermStart, oItem)
						else if ( L"w:proofErr" == sName )
							AssignPtrXmlContent(pItem, CProofErr, oItem)
						else if ( L"w:r" == sName )
							AssignPtrXmlContent(pItem, CRun, oItem)
						else if ( L"w:sdt" == sName )
							AssignPtrXmlContent(pItem, CSdt, oItem)
						else if ( L"w:smartTag" == sName )
							AssignPtrXmlContent(pItem, CSmartTag, oItem)
						//else if ( L"w:subDoc" == sName )
						//	pItem = new CSubDoc( oItem );
						else if ( L"w:ffData" == sName )
						{
							m_oFFData = oItem;
						}
						else if (L"w:fldData" == sName)
						{
							m_sPrivateData = oItem.GetText();
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

			OOX::Document* document = WritingElement::m_pMainDocument;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( L"w:bdo" == sName )
					AssignPtrXmlContent(pItem, CBdo, oReader)
				else if ( L"w:bookmarkEnd" == sName )
					AssignPtrXmlContent(pItem, CBookmarkEnd, oReader)
				else if ( L"w:bookmarkStart" == sName )
					AssignPtrXmlContent(pItem, CBookmarkStart, oReader)
				else if ( L"w:commentRangeEnd" == sName )
					AssignPtrXmlContent(pItem, CCommentRangeEnd, oReader)
				else if ( L"w:commentRangeStart" == sName )
					AssignPtrXmlContent(pItem, CCommentRangeStart, oReader)
				//else if ( L"w:customXml" == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( L"w:customXmlDelRangeEnd" == sName )
					AssignPtrXmlContent(pItem, CCustomXmlDelRangeEnd, oReader)
				else if ( L"w:customXmlDelRangeStart" == sName )
					AssignPtrXmlContent(pItem, CCustomXmlDelRangeStart, oReader)
				else if ( L"w:customXmlInsRangeEnd" == sName )
					AssignPtrXmlContent(pItem, CCustomXmlInsRangeEnd, oReader)
				else if ( L"w:customXmlInsRangeStart" == sName )
					AssignPtrXmlContent(pItem, CCustomXmlInsRangeStart, oReader)
				else if ( L"w:customXmlMoveFromRangeEnd" == sName ) 
					AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeEnd, oReader)
				else if ( L"w:customXmlMoveFromRangeStart" == sName )
					AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeStart, oReader)
				else if ( L"w:customXmlMoveToRangeEnd" == sName ) 
					AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeEnd, oReader)
				else if ( L"w:customXmlMoveToRangeStart" == sName )
					AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeStart, oReader)
				else if ( L"w:del" == sName )
					AssignPtrXmlContent(pItem, CDel, oReader)
				else if ( L"w:dir" == sName )
					AssignPtrXmlContent(pItem, CDir, oReader)
				else if ( L"w:fldSimple" == sName )
					AssignPtrXmlContent(pItem, CFldSimple, oReader)
				else if ( L"w:hyperlink" == sName )
					AssignPtrXmlContent(pItem, CHyperlink, oReader)
				else if ( L"w:ins" == sName )
					AssignPtrXmlContent(pItem, CIns, oReader)
				else if ( L"w:moveFrom" == sName )
					AssignPtrXmlContent(pItem, CMoveFrom, oReader)
				else if ( L"w:moveFromRangeEnd" == sName )
					AssignPtrXmlContent(pItem, CMoveFromRangeEnd, oReader)
				else if ( L"w:moveFromRangeStart" == sName )
					AssignPtrXmlContent(pItem, CMoveFromRangeStart, oReader)
				else if ( L"w:moveTo" == sName )
					AssignPtrXmlContent(pItem, CMoveTo, oReader)
				else if ( L"w:moveToRangeEnd" == sName )
					AssignPtrXmlContent(pItem, CMoveToRangeEnd, oReader)
				else if ( L"w:moveToRangeStart" == sName )
					AssignPtrXmlContent(pItem, CMoveToRangeStart, oReader)
				else if ( L"m:oMath" == sName )
					AssignPtrXmlContent(pItem, COMath, oReader)
				else if ( L"m:oMathPara" == sName )
					AssignPtrXmlContent(pItem, COMathPara, oReader)
				else if ( L"w:permEnd" == sName )
					AssignPtrXmlContent(pItem, CPermEnd, oReader)
				else if ( L"w:permStart" == sName )
					AssignPtrXmlContent(pItem, CPermStart, oReader)
				else if ( L"w:proofErr" == sName )
					AssignPtrXmlContent(pItem, CProofErr, oReader)
				else if ( L"w:r" == sName )
					AssignPtrXmlContent(pItem, CRun, oReader)
				else if ( L"w:sdt" == sName )
					AssignPtrXmlContent(pItem, CSdt, oReader)
				else if ( L"w:smartTag" == sName )
					AssignPtrXmlContent(pItem, CSmartTag, oReader)
				//else if ( L"w:subDoc" == sName )
				//	pItem = new CSubDoc( oReader );
				else if (L"w:ffData" == sName)
				{
					m_oFFData = oReader;
				}
				else if (L"w:fldData" == sName)
				{
					m_sPrivateData = oReader.GetText2();
				}

				if (pItem)
				{
					m_arrItems.push_back(pItem);
				}
			}
		}
		std::wstring CFldSimple::toXML() const
		{
			std::wstring sResult = L"<w:fldSimple ";

			sResult += L"w:dirty=\"";
			sResult += m_oDirty.ToString();
			sResult += L"\" ";
			sResult += L"w:fldLock=\"";
			sResult += m_oFldLock.ToString();
			sResult += L"\" ";

			if ( m_sInstr.IsInit() )
			{
				sResult += L"w:instr=\"" + *m_sInstr + L"\" ";
			}

			sResult += L">";

			if (m_oFFData.IsInit())
			{
				sResult += m_oFFData->toXML();
			}
			if (m_sPrivateData.IsInit())
			{
				sResult += L"<w:fldData xml:space=\"preserve\">" + *m_sPrivateData + L"</w:fldData>";
			}
            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += L"</w:fldSimple>";

			return sResult;
		}
		EElementType CFldSimple::getType() const
		{
			return et_w_fldSimple;
		}
		void CFldSimple::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:dirty",   m_oDirty )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:fldLock", m_oFldLock )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:instr",   m_sInstr )
			WritingElement_ReadAttributes_End( oReader )
		}

	} // namespace Logic
} // namespace OOX
