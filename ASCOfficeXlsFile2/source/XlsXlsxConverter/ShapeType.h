#pragma once

#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #include <tchar.h>
#else
    #ifdef UNICODE
        #define __T(x)      L##x
    #else // UNICODE
        #define __T(x)      x
    #endif // UNICODE

    #define _T(x)       __T(x)
    #define _TEXT(x)    __T(x)
#endif

namespace oox 
{
	enum MSOSPT
	{
		msosptNotPrimitive					=	0x00000000, //	( - ) A shape that has no preset geometry, and is drawn with custom geometry instead. For example, freeform shapes drawn by users fall into this category.
		msosptRectangle						=	0x00000001, //	A rectangle shape.
		msosptRoundRectangle				=	0x00000002, //	A rectangle shape with rounded corners.
		msosptEllipse						=	0x00000003, //	An ellipse shape.
		msosptDiamond						=	0x00000004, //	A diamond shape.
		msosptIsocelesTriangle				=	0x00000005, //	An isosceles triangle shape.
		msosptRightTriangle					=	0x00000006, //	A right triangle shape.
		msosptParallelogram					=	0x00000007, //	A parallelogram shape.
		msosptTrapezoid						=	0x00000008, //	A trapezoid shape.
		msosptHexagon						=	0x00000009, //	A hexagon shape.
		msosptOctagon						=	0x0000000A, //	An octagon shape.
		msosptPlus							=	0x0000000B, //	A plus shape.
		msosptStar							=	0x0000000C, //	A star shape.
		msosptArrow							=	0x0000000D, //	An arrow shape.
		msosptThickArrow					=	0x0000000E, //	( - ) This value SHOULD NOT be used.
		msosptHomePlate						=	0x0000000F, //	An irregular pentagon shape.
		msosptCube							=	0x00000010, //	A cube shape.
		msosptBalloon						=	0x00000011, //	( - ) A speech balloon shape.
		msosptSeal							=	0x00000012, //	( - ) A seal shape.
		msosptArc							=	0x00000013, //	A curved arc shape.
		msosptLine							=	0x00000014, //	A line shape.
		msosptPlaque						=	0x00000015, //	A plaque shape.
		msosptCan							=	0x00000016, //	A cylinder shape.
		msosptDonut							=	0x00000017, //	A donut shape.
		msosptTextSimple					=	0x00000018, //	A simple text shape. Text SHOULD be drawn on a straight line.
		msosptTextOctagon					=	0x00000019, //	An octagonal text shape. Text SHOULD be drawn within an octagonal boundary.
		msosptTextHexagon					=	0x0000001A, //	A hexagonal text shape. Text SHOULD be drawn within a hexagonal boundary.
		msosptTextCurve						=	0x0000001B, //	A curved text shape. Text SHOULD be drawn on a curved line.
		msosptTextWave						=	0x0000001C, //	A wavy text shape. Text SHOULD be drawn on a wavy line.
		msosptTextRing						=	0x0000001D, //	A ringed text shape. Text SHOULD be drawn within a semicircular arc.
		msosptTextOnCurve					=	0x0000001E, //	A text shape that draws text on a curve. Text SHOULD be drawn on a curved line.
		msosptTextOnRing					=	0x0000001F, //	A text shape that draws text on a ring. Text SHOULD be drawn on a semicircular arc.
		msosptStraightConnector1			=	0x00000020, //	A straight connector shape.
		msosptBentConnector2				=	0x00000021, //	A bent connector shape.
		msosptBentConnector3				=	0x00000022, //	A bent connector shape.
		msosptBentConnector4				=	0x00000023, //	A bent connector shape.
		msosptBentConnector5				=	0x00000024, //	A bent connector shape.
		msosptCurvedConnector2				=	0x00000025, //	A curved connector shape.
		msosptCurvedConnector3				=	0x00000026, //	A curved connector shape.
		msosptCurvedConnector4				=	0x00000027, //	A curved connector shape.
		msosptCurvedConnector5				=	0x00000028, //	A curved connector shape.
		msosptCallout1						=	0x00000029, //	A callout shape.
		msosptCallout2						=	0x0000002A, //	A callout shape.
		msosptCallout3						=	0x0000002B, //	A callout shape.
		msosptAccentCallout1				=	0x0000002C, //	A callout shape with side accent.
		msosptAccentCallout2				=	0x0000002D, //	A callout shape with side accent.
		msosptAccentCallout3				=	0x0000002E, //	A callout shape with side accent.
		msosptBorderCallout1				=	0x0000002F, //	A callout shape with border.
		msosptBorderCallout2				=	0x00000030, //	A callout shape with border.
		msosptBorderCallout3				=	0x00000031, //	A callout shape with border.
		msosptAccentBorderCallout1			=	0x00000032, //	A callout shape with border and side accent.
		msosptAccentBorderCallout2			=	0x00000033, //	A callout shape with border and side accent.
		msosptAccentBorderCallout3			=	0x00000034, //	A callout shape with border and side accent.
		msosptRibbon						=	0x00000035, //	A ribbon shape.
		msosptRibbon2						=	0x00000036, //	A ribbon shape.
		msosptChevron						=	0x00000037, //	A chevron shape.
		msosptPentagon						=	0x00000038, //	A regular pentagon shape.
		msosptNoSmoking						=	0x00000039, //	A circle-slash shape.
		msosptSeal8							=	0x0000003A, //	A seal shape with eight points.
		msosptSeal16						=	0x0000003B, //	A seal shape with sixteen points.
		msosptSeal32						=	0x0000003C, //	A seal shape with thirty-two points.
		msosptWedgeRectCallout				=	0x0000003D, //	A rectangular callout shape.
		msosptWedgeRRectCallout				=	0x0000003E, //	A rectangular callout shape with rounded corners.
		msosptWedgeEllipseCallout			=	0x0000003F, //	An elliptical callout shape.
		msosptWave							=	0x00000040, //	A wave shape.
		msosptFoldedCorner					=	0x00000041, //	A rectangular shape with a folded corner.
		msosptLeftArrow						=	0x00000042, //	An arrow shape pointing left.
		msosptDownArrow						=	0x00000043, //	An arrow shape pointing down.
		msosptUpArrow						=	0x00000044, //	An arrow shape pointing up.
		msosptLeftRightArrow				=	0x00000045, //	An arrow shape pointing left and right.
		msosptUpDownArrow					=	0x00000046, //	An arrow shape pointing down and up.
		msosptIrregularSeal1				=	0x00000047, //	An irregular seal shape.
		msosptIrregularSeal2				=	0x00000048, //	An irregular seal shape.
		msosptLightningBolt					=	0x00000049, //	A lightning bolt shape.
		msosptHeart							=	0x0000004A, //	A heart shape.
		msosptPictureFrame					=	0x0000004B, //	A frame shape.
		msosptQuadArrow						=	0x0000004C, //	A shape with arrows pointing down, left, right, and up.
		msosptLeftArrowCallout				=	0x0000004D, //	A callout shape with an arrow pointing left.
		msosptRightArrowCallout				=	0x0000004E, //	A callout shape with an arrow pointing right.
		msosptUpArrowCallout				=	0x0000004F, //	A callout shape with an arrow pointing up.
		msosptDownArrowCallout				=	0x00000050, //	A callout shape with an arrow pointing down.
		msosptLeftRightArrowCallout			=	0x00000051, //	A callout shape with arrows pointing left and right.
		msosptUpDownArrowCallout			=	0x00000052, //	A callout shape with arrows pointing down and up.
		msosptQuadArrowCallout				=	0x00000053, //	A callout shape with arrows pointing down, left, right, and up.
		msosptBevel							=	0x00000054, //	A beveled rectangle shape.
		msosptLeftBracket					=	0x00000055, //	An opening bracket shape.
		msosptRightBracket					=	0x00000056, //	A closing bracket shape.
		msosptLeftBrace						=	0x00000057, //	An opening brace shape.
		msosptRightBrace					=	0x00000058, //	A closing brace shape.
		msosptLeftUpArrow					=	0x00000059, //	An arrow shape pointing left and up.
		msosptBentUpArrow					=	0x0000005A, //	A bent arrow shape with its base on the left, pointing up.
		msosptBentArrow						=	0x0000005B, //	A curved arrow shape with its base on the bottom, pointing right.
		msosptSeal24						=	0x0000005C, //	A seal shape with twenty-four points.
		msosptStripedRightArrow				=	0x0000005D, //	A striped arrow shape pointing right.
		msosptNotchedRightArrow				=	0x0000005E, //	A notched arrow shape pointing right.
		msosptBlockArc						=	0x0000005F, //	A semicircular arc shape.
		msosptSmileyFace					=	0x00000060, //	A smiling face shape.
		msosptVerticalScroll				=	0x00000061, //	A scroll shape opened vertically.
		msosptHorizontalScroll				=	0x00000062, //	A scroll shape opened horizontally.
		msosptCircularArrow					=	0x00000063, //	A semicircular arrow shape.
		msosptNotchedCircularArrow			=	0x00000064, //	This value SHOULD NOT be used.
		msosptUturnArrow					=	0x00000065, //	A semicircular arrow shape with a straight tail.
		msosptCurvedRightArrow				=	0x00000066, //	An arrow shape that curves to the right.
		msosptCurvedLeftArrow				=	0x00000067, //	An arrow shape that curves to the left.
		msosptCurvedUpArrow					=	0x00000068, //	An arrow shape that curves upward.
		msosptCurvedDownArrow				=	0x00000069, //	An arrow shape that curves downward.
		msosptCloudCallout					=	0x0000006A, //	A cloud-shaped callout.
		msosptEllipseRibbon					=	0x0000006B, //	An elliptical ribbon shape.
		msosptEllipseRibbon2				=	0x0000006C, //	An elliptical ribbon shape.
		msosptFlowChartProcess				=	0x0000006D, //	A process shape for flowcharts.
		msosptFlowChartDecision				=	0x0000006E, //	A decision shape for flowcharts.
		msosptFlowChartInputOutput			=	0x0000006F, //	An input-output shape for flowcharts.
		msosptFlowChartPredefinedProcess	=	0x00000070, //	A predefined process shape for flowcharts.
		msosptFlowChartInternalStorage		=	0x00000071, //	An internal storage shape for flowcharts.
		msosptFlowChartDocument				=	0x00000072, //	A document shape for flowcharts.
		msosptFlowChartMultidocument		=	0x00000073, //	A multidocument shape for flowcharts.
		msosptFlowChartTerminator			=	0x00000074, //	A terminator shape for flowcharts.
		msosptFlowChartPreparation			=	0x00000075, //	A preparation shape for flowcharts.
		msosptFlowChartManualInput			=	0x00000076, //	A manual input shape for flowcharts.
		msosptFlowChartManualOperation		=	0x00000077, //	A manual operation shape for flowcharts.
		msosptFlowChartConnector			=	0x00000078, //	A connector shape for flowcharts.
		msosptFlowChartPunchedCard			=	0x00000079, //	A punched card shape for flowcharts.
		msosptFlowChartPunchedTape			=	0x0000007A, //	A punched tape shape for flowcharts.
		msosptFlowChartSummingJunction		=	0x0000007B, //	A summing junction shape for flowcharts.
		msosptFlowChartOr					=	0x0000007C, //	An OR shape for flowcharts.
		msosptFlowChartCollate				=	0x0000007D, //	A collation shape for flowcharts.
		msosptFlowChartSort					=	0x0000007E, //	A sorting shape for flowcharts.
		msosptFlowChartExtract				=	0x0000007F, //	An extraction shape for flowcharts.
		msosptFlowChartMerge				=	0x00000080, //	A merging shape for flowcharts.
		msosptFlowChartOfflineStorage		=	0x00000081, //	An offline storage shape for flowcharts.
		msosptFlowChartOnlineStorage		=	0x00000082, //	An online storage shape for flowcharts.
		msosptFlowChartMagneticTape			=	0x00000083, //	A magnetic tape shape for flowcharts.
		msosptFlowChartMagneticDisk			=	0x00000084, //	A magnetic disk shape for flowcharts.
		msosptFlowChartMagneticDrum			=	0x00000085, //	A magnetic drum shape for flowcharts.
		msosptFlowChartDisplay				=	0x00000086, //	A display shape for flowcharts.
		msosptFlowChartDelay				=	0x00000087, //	A delay shape for flowcharts.
		msosptTextPlainText					=	0x00000088, //	A plain text shape.
		msosptTextStop						=	0x00000089, //	An octagonal text shape.
		msosptTextTriangle					=	0x0000008A, //	A triangular text shape pointing upwards.
		msosptTextTriangleInverted			=	0x0000008B, //	A triangular text shape pointing downwards.
		msosptTextChevron					=	0x0000008C, //	A chevron text shape pointing upwards.
		msosptTextChevronInverted			=	0x0000008D, //	A chevron text shape pointing downwards.
		msosptTextRingInside				=	0x0000008E, //	A circular text shape, as if reading an inscription on the inside of a ring.
		msosptTextRingOutside				=	0x0000008F, //	A circular text shape, as if reading an inscription on the outside of a ring.
		msosptTextArchUpCurve				=	0x00000090, //	An upward arching curved text shape.
		msosptTextArchDownCurve				=	0x00000091, //	A downward arching curved text shape.
		msosptTextCircleCurve				=	0x00000092, //	A circular text shape.
		msosptTextButtonCurve				=	0x00000093, //	A text shape that resembles a button.
		msosptTextArchUpPour				=	0x00000094, //	An upward arching text shape.
		msosptTextArchDownPour				=	0x00000095, //	A downward arching text shape.
		msosptTextCirclePour				=	0x00000096, //	A circular text shape.
		msosptTextButtonPour				=	0x00000097, //	A text shape that resembles a button.
		msosptTextCurveUp					=	0x00000098, //	An upward curving text shape.
		msosptTextCurveDown					=	0x00000099, //	A downward curving text shape.
		msosptTextCascadeUp					=	0x0000009A, //	A cascading text shape pointed upwards.
		msosptTextCascadeDown				=	0x0000009B, //	A cascading text shape pointed downwards.
		msosptTextWave1						=	0x0000009C, //	A wavy text shape.
		msosptTextWave2						=	0x0000009D, //	A wavy text shape.
		msosptTextWave3						=	0x0000009E, //	A wavy text shape.
		msosptTextWave4						=	0x0000009F, //	A wavy text shape.
		msosptTextInflate					=	0x000000A0, //	A text shape that expands vertically in the middle.
		msosptTextDeflate					=	0x000000A1, //	A text shape that shrinks vertically in the middle.
		msosptTextInflateBottom				=	0x000000A2, //	A text shape that expands downward in the middle.
		msosptTextDeflateBottom				=	0x000000A3, //	A text shape that shrinks upwards in the middle.
		msosptTextInflateTop				=	0x000000A4, //	A text shape that expands upward in the middle.
		msosptTextDeflateTop				=	0x000000A5, //	A text shape that shrinks downward in the middle.
		msosptTextDeflateInflate			=	0x000000A6, //	A text shape where lower lines expand upward. Upper lines shrink to compensate.
		msosptTextDeflateInflateDeflate		=	0x000000A7, //	A text shape where lines in the center expand vertically. Upper and lower lines shrink to compensate.
		msosptTextFadeRight					=	0x000000A8, //	A text shape that shrinks vertically on the right side.
		msosptTextFadeLeft					=	0x000000A9, //	A text shape that shrinks vertically on the left side.
		msosptTextFadeUp					=	0x000000AA, //	A text shape that shrinks horizontally on top.
		msosptTextFadeDown					=	0x000000AB, //	A text shape that shrinks horizontally on bottom.
		msosptTextSlantUp					=	0x000000AC, //	An upward slanted text shape.
		msosptTextSlantDown					=	0x000000AD, //	A downward slanted text shape.
		msosptTextCanUp						=	0x000000AE, //	A text shape that is curved upwards as if being read on the side of a can.
		msosptTextCanDown					=	0x000000AF, //	A text shape that is curved downwards as if being read on the side of a can.
		msosptFlowChartAlternateProcess		=	0x000000B0, //	An alternate process shape for flowcharts.
		msosptFlowChartOffpageConnector		=	0x000000B1, //	An off page connector shape for flowcharts.
		msosptCallout90						=	0x000000B2, //	A callout shape.
		msosptAccentCallout90				=	0x000000B3, //	A callout shape.
		msosptBorderCallout90				=	0x000000B4, //	A callout shape with border.
		msosptAccentBorderCallout90			=	0x000000B5, //	A callout shape with border.
		msosptLeftRightUpArrow				=	0x000000B6, //	A shape with arrows pointing left, right, and up.
		msosptSun							=	0x000000B7, //	A sun shape.
		msosptMoon							=	0x000000B8, //	A moon shape.
		msosptBracketPair					=	0x000000B9, //	A shape enclosed in brackets.
		msosptBracePair						=	0x000000BA, //	A shape enclosed in braces.
		msosptSeal4							=	0x000000BB, //	A seal shape with four points.
		msosptDoubleWave					=	0x000000BC, //	A double wave shape.
		msosptActionButtonBlank				=	0x000000BD, //	A blank button shape.
		msosptActionButtonHome				=	0x000000BE, //	A home button shape.
		msosptActionButtonHelp				=	0x000000BF, //	A help button shape.
		msosptActionButtonInformation		=	0x000000C0, //	An information button shape.
		msosptActionButtonForwardNext		=	0x000000C1, //	A forward or next button shape.
		msosptActionButtonBackPrevious		=	0x000000C2, //	A back or previous button shape.
		msosptActionButtonEnd				=	0x000000C3, //	An end button shape.
		msosptActionButtonBeginning			=	0x000000C4, //	A beginning button shape.
		msosptActionButtonReturn			=	0x000000C5, //	A return button shape.
		msosptActionButtonDocument			=	0x000000C6, //	A document button shape.
		msosptActionButtonSound				=	0x000000C7, //	A sound button shape.
		msosptActionButtonMovie				=	0x000000C8, //	A movie button shape.
		msosptHostControl					=	0x000000C9, //	This value SHOULD NOT be used.
		msosptTextBox						=	0x000000CA //	A text box shape.
	};


	std::wstring static Spt2ShapeType( MSOSPT type) 
	{
		switch(type)
		{
		case msosptTextBox:						return _T("rect");
		case msosptRectangle : 					return _T("rect");    
		case msosptRoundRectangle : 			return _T("roundRect");    
		case msosptEllipse : 					return _T("ellipse");    
		case msosptDiamond : 					return _T("diamond");    
		case msosptIsocelesTriangle :			return _T("triangle");    
		case msosptRightTriangle : 				return _T("rtTriangle");    
		case msosptParallelogram : 				return _T("parallelogram");    
		case msosptTrapezoid : 					return _T("trapezoid");    
		case msosptHexagon : 					return _T("hexagon");    
		case msosptOctagon : 					return _T("octagon");    
		case msosptPlus : 						return _T("plus");    
		case msosptStar : 						return _T("star5");    
		case msosptArrow : 						return _T("rightArrow");    //???
		case msosptThickArrow : 				return _T("thickArrow");   
		case msosptHomePlate : 					return _T("homePlate");    
		case msosptCube : 						return _T("cube");    
		case msosptBalloon : 					return _T("ballon");    
		case msosptSeal : 						return _T("irregularSeal1");    
		case msosptArc : 						return _T("arc");    
		case msosptLine : 						return _T("line");    
		case msosptPlaque : 					return _T("plaque");   
		case msosptCan : 						return _T("can");    
		case msosptDonut : 						return _T("donut");    
		case msosptStraightConnector1 : 		return _T("straightConnector1");    
		case msosptBentConnector2 : 			return _T("bentConnector2 ");    
		case msosptBentConnector3 : 			return _T("bentConnector3");    
		case msosptBentConnector4 : 			return _T("bentConnector4");    
		case msosptBentConnector5 : 			return _T("bentConnector5");    
		case msosptCurvedConnector2 : 			return _T("curvedConnector2");    
		case msosptCurvedConnector3 : 			return _T("curvedConnector3");    
		case msosptCurvedConnector4 : 			return _T("curvedConnector4");    
		case msosptCurvedConnector5 : 			return _T("curvedConnector5");    
		case msosptCallout1 : 					return _T("callout1 ");    
		case msosptCallout2 : 					return _T("callout2 ");    
		case msosptCallout3 : 					return _T("callout3 ");   
		case msosptAccentCallout1 : 			return _T("accentCallout1");    
		case msosptAccentCallout2 : 			return _T("accentCallout2");    
		case msosptAccentCallout3 : 			return _T("accentCallout3");    
		case msosptBorderCallout1 : 			return _T("accentCallout1");    
		case msosptBorderCallout2 : 			return _T("accentCallout2");    
		case msosptBorderCallout3 : 			return _T("accentCallout3");    
		case msosptAccentBorderCallout1 : 		return _T("accentBorderCallout1");    
		case msosptAccentBorderCallout2 : 		return _T("accentBorderCallout2");    
		case msosptAccentBorderCallout3 : 		return _T("accentBorderCallout3");    
		case msosptRibbon : 					return _T("ribbon");    
		case msosptRibbon2 : 					return _T("ribbon2");    
		case msosptChevron : 					return _T("chevron");    
		case msosptPentagon : 					return _T("pentagon");  
		case msosptNoSmoking : 					return _T("noSmoking");    
		case msosptSeal8 : 						return _T("star8");    
		case msosptSeal16 : 					return _T("star16");    
		case msosptSeal32 : 					return _T("star32");    
		case msosptWedgeRectCallout : 			return _T("wedgeRectCallout");    
		case msosptWedgeRRectCallout : 			return _T("wedgeRoundRectCallout");    
		case msosptWedgeEllipseCallout : 		return _T("wedgeEllipseCallout");    
		case msosptWave : 						return _T("wave");    
		case msosptFoldedCorner : 				return _T("foldedCorner");    
		case msosptLeftArrow : 					return _T("leftArrow");    
		case msosptDownArrow : 					return _T("downArrow");    
		case msosptUpArrow : 					return _T("upArrow ");    
		case msosptLeftRightArrow : 			return _T("leftRightArrow");   
		case msosptUpDownArrow : 				return _T("upDownArrow");    
		case msosptIrregularSeal1 : 			return _T("irregularSeal1");    
		case msosptIrregularSeal2 : 			return _T("irregularSeal2");    
		case msosptLightningBolt : 				return _T("lightningBolt");    
		case msosptHeart : 						return _T("heart");   
		//case msosptPictureFrame : 			return 3000;//_T("shapetypeFrame");   _T("shapetypeRect");//
		case msosptQuadArrow : 					return _T("quadArrow");    
		case msosptLeftArrowCallout : 			return _T("leftArrowCallout");    
		case msosptRightArrowCallout : 			return _T("rightArrowCallout");    
		case msosptUpArrowCallout : 			return _T("upArrowCallout");    
		case msosptDownArrowCallout : 			return _T("downArrowCallout");    
		case msosptLeftRightArrowCallout : 		return _T("leftRightArrowCallout");    
		case msosptUpDownArrowCallout : 		return _T("UpDownArrowCallout");    
		case msosptQuadArrowCallout : 			return _T("QuadArrowCallout");    
		case msosptBevel : 						return _T("bevel");    
		case msosptLeftBracket : 				return _T("leftBracket");    
		case msosptRightBracket : 				return _T("rightBracket");    
		case msosptLeftBrace : 					return _T("leftBrace");    
		case msosptRightBrace : 				return _T("rightBrace");    
		case msosptLeftUpArrow : 				return _T("leftUpArrow");    
		case msosptBentUpArrow : 				return _T("bentUpArrow");    
		case msosptBentArrow : 					return _T("bentArrow");    
		case msosptSeal24 : 					return _T("star24");    
		case msosptStripedRightArrow : 			return _T("stripedRightArrow");    
		case msosptNotchedRightArrow : 			return _T("notchedRightArrow");    
		case msosptBlockArc : 					return _T("blockArc");    
		case msosptSmileyFace : 				return _T("smileyFace");    
		case msosptVerticalScroll : 			return _T("verticalScroll");    
		case msosptHorizontalScroll : 			return _T("horizontalScroll");    
		case msosptCircularArrow : 				return _T("circularArrow");    
		//case msosptNotchedCircularArrow :		return _T("notchedCircularArrow");    
		case msosptUturnArrow : 				return _T("uturnArrow");    
		case msosptCurvedRightArrow : 			return _T("curvedRightArrow ");    
		case msosptCurvedLeftArrow : 			return _T("curvedLeftArrow");    
		case msosptCurvedUpArrow : 				return _T("curvedUpArrow");    
		case msosptCurvedDownArrow : 			return _T("curvedDownArrow");   
		case msosptCloudCallout : 				return _T("cloudCallout");    
		case msosptEllipseRibbon : 				return _T("ellipseRibbon");    
		case msosptEllipseRibbon2 : 			return _T("ellipseRibbon2");    
		case msosptFlowChartProcess : 			return _T("flowChartProcess");    
		case msosptFlowChartDecision : 			return _T("flowChartDecision");    
		case msosptFlowChartInputOutput : 		return _T("flowChartInputOutput");    
		case msosptFlowChartPredefinedProcess:	return _T("flowChartPredefinedProcess");    
		case msosptFlowChartInternalStorage :	return _T("flowChartInternalStorage");    
		case msosptFlowChartDocument : 			return _T("flowChartDocument");    
		case msosptFlowChartMultidocument :		return _T("flowChartMultidocument");    
		case msosptFlowChartTerminator : 		return _T("flowChartTerminator");    
		case msosptFlowChartPreparation : 		return _T("flowChartPreparation");    
		case msosptFlowChartManualInput : 		return _T("flowChartManualInput");   
		case msosptFlowChartManualOperation :	return _T("flowChartManualOperation");   
		case msosptFlowChartConnector : 		return _T("flowChartConnector ");    
		case msosptFlowChartPunchedCard : 		return _T("flowChartPunchedCard");    
		case msosptFlowChartPunchedTape : 		return _T("flowChartPunchedTape ");    
		case msosptFlowChartSummingJunction :	return _T("flowChartSummingJunction");   
		case msosptFlowChartOr : 				return _T("flowChartOr");    
		case msosptFlowChartCollate : 			return _T("flowChartCollate ");    
		case msosptFlowChartSort : 				return _T("flowChartSort");    
		case msosptFlowChartExtract : 			return _T("flowChartExtract");    
		case msosptFlowChartMerge : 			return _T("flowChartMerge");    
		//case msosptFlowChartOfflineStorage : 	return _T("chartOfflineStorage");    
		case msosptFlowChartOnlineStorage : 	return _T("flowChartOnlineStorage");    
		case msosptFlowChartMagneticTape :		return _T("flowChartMagneticTape");    
		case msosptFlowChartMagneticDisk :		return _T("flowChartMagneticDisk");    
		case msosptFlowChartMagneticDrum :		return _T("flowChartMagneticDrum");    
		case msosptFlowChartDisplay : 			return _T("flowChartDisplay");    
		case msosptFlowChartDelay : 			return _T("flowChartDelay");   
		case msosptFlowChartAlternateProcess :	return _T("flowChartAlternateProcess");    
		case msosptFlowChartOffpageConnector :	return _T("flowChartOffpageConnector");    
		case msosptCallout90 : 					return _T("callout1");    
		case msosptAccentCallout90 : 			return _T("accentCallout1");    
		case msosptBorderCallout90 : 			return _T("borderCallout1");    
		case msosptAccentBorderCallout90 : 		return _T("accentBorderCallout1");   
		case msosptLeftRightUpArrow : 			return _T("leftRightUpArrow");    
		case msosptSun : 						return _T("sun");    
		case msosptMoon : 						return _T("moon");    
		case msosptBracketPair : 				return _T("bracketPair");    
		case msosptBracePair : 					return _T("bracePair");    
		case msosptSeal4 : 						return _T("star4");    
		case msosptDoubleWave : 				return _T("doubleWave");    
		case msosptActionButtonBlank : 			return _T("actionButtonBlank");    
		case msosptActionButtonHome : 			return _T("actionButtonHome");    
		case msosptActionButtonHelp : 			return _T("actionButtonHelp");    
		case msosptActionButtonInformation :	return _T("actionButtonInformation");    
		case msosptActionButtonForwardNext :	return _T("actionButtonForwardNext");    
		case msosptActionButtonBackPrevious :	return _T("actionButtonBackPrevious ");   
		case msosptActionButtonEnd : 			return _T("actionButtonEnd");    
		case msosptActionButtonBeginning : 		return _T("actionButtonBeginning");    
		case msosptActionButtonReturn : 		return _T("actionButtonReturn");    
		case msosptActionButtonDocument : 		return _T("actionButtonDocument");    
		case msosptActionButtonSound : 			return _T("actionButtonSound");    
		case msosptActionButtonMovie : 			return _T("actionButtonMovie");   
		}
		return _T("");
	}
	std::wstring static Spt2WordArtShapeType( MSOSPT type) 
	{
		switch(type)
		{
		case msosptTextSimple : 				return _T("textSimple");    
		case msosptTextOctagon : 				return _T("textOctagon");    
		case msosptTextHexagon : 				return _T("textHexagon");    
		case msosptTextCurve : 					return _T("textCurve");    
		case msosptTextWave : 					return _T("textWave");    
		case msosptTextRing : 					return _T("textRing ");    
		case msosptTextOnCurve : 				return _T("textOnCurve");    
		case msosptTextOnRing : 				return _T("textOnRing");    
		case msosptTextPlainText : 				return _T("textPlain");   
		case msosptTextStop : 					return _T("textStop");   
		case msosptTextTriangle : 				return _T("textTriangle");    
		case msosptTextTriangleInverted :		return _T("textTriangleInverted");    
		case msosptTextChevron : 				return _T("textChevron");   
		case msosptTextChevronInverted : 		return _T("textChevronInverted");    
		case msosptTextRingInside : 			return _T("textRingInside");    
		case msosptTextRingOutside : 			return _T("textRingOutside");    
		case msosptTextArchUpCurve : 			return _T("textArchUp");   
		case msosptTextArchDownCurve : 			return _T("textArchDown");    
		case msosptTextCircleCurve : 			return _T("textCircle");   
		case msosptTextButtonCurve : 			return _T("textButton");   
		case msosptTextArchUpPour : 			return _T("textArchUpPour");    
		case msosptTextArchDownPour : 			return _T("textArchDownPour");    
		case msosptTextCirclePour : 			return _T("textCirclePour");    
		case msosptTextButtonPour : 			return _T("textButtonPour");    
		case msosptTextCurveUp : 				return _T("textCurveUp");    
		case msosptTextCurveDown : 				return _T("textCurveDown");   
		case msosptTextCascadeUp : 				return _T("textCascadeUp");   
		case msosptTextCascadeDown : 			return _T("textCascadeDown");   
		case msosptTextWave1 : 					return _T("textWave1");   
		case msosptTextWave2 : 					return _T("textWave2");   
		//case msosptTextWave3 : 				return _T("textWave3");    
		case msosptTextWave4 : 					return _T("textWave4");   
		case msosptTextInflate : 				return _T("textInflate");   
		case msosptTextDeflate : 				return _T("textDeflate"); 
		case msosptTextInflateBottom : 			return _T("textInflateBottom");    
		case msosptTextDeflateBottom : 			return _T("textDeflateBottom");    
		case msosptTextInflateTop : 			return _T("textInflateTop"); 
		case msosptTextDeflateTop : 			return _T("textDeflateTop");    
		case msosptTextDeflateInflate : 		return _T("textDeflateInflate");    
		case msosptTextDeflateInflateDeflate :	return _T("textDeflateInflateDeflat");    
		case msosptTextFadeRight : 				return _T("textFadeRight ");    
		case msosptTextFadeLeft : 				return _T("textFadeLeft");    
		case msosptTextFadeUp : 				return _T("textFadeUp");    
		case msosptTextFadeDown : 				return _T("textFadeDown");    
		case msosptTextSlantUp : 				return _T("textSlantUp");  
		case msosptTextSlantDown : 				return _T("textSlantDown"); 
		case msosptTextCanUp : 					return _T("textCanUp	");
		case msosptTextCanDown : 				return _T("textCanDown"); 
		}
		return _T("");
	}
}
