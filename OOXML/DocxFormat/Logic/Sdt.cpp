﻿/*
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
#include "Sdt.h"
#include "Paragraph.h"
#include "Annotations.h"
#include "Run.h"
#include "RunProperty.h"
#include "ParagraphProperty.h"
#include "SectionProperty.h"
#include "FldSimple.h"
#include "Bdo.h"
#include "Table.h"
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


namespace ComplexTypes
{
	namespace Word
	{
		CComb::CComb()
		{
		}
		CComb::~CComb()
		{
		}
		void CComb::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:width", m_oWidth );
			XmlMacroReadAttributeBase( oNode, L"w:sym", m_oSym );
			XmlMacroReadAttributeBase( oNode, L"w:font", m_oFont );
			XmlMacroReadAttributeBase(oNode, L"w:wRule", m_oWRule);
		}
		void CComb::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CComb::ToString() const
		{
			std::wstring sResult;
			if (m_oWidth.IsInit())
			{
				sResult += L"w:width=\"" + std::to_wstring(m_oWidth.get()) + L"\" ";
			}
			if (m_oSym.IsInit())
			{
				sResult += L"w:sym=\"" + XmlUtils::EncodeXmlString(m_oSym.get()) + L"\" ";
			}
			if (m_oFont.IsInit())
			{
				sResult += L"w:font=\"" + XmlUtils::EncodeXmlString(m_oFont.get()) + L"\" ";
			}
			if (m_oWRule.IsInit())
			{
				sResult += L"w:wRule=\"" + m_oWRule->ToString() + L"\" ";
			}
			return sResult;
		}
		void CComb::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, L"w:width", m_oWidth )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:sym", m_oSym )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:font", m_oFont )
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:wRule", m_oWRule )
			WritingElement_ReadAttributes_End( oReader )
		}
//----------------------------------------------------------------------------------------------------
		CTextFormFormat::CTextFormFormat()
		{
		}
		CTextFormFormat::~CTextFormFormat()
		{
		}
		void CTextFormFormat::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"w:type", m_oType);
			XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			XmlMacroReadAttributeBase(oNode, L"w:symbols", m_oSymbols);
		}
		void CTextFormFormat::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		std::wstring CTextFormFormat::ToString() const
		{
			std::wstring sResult;
			if (m_oType.IsInit())
			{
				sResult += L"w:type=\"" + m_oType->ToString() + L"\" ";
			}
			if (m_oVal.IsInit())
			{
				sResult += L"w:val=\"" + XmlUtils::EncodeXmlString(m_oVal.get()) + L"\" ";
			}
			if (m_oSymbols.IsInit())
			{
				sResult += L"w:symbols=\"" + XmlUtils::EncodeXmlString(m_oSymbols.get()) + L"\" ";
			}
			return sResult;
		}
		void CTextFormFormat::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"w:type", m_oType)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:symbols", m_oSymbols)
			WritingElement_ReadAttributes_End(oReader)
		}
//----------------------------------------------------------------------------------------------------
		CSdtAppearance::CSdtAppearance()
		{
		}
		CSdtAppearance::~CSdtAppearance()
		{
		}
		void CSdtAppearance::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w15:val", m_oVal );
		}
		void CSdtAppearance::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CSdtAppearance::ToString() const
		{
			std::wstring sResult;
			if(m_oVal.IsInit())
			{
				sResult = L"w15:val=\"" + m_oVal->ToString() + L"\"";
			}

			return sResult;
		}
		void CSdtAppearance::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w15:val", m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}
//----------------------------------------------------------------------------------------------------
		CSdtText::CSdtText()
		{
		}
		CSdtText::~CSdtText()
		{
		}
		void CSdtText::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:multiLine", m_oMultiLine );
		}
		void CSdtText::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CSdtText::ToString() const
		{
			std::wstring sResult;
			if(m_oMultiLine.IsInit())
			{
				sResult = L"w:multiLine=\"" + m_oMultiLine->ToString() + L"\"";
			}

			return sResult;
		}
		void CSdtText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:multiLine", m_oMultiLine )
			WritingElement_ReadAttributes_End( oReader )
		}
//----------------------------------------------------------------------------------------------------
		CLock::CLock()
		{
		}
		CLock::~CLock()
		{
		}
		void CLock::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
		}
		void CLock::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CLock::ToString() const
		{
			std::wstring sResult = L"w:val=\"" + m_oVal.ToString() + L"\"";

			return sResult;
		}
		void CLock::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}
//----------------------------------------------------------------------------------------------------
		CSdtDateMappingType::CSdtDateMappingType()
		{
		}
		CSdtDateMappingType::~CSdtDateMappingType()
		{
		}
		void CSdtDateMappingType::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
		}
		void CSdtDateMappingType::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CSdtDateMappingType::ToString() const
		{
			std::wstring sResult = L"w:val=\"" + m_oVal.ToString() + L"\"";
			return sResult;
		}
		void CSdtDateMappingType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}
//----------------------------------------------------------------------------------------------------
		CCalendarType::CCalendarType()
		{
		}
		CCalendarType::~CCalendarType()
		{
		}
		void CCalendarType::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
		}
		void CCalendarType::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCalendarType::ToString() const
		{
			std::wstring sResult = L"w:val=\"" + m_oVal.ToString() + L"\"";
			return sResult;
		}
		void CCalendarType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			m_oVal.SetValue(SimpleTypes::calendartypeGregorian);

			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}
//----------------------------------------------------------------------------------------------------
		CDataBinding::CDataBinding()
		{
		}
		CDataBinding::~CDataBinding()
		{
		}
		void CDataBinding::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:prefixMappings", m_sPrefixMappings );
			XmlMacroReadAttributeBase( oNode, L"w:storeItemID",    m_sStoreItemID );
			XmlMacroReadAttributeBase( oNode, L"w:xpath",          m_sXPath );
		}
		void CDataBinding::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CDataBinding::ToString() const
		{
			std::wstring sResult;

			if ( m_sPrefixMappings.IsInit() )
			{
				sResult += L" w:prefixMappings=\"" + *m_sPrefixMappings + L"\"";
			}
			if ( m_sXPath.IsInit() )
			{
				sResult += L" w:xpath=\"" + *m_sXPath + L"\"";
			}
			if ( m_sStoreItemID.IsInit() )
			{
				sResult += L" w:storeItemID=\"" + *m_sStoreItemID + L"\"";
			}
			if (m_sStoreItemChecksum.IsInit())
			{
				sResult += L" w16sdtdh:storeItemChecksum=\"" + *m_sStoreItemChecksum + L"\"";
			}
			return sResult;
		}
		void CDataBinding::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"prefixMappings", m_sPrefixMappings )
				WritingElement_ReadAttributes_Read_else_if (oReader, L"storeItemID", m_sStoreItemID )
				WritingElement_ReadAttributes_Read_else_if (oReader, L"xpath", m_sXPath )
				WritingElement_ReadAttributes_Read_else_if (oReader, L"storeItemChecksum", m_sStoreItemChecksum)
			WritingElement_ReadAttributes_End_No_NS(oReader )
		}
//----------------------------------------------------------------------------------------------------
		CSdtListItem::CSdtListItem()
		{
		}
		CSdtListItem::~CSdtListItem()
		{
		}
		void CSdtListItem::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:displayText", m_sDisplayText );
			XmlMacroReadAttributeBase( oNode, L"w:value",       m_sValue );
		}
		void CSdtListItem::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CSdtListItem::ToString() const
		{
			std::wstring sResult;

			if ( m_sDisplayText.IsInit() )
			{
				sResult += L"w:displayText=\"";
				sResult += XmlUtils::EncodeXmlString(*m_sDisplayText);
				sResult += L"\" ";
			}

			if ( m_sValue.IsInit() )
			{
				sResult += L"w:value=\"";
				sResult += XmlUtils::EncodeXmlString(*m_sValue);
				sResult += L"\" ";
			}

			return sResult;
		}
		void CSdtListItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:displayText", m_sDisplayText )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:value",       m_sValue )
			WritingElement_ReadAttributes_End( oReader )
		}
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
	}
}
//-----------------------------------------------------------------------------------------------------------------------------
namespace OOX
{
	namespace Logic
	{
//--------------------------------------------------------------------------------
// CSdtContent
//--------------------------------------------------------------------------------	
		CSdtContent::CSdtContent(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}
		void CSdtContent::fromXML(XmlUtils::CXmlNode& oNode)
		{
			ClearItems();

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
						else if ( L"w:p" == sName )
							AssignPtrXmlContent(pItem, CParagraph, oItem)
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
						else if ( L"w:tbl" == sName )
							AssignPtrXmlContent(pItem, CTbl, oItem)
						else if ( L"w:tc" == sName )
							AssignPtrXmlContent(pItem, CTc, oItem)
						else if ( L"w:tr" == sName )
							AssignPtrXmlContent(pItem, CTr, oItem)

						if ( pItem )
							m_arrItems.push_back( pItem );
					}
				}
			}
		}
		void CSdtContent::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			
			OOX::Document* document = WritingElement::m_pMainDocument;
			
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
				else if (L"w:p" == sName)
				{
					pItem = new CParagraph(document, this);
					pItem->fromXML(oReader); 
				}
				else if ( L"w:permEnd" == sName )
					AssignPtrXmlContent(pItem, CPermEnd, oReader)
				else if ( L"w:permStart" == sName )
					AssignPtrXmlContent(pItem, CPermStart, oReader)
				else if ( L"w:proofErr" == sName )
					AssignPtrXmlContent(pItem, CProofErr, oReader)
				else if ( L"w:r" == sName )
				{
					pItem = new CRun(document, this);
					pItem->fromXML(oReader);
				}
				else if ( L"w:sdt" == sName )
					AssignPtrXmlContent(pItem, CSdt, oReader)
				else if ( L"w:smartTag" == sName )
					AssignPtrXmlContent(pItem, CSmartTag, oReader)
				//else if ( L"w:subDoc" == sName )
				//	pItem = new CSubDoc( document );
				else if ( L"w:tbl" == sName )
					AssignPtrXmlContent(pItem, CTbl, oReader)
				else if ( L"w:tc" == sName )
					AssignPtrXmlContent(pItem, CTc, oReader)
				else if ( L"w:tr" == sName )
					AssignPtrXmlContent(pItem, CTr, oReader)

				if (pItem)
				{
					m_arrItems.push_back(pItem);
				}
			}
		}
		std::wstring CSdtContent::toXML() const
		{
			std::wstring sResult = L"<w:sdtContent>";

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += L"</w:sdtContent>";

			return sResult;
		}
		EElementType CSdtContent::getType() const
		{
			return et_w_sdtContent;
		}
//--------------------------------------------------------------------------------
		CFormPr::CFormPr(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CFormPr::~CFormPr()
		{
		}
		EElementType CFormPr::getType() const
		{
			return et_w_formPr;
		}
		void CFormPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			XmlMacroReadAttributeBase(oNode, L"w:key", m_oKey);
			XmlMacroReadAttributeBase(oNode, L"w:label", m_oLabel);
			XmlMacroReadAttributeBase(oNode, L"w:helpText", m_oHelpText);
			XmlMacroReadAttributeBase(oNode, L"w:required", m_oRequired);
			
			WritingElement_ReadNode(oNode, oChild, L"w:border", m_oBorder);
			WritingElement_ReadNode(oNode, oChild, L"w:shd", m_oShd);
		}
		void CFormPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				if (L"w:shd" == sName)
					m_oShd = oReader;
				else if (L"w:border" == sName)
					m_oBorder = oReader;
				else if (L"w:field" == sName)
				{
					WritingElement_ReadAttributes_Start(oReader)
						WritingElement_ReadAttributes_ReadSingle(oReader, L"r:id", m_oFieldRid)
					WritingElement_ReadAttributes_End(oReader)
				}
			}
		}
		std::wstring CFormPr::toXML() const
		{
			std::wstring sResult = L"<w:formPr";

			if (m_oKey.IsInit())
			{
				sResult += L" w:key=\"" + XmlUtils::EncodeXmlString(m_oKey.get()) + L"\"";
			}
			if (m_oLabel.IsInit())
			{
				sResult += L" w:label=\"" + XmlUtils::EncodeXmlString(m_oLabel.get()) + L"\"";
			}
			if (m_oHelpText.IsInit())
			{
				sResult += L" w:helpText=\"" + XmlUtils::EncodeXmlString(m_oHelpText.get()) + L"\"";
			}
			if (m_oRequired.IsInit())
			{
				if (*m_oRequired)
				{
					sResult += L" w:required=\"1\" ";
				}
				else
				{
					sResult += L" w:required=\"0\" ";
				}

			}
			sResult += L">";
			WritingElement_WriteNode_1(L"<w:shd ", m_oShd);
			WritingElement_WriteNode_1(L"<w:border ", m_oBorder);

			if (m_oFieldRid.IsInit())
			{
				sResult += L"<w:field r:id=\"" + m_oFieldRid->ToString() + L"\"/>";
			}
			sResult += L"</w:formPr>";

			return sResult;
		}
		void CFormPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"key", m_oKey)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"label", m_oLabel)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"helpText", m_oHelpText)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"required", m_oRequired)
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CTextFormPr::CTextFormPr(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CTextFormPr::~CTextFormPr()
		{
		}
		void CTextFormPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, L"w:comb", m_oComb );
			WritingElement_ReadNode( oNode, oChild, L"w:maxCharacters", m_oMaxCharacters );
			WritingElement_ReadNode( oNode, oChild, L"w:combBorder", m_oCombBorder );
			WritingElement_ReadNode( oNode, oChild, L"w:format", m_oFormat);
		}
		void CTextFormPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:comb" == sName )
					m_oComb = oReader;
				else if ( L"w:maxCharacters" == sName )
					m_oMaxCharacters = oReader;
				else if ( L"w:combBorder" == sName )
					m_oCombBorder = oReader;
				else if (L"w:format" == sName)
					m_oFormat = oReader;
			}
		}
		std::wstring CTextFormPr::toXML() const
		{
			std::wstring sResult= L"<w:textFormPr";
			if (m_oMultiLine.IsInit())
			{
				sResult += L" w:multiLine=\"" + std::wstring(*m_oMultiLine ? L"1" : L"0") + L"\"";
			}
			if (m_oAutoFit.IsInit())
			{
				sResult += L" w:autoFit=\"" + std::wstring(*m_oAutoFit ? L"1" : L"0") + L"\"";
			}
			sResult += L">";
			WritingElement_WriteNode_1( L"<w:comb ", m_oComb );
			WritingElement_WriteNode_1( L"<w:maxCharacters ", m_oMaxCharacters );
			WritingElement_WriteNode_1( L"<w:combBorder ", m_oCombBorder );
			WritingElement_WriteNode_1( L"<w:format ", m_oFormat);
			sResult += L"</w:textFormPr>";

			return sResult;
		}
		EElementType CTextFormPr::getType() const
		{
			return et_w_textFormPr;
		}
		void CTextFormPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"w:autoFit", m_oAutoFit)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:multiLine", m_oMultiLine)
			WritingElement_ReadAttributes_End(oReader)
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CComplexFormPr::CComplexFormPr(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CComplexFormPr::~CComplexFormPr()
		{
		}
		void CComplexFormPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CComplexFormPr::fromXML(XmlUtils::CXmlLiteReader &oReader)
		{
			ReadAttributes(oReader);
		}
		std::wstring CComplexFormPr::toXML() const
		{
			std::wstring sResult= L"<w:complexFormPr";

			if (m_oType.IsInit() && !m_oType->IsDefaultValue())
			{
				sResult += L" w:type=\"" + m_oType->ToString() + L"\"";
			}
			sResult += L"/>";
			return sResult;
		}
		EElementType CComplexFormPr::getType() const
		{
			return et_w_complexFormPr;
		}
		void CComplexFormPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"w:type", m_oType)
			WritingElement_ReadAttributes_End(oReader)
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CSdtComboBox::CSdtComboBox(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSdtComboBox::~CSdtComboBox()
		{
			for ( size_t nIndex = 0; nIndex < m_arrListItem.size(); nIndex++ )
			{
				if ( m_arrListItem[nIndex] )
					delete m_arrListItem[nIndex];

				m_arrListItem[nIndex] = NULL;
			}

			m_arrListItem.clear();
		}
		void CSdtComboBox::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:lastValue", m_sLastValue );

			std::vector<XmlUtils::CXmlNode> oChilds;
			if ( oNode.GetNodes( L"w:listItem", oChilds ) )
			{
				for ( size_t nIndex = 0; nIndex < oChilds.size(); nIndex++ )
				{
					XmlUtils::CXmlNode& oItem = oChilds[nIndex];
					if (oItem.IsValid())
					{
						ComplexTypes::Word::CSdtListItem *oListItem = new ComplexTypes::Word::CSdtListItem();
						*oListItem = oItem;

						if (oListItem) m_arrListItem.push_back( oListItem );
					}
				}
			}
			XmlUtils::CXmlNode oChild;
			if (oNode.GetNode(L"w:format", oChild))
			{
				m_oFormat = oChild;
			}
		}
		void CSdtComboBox::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:listItem" == sName )
				{
					ComplexTypes::Word::CSdtListItem *oListItem = new ComplexTypes::Word::CSdtListItem();
					*oListItem = oReader;

					m_arrListItem.push_back( oListItem );
				}
				else if (L"w:format" == sName)
					m_oFormat = oReader;
			}
		}
		std::wstring CSdtComboBox::toXML() const
		{
			std::wstring sResult;

			if ( m_sLastValue.IsInit() )
			{
				sResult = L"<w:comboBox w:lastValue=\"";
				sResult += m_sLastValue.get2();
				sResult += L"\">";
			}
			else
				sResult = L"<w:comboBox>";

			WritingElement_WriteNode_1(L"<w:format ", m_oFormat);
			for (size_t nIndex = 0; nIndex < m_arrListItem.size(); nIndex++ )
			{
				sResult += L"<w:listItem ";
				if (m_arrListItem[nIndex])
					sResult += m_arrListItem[nIndex]->ToString();
				sResult += L"/>";
			}
			sResult += L"</w:comboBox>";

			return sResult;
		}
		EElementType CSdtComboBox::getType() const
		{
			return et_w_comboBox;
		}
		void CSdtComboBox::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:lastValue", m_sLastValue )
			WritingElement_ReadAttributes_End( oReader )
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CDate::CDate(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CDate::~CDate()
		{
		}
		void CDate::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:fullDate", m_oFullDate );

			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, L"w:calendar",          m_oCalendar );
			WritingElement_ReadNode( oNode, oChild, L"w:dateFormat",        m_oDateFormat );
			WritingElement_ReadNode( oNode, oChild, L"w:lid",               m_oLid );
			WritingElement_ReadNode( oNode, oChild, L"w:storeMappedDateAs", m_oStoreMappedDateAs );
		}
		void CDate::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:calendar" == sName )
					m_oCalendar = oReader;
				else if ( L"w:dateFormat" == sName )
					m_oDateFormat = oReader;
				else if ( L"w:lid" == sName )
					m_oLid = oReader;
				else if ( L"w:storeMappedDateAs" == sName )
					m_oStoreMappedDateAs = oReader;
			}
		}
		std::wstring CDate::toXML() const
		{
			std::wstring sResult;

			if ( m_oFullDate.IsInit() )
			{
				sResult = L"<w:date w:fullDate=\"";
				sResult += m_oFullDate->ToString();
				sResult += L"\">";
			}
			else
				sResult = L"<w:date>";

			WritingElement_WriteNode_1( L"<w:calendar ",          m_oCalendar );
			WritingElement_WriteNode_1( L"<w:dateFormat ",        m_oDateFormat );
			WritingElement_WriteNode_1( L"<w:lid ",               m_oLid );
			WritingElement_WriteNode_1( L"<w:storeMappedDateAs ", m_oStoreMappedDateAs );

			sResult += L"</w:date>";

			return sResult;
		}
		EElementType CDate::getType() const
		{
			return et_w_date;
		}
		void CDate::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:fullDate", m_oFullDate )
			WritingElement_ReadAttributes_End( oReader )
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CSdtDocPart::CSdtDocPart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSdtDocPart::~CSdtDocPart()
		{
		}
		void CSdtDocPart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, L"w:docPartCategory", m_oDocPartCategory );
			WritingElement_ReadNode( oNode, oChild, L"w:docPartGallery",  m_oDocPartGallery );
			WritingElement_ReadNode( oNode, oChild, L"w:docPartUnique",   m_oDocPartUnique );
		}
		void CSdtDocPart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:docPartCategory" == sName )
					m_oDocPartCategory = oReader;
				else if ( L"w:docPartGallery" == sName )
					m_oDocPartGallery = oReader;
				else if ( L"w:docPartUnique" == sName )
					m_oDocPartUnique = oReader;
			}
		}
		std::wstring CSdtDocPart::toXML() const
		{
			std::wstring sResult = L"<w:docPartList>";

			WritingElement_WriteNode_1( L"<w:docPartCategory ", m_oDocPartCategory );
			WritingElement_WriteNode_1( L"<w:docPartGallery ",  m_oDocPartGallery );
			WritingElement_WriteNode_1( L"<w:docPartUnique ",   m_oDocPartUnique );

			sResult += L"</w:docPartList>";

			return sResult;
		}
		std::wstring CSdtDocPart::toXML2(const std::wstring& sName) const
		{
			std::wstring sResult = L"<" + sName + L">";

			WritingElement_WriteNode_1( L"<w:docPartCategory ", m_oDocPartCategory );
			WritingElement_WriteNode_1( L"<w:docPartGallery ",  m_oDocPartGallery );
			WritingElement_WriteNode_1( L"<w:docPartUnique ",   m_oDocPartUnique );

			sResult += L"</" + sName + L">";;

			return sResult;
		}
		EElementType CSdtDocPart::getType() const
		{
			return et_w_docPartList;
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CSdtDropDownList::CSdtDropDownList(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSdtDropDownList::~CSdtDropDownList()
		{
			for ( size_t nIndex = 0; nIndex < m_arrListItem.size(); nIndex++ )
			{
				if ( m_arrListItem[nIndex] )
					delete m_arrListItem[nIndex];

				m_arrListItem[nIndex] = NULL;
			}

			m_arrListItem.clear();
		}
		void CSdtDropDownList::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:lastValue", m_sLastValue );

			std::vector<XmlUtils::CXmlNode> oChilds;
			if ( oNode.GetNodes( L"w:listItem", oChilds ) )
			{
				for ( size_t nIndex = 0; nIndex < oChilds.size(); nIndex++ )
				{
					XmlUtils::CXmlNode& oItem = oChilds[nIndex];
					if (oItem.IsValid())
					{
						ComplexTypes::Word::CSdtListItem *oListItem = new ComplexTypes::Word::CSdtListItem();
						*oListItem = oItem;

						if (oListItem) m_arrListItem.push_back( oListItem );
					}
				}
			}
		}
		void CSdtDropDownList::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:listItem" == sName )
				{
					ComplexTypes::Word::CSdtListItem *oListItem = new ComplexTypes::Word::CSdtListItem();
					*oListItem = oReader;

					if (oListItem)m_arrListItem.push_back( oListItem );
				}
			}
		}
		std::wstring CSdtDropDownList::toXML() const
		{
			std::wstring sResult;

			if ( m_sLastValue.IsInit() )
			{
				sResult = L"<w:dropDownList w:lastValue=\"";
				sResult += *m_sLastValue;
				sResult += L"\">";
			}
			else
				sResult = L"<w:dropDownList>";

			for (size_t nIndex = 0; nIndex < m_arrListItem.size(); nIndex++ )
			{
				sResult += L"<w:listItem ";
				if (m_arrListItem[nIndex])
					sResult += m_arrListItem[nIndex]->ToString();
				sResult += L"/>";
			}

			sResult += L"</w:dropDownList>";

			return sResult;
		}
		EElementType CSdtDropDownList::getType() const
		{
			return et_w_dropDownList;
		}
		void CSdtDropDownList::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:lastValue", m_sLastValue )
			WritingElement_ReadAttributes_End( oReader )
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CPlaceHolder::CPlaceHolder(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CPlaceHolder::~CPlaceHolder()
		{
		}
		void CPlaceHolder::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, L"w:docPart", m_oDocPart );
		}
		void CPlaceHolder::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:docPart" == sName )
					m_oDocPart = oReader;
			}
		}
		std::wstring CPlaceHolder::toXML() const
		{
			std::wstring sResult = L"<w:placeholder>";

			WritingElement_WriteNode_1( L"<w:docPart ", m_oDocPart );

			sResult += L"</w:placeholder>";

			return sResult;
		}
		EElementType CPlaceHolder::getType() const
		{
			return et_w_placeholder;
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CSdtEndPr::CSdtEndPr(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSdtEndPr::~CSdtEndPr()
		{
		}
		void CSdtEndPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( L"w:rPr", oChild ) )
				m_oRPr = oChild;
		}
		void CSdtEndPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:rPr" == sName )
					m_oRPr = oReader;
			}
		}
		std::wstring CSdtEndPr::toXML() const
		{
			std::wstring sResult = L"<w:sdtEndPr>";

			if ( m_oRPr.IsInit() )
				sResult += m_oRPr->toXML();

			sResult += L"</w:sdtEndPr>";

			return sResult;
		}
		EElementType CSdtEndPr::getType() const
		{
			return et_w_sdtEndPr;
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CSdtCheckBoxSymbol::CSdtCheckBoxSymbol(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSdtCheckBoxSymbol::~CSdtCheckBoxSymbol()
		{
		}
		void CSdtCheckBoxSymbol::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w14:val", m_oVal );
			XmlMacroReadAttributeBase( oNode, L"w14:font", m_oFont );
		}
		void CSdtCheckBoxSymbol::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CSdtCheckBoxSymbol::toXML() const
		{
			return L"<w14:checkedState " + ToString() + L"/>";
		}
		std::wstring CSdtCheckBoxSymbol::ToString() const
		{
			std::wstring sResult;
			ComplexTypes_WriteAttribute( L"w14:val=\"", m_oVal );
			ComplexTypes_WriteAttribute2( L"w14:font=\"", m_oFont );
			return sResult;
		}
		EElementType CSdtCheckBoxSymbol::getType() const
		{
			return et_w_sdtCheckboxSymbol;
		}
		void CSdtCheckBoxSymbol::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if( oReader, L"w14:val", m_oVal )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w14:font", m_oFont )
			WritingElement_ReadAttributes_End( oReader )
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CSdtPr::CSdtPr(OOX::Document *pMain) : WritingElement(pMain)
		{
			m_eType = sdttypeUnknown;
		}
		CSdtPr::~CSdtPr()
		{
		}
		void CSdtPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_eType = sdttypeUnknown;

			XmlUtils::CXmlNode oChild;

			if (oNode.GetNode(L"w:alias", oChild))
				m_oAlias = oChild;

			if (oNode.GetNode(L"w15:appearance", oChild))
				m_oAppearance = oChild;

			if (oNode.GetNode(L"w:bibliography", oChild))
				m_eType = sdttypeBibliography;

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:citation", oChild))
				m_eType = sdttypeCitation;

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:comboBox", oChild))
			{
				m_oComboBox = oChild;
				m_eType = sdttypeComboBox;
			}
			if (oNode.GetNode(L"w15:color", oChild))
				m_oColor = oChild;

			if (oNode.GetNode(L"w:dataBinding", oChild))
				m_oDataBinding = oChild;

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:date", oChild))
			{
				m_oDate = oChild;
				m_eType = sdttypeDate;
			}

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:docPartList", oChild))
			{
				m_oDocPartList = oChild;
				m_eType = sdttypeDocPartList;
			}

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:docPartObj", oChild))
			{
				m_oDocPartObj = oChild;
				m_eType = sdttypeDocPartObj;
			}

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:dropDownList", oChild))
			{
				m_oDropDownList = oChild;
				m_eType = sdttypeDropDownList;
			}

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:equation", oChild))
				m_eType = sdttypeEquation;

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:group", oChild))
				m_eType = sdttypeGroup;

			if (oNode.GetNode(L"w:id", oChild))
				m_oId = oChild;

			if (oNode.GetNode(L"w:label", oChild))
				m_oLabel = oChild;

			if (oNode.GetNode(L"w:lock", oChild))
				m_oLock = oChild;

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:picture", oChild))
			{
				m_oPicture = oChild;
				m_eType = sdttypePicture;
			}

			if (oNode.GetNode(L"w:placeholder", oChild))
				m_oPlaceHolder = oChild;

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:richText", oChild))
				m_eType = sdttypeRichText;

			if (oNode.GetNode(L"w:rPr", oChild))
				m_oRPr = oChild;

			if (oNode.GetNode(L"w:showingPlcHdr", oChild))
				m_oShowingPlcHdr = oChild;

			if (oNode.GetNode(L"w:tabIndex", oChild))
				m_oTabIndex = oChild;

			if (oNode.GetNode(L"w:tag", oChild))
				m_oTag = oChild;

			if (oNode.GetNode(L"w:temporary", oChild))
				m_oTemporary = oChild;
			if (oNode.GetNode(L"w:formPr", oChild))
				m_oFormPr = oChild;
			if (oNode.GetNode(L"w:textFormPr", oChild))
				m_oTextFormPr = oChild;

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w:text", oChild))
			{
				m_oText = oChild;
				m_eType = sdttypeText;
			}

			if (sdttypeUnknown == m_eType && oNode.GetNode(L"w14:checkbox", oChild))
			{
				m_oCheckbox = oChild;
				m_eType = sdttypeCheckBox;
			}

			if (oNode.GetNode(L"w:complexFormPr", oChild))
				m_oComplexFormPr = oChild;
		}
		void CSdtPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_eType = sdttypeUnknown;

			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetNameNoNS();

				if (L"alias" == sName)
					m_oAlias = oReader;
				else if (L"appearance" == sName)
					m_oAppearance = oReader;
				else if (L"bibliography" == sName)
					m_eType = sdttypeBibliography;
				else if (sdttypeUnknown == m_eType && L"citation" == sName)
					m_eType = sdttypeCitation;
				else if (sdttypeUnknown == m_eType && L"comboBox" == sName)
				{
					m_oComboBox = oReader;
					m_eType = sdttypeComboBox;
				}
				else if (L"color" == sName)
					m_oColor = oReader;
				else if (L"dataBinding" == sName)
					m_oDataBinding = oReader;
				else if (sdttypeUnknown == m_eType && L"date" == sName)
				{
					m_oDate = oReader;
					m_eType = sdttypeDate;
				}
				else if (sdttypeUnknown == m_eType && L"docPartList" == sName)
				{
					m_oDocPartList = oReader;
					m_eType = sdttypeDocPartList;
				}
				else if (sdttypeUnknown == m_eType && L"docPartObj" == sName)
				{
					m_oDocPartObj = oReader;
					m_eType = sdttypeDocPartObj;

					if (m_oDocPartObj.IsInit() && m_oDocPartObj->m_oDocPartGallery.IsInit())
					{
						if (m_oDocPartObj->m_oDocPartGallery->m_sVal == L"Bibliographies")
							m_eType = sdttypeBibliography;
					}
				}
				else if (sdttypeUnknown == m_eType && L"dropDownList" == sName)
				{
					m_oDropDownList = oReader;
					m_eType = sdttypeDropDownList;
				}
				else if (sdttypeUnknown == m_eType && L"equation" == sName)
					m_eType = sdttypeEquation;
				else if (sdttypeUnknown == m_eType && L"group" == sName)
					m_eType = sdttypeGroup;
				else if (L"id" == sName)
					m_oId = oReader;
				else if (L"label" == sName)
					m_oLabel = oReader;
				else if (L"lock" == sName)
					m_oLock = oReader;
				else if (sdttypeUnknown == m_eType && L"picture" == sName)
				{
					m_oPicture = oReader;
					m_eType = sdttypePicture;
				}
				else if (L"placeholder" == sName)
					m_oPlaceHolder = oReader;
				else if (sdttypeUnknown == m_eType && L"richText" == sName)
					m_eType = sdttypeRichText;
				else if (L"rPr" == sName)
					m_oRPr = oReader;
				else if (L"showingPlcHdr" == sName)
					m_oShowingPlcHdr = oReader;
				else if (L"tabIndex" == sName)
					m_oTabIndex = oReader;
				else if (L"tag" == sName)
					m_oTag = oReader;
				else if (L"temporary" == sName)
					m_oTemporary = oReader;
				else if (L"formPr" == sName)
					m_oFormPr = oReader;
				else if (L"textFormPr" == sName)
					m_oTextFormPr = oReader;
				else if (sdttypeUnknown == m_eType && L"text" == sName)
				{
					m_oText = oReader;
					m_eType = sdttypeText;
				}
				else if (sdttypeUnknown == m_eType && L"checkbox" == sName)
				{
					m_oCheckbox = oReader;
					m_eType = sdttypeCheckBox;
				}
				else if (L"complexFormPr" == sName)
					m_oComplexFormPr = oReader;
			}
		}
		std::wstring CSdtPr::toXMLStart() const
		{
			std::wstring sResult = L"<w:sdtPr>";

			WritingElement_WriteNode_2(m_oRPr);
			WritingElement_WriteNode_1(L"<w:alias ", m_oAlias);
			WritingElement_WriteNode_1(L"<w15:appearance ", m_oAppearance);
			WritingElement_WriteNode_1(L"<w15:color ", m_oColor);
			WritingElement_WriteNode_1(L"<w:id ", m_oId);
			WritingElement_WriteNode_1(L"<w:label ", m_oLabel);
			WritingElement_WriteNode_1(L"<w:tabIndex ", m_oTabIndex);
			WritingElement_WriteNode_1(L"<w:lock ", m_oLock);
			WritingElement_WriteNode_2(m_oPlaceHolder);
			WritingElement_WriteNode_1(L"<w:showingPlcHdr ", m_oShowingPlcHdr);

			if (m_oDataBinding.IsInit())
			{ 
				if (m_oDataBinding->m_sStoreItemChecksum.IsInit())
				{
					WritingElement_WriteNode_1(L"<w15:dataBinding", m_oDataBinding);
				}
				else
				{
					WritingElement_WriteNode_1(L"<w:dataBinding", m_oDataBinding);
				}
			}
			WritingElement_WriteNode_1(L"<w:temporary ", m_oTemporary);
			WritingElement_WriteNode_1(L"<w:tag ", m_oTag);

			switch (m_eType)
			{
			case sdttypeBibliography:
			{
				sResult += L"<w:bibliography/>";
				break;
			}
			case sdttypeCitation:
			{
				sResult += L"<w:citation/>";
				break;
			}
			case sdttypeComboBox:
			{
				if (m_oComboBox.IsInit())
					sResult += m_oComboBox->toXML();

				break;
			}
			case sdttypeDate:
			{
				if (m_oDate.IsInit())
					sResult += m_oDate->toXML();

				break;
			}
			case sdttypeDocPartList:
			{
				if (m_oDocPartList.IsInit())
					sResult += m_oDocPartList->toXML2(L"w:docPartList");

				break;
			}
			case sdttypeDocPartObj:
			{
				if (m_oDocPartObj.IsInit())
					sResult += m_oDocPartObj->toXML2(L"w:docPartObj");

				break;
			}
			case sdttypeDropDownList:
			{
				if (m_oDropDownList.IsInit())
					sResult += m_oDropDownList->toXML();

				break;
			}
			case sdttypeEquation:
			{
				sResult += L"<w:equation/>";
				break;
			}
			case sdttypeGroup:
			{
				sResult += L"<w:group/>";
				break;
			}
			case sdttypePicture:
			{
				if (m_oPicture.IsInit())
					sResult += m_oPicture->toXML();
				else
					sResult += L"<w:picture/>";
				break;
			}
			case sdttypeRichText:
			{
				sResult += L"<w:richText/>";
				break;
			}
			case sdttypeText:
			{
				sResult += L"<w:text ";
				if (m_oText.IsInit())
				{
					sResult += m_oText->ToString();
				}
				sResult += L"/>";
				break;
			}
			case sdttypeCheckBox:
			{
				WritingElement_WriteNode_2(m_oCheckbox);
				break;
			}
			}
			WritingElement_WriteNode_2(m_oFormPr);
			WritingElement_WriteNode_2(m_oTextFormPr);
			WritingElement_WriteNode_2(m_oComplexFormPr);

			return sResult;
		}
		std::wstring CSdtPr::toXMLEnd() const
		{
			return L"</w:sdtPr>";
		}
		std::wstring CSdtPr::toXML() const
		{
			return toXMLStart() + toXMLEnd();
		}
		EElementType CSdtPr::getType() const
		{
			return et_w_sdtPr;
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CSdtCheckBox::CSdtCheckBox(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSdtCheckBox::~CSdtCheckBox()
		{
		}
		void CSdtCheckBox::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			if (oNode.GetNode(L"w14:checked", oChild))
				m_oChecked = oChild;
			else if (oNode.GetNode(L"w14:checkedState", oChild))
				m_oCheckedState = oChild;
			else if (oNode.GetNode(L"w14:uncheckedState", oChild))
				m_oUncheckedState = oChild;
			else if (oNode.GetNode(L"w14:groupKey", oChild))
				m_oGroupKey = oChild;
		}
		void CSdtCheckBox::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				if (L"w14:checked" == sName)
					m_oChecked = oReader;
				else if (L"w14:checkedState" == sName)
					m_oCheckedState = oReader;
				else if (L"w14:uncheckedState" == sName)
					m_oUncheckedState = oReader;
				else if (L"w14:groupKey" == sName)
					m_oGroupKey = oReader;
			}
		}
		std::wstring CSdtCheckBox::toXML() const
		{
			std::wstring sResult = L"<w14:checkbox>";

			if (m_oChecked.IsInit())
			{
				sResult += L"<w14:checked w14:val=\"";
				sResult += m_oChecked->m_oVal.ToString2(SimpleTypes::onofftostring1);
				sResult += L"\"/>";
			}
			WritingElement_WriteNode_1(L"<w14:checkedState ", m_oCheckedState);
			WritingElement_WriteNode_1(L"<w14:uncheckedState ", m_oUncheckedState);
			if (m_oGroupKey.IsInit())
			{
				sResult += L"<w14:groupKey ";
				sResult += m_oGroupKey->ToStringWithNS(L"w14:");
				sResult += L"/>";
			}

			sResult += L"</w14:checkbox>";

			return sResult;
		}
		EElementType CSdtCheckBox::getType() const
		{
			return et_w_sdtCheckbox;
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CSdtPicture::CSdtPicture(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSdtPicture::~CSdtPicture()
		{
		}
		void CSdtPicture::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"w:scaleFlag", m_oScaleFlag);
			XmlMacroReadAttributeBase(oNode, L"w:lockProportions", m_oLockProportions);
			XmlMacroReadAttributeBase(oNode, L"w:respectBorders", m_oRespectBorders);
			XmlMacroReadAttributeBase(oNode, L"w:shiftX", m_oShiftX);
			XmlMacroReadAttributeBase(oNode, L"w:shiftY", m_oShiftY);
		}
		void CSdtPicture::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"w:scaleFlag", m_oScaleFlag)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:lockProportions", m_oLockProportions)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:respectBorders", m_oRespectBorders)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:shiftX", m_oShiftX)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:shiftY", m_oShiftY)
			WritingElement_ReadAttributes_End(oReader)
		}
		void CSdtPicture::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			
			if (oReader.IsEmptyNode())
				return;
		}
		std::wstring CSdtPicture::toXML() const
		{
			std::wstring sResult = L"<w:picture";
			if (m_oScaleFlag.IsInit())
			{
				sResult += L" w:scaleFlag=\"" + std::to_wstring(*m_oScaleFlag) + L"\"";
			}
			if (m_oLockProportions.IsInit())
			{
				sResult += L" w:lockProportions=\"" + std::wstring(*m_oLockProportions ? L"1" : L"0") + L"\"";
			}
			if (m_oRespectBorders.IsInit())
			{
				sResult += L" w:respectBorders=\"" + std::wstring(*m_oRespectBorders ? L"1" : L"0") + L"\"";
			}
			if (m_oShiftX.IsInit())
			{
				sResult += L" w:shiftX=\"" + std::to_wstring(*m_oShiftX) + L"\"";
			}
			if (m_oShiftY.IsInit())
			{
				sResult += L" w:shiftY=\"" + std::to_wstring(*m_oShiftY) + L"\"";
			}
			sResult += L"/>";
			return sResult;
		}
		EElementType CSdtPicture::getType() const
		{
			return et_w_sdtPicture;
		}
//-----------------------------------------------------------------------------------------------------------------------------
		CSdt::CSdt(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSdt::~CSdt()
		{
		}
		void CSdt::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( L"w:sdtContent", oChild ) )
				m_oSdtContent = oChild;

			if ( oNode.GetNode( L"w:sdtEndPr", oChild ) )
				m_oSdtEndPr = oChild;

			if ( oNode.GetNode( L"w:sdtPr", oChild ) )
				m_oSdtPr = oChild;
		}
		void CSdt::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return; 

			int nParentDepth = oReader.GetDepth();
			
			OOX::Document* document = WritingElement::m_pMainDocument;
			
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if (L"w:sdtContent" == sName)
				{
					m_oSdtContent = new CSdtContent(document);
					m_oSdtContent->fromXML(oReader);
				}
				else if (L"w:sdtEndPr" == sName)
				{
					m_oSdtEndPr = new CSdtEndPr(document);
					m_oSdtEndPr->fromXML(oReader);
				}
				else if (L"w:sdtPr" == sName)
				{
					 m_oSdtPr = new CSdtPr(document);
					 m_oSdtPr->fromXML(oReader);
				}
			}
		}
		std::wstring CSdt::toXML() const
		{
			std::wstring sResult = L"<w:sdt>";

			if ( m_oSdtPr.IsInit() )
				sResult += m_oSdtPr->toXML();

			if ( m_oSdtEndPr.IsInit() )
				sResult += m_oSdtEndPr->toXML();

			if ( m_oSdtContent.IsInit() )
				sResult += m_oSdtContent->toXML();

			sResult += L"</w:sdt>";

			return sResult;
		}
		EElementType CSdt::getType() const
		{
			return et_w_sdt;
		}

	} // namespace Logic
} // namespace OOX
