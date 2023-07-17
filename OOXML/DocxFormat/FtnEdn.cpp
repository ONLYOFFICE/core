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

#include "FtnEdn.h"

#include "Math/oMathPara.h"
#include "Math/OMath.h"

#include "Logic/Annotations.h"
#include "Logic/Hyperlink.h"
#include "Logic/Paragraph.h"
#include "Logic/Sdt.h"
#include "Logic/Table.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CFtnEdn 17.11.2 && 17.11.10 (Part 1)
	//--------------------------------------------------------------------------------

	CFtnEdn::CFtnEdn(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
	{
		m_eType = et_Unknown;
	}
	CFtnEdn::~CFtnEdn()
	{
	}
	void CFtnEdn::ClearItems()
	{
		m_eType = et_Unknown;

		m_oId.reset();
		m_oType.reset();

		WritingElementWithChilds::ClearItems();
	}
	void CFtnEdn::fromXML(XmlUtils::CXmlNode& oNode)
	{
		if ( L"w:footnote" == oNode.GetName() )
			m_eType = et_w_footnote;
		else if ( L"w:endnote" == oNode.GetName() )
			m_eType = et_w_endnote;
		else
			return;

		XmlMacroReadAttributeBase( oNode, L"w:id",   m_oId   );
		XmlMacroReadAttributeBase( oNode, L"w:type", m_oType );

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

					if ( L"w:altChunk" == sName )
						AssignPtrXmlContent(pItem, Logic::CAltChunk, oItem)
					else if ( L"w:bookmarkEnd" == sName )
						AssignPtrXmlContent(pItem, Logic::CBookmarkEnd, oItem)
					else if ( L"w:bookmarkStart" == sName )
						AssignPtrXmlContent(pItem, Logic::CBookmarkStart, oItem)
					else if ( L"w:commentRangeEnd" == sName )
						AssignPtrXmlContent(pItem, Logic::CCommentRangeEnd, oItem)
					else if ( L"w:commentRangeStart" == sName )
						AssignPtrXmlContent(pItem, Logic::CCommentRangeStart, oItem)
					//else if ( L"w:customXml" == sName )
					//	pItem = new Logic::CCustomXml( oItem );
					else if ( L"w:customXmlDelRangeEnd" == sName )
						AssignPtrXmlContent(pItem, Logic::CCustomXmlDelRangeEnd, oItem)
					else if ( L"w:customXmlDelRangeStart" == sName )
						AssignPtrXmlContent(pItem, Logic::CCustomXmlDelRangeStart, oItem)
					else if ( L"w:customXmlInsRangeEnd" == sName )
						AssignPtrXmlContent(pItem, Logic::CCustomXmlInsRangeEnd, oItem)
					else if ( L"w:customXmlInsRangeStart" == sName )
						AssignPtrXmlContent(pItem, Logic::CCustomXmlInsRangeStart, oItem)
					else if ( L"w:customXmlMoveFromRangeEnd" == sName )
						AssignPtrXmlContent(pItem, Logic::CCustomXmlMoveFromRangeEnd, oItem)
					else if ( L"w:customXmlMoveFromRangeStart" == sName )
						AssignPtrXmlContent(pItem, Logic::CCustomXmlMoveFromRangeStart, oItem)
					else if ( L"w:customXmlMoveToRangeEnd" == sName )
						AssignPtrXmlContent(pItem, Logic::CCustomXmlMoveToRangeEnd, oItem)
					else if ( L"w:customXmlMoveToRangeStart" == sName )
						AssignPtrXmlContent(pItem, Logic::CCustomXmlMoveToRangeStart, oItem)
					//else if ( L"w:del" == sName )
					//	pItem = new Logic::CDel( oItem );
					//else if ( L"w:ins" == sName )
					//	pItem = new Logic::CIns( oItem );
					//else if ( L"w:moveFrom" == sName )
					//	pItem = new Logic::CMoveFrom( oItem );
					else if ( L"w:moveFromRangeEnd" == sName )
						AssignPtrXmlContent(pItem, Logic::CMoveFromRangeEnd, oItem)
					else if ( L"w:moveFromRangeStart" == sName )
						AssignPtrXmlContent(pItem, Logic::CMoveFromRangeStart, oItem)
					//else if ( L"w:moveTo" == sName )
					//	pItem = new Logic::CMoveTo( oItem );
					else if ( L"w:moveToRangeEnd" == sName )
						AssignPtrXmlContent(pItem, Logic::CMoveToRangeEnd, oItem)
					else if ( L"w:moveToRangeStart" == sName )
						AssignPtrXmlContent(pItem, Logic::CMoveToRangeStart, oItem)
					else if ( L"m:oMath" == sName )
						AssignPtrXmlContent(pItem, Logic::COMath, oItem)
					else if ( L"m:oMathPara" == sName )
						AssignPtrXmlContent(pItem, Logic::COMathPara, oItem)
					else if ( L"w:p" == sName )
						AssignPtrXmlContent(pItem, Logic::CParagraph, oItem)
					else if ( L"w:permEnd" == sName )
						AssignPtrXmlContent(pItem, Logic::CPermEnd, oItem)
					else if ( L"w:permStart" == sName )
						AssignPtrXmlContent(pItem, Logic::CPermStart, oItem)
					else if ( L"w:proofErr" == sName )
						AssignPtrXmlContent(pItem, Logic::CProofErr, oItem)
					else if ( L"w:sdt" == sName )
						AssignPtrXmlContent(pItem, Logic::CSdt, oItem)
					else if ( L"w:tbl" == sName )
						AssignPtrXmlContent(pItem, Logic::CTbl, oItem)

					if ( pItem )
						m_arrItems.push_back( pItem );
				}
			}
		}
	}
	void CFtnEdn::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		std::wstring sName = oReader.GetName();
		if ( L"w:footnote" == sName )
			m_eType = et_w_footnote;
		else if ( L"w:endnote" == sName )
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

			if ( L"w:altChunk" == sName )
				AssignPtrXmlContent(pItem, Logic::CAltChunk, oReader)
			else if ( L"w:bookmarkEnd" == sName )
				AssignPtrXmlContent(pItem, Logic::CBookmarkEnd, oReader)
			else if ( L"w:bookmarkStart" == sName )
				AssignPtrXmlContent(pItem, Logic::CBookmarkStart, oReader)
			else if ( L"w:commentRangeEnd" == sName )
				AssignPtrXmlContent(pItem, Logic::CCommentRangeEnd, oReader)
			else if ( L"w:commentRangeStart" == sName )
				AssignPtrXmlContent(pItem, Logic::CCommentRangeStart, oReader)
			//else if ( L"w:customXml" == sName )
			//	pItem = new Logic::CCustomXml( oReader );
			else if ( L"w:customXmlDelRangeEnd" == sName )
				AssignPtrXmlContent(pItem, Logic::CCustomXmlDelRangeEnd, oReader)
			else if ( L"w:customXmlDelRangeStart" == sName )
				AssignPtrXmlContent(pItem, Logic::CCustomXmlDelRangeStart, oReader)
			else if ( L"w:customXmlInsRangeEnd" == sName )
				AssignPtrXmlContent(pItem, Logic::CCustomXmlInsRangeEnd, oReader)
			else if ( L"w:customXmlInsRangeStart" == sName )
				AssignPtrXmlContent(pItem, Logic::CCustomXmlInsRangeStart, oReader)
			else if ( L"w:customXmlMoveFromRangeEnd" == sName )
				AssignPtrXmlContent(pItem, Logic::CCustomXmlMoveFromRangeEnd, oReader)
			else if ( L"w:customXmlMoveFromRangeStart" == sName )
				AssignPtrXmlContent(pItem, Logic::CCustomXmlMoveFromRangeStart, oReader)
			else if ( L"w:customXmlMoveToRangeEnd" == sName )
				AssignPtrXmlContent(pItem, Logic::CCustomXmlMoveToRangeEnd, oReader)
			else if ( L"w:customXmlMoveToRangeStart" == sName )
				AssignPtrXmlContent(pItem, Logic::CCustomXmlMoveToRangeStart, oReader)
			else if ( L"w:del" == sName )
				AssignPtrXmlContent(pItem, Logic::CDel, oReader)
			else if ( L"w:ins" == sName )
				AssignPtrXmlContent(pItem, Logic::CIns, oReader)
			else if ( L"w:moveFrom" == sName )
				AssignPtrXmlContent(pItem, Logic::CMoveFrom, oReader)
			else if ( L"w:moveFromRangeEnd" == sName )
				AssignPtrXmlContent(pItem, Logic::CMoveFromRangeEnd, oReader)
			else if ( L"w:moveFromRangeStart" == sName )
				AssignPtrXmlContent(pItem, Logic::CMoveFromRangeStart, oReader)
			else if ( L"w:moveTo" == sName )
				AssignPtrXmlContent(pItem, Logic::CMoveTo, oReader)
			else if ( L"w:moveToRangeEnd" == sName )
				AssignPtrXmlContent(pItem, Logic::CMoveToRangeEnd, oReader)
			else if ( L"w:moveToRangeStart" == sName )
				AssignPtrXmlContent(pItem, Logic::CMoveToRangeStart, oReader)
			else if ( L"m:oMath" == sName )
				AssignPtrXmlContent(pItem, Logic::COMath, oReader)
			else if ( L"m:oMathPara" == sName )
				AssignPtrXmlContent(pItem, Logic::COMathPara, oReader)
			else if ( L"w:p" == sName )
				AssignPtrXmlContent(pItem, Logic::CParagraph, oReader)
			else if ( L"w:permEnd" == sName )
				AssignPtrXmlContent(pItem, Logic::CPermEnd, oReader)
			else if ( L"w:permStart" == sName )
				AssignPtrXmlContent(pItem, Logic::CPermStart, oReader)
			else if ( L"w:proofErr" == sName )
				AssignPtrXmlContent(pItem, Logic::CProofErr, oReader)
			else if ( L"w:sdt" == sName )
				AssignPtrXmlContent(pItem, Logic::CSdt, oReader)
			else if ( L"w:tbl" == sName )
				AssignPtrXmlContent(pItem, Logic::CTbl, oReader)

			if ( pItem )
				m_arrItems.push_back( pItem );
		}
	}
	void CFtnEdn::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		// Читаем атрибуты
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if( oReader, L"w:id", m_oId )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"w:type", m_oType )
		WritingElement_ReadAttributes_End( oReader )
	}
	std::wstring CFtnEdn::toXML() const
	{
		std::wstring sResult;

		if ( m_eType == et_w_footnote )
			sResult = L"<w:footnote";
		else if ( m_eType == et_w_endnote )
			sResult = L"<w:endnote";
		else
			return L"";

		ComplexTypes_WriteAttribute_( L" w:type=\"", m_oType );
		ComplexTypes_WriteAttribute_( L" w:id=\"",   m_oId );

		sResult += L">";

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (  m_arrItems[i] )
			{
				sResult += m_arrItems[i]->toXML();
			}
		}

		if ( m_eType == et_w_footnote )
			sResult += L"</w:footnote>";
		else if ( m_eType == et_w_endnote )
			sResult += L"</w:endnote>";

		return sResult;
	}
	EElementType CFtnEdn::getType() const
	{
		return m_eType;
	}

	//--------------------------------------------------------------------------------
	// CFtnEdnSepRef 17.11.3 && 17.11.9 (Part 1)
	//--------------------------------------------------------------------------------

	CFtnEdnSepRef::CFtnEdnSepRef()
	{
		m_eType = et_Unknown;
	}
	CFtnEdnSepRef::CFtnEdnSepRef(XmlUtils::CXmlNode& oNode)
	{
		m_eType = et_Unknown;
		fromXML( (XmlUtils::CXmlNode&)oNode );
	}
	CFtnEdnSepRef::CFtnEdnSepRef(XmlUtils::CXmlLiteReader& oReader)
	{
		m_eType = et_Unknown;
		fromXML( oReader );
	}
	CFtnEdnSepRef::~CFtnEdnSepRef()
	{
	}
	const CFtnEdnSepRef& CFtnEdnSepRef::operator =(const XmlUtils::CXmlNode& oNode)
	{
		fromXML( (XmlUtils::CXmlNode&)oNode );
		return *this;
	}
	const CFtnEdnSepRef& CFtnEdnSepRef::operator =(const XmlUtils::CXmlLiteReader& oReader)
	{
		fromXML( (XmlUtils::CXmlLiteReader&)oReader );
		return *this;
	}
	void CFtnEdnSepRef::fromXML(XmlUtils::CXmlNode& oNode)
	{
		if ( L"w:footnote" == oNode.GetName() )
			m_eType = et_w_footnote;
		else if ( L"w:endnote" == oNode.GetName() )
			m_eType = et_w_endnote;
		else
			return;

		XmlMacroReadAttributeBase( oNode, L"w:id",   m_oId   );
	}
	void CFtnEdnSepRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		std::wstring sName = oReader.GetName();
		if ( L"w:footnote" == sName )
			m_eType = et_w_footnote;
		else if ( L"w:endnote" == sName )
			m_eType = et_w_endnote;
		else
			return;

		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CFtnEdnSepRef::toXML() const
	{
		std::wstring sResult;

		if ( m_eType == et_w_footnote )
			sResult = L"<w:footnote ";
		else if ( m_eType == et_w_endnote )
			sResult = L"<w:endnote ";
		else
			return L"";

		ComplexTypes_WriteAttribute_( L" w:id=\"",   m_oId );

		sResult += L"/>";

		return sResult;
	}
	EElementType CFtnEdnSepRef::getType() const
	{
		return m_eType;
	}
	void CFtnEdnSepRef::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.GetAttributesCount() <= 0 )
				return;

			if ( !oReader.MoveToFirstAttribute() )
				return;

			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				if ( L"w:id" == wsName )
					m_oId = oReader.GetText();

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}

			oReader.MoveToElement();
		}

} // namespace OOX
