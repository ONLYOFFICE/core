#pragma once

#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Drawing.h"

namespace OOX
{
	namespace Vml
	{
		enum SptType
		{    
			sptMin = 0,    
			sptNotPrimitive = sptMin,    
			sptRectangle = 1,    
			sptRoundRectangle = 2,    
			sptEllipse = 3,    
			sptDiamond = 4,    
			sptIsocelesTriangle = 5,    
			sptRightTriangle = 6,    
			sptParallelogram = 7,    
			sptTrapezoid = 8,    
			sptHexagon = 9,    
			sptOctagon = 10,    
			sptPlus = 11,    
			sptStar = 12,    
			sptArrow = 13,    
			sptThickArrow = 14,   
			sptHomePlate = 15,    
			sptCube = 16,    
			sptBalloon = 17,    
			sptSeal = 18,    
			sptArc = 19,    
			sptLine = 20,    
			sptPlaque = 21,   
			sptCan = 22,    
			sptDonut = 23,    
			sptTextSimple = 24,    
			sptTextOctagon = 25,    
			sptTextHexagon = 26,    
			sptTextCurve = 27,    
			sptTextWave = 28,    
			sptTextRing = 29,    
			sptTextOnCurve = 30,    
			sptTextOnRing = 31,    
			sptStraightConnector1 = 32,    
			sptBentConnector2 = 33,    
			sptBentConnector3 = 34,    
			sptBentConnector4 = 35,    
			sptBentConnector5 = 36,    
			sptCurvedConnector2 = 37,    
			sptCurvedConnector3 = 38,    
			sptCurvedConnector4 = 39,    
			sptCurvedConnector5 = 40,    
			sptCallout1 = 41,    
			sptCallout2 = 42,    
			sptCallout3 = 43,   
			sptAccentCallout1 = 44,    
			sptAccentCallout2 = 45,    
			sptAccentCallout3 = 46,    
			sptBorderCallout1 = 47,    
			sptBorderCallout2 = 48,    
			sptBorderCallout3 = 49,    
			sptAccentBorderCallout1 = 50,    
			sptAccentBorderCallout2 = 51,    
			sptAccentBorderCallout3 = 52,    
			sptRibbon = 53,    
			sptRibbon2 = 54,    
			sptChevron = 55,    
			sptPentagon = 56,  
			sptNoSmoking = 57,    
			sptSeal8 = 58,    
			sptSeal16 = 59,    
			sptSeal32 = 60,    
			sptWedgeRectCallout = 61,    
			sptWedgeRRectCallout = 62,    
			sptWedgeEllipseCallout = 63,    
			sptWave = 64,    
			sptFoldedCorner = 65,    
			sptLeftArrow = 66,    
			sptDownArrow = 67,    
			sptUpArrow = 68,    
			sptLeftRightArrow = 69,   
			sptUpDownArrow = 70,    
			sptIrregularSeal1 = 71,    
			sptIrregularSeal2 = 72,    
			sptLightningBolt = 73,    
			sptHeart = 74,   
			sptPictureFrame = 75,    
			sptQuadArrow = 76,    
			sptLeftArrowCallout = 77,    
			sptRightArrowCallout = 78,    
			sptUpArrowCallout = 79,    
			sptDownArrowCallout = 80,    
			sptLeftRightArrowCallout = 81,    
			sptUpDownArrowCallout = 82,    
			sptQuadArrowCallout = 83,    
			sptBevel = 84,    
			sptLeftBracket = 85,    
			sptRightBracket = 86,    
			sptLeftBrace = 87,    
			sptRightBrace = 88,    
			sptLeftUpArrow = 89,    
			sptBentUpArrow = 90,    
			sptBentArrow = 91,    
			sptSeal24 = 92,    
			sptStripedRightArrow = 93,    
			sptNotchedRightArrow = 94,    
			sptBlockArc = 95,    
			sptSmileyFace = 96,    
			sptVerticalScroll = 97,    
			sptHorizontalScroll = 98,    
			sptCircularArrow = 99,    
			sptNotchedCircularArrow = 100,    
			sptUturnArrow = 101,    
			sptCurvedRightArrow = 102,    
			sptCurvedLeftArrow = 103,    
			sptCurvedUpArrow = 104,    
			sptCurvedDownArrow = 105,   
			sptCloudCallout = 106,    
			sptEllipseRibbon = 107,    
			sptEllipseRibbon2 = 108,    
			sptFlowChartProcess = 109,    
			sptFlowChartDecision = 110,    
			sptFlowChartInputOutput = 111,    
			sptFlowChartPredefinedProcess = 112,    
			sptFlowChartInternalStorage = 113,    
			sptFlowChartDocument = 114,    
			sptFlowChartMultidocument = 115,    
			sptFlowChartTerminator = 116,    
			sptFlowChartPreparation = 117,    
			sptFlowChartManualInput = 118,   
			sptFlowChartManualOperation = 119,   
			sptFlowChartConnector = 120,    
			sptFlowChartPunchedCard = 121,    
			sptFlowChartPunchedTape = 122,    
			sptFlowChartSummingJunction = 123,   
			sptFlowChartOr = 124,    
			sptFlowChartCollate = 125,    
			sptFlowChartSort = 126,    
			sptFlowChartExtract = 127,    
			sptFlowChartMerge = 128,    
			sptFlowChartOfflineStorage = 129,    
			sptFlowChartOnlineStorage = 130,    
			sptFlowChartMagneticTape = 131,    
			sptFlowChartMagneticDisk = 132,    
			sptFlowChartMagneticDrum = 133,    
			sptFlowChartDisplay = 134,    
			sptFlowChartDelay = 135,   
			sptTextPlainText = 136,    
			sptTextStop = 137,    
			sptTextTriangle = 138,    
			sptTextTriangleInverted = 139,    
			sptTextChevron = 140,    
			sptTextChevronInverted = 141,    
			sptTextRingInside = 142,    
			sptTextRingOutside = 143,    
			sptTextArchUpCurve = 144,    
			sptTextArchDownCurve = 145,    
			sptTextCircleCurve = 146,    
			sptTextButtonCurve = 147,    
			sptTextArchUpPour = 148,    
			sptTextArchDownPour = 149,    
			sptTextCirclePour = 150,    
			sptTextButtonPour = 151,    
			sptTextCurveUp = 152,    
			sptTextCurveDown = 153,   
			sptTextCascadeUp = 154,   
			sptTextCascadeDown = 155,   
			sptTextWave1 = 156,    
			sptTextWave2 = 157,    
			sptTextWave3 = 158,    
			sptTextWave4 = 159,    
			sptTextInflate = 160,    
			sptTextDeflate = 161,    
			sptTextInflateBottom = 162,    
			sptTextDeflateBottom = 163,    
			sptTextInflateTop = 164, 
			sptTextDeflateTop = 165,    
			sptTextDeflateInflate = 166,    
			sptTextDeflateInflateDeflate = 167,    
			sptTextFadeRight = 168,    
			sptTextFadeLeft = 169,    
			sptTextFadeUp = 170,    
			sptTextFadeDown = 171,    
			sptTextSlantUp = 172,    
			sptTextSlantDown = 173,    
			sptTextCanUp = 174,    
			sptTextCanDown = 175,    
			sptFlowChartAlternateProcess = 176,    
			sptFlowChartOffpageConnector = 177,    
			sptCallout90 = 178,    
			sptAccentCallout90 = 179,    
			sptBorderCallout90 = 180,    
			sptAccentBorderCallout90 = 181,   
			sptLeftRightUpArrow = 182,    
			sptSun = 183,    
			sptMoon = 184,    
			sptBracketPair = 185,    
			sptBracePair = 186,    
			sptSeal4 = 187,    
			sptDoubleWave = 188,    
			sptActionButtonBlank = 189,    
			sptActionButtonHome = 190,    
			sptActionButtonHelp = 191,    
			sptActionButtonInformation = 192,    
			sptActionButtonForwardNext = 193,    
			sptActionButtonBackPrevious = 194,   
			sptActionButtonEnd = 195,    
			sptActionButtonBeginning = 196,    
			sptActionButtonReturn = 197,    
			sptActionButtonDocument = 198,    
			sptActionButtonSound = 199,    
			sptActionButtonMovie = 200,   
			sptHostControl = 201,    
			sptTextBox = 202,   
			sptMax,    
			sptNil = 0x0FFF
		};
	}
	int static Spt2ShapeType(Vml::SptType type) 
	{
		switch(type)
		{
		//case Vml::sptMin = 0,    
		//case Vml::sptNotPrimitive = case Vml::sptMin,    
		case Vml::sptRectangle : 				return SimpleTypes::shapetypeRect ;    
		case Vml::sptRoundRectangle : 			return SimpleTypes::shapetypeRoundRect ;    
		case Vml::sptEllipse : 					return SimpleTypes::shapetypeEllipse ;    
		case Vml::sptDiamond : 					return SimpleTypes::shapetypeDiamond ;    
		case Vml::sptIsocelesTriangle :			return SimpleTypes::shapetypeTriangle ;    
		case Vml::sptRightTriangle : 			return SimpleTypes::shapetypeRtTriangle ;    
		case Vml::sptParallelogram : 			return SimpleTypes::shapetypeParallelogram ;    
		case Vml::sptTrapezoid : 				return SimpleTypes::shapetypeTrapezoid ;    
		case Vml::sptHexagon : 					return SimpleTypes::shapetypeHexagon ;    
		case Vml::sptOctagon : 					return SimpleTypes::shapetypeOctagon ;    
		case Vml::sptPlus : 					return SimpleTypes::shapetypePlus ;    
		case Vml::sptStar : 					return SimpleTypes::shapetypeStar5 ;    
		case Vml::sptArrow : 					return SimpleTypes::shapetypeRightArrow ;    //???
		case Vml::sptThickArrow : 				return SimpleTypes::shapetypeThickArrow ;   
		case Vml::sptHomePlate : 				return SimpleTypes::shapetypeHomePlate ;    
		case Vml::sptCube : 					return SimpleTypes::shapetypeCube ;    
		case Vml::sptBalloon : 					return SimpleTypes::shapetypeBallon ;    
		case Vml::sptSeal : 					return SimpleTypes::shapetypeIrregularSeal1 ;    
		case Vml::sptArc : 						return SimpleTypes::shapetypeArc ;    
		case Vml::sptLine : 					return SimpleTypes::shapetypeLine ;    
		case Vml::sptPlaque : 					return SimpleTypes::shapetypePlaque ;   
		case Vml::sptCan : 						return SimpleTypes::shapetypeCan ;    
		case Vml::sptDonut : 					return SimpleTypes::shapetypeDonut ;    
		//case Vml::sptTextSimple : 				return SimpleTypes::shapetypeTextSimple ;    
		//case Vml::sptTextOctagon : 				return SimpleTypes::shapetypeTextOctagon ;    
		//case Vml::sptTextHexagon : 				return SimpleTypes::shapetypeTextHexagon ;    
		//case Vml::sptTextCurve : 				return SimpleTypes::shapetypeTextCurve ;    
		//case Vml::sptTextWave : 				return SimpleTypes::shapetypeTextWave ;    
		//case Vml::sptTextRing : 				return SimpleTypes::shapetypeTextRing  ;    
		//case Vml::sptTextOnCurve : 				return SimpleTypes::shapetypeTextOnCurve ;    
		//case Vml::sptTextOnRing : 				return SimpleTypes::shapetypeTextOnRing ;    
		case Vml::sptStraightConnector1 : 		return SimpleTypes::shapetypeStraightConnector1 ;    
		case Vml::sptBentConnector2 : 			return SimpleTypes::shapetypeBentConnector2  ;    
		case Vml::sptBentConnector3 : 			return SimpleTypes::shapetypeBentConnector3 ;    
		case Vml::sptBentConnector4 : 			return SimpleTypes::shapetypeBentConnector4 ;    
		case Vml::sptBentConnector5 : 			return SimpleTypes::shapetypeBentConnector5 ;    
		case Vml::sptCurvedConnector2 : 		return SimpleTypes::shapetypeCurvedConnector2 ;    
		case Vml::sptCurvedConnector3 : 		return SimpleTypes::shapetypeCurvedConnector3 ;    
		case Vml::sptCurvedConnector4 : 		return SimpleTypes::shapetypeCurvedConnector4 ;    
		case Vml::sptCurvedConnector5 : 		return SimpleTypes::shapetypeCurvedConnector5 ;    
		case Vml::sptCallout1 : 				return SimpleTypes::shapetypeCallout1  ;    
		case Vml::sptCallout2 : 				return SimpleTypes::shapetypeCallout2  ;    
		case Vml::sptCallout3 : 				return SimpleTypes::shapetypeCallout3  ;   
		case Vml::sptAccentCallout1 : 			return SimpleTypes::shapetypeAccentCallout1 ;    
		case Vml::sptAccentCallout2 : 			return SimpleTypes::shapetypeAccentCallout2 ;    
		case Vml::sptAccentCallout3 : 			return SimpleTypes::shapetypeAccentCallout3 ;    
		case Vml::sptBorderCallout1 : 			return SimpleTypes::shapetypeAccentCallout1 ;    
		case Vml::sptBorderCallout2 : 			return SimpleTypes::shapetypeAccentCallout2 ;    
		case Vml::sptBorderCallout3 : 			return SimpleTypes::shapetypeAccentCallout3 ;    
		case Vml::sptAccentBorderCallout1 : 	return SimpleTypes::shapetypeAccentBorderCallout1 ;    
		case Vml::sptAccentBorderCallout2 : 	return SimpleTypes::shapetypeAccentBorderCallout2 ;    
		case Vml::sptAccentBorderCallout3 : 	return SimpleTypes::shapetypeAccentBorderCallout3 ;    
		case Vml::sptRibbon : 					return SimpleTypes::shapetypeRibbon ;    
		case Vml::sptRibbon2 : 					return SimpleTypes::shapetypeRibbon2 ;    
		case Vml::sptChevron : 					return SimpleTypes::shapetypeChevron ;    
		case Vml::sptPentagon : 				return SimpleTypes::shapetypePentagon ;  
		case Vml::sptNoSmoking : 				return SimpleTypes::shapetypeNoSmoking ;    
		case Vml::sptSeal8 : 					return SimpleTypes::shapetypeStar8 ;    
		case Vml::sptSeal16 : 					return SimpleTypes::shapetypeStar16 ;    
		case Vml::sptSeal32 : 					return SimpleTypes::shapetypeStar32 ;    
		case Vml::sptWedgeRectCallout : 		return SimpleTypes::shapetypeWedgeRectCallout ;    
		case Vml::sptWedgeRRectCallout : 		return SimpleTypes::shapetypeWedgeRoundRectCallout ;    
		case Vml::sptWedgeEllipseCallout : 		return SimpleTypes::shapetypeWedgeEllipseCallout ;    
		case Vml::sptWave : 					return SimpleTypes::shapetypeWave ;    
		case Vml::sptFoldedCorner : 			return SimpleTypes::shapetypeFoldedCorner ;    
		case Vml::sptLeftArrow : 				return SimpleTypes::shapetypeLeftArrow ;    
		case Vml::sptDownArrow : 				return SimpleTypes::shapetypeDownArrow ;    
		case Vml::sptUpArrow : 					return SimpleTypes::shapetypeUpArrow  ;    
		case Vml::sptLeftRightArrow : 			return SimpleTypes::shapetypeLeftRightArrow ;   
		case Vml::sptUpDownArrow : 				return SimpleTypes::shapetypeUpDownArrow ;    
		case Vml::sptIrregularSeal1 : 			return SimpleTypes::shapetypeIrregularSeal1 ;    
		case Vml::sptIrregularSeal2 : 			return SimpleTypes::shapetypeIrregularSeal2 ;    
		case Vml::sptLightningBolt : 			return SimpleTypes::shapetypeLightningBolt ;    
		case Vml::sptHeart : 					return SimpleTypes::shapetypeHeart ;   
		case Vml::sptPictureFrame : 			return 3000;//SimpleTypes::shapetypeFrame ;   SimpleTypes::shapetypeRect ;//
		case Vml::sptQuadArrow : 				return SimpleTypes::shapetypeQuadArrow ;    
		case Vml::sptLeftArrowCallout : 		return SimpleTypes::shapetypeLeftArrowCallout ;    
		case Vml::sptRightArrowCallout : 		return SimpleTypes::shapetypeRightArrowCallout ;    
		case Vml::sptUpArrowCallout : 			return SimpleTypes::shapetypeUpArrowCallout ;    
		case Vml::sptDownArrowCallout : 		return SimpleTypes::shapetypeDownArrowCallout ;    
		case Vml::sptLeftRightArrowCallout : 	return SimpleTypes::shapetypeLeftRightArrowCallout ;    
		case Vml::sptUpDownArrowCallout : 		return SimpleTypes::shapetypeUpDownArrowCallout ;    
		case Vml::sptQuadArrowCallout : 		return SimpleTypes::shapetypeQuadArrowCallout ;    
		case Vml::sptBevel : 					return SimpleTypes::shapetypeBevel ;    
		case Vml::sptLeftBracket : 				return SimpleTypes::shapetypeLeftBracket ;    
		case Vml::sptRightBracket : 			return SimpleTypes::shapetypeRightBracket ;    
		case Vml::sptLeftBrace : 				return SimpleTypes::shapetypeLeftBrace ;    
		case Vml::sptRightBrace : 				return SimpleTypes::shapetypeRightBrace ;    
		case Vml::sptLeftUpArrow : 				return SimpleTypes::shapetypeLeftUpArrow ;    
		case Vml::sptBentUpArrow : 				return SimpleTypes::shapetypeBentUpArrow ;    
		case Vml::sptBentArrow : 				return SimpleTypes::shapetypeBentArrow ;    
		case Vml::sptSeal24 : 					return SimpleTypes::shapetypeStar24 ;    
		case Vml::sptStripedRightArrow : 		return SimpleTypes::shapetypeStripedRightArrow ;    
		case Vml::sptNotchedRightArrow : 		return SimpleTypes::shapetypeNotchedRightArrow ;    
		case Vml::sptBlockArc : 				return SimpleTypes::shapetypeBlockArc ;    
		case Vml::sptSmileyFace : 				return SimpleTypes::shapetypeSmileyFace ;    
		case Vml::sptVerticalScroll : 			return SimpleTypes::shapetypeVerticalScroll ;    
		case Vml::sptHorizontalScroll : 		return SimpleTypes::shapetypeHorizontalScroll ;    
		case Vml::sptCircularArrow : 			return SimpleTypes::shapetypeCircularArrow ;    
		//case Vml::sptNotchedCircularArrow : 	return SimpleTypes::shapetypeNotchedCircularArrow ;    
		case Vml::sptUturnArrow : 				return SimpleTypes::shapetypeUturnArrow ;    
		case Vml::sptCurvedRightArrow : 		return SimpleTypes::shapetypeCurvedRightArrow  ;    
		case Vml::sptCurvedLeftArrow : 			return SimpleTypes::shapetypeCurvedLeftArrow ;    
		case Vml::sptCurvedUpArrow : 			return SimpleTypes::shapetypeCurvedUpArrow ;    
		case Vml::sptCurvedDownArrow : 			return SimpleTypes::shapetypeCurvedDownArrow ;   
		case Vml::sptCloudCallout : 			return SimpleTypes::shapetypeCloudCallout ;    
		case Vml::sptEllipseRibbon : 			return SimpleTypes::shapetypeEllipseRibbon ;    
		case Vml::sptEllipseRibbon2 : 			return SimpleTypes::shapetypeEllipseRibbon2 ;    
		case Vml::sptFlowChartProcess : 		return SimpleTypes::shapetypeFlowChartProcess ;    
		case Vml::sptFlowChartDecision : 		return SimpleTypes::shapetypeFlowChartDecision ;    
		case Vml::sptFlowChartInputOutput : 	return SimpleTypes::shapetypeFlowChartInputOutput ;    
		case Vml::sptFlowChartPredefinedProcess:return SimpleTypes::shapetypeFlowChartPredefinedProcess ;    
		case Vml::sptFlowChartInternalStorage : return SimpleTypes::shapetypeFlowChartInternalStorage ;    
		case Vml::sptFlowChartDocument : 		return SimpleTypes::shapetypeFlowChartDocument ;    
		case Vml::sptFlowChartMultidocument : 	return SimpleTypes::shapetypeFlowChartMultidocument ;    
		case Vml::sptFlowChartTerminator : 		return SimpleTypes::shapetypeFlowChartTerminator ;    
		case Vml::sptFlowChartPreparation : 	return SimpleTypes::shapetypeFlowChartPreparation ;    
		case Vml::sptFlowChartManualInput : 	return SimpleTypes::shapetypeFlowChartManualInput ;   
		case Vml::sptFlowChartManualOperation : return SimpleTypes::shapetypeFlowChartManualOperation ;   
		case Vml::sptFlowChartConnector : 		return SimpleTypes::shapetypeFlowChartConnector  ;    
		case Vml::sptFlowChartPunchedCard : 	return SimpleTypes::shapetypeFlowChartPunchedCard ;    
		case Vml::sptFlowChartPunchedTape : 	return SimpleTypes::shapetypeFlowChartPunchedTape  ;    
		case Vml::sptFlowChartSummingJunction :	return SimpleTypes::shapetypeFlowChartSummingJunction ;   
		case Vml::sptFlowChartOr : 				return SimpleTypes::shapetypeFlowChartOr ;    
		case Vml::sptFlowChartCollate : 		return SimpleTypes::shapetypeFlowChartCollate  ;    
		case Vml::sptFlowChartSort : 			return SimpleTypes::shapetypeFlowChartSort ;    
		case Vml::sptFlowChartExtract : 		return SimpleTypes::shapetypeFlowChartExtract ;    
		case Vml::sptFlowChartMerge : 			return SimpleTypes::shapetypeFlowChartMerge ;    
		//case Vml::sptFlowChartOfflineStorage : 	return SimpleTypes::shapetypeChartOfflineStorage ;    
		case Vml::sptFlowChartOnlineStorage : 	return SimpleTypes::shapetypeFlowChartOnlineStorage ;    
		case Vml::sptFlowChartMagneticTape :	return SimpleTypes::shapetypeFlowChartMagneticTape ;    
		case Vml::sptFlowChartMagneticDisk :	return SimpleTypes::shapetypeFlowChartMagneticDisk ;    
		case Vml::sptFlowChartMagneticDrum :	return SimpleTypes::shapetypeFlowChartMagneticDrum ;    
		case Vml::sptFlowChartDisplay : 		return SimpleTypes::shapetypeFlowChartDisplay ;    
		case Vml::sptFlowChartDelay : 			return SimpleTypes::shapetypeFlowChartDelay ;   
	//////////////////////////////////////////////////////////////////////////////
		case Vml::sptTextPlainText : 			return SimpleTypes::textshapetypeTextPlain			+2001;    
		case Vml::sptTextStop : 				return SimpleTypes::textshapetypeTextStop			+2001;    
		case Vml::sptTextTriangle : 			return SimpleTypes::textshapetypeTextTriangle		+2001;    
		case Vml::sptTextTriangleInverted :		return SimpleTypes::textshapetypeTextTriangleInverted+2001;    
		case Vml::sptTextChevron : 				return SimpleTypes::textshapetypeTextChevron 		+2001;    
		case Vml::sptTextChevronInverted : 		return SimpleTypes::textshapetypeTextChevronInverted+2001;    
		case Vml::sptTextRingInside : 			return SimpleTypes::textshapetypeTextRingInside 	+2001;    
		case Vml::sptTextRingOutside : 			return SimpleTypes::textshapetypeTextRingOutside 	+2001;    
		case Vml::sptTextArchUpCurve : 			return SimpleTypes::textshapetypeTextArchUp			+2001;    
		case Vml::sptTextArchDownCurve : 		return SimpleTypes::textshapetypeTextArchDown		+2001;    
		case Vml::sptTextCircleCurve : 			return SimpleTypes::textshapetypeTextCircle			+2001;    
		case Vml::sptTextButtonCurve : 			return SimpleTypes::textshapetypeTextButton			+2001;    
		case Vml::sptTextArchUpPour : 			return SimpleTypes::textshapetypeTextArchUpPour		+2001;    
		case Vml::sptTextArchDownPour : 		return SimpleTypes::textshapetypeTextArchDownPour	+2001;    
		case Vml::sptTextCirclePour : 			return SimpleTypes::textshapetypeTextCirclePour		+2001;    
		case Vml::sptTextButtonPour : 			return SimpleTypes::textshapetypeTextButtonPour		+2001;    
		case Vml::sptTextCurveUp : 				return SimpleTypes::textshapetypeTextCurveUp		+2001;    
		case Vml::sptTextCurveDown : 			return SimpleTypes::textshapetypeTextCurveDown		+2001;   
		case Vml::sptTextCascadeUp : 			return SimpleTypes::textshapetypeTextCascadeUp		+2001;   
		case Vml::sptTextCascadeDown : 			return SimpleTypes::textshapetypeTextCascadeDown	+2001;   
		case Vml::sptTextWave1 : 				return SimpleTypes::textshapetypeTextWave1			+2001;    
		case Vml::sptTextWave2 : 				return SimpleTypes::textshapetypeTextWave2			+2001;    
		//case Vml::sptTextWave3 : 				return SimpleTypes::shapetypeTextWave3 ;    
		case Vml::sptTextWave4 : 				return SimpleTypes::textshapetypeTextWave4			+2001;    
		case Vml::sptTextInflate : 				return SimpleTypes::textshapetypeTextInflate 		+2001;    
		case Vml::sptTextDeflate : 				return SimpleTypes::textshapetypeTextDeflate 		+2001;  

		//case Vml::sptTextInflateBottom : 		return SimpleTypes::shapetypeTextInflateBottom ;    
		//case Vml::sptTextDeflateBottom : 		return SimpleTypes::shapetypeTextDeflateBottom ;    
		//case Vml::sptTextInflateTop : 			return SimpleTypes::shapetypeTextInflateTop ; 
		//case Vml::sptTextDeflateTop : 			return SimpleTypes::shapetypeTextDeflateTop ;    
		//case Vml::sptTextDeflateInflate : 		return SimpleTypes::shapetypeTextDeflateInflate ;    
		//case Vml::sptTextDeflateInflateDeflate :return SimpleTypes::shapetypeTextDeflateInflateDeflat ;    
		//case Vml::sptTextFadeRight : 			return SimpleTypes::shapetypeTextFadeRight  ;    
		//case Vml::sptTextFadeLeft : 			return SimpleTypes::shapetypeTextFadeLeft ;    
		//case Vml::sptTextFadeUp : 				return SimpleTypes::shapetypeTextFadeUp ;    
		//case Vml::sptTextFadeDown : 			return SimpleTypes::shapetypeTextFadeDown ;    
		case Vml::sptTextSlantUp : 				return SimpleTypes::textshapetypeTextSlantUp 		+2001;   
		case Vml::sptTextSlantDown : 			return SimpleTypes::textshapetypeTextSlantDown 		+2001;  
		case Vml::sptTextCanUp : 				return SimpleTypes::textshapetypeTextCanUp	 		+2001; 
		case Vml::sptTextCanDown : 				return SimpleTypes::textshapetypeTextCanDown 		+2001;  
		
		case Vml::sptFlowChartAlternateProcess :return SimpleTypes::shapetypeFlowChartAlternateProcess ;    
		case Vml::sptFlowChartOffpageConnector :return SimpleTypes::shapetypeFlowChartOffpageConnector ;    
		case Vml::sptCallout90 : 				return SimpleTypes::shapetypeCallout1 ;    
		case Vml::sptAccentCallout90 : 			return SimpleTypes::shapetypeAccentCallout1 ;    
		case Vml::sptBorderCallout90 : 			return SimpleTypes::shapetypeBorderCallout1;    
		case Vml::sptAccentBorderCallout90 : 	return SimpleTypes::shapetypeAccentBorderCallout1 ;   
		case Vml::sptLeftRightUpArrow : 		return SimpleTypes::shapetypeLeftRightUpArrow ;    
		case Vml::sptSun : 						return SimpleTypes::shapetypeSun ;    
		case Vml::sptMoon : 					return SimpleTypes::shapetypeMoon ;    
		case Vml::sptBracketPair : 				return SimpleTypes::shapetypeBracketPair ;    
		case Vml::sptBracePair : 				return SimpleTypes::shapetypeBracePair ;    
		case Vml::sptSeal4 : 					return SimpleTypes::shapetypeStar4 ;    
		case Vml::sptDoubleWave : 				return SimpleTypes::shapetypeDoubleWave ;    
		case Vml::sptActionButtonBlank : 		return SimpleTypes::shapetypeActionButtonBlank ;    
		case Vml::sptActionButtonHome : 		return SimpleTypes::shapetypeActionButtonHome ;    
		case Vml::sptActionButtonHelp : 		return SimpleTypes::shapetypeActionButtonHelp ;    
		case Vml::sptActionButtonInformation : 	return SimpleTypes::shapetypeActionButtonInformation ;    
		case Vml::sptActionButtonForwardNext : 	return SimpleTypes::shapetypeActionButtonForwardNext ;    
		case Vml::sptActionButtonBackPrevious : return SimpleTypes::shapetypeActionButtonBackPrevious  ;   
		case Vml::sptActionButtonEnd : 			return SimpleTypes::shapetypeActionButtonEnd ;    
		case Vml::sptActionButtonBeginning : 	return SimpleTypes::shapetypeActionButtonBeginning ;    
		case Vml::sptActionButtonReturn : 		return SimpleTypes::shapetypeActionButtonReturn ;    
		case Vml::sptActionButtonDocument : 	return SimpleTypes::shapetypeActionButtonDocument ;    
		case Vml::sptActionButtonSound : 		return SimpleTypes::shapetypeActionButtonSound ;    
		case Vml::sptActionButtonMovie : 		return SimpleTypes::shapetypeActionButtonMovie ;   
		//case Vml::sptHostControl : 				return SimpleTypes::shapetype ;    
		case Vml::sptTextBox : 					return 2000;
		default:
			return SimpleTypes::shapetypeRect;

		}
	}

}
