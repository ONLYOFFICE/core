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

#include "SimpleTypes_Base.h"

// Здесь представлены все простые типы Drawing-Main из спецификации Office Open Xml (20.1.10)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// AdjAngle 20.1.10.1 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAdjAngle
	{
		adjangleAngle = 0,
		adjangleGuide = 1
	};	

	DEFINE_SIMPLE_TYPE_START(CAdjAngle, EAdjAngle, adjangleAngle)
	public:
		double  GetAngle() const;
		std::wstring GetGuide() const;

	private:
		void Parse(const std::wstring &sValue);

		int				m_nAngle = adjangleAngle;
		std::wstring	m_sGuide;
	};

	//--------------------------------------------------------------------------------
	// AdjCoordinate 20.1.10.2 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAdjCoordinate
	{
		adjcoordinateCoord = 0,
		adjcoordinateGuide = 1
	};

	template<EAdjCoordinate eDefValue = adjcoordinateCoord>
	class CAdjCoordinate : public CUniversalMeasure
	{
	public:
		CAdjCoordinate();

		virtual void SetValue(double dValue);
		virtual double  FromString(const std::wstring &sValue);
		virtual std::wstring ToString() const;

		SimpleTypes_Default(CAdjCoordinate)

		std::wstring GetGuide() const;
		double GetValue() const;

	private:
		void Parse2(const std::wstring &sValue);

		EAdjCoordinate      m_eValue;
		std::wstring        m_sGuide;
	};

	//--------------------------------------------------------------------------------
	// Angle 20.1.10.3 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CAngle, int, 0)
	public:
		double  GetAngle() const;
	};

	//--------------------------------------------------------------------------------
	// AnimationBuildType 20.1.10.4 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAnimationBuildType
	{
		animationbuildtypeAllAtOnce = 0
	};

	DEFINE_SIMPLE_TYPE(CAnimationBuildType, EAnimationBuildType, animationbuildtypeAllAtOnce)

	//--------------------------------------------------------------------------------
	// AnimationChartBuildType 20.1.10.5 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAnimationChartBuildType
	{
		animationchartbuildtypeAllAtOnce  = 0,
		animationchartbuildtypeCategory   = 1,
		animationchartbuildtypeCategoryEl = 2,
		animationchartbuildtypeSeries     = 3,
		animationchartbuildtypeSeriesEl   = 4
	};

	DEFINE_SIMPLE_TYPE(CAnimationChartBuildType, EAnimationChartBuildType, animationchartbuildtypeAllAtOnce)

	//--------------------------------------------------------------------------------
	// AnimationChartOnlyBuildType 20.1.10.6 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAnimationChartOnlyBuildType
	{
		animationchartonlybuildtypeCategory   = 0,
		animationchartonlybuildtypeCategoryEl = 1,
		animationchartonlybuildtypeSeries     = 2,
		animationchartonlybuildtypeSeriesEl   = 3
	};

	DEFINE_SIMPLE_TYPE(CAnimationChartOnlyBuildType, EAnimationChartOnlyBuildType, animationchartonlybuildtypeCategory)

	//--------------------------------------------------------------------------------
	// AnimationDgmBuildType 20.1.10.7 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAnimationDgmBuildType
	{
		animationdgmbuildtypeAllAtOnce = 0,
		animationdgmbuildtypeLvlAtOnce = 1,
		animationdgmbuildtypeLvlOne    = 2,
		animationdgmbuildtypeOne       = 3
	};

	DEFINE_SIMPLE_TYPE(CAnimationDgmBuildType, EAnimationDgmBuildType, animationdgmbuildtypeAllAtOnce)

	//--------------------------------------------------------------------------------
	// AnimationDgmOnlyBuildType 20.1.10.8 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAnimationDgmOnlyBuildType
	{
		animationdgmonlybuildtypeLvlAtOnce = 0,
		animationdgmonlybuildtypeLvlOne    = 1,
		animationdgmonlybuildtypeOne       = 2
	};

	DEFINE_SIMPLE_TYPE(CAnimationDgmOnlyBuildType, EAnimationDgmOnlyBuildType, animationdgmonlybuildtypeOne)

	//--------------------------------------------------------------------------------
	// BevelPresetType 20.1.10.9 (Part 1)
	//--------------------------------------------------------------------------------

	enum EBevelPresetType
	{
		bevelpresettypeAngle        =  0,
		bevelpresettypeArtDeco      =  1,
		bevelpresettypeCircle       =  2,
		bevelpresettypeConvex       =  3,
		bevelpresettypeCoolSlant    =  4,
		bevelpresettypeCross        =  5,
		bevelpresettypeDivot        =  6,
		bevelpresettypeHardEdge     =  7,
		bevelpresettypeRelaxedInset =  8,
		bevelpresettypeRiblet       =  9,
		bevelpresettypeSlope        = 10,
		bevelpresettypeSoftRound    = 11
	};

	DEFINE_SIMPLE_TYPE(CBevelPresetType, EBevelPresetType, bevelpresettypeAngle)

	//--------------------------------------------------------------------------------
	// BlackWhiteMode 20.1.10.10 (Part 1)
	//--------------------------------------------------------------------------------

	enum EBlackWhiteMode
	{
		blackwhitemodeAuto       =  0,
		blackwhitemodeBlack      =  1,
		blackwhitemodeBlackGray  =  2,
		blackwhitemodeBlackWhite =  3,
		blackwhitemodeClr        =  4,
		blackwhitemodeGray       =  5,
		blackwhitemodeGrayWhite  =  6,
		blackwhitemodeHidden     =  7,
		blackwhitemodeInvGray    =  8,
		blackwhitemodeLtGray     =  9,
		blackwhitemodeWhite      = 10
	};

	DEFINE_SIMPLE_TYPE(CBlackWhiteMode, EBlackWhiteMode, blackwhitemodeAuto)

	//--------------------------------------------------------------------------------
	// BlendMode 20.1.10.11 (Part 1)
	//--------------------------------------------------------------------------------

	enum EBlendMode
	{
		blendmodeDarken  = 0,
		blendmodeLighten = 1,
		blendmodeMult    = 2,
		blendmodeOver    = 3,
		blendmodeScreen  = 4
	};

	DEFINE_SIMPLE_TYPE(CBlendMode, EBlendMode, blendmodeMult)

	//--------------------------------------------------------------------------------
	// BlipCompression 20.1.10.12 (Part 1)
	//--------------------------------------------------------------------------------

	enum EBlipCompression
	{
		blipcompressionEmail   = 0,
		blipcompressionHQPrint = 1,
		blipcompressionNone    = 2,
		blipcompressionPrint   = 3,
		blipcompressionScreen  = 4
	};

	DEFINE_SIMPLE_TYPE(CBlipCompression, EBlipCompression, blipcompressionNone)

	//--------------------------------------------------------------------------------
	// ColorSchemeIndex 20.1.10.14 (Part 1)
	//--------------------------------------------------------------------------------

	enum EColorSchemeIndex
	{
		colorschemeindexAccent1  = 0,
		colorschemeindexAccent2  = 1,
		colorschemeindexAccent3  = 2,
		colorschemeindexAccent4  = 3,
		colorschemeindexAccent5  = 4,
		colorschemeindexAccent6  = 5,
		colorschemeindexDk1      = 6,
		colorschemeindexDk2      = 7,
		colorschemeindexFolHlink = 8,
		colorschemeindexHlink    = 9,
		colorschemeindexLt1      = 10,
		colorschemeindexLt2      = 11,
	};

	DEFINE_SIMPLE_TYPE(CColorSchemeIndex, EColorSchemeIndex, colorschemeindexAccent1)

	//--------------------------------------------------------------------------------
	// CompoundLine 20.1.10.15 (Part 1)
	//--------------------------------------------------------------------------------

	enum ECompoundLine
	{
		compoundlineDbl       = 0,
		compoundlineSng       = 1,
		compoundlineThickThin = 2,
		compoundlineThinThick = 3,
		compoundlineTri       = 4,
	};

	DEFINE_SIMPLE_TYPE(CCompoundLine, ECompoundLine, compoundlineSng)

	//--------------------------------------------------------------------------------
	// Coordinate 20.1.10.16 (Part 1)
	//--------------------------------------------------------------------------------

	class CCoordinate : public CUniversalMeasure
	{
	public:
		CCoordinate();

		double GetValue() const;
		virtual void SetValue(double dValue);
		virtual double  FromString(const std::wstring &sValue);

		virtual std::wstring ToString  () const;

		_INT64 ToEmu() const;
		double ToCm() const;

		SimpleTypes_DefaultD(CCoordinate)
	};

	//--------------------------------------------------------------------------------
	// Coordinate32 20.1.10.17 (Part 1)
	//--------------------------------------------------------------------------------

#define	CCoordinate32 CCoordinate

	//--------------------------------------------------------------------------------
	// DrawingElementId 20.1.10.21 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE(CDrawingElementId, int, 0)

	//--------------------------------------------------------------------------------
	// EffectContainerType 20.1.10.22 (Part 1)
	//--------------------------------------------------------------------------------

	enum EEffectContainerType
	{
		effectcontainertypeSib  = 0,
		effectcontainertypeTree = 1,
	};

	DEFINE_SIMPLE_TYPE(CEffectContainerType, EEffectContainerType, effectcontainertypeSib)

	//--------------------------------------------------------------------------------
	// FixedAngle 20.1.10.23 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START_NSV(CFixedAngle, int, 0)
	public:
		double  GetAngle() const;
	};

	//--------------------------------------------------------------------------------
	// FixedPercentage 20.1.10.24 (Part 1) + 12.1.2.1 (Part4)
	//--------------------------------------------------------------------------------

	class CFixedPercentage
	{
	public:
		CFixedPercentage();

		double GetValue() const;
		void   SetValue(double dValue);

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultD(CFixedPercentage)

		private:

			double m_dValue;
	};

	//--------------------------------------------------------------------------------
	// FontCollectionIndex 20.1.10.25 (Part 1)
	//--------------------------------------------------------------------------------

	enum EFontCollectionIndex
	{
		fontcollectionindexMajor = 0,
		fontcollectionindexMinor = 1,
		fontcollectionindexNone  = 2,
	};

	DEFINE_SIMPLE_TYPE(CFontCollectionIndex, EFontCollectionIndex, fontcollectionindexNone)

	//--------------------------------------------------------------------------------
	// FOVAngle 20.1.10.26 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START_NSV(CFOVAngle, int, 0)
	public:
		double  GetAngle() const;
	};

	//--------------------------------------------------------------------------------
	// GeomGuideFormula 20.1.10.27 (Part 1)
	//--------------------------------------------------------------------------------

	class CGeomGuideFormula
	{
	public:
		CGeomGuideFormula();

		std::wstring GetValue() const;
		void    SetValue(std::wstring &sValue);

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CGeomGuideFormula)

		private:
			void Parse();

	private:
		std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// GeomGuideName 20.1.10.28 (Part 1)
	//--------------------------------------------------------------------------------

	class CGeomGuideName
	{
	public:
		CGeomGuideName();

		std::wstring GetValue() const;
		void    SetValue(std::wstring &sValue);

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CGeomGuideName)

		private:

			std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// LightRigDirection 20.1.10.29 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELightRigDirection
	{
		lightrigdirectionB  = 0,
		lightrigdirectionBL = 1,
		lightrigdirectionBR = 2,
		lightrigdirectionL  = 3,
		lightrigdirectionR  = 4,
		lightrigdirectionT  = 5,
		lightrigdirectionTL = 6,
		lightrigdirectionTR = 7,
	};

	DEFINE_SIMPLE_TYPE(CLightRigDirection, ELightRigDirection, lightrigdirectionTR)

	//--------------------------------------------------------------------------------
	// LightRigType 20.1.10.30 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELightRigType
	{
		lightrigtypeBalanced      = 0,
		lightrigtypeBrightRoom    = 1,
		lightrigtypeChilly        = 2,
		lightrigtypeContrasting   = 3,
		lightrigtypeFlat          = 4,
		lightrigtypeFlood         = 5,
		lightrigtypeFreezing      = 6,
		lightrigtypeGlow          = 7,
		lightrigtypeHarsh         = 8,
		lightrigtypeLegacyFlat1   = 9,
		lightrigtypeLegacyFlat2   = 10,
		lightrigtypeLegacyFlat3   = 11,
		lightrigtypeLegacyFlat4   = 12,
		lightrigtypeLegacyHarsh1  = 13,
		lightrigtypeLegacyHarsh2  = 14,
		lightrigtypeLegacyHarsh3  = 15,
		lightrigtypeLegacyHarsh4  = 16,
		lightrigtypeLegacyNormal1 = 17,
		lightrigtypeLegacyNormal2 = 18,
		lightrigtypeLegacyNormal3 = 19,
		lightrigtypeLegacyNormal4 = 20,
		lightrigtypeMorning       = 21,
		lightrigtypeSoft          = 22,
		lightrigtypeSunrise       = 23,
		lightrigtypeSunset        = 24,
		lightrigtypeThreePt       = 25,
		lightrigtypeTwoPt         = 26,
	};

	DEFINE_SIMPLE_TYPE(CLightRigType, ELightRigType, lightrigtypeBalanced)

	//--------------------------------------------------------------------------------
	// LineCap 20.1.10.31 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELineCap
	{
		linecapFlat = 0,
		linecapRnd  = 1,
		linecapSq   = 2,
	};

	DEFINE_SIMPLE_TYPE(CLineCap, ELineCap, linecapRnd)

	//--------------------------------------------------------------------------------
	// LineEndLength 20.1.10.32 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELineEndLength
	{
		lineendlengthLarge  = 0,
		lineendlengthMedium = 1,
		lineendlengthSmall  = 2,
	};

	DEFINE_SIMPLE_TYPE(CLineEndLength, ELineEndLength, lineendlengthMedium)

	//--------------------------------------------------------------------------------
	// LineEndType 20.1.10.33 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELineEndType
	{
		lineendtypeArrow    = 0,
		lineendtypeDiamond  = 1,
		lineendtypeNone     = 2,
		lineendtypeOval     = 3,
		lineendtypeStealth  = 4,
		lineendtypeTriangle = 5,
	};

	DEFINE_SIMPLE_TYPE(CLineEndType, ELineEndType, lineendtypeNone)

	//--------------------------------------------------------------------------------
	// LineEndWidth 20.1.10.34 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELineEndWidth
	{
		lineendwidthLarge  = 0,
		lineendwidthMedium = 1,
		lineendwidthSmall  = 2,
	};

	DEFINE_SIMPLE_TYPE(CLineEndWidth, ELineEndWidth, lineendwidthMedium)

	//--------------------------------------------------------------------------------
	// LineWidth 20.1.10.35 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CLineWidth, _INT64, 0)
	public:
		double ToPoints();
		double ToInches();

		double FromEmu(const _INT64 nEmu);
	};

	//--------------------------------------------------------------------------------
	// PathFillMode 20.1.10.37 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPathFillMode
	{
		pathfillmodeDarken      = 0,
		pathfillmodeDarkenLess  = 1,
		pathfillmodeLighten     = 2,
		pathfillmodeLightenLess = 3,
		pathfillmodeNone        = 4,
		pathfillmodeNorm        = 5,
	};

	DEFINE_SIMPLE_TYPE(CPathFillMode, EPathFillMode, pathfillmodeNone)

	//--------------------------------------------------------------------------------
	// PathShadeType 20.1.10.38 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPathShadeType
	{
		pathshadetypeCircle = 0,
		pathshadetypeRect   = 1,
		pathshadetypeShape  = 2,
	};

	DEFINE_SIMPLE_TYPE(CPathShadeType, EPathShadeType, pathshadetypeRect)

	//--------------------------------------------------------------------------------
	// PenAlignment 20.1.10.39 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPenAlignment
	{
		penalignmentCtr = 0,
		penalignmentIn  = 1,
	};

	DEFINE_SIMPLE_TYPE(CPenAlignment, EPenAlignment, penalignmentCtr)

	//--------------------------------------------------------------------------------
	// Percentage 20.1.10.40 (Part 1) + 12.1.2.2 (Part4)
	//--------------------------------------------------------------------------------

	class CPercentage
	{
	public:
		CPercentage();

		double GetValue() const;
		void   SetValue(double dValue);

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;
		virtual std::wstring ToStringDecimalNumber  () const;

		SimpleTypes_DefaultD(CPercentage)

		private:

			double m_dValue;
	};

	//--------------------------------------------------------------------------------
	// PositiveCoordinate 20.1.10.42 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CPositiveCoordinate, _INT64, 0)
	public:
		double ToPoints() const;
		double ToMM() const;

		double ToInches();
		double ToTwips();
		double FromEmu(const _INT64& nEmu);
	};

	//--------------------------------------------------------------------------------
	// PositiveCoordinate32 20.1.10.43 (Part 1)
	//--------------------------------------------------------------------------------

#define CPositiveCoordinate32 CPositiveCoordinate

	//--------------------------------------------------------------------------------
	// PositiveFixedAngle 20.1.10.44 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START_NSV(CPositiveFixedAngle, int, 0)
	public:
		double  GetAngle() const;
	};

	//--------------------------------------------------------------------------------
	// PositiveFixedPercentage 20.1.10.45 (Part 1) + 12.1.2.3 (Part4)
	//--------------------------------------------------------------------------------

	class CPositiveFixedPercentage
	{
	public:
		CPositiveFixedPercentage();

		double GetValue() const;
		void   SetValue(double dValue);

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultD(CPositiveFixedPercentage)

		private:
			double m_dValue;
	};

	//--------------------------------------------------------------------------------
	// PositivePercentage 20.1.10.46 (Part 1) + 12.1.2.4 (Part4)
	//--------------------------------------------------------------------------------

	class CPositivePercentage
	{
	public:
		CPositivePercentage();

		double GetValue() const;
		void   SetValue(double dValue);

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultD(CPositivePercentage)

		private:
			double m_dValue;
	};

	//--------------------------------------------------------------------------------
	// PresetCameraType 20.1.10.47 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPresetCameraType
	{
		presetcameratypeIsometricBottomDown, // (Isometric Bottom Down)
		presetcameratypeIsometricBottomUp, // (Isometric Bottom Up)
		presetcameratypeIsometricLeftDown, // (Isometric Left Down)
		presetcameratypeIsometricLeftUp, // (Isometric Left Up)
		presetcameratypeIsometricOffAxis1Left, // (Isometric Off Axis 1 Left)
		presetcameratypeIsometricOffAxis1Right, // (Isometric Off Axis 1 Right)
		presetcameratypeIsometricOffAxis1Top, // (Isometric Off Axis 1 Top)
		presetcameratypeIsometricOffAxis2Left, // (Isometric Off Axis 2 Left)
		presetcameratypeIsometricOffAxis2Right, // (Isometric Off Axis 2 Right
		presetcameratypeIsometricOffAxis2Top, // (Isometric Off Axis 2 Top)
		presetcameratypeIsometricOffAxis3Bottom, // (Isometric Off Axis 3 Bottom)
		presetcameratypeIsometricOffAxis3Left, // (Isometric Off Axis 3 Left)
		presetcameratypeIsometricOffAxis3Right, // (Isometric Off Axis 3 Right)
		presetcameratypeIsometricOffAxis4Bottom, // (Isometric Off Axis 4 Bottom)
		presetcameratypeIsometricOffAxis4Left, // (Isometric Off Axis 4 Left)
		presetcameratypeIsometricOffAxis4Right, // (Isometric Off Axis 4 Right)
		presetcameratypeIsometricRightDown, // (Isometric Right Down)
		presetcameratypeIsometricRightUp, // (Isometric Right Up)
		presetcameratypeIsometricTopDown, // (Isometric Top Down)
		presetcameratypeIsometricTopUp, // (Isometric Top Up)
		presetcameratypeLegacyObliqueBottom, // (Legacy Oblique Bottom)
		presetcameratypeLegacyObliqueBottomLeft, // (Legacy Oblique Bottom Left)
		presetcameratypeLegacyObliqueBottomRight, // (Legacy Oblique Bottom Right)
		presetcameratypeLegacyObliqueFront, // (Legacy Oblique Front)
		presetcameratypeLegacyObliqueLeft, // (Legacy Oblique Left)
		presetcameratypeLegacyObliqueRight, // (Legacy Oblique Right)
		presetcameratypeLegacyObliqueTop, // (Legacy Oblique Top)
		presetcameratypeLegacyObliqueTopLeft, // (Legacy Oblique Top Left)
		presetcameratypeLegacyObliqueTopRight, // (Legacy Oblique Top Right)
		presetcameratypeLegacyPerspectiveBottom, // (Legacy Perspective Bottom)
		presetcameratypeLegacyPerspectiveBottomLeft, // (Legacy Perspective Bottom Left)
		presetcameratypeLegacyPerspectiveBottomRight, // (Legacy Perspective Bottom Right)
		presetcameratypeLegacyPerspectiveFront, // (Legacy Perspective Front)
		presetcameratypeLegacyPerspectiveLeft, // (Legacy Perspective Left)
		presetcameratypeLegacyPerspectiveRight, // (Legacy Perspective Right)
		presetcameratypeLegacyPerspectiveTop, // (Legacy Perspective Top)
		presetcameratypeLegacyPerspectiveTopLeft, // (Legacy Perspective Top Left)
		presetcameratypeLegacyPerspectiveTopRight, // (Legacy Perspective Top Right)
		presetcameratypeObliqueBottom, // (Oblique Bottom)
		presetcameratypeObliqueBottomLeft, // (Oblique Bottom Left)
		presetcameratypeObliqueBottomRight, // (Oblique Bottom Right)
		presetcameratypeObliqueLeft, // (Oblique Left)
		presetcameratypeObliqueRight, // (Oblique Right)
		presetcameratypeObliqueTop, // (Oblique Top)
		presetcameratypeObliqueTopLeft, // (Oblique Top Left)
		presetcameratypeObliqueTopRight, // (Oblique Top Right)
		presetcameratypeOrthographicFront, // (Orthographic Front)
		presetcameratypePerspectiveAbove, // (Orthographic Above)
		presetcameratypePerspectiveAboveLeftFacing, // (Perspective Above Left Facing)
		presetcameratypePerspectiveAboveRightFacing, // (Perspective Above Right Facing)
		presetcameratypePerspectiveBelow, // (Perspective Below)
		presetcameratypePerspectiveContrastingLeftFacing, // (Perspective Contrasting Left Facing)
		presetcameratypePerspectiveContrastingRightFacing, // (Perspective Contrasting Right Facing)
		presetcameratypePerspectiveFront, // (Perspective Front)
		presetcameratypePerspectiveHeroicExtremeLeftFacing, // (Perspective Heroic Extreme Left Facing)
		presetcameratypePerspectiveHeroicExtremeRightFacing, // (Perspective Heroic Extreme Right Facing)
		presetcameratypePerspectiveHeroicLeftFacing, // (Perspective Heroic Left Facing)
		presetcameratypePerspectiveHeroicRightFacing, // (Perspective Heroic Right Facing)
		presetcameratypePerspectiveLeft, // (Perspective Left)
		presetcameratypePerspectiveRelaxed, // (Perspective Relaxed)
		presetcameratypePerspectiveRelaxedModerately, // (Perspective Relaxed Moderately)
		presetcameratypePerspectiveRight, // (Perspective Right)
	};

	DEFINE_SIMPLE_TYPE(CPresetCameraType, EPresetCameraType, presetcameratypePerspectiveFront)

	//--------------------------------------------------------------------------------
	// PresetColorVal 20.1.10.48 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPresetColorVal
	{
		presetcolorvalAliceBlue = 0, // (Alice Blue Preset Color)
		presetcolorvalAntiqueWhite, // (Antique White Preset Color)
		presetcolorvalAqua, // (Aqua Preset Color)
		presetcolorvalAquamarine, // (Aquamarine Preset Color)
		presetcolorvalAzure, // (Azure Preset Color)
		presetcolorvalBeige, // (Beige Preset Color)
		presetcolorvalBisque, // (Bisque Preset Color)
		presetcolorvalBlack, // (Black Preset Color)
		presetcolorvalBlanchedAlmond, // (Blanched Almond Preset Color)
		presetcolorvalBlue, // (Blue Preset Color)
		presetcolorvalBlueViolet, // (Blue Violet Preset Color)
		presetcolorvalBrown, // (Brown Preset Color)
		presetcolorvalBurlyWood, // (Burly Wood Preset Color)
		presetcolorvalCadetBlue, // (Cadet Blue Preset Color)
		presetcolorvalChartreuse, // (Chartreuse Preset Color)
		presetcolorvalChocolate, // (Chocolate Preset Color)
		presetcolorvalCoral, // (Coral Preset Color)
		presetcolorvalCornflowerBlue, // (Cornflower Blue Preset Color)
		presetcolorvalCornsilk, // (Cornsilk Preset Color)
		presetcolorvalCrimson, // (Crimson Preset Color)
		presetcolorvalCyan, // (Cyan Preset Color)
		presetcolorvalDarkBlue, // (Dark Blue Preset Color)
		presetcolorvalDarkCyan, // (Dark Cyan Preset Color)
		presetcolorvalDarkGoldenrod, // (Dark Goldenrod Preset Color)
		presetcolorvalDarkGray, // (Dark Gray Preset Color)
		presetcolorvalDarkGreen, // (Dark Green Preset Color)
		presetcolorvalDarkGrey, // (Dark Gray Preset Color)
		presetcolorvalDarkKhaki, // (Dark Khaki Preset Color)
		presetcolorvalDarkMagenta, // (Dark Magenta Preset Color)
		presetcolorvalDarkOliveGreen, // (Dark Olive Green Preset Color)
		presetcolorvalDarkOrange, // (Dark Orange Preset Color)
		presetcolorvalDarkOrchid, // (Dark Orchid Preset Color)
		presetcolorvalDarkRed, // (Dark Red Preset Color)
		presetcolorvalDarkSalmon, // (Dark Salmon Preset Color)
		presetcolorvalDarkSeaGreen, // (Dark Sea Green Preset Color)
		presetcolorvalDarkSlateBlue, // (Dark Slate Blue Preset Color)
		presetcolorvalDarkSlateGray, // (Dark Slate Gray Preset Color)
		presetcolorvalDarkSlateGrey, // (Dark Slate Gray Preset Color)
		presetcolorvalDarkTurquoise, // (Dark Turquoise Preset Color)
		presetcolorvalDarkViolet, // (Dark Violet Preset Color)
		presetcolorvalDeepPink, // (Deep Pink Preset Color)
		presetcolorvalDeepSkyBlue, // (Deep Sky Blue Preset Color)
		presetcolorvalDimGray, // (Dim Gray Preset Color)
		presetcolorvalDimGrey, // (Dim Gray Preset Color)
		presetcolorvalDkBlue, // (Dark Blue Preset Color)
		presetcolorvalDkCyan, // (Dark Cyan Preset Color)
		presetcolorvalDkGoldenrod, // (Dark Goldenrod Preset Color)
		presetcolorvalDkGray, // (Dark Gray Preset Color)
		presetcolorvalDkGreen, // (Dark Green Preset Color)
		presetcolorvalDkGrey, // (Dark Gray Preset Color)
		presetcolorvalDkKhaki, // (Dark Khaki Preset Color)
		presetcolorvalDkMagenta, // (Dark Magenta Preset Color)
		presetcolorvalDkOliveGreen, // (Dark Olive Green Preset Color)
		presetcolorvalDkOrange, // (Dark Orange Preset Color)
		presetcolorvalDkOrchid, // (Dark Orchid Preset Color)
		presetcolorvalDkRed, // (Dark Red Preset Color)
		presetcolorvalDkSalmon, // (Dark Salmon Preset Color)
		presetcolorvalDkSeaGreen, // (Dark Sea Green Preset Color)
		presetcolorvalDkSlateBlue, // (Dark Slate Blue Preset Color)
		presetcolorvalDkSlateGray, // (Dark Slate Gray Preset Color)
		presetcolorvalDkSlateGrey, // (Dark Slate Gray Preset Color)
		presetcolorvalDkTurquoise, // (Dark Turquoise Preset Color)
		presetcolorvalDkViolet, // (Dark Violet Preset Color)
		presetcolorvalDodgerBlue, // (Dodger Blue Preset Color)
		presetcolorvalFirebrick, // (Firebrick Preset Color)
		presetcolorvalFloralWhite, // (Floral White Preset Color)
		presetcolorvalForestGreen, // (Forest Green Preset Color)
		presetcolorvalFuchsia, // (Fuchsia Preset Color)
		presetcolorvalGainsboro, // (Gainsboro Preset Color)
		presetcolorvalGhostWhite, // (Ghost White Preset Color)
		presetcolorvalGold, // (Gold Preset Color)
		presetcolorvalGoldenrod, // (Goldenrod Preset Color)
		presetcolorvalGray, // (Gray Preset Color)
		presetcolorvalGreen, // (Green Preset Color)
		presetcolorvalGreenYellow, // (Green Yellow Preset Color)
		presetcolorvalGrey, // (Gray Preset Color)
		presetcolorvalHoneydew, // (Honeydew Preset Color)
		presetcolorvalHotPink, // (Hot Pink Preset Color)
		presetcolorvalIndianRed, // (Indian Red Preset Color)
		presetcolorvalIndigo, // (Indigo Preset Color)
		presetcolorvalIvory, // (Ivory Preset Color)
		presetcolorvalKhaki, // (Khaki Preset Color)
		presetcolorvalLavender, // (Lavender Preset Color)
		presetcolorvalLavenderBlush, // (Lavender Blush Preset Color)
		presetcolorvalLawnGreen, // (Lawn Green Preset Color)
		presetcolorvalLemonChiffon, // (Lemon Chiffon Preset Color)
		presetcolorvalLightBlue, // (Light Blue Preset Color)
		presetcolorvalLightCoral, // (Light Coral Preset Color)
		presetcolorvalLightCyan, // (Light Cyan Preset Color)
		presetcolorvalLightGoldenrodYellow, // (Light Goldenrod Yellow PreseColor)
		presetcolorvalLightGray, // (Light Gray Preset Color)
		presetcolorvalLightGreen, // (Light Green Preset Color)
		presetcolorvalLightGrey, // (Light Gray Preset Color)
		presetcolorvalLightPink, // (Light Pink Preset Color)
		presetcolorvalLightSalmon, // (Light Salmon Preset Color)
		presetcolorvalLightSeaGreen, // (Light Sea Green Preset Color)
		presetcolorvalLightSkyBlue, // (Light Sky Blue Preset Color)
		presetcolorvalLightSlateGray, // (Light Slate Gray Preset Color)
		presetcolorvalLightSlateGrey, // (Light Slate Gray Preset Color)
		presetcolorvalLightSteelBlue, // (Light Steel Blue Preset Color)
		presetcolorvalLightYellow, // (Light Yellow Preset Color)
		presetcolorvalLime, // (Lime Preset Color)
		presetcolorvalLimeGreen, // (Lime Green Preset Color)
		presetcolorvalLinen, // (Linen Preset Color)
		presetcolorvalLtBlue, // (Light Blue Preset Color)
		presetcolorvalLtCoral, // (Light Coral Preset Color)
		presetcolorvalLtCyan, // (Light Cyan Preset Color)
		presetcolorvalLtGoldenrodYellow, // (Light Goldenrod Yellow Preset Color)
		presetcolorvalLtGray, // (Light Gray Preset Color)
		presetcolorvalLtGreen, // (Light Green Preset Color)
		presetcolorvalLtGrey, // (Light Gray Preset Color)
		presetcolorvalLtPink, // (Light Pink Preset Color)
		presetcolorvalLtSalmon, // (Light Salmon Preset Color)
		presetcolorvalLtSeaGreen, // (Light Sea Green Preset Color)
		presetcolorvalLtSkyBlue, // (Light Sky Blue Preset Color)
		presetcolorvalLtSlateGray, // (Light Slate Gray Preset Color)
		presetcolorvalLtSlateGrey, // (Light Slate Gray Preset Color)
		presetcolorvalLtSteelBlue, // (Light Steel Blue Preset Color)
		presetcolorvalLtYellow, // (Light Yellow Preset Color)
		presetcolorvalMagenta, // (Magenta Preset Color)
		presetcolorvalMaroon, // (Maroon Preset Color)
		presetcolorvalMedAquamarine, // (Medium Aquamarine Preset Color)
		presetcolorvalMedBlue, // (Medium Blue Preset Color)
		presetcolorvalMediumAquamarine, // (Medium Aquamarine Preset Color)
		presetcolorvalMediumBlue, // (Medium Blue Preset Color)
		presetcolorvalMediumOrchid, // (Medium Orchid Preset Color)
		presetcolorvalMediumPurple, // (Medium Purple Preset Color)
		presetcolorvalMediumSeaGreen, // (Medium Sea Green Preset Color)
		presetcolorvalMediumSlateBlue, // (Medium Slate Blue Preset Color)
		presetcolorvalMediumSpringGreen, // (Medium Spring Green Preset Color)
		presetcolorvalMediumTurquoise, // (Medium Turquoise Preset Color)
		presetcolorvalMediumVioletRed, // (Medium Violet Red Preset Color)
		presetcolorvalMedOrchid, // (Medium Orchid Preset Color)
		presetcolorvalMedPurple, // (Medium Purple Preset Color)
		presetcolorvalMedSeaGreen, // (Medium Sea Green Preset Color)
		presetcolorvalMedSlateBlue, // (Medium Slate Blue Preset Color)
		presetcolorvalMedSpringGreen, // (Medium Spring Green Preset Color)
		presetcolorvalMedTurquoise, // (Medium Turquoise Preset Color)
		presetcolorvalMedVioletRed, // (Medium Violet Red Preset Color)
		presetcolorvalMidnightBlue, // (Midnight Blue Preset Color)
		presetcolorvalMintCream, // (Mint Cream Preset Color)
		presetcolorvalMistyRose, // (Misty Rose Preset Color)
		presetcolorvalMoccasin, // (Moccasin Preset Color)
		presetcolorvalNavajoWhite, // (Navajo White Preset Color)
		presetcolorvalNavy, // (Navy Preset Color)
		presetcolorvalOldLace, // (Old Lace Preset Color)
		presetcolorvalOlive, // (Olive Preset Color)
		presetcolorvalOliveDrab, // (Olive Drab Preset Color)
		presetcolorvalOrange, // (Orange Preset Color)
		presetcolorvalOrangeRed, // (Orange Red Preset Color)
		presetcolorvalOrchid, // (Orchid Preset Color)
		presetcolorvalPaleGoldenrod, // (Pale Goldenrod Preset Color)
		presetcolorvalPaleGreen, // (Pale Green Preset Color)
		presetcolorvalPaleTurquoise, // (Pale Turquoise Preset Color)
		presetcolorvalPaleVioletRed, // (Pale Violet Red Preset Color)
		presetcolorvalPapayaWhip, // (Papaya Whip Preset Color)
		presetcolorvalPeachPuff, // (Peach Puff Preset Color)
		presetcolorvalPeru, // (Peru Preset Color)
		presetcolorvalPink, // (Pink Preset Color)
		presetcolorvalPlum, // (Plum Preset Color)
		presetcolorvalPowderBlue, // (Powder Blue Preset Color)
		presetcolorvalPurple, // (Purple Preset Color)
		presetcolorvalRed, // (Red Preset Color)
		presetcolorvalRosyBrown, // (Rosy Brown Preset Color)
		presetcolorvalRoyalBlue, // (Royal Blue Preset Color)
		presetcolorvalSaddleBrown, // (Saddle Brown Preset Color)
		presetcolorvalSalmon, // (Salmon Preset Color)
		presetcolorvalSandyBrown, // (Sandy Brown Preset Color)
		presetcolorvalSeaGreen, // (Sea Green Preset Color)
		presetcolorvalSeaShell, // (Sea Shell Preset Color)
		presetcolorvalSienna, // (Sienna Preset Color)
		presetcolorvalSilver, // (Silver Preset Color)
		presetcolorvalSkyBlue, // (Sky Blue Preset Color)
		presetcolorvalSlateBlue, // (Slate Blue Preset Color)
		presetcolorvalSlateGray, // (Slate Gray Preset Color)
		presetcolorvalSlateGrey, // (Slate Gray Preset Color)
		presetcolorvalSnow, // (Snow Preset Color)
		presetcolorvalSpringGreen, // (Spring Green Preset Color)
		presetcolorvalSteelBlue, // (Steel Blue Preset Color)
		presetcolorvalTan, // (Tan Preset Color)
		presetcolorvalTeal, // (Teal Preset Color)
		presetcolorvalThistle, // (Thistle Preset Color)
		presetcolorvalTomato, // (Tomato Preset Color)
		presetcolorvalTurquoise, // (Turquoise Preset Color)
		presetcolorvalViolet, // (Violet Preset Color)
		presetcolorvalWheat, // (Wheat Preset Color)
		presetcolorvalWhite, // (White Preset Color)
		presetcolorvalWhiteSmoke, // (White Smoke Preset Color)
		presetcolorvalYellow, // (Yellow Preset Color)
		presetcolorvalYellowGreen, // (Yellow Green Preset Color)
	};

	DEFINE_SIMPLE_TYPE_START(CPresetColorVal, EPresetColorVal, presetcolorvalBlack)
	public:
		bool FromStringIgnoreCase(const std::wstring &sValue);
		unsigned char Get_R() const;
		unsigned char Get_G() const;
		unsigned char Get_B() const;
		unsigned char Get_A() const;

		void SetRGBA(unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA = 255);

	private:
		unsigned char m_unR;
		unsigned char m_unG;
		unsigned char m_unB;
		unsigned char m_unA;
	};

	//--------------------------------------------------------------------------------
	// PresetLineDashVal 20.1.10.49 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPresetLineDashVal
	{
		presetlinedashvalDash          = 0,
		presetlinedashvalDashDot       = 1,
		presetlinedashvalDot           = 2,
		presetlinedashvalLgDash        = 3,
		presetlinedashvalLgDashDot     = 4,
		presetlinedashvalLgDashDotDot  = 5,
		presetlinedashvalSolid         = 6,
		presetlinedashvalSysDash       = 7,
		presetlinedashvalSysDashDot    = 8,
		presetlinedashvalSysDashDotDot = 9,
		presetlinedashvalSysDot        = 10,
	};

	DEFINE_SIMPLE_TYPE(CPresetLineDashVal, EPresetLineDashVal, presetlinedashvalSolid)

	//--------------------------------------------------------------------------------
	// PresetMaterialType 20.1.10.50 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPresetMaterialType
	{
		presetmaterialtypeClear             = 0,
		presetmaterialtypeDkEdge            = 1,
		presetmaterialtypeFlat              = 2,
		presetmaterialtypeLegacyMatte       = 3,
		presetmaterialtypeLegacyMetal       = 4,
		presetmaterialtypeLegacyPlastic     = 5,
		presetmaterialtypeLegacyWireframe   = 6,
		presetmaterialtypeMatte             = 7,
		presetmaterialtypeMetal             = 8,
		presetmaterialtypePlastic           = 9,
		presetmaterialtypePowder            = 10,
		presetmaterialtypeSoftEdge          = 11,
		presetmaterialtypeSoftmetal         = 12,
		presetmaterialtypeTranslucentPowder = 13,
		presetmaterialtypeWarmMatte         = 14,
	};

	DEFINE_SIMPLE_TYPE(CPresetMaterialType, EPresetMaterialType, presetmaterialtypeClear)

	//--------------------------------------------------------------------------------
	// PresetPatternVal 20.1.10.51 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPresetPatternVal
	{
		presetpatternvalCross = 0, // (Cross)
		presetpatternvalDashDnDiag, // (Dashed Downward Diagonal)
		presetpatternvalDashHorz, // (Dashed Horizontal)
		presetpatternvalDashUpDiag, // (Dashed Upward DIagonal)
		presetpatternvalDashVert, // (Dashed Vertical)
		presetpatternvalDiagBrick, // (Diagonal Brick)
		presetpatternvalDiagCross, // (Diagonal Cross)
		presetpatternvalDivot, // (Divot)
		presetpatternvalDkDnDiag, // (Dark Downward Diagonal)
		presetpatternvalDkHorz, // (Dark Horizontal)
		presetpatternvalDkUpDiag, // (Dark Upward Diagonal)
		presetpatternvalDkVert, // (Dark Vertical)
		presetpatternvalDnDiag, // (Downward Diagonal)
		presetpatternvalDotDmnd, // (Dotted Diamond)
		presetpatternvalDotGrid, // (Dotted Grid)
		presetpatternvalHorz, // (Horizontal)
		presetpatternvalHorzBrick, // (Horizontal Brick)
		presetpatternvalLgCheck, // (Large Checker Board)
		presetpatternvalLgConfetti, // (Large Confetti)
		presetpatternvalLgGrid, // (Large Grid)
		presetpatternvalLtDnDiag, // (Light Downward Diagonal)
		presetpatternvalLtHorz, // (Light Horizontal)
		presetpatternvalLtUpDiag, // (Light Upward Diagonal)
		presetpatternvalLtVert, // (Light Vertical)
		presetpatternvalNarHorz, // (Narrow Horizontal)
		presetpatternvalNarVert, // (Narrow Vertical)
		presetpatternvalOpenDmnd, // (Open Diamond)
		presetpatternvalPct10, // (10%)
		presetpatternvalPct20, // (20%)
		presetpatternvalPct25, // (25%)
		presetpatternvalPct30, // (30%)
		presetpatternvalPct40, // (40%)
		presetpatternvalPct5, // (5%)
		presetpatternvalPct50, // (50%)
		presetpatternvalPct60, // (60%)
		presetpatternvalPct70, // (70%)
		presetpatternvalPct75, // (75%)
		presetpatternvalPct80, // (80%)
		presetpatternvalPct90, // (90%)
		presetpatternvalPlaid, // (Plaid)
		presetpatternvalShingle, // (Shingle)
		presetpatternvalSmCheck, // (Small Checker Board)
		presetpatternvalSmConfetti, // (Small Confetti)
		presetpatternvalSmGrid, // (Small Grid)
		presetpatternvalSolidDmnd, // (Solid Diamond)
		presetpatternvalSphere, // (Sphere)
		presetpatternvalTrellis, // (Trellis)
		presetpatternvalUpDiag, // (Upward Diagonal)
		presetpatternvalVert, // (Vertical)
		presetpatternvalWave, // (Wave)
		presetpatternvalWdDnDiag, // (Wide Downward Diagonal)
		presetpatternvalWdUpDiag, // (Wide Upward Diagonal)
		presetpatternvalWeave, // (Weave)
		presetpatternvalZigZag, // (Zig Zag)
	};

	DEFINE_SIMPLE_TYPE(CPresetPatternVal, EPresetPatternVal, presetpatternvalPct10)

	//--------------------------------------------------------------------------------
	// PresetShadowVal 20.1.10.52 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPresetShadowVal
	{
		presetshadowvalShdw1  = 1,
		presetshadowvalShdw10 = 10,
		presetshadowvalShdw11 = 11,
		presetshadowvalShdw12 = 12,
		presetshadowvalShdw13 = 13,
		presetshadowvalShdw14 = 14,
		presetshadowvalShdw15 = 15,
		presetshadowvalShdw16 = 16,
		presetshadowvalShdw17 = 17,
		presetshadowvalShdw18 = 18,
		presetshadowvalShdw19 = 19,
		presetshadowvalShdw2  = 2,
		presetshadowvalShdw20 = 20,
		presetshadowvalShdw3  = 3,
		presetshadowvalShdw4  = 4,
		presetshadowvalShdw5  = 5,
		presetshadowvalShdw6  = 6,
		presetshadowvalShdw7  = 7,
		presetshadowvalShdw8  = 8,
		presetshadowvalShdw9  = 9,
	};

	DEFINE_SIMPLE_TYPE(CPresetShadowVal, EPresetShadowVal, presetshadowvalShdw14)

	//--------------------------------------------------------------------------------
	// RectAlignment 20.1.10.53 (Part 1)
	//--------------------------------------------------------------------------------

	enum ERectAlignment
	{
		rectalignmentB   = 0,
		rectalignmentBL  = 1,
		rectalignmentBR  = 2,
		rectalignmentCtr = 3,
		rectalignmentL   = 4,
		rectalignmentR   = 5,
		rectalignmentT   = 6,
		rectalignmentTL  = 7,
		rectalignmentTR  = 8,
	};

	DEFINE_SIMPLE_TYPE(CRectAlignment, ERectAlignment, rectalignmentBL)

	//--------------------------------------------------------------------------------
	// ShemeColorVal 20.1.10.54 (Part 1)
	//--------------------------------------------------------------------------------

	enum EShemeColorVal
	{
		shemecolorvalAccent1  = 0,
		shemecolorvalAccent2  = 1,
		shemecolorvalAccent3  = 2,
		shemecolorvalAccent4  = 3,
		shemecolorvalAccent5  = 4,
		shemecolorvalAccent6  = 5,
		shemecolorvalBg1      = 6,
		shemecolorvalBg2      = 7,
		shemecolorvalDk1      = 8,
		shemecolorvalDk2      = 9,
		shemecolorvalFolHlink = 10,
		shemecolorvalHlink    = 11,
		shemecolorvalLt1      = 12,
		shemecolorvalLt2      = 13,
		shemecolorvalPhClr    = 14,
		shemecolorvalTx1      = 15,
		shemecolorvalTx2      = 16,
	};

	DEFINE_SIMPLE_TYPE(CShemeColorVal, EShemeColorVal, shemecolorvalAccent1)

	//--------------------------------------------------------------------------------
	// ShapeType 20.1.10.56 (Part 1)
	//--------------------------------------------------------------------------------

	enum EShapeType
	{
		shapetypeAccentBorderCallout1 = 0,
		shapetypeAccentBorderCallout2,
		shapetypeAccentBorderCallout3,
		shapetypeAccentCallout1,
		shapetypeAccentCallout2,
		shapetypeAccentCallout3,
		shapetypeActionButtonBackPrevious,
		shapetypeActionButtonBeginning,
		shapetypeActionButtonBlank,
		shapetypeActionButtonDocument,
		shapetypeActionButtonEnd,
		shapetypeActionButtonForwardNext,
		shapetypeActionButtonHelp,
		shapetypeActionButtonHome,
		shapetypeActionButtonInformation,
		shapetypeActionButtonMovie,
		shapetypeActionButtonReturn,
		shapetypeActionButtonSound,
		shapetypeArc,
		shapetypeBentArrow,
		shapetypeBentConnector2,
		shapetypeBentConnector3,
		shapetypeBentConnector4,
		shapetypeBentConnector5,
		shapetypeBentUpArrow,
		shapetypeBevel,
		shapetypeBlockArc,
		shapetypeBorderCallout1,
		shapetypeBorderCallout2,
		shapetypeBorderCallout3,
		shapetypeBracePair,
		shapetypeBracketPair,
		shapetypeCallout1,
		shapetypeCallout2,
		shapetypeCallout3,
		shapetypeCan,
		shapetypeChartPlus,
		shapetypeChartStar,
		shapetypeChartX,
		shapetypeChevron,
		shapetypeChord,
		shapetypeCircularArrow,
		shapetypeCloud,
		shapetypeCloudCallout,
		shapetypeCorner,
		shapetypeCornerTabs,
		shapetypeCube,
		shapetypeCurvedConnector2,
		shapetypeCurvedConnector3,
		shapetypeCurvedConnector4,
		shapetypeCurvedConnector5,
		shapetypeCurvedDownArrow,
		shapetypeCurvedLeftArrow,
		shapetypeCurvedRightArrow,
		shapetypeCurvedUpArrow,
		shapetypeDecagon,
		shapetypeDiagStripe,
		shapetypeDiamond,
		shapetypeDodecagon,
		shapetypeDonut,
		shapetypeDoubleWave,
		shapetypeDownArrow,
		shapetypeDownArrowCallout,
		shapetypeEllipse,
		shapetypeEllipseRibbon,
		shapetypeEllipseRibbon2,
		shapetypeFlowChartAlternateProcess,
		shapetypeFlowChartCollate,
		shapetypeFlowChartConnector,
		shapetypeFlowChartDecision,
		shapetypeFlowChartDelay,
		shapetypeFlowChartDisplay,
		shapetypeFlowChartDocument,
		shapetypeFlowChartExtract,
		shapetypeFlowChartInputOutput,
		shapetypeFlowChartInternalStorage,
		shapetypeFlowChartMagneticDisk,
		shapetypeFlowChartMagneticDrum,
		shapetypeFlowChartMagneticTape,
		shapetypeFlowChartManualInput,
		shapetypeFlowChartManualOperation,
		shapetypeFlowChartMerge,
		shapetypeFlowChartMultidocument,
		shapetypeFlowChartOfflineStorage,
		shapetypeFlowChartOffpageConnector,
		shapetypeFlowChartOnlineStorage,
		shapetypeFlowChartOr,
		shapetypeFlowChartPredefinedProcess,
		shapetypeFlowChartPreparation,
		shapetypeFlowChartProcess,
		shapetypeFlowChartPunchedCard,
		shapetypeFlowChartPunchedTape,
		shapetypeFlowChartSort,
		shapetypeFlowChartSummingJunction,
		shapetypeFlowChartTerminator,
		shapetypeFoldedCorner,
		shapetypeFrame,
		shapetypeFunnel,
		shapetypeGear6,
		shapetypeGear9,
		shapetypeHalfFrame,
		shapetypeHeart,
		shapetypeHeptagon,
		shapetypeHexagon,
		shapetypeHomePlate,
		shapetypeHorizontalScroll,
		shapetypeIrregularSeal1,
		shapetypeIrregularSeal2,
		shapetypeLeftArrow,
		shapetypeLeftArrowCallout,
		shapetypeLeftBrace,
		shapetypeLeftBracket,
		shapetypeLeftCircularArrow,
		shapetypeLeftRightArrow,
		shapetypeLeftRightArrowCallout,
		shapetypeLeftRightCircularArrow,
		shapetypeLeftRightRibbon,
		shapetypeLeftRightUpArrow,
		shapetypeLeftUpArrow,
		shapetypeLightningBolt,
		shapetypeLine,
		shapetypeLineInv,
		shapetypeMathDivide,
		shapetypeMathEqual,
		shapetypeMathMinus,
		shapetypeMathMultiply,
		shapetypeMathNotEqual,
		shapetypeMathPlus,
		shapetypeMoon,
		shapetypeNonIsoscelesTrapezoid,
		shapetypeNoSmoking,
		shapetypeNotchedRightArrow,
		shapetypeOctagon,
		shapetypeParallelogram,
		shapetypePentagon,
		shapetypePie,
		shapetypePieWedge,
		shapetypePlaque,
		shapetypePlaqueTabs,
		shapetypePlus,
		shapetypeQuadArrow,
		shapetypeQuadArrowCallout,
		shapetypeRect,
		shapetypeRibbon,
		shapetypeRibbon2,
		shapetypeRightArrow,
		shapetypeRightArrowCallout,
		shapetypeRightBrace,
		shapetypeRightBracket,
		shapetypeRound1Rect,
		shapetypeRound2DiagRect,
		shapetypeRound2SameRect,
		shapetypeRoundRect,
		shapetypeRtTriangle,
		shapetypeSmileyFace,
		shapetypeSnip1Rect,
		shapetypeSnip2DiagRect,
		shapetypeSnip2SameRect,
		shapetypeSnipRoundRect,
		shapetypeSquareTabs,
		shapetypeStar10,
		shapetypeStar12,
		shapetypeStar16,
		shapetypeStar24,
		shapetypeStar32,
		shapetypeStar4,
		shapetypeStar5,
		shapetypeStar6,
		shapetypeStar7,
		shapetypeStar8,
		shapetypeStraightConnector1,
		shapetypeStripedRightArrow,
		shapetypeSun,
		shapetypeSwooshArrow,
		shapetypeTeardrop,
		shapetypeTrapezoid,
		shapetypeTriangle,
		shapetypeUpArrow,
		shapetypeUpArrowCallout,
		shapetypeUpDownArrow,
		shapetypeUpDownArrowCallout,
		shapetypeUturnArrow,
		shapetypeVerticalScroll,
		shapetypeWave,
		shapetypeWedgeEllipseCallout,
		shapetypeWedgeRectCallout,
		shapetypeWedgeRoundRectCallout,
		////new//////////////
		shapetypeBallon,
		shapetypeRightUpArrow,
		//дубирует с WordArd - может задаваться как внутри текстого бокса, так и в виде объекта
		shapetypeTextArchDownPour,
		shapetypeTextArchUpPour,
		shapetypeTextCanDown,
		shapetypeTextCanUp,
		shapetypeTextCirclePour,
		shapetypeTextCurveDown,
		shapetypeTextCurveUp,
		shapetypeTextDeflate,
		shapetypeTextDeflateBottom,
		shapetypeTextDeflateInflate,
		shapetypeTextDeflateInflateDeflat,
		shapetypeTextDeflateTop,
		shapetypeTextDoubleWave1,
		shapetypeTextFadeDown,
		shapetypeTextFadeLeft,
		shapetypeTextFadeRight,
		shapetypeTextFadeUp,
		shapetypeTextInflateBottom,
		shapetypeTextInflateTop,
		shapetypeTextRingInside,
		shapetypeTextRingOutside,
		shapetypeTextWave1,
		shapetypeTextWave2,
		shapetypeTextWave4,
		shapetypeThickArrow

	};

	DEFINE_SIMPLE_TYPE(CShapeType, EShapeType, shapetypeRect)

	//--------------------------------------------------------------------------------
	// SystemColorVal 20.1.10.58 (Part 1) (included from ASCWinAPI.h)
	//--------------------------------------------------------------------------------

	enum ESystemColorVal
	{
		systemcolorval3dDkShadow = 0, // (3D Dark System Color)
		systemcolorval3dLight, // (3D Light System Color)
		systemcolorvalActiveBorder, // (Active Border System Color)
		systemcolorvalActiveCaption, // (Active Caption System Color)
		systemcolorvalAppWorkspace, // (Application Workspace System Color)
		systemcolorvalBackground, // (Background System Color)
		systemcolorvalBtnFace, // (Button Face System Color)
		systemcolorvalBtnHighlight, // (Button Highlight System Color)
		systemcolorvalBtnShadow, // (Button Shadow System Color)
		systemcolorvalBtnText, // (Button Text System Color)
		systemcolorvalCaptionText, // (Caption Text System Color)
		systemcolorvalGradientActiveCaption, // (Gradient Active Caption System Color)
		systemcolorvalGradientInactiveCaption, // (Gradient Inactive Caption System Color)
		systemcolorvalGrayText, // (Gray Text System Color)
		systemcolorvalHighlight, // (Highlight System Color)
		systemcolorvalHighlightText, // (Highlight Text System Color)
		systemcolorvalHotLight, // (Hot Light System Color)
		systemcolorvalInactiveBorder, // (Inactive Border System Color)
		systemcolorvalInactiveCaption, // (Inactive Caption System Color)
		systemcolorvalInactiveCaptionText, // (Inactive Caption Text System Color)
		systemcolorvalInfoBk, // (Info Back System Color)
		systemcolorvalInfoText, // (Info Text System Color)
		systemcolorvalMenu, // (Menu System Color)
		systemcolorvalMenuBar, // (Menu Bar System Color)
		systemcolorvalMenuHighlight, // (Menu Highlight System Color)
		systemcolorvalMenuText, // (Menu Text System Color)
		systemcolorvalScrollBar, // (Scroll Bar System Color)
		systemcolorvalWindow, // (Window System Color)
		systemcolorvalWindowFrame, // (Window Frame System Color)
		systemcolorvalWindowText, // (Window Text System Color)
	};

	DEFINE_SIMPLE_TYPE_START(CSystemColorVal, ESystemColorVal, systemcolorvalWindow)
	public:
		unsigned char Get_R() const;
		unsigned char Get_G() const;
		unsigned char Get_B() const;
		unsigned char Get_A() const;

		void SetRGBASys(int nIndex);
		void SetRGBA(unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA = 255);

	private:

		unsigned char m_unR;
		unsigned char m_unG;
		unsigned char m_unB;
		unsigned char m_unA;
	};

	//--------------------------------------------------------------------------------
	// TextAlignmentType 20.1.10.59 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextAlignmentType
	{
		textalignmenttypeL    = 0,
		textalignmenttypeCtr  = 1,
		textalignmenttypeDist = 2,
		textalignmenttypeJust = 3,
		textalignmenttypeR    = 4,
	};

	DEFINE_SIMPLE_TYPE(CTextAlignmentType, ETextAlignmentType, textalignmenttypeL)
	
	//--------------------------------------------------------------------------------
	// TextAnchoringType 20.1.10.60 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextAnchoringType
	{
		textanchoringtypeB    = 0,
		textanchoringtypeCtr  = 1,
		textanchoringtypeDist = 2,
		textanchoringtypeJust = 3,
		textanchoringtypeT    = 4,
	};

	DEFINE_SIMPLE_TYPE(CTextAnchoringType, ETextAnchoringType, textanchoringtypeT)

	//--------------------------------------------------------------------------------
	// TextColumnCount 20.1.10.65 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_NSV(CTextColumnCount, unsigned char, 1)
	
	//--------------------------------------------------------------------------------
	// 20.1.10.66 TextFontAlignType (Font Alignment Types)
	//--------------------------------------------------------------------------------

	enum ETextFontAlignType
	{
		textfontaligntypeAuto   = 0,
		textfontaligntypeCtr	= 1,
		textfontaligntypeBase	= 2,
		textfontaligntypeT		= 3,
		textfontaligntypeB		= 4,
	};

	DEFINE_SIMPLE_TYPE(CTextFontAlignType, ETextFontAlignType, textfontaligntypeAuto)
	
	//--------------------------------------------------------------------------------
	// TextFontScalePercentOrPercentString 20.1.10.67 (Part 1) + 12.1.2.5 (Part4)
	//--------------------------------------------------------------------------------

	class CTextFontScalePercentOrPercentString
	{
	public:
		CTextFontScalePercentOrPercentString();

		double GetValue() const;
		void   SetValue(double dValue);

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultD(CTextFontScalePercentOrPercentString)

		private:

			double m_dValue;
	};

	//--------------------------------------------------------------------------------
	// TextHorzOverflowType 20.1.10.69 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextHorzOverflowType
	{
		texthorzoverflowtypeClip     = 0,
		texthorzoverflowtypeOverflow = 1,
	};

	DEFINE_SIMPLE_TYPE(CTextHorzOverflowType, ETextHorzOverflowType, texthorzoverflowtypeOverflow)

	//--------------------------------------------------------------------------------
	// 20.1.10.70 ST_TextIndent (Text Indentation)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CTextIndent, _INT64, 0)
	public:
		double ToPoints();
		double ToInches();
		double ToCm();
	};

	//--------------------------------------------------------------------------------
	// 20.1.10.72 ST_TextMargin (Text Margin)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CTextMargin, _INT64, 0)
	public:
		double ToPoints();
		double ToInches();
		double ToCm();
	};

	//	//--------------------------------------------------------------------------------
	//    // 20.1.10.74 ST_TextPoint (Text Point)
	//	//--------------------------------------------------------------------------------
	//    template<__int64 nDefValue = 0>
	//    class CTextPoint : public CSimpleType<__int64, nDefValue>
	//    {
	//    public:
	//	The ST_TextPointUnqualified simple type (§20.1.10.75).
	//	The ST_UniversalMeasure simple type (§22.9.2.15).
	//	CTextPoint();
	//
	//        virtual __int64 FromString(const std::wstring &sValue)
	//        {
    //            this->m_eValue = XmlUtils::GetInteger64( sValue );
	//            if (this->m_eValue < 0)
	//                this->m_eValue = 0;
	//            if (this->m_eValue > 51206400)
	//                this->m_eValue = 51206400;
	//
	//            return this->m_eValue;
	//        }
	//
	//        virtual std::wstring ToString  () const
	//        {
	//            std::wstring sResult = std::to_wstring( this->m_eValue);
	//
	//            return sResult;
	//        }
	//
	//        SimpleType_FromString     (__int64)
	//        SimpleTypes_Default (TextPoint)
	//
	//        double ToPoints()
	//        {
	//            return  Emu_To_Pt(this->m_eValue);
	//        }
	//
	//        double ToInches()
	//        {
	//            return Emu_To_Inch( this->m_eValue );
	//        }
	//        double ToCm()
	//        {
	//            return Emu_To_Cm( this->m_eValue );
	//        }
	//    };

	//--------------------------------------------------------------------------------
	// TextShapeType 20.1.10.76 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextShapeType
	{
		textshapetypeTextArchDown = 0,
		textshapetypeTextArchDownPour,
		textshapetypeTextArchUp,
		textshapetypeTextArchUpPour,
		textshapetypeTextButton,
		textshapetypeTextButtonPour,
		textshapetypeTextCanDown,
		textshapetypeTextCanUp,
		textshapetypeTextCascadeDown,
		textshapetypeTextCascadeUp,
		textshapetypeTextChevron,
		textshapetypeTextChevronInverted,
		textshapetypeTextCircle,
		textshapetypeTextCirclePour,
		textshapetypeTextCurveDown,
		textshapetypeTextCurveUp,
		textshapetypeTextDeflate,
		textshapetypeTextDeflateBottom,
		textshapetypeTextDeflateInflate,
		textshapetypeTextDeflateInflateDeflate,
		textshapetypeTextDeflateTop,
		textshapetypeTextDoubleWave1,
		textshapetypeTextFadeDown,
		textshapetypeTextFadeLeft,
		textshapetypeTextFadeRight,
		textshapetypeTextFadeUp,
		textshapetypeTextInflate,
		textshapetypeTextInflateBottom,
		textshapetypeTextInflateTop,
		textshapetypeTextNoShape,
		textshapetypeTextPlain,
		textshapetypeTextRingInside,
		textshapetypeTextRingOutside,
		textshapetypeTextSlantDown,
		textshapetypeTextSlantUp,
		textshapetypeTextStop,
		textshapetypeTextTriangle,
		textshapetypeTextTriangleInverted,
		textshapetypeTextWave1,
		textshapetypeTextWave2,
		textshapetypeTextWave4,
	};

	DEFINE_SIMPLE_TYPE(CTextShapeType, ETextShapeType, textshapetypeTextPlain)

	//--------------------------------------------------------------------------------
	// TextSpacingPercentOrPercentString 20.1.10.77 (Part 1) + 12.1.2.7 (Part4)
	//--------------------------------------------------------------------------------

	class CTextSpacingPercentOrPercentString
	{
	public:
		CTextSpacingPercentOrPercentString();

		double GetValue() const;
		void   SetValue(double dValue);

		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultD(CTextSpacingPercentOrPercentString)

		private:
			double m_dValue;
	};

	//--------------------------------------------------------------------------------
	// 20.1.10.78 ST_TextSpacingPoint (Text Spacing Point)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CTextSpacingPoint, _INT64, 0)
	public:
		double ToPoints();
		double ToInches();
		double ToCm();
	};

	//--------------------------------------------------------------------------------
	// TextTypeface 20.1.10.81 (Part 1)
	//--------------------------------------------------------------------------------

	class CTextTypeface
	{
	public:
		CTextTypeface();

		std::wstring GetValue() const;
		void    SetValue(std::wstring &sValue);

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CTextTypeface)

		private:
			std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// TextVerticalType 20.1.10.83 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextVerticalType
	{
		textverticaltypeEaVert         = 0,
		textverticaltypeHorz           = 1,
		textverticaltypeMongolianVert  = 2,
		textverticaltypeVert           = 3,
		textverticaltypeVert270        = 4,
		textverticaltypeWordArtVert    = 5,
		textverticaltypeWordArtVertRtl = 6,
	};

	DEFINE_SIMPLE_TYPE(CTextVerticalType, ETextVerticalType, textverticaltypeHorz)

	//--------------------------------------------------------------------------------
	// TextVertOverflowType 20.1.10.84 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextVertOverflowType
	{
		textvertoverflowtypeClip     = 0,
		textvertoverflowtypeEllipsis = 1,
		textvertoverflowtypeOverflow = 2,
	};

	DEFINE_SIMPLE_TYPE(CTextVertOverflowType, ETextVertOverflowType, textvertoverflowtypeOverflow)

	//--------------------------------------------------------------------------------
	// TextWrappingType 20.1.10.85 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextWrappingType
	{
		textwrappingtypeNone   = 0,
		textwrappingtypeSquare = 1,
	};

	DEFINE_SIMPLE_TYPE(CTextWrappingType, ETextWrappingType, textwrappingtypeNone)

	//--------------------------------------------------------------------------------
	// TileFlipMode 20.1.10.86 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETileFlipMode
	{
		tileflipmodeNone = 0,
		tileflipmodeX    = 1,
		tileflipmodeXY   = 2,
		tileflipmodeY    = 3,
	};

	DEFINE_SIMPLE_TYPE(CTileFlipMode, ETileFlipMode, tileflipmodeNone)

} // SimpleTypes

// Здесь представлены все простые типы Drawing-Word из спецификации Office Open Xml (20.4.3)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// AlignH 20.4.3.1 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAlignH
	{
		alignhCenter  = 0,
		alignhInside  = 1,
		alignhLeft    = 2,
		alignhOutside = 3,
		alignhRight   = 4
	};

	DEFINE_SIMPLE_TYPE(CAlignH, EAlignH, alignhLeft)

	//--------------------------------------------------------------------------------
	// AlignV 20.4.3.2 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAlignV
	{
		alignvBottom  = 0,
		alignvCenter  = 1,
		alignvInside  = 2,
		alignvOutside = 3,
		alignvTop     = 4
	};

	DEFINE_SIMPLE_TYPE(CAlignV, EAlignV, alignvTop)

	//--------------------------------------------------------------------------------
	// PositionOffset 10.4.3.3 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CPositionOffset, _INT64, 0)
	public:
		double ToPoints();
		double ToMM() const;
		double ToInches();
		double ToTwips();
	};

	//--------------------------------------------------------------------------------
	// RelFromH 20.4.3.4 (Part 1)
	//--------------------------------------------------------------------------------

	enum ERelFromH
	{
		relfromhCharacter     = 0,
		relfromhColumn        = 1,
		relfromhInsideMargin  = 2,
		relfromhLeftMargin    = 3,
		relfromhMargin        = 4,
		relfromhOutsideMargin = 5,
		relfromhPage          = 6,
		relfromhRightMargin   = 7
	};

	DEFINE_SIMPLE_TYPE(CRelFromH, ERelFromH, relfromhPage)

	//--------------------------------------------------------------------------------
	// RelFromV 20.4.3.5 (Part 1)
	//--------------------------------------------------------------------------------

	enum ERelFromV
	{
		relfromvBottomMargin  = 0,
		relfromvInsideMargin  = 1,
		relfromvLine          = 2,
		relfromvMargin        = 3,
		relfromvOutsideMargin = 4,
		relfromvPage          = 5,
		relfromvParagraph     = 6,
		relfromvTopMargin     = 7
	};

	DEFINE_SIMPLE_TYPE(CRelFromV, ERelFromV, relfromvPage)

	//--------------------------------------------------------------------------------
	// SizeRelFromH
	//--------------------------------------------------------------------------------

	enum ESizeRelFromH
	{
		sizerelfromhMargin     = 0,
		sizerelfromhPage        = 1,
		sizerelfromhLeftMargin    = 2,
		sizerelfromhRightMargin   = 3,
		sizerelfromhInsideMargin  = 4,
		sizerelfromhOutsideMargin = 5
	};

	DEFINE_SIMPLE_TYPE(CSizeRelFromH, ESizeRelFromH, sizerelfromhPage)

	//--------------------------------------------------------------------------------
	// SizeRelFromV
	//--------------------------------------------------------------------------------

	enum ESizeRelFromV
	{
		sizerelfromvMargin			= 0,
		sizerelfromvPage			= 1,
		sizerelfromvTopMargin		= 2,
		sizerelfromvBottomMargin	= 3,
		sizerelfromvInsideMargin	= 4,
		sizerelfromvOutsideMargin	= 5
	};

	DEFINE_SIMPLE_TYPE(CSizeRelFromV, ESizeRelFromV, sizerelfromvPage)

	//--------------------------------------------------------------------------------
	// WrapDistance 10.4.3.6 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CWrapDistance, _INT64, 0)
	public:
		double ToPoints();
		double ToMM() const;
		double ToInches();
		double ToTwips();
	};

	//--------------------------------------------------------------------------------
	// WrapText 20.4.3.7 (Part 1)
	//--------------------------------------------------------------------------------

	enum EWrapText
	{
		wraptextBothSides = 0,
		wraptextLargest   = 1,
		wraptextLeft      = 2,
		wraptextRight     = 3
	};

	DEFINE_SIMPLE_TYPE(CWrapText, EWrapText, wraptextLeft)

} // SimpleTypes

// Здесь представлены все простые типы Drawing-Spreadsheet из спецификации Office Open Xml (20.5.3)
namespace SimpleTypes
{

} // SimpleTypes

// Здесь представлены все простые типы Drawing-Charts из спецификации Office Open Xml (21.2.3, 21.3.3)
namespace SimpleTypes
{

} // SimpleTypes

// Здесь представлены все простые типы Drawing-Diagrams из спецификации Office Open Xml (21.4.7)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// HueDirType
	//--------------------------------------------------------------------------------

	enum EHueDirType
	{
		hueDirCCw = 0,
		hueDirCw = 1
	};

	DEFINE_SIMPLE_TYPE(CHueDirType, EHueDirType, hueDirCw)

	//--------------------------------------------------------------------------------
	// ColorMethod
	//--------------------------------------------------------------------------------

	enum EColorMethod
	{
		colorMethodCycle = 0,
		colorMethodRepeat = 1,
		colorMethodSpan = 2
	};

	DEFINE_SIMPLE_TYPE(CColorMethod, EColorMethod, colorMethodSpan)

	//--------------------------------------------------------------------------------
	// ChOrder
	//--------------------------------------------------------------------------------

	enum EChOrder
	{
		chOrderB = 0,
		chOrderT = 1
	};

	DEFINE_SIMPLE_TYPE(CChOrder, EChOrder, chOrderB)

	//--------------------------------------------------------------------------------
	// AxisTypes
	//--------------------------------------------------------------------------------

	enum EAxisTypes
	{
		axisTypes_none = 0,
		axisTypes_self = 1,
		axisTypes_ch = 2,
		axisTypes_des = 3,
		axisTypes_desOrSelf = 4,
		axisTypes_par = 5,
		axisTypes_ancst = 6,
		axisTypes_ancstOrSelf = 7,
		axisTypes_followSib = 8,
		axisTypes_precedSib = 9,
		axisTypes_follow = 10,
		axisTypes_preced = 11,
		axisTypes_root = 12
	};

	DEFINE_SIMPLE_TYPE(CAxisTypes, EAxisTypes, axisTypes_none)

	//--------------------------------------------------------------------------------
	// ParameterId
	//--------------------------------------------------------------------------------

	enum EParameterId
	{
		parameterId_horzAlign = 0,
		parameterId_vertAlign = 1,
		parameterId_chDir = 2,
		parameterId_chAlign = 3,
		parameterId_secChAlign = 4,
		parameterId_linDir = 5,
		parameterId_secLinDir = 6,
		parameterId_stElem = 7,
		parameterId_bendPt = 8,
		parameterId_connRout = 9,
		parameterId_begSty = 10,
		parameterId_endSty = 11,
		parameterId_dim = 12,
		parameterId_rotPath = 13,
		parameterId_ctrShpMap = 14,
		parameterId_nodeHorzAlign = 15,
		parameterId_nodeVertAlign = 16,
		parameterId_fallback = 17,
		parameterId_txDir = 18,
		parameterId_pyraAcctPos = 19,
		parameterId_pyraAcctTxMar = 20,
		parameterId_txBlDir = 21,
		parameterId_txAnchorHorz = 22,
		parameterId_txAnchorVert = 23,
		parameterId_txAnchorHorzCh = 24,
		parameterId_txAnchorVertCh = 25,
		parameterId_parTxLTRAlign = 26,
		parameterId_parTxRTLAlign = 27,
		parameterId_shpTxLTRAlignCh = 28,
		parameterId_shpTxRTLAlignCh = 29,
		parameterId_autoTxRot = 30,
		parameterId_grDir = 31,
		parameterId_flowDir = 32,
		parameterId_contDir = 33,
		parameterId_bkpt = 34,
		parameterId_off = 35,
		parameterId_hierAlign = 36,
		parameterId_bkPtFixedVal = 37,
		parameterId_stBulletLvl = 38,
		parameterId_stAng = 39,
		parameterId_spanAng = 40,
		parameterId_ar = 41,
		parameterId_lnSpPar = 42,
		parameterId_lnSpAfParP = 43,
		parameterId_lnSpCh = 44,
		parameterId_lnSpAfChP = 45,
		parameterId_rtShortDist = 46,
		parameterId_alignTx = 47,
		parameterId_pyraLvlNode = 48,
		parameterId_pyraAcctBkgdNode = 49,
		parameterId_pyraAcctTxNode = 50,
		parameterId_srcNode = 51,
		parameterId_dstNode = 52,
		parameterId_begPts = 53,
		parameterId_endPts = 54
	};

	DEFINE_SIMPLE_TYPE(CParameterId, EParameterId, parameterId_horzAlign)

	//--------------------------------------------------------------------------------
	// ConstraintRelationship
	//--------------------------------------------------------------------------------

	enum EConstraintRelationship
	{
		constraintRels_self = 0,
		constraintRels_ch = 1,
		constraintRels_des = 2
	};

	DEFINE_SIMPLE_TYPE(CConstraintRelationship, EConstraintRelationship, constraintRels_self)

	//--------------------------------------------------------------------------------
	// ConstraintType
	//--------------------------------------------------------------------------------

	enum EConstraintType
	{
		constraintType_none = 0,
		constraintType_alignOff = 1,
		constraintType_begMarg = 2,
		constraintType_bendDist = 3,
		constraintType_begPad = 4,
		constraintType_b = 5,
		constraintType_bMarg = 6,
		constraintType_bOff = 7,
		constraintType_ctrX = 8,
		constraintType_ctrXOff = 9,
		constraintType_ctrY = 10,
		constraintType_ctrYOff = 11,
		constraintType_connDist = 12,
		constraintType_diam = 13,
		constraintType_endMarg = 14,
		constraintType_endPad = 15,
		constraintType_h = 16,
		constraintType_hArH = 17,
		constraintType_l = 18,
		constraintType_lMarg = 19,
		constraintType_lOff = 20,
		constraintType_r = 21,
		constraintType_rMarg = 22,
		constraintType_rOff = 23,
		constraintType_primFontSz = 24,
		constraintType_pyraAcctRatio = 25,
		constraintType_secFontSz = 26,
		constraintType_sibSp = 27,
		constraintType_secSibSp = 28,
		constraintType_sp = 29,
		sconstraintType_temThick = 30,
		constraintType_t = 31,
		constraintType_tMarg = 32,
		constraintType_tOff = 33,
		constraintType_userA = 34,
		constraintType_userB = 35,
		constraintType_userC = 36,
		constraintType_userD = 37,
		constraintType_userE = 38,
		constraintType_userF = 39,
		constraintType_userG = 40,
		constraintType_userH = 41,
		constraintType_userI = 42,
		constraintType_userJ = 43,
		constraintType_userK = 44,
		constraintType_userL = 45,
		constraintType_userM = 46,
		constraintType_userN = 47,
		constraintType_userO = 48,
		constraintType_userP = 49,
		constraintType_userQ = 50,
		constraintType_userR = 51,
		constraintType_userS = 52,
		constraintType_userT = 53,
		constraintType_userU = 54,
		constraintType_userV = 55,
		constraintType_userW = 56,
		constraintType_userX = 57,
		constraintType_userY = 58,
		constraintType_userZ = 59,
		constraintType_w = 60,
		constraintType_wArH = 61,
		constraintType_wOff = 62
	};

	DEFINE_SIMPLE_TYPE(CConstraintType, EConstraintType, constraintType_none)

	//--------------------------------------------------------------------------------
	// BoolOperator
	//--------------------------------------------------------------------------------

	enum EBoolOperator
	{
		boolOperator_none = 0,
		boolOperator_equ = 1,
		boolOperator_gte = 2,
		boolOperator_lte = 3
	};

	DEFINE_SIMPLE_TYPE(CBoolOperator, EBoolOperator, boolOperator_none)

	//--------------------------------------------------------------------------------
	// ElementTypes
	//--------------------------------------------------------------------------------

	enum EElementTypes
	{
		elementTypes_all = 0,
		elementTypes_doc = 1,
		elementTypes_node = 2,
		elementTypes_norm = 3,
		elementTypes_nonNorm = 4,
		elementTypes_asst = 5,
		elementTypes_nonAsst = 6,
		elementTypes_parTrans = 7,
		elementTypes_pres = 8,
		elementTypes_sibTrans = 9
	};

	DEFINE_SIMPLE_TYPE(CElementTypes, EElementTypes, elementTypes_all)

	//--------------------------------------------------------------------------------
	// AlgorithmType
	//--------------------------------------------------------------------------------

	enum EAlgorithmType
	{
		algorithmType_composite = 0,
		algorithmType_conn = 1,
		algorithmType_cycle = 2,
		algorithmType_hierChild = 3,
		algorithmType_hierRoot = 4,
		algorithmType_pyra = 5,
		algorithmType_lin = 6,
		algorithmType_sp = 7,
		algorithmType_tx = 8,
		algorithmType_snake = 9
	};

	DEFINE_SIMPLE_TYPE(CAlgorithmType, EAlgorithmType, algorithmType_composite)

	//--------------------------------------------------------------------------------
	// FunctionType
	//--------------------------------------------------------------------------------

	enum EFunctionType
	{
		functionType_cnt = 0,
		functionType_pos = 1,
		functionType_revPos = 2,
		functionType_posEven = 3,
		functionType_posOdd = 4,
		functionType_var = 5,
		functionType_depth = 6,
		functionType_maxDepth = 7
	};

	DEFINE_SIMPLE_TYPE(CFunctionType, EFunctionType, functionType_cnt)

	//--------------------------------------------------------------------------------
	// FunctionOperator
	//--------------------------------------------------------------------------------

	enum EFunctionOperator
	{
		functionOperator_equ = 0,
		functionOperator_neq = 1,
		functionOperator_gt = 2,
		functionOperator_lt = 3,
		functionOperator_gte = 4,
		functionOperator_lte = 5
	};

	DEFINE_SIMPLE_TYPE(CFunctionOperator, EFunctionOperator, functionOperator_equ)

	//--------------------------------------------------------------------------------
	// AnimLvlStr
	//--------------------------------------------------------------------------------

	enum EAnimLvlStr
	{
		animLvlStr_none = 0,
		animLvlStr_ctr = 1,
		animLvlStr_lvl = 2
	};

	DEFINE_SIMPLE_TYPE(CAnimLvlStr, EAnimLvlStr, animLvlStr_none)

	//--------------------------------------------------------------------------------
	// AnimOneStr
	//--------------------------------------------------------------------------------

	enum EAnimOneStr
	{
		animOneStr_none = 0,
		animOneStr_branch = 1,
		animOneStr_one = 2
	};

	DEFINE_SIMPLE_TYPE(CAnimOneStr, EAnimOneStr, animOneStr_none)

	//--------------------------------------------------------------------------------
	// DirectionDraw
	//--------------------------------------------------------------------------------

	enum EDirectionDraw
	{
		direction_norm = 0,
		direction_rev = 1
	};

	DEFINE_SIMPLE_TYPE(CDirectionDraw, EDirectionDraw, direction_norm)

	//--------------------------------------------------------------------------------
	// HierBranch
	//--------------------------------------------------------------------------------

	enum EHierBranch
	{
		hierBranch_hang = 0,
		hierBranch_init = 1,
		hierBranch_l = 2,
		hierBranch_r = 3,
		hierBranch_std = 4
	};

	DEFINE_SIMPLE_TYPE(CHierBranch, EHierBranch, hierBranch_hang)

	//--------------------------------------------------------------------------------
	// ResizeHandles
	//--------------------------------------------------------------------------------

	enum EResizeHandles
	{
		resizeHandles_exact = 0,
		resizeHandles_rel = 1
	};

	DEFINE_SIMPLE_TYPE(CResizeHandles, EResizeHandles, resizeHandles_exact)

	//--------------------------------------------------------------------------------
	// PtTypes
	//--------------------------------------------------------------------------------

	enum EPtTypes
	{
		ptTypes_node = 0,
		ptTypes_asst = 1,
		ptTypes_doc = 2,
		ptTypes_pres = 3,
		ptTypes_parTrans = 4,
		ptTypes_sibTrans = 5
	};

	DEFINE_SIMPLE_TYPE(CPtTypes, EPtTypes, ptTypes_node)

} // SimpleTypes

