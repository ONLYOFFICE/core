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

#include "SimpleTypes_Drawing.h"

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

// Here are simple types Vml from spec Office Open Xml Part4 14
namespace SimpleTypes
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

	//--------------------------------------------------------------------------------
	// EditAs 14.1.3.1 (Part 4)
	//--------------------------------------------------------------------------------

	enum EEditAs
	{
		editasBullseye = 0,
		editasCanvas   = 1,
		editasCycle    = 2,
		editasOrgchart = 3,
		editasRadial   = 4,
		editasStacked  = 5,
		editasVenn     = 6,
	};

	DEFINE_SIMPLE_TYPE(CEditAs, EEditAs, editasCanvas)

	//--------------------------------------------------------------------------------
	// Ext 14.1.3.2 (Part 4)
	//--------------------------------------------------------------------------------

	enum EExt
	{
		extBackwardCompatible = 0,
		extEdit					= 1,
		extView					= 2,
	};

	DEFINE_SIMPLE_TYPE(CExt, EExt, extView)

	//--------------------------------------------------------------------------------
	// FillMethod 14.1.3.3 (Part 4)
	//--------------------------------------------------------------------------------

	enum EFillMethod
	{
		fillmethodAny,
		fillmethodLinear,
		fillmethodLinearSigma,
		fillmethodSigma,
		fillmethodNone
	};

	DEFINE_SIMPLE_TYPE(CFillMethod, EFillMethod, fillmethodNone)

	//--------------------------------------------------------------------------------
	// FillType 14.1.3.4;14.2.3.14 (Part 4)
	//--------------------------------------------------------------------------------

	enum EFillType
	{
		filltypeBackground       = 0,
		filltypeFrame            = 1,
		filltypeGradient         = 2,
		filltypeGradientCenter   = 3,
		filltypeGradientRadial   = 4,
		filltypeGradientUnscaled = 5,
		filltypePattern          = 6,
		filltypeSolid            = 7,
		filltypeTile             = 8
	};

	// Тут разделяем два типа 14.13.4 - m_nType = 0 и 14.2.3.14 - m_nType != 0

	DEFINE_SIMPLE_TYPE_NSV(CFillType, EFillType, filltypeSolid)

	//--------------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CFillType1, EFillType, filltypeSolid)
	private:
		bool m_nType;
	};

	//--------------------------------------------------------------------------------
	// ImageAspect 14.1.3.5 (Part 4)
	//--------------------------------------------------------------------------------

	enum EImageAspect
	{
		imageaspectAtLeast = 0,
		imageaspectAtMost  = 1,
		imageaspectIgnore  = 2,
	};

	DEFINE_SIMPLE_TYPE(CImageAspect, EImageAspect, imageaspectIgnore)

	// -------------------------------------------------------------------------------
	// ShadowType 14.1.3.6 (Part 4)
	// -------------------------------------------------------------------------------

	enum EShadowType
	{
		shadowtypeDouble,
		shadowtypeEmboss,
		shadowtypePerspective,
		shadowtypeSingle,
	};

	DEFINE_SIMPLE_TYPE(CShadowType, EShadowType, shadowtypeSingle)

	//--------------------------------------------------------------------------------
	// StrokeArrowLength 14.1.3.7 (Part 4)
	//--------------------------------------------------------------------------------

	enum EStrokeArrowLength
	{
		strokearrowlengthLong   = 0,
		strokearrowlengthMedium = 1,
		strokearrowlengthShort  = 2,
	};

	DEFINE_SIMPLE_TYPE(CStrokeArrowLength, EStrokeArrowLength, strokearrowlengthMedium)

	//--------------------------------------------------------------------------------
	// StrokeArrowType 14.1.3.8 (Part 4)
	//--------------------------------------------------------------------------------

	enum EStrokeArrowType
	{
		strokearrowtypeBlock   = 0,
		strokearrowtypeClassic = 1,
		strokearrowtypeDiamond = 2,
		strokearrowtypeNone    = 3,
		strokearrowtypeOpen    = 4,
		strokearrowtypeOval    = 5,
	};

	DEFINE_SIMPLE_TYPE(CStrokeArrowType, EStrokeArrowType, strokearrowtypeNone)

	//--------------------------------------------------------------------------------
	// StrokeArrowWidth 14.1.3.9 (Part 4)
	//--------------------------------------------------------------------------------

	enum EStrokeArrowWidth
	{
		strokearrowwidthMedium = 0,
		strokearrowwidthNarrow = 1,
		strokearrowwidthWide   = 2,
	};

	DEFINE_SIMPLE_TYPE(CStrokeArrowWidth, EStrokeArrowWidth, strokearrowwidthMedium)

	//--------------------------------------------------------------------------------
	// StrokeEndCap 14.1.3.10 (Part 4)
	//--------------------------------------------------------------------------------

	enum EStrokeEndCap
	{
		strokeendcapFlat   = 0,
		strokeendcapRound  = 1,
		strokeendcapSqaure = 2,
	};

	DEFINE_SIMPLE_TYPE(CStrokeEndCap, EStrokeEndCap, strokeendcapRound)

	//--------------------------------------------------------------------------------
	// StrokeJoinStyle 14.1.3.11 (Part 4)
	//--------------------------------------------------------------------------------

	enum EStrokeJoinStyle
	{
		strokejoinstyleBevel,
		strokejoinstyleMiter,
		strokejoinstyleRound,
	};

	DEFINE_SIMPLE_TYPE(CStrokeJoinStyle, EStrokeJoinStyle, strokejoinstyleRound)

	//--------------------------------------------------------------------------------
	// StrokeLineStyle 14.1.3.12 (Part 4)
	//--------------------------------------------------------------------------------

	enum EStrokeLineStyle
	{
		strokelinestyleSingle           = 0,
		strokelinestyleThickBetweenThin = 1,
		strokelinestyleThickThin        = 2,
		strokelinestyleThinThick        = 3,
		strokelinestyleThinThin         = 4,
	};

	DEFINE_SIMPLE_TYPE(CStrokeLineStyle, EStrokeLineStyle, strokelinestyleSingle)
}
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// AlternateMathContentType 14.2.3.1 (Part 4)
	//--------------------------------------------------------------------------------

	enum EAlternateMathContentType
	{
		alternatemathcontenttypeOfficeOpenXmlMath = 0,
		alternatemathcontenttypeMathMl            = 1,
	};

	DEFINE_SIMPLE_TYPE(CAlternateMathContentType, EAlternateMathContentType, alternatemathcontenttypeOfficeOpenXmlMath)

	//--------------------------------------------------------------------------------
	// VmlAngle 14.2.3.2 (Part 4)
	//--------------------------------------------------------------------------------

	enum EVmlAngle
	{
		vmlangle30   = 0,
		vmlangle45   = 1,
		vmlangle60   = 2,
		vmlangle90   = 3,
		vmlangleAny  = 4,
		vmlangleAuto = 5
	};

	DEFINE_SIMPLE_TYPE(CVmlAngle, EVmlAngle, vmlangleAuto)

	//--------------------------------------------------------------------------------
	// BWMode 14.2.3.3 (Part 4)
	//--------------------------------------------------------------------------------

	enum EBWMode
	{
		bwmodeAuto					= 0,
		bwmodeBlack             = 1,
		bwmodeBlackTextAndLines = 2,
		bwmodeColor             = 3,
		bwmodeGrayOutline       = 4,
		bwmodeGrayScale         = 5,
		bwmodeHide					= 6,
		bwmodeHighContrast      = 7,
		bwmodeInverseGray       = 8,
		bwmodeLightGrayscale    = 9,
		bwmodeUndrawn           = 10,
		bwmodeWhite             = 11,
	};	

	DEFINE_SIMPLE_TYPE(CBWMode, EBWMode, bwmodeAuto)

	//--------------------------------------------------------------------------------
	// CalloutDrop 14.2.3.4 (Part 4)
	//--------------------------------------------------------------------------------

	class CCalloutDrop
	{
	public:
		CCalloutDrop();

		std::wstring GetValue() const;
		void    SetValue(std::wstring &sValue);


		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CCalloutDrop)

		private:
			std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// ColorMode 14.2.3.6 (Part 4)
	//--------------------------------------------------------------------------------

	enum EColorMode
	{
		colormodeAuto   = 0,
		colormodeCustom = 1,
	};	

	DEFINE_SIMPLE_TYPE(CColorMode, EColorMode, colormodeAuto)

	//--------------------------------------------------------------------------------
	// ConnectorType 14.2.3.7 (Part 4)
	//--------------------------------------------------------------------------------

	enum EConnectorType
	{
		connectortypeCurved   = 0,
		connectortypeElbow    = 1,
		connectortypeNone     = 2,
		connectortypeStraight = 3,
	};	

	DEFINE_SIMPLE_TYPE(CConnectorType, EConnectorType, connectortypeNone)

	//--------------------------------------------------------------------------------
	// ConnectType 14.2.3.8 (Part 4)
	//--------------------------------------------------------------------------------

	enum EConnectType
	{
		connecttypeCustom   = 0,
		connecttypeNone     = 1,
		connecttypeRect     = 2,
		connecttypeSegments = 3,
	};	

	DEFINE_SIMPLE_TYPE(CConnectType, EConnectType, connecttypeNone)

	//--------------------------------------------------------------------------------
	// ContentType 14.2.3.9 (Part 4)
	//--------------------------------------------------------------------------------

	class CContentType
	{
	public:
		CContentType();

		std::wstring GetValue() const;
		void    SetValue(std::wstring &sValue);

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CContentType)

		private:
			std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// DiagramLayout 14.2.3.10 (Part 4)
	//--------------------------------------------------------------------------------

	enum EDiagramLayout
	{
		diagramlayout0 = 0,
		diagramlayout1 = 1,
		diagramlayout2 = 2,
		diagramlayout3 = 3,
	};

	DEFINE_SIMPLE_TYPE(CDiagramLayout, EDiagramLayout, diagramlayout0)

	//--------------------------------------------------------------------------------
	// ExtrusionPlane 14.2.3.11 (Part 4)
	//--------------------------------------------------------------------------------

	enum EExtrusionPlane
	{
		extrusionplaneXY = 0,
		extrusionplaneYZ = 1,
		extrusionplaneZX = 2,
	};

	DEFINE_SIMPLE_TYPE(CExtrusionPlane, EExtrusionPlane, extrusionplaneXY)

	//--------------------------------------------------------------------------------
	// ExtrusionRender 14.2.3.12 (Part 4)
	//--------------------------------------------------------------------------------

	enum EExtrusionRender
	{
		extrusionrenderBoundingCube = 0,
		extrusionrenderSolid        = 1,
		extrusionrenderWireFrame    = 2,
	};

	DEFINE_SIMPLE_TYPE(CExtrusionRender, EExtrusionRender, extrusionrenderSolid)

	//--------------------------------------------------------------------------------
	// ExtructionType 14.2.3.13 (Part 4)
	//--------------------------------------------------------------------------------

	enum EExtrusionType
	{
		extrusiontypeParallel,
		extrusiontypePerspective
	};

	DEFINE_SIMPLE_TYPE(CExtrusionType, EExtrusionType, extrusiontypeParallel)

	//--------------------------------------------------------------------------------
	// How 14.2.3.15 (Part 4)
	//--------------------------------------------------------------------------------

	enum EHow
	{
		howBottom = 0,
		howCenter = 1,
		howLeft   = 2,
		howMiddle = 3,
		howRight  = 4,
		howTop    = 5,
	};

	DEFINE_SIMPLE_TYPE(CHow, EHow, howTop)

	//--------------------------------------------------------------------------------
	// HrAlign 14.2.3.16 (Part 4)
	//--------------------------------------------------------------------------------

	enum EHrAlign
	{
		hralignCenter = 0,
		hralignLeft   = 1,
		hralignRight  = 2,
	};

	DEFINE_SIMPLE_TYPE(CHrAlign, EHrAlign, hralignLeft)

	//--------------------------------------------------------------------------------
	// InsetMode 14.2.3.17 (Part 4)
	//--------------------------------------------------------------------------------

	enum EInsetMode
	{
		insetmodeAuto   = 0,
		insetmodeCustom = 1,
	};

	DEFINE_SIMPLE_TYPE(CInsetMode, EInsetMode, insetmodeAuto)

	//--------------------------------------------------------------------------------
	// OLEDrawAspect 14.2.3.18 (Part 4)
	//--------------------------------------------------------------------------------

	enum EOLEDrawAspect
	{
		oledrawaspectContent = 0,
		oledrawaspectIcon    = 1,
	};

	DEFINE_SIMPLE_TYPE(COLEDrawAspect, EOLEDrawAspect, oledrawaspectIcon)

	//--------------------------------------------------------------------------------
	// OLELinkType 14.2.3.19 (Part 4)
	//--------------------------------------------------------------------------------

	enum EOLELinkType
	{
		olelinktypeBitmap  = 0,
		olelinktypeEmf     = 1,
		olelinktypeJpeg    = 2,
		olelinktypePicture = 3,
		olelinktypePng     = 4,
	};

	DEFINE_SIMPLE_TYPE(COLELinkType, EOLELinkType, olelinktypeBitmap)

	//--------------------------------------------------------------------------------
	// OleType 14.2.3.20 (Part 4)
	//--------------------------------------------------------------------------------

	enum EOLEType
	{
		oletypeEmbed = 0,
		oletypeLink  = 1,
	};

	DEFINE_SIMPLE_TYPE(COLEType, EOLEType, oletypeEmbed)

	//--------------------------------------------------------------------------------
	// OLEUpdateMode 14.2.3.21 (Part 4)
	//--------------------------------------------------------------------------------

	enum EOLEUpdateMode
	{
		oleupdatemodeAlways = 0,
		oleupdatemodeOnCall = 1,
	};

	DEFINE_SIMPLE_TYPE(COLEUpdateMode, EOLEUpdateMode, oleupdatemodeAlways)

	//--------------------------------------------------------------------------------
	// RType 14.2.3.22 (Part 4)
	//--------------------------------------------------------------------------------

	enum ERType
	{
		rtypeAlign     = 0,
		rtypeArc       = 1,
		rtypeCallout   = 2,
		rtypeConnector = 3,
	};

	DEFINE_SIMPLE_TYPE(CRType, ERType, rtypeAlign)

	//--------------------------------------------------------------------------------
	// ScreenSize 14.2.3.23 (Part 4)
	//--------------------------------------------------------------------------------

	enum EScreenSize
	{
		screensize1024x768 = 0,
		screensize1152x862 = 1,
		screensize544x376  = 2,
		screensize640x480  = 3,
		screensize720x512  = 4,
		screensize800x600  = 5,
	};

	DEFINE_SIMPLE_TYPE(CScreenSize, EScreenSize, screensize640x480)

} // SimpleTypes


namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// BorderShadow 14.3.3.1 (Part 4)
	//--------------------------------------------------------------------------------

	enum EBorderShadow
	{
		bordershadowFalse = 0,
		bordershadowTrue  = 1,
	};

	DEFINE_SIMPLE_TYPE(CBorderShadow, EBorderShadow, bordershadowFalse)

	//--------------------------------------------------------------------------------
	// BorderType 14.3.3.2 (Part 4)
	//--------------------------------------------------------------------------------

	enum EBorderType
	{
		bordertypeDash = 0, // (pecifies a line border consisting of a dashed line around the parent object.)
		bordertypeDashDotDot, // (Dash Dot Dot Border)
		bordertypeDashDotStroked, // (Stroked Dash Dot Border)
		bordertypeDashedSmall, // (Small Dash Border)
		bordertypeDot, // (Dotted Border)
		bordertypeDotDash, // (Dot Dash Border)
		bordertypeDouble, // (Double Line Border)
		bordertypeDoubleWave, // (Double Wavy Lines Border)
		bordertypeHairline, // (Hairline Border)
		bordertypeHTMLInset, // (Inset Border)
		bordertypeHTMLOutset, // (Outset Border)
		bordertypeNone, // (No Border)
		bordertypeSingle, // (Single Line Border)
		bordertypeThick, // (Thick Line Border)
		bordertypeThickBetweenThin, // (Thin-thick-thin Border)
		bordertypeThickBetweenThinLarge, // (Large thin-thick-thin Border)
		bordertypeThickBetweenThinSmall, // (Small thin-thick-thin Lines Border)
		bordertypeThickThin, // (Thick Thin Line Border)
		bordertypeThickThinLarge, // (Thick Thin Large Gap Border)
		bordertypeThickThinSmall, // (Small thick-thin lines border)
		bordertypeThinThick, // (Thin Thick Line Border)
		bordertypeThinThickLarge, // (Thin Thick Large Gap Border)
		bordertypeThinThickSmall, // (Thin Thick Small Gap Border)
		bordertypeThreeDEmboss, // (3D Embossed Border)
		bordertypeThreeDEngrave, // (3D Engraved Border)
		bordertypeTriple, // (Triple Line Border)
		bordertypeWave, // (Wavy Border)
	};

	DEFINE_SIMPLE_TYPE(CBorderType, EBorderType, bordertypeNone)

	//--------------------------------------------------------------------------------
	// HorizontalAnchor 14.3.3.3 (Part 4)
	//--------------------------------------------------------------------------------

	enum EHorizontalAnchor
	{
		horizontalanchorChar   = 0,
		horizontalanchorMargin = 1,
		horizontalanchorPage   = 2,
		horizontalanchorText   = 3,
	};

	DEFINE_SIMPLE_TYPE(CHorizontalAnchor, EHorizontalAnchor, horizontalanchorChar)

	//--------------------------------------------------------------------------------
	// VerticalAnchor 14.3.3.4 (Part 4)
	//--------------------------------------------------------------------------------

	enum EVerticalAnchor
	{
		verticalanchorLine   = 0,
		verticalanchorMargin = 1,
		verticalanchorPage   = 2,
		verticalanchorText   = 3,
	};

	DEFINE_SIMPLE_TYPE(CVerticalAnchor, EVerticalAnchor, verticalanchorLine)

	//--------------------------------------------------------------------------------
	// WrapSide 14.3.3.5 (Part 4)
	//--------------------------------------------------------------------------------

	enum EWrapSide
	{
		wrapsideBoth    = 0,
		wrapsideLargest = 1,
		wrapsideLeft    = 2,
		wrapsideRight   = 3,
	};

	DEFINE_SIMPLE_TYPE(CWrapSide, EWrapSide, wrapsideBoth)

	//--------------------------------------------------------------------------------
	// WrapType 14.3.3.6 (Part 4)
	//--------------------------------------------------------------------------------

	enum EWrapType
	{
		wraptypeNone         = 0,
		wraptypeSquare       = 1,
		wraptypeThrough      = 2,
		wraptypeTight        = 3,
		wraptypeTopAndBottom = 4,
	};

	DEFINE_SIMPLE_TYPE(CWrapType, EWrapType, wraptypeNone)

} // SimpleTypes

// Дополнительные простые типы, не входящие в спецификацию
namespace SimpleTypes
{
	namespace Vml
	{

		//--------------------------------------------------------------------------------
		// VmlClientDataObjectType
		//--------------------------------------------------------------------------------

		enum EVmlClientDataObjectType
		{
			vmlclientdataobjecttypeButton		= 0,
			vmlclientdataobjecttypeCheckbox		= 1,
			vmlclientdataobjecttypeDialog		= 2,
			vmlclientdataobjecttypeDrop			= 3,
			vmlclientdataobjecttypeEdit			= 4,
			vmlclientdataobjecttypeGBox			= 5,
			vmlclientdataobjecttypeGroup		= 6,
			vmlclientdataobjecttypeLabel		= 7,
			vmlclientdataobjecttypeLineA		= 8,
			vmlclientdataobjecttypeList			= 9,
			vmlclientdataobjecttypeMovie		= 10,
			vmlclientdataobjecttypeNote			= 11,
			vmlclientdataobjecttypePict			= 12,
			vmlclientdataobjecttypeRadio		= 13,
			vmlclientdataobjecttypeRect			= 14,
			vmlclientdataobjecttypeRectA		= 15,
			vmlclientdataobjecttypeScroll		= 16,
			vmlclientdataobjecttypeShape		= 17,
			vmlclientdataobjecttypeSpin			= 18
		};

		DEFINE_SIMPLE_TYPE(CVmlClientDataObjectType, EVmlClientDataObjectType, vmlclientdataobjecttypeButton)

		//--------------------------------------------------------------------------------
		// VmlCalloutType 14.2.2.2 (type) (Part 4)
		//--------------------------------------------------------------------------------

		enum EVmlCalloutType
		{
			vmlcallouttypeRectangle      = 0,
			vmlcallouttypeRoundRectangle = 1,
			vmlcallouttypeOval           = 2,
			vmlcallouttypeCloud          = 3,
		};

		DEFINE_SIMPLE_TYPE(CVmlCalloutType, EVmlCalloutType, vmlcallouttypeRectangle)

		//--------------------------------------------------------------------------------
		// VmlPath 14.2.2.3 (v) (Part 4)
		//--------------------------------------------------------------------------------

		class CVmlPath
		{
		public:
			CVmlPath();

			std::wstring GetValue() const;
			void SetValue(std::wstring &sValue);

			std::wstring FromString(const std::wstring &sValue);
			std::wstring ToString  () const;

			SimpleTypes_DefaultS(CVmlPath)

			private:
				std::wstring m_sValue;
		};

		//--------------------------------------------------------------------------------
		// VmlDashStyle 14.2.2.1 (dashstyle) (Part 4)
		//--------------------------------------------------------------------------------

		enum EVmlDashStyle
		{
			vmldashstyleSolid           = 0,
			vmldashstyleShortDash       = 1,
			vmldashstyleShortDot        = 2,
			vmldashstyleShortDashDot    = 3,
			vmldashstyleShortDashDotDot = 4,
			vmldashstyleDot             = 5,
			vmldashstyleDash            = 6,
			vmldashstyleLongDash        = 7,
			vmldashstyleDashDot         = 8,
			vmldashstyleLongDashDot     = 9,
			vmldashstyleLongDashDotDot  = 10,
			vmldashstyleCustom          = 11,
		};

		DEFINE_SIMPLE_TYPE_START(CVmlDashStyle, EVmlDashStyle, vmldashstyleSolid)
		private:

			double  m_arrValues[32];
			int     m_nCount = 0;
		};

		//--------------------------------------------------------------------------------
		// Vml_1_65536 14.2.2.11 (brightness)
		//--------------------------------------------------------------------------------

		class CVml_1_65536
		{
		public:
			CVml_1_65536();

			double GetValue() const;

			void SetValue(double dValue);

			void SetValue(int nValue);

			virtual double FromString(const std::wstring &sValue);

			virtual std::wstring ToString  () const;

			SimpleTypes_DefaultD(CVml_1_65536)

			private:
				double m_dValue;
		};

		//--------------------------------------------------------------------------------
		// Vml_Vector3D_65536 14.2.2.11 (lightposition)
		//--------------------------------------------------------------------------------

		class CVml_Vector3D_65536
		{
		public:
			CVml_Vector3D_65536();

			double GetX() const;
			double GetY() const;
			double GetZ() const;

			void   SetValue(int nX, int nY, int nZ);
			void   SetValue(double dX, double dY, double dZ);

			virtual double FromString(const std::wstring &sValue);

			virtual std::wstring ToString  () const;

			SimpleTypes_DefaultString(CVml_Vector3D_65536)

			private:

				int m_nX;
			int m_nY;
			int m_nZ;
		};

		//--------------------------------------------------------------------------------
		// Vml_Vector3D 14.2.2.11 (orientation)
		//--------------------------------------------------------------------------------

		class CVml_Vector3D
		{
		public:
			CVml_Vector3D();

			int GetX() const;
			int GetY() const;
			int GetZ() const;

			void   SetValue(int nX, int nY, int nZ);

			virtual double FromString(const std::wstring &sValue);

			virtual std::wstring ToString  () const;

			SimpleTypes_DefaultString(CVml_Vector3D)

			private:
				int m_nX;
			int m_nY;
			int m_nZ;
		};

		//--------------------------------------------------------------------------------
		// Vml_Vector2D 14.2.2.11 (rotationangle)
		//--------------------------------------------------------------------------------

		class CVml_Vector2D
		{
		public:
			CVml_Vector2D();

			int GetX() const;
			int GetY() const;

			void   SetValue(int nX, int nY);

			virtual double FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_DefaultString(CVml_Vector2D)

			private:
				int m_nX;
			int m_nY;
		};

		//--------------------------------------------------------------------------------
		// Vml_Vector2D_F 14.2.2.11 (viewpointorigin)
		//--------------------------------------------------------------------------------

		class CVml_Vector2D_F
		{
		public:
			CVml_Vector2D_F();

			double GetX() const;
			double GetY() const;

			void   SetValue(double dX, double dY);

			virtual double FromString(const std::wstring &sValue);

			virtual std::wstring ToString  () const;

			SimpleTypes_DefaultString(CVml_Vector2D_F)

			private:
				double m_dX;
			double m_dY;
		};

		//--------------------------------------------------------------------------------
		// Vml_Polygon2D 14.1.2.1 (wrapcoords)
		//--------------------------------------------------------------------------------

		class CVml_Polygon2D
		{
		public:
			CVml_Polygon2D();
			int GetSize() const;

			int GetX(int nIndex) const;
			int GetY(int nIndex) const;

			void   AddPoint(int nX, int nY);

			virtual double FromString(const std::wstring &sValue);
			virtual std::wstring ToString  () const;

			SimpleTypes_DefaultString(CVml_Polygon2D)

			private:

				struct TPoint
			{
						   int nX;
			int nY;

			TPoint( int n_X, int n_Y )
			{
				nX = n_X;
				nY = n_Y;
			}
		};

		std::vector<TPoint> m_arrPoints;
	};

	//------------------------------------------------------------------------------------------
	// CCssProperty
	//------------------------------------------------------------------------------------------

#define CSS_MAX_NAME_LEN 127

	std::wstring RemoveWhiteSpaces(const std::wstring &sText);

	enum ECssPropertyType
	{
		cssptUnknown						= 0000,

		cssptFlip							= 1000,
		cssptHeight							= 1001,
		cssptLeft							= 1002,
		cssptMarginBottom					= 1003,
		cssptMarginLeft						= 1004,
		cssptMarginRight					= 1005,
		cssptMarginTop						= 1006,
		cssptMsoPositionHorizontal          = 1007,
		cssptMsoPositionHorizontalRelative	= 1008,
		cssptMsoPositionVertical			= 1009,
		cssptMsoPositionVerticalRelative	= 1010,
		cssptMsoWrapDistanceBottom			= 1011,
		cssptMsoWrapDistanceLeft			= 1012,
		cssptMsoWrapDistanceRight			= 1013,
		cssptMsoWrapDistanceTop				= 1014,
		cssptMsoWrapEdited					= 1015,
		cssptMsoWrapStyle					= 1016,
		cssptPosition				= 1017,
		cssptRotation				= 1018,
		cssptTop					= 1019,
		cssptVisibility				= 1020,
		cssptWidth					= 1021,
		cssptZIndex					= 1022,
		csspctMsoWidthPercent		= 1023,
		csspctMsoHeightPercent		= 1024,
		csspctMsoTopPercent			= 1025,
		csspctMsoLeftPercent		= 1026,

		//  Для элемента Textbox 14.1.2.22
		cssptDirection				= 1100,
		cssptLayoutFlow				= 1101,
		cssptMsoDirectionAlt		= 1102,
		cssptMsoFitShapeToText		= 1103,
		cssptMsoFitTextToShape		= 1104,
		cssptMsoLayoutFlowAlt		= 1105,
		cssptMsoNextTextbox			= 1106,
		cssptMsoRotate				= 1107,
		cssptMsoTextScale			= 1108,
		cssptVTextAnchor			= 1109,

		// Для элемента Textpath 14.1.2.23
		cssptFont					= 1200,
		cssptFontFamily				= 1201,
		cssptFontSize				= 1202,
		cssptFontStyle				= 1203,
		cssptFontVariant			= 1204,
		cssptFontWeight				= 1205,
		cssptMsoTextShadow			= 1206,
		cssptTextDecoration			= 1207,
		cssptVRotateLetters			= 1208,
		cssptVSameLetterHeights		= 1209,
		cssptVTextAlign				= 1210,
		cssptVTextKern				= 1211,
		cssptVTextReverse			= 1212,
		cssptVTextSpacingMode		= 1213,
		cssptVTextSpacing			= 1214,
		cssptHTextAlign				= 1215,
	};

	enum ECssFlip
	{
		cssflipX  = 0,
		cssflipY  = 1,
		cssflipXY = 2,
		cssflipYX = 3,
	};
	enum ECssUnitsType
	{
		cssunitstypeAuto		= 0,
		cssunitstypeUnits		= 1,
		cssunitstypePerc		= 2,
		cssunitstypeAbsolute	= 3
	};
	struct TCssUnitsValue
	{
		ECssUnitsType eType;
		double        dValue; // значение в пунктах
	};
	enum ECssMsoPosHor
	{
		cssmsoposhorAbsolute = 0,
		cssmsoposhorLeft     = 1,
		cssmsoposhorCenter   = 2,
		cssmsoposhorRight    = 3,
		cssmsoposhorInside   = 4,
		cssmsoposhorOutside  = 5,
	};
	enum ECssMsoPosHorRel
	{
		cssmsoposhorrelMargin		= 0,
		cssmsoposhorrelPage			= 1,
		cssmsoposhorrelText			= 2,
		cssmsoposhorrelChar			= 3,
		cssmsoposhorrelLeftMargin	= 4,
		cssmsoposhorrelRightMargin	= 5
	};
	enum ECssMsoPosVer
	{
		cssmsoposverAbsolute = 0,
		cssmsoposverTop      = 1,
		cssmsoposverCenter   = 2,
		cssmsoposverBottom   = 3,
		cssmsoposverInside   = 4,
		cssmsoposverOutside  = 5,
	};
	enum ECssMsoPosVerRel
	{
		cssmsoposverrelMargin		= 0,
		cssmsoposverrelPage			= 1,
		cssmsoposverrelText			= 2,
		cssmsoposverrelLine			= 3,
		cssmsoposverrelTopMargin	= 4,
		cssmsoposverrelBottomMargin	= 5
	};
	enum ECssMsoWrapStyle
	{
		cssmsowrapstyleSqaure = 0,
		cssmsowrapstyleNone   = 1,
	};
	enum ECssPosition
	{
		csspositionStatic   = 0,
		csspositionAbsolute = 1,
		csspositionRelative = 2,
	};
	enum ECssVisibility
	{
		cssvisibilityHidden  = 0,
		cssvisibilityInherit = 1,
	};
	enum ECssZIndexType
	{
		csszindextypeAuto  = 0,
		csszindextypeOrder = 1,
	};
	struct TCssZIndexValue
	{
		ECssZIndexType eType;
		int            nOrder;
	};
	enum ECssDirection
	{
		cssdirectionLTR = 0,
		cssdirectionRTL = 1,
	};
	enum ECssLayoutFlow
	{
		csslayoutflowHorizontal				= 0,
		csslayoutflowVertical				= 1,
		csslayoutflowVerticalIdeographic	= 2,
		csslayoutflowHorizontalIdeographic	= 3,
	};
	enum ECssDirectionAlt
	{
		cssdirectionaltContext = 0,
	};
	enum ECssLayoutFlowAlt
	{
		csslayoutflowaltBottomToTop = 0,
	};

	enum ECssMsoRotate
	{
		cssmsorotate0   =   0,
		cssmsorotate90  =  90,
		cssmsorotate180 = 180,
		cssmsorotate270 = -90,
	};
	enum ECssVTextAnchor
	{
		cssvtextanchorTop					 = 0,
		cssvtextanchorMiddle					= 1,
		cssvtextanchorBottom					= 2,
		cssvtextanchorTopCenter            = 3,
		cssvtextanchorMiddleCenter         = 4,
		cssvtextanchorBottomCenter         = 5,
		cssvtextanchorTopBaseline          = 6,
		cssvtextanchorBottomBaseline       = 7,
		cssvtextanchorTopCenterBaseline    = 8,
		cssvtextanchorBottomCenterBaseline = 9,
	};
	enum ECssFontStyle
	{
		cssfontstyleNormal  = 0,
		cssfontstyleItalic  = 1,
		cssfontstyleOblique = 2,
	};
	enum ECssFontVarian
	{
		cssfontvariantNormal    = 0,
		cssfontvariantSmallCaps = 1,
	};
	enum ECssFontWeight
	{
		cssfontweightNormal  = 0,
		cssfontweightLighter = 1,
		cssfontweight100     = 100,
		cssfontweight200     = 200,
		cssfontweight300     = 300,
		cssfontweight400     = 400,
		cssfontweightBold    = 550,
		cssfontweightBolder  = 750,
		cssfontweight500     = 500,
		cssfontweight600     = 600,
		cssfontweight700     = 700,
		cssfontweight800     = 800,
		cssfontweight900     = 900
	};
	enum ECssTextDecoration
	{
		csstextdecorationNone        = 0,
		csstextdecorationUnderline   = 1,
		csstextdecorationOverline    = 2,
		csstextdecorationLineThrough = 3,
		csstextdecorationBlink       = 4,
	};
	enum ECssVTextAlign
	{
		cssvtextalignLeft           = 0,
		cssvtextalignRight          = 1,
		cssvtextalignCenter         = 2,
		cssvtextalignJustify        = 3,
		cssvtextalignLetterJustify  = 4,
		cssvtextalignStretchJustify = 5,
	};
	enum ECssVTextSpacingMode
	{
		cssvtextspacingmodeTightening = 0,
		cssvtextspacingmodeTracking   = 1
	};


	union UCssValue
	{
		ECssFlip			eFlip;
		TCssUnitsValue		oValue;
		ECssMsoPosHor		eMsoPosHor;
		ECssMsoPosHorRel	eMsoPosHorRel;
		ECssMsoPosVer		eMsoPosVer;
		ECssMsoPosVerRel	eMsoPosVerRel;
		bool				bValue;
		ECssMsoWrapStyle	eMsoWrapStyle;
		ECssPosition		ePosition;
		ECssVisibility		eVisibility;
		TCssZIndexValue		oZIndex;
		ECssDirection		eDirection;
		ECssLayoutFlow		eLayoutFlow;
		ECssDirectionAlt	eDirectionAlt;
		ECssLayoutFlowAlt	eLayoutFlowAlt;
		wchar_t				wsValue[CSS_MAX_NAME_LEN + 1];
		ECssMsoRotate        eRotate;
		ECssVTextAnchor      eVTextAnchor;
		ECssFontStyle        eFontStyle;
		ECssFontVarian       eFontVariant;
		ECssFontWeight       eFontWeight;
		ECssTextDecoration   eTextDecoration;
		ECssVTextAlign       eVTextAlign;
		ECssVTextSpacingMode eVTextSpacingMode;
		ECssVTextAlign       eHTextAlign;
	};

	class CCssProperty
	{
	public:

		CCssProperty();

		CCssProperty(std::wstring sBuffer);
		~CCssProperty();


		const UCssValue &get_Value() const;

		const ECssPropertyType &get_Type() const;

	private:

		void Parse(const std::wstring &sBuffer);

	private:

		void ReadValue_Unknown(std::wstring& sValue);
		void ReadValue_Flip(std::wstring& sValue);
		void ReadValue_Units(std::wstring& sValue);

		void ReadValue_MsoPosHor(std::wstring& sValue);
		void ReadValue_MsoPosHorRel(std::wstring& sValue);
		void ReadValue_MsoPosVer(std::wstring& sValue);
		void ReadValue_MsoPosVerRel(std::wstring& sValue);

		void ReadValue_Rotation(std::wstring& sValue);
		void ReadValue_Boolean(std::wstring& sValue);
		void ReadValue_MsoWrapStyle(std::wstring& sValue);
		void ReadValue_Position(std::wstring& sValue);
		void ReadValue_Visibility(std::wstring& sValue);
		void ReadValue_ZIndex(std::wstring& sValue);
		void ReadValue_Direction(std::wstring& sValue);
		void ReadValue_LayoutFlow(std::wstring& sValue);
		void ReadValue_DirectionAlt(std::wstring& sValue);
		void ReadValue_LayoutFlowAlt(std::wstring& sValue);
		void ReadValue_String(std::wstring& sValue);

		void ReadValue_MsoRotate(std::wstring& sValue);
		void ReadValue_VTextAnchor(std::wstring& sValue);
		void ReadValue_FontStyle(std::wstring& sValue);
		void ReadValue_FontVariant(std::wstring& sValue);
		void ReadValue_FontWeight(std::wstring& sValue);
		void ReadValue_TextDecoration(std::wstring& sValue);
		void ReadValue_VTextAlign(std::wstring& sValue);
		void ReadValue_VTextSpacingMode(std::wstring& sValue);

	private:
		ECssPropertyType  m_eType;
		UCssValue         m_oValue;

	};

	//--------------------------------------------------------------------------------
	// CCssStyle 14.1.2.1 (style) (Part 1)
	//--------------------------------------------------------------------------------

	typedef boost::shared_ptr<CCssProperty>	CCssPropertyPtr;

	class CCssStyle
	{
	public:
		CCssStyle();
		~CCssStyle();
		void Clear();

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		void mergeFrom(CCssStyle* parent);

		SimpleTypes_DefaultString(CCssStyle)

		private:

			bool ParseProperties();

	public:
		std::map<ECssPropertyType, size_t>	m_mapProperties;
		std::vector<CCssPropertyPtr>		m_arrProperties;
		std::wstring 						m_sCss;
	};

	//--------------------------------------------------------------------------------
	// Vml_Vector2D_Units 14.1.2.3 (from, control1, control2, to)
	//--------------------------------------------------------------------------------

	class CVml_Vector2D_Units
	{
	public:
		CVml_Vector2D_Units();

		double GetX() const;
		double GetY() const;

		void   SetValue(double dX, double dY);

		virtual double FromString(const std::wstring &sValue);

		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultString(CVml_Vector2D_Units)

		private:

			double m_dX; // В пунктах
		double m_dY; // В пунктах
	};

	//--------------------------------------------------------------------------------
	// Vml_Vector2D_Percentage
	//--------------------------------------------------------------------------------

	class CVml_Vector2D_Percentage
	{
	public:
		CVml_Vector2D_Percentage();

		double GetX() const;
		double GetY() const;

		void   SetValue(double dX, double dY);

		virtual double FromString(const std::wstring &sValue);

		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultString(CVml_Vector2D_Percentage)

		private:
			double m_dX; //
		double m_dY; //
	};

	//--------------------------------------------------------------------------------
	// Vml_Vector2D_1_65536
	//--------------------------------------------------------------------------------

	class CVml_Vector2D_1_65536
	{
	public:
		CVml_Vector2D_1_65536();

		double GetX() const;
		double GetY() const;

		void   SetValue(double dX, double dY);

		virtual double FromString(const std::wstring &sValue);

		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultString(CVml_Vector2D_1_65536)

		private:
			double m_dX; //
		double m_dY; //
	};

	//--------------------------------------------------------------------------------
	// CVml_TableLimits
	//--------------------------------------------------------------------------------

	class CVml_TableLimits
	{
	public:
		CVml_TableLimits();

		unsigned int GetSize() const;
		double GetAt(int nIndex) const;

		void AddValue(double dValue);

		int FromString(const std::wstring &sValue);

		std::wstring ToString  () const;

		SimpleTypes_DefaultString(CVml_TableLimits)

		private:
			std::vector<double> m_arrLimits;
	};

	//--------------------------------------------------------------------------------
	// CVml_TableProperties
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CVml_TableProperties, int, 0)
	public:
		bool IsTable() const;
		bool IsPlaceholder() const;
		bool IsBiDirectionalText() const;
	};

	//--------------------------------------------------------------------------------
	// Vml_Vector2D_Position 14.1.2.8 (position)
	//--------------------------------------------------------------------------------

	enum EVml_Vector2D_Position
	{
		vmlvector2dposConstant    = 0,
		vmlvector2dposFormula     = 1,
		vmlvector2dposAdjValue    = 2,
		vmlvector2dposCenter      = 3,
		vmlvector2dposTopLeft     = 4,
		vmlvector2dposBottomRight = 5,
	};

	class CVml_Vector2D_Position
	{
	public:
		CVml_Vector2D_Position();

		EVml_Vector2D_Position GetTypeX() const;
		EVml_Vector2D_Position GetTypeY() const;
		std::wstring GetIdX() const;
		std::wstring GetIdY() const;
		double					GetX() const;
		double					GetY() const;
		void   SetConstantX(double dX);
		void   SetConstantY(double dY);

		double FromString(const std::wstring &sValue);

		std::wstring ToString  () const;

		SimpleTypes_DefaultString(CVml_Vector2D_Position)

		private:
			void Parse(std::wstring &sValue, bool bFirst);

	private:

		EVml_Vector2D_Position m_eTypeX;
		EVml_Vector2D_Position m_eTypeY;

		std::wstring m_sIdX;  // Значение для типа Formula иди AdjValue
		std::wstring m_sIdY;  //

		double					m_dX;    // Значение для типа Constant
		double					m_dY;    //
	};

	//--------------------------------------------------------------------------------
	// Vml_Polygon2D_Units 14.1.2.15 (points)
	//--------------------------------------------------------------------------------

	class CVml_Polygon2D_Units
	{
	public:
		CVml_Polygon2D_Units();
		CVml_Polygon2D_Units(const CVml_Polygon2D_Units &oOther);
		CVml_Polygon2D_Units(const std::wstring& wsStr);
		CVml_Polygon2D_Units &operator =(const std::wstring& wsStr);

		void SetDelimiter(wchar_t wcNew);
		double GetX(int nIndex) const;
		double GetY(int nIndex) const;

		void   AddPoint(double dX, double dY);

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

	private:

		struct TPoint
		{
			double dX;
			double dY;

			TPoint(double d_X, double d_Y )
			{
				dX = d_X;
				dY = d_Y;
			}
		};

		std::vector<TPoint> m_arrPoints;
		wchar_t	m_wcDelimiter; // Разделитель, по умолчнию пробел ' '
	};

	//--------------------------------------------------------------------------------
	// Vml_1_65536_Or_Percentage 14.2.2.11 (brightness)
	//--------------------------------------------------------------------------------

	class CVml_1_65536_Or_Percentage
	{
	public:
		CVml_1_65536_Or_Percentage();

		double GetValue() const;

		void   SetValue(double dValue);
		void   SetValue(int nValue);
		void   SetPercentage(double dValue);

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultD(CVml_1_65536_Or_Percentage)

		private:
			double m_dValue;
	};

	//--------------------------------------------------------------------------------
	// Vml_Matrix 14.1.2.18 (matrix)
	//--------------------------------------------------------------------------------

	class CVml_Matrix
	{
	public:
		CVml_Matrix();
		CVml_Matrix(const CVml_Matrix &oOther);
		CVml_Matrix(const std::wstring& wsStr);
		CVml_Matrix &operator =(const std::wstring& wsStr);

		void ResetMatrix();
		void SetMatrix(double dSxx, double dSxy, double dSyx, double dSyy, double dPx, double dPy);

		double Get_Sxx() const;
		double Get_Sxy() const;
		double Get_Syx() const;
		double Get_Syy() const;

		double Get_Px() const;
		double Get_Py() const;

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

	private:
		double m_dSxx;
		double m_dSxy;
		double m_dSyx;
		double m_dSyy;
		double m_dPx;
		double m_dPy;
	};

	//--------------------------------------------------------------------------------
	// Vml_Vector2D_Units_Or_Percentage
	//--------------------------------------------------------------------------------

	class CVml_Vector2D_Units_Or_Percentage
	{
	public:
		CVml_Vector2D_Units_Or_Percentage();

		double GetX() const;
		double GetY() const;
		bool IsXinPoints() const;
		bool IsYinPoints() const;
		void SetValue_Points(double dX, double dY);

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultString(CVml_Vector2D_Units_Or_Percentage)

		private:
			double m_dX; //
		double m_dY; //

		bool   m_bUnitsX;
		bool   m_bUnitsY;
	};

	//--------------------------------------------------------------------------------
	// Vml_TextBoxInset 14.1.2.22 (inset)
	//--------------------------------------------------------------------------------

	class CVml_TextBoxInset
	{
	public:
		CVml_TextBoxInset();
		double GetLeft(int nIndex) const;
		double GetTop(int nIndex) const;
		double GetRight(int nIndex) const;
		double GetBottom(int nIndex) const;

		void Set(double dL, double dT, double dR, double dB);

		double FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultString(CVml_TextBoxInset)

		private:
			double m_dLeft;
		double m_dTop;
		double m_dRight;
		double m_dBottom;
	};

} // Vml
} // SimpleTypes

