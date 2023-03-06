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
#include "../BaseShape.h"

using namespace ODRAW;

namespace OOXMLShapes
{
    enum ShapeType : unsigned short
	{
		sptMin = 0,
		sptNotPrimitive = sptMin,
		sptCAccentBorderCallout1,
		sptCAccentBorderCallout2,
		sptCAccentBorderCallout3,
		sptCAccentCallout1,
		sptCAccentCallout2,
		sptCAccentCallout3,
		sptCActionButtonBackPrevious,
		sptCActionButtonBeginning,
		sptCActionButtonBlank,
		sptCActionButtonDocument,
		sptCActionButtonEnd,
		sptCActionButtonForwardNext,
		sptCActionButtonHelp,
		sptCActionButtonHome,
		sptCActionButtonInformation,
		sptCActionButtonMovie,
		sptCActionButtonReturn,
		sptCActionButtonSound,
		sptCArc,
		sptCBentArrow,
		sptCBentConnector2,
		sptCBentConnector3,
		sptCBentConnector4,
		sptCBentConnector5,
		sptCBentUpArrow,
		sptCBevel,
		sptCBlockArc,
		sptCBorderCallout1,
		sptCBorderCallout2,
		sptCBorderCallout3,
		sptCBracePair,
		sptCBracketPair,
		sptCCallout1,
		sptCCallout2,
		sptCCallout3,
		sptCCan,
		sptCChartPlus,
		sptCChartStar,
		sptCChartX,
		sptCChevron,
		sptCChord,
		sptCCircularArrow,
		sptCCloud,
		sptCCloudCallout,
		sptCCorner,
		sptCCornerTabs,
		sptCCube,
		sptCCurvedConnector2,
		sptCCurvedConnector3,
		sptCCurvedConnector4,
		sptCCurvedConnector5,
		sptCCurvedDownArrow,
		sptCCurvedLeftArrow,
		sptCCurvedRightArrow,
		sptCCurvedUpArrow,
		sptCDecagon,
		sptCDiagStripe,
		sptCDiamond,
		sptCDodecagon,
		sptCDonut,
		sptCDoubleWave,
		sptCDownArrow,
		sptCDownArrowCallout,
		sptCEllipse,
		sptCEllipseRibbon,
		sptCEllipseRibbon2,
		sptCFlowChartAlternateProcess,
		sptCFlowChartCollate,
		sptCFlowChartConnector,
		sptCFlowChartDecision,
		sptCFlowChartDelay,
		sptCFlowChartDisplay,
		sptCFlowChartDocument,
		sptCFlowChartExtract,
		sptCFlowChartInputOutput,
		sptCFlowChartInternalStorage,
		sptCFlowChartMagneticDisk,
		sptCFlowChartMagneticDrum,
		sptCFlowChartMagneticTape,
		sptCFlowChartManualInput,
		sptCFlowChartManualOperation,
		sptCFlowChartMerge,
		sptCFlowChartMultidocument,
		sptCFlowChartOfflineStorage,
		sptCFlowChartOffpageConnector,
		sptCFlowChartOnlineStorage,
		sptCFlowChartOr,
		sptCFlowChartPredefinedProcess,
		sptCFlowChartPreparation,
		sptCFlowChartProcess,
		sptCFlowChartPunchedCard,
		sptCFlowChartPunchedTape,
		sptCFlowChartSort,
		sptCFlowChartSummingJunction,
		sptCFlowChartTerminator,
		sptCFoldedCorner,
		sptCFrame,
		sptCFunnel,
		sptCGear6,
		sptCGear9,
		sptCHalfFrame,
		sptCHeart,
		sptCHeptagon,
		sptCHexagon,
		sptCHomePlate,
		sptCHorizontalScroll,
		sptCIrregularSeal1,
		sptCIrregularSeal2,
		sptCLeftArrow,
		sptCLeftArrowCallout,
		sptCLeftBrace,
		sptCLeftBracket,
		sptCLeftCircularArrow,
		sptCLeftRightArrow,
		sptCLeftRightArrowCallout,
		sptCLeftRightCircularArrow,
		sptCLeftRightRibbon,
		sptCLeftRightUpArrow,
		sptCLeftUpArrow,
		sptCLightningBolt,
		sptCLine,
		sptCLineInv,
		sptCMathDivide,
		sptCMathEqual,
		sptCMathMinus,
		sptCMathMultiply,
		sptCMathNotEqual,
		sptCMathPlus,
		sptCMoon,
		sptCNonIsoscelesTrapezoid,
		sptCNoSmoking,
		sptCNotchedRightArrow,
		sptCOctagon,
		sptCParallelogram,
		sptCPentagon,
		sptCPie,
		sptCPieWedge,
		sptCPlaque,
		sptCPlaqueTabs,
		sptCPlus,
		sptCQuadArrow,
		sptCQuadArrowCallout,
		sptCRect,
		sptCRibbon,
		sptCRibbon2,
		sptCRightArrow,
		sptCRightArrowCallout,
		sptCRightBrace,
		sptCRightBracket,
		sptCRound1Rect,
		sptCRound2DiagRect,
		sptCRound2SameRect,
		sptCRoundRect,
		sptCRtTriangle,
		sptCSmileyFace,
		sptCSnip1Rect,
		sptCSnip2DiagRect,
		sptCSnip2SameRect,
		sptCSnipRoundRect,
		sptCSquareTabs,
		sptCStar10,
		sptCStar12,
		sptCStar16,
		sptCStar24,
		sptCStar32,
		sptCStar4,
		sptCStar5,
		sptCStar6,
		sptCStar7,
		sptCStar8,
		sptCStraightConnector1,
		sptCStripedRightArrow,
		sptCSun,
		sptCSwooshArrow,
		sptCTeardrop,
		sptCTextArchDown,
		sptCTextArchDownPour,
		sptCTextArchUp,
		sptCTextArchUpPour,
		sptCTextButton,
		sptCTextButtonPour,
		sptCTextCanDown,
		sptCTextCanUp,
		sptCTextCascadeDown,
		sptCTextCascadeUp,
		sptCTextChevron,
		sptCTextChevronInverted,
		sptCTextCircle,
		sptCTextCirclePour,
		sptCTextCurveDown,
		sptCTextCurveUp,
		sptCTextDeflate,
		sptCTextDeflateBottom,
		sptCTextDeflateInflate,
		sptCTextDeflateInflateDeflate,
		sptCTextDeflateTop,
		sptCTextDoubleWave1,
		sptCTextFadeDown,
		sptCTextFadeLeft,
		sptCTextFadeRight,
		sptCTextFadeUp,
		sptCTextInflate,
		sptCTextInflateBottom,
		sptCTextInflateTop,
		sptCTextPlain,
		sptCTextRingInside,
		sptCTextRingOutside,
		sptCTextSlantDown,
		sptCTextSlantUp,
		sptCTextStop,
		sptCTextTriangle,
		sptCTextTriangleInverted,
		sptCTextWave1,
		sptCTextWave2,
		sptCTextWave4,
		sptCTrapezoid,
		sptCTriangle,
		sptCUpArrow,
		sptCUpArrowCallout,
		sptCUpDownArrow,
		sptCUpDownArrowCallout,
		sptCUturnArrow,
		sptCVerticalScroll,
		sptCWave,
		sptCWedgeEllipseCallout,
		sptCWedgeRectCallout,
		sptCWedgeRoundRectCallout,

		sptMax,
		sptNil = 0x0FFF,
		sptCustom = 0x1000
	};
}

class CPPTXShape : public CBaseShape
{
public:
    OOXMLShapes::ShapeType m_eType;

    NSGuidesOOXML::CFormulaManager FManager;

	CPPTXShape();
	~CPPTXShape();

	virtual bool LoadFromXML(const std::wstring& xml);
	virtual bool LoadFromXML(XmlUtils::CXmlNode& root);

	virtual bool LoadAdjustValuesList(const std::wstring& xml);
	virtual bool LoadGuidesList(const std::wstring& xml);

	virtual bool LoadAdjustHandlesList(const std::wstring& xml);
	virtual bool LoadConnectorsList(const std::wstring& xml);

	virtual bool LoadTextRect(const std::wstring& xml);
	virtual bool LoadPathList(const std::wstring& xml);

	virtual bool SetAdjustment(long index, long value);

	virtual std::wstring ToXML(CGeomShapeInfo& GeomInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen);
	virtual void ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore);

	virtual void ReCalculate();
    static CBaseShapePtr CreateByType(OOXMLShapes::ShapeType type);    
	virtual const ClassType GetClassType() const;

	virtual bool SetProperties(CBaseShape* Shape);
	virtual bool SetToDublicate(CBaseShape* Shape);

	bool SetShapeType(OOXMLShapes::ShapeType type);

	virtual void SetWidthHeightLogic(const double& dWidth, const double& dHeight);
	virtual void GetWidthHeightLogic(double& dWidth, double& dHeight);
};
