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

#include "../../../OOXML/Common/SimpleTypes_Vml.h"

namespace OOX
{
	static int VmlShapeType2PrstShape(SimpleTypes::Vml::SptType type) 
	{
		switch(type)
		{
		//case SimpleTypes::Vml::sptMin = 0,    
		//case SimpleTypes::Vml::sptNotPrimitive = case SimpleTypes::Vml::sptMin,    
		case SimpleTypes::Vml::sptRectangle : 				return SimpleTypes::shapetypeRect ;    
		case SimpleTypes::Vml::sptRoundRectangle : 			return SimpleTypes::shapetypeRoundRect ;    
		case SimpleTypes::Vml::sptEllipse : 				return SimpleTypes::shapetypeEllipse ;    
		case SimpleTypes::Vml::sptDiamond : 				return SimpleTypes::shapetypeDiamond ;    
		case SimpleTypes::Vml::sptIsocelesTriangle :		return SimpleTypes::shapetypeTriangle ;    
		case SimpleTypes::Vml::sptRightTriangle : 			return SimpleTypes::shapetypeRtTriangle ;    
		case SimpleTypes::Vml::sptParallelogram : 			return SimpleTypes::shapetypeParallelogram ;    
		case SimpleTypes::Vml::sptTrapezoid : 				return SimpleTypes::shapetypeTrapezoid ;    
		case SimpleTypes::Vml::sptHexagon : 				return SimpleTypes::shapetypeHexagon ;    
		case SimpleTypes::Vml::sptOctagon : 				return SimpleTypes::shapetypeOctagon ;    
		case SimpleTypes::Vml::sptPlus : 					return SimpleTypes::shapetypePlus ;    
		case SimpleTypes::Vml::sptStar : 					return SimpleTypes::shapetypeStar5 ;    
		case SimpleTypes::Vml::sptArrow : 					return SimpleTypes::shapetypeRightArrow ;    //???
		case SimpleTypes::Vml::sptThickArrow : 				return SimpleTypes::shapetypeThickArrow ;   
		case SimpleTypes::Vml::sptHomePlate : 				return SimpleTypes::shapetypeHomePlate ;    
		case SimpleTypes::Vml::sptCube : 					return SimpleTypes::shapetypeCube ;    
		case SimpleTypes::Vml::sptBalloon : 				return SimpleTypes::shapetypeBallon ;    
		case SimpleTypes::Vml::sptSeal : 					return SimpleTypes::shapetypeIrregularSeal1 ;    
		case SimpleTypes::Vml::sptArc : 					return SimpleTypes::shapetypeArc ;    
		case SimpleTypes::Vml::sptLine : 					return SimpleTypes::shapetypeLine ;    
		case SimpleTypes::Vml::sptPlaque : 					return SimpleTypes::shapetypePlaque ;   
		case SimpleTypes::Vml::sptCan : 					return SimpleTypes::shapetypeCan ;    
		case SimpleTypes::Vml::sptDonut : 					return SimpleTypes::shapetypeDonut ;    
		//case SimpleTypes::Vml::sptTextSimple : 			return SimpleTypes::shapetypeTextSimple ;    
		//case SimpleTypes::Vml::sptTextOctagon : 			return SimpleTypes::shapetypeTextOctagon ;    
		//case SimpleTypes::Vml::sptTextHexagon : 			return SimpleTypes::shapetypeTextHexagon ;    
		//case SimpleTypes::Vml::sptTextCurve : 			return SimpleTypes::shapetypeTextCurve ;    
		//case SimpleTypes::Vml::sptTextWave : 				return SimpleTypes::shapetypeTextWave ;    
		//case SimpleTypes::Vml::sptTextRing : 				return SimpleTypes::shapetypeTextRing  ;    
		//case SimpleTypes::Vml::sptTextOnCurve : 			return SimpleTypes::shapetypeTextOnCurve ;    
		//case SimpleTypes::Vml::sptTextOnRing : 			return SimpleTypes::shapetypeTextOnRing ;    
		case SimpleTypes::Vml::sptStraightConnector1 : 		return SimpleTypes::shapetypeStraightConnector1 ;    
		case SimpleTypes::Vml::sptBentConnector2 : 			return SimpleTypes::shapetypeBentConnector2  ;    
		case SimpleTypes::Vml::sptBentConnector3 : 			return SimpleTypes::shapetypeBentConnector3 ;    
		case SimpleTypes::Vml::sptBentConnector4 : 			return SimpleTypes::shapetypeBentConnector4 ;    
		case SimpleTypes::Vml::sptBentConnector5 : 			return SimpleTypes::shapetypeBentConnector5 ;    
		case SimpleTypes::Vml::sptCurvedConnector2 : 		return SimpleTypes::shapetypeCurvedConnector2 ;    
		case SimpleTypes::Vml::sptCurvedConnector3 : 		return SimpleTypes::shapetypeCurvedConnector3 ;    
		case SimpleTypes::Vml::sptCurvedConnector4 : 		return SimpleTypes::shapetypeCurvedConnector4 ;    
		case SimpleTypes::Vml::sptCurvedConnector5 : 		return SimpleTypes::shapetypeCurvedConnector5 ;    
		case SimpleTypes::Vml::sptCallout1 : 				return SimpleTypes::shapetypeCallout1  ;    
		case SimpleTypes::Vml::sptCallout2 : 				return SimpleTypes::shapetypeCallout2  ;    
		case SimpleTypes::Vml::sptCallout3 : 				return SimpleTypes::shapetypeCallout3  ;   
		case SimpleTypes::Vml::sptAccentCallout1 : 			return SimpleTypes::shapetypeAccentCallout1 ;    
		case SimpleTypes::Vml::sptAccentCallout2 : 			return SimpleTypes::shapetypeAccentCallout2 ;    
		case SimpleTypes::Vml::sptAccentCallout3 : 			return SimpleTypes::shapetypeAccentCallout3 ;    
		case SimpleTypes::Vml::sptBorderCallout1 : 			return SimpleTypes::shapetypeAccentCallout1 ;    
		case SimpleTypes::Vml::sptBorderCallout2 : 			return SimpleTypes::shapetypeAccentCallout2 ;    
		case SimpleTypes::Vml::sptBorderCallout3 : 			return SimpleTypes::shapetypeAccentCallout3 ;    
		case SimpleTypes::Vml::sptAccentBorderCallout1 : 	return SimpleTypes::shapetypeAccentBorderCallout1 ;    
		case SimpleTypes::Vml::sptAccentBorderCallout2 : 	return SimpleTypes::shapetypeAccentBorderCallout2 ;    
		case SimpleTypes::Vml::sptAccentBorderCallout3 : 	return SimpleTypes::shapetypeAccentBorderCallout3 ;    
		case SimpleTypes::Vml::sptRibbon : 					return SimpleTypes::shapetypeRibbon ;    
		case SimpleTypes::Vml::sptRibbon2 : 				return SimpleTypes::shapetypeRibbon2 ;    
		case SimpleTypes::Vml::sptChevron : 				return SimpleTypes::shapetypeChevron ;    
		case SimpleTypes::Vml::sptPentagon : 				return SimpleTypes::shapetypePentagon ;  
		case SimpleTypes::Vml::sptNoSmoking : 				return SimpleTypes::shapetypeNoSmoking ;    
		case SimpleTypes::Vml::sptSeal8 : 					return SimpleTypes::shapetypeStar8 ;    
		case SimpleTypes::Vml::sptSeal16 : 					return SimpleTypes::shapetypeStar16 ;    
		case SimpleTypes::Vml::sptSeal32 : 					return SimpleTypes::shapetypeStar32 ;    
		case SimpleTypes::Vml::sptWedgeRectCallout : 		return SimpleTypes::shapetypeWedgeRectCallout ;    
		case SimpleTypes::Vml::sptWedgeRRectCallout : 		return SimpleTypes::shapetypeWedgeRoundRectCallout ;    
		case SimpleTypes::Vml::sptWedgeEllipseCallout : 	return SimpleTypes::shapetypeWedgeEllipseCallout ;    
		case SimpleTypes::Vml::sptWave : 					return SimpleTypes::shapetypeWave ;    
		case SimpleTypes::Vml::sptFoldedCorner : 			return SimpleTypes::shapetypeFoldedCorner ;    
		case SimpleTypes::Vml::sptLeftArrow : 				return SimpleTypes::shapetypeLeftArrow ;    
		case SimpleTypes::Vml::sptDownArrow : 				return SimpleTypes::shapetypeDownArrow ;    
		case SimpleTypes::Vml::sptUpArrow : 				return SimpleTypes::shapetypeUpArrow  ;    
		case SimpleTypes::Vml::sptLeftRightArrow : 			return SimpleTypes::shapetypeLeftRightArrow ;   
		case SimpleTypes::Vml::sptUpDownArrow : 			return SimpleTypes::shapetypeUpDownArrow ;    
		case SimpleTypes::Vml::sptIrregularSeal1 : 			return SimpleTypes::shapetypeIrregularSeal1 ;    
		case SimpleTypes::Vml::sptIrregularSeal2 : 			return SimpleTypes::shapetypeIrregularSeal2 ;    
		case SimpleTypes::Vml::sptLightningBolt : 			return SimpleTypes::shapetypeLightningBolt ;    
		case SimpleTypes::Vml::sptHeart : 					return SimpleTypes::shapetypeHeart ;   
		case SimpleTypes::Vml::sptPictureFrame : 			return 3000;//SimpleTypes::shapetypeFrame ;   SimpleTypes::shapetypeRect ;//
		case SimpleTypes::Vml::sptQuadArrow : 				return SimpleTypes::shapetypeQuadArrow ;    
		case SimpleTypes::Vml::sptLeftArrowCallout : 		return SimpleTypes::shapetypeLeftArrowCallout ;    
		case SimpleTypes::Vml::sptRightArrowCallout : 		return SimpleTypes::shapetypeRightArrowCallout ;    
		case SimpleTypes::Vml::sptUpArrowCallout : 			return SimpleTypes::shapetypeUpArrowCallout ;    
		case SimpleTypes::Vml::sptDownArrowCallout : 		return SimpleTypes::shapetypeDownArrowCallout ;    
		case SimpleTypes::Vml::sptLeftRightArrowCallout : 	return SimpleTypes::shapetypeLeftRightArrowCallout ;    
		case SimpleTypes::Vml::sptUpDownArrowCallout : 		return SimpleTypes::shapetypeUpDownArrowCallout ;    
		case SimpleTypes::Vml::sptQuadArrowCallout : 		return SimpleTypes::shapetypeQuadArrowCallout ;    
		case SimpleTypes::Vml::sptBevel : 					return SimpleTypes::shapetypeBevel ;    
		case SimpleTypes::Vml::sptLeftBracket : 			return SimpleTypes::shapetypeLeftBracket ;    
		case SimpleTypes::Vml::sptRightBracket : 			return SimpleTypes::shapetypeRightBracket ;    
		case SimpleTypes::Vml::sptLeftBrace : 				return SimpleTypes::shapetypeLeftBrace ;    
		case SimpleTypes::Vml::sptRightBrace : 				return SimpleTypes::shapetypeRightBrace ;    
		case SimpleTypes::Vml::sptLeftUpArrow : 			return SimpleTypes::shapetypeLeftUpArrow ;    
		case SimpleTypes::Vml::sptBentUpArrow : 			return SimpleTypes::shapetypeBentUpArrow ;    
		case SimpleTypes::Vml::sptBentArrow : 				return SimpleTypes::shapetypeBentArrow ;    
		case SimpleTypes::Vml::sptSeal24 : 					return SimpleTypes::shapetypeStar24 ;    
		case SimpleTypes::Vml::sptStripedRightArrow : 		return SimpleTypes::shapetypeStripedRightArrow ;    
		case SimpleTypes::Vml::sptNotchedRightArrow : 		return SimpleTypes::shapetypeNotchedRightArrow ;    
		case SimpleTypes::Vml::sptBlockArc : 				return SimpleTypes::shapetypeBlockArc ;    
		case SimpleTypes::Vml::sptSmileyFace : 				return SimpleTypes::shapetypeSmileyFace ;    
		case SimpleTypes::Vml::sptVerticalScroll : 			return SimpleTypes::shapetypeVerticalScroll ;    
		case SimpleTypes::Vml::sptHorizontalScroll : 		return SimpleTypes::shapetypeHorizontalScroll ;    
		case SimpleTypes::Vml::sptCircularArrow : 			return SimpleTypes::shapetypeCircularArrow ;    
		//case SimpleTypes::Vml::sptNotchedCircularArrow : 	return SimpleTypes::shapetypeNotchedCircularArrow ;    
		case SimpleTypes::Vml::sptUturnArrow : 				return SimpleTypes::shapetypeUturnArrow ;    
		case SimpleTypes::Vml::sptCurvedRightArrow : 		return SimpleTypes::shapetypeCurvedRightArrow  ;    
		case SimpleTypes::Vml::sptCurvedLeftArrow : 		return SimpleTypes::shapetypeCurvedLeftArrow ;    
		case SimpleTypes::Vml::sptCurvedUpArrow : 			return SimpleTypes::shapetypeCurvedUpArrow ;    
		case SimpleTypes::Vml::sptCurvedDownArrow : 		return SimpleTypes::shapetypeCurvedDownArrow ;   
		case SimpleTypes::Vml::sptCloudCallout : 			return SimpleTypes::shapetypeCloudCallout ;    
		case SimpleTypes::Vml::sptEllipseRibbon : 			return SimpleTypes::shapetypeEllipseRibbon ;    
		case SimpleTypes::Vml::sptEllipseRibbon2 : 			return SimpleTypes::shapetypeEllipseRibbon2 ;    
		case SimpleTypes::Vml::sptFlowChartProcess : 		return SimpleTypes::shapetypeFlowChartProcess ;    
		case SimpleTypes::Vml::sptFlowChartDecision : 		return SimpleTypes::shapetypeFlowChartDecision ;    
		case SimpleTypes::Vml::sptFlowChartInputOutput : 	return SimpleTypes::shapetypeFlowChartInputOutput ;    
		case SimpleTypes::Vml::sptFlowChartPredefinedProcess:return SimpleTypes::shapetypeFlowChartPredefinedProcess ;    
		case SimpleTypes::Vml::sptFlowChartInternalStorage : return SimpleTypes::shapetypeFlowChartInternalStorage ;    
		case SimpleTypes::Vml::sptFlowChartDocument : 		return SimpleTypes::shapetypeFlowChartDocument ;    
		case SimpleTypes::Vml::sptFlowChartMultidocument : 	return SimpleTypes::shapetypeFlowChartMultidocument ;    
		case SimpleTypes::Vml::sptFlowChartTerminator : 	return SimpleTypes::shapetypeFlowChartTerminator ;    
		case SimpleTypes::Vml::sptFlowChartPreparation : 	return SimpleTypes::shapetypeFlowChartPreparation ;    
		case SimpleTypes::Vml::sptFlowChartManualInput : 	return SimpleTypes::shapetypeFlowChartManualInput ;   
		case SimpleTypes::Vml::sptFlowChartManualOperation : return SimpleTypes::shapetypeFlowChartManualOperation ;   
		case SimpleTypes::Vml::sptFlowChartConnector : 		return SimpleTypes::shapetypeFlowChartConnector  ;    
		case SimpleTypes::Vml::sptFlowChartPunchedCard : 	return SimpleTypes::shapetypeFlowChartPunchedCard ;    
		case SimpleTypes::Vml::sptFlowChartPunchedTape : 	return SimpleTypes::shapetypeFlowChartPunchedTape  ;    
		case SimpleTypes::Vml::sptFlowChartSummingJunction :return SimpleTypes::shapetypeFlowChartSummingJunction ;   
		case SimpleTypes::Vml::sptFlowChartOr : 			return SimpleTypes::shapetypeFlowChartOr ;    
		case SimpleTypes::Vml::sptFlowChartCollate : 		return SimpleTypes::shapetypeFlowChartCollate  ;    
		case SimpleTypes::Vml::sptFlowChartSort : 			return SimpleTypes::shapetypeFlowChartSort ;    
		case SimpleTypes::Vml::sptFlowChartExtract : 		return SimpleTypes::shapetypeFlowChartExtract ;    
		case SimpleTypes::Vml::sptFlowChartMerge : 			return SimpleTypes::shapetypeFlowChartMerge ;    
		//case SimpleTypes::Vml::sptFlowChartOfflineStorage : 	return SimpleTypes::shapetypeChartOfflineStorage ;    
		case SimpleTypes::Vml::sptFlowChartOnlineStorage : 	return SimpleTypes::shapetypeFlowChartOnlineStorage ;    
		case SimpleTypes::Vml::sptFlowChartMagneticTape :	return SimpleTypes::shapetypeFlowChartMagneticTape ;    
		case SimpleTypes::Vml::sptFlowChartMagneticDisk :	return SimpleTypes::shapetypeFlowChartMagneticDisk ;    
		case SimpleTypes::Vml::sptFlowChartMagneticDrum :	return SimpleTypes::shapetypeFlowChartMagneticDrum ;    
		case SimpleTypes::Vml::sptFlowChartDisplay : 		return SimpleTypes::shapetypeFlowChartDisplay ;    
		case SimpleTypes::Vml::sptFlowChartDelay : 			return SimpleTypes::shapetypeFlowChartDelay ;   
	//////////////////////////////////////////////////////////////////////////////
		case SimpleTypes::Vml::sptTextPlainText : 			return SimpleTypes::textshapetypeTextPlain			+2001;    
		case SimpleTypes::Vml::sptTextStop : 				return SimpleTypes::textshapetypeTextStop			+2001;    
		case SimpleTypes::Vml::sptTextTriangle : 			return SimpleTypes::textshapetypeTextTriangle		+2001;    
		case SimpleTypes::Vml::sptTextTriangleInverted :	return SimpleTypes::textshapetypeTextTriangleInverted+2001;    
		case SimpleTypes::Vml::sptTextChevron : 			return SimpleTypes::textshapetypeTextChevron 		+2001;    
		case SimpleTypes::Vml::sptTextChevronInverted : 	return SimpleTypes::textshapetypeTextChevronInverted+2001;    
		case SimpleTypes::Vml::sptTextRingInside : 			return SimpleTypes::textshapetypeTextRingInside 	+2001;    
		case SimpleTypes::Vml::sptTextRingOutside : 		return SimpleTypes::textshapetypeTextRingOutside 	+2001;    
		case SimpleTypes::Vml::sptTextArchUpCurve : 		return SimpleTypes::textshapetypeTextArchUp			+2001;    
		case SimpleTypes::Vml::sptTextArchDownCurve : 		return SimpleTypes::textshapetypeTextArchDown		+2001;    
		case SimpleTypes::Vml::sptTextCircleCurve : 		return SimpleTypes::textshapetypeTextCircle			+2001;    
		case SimpleTypes::Vml::sptTextButtonCurve : 		return SimpleTypes::textshapetypeTextButton			+2001;    
		case SimpleTypes::Vml::sptTextArchUpPour : 			return SimpleTypes::textshapetypeTextArchUpPour		+2001;    
		case SimpleTypes::Vml::sptTextArchDownPour : 		return SimpleTypes::textshapetypeTextArchDownPour	+2001;    
		case SimpleTypes::Vml::sptTextCirclePour : 			return SimpleTypes::textshapetypeTextCirclePour		+2001;    
		case SimpleTypes::Vml::sptTextButtonPour : 			return SimpleTypes::textshapetypeTextButtonPour		+2001;    
		case SimpleTypes::Vml::sptTextCurveUp : 			return SimpleTypes::textshapetypeTextCurveUp		+2001;    
		case SimpleTypes::Vml::sptTextCurveDown : 			return SimpleTypes::textshapetypeTextCurveDown		+2001;   
		case SimpleTypes::Vml::sptTextCascadeUp : 			return SimpleTypes::textshapetypeTextCascadeUp		+2001;   
		case SimpleTypes::Vml::sptTextCascadeDown : 		return SimpleTypes::textshapetypeTextCascadeDown	+2001;   
		case SimpleTypes::Vml::sptTextWave1 : 				return SimpleTypes::textshapetypeTextWave1			+2001;    
		case SimpleTypes::Vml::sptTextWave2 : 				return SimpleTypes::textshapetypeTextWave2			+2001;    
		//case SimpleTypes::Vml::sptTextWave3 : 			return SimpleTypes::shapetypeTextWave3 ;    
		case SimpleTypes::Vml::sptTextWave4 : 				return SimpleTypes::textshapetypeTextWave4			+2001;    
		case SimpleTypes::Vml::sptTextInflate : 			return SimpleTypes::textshapetypeTextInflate 		+2001;    
		case SimpleTypes::Vml::sptTextDeflate : 			return SimpleTypes::textshapetypeTextDeflate 		+2001;  

		//case SimpleTypes::Vml::sptTextInflateBottom : 		return SimpleTypes::shapetypeTextInflateBottom ;    
		//case SimpleTypes::Vml::sptTextDeflateBottom : 		return SimpleTypes::shapetypeTextDeflateBottom ;    
		//case SimpleTypes::Vml::sptTextInflateTop : 			return SimpleTypes::shapetypeTextInflateTop ; 
		//case SimpleTypes::Vml::sptTextDeflateTop : 			return SimpleTypes::shapetypeTextDeflateTop ;    
		//case SimpleTypes::Vml::sptTextDeflateInflate : 		return SimpleTypes::shapetypeTextDeflateInflate ;    
		//case SimpleTypes::Vml::sptTextDeflateInflateDeflate :	return SimpleTypes::shapetypeTextDeflateInflateDeflat ;    
		//case SimpleTypes::Vml::sptTextFadeRight : 			return SimpleTypes::shapetypeTextFadeRight  ;    
		//case SimpleTypes::Vml::sptTextFadeLeft : 				return SimpleTypes::shapetypeTextFadeLeft ;    
		//case SimpleTypes::Vml::sptTextFadeUp : 				return SimpleTypes::shapetypeTextFadeUp ;    
		//case SimpleTypes::Vml::sptTextFadeDown : 				return SimpleTypes::shapetypeTextFadeDown ;    
		case SimpleTypes::Vml::sptTextSlantUp : 				return SimpleTypes::textshapetypeTextSlantUp 	+2001;   
		case SimpleTypes::Vml::sptTextSlantDown : 				return SimpleTypes::textshapetypeTextSlantDown 	+2001;  
		case SimpleTypes::Vml::sptTextCanUp : 					return SimpleTypes::textshapetypeTextCanUp	 	+2001; 
		case SimpleTypes::Vml::sptTextCanDown : 				return SimpleTypes::textshapetypeTextCanDown 	+2001;  
		
		case SimpleTypes::Vml::sptFlowChartAlternateProcess :	return SimpleTypes::shapetypeFlowChartAlternateProcess ;    
		case SimpleTypes::Vml::sptFlowChartOffpageConnector :	return SimpleTypes::shapetypeFlowChartOffpageConnector ;    
		case SimpleTypes::Vml::sptCallout90 : 					return SimpleTypes::shapetypeCallout1 ;    
		case SimpleTypes::Vml::sptAccentCallout90 : 			return SimpleTypes::shapetypeAccentCallout1 ;    
		case SimpleTypes::Vml::sptBorderCallout90 : 			return SimpleTypes::shapetypeBorderCallout1;    
		case SimpleTypes::Vml::sptAccentBorderCallout90 : 		return SimpleTypes::shapetypeAccentBorderCallout1 ;   
		case SimpleTypes::Vml::sptLeftRightUpArrow : 			return SimpleTypes::shapetypeLeftRightUpArrow ;    
		case SimpleTypes::Vml::sptSun : 						return SimpleTypes::shapetypeSun ;    
		case SimpleTypes::Vml::sptMoon : 						return SimpleTypes::shapetypeMoon ;    
		case SimpleTypes::Vml::sptBracketPair : 				return SimpleTypes::shapetypeBracketPair ;    
		case SimpleTypes::Vml::sptBracePair : 					return SimpleTypes::shapetypeBracePair ;    
		case SimpleTypes::Vml::sptSeal4 : 						return SimpleTypes::shapetypeStar4 ;    
		case SimpleTypes::Vml::sptDoubleWave : 					return SimpleTypes::shapetypeDoubleWave ;    
		case SimpleTypes::Vml::sptActionButtonBlank : 			return SimpleTypes::shapetypeActionButtonBlank ;    
		case SimpleTypes::Vml::sptActionButtonHome : 			return SimpleTypes::shapetypeActionButtonHome ;    
		case SimpleTypes::Vml::sptActionButtonHelp : 			return SimpleTypes::shapetypeActionButtonHelp ;    
		case SimpleTypes::Vml::sptActionButtonInformation : 	return SimpleTypes::shapetypeActionButtonInformation ;    
		case SimpleTypes::Vml::sptActionButtonForwardNext : 	return SimpleTypes::shapetypeActionButtonForwardNext ;    
		case SimpleTypes::Vml::sptActionButtonBackPrevious :	return SimpleTypes::shapetypeActionButtonBackPrevious  ;   
		case SimpleTypes::Vml::sptActionButtonEnd : 			return SimpleTypes::shapetypeActionButtonEnd ;    
		case SimpleTypes::Vml::sptActionButtonBeginning : 		return SimpleTypes::shapetypeActionButtonBeginning ;    
		case SimpleTypes::Vml::sptActionButtonReturn : 			return SimpleTypes::shapetypeActionButtonReturn ;    
		case SimpleTypes::Vml::sptActionButtonDocument : 		return SimpleTypes::shapetypeActionButtonDocument ;    
		case SimpleTypes::Vml::sptActionButtonSound : 			return SimpleTypes::shapetypeActionButtonSound ;    
		case SimpleTypes::Vml::sptActionButtonMovie : 			return SimpleTypes::shapetypeActionButtonMovie ;   
		//case SimpleTypes::Vml::sptHostControl : 				return SimpleTypes::shapetype ;    
		case SimpleTypes::Vml::sptTextBox : 					return 2000;
		default:
			return SimpleTypes::shapetypeRect;

		}
	}

}
