﻿/*
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
#include "Sdt.h"
#include "Paragraph.h"
#include "Annotations.h"
#include "Run.h"
#include "RunProperty.h"
#include "ParagraphProperty.h"
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
		void CSdtContent::fromXML(XmlUtils::CXmlNode& oNode)
		{
			ClearItems();

			XmlUtils::CXmlNodes oChilds;
			if ( oNode.GetNodes( L"*", oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( L"w:bdo" == sName )
							pItem = new CBdo( oItem );
						else if ( L"w:bookmarkEnd" == sName )
							pItem = new CBookmarkEnd( oItem );
						else if ( L"w:bookmarkStart" == sName )
							pItem = new CBookmarkStart( oItem );
						else if ( L"w:commentRangeEnd" == sName )
							pItem = new CCommentRangeEnd( oItem );
						else if ( L"w:commentRangeStart" == sName )
							pItem = new CCommentRangeStart( oItem );
						//else if ( L"w:customXml" == sName )
						//	pItem = new CCustomXml( oItem );
						else if ( L"w:customXmlDelRangeEnd" == sName )
							pItem = new CCustomXmlDelRangeEnd( oItem );
						else if ( L"w:customXmlDelRangeStart" == sName )
							pItem = new CCustomXmlDelRangeStart( oItem );
						else if ( L"w:customXmlInsRangeEnd" == sName )
							pItem = new CCustomXmlInsRangeEnd( oItem );
						else if ( L"w:customXmlInsRangeStart" == sName )
							pItem = new CCustomXmlInsRangeStart( oItem );
						else if ( L"w:customXmlMoveFromRangeEnd" == sName ) 
							pItem = new CCustomXmlMoveFromRangeEnd( oItem );
						else if ( L"w:customXmlMoveFromRangeStart" == sName )
							pItem = new CCustomXmlMoveFromRangeStart( oItem );
						else if ( L"w:customXmlMoveToRangeEnd" == sName ) 
							pItem = new CCustomXmlMoveToRangeEnd( oItem );
						else if ( L"w:customXmlMoveToRangeStart" == sName )
							pItem = new CCustomXmlMoveToRangeStart( oItem );
						//else if ( L"w:del" == sName )
						//	pItem = new CDel( oItem );
						else if ( L"w:dir" == sName )
							pItem = new CDir( oItem );
						else if ( L"w:fldSimple" == sName )
							pItem = new CFldSimple( oItem );
						else if ( L"w:hyperlink" == sName )
							pItem = new CHyperlink( oItem );
						//else if ( L"w:ins" == sName )
						//	pItem = new CIns( oItem );
						//else if ( L"w:moveFrom" == sName )
						//	pItem = new CMoveFrom( oItem );
						else if ( L"w:moveFromRangeEnd" == sName )
							pItem = new CMoveFromRangeEnd( oItem );
						else if ( L"w:moveFromRangeStart" == sName )
							pItem = new CMoveFromRangeStart( oItem );
						//else if ( L"w:moveTo" == sName )
						//	pItem = new CMoveTo( oItem );
						else if ( L"w:moveToRangeEnd" == sName )
							pItem = new CMoveToRangeEnd( oItem );
						else if ( L"w:moveToRangeStart" == sName )
							pItem = new CMoveToRangeStart( oItem );
						else if ( L"m:oMath" == sName )
							pItem = new COMath( oItem );
						else if ( L"m:oMathPara" == sName )
							pItem = new COMathPara( oItem );
						else if ( L"w:p" == sName )
							pItem = new CParagraph( oItem );
						else if ( L"w:permEnd" == sName )
							pItem = new CPermEnd( oItem );
						else if ( L"w:permStart" == sName )
							pItem = new CPermStart( oItem );
						else if ( L"w:proofErr" == sName )
							pItem = new CProofErr( oItem );
						else if ( L"w:r" == sName )
							pItem = new CRun( oItem );
						else if ( L"w:sdt" == sName )
							pItem = new CSdt( oItem );
						else if ( L"w:smartTag" == sName )
							pItem = new CSmartTag( oItem );
						//else if ( L"w:subDoc" == sName )
						//	pItem = new CSubDoc( oItem );
						else if ( L"w:tbl" == sName )
							pItem = new CTbl( oItem );
						else if ( L"w:tc" == sName )
							pItem = new CTc( oItem );
						else if ( L"w:tr" == sName )
							pItem = new CTr( oItem );

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
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( L"w:bdo" == sName )
					pItem = new CBdo( oReader );
				else if ( L"w:bookmarkEnd" == sName )
					pItem = new CBookmarkEnd( oReader );
				else if ( L"w:bookmarkStart" == sName )
					pItem = new CBookmarkStart( oReader );
				else if ( L"w:commentRangeEnd" == sName )
					pItem = new CCommentRangeEnd( oReader );
				else if ( L"w:commentRangeStart" == sName )
					pItem = new CCommentRangeStart( oReader );
				//else if ( L"w:customXml" == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( L"w:customXmlDelRangeEnd" == sName )
					pItem = new CCustomXmlDelRangeEnd( oReader );
				else if ( L"w:customXmlDelRangeStart" == sName )
					pItem = new CCustomXmlDelRangeStart( oReader );
				else if ( L"w:customXmlInsRangeEnd" == sName )
					pItem = new CCustomXmlInsRangeEnd( oReader );
				else if ( L"w:customXmlInsRangeStart" == sName )
					pItem = new CCustomXmlInsRangeStart( oReader );
				else if ( L"w:customXmlMoveFromRangeEnd" == sName ) 
					pItem = new CCustomXmlMoveFromRangeEnd( oReader );
				else if ( L"w:customXmlMoveFromRangeStart" == sName )
					pItem = new CCustomXmlMoveFromRangeStart( oReader );
				else if ( L"w:customXmlMoveToRangeEnd" == sName ) 
					pItem = new CCustomXmlMoveToRangeEnd( oReader );
				else if ( L"w:customXmlMoveToRangeStart" == sName )
					pItem = new CCustomXmlMoveToRangeStart( oReader );
				else if ( L"w:del" == sName )
					pItem = new CDel( oReader );
				else if ( L"w:dir" == sName )
					pItem = new CDir( oReader );
				else if ( L"w:fldSimple" == sName )
					pItem = new CFldSimple( oReader );
				else if ( L"w:hyperlink" == sName )
					pItem = new CHyperlink( oReader );
				else if ( L"w:ins" == sName )
					pItem = new CIns( oReader );
				else if ( L"w:moveFrom" == sName )
					pItem = new CMoveFrom( oReader );
				else if ( L"w:moveFromRangeEnd" == sName )
					pItem = new CMoveFromRangeEnd( oReader );
				else if ( L"w:moveFromRangeStart" == sName )
					pItem = new CMoveFromRangeStart( oReader );
				else if ( L"w:moveTo" == sName )
					pItem = new CMoveTo( oReader );
				else if ( L"w:moveToRangeEnd" == sName )
					pItem = new CMoveToRangeEnd( oReader );
				else if ( L"w:moveToRangeStart" == sName )
					pItem = new CMoveToRangeStart( oReader );
				else if ( L"m:oMath" == sName )
					pItem = new COMath( oReader );
				else if ( L"m:oMathPara" == sName )
					pItem = new COMathPara( oReader );
				else if ( L"w:p" == sName )
					pItem = new CParagraph( oReader );
				else if ( L"w:permEnd" == sName )
					pItem = new CPermEnd( oReader );
				else if ( L"w:permStart" == sName )
					pItem = new CPermStart( oReader );
				else if ( L"w:proofErr" == sName )
					pItem = new CProofErr( oReader );
				else if ( L"w:r" == sName )
					pItem = new CRun( oReader );
				else if ( L"w:sdt" == sName )
					pItem = new CSdt( oReader );
				else if ( L"w:smartTag" == sName )
					pItem = new CSmartTag( oReader );
				//else if ( L"w:subDoc" == sName )
				//	pItem = new CSubDoc( oReader );
				else if ( L"w:tbl" == sName )
					pItem = new CTbl( oReader );
				else if ( L"w:tc" == sName )
					pItem = new CTc( oReader );
				else if ( L"w:tr" == sName )
					pItem = new CTr( oReader );

				if ( pItem )
					m_arrItems.push_back( pItem );
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
//--------------------------------------------------------------------------------	
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
		void CTextFormPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, L"w:comb", m_oComb );
			WritingElement_ReadNode( oNode, oChild, L"w:maxCharacters", m_oMaxCharacters );
			WritingElement_ReadNode( oNode, oChild, L"w:combBorder", m_oCombBorder );
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
		}
		void CSdtPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_eType = sdttypeUnknown;

			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"w:alias" == sName)
					m_oAlias = oReader;
				else if (L"w15:appearance" == sName)
					m_oAppearance = oReader;
				else if (L"w:bibliography" == sName)
					m_eType = sdttypeBibliography;
				else if (sdttypeUnknown == m_eType && L"w:citation" == sName)
					m_eType = sdttypeCitation;
				else if (sdttypeUnknown == m_eType && L"w:comboBox" == sName)
				{
					m_oComboBox = oReader;
					m_eType = sdttypeComboBox;
				}
				else if (L"w15:color" == sName)
					m_oColor = oReader;
				else if (L"w:dataBinding" == sName)
					m_oDataBinding = oReader;
				else if (sdttypeUnknown == m_eType && L"w:date" == sName)
				{
					m_oDate = oReader;
					m_eType = sdttypeDate;
				}
				else if (sdttypeUnknown == m_eType && L"w:docPartList" == sName)
				{
					m_oDocPartList = oReader;
					m_eType = sdttypeDocPartList;
				}
				else if (sdttypeUnknown == m_eType && L"w:docPartObj" == sName)
				{
					m_oDocPartObj = oReader;
					m_eType = sdttypeDocPartObj;

					if (m_oDocPartObj.IsInit() && m_oDocPartObj->m_oDocPartGallery.IsInit())
					{
						if (m_oDocPartObj->m_oDocPartGallery->m_sVal == L"Bibliographies")
							m_eType = sdttypeBibliography;
					}
				}
				else if (sdttypeUnknown == m_eType && L"w:dropDownList" == sName)
				{
					m_oDropDownList = oReader;
					m_eType = sdttypeDropDownList;
				}
				else if (sdttypeUnknown == m_eType && L"w:equation" == sName)
					m_eType = sdttypeEquation;
				else if (sdttypeUnknown == m_eType && L"w:group" == sName)
					m_eType = sdttypeGroup;
				else if (L"w:id" == sName)
					m_oId = oReader;
				else if (L"w:label" == sName)
					m_oLabel = oReader;
				else if (L"w:lock" == sName)
					m_oLock = oReader;
				else if (sdttypeUnknown == m_eType && L"w:picture" == sName)
				{
					m_oPicture = oReader;
					m_eType = sdttypePicture;
				}
				else if (L"w:placeholder" == sName)
					m_oPlaceHolder = oReader;
				else if (sdttypeUnknown == m_eType && L"w:richText" == sName)
					m_eType = sdttypeRichText;
				else if (L"w:rPr" == sName)
					m_oRPr = oReader;
				else if (L"w:showingPlcHdr" == sName)
					m_oShowingPlcHdr = oReader;
				else if (L"w:tabIndex" == sName)
					m_oTabIndex = oReader;
				else if (L"w:tag" == sName)
					m_oTag = oReader;
				else if (L"w:temporary" == sName)
					m_oTemporary = oReader;
				else if (L"w:formPr" == sName)
					m_oFormPr = oReader;
				else if (L"w:textFormPr" == sName)
					m_oTextFormPr = oReader;
				else if (sdttypeUnknown == m_eType && L"w:text" == sName)
				{
					m_oText = oReader;
					m_eType = sdttypeText;
				}
				else if (sdttypeUnknown == m_eType && L"w14:checkbox" == sName)
				{
					m_oCheckbox = oReader;
					m_eType = sdttypeCheckBox;
				}
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
			WritingElement_WriteNode_1(L"<w:dataBinding ", m_oDataBinding);
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

			return sResult;
		}
//-----------------------------------------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------------------------------------
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

	} // namespace Logic
} // namespace OOX
