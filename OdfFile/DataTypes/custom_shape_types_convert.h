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

struct _shape_converter
{
	std::wstring odf_reader;
	std::wstring oox;
	int count_values;
	int min;
	int max;
};

static const _shape_converter _OO_OOX_custom_shapes[]= 
{
{L"rectangle"					,L"rect"	,0	,0	,0	},
{L"diamond"						,L"diamond"	,0	,0	,0	},
{L"parallelogram"				,L"parallelogram"	,0	,0	,0	},
{L"isosceles-triangle"			,L"triangle"	,0	,0	,0	},
{L"right-triangle"				,L"rtTriangle"	,0	,0	,0	},
{L"trapezoid"					,L"trapezoid"	,0	,0	,0	},
{L"pentagon"					,L"pentagon"	,0	,0	,0	},
{L"hexagon"						,L"hexagon"	,0	,0	,0	},
{L"octagon"						,L"octagon"	,0	,0	,0	},
{L"star4"						,L"star4"	,1	,0	,50000	},
{L"star5"						,L"star5"	,0	,0	,50000	},//??? 19098 неверно
{L"star8"						,L"star8"	,1	,0	,50000	},
{L"star24"						,L"star24"	,1	,0	,50000	},
{L"star6"						,L"star6"	,0	,0	,50000	},
{L"star12"						,L"star12"	,1	,0	,50000	},
{L"round-rectangle"				,L"roundRect"	,0	,0	,0	},
{L"ellipse"						,L"ellipse"	,0	,0	,0	},
{L"chevron"						,L"chevron"	,0	,0	,0	},
{L"block-arc"					,L"blockArc"	,0	,0	,0	},
{L"ring"						,L"donut"	,0	,0	,0	},
{L"right-arrow"					,L"rightArrow"	,0	,0	,0	},
{L"left-arrow"					,L"leftArrow"	,0	,0	,0	},
{L"up-arrow"					,L"upArrow"	,0	,0	,0	},
{L"down-arrow"					,L"downArrow"	,0	,0	,0	},
{L"striped-right-arrow"			,L"stripedRightArrow"	,0	,0	,0	},
{L"notched-right-arrow"			,L"notchedRightArrow"	,0	,0	,0	},
{L"left-right-arrow"			,L"leftRightArrow"	,0	,0	,0	},
{L"up-down-arrow"				,L"upDownArrow"	,0	,0	,0	},
{L"quad-arrow"					,L"quadArrow"	,0	,0	,0	},
{L"left-arrow-callout"			,L"leftArrowCallout"	,0	,0	,0	},
{L"right-arrow-callout"			,L"rightArrowCallout"	,0	,0	,0	},
{L"up-arrow-callout"			,L"upArrowCallout"	,0	,0	,0	},
{L"down-arrow-callout"			,L"downArrowCallout"	,0	,0	,0	},
{L"left-right-arrow-callout"	,L"leftRightArrowCallout"	,0	,0	,0	},
{L"up-down-arrow-callout"		,L"upDownArrowCallout"	,0	,0	,0	},
{L"quad-arrow-callout"			,L"quadArrowCallout"	,0	,0	,0	},
{L"circular-arrow"				,L"circularArrow"	,0	,0	,0	},
{L"can"							,L"can"	,0	,0	,0	},
{L"lightning"					,L"lightningBolt"	,0	,0	,0	},
{L"heart"						,L"heart"	,0	,0	,0	},
{L"sun"							,L"sun"	,1	,46875	,12500	},
{L"moon"						,L"moon"	,0	,0	,0	},
{L"smiley"						,L"smileyFace"	,1	,-4652	,4652	},
{L"bracket-pair"				,L"bracketPair"	,0	,0	,0	},
{L"brace-pair"					,L"bracePair"	,0	,0	,0	},
{L"line-callout-1"				,L"callout1"	,4	,0	,0	},
{L"line-callout-2"				,L"callout2"	,6	,0	,0	},
{L"line-callout-3"				,L"callout3"	,8	,0	,0	},
{L"rectangular-callout"			,L"wedgeRectCallout"	,2	,0	,842870	},
{L"round-rectangular-callout"	,L"wedgeRoundRectCallout"	,0	,0	,842870	},
{L"round-callout"				,L"wedgeEllipseCallout"	,0	,0	,842870	},//2
{L"cloud-callout"				,L"cloudCallout"	,0	,0	,842870	},//2
{L"vertical-scroll"				,L"verticalScroll"	,1	,0	,25000	},
{L"horizontal-scroll"			,L"horizontalScroll"	,1	,0	,25000	},
{L"flowchart-process"			,L"flowChartProcess"	,0	,0	,0	},
{L"flowchart-decision"			,L"flowChartDecision"	,0	,0	,0	},
{L"flowchart-data"				,L"flowChartInputOutput"	,0	,0	,0	},
{L"flowchart-predefined-process"	,L"flowChartPredefinedProcess"	,0	,0	,0	},
{L"flowchart-internal-storage"		,L"flowChartInternalStorage"	,0	,0	,0	},
{L"flowchart-document"				,L"flowChartDocument"	,0	,0	,0	},
{L"flowchart-multidocument"			,L"flowChartMultidocument"	,0	,0	,0	},
{L"flowchart-terminator"			,L"flowChartTerminator"	,0	,0	,0	},
{L"flowchart-preparation"			,L"flowChartPreparation"	,0	,0	,0	},
{L"flowchart-manual-input"			,L"flowChartManualInput"	,0	,0	,0	},
{L"flowchart-manual-operation"		,L"flowChartManualOperation"	,0	,0	,0	},
{L"flowchart-connector"				,L"flowChartConnector"	,0	,0	,0	},
{L"flowchart-card"					,L"flowChartPunchedCard"	,0	,0	,0	},
{L"flowchart-punched-tape"			,L"flowChartPunchedTape"	,0	,0	,0	},
{L"flowchart-summing-junction"		,L"flowChartSummingJunction"	,0	,0	,0	},
{L"flowchart-or"					,L"flowChartOr"	,0	,0	,0	},
{L"flowchart-collate"				,L"flowChartCollate"	,0	,0	,0	},
{L"flowchart-sort"					,L"flowChartSort"	,0	,0	,0	},
{L"flowchart-extract"				,L"flowChartExtract"	,0	,0	,0	},
{L"flowchart-merge"					,L"flowChartMerge"	,0	,0	,0	},
{L"flowchart-stored-data"			,L"flowChartOnlineStorage"	,0	,0	,0	},
{L"flowchart-sequential-access"		,L"flowChartMagneticTape"	,0	,0	,0	},
{L"flowchart-magnetic-disk"			,L"flowChartMagneticDisk"	,0	,0	,0	},
{L"flowchart-direct-access-storage"	,L"flowChartMagneticDrum"	,0	,0	,0	},
{L"flowchart-display"				,L"flowChartDisplay"	,0	,0	,0	},
{L"flowchart-delay"					,L"flowChartDelay"	,0	,0	,0	},
{L"flowchart-alternate-process"		,L"flowChartAlternateProcess"	,0	,0	,0	},
{L"flowchart-off-page-connector"	,L"flowChartOffpageConnector"	,0	,0	,0	},
{L"left-brace"						,L"leftBrace" 	,0	,0	,0	},
{L"right-brace"						,L"rightBrace" 	,0	,0	,0	},
{L"left-bracket"					,L"leftBracket" 	,0	,0	,0	},
{L"right-bracket"					,L"rightBracket" 	,0	,0	,0	},
{L"forbidden"						,L"noSmoking"	,0	,0	,0	},
{L"paper"							,L"foldedCorner"	,0	,0	,0	},
{L"pentagon-right"					,L"homePlate"	,0	,0	,0	},
{L"cube"							,L"cube"	,0	,0	,0	},
{L"cross"							,L"plus"	,0	,0	,0	},
{L"bang"							,L"irregularSeal2"	,0	,0	,0	},
{L"mso-spt14"						,L"thickArrow" 	,0	,0	,0	},
{L"mso-spt17"						,L"Balloon" 	,0	,0	,0	},
{L"mso-spt18"						,L"irregularSeal1"	,0	,0	,0	},
{L"mso-spt19"						,L"rect"	,0	,0	,0	},
{L"mso-spt24"						,L"textBox" 	,0	,0	,0	},
{L"mso-spt32"				,L"straightConnector1"	,0	,0	,0	},
{L"mso-spt33"				,L"bentConnector2"	,0	,0	,0	},
{L"mso-spt34"				,L"bentConnector3"	,0	,0	,0	},
{L"mso-spt35"				,L"bentConnector4"	,0	,0	,0	},
{L"mso-spt36"				,L"bentConnector5"	,0	,0	,0	},
{L"mso-spt37"				,L"curvedConnector2"	,0	,0	,0	},
{L"mso-spt38"				,L"curvedConnector3"	,0	,0	,0	},
{L"mso-spt39"				,L"curvedConnector4"	,0	,0	,0	},
{L"mso-spt40"				,L"curvedConnector5"	,0	,0	,0	},
{L"mso-spt41"				,L"callout1"	,4	,0	,0	},
{L"mso-spt42"				,L"callout2"	,6	,0	,0	},
{L"mso-spt43"				,L"callout3"	,8	,0	,0	},
{L"mso-spt44"				,L"accentCallout1"	,4	,0	,0	},
{L"mso-spt45"				,L"accentCallout2"	,6	,0	,0	},
{L"mso-spt46"				,L"accentCallout3"	,8	,0	,0	},
{L"mso-spt49"				,L"borderCallout1"	,4	,0	,0	},
{L"mso-spt50"				,L"borderCallout2"	,6	,0	,0	},
{L"mso-spt51"				,L"borderCallout3"	,8	,0	,0	},
{L"mso-spt53"				,L"ribbon"	,0	,0	,0	},
{L"mso-spt54"				,L"ribbon2"	,0	,0	,0	},
{L"mso-spt59"				,L"irregularSeal1" 	,0	,0	,0	},
{L"mso-spt60"				,L"irregularSeal2" 	,0	,0	,0	},
{L"mso-spt75"				,L"frame" 	,0	,0	,0	},
{L"mso-spt89"				,L"leftUpArrow" 	,0	,0	,0	},
{L"mso-spt90"				,L"bentUpArrow" 	,0	,0	,0	},
{L"mso-spt91"				,L"bentArrow" 	,0	,0	,0	},
{L"mso-spt100"				,L"pie" 	,2	,0	,0	},
{L"mso-spt101"				,L"uturnArrow"	,0	,0	,0	},
{L"mso-spt102"				,L"curvedRightArrow"	,0	,0	,0	},
{L"mso-spt103"				,L"curvedLeftArrow"	,0	,0	,0	},
{L"mso-spt104"				,L"curvedUpArrow"	,0	,0	,0	},
{L"mso-spt105"				,L"curvedDownArrow"	,0	,0	,0	},
{L"mso-spt107"				,L"ellipseRibbon"	,0	,0	,0	},
{L"mso-spt108"				,L"ellipseRibbon2"	,0	,0	,0	},
{L"mso-spt129"				,L"flowChartMagneticDrum" 	,0	,0	,0	},

{L"mso-spt178"				,L"callout1"	,0	,0	,0	},
{L"mso-spt179"				,L"callout2"	,0	,0	,0	},
{L"mso-spt180"				,L"callout3"	,0	,0	,0	},
{L"mso-spt182"				,L"leftRightUpArrow"	,0	,0	,0	},
{L"mso-spt188"				,L"flowChartPunchedTape",0	,0	,0	},
{L"mso-spt189"				,L"actionButtonBlank"	,0	,0	,0	},
{L"mso-spt190"				,L"actionButtonHome"	,0	,0	,0	},
{L"mso-spt191"				,L"actionButtonHelp"	,0	,0	,0	},
{L"mso-spt192"				,L"actionButtonInformation"	,0	,0	,0	},
{L"mso-spt193"				,L"actionButtonForwardNext"	,0	,0	,0	},
{L"mso-spt194"				,L"actionButtonBackPrevious"	,0	,0	,0	},
{L"mso-spt195"				,L"actionButtonEnd"	,0	,0	,0	},
{L"mso-spt196"				,L"actionButtonBeginning"	,0	,0	,0	},
{L"mso-spt197"				,L"actionButtonReturn"	,0	,0	,0	},
{L"mso-spt198"				,L"actionButtonDocument"	,0	,0	,0	},
{L"mso-spt199"				,L"actionButtonSound"	,0	,0	,0	},
{L"mso-spt200"				,L"actionButtonMovie"	,0	,0	,0	},
{L"mso-spt201"				,L"rect"	,0	,0	,0	},
{L"mso-spt202"				,L"textBox"	,0	,0	,0	},
{L"quadrat"					,L"rect"	,0	,0	,0	},
{L"round-quadrat"			,L"roundRect"	,0	,0	,0	},
{L"circle"					,L"ellipse"	,0	,0	,0	},
{L"circle-pie"				,L"pie"	,0	,0	,0	},
{L"frame"					,L"frame"	,0	,0	,0	},
{L"cloud"					,L"cloud"	,0	,0	,0	},
{L"up-right-arrow"			,L"rightUpArrow"	,0	,0	,0	},
{L"col-502ad400"			,L"bevel"	,0	,0	,0	},
{L"col-60da8460"			,L"bevel"	,0	,0	,0	},
{L"quad-bevel"				,L"bevel"	,0	,0	,0	},
{L"flower"					,L""	,0	,0	,0	},
{L"puzzle"					,L""	,0	,0	,0	},
{L"up-right-down-arrow"		,L""	,0	,0	,0	},
{L"corner-right-arrow"		,L""	,0	,0	,0	},
{L"split-arrow"				,L""	,0	,0	,0	},
{L"up-right-arrow-callout"	,L""	,0	,0	,0	},
{L"split-round-arrow"		,L""	,0	,0	,0	},
{L"s-sharped-arrow"			,L""	,0	,0	,0	},
{L"concave-star6"			,L""	,0	,0	,0	},
{L"signet"					,L""	,0	,0	,0	},
{L"doorplate"				,L""	,0	,0	,0	},
{L""						,L"round1Rect"		,0	,0	,0	},
{L""						,L"round2DiagRect"	,0	,0	,0	},
{L""						,L"snip2DiagRect"	,0	,0	,0	},
{L""						,L"round2SameRect"	,0	,0	,0	},
{L""						,L"mathPlus"		,0	,0	,0	},
{L""						,L"mathMinus"		,0	,0	,0	},
{L""						,L"mathMultiply"	,0	,0	,0	},
{L""						,L"mathDivide"		,0	,0	,0	},
{L""						,L"mathEqual"		,0	,0	,0	},
{L""						,L"mathNotEqual"	,0	,0	,0	},
{L""						,L"plaque"			,0	,0	,0	},
{L""						,L"decagon"			,0	,0	,0	},
{L""						,L"dodecagon"		,0	,0	,0	},
{L""						,L"star7"			,0	,0	,0	},
{L""						,L"star10"			,0	,0	,0	},
{L""						,L"star16"			,0	,0	,0	},
{L""						,L"star32"			,0	,0	,0	},
{L""						,L"diagStripe"		,0	,0	,0	},
{L""						,L"teardrop"		,0	,0	,0	},
{L""						,L"arc"				,0	,0	,0	},
{L""						,L"chord"			,0	,0	,0	},
{L""						,L"heptagon"		,0	,0	,0	},
{L""						,L"wave"			,0	,0	,0	},
{L""						,L"doubleWave"		,0	,0	,0	},
{L""						,L"corner"			,0	,0	,0	},
{L""						,L"accentBorderCallout3",0	,0	,0	},
{L""						,L"accentBorderCallout2",0	,0	,0	},
{L""						,L"accentBorderCallout1",0	,0	,0	},
{L""						,L"halfFrame"		,0	,0	,0	},
{L""						,L"leftCircularArrow",0	,0	,0	},
{L""						,L"leftRightRibbon",0	,0	,0	},
{L""						,L"pieWedge",0	,0	,0	},
{L""						,L"swooshArrow",0	,0	,0	}

};

static const _shape_converter _OO_OOX_wordart[]=
//https://msdn.microsoft.com/ru-ru/library/documentformat.openxml.drawing.textshapevalues.aspx?f=255&MSPPError=-2147217396
{
{L"fontwork-plain-text"			,L"textPlain"			,1	,30000	,	70000 },

{L"fontwork-curve-up"			,L"textCurveUp"			,0	,0	,0	},
{L"fontwork-curve-down"			,L"textCurveDown"		,0	,0	,0	},
{L"fontwork-wave"				,L"textWave1"			,0	,0	,0	},

{L"fontwork-fade-right"			,L"textFadeRight"		,0	,0	,0	},
{L"fontwork-fade-left"			,L"textFadeLeft"		,0	,0	,0	},
{L"fontwork-fade-up"			,L"textFadeUp"			,0	,0	,0	},
{L"fontwork-fade-down"			,L"textFadeDown"		,0	,0	,0	},
{L"fontwork-fade-up-and-right"	,L"textFadeUp"			,0	,0	,0	},
{L"fontwork-fade-up-and-left"	,L"textFadeUp"			,0	,0	,0	},

{L"fontwork-arch-left-curve"	,L"textArchLeftCurve"	,0	,0	,0	},
{L"fontwork-arch-right-curve"	,L"textArchRightCurve"	,0	,0	,0	},
{L"fontwork-arch-up-curve"		,L"textArchUpPour"		,0	,0	,0	},
{L"fontwork-arch-down-curve"	,L"textArchDownPour"	,0	,0	,0	},

{L"fontwork-arch-up-pour"		,L"textArchUpPour"		,0	,0	,0	},
{L"fontwork-arch-down-pour"		,L"textArchDownPour"	,0	,0	,0},
{L"fontwork-arch-left-pour"		,L"textArchLeftPour"	,0	,0	,0	},
{L"fontwork-arch-right-pour"	,L"textArchRightPour"	,0	,0	,0	},

{L"fontwork-chevron-up"			,L"textChevron"			,0	,0	,0	},
{L"fontwork-chevron-down"		,L"textChevronInverted"	,0	,0	,0	},
{L"fontwork-triangle-up"		,L"textTriangle"		,0	,0	,0	},
{L"fontwork-triangle-down"		,L"textTriangleInverted"	,0	,0	,0	},

{L"fontwork-circle-pour"		,L"textCirclePour"	,0	,0	,0	},
{L"fontwork-circle-curve"		,L"textCircle"		,0	,0	,0	},
{L"fontwork-open-circle-curve"	,L"textPlain"		,1	,30000	,	70000 },
{L"fontwork-open-circle-pour"	,L"textPlain"		,1	,30000	,	70000 },

{L"fontwork-inflate"			,L"textInflate"		,0	,0	,0	},
{L"fontwork-deflate"			,L"textDeflate"		,0	,0	,0	},
{L"fontwork-slant-up"			,L"textSlantUp"		,0	,0	,0	},
{L"fontwork-slant-down"			,L"textSlantDown"	,0	,0	,0	},

{L"mso-spt25"					,L"textPlain"	,0	,0	,0	},//
{L"mso-spt26"					,L"textPlain"	,0	,0	,0	},
{L"mso-spt27"					,L"textCurveDown"	,0	,0	,0	},
{L"mso-spt28"					,L"textWave1"	,0	,0	,0	},
{L"mso-spt29"					,L"textRingInside"	,0	,0	,0	},
{L"mso-spt30"					,L"textPlain"	,0	,0	,0	},
{L"mso-spt31"					,L"textRingOutside"	,0	,0	,0	},
{L"mso-spt142"					,L"textRingInside"	,0	,0	,0	},
{L"mso-spt143"					,L"textRingOutside"	,0	,0	,0	},
{L"mso-spt157"					,L"textWave2"		,0	,0	,0	},
{L"mso-spt158"					,L"textDoubleWave1"	,0	,0	,0	},
{L"mso-spt159"					,L"textWave4"		,0	,0	,0	},
{L"mso-spt160"					,L"textInflate"		,0	,0	,0	},//???
{L"mso-spt161"					,L"textDeflate"		,0	,0	,0	},
{L"mso-spt162"					,L"textInflateBottom"	,0	,0	,0	},
{L"mso-spt163"					,L"textDeflateBottom"	,0	,0	,0	},
{L"mso-spt164"					,L"textInflateTop"	,0	,0	,0	},
{L"mso-spt165"					,L"textDeflateTop"	,0	,0	,0	},
{L"mso-spt166"					,L"textDeflateInflate"	,0	,0	,0	},
{L"mso-spt167"					,L"textDeflateInflateDeflate"	,0	,0	,0	},
{L"mso-spt174"					,L"textCanUp"		,0	,0	,0	},
{L"mso-spt175"					,L"textCanDown"		,0	,0	,0	}, 
{L"mso-spt188"					,L"textDoubleWave1"	,0	,0	,0	},

};