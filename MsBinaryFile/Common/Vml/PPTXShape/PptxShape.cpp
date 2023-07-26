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
#pragma once

#include "PptxShape.h"
#include "PresetShapesHeader.h"

CPPTXShape::CPPTXShape() : CBaseShape(), FManager(m_arAdjustments, m_arGuides)
{
	m_eType = OOXMLShapes::sptMin;
}
CPPTXShape::~CPPTXShape()
{
}
bool CPPTXShape::LoadFromXML(const std::wstring& xml)
{
	XmlUtils::CXmlNode oNodePict;
	if (oNodePict.FromXmlString(xml))
	{
		return LoadFromXML(oNodePict);
	}
	return false;
}
bool CPPTXShape::LoadFromXML(XmlUtils::CXmlNode& root)
{
	//std::wstring tempXML = _T("<ooxml-shape>") + xml + _T("</ooxml-shape>");

	XmlUtils::CXmlNode avLst;
	bool res = true;
	if(root.GetNode(_T("avLst"), avLst))
		res &= LoadAdjustValuesList(avLst.GetXml());

	XmlUtils::CXmlNode gdLst;
	if(root.GetNode(_T("gdLst"), gdLst))
		res &= LoadGuidesList(gdLst.GetXml());

	XmlUtils::CXmlNode ahLst;
	if(root.GetNode(_T("ahLst"), ahLst))
		res &= LoadAdjustHandlesList(ahLst.GetXml());

	XmlUtils::CXmlNode cxnLst;
	if(root.GetNode(_T("cxnLst"), cxnLst))
		res &= LoadConnectorsList(cxnLst.GetXml());

	XmlUtils::CXmlNode textRect;
	if(root.GetNode(_T("rect"), textRect))
	{
		m_strRect = textRect.GetXml();
		res &= LoadTextRect(m_strRect);
	}

	XmlUtils::CXmlNode pathLst;
	if(root.GetNode(_T("pathLst"), pathLst))
	{
		m_strPath = pathLst.GetXml();
		res &= LoadPathList(m_strPath);
	}

	return res;
}
bool CPPTXShape::LoadAdjustValuesList(const std::wstring& xml)
{
	XmlUtils::CXmlNode avLst;
	if(avLst.FromXmlString(xml))
	{
		std::vector<XmlUtils::CXmlNode> list;
		if(avLst.GetNodes(_T("gd"), list))
		{
			for(size_t i = 0; i < list.size(); i++)
			{
				XmlUtils::CXmlNode & gd = list[i];
				FManager.AddAdjustment(gd.GetAttribute(_T("name")), gd.GetAttribute(_T("fmla")));
			}
		}
		return true;
	}
	return false;
}
bool CPPTXShape::LoadGuidesList(const std::wstring& xml)
{
	XmlUtils::CXmlNode gdLst;
	if(gdLst.FromXmlString(xml))
	{
		std::vector<XmlUtils::CXmlNode> list;
		if(gdLst.GetNodes(_T("gd"), list))
		{
			for(size_t i = 0; i < list.size(); i++)
			{
				XmlUtils::CXmlNode & gd = list[i];
				FManager.AddGuide(gd.GetAttribute(_T("name")), gd.GetAttribute(_T("fmla")));
			}
		}
		return true;
	}
	return false;
}
bool CPPTXShape::LoadAdjustHandlesList(const std::wstring& xml)
{
	/*XmlUtils::CXmlNode ahLst;
			if(ahLst.FromXmlString(xml))
			{
					std::vector<XmlUtils::CXmlNode> oNodes;
					if (ahLst.GetNodes(_T("ahXY"), oNodes))
					{
							size_t nCount = oNodes.size();
							for (int i = 0; i < nCount; ++i)
							{
									XmlUtils::CXmlNode oNodeH;
									oNodes.GetAt(i, oNodeH);

									CHandle_ oH;
									oH.switchHandle = oNodeH.GetAttribute(_T("switch"));
									oH.xrange = oNodeH.GetAttribute(_T("minX")) + _T(" ") + oNodeH.GetAttribute(_T("maxX"));
									oH.yrange = oNodeH.GetAttribute(_T("minY")) + _T(" ") + oNodeH.GetAttribute(_T("maxY"));

									std::vector<XmlUtils::CXmlNode> oPosNodes;
									if (oNodeH.GetNodes(_T("pos"), oPosNodes))
									{
											int nCountPos = oPosNodes.GetCount();
											XmlUtils::CXmlNode oPosNode;
											oPosNodes.GetAt(0, oPosNode);

											std::wstring strX = oPosNode.GetAttribute(_T("x"));
											std::wstring strY = oPosNode.GetAttribute(_T("y"));

											if (strX == _T("l"))
													oH.position += _T("topLeft");
											else if (strX == _T("r"))
													oH.position += _T("bottomRight");
											else
													oH.position += oNodeH.GetAttribute(_T("gdRefX"));

											if (strY == _T("t"))
													oH.position += _T("topLeft");
											else if (strY == _T("b"))
													oH.position += _T("bottomRight");
											else
													oH.position += _T(" ") + oNodeH.GetAttribute(_T("gdRefY"));
									}
									//TODO разобраться с полярными хендлами
									//поля:
									//polar = 10800, 10800
									//position = gdrefR, gdrefAng (adj)
									//radiusrange = minR, maxR

									m_arHandles.push_back(oH);
							}
					}
					return true;
			}
			return false;*/
	return true;
}
bool CPPTXShape::LoadConnectorsList(const std::wstring& xml)
{
	return true;
}
bool CPPTXShape::LoadTextRect(const std::wstring& xml)
{
	XmlUtils::CXmlNode rect;
	if(rect.FromXmlString(xml))
	{
		Aggplus::RECT TextRect;
		TextRect.top = (long)FManager.GetValue(rect.GetAttribute(_T("t")));
		TextRect.left = (long)FManager.GetValue(rect.GetAttribute(_T("l")));
		TextRect.right = (long)FManager.GetValue(rect.GetAttribute(_T("r")));
		TextRect.bottom = (long)FManager.GetValue(rect.GetAttribute(_T("b")));
		if(m_arTextRects.size() > 0)
			m_arTextRects[0] = TextRect;
		else m_arTextRects.push_back(TextRect);
		return true;
	}
	return false;
}
bool CPPTXShape::LoadPathList(const std::wstring& xml)
{
	XmlUtils::CXmlNode pathLst;
	if(pathLst.FromXmlString(xml))
	{
		std::vector<XmlUtils::CXmlNode> list;
		if(pathLst.GetNodes(_T("path"), list))
		{
			m_oPath.FromXML(list, FManager);
		}
		return true;
	}
	return false;
}
bool CPPTXShape::SetAdjustment(long index, long value)
{
	FManager.Clear();
	if (index < (long)m_arAdjustments.size() && index >=0)
	{
		m_arAdjustments[index] = value;
		return TRUE;
	}
	return FALSE;
}
std::wstring CPPTXShape::ToXML(CGeomShapeInfo& GeomInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen)
{
	FManager.SetWidthHeight(GeomInfo.m_dWidth, GeomInfo.m_dHeight);
	ReCalculate();

	return m_oPath.ToXml(GeomInfo, StartTime, EndTime, Pen, Brush, NSBaseShape::pptx);
}
void CPPTXShape::ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore)
{
	FManager.SetWidthHeight(pGeomInfo.m_dWidth, pGeomInfo.m_dHeight);
	ReCalculate();

	m_oPath.ToRenderer(pRenderer, pGeomInfo, dStartTime, dEndTime, pPen, pFore, GetClassType());
}
void CPPTXShape::ReCalculate()
{
	FManager.ReCalculateGuides();
	LoadTextRect(m_strRect);
	LoadPathList(m_strPath);
}
const ClassType CPPTXShape::GetClassType()const
{
	return NSBaseShape::pptx;
}
bool CPPTXShape::SetProperties(CBaseShape* Shape)
{
	if(Shape == NULL)
		return false;

	if(Shape->GetClassType() != NSBaseShape::pptx)
		return false;

	FManager = ((CPPTXShape*)Shape)->FManager;
	return CBaseShape::SetProperties(Shape);
}
bool CPPTXShape::SetToDublicate(CBaseShape* Shape)
{
	if(Shape == NULL)
		return false;

	if(Shape->GetClassType() != NSBaseShape::pptx)
		return false;

	((CPPTXShape*)Shape)->FManager = FManager;
	return CBaseShape::SetToDublicate(Shape);
}
bool CPPTXShape::SetShapeType(OOXMLShapes::ShapeType type)
{
	CBaseShapePtr pShape = CreateByType(type);
	if(pShape)
	{
		m_eType = type;

		SetProperties(pShape.get());
		return true;
	}

	m_eType = OOXMLShapes::sptCustom;
	return false;
}
void CPPTXShape::SetWidthHeightLogic(const double& dWidth, const double& dHeight)
{
	FManager.SetWidthHeight(dWidth, dHeight);
}
void CPPTXShape::GetWidthHeightLogic(double& dWidth, double& dHeight)
{
	dWidth = FManager.GetWidth();
	dHeight = FManager.GetHeight();
}
CBaseShapePtr CPPTXShape::CreateByType(OOXMLShapes::ShapeType type)
{
	CBaseShapePtr shape;
    switch(type)
	{
	case OOXMLShapes::sptCAccentBorderCallout1:		return CBaseShapePtr(new OOXMLShapes::CAccentBorderCallout1());
	case OOXMLShapes::sptCAccentBorderCallout2:		return CBaseShapePtr(new OOXMLShapes::CAccentBorderCallout2());
	case OOXMLShapes::sptCAccentBorderCallout3:		return CBaseShapePtr(new OOXMLShapes::CAccentBorderCallout3());
	case OOXMLShapes::sptCAccentCallout1:			return CBaseShapePtr(new OOXMLShapes::CAccentCallout1());
	case OOXMLShapes::sptCAccentCallout2:			return CBaseShapePtr(new OOXMLShapes::CAccentCallout2());
	case OOXMLShapes::sptCAccentCallout3:			return CBaseShapePtr(new OOXMLShapes::CAccentCallout3());
	case OOXMLShapes::sptCActionButtonBackPrevious:	return CBaseShapePtr(new OOXMLShapes::CActionButtonBackPrevious());
	case OOXMLShapes::sptCActionButtonBeginning:	return CBaseShapePtr(new OOXMLShapes::CActionButtonBeginning());
	case OOXMLShapes::sptCActionButtonBlank:		return CBaseShapePtr(new OOXMLShapes::CActionButtonBlank());
	case OOXMLShapes::sptCActionButtonDocument:		return CBaseShapePtr(new OOXMLShapes::CActionButtonDocument());
	case OOXMLShapes::sptCActionButtonEnd:			return CBaseShapePtr(new OOXMLShapes::CActionButtonEnd());
	case OOXMLShapes::sptCActionButtonForwardNext:	return CBaseShapePtr(new OOXMLShapes::CActionButtonForwardNext());
	case OOXMLShapes::sptCActionButtonHelp:			return CBaseShapePtr(new OOXMLShapes::CActionButtonHelp());
	case OOXMLShapes::sptCActionButtonHome:			return CBaseShapePtr(new OOXMLShapes::CActionButtonHome());
	case OOXMLShapes::sptCActionButtonInformation:	return CBaseShapePtr(new OOXMLShapes::CActionButtonInformation());
	case OOXMLShapes::sptCActionButtonMovie:		return CBaseShapePtr(new OOXMLShapes::CActionButtonMovie());
	case OOXMLShapes::sptCActionButtonReturn:		return CBaseShapePtr(new OOXMLShapes::CActionButtonReturn());
	case OOXMLShapes::sptCActionButtonSound:		return CBaseShapePtr(new OOXMLShapes::CActionButtonSound());
	case OOXMLShapes::sptCArc:						return CBaseShapePtr(new OOXMLShapes::CArc());
	case OOXMLShapes::sptCBentArrow:				return CBaseShapePtr(new OOXMLShapes::CBentArrow());
	case OOXMLShapes::sptCBentConnector2:			return CBaseShapePtr(new OOXMLShapes::CBentConnector2());
	case OOXMLShapes::sptCBentConnector3:			return CBaseShapePtr(new OOXMLShapes::CBentConnector3());
	case OOXMLShapes::sptCBentConnector4:			return CBaseShapePtr(new OOXMLShapes::CBentConnector4());
	case OOXMLShapes::sptCBentConnector5:			return CBaseShapePtr(new OOXMLShapes::CBentConnector5());
	case OOXMLShapes::sptCBentUpArrow:				return CBaseShapePtr(new OOXMLShapes::CBentUpArrow());
	case OOXMLShapes::sptCBevel:					return CBaseShapePtr(new OOXMLShapes::CBevel());
	case OOXMLShapes::sptCBlockArc:					return CBaseShapePtr(new OOXMLShapes::CBlockArc());
	case OOXMLShapes::sptCBorderCallout1:			return CBaseShapePtr(new OOXMLShapes::CBorderCallout1());
	case OOXMLShapes::sptCBorderCallout2:			return CBaseShapePtr(new OOXMLShapes::CBorderCallout2());
	case OOXMLShapes::sptCBorderCallout3:			return CBaseShapePtr(new OOXMLShapes::CBorderCallout3());
	case OOXMLShapes::sptCBracePair:				return CBaseShapePtr(new OOXMLShapes::CBracePair());
	case OOXMLShapes::sptCBracketPair:				return CBaseShapePtr(new OOXMLShapes::CBracketPair());
	case OOXMLShapes::sptCCallout1:					return CBaseShapePtr(new OOXMLShapes::CCallout1());
	case OOXMLShapes::sptCCallout2:					return CBaseShapePtr(new OOXMLShapes::CCallout2());
	case OOXMLShapes::sptCCallout3:					return CBaseShapePtr(new OOXMLShapes::CCallout3());
	case OOXMLShapes::sptCCan:						return CBaseShapePtr(new OOXMLShapes::CCan());
	case OOXMLShapes::sptCChartPlus:				return CBaseShapePtr(new OOXMLShapes::CChartPlus());
	case OOXMLShapes::sptCChartStar:				return CBaseShapePtr(new OOXMLShapes::CChartStar());
	case OOXMLShapes::sptCChartX:					return CBaseShapePtr(new OOXMLShapes::CChartX());
	case OOXMLShapes::sptCChevron:					return CBaseShapePtr(new OOXMLShapes::CChevron());
	case OOXMLShapes::sptCChord:					return CBaseShapePtr(new OOXMLShapes::CChord());
	case OOXMLShapes::sptCCircularArrow:			return CBaseShapePtr(new OOXMLShapes::CCircularArrow());
	case OOXMLShapes::sptCCloud:					return CBaseShapePtr(new OOXMLShapes::CCloud());
	case OOXMLShapes::sptCCloudCallout:				return CBaseShapePtr(new OOXMLShapes::CCloudCallout());
	case OOXMLShapes::sptCCorner:					return CBaseShapePtr(new OOXMLShapes::CCorner());
	case OOXMLShapes::sptCCornerTabs:				return CBaseShapePtr(new OOXMLShapes::CCornerTabs());
	case OOXMLShapes::sptCCube:						return CBaseShapePtr(new OOXMLShapes::CCube());
	case OOXMLShapes::sptCCurvedConnector2:			return CBaseShapePtr(new OOXMLShapes::CCurvedConnector2());
	case OOXMLShapes::sptCCurvedConnector3:			return CBaseShapePtr(new OOXMLShapes::CCurvedConnector3());
	case OOXMLShapes::sptCCurvedConnector4:			return CBaseShapePtr(new OOXMLShapes::CCurvedConnector4());
	case OOXMLShapes::sptCCurvedConnector5:			return CBaseShapePtr(new OOXMLShapes::CCurvedConnector5());
	case OOXMLShapes::sptCCurvedDownArrow:			return CBaseShapePtr(new OOXMLShapes::CCurvedDownArrow());
	case OOXMLShapes::sptCCurvedLeftArrow:			return CBaseShapePtr(new OOXMLShapes::CCurvedLeftArrow());
	case OOXMLShapes::sptCCurvedRightArrow:			return CBaseShapePtr(new OOXMLShapes::CCurvedRightArrow());
	case OOXMLShapes::sptCCurvedUpArrow:			return CBaseShapePtr(new OOXMLShapes::CCurvedUpArrow());
	case OOXMLShapes::sptCDecagon:					return CBaseShapePtr(new OOXMLShapes::CDecagon());
	case OOXMLShapes::sptCDiagStripe:				return CBaseShapePtr(new OOXMLShapes::CDiagStripe());
	case OOXMLShapes::sptCDiamond:					return CBaseShapePtr(new OOXMLShapes::CDiamond());
	case OOXMLShapes::sptCDodecagon:				return CBaseShapePtr(new OOXMLShapes::CDodecagon());
	case OOXMLShapes::sptCDonut:					return CBaseShapePtr(new OOXMLShapes::CDonut());
	case OOXMLShapes::sptCDoubleWave:				return CBaseShapePtr(new OOXMLShapes::CDoubleWave());
	case OOXMLShapes::sptCDownArrow:				return CBaseShapePtr(new OOXMLShapes::CDownArrow());
	case OOXMLShapes::sptCDownArrowCallout:			return CBaseShapePtr(new OOXMLShapes::CDownArrowCallout());
	//case OOXMLShapes::sptCEllipse:					return CBaseShapePtr(new OOXMLShapes::CEllipse());
	case OOXMLShapes::sptCEllipseRibbon:			return CBaseShapePtr(new OOXMLShapes::CEllipseRibbon());
	case OOXMLShapes::sptCEllipseRibbon2:			return CBaseShapePtr(new OOXMLShapes::CEllipseRibbon2());
	case OOXMLShapes::sptCFlowChartAlternateProcess:return CBaseShapePtr(new OOXMLShapes::CFlowChartAlternateProcess());
	case OOXMLShapes::sptCFlowChartCollate:			return CBaseShapePtr(new OOXMLShapes::CFlowChartCollate());
	case OOXMLShapes::sptCFlowChartConnector:		return CBaseShapePtr(new OOXMLShapes::CFlowChartConnector());
	case OOXMLShapes::sptCFlowChartDecision:		return CBaseShapePtr(new OOXMLShapes::CFlowChartDecision());
	case OOXMLShapes::sptCFlowChartDelay:			return CBaseShapePtr(new OOXMLShapes::CFlowChartDelay());
	case OOXMLShapes::sptCFlowChartDisplay:			return CBaseShapePtr(new OOXMLShapes::CFlowChartDisplay());
	case OOXMLShapes::sptCFlowChartDocument:		return CBaseShapePtr(new OOXMLShapes::CFlowChartDocument());
	case OOXMLShapes::sptCFlowChartExtract:			return CBaseShapePtr(new OOXMLShapes::CFlowChartExtract());
	case OOXMLShapes::sptCFlowChartInputOutput:		return CBaseShapePtr(new OOXMLShapes::CFlowChartInputOutput());
	case OOXMLShapes::sptCFlowChartInternalStorage: return CBaseShapePtr(new OOXMLShapes::CFlowChartInternalStorage());
	case OOXMLShapes::sptCFlowChartMagneticDisk:	return CBaseShapePtr(new OOXMLShapes::CFlowChartMagneticDisk());
	case OOXMLShapes::sptCFlowChartMagneticDrum:	return CBaseShapePtr(new OOXMLShapes::CFlowChartMagneticDrum());
	case OOXMLShapes::sptCFlowChartMagneticTape:	return CBaseShapePtr(new OOXMLShapes::CFlowChartMagneticTape());
	case OOXMLShapes::sptCFlowChartManualInput:		return CBaseShapePtr(new OOXMLShapes::CFlowChartManualInput());
	case OOXMLShapes::sptCFlowChartManualOperation: return CBaseShapePtr(new OOXMLShapes::CFlowChartManualOperation());
	case OOXMLShapes::sptCFlowChartMerge:			return CBaseShapePtr(new OOXMLShapes::CFlowChartMerge());
	case OOXMLShapes::sptCFlowChartMultidocument:	return CBaseShapePtr(new OOXMLShapes::CFlowChartMultidocument());
	case OOXMLShapes::sptCFlowChartOfflineStorage:	return CBaseShapePtr(new OOXMLShapes::CFlowChartOfflineStorage());
	case OOXMLShapes::sptCFlowChartOffpageConnector: return CBaseShapePtr(new OOXMLShapes::CFlowChartOffpageConnector());
	case OOXMLShapes::sptCFlowChartOnlineStorage:	return CBaseShapePtr(new OOXMLShapes::CFlowChartOnlineStorage());
	case OOXMLShapes::sptCFlowChartOr:				return CBaseShapePtr(new OOXMLShapes::CFlowChartOr());
	case OOXMLShapes::sptCFlowChartPredefinedProcess: return CBaseShapePtr(new OOXMLShapes::CFlowChartPredefinedProcess());
	case OOXMLShapes::sptCFlowChartPreparation:		return CBaseShapePtr(new OOXMLShapes::CFlowChartPreparation());
	case OOXMLShapes::sptCFlowChartProcess:			return CBaseShapePtr(new OOXMLShapes::CFlowChartProcess());
	case OOXMLShapes::sptCFlowChartPunchedCard:		return CBaseShapePtr(new OOXMLShapes::CFlowChartPunchedCard());
	case OOXMLShapes::sptCFlowChartPunchedTape:		return CBaseShapePtr(new OOXMLShapes::CFlowChartPunchedTape());
	case OOXMLShapes::sptCFlowChartSort:			return CBaseShapePtr(new OOXMLShapes::CFlowChartSort());
	case OOXMLShapes::sptCFlowChartSummingJunction: return CBaseShapePtr(new OOXMLShapes::CFlowChartSummingJunction());
	case OOXMLShapes::sptCFlowChartTerminator:		return CBaseShapePtr(new OOXMLShapes::CFlowChartTerminator());
	case OOXMLShapes::sptCFoldedCorner:				return CBaseShapePtr(new OOXMLShapes::CFoldedCorner());
	case OOXMLShapes::sptCFrame:					return CBaseShapePtr(new OOXMLShapes::CFrame());
	case OOXMLShapes::sptCFunnel:					return CBaseShapePtr(new OOXMLShapes::CFunnel());
	case OOXMLShapes::sptCGear6:					return CBaseShapePtr(new OOXMLShapes::CGear6());
	case OOXMLShapes::sptCGear9:					return CBaseShapePtr(new OOXMLShapes::CGear9());
	case OOXMLShapes::sptCHalfFrame:				return CBaseShapePtr(new OOXMLShapes::CHalfFrame());
	case OOXMLShapes::sptCHeart:					return CBaseShapePtr(new OOXMLShapes::CHeart());
	case OOXMLShapes::sptCHeptagon:					return CBaseShapePtr(new OOXMLShapes::CHeptagon());
	case OOXMLShapes::sptCHexagon:					return CBaseShapePtr(new OOXMLShapes::CHexagon());
	case OOXMLShapes::sptCHomePlate:				return CBaseShapePtr(new OOXMLShapes::CHomePlate());
	case OOXMLShapes::sptCHorizontalScroll:			return CBaseShapePtr(new OOXMLShapes::CHorizontalScroll());
	case OOXMLShapes::sptCIrregularSeal1:			return CBaseShapePtr(new OOXMLShapes::CIrregularSeal1());
	case OOXMLShapes::sptCIrregularSeal2:			return CBaseShapePtr(new OOXMLShapes::CIrregularSeal2());
	case OOXMLShapes::sptCLeftArrow:				return CBaseShapePtr(new OOXMLShapes::CLeftArrow());
	case OOXMLShapes::sptCLeftArrowCallout:			return CBaseShapePtr(new OOXMLShapes::CLeftArrowCallout());
	case OOXMLShapes::sptCLeftBrace:				return CBaseShapePtr(new OOXMLShapes::CLeftBrace());
	case OOXMLShapes::sptCLeftBracket:				return CBaseShapePtr(new OOXMLShapes::CLeftBracket());
	case OOXMLShapes::sptCLeftCircularArrow:		return CBaseShapePtr(new OOXMLShapes::CLeftCircularArrow());
	case OOXMLShapes::sptCLeftRightArrow:			return CBaseShapePtr(new OOXMLShapes::CLeftRightArrow());
	case OOXMLShapes::sptCLeftRightArrowCallout:	return CBaseShapePtr(new OOXMLShapes::CLeftRightArrowCallout());
	case OOXMLShapes::sptCLeftRightCircularArrow:	return CBaseShapePtr(new OOXMLShapes::CLeftRightCircularArrow());
	case OOXMLShapes::sptCLeftRightRibbon:			return CBaseShapePtr(new OOXMLShapes::CLeftRightRibbon());
	case OOXMLShapes::sptCLeftRightUpArrow:			return CBaseShapePtr(new OOXMLShapes::CLeftRightUpArrow());
	case OOXMLShapes::sptCLeftUpArrow:				return CBaseShapePtr(new OOXMLShapes::CLeftUpArrow());
	case OOXMLShapes::sptCLightningBolt:			return CBaseShapePtr(new OOXMLShapes::CLightningBolt());
	//case OOXMLShapes::sptCLine:						return CBaseShapePtr(new OOXMLShapes::CLine());
	case OOXMLShapes::sptCLineInv:					return CBaseShapePtr(new OOXMLShapes::CLineInv());
	case OOXMLShapes::sptCMathDivide:				return CBaseShapePtr(new OOXMLShapes::CMathDivide());
	case OOXMLShapes::sptCMathEqual:				return CBaseShapePtr(new OOXMLShapes::CMathEqual());
	case OOXMLShapes::sptCMathMinus:				return CBaseShapePtr(new OOXMLShapes::CMathMinus());
	case OOXMLShapes::sptCMathMultiply:				return CBaseShapePtr(new OOXMLShapes::CMathMultiply());
	case OOXMLShapes::sptCMathNotEqual:				return CBaseShapePtr(new OOXMLShapes::CMathNotEqual());
	case OOXMLShapes::sptCMathPlus:					return CBaseShapePtr(new OOXMLShapes::CMathPlus());
	case OOXMLShapes::sptCMoon:						return CBaseShapePtr(new OOXMLShapes::CMoon());
	case OOXMLShapes::sptCNonIsoscelesTrapezoid:	return CBaseShapePtr(new OOXMLShapes::CNonIsoscelesTrapezoid());
	case OOXMLShapes::sptCNoSmoking:				return CBaseShapePtr(new OOXMLShapes::CNoSmoking());
	case OOXMLShapes::sptCNotchedRightArrow:		return CBaseShapePtr(new OOXMLShapes::CNotchedRightArrow());
	case OOXMLShapes::sptCOctagon:					return CBaseShapePtr(new OOXMLShapes::COctagon());
	case OOXMLShapes::sptCParallelogram:			return CBaseShapePtr(new OOXMLShapes::CParallelogram());
	case OOXMLShapes::sptCPentagon:					return CBaseShapePtr(new OOXMLShapes::CPentagon());
	case OOXMLShapes::sptCPie:						return CBaseShapePtr(new OOXMLShapes::CPie());
	case OOXMLShapes::sptCPieWedge:					return CBaseShapePtr(new OOXMLShapes::CPieWedge());
	case OOXMLShapes::sptCPlaque:					return CBaseShapePtr(new OOXMLShapes::CPlaque());
	case OOXMLShapes::sptCPlaqueTabs:				return CBaseShapePtr(new OOXMLShapes::CPlaqueTabs());
	case OOXMLShapes::sptCPlus:						return CBaseShapePtr(new OOXMLShapes::CPlus());
	case OOXMLShapes::sptCQuadArrow:				return CBaseShapePtr(new OOXMLShapes::CQuadArrow());
	case OOXMLShapes::sptCQuadArrowCallout:			return CBaseShapePtr(new OOXMLShapes::CQuadArrowCallout());
	//case OOXMLShapes::sptCRect:						return CBaseShapePtr(new OOXMLShapes::CRect());
	case OOXMLShapes::sptCRibbon:					return CBaseShapePtr(new OOXMLShapes::CRibbon());
	case OOXMLShapes::sptCRibbon2:					return CBaseShapePtr(new OOXMLShapes::CRibbon2());
	case OOXMLShapes::sptCRightArrow:				return CBaseShapePtr(new OOXMLShapes::CRightArrow());
	case OOXMLShapes::sptCRightArrowCallout:		return CBaseShapePtr(new OOXMLShapes::CRightArrowCallout());
	case OOXMLShapes::sptCRightBrace:				return CBaseShapePtr(new OOXMLShapes::CRightBrace());
	case OOXMLShapes::sptCRightBracket:				return CBaseShapePtr(new OOXMLShapes::CRightBracket());
	case OOXMLShapes::sptCRound1Rect:				return CBaseShapePtr(new OOXMLShapes::CRound1Rect());
	case OOXMLShapes::sptCRound2DiagRect:			return CBaseShapePtr(new OOXMLShapes::CRound2DiagRect());
	case OOXMLShapes::sptCRound2SameRect:			return CBaseShapePtr(new OOXMLShapes::CRound2SameRect());
	case OOXMLShapes::sptCRoundRect:				return CBaseShapePtr(new OOXMLShapes::CRoundRect());
	case OOXMLShapes::sptCRtTriangle:				return CBaseShapePtr(new OOXMLShapes::CRtTriangle());
	case OOXMLShapes::sptCSmileyFace:				return CBaseShapePtr(new OOXMLShapes::CSmileyFace());
	case OOXMLShapes::sptCSnip1Rect:				return CBaseShapePtr(new OOXMLShapes::CSnip1Rect());
	case OOXMLShapes::sptCSnip2DiagRect:			return CBaseShapePtr(new OOXMLShapes::CSnip2DiagRect());
	case OOXMLShapes::sptCSnip2SameRect:			return CBaseShapePtr(new OOXMLShapes::CSnip2SameRect());
	case OOXMLShapes::sptCSnipRoundRect:			return CBaseShapePtr(new OOXMLShapes::CSnipRoundRect());
	case OOXMLShapes::sptCSquareTabs:				return CBaseShapePtr(new OOXMLShapes::CSquareTabs());
	case OOXMLShapes::sptCStar10:					return CBaseShapePtr(new OOXMLShapes::CStar10());
	case OOXMLShapes::sptCStar12:					return CBaseShapePtr(new OOXMLShapes::CStar12());
	case OOXMLShapes::sptCStar16:					return CBaseShapePtr(new OOXMLShapes::CStar16());
	case OOXMLShapes::sptCStar24:					return CBaseShapePtr(new OOXMLShapes::CStar24());
	case OOXMLShapes::sptCStar32:					return CBaseShapePtr(new OOXMLShapes::CStar32());
	case OOXMLShapes::sptCStar4:					return CBaseShapePtr(new OOXMLShapes::CStar4());
	case OOXMLShapes::sptCStar5:					return CBaseShapePtr(new OOXMLShapes::CStar5());
	case OOXMLShapes::sptCStar6:					return CBaseShapePtr(new OOXMLShapes::CStar6());
	case OOXMLShapes::sptCStar7:					return CBaseShapePtr(new OOXMLShapes::CStar7());
	case OOXMLShapes::sptCStar8:					return CBaseShapePtr(new OOXMLShapes::CStar8());
	case OOXMLShapes::sptCStraightConnector1:		return CBaseShapePtr(new OOXMLShapes::CStraightConnector1());
	case OOXMLShapes::sptCStripedRightArrow:		return CBaseShapePtr(new OOXMLShapes::CStripedRightArrow());
	case OOXMLShapes::sptCSun:						return CBaseShapePtr(new OOXMLShapes::CSun());
	case OOXMLShapes::sptCSwooshArrow:				return CBaseShapePtr(new OOXMLShapes::CSwooshArrow());
	case OOXMLShapes::sptCTeardrop:					return CBaseShapePtr(new OOXMLShapes::CTeardrop());
	case OOXMLShapes::sptCTextArchDown:				return CBaseShapePtr(new OOXMLShapes::CTextArchDown());
	case OOXMLShapes::sptCTextArchDownPour:			return CBaseShapePtr(new OOXMLShapes::CTextArchDownPour());
	case OOXMLShapes::sptCTextArchUp:				return CBaseShapePtr(new OOXMLShapes::CTextArchUp());
	case OOXMLShapes::sptCTextArchUpPour:			return CBaseShapePtr(new OOXMLShapes::CTextArchUpPour());
	case OOXMLShapes::sptCTextButton:				return CBaseShapePtr(new OOXMLShapes::CTextButton());
	case OOXMLShapes::sptCTextButtonPour:			return CBaseShapePtr(new OOXMLShapes::CTextButtonPour());
	case OOXMLShapes::sptCTextCanDown:				return CBaseShapePtr(new OOXMLShapes::CTextCanDown());
	case OOXMLShapes::sptCTextCanUp:				return CBaseShapePtr(new OOXMLShapes::CTextCanUp());
	case OOXMLShapes::sptCTextCascadeDown:			return CBaseShapePtr(new OOXMLShapes::CTextCascadeDown());
	case OOXMLShapes::sptCTextCascadeUp:			return CBaseShapePtr(new OOXMLShapes::CTextCascadeUp());
	case OOXMLShapes::sptCTextChevron:				return CBaseShapePtr(new OOXMLShapes::CTextChevron());
	case OOXMLShapes::sptCTextChevronInverted:		return CBaseShapePtr(new OOXMLShapes::CTextChevronInverted());
	case OOXMLShapes::sptCTextCircle:				return CBaseShapePtr(new OOXMLShapes::CTextCircle());
	case OOXMLShapes::sptCTextCirclePour:			return CBaseShapePtr(new OOXMLShapes::CTextCirclePour());
	case OOXMLShapes::sptCTextCurveDown:			return CBaseShapePtr(new OOXMLShapes::CTextCurveDown());
	case OOXMLShapes::sptCTextCurveUp:				return CBaseShapePtr(new OOXMLShapes::CTextCurveUp());
	case OOXMLShapes::sptCTextDeflate:				return CBaseShapePtr(new OOXMLShapes::CTextDeflate());
	case OOXMLShapes::sptCTextDeflateBottom:		return CBaseShapePtr(new OOXMLShapes::CTextDeflateBottom());
	case OOXMLShapes::sptCTextDeflateInflate:		return CBaseShapePtr(new OOXMLShapes::CTextDeflateInflate());
	case OOXMLShapes::sptCTextDeflateInflateDeflate:return CBaseShapePtr(new OOXMLShapes::CTextDeflateInflateDeflate());
	case OOXMLShapes::sptCTextDeflateTop:			return CBaseShapePtr(new OOXMLShapes::CTextDeflateTop());
	case OOXMLShapes::sptCTextDoubleWave1:			return CBaseShapePtr(new OOXMLShapes::CTextDoubleWave1());
	case OOXMLShapes::sptCTextFadeDown:				return CBaseShapePtr(new OOXMLShapes::CTextFadeDown());
	case OOXMLShapes::sptCTextFadeLeft:				return CBaseShapePtr(new OOXMLShapes::CTextFadeLeft());
	case OOXMLShapes::sptCTextFadeRight:			return CBaseShapePtr(new OOXMLShapes::CTextFadeRight());
	case OOXMLShapes::sptCTextFadeUp:				return CBaseShapePtr(new OOXMLShapes::CTextFadeUp());
	case OOXMLShapes::sptCTextInflate:				return CBaseShapePtr(new OOXMLShapes::CTextInflate());
	case OOXMLShapes::sptCTextInflateBottom:		return CBaseShapePtr(new OOXMLShapes::CTextInflateBottom());
	case OOXMLShapes::sptCTextInflateTop:			return CBaseShapePtr(new OOXMLShapes::CTextInflateTop());
	case OOXMLShapes::sptCTextPlain:				return CBaseShapePtr(new OOXMLShapes::CTextPlain());
	case OOXMLShapes::sptCTextRingInside:			return CBaseShapePtr(new OOXMLShapes::CTextRingInside());
	case OOXMLShapes::sptCTextRingOutside:			return CBaseShapePtr(new OOXMLShapes::CTextRingOutside());
	case OOXMLShapes::sptCTextSlantDown:			return CBaseShapePtr(new OOXMLShapes::CTextSlantDown());
	case OOXMLShapes::sptCTextSlantUp:				return CBaseShapePtr(new OOXMLShapes::CTextSlantUp());
	case OOXMLShapes::sptCTextStop:					return CBaseShapePtr(new OOXMLShapes::CTextStop());
	case OOXMLShapes::sptCTextTriangle:				return CBaseShapePtr(new OOXMLShapes::CTextTriangle());
	case OOXMLShapes::sptCTextTriangleInverted:		return CBaseShapePtr(new OOXMLShapes::CTextTriangleInverted());
	case OOXMLShapes::sptCTextWave1:				return CBaseShapePtr(new OOXMLShapes::CTextWave1());
	case OOXMLShapes::sptCTextWave2:				return CBaseShapePtr(new OOXMLShapes::CTextWave2());
	case OOXMLShapes::sptCTextWave4:				return CBaseShapePtr(new OOXMLShapes::CTextWave4());
	case OOXMLShapes::sptCTrapezoid:				return CBaseShapePtr(new OOXMLShapes::CTrapezoid());
	case OOXMLShapes::sptCTriangle:					return CBaseShapePtr(new OOXMLShapes::CTriangle());
	case OOXMLShapes::sptCUpArrow:					return CBaseShapePtr(new OOXMLShapes::CUpArrow());
	case OOXMLShapes::sptCUpArrowCallout:			return CBaseShapePtr(new OOXMLShapes::CUpArrowCallout());
	case OOXMLShapes::sptCUpDownArrow:				return CBaseShapePtr(new OOXMLShapes::CUpDownArrow());
	case OOXMLShapes::sptCUpDownArrowCallout:		return CBaseShapePtr(new OOXMLShapes::CUpDownArrowCallout());
	case OOXMLShapes::sptCUturnArrow:				return CBaseShapePtr(new OOXMLShapes::CUturnArrow());
	case OOXMLShapes::sptCVerticalScroll:			return CBaseShapePtr(new OOXMLShapes::CVerticalScroll());
	case OOXMLShapes::sptCWave:						return CBaseShapePtr(new OOXMLShapes::CWave());
	case OOXMLShapes::sptCWedgeEllipseCallout:		return CBaseShapePtr(new OOXMLShapes::CWedgeEllipseCallout());
	case OOXMLShapes::sptCWedgeRectCallout:			return CBaseShapePtr(new OOXMLShapes::CWedgeRectCallout());
	case OOXMLShapes::sptCWedgeRoundRectCallout:	return CBaseShapePtr(new OOXMLShapes::CWedgeRoundRectCallout());
	}
	return shape;
}
