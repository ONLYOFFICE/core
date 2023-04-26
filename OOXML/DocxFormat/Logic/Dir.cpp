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
