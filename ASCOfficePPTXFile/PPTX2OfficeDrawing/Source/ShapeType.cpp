#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2OfficeDrawing
{
	OOXMLShapes::ShapeType Converter::GetShapeTypeFromStr(const std::string& str)//const
	{
		if(str == "")
			return OOXMLShapes::sptNil;
		switch(str[0])
		{
		case 'a':
			if(str == "accentBorderCallout1") return OOXMLShapes::sptCAccentBorderCallout1;
			if(str == "accentBorderCallout2") return OOXMLShapes::sptCAccentBorderCallout2;
			if(str == "accentBorderCallout3") return OOXMLShapes::sptCAccentBorderCallout3;
			if(str == "accentCallout1") return OOXMLShapes::sptCAccentCallout1;
			if(str == "accentCallout2") return OOXMLShapes::sptCAccentCallout2;
			if(str == "accentCallout3") return OOXMLShapes::sptCAccentCallout3;
			if(str == "actionButtonBackPrevious") return OOXMLShapes::sptCActionButtonBackPrevious;
			if(str == "actionButtonBeginning") return OOXMLShapes::sptCActionButtonBeginning;
			if(str == "actionButtonBlank") return OOXMLShapes::sptCActionButtonBlank;
			if(str == "actionButtonDocument") return OOXMLShapes::sptCActionButtonDocument;
			if(str == "actionButtonEnd") return OOXMLShapes::sptCActionButtonEnd;
			if(str == "actionButtonForwardNext") return OOXMLShapes::sptCActionButtonForwardNext;
			if(str == "actionButtonHelp") return OOXMLShapes::sptCActionButtonHelp;
			if(str == "actionButtonHome") return OOXMLShapes::sptCActionButtonHome;
			if(str == "actionButtonInformation") return OOXMLShapes::sptCActionButtonInformation;
			if(str == "actionButtonMovie") return OOXMLShapes::sptCActionButtonMovie;
			if(str == "actionButtonReturn") return OOXMLShapes::sptCActionButtonReturn;
			if(str == "actionButtonSound") return OOXMLShapes::sptCActionButtonSound;
			if(str == "arc") return OOXMLShapes::sptCArc;
			break;
		case 'b':
			if(str == "bentArrow") return OOXMLShapes::sptCBentArrow;
			if(str == "bentConnector2") return OOXMLShapes::sptCBentConnector2;
			if(str == "bentConnector3") return OOXMLShapes::sptCBentConnector3;
			if(str == "bentConnector4") return OOXMLShapes::sptCBentConnector4;
			if(str == "bentConnector5") return OOXMLShapes::sptCBentConnector5;
			if(str == "bentUpArrow") return OOXMLShapes::sptCBentUpArrow;
			if(str == "bevel") return OOXMLShapes::sptCBevel;
			if(str == "blockArc") return OOXMLShapes::sptCBlockArc;
			if(str == "borderCallout1") return OOXMLShapes::sptCBorderCallout1;
			if(str == "borderCallout2") return OOXMLShapes::sptCBorderCallout2;
			if(str == "borderCallout3") return OOXMLShapes::sptCBorderCallout3;
			if(str == "bracePair") return OOXMLShapes::sptCBracePair;
			if(str == "bracketPair") return OOXMLShapes::sptCBracketPair;
			break;
		case 'c':
			if(str == "callout1") return OOXMLShapes::sptCCallout1;
			if(str == "callout2") return OOXMLShapes::sptCCallout2;
			if(str == "callout3") return OOXMLShapes::sptCCallout3;
			if(str == "can") return OOXMLShapes::sptCCan;
			if(str == "chartPlus") return OOXMLShapes::sptCChartPlus;
			if(str == "chartStar") return OOXMLShapes::sptCChartStar;
			if(str == "chartX") return OOXMLShapes::sptCChartX;
			if(str == "chevron") return OOXMLShapes::sptCChevron;
			if(str == "chord") return OOXMLShapes::sptCChord;
			if(str == "circularArrow") return OOXMLShapes::sptCCircularArrow;
			if(str == "cloud") return OOXMLShapes::sptCCloud;
			if(str == "cloudCallout") return OOXMLShapes::sptCCloudCallout;
			if(str == "corner") return OOXMLShapes::sptCCorner;
			if(str == "cornerTabs") return OOXMLShapes::sptCCornerTabs;
			if(str == "cube") return OOXMLShapes::sptCCube;
			if(str == "curvedConnector2") return OOXMLShapes::sptCCurvedConnector2;
			if(str == "curvedConnector3") return OOXMLShapes::sptCCurvedConnector3;
			if(str == "curvedConnector4") return OOXMLShapes::sptCCurvedConnector4;
			if(str == "curvedConnector5") return OOXMLShapes::sptCCurvedConnector5;
			if(str == "curvedDownArrow") return OOXMLShapes::sptCCurvedDownArrow;
			if(str == "curvedLeftArrow") return OOXMLShapes::sptCCurvedLeftArrow;
			if(str == "curvedRightArrow") return OOXMLShapes::sptCCurvedRightArrow;
			if(str == "curvedUpArrow") return OOXMLShapes::sptCCurvedUpArrow;
			break;
		case 'd':
			if(str == "decagon") return OOXMLShapes::sptCDecagon;
			if(str == "diagStripe") return OOXMLShapes::sptCDiagStripe;
			if(str == "diamond") return OOXMLShapes::sptCDiamond;
			if(str == "dodecagon") return OOXMLShapes::sptCDodecagon;
			if(str == "donut") return OOXMLShapes::sptCDonut;
			if(str == "doubleWave") return OOXMLShapes::sptCDoubleWave;
			if(str == "downArrow") return OOXMLShapes::sptCDownArrow;
			if(str == "downArrowCallout") return OOXMLShapes::sptCDownArrowCallout;
			break;
		case 'e':
			if(str == "ellipse") return OOXMLShapes::sptCEllipse;
			if(str == "ellipseRibbon") return OOXMLShapes::sptCEllipseRibbon;
			if(str == "ellipseRibbon2") return OOXMLShapes::sptCEllipseRibbon2;
			break;
		case 'f':
			if(str == "flowChartAlternateProcess") return OOXMLShapes::sptCFlowChartAlternateProcess;
			if(str == "flowChartCollate") return OOXMLShapes::sptCFlowChartCollate;
			if(str == "flowChartConnector") return OOXMLShapes::sptCFlowChartConnector;
			if(str == "flowChartDecision") return OOXMLShapes::sptCFlowChartDecision;
			if(str == "flowChartDelay") return OOXMLShapes::sptCFlowChartDelay;
			if(str == "flowChartDisplay") return OOXMLShapes::sptCFlowChartDisplay;
			if(str == "flowChartDocument") return OOXMLShapes::sptCFlowChartDocument;
			if(str == "flowChartExtract") return OOXMLShapes::sptCFlowChartExtract;
			if(str == "flowChartInputOutput") return OOXMLShapes::sptCFlowChartInputOutput;
			if(str == "flowChartInternalStorage") return OOXMLShapes::sptCFlowChartInternalStorage;
			if(str == "flowChartMagneticDisk") return OOXMLShapes::sptCFlowChartMagneticDisk;
			if(str == "flowChartMagneticDrum") return OOXMLShapes::sptCFlowChartMagneticDrum;
			if(str == "flowChartMagneticTape") return OOXMLShapes::sptCFlowChartMagneticTape;
			if(str == "flowChartManualInput") return OOXMLShapes::sptCFlowChartManualInput;
			if(str == "flowChartManualOperation") return OOXMLShapes::sptCFlowChartManualOperation;
			if(str == "flowChartMerge") return OOXMLShapes::sptCFlowChartMerge;
			if(str == "flowChartMultidocument") return OOXMLShapes::sptCFlowChartMultidocument;
			if(str == "flowChartOfflineStorage") return OOXMLShapes::sptCFlowChartOfflineStorage;
			if(str == "flowChartOffpageConnector") return OOXMLShapes::sptCFlowChartOffpageConnector;
			if(str == "flowChartOnlineStorage") return OOXMLShapes::sptCFlowChartOnlineStorage;
			if(str == "flowChartOr") return OOXMLShapes::sptCFlowChartOr;
			if(str == "flowChartPredefinedProcess") return OOXMLShapes::sptCFlowChartPredefinedProcess;
			if(str == "flowChartPreparation") return OOXMLShapes::sptCFlowChartPreparation;
			if(str == "flowChartProcess") return OOXMLShapes::sptCFlowChartProcess;
			if(str == "flowChartPunchedCard") return OOXMLShapes::sptCFlowChartPunchedCard;
			if(str == "flowChartPunchedTape") return OOXMLShapes::sptCFlowChartPunchedTape;
			if(str == "flowChartSort") return OOXMLShapes::sptCFlowChartSort;
			if(str == "flowChartSummingJunction") return OOXMLShapes::sptCFlowChartSummingJunction;
			if(str == "flowChartTerminator") return OOXMLShapes::sptCFlowChartTerminator;
			if(str == "foldedCorner") return OOXMLShapes::sptCFoldedCorner;
			if(str == "frame") return OOXMLShapes::sptCFrame;
			if(str == "funnel") return OOXMLShapes::sptCFunnel;
			break;
		case 'g':
			if(str == "gear6") return OOXMLShapes::sptCGear6;
			if(str == "gear9") return OOXMLShapes::sptCGear9;
			break;
		case 'h':
			if(str == "halfFrame") return OOXMLShapes::sptCHalfFrame;
			if(str == "heart") return OOXMLShapes::sptCHeart;
			if(str == "heptagon") return OOXMLShapes::sptCHeptagon;
			if(str == "hexagon") return OOXMLShapes::sptCHexagon;
			if(str == "homePlate") return OOXMLShapes::sptCHomePlate;
			if(str == "horizontalScroll") return OOXMLShapes::sptCHorizontalScroll;
			break;
		case 'i':
			if(str == "irregularSeal1") return OOXMLShapes::sptCIrregularSeal1;
			if(str == "irregularSeal2") return OOXMLShapes::sptCIrregularSeal2;
			break;
		case 'l':
			if(str == "leftArrow") return OOXMLShapes::sptCLeftArrow;
			if(str == "leftArrowCallout") return OOXMLShapes::sptCLeftArrowCallout;
			if(str == "leftBrace") return OOXMLShapes::sptCLeftBrace;
			if(str == "leftBracket") return OOXMLShapes::sptCLeftBracket;
			if(str == "leftCircularArrow") return OOXMLShapes::sptCLeftCircularArrow;
			if(str == "leftRightArrow") return OOXMLShapes::sptCLeftRightArrow;
			if(str == "leftRightArrowCallout") return OOXMLShapes::sptCLeftRightArrowCallout;
			if(str == "leftRightCircularArrow") return OOXMLShapes::sptCLeftRightCircularArrow;
			if(str == "leftRightRibbon") return OOXMLShapes::sptCLeftRightRibbon;
			if(str == "leftRightUpArrow") return OOXMLShapes::sptCLeftRightUpArrow;
			if(str == "leftUpArrow") return OOXMLShapes::sptCLeftUpArrow;
			if(str == "lightningBolt") return OOXMLShapes::sptCLightningBolt;
			if(str == "line") return OOXMLShapes::sptCLine;
			if(str == "lineInv") return OOXMLShapes::sptCLineInv;
			break;
		case 'm':
			if(str == "mathDivide") return OOXMLShapes::sptCMathDivide;
			if(str == "mathEqual") return OOXMLShapes::sptCMathEqual;
			if(str == "mathMinus") return OOXMLShapes::sptCMathMinus;
			if(str == "mathMultiply") return OOXMLShapes::sptCMathMultiply;
			if(str == "mathNotEqual") return OOXMLShapes::sptCMathNotEqual;
			if(str == "mathPlus") return OOXMLShapes::sptCMathPlus;
			if(str == "moon") return OOXMLShapes::sptCMoon;
			break;
		case 'n':
			if(str == "nonIsoscelesTrapezoid") return OOXMLShapes::sptCNonIsoscelesTrapezoid;
			if(str == "noSmoking") return OOXMLShapes::sptCNoSmoking;
			if(str == "notchedRightArrow") return OOXMLShapes::sptCNotchedRightArrow;
			break;
		case 'o':
			if(str == "octagon") return OOXMLShapes::sptCOctagon;
			break;
		case 'p':
			if(str == "parallelogram") return OOXMLShapes::sptCParallelogram;
			if(str == "pentagon") return OOXMLShapes::sptCPentagon;
			if(str == "pie") return OOXMLShapes::sptCPie;
			if(str == "pieWedge") return OOXMLShapes::sptCPieWedge;
			if(str == "plaque") return OOXMLShapes::sptCPlaque;
			if(str == "plaqueTabs") return OOXMLShapes::sptCPlaqueTabs;
			if(str == "plus") return OOXMLShapes::sptCPlus;
			break;
		case 'q':
			if(str == "quadArrow") return OOXMLShapes::sptCQuadArrow;
			if(str == "quadArrowCallout") return OOXMLShapes::sptCQuadArrowCallout;
			break;
		case 'r':
			if(str == "rect") return OOXMLShapes::sptCRect;
			if(str == "ribbon") return OOXMLShapes::sptCRibbon;
			if(str == "ribbon2") return OOXMLShapes::sptCRibbon2;
			if(str == "rightArrow") return OOXMLShapes::sptCRightArrow;
			if(str == "rightArrowCallout") return OOXMLShapes::sptCRightArrowCallout;
			if(str == "rightBrace") return OOXMLShapes::sptCRightBrace;
			if(str == "rightBracket") return OOXMLShapes::sptCRightBracket;
			if(str == "round1Rect") return OOXMLShapes::sptCRound1Rect;
			if(str == "round2DiagRect") return OOXMLShapes::sptCRound2DiagRect;
			if(str == "round2SameRect") return OOXMLShapes::sptCRound2SameRect;
			if(str == "roundRect") return OOXMLShapes::sptCRoundRect;
			if(str == "rtTriangle") return OOXMLShapes::sptCRtTriangle;
			break;
		case 's':
			if(str == "smileyFace") return OOXMLShapes::sptCSmileyFace;
			if(str == "snip1Rect") return OOXMLShapes::sptCSnip1Rect;
			if(str == "snip2DiagRect") return OOXMLShapes::sptCSnip2DiagRect;
			if(str == "snip2SameRect") return OOXMLShapes::sptCSnip2SameRect;
			if(str == "snipRoundRect") return OOXMLShapes::sptCSnipRoundRect;
			if(str == "squareTabs") return OOXMLShapes::sptCSquareTabs;
			if(str == "star10") return OOXMLShapes::sptCStar10;
			if(str == "star12") return OOXMLShapes::sptCStar12;
			if(str == "star16") return OOXMLShapes::sptCStar16;
			if(str == "star24") return OOXMLShapes::sptCStar24;
			if(str == "star32") return OOXMLShapes::sptCStar32;
			if(str == "star4") return OOXMLShapes::sptCStar4;
			if(str == "star5") return OOXMLShapes::sptCStar5;
			if(str == "star6") return OOXMLShapes::sptCStar6;
			if(str == "star7") return OOXMLShapes::sptCStar7;
			if(str == "star8") return OOXMLShapes::sptCStar8;
			if(str == "straightConnector1") return OOXMLShapes::sptCStraightConnector1;
			if(str == "stripedRightArrow") return OOXMLShapes::sptCStripedRightArrow;
			if(str == "sun") return OOXMLShapes::sptCSun;
			if(str == "swooshArrow") return OOXMLShapes::sptCSwooshArrow;
			break;
		case 't':
			if(str == "teardrop") return OOXMLShapes::sptCTeardrop;
			if(str == "trapezoid") return OOXMLShapes::sptCTrapezoid;
			if(str == "triangle") return OOXMLShapes::sptCTriangle;
			break;
		case 'u':
			if(str == "upArrow") return OOXMLShapes::sptCUpArrow;
			if(str == "upArrowCallout") return OOXMLShapes::sptCUpArrowCallout;
			if(str == "upDownArrow") return OOXMLShapes::sptCUpDownArrow;
			if(str == "upDownArrowCallout") return OOXMLShapes::sptCUpDownArrowCallout;
			if(str == "uturnArrow") return OOXMLShapes::sptCUturnArrow;
			break;
		case 'v': 
			if(str == "verticalScroll") return OOXMLShapes::sptCVerticalScroll;
			break;
		case 'w':
			if(str == "wave") return OOXMLShapes::sptCWave;
			if(str == "wedgeEllipseCallout") return OOXMLShapes::sptCWedgeEllipseCallout;
			if(str == "wedgeRectCallout") return OOXMLShapes::sptCWedgeRectCallout;
			if(str == "wedgeRoundRectCallout") return OOXMLShapes::sptCWedgeRoundRectCallout;
			break;
		}
		return OOXMLShapes::sptNil;
	}
}