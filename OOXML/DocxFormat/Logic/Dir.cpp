/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "Dir.h"
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
#include "../Math/oMathPara.h"
#include "../Math/OMath.h"

#include "../../Common/SimpleTypes_Word.h"

namespace OOX
{
	namespace Logic
	{
		CDir::CDir(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}		
		CDir::~CDir()
		{
		}

		const CDir& CDir::operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CDir& CDir::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}

		void CDir::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CDir::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
				else if (L"w:p" == sName)
				{
					pItem = new CParagraph(document, this);
					pItem->fromXML(oReader);
				}
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
				else if ( _T("w:tbl") == sName )
					AssignPtrXmlContent(pItem, CTbl, oReader)
				else if ( _T("w:tc") == sName )
					AssignPtrXmlContent(pItem, CTc, oReader)
				else if ( _T("w:tr") == sName )
					AssignPtrXmlContent(pItem, CTr, oReader)

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}

		std::wstring CDir::toXML() const
		{
			std::wstring sResult = _T("<w:dir");
			if(m_oVal.IsInit())
            {
                sResult += _T(" val=\"") + m_oVal->ToString() + _T("\"");
            }
			sResult += _T(">");
			
            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += _T("</w:dir>");

			return sResult;
		}
		EElementType CDir::getType() const
		{
			return et_w_dir;
		}

		void CDir::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

	} // namespace Logic
} // namespace OOX
