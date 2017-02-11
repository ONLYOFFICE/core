/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef PPTX_LOGIC_SHAPE_INCLUDE_H_
#define PPTX_LOGIC_SHAPE_INCLUDE_H_

#include "../WrapperWritingElement.h"
#include "NvSpPr.h"
#include "SpPr.h"
#include "ShapeStyle.h"
#include "TxBody.h"
#include "ShapeProperties.h"
#include "ShapeTextProperties.h"
#include "UniFill.h"
#include "Ln.h"

#include "../../../ASCOfficeDocxFile2/DocWrapper/DocxSerializer.h"
#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Vml.h"

namespace OOX
{
	static SimpleTypes::Vml::SptType PrstGeom2VmlShapeType(SimpleTypes::EShapeType type) 
	{
		switch(type)
		{
			case	SimpleTypes::shapetypeRect	:			return	SimpleTypes::Vml::sptRectangle	;
			case	SimpleTypes::shapetypeRoundRect	:		return	SimpleTypes::Vml::sptRoundRectangle	;
			case	SimpleTypes::shapetypeEllipse	:		return	SimpleTypes::Vml::sptEllipse	;
			case	SimpleTypes::shapetypeDiamond	:		return	SimpleTypes::Vml::sptDiamond	;
			case	SimpleTypes::shapetypeTriangle	:		return	SimpleTypes::Vml::sptIsocelesTriangle	;
			case	SimpleTypes::shapetypeRtTriangle	:	return	SimpleTypes::Vml::sptRightTriangle	;
			case	SimpleTypes::shapetypeParallelogram	:	return	SimpleTypes::Vml::sptParallelogram	;
			case	SimpleTypes::shapetypeTrapezoid	:		return	SimpleTypes::Vml::sptTrapezoid	;
			case	SimpleTypes::shapetypeHexagon	:		return	SimpleTypes::Vml::sptHexagon	;
			case	SimpleTypes::shapetypeOctagon	:		return	SimpleTypes::Vml::sptOctagon	;
			case	SimpleTypes::shapetypePlus	:			return	SimpleTypes::Vml::sptPlus	;
			case	SimpleTypes::shapetypeStar5	:			return	SimpleTypes::Vml::sptStar	;
			case	SimpleTypes::shapetypeRightArrow:		return	SimpleTypes::Vml::sptArrow	;
			case	SimpleTypes::shapetypeThickArrow:		return	SimpleTypes::Vml::sptThickArrow	;
			case	SimpleTypes::shapetypeHomePlate	:		return	SimpleTypes::Vml::sptHomePlate	;
			case	SimpleTypes::shapetypeCube	:			return	SimpleTypes::Vml::sptCube	;
			case	SimpleTypes::shapetypeBallon	:		return	SimpleTypes::Vml::sptBalloon	;
//			case	SimpleTypes::shapetypeIrregularSeal1:	return	SimpleTypes::Vml::sptSeal	;
			case	SimpleTypes::shapetypeArc	:			return	SimpleTypes::Vml::sptArc	;
			case	SimpleTypes::shapetypeLine	:			return	SimpleTypes::Vml::sptLine	;
			case	SimpleTypes::shapetypePlaque	:		return	SimpleTypes::Vml::sptPlaque	;
			case	SimpleTypes::shapetypeCan	:			return	SimpleTypes::Vml::sptCan	;
			case	SimpleTypes::shapetypeDonut	:			return	SimpleTypes::Vml::sptDonut	;
//			case	SimpleTypes::shapetypeTextSimple	:	return	SimpleTypes::Vml::sptTextSimple	;
//			case	SimpleTypes::shapetypeTextOctagon	:	return	SimpleTypes::Vml::sptTextOctagon	;
//			case	SimpleTypes::shapetypeTextHexagon	:	return	SimpleTypes::Vml::sptTextHexagon	;
//			case	SimpleTypes::shapetypeTextCurve	:		return	SimpleTypes::Vml::sptTextCurve	;
//			case	SimpleTypes::shapetypeTextWave	:		return	SimpleTypes::Vml::sptTextWave	;
//			case	SimpleTypes::shapetypeTextRing	:		return	SimpleTypes::Vml::sptTextRing	;
//			case	SimpleTypes::shapetypeTextOnCurve	:	return	SimpleTypes::Vml::sptTextOnCurve	;
//			case	SimpleTypes::shapetypeTextOnRing	:	return	SimpleTypes::Vml::sptTextOnRing	;
			case	SimpleTypes::shapetypeStraightConnector1:	return	SimpleTypes::Vml::sptStraightConnector1	;
			case	SimpleTypes::shapetypeBentConnector2	:	return	SimpleTypes::Vml::sptBentConnector2	;
			case	SimpleTypes::shapetypeBentConnector3	:	return	SimpleTypes::Vml::sptBentConnector3	;
			case	SimpleTypes::shapetypeBentConnector4	:	return	SimpleTypes::Vml::sptBentConnector4	;
			case	SimpleTypes::shapetypeBentConnector5	:	return	SimpleTypes::Vml::sptBentConnector5	;
			case	SimpleTypes::shapetypeCurvedConnector2	:	return	SimpleTypes::Vml::sptCurvedConnector2	;
			case	SimpleTypes::shapetypeCurvedConnector3	:	return	SimpleTypes::Vml::sptCurvedConnector3	;
			case	SimpleTypes::shapetypeCurvedConnector4	:	return	SimpleTypes::Vml::sptCurvedConnector4	;
			case	SimpleTypes::shapetypeCurvedConnector5	:	return	SimpleTypes::Vml::sptCurvedConnector5	;
			case	SimpleTypes::shapetypeCallout1	:			return	SimpleTypes::Vml::sptCallout1	;
			case	SimpleTypes::shapetypeCallout2	:			return	SimpleTypes::Vml::sptCallout2	;
			case	SimpleTypes::shapetypeCallout3	:			return	SimpleTypes::Vml::sptCallout3	;
			case	SimpleTypes::shapetypeAccentCallout1	:	return	SimpleTypes::Vml::sptAccentCallout1	;
			case	SimpleTypes::shapetypeAccentCallout2	:	return	SimpleTypes::Vml::sptAccentCallout2	;
			case	SimpleTypes::shapetypeAccentCallout3	:	return	SimpleTypes::Vml::sptAccentCallout3	;
			case	SimpleTypes::shapetypeAccentBorderCallout1	:	return	SimpleTypes::Vml::sptAccentBorderCallout1	;
			case	SimpleTypes::shapetypeAccentBorderCallout2	:	return	SimpleTypes::Vml::sptAccentBorderCallout2	;
			case	SimpleTypes::shapetypeAccentBorderCallout3	:	return	SimpleTypes::Vml::sptAccentBorderCallout3	;
			case	SimpleTypes::shapetypeRibbon	:				return	SimpleTypes::Vml::sptRibbon	;
			case	SimpleTypes::shapetypeRibbon2	:				return	SimpleTypes::Vml::sptRibbon2	;
			case	SimpleTypes::shapetypeChevron	:				return	SimpleTypes::Vml::sptChevron	;
			case	SimpleTypes::shapetypePentagon	:				return	SimpleTypes::Vml::sptPentagon	;
			case	SimpleTypes::shapetypeNoSmoking	:				return	SimpleTypes::Vml::sptNoSmoking	;
			case	SimpleTypes::shapetypeStar8	:					return	SimpleTypes::Vml::sptSeal8	;
			case	SimpleTypes::shapetypeStar16	:				return	SimpleTypes::Vml::sptSeal16	;
			case	SimpleTypes::shapetypeStar32	:				return	SimpleTypes::Vml::sptSeal32	;
			case	SimpleTypes::shapetypeWedgeRectCallout	:		return	SimpleTypes::Vml::sptWedgeRectCallout	;
			case	SimpleTypes::shapetypeWedgeRoundRectCallout	:	return	SimpleTypes::Vml::sptWedgeRRectCallout	;
			case	SimpleTypes::shapetypeWedgeEllipseCallout	:	return	SimpleTypes::Vml::sptWedgeEllipseCallout	;
			case	SimpleTypes::shapetypeWave	:					return	SimpleTypes::Vml::sptWave	;
			case	SimpleTypes::shapetypeFoldedCorner	:			return	SimpleTypes::Vml::sptFoldedCorner	;
			case	SimpleTypes::shapetypeLeftArrow	:				return	SimpleTypes::Vml::sptLeftArrow	;
			case	SimpleTypes::shapetypeDownArrow	:				return	SimpleTypes::Vml::sptDownArrow	;
			case	SimpleTypes::shapetypeUpArrow	:				return	SimpleTypes::Vml::sptUpArrow	;
			case	SimpleTypes::shapetypeLeftRightArrow	:		return	SimpleTypes::Vml::sptLeftRightArrow	;
			case	SimpleTypes::shapetypeUpDownArrow	:			return	SimpleTypes::Vml::sptUpDownArrow	;
			case	SimpleTypes::shapetypeIrregularSeal1	:		return	SimpleTypes::Vml::sptIrregularSeal1	;
			case	SimpleTypes::shapetypeIrregularSeal2	:		return	SimpleTypes::Vml::sptIrregularSeal2	;
			case	SimpleTypes::shapetypeLightningBolt	:			return	SimpleTypes::Vml::sptLightningBolt	;
			case	SimpleTypes::shapetypeHeart	:					return	SimpleTypes::Vml::sptHeart	;
			case	SimpleTypes::shapetypeQuadArrow	:				return	SimpleTypes::Vml::sptQuadArrow	;
			case	SimpleTypes::shapetypeLeftArrowCallout	:		return	SimpleTypes::Vml::sptLeftArrowCallout	;
			case	SimpleTypes::shapetypeRightArrowCallout	:		return	SimpleTypes::Vml::sptRightArrowCallout	;
			case	SimpleTypes::shapetypeUpArrowCallout	:		return	SimpleTypes::Vml::sptUpArrowCallout	;
			case	SimpleTypes::shapetypeDownArrowCallout	:		return	SimpleTypes::Vml::sptDownArrowCallout	;
			case	SimpleTypes::shapetypeLeftRightArrowCallout	:	return	SimpleTypes::Vml::sptLeftRightArrowCallout	;
			case	SimpleTypes::shapetypeUpDownArrowCallout	:	return	SimpleTypes::Vml::sptUpDownArrowCallout	;
			case	SimpleTypes::shapetypeQuadArrowCallout	:		return	SimpleTypes::Vml::sptQuadArrowCallout	;
			case	SimpleTypes::shapetypeBevel	:					return	SimpleTypes::Vml::sptBevel	;
			case	SimpleTypes::shapetypeLeftBracket	:			return	SimpleTypes::Vml::sptLeftBracket	;
			case	SimpleTypes::shapetypeRightBracket	:			return	SimpleTypes::Vml::sptRightBracket	;
			case	SimpleTypes::shapetypeLeftBrace	:				return	SimpleTypes::Vml::sptLeftBrace	;
			case	SimpleTypes::shapetypeRightBrace	:			return	SimpleTypes::Vml::sptRightBrace	;
			case	SimpleTypes::shapetypeLeftUpArrow	:			return	SimpleTypes::Vml::sptLeftUpArrow	;
			case	SimpleTypes::shapetypeBentUpArrow	:			return	SimpleTypes::Vml::sptBentUpArrow	;
			case	SimpleTypes::shapetypeBentArrow	:				return	SimpleTypes::Vml::sptBentArrow	;
			case	SimpleTypes::shapetypeStar24	:				return	SimpleTypes::Vml::sptSeal24	;
			case	SimpleTypes::shapetypeStripedRightArrow	:		return	SimpleTypes::Vml::sptStripedRightArrow	;
			case	SimpleTypes::shapetypeNotchedRightArrow	:		return	SimpleTypes::Vml::sptNotchedRightArrow	;
			case	SimpleTypes::shapetypeBlockArc	:				return	SimpleTypes::Vml::sptBlockArc	;
			case	SimpleTypes::shapetypeSmileyFace	:			return	SimpleTypes::Vml::sptSmileyFace	;
			case	SimpleTypes::shapetypeVerticalScroll	:		return	SimpleTypes::Vml::sptVerticalScroll	;
			case	SimpleTypes::shapetypeHorizontalScroll	:		return	SimpleTypes::Vml::sptHorizontalScroll	;
			case	SimpleTypes::shapetypeCircularArrow	:			return	SimpleTypes::Vml::sptCircularArrow	;
//			case	SimpleTypes::shapetypeNotchedCircularArrow	:		return	SimpleTypes::Vml::sptNotchedCircularArrow	;
			case	SimpleTypes::shapetypeUturnArrow	:				return	SimpleTypes::Vml::sptUturnArrow	;
			case	SimpleTypes::shapetypeCurvedRightArrow	:			return	SimpleTypes::Vml::sptCurvedRightArrow	;
			case	SimpleTypes::shapetypeCurvedLeftArrow	:			return	SimpleTypes::Vml::sptCurvedLeftArrow	;
			case	SimpleTypes::shapetypeCurvedUpArrow	:				return	SimpleTypes::Vml::sptCurvedUpArrow	;
			case	SimpleTypes::shapetypeCurvedDownArrow	:			return	SimpleTypes::Vml::sptCurvedDownArrow	;
			case	SimpleTypes::shapetypeCloudCallout	:				return	SimpleTypes::Vml::sptCloudCallout	;
			case	SimpleTypes::shapetypeEllipseRibbon	:				return	SimpleTypes::Vml::sptEllipseRibbon	;
			case	SimpleTypes::shapetypeEllipseRibbon2	:			return	SimpleTypes::Vml::sptEllipseRibbon2	;
			case	SimpleTypes::shapetypeFlowChartProcess	:			return	SimpleTypes::Vml::sptFlowChartProcess	;
			case	SimpleTypes::shapetypeFlowChartDecision	:			return	SimpleTypes::Vml::sptFlowChartDecision	;
			case	SimpleTypes::shapetypeFlowChartInputOutput	:		return	SimpleTypes::Vml::sptFlowChartInputOutput	;
			case	SimpleTypes::shapetypeFlowChartInternalStorage	:	return	SimpleTypes::Vml::sptFlowChartInternalStorage	;
			case	SimpleTypes::shapetypeFlowChartDocument	:			return	SimpleTypes::Vml::sptFlowChartDocument	;
			case	SimpleTypes::shapetypeFlowChartMultidocument	:	return	SimpleTypes::Vml::sptFlowChartMultidocument	;
			case	SimpleTypes::shapetypeFlowChartTerminator	:		return	SimpleTypes::Vml::sptFlowChartTerminator	;
			case	SimpleTypes::shapetypeFlowChartPreparation	:		return	SimpleTypes::Vml::sptFlowChartPreparation	;
			case	SimpleTypes::shapetypeFlowChartManualInput	:		return	SimpleTypes::Vml::sptFlowChartManualInput	;
			case	SimpleTypes::shapetypeFlowChartManualOperation	:	return	SimpleTypes::Vml::sptFlowChartManualOperation	;
			case	SimpleTypes::shapetypeFlowChartConnector	:		return	SimpleTypes::Vml::sptFlowChartConnector	;
			case	SimpleTypes::shapetypeFlowChartPunchedCard	:		return	SimpleTypes::Vml::sptFlowChartPunchedCard	;
			case	SimpleTypes::shapetypeFlowChartPunchedTape	:		return	SimpleTypes::Vml::sptFlowChartPunchedTape	;
			case	SimpleTypes::shapetypeFlowChartSummingJunction	:	return	SimpleTypes::Vml::sptFlowChartSummingJunction	;
			case	SimpleTypes::shapetypeFlowChartOr	:			return	SimpleTypes::Vml::sptFlowChartOr	;
			case	SimpleTypes::shapetypeFlowChartCollate	:		return	SimpleTypes::Vml::sptFlowChartCollate	;
			case	SimpleTypes::shapetypeFlowChartSort	:			return	SimpleTypes::Vml::sptFlowChartSort	;
			case	SimpleTypes::shapetypeFlowChartExtract	:		return	SimpleTypes::Vml::sptFlowChartExtract	;
			case	SimpleTypes::shapetypeFlowChartMerge	:		return	SimpleTypes::Vml::sptFlowChartMerge	;
			//case	SimpleTypes::shapetypeChartOfflineStorage	:	return	SimpleTypes::Vml::sptFlowChartOfflineStorage	;
			case	SimpleTypes::shapetypeFlowChartOnlineStorage:	return	SimpleTypes::Vml::sptFlowChartOnlineStorage	;
			case	SimpleTypes::shapetypeFlowChartMagneticTape	:	return	SimpleTypes::Vml::sptFlowChartMagneticTape	;
			case	SimpleTypes::shapetypeFlowChartMagneticDisk	:	return	SimpleTypes::Vml::sptFlowChartMagneticDisk	;
			case	SimpleTypes::shapetypeFlowChartMagneticDrum	:	return	SimpleTypes::Vml::sptFlowChartMagneticDrum	;
			case	SimpleTypes::shapetypeFlowChartDisplay	:		return	SimpleTypes::Vml::sptFlowChartDisplay	;
			case	SimpleTypes::shapetypeFlowChartDelay	:		return	SimpleTypes::Vml::sptFlowChartDelay	;
//			case	SimpleTypes::shapetypeTextWave3	:				return	SimpleTypes::Vml::sptTextWave3	;
			case	SimpleTypes::shapetypeTextInflateBottom	:		return	SimpleTypes::Vml::sptTextInflateBottom	;
			case	SimpleTypes::shapetypeTextDeflateBottom	:		return	SimpleTypes::Vml::sptTextDeflateBottom	;
			case	SimpleTypes::shapetypeTextInflateTop	:		return	SimpleTypes::Vml::sptTextInflateTop	;
			case	SimpleTypes::shapetypeTextDeflateTop	:		return	SimpleTypes::Vml::sptTextDeflateTop	;
			case	SimpleTypes::shapetypeTextDeflateInflate:		return	SimpleTypes::Vml::sptTextDeflateInflate	;
			case	SimpleTypes::shapetypeTextFadeRight	:			return	SimpleTypes::Vml::sptTextFadeRight	;
			case	SimpleTypes::shapetypeTextFadeLeft	:			return	SimpleTypes::Vml::sptTextFadeLeft	;
			case	SimpleTypes::shapetypeTextFadeUp	:			return	SimpleTypes::Vml::sptTextFadeUp	;
			case	SimpleTypes::shapetypeTextFadeDown	:			return	SimpleTypes::Vml::sptTextFadeDown	;
			case	SimpleTypes::shapetypeBorderCallout1	:		return	SimpleTypes::Vml::sptBorderCallout1	;
			case	SimpleTypes::shapetypeBorderCallout2	:		return	SimpleTypes::Vml::sptBorderCallout2	;
			case	SimpleTypes::shapetypeBorderCallout3	:		return	SimpleTypes::Vml::sptBorderCallout3	;
			case	SimpleTypes::shapetypeLeftRightUpArrow	:		return	SimpleTypes::Vml::sptLeftRightUpArrow	;
			case	SimpleTypes::shapetypeSun	:					return	SimpleTypes::Vml::sptSun	;
			case	SimpleTypes::shapetypeMoon	:					return	SimpleTypes::Vml::sptMoon	;
			case	SimpleTypes::shapetypeBracketPair	:			return	SimpleTypes::Vml::sptBracketPair	;
			case	SimpleTypes::shapetypeBracePair	:				return	SimpleTypes::Vml::sptBracePair	;
			case	SimpleTypes::shapetypeStar4	:					return	SimpleTypes::Vml::sptSeal4	;
			case	SimpleTypes::shapetypeDoubleWave	:			return	SimpleTypes::Vml::sptDoubleWave	;
			case	SimpleTypes::shapetypeActionButtonBlank	:		return	SimpleTypes::Vml::sptActionButtonBlank	;
			case	SimpleTypes::shapetypeActionButtonHome	:		return	SimpleTypes::Vml::sptActionButtonHome	;
			case	SimpleTypes::shapetypeActionButtonHelp	:		return	SimpleTypes::Vml::sptActionButtonHelp	;
			case	SimpleTypes::shapetypeActionButtonInformation	:	return	SimpleTypes::Vml::sptActionButtonInformation	;
			case	SimpleTypes::shapetypeActionButtonForwardNext	:	return	SimpleTypes::Vml::sptActionButtonForwardNext	;
			case	SimpleTypes::shapetypeActionButtonBackPrevious	:	return	SimpleTypes::Vml::sptActionButtonBackPrevious	;
			case	SimpleTypes::shapetypeActionButtonEnd	:			return	SimpleTypes::Vml::sptActionButtonEnd	;
			case	SimpleTypes::shapetypeActionButtonBeginning	:		return	SimpleTypes::Vml::sptActionButtonBeginning	;
			case	SimpleTypes::shapetypeActionButtonReturn	:		return	SimpleTypes::Vml::sptActionButtonReturn	;
			case	SimpleTypes::shapetypeActionButtonDocument	:		return	SimpleTypes::Vml::sptActionButtonDocument	;
			case	SimpleTypes::shapetypeActionButtonSound	:			return	SimpleTypes::Vml::sptActionButtonSound	;
			case	SimpleTypes::shapetypeActionButtonMovie	:			return	SimpleTypes::Vml::sptActionButtonMovie	;
		default:
			return SimpleTypes::Vml::sptNotPrimitive;
		}
	}

	static SimpleTypes::Vml::SptType PrstTx2VmlShapeType(SimpleTypes::ETextShapeType type) 
	{
		switch(type)
		{
	//		case SimpleTypes::textshapetypeTextArchDown:		return	SimpleTypes::Vml::sptTextArchDown;
			case SimpleTypes::textshapetypeTextArchDownPour:	return	SimpleTypes::Vml::sptTextArchDownPour ;
	//		case SimpleTypes::textshapetypeTextArchUp:			return	SimpleTypes::Vml::sptTextArchUp ;
			case SimpleTypes::textshapetypeTextArchUpPour:		return	SimpleTypes::Vml::sptTextArchUpPour ;
	//		case SimpleTypes::textshapetypeTextButton:			return	SimpleTypes::Vml::sptTextButton ;
			case SimpleTypes::textshapetypeTextButtonPour:		return	SimpleTypes::Vml::sptTextButtonPour ;
			case SimpleTypes::textshapetypeTextCanDown:			return	SimpleTypes::Vml::sptTextCanDown ;
			case SimpleTypes::textshapetypeTextCanUp:			return	SimpleTypes::Vml::sptTextCanUp ;
			case SimpleTypes::textshapetypeTextCascadeDown:		return	SimpleTypes::Vml::sptTextCascadeDown;
			case SimpleTypes::textshapetypeTextCascadeUp:		return	SimpleTypes::Vml::sptTextCascadeUp ;
			case SimpleTypes::textshapetypeTextChevron:			return	SimpleTypes::Vml::sptTextChevron ;
			case SimpleTypes::textshapetypeTextChevronInverted:	return	SimpleTypes::Vml::sptTextChevronInverted ;
	//		case SimpleTypes::textshapetypeTextCircle:			return	SimpleTypes::Vml::sptTextCircle ;
			case SimpleTypes::textshapetypeTextCirclePour:		return	SimpleTypes::Vml::sptTextCirclePour ;
			case SimpleTypes::textshapetypeTextCurveDown:		return	SimpleTypes::Vml::sptTextCurveDown ;
			case SimpleTypes::textshapetypeTextCurveUp:			return	SimpleTypes::Vml::sptTextCurveUp ;
			case SimpleTypes::textshapetypeTextDeflate:			return	SimpleTypes::Vml::sptTextDeflate ;
			case SimpleTypes::textshapetypeTextDeflateBottom:	return	SimpleTypes::Vml::sptTextDeflateBottom;
			case SimpleTypes::textshapetypeTextDeflateInflate:	return	SimpleTypes::Vml::sptTextDeflateInflate ;
	//		case SimpleTypes::textshapetypeTextDeflateInflateDeflate: return	SimpleTypes::Vml::sptDeflateInflateDeflate;
			case SimpleTypes::textshapetypeTextDeflateTop:		return	SimpleTypes::Vml::sptTextDeflateTop ;
	//		case SimpleTypes::textshapetypeTextDoubleWave1:		return	SimpleTypes::Vml::sptTextDoubleWave ;
			case SimpleTypes::textshapetypeTextFadeDown:		return	SimpleTypes::Vml::sptTextFadeDown ;
			case SimpleTypes::textshapetypeTextFadeLeft:		return	SimpleTypes::Vml::sptTextFadeLeft ;
			case SimpleTypes::textshapetypeTextFadeRight:		return	SimpleTypes::Vml::sptTextFadeRight ;
			case SimpleTypes::textshapetypeTextFadeUp:			return	SimpleTypes::Vml::sptTextFadeUp ;
			case SimpleTypes::textshapetypeTextInflate:			return	SimpleTypes::Vml::sptTextPlainText ;
			case SimpleTypes::textshapetypeTextInflateBottom:	return	SimpleTypes::Vml::sptTextPlainText ;
			case SimpleTypes::textshapetypeTextInflateTop:		return	SimpleTypes::Vml::sptTextInflateTop	;
			case SimpleTypes::textshapetypeTextPlain:			return	SimpleTypes::Vml::sptTextPlainText ;
			case SimpleTypes::textshapetypeTextRingInside:		return	SimpleTypes::Vml::sptTextRingInside ;
			case SimpleTypes::textshapetypeTextRingOutside:		return	SimpleTypes::Vml::sptTextRingOutside ;
			case SimpleTypes::textshapetypeTextSlantDown:		return	SimpleTypes::Vml::sptTextSlantDown ;
			case SimpleTypes::textshapetypeTextSlantUp:			return	SimpleTypes::Vml::sptTextSlantUp;
			case SimpleTypes::textshapetypeTextStop:			return	SimpleTypes::Vml::sptTextStop ;
			case SimpleTypes::textshapetypeTextTriangle:		return	SimpleTypes::Vml::sptTextTriangle ;
			case SimpleTypes::textshapetypeTextTriangleInverted:return	SimpleTypes::Vml::sptTextTriangleInverted ;
			case SimpleTypes::textshapetypeTextWave1:			return	SimpleTypes::Vml::sptTextWave1 ;
			case SimpleTypes::textshapetypeTextWave2:			return	SimpleTypes::Vml::sptTextWave2 ;
			case SimpleTypes::textshapetypeTextWave4:			return	SimpleTypes::Vml::sptTextWave4 ;
		default:
			return SimpleTypes::Vml::sptNotPrimitive;
		}
	}
}
namespace PPTX
{
	namespace Logic
	{		
		class Shape : public WrapperWritingElement
		{
		public:
			Shape();
			virtual ~Shape();			
			explicit Shape(XmlUtils::CXmlNode& node);
			const Shape& operator =(XmlUtils::CXmlNode& node);

			virtual OOX::EElementType getType () const
			{
				return OOX::et_p_Shape;
			}

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			std::wstring GetText()const{if(txBody.IsInit()) return txBody->GetText(); return _T(""); };

			void GetShapeFullDescription(Shape& shape, int level = 0)const;
			void GetRect(Aggplus::RECT& pRect)const;
			DWORD GetFill(UniFill& fill)const;
			DWORD GetLine(Ln& line)const;

			void FillShapeProperties(ShapeProperties& props);
			void FillShapeTextProperties(CShapeTextProperties& props);

		private:
			void FillLevelUp()const;
			mutable Shape const * levelUp;
			void Merge(Shape& shape, bool bIsSlidePlaceholder = false)const;
		public:
			void SetLevelUpElement(const Shape& p)const{levelUp = &p;};

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(SPTREE_TYPE_SHAPE);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, attrUseBgFill);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, nvSpPr);
				pWriter->WriteRecord1(1, spPr);
				pWriter->WriteRecord2(2, style);

				if (pWriter->m_pMainDocument != NULL)
				{
					if (TextBoxShape.is_init())
					{					
                        long lDataSize = 0;
                        ULONG lPos = pWriter->GetPosition();
						pWriter->SetPosition(lPos);
						pWriter->StartRecord(4);
						pWriter->m_pMainDocument->getBinaryContent(TextBoxShape.get(), *pWriter, lDataSize);
						pWriter->EndRecord();
						
						if (TextBoxBodyPr.is_init())
						{
							pWriter->StartRecord(5);
							TextBoxBodyPr->toPPTY(pWriter);
							pWriter->EndRecord();
						}
					}
					else if (txBody.is_init())
					{
						std::wstring strContent = txBody->GetDocxTxBoxContent(pWriter, style);

                        long lDataSize = 0;
                        ULONG lPos = pWriter->GetPosition();
						pWriter->SetPosition(lPos);
						pWriter->StartRecord(4);
						pWriter->m_pMainDocument->getBinaryContent(strContent, *pWriter, lDataSize);
						pWriter->EndRecord();

						pWriter->WriteRecord1(5, txBody->bodyPr);
					}
				}
				else
				{
					pWriter->WriteRecord2(3, txBody);
				}

				pWriter->WriteRecord2(6, txXfrm);
				
				pWriter->EndRecord();
			}

			void toXmlWriterVML(NSBinPptxRW::CXmlWriter* pWriter, smart_ptr<PPTX::WrapperFile>& oTheme, smart_ptr<PPTX::WrapperWritingElement>& oClrMap, bool in_group = false);
			void toXmlWriterVMLBackground(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::WrapperFile>& oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& oClrMap);

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
					pWriter->StartNode(_T("wps:wsp"));
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->StartNode(_T("xdr:sp"));
				else
					pWriter->StartNode(_T("p:sp"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("useBgFill"), attrUseBgFill);
				pWriter->EndAttributes();

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
				{
					//nvSpPr.cNvPr.toXmlWriter2(_T("wps"), pWriter);
					nvSpPr.cNvSpPr.toXmlWriter2(_T("wps"), pWriter);
				}
				else
					nvSpPr.toXmlWriter(pWriter);
				
				bool bIsPresentStyle = false;
				if (style.is_init() && (style->fillRef.idx.is_init() || style->fillRef.Color.Color.is_init()))
				{
					bIsPresentStyle = true;
				}
				if (pWriter->m_lGroupIndex > 1 && !bIsPresentStyle)
				{
					pWriter->m_lFlag += 0x02;
				}
				spPr.toXmlWriter(pWriter);
				if (pWriter->m_lGroupIndex > 1 && !bIsPresentStyle)
				{
					pWriter->m_lFlag -= 0x02;
				}

				if (style.is_init())
				{
					if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
						style->m_ns = _T("wps");
					else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
						style->m_ns = _T("xdr");

                    pWriter->Write(style);
                }

				if (pWriter->m_lDocType != XMLWRITER_DOC_TYPE_DOCX)
				{
					if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX && txBody.is_init())
						txBody->m_ns = _T("xdr");
					pWriter->Write(txBody);
				}

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
				{	
					bool bIsWritedBodyPr = false;
					if (TextBoxShape.is_init())
					{
						pWriter->WriteString(_T("<wps:txbx>"));
						pWriter->WriteString(*TextBoxShape);
						pWriter->WriteString(_T("</wps:txbx>"));

						if (TextBoxBodyPr.is_init())
						{
							TextBoxBodyPr->m_namespace = _T("wps");
							TextBoxBodyPr->toXmlWriter(pWriter);
							bIsWritedBodyPr = true;
						}
					}

                    if (!bIsWritedBodyPr)
					{
						pWriter->WriteString(_T("<wps:bodyPr rot=\"0\"><a:prstTxWarp prst=\"textNoShape\"><a:avLst/></a:prstTxWarp><a:noAutofit/></wps:bodyPr>"));
					}
				}

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
					pWriter->EndNode(_T("wps:wsp"));
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->EndNode(_T("xdr:sp"));
				else
					pWriter->EndNode(_T("p:sp"));
			}
			
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
            {
                LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							attrUseBgFill = pReader->GetBool();
							break;
						}
						default:
							break;
					}
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							nvSpPr.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							spPr.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							style = new ShapeStyle();
							style->m_ns = _T("p");
							style->fromPPTY(pReader);
							break;
						}
						case 3:
						{
							txBody = new TxBody();
							txBody->fromPPTY(pReader);
							break;
						}
						case 4:
						{
							if (NULL != pReader->m_pMainDocument)
							{
                                LONG lLenRec = pReader->GetLong();

                                LONG lPosition = pReader->GetPos();
                                LONG lSize_Reader = pReader->GetSize();
								BYTE* pData_Reader = pReader->GetData();
				
								std::wstring sXmlContent;
								pReader->m_pMainDocument->getXmlContent(*pReader, lLenRec, sXmlContent);

								std::wstring strC = _T("<w:txbxContent>");
								strC += sXmlContent;
								strC += _T("</w:txbxContent>");
								TextBoxShape = strC;

								//pReader->Seek(lPosition + lLenRec);
								pReader->Init(pData_Reader, lPosition + lLenRec, lSize_Reader - (lPosition + lLenRec));
							}
							else
							{
								pReader->SkipRecord();
							}
							break;
						}
						case 5:
						{
							TextBoxBodyPr = new PPTX::Logic::BodyPr();
							TextBoxBodyPr->fromPPTY(pReader);
							break;
						}
						case 6:
						{
							txXfrm = new PPTX::Logic::Xfrm();
							txXfrm->fromPPTY(pReader);
							break;
						}
						default:
						{
							break;
						}
					}
				}

				pReader->Seek(_end_rec);
			}


			NvSpPr					nvSpPr;
			SpPr					spPr;
			nullable<ShapeStyle>	style;
			nullable<TxBody>		txBody;
			nullable<Xfrm>			txXfrm;

			nullable_string			TextBoxShape;
			nullable<BodyPr>		TextBoxBodyPr;

			bool								isFontRefInSlide;
			mutable nullable<TextParagraphPr>	body[10];
 
			// Attributes
			nullable_bool			attrUseBgFill;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SHAPE_INCLUDE_H
