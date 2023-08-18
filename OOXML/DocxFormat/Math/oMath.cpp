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
#include "oMathPara.h"

// TO DO: Нехватающие классы:
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
			if(m_sNodeName == _T("m:deg"))
				m_eType = et_m_deg;
			else if (m_sNodeName == _T("m:den"))
				m_eType = et_m_den;
			else if (m_sNodeName == _T("m:e"))
				m_eType = et_m_e;
			else if (m_sNodeName == _T("m:fName"))
				m_eType = et_m_fName;
			else if (m_sNodeName == _T("m:lim"))
				m_eType = et_m_lim;
			else if (m_sNodeName == _T("m:num"))
				m_eType = et_m_num;
			else if (m_sNodeName == _T("m:oMath"))
				m_eType = et_m_oMath;
			else if (m_sNodeName == _T("m:sub"))
				m_eType = et_m_sub;
			else if (m_sNodeName == _T("m:sup"))
				m_eType = et_m_sup;
			else
				m_eType = et_Unknown;

			std::vector<XmlUtils::CXmlNode> oChilds;
			if ( oNode.GetNodes( _T("*"), oChilds ) )
			{
				for ( size_t nIndex = 0; nIndex < oChilds.size(); nIndex++ )
				{
					XmlUtils::CXmlNode& oItem = oChilds[nIndex];
					if ( oItem.IsValid( ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;
						
						if ( _T("m:acc") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CAcc, oItem)
						else if ( _T("m:argPr") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CArgPr, oItem)
						else if ( _T("m:bar") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBar, oItem)
						else if ( _T("w:bookmarkEnd") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBookmarkEnd, oItem)
						else if ( _T("w:bookmarkStart") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBookmarkStart, oItem)
						else if ( _T("m:borderBox") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBorderBox, oItem)
						else if ( _T("m:box") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CBox, oItem)
						else if ( _T("w:commentRangeEnd") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCommentRangeEnd, oItem)
						else if ( _T("w:commentRangeStart") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCommentRangeStart, oItem)
						else if ( _T("m:ctrlPr") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCtrlPr, oItem)
						else if ( _T("w:customXmlDelRangeEnd") == sName ) 
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlDelRangeEnd, oItem)
						else if ( _T("w:customXmlDelRangeStart") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlDelRangeStart, oItem)
						else if ( _T("w:customXmlInsRangeEnd") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlInsRangeEnd, oItem)
						else if ( _T("w:customXmlInsRangeStart") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlInsRangeStart, oItem)
						else if ( _T("w:customXmlMoveFromRangeEnd") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveFromRangeEnd, oItem)
						else if ( _T("w:customXmlMoveFromRangeStart") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveFromRangeStart, oItem)
						else if ( _T("w:customXmlMoveToRangeEnd") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveToRangeEnd, oItem)
						else if ( _T("w:customXmlMoveToRangeStart") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveToRangeStart, oItem)
						else if ( _T("m:d") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CDelimiter, oItem)
						else if ( _T("w:del") == sName )
							AssignPtrXmlContent(pItem, CDel, oItem)
						else if ( _T("m:eqArr") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CEqArr, oItem)
						else if ( _T("m:f") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CFraction, oItem)
						else if ( _T("m:func") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CFunc, oItem)
						else if ( _T("m:groupChr") == sName )
							AssignPtrXmlContent(pItem, CGroupChr, oItem)
						else if ( _T("w:ins") == sName )
							AssignPtrXmlContent(pItem, CIns, oItem)
						else if ( _T("m:limLow") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CLimLow, oItem)
						else if ( _T("m:limUpp") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CLimUpp, oItem)
						else if ( _T("m:m") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMatrix, oItem)
						else if ( _T("w:moveFrom") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveFrom, oItem)
						else if ( _T("w:moveFromRangeEnd") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveFromRangeEnd, oItem)
						else if ( _T("w:moveFromRangeStart") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveFromRangeStart, oItem)
						else if ( _T("w:moveTo") == sName )
							AssignPtrXmlContent(pItem, CMoveTo, oItem)
						else if ( _T("w:moveToRangeEnd") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveToRangeEnd, oItem)
						else if ( _T("w:moveToRangeStart") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMoveToRangeStart, oItem)
						else if ( _T("m:nary") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CNary, oItem)
						else if ( _T("m:oMath") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::COMath, oItem)
						else if ( _T("m:oMathPara") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::COMathPara, oItem)
						else if ( _T("w:permEnd") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CPermEnd, oItem)
						else if ( _T("w:permStart") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CPermStart, oItem)
						else if ( _T("m:phant") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CPhant, oItem)
						else if ( _T("w:proofErr") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CProofErr, oItem)
						else if ( _T("m:r") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CMRun, oItem)
						else if ( _T("m:rad") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CRad, oItem)
						else if ( _T("m:sPre") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CSPre, oItem)
						else if ( _T("m:sSub") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CSSub, oItem)
						else if ( _T("m:sSubSup") == sName )
							AssignPtrXmlContent(pItem, OOX::Logic::CSSubSup, oItem)
						else if ( _T("m:sSup") == sName )
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
			if(m_sNodeName == _T("m:deg"))
				m_eType = et_m_deg;
			else if (m_sNodeName == _T("m:den"))
				m_eType = et_m_den;
			else if (m_sNodeName == _T("m:e"))
				m_eType = et_m_e;
			else if (m_sNodeName == _T("m:fName"))
				m_eType = et_m_fName;
			else if (m_sNodeName == _T("m:lim"))
				m_eType = et_m_lim;
			else if (m_sNodeName == _T("m:num"))
				m_eType = et_m_num;
			else if (m_sNodeName == _T("m:oMath"))
				m_eType = et_m_oMath;
			else if (m_sNodeName == _T("m:sub"))
				m_eType = et_m_sub;
			else if (m_sNodeName == _T("m:sup"))
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

				if ( _T("m:acc") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CAcc, oReader)
				else if ( _T("m:argPr") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CArgPr, oReader)
				else if ( _T("m:bar") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBar, oReader)
				else if ( _T("w:bookmarkEnd") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBookmarkEnd, oReader)
				else if ( _T("w:bookmarkStart") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBookmarkStart, oReader)
				else if ( _T("m:borderBox") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBorderBox, oReader)
				else if ( _T("m:box") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CBox, oReader)
				else if ( _T("w:commentRangeEnd") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCommentRangeEnd, oReader)
				else if ( _T("w:commentRangeStart") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCommentRangeStart, oReader)
				else if ( _T("m:ctrlPr") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCtrlPr, oReader)
				else if ( _T("w:customXmlDelRangeEnd") == sName ) 
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlDelRangeEnd, oReader)
				else if ( _T("w:customXmlDelRangeStart") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlDelRangeStart, oReader)
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlInsRangeEnd, oReader)
				else if ( _T("w:customXmlInsRangeStart") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlInsRangeStart, oReader)
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveFromRangeEnd, oReader)
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveFromRangeStart, oReader)
				else if ( _T("w:customXmlMoveToRangeEnd") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveToRangeEnd, oReader)
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CCustomXmlMoveToRangeStart, oReader)
				else if ( _T("m:d") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CDelimiter, oReader)
				else if ( _T("w:del") == sName )
					AssignPtrXmlContent(pItem, CDel, oReader)
				else if ( _T("m:eqArr") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CEqArr, oReader)
				else if ( _T("m:f") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CFraction, oReader)
				else if ( _T("m:func") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CFunc, oReader)
				else if ( _T("m:groupChr") == sName )
					AssignPtrXmlContent(pItem, CGroupChr, oReader)
				else if ( _T("w:ins") == sName )
					AssignPtrXmlContent(pItem, CIns, oReader)
				else if ( _T("m:limLow") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CLimLow, oReader)
				else if ( _T("m:limUpp") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CLimUpp, oReader)
				else if ( _T("m:m") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMatrix, oReader)
				else if ( _T("w:moveFrom") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveFrom, oReader)
				else if ( _T("w:moveFromRangeEnd") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveFromRangeEnd, oReader)
				else if ( _T("w:moveFromRangeStart") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveFromRangeStart, oReader)
				else if ( _T("w:moveTo") == sName )
					AssignPtrXmlContent(pItem, CMoveTo, oReader)
				else if ( _T("w:moveToRangeEnd") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveToRangeEnd, oReader)
				else if ( _T("w:moveToRangeStart") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMoveToRangeStart, oReader)
				else if ( _T("m:nary") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CNary, oReader)
				else if ( _T("m:oMath") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::COMath, oReader)
				else if ( _T("m:oMathPara") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::COMathPara, oReader)
				else if ( _T("w:permEnd") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CPermEnd, oReader)
				else if ( _T("w:permStart") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CPermStart, oReader)
				else if ( _T("m:phant") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CPhant, oReader)
				else if ( _T("w:proofErr") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CProofErr, oReader)
				else if ( _T("m:r") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CMRun, oReader)
				else if ( _T("m:rad") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CRad, oReader)
				else if ( _T("m:sPre") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CSPre, oReader)
				else if ( _T("m:sSub") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CSSub, oReader)
				else if ( _T("m:sSubSup") == sName )
					AssignPtrXmlContent(pItem, OOX::Logic::CSSubSup, oReader)
				else if ( _T("m:sSup") == sName )
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

			std::wstring sResult = _T("<") + sNodeName + _T(">");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					sResult += m_arrItems[i]->toXML();
				}
			}

			sResult += _T("</") + sNodeName + _T(">");

			return sResult;
		}
		EElementType CMathArgNodes::getType() const
		{
			return m_eType;
		}
		std::wstring CMathArgNodes::GetMathNodeName(const EElementType & enumType)  const
		{//todooo вытащить в одно место - пересекается с MathBottomNodes
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
