/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

namespace OfficeArt
{
	namespace Enumerations
	{
		enum MSOSPT
		{
			msosptNotPrimitive					=	0x00000000, //	A shape that has no preset geometry, and is drawn with custom geometry instead. For example, freeform shapes drawn by users fall into this category.
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
			msosptThickArrow					=	0x0000000E, //	This value SHOULD NOT be used.
			msosptHomePlate						=	0x0000000F, //	An irregular pentagon shape.
			msosptCube							=	0x00000010, //	A cube shape.
			msosptBalloon						=	0x00000011, //	A speech balloon shape.
			msosptSeal							=	0x00000012, //	A seal shape.
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
			msosptMoon							=	0x000000B8,	//	A moon shape.
			msosptBracketPair					=	0x000000B9,	//	A shape enclosed in brackets.
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
			msosptTextBox						=	0x000000CA	//	A text box shape.
		};

		enum MSOSHAPEPATH
		{
			msoshapeLines						=	0x00000000,
			msoshapeLinesClosed					=	0x00000001,
			msoshapeCurves						=	0x00000002,
			msoshapeCurvesClosed				=	0x00000003,
			msoshapeComplex						=	0x00000004
		};

		enum MSOFILLTYPE
		{
			msofillSolid						=	0x00000000,	//  A solid fill
			msofillPattern						=	0x00000001,	//  A patterned fill
			msofillTexture						=	0x00000002,	//  A textured fill
			msofillPicture						=	0x00000003,	//  A picture fill
			msofillShade						=	0x00000004,	//  A gradient fill that starts and ends with defined endpoints
			msofillShadeCenter					=	0x00000005,	//  A gradient fill that starts and ends based on the bounds of the shape
			msofillShadeShape					=	0x00000006,	//  A gradient fill that starts on the outline of the shape and ends at a point defined within the shape
			msofillShadeScale					=	0x00000007,	//  A gradient fill that starts on the outline of the shape and ends at a point defined within the shape.  The fill angle is scaled by the aspect ratio of the shape
			msofillShadeTitle					=	0x00000008,	//  A gradient fill interpreted by the host application
			msofillBackground					=	0x00000009	//  A fill that matches the background fill
		};
	}

	namespace Enumerations
	{
		enum MSOLINETYPE
		{
			msolineSolidType					=	0x00000000,	//  A solid fill
			msolinePattern						=	0x00000001,	//  A patterned fill
			msolineTexture						=	0x00000002	//  A textured fill
		};

		enum MSOLINESTYLE
		{
			msolineSimple						=	0x00000000,
			msolineDouble						=	0x00000001,
			msolineThickThin					=	0x00000002,
			msolineThinThick					=	0x00000003,
			msolineTriple						=	0x00000004
		};

		enum MSOLINEDASHING
		{
			msolineSolid						=	0x00000000,
			msolineDashSys						=	0x00000001,
			msolineDotSys						=	0x00000002,
			msolineDashDotSys					=	0x00000003,
			msolineDashDotDotSys				=	0x00000004,
			msolineDotGEL 						=	0x00000005,
			msolineDashGEL						=	0x00000006,
			msolineLongDashGEL					=	0x00000007,
			msolineDashDotGEL					=	0x00000008,
			msolineLongDashDotGEL				=	0x00000009,
			msolineLongDashDotDotGEL			=	0x0000000A
		};

		enum MSOLINECAP
		{
			msolineEndCapRound					=	0x00000000,
			msolineEndCapSquare					=	0x00000001,
			msolineEndCapFlat					=	0x00000002
		};

		enum MSOLINEEND
		{
			msolineNoEnd						=	0x00000000,
			msolineArrowEnd						=	0x00000001,
			msolineArrowStealthEnd				=	0x00000002,
			msolineArrowDiamondEnd				=	0x00000003,
			msolineArrowOvalEnd					=	0x00000004,
			msolineArrowOpenEnd					=	0x00000005,
			msolineArrowChevronEnd				=	0x00000006,	//	A value that MUST be ignored.
			msolineArrowDoubleChevronEnd		=	0x00000007	//	A value that MUST be ignored
		};

		enum MSOLINEENDWIDTH
		{
			msolineNarrowArrow					=	0x00000000,
			msolineMediumWidthArrow				=	0x00000001,
			msolineWideArrow					=	0x00000002
		};

		enum MSOLINEENDLENGTH
		{
			msolineShortArrow					=	0x00000000,
			msolineMediumLenArrow				=	0x00000001,
			msolineLongArrow					=	0x00000002
		};
	}

	namespace Enumerations
	{
		enum MSOWRAPMODE
		{
			msowrapSquare						=	0x00000000,
			msowrapByPoints						=	0x00000001,
			msowrapNone							=	0x00000002,
			msowrapTopBottom					=	0x00000003,
			msowrapThrough						=	0x00000004	
		};

		enum MSOANCHOR
		{
			msoanchorTop						=	0x00000000,
			msoanchorMiddle						=	0x00000001,
			msoanchorBottom						=	0x00000002,
			msoanchorTopCentered				=	0x00000003,
			msoanchorMiddleCentered				=	0x00000004,
			msoanchorBottomCentered				=	0x00000005,
			msoanchorTopBaseline				=	0x00000006,
			msoanchorBottomBaseline				=	0x00000007,
			msoanchorTopCenteredBaseline		=	0x00000008,
			msoanchorBottomCenteredBaseline		=	0x00000009
		};

		enum POSH
		{
			msophAbs							=	0x00000000,
			msophLeft							=	0x00000001, 
			msophCenter							=	0x00000002,
			msophRight							=	0x00000003, 
			msophInside							=	0x00000004, 
			msophOutside						=	0x00000005
		};

		enum POSRELH										// ! Значения не соотвествуют спецификации
		{
			msoprhMargin						=	0x00000001,
			msoprhPage							=	0x00000002,
			msoprhText							=	0x00000003,
			msoprhChar							=	0x00000004
		};

		enum POSV
		{
			msopvAbs							=	0x00000000, 
			msopvTop							=	0x00000001, 
			msopvCenter							=	0x00000002, 
			msopvBottom							=	0x00000003, 
			msopvInside							=	0x00000004, 
			msopvOutside 						=	0x00000005  
		};

		enum POSRELV										// ! Значения не соотвествуют спецификации
		{
			msoprvMargin						=	0x00000001,
			msoprvPage							=	0x00000002, 
			msoprvText							=	0x00000003,
			msoprvLine							=	0x00000004
		};

		enum MSOSHADOWTYPE
		{
			msoshadowOffset						=	0x00000000,
			msoshadowDouble						=	0x00000001,
			msoshadowRich						=	0x00000002,
			msoshadowShape						=	0x00000003,
			msoshadowDrawing					=	0x00000004,
			msoshadowEmbossOrEngrave			=	0x00000005
		};

		enum MSOCX 
		{
			msocxkNone							=	0x00000000,
			msocxkSegments						=	0x00000001,
			msocxkCustom						=	0x00000002,
			msocxkRect							=	0x00000003
		};
	}

	namespace Enumerations
	{
		enum GroupShape
		{
			dxWrapDistLeft						=	0x0384,
			dyWrapDistTop						=	0x0385,
			dxWrapDistRight						=	0x0386,
			dyWrapDistBottom					=	0x0387,
			lidRegroup							=	0x0388,

			posh								=	0x038F,
			posrelh								=	0x0390,
			posv								=	0x0391,
			posrelv								=	0x0392,

			groupShapeBooleanProperties			=	0x03BF
		};

		enum GroupShape2
		{
			pctHoriz							=	0x07C0,
			pctVert								=	0x07C1,
			pctHorizPos							=	0x07C2,
			pctVertPos							=	0x07C3,
			sizerelh							=	0x07C4,
			sizerelv							=	0x07C5
		};

		enum Geometry
		{
			geoLeft								=	0x0140,
			geoTop								=	0x0141,
			geoRight							=	0x0142,
			geoBottom							=	0x0143,

			shapePath							=	0x0144,
			pVertices							=	0x0145,
			pSegmentInfo						=	0x0146,

			pConnectionSites					=	0x0151,
			pConnectionSitesDir					=	0x0152,

			adjustValue							=	0x0147,
			adjust2Value						=	0x0148,
			adjust3Value						=	0x0149,
			adjust4Value						=	0x014A,
			adjust5Value						=	0x014B,
			adjust6Value						=	0x014C,
			adjust7Value						=	0x014D,
			adjust8Value						=	0x014E,

			pAdjustHandles						=	0x0155,
			pGuides								=	0x0156,
			pInscribe							=	0x0157,
			cxk									=	0x0158,
			geometryBooleanProperties			=	0x017F
		};

		enum FillStyle
		{
			fillType							=	0x0180,
			fillColor							=	0x0181,
			fillOpacity							=	0x0182,
			fillBackColor						=	0x0183,
			fillBackOpacity						=	0x0184,
			fillCrMod							=	0x0185,
			fillBlip							=	0x0186,

			fillStyleBooleanProperties			=	0x01BF
		};

		enum LineStyle
		{
			lineColor							=	0x01C0,
			lineOpacity							=	0x01C1,
			lineBackColor						=	0x01C2,
			lineCrMod							=	0x01C3,
			lineType							=	0x01C4,

			lineFillBlip						=	0x01C5,
			lineWidth							=	0x01CB,

			lineStyle							=	0x01CD,
			lineDashing							=	0x01CE,

			lineStartArrowhead					=	0x01D0,
			lineEndArrowhead					=	0x01D1,
			lineStartArrowWidth					=	0x01D2,
			lineStartArrowLength				=	0x01D3,
			lineEndArrowWidth					=	0x01D4,
			lineEndArrowLength					=	0x01D5,
			lineJoinStyle						=	0x01D6,
			lineEndCapStyle						=	0x01D7,

			lineStyleBooleanProperties			=	0x01FF
		};

		enum ShadowStyle
		{
			shadowType							=	0x0200,
			shadowColor							=	0x0201,
			shadowOpacity						=	0x0204,
			shadowOffsetX						=	0x0205,
			shadowOffsetY						=	0x0206,

			shadowOriginX						=	0x0210,
			shadowOriginY						=	0x0211,	// ошибка в спецификации (id-кривой)

			shadowScaleXToX						=	0x209,
			shadowScaleYToX						=	0x20A,
			shadowScaleXToY						=	0x20B,
			shadowScaleYToY						=	0x20C,
			shadowPerspectiveX					=	0x20D,
			shadowPerspectiveY					=	0x20E,

			shadowSoftness						=	0x021C,
			shadowStyleBooleanProperties		=	0x023F
		};

		enum Diagram
		{

			diagramBooleans						=	0x053F	
		};

		enum Transform
		{

			rotation							=	0x0004

		};

		enum Protection
		{
			protectionBooleans					=	0x007F
		};

		enum Text
		{
			lTxid								=	0x0080,
			dxTextLeft							=	0x0081,
			dyTextTop							=	0x0082,
			dxTextRight							=	0x0083,
			dyTextBottom						=	0x0084,
			wrapText							=	0x0085,
			anchorText							=	0x0087,
			txflTextFlow						=	0x0088,
			cdirFont							=	0x0089,
			hspNext								=	0x008A,
			txdir								=	0x008B,
			textBooleanProperties				=	0x00BF
		};

		enum Blip
		{
			cropFromTop							=	0x0100,
			cropFromBottom						=	0x0101,
			cropFromLeft						=	0x0102,
			cropFromRight						=	0x0103,

			pib									=	0x0104,

			blipBooleanProperties				=	0x013F
		};
	}
}