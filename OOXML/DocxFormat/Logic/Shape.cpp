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

#include "Shape.h"

#include "Paragraph.h"
#include "Annotations.h"
#include "Run.h"
#include "RunProperty.h"
#include "ParagraphProperty.h"
#include "Sdt.h"
#include "Hyperlink.h"
#include "Table.h"

#include "../Math/oMathPara.h"
#include "../Math/OMath.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CTxbxContent 9.8.1.1 (Part 4)
		// 20.4.2.38 txbxContent (Rich Text Box Content Container)
		//--------------------------------------------------------------------------------

		CTxbxContent::CTxbxContent(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}
		CTxbxContent::CTxbxContent(XmlUtils::CXmlNode &oNode) : WritingElementWithChilds<>(NULL)
		{
			fromXML( oNode );
		}
		CTxbxContent::CTxbxContent(XmlUtils::CXmlLiteReader& oReader) : WritingElementWithChilds<>(NULL)
		{
			fromXML( oReader );
		}
		CTxbxContent::~CTxbxContent()
		{

		}
		const CTxbxContent& CTxbxContent::operator =(const XmlUtils::CXmlNode& oNode)
		{
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CTxbxContent& CTxbxContent::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CTxbxContent::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CTxbxContent::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CTxbxContent::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();

			if ( oReader.IsEmptyNode() )
				return;

			OOX::Document* document = WritingElement::m_pMainDocument;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if (L"w:altChunk" == sName )
					pItem = new CAltChunk( document );
				else if (L"w:bookmarkEnd" == sName )
					pItem = new CBookmarkEnd( document );
				else if (L"w:bookmarkStart" == sName )
					pItem = new CBookmarkStart( document );
				else if (L"w:commentRangeEnd" == sName )
					pItem = new CCommentRangeEnd( document );
				else if (L"w:commentRangeStart" == sName )
					pItem = new CCommentRangeStart( document );
				//else if (L"w:customXml" == sName )
				//	pItem = new CCustomXml( document );
				else if (L"w:customXmlDelRangeEnd" == sName )
					pItem = new CCustomXmlDelRangeEnd( document );
				else if (L"w:customXmlDelRangeStart" == sName )
					pItem = new CCustomXmlDelRangeStart( document );
				else if (L"w:customXmlInsRangeEnd" == sName )
					pItem = new CCustomXmlInsRangeEnd( document );
				else if (L"w:customXmlInsRangeStart" == sName )
					pItem = new CCustomXmlInsRangeStart( document );
				else if (L"w:customXmlMoveFromRangeEnd" == sName )
					pItem = new CCustomXmlMoveFromRangeEnd( document );
				else if (L"w:customXmlMoveFromRangeStart" == sName )
					pItem = new CCustomXmlMoveFromRangeStart( document );
				else if (L"w:customXmlMoveToRangeEnd" == sName )
					pItem = new CCustomXmlMoveToRangeEnd( document );
				else if (L"w:customXmlMoveToRangeStart" == sName )
					pItem = new CCustomXmlMoveToRangeStart( document );
				else if (L"w:del" == sName )
					pItem = new CDel( document );
				else if (L"w:ins" == sName )
					pItem = new CIns( document );
				else if (L"w:moveFrom" == sName )
					pItem = new CMoveFrom( document );
				else if (L"w:moveFromRangeEnd" == sName )
					pItem = new CMoveFromRangeEnd( document );
				else if (L"w:moveFromRangeStart" == sName )
					pItem = new CMoveFromRangeStart( document );
				else if (L"w:moveTo" == sName )
					pItem = new CMoveTo( document );
				else if (L"w:moveToRangeEnd" == sName )
					pItem = new CMoveToRangeEnd( document );
				else if (L"w:moveToRangeStart" == sName )
					pItem = new CMoveToRangeStart( document );
				else if (L"m:oMath" == sName )
					pItem = new COMath( document );
				else if (L"m:oMathPara" == sName )
					pItem = new COMathPara( document );
				else if (L"w:p" == sName )
					pItem = new CParagraph( document, this );
				else if (L"w:permEnd" == sName )
					pItem = new CPermEnd( document );
				else if (L"w:permStart" == sName )
					pItem = new CPermStart( document );
				else if (L"w:proofErr" == sName )
					pItem = new CProofErr( document );
				else if (L"w:sdt" == sName )
					pItem = new CSdt( document );
				else if (L"w:tbl" == sName )
					pItem = new CTbl( document );

				if ( pItem )
				{
					pItem->fromXML(oReader);
					m_arrItems.push_back( pItem );
				}
			}
		}
		std::wstring CTxbxContent::toXML() const
		{
			std::wstring sResult = L"<w:txbxContent>";

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if ( m_arrItems[i] )
				{
					sResult += m_arrItems[i]->toXML();
				}
			}

			sResult += L"</w:txbxContent>";

			return sResult;
		}
		EElementType CTxbxContent::getType() const
			{
				return et_w_txbxContent;
			}

		//--------------------------------------------------------------------------------
		// 20.4.2.37 txbx (Textual contents of shape)
		//--------------------------------------------------------------------------------

		CTextBody::CTextBody(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CTextBody::CTextBody(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CTextBody::CTextBody()
		{
		}
		CTextBody::~CTextBody()
		{
		}
		std::wstring CTextBody::toXML() const
		{
			return L"";
		}
		void CTextBody::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CTextBody::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CTextBody::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"txbxContent" == sName) //namespaces w & wne
					m_oTxtbxContent = oReader;
			}
		}
		EElementType CTextBody::getType () const
		{
			return et_w_ShapeTextBody;
		}
		void CTextBody::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
			WritingElement_ReadAttributes_End	( oReader )
		}

	} //Logic
} // namespace OOX
