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
#pragma once

#include "PptShape.h"
#include "PresetShapesHeader.h"

//-------------------------------------------------------------------------------
#define CREATE_BY_SPT(SHAPE_TYPE, CLASS_SHAPE_NAME)								\
	case SHAPE_TYPE: { pShape = new CLASS_SHAPE_NAME(); break; }				\
//-------------------------------------------------------------------------------

using namespace ODRAW;

CPPTShape::CPPTShape() : CBaseShape(), m_arStringTextRects()
{
    m_eType = PPTShapes::sptMin;

    m_arStringTextRects.push_back(L"0,0,21600,21600");

    m_strPathLimoX.clear();
    m_strPathLimoY.clear();
    m_bIsShapeType = false;

    m_bIsFilled = true;
    m_bIsStroked = true;
}
CPPTShape::~CPPTShape()
{
}

CBaseShapePtr CPPTShape::CreateByType(PPTShapes::ShapeType type)
{
	CPPTShape* pShape = NULL;
	switch (type)
	{
// msosptNotchedCircularArrow	0x00000064 A value that SHOULD NOT be used.
// msosptHostControl			0x000000C9 A value that SHOULD NOT be used.

	case sptHostControl:		
			{ pShape = new CPPTShape();  pShape->m_eType = type; break; }
	case 0: { pShape = new CRectangleType(); break; }
	
	case sptNotchedCircularArrow:
	case sptPictureFrame: { pShape = new CRectangleType(); pShape->m_eType = type; break; }

		CREATE_BY_SPT(sptAccentBorderCallout90, CAccentBorderCallout90Type)
		CREATE_BY_SPT(sptAccentBorderCallout1, CAccentBorderCallout1Type)
		CREATE_BY_SPT(sptAccentBorderCallout2, CAccentBorderCallout2Type)
		CREATE_BY_SPT(sptAccentBorderCallout3, CAccentBorderCallout3Type)

		CREATE_BY_SPT(sptAccentCallout90, CAccentCallout90Type)
		CREATE_BY_SPT(sptAccentCallout1, CAccentCallout1Type)
		CREATE_BY_SPT(sptAccentCallout2, CAccentCallout2Type)
		CREATE_BY_SPT(sptAccentCallout3, CAccentCallout3Type)

		CREATE_BY_SPT(sptBorderCallout90, CBorderCallout90Type)
		CREATE_BY_SPT(sptBorderCallout1, CBorderCallout1Type)
		CREATE_BY_SPT(sptBorderCallout2, CBorderCallout2Type)
		CREATE_BY_SPT(sptBorderCallout3, CBorderCallout3Type)

		CREATE_BY_SPT(sptCallout90, CCallout90Type)
		CREATE_BY_SPT(sptCallout1, CCallout1Type)
		CREATE_BY_SPT(sptCallout2, CCallout2Type)
		CREATE_BY_SPT(sptCallout3, CCallout3Type)

		CREATE_BY_SPT(sptActionButtonBlank, CActionButtonBlankType)
		CREATE_BY_SPT(sptActionButtonHome, CActionButtonHomeType)
		CREATE_BY_SPT(sptActionButtonHelp, CActionButtonHelpType)
		CREATE_BY_SPT(sptActionButtonInformation, CActionButtonInfoType)
		CREATE_BY_SPT(sptActionButtonBackPrevious, CActionButtonBackType)
		CREATE_BY_SPT(sptActionButtonForwardNext, CActionButtonNextType)
		CREATE_BY_SPT(sptActionButtonBeginning, CActionButtonBeginType)
		CREATE_BY_SPT(sptActionButtonEnd, CActionButtonEndType)
		CREATE_BY_SPT(sptActionButtonReturn, CActionButtonReturnType)
		CREATE_BY_SPT(sptActionButtonDocument, CActionButtonDocType)
		CREATE_BY_SPT(sptActionButtonSound, CActionButtonSoundType)
		CREATE_BY_SPT(sptActionButtonMovie, CActionButtonMovieType)
		
		CREATE_BY_SPT(sptArc, CArcType)
		CREATE_BY_SPT(sptLine, CLineType)

		CREATE_BY_SPT(sptBentArrow, CBentArrowType)
		CREATE_BY_SPT(sptBentUpArrow, CBentUpArrowType)
		CREATE_BY_SPT(sptBevel, CBevelType)
		CREATE_BY_SPT(sptBlockArc, CBlockArcType)
		CREATE_BY_SPT(sptBracePair, CBracePairType)
		CREATE_BY_SPT(sptBracketPair, CBracketPairType)
		
		CREATE_BY_SPT(sptCan, CCanType)
		CREATE_BY_SPT(sptChevron, CChevronType)
		CREATE_BY_SPT(sptCircularArrow, CCircularArrowType)
		CREATE_BY_SPT(sptCloudCallout, CCloudCalloutType)
		CREATE_BY_SPT(sptCube, CCubeType)
		CREATE_BY_SPT(sptCurvedDownArrow, CCurvedDownArrowType)
		CREATE_BY_SPT(sptCurvedLeftArrow, CCurvedLeftArrowType)
		CREATE_BY_SPT(sptCurvedRightArrow, CCurvedRightArrowType)
		CREATE_BY_SPT(sptCurvedUpArrow, CCurvedUpArrowType)

		CREATE_BY_SPT(sptDiamond, CDiamondType)
		CREATE_BY_SPT(sptDonut, CDonutType)
		CREATE_BY_SPT(sptDownArrowCallout, CDownArrowCalloutType)
		CREATE_BY_SPT(sptDownArrow, CDownArrowType)

		CREATE_BY_SPT(sptEllipse, CEllipseType)
		CREATE_BY_SPT(sptEllipseRibbon, CEllipceRibbonType)
		CREATE_BY_SPT(sptEllipseRibbon2, CEllipceRibbon2Type)

		CREATE_BY_SPT(sptFlowChartAlternateProcess, CFlowChartAlternateProcessType)
		CREATE_BY_SPT(sptFlowChartCollate, CFlowChartCollateType)
		CREATE_BY_SPT(sptFlowChartConnector, CFlowChartConnectorType)
		CREATE_BY_SPT(sptFlowChartDecision, CFlowChartDecisionType)
		CREATE_BY_SPT(sptFlowChartDisplay, CFlowChartDisplayType)
		CREATE_BY_SPT(sptFlowChartDelay, CFlowChartDelayType)
		CREATE_BY_SPT(sptFlowChartDocument, CFlowChartDocumentType)
		CREATE_BY_SPT(sptFlowChartExtract, CFlowChartExtractType)
		CREATE_BY_SPT(sptFlowChartInputOutput, CFlowChartInputOutputType)
		CREATE_BY_SPT(sptFlowChartInternalStorage, CFlowChartInternalStorageType)
		CREATE_BY_SPT(sptFlowChartMagneticDisk, CFlowChartMagneticDiskType)
		CREATE_BY_SPT(sptFlowChartMagneticDrum, CFlowChartMagneticDrumType)
		CREATE_BY_SPT(sptFlowChartMagneticTape, CFlowChartMagneticTapeType)
		CREATE_BY_SPT(sptFlowChartManualInput, CFlowChartManualInputType)
		CREATE_BY_SPT(sptFlowChartManualOperation, CFlowChartManualOperationType)
		CREATE_BY_SPT(sptFlowChartMerge, CFlowChartMergeType)
		CREATE_BY_SPT(sptFlowChartMultidocument, CFlowChartMultidocumentType)
		CREATE_BY_SPT(sptFlowChartOffpageConnector, CFlowChartOffpageConnectorType)
		CREATE_BY_SPT(sptFlowChartOnlineStorage, CFlowChartOnlineStorageType)
		CREATE_BY_SPT(sptFlowChartOr, CFlowChartOrType)
		CREATE_BY_SPT(sptFlowChartPredefinedProcess, CFlowChartPredefinedProcessType)
		CREATE_BY_SPT(sptFlowChartPreparation, CFlowChartPreparationType)
		CREATE_BY_SPT(sptFlowChartProcess, CFlowChartProcessType)
		CREATE_BY_SPT(sptFlowChartPunchedCard, CFlowChartPunchedCardType)
		CREATE_BY_SPT(sptFlowChartPunchedTape, CFlowChartPunchedTapeType)
		CREATE_BY_SPT(sptFlowChartSort, CFlowChartSortType)
		CREATE_BY_SPT(sptFlowChartSummingJunction, CFlowChartSummingJunctionType)
		CREATE_BY_SPT(sptFlowChartTerminator, CFlowChartTerminatorType)
		CREATE_BY_SPT(sptFoldedCorner, CFoldedCornerType)

		CREATE_BY_SPT(sptHeart, CHeartType)
		CREATE_BY_SPT(sptHexagon, CHexagonType)
		CREATE_BY_SPT(sptHomePlate, CHomePlateType)

		CREATE_BY_SPT(sptIrregularSeal1, CIrregularSealOneType)
		CREATE_BY_SPT(sptIrregularSeal2, CIrregularSealTwo)
		CREATE_BY_SPT(sptIsocelesTriangle, CIsoscelesTriangleType)

		CREATE_BY_SPT(sptLeftArrowCallout, CLeftArrowCalloutType)
		CREATE_BY_SPT(sptLeftArrow, CLeftArrowType)
		CREATE_BY_SPT(sptLeftBrace, CLeftBraceType)
		CREATE_BY_SPT(sptLeftBracket, CLeftBracketType)
		CREATE_BY_SPT(sptLeftRightArrowCallout, CLeftRightArrowCalloutType)
		CREATE_BY_SPT(sptLeftRightArrow, CLeftRightArrowType)
		CREATE_BY_SPT(sptLeftRightUpArrow, CLeftRightUpArrow)
		CREATE_BY_SPT(sptLeftUpArrow, CLeftUpArrowType)
		CREATE_BY_SPT(sptLightningBolt, CLightningBoltType)

		CREATE_BY_SPT(sptMoon, CMoonType)

		CREATE_BY_SPT(sptNoSmoking, CNoSmokingType)
		CREATE_BY_SPT(sptNotchedRightArrow, CNotchedRightArrowType)

		CREATE_BY_SPT(sptOctagon, COctagonType)

		CREATE_BY_SPT(sptParallelogram, CParallelogramType)
		CREATE_BY_SPT(sptPentagon, CPentagonType)
		CREATE_BY_SPT(sptPlaque, CPlaqueType)
		CREATE_BY_SPT(sptPlus, CPlusType)

		CREATE_BY_SPT(sptQuadArrowCallout, CQuadArrowCalloutType)
		CREATE_BY_SPT(sptQuadArrow, CQuadArrowType)

		CREATE_BY_SPT(sptRectangle, CRectangleType)
		CREATE_BY_SPT(sptRibbon, CRibbonDownType)
		CREATE_BY_SPT(sptRibbon2, CRibbonUpType)
		CREATE_BY_SPT(sptRightArrowCallout, CRightArrowCalloutType)
		CREATE_BY_SPT(sptArrow, CRightArrowType)
		CREATE_BY_SPT(sptRightBrace, CRightBracetype)
		CREATE_BY_SPT(sptRightBracket, CRightBracketType)
		CREATE_BY_SPT(sptRightTriangle, CRightTriangleType)
		CREATE_BY_SPT(sptRoundRectangle, CRoundedRectangleType)

		CREATE_BY_SPT(sptSeal16, CSeal16Type)
		CREATE_BY_SPT(sptSeal24, CSeal24Type)
		CREATE_BY_SPT(sptSeal32, CSeal32Type)
		CREATE_BY_SPT(sptSeal4, CSeal4Type)
		CREATE_BY_SPT(sptSeal8, CSeal8Type)
		CREATE_BY_SPT(sptSmileyFace, CSmileyFaceType)
		CREATE_BY_SPT(sptStar, CStarType)
		CREATE_BY_SPT(sptStraightConnector1, CStraightConnectorType)
		CREATE_BY_SPT(sptStripedRightArrow, CStripedRightArrowType)
		CREATE_BY_SPT(sptSun, CSunType)

		CREATE_BY_SPT(sptTextBox, CTextboxType)
		CREATE_BY_SPT(sptTrapezoid, CTrapezoidType)

		CREATE_BY_SPT(sptUpArrowCallout, CUpArrowCalloutType)
		CREATE_BY_SPT(sptUpArrow, CUpArrowType)
		CREATE_BY_SPT(sptUpDownArrowCallout, CUpDownArrowCalloutType)
		CREATE_BY_SPT(sptUpDownArrow, CUpDownArrowType)
		CREATE_BY_SPT(sptUturnArrow, CUturnArrowType)

		CREATE_BY_SPT(sptVerticalScroll, CVerticalScrollType)
		CREATE_BY_SPT(sptHorizontalScroll, CHorizontalScrollType)

		CREATE_BY_SPT(sptWedgeEllipseCallout, CWedgeEllipseCalloutType)
		CREATE_BY_SPT(sptWedgeRectCallout, CWedgeRectCalloutType)
		CREATE_BY_SPT(sptWedgeRRectCallout, CWedgeRoundedRectCalloutType)

		CREATE_BY_SPT(sptWave, CWaveType)
		CREATE_BY_SPT(sptDoubleWave, CWaveDoubleType)

		case sptBentConnector2:
		case sptBentConnector3:    
		case sptBentConnector4:
		case sptBentConnector5:
			{
				pShape = new CBentConnectorType(); 
				break;
			}
		case sptCurvedConnector2:
		case sptCurvedConnector3:    
		case sptCurvedConnector4:
		case sptCurvedConnector5:
			{
				pShape = new CCurvedConnectorType();
				break;
			}

		case sptTextPlainText:    
		case sptTextStop:  
		case sptTextTriangle:   
		case sptTextTriangleInverted:
		case sptTextChevron:
		case sptTextChevronInverted:
		case sptTextRingInside:
		case sptTextRingOutside:
		case sptTextArchUpCurve:   
		case sptTextArchDownCurve: 
		case sptTextCircleCurve: 
		case sptTextButtonCurve: 
		case sptTextArchUpPour:  
		case sptTextArchDownPour: 
		case sptTextCirclePour:
		case sptTextButtonPour:  
		case sptTextCurveUp:  
		case sptTextCurveDown: 
		case sptTextCascadeUp:   
		case sptTextCascadeDown:
		case sptTextWave1:   
		case sptTextWave2:   
		case sptTextWave3:   
		case sptTextWave4: 
		case sptTextInflate:   
		case sptTextDeflate:    
		case sptTextInflateBottom:  
		case sptTextDeflateBottom:  
		case sptTextInflateTop:
		case sptTextDeflateTop:   
		case sptTextDeflateInflate:   
		case sptTextDeflateInflateDeflate:
		case sptTextFadeRight: 
		case sptTextFadeLeft:   
		case sptTextFadeUp:   
		case sptTextFadeDown:   
		case sptTextSlantUp:    
		case sptTextSlantDown:   
		case sptTextCanUp:   
		case sptTextCanDown:
		{
			pShape = new CTextboxType();
		}

		default: break;
	};

	if (NULL != pShape)
		pShape->m_eType = type;

	return CBaseShapePtr(pShape);
}


bool CPPTShape::LoadFromXML(const std::wstring& xml)
{
    XmlUtils::CXmlNode oNodePict;
    if (oNodePict.FromXmlString(xml))
    {
        return LoadFromXML(oNodePict);
    }
    return false;
}

bool CPPTShape::LoadFromXML(XmlUtils::CXmlNode& oNodePict)
{
    std::wstring id = oNodePict.GetAttributeOrValue(L"type");
    bool isPathList = false;
    if (false == id.empty())
    {
        SetShapeType((PPTShapes::ShapeType)XmlUtils::GetInteger(id));
    }
    else
    {
        XmlUtils::CXmlNode oNodeTemplate;
        if (oNodePict.GetNode(L"template", oNodeTemplate))
        {
            std::wstring strAdj = oNodeTemplate.GetAttributeOrValue(L"adj");
            LoadAdjustValuesList(strAdj);

            XmlUtils::CXmlNode oNodeGuides;
            if (oNodeTemplate.GetNode(L"v:formulas", oNodeGuides))
            {
                LoadGuidesList(oNodeGuides.GetXml());
            }

            std::wstring strPath = oNodeTemplate.GetAttributeOrValue(L"path");
            if (false == strPath.empty())
            {
                LoadPathList(strPath);
                isPathList = true;
            }
        }
    }

    XmlUtils::CXmlNode oNodeGuides;
    if (oNodePict.GetNode(L"path", oNodeGuides))
    {
        std::wstring strPath = oNodeGuides.GetAttributeOrValue(L"val");
        if (false == strPath.empty())
        {
            LoadPathList(strPath);
            isPathList = true;
        }
    }

    if (!isPathList)
        ReCalculate();
    return true;
}

bool CPPTShape::LoadFromXMLShapeType(XmlUtils::CXmlNode& oNodeShapeType) // vml object
{		// из за особенносей форматирования vmlDrawing могут вылезти пустые текстовые значения - value ..
    std::wstring sId = oNodeShapeType.GetAttribute(L"o:spt");

    bool bIsNeedRecalc = true;
	bool bCustomSet = false;

	if (false == sId.empty())
    {
        int id = XmlUtils::GetInteger(sId);
        if (id > 0)
        {
            SetShapeType((PPTShapes::ShapeType)id);
            //ReCalculate();
            m_eType = (PPTShapes::ShapeType)id;
			bCustomSet = true;
        }
    }
    std::wstring strAdj = oNodeShapeType.GetAttribute(L"adj");
    if (false == strAdj.empty())
        LoadAdjustValuesList(strAdj);

    XmlUtils::CXmlNode oNodeGuides;
    if (oNodeShapeType.GetNode(L"v:formulas", oNodeGuides))
    {
		bCustomSet = LoadGuidesList(oNodeGuides.GetXml());
	}

    XmlUtils::CXmlNode oNodePath;
    if (oNodeShapeType.GetNode(L"v:path", oNodePath))
    {
		std::wstring strPath = oNodePath.GetAttribute(L"v");
		if (false == strPath.empty())
		{
			bCustomSet = LoadPathList(strPath);
		}
		std::wstring strTextR = oNodePath.GetAttribute(L"textboxrect");
        if (false == strTextR.empty())
            bCustomSet = LoadTextRect(strTextR);
	}

    XmlUtils::CXmlNode oNodeAHs;
    if (oNodeShapeType.GetNode(L"v:handles", oNodeAHs))
    {
        LoadAHList(oNodeAHs);
    }

    std::wstring strPath = oNodeShapeType.GetAttribute(L"path");
    if (false == strPath.empty())
    {
        bCustomSet = LoadPathList(strPath);
	}

    XmlUtils::CXmlNode oNodeTextPath;
    if (oNodeShapeType.GetNode(L"v:textpath", oNodeTextPath))
    {
		bCustomSet = true;
		m_textPath.bEnabled = true;

        if (m_eType < PPTShapes::ShapeType::sptCTextPlain || m_eType > PPTShapes::ShapeType::sptCTextCanDown)
            m_eType = PPTShapes::ShapeType::sptCTextPlain;
		nullable_bool oFitPath;
		
		XmlMacroReadAttributeBase(oNodeTextPath, L"fitpath", oFitPath);
		if (oFitPath.is_init() && (*oFitPath))
		{
			m_textPath.bFitPath = true;
		}

		nullable_bool oFitShape;
		XmlMacroReadAttributeBase(oNodeTextPath, L"fitshape", oFitShape);
		if (oFitShape.is_init() && (*oFitShape))
		{
			m_textPath.bFitShape = true;
		}

		nullable_bool oTrim;
		XmlMacroReadAttributeBase(oNodeTextPath, L"trim", oTrim);
		if (oTrim.is_init() && (*oTrim))
		{
			m_textPath.bTrim = true;
		}
		nullable_string oStyle;
		XmlMacroReadAttributeBase(oNodeTextPath, L"style", oStyle);
		if (oStyle.is_init() && (false == oStyle->empty()))
		{
			if (m_textPath.sStyle)
				m_textPath.sStyle = (*m_textPath.sStyle) + L";" + (*oStyle);
			else 
				m_textPath.sStyle = *oStyle;
		}
	}

    std::wstring strFilled = oNodeShapeType.GetAttribute(L"filled");
    std::wstring strStroked = oNodeShapeType.GetAttribute(L"stroked");

    if (false == strFilled.empty())
    {
        if (strFilled == L"false" || strFilled == L"f")
            m_bIsFilled = false;
        else
            m_bIsFilled = true;
    }

    if (false == strStroked.empty())
    {
        if (strStroked == L"false" || strStroked == L"f")
            m_bIsStroked = false;
        else
            m_bIsStroked = true;
    }
    XmlUtils::CXmlNode oNodeSignature;
    if (oNodeShapeType.GetNode(L"o:signatureline", oNodeSignature))
    {
        m_oSignatureLine = oNodeSignature;
    }

	XmlUtils::CXmlNode oNodeImagedata;
	if (!bCustomSet && oNodeShapeType.GetNode(L"v:imagedata", oNodeImagedata))
	{
		std::wstring sRid = oNodeImagedata.GetAttribute(L"r:id");
		std::wstring sRelid = oNodeImagedata.GetAttribute(L"o:relid");
		std::wstring sPictId = oNodeImagedata.GetAttribute(L"r:pict");

		if (!sRid.empty() || !sRelid.empty() || !sPictId.empty())
			m_eType = PPTShapes::ShapeType::sptCFrame;
	}
	ReCalculate();
    return true;
}
