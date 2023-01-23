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

// Здесь представлены все простые типы Word из спецификации Office Open Xml (17.18)
namespace SimpleTypes
{	
	//--------------------------------------------------------------------------------
	// AnnotationVMerge 17.18.1 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAnnotationVMerge
	{
		annotvmergeCont = 0,
		annotvmergeRest = 1
	};	

	DEFINE_SIMPLE_TYPE(CAnnotationVMerge, EAnnotationVMerge, annotvmergeCont)

	//--------------------------------------------------------------------------------
	// Border 17.18.2 (Part 1)
	//--------------------------------------------------------------------------------

	// Значения с 1-го по 25-ый сделаны по таблице 17.4.67
	enum EBorder
	{
		// No Border
		bordervalueNone                      = -1,
		bordervalueNil                       =  0,

		// Line Border
		bordervalueSingle                    =  1,
		bordervalueThick                     =  2,
		bordervalueDouble                    =  3,
		bordervalueDotted                    =  4,
		bordervalueDashed                    =  5,
		bordervalueDotDash                   =  6,
		bordervalueDotDotDash                =  7,
		bordervalueTriple                    =  8,
		bordervalueThinThickSmallGap         =  9,
		bordervalueThickThinSmallGap         = 10,
		bordervalueThinThickThinSmallGap     = 11,
		bordervalueThinThickMediumGap        = 12,
		bordervalueThickThinMediumGap        = 13,
		bordervalueThinThickThinMediumGap    = 14,
		bordervalueThinThickLargeGap         = 15,
		bordervalueThickThinLargeGap         = 16,
		bordervalueThinThickThinLargeGap     = 17,
		bordervalueWave                      = 18,
		bordervalueDoubleWave                = 19,
		bordervalueDashSmallGap              = 20,
		bordervalueDashDotStroked            = 21,
		bordervalueThreeDEmboss              = 22,
		bordervalueThreeDEngrave             = 23,
		bordervalueOutset                    = 24,
		bordervalueInset                     = 25,

		// ArtBorder
		bordervalueApples                    = 101,
		bordervalueArchedScallops            = 102,
		bordervalueBabyPacifier              = 103,
		bordervalueBabyRattle                = 104,
		bordervalueBalloons3Colors           = 105,
		bordervalueBalloonsHotAir            = 106,
		bordervalueBasicBlackDashes          = 107,
		bordervalueBasicBlackDots            = 108,
		bordervalueBasicBlackSquares         = 109,
		bordervalueBasicThinLines            = 110,
		bordervalueBasicWhiteDashes          = 111,
		bordervalueBasicWhiteDots            = 112,
		bordervalueBasicWhiteSquares         = 113,
		bordervalueBasicWideInline           = 114,
		bordervalueBasicWideMidline          = 115,
		bordervalueBasicWideOutline          = 116,
		bordervalueBats                      = 117,
		bordervalueBirds                     = 118,
		bordervalueBirdsFlight               = 119,
		bordervalueCabins                    = 120,
		bordervalueCakeSlice                 = 121,
		bordervalueCandyCorn                 = 122,
		bordervalueCelticKnotwork            = 123,
		bordervalueCertificateBanner         = 124,
		bordervalueChainLink                 = 125,
		bordervalueChampagneBottle           = 126,
		bordervalueCheckedBarBlack           = 127,
		bordervalueCheckedBarColor           = 128,
		bordervalueCheckered                 = 129,
		bordervalueChristmasTree             = 130,
		bordervalueCirclesLines              = 131,
		bordervalueCirclesRectangles         = 132,
		bordervalueClassicalWave             = 133,
		bordervalueClocks                    = 134,
		bordervalueCompass                   = 135,
		bordervalueConfetti                  = 136,
		bordervalueConfettiGrays             = 137,
		bordervalueConfettiOutline           = 138,
		bordervalueConfettiStreamers         = 139,
		bordervalueConfettiWhite             = 140,
		bordervalueCornerTriangles           = 141,
		bordervalueCouponCutoutDashes        = 142,
		bordervalueCouponCutoutDots          = 143,
		bordervalueCrazyMaze                 = 144,
		bordervalueCreaturesButterfly        = 145,
		bordervalueCreaturesFish             = 146,
		bordervalueCreaturesInsects          = 147,
		bordervalueCreaturesLadyBug          = 148,
		bordervalueCrossStitch               = 149,
		bordervalueCup                       = 150,
		bordervalueCustom                    = 151,
		bordervalueDecoArch                  = 152,
		bordervalueDecoArchColor             = 153,
		bordervalueDecoBlocks                = 154,
		bordervalueDiamondsGray              = 155,
		bordervalueDoubleD                   = 156,
		bordervalueDoubleDiamonds            = 157,
		bordervalueEarth1                    = 158,
		bordervalueEarth2                    = 159,
		bordervalueEarth3                    = 160,
		bordervalueEclipsingSquares1         = 161,
		bordervalueEclipsingSquares2         = 162,
		bordervalueEggsBlack                 = 163,
		bordervalueFans                      = 164,
		bordervalueFilm                      = 165,
		bordervalueFirecrackers              = 166,
		bordervalueFlowersBlockPrint         = 167,
		bordervalueFlowersDaisies            = 168,
		bordervalueFlowersModern1            = 169,
		bordervalueFlowersModern2            = 170,
		bordervalueFlowersPansy              = 171,
		bordervalueFlowersRedRose            = 172,
		bordervalueFlowersRoses              = 173,
		bordervalueFlowersTeacup             = 174,
		bordervalueFlowersTiny               = 175,
		bordervalueGems                      = 176,
		bordervalueGingerbreadMan            = 177,
		bordervalueGradient                  = 178,
		bordervalueHandmade1                 = 179,
		bordervalueHandmade2                 = 180,
		bordervalueHeartBalloon              = 181,
		bordervalueHeartGray                 = 182,
		bordervalueHearts                    = 183,
		bordervalueHeebieJeebies             = 184,
		bordervalueHolly                     = 185,
		bordervalueHouseFunky                = 186,
		bordervalueHypnotic                  = 187,
		bordervalueIceCreamCones             = 188,
		bordervalueLightBulb                 = 189,
		bordervalueLightning1                = 190,
		bordervalueLightning2                = 191,
		bordervalueMapleLeaf                 = 192,
		bordervalueMapleMuffins              = 193,
		bordervalueMapPins                   = 194,
		bordervalueMarquee                   = 195,
		bordervalueMarqueeToothed            = 196,
		bordervalueMoons                     = 197,
		bordervalueMosaic                    = 198,
		bordervalueMusicNotes                = 199,
		bordervalueNorthwest                 = 200,
		bordervalueOvals                     = 201,
		bordervaluePackages                  = 202,
		bordervaluePalmsBlack                = 203,
		bordervaluePalmsColor                = 204,
		bordervaluePaperClips                = 205,
		bordervaluePapyrus                   = 206,
		bordervaluePartyFavor                = 207,
		bordervaluePartyGlass                = 208,
		bordervaluePencils                   = 209,
		bordervaluePeople                    = 210,
		bordervaluePeopleHats                = 211,
		bordervaluePeopleWaving              = 212,
		bordervaluePoinsettias               = 213,
		bordervaluePostageStamp              = 214,
		bordervaluePumpkin1                  = 215,
		bordervaluePushPinNote1              = 216,
		bordervaluePushPinNote2              = 217,
		bordervaluePyramids                  = 218,
		bordervaluePyramidsAbove             = 219,
		bordervalueQuadrants                 = 220,
		bordervalueRings                     = 221,
		bordervalueSafari                    = 222,
		bordervalueSawtooth                  = 223,
		bordervalueSawtoothGray              = 224,
		bordervalueScaredCat                 = 225,
		bordervalueSeattle                   = 226,
		bordervalueSharksTeeth               = 227,
		bordervalueShadowedSquares           = 228,
		bordervalueShapes1                   = 229,
		bordervalueShapes2                   = 230,
		bordervalueShorebirdTracks           = 231,
		bordervalueSkyrocket                 = 232,
		bordervalueSnowflakeFancy            = 233,
		bordervalueSnowflakes                = 234,
		bordervalueSombrero                  = 235,
		bordervalueSouthwest                 = 236,
		bordervalueStars                     = 237,
		bordervalueStars3d                   = 238,
		bordervalueStarsBlack                = 239,
		bordervalueStarsShadowed             = 240,
		bordervalueStarsTop                  = 241,
		bordervalueSun                       = 242,
		bordervalueSwirligig                 = 243,
		bordervalueTornPaper                 = 244,
		bordervalueTornPaperBlack            = 245,
		bordervalueTrees                     = 246,
		bordervalueTriangle1                 = 247,
		bordervalueTriangle2                 = 248,
		bordervalueTriangleCircle1           = 249,
		bordervalueTriangleCircle2           = 250,
		bordervalueTriangleParty             = 251,
		bordervalueTriangles                 = 252,
		bordervalueTwistedLines1             = 253,
		bordervalueTwistedLines2             = 254,
		bordervalueVine                      = 255,
		bordervalueWaveline                  = 256,
		bordervalueWeavingAngles             = 257,
		bordervalueWeavingBraid              = 258,
		bordervalueWeavingRibbon             = 259,
		bordervalueWeavingStrips             = 260,
		bordervalueWhiteFlowers              = 261,
		bordervalueWoodwork                  = 262,
		bordervalueXIllusions                = 263,
		bordervalueZanyTriangles             = 264,
		bordervalueZigZag                    = 265,
		bordervalueZigZagStitch              = 266
	};

	DEFINE_SIMPLE_TYPE_START(CBorder, EBorder, bordervalueNone)

	public:
		bool IsArtBorder () const;
		bool IsLineBorder() const;
		bool IsNoBorder  () const;
	};

	//--------------------------------------------------------------------------------
	// BrClear 17.18.3 (Part 1)
	//--------------------------------------------------------------------------------

	enum EBrClear
	{
		brclearAll   = 0,
		brclearLeft  = 1,
		brclearNone  = 2,
		brclearRight = 3
	};

	DEFINE_SIMPLE_TYPE(CBrClear, EBrClear, brclearAll)

	//--------------------------------------------------------------------------------
	// BrType 17.18.4 (Part 1)
	//--------------------------------------------------------------------------------

	enum EBrType
	{
		brtypeColumn       = 0,
		brtypePage         = 1,
		brtypeTextWrapping = 2
	};

	DEFINE_SIMPLE_TYPE(CBrType, EBrType, brtypePage)

	//--------------------------------------------------------------------------------
	// CaptionPos 17.18.5 (Part 1)
	//--------------------------------------------------------------------------------

	enum ECaptionPos
	{
		captionposAbove = 0,
		captionposBelow = 1,
		captionposLeft  = 2,
		captionposRight = 3
	};

	DEFINE_SIMPLE_TYPE(CCaptionPos, ECaptionPos, captionposAbove)

	//--------------------------------------------------------------------------------
	// ChapterSep 17.18.6 (Part 1)
	//--------------------------------------------------------------------------------

	enum EChapterSep
	{
		chaptersepColon  = 0,
		chaptersepEmDash = 1,
		chaptersepEnDash = 2,
		chaptersepHyphen = 3,
		chaptersepPeriod = 4
	};

	DEFINE_SIMPLE_TYPE(CChapterSep, EChapterSep, chaptersepColon)

	//--------------------------------------------------------------------------------
	// CharacterSpacing 17.18.7 (Part 1)
	//--------------------------------------------------------------------------------

	enum ECharacterSpacing
	{
		charspacingCompressPunctuation                = 0,
		charspacingCompressPunctuationAndJapaneseKana = 1,
		charspacingDoNotCompress                      = 2
	};

	DEFINE_SIMPLE_TYPE(CCharacterSpacing, ECharacterSpacing, charspacingDoNotCompress)

	//--------------------------------------------------------------------------------
	// CombineBrackets 17.18.8 (Part 1)
	//--------------------------------------------------------------------------------

	enum ECombineBrackets
	{
		combinebracketsAngle  = 0,
		combinebracketsCurly  = 1,
		combinebracketsNone   = 2,
		combinebracketsRound  = 3,
		combinebracketsSquare = 4
	};

	DEFINE_SIMPLE_TYPE(CCombineBrackets, ECombineBrackets, combinebracketsNone)

	//--------------------------------------------------------------------------------
	// DateTime 17.18.9 (Part 1)
	//--------------------------------------------------------------------------------

	class CDateTime
	{
	public:
		CDateTime();

		std::wstring GetValue() const;
		void    SetValue(const std::wstring &sValue);

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CDateTime)

	private:
		std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// DecimalNumberOrPercent 17.18.11 (Part 1)
	//--------------------------------------------------------------------------------

	class CDecimalNumberOrPercent
	{
	public:
		CDecimalNumberOrPercent();
		double GetValue() const;

		int GetValueIntegerPercent() const;
		void   SetValue(double dValue);
		double  FromString(const std::wstring &sValue, double dDefValue = 0);
		std::wstring ToString  () const;

		SimpleTypes_DefaultString(CDecimalNumberOrPercent)

		bool IsPercent() const;
		void SetPercent(bool bPercent);

	private:
		double m_dValue;
		bool   m_bTrailingPercentSign; // Есть ли знак процента в конце?
	};

	//--------------------------------------------------------------------------------
	// Direction 17.18.12 (Part 1)
	//--------------------------------------------------------------------------------

	enum EDirection
	{
		directionLTR = 0,
		directionRTL = 1
	};

	DEFINE_SIMPLE_TYPE(CDirection, EDirection, directionLTR)

	//--------------------------------------------------------------------------------
	// DisplacedByCustomXml 17.18.13 (Part 1)
	//--------------------------------------------------------------------------------

	enum EDisplacedByCustomXml
	{
		displacedbycustomxmlNext = 0,
		displacedbycustomxmlPrev = 1
	};

	DEFINE_SIMPLE_TYPE(CDisplacedByCustomXml, EDisplacedByCustomXml, displacedbycustomxmlNext)

	//--------------------------------------------------------------------------------
	// DocGrid 17.18.14 (Part 1)
	//--------------------------------------------------------------------------------

	enum EDocGrid
	{
		docgridDefault       = 0,
		docgridLines         = 1,
		docgridLinesAndChars = 2,
		docgridSnapToChars   = 3,
	};

	DEFINE_SIMPLE_TYPE(CDocGrid, EDocGrid, docgridDefault)

	//--------------------------------------------------------------------------------
	// DocPartBehavior 17.18.15 (Part 1)
	//--------------------------------------------------------------------------------

	enum EDocPartBehavior
	{
		docpartbehaviorContent = 0,
		docpartbehaviorP       = 1,
		docpartbehaviorPg      = 2
	};

	DEFINE_SIMPLE_TYPE(CDocPartBehavior, EDocPartBehavior, docpartbehaviorContent)

	//--------------------------------------------------------------------------------
	// DocPartGallery 17.18.16 (Part 1)
	//--------------------------------------------------------------------------------

	enum EDocPartGallery
	{
		docpartgalleryAny               =  0,
		docpartgalleryAutoTxt           =  1,
		docpartgalleryBib               =  2,
		docpartgalleryCoverPg           =  3,
		docpartgalleryCustAutoTxt       =  4,
		docpartgalleryCustBib           =  5,
		docpartgalleryCustCoverPg       =  6,
		docpartgalleryCustEq            =  7,
		docpartgalleryCustFtrs          =  8,
		docpartgalleryCustHdrs          =  9,
		docpartgalleryCustom1           = 10,
		docpartgalleryCustom2           = 11,
		docpartgalleryCustom3           = 12,
		docpartgalleryCustom4           = 13,
		docpartgalleryCustom5           = 14,
		docpartgalleryCustPgNum         = 15,
		docpartgalleryCustPgNumB        = 16,
		docpartgalleryCustPgNumMargins  = 17,
		docpartgalleryCustPgNumT        = 18,
		docpartgalleryCustQuickParts    = 19,
		docpartgalleryCustTblOfContents = 20,
		docpartgalleryCustTbls          = 21,
		docpartgalleryCustTxtBox        = 22,
		docpartgalleryCustWatermarks    = 23,
		docpartgalleryDefault           = 24,
		docpartgalleryDocParts          = 25,
		docpartgalleryEq                = 26,
		docpartgalleryFtrs              = 27,
		docpartgalleryHdrs              = 28,
		docpartgalleryPgNum             = 29,
		docpartgalleryPgNumB            = 30,
		docpartgalleryPgNumMargins      = 31,
		docpartgalleryPgNumT            = 32,
		docpartgalleryPlaceHolder       = 33,
		docpartgalleryTblOfContents     = 34,
		docpartgalleryTbls              = 35,
		docpartgalleryTxtBox            = 36,
		docpartgalleryWatermarks        = 37
	};

	DEFINE_SIMPLE_TYPE(CDocPartGallery, EDocPartGallery, docpartgalleryDefault)

	//--------------------------------------------------------------------------------
	// DocPartType 17.18.17 (Part 1)
	//--------------------------------------------------------------------------------

	enum EDocPartType
	{
		docparttypeAutoExp  = 0,
		docparttypeBBPlcHdr = 1,
		docparttypeFormFld  = 2,
		docparttypeNone     = 3,
		docparttypeNormal   = 4,
		docparttypeSpeller  = 5,
		docparttypeToolbar  = 6
	};

	DEFINE_SIMPLE_TYPE(CDocPartType, EDocPartType, docparttypeNone)

	//--------------------------------------------------------------------------------
	// DocProtect 17.18.18 (Part 1)
	//--------------------------------------------------------------------------------

	enum EDocProtect
	{
		docprotectComments       = 0,
		docprotectForms          = 1,
		docprotectNone           = 2,
		docprotectReadOnly       = 3,
		docprotectTrackedChanges = 4
	};

	DEFINE_SIMPLE_TYPE(CDocProtect, EDocProtect, docprotectNone)

	//--------------------------------------------------------------------------------
	// DocType 17.18.19 (Part 1)
	//--------------------------------------------------------------------------------

	enum EDocType
	{
		doctypeEMail        = 0,
		doctypeLetter       = 1,
		doctypeNotSpecified = 2
	};

	DEFINE_SIMPLE_TYPE(CDocType, EDocType, doctypeNotSpecified)

	//--------------------------------------------------------------------------------
	// DropCap 17.18.20 (Part 1)
	//--------------------------------------------------------------------------------

	enum EDropCap
	{
		dropcapNone   = 0,
		dropcapDrop   = 1,
		dropcapMargin = 2
	};

	DEFINE_SIMPLE_TYPE(CDropCap, EDropCap, dropcapNone)

	//--------------------------------------------------------------------------------
	// EdGrp 17.18.21 (Part 1)
	//--------------------------------------------------------------------------------

	enum EEdGrp
	{
		edgrpAdministrators = 0,
		edgrpContributors   = 1,
		edgrpCurrent        = 2,
		edgrpEditors        = 3,
		edgrpEveryone       = 4,
		edgrpNone           = 5,
		edgrpOwners         = 6
	};

	DEFINE_SIMPLE_TYPE(CEdGrp, EEdGrp, edgrpEveryone)

	//--------------------------------------------------------------------------------
	// EdnPos 17.18.22 (Part 1)
	//--------------------------------------------------------------------------------

	enum EEdnPos
	{
		ednposDocEnd  = 0,
		ednposSectEnd = 1
	};

	DEFINE_SIMPLE_TYPE(CEdnPos, EEdnPos, ednposSectEnd)

	//--------------------------------------------------------------------------------
	// EighthPointMeasure 17.18.23 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CEighthPointMeasure, int, 0)
	public:
		double ToPoints();
		double ToMM();
		double ToInches();

		int    FromPoints(double dValue);
		int    FromInches(double dValue);
	};

	//--------------------------------------------------------------------------------
	// Em 17.18.24 (Part 1)
	//--------------------------------------------------------------------------------

	enum EEm
	{
		emCircle   = 0,
		emComma    = 1,
		emDot      = 2,
		emNone     = 3,
		emUnderDot = 4
	};

	DEFINE_SIMPLE_TYPE(CEm, EEm, emNone)

	//--------------------------------------------------------------------------------
	// FFHelpTextVal 17.18.25 (Part 1)
	//--------------------------------------------------------------------------------

	class CFFHelpTextVal
	{
	public:
		CFFHelpTextVal();

		std::wstring GetValue() const;
		void    SetValue(std::wstring &sValue);
		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;
		std::wstring ToXmlString() const;

		SimpleTypes_DefaultS(CFFHelpTextVal)

	private:
		std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// FFName 17.18.26 (Part 1)
	//--------------------------------------------------------------------------------

	class CFFName
	{
	public:
		CFFName();

		std::wstring GetValue() const;
		void    SetValue(std::wstring &sValue);

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;
		std::wstring ToXmlString() const;
		
		SimpleTypes_DefaultS(CFFName)

	private:
		std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// FFStatusTextVal 17.18.27 (Part 1)
	//--------------------------------------------------------------------------------

	class CFFStatusTextVal
	{
	public:
		CFFStatusTextVal();
		std::wstring GetValue() const;
		void SetValue(std::wstring &sValue);

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString () const;
		std::wstring ToXmlString() const;

		SimpleTypes_DefaultS(CFFStatusTextVal)
	private:
		std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// FFTextType 17.18.28 (Part 1)
	//--------------------------------------------------------------------------------

	enum EFFTextType
	{
		fftexttypeCalculated  = 0,
		fftexttypeCurrentDate = 1,
		fftexttypeCurrentTime = 2,
		fftexttypeDate        = 3,
		fftexttypeNumber      = 4,
		fftexttypeRegular     = 5
	};

	DEFINE_SIMPLE_TYPE(CFFTextType, EFFTextType, fftexttypeRegular)

	//--------------------------------------------------------------------------------
	// FldCharType 17.18.29 (Part 1)
	//--------------------------------------------------------------------------------

	enum EFldCharType
	{
		fldchartypeBegin    = 0,
		fldchartypeSeparate = 1,
		fldchartypeEnd      = 2
	};

	DEFINE_SIMPLE_TYPE(CFldCharType, EFldCharType, fldchartypeBegin)

	//--------------------------------------------------------------------------------
	// FontFamily 17.18.30 (Part 1)
	//--------------------------------------------------------------------------------

	enum EFontFamily
	{
		fontfamilyAuto       = 0,
		fontfamilyDecorative = 1,
		fontfamilyModern     = 2,
		fontfamilyRoman      = 3,
		fontfamilyScript     = 4,
		fontfamilySwiss      = 5
	};

	DEFINE_SIMPLE_TYPE(CFontFamily, EFontFamily, fontfamilyAuto)

	//--------------------------------------------------------------------------------
	// FrameLayout 17.18.31 (Part 1)
	//--------------------------------------------------------------------------------

	enum EFrameLayout
	{
		framelayoutCols = 0,
		framelayoutNone = 1,
		framelayoutRows = 2
	};

	DEFINE_SIMPLE_TYPE(CFrameLayout, EFrameLayout, framelayoutNone)

	//--------------------------------------------------------------------------------
	// FrameScrollbar 17.18.32 (Part 1)
	//--------------------------------------------------------------------------------

	enum EFrameScrollbar
	{
		framescrollbarAuto = 0,
		framescrollbarOff  = 1,
		framescrollbarOn   = 2
	};

	DEFINE_SIMPLE_TYPE(CFrameScrollbar, EFrameScrollbar, framescrollbarAuto)

	//--------------------------------------------------------------------------------
	// FtnEdn 17.18.33 (Part 1)
	//--------------------------------------------------------------------------------

	enum EFtnEdn
	{
		ftnednContinuationNotice    = 0,
		ftnednContinuationSeparator = 1,
		ftnednNormal                = 2,
		ftnednSeparator             = 3
	};

	DEFINE_SIMPLE_TYPE(CFtnEdn, EFtnEdn, ftnednNormal)

	//--------------------------------------------------------------------------------
	// FtnPos 17.18.34 (Part 1)
	//--------------------------------------------------------------------------------

	enum EFtnPos
	{
		ftnposBeneathText = 0,
		ftnposDocEnd      = 1,
		ftnposPageBottom  = 2,
		ftnposSectEnd     = 3
	};

	DEFINE_SIMPLE_TYPE(CFtnPos, EFtnPos, ftnposSectEnd)

	//--------------------------------------------------------------------------------
	// HAnchor 17.18.35 (Part 1)
	//--------------------------------------------------------------------------------

	enum EHAnchor
	{
		hanchorMargin = 0,
		hanchorPage   = 1,
		hanchorText   = 2
	};

	DEFINE_SIMPLE_TYPE(CHAnchor, EHAnchor, hanchorText)

	//--------------------------------------------------------------------------------
	// HdrFtr 17.18.36 (Part 1)
	//--------------------------------------------------------------------------------

	enum EHdrFtr
	{
		hdrftrDefault = 0,
		hdrftrEven    = 1,
		hdrftrFirst   = 2
	};

	DEFINE_SIMPLE_TYPE(CHdrFtr, EHdrFtr, hdrftrDefault)

	//--------------------------------------------------------------------------------
	// HeightRule 17.18.37 (Part 1)
	//--------------------------------------------------------------------------------

	enum EHeightRule
	{
		heightruleAtLeast = 0,
		heightruleAuto    = 1,
		heightruleExact   = 2
	};

	DEFINE_SIMPLE_TYPE(CHeightRule, EHeightRule, heightruleAuto)

	//--------------------------------------------------------------------------------
	// TextFormFormatType
	//--------------------------------------------------------------------------------

	enum ETextFormFormatType
	{
		textFormFormatTypeNone   = 0,
		textFormFormatTypeDigit  = 1,
		textFormFormatTypeLetter = 2,
		textFormFormatTypeMask   = 3,
		textFormFormatTypeRegExp = 4
	};

	DEFINE_SIMPLE_TYPE(CTextFormFormatType, ETextFormFormatType, textFormFormatTypeNone)

	//--------------------------------------------------------------------------------
	// ComplexFormType
	//--------------------------------------------------------------------------------

	enum EComplexFormType
	{
		complexFormTypeCustom    = 0,
		complexFormTypeTelephone = 1,
		complexFormTypeEmail     = 2
	};	

	DEFINE_SIMPLE_TYPE_START(CComplexFormType, EComplexFormType, complexFormTypeCustom)
	public:
		bool IsDefaultValue() const;
	};

	//--------------------------------------------------------------------------------
	// HexColor 17.18.38 (Part 1)
	//--------------------------------------------------------------------------------

	enum EHexColor
	{
		hexcolorAuto = 0,
		hexcolorRGB  = 1
	};

	//--------------------------------------------------------------------------------
	// HighlightColor 17.18.40 (Part 1)
	//--------------------------------------------------------------------------------

	enum EHighlightColor
	{
		highlightcolorBlack       =  0,
		highlightcolorBlue        =  1,
		highlightcolorCyan        =  2,
		highlightcolorDarkBlue    =  3,
		highlightcolorDarkCyan    =  4,
		highlightcolorDarkGray    =  5,
		highlightcolorDarkGreen   =  6,
		highlightcolorDarkMagenta =  7,
		highlightcolorDarkRed     =  8,
		highlightcolorDarkYellow  =  9,
		highlightcolorGreen       = 10,
		highlightcolorLightGray   = 11,
		highlightcolorMagenta     = 12,
		highlightcolorNone        = 13,
		highlightcolorRed         = 14,
		highlightcolorWhite       = 15,
		highlightcolorYellow      = 16
	};	

	DEFINE_SIMPLE_TYPE_START(CHexColor, EHexColor, hexcolorAuto)
	public:
		CHexColor(unsigned char r, unsigned char g, unsigned char b);

		std::wstring   ToStringNoAlpha  () const;

		void Set_R(unsigned char R);
		void Set_G(unsigned char G);
		void Set_B(unsigned char B);
		unsigned char Get_R() const;
		unsigned char Get_G() const;
		unsigned char Get_B() const;
		unsigned char Get_A() const;

	private:
		void Parse();
		void Parse3();
		int	HexToInt(int nHex);

	private:
		std::wstring m_sValue;

		unsigned char m_unR = 0;
		unsigned char m_unG = 0;
		unsigned char m_unB = 0;
	};

	//--------------------------------------------------------------------------------
	// HexColorAuto 17.18.39 (Part 1)
	//--------------------------------------------------------------------------------

	enum EHexColorAuto
	{
		hexcolorautoAuto = 0
	};

	DEFINE_SIMPLE_TYPE(CHexColorAuto, EHexColorAuto, hexcolorautoAuto)

	//--------------------------------------------------------------------------------
	// HighlightColor 17.18.40 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CHighlightColor, EHighlightColor, highlightcolorNone)
	public:
		CHighlightColor(const CHexColor& color);

		unsigned char Get_R() const;
		unsigned char Get_G() const;
		unsigned char Get_B() const;
		unsigned char Get_A() const;

	private:
		unsigned char m_unR = 0;
		unsigned char m_unG = 0;
		unsigned char m_unB = 0;
		unsigned char m_unA = 255;
	};

	//--------------------------------------------------------------------------------
	// Hint 17.18.41 (Part 1)
	//--------------------------------------------------------------------------------

	enum EHint
	{
		hintCs       = 0,
		hintDefault  = 1,
		hintEastAsia = 2
	};

	DEFINE_SIMPLE_TYPE(CHint, EHint, hintDefault)

	//--------------------------------------------------------------------------------
	// HpsMeasure 17.18.42 (Part 1)
	//--------------------------------------------------------------------------------

	class CHpsMeasure : public CUniversalMeasure
	{
	public:
		CHpsMeasure();

		virtual double  FromString(const std::wstring &sValue);
		virtual void SetValue(double dValue);
		virtual std::wstring ToString  () const;
		long ToHps() const;
		virtual double FromPoints(double dValue);
		virtual double FromInches(double dValue);
		virtual double FromHps(double dValue);

		SimpleTypes_DefaultD(CHpsMeasure)
	};

	//--------------------------------------------------------------------------------
	// InfoTextType 17.18.43 (Part 1)
	//--------------------------------------------------------------------------------

	enum EInfoTextType
	{
		infotexttypeAutoText = 0,
		infotexttypeText     = 1
	};

	DEFINE_SIMPLE_TYPE(CInfoTextType, EInfoTextType, infotexttypeText)

	//--------------------------------------------------------------------------------
	// Jc 17.18.44 (Part 1) + 9.10.2 (Part 4)
	//--------------------------------------------------------------------------------

	enum EJc
	{
		jcBoth            =  0,
		jcCenter          =  1,
		jcDistribute      =  2,
		jcEnd             =  3,
		jcHighKashida     =  4,
		jcLowKashida      =  5,
		jcMediumKashida   =  6,
		jcNumTab          =  7,
		jcStart           =  8,
		jcThaiDistribute  =  9,
		jcLeft            = 10,
		jcRight           = 11
	};

	DEFINE_SIMPLE_TYPE(CJc, EJc, jcLeft)

	//--------------------------------------------------------------------------------
	// JcTable 17.18.45 (Part 1) + 9.10.3 (Part 4)
	//--------------------------------------------------------------------------------

	enum EJcTable
	{
		jctableCenter = 0,
		jctableEnd    = 1,
		jctableStart  = 2,
		jctableLeft   = 3,
		jctableRight  = 4
	};

	DEFINE_SIMPLE_TYPE(CJcTable, EJcTable, jctableLeft)

	//--------------------------------------------------------------------------------
	// LevelSuffix 17.18.46 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELevelSuffix
	{
		levelsuffixNothing = 0,
		levelsuffixSpace   = 1,
		levelsuffixTab     = 2
	};

	DEFINE_SIMPLE_TYPE(CLevelSuffix, ELevelSuffix, levelsuffixNothing)

	//--------------------------------------------------------------------------------
	// LineNumberRestart 17.18.47 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELineNumberRestart
	{
		linenumberrestartContinious = 1,
		linenumberrestartNewPage    = 2,
		linenumberrestartNewSection = 3
	};

	DEFINE_SIMPLE_TYPE(CLineNumberRestart, ELineNumberRestart, linenumberrestartNewPage)

	//--------------------------------------------------------------------------------
	// LineSpacingRule 17.18.48 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELineSpacingRule
	{
		linespacingruleAtLeast = 0,
		linespacingruleAuto    = 1,
		linespacingruleExact   = 2
	};

	DEFINE_SIMPLE_TYPE(CLineSpacingRule, ELineSpacingRule, linespacingruleAuto)

	//--------------------------------------------------------------------------------
	// Lock 17.18.49 (Part 1)
	//--------------------------------------------------------------------------------

	enum ELock
	{
		lockContentLocked    = 0,
		lockSdtContentLocked = 1,
		lockSdtLocked        = 2,
		lockUnlocked         = 3
	};

	DEFINE_SIMPLE_TYPE(CLock, ELock, lockUnlocked)

	//--------------------------------------------------------------------------------
	// LongHexNumber 17.18.50 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CLongHexNumber, unsigned int, 0)
	private:
		bool Parse(const std::wstring &sValue);
		int	HexToInt(int nHex, bool &bResult);
	};

	//--------------------------------------------------------------------------------
	// MacroName 17.18.51 (Part 1)
	//--------------------------------------------------------------------------------

	class CMacroName
	{
	public:
		CMacroName();
		std::wstring GetValue() const;

		void    SetValue(std::wstring &sValue);
		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CMacroName)

	private:
		std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// MailMergeDataType 17.18.52 (Part 1)
	//--------------------------------------------------------------------------------

	enum EMailMergeDataType
	{
		mailmergeddatatypeNative      = 0,
		mailmergeddatatypeODBC        = 1,
		mailmergeddatatypeQuery       = 2,
		mailmergeddatatypeSoap        = 3,
		mailmergeddatatypeSpreadsheet = 4,
		mailmergeddatatypeTextFile    = 5,
		mailmergeddatatypeXQuery      = 6,
		mailmergeddatatypeXmlFile     = 7
	};

	DEFINE_SIMPLE_TYPE(CMailMergeDataType, EMailMergeDataType, mailmergeddatatypeTextFile)

	//--------------------------------------------------------------------------------
	// MailMergeDest 17.18.53 (Part 1)
	//--------------------------------------------------------------------------------

	enum EMailMergeDest
	{
		mailmergeddestEmail       = 0,
		mailmergeddestFax         = 1,
		mailmergeddestNewDocument = 2,
		mailmergeddestPrinter     = 3
	};

	DEFINE_SIMPLE_TYPE(CMailMergeDest, EMailMergeDest, mailmergeddestEmail)

	//--------------------------------------------------------------------------------
	// MailMergeDocType 17.18.54 (Part 1)
	//--------------------------------------------------------------------------------

	enum EMailMergeDocType
	{
		mailmergeddoctypeCatalog       = 0,
		mailmergeddoctypeEmail         = 1,
		mailmergeddoctypeEnvelopes     = 2,
		mailmergeddoctypeFax           = 3,
		mailmergeddoctypeFormLetters   = 4,
		mailmergeddoctypeMailingLabels = 5
	};

	DEFINE_SIMPLE_TYPE(CMailMergeDocType, EMailMergeDocType, mailmergeddoctypeEmail)

	//--------------------------------------------------------------------------------
	// MailMergeOdsoFMDFieldType 17.18.55 (Part 1)
	//--------------------------------------------------------------------------------

	enum EMailMergeOdsoFMDFieldType
	{
		mailmergeodsofmdfieldtypeDbColumn = 0,
		mailmergeodsofmdfieldtypeNull     = 1
	};

	DEFINE_SIMPLE_TYPE(CMailMergeOdsoFMDFieldType, EMailMergeOdsoFMDFieldType, mailmergeodsofmdfieldtypeNull)

	//--------------------------------------------------------------------------------
	// MailMergeSourceType 17.18.56 (Part 1)
	//--------------------------------------------------------------------------------

	enum EMailMergeSourceType
	{
		mailmergesourcetypeAddressBook = 0,
		mailmergesourcetypeDatabase    = 1,
		mailmergesourcetypeDocument1   = 2,
		mailmergesourcetypeDocument2   = 3,
		mailmergesourcetypeEmail       = 4,
		mailmergesourcetypeLegacy      = 5,
		mailmergesourcetypeMaster      = 6,
		mailmergesourcetypeNative      = 7,
		mailmergesourcetypeText        = 8,
	};

	DEFINE_SIMPLE_TYPE(CMailMergeSourceType, EMailMergeSourceType, mailmergesourcetypeText)

	//--------------------------------------------------------------------------------
	// Merge 17.18.57 (Part 1)
	//--------------------------------------------------------------------------------

	enum EMerge
	{
		mergeContinue = 0,
		mergeRestart  = 1
	};

	DEFINE_SIMPLE_TYPE(CMerge, EMerge, mergeContinue)

	//--------------------------------------------------------------------------------
	// MultiLevelType 17.18.58 (Part 1)
	//--------------------------------------------------------------------------------

	enum EMultiLevelType
	{
		emultileveltypeHybridMultilevel = 0,
		emultileveltypeMultilevel       = 1,
		emultileveltypeSingleLevel      = 2
	};

	DEFINE_SIMPLE_TYPE(CMultiLevelType, EMultiLevelType, emultileveltypeSingleLevel)

	//--------------------------------------------------------------------------------
	// NumberFormat 17.18.59 (Part 1)
	//--------------------------------------------------------------------------------

	enum ENumberFormat
	{
		numberformatAiueo                        =  0,
		numberformatAiueoFullWidth               =  1,
		numberformatArabicAbjad                  =  2,
		numberformatArabicAlpha                  =  3,
		numberformatBahtText                     =  4,
		numberformatBullet                       =  5,
		numberformatCardinalText                 =  6,
		numberformatChicago                      =  7,
		numberformatChineseCounting              =  8,
		numberformatChineseCountingThousand      =  9,
		numberformatChineseLegalSimplified       = 10,
		numberformatChosung                      = 11,
		numberformatCustom                       = 12,
		numberformatDecimal                      = 13,
		numberformatDecimalEnclosedCircle        = 14,
		numberformatDecimalEnclosedCircleChinese = 15,
		numberformatDecimalEnclosedFullstop      = 16,
		numberformatDecimalEnclosedParen         = 17,
		numberformatDecimalFullWidth             = 18,
		numberformatDecimalFullWidth2            = 19,
		numberformatDecimalHalfWidth             = 20,
		numberformatDecimalZero                  = 21,
		numberformatDollarText                   = 22,
		numberformatGanada                       = 23,
		numberformatHebrew1                      = 24,
		numberformatHebrew2                      = 25,
		numberformatHex                          = 26,
		numberformatHindiConsonants              = 27,
		numberformatHindiCounting                = 28,
		numberformatHindiNumbers                 = 29,
		numberformatHindiVowels                  = 30,
		numberformatIdeographDigital             = 31,
		numberformatIdeographEnclosedCircle      = 32,
		numberformatIdeographLegalTraditional    = 33,
		numberformatIdeographTraditional         = 34,
		numberformatIdeographZodiac              = 35,
		numberformatIdeographZodiacTraditional   = 36,
		numberformatIroha                        = 37,
		numberformatIrohaFullWidth               = 38,
		numberformatJapaneseCounting             = 39,
		numberformatJapaneseDigitalTenThousand   = 40,
		numberformatJapaneseLegal                = 41,
		numberformatKoreanCounting               = 42,
		numberformatKoreanDigital                = 43,
		numberformatKoreanDigital2               = 44,
		numberformatKoreanLegal                  = 45,
		numberformatLowerLetter                  = 46,
		numberformatLowerRoman                   = 47,
		numberformatNone                         = 48,
		numberformatNumberInDash                 = 49,
		numberformatOrdinal                      = 50,
		numberformatOrdinalText                  = 51,
		numberformatRussianLower                 = 52,
		numberformatRussianUpper                 = 53,
		numberformatTaiwaneseCounting            = 54,
		numberformatTaiwaneseCountingThousand    = 55,
		numberformatTaiwaneseDigital             = 56,
		numberformatThaiCounting                 = 57,
		numberformatThaiLetters                  = 58,
		numberformatThaiNumbers                  = 59,
		numberformatUpperLetter                  = 60,
		numberformatUpperRoman                   = 61,
		numberformatVietnameseCounting           = 62
	};

	DEFINE_SIMPLE_TYPE(CNumberFormat, ENumberFormat, numberformatNone)

	//--------------------------------------------------------------------------------
	// ObjectDrawAspect 17.18.60 (Part 1)
	//--------------------------------------------------------------------------------

	enum EObjectDrawAspect
	{
		objectdrawaspectContent = 0,
		objectdrawaspectIcon    = 1
	};

	DEFINE_SIMPLE_TYPE(CObjectDrawAspect, EObjectDrawAspect, objectdrawaspectContent)

	//--------------------------------------------------------------------------------
	// ObjectUpdateMode 17.18.61 (Part 1)
	//--------------------------------------------------------------------------------

	enum EObjectUpdateMode
	{
		objectupdatemodeAlways = 0,
		objectupdatemodeOnCall = 1
	};

	DEFINE_SIMPLE_TYPE(CObjectUpdateMode, EObjectUpdateMode, objectupdatemodeAlways)

	//--------------------------------------------------------------------------------
	// PageBorderDisplay 17.18.62 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPageBorderDisplay
	{
		pageborderdisplayAllPages     = 0,
		pageborderdisplayFirstPage    = 1,
		pageborderdisplayNotFirstPage = 2
	};

	DEFINE_SIMPLE_TYPE(CPageBorderDisplay, EPageBorderDisplay, pageborderdisplayAllPages)

	//--------------------------------------------------------------------------------
	// PageBorderOffset 17.18.63 (Part1)
	//--------------------------------------------------------------------------------

	enum EPageBorderOffset
	{
		pageborderoffsetPage = 0,
		pageborderoffsetText = 1
	};

	DEFINE_SIMPLE_TYPE(CPageBorderOffset, EPageBorderOffset, pageborderoffsetPage)

	//--------------------------------------------------------------------------------
	// PageBorderZOrder 17.18.64 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPageBorderZOrder
	{
		pageborderzorderBack  = 0,
		pageborderzorderFront = 1
	};

	DEFINE_SIMPLE_TYPE(CPageBorderZOrder, EPageBorderZOrder, pageborderzorderBack)

	//--------------------------------------------------------------------------------
	// PageOrientation 17.18.65 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPageOrientation
	{
		pageorientLandscape = 0,
		pageorientPortrait  = 1
	};

	DEFINE_SIMPLE_TYPE(CPageOrientation, EPageOrientation, pageorientPortrait)

	//--------------------------------------------------------------------------------
	// Pitch 17.18.66 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPitch
	{
		pitchDefault  = 0,
		pitchFixed    = 1,
		pitchVariable = 2
	};

	DEFINE_SIMPLE_TYPE(CPitch, EPitch, pitchDefault)

	//--------------------------------------------------------------------------------
	// PixelsMeasure 17.18.67 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE(CPixelsMeasure, int, 0)

	//--------------------------------------------------------------------------------
	// PointMeasure 17.18.68 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CPointMeasure, int, 0)
	public:
		double ToPoints() const;
		double ToMM() const;
		double ToInches() const;

		void FromPoints(double dValue);
		void FromInches(double dValue);
	};

	//--------------------------------------------------------------------------------
	// Proof 17.18.69 (Part 1)
	//--------------------------------------------------------------------------------

	enum EProof
	{
		proofClean = 0,
		proofDirty = 1
	};

	DEFINE_SIMPLE_TYPE(CProof, EProof, proofDirty)

	//--------------------------------------------------------------------------------
	// ProofErr 17.18.70 (Part 1)
	//--------------------------------------------------------------------------------

	enum EProofErr
	{
		prooferrGramEnd    = 0,
		prooferrGramStart  = 1,
		prooferrSpellEnd   = 2,
		prooferrSpellStart = 3
	};

	DEFINE_SIMPLE_TYPE(CProofErr, EProofErr, prooferrSpellStart)

	//--------------------------------------------------------------------------------
	// PTabAlignment 17.18.71 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPTabAlignment
	{
		ptabalignmentCenter = 0,
		ptabalignmentLeft   = 1,
		ptabalignmentRight  = 2
	};

	DEFINE_SIMPLE_TYPE(CPTabAlignment, EPTabAlignment, ptabalignmentLeft)

	//--------------------------------------------------------------------------------
	// PTabLeader 17.18.72 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPTabLeader
	{
		ptableaderDot        = 0,
		ptableaderHyphen     = 1,
		ptableaderMiddleDot  = 2,
		ptableaderNone       = 3,
		ptableaderUnderscore = 4
	};

	DEFINE_SIMPLE_TYPE(CPTabLeader, EPTabLeader, ptableaderNone)

	//--------------------------------------------------------------------------------
	// PTabRelativeTo 17.18.73 (Part 1)
	//--------------------------------------------------------------------------------

	enum EPTabRelativeTo
	{
		ptabrelativetoIndent = 0,
		ptabrelativetoMargin = 1
	};

	DEFINE_SIMPLE_TYPE(CPTabRelativeTo, EPTabRelativeTo, ptabrelativetoIndent)

	//--------------------------------------------------------------------------------
	// RestartNumber 17.18.74 (Part 1)
	//--------------------------------------------------------------------------------

	enum ERestartNumber
	{
		restartnumberContinious = 0,
		restartnumberEachSect   = 1,
		restartnumberEachPage   = 2
	};

	DEFINE_SIMPLE_TYPE(CRestartNumber, ERestartNumber, restartnumberContinious)

	//--------------------------------------------------------------------------------
	// RubyAlign 17.18.75 (Part 1)
	//--------------------------------------------------------------------------------

	enum ERubyAlign
	{
		erubyalignCenter           = 0,
		erubyalignDistributeLetter = 1,
		erubyalignDistributeSpace  = 2,
		erubyalignLeft             = 3,
		erubyalignRight            = 4,
		erubyalignRightVertical    = 5
	};

	DEFINE_SIMPLE_TYPE(CRubyAlign, ERubyAlign, erubyalignLeft)

	//--------------------------------------------------------------------------------
	// SdtDateMappingType 17.18.76 (Part 1)
	//--------------------------------------------------------------------------------

	enum ESdtDateMappingType
	{
		sdtdatemappingtypeDate     = 0,
		sdtdatemappingtypeDateTime = 1,
		sdtdatemappingtypeText     = 2
	};

	DEFINE_SIMPLE_TYPE(CSdtDateMappingType, ESdtDateMappingType, sdtdatemappingtypeText)

	//--------------------------------------------------------------------------------
	// SectionMark 17.18.77 (Part 1)
	//--------------------------------------------------------------------------------

	enum ESectionMark
	{
		sectionmarkContinious = 0,
		sectionmarkEvenPage   = 1,
		sectionmarkNextColumn = 2,
		sectionmarkNextPage   = 3,
		sectionmarkOddPage    = 4
	};

	DEFINE_SIMPLE_TYPE(CSectionMark, ESectionMark, sectionmarkNextPage)

	//--------------------------------------------------------------------------------
	// Shd 17.18.78 (Part 1)
	//--------------------------------------------------------------------------------

	enum EShd
	{
		shdClear                 =  0,
		shdNil                   =  1,
		shdDiagCross             =  2,
		shdDiagStripe            =  3,
		shdHorzCross             =  4,
		shdHorzStripe            =  5,
		shdPct10                 =  6,
		shdPct12                 =  7,
		shdPct15                 =  8,
		shdPct20                 =  9,
		shdPct25                 = 10,
		shdPct30                 = 11,
		shdPct35                 = 12,
		shdPct37                 = 13,
		shdPct40                 = 14,
		shdPct45                 = 15,
		shdPct5                  = 16,
		shdPct50                 = 17,
		shdPct55                 = 18,
		shdPct60                 = 19,
		shdPct62                 = 20,
		shdPct65                 = 21,
		shdPct70                 = 22,
		shdPct75                 = 23,
		shdPct80                 = 24,
		shdPct85                 = 25,
		shdPct87                 = 26,
		shdPct90                 = 27,
		shdPct95                 = 28,
		shdReverseDiagStripe     = 29,
		shdSolid                 = 30,
		shdThinDiagCross         = 31,
		shdThinDiagStripe        = 32,
		shdThinHorzCross         = 33,
		shdThinHorzStripe        = 34,
		shdThinReverseDiagStripe = 35,
		shdThinVertStripe        = 36,
		shdVertStripe            = 37
	};

	DEFINE_SIMPLE_TYPE(CShd, EShd, shdSolid)

	//--------------------------------------------------------------------------------
	// ShortHexNumber 17.18.79 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CShortHexNumber, int, 0)
	private:
		bool Parse(const std::wstring &sValue);
		int	HexToInt(int nHex, bool &bResult);
	};

	//--------------------------------------------------------------------------------
	// SignedHpsMeasure 17.18.80 (Part 1)
	//--------------------------------------------------------------------------------

	class CSignedHpsMeasure : public CUniversalMeasure
	{
	public:
		CSignedHpsMeasure();

		virtual double  FromString(const std::wstring &sValue);
		virtual void SetValue(double dValue);
		virtual std::wstring ToString () const;

		SimpleTypes_DefaultD(CSignedHpsMeasure)
	};

	//--------------------------------------------------------------------------------
	// SignedTwipsMeasure 17.18.81 (Part 1)
	//--------------------------------------------------------------------------------

	class CSignedTwipsMeasure : public CUniversalMeasure
	{
	public:
		CSignedTwipsMeasure();
		virtual void SetValue(double dValue);
		virtual double  FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

		SimpleTypes_DefaultD(CSignedTwipsMeasure)
	};

	//--------------------------------------------------------------------------------
	// StyleSort 17.18.82 (Part 1) + 9.10.4 (Part 4)
	//--------------------------------------------------------------------------------

	enum EStyleSort
	{
		stylesortBasedOn  = 4,
		stylesortDefault  = 2,
		stylesortFont     = 3,
		stylesortName     = 0,
		stylesortPriority = 1,
		stylesortType     = 5
	};

	DEFINE_SIMPLE_TYPE_START(CStyleSort, EStyleSort, stylesortName)
	public:
		void WriteNames(bool bNames);
	private:
		bool m_bNames;
	};

	//--------------------------------------------------------------------------------
	// StyleType 17.18.83 (Part 1)
	//--------------------------------------------------------------------------------

	enum EStyleType
	{
		styletypeCharacter = 0,
		styletypeNumbering = 1,
		styletypeParagraph = 2,
		styletypeTable     = 3
	};

	DEFINE_SIMPLE_TYPE(CStyleType, EStyleType, styletypeParagraph)

	//--------------------------------------------------------------------------------
	// TabJc 17.18.84 (Part 1) + 9.10.5 (Part 4)
	//--------------------------------------------------------------------------------

	enum ETabJc
	{
		tabjcBar     = 0,
		tabjcCenter  = 1,
		tabjcClear   = 2,
		tabjcDecimal = 3,
		tabjcEnd     = 4,
		tabjcNum     = 5,
		tabjcStart   = 6,
		tabjcRight   = 7,
		tabjcLeft    = 8
	};

	DEFINE_SIMPLE_TYPE(CTabJc, ETabJc, tabjcLeft)

	//--------------------------------------------------------------------------------
	// TabTlc 17.18.85 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETabTlc
	{
		tabtlcDot        = 0,
		tabtlcHeavy      = 1,
		tabtlcHyphen     = 2,
		tabtlcMiddleDot  = 3,
		tabtlcNone       = 4,
		tabtlcUnderscore = 5
	};

	DEFINE_SIMPLE_TYPE(CTabTlc, ETabTlc, tabtlcNone)

	//--------------------------------------------------------------------------------
	// TargetScreenSz 17.18.86 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETargetScreenSz
	{
		targetscreensz1024x768  =  1,
		targetscreensz1152x882  =  2,
		targetscreensz1152x900  =  3,
		targetscreensz1280x1024 =  4,
		targetscreensz1600x1200 =  5,
		targetscreensz1800x1440 =  6,
		targetscreensz1920x1200 =  7,
		targetscreensz544x376   =  8,
		targetscreensz640x480   =  9,
		targetscreensz720x512   = 10,
		targetscreensz800x600   = 11
	};

	DEFINE_SIMPLE_TYPE(CTargetScreenSz, ETargetScreenSz, targetscreensz1280x1024)

	//--------------------------------------------------------------------------------
	// TblLayoutType 17.18.87 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETblLayoutType
	{
		tbllayouttypeAutofit = 1,
		tbllayouttypeFixed   = 2
	};

	DEFINE_SIMPLE_TYPE(CTblLayoutType, ETblLayoutType, tbllayouttypeAutofit)

	//--------------------------------------------------------------------------------
	// TblOverlap 17.18.88 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETblOverlap
	{
		tbloverlapNever   = 1,
		tbloverlapOverlap = 2
	};

	DEFINE_SIMPLE_TYPE(CTblOverlap, ETblOverlap, tbloverlapOverlap)

	//--------------------------------------------------------------------------------
	// TblStyleOverrideType 17.18.89 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETblStyleOverrideType
	{
		tblstyleoverridetypeBand1Horz  =  0,
		tblstyleoverridetypeBand1Vert  =  1,
		tblstyleoverridetypeBand2Horz  =  2,
		tblstyleoverridetypeBand2Vert  =  3,
		tblstyleoverridetypeFirstCol   =  4,
		tblstyleoverridetypeFirstRow   =  5,
		tblstyleoverridetypeLastCol    =  6,
		tblstyleoverridetypeLastRow    =  7,
		tblstyleoverridetypeNeCell     =  8,
		tblstyleoverridetypeNwCell     =  9,
		tblstyleoverridetypeSeCell     = 10,
		tblstyleoverridetypeSwCell     = 11,
		tblstyleoverridetypeWholeTable = 12
	};

	DEFINE_SIMPLE_TYPE(CTblStyleOverrideType, ETblStyleOverrideType, tblstyleoverridetypeWholeTable)

	//--------------------------------------------------------------------------------
	// TblWidth 17.18.90 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETblWidth
	{
		tblwidthAuto = 0,
		tblwidthDxa  = 1,
		tblwidthNil  = 2,
		tblwidthPct  = 3
	};

	DEFINE_SIMPLE_TYPE(CTblWidth, ETblWidth, tblwidthAuto)

	//--------------------------------------------------------------------------------
	// TextAlignment 17.18.91 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextAlignment
	{
		textalignAuto     = 0,
		textalignBaseLine = 1,
		textalignBottom   = 2,
		textalignCenter   = 3,
		textalignTop      = 4,
	};

	DEFINE_SIMPLE_TYPE(CTextAlignment, ETextAlignment, textalignAuto)

	//--------------------------------------------------------------------------------
	// TextboxTightWrap 17.18.92 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextboxTightWrap
	{
		textboxtightwrapAllLines         = 0,
		textboxtightwrapFirstAndLastLine = 1,
		textboxtightwrapFirstLineOnly    = 2,
		textboxtightwrapLastLineOnly     = 3,
		textboxtightwrapNone             = 4
	};

	DEFINE_SIMPLE_TYPE(CTextboxTightWrap, ETextboxTightWrap, textboxtightwrapNone)

	//--------------------------------------------------------------------------------
	// TextDirection 17.18.93 (Part 1) + 9.10.6 (Part 4)
	//--------------------------------------------------------------------------------

	enum ETextDirection
	{
		textdirectionTb   = 0,
		textdirectionRl   = 1,
		textdirectionLr   = 2,
		textdirectionTbV  = 3,
		textdirectionRlV  = 4,
		textdirectionLrV  = 5
	};

	DEFINE_SIMPLE_TYPE(CTextDirection, ETextDirection, textdirectionTb)

	//--------------------------------------------------------------------------------
	// TextEffect 17.18.94 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETextEffect
	{
		texteffectAntsBlack       = 0,
		texteffectAntsRed         = 1,
		texteffectBlinkBackground = 2,
		texteffectLights          = 3,
		texteffectNone            = 4,
		texteffectShimmer         = 5,
		texteffectSparkle         = 6
	};

	DEFINE_SIMPLE_TYPE(CTextEffect, ETextEffect, texteffectNone)

	//--------------------------------------------------------------------------------
	// TextScale 17.18.95 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_NSV(CTextScale, int, 100)

	//--------------------------------------------------------------------------------
	// Theme 17.18.96 (Part 1)
	//--------------------------------------------------------------------------------

	enum ETheme
	{
		themeMajorAscii    = 0,
		themeMajorBidi     = 1,
		themeMajorEastAsia = 2,
		themeMajorHAnsi    = 3,
		themeMinorAscii    = 4,
		themeMinorBidi     = 5,
		themeMinorEastAsia = 6,
		themeMinorHAnsi    = 7
	};

	DEFINE_SIMPLE_TYPE(CTheme, ETheme, themeMajorAscii)

	//--------------------------------------------------------------------------------
	// ThemeColor 17.18.97 (Part 1)
	//--------------------------------------------------------------------------------

	enum EThemeColor
	{
		themecolorAccent1           =  0,
		themecolorAccent2           =  1,
		themecolorAccent3           =  2,
		themecolorAccent4           =  3,
		themecolorAccent5           =  4,
		themecolorAccent6           =  5,
		themecolorBackground1       =  6,
		themecolorBackground2       =  7,
		themecolorDark1             =  8,
		themecolorDark2             =  9,
		themecolorFollowedHyperlink = 10,
		themecolorHyperlink         = 11,
		themecolorLight1            = 12,
		themecolorLight2            = 13,
		themecolorNone              = 14,
		themecolorText1             = 15,
		themecolorText2             = 16
	};

	DEFINE_SIMPLE_TYPE(CThemeColor, EThemeColor, themecolorNone)

	//--------------------------------------------------------------------------------
	// UcharHexNumber 17.18.98 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CUcharHexNumber, unsigned char, 0)
	private:
		bool Parse(const std::wstring &sValue);
		int	HexToInt(int nHex, bool &bResult);
	};

	//--------------------------------------------------------------------------------
	// Underline 17.18.99 (Part 1) 20.1.10.82
	//--------------------------------------------------------------------------------

	enum EUnderline
	{
		underlineDash            =  0,
		underlineDashDotDotHeavy =  1,
		underlineDashDotHeavy    =  2,
		underlineDashedHeavy     =  3,
		underlineDashLong        =  4,
		underlineDashLongHeavy   =  5,
		underlineDotDash         =  6,
		underlineDotDotDash      =  7,
		underlineDotted          =  8,
		underlineDottedHeavy     =  9,
		underlineDouble          = 10,
		underlineNone            = 11,
		underlineSingle          = 12,
		underlineThick           = 13,
		underlineWave            = 14,
		underlineWavyDouble      = 15,
		underlineWavyHeavy       = 16,
		underlineWords           = 17
	};

	DEFINE_SIMPLE_TYPE(CUnderline, EUnderline, underlineNone)

	//--------------------------------------------------------------------------------
	// VAnchor 17.18.100 (Part 1)
	//--------------------------------------------------------------------------------

	enum EVAnchor
	{
		vanchorMargin = 0,
		vanchorPage   = 1,
		vanchorText   = 2
	};

	DEFINE_SIMPLE_TYPE(CVAnchor, EVAnchor, vanchorText)

	//--------------------------------------------------------------------------------
	// VerticalJc 17.18.101 (Part 1)
	//--------------------------------------------------------------------------------

	enum EVerticalJc
	{
		verticaljcBoth   = 0,
		verticaljcBottom = 1,
		verticaljcCenter = 2,
		verticaljcTop    = 3
	};

	DEFINE_SIMPLE_TYPE(CVerticalJc, EVerticalJc, verticaljcTop)

	//--------------------------------------------------------------------------------
	// View 17.18.102 (Part 1)
	//--------------------------------------------------------------------------------

	enum EView
	{
		viewMasterPages = 0,
		viewNone        = 1,
		viewNormal      = 2,
		viewOutline     = 3,
		viewPrint       = 4,
		viewWeb         = 5
	};

	DEFINE_SIMPLE_TYPE(CView, EView, viewNone)

	//--------------------------------------------------------------------------------
	// WmlColorSchemeIndex 17.18.103 (Part 1)
	//--------------------------------------------------------------------------------

	enum EWmlColorSchemeIndex
	{
		wmlcolorschemeindexAccent1           =  0,
		wmlcolorschemeindexAccent2           =  1,
		wmlcolorschemeindexAccent3           =  2,
		wmlcolorschemeindexAccent4           =  3,
		wmlcolorschemeindexAccent5           =  4,
		wmlcolorschemeindexAccent6           =  5,
		wmlcolorschemeindexDark1             =  6,
		wmlcolorschemeindexDark2             =  7,
		wmlcolorschemeindexFollowedHyperlink =  8,
		wmlcolorschemeindexHyperlink         =  9,
		wmlcolorschemeindexLight1            = 10,
		wmlcolorschemeindexLight2            = 11
	};

	DEFINE_SIMPLE_TYPE(CWmlColorSchemeIndex, EWmlColorSchemeIndex, wmlcolorschemeindexAccent1)

	//--------------------------------------------------------------------------------
	// Wrap 17.18.104 (Part 1)
	//--------------------------------------------------------------------------------

	enum EWrap
	{
		wrapAround    = 0,
		wrapAuto      = 1,
		wrapNone      = 2,
		wrapNotBeside = 3,
		wrapThrough   = 4,
		wrapTight     = 5
	};

	DEFINE_SIMPLE_TYPE(CWrap, EWrap, wrapAuto)

	//--------------------------------------------------------------------------------
	// Zoom 17.18.105 (Part 1)
	//--------------------------------------------------------------------------------

	enum EZoom
	{
		zoomBestFit  = 0,
		zoomFullPage = 1,
		zoomNone     = 2,
		zoomTextFit  = 3
	};	

	DEFINE_SIMPLE_TYPE(CZoom, EZoom, zoomNone)

}; // SimpleTypes

// Дополнительные типы
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// CFontCharset 9.4.1.1 (Part 4)
	//--------------------------------------------------------------------------------

	enum EFontCharset
	{
		fontcharsetANSI        =   0, // 00
		fontcharsetDefault     =   1, // 01
		fontcharsetSymbol      =   2, // 02
		fontcharsetMacintosh   =  77, // 4D
		fontcharsetShitJIS     = 128, // 80
		fontcharsetHangeul     = 129, // 81
		fontcharsetJohab       = 130, // 82
		fontcharsetGB2313      = 134, // 86
		fontcharsetChineseBig5 = 136, // 88
		fontcharsetGreek       = 161, // A1
		fontcharsetTurkish     = 162, // A2
		fontcharsetVietnamese  = 163, // A3
		fontcharsetHebrew      = 177, // B1
		fontcharsetArabic      = 178, // B2
		fontcharsetBaltic      = 186, // BA
		fontcharsetRussian     = 204, // CC
		fontcharsetThai        = 222, // DE
		fontcharsetEastEurope  = 238, // EE
		fontcharsetOEM         = 255, // FF
	};

	DEFINE_SIMPLE_TYPE(CFontCharset, EFontCharset, fontcharsetANSI)

	//--------------------------------------------------------------------------------
	// CCnf 9.10.7 (Part 4)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CCnf, unsigned short, 0)
	public:
		bool IsFirstRow       () const;
		bool IsLastRow        () const;
		bool IsFirstColumn    () const;
		bool IsLastColumn     () const;
		bool IsBand1Vertical  () const;
		bool IsBand2Vertical  () const;
		bool IsBand1Horizontal() const;
		bool IsBand2Horizontal() const;
		bool IsNeCell         () const;
		bool IsNwCell         () const;
		bool IsSeCell         () const;
		bool IsSwCell         () const;

	private:
		void Parse(const std::wstring &sValue);
	};

	//--------------------------------------------------------------------------------
	// XmlSpace
	//--------------------------------------------------------------------------------

	enum EXmlSpace
	{
		xmlspaceDefault  = 0,
		xmlspacePreserve = 1
	};

	DEFINE_SIMPLE_TYPE(CXmlSpace, EXmlSpace, xmlspaceDefault)

	//--------------------------------------------------------------------------------
	// CryptAlgClass
	//--------------------------------------------------------------------------------

	enum ECryptAlgClass
	{
		AlgClassCustom = 0,
		AlgClassHash = 1
	};

	DEFINE_SIMPLE_TYPE(CCryptAlgClass, ECryptAlgClass, AlgClassCustom)

	//--------------------------------------------------------------------------------
	// CryptAlgType
	//--------------------------------------------------------------------------------

	enum ECryptAlgType
	{
		AlgTypeCustom = 0,
		AlgTypeTypeAny = 1
	};

	DEFINE_SIMPLE_TYPE(CCryptAlgType, ECryptAlgType, AlgTypeCustom)

	//--------------------------------------------------------------------------------
	// CryptProv
	//--------------------------------------------------------------------------------

	enum ECryptProv
	{
		CryptProvCustom = 0,
		CryptProvRsaAES = 1,
		CryptProvRsaFull = 2
	};

	DEFINE_SIMPLE_TYPE(CCryptProv, ECryptProv, CryptProvCustom)

	//--------------------------------------------------------------------------------
	// CryptAlgoritmName
	//--------------------------------------------------------------------------------

	enum ECryptAlgoritmName
	{
		cryptalgoritmnameUnknown    = 0,
		cryptalgoritmnameMD2        = 1,
		cryptalgoritmnameMD4        = 2,
		cryptalgoritmnameMD5        = 3,
		cryptalgoritmnameRIPEMD128  = 4,
		cryptalgoritmnameRIPEMD160  = 5,
		cryptalgoritmnameSHA1       = 6,
		cryptalgoritmnameSHA256     = 7,
		cryptalgoritmnameSHA384     = 8,
		cryptalgoritmnameSHA512     = 9,
		cryptalgoritmnameWHIRLPOOL  = 10,
	};

	DEFINE_SIMPLE_TYPE(CCryptAlgoritmName, ECryptAlgoritmName, cryptalgoritmnameUnknown)

	//--------------------------------------------------------------------------------
	// PitchFamily
	//--------------------------------------------------------------------------------

	enum EPitchFamily
	{
		pitchfamilyDefUnk = 0x00,
		pitchfamilyFixUnk = 0x01,
		pitchfamilyVarUnk = 0x02,
		pitchfamilyDefRom = 0x10,
		pitchfamilyFixRom = 0x11,
		pitchfamilyVarRom = 0x12,
		pitchfamilyDefSwi = 0x20,
		pitchfamilyFixSwi = 0x21,
		pitchfamilyVarSwi = 0x22,
		pitchfamilyDefMod = 0x30,
		pitchfamilyFixMod = 0x31,
		pitchfamilyVarMod = 0x32,
		pitchfamilyDefScr = 0x40,
		pitchfamilyFixScr = 0x41,
		pitchfamilyVarScr = 0x42,
		pitchfamilyDefDec = 0x50,
		pitchfamilyFixDec = 0x51,
		pitchfamilyVarDec = 0x52,
	};

	DEFINE_SIMPLE_TYPE_START(CPitchFamily, EPitchFamily, pitchfamilyDefUnk)
	public:
		unsigned char GetPitch();
		unsigned char GetFamily();
	};

	//--------------------------------------------------------------------------------
	// DirVal
	//--------------------------------------------------------------------------------

	enum EDirVal
	{
		dirLtr    = 0,
		dirRtl    = 1
	};

	DEFINE_SIMPLE_TYPE(CDirVal, EDirVal, dirLtr)

	//--------------------------------------------------------------------------------
	// Ligatures
	//--------------------------------------------------------------------------------

	enum ELigaturesFlags
	{
		ligaturesFlagStandard     = (1 << 0),
		ligaturesFlagContextual   = (1 << 1),
		ligaturesFlagHistorical   = (1 << 2),
		ligaturesFlagDiscretional = (1 << 3)
	};
	
	enum ELigatures
	{
		ligatureNone                             = 0,
		ligatureStandard                         = ligaturesFlagStandard,
		ligatureContextual                       = ligaturesFlagContextual,
		ligatureHistorical                       = ligaturesFlagHistorical,
		ligatureDiscretional                     = ligaturesFlagDiscretional,
		ligatureStandardContextual               = ligaturesFlagStandard | ligaturesFlagContextual,
		ligatureStandardHistorical               = ligaturesFlagStandard | ligaturesFlagHistorical,
		ligatureContextualHistorical             = ligaturesFlagContextual | ligaturesFlagHistorical,
		ligatureStandardDiscretional             = ligaturesFlagStandard | ligaturesFlagDiscretional,
		ligatureContextualDiscretional           = ligaturesFlagContextual | ligaturesFlagDiscretional,
		ligatureHistoricalDiscretional           = ligaturesFlagHistorical | ligaturesFlagDiscretional,
		ligatureStandardContextualHistorical     = ligaturesFlagStandard | ligaturesFlagContextual | ligaturesFlagHistorical,
		ligatureStandardContextualDiscretional   = ligaturesFlagStandard | ligaturesFlagContextual | ligaturesFlagDiscretional,
		ligatureStandardHistoricalDiscretional   = ligaturesFlagStandard  | ligaturesFlagHistorical | ligaturesFlagDiscretional,
		ligatureContextualHistoricalDiscretional = ligaturesFlagContextual | ligaturesFlagHistorical | ligaturesFlagDiscretional,
		ligatureAll                              = ligaturesFlagStandard | ligaturesFlagContextual | ligaturesFlagHistorical | ligaturesFlagDiscretional
	};

	DEFINE_SIMPLE_TYPE(CLigatures, ELigatures, ligatureStandard)

	//--------------------------------------------------------------------------------
	// NumForm
	//--------------------------------------------------------------------------------

	enum ENumForm
	{
		numFormDefault = 0,
		numFormLining = 1,
		numFormOldStyle = 2
	};

	DEFINE_SIMPLE_TYPE(CNumForm, ENumForm, numFormDefault)

	//--------------------------------------------------------------------------------
	// NumSpacing
	//--------------------------------------------------------------------------------

	enum ENumSpacing
	{
		numSpacingDefault = 0,
		numSpacingProportional = 1,
		numSpacingTabular = 2
	};

	DEFINE_SIMPLE_TYPE(CNumSpacing, ENumSpacing, numSpacingDefault)

} // SimpleTypes
