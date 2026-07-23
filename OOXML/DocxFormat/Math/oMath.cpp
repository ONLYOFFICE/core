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
#include "oMathPara.h"

// TO DO: Missing classes:
//        <w:moveFrom>
//        <w:moveTo>

namespace OOX
{	
	namespace Logic
	{		
		CMathArgNodes::CMathArgNodes(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
			m_eType = et_Unknown;
		}		
		CMathArgNodes::~CMathArgNodes()
		{
		}
		const CMathArgNodes& CMathArgNodes::operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CMathArgNodes& CMathArgNodes::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CMathArgNodes::fromXML(XmlUtils::CXmlNode& oNode)
		{
            m_sNodeName = oNode.GetName();
			if(m_sNodeName == L"m:deg")
				m_eType = et_m_deg;
			else if (m_sNodeName == L"m:den")
				m_eType = et_m_den;
			else if (m_sNodeName == L"m:e")
				m_eType = et_m_e;
			else if (m_sNodeName == L"m:fName")
				m_eType = et_m_fName;
			else if (m_sNodeName == L"m:lim")
				m_eType = et_m_lim;
			else if (m_sNodeName == L"m:num")
				m_eType = et_m_num;
			else if (m_sNodeName == L"m:oMath")
				m_eType = et_m_oMath;
			else if (m_sNodeName == L"m:sub")
				m_eType = et_m_sub;
			else if (m_sNodeName == L"m:sup")
				m_eType = et_m_sup;
			else
				m_eType = et_Unknown;

			std::vector<XmlUtils::CXmlNode> oChilds;
			if ( oNode.GetNodes( L"*", oChilds ) )
			{
				for ( size_t nIndex = 0; nIndex < oChilds.size(); nIndex++ )
				{
					XmlUtils::CXmlNode& oItem = oChilds[nIndex];
					if ( oItem.IsValid( ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;
						
						if ( L"m:acc" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CAcc, oItem)
						else if ( L"m:argPr" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CArgPr, oItem)
						else if ( L"m:bar" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBar, oItem)
						else if ( L"w:bookmarkEnd" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBookmarkEnd, oItem)
						else if ( L"w:bookmarkStart" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBookmarkStart, oItem)
						else if ( L"m:borderBox" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBorderBox, oItem)
						else if ( L"m:box" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBox, oItem)
						else if ( L"w:commentRangeEnd" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCommentRangeEnd, oItem)
						else if ( L"w:commentRangeStart" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCommentRangeStart, oItem)
						else if ( L"m:ctrlPr" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCtrlPr, oItem)
						else if ( L"w:customXmlDelRangeEnd" == sName ) 
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlDelRangeEnd, oItem)
						else if ( L"w:customXmlDelRangeStart" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlDelRangeStart, oItem)
						else if ( L"w:customXmlInsRangeEnd" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlInsRangeEnd, oItem)
						else if ( L"w:customXmlInsRangeStart" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlInsRangeStart, oItem)
						else if ( L"w:customXmlMoveFromRangeEnd" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveFromRangeEnd, oItem)
						else if ( L"w:customXmlMoveFromRangeStart" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveFromRangeStart, oItem)
						else if ( L"w:customXmlMoveToRangeEnd" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveToRangeEnd, oItem)
						else if ( L"w:customXmlMoveToRangeStart" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveToRangeStart, oItem)
						else if ( L"m:d" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CDelimiter, oItem)
						else if ( L"w:del" == sName )
							AssignPtrXmlContent(pItem, CDel, oItem)
						else if ( L"m:eqArr" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CEqArr, oItem)
						else if ( L"m:f" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CFraction, oItem)
						else if ( L"m:func" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CFunc, oItem)
						else if ( L"m:groupChr" == sName )
							AssignPtrXmlContent(pItem, CGroupChr, oItem)
						else if ( L"w:ins" == sName )
							AssignPtrXmlContent(pItem, CIns, oItem)
						else if ( L"m:limLow" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CLimLow, oItem)
						else if ( L"m:limUpp" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CLimUpp, oItem)
						else if ( L"m:m" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMatrix, oItem)
						else if ( L"w:moveFrom" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveFrom, oItem)
						else if ( L"w:moveFromRangeEnd" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveFromRangeEnd, oItem)
						else if ( L"w:moveFromRangeStart" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveFromRangeStart, oItem)
						else if ( L"w:moveTo" == sName )
							AssignPtrXmlContent(pItem, CMoveTo, oItem)
						else if ( L"w:moveToRangeEnd" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveToRangeEnd, oItem)
						else if ( L"w:moveToRangeStart" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveToRangeStart, oItem)
						else if ( L"m:nary" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CNary, oItem)
						else if ( L"m:oMath" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::COMath, oItem)
						else if ( L"m:oMathPara" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::COMathPara, oItem)
						else if ( L"w:permEnd" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CPermEnd, oItem)
						else if ( L"w:permStart" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CPermStart, oItem)
						else if ( L"m:phant" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CPhant, oItem)
						else if ( L"w:proofErr" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CProofErr, oItem)
						else if ( L"m:r" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMRun, oItem)
						else if ( L"m:rad" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CRad, oItem)
						else if ( L"m:sPre" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CSPre, oItem)
						else if ( L"m:sSub" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CSSub, oItem)
						else if ( L"m:sSubSup" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CSSubSup, oItem)
						else if ( L"m:sSup" == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CSSup, oItem)

						if ( pItem )
							m_arrItems.push_back( pItem );
					}
				}
			}

		}
		void CMathArgNodes::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
            m_sNodeName = oReader.GetName();
			if(m_sNodeName == L"m:deg")
				m_eType = et_m_deg;
			else if (m_sNodeName == L"m:den")
				m_eType = et_m_den;
			else if (m_sNodeName == L"m:e")
				m_eType = et_m_e;
			else if (m_sNodeName == L"m:fName")
				m_eType = et_m_fName;
			else if (m_sNodeName == L"m:lim")
				m_eType = et_m_lim;
			else if (m_sNodeName == L"m:num")
				m_eType = et_m_num;
			else if (m_sNodeName == L"m:oMath")
				m_eType = et_m_oMath;
			else if (m_sNodeName == L"m:sub")
				m_eType = et_m_sub;
			else if (m_sNodeName == L"m:sup")
				m_eType = et_m_sup;
			else
				m_eType = et_Unknown;

			if ( oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( L"m:acc" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CAcc, oReader)
				else if ( L"m:argPr" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CArgPr, oReader)
				else if ( L"m:bar" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBar, oReader)
				else if ( L"w:bookmarkEnd" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBookmarkEnd, oReader)
				else if ( L"w:bookmarkStart" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBookmarkStart, oReader)
				else if ( L"m:borderBox" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBorderBox, oReader)
				else if ( L"m:box" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBox, oReader)
				else if ( L"w:commentRangeEnd" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCommentRangeEnd, oReader)
				else if ( L"w:commentRangeStart" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCommentRangeStart, oReader)
				else if ( L"m:ctrlPr" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCtrlPr, oReader)
				else if ( L"w:customXmlDelRangeEnd" == sName ) 
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlDelRangeEnd, oReader)
				else if ( L"w:customXmlDelRangeStart" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlDelRangeStart, oReader)
				else if ( L"w:customXmlInsRangeEnd" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlInsRangeEnd, oReader)
				else if ( L"w:customXmlInsRangeStart" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlInsRangeStart, oReader)
				else if ( L"w:customXmlMoveFromRangeEnd" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveFromRangeEnd, oReader)
				else if ( L"w:customXmlMoveFromRangeStart" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveFromRangeStart, oReader)
				else if ( L"w:customXmlMoveToRangeEnd" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveToRangeEnd, oReader)
				else if ( L"w:customXmlMoveToRangeStart" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveToRangeStart, oReader)
				else if ( L"m:d" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CDelimiter, oReader)
				else if ( L"w:del" == sName )
					AssignPtrXmlContent(pItem, CDel, oReader)
				else if ( L"m:eqArr" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CEqArr, oReader)
				else if ( L"m:f" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CFraction, oReader)
				else if ( L"m:func" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CFunc, oReader)
				else if ( L"m:groupChr" == sName )
					AssignPtrXmlContent(pItem, CGroupChr, oReader)
				else if ( L"w:ins" == sName )
					AssignPtrXmlContent(pItem, CIns, oReader)
				else if ( L"m:limLow" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CLimLow, oReader)
				else if ( L"m:limUpp" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CLimUpp, oReader)
				else if ( L"m:m" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMatrix, oReader)
				else if ( L"w:moveFrom" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveFrom, oReader)
				else if ( L"w:moveFromRangeEnd" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveFromRangeEnd, oReader)
				else if ( L"w:moveFromRangeStart" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveFromRangeStart, oReader)
				else if ( L"w:moveTo" == sName )
					AssignPtrXmlContent(pItem, CMoveTo, oReader)
				else if ( L"w:moveToRangeEnd" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveToRangeEnd, oReader)
				else if ( L"w:moveToRangeStart" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveToRangeStart, oReader)
				else if ( L"m:nary" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CNary, oReader)
				else if ( L"m:oMath" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::COMath, oReader)
				else if ( L"m:oMathPara" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::COMathPara, oReader)
				else if ( L"w:permEnd" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CPermEnd, oReader)
				else if ( L"w:permStart" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CPermStart, oReader)
				else if ( L"m:phant" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CPhant, oReader)
				else if ( L"w:proofErr" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CProofErr, oReader)
				else if ( L"m:r" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMRun, oReader)
				else if ( L"m:rad" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CRad, oReader)
				else if ( L"m:sPre" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CSPre, oReader)
				else if ( L"m:sSub" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CSSub, oReader)
				else if ( L"m:sSubSup" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CSSubSup, oReader)
				else if ( L"m:sSup" == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CSSup, oReader)

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}
		std::wstring CMathArgNodes::toXML() const
		{
			std::wstring sNodeName = m_sNodeName;
			if (sNodeName.empty())
				sNodeName = GetMathNodeName(getType());

			if (sNodeName.empty()) return L"";

			std::wstring sResult = L"<" + sNodeName + L">";

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					sResult += m_arrItems[i]->toXML();
				}
			}

			sResult += L"</" + sNodeName + L">";

			return sResult;
		}
		EElementType CMathArgNodes::getType() const
		{
			return m_eType;
		}
		std::wstring CMathArgNodes::GetMathNodeName(const EElementType & enumType)  const
		{//todooo extract to one place - overlaps with MathBottomNodes
			switch(enumType)
			{
				case OOX::et_m_deg:		return L"m:deg";
				case OOX::et_m_den:		return L"m:den";
				case OOX::et_m_e:		return L"m:e";
				case OOX::et_m_fName:	return L"m:fName";
				case OOX::et_m_lim:		return L"m:lim";
				case OOX::et_m_num:		return L"m:num";
				case OOX::et_m_oMath:	return L"m:oMath";
				case OOX::et_m_sub:		return L"m:sub";
				case OOX::et_m_sup:		return L"m:sup";
				default:
					break;
			}
			return L"";
		}

	}//namespace Logic
}//namespace OOX
