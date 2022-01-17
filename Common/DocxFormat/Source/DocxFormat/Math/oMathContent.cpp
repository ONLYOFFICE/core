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
#include "oMathContent.h"
namespace OOX
{
namespace Logic
{
	void CMathPr::fromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlUtils::CXmlNodes oChilds;
		if (oNode.GetNodes(L"*", oChilds))
		{
			XmlUtils::CXmlNode oItem;
			for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
			{
				if (oChilds.GetAt(nIndex, oItem))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oItem.GetName());
					WritingElement *pItem = NULL;

					if (L"brkBin" == sName)
						m_oBrkBin = oItem;
					else if (L"brkBinSub" == sName)
						m_oBrkBinSub = oItem;
					else if (L"defJc" == sName)
						m_oDefJc = oItem;
					else if (L"w:dispDef" == sName)
						m_oDispDef = oItem;
					else if (L"w:interSp" == sName)
						m_oInterSp = oItem;
					else if (L"intLim" == sName)
						m_oIntLim = oItem;
					else if (L"intraSp" == sName)
						m_oIntraSp = oItem;
					else if (L"lMargin" == sName)
						m_oLMargin = oItem;
					else if (L"mathFont" == sName)
						m_oMathFont = oItem;
					else if (L"naryLim" == sName)
						m_oNaryLim = oItem;
					else if (L"postSp" == sName)
						m_oPostSp = oItem;
					else if (L"preSp" == sName)
						m_oPreSp = oItem;
					else if (L"rMargin" == sName)
						m_oRMargin = oItem;
					else if (L"smallFrac" == sName)
						m_oSmallFrac = oItem;
					else if (L"wrapIndent" == sName)
						m_oWrapIndent = oItem;
					else if (L"wrapRight" == sName)
						m_oWrapRight = oItem;
				}
			}
		}
	}
	void CMathPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			WritingElement *pItem = NULL;

			if (L"brkBin" == sName)
				m_oBrkBin = oReader;
			else if (L"brkBinSub" == sName)
				m_oBrkBinSub = oReader;
			else if (L"defJc" == sName)
				m_oDefJc = oReader;
			else if (L"dispDef" == sName)
				m_oDispDef = oReader;
			else if (L"interSp" == sName)
				m_oInterSp = oReader;
			else if (L"intLim" == sName)
				m_oIntLim = oReader;
			else if (L"intraSp" == sName)
				m_oIntraSp = oReader;
			else if (L"lMargin" == sName)
				m_oLMargin = oReader;
			else if (L"mathFont" == sName)
				m_oMathFont = oReader;
			else if (L"naryLim" == sName)
				m_oNaryLim = oReader;
			else if (L"postSp" == sName)
				m_oPostSp = oReader;
			else if (L"preSp" == sName)
				m_oPreSp = oReader;
			else if (L"rMargin" == sName)
				m_oRMargin = oReader;
			else if (L"smallFrac" == sName)
				m_oSmallFrac = oReader;
			else if (L"wrapIndent" == sName)
				m_oWrapIndent = oReader;
			else if (L"wrapRight" == sName)
				m_oWrapRight = oReader;
		}
	}
	std::wstring CMathPr::toXML() const
	{
		std::wstring sResult = L"<m:mathPr>";

		if (m_oMathFont.IsInit()) sResult += m_oMathFont->toXML();
		if (m_oBrkBin.IsInit()) sResult += m_oBrkBin->toXML();
		if (m_oBrkBinSub.IsInit()) sResult += m_oBrkBinSub->toXML();
		if (m_oSmallFrac.IsInit()) sResult += m_oSmallFrac->toXML();
		if (m_oDispDef.IsInit()) sResult += m_oDispDef->toXML();
		if (m_oLMargin.IsInit()) sResult += m_oLMargin->toXML();
		if (m_oRMargin.IsInit()) sResult += m_oRMargin->toXML();
		if (m_oDefJc.IsInit()) sResult += m_oDefJc->toXML();
		if (m_oPreSp.IsInit()) sResult += m_oPreSp->toXML();
		if (m_oPostSp.IsInit()) sResult += m_oPostSp->toXML();
		if (m_oInterSp.IsInit()) sResult += m_oInterSp->toXML();
		if (m_oIntraSp.IsInit()) sResult += m_oIntraSp->toXML();
		if (m_oWrapIndent.IsInit()) sResult += m_oWrapIndent->toXML();
		if (m_oWrapRight.IsInit()) sResult += m_oWrapRight->toXML();
		if (m_oIntLim.IsInit()) sResult += m_oIntLim->toXML();
		if (m_oNaryLim.IsInit()) sResult += m_oNaryLim->toXML();

		sResult += L"</m:mathPr>";

		return sResult;
	}
	void CMRPr::fromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlUtils::CXmlNodes oChilds;
		if (oNode.GetNodes(L"*", oChilds))
		{
			XmlUtils::CXmlNode oItem;
			for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
			{
				if (oChilds.GetAt(nIndex, oItem))
				{
					std::wstring sName = oItem.GetName();
					WritingElement *pItem = NULL;

					if (L"m:aln" == sName)
						m_oAln = new OOX::Logic::CAln(oItem);
					else if (L"m:brk" == sName)
						m_oBrk = new OOX::Logic::CBrk(oItem);
					else if (L"m:lit" == sName)
						m_oLit = new OOX::Logic::CLit(oItem);
					else if (L"m:nor" == sName)
						m_oNor = new OOX::Logic::CNor(oItem);
					else if (L"m:scr" == sName)
						m_oScr = new OOX::Logic::CScr(oItem);
					else if (L"m:sty" == sName)
						m_oSty = new OOX::Logic::CSty(oItem);
				}
			}
		}
	}
	void CMRPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"m:aln" == sName)
				m_oAln = oReader;
			else if (L"m:brk" == sName)
				m_oBrk = oReader;
			else if (L"m:lit" == sName)
				m_oLit = oReader;
			else if (L"m:nor" == sName)
				m_oNor = oReader;
			else if (L"m:scr" == sName)
				m_oScr = oReader;
			else if (L"m:sty" == sName)
				m_oSty = oReader;
		}
	}
	std::wstring CMRPr::toXML() const
	{
		std::wstring sResult = L"<m:rPr>";

		if (m_oAln.IsInit())
			sResult += m_oAln->toXML();

		if (m_oBrk.IsInit())
			sResult += m_oBrk->toXML();

		if (m_oLit.IsInit())
			sResult += m_oLit->toXML();

		if (m_oNor.IsInit())
			sResult += m_oNor->toXML();

		if (m_oScr.IsInit())
			sResult += m_oScr->toXML();

		if (m_oSty.IsInit())
			sResult += m_oSty->toXML();

		sResult += L"</m:rPr>";
		return sResult;
	}

	void CNaryPr::fromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlUtils::CXmlNodes oChilds;
		if (oNode.GetNodes(L"*", oChilds))
		{
			XmlUtils::CXmlNode oItem;
			for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
			{
				if (oChilds.GetAt(nIndex, oItem))
				{
					std::wstring sName = oItem.GetName();
					WritingElement *pItem = NULL;

					if (L"m:ctrlPr" == sName)
						m_oCtrlPr = new OOX::Logic::CCtrlPr(oItem);
					else if (L"m:chr" == sName)
						m_oChr = new OOX::Logic::CChr(oItem);
					else if (L"m:grow" == sName)
						m_oGrow = new OOX::Logic::CGrow(oItem);
					else if (L"m:limLoc" == sName)
						m_oLimLoc = new OOX::Logic::CLimLoc(oItem);
					else if (L"m:subHide" == sName)
						m_oSubHide = new OOX::Logic::CSubHide(oItem);
					else if (L"m:supHide" == sName)
						m_oSupHide = new OOX::Logic::CSupHide(oItem);
				}
			}
		}
	}
	void CNaryPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"m:chr" == sName)
				m_oChr = oReader;
			else if (L"m:ctrlPr" == sName)
				m_oCtrlPr = oReader;
			else if (L"m:grow" == sName)
				m_oGrow = oReader;
			else if (L"m:limLoc" == sName)
				m_oLimLoc = oReader;
			else if (L"m:subHide" == sName)
				m_oSubHide = oReader;
			else if (L"m:supHide" == sName)
				m_oSupHide = oReader;
		}
	}
	std::wstring CNaryPr::toXML() const
	{
		std::wstring sResult = L"<m:naryPr>";

		if (m_oChr.IsInit())
			sResult += m_oChr->toXML();

		if (m_oCtrlPr.IsInit())
			sResult += m_oCtrlPr->toXML();

		if (m_oGrow.IsInit())
			sResult += m_oGrow->toXML();

		if (m_oLimLoc.IsInit())
			sResult += m_oLimLoc->toXML();

		if (m_oSubHide.IsInit())
			sResult += m_oSubHide->toXML();

		if (m_oSupHide.IsInit())
			sResult += m_oSupHide->toXML();

		sResult += L"</m:naryPr>";

		return sResult;
	}
	
	void CMPr::fromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlUtils::CXmlNodes oChilds;
		if (oNode.GetNodes(L"*", oChilds))
		{
			XmlUtils::CXmlNode oItem;
			for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
			{
				if (oChilds.GetAt(nIndex, oItem))
				{
					std::wstring sName = oItem.GetName();
					WritingElement *pItem = NULL;

					if (L"m:baseJc" == sName)
						m_oBaseJc = new OOX::Logic::CBaseJc(oItem);
					else if (L"m:cGp" == sName)
						m_oCGp = new OOX::Logic::CCGp(oItem);
					else if (L"m:cGpRule" == sName)
						m_oCGpRule = new OOX::Logic::CCGpRule(oItem);
					else if (L"m:cSp" == sName)
						m_oCSp = new OOX::Logic::CCSp(oItem);
					else if (L"m:ctrlPr" == sName)
						m_oCtrlPr = new OOX::Logic::CCtrlPr(oItem);
					else if (L"m:mcs" == sName)
						m_oMcs = new OOX::Logic::CMcs(oItem);
					else if (L"m:plcHide" == sName)
						m_oPlcHide = new OOX::Logic::CPlcHide(oItem);
					else if (L"m:rSp" == sName)
						m_oRSp = new OOX::Logic::CRSp(oItem);
					else if (L"m:rSpRule" == sName)
						m_oRSpRule = new OOX::Logic::CRSpRule(oItem);
				}
			}
		}
	}
	void CMPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"m:baseJc" == sName)
				m_oBaseJc = oReader;
			else if (L"m:cGp" == sName)
				m_oCGp = oReader;
			else if (L"m:cGpRule" == sName)
				m_oCGpRule = oReader;
			else if (L"m:cSp" == sName)
				m_oCSp = oReader;
			else if (L"m:ctrlPr" == sName)
				m_oCtrlPr = oReader;
			else if (L"m:mcs" == sName)
				m_oMcs = oReader;
			else if (L"m:plcHide" == sName)
				m_oPlcHide = oReader;
			else if (L"m:rSp" == sName)
				m_oRSp = oReader;
			else if (L"m:rSpRule" == sName)
				m_oRSpRule = oReader;
		}
	}
	std::wstring CMPr::toXML() const
	{
		std::wstring sResult = L"<m:mPr>";

		if (m_oBaseJc.IsInit())
			sResult += m_oBaseJc->toXML();
		if (m_oCGp.IsInit())
			sResult += m_oCGp->toXML();
		if (m_oCGpRule.IsInit())
			sResult += m_oCGpRule->toXML();
		if (m_oCSp.IsInit())
			sResult += m_oCSp->toXML();
		if (m_oCtrlPr.IsInit())
			sResult += m_oCtrlPr->toXML();
		if (m_oMcs.IsInit())
			sResult += m_oMcs->toXML();
		if (m_oPlcHide.IsInit())
			sResult += m_oPlcHide->toXML();
		if (m_oRSp.IsInit())
			sResult += m_oRSp->toXML();
		if (m_oRSpRule.IsInit())
			sResult += m_oRSpRule->toXML();

		sResult += L"</m:mPr>";

		return sResult;
	}

	std::wstring CAcc::toXML() const
	{
		std::wstring sResult = L"<m:acc>";

		if (  m_oAccPr.IsInit())
			sResult += m_oAccPr->toXML();

		if ( m_oElement.IsInit())
			sResult += m_oElement->toXML();
				
		sResult += L"</m:acc>";

		return sResult;
	}

	std::wstring      CAccPr::toXML() const
	 {
		std::wstring sResult = L"<m:accPr>";
		
		if ( m_oChr.IsInit() )
			sResult += m_oChr->toXML();

		if ( m_oCtrlPr.IsInit() )
			sResult += m_oCtrlPr->toXML();
		
		sResult += L"</m:accPr>";

		return sResult;
	}

	std::wstring      CBar::toXML() const
	{
		std::wstring sResult = L"<m:bar>";
		
		if ( m_oBarPr.IsInit() )
			sResult += m_oBarPr->toXML();

		if ( m_oElement.IsInit() )
			sResult += m_oElement->toXML();
		
		sResult += L"</m:bar>";

		return sResult;
	}

	std::wstring      CBarPr::toXML() const
	{
		std::wstring sResult = L"<m:barPr>";
		
		if ( m_oCtrlPr.IsInit() )
			sResult += m_oCtrlPr->toXML();

		if ( m_oPos.IsInit() )
			sResult += m_oPos->toXML();
		
		sResult += L"</m:barPr>";

		return sResult;
	}

	std::wstring      CBorderBox::toXML() const
	{
		std::wstring sResult = L"<m:borderBox>";
		
		if ( m_oBorderBoxPr.IsInit() )
			sResult += m_oBorderBoxPr->toXML();

		if ( m_oElement.IsInit() )
			sResult += m_oElement->toXML();

		sResult += L"</m:borderBox>";

		return sResult;
	}

	std::wstring      CBorderBoxPr::toXML() const
	{
		std::wstring sResult = L"<m:borderBoxPr>";

		if ( m_oCtrlPr.IsInit() )
			sResult += m_oCtrlPr->toXML();

		if ( m_oHideBot.IsInit() )
			sResult += m_oHideBot->toXML();

		if ( m_oHideLeft.IsInit() )
			sResult += m_oHideLeft->toXML();

		if ( m_oHideRight.IsInit() )
			sResult += m_oHideRight->toXML();

		if ( m_oHideTop.IsInit() )
			sResult += m_oHideTop->toXML();

		if ( m_oStrikeBLTR.IsInit() )
			sResult += m_oStrikeBLTR->toXML();

		if ( m_oStrikeH.IsInit() )
			sResult += m_oStrikeH->toXML();

		if ( m_oStrikeTLBR.IsInit() )
			sResult += m_oStrikeTLBR->toXML();

		if ( m_oStrikeV.IsInit() )
			sResult += m_oStrikeV->toXML();
						
		sResult += L"</m:borderBoxPr>";

		return sResult;
	}

	std::wstring      CBox::toXML() const
	{
		std::wstring sResult = L"<m:box>";

		if ( m_oBoxPr.IsInit() )
			sResult += m_oBoxPr->toXML();

		if ( m_oElement.IsInit() )
			sResult += m_oElement->toXML();
		
		sResult += L"</m:box>";

		return sResult;
	}

	std::wstring      CBoxPr::toXML() const
	{
		std::wstring sResult = L"<m:boxPr>";
		
		if ( m_oAln.IsInit() )
			sResult += m_oAln->toXML();

		if ( m_oBrk.IsInit() )
			sResult += m_oBrk->toXML();

		if ( m_oCtrlPr.IsInit() )
			sResult += m_oCtrlPr->toXML();

		if ( m_oDiff.IsInit() )
			sResult += m_oDiff->toXML();

		if ( m_oNoBreak.IsInit() )
			sResult += m_oNoBreak->toXML();

		if ( m_oOpEmu.IsInit() )
			sResult += m_oOpEmu->toXML();
						
		sResult += L"</m:boxPr>";

		return sResult;
	}
	void CCtrlPr::fromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlUtils::CXmlNodes oChilds;
		if (oNode.GetNodes(L"*", oChilds))
		{
			XmlUtils::CXmlNode oItem;
			for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
			{
				if (oChilds.GetAt(nIndex, oItem))
				{
					std::wstring sName = oItem.GetName();
					WritingElement *pItem = NULL;

					if (L"w:rP" == sName)
						m_oRPr = new OOX::Logic::CRunProperty(oItem);
					else if (L"a:rPr" == sName)
						m_oARPr = new PPTX::Logic::RunProperties(oItem);
					else if (L"w:ins" == sName)
						m_oIns = new OOX::Logic::CRPrChange(oItem);
					else if (L"w:del" == sName)
						m_oDel = new OOX::Logic::CRPrChange(oItem);

				}
			}
		}
	}
	void CCtrlPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"w:rPr" == sName)
				m_oRPr = oReader;
			else if (L"a:rPr" == sName)
			{
				std::wstring sXml = oReader.GetOuterXml();
				XmlUtils::CXmlNode node;
				node.FromXmlString(sXml);

				m_oARPr = node;
			}
			else if (L"w:ins" == sName)
				m_oIns = oReader;
			else if (L"w:del" == sName)
				m_oDel = oReader;
		}
	}
	std::wstring CCtrlPr::toXML() const
	{
		std::wstring sResult = L"<m:ctrlPr>";

		if (m_oRPr.IsInit())
			sResult += m_oRPr->toXML();
		if (m_oARPr.IsInit())
			sResult += m_oARPr->toXML();
		if (m_oIns.IsInit())
			sResult += m_oIns->toXML();
		if (m_oDel.IsInit())
			sResult += m_oDel->toXML();

		sResult += L"</m:ctrlPr>";

		return sResult;
	}
	void CDelimiter::fromXML(XmlUtils::CXmlNode& oNode)
	{
		m_lColumn = 0;

		XmlUtils::CXmlNodes oChilds;
		if (oNode.GetNodes(L"*", oChilds))
		{
			XmlUtils::CXmlNode oItem;
			for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
			{
				if (oChilds.GetAt(nIndex, oItem))
				{
					std::wstring sName = oItem.GetName();
					WritingElement *pItem = NULL;

					if (L"m:dPr" == sName)
						pItem = new CDelimiterPr(oItem);
					else if (L"m:e" == sName)
					{
						pItem = new CElement(oItem);
						//нужно заранее знать колисество столбцов для отрисовки
						m_lColumn++;
					}

					if (pItem)
						m_arrItems.push_back(pItem);
				}
			}
		}
	}
	void CDelimiter::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		m_lColumn = 0;

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();
			WritingElement *pItem = NULL;

			if (L"m:dPr" == sName)
				pItem = new CDelimiterPr(oReader);
			else if (L"m:e" == sName)
			{
				pItem = new CElement(oReader);
				//нужно заранее знать колисество столбцов для отрисовки
				m_lColumn++;
			}

			if (pItem)
				m_arrItems.push_back(pItem);

		}
	}
	std::wstring CDelimiter::toXML() const
	{
		std::wstring sResult = L"<m:d>";

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				sResult += m_arrItems[i]->toXML();
			}
		}

		sResult += L"</m:d>";

		return sResult;
	}

	std::wstring CEqArrPr::toXML() const
	{
		std::wstring sResult = L"<m:eqArrPr>";

		if (m_oBaseJc.IsInit())
			sResult += m_oBaseJc->toXML();

		if (m_oCtrlPr.IsInit())
			sResult += m_oCtrlPr->toXML();

		if (m_oMaxDist.IsInit())
			sResult += m_oMaxDist->toXML();

		if (m_oObjDist.IsInit())
			sResult += m_oObjDist->toXML();

		if (m_oRSp.IsInit())
			sResult += m_oRSp->toXML();

		if (m_oRSpRule.IsInit())
			sResult += m_oRSpRule->toXML();

		sResult += L"</m:eqArrPr>";

		return sResult;
	}

	std::wstring CFraction::toXML() const
	{
		std::wstring sResult = L"<m:f>";

		if ( m_oDen.IsInit() )
			sResult += m_oDen->toXML();

		if ( m_oFPr.IsInit() )
			sResult += m_oFPr->toXML();

		if ( m_oNum.IsInit() )
			sResult += m_oNum->toXML();
		
		sResult += L"</m:f>";

		return sResult;
	}

	std::wstring CFunc::toXML() const
	{
		std::wstring sResult = L"<m:func>";

		if ( m_oElement.IsInit() )
			sResult += m_oElement->toXML();

		if ( m_oFName.IsInit() )
			sResult += m_oFName->toXML();

		if ( m_oFuncPr.IsInit() )
			sResult += m_oFuncPr->toXML();
		
		sResult += L"</m:func>";

		return sResult;
	}

	std::wstring CGroupChr::toXML() const
	{
		std::wstring sResult = L"<m:groupChr>";

		if ( m_oElement.IsInit() )
			sResult += m_oElement->toXML();

		if ( m_oGroupChrPr.IsInit() )
			sResult += m_oGroupChrPr->toXML();
		
		sResult += L"</m:groupChr>";

		return sResult;
	}

	std::wstring CLimLow::toXML() const
	{
		std::wstring sResult = L"<m:limLow>";

		if ( m_oElement.IsInit() )
			sResult += m_oElement->toXML();

		if ( m_oLim.IsInit() )
			sResult += m_oLim->toXML();

		if ( m_oLimLowPr.IsInit() )
			sResult += m_oLimLowPr->toXML();
		
		sResult += L"</m:limLow>";

		return sResult;
	}
	void CLimUpp::fromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlUtils::CXmlNodes oChilds;
		if (oNode.GetNodes(L"*", oChilds))
		{
			XmlUtils::CXmlNode oItem;
			for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
			{
				if (oChilds.GetAt(nIndex, oItem))
				{
					std::wstring sName = oItem.GetName();
					WritingElement *pItem = NULL;

					if (L"m:e" == sName)
						m_oElement = new OOX::Logic::CElement(oItem);
					else if (L"m:lim" == sName)
						m_oLim = new OOX::Logic::CLim(oItem);
					else if (L"m:limUppPr" == sName)
						m_oLimUppPr = new OOX::Logic::CLimUppPr(oItem);
				}
			}
		}
	}
	void CLimUpp::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"m:e" == sName)
				m_oElement = oReader;
			else if (L"m:lim" == sName)
				m_oLim = oReader;
			else if (L"m:limUppPr" == sName)
				m_oLimUppPr = oReader;
		}
	}
	std::wstring CLimUpp::toXML() const
	{
		std::wstring sResult = L"<m:limUpp>";

		if ( m_oElement.IsInit() )
			sResult += m_oElement->toXML();

		if ( m_oLim.IsInit() )
			sResult += m_oLim->toXML();

		if ( m_oLimUppPr.IsInit() )
			sResult += m_oLimUppPr->toXML();
		
		sResult += L"</m:limUpp>";

		return sResult;
	}			

	std::wstring      CMc::toXML() const
	{
		std::wstring sResult = L"<m:mc>";

		if ( m_oMcPr.IsInit() )
			sResult += m_oMcPr->toXML();
		
		sResult += L"</m:mc>";

		return sResult;
	}

	std::wstring      CNary::toXML() const
	{
		std::wstring sResult = L"<m:nary>";

		if ( m_oElement.IsInit() )
			sResult += m_oElement->toXML();

		if ( m_oNaryPr.IsInit() )
			sResult += m_oNaryPr->toXML();

		if ( m_oSub.IsInit() )
			sResult += m_oSub->toXML();

		if ( m_oSup.IsInit() )
			sResult += m_oSup->toXML();
		
		sResult += L"</m:nary>";

		return sResult;
	}

	std::wstring      CPhant::toXML() const
	{
		std::wstring sResult = L"<m:phant>";

		if ( m_oElement.IsInit() )
			sResult += m_oElement->toXML();

		if ( m_oPhantPr.IsInit() )
			sResult += m_oPhantPr->toXML();
		
		sResult += L"</m:phant>";

		return sResult;
	}

	std::wstring      CMRun::toXML() const
	{
		std::wstring sResult = L"<m:r>";

		sResult += toXMLInner();
		
		sResult += L"</m:r>";

		return sResult;
	}
	std::wstring      CMRun::toXMLInner() const
	 {
		 std::wstring sResult;

		 if ( m_oAnnotationRef.IsInit() )
			 sResult += m_oAnnotationRef->toXML();

		 if ( m_oBr.IsInit() )
			 sResult += m_oBr->toXML();

		 if ( m_oCommentReference.IsInit() )
			 sResult += m_oCommentReference->toXML();

		 if ( m_oContentPart.IsInit() )
			 sResult += m_oContentPart->toXML();

		 if ( m_oContinuationSeparator.IsInit() )
			 sResult += m_oContinuationSeparator->toXML();

		 if ( m_oCr.IsInit() )
			 sResult += m_oCr->toXML();

		 if ( m_oDayLong.IsInit() )
			 sResult += m_oDayLong->toXML();

		 if ( m_oDayShort.IsInit() )
			 sResult += m_oDayShort->toXML();

		 if ( m_oDelInstrText.IsInit() )
			 sResult += m_oDelInstrText->toXML();

		 if ( m_oDelText.IsInit() )
			 sResult += m_oDelText->toXML();

		 if ( m_oDrawing.IsInit() )
			 sResult += m_oDrawing->toXML();

		 if ( m_oEndnoteRef.IsInit() )
			 sResult += m_oEndnoteRef->toXML();

		 if ( m_oEndnoteReference.IsInit() )
			 sResult += m_oEndnoteReference->toXML();

		 if ( m_oFldChar.IsInit() )
			 sResult += m_oFldChar->toXML();

		 if ( m_oFootnoteRef.IsInit() )
			 sResult += m_oFootnoteRef->toXML();

		 if ( m_oFootnoteReference.IsInit() )
			 sResult += m_oFootnoteReference->toXML();

		 if ( m_oInstrText.IsInit() )
			 sResult += m_oInstrText->toXML();

		 if ( m_oLastRenderedPageBreak.IsInit() )
			 sResult += m_oLastRenderedPageBreak->toXML();

		 if ( m_oMonthLong.IsInit() )
			 sResult += m_oMonthLong->toXML();

		 if ( m_oMonthShort.IsInit() )
			 sResult += m_oMonthShort->toXML();

		 if ( m_oNoBreakHyphen.IsInit() )
			 sResult += m_oNoBreakHyphen->toXML();

		 if ( m_oObject.IsInit() )
			 sResult += m_oObject->toXML();

		 if ( m_oPgNum.IsInit() )
			 sResult += m_oPgNum->toXML();

		 if ( m_oPtab.IsInit() )
			 sResult += m_oPtab->toXML();

		 if ( m_oMRPr.IsInit() )
			 sResult += m_oMRPr->toXML();

		 if ( m_oARPr.IsInit() )
			 sResult += m_oARPr->toXML();

		 if ( m_oRPr.IsInit() )
			 sResult += m_oRPr->toXML();

		 if ( m_oRuby.IsInit() )
			 sResult += m_oRuby->toXML();

		 if ( m_oSeparator.IsInit() )
			 sResult += m_oSeparator->toXML();

		 if ( m_oSoftHyphen.IsInit() )
			 sResult += m_oSoftHyphen->toXML();

		 if ( m_oSym.IsInit() )
			 sResult += m_oSym->toXML();

		 if ( m_oMText.IsInit() )
			 sResult += m_oMText->toXML();

		 if ( m_oText.IsInit() )
			 sResult += m_oText->toXML();

		 if ( m_oTab.IsInit() )
			 sResult += m_oTab->toXML();

		 if ( m_oYearLong.IsInit() )
			 sResult += m_oYearLong->toXML();

		 if ( m_oYearShort.IsInit() )
			 sResult += m_oYearShort->toXML();

		 return sResult;
	 }
	}
}
