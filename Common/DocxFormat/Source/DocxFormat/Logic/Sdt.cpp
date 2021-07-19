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
		void CFormPr::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:key"), m_oKey );
			XmlMacroReadAttributeBase( oNode, _T("w:label"), m_oLabel );
			XmlMacroReadAttributeBase( oNode, _T("w:helpText"), m_oHelpText );
			XmlMacroReadAttributeBase( oNode, _T("w:required"), m_oRequired );
		}
		void CFormPr::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CFormPr::ToString() const
		{
			std::wstring sResult;
			if(m_oKey.IsInit())
			{
				sResult += _T("w:key=\"") + XmlUtils::EncodeXmlString(m_oKey.get()) + _T("\" ");
			}
			if(m_oLabel.IsInit())
			{
				sResult += _T("w:label=\"") + XmlUtils::EncodeXmlString(m_oLabel.get()) + _T("\" ");
			}
			if(m_oHelpText.IsInit())
			{
				sResult += _T("w:helpText=\"") + XmlUtils::EncodeXmlString(m_oHelpText.get()) + _T("\" ");
			}
			if(m_oRequired.IsInit())
			{
				if(*m_oRequired)
				{
					sResult += _T("w:required=\"1\" ");
				}
				else
				{
					sResult += _T("w:required=\"0\" ");
				}

			}

			return sResult;
		}
		void CFormPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if( oReader, _T("key"), m_oKey )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("label"), m_oLabel )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("helpText"), m_oHelpText )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("required"), m_oRequired )
					WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void CComb::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:width"), m_oWidth );
			XmlMacroReadAttributeBase( oNode, _T("w:sym"), m_oSym );
			XmlMacroReadAttributeBase( oNode, _T("w:font"), m_oFont );
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
			if(m_oWidth.IsInit())
			{
				sResult += _T("w:width=\"") + std::to_wstring(m_oWidth.get()) + _T("\" ");
			}
			if(m_oSym.IsInit())
			{
				sResult += _T("w:sym=\"") + XmlUtils::EncodeXmlString(m_oSym.get()) + _T("\" ");
			}
			if(m_oFont.IsInit())
			{
				sResult += _T("w:font=\"") + XmlUtils::EncodeXmlString(m_oFont.get()) + _T("\" ");
			}

			return sResult;
		}
		void CComb::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if( oReader, _T("w:width"), m_oWidth )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:sym"), m_oSym )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:font"), m_oFont )
			WritingElement_ReadAttributes_End( oReader )
		}
	}
}
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
						else if ( _T("w:p") == sName )
							pItem = new CParagraph( oItem );
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
						else if ( _T("w:tbl") == sName )
							pItem = new CTbl( oItem );
						else if ( _T("w:tc") == sName )
							pItem = new CTc( oItem );
						else if ( _T("w:tr") == sName )
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

				if ( _T("w:bdo") == sName )
					pItem = new CBdo( oReader );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new CBookmarkEnd( oReader );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new CBookmarkStart( oReader );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new CCommentRangeEnd( oReader );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new CCommentRangeStart( oReader );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new CCustomXmlDelRangeEnd( oReader );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new CCustomXmlDelRangeStart( oReader );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new CCustomXmlInsRangeEnd( oReader );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new CCustomXmlInsRangeStart( oReader );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveFromRangeEnd( oReader );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new CCustomXmlMoveFromRangeStart( oReader );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveToRangeEnd( oReader );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new CCustomXmlMoveToRangeStart( oReader );
				else if ( _T("w:del") == sName )
					pItem = new CDel( oReader );
				else if ( _T("w:dir") == sName )
					pItem = new CDir( oReader );
				else if ( _T("w:fldSimple") == sName )
					pItem = new CFldSimple( oReader );
				else if ( _T("w:hyperlink") == sName )
					pItem = new CHyperlink( oReader );
				else if ( _T("w:ins") == sName )
					pItem = new CIns( oReader );
				else if ( _T("w:moveFrom") == sName )
					pItem = new CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveFromRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveFromRangeStart( oReader );
				else if ( _T("w:moveTo") == sName )
					pItem = new CMoveTo( oReader );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new CMoveToRangeEnd( oReader );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new CMoveToRangeStart( oReader );
				else if ( _T("m:oMath") == sName )
					pItem = new COMath( oReader );
				else if ( _T("m:oMathPara") == sName )
					pItem = new COMathPara( oReader );
				else if ( _T("w:p") == sName )
					pItem = new CParagraph( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr( oReader );
				else if ( _T("w:r") == sName )
					pItem = new CRun( oReader );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( oReader );
				else if ( _T("w:smartTag") == sName )
					pItem = new CSmartTag( oReader );
				//else if ( _T("w:subDoc") == sName )
				//	pItem = new CSubDoc( oReader );
				else if ( _T("w:tbl") == sName )
					pItem = new CTbl( oReader );
				else if ( _T("w:tc") == sName )
					pItem = new CTc( oReader );
				else if ( _T("w:tr") == sName )
					pItem = new CTr( oReader );

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}
		std::wstring CSdtContent::toXML() const
		{
				std::wstring sResult = _T("<w:sdtContent>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }

				sResult += _T("</w:sdtContent>");

				return sResult;
		}
		void CTextFormPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, _T("w:comb"),          m_oComb );
			WritingElement_ReadNode( oNode, oChild, _T("w:maxCharacters"),        m_oMaxCharacters );
			WritingElement_ReadNode( oNode, oChild, _T("w:combBorder"),          m_oCombBorder );
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
				if ( _T("w:comb") == sName )
					m_oComb = oReader;
				else if ( _T("w:maxCharacters") == sName )
					m_oMaxCharacters = oReader;
				else if ( _T("w:combBorder") == sName )
					m_oCombBorder = oReader;
			}
		}
		std::wstring CTextFormPr::toXML() const
		{
			std::wstring sResult= _T("<w:textFormPr>");

			WritingElement_WriteNode_1( _T("<w:comb "),          m_oComb );
			WritingElement_WriteNode_1( _T("<w:maxCharacters "),        m_oMaxCharacters );
			WritingElement_WriteNode_1( _T("<w:combBorder "),        m_oCombBorder );

			sResult += _T("</w:textFormPr>");

			return sResult;
		}
		EElementType CTextFormPr::getType() const
		{
			return et_w_textFormPr;
		}
		void CTextFormPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

	} // namespace Logic
} // namespace OOX
