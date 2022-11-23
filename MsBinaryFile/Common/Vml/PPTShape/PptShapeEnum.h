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

namespace PPTShapes
{
	enum ShapeType : unsigned short
	{
		sptMin = 0,
		sptNotPrimitive = sptMin,
		sptCRect = 1,
		sptCRoundRect = 2,
		sptCEllipse = 3,
		sptCDiamond = 4,
		sptCIsocelesTriangle = 5,//
		sptCRtTriangle = 6,
		sptCParallelogram = 7,
		sptCTrapezoid = 8,
		sptCHexagon = 9,
		sptCOctagon = 10,
		sptCPlus = 11,
		sptCStar5 = 12,//
		sptCRightArrow = 13,
		sptCThickArrow = 14,//
		sptCHomePlate = 15,
		sptCCube = 16,
		sptCBalloon = 17,//
		sptCSeal = 18,//
		sptCArc = 19,
		sptCLine = 20,
		sptCPlaque = 21,
		sptCCan = 22,
		sptCDonut = 23,
		sptCTextSimple = 24,//
		sptCTextOctagon = 25,//
		sptCTextHexagon = 26,//
		sptCTextCurve = 27,//
		sptCTextWave = 28,//
		sptCTextRing = 29,//
		sptCTextOnCurve = 30,//
		sptCTextOnRing = 31,//
		sptCStraightConnector1 = 32,
		sptCBentConnector2 = 33,
		sptCBentConnector3 = 34,
		sptCBentConnector4 = 35,
		sptCBentConnector5 = 36,
		sptCCurvedConnector2 = 37,
		sptCCurvedConnector3 = 38,
		sptCCurvedConnector4 = 39,
		sptCCurvedConnector5 = 40,
		sptCCallout1 = 41,
		sptCCallout2 = 42,
		sptCCallout3 = 43,
		sptCAccentCallout1 = 44,
		sptCAccentCallout2 = 45,
		sptCAccentCallout3 = 46,
		sptCBorderCallout1 = 47,
		sptCBorderCallout2 = 48,
		sptCBorderCallout3 = 49,
		sptCAccentBorderCallout1 = 50,
		sptCAccentBorderCallout2 = 51,
		sptCAccentBorderCallout3 = 52,
		sptCRibbon = 53,
		sptCRibbon2 = 54,
		sptCChevron = 55,
		sptCPentagon = 56,
		sptCNoSmoking = 57,
		sptCStar8 = 58,//
		sptCStar16 = 59,//
		sptCStar32 = 60,//
		sptCWedgeRectCallout = 61,
		sptCWedgeRoundRectCallout = 62,
		sptCWedgeEllipseCallout = 63,
		sptCWave = 64,
		sptCFoldedCorner = 65,
		sptCLeftArrow = 66,
		sptCDownArrow = 67,
		sptCUpArrow = 68,//
		sptCLeftRightArrow = 69,
		sptCUpDownArrow = 70,
		sptCIrregularSeal1 = 71,
		sptCIrregularSeal2 = 72,
		sptCLightningBolt = 73,
		sptCHeart = 74,
		sptCFrame = 75,
		sptCQuadArrow = 76,
		sptCLeftArrowCallout = 77,
		sptCRightArrowCallout = 78,
		sptCUpArrowCallout = 79,
		sptCDownArrowCallout = 80,
		sptCLeftRightArrowCallout = 81,
		sptCUpDownArrowCallout = 82,
		sptCQuadArrowCallout = 83,
		sptCBevel = 84,
		sptCLeftBracket = 85,
		sptCRightBracket = 86,
		sptCLeftBrace = 87,
		sptCRightBrace = 88,
		sptCLeftUpArrow = 89,
		sptCBentUpArrow = 90,
		sptCBentArrow = 91,
		sptCStar24 = 92,//
		sptCStripedRightArrow = 93,
		sptCNotchedRightArrow = 94,
		sptCBlockArc = 95,
		sptCSmileyFace = 96,
		sptCVerticalScroll = 97,
		sptCHorizontalScroll = 98,
		sptCCircularArrow = 99,
		sptCNotchedCircularArrow = 100,//
		sptCUturnArrow = 101,
		sptCCurvedRightArrow = 102,
		sptCCurvedLeftArrow = 103,
		sptCCurvedUpArrow = 104,
		sptCCurvedDownArrow = 105,
		sptCCloudCallout = 106,
		sptCEllipseRibbon = 107,
		sptCEllipseRibbon2 = 108,
		sptCFlowChartProcess = 109,
		sptCFlowChartDecision = 110,
		sptCFlowChartInputOutput = 111,
		sptCFlowChartPredefinedProcess = 112,
		sptCFlowChartInternalStorage = 113,
		sptCFlowChartDocument = 114,
		sptCFlowChartMultidocument = 115,
		sptCFlowChartTerminator = 116,
		sptCFlowChartPreparation = 117,
		sptCFlowChartManualInput = 118,
		sptCFlowChartManualOperation = 119,
		sptCFlowChartConnector = 120,
		sptCFlowChartPunchedCard = 121,
		sptCFlowChartPunchedTape = 122,
		sptCFlowChartSummingJunction = 123,
		sptCFlowChartOr = 124,
		sptCFlowChartCollate = 125,
		sptCFlowChartSort = 126,
		sptCFlowChartExtract = 127,
		sptCFlowChartMerge = 128,
		sptCFlowChartOfflineStorage = 129,
		sptCFlowChartOnlineStorage = 130,
		sptCFlowChartMagneticTape = 131,
		sptCFlowChartMagneticDisk = 132,
		sptCFlowChartMagneticDrum = 133,
		sptCFlowChartDisplay = 134,
		sptCFlowChartDelay = 135,
		sptCTextPlain = 136,
		sptCTextStop = 137,
		sptCTextTriangle = 138,
		sptCTextTriangleInverted = 139,
		sptCTextChevron = 140,
		sptCTextChevronInverted = 141,
		sptCTextRingInside = 142,
		sptCTextRingOutside = 143,
		sptCTextArchUp = 144,
		sptCTextArchDown = 145,
		sptCTextCircle = 146,
		sptCTextButton = 147,
		sptCTextArchUpPour = 148,
		sptCTextArchDownPour = 149,
		sptCTextCirclePour = 150,
		sptCTextButtonPour = 151,
		sptCTextCurveUp = 152,
		sptCTextCurveDown = 153,
		sptCTextCascadeUp = 154,
		sptCTextCascadeDown = 155,
		sptCTextWave1 = 156,
		sptCTextWave2 = 157,
		sptCTextWave3 = 158,//
		sptCTextWave4 = 159,
		sptCTextInflate = 160,
		sptCTextDeflate = 161,
		sptCTextInflateBottom = 162,
		sptCTextDeflateBottom = 163,
		sptCTextInflateTop = 164,
		sptCTextDeflateTop = 165,
		sptCTextDeflateInflate = 166,
		sptCTextDeflateInflateDeflate = 167,
		sptCTextFadeRight = 168,
		sptCTextFadeLeft = 169,
		sptCTextFadeUp = 170,
		sptCTextFadeDown = 171,
		sptCTextSlantUp = 172,
		sptCTextSlantDown = 173,
		sptCTextCanUp = 174,
		sptCTextCanDown = 175,
		sptCFlowChartAlternateProcess = 176,
		sptCFlowChartOffpageConnector = 177,
		sptCCallout90 = 178,//
		sptCAccentCallout90 = 179,//
		sptCBorderCallout90 = 180,//
		sptCAccentBorderCallout90 = 181,//
		sptCLeftRightUpArrow = 182,
		sptCSun = 183,
		sptCMoon = 184,
		sptCBracketPair = 185,
		sptCBracePair = 186,
		sptCStar4 = 187,//
		sptCDoubleWave = 188,
		sptCActionButtonBlank = 189,
		sptCActionButtonHome = 190,
		sptCActionButtonHelp = 191,
		sptCActionButtonInformation = 192,
		sptCActionButtonForwardNext = 193,
		sptCActionButtonBackPrevious = 194,
		sptCActionButtonEnd = 195,
		sptCActionButtonBeginning = 196,
		sptCActionButtonReturn = 197,
		sptCActionButtonDocument = 198,
		sptCActionButtonSound = 199,
		sptCActionButtonMovie = 200,
		sptCHostControl = 201,//
		sptCTextBox = 202,//

		//далее номеров не имеют и упорядочены по алфавиту
		sptCChartPlus = 203,
		sptCChartStar,
		sptCChartX,
		sptCChord,
		sptCCloud,
		sptCCorner,
		sptCCornerTabs,
		sptCDecagon,
		sptCDiagStripe,
		sptCDodecagon,
		sptCFunnel,
		sptCGear6,
		sptCGear9,
		sptCHalfFrame,
		sptCHeptagon,
		sptCLeftCircularArrow,
		sptCLeftRightCircularArrow,
		sptCLeftRightRibbon,
		sptCLineInv,
		sptCMathDivide,
		sptCMathEqual,
		sptCMathMinus,
		sptCMathMultiply,
		sptCMathNotEqual,
		sptCMathPlus,
		sptCNonIsoscelesTrapezoid,
		sptCPie,
		sptCPieWedge,
		sptCPlaqueTabs,
		sptCRound1Rect,
		sptCRound2DiagRect,
		sptCRound2SameRect,
		sptCSnip1Rect,
		sptCSnip2DiagRect,
		sptCSnip2SameRect,
		sptCSnipRoundRect,
		sptCSquareTabs,
		sptCStar10,
		sptCStar12,
		//sptCStar16,
		//sptCStar24,
		//sptCStar32,
		//sptCStar4,
		//sptCStar5,
		sptCStar6,
		sptCStar7,
		//sptCStar8,
		sptCSwooshArrow,
		sptCTeardrop,
		sptCTextDoubleWave1,
		sptCTriangle,

		sptMax,
		sptNil = 0x0FFF,
		sptCustom = 0x1000
	};
}
