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

	template<EAnnotationVMerge eDefValue = annotvmergeCont>
	class CAnnotationVMerge : public CSimpleType<EAnnotationVMerge, eDefValue>
	{
	public:

		CAnnotationVMerge() {} 

		virtual EAnnotationVMerge FromString(CString &sValue)
		{
            if       ( _T("cont") == sValue ) this->m_eValue = annotvmergeCont;
            else if  ( _T("rest") == sValue ) this->m_eValue = annotvmergeRest;
            else                              this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString() const 
		{
            switch(this->m_eValue)
			{
			case annotvmergeCont : return _T("cont");
			case annotvmergeRest : return _T("rest");
			default              : return _T("cont");
			}
		}

		SimpleType_FromString     (EAnnotationVMerge)
		SimpleType_Operator_Equal (CAnnotationVMerge)
	};


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


	template<EBorder eDefValue = bordervalueNone>
	class CBorder : public CSimpleType<EBorder, eDefValue>
	{
	public:

		CBorder() {}

		virtual EBorder FromString(CString &sValue)
		{
			wchar_t wsFirstChar = 0;

			if ( sValue.GetLength() > 0 )
				wsFirstChar = sValue[0];

			switch(wsFirstChar)
			{
			case 'a':
				{
                    if      ( _T("apples")                 == sValue ) this->m_eValue = bordervalueApples;
                    else if ( _T("archedScallops")         == sValue ) this->m_eValue = bordervalueArchedScallops;
                    else                                               this->m_eValue = eDefValue;
					break;
				}
			case 'b':
				{
                    if      ( _T("babyPacifier")           == sValue ) this->m_eValue = bordervalueBabyPacifier;
                    else if ( _T("babyRattle")             == sValue ) this->m_eValue = bordervalueBabyRattle;
                    else if ( _T("balloons3Colors")        == sValue ) this->m_eValue = bordervalueBalloons3Colors;
                    else if ( _T("balloonsHotAir")         == sValue ) this->m_eValue = bordervalueBalloonsHotAir;
                    else if ( _T("basicBlackDashes")       == sValue ) this->m_eValue = bordervalueBasicBlackDashes;
                    else if ( _T("basicBlackDots")         == sValue ) this->m_eValue = bordervalueBasicBlackDots;
                    else if ( _T("basicBlackSquares")      == sValue ) this->m_eValue = bordervalueBasicBlackSquares;
                    else if ( _T("basicThinLines")         == sValue ) this->m_eValue = bordervalueBasicThinLines;
                    else if ( _T("basicWhiteDashes")       == sValue ) this->m_eValue = bordervalueBasicWhiteDashes;
                    else if ( _T("basicWhiteDots")         == sValue ) this->m_eValue = bordervalueBasicWhiteDots;
                    else if ( _T("basicWhiteSquares")      == sValue ) this->m_eValue = bordervalueBasicWhiteSquares;
                    else if ( _T("basicWideInline")        == sValue ) this->m_eValue = bordervalueBasicWideInline;
                    else if ( _T("basicWideMidline")       == sValue ) this->m_eValue = bordervalueBasicWideMidline;
                    else if ( _T("basicWideOutline")       == sValue ) this->m_eValue = bordervalueBasicWideOutline;
                    else if ( _T("bats")                   == sValue ) this->m_eValue = bordervalueBats;
                    else if ( _T("birds")                  == sValue ) this->m_eValue = bordervalueBirds;
                    else if ( _T("birdsFlight")            == sValue ) this->m_eValue = bordervalueBirdsFlight;
                    else                                               this->m_eValue = eDefValue;
					break;
				}
			case 'c':
				{
                    if      ( _T("cabins")                 == sValue ) this->m_eValue = bordervalueCabins;
                    else if ( _T("cakeSlice")              == sValue ) this->m_eValue = bordervalueCakeSlice;
                    else if ( _T("candyCorn")              == sValue ) this->m_eValue = bordervalueCandyCorn;
                    else if ( _T("celticKnotwork")         == sValue ) this->m_eValue = bordervalueCelticKnotwork;
                    else if ( _T("certificateBanner")      == sValue ) this->m_eValue = bordervalueCertificateBanner;
                    else if ( _T("chainLink")              == sValue ) this->m_eValue = bordervalueChainLink;
                    else if ( _T("champagneBottle")        == sValue ) this->m_eValue = bordervalueChampagneBottle;
                    else if ( _T("checkedBarBlack")        == sValue ) this->m_eValue = bordervalueCheckedBarBlack;
                    else if ( _T("checkedBarColor")        == sValue ) this->m_eValue = bordervalueCheckedBarColor;
                    else if ( _T("checkered")              == sValue ) this->m_eValue = bordervalueCheckered;
                    else if ( _T("christmasTree")          == sValue ) this->m_eValue = bordervalueChristmasTree;
                    else if ( _T("circlesLines")           == sValue ) this->m_eValue = bordervalueCirclesLines;
                    else if ( _T("circlesRectangles")      == sValue ) this->m_eValue = bordervalueCirclesRectangles;
                    else if ( _T("classicalWave")          == sValue ) this->m_eValue = bordervalueClassicalWave;
                    else if ( _T("clocks")                 == sValue ) this->m_eValue = bordervalueClocks;
                    else if ( _T("compass")                == sValue ) this->m_eValue = bordervalueCompass;
                    else if ( _T("confetti")               == sValue ) this->m_eValue = bordervalueConfetti;
                    else if ( _T("confettiGrays")          == sValue ) this->m_eValue = bordervalueConfettiGrays;
                    else if ( _T("confettiOutline")        == sValue ) this->m_eValue = bordervalueConfettiOutline;
                    else if ( _T("confettiStreamers")      == sValue ) this->m_eValue = bordervalueConfettiStreamers;
                    else if ( _T("confettiWhite")          == sValue ) this->m_eValue = bordervalueConfettiWhite;
                    else if ( _T("cornerTriangles")        == sValue ) this->m_eValue = bordervalueCornerTriangles;
                    else if ( _T("couponCutoutDashes")     == sValue ) this->m_eValue = bordervalueCouponCutoutDashes;
                    else if ( _T("couponCutoutDots")       == sValue ) this->m_eValue = bordervalueCouponCutoutDots;
                    else if ( _T("crazyMaze")              == sValue ) this->m_eValue = bordervalueCrazyMaze;
                    else if ( _T("creaturesButterfly")     == sValue ) this->m_eValue = bordervalueCreaturesButterfly;
                    else if ( _T("creaturesFish")          == sValue ) this->m_eValue = bordervalueCreaturesFish;
                    else if ( _T("creaturesInsects")       == sValue ) this->m_eValue = bordervalueCreaturesInsects;
                    else if ( _T("creaturesLadyBug")       == sValue ) this->m_eValue = bordervalueCreaturesLadyBug;
                    else if ( _T("crossStitch")            == sValue ) this->m_eValue = bordervalueCrossStitch;
                    else if ( _T("cup")                    == sValue ) this->m_eValue = bordervalueCup;
                    else if ( _T("custom")                 == sValue ) this->m_eValue = bordervalueCustom;
                    else                                               this->m_eValue = eDefValue;
					break;
				}
			case 'd':
				{
                    if      ( _T("dashDotStroked")         == sValue ) this->m_eValue = bordervalueDashDotStroked;
                    else if ( _T("dashed")                 == sValue ) this->m_eValue = bordervalueDashed;
                    else if ( _T("dashSmallGap")           == sValue ) this->m_eValue = bordervalueDashSmallGap;
                    else if ( _T("decoArch")               == sValue ) this->m_eValue = bordervalueDecoArch;
                    else if ( _T("decoArchColor")          == sValue ) this->m_eValue = bordervalueDecoArchColor;
                    else if ( _T("decoBlocks")             == sValue ) this->m_eValue = bordervalueDecoBlocks;
                    else if ( _T("diamondsGray")           == sValue ) this->m_eValue = bordervalueDiamondsGray;
                    else if ( _T("dotDash")                == sValue ) this->m_eValue = bordervalueDotDash;
                    else if ( _T("dotDotDash")             == sValue ) this->m_eValue = bordervalueDotDotDash;
                    else if ( _T("dotted")                 == sValue ) this->m_eValue = bordervalueDotted;
                    else if ( _T("double")                 == sValue ) this->m_eValue = bordervalueDouble;
                    else if ( _T("doubleD")                == sValue ) this->m_eValue = bordervalueDoubleD;
                    else if ( _T("doubleDiamonds")         == sValue ) this->m_eValue = bordervalueDoubleDiamonds;
                    else if ( _T("doubleWave")             == sValue ) this->m_eValue = bordervalueDoubleWave;
                    else                                               this->m_eValue = eDefValue;
					break;
				}
			case 'e':
				{
                    if      ( _T("earth1")                 == sValue ) this->m_eValue = bordervalueEarth1;
                    else if ( _T("earth2")                 == sValue ) this->m_eValue = bordervalueEarth2;
                    else if ( _T("earth3")                 == sValue ) this->m_eValue = bordervalueEarth3;
                    else if ( _T("eclipsingSquares1")      == sValue ) this->m_eValue = bordervalueEclipsingSquares1;
                    else if ( _T("eclipsingSquares2")      == sValue ) this->m_eValue = bordervalueEclipsingSquares2;
                    else if ( _T("eggsBlack")              == sValue ) this->m_eValue = bordervalueEggsBlack;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'f':
				{
                    if      ( _T("fans")                   == sValue ) this->m_eValue = bordervalueFans;
                    else if ( _T("film")                   == sValue ) this->m_eValue = bordervalueFilm;
                    else if ( _T("firecrackers")           == sValue ) this->m_eValue = bordervalueFirecrackers;
                    else if ( _T("flowersBlockPrint")      == sValue ) this->m_eValue = bordervalueFlowersBlockPrint;
                    else if ( _T("flowersDaisies")         == sValue ) this->m_eValue = bordervalueFlowersDaisies;
                    else if ( _T("flowersModern1")         == sValue ) this->m_eValue = bordervalueFlowersModern1;
                    else if ( _T("flowersModern2")         == sValue ) this->m_eValue = bordervalueFlowersModern2;
                    else if ( _T("flowersPansy")           == sValue ) this->m_eValue = bordervalueFlowersPansy;
                    else if ( _T("flowersRedRose")         == sValue ) this->m_eValue = bordervalueFlowersRedRose;
                    else if ( _T("flowersRoses")           == sValue ) this->m_eValue = bordervalueFlowersRoses;
                    else if ( _T("flowersTeacup")          == sValue ) this->m_eValue = bordervalueFlowersTeacup;
                    else if ( _T("flowersTiny")            == sValue ) this->m_eValue = bordervalueFlowersTiny;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'g':
				{
                    if      ( _T("gems")                   == sValue ) this->m_eValue = bordervalueGems;
                    else if ( _T("gingerbreadMan")         == sValue ) this->m_eValue = bordervalueGingerbreadMan;
                    else if ( _T("gradient")               == sValue ) this->m_eValue = bordervalueGradient;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'h':
				{
                    if      ( _T("handmade1")              == sValue ) this->m_eValue = bordervalueHandmade1;
                    else if ( _T("handmade2")              == sValue ) this->m_eValue = bordervalueHandmade2;
                    else if ( _T("heartBalloon")           == sValue ) this->m_eValue = bordervalueHeartBalloon;
                    else if ( _T("heartGray")              == sValue ) this->m_eValue = bordervalueHeartGray;
                    else if ( _T("hearts")                 == sValue ) this->m_eValue = bordervalueHearts;
                    else if ( _T("heebieJeebies")          == sValue ) this->m_eValue = bordervalueHeebieJeebies;
                    else if ( _T("holly")                  == sValue ) this->m_eValue = bordervalueHolly;
                    else if ( _T("houseFunky")             == sValue ) this->m_eValue = bordervalueHouseFunky;
                    else if ( _T("hypnotic")               == sValue ) this->m_eValue = bordervalueHypnotic;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'i':
				{
                    if      ( _T("iceCreamCones")          == sValue ) this->m_eValue = bordervalueIceCreamCones;
                    else if ( _T("inset")                  == sValue ) this->m_eValue = bordervalueInset;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'l':
				{
                    if      ( _T("lightBulb")              == sValue ) this->m_eValue = bordervalueLightBulb;
                    else if ( _T("lightning1")             == sValue ) this->m_eValue = bordervalueLightning1;
                    else if ( _T("lightning2")             == sValue ) this->m_eValue = bordervalueLightning2;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'm':
				{
                    if      ( _T("mapleLeaf")              == sValue ) this->m_eValue = bordervalueMapleLeaf;
                    else if ( _T("mapleMuffins")           == sValue ) this->m_eValue = bordervalueMapleMuffins;
                    else if ( _T("mapPins")                == sValue ) this->m_eValue = bordervalueMapPins;
                    else if ( _T("marquee")                == sValue ) this->m_eValue = bordervalueMarquee;
                    else if ( _T("marqueeToothed")         == sValue ) this->m_eValue = bordervalueMarqueeToothed;
                    else if ( _T("moons")                  == sValue ) this->m_eValue = bordervalueMoons;
                    else if ( _T("mosaic")                 == sValue ) this->m_eValue = bordervalueMosaic;
                    else if ( _T("musicNotes")             == sValue ) this->m_eValue = bordervalueMusicNotes;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'n':
				{
                    if      ( _T("nil")                    == sValue ) this->m_eValue = bordervalueNil;
                    else if ( _T("none")                   == sValue ) this->m_eValue = bordervalueNone;
                    else if ( _T("northwest")              == sValue ) this->m_eValue = bordervalueNorthwest;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'o':
				{
                    if      ( _T("outset")                 == sValue ) this->m_eValue = bordervalueOutset;
                    else if ( _T("ovals")                  == sValue ) this->m_eValue = bordervalueOvals;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'p':
				{
                    if      ( _T("packages")               == sValue ) this->m_eValue = bordervaluePackages;
                    else if ( _T("palmsBlack")             == sValue ) this->m_eValue = bordervaluePalmsBlack;
                    else if ( _T("palmsColor")             == sValue ) this->m_eValue = bordervaluePalmsColor;
                    else if ( _T("paperClips")             == sValue ) this->m_eValue = bordervaluePaperClips;
                    else if ( _T("papyrus")                == sValue ) this->m_eValue = bordervaluePapyrus;
                    else if ( _T("partyFavor")             == sValue ) this->m_eValue = bordervaluePartyFavor;
                    else if ( _T("partyGlass")             == sValue ) this->m_eValue = bordervaluePartyGlass;
                    else if ( _T("pencils")                == sValue ) this->m_eValue = bordervaluePencils;
                    else if ( _T("people")                 == sValue ) this->m_eValue = bordervaluePeople;
                    else if ( _T("peopleHats")             == sValue ) this->m_eValue = bordervaluePeopleHats;
                    else if ( _T("peopleWaving")           == sValue ) this->m_eValue = bordervaluePeopleWaving;
                    else if ( _T("poinsettias")            == sValue ) this->m_eValue = bordervaluePoinsettias;
                    else if ( _T("postageStamp")           == sValue ) this->m_eValue = bordervaluePostageStamp;
                    else if ( _T("pumpkin1")               == sValue ) this->m_eValue = bordervaluePumpkin1;
                    else if ( _T("pushPinNote1")           == sValue ) this->m_eValue = bordervaluePushPinNote1;
                    else if ( _T("pushPinNote2")           == sValue ) this->m_eValue = bordervaluePushPinNote2;
                    else if ( _T("pyramids")               == sValue ) this->m_eValue = bordervaluePyramids;
                    else if ( _T("pyramidsAbove")          == sValue ) this->m_eValue = bordervaluePyramidsAbove;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'q':
				{
                    if      ( _T("quadrants")              == sValue ) this->m_eValue = bordervalueQuadrants;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'r':
				{
                    if      ( _T("rings")                  == sValue ) this->m_eValue = bordervalueRings;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 's':
				{
                    if      ( _T("safari")                 == sValue ) this->m_eValue = bordervalueSafari;
                    else if ( _T("sawtooth")               == sValue ) this->m_eValue = bordervalueSawtooth;
                    else if ( _T("sawtoothGray")           == sValue ) this->m_eValue = bordervalueSawtoothGray;
                    else if ( _T("scaredCat")              == sValue ) this->m_eValue = bordervalueScaredCat;
                    else if ( _T("seattle")                == sValue ) this->m_eValue = bordervalueSeattle;
                    else if ( _T("shadowedSquares")        == sValue ) this->m_eValue = bordervalueShadowedSquares;
                    else if ( _T("shapes1")                == sValue ) this->m_eValue = bordervalueShapes1;
                    else if ( _T("shapes2")                == sValue ) this->m_eValue = bordervalueShapes2;
                    else if ( _T("sharksTeeth")            == sValue ) this->m_eValue = bordervalueSharksTeeth;
                    else if ( _T("shorebirdTracks")        == sValue ) this->m_eValue = bordervalueShorebirdTracks;
                    else if ( _T("single")                 == sValue ) this->m_eValue = bordervalueSingle;
                    else if ( _T("skyrocket")              == sValue ) this->m_eValue = bordervalueSkyrocket;
                    else if ( _T("snowflakeFancy")         == sValue ) this->m_eValue = bordervalueSnowflakeFancy;
                    else if ( _T("snowflakes")             == sValue ) this->m_eValue = bordervalueSnowflakes;
                    else if ( _T("sombrero")               == sValue ) this->m_eValue = bordervalueSombrero;
                    else if ( _T("southwest")              == sValue ) this->m_eValue = bordervalueSouthwest;
                    else if ( _T("stars")                  == sValue ) this->m_eValue = bordervalueStars;
                    else if ( _T("stars3d")                == sValue ) this->m_eValue = bordervalueStars3d;
                    else if ( _T("starsBlack")             == sValue ) this->m_eValue = bordervalueStarsBlack;
                    else if ( _T("starsShadowed")          == sValue ) this->m_eValue = bordervalueStarsShadowed;
                    else if ( _T("starsTop")               == sValue ) this->m_eValue = bordervalueStarsTop;
                    else if ( _T("sun")                    == sValue ) this->m_eValue = bordervalueSun;
                    else if ( _T("swirligig")              == sValue ) this->m_eValue = bordervalueSwirligig;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 't':
				{
                    if      ( _T("thick")                  == sValue ) this->m_eValue = bordervalueThick;
                    else if ( _T("thickThinLargeGap")      == sValue ) this->m_eValue = bordervalueThickThinLargeGap;
                    else if ( _T("thickThinMediumGap")     == sValue ) this->m_eValue = bordervalueThickThinMediumGap;
                    else if ( _T("thickThinSmallGap")      == sValue ) this->m_eValue = bordervalueThickThinSmallGap;
                    else if ( _T("thinThickLargeGap")      == sValue ) this->m_eValue = bordervalueThinThickLargeGap;
                    else if ( _T("thinThickMediumGap")     == sValue ) this->m_eValue = bordervalueThinThickMediumGap;
                    else if ( _T("thinThickSmallGap")      == sValue ) this->m_eValue = bordervalueThinThickSmallGap;
                    else if ( _T("thinThickThinLargeGap")  == sValue ) this->m_eValue = bordervalueThinThickThinLargeGap;
                    else if ( _T("thinThickThinMediumGap") == sValue ) this->m_eValue = bordervalueThinThickThinMediumGap;
                    else if ( _T("thinThickThinSmallGap")  == sValue ) this->m_eValue = bordervalueThinThickThinSmallGap;
                    else if ( _T("threeDEmboss")           == sValue ) this->m_eValue = bordervalueThreeDEmboss;
                    else if ( _T("threeDEngrave")          == sValue ) this->m_eValue = bordervalueThreeDEngrave;
                    else if ( _T("tornPaper")              == sValue ) this->m_eValue = bordervalueTornPaper;
                    else if ( _T("tornPaperBlack")         == sValue ) this->m_eValue = bordervalueTornPaperBlack;
                    else if ( _T("trees")                  == sValue ) this->m_eValue = bordervalueTrees;
                    else if ( _T("triangle1")              == sValue ) this->m_eValue = bordervalueTriangle1;
                    else if ( _T("triangle2")              == sValue ) this->m_eValue = bordervalueTriangle2;
                    else if ( _T("triangleCircle1")        == sValue ) this->m_eValue = bordervalueTriangleCircle1;
                    else if ( _T("triangleCircle2")        == sValue ) this->m_eValue = bordervalueTriangleCircle2;
                    else if ( _T("triangleParty")          == sValue ) this->m_eValue = bordervalueTriangleParty;
                    else if ( _T("triangles")              == sValue ) this->m_eValue = bordervalueTriangles;
                    else if ( _T("triple")                 == sValue ) this->m_eValue = bordervalueTriple;
                    else if ( _T("twistedLines1")          == sValue ) this->m_eValue = bordervalueTwistedLines1;
                    else if ( _T("twistedLines2")          == sValue ) this->m_eValue = bordervalueTwistedLines2;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'v':
				{
                    if      ( _T("vine")                   == sValue ) this->m_eValue = bordervalueVine;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'w':
				{
                    if      ( _T("wave")                   == sValue ) this->m_eValue = bordervalueWave;
                    else if ( _T("waveline")               == sValue ) this->m_eValue = bordervalueWaveline;
                    else if ( _T("weavingAngles")          == sValue ) this->m_eValue = bordervalueWeavingAngles;
                    else if ( _T("weavingBraid")           == sValue ) this->m_eValue = bordervalueWeavingBraid;
                    else if ( _T("weavingRibbon")          == sValue ) this->m_eValue = bordervalueWeavingRibbon;
                    else if ( _T("weavingStrips")          == sValue ) this->m_eValue = bordervalueWeavingStrips;
                    else if ( _T("whiteFlowers")           == sValue ) this->m_eValue = bordervalueWhiteFlowers;
                    else if ( _T("woodwork")               == sValue ) this->m_eValue = bordervalueWoodwork;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'x':
				{
                    if      ( _T("xIllusions")             == sValue ) this->m_eValue = bordervalueXIllusions;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			case 'z':
				{
                    if      ( _T("zanyTriangles")          == sValue ) this->m_eValue = bordervalueZanyTriangles;
                    else if ( _T("zigZag")                 == sValue ) this->m_eValue = bordervalueZigZag;
                    else if ( _T("zigZagStitch")           == sValue ) this->m_eValue = bordervalueZigZagStitch;
                    else                                               this->m_eValue = eDefValue;
					break;					
				}
			default:
				{
                    this->m_eValue = eDefValue;
					break;
				}
			}

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case bordervalueApples                : return _T("apples");
			case bordervalueArchedScallops        : return _T("archedScallops");
			case bordervalueBabyPacifier          : return _T("babyPacifier");
			case bordervalueBabyRattle            : return _T("babyRattle");
			case bordervalueBalloons3Colors       : return _T("balloons3Colors");
			case bordervalueBalloonsHotAir        : return _T("balloonsHotAir");
			case bordervalueBasicBlackDashes      : return _T("basicBlackDashes");
			case bordervalueBasicBlackDots        : return _T("basicBlackDots");
			case bordervalueBasicBlackSquares     : return _T("basicBlackSquares");
			case bordervalueBasicThinLines        : return _T("basicThinLines");
			case bordervalueBasicWhiteDashes      : return _T("basicWhiteDashes");
			case bordervalueBasicWhiteDots        : return _T("basicWhiteDots");
			case bordervalueBasicWhiteSquares     : return _T("basicWhiteSquares");
			case bordervalueBasicWideInline       : return _T("basicWideInline");
			case bordervalueBasicWideMidline      : return _T("basicWideMidline");
			case bordervalueBasicWideOutline      : return _T("basicWideOutline");
			case bordervalueBats                  : return _T("bats");
			case bordervalueBirds                 : return _T("birds");
			case bordervalueBirdsFlight           : return _T("birdsFlight");
			case bordervalueCabins                : return _T("cabins");
			case bordervalueCakeSlice             : return _T("cakeSlice");
			case bordervalueCandyCorn             : return _T("candyCorn");
			case bordervalueCelticKnotwork        : return _T("celticKnotwork");
			case bordervalueCertificateBanner     : return _T("certificateBanner");
			case bordervalueChainLink             : return _T("chainLink");
			case bordervalueChampagneBottle       : return _T("champagneBottle");
			case bordervalueCheckedBarBlack       : return _T("checkedBarBlack");
			case bordervalueCheckedBarColor       : return _T("checkedBarColor");
			case bordervalueCheckered             : return _T("checkered");
			case bordervalueChristmasTree         : return _T("christmasTree");
			case bordervalueCirclesLines          : return _T("circlesLines");
			case bordervalueCirclesRectangles     : return _T("circlesRectangles");
			case bordervalueClassicalWave         : return _T("classicalWave");
			case bordervalueClocks                : return _T("clocks");
			case bordervalueCompass               : return _T("compass");
			case bordervalueConfetti              : return _T("confetti");
			case bordervalueConfettiGrays         : return _T("confettiGrays");
			case bordervalueConfettiOutline       : return _T("confettiOutline");
			case bordervalueConfettiStreamers     : return _T("confettiStreamers");
			case bordervalueConfettiWhite         : return _T("confettiWhite");
			case bordervalueCornerTriangles       : return _T("cornerTriangles");
			case bordervalueCouponCutoutDashes    : return _T("couponCutoutDashes");
			case bordervalueCouponCutoutDots      : return _T("couponCutoutDots");
			case bordervalueCrazyMaze             : return _T("crazyMaze");
			case bordervalueCreaturesButterfly    : return _T("creaturesButterfly");
			case bordervalueCreaturesFish         : return _T("creaturesFish");
			case bordervalueCreaturesInsects      : return _T("creaturesInsects");
			case bordervalueCreaturesLadyBug      : return _T("creaturesLadyBug");
			case bordervalueCrossStitch           : return _T("crossStitch");
			case bordervalueCup                   : return _T("cup");
			case bordervalueCustom                : return _T("custom");
			case bordervalueDashDotStroked        : return _T("dashDotStroked");
			case bordervalueDashed                : return _T("dashed");
			case bordervalueDashSmallGap          : return _T("dashSmallGap");
			case bordervalueDecoArch              : return _T("decoArch");
			case bordervalueDecoArchColor         : return _T("decoArchColor");
			case bordervalueDecoBlocks            : return _T("decoBlocks");
			case bordervalueDiamondsGray          : return _T("diamondsGray");
			case bordervalueDotDash               : return _T("dotDash");
			case bordervalueDotDotDash            : return _T("dotDotDash");
			case bordervalueDotted                : return _T("dotted");
			case bordervalueDouble                : return _T("double");
			case bordervalueDoubleD               : return _T("doubleD");
			case bordervalueDoubleDiamonds        : return _T("doubleDiamonds");
			case bordervalueDoubleWave            : return _T("doubleWave");
			case bordervalueEarth1                : return _T("earth1");
			case bordervalueEarth2                : return _T("earth2");
			case bordervalueEarth3                : return _T("earth3");
			case bordervalueEclipsingSquares1     : return _T("eclipsingSquares1");
			case bordervalueEclipsingSquares2     : return _T("eclipsingSquares2");
			case bordervalueEggsBlack             : return _T("eggsBlack");
			case bordervalueFans                  : return _T("fans");
			case bordervalueFilm                  : return _T("film");
			case bordervalueFirecrackers          : return _T("firecrackers");
			case bordervalueFlowersBlockPrint     : return _T("flowersBlockPrint");
			case bordervalueFlowersDaisies        : return _T("flowersDaisies");
			case bordervalueFlowersModern1        : return _T("flowersModern1");
			case bordervalueFlowersModern2        : return _T("flowersModern2");
			case bordervalueFlowersPansy          : return _T("flowersPansy");
			case bordervalueFlowersRedRose        : return _T("flowersRedRose");
			case bordervalueFlowersRoses          : return _T("flowersRoses");
			case bordervalueFlowersTeacup         : return _T("flowersTeacup");
			case bordervalueFlowersTiny           : return _T("flowersTiny");
			case bordervalueGems                  : return _T("gems");
			case bordervalueGingerbreadMan        : return _T("gingerbreadMan");
			case bordervalueGradient              : return _T("gradient");
			case bordervalueHandmade1             : return _T("handmade1");
			case bordervalueHandmade2             : return _T("handmade2");
			case bordervalueHeartBalloon          : return _T("heartBalloon");
			case bordervalueHeartGray             : return _T("heartGray");
			case bordervalueHearts                : return _T("hearts");
			case bordervalueHeebieJeebies         : return _T("heebieJeebies");
			case bordervalueHolly                 : return _T("holly");
			case bordervalueHouseFunky            : return _T("houseFunky");
			case bordervalueHypnotic              : return _T("hypnotic");
			case bordervalueIceCreamCones         : return _T("iceCreamCones");
			case bordervalueInset                 : return _T("inset");
			case bordervalueLightBulb             : return _T("lightBulb");
			case bordervalueLightning1            : return _T("lightning1");
			case bordervalueLightning2            : return _T("lightning2");
			case bordervalueMapleLeaf             : return _T("mapleLeaf");
			case bordervalueMapleMuffins          : return _T("mapleMuffins");
			case bordervalueMapPins               : return _T("mapPins");
			case bordervalueMarquee               : return _T("marquee");
			case bordervalueMarqueeToothed        : return _T("marqueeToothed");
			case bordervalueMoons                 : return _T("moons");
			case bordervalueMosaic                : return _T("mosaic");
			case bordervalueMusicNotes            : return _T("musicNotes");
			case bordervalueNil                   : return _T("nil");
			case bordervalueNone                  : return _T("none");
			case bordervalueNorthwest             : return _T("northwest");
			case bordervalueOutset                : return _T("outset");
			case bordervalueOvals                 : return _T("ovals");
			case bordervaluePackages              : return _T("packages");
			case bordervaluePalmsBlack            : return _T("palmsBlack");
			case bordervaluePalmsColor            : return _T("palmsColor");
			case bordervaluePaperClips            : return _T("paperClips");
			case bordervaluePapyrus               : return _T("papyrus");
			case bordervaluePartyFavor            : return _T("partyFavor");
			case bordervaluePartyGlass            : return _T("partyGlass");
			case bordervaluePencils               : return _T("pencils");
			case bordervaluePeople                : return _T("people");
			case bordervaluePeopleHats            : return _T("peopleHats");
			case bordervaluePeopleWaving          : return _T("peopleWaving");
			case bordervaluePoinsettias           : return _T("poinsettias");
			case bordervaluePostageStamp          : return _T("postageStamp");
			case bordervaluePumpkin1              : return _T("pumpkin1");
			case bordervaluePushPinNote1          : return _T("pushPinNote1");
			case bordervaluePushPinNote2          : return _T("pushPinNote2");
			case bordervaluePyramids              : return _T("pyramids");
			case bordervaluePyramidsAbove         : return _T("pyramidsAbove");
			case bordervalueQuadrants             : return _T("quadrants");
			case bordervalueRings                 : return _T("rings");
			case bordervalueSafari                : return _T("safari");
			case bordervalueSawtooth              : return _T("sawtooth");
			case bordervalueSawtoothGray          : return _T("sawtoothGray");
			case bordervalueScaredCat             : return _T("scaredCat");
			case bordervalueSeattle               : return _T("seattle");
			case bordervalueShadowedSquares       : return _T("shadowedSquares");
			case bordervalueShapes1               : return _T("shapes1");
			case bordervalueShapes2               : return _T("shapes2");
			case bordervalueSharksTeeth           : return _T("sharksTeeth");
			case bordervalueShorebirdTracks       : return _T("shorebirdTracks");
			case bordervalueSingle                : return _T("single");
			case bordervalueSkyrocket             : return _T("skyrocket");
			case bordervalueSnowflakeFancy        : return _T("snowflakeFancy");
			case bordervalueSnowflakes            : return _T("snowflakes");
			case bordervalueSombrero              : return _T("sombrero");
			case bordervalueSouthwest             : return _T("southwest");
			case bordervalueStars                 : return _T("stars");
			case bordervalueStars3d               : return _T("stars3d");
			case bordervalueStarsBlack            : return _T("starsBlack");
			case bordervalueStarsShadowed         : return _T("starsShadowed");
			case bordervalueStarsTop              : return _T("starsTop");
			case bordervalueSun                   : return _T("sun");
			case bordervalueSwirligig             : return _T("swirligig");
			case bordervalueThick                 : return _T("thick");
			case bordervalueThickThinLargeGap     : return _T("thickThinLargeGap");
			case bordervalueThickThinMediumGap    : return _T("thickThinMediumGap");
			case bordervalueThickThinSmallGap     : return _T("thickThinSmallGap");
			case bordervalueThinThickLargeGap     : return _T("thinThickLargeGap");
			case bordervalueThinThickMediumGap    : return _T("thinThickMediumGap");
			case bordervalueThinThickSmallGap     : return _T("thinThickSmallGap");
			case bordervalueThinThickThinLargeGap : return _T("thinThickThinLargeGap");
			case bordervalueThinThickThinMediumGap: return _T("thinThickThinMediumGap");
			case bordervalueThinThickThinSmallGap : return _T("thinThickThinSmallGap");
			case bordervalueThreeDEmboss          : return _T("threeDEmboss");
			case bordervalueThreeDEngrave         : return _T("threeDEngrave");
			case bordervalueTornPaper             : return _T("tornPaper");
			case bordervalueTornPaperBlack        : return _T("tornPaperBlack");
			case bordervalueTrees                 : return _T("trees");
			case bordervalueTriangle1             : return _T("triangle1");
			case bordervalueTriangle2             : return _T("triangle2");
			case bordervalueTriangleCircle1       : return _T("triangleCircle1");
			case bordervalueTriangleCircle2       : return _T("triangleCircle2");
			case bordervalueTriangleParty         : return _T("triangleParty");
			case bordervalueTriangles             : return _T("triangles");
			case bordervalueTriple                : return _T("triple");
			case bordervalueTwistedLines1         : return _T("twistedLines1");
			case bordervalueTwistedLines2         : return _T("twistedLines2");
			case bordervalueVine                  : return _T("vine");
			case bordervalueWave                  : return _T("wave");
			case bordervalueWaveline              : return _T("waveline");
			case bordervalueWeavingAngles         : return _T("weavingAngles");
			case bordervalueWeavingBraid          : return _T("weavingBraid");
			case bordervalueWeavingRibbon         : return _T("weavingRibbon");
			case bordervalueWeavingStrips         : return _T("weavingStrips");
			case bordervalueWhiteFlowers          : return _T("whiteFlowers");
			case bordervalueWoodwork              : return _T("woodwork");
			case bordervalueXIllusions            : return _T("xIllusions");
			case bordervalueZanyTriangles         : return _T("zanyTriangles");
			case bordervalueZigZag                : return _T("zigZag");
			case bordervalueZigZagStitch          : return _T("zigZagStitch");

			default                               : return _T("none");
			}
		}




		SimpleType_FromString     (EBorder)
		SimpleType_Operator_Equal (CBorder)

		bool IsArtBorder () const
		{
            return ((int)this->m_eValue > 100);
		}
		bool IsLineBorder() const
		{
            return ( (int)this->m_eValue <= 25 && (int)this->m_eValue >= 1 );
		}
		bool IsNoBorder  () const
		{
            return ( (int)this->m_eValue <= 0 );
		}

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

	template<EBrClear eDefValue = brclearAll>
	class CBrClear : public CSimpleType<EBrClear, eDefValue>
	{
	public:
		CBrClear() {}

		virtual EBrClear FromString(CString &sValue)
		{
            if      ( _T("all")   == sValue ) this->m_eValue = brclearAll;
            else if ( _T("left")  == sValue ) this->m_eValue = brclearLeft;
            else if ( _T("none")  == sValue ) this->m_eValue = brclearNone;
            else if ( _T("right") == sValue ) this->m_eValue = brclearRight;
            else                              this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString  ToString  () const 
		{
            switch(this->m_eValue)
			{
			case brclearAll   : return _T("all");
			case brclearLeft  : return _T("left");
			case brclearNone  : return _T("none");
			case brclearRight : return _T("right");
			default           : return _T("all");
			}
		}

		SimpleType_FromString     (EBrClear)
		SimpleType_Operator_Equal (CBrClear)
	};
	//--------------------------------------------------------------------------------
	// BrType 17.18.4 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EBrType
	{
		brtypeColumn       = 0,
		brtypePage         = 1,
		brtypeTextWrapping = 2
	};

	template<EBrType eDefValue = brtypePage>
	class CBrType : public CSimpleType<EBrType, eDefValue>
	{
	public:
		CBrType() {}

		virtual EBrType FromString(CString &sValue)
		{
            if      ( _T("column")       == sValue ) this->m_eValue = brtypeColumn;
            else if ( _T("page")         == sValue ) this->m_eValue = brtypePage;
            else if ( _T("textWrapping") == sValue ) this->m_eValue = brtypeTextWrapping;
            else                                     this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case brtypeColumn       : return _T("column");
			case brtypePage         : return _T("page");
			case brtypeTextWrapping : return _T("textWrapping");
			default                 : return _T("page");
			}
		}


		SimpleType_FromString     (EBrType)
		SimpleType_Operator_Equal (CBrType)
	};

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

	template<ECaptionPos eDefValue = captionposAbove>
	class CCaptionPos : public CSimpleType<ECaptionPos, eDefValue>
	{
	public:
		CCaptionPos() {}

		virtual ECaptionPos FromString(CString &sValue)
		{
            if      ( _T("above") == sValue ) this->m_eValue = captionposAbove;
            else if ( _T("below") == sValue ) this->m_eValue = captionposBelow;
            else if ( _T("left")  == sValue ) this->m_eValue = captionposLeft;
            else if ( _T("right") == sValue ) this->m_eValue = captionposRight;
            else                              this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case captionposAbove : return _T("above");
			case captionposBelow : return _T("below");
			case captionposLeft  : return _T("left");
			case captionposRight : return _T("right");
			default              : return _T("above");
			}
		}


		SimpleType_FromString     (ECaptionPos)
		SimpleType_Operator_Equal (CCaptionPos)
	};
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

	template<EChapterSep eDefValue = chaptersepColon>
	class CChapterSep : public CSimpleType<EChapterSep, eDefValue>
	{
	public:
		CChapterSep() {}

		virtual EChapterSep FromString(CString &sValue)
		{
            if      ( _T("colon")  == sValue ) this->m_eValue = chaptersepColon;
            else if ( _T("emDash") == sValue ) this->m_eValue = chaptersepEmDash;
            else if ( _T("enDash") == sValue ) this->m_eValue = chaptersepEnDash;
            else if ( _T("hyphen") == sValue ) this->m_eValue = chaptersepHyphen;
            else if ( _T("period") == sValue ) this->m_eValue = chaptersepPeriod;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case chaptersepColon  : return _T("colon");
			case chaptersepEmDash : return _T("emDash");
			case chaptersepEnDash : return _T("enDash");
			case chaptersepHyphen : return _T("hyphen");
			case chaptersepPeriod : return _T("period");
			default               : return _T("colon");
			}
		}


		SimpleType_FromString     (EChapterSep)
		SimpleType_Operator_Equal (CChapterSep)
	};
	//--------------------------------------------------------------------------------
	// CharacterSpacing 17.18.7 (Part 1)
	//--------------------------------------------------------------------------------		

	enum ECharacterSpacing
	{
		charspacingCompressPunctuation                = 0,
		charspacingCompressPunctuationAndJapaneseKana = 1,
		charspacingDoNotCompress                      = 2
	};

	template<ECharacterSpacing eDefValue = charspacingDoNotCompress>
	class CCharacterSpacing : public CSimpleType<ECharacterSpacing, eDefValue>
	{
	public:
		CCharacterSpacing() {}

		virtual ECharacterSpacing FromString(CString &sValue)
		{
            if      ( _T("compressPunctuation")                == sValue ) this->m_eValue = charspacingCompressPunctuation;
            else if ( _T("compressPunctuationAndJapaneseKana") == sValue ) this->m_eValue = charspacingCompressPunctuationAndJapaneseKana;
            else if ( _T("doNotCompress")                      == sValue ) this->m_eValue = charspacingDoNotCompress;
            else                                                           this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case charspacingCompressPunctuation                : return _T("compressPunctuation");
			case charspacingCompressPunctuationAndJapaneseKana : return _T("compressPunctuationAndJapaneseKana");
			case charspacingDoNotCompress                      : return _T("doNotCompress");
			default                                            : return _T("doNotCompress");
			}
		}


		SimpleType_FromString     (ECharacterSpacing)
		SimpleType_Operator_Equal (CCharacterSpacing)
	};
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

	template<ECombineBrackets eDefValue = combinebracketsNone>
	class CCombineBrackets : public CSimpleType<ECombineBrackets, eDefValue>
	{
	public:
		CCombineBrackets() {}

		virtual ECombineBrackets FromString(CString &sValue)
		{
            if      ( _T("angle")  == sValue ) this->m_eValue = combinebracketsAngle;
            else if ( _T("curly")  == sValue ) this->m_eValue = combinebracketsCurly;
            else if ( _T("none")   == sValue ) this->m_eValue = combinebracketsNone;
            else if ( _T("round")  == sValue ) this->m_eValue = combinebracketsRound;
            else if ( _T("square") == sValue ) this->m_eValue = combinebracketsSquare;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString          ToString  () const 
		{
            switch(this->m_eValue)
			{
			case combinebracketsAngle  : return _T("angle");
			case combinebracketsCurly  : return _T("curly");
			case combinebracketsNone   : return _T("none");
			case combinebracketsRound  : return _T("round");
			case combinebracketsSquare : return _T("square");
			default                    : return _T("none");
			}
		}


		SimpleType_FromString     (ECombineBrackets)
		SimpleType_Operator_Equal (CCombineBrackets)
	};


	//--------------------------------------------------------------------------------
	// DateTime 17.18.9 (Part 1)
	//--------------------------------------------------------------------------------		

	class CDateTime
	{
	public:
		CDateTime() {}

		CString GetValue() const
		{
			return m_sValue;
		}

		void    SetValue(CString &sValue)
		{
			m_sValue = sValue;
		}


		CString FromString(CString &sValue)
		{
			m_sValue = sValue;

			return m_sValue;
		}

		CString ToString  () const 
		{
			return m_sValue;
		}

		SimpleType_FromString2    (CString)
		SimpleType_Operator_Equal (CDateTime)

	private:

		CString m_sValue;
	};

	//--------------------------------------------------------------------------------
	// DecimalNumber 17.18.10 (Part 1)
	//--------------------------------------------------------------------------------		

	template<int nDefValue = 0>
	class CDecimalNumber : public CSimpleType<int, nDefValue>
	{
	public:
		CDecimalNumber() {}

		virtual int     FromString(CString &sValue)
		{
            this->m_eValue = _wtoi( sValue );

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

			return sResult;
		}


		SimpleType_FromString           (int)
		SimpleType_Operator_Equal       (CDecimalNumber)
		SimpleTypes_AdditionalOpearators(CDecimalNumber)
	};

	//--------------------------------------------------------------------------------
	// DecimalNumberOrPercent 17.18.11 (Part 1)
	//--------------------------------------------------------------------------------		

	class CDecimalNumberOrPercent
	{
	public:
		CDecimalNumberOrPercent() 
		{
			m_bTrailingPercentSign = false;
		}

		double GetValue() const
		{
			return m_dValue;
		}

		void   SetValue(double &dValue)
		{
			m_dValue = dValue;
		}

		double  FromString(CString &sValue, double dDefValue = 0)
		{
			if ( sValue.GetLength() <= 0 )
			{
				m_dValue = dDefValue;
				return m_dValue;
			}

			int nLen = sValue.GetLength();
			wchar_t wsLastChar = sValue.GetAt( nLen - 1 );
			if ( wsLastChar == '%' )
			{
				nLen--;
				m_bTrailingPercentSign = true;
			}
			else
				m_bTrailingPercentSign = false;

			m_dValue = _wtof( sValue.Mid(0, nLen) );

			return m_dValue;
		}

		CString ToString  () const 
		{
			CString sResult;

			if ( !m_bTrailingPercentSign )
				sResult.Format( _T("%d"), (int)m_dValue);
			else
				sResult.Format( _T("%f%%"), m_dValue);

			return sResult;
		}



		SimpleType_FromString2    (double)
		SimpleType_Operator_Equal (CDecimalNumberOrPercent)

		bool IsPercent() const
		{
			return m_bTrailingPercentSign;
		}

		void SetPercent(bool bPercent)
		{
			m_bTrailingPercentSign = bPercent;
		}

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

	template<EDirection eDefValue = directionLTR>
	class CDirection : public CSimpleType<EDirection, eDefValue>
	{
	public:
		CDirection() {}

		virtual EDirection FromString(CString &sValue)
		{
            if      ( _T("ltr")  == sValue ) this->m_eValue = directionLTR;
            else if ( _T("rtl")  == sValue ) this->m_eValue = directionRTL;
            else                             this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString    ToString  () const 
		{
            switch(this->m_eValue)
			{
			case directionLTR : return _T("ltr");
			case directionRTL : return _T("rtl");
			default           : return _T("ltr");
			}
		}

		SimpleType_FromString     (EDirection)
		SimpleType_Operator_Equal (CDirection)
	};

	//--------------------------------------------------------------------------------
	// DisplacedByCustomXml 17.18.13 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EDisplacedByCustomXml
	{
		displacedbycustomxmlNext = 0,
		displacedbycustomxmlPrev = 1
	};

	template<EDisplacedByCustomXml eDefValue = displacedbycustomxmlNext>
	class CDisplacedByCustomXml : public CSimpleType<EDisplacedByCustomXml, eDefValue>
	{
	public:
		CDisplacedByCustomXml() {}

		virtual EDisplacedByCustomXml FromString(CString &sValue)
		{
            if      ( _T("next")  == sValue ) this->m_eValue = displacedbycustomxmlNext;
            else if ( _T("prev")  == sValue ) this->m_eValue = displacedbycustomxmlPrev;
            else                              this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString          ToString  () const 
		{
            switch(this->m_eValue)
			{
			case displacedbycustomxmlNext : return _T("next");
			case displacedbycustomxmlPrev : return _T("prev");
			default                       : return _T("next");
			}
		}

		SimpleType_FromString     (EDisplacedByCustomXml)
		SimpleType_Operator_Equal (CDisplacedByCustomXml)
	};


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

	template<EDocGrid eDefValue = docgridDefault>
	class CDocGrid : public CSimpleType<EDocGrid, eDefValue>
	{
	public:
		CDocGrid() {}

		virtual EDocGrid FromString(CString &sValue)
		{
            if      ( _T("default")       == sValue ) this->m_eValue = docgridDefault;
            else if ( _T("lines")         == sValue ) this->m_eValue = docgridLines;
            else if ( _T("linesAndChars") == sValue ) this->m_eValue = docgridLinesAndChars;
            else if ( _T("snapToChars")   == sValue ) this->m_eValue = docgridSnapToChars;
            else                                      this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString  ToString  () const 
		{
            switch(this->m_eValue)
			{
			case docgridDefault       : return _T("default");
			case docgridLines         : return _T("lines");
			case docgridLinesAndChars : return _T("linesAndChars");
			case docgridSnapToChars   : return _T("snapToChars");
			default                   : return _T("default");
			}
		}

		SimpleType_FromString     (EDocGrid)
		SimpleType_Operator_Equal (CDocGrid)
	};


	//--------------------------------------------------------------------------------
	// DocPartBehavior 17.18.15 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EDocPartBehavior
	{
		docpartbehaviorContent = 0,
		docpartbehaviorP       = 1,
		docpartbehaviorPg      = 2
	};

	template<EDocPartBehavior eDefValue = docpartbehaviorContent>
	class CDocPartBehavior : public CSimpleType<EDocPartBehavior, eDefValue>
	{
	public:
		CDocPartBehavior() {}

		virtual EDocPartBehavior FromString(CString &sValue)
		{
            if      ( _T("content") == sValue ) this->m_eValue = docpartbehaviorContent;
            else if ( _T("p")       == sValue ) this->m_eValue = docpartbehaviorP;
            else if ( _T("pg")      == sValue ) this->m_eValue = docpartbehaviorPg;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString          ToString  () const 
		{
            switch(this->m_eValue)
			{
			case docpartbehaviorContent : return _T("content");
			case docpartbehaviorP       : return _T("p");
			case docpartbehaviorPg      : return _T("pg");
			default                     : return _T("content");
			}
		}

		SimpleType_FromString     (EDocPartBehavior)
		SimpleType_Operator_Equal (CDocPartBehavior)
	};


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

	template<EDocPartGallery eDefValue = docpartgalleryDefault>
	class CDocPartGallery : public CSimpleType<EDocPartGallery, eDefValue>
	{
	public:
		CDocPartGallery() {}

		virtual EDocPartGallery FromString(CString &sValue)
		{
            if      ( _T("any")               == sValue ) this->m_eValue = docpartgalleryAny;
            else if ( _T("autoTxt")           == sValue ) this->m_eValue = docpartgalleryAutoTxt;
            else if ( _T("bib")               == sValue ) this->m_eValue = docpartgalleryBib;
            else if ( _T("coverPg")           == sValue ) this->m_eValue = docpartgalleryCoverPg;
            else if ( _T("custAutoTxt")       == sValue ) this->m_eValue = docpartgalleryCustAutoTxt;
            else if ( _T("custBib")           == sValue ) this->m_eValue = docpartgalleryCustBib;
            else if ( _T("custCoverPg")       == sValue ) this->m_eValue = docpartgalleryCustCoverPg;
            else if ( _T("custEq")            == sValue ) this->m_eValue = docpartgalleryCustEq;
            else if ( _T("custFtrs")          == sValue ) this->m_eValue = docpartgalleryCustFtrs;
            else if ( _T("custHdrs")          == sValue ) this->m_eValue = docpartgalleryCustHdrs;
            else if ( _T("custom1")           == sValue ) this->m_eValue = docpartgalleryCustom1;
            else if ( _T("custom2")           == sValue ) this->m_eValue = docpartgalleryCustom2;
            else if ( _T("custom3")           == sValue ) this->m_eValue = docpartgalleryCustom3;
            else if ( _T("custom4")           == sValue ) this->m_eValue = docpartgalleryCustom4;
            else if ( _T("custom5")           == sValue ) this->m_eValue = docpartgalleryCustom5;
            else if ( _T("custPgNum")         == sValue ) this->m_eValue = docpartgalleryCustPgNum;
            else if ( _T("custPgNumB")        == sValue ) this->m_eValue = docpartgalleryCustPgNumB;
            else if ( _T("custPgNumMargins")  == sValue ) this->m_eValue = docpartgalleryCustPgNumMargins;
            else if ( _T("custPgNumT")        == sValue ) this->m_eValue = docpartgalleryCustPgNumT;
            else if ( _T("custQuickParts")    == sValue ) this->m_eValue = docpartgalleryCustQuickParts;
            else if ( _T("custTblOfContents") == sValue ) this->m_eValue = docpartgalleryCustTblOfContents;
            else if ( _T("custTbls")          == sValue ) this->m_eValue = docpartgalleryCustTbls;
            else if ( _T("custTxtBox")        == sValue ) this->m_eValue = docpartgalleryCustTxtBox;
            else if ( _T("custWatermarks")    == sValue ) this->m_eValue = docpartgalleryCustWatermarks;
            else if ( _T("default")           == sValue ) this->m_eValue = docpartgalleryDefault;
            else if ( _T("docParts")          == sValue ) this->m_eValue = docpartgalleryDocParts;
            else if ( _T("eq")                == sValue ) this->m_eValue = docpartgalleryEq;
            else if ( _T("ftrs")              == sValue ) this->m_eValue = docpartgalleryFtrs;
            else if ( _T("hdrs")              == sValue ) this->m_eValue = docpartgalleryHdrs;
            else if ( _T("pgNum")             == sValue ) this->m_eValue = docpartgalleryPgNum;
            else if ( _T("pgNumB")            == sValue ) this->m_eValue = docpartgalleryPgNumB;
            else if ( _T("pgNumMargins")      == sValue ) this->m_eValue = docpartgalleryPgNumMargins;
            else if ( _T("pgNumT")            == sValue ) this->m_eValue = docpartgalleryPgNumT;
            else if ( _T("placeholder")       == sValue ) this->m_eValue = docpartgalleryPlaceHolder;
            else if ( _T("tblOfContents")     == sValue ) this->m_eValue = docpartgalleryTblOfContents;
            else if ( _T("tbls")              == sValue ) this->m_eValue = docpartgalleryTbls;
            else if ( _T("txtBox")            == sValue ) this->m_eValue = docpartgalleryTxtBox;
            else if ( _T("watermarks")        == sValue ) this->m_eValue = docpartgalleryWatermarks;
            else                                          this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString         ToString  () const 
		{
            switch(this->m_eValue)
			{
			case docpartgalleryAny               : return _T("any");
			case docpartgalleryAutoTxt           : return _T("autoTxt");
			case docpartgalleryBib               : return _T("bib");
			case docpartgalleryCoverPg           : return _T("coverPg");
			case docpartgalleryCustAutoTxt       : return _T("custAutoTxt");
			case docpartgalleryCustBib           : return _T("custBib");
			case docpartgalleryCustCoverPg       : return _T("custCoverPg");
			case docpartgalleryCustEq            : return _T("custEq");
			case docpartgalleryCustFtrs          : return _T("custFtrs");
			case docpartgalleryCustHdrs          : return _T("custHdrs");
			case docpartgalleryCustom1           : return _T("custom1");
			case docpartgalleryCustom2           : return _T("custom2");
			case docpartgalleryCustom3           : return _T("custom3");
			case docpartgalleryCustom4           : return _T("custom4");
			case docpartgalleryCustom5           : return _T("custom5");
			case docpartgalleryCustPgNum         : return _T("custPgNum");
			case docpartgalleryCustPgNumB        : return _T("custPgNumB");
			case docpartgalleryCustPgNumMargins  : return _T("custPgNumMargins");
			case docpartgalleryCustPgNumT        : return _T("custPgNumT");
			case docpartgalleryCustQuickParts    : return _T("custQuickParts");
			case docpartgalleryCustTblOfContents : return _T("custTblOfContents");
			case docpartgalleryCustTbls          : return _T("custTbls");
			case docpartgalleryCustTxtBox        : return _T("custTxtBox");
			case docpartgalleryCustWatermarks    : return _T("custWatermarks");
			case docpartgalleryDefault           : return _T("default");
			case docpartgalleryDocParts          : return _T("docParts");
			case docpartgalleryEq                : return _T("eq");
			case docpartgalleryFtrs              : return _T("ftrs");
			case docpartgalleryHdrs              : return _T("hdrs");
			case docpartgalleryPgNum             : return _T("pgNum");
			case docpartgalleryPgNumB            : return _T("pgNumB");
			case docpartgalleryPgNumMargins      : return _T("pgNumMargins");
			case docpartgalleryPgNumT            : return _T("pgNumT");
			case docpartgalleryPlaceHolder       : return _T("placeholder");
			case docpartgalleryTblOfContents     : return _T("tblOfContents");
			case docpartgalleryTbls              : return _T("tbls");
			case docpartgalleryTxtBox            : return _T("txtBox");
			case docpartgalleryWatermarks        : return _T("watermarks");
			default                              : return _T("default");
			}
		}

		SimpleType_FromString     (EDocPartGallery)
		SimpleType_Operator_Equal (CDocPartGallery)
	};

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

	template<EDocPartType eDefValue = docparttypeNone>
	class CDocPartType : public CSimpleType<EDocPartType, eDefValue>
	{
	public:
		CDocPartType() {}

		virtual EDocPartType FromString(CString &sValue)
		{
            if      ( _T("autoExp")  == sValue ) this->m_eValue = docparttypeAutoExp;
            else if ( _T("bbPlcHdr") == sValue ) this->m_eValue = docparttypeBBPlcHdr;
            else if ( _T("formFld")  == sValue ) this->m_eValue = docparttypeFormFld;
            else if ( _T("none")     == sValue ) this->m_eValue = docparttypeNone;
            else if ( _T("normal")   == sValue ) this->m_eValue = docparttypeNormal;
            else if ( _T("speller")  == sValue ) this->m_eValue = docparttypeSpeller;
            else if ( _T("toolbar")  == sValue ) this->m_eValue = docparttypeToolbar;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString      ToString  () const 
		{
            switch(this->m_eValue)
			{
			case docparttypeAutoExp  : return _T("autoExp");
			case docparttypeBBPlcHdr : return _T("bbPlcHdr");
			case docparttypeFormFld  : return _T("formFld");
			case docparttypeNone     : return _T("none");
			case docparttypeNormal   : return _T("normal");
			case docparttypeSpeller  : return _T("speller");
			case docparttypeToolbar  : return _T("toolbar");
			default                  : return _T("none");
			}
		}

		SimpleType_FromString     (EDocPartType)
		SimpleType_Operator_Equal (CDocPartType)
	};

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

	template<EDocProtect eDefValue = docprotectNone>
	class CDocProtect : public CSimpleType<EDocProtect, eDefValue>
	{
	public:
		CDocProtect() {}

		virtual EDocProtect FromString(CString &sValue)
		{
            if      ( _T("comments")       == sValue ) this->m_eValue = docprotectComments;
            else if ( _T("forms")          == sValue ) this->m_eValue = docprotectForms;
            else if ( _T("none")           == sValue ) this->m_eValue = docprotectNone;
            else if ( _T("readOnly")       == sValue ) this->m_eValue = docprotectReadOnly;
            else if ( _T("trackedChanges" )== sValue ) this->m_eValue = docprotectTrackedChanges;
            else                                       this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case docprotectComments       : return _T("comments");
			case docprotectForms          : return _T("forms");
			case docprotectNone           : return _T("none");
			case docprotectReadOnly       : return _T("readOnly");
			case docprotectTrackedChanges : return _T("trackedChanges");
			default                       : return _T("none");
			}
		}

		SimpleType_FromString     (EDocProtect)
		SimpleType_Operator_Equal (CDocProtect)
	};

	//--------------------------------------------------------------------------------
	// DocType 17.18.19 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EDocType
	{
		doctypeEMail        = 0,
		doctypeLetter       = 1,
		doctypeNotSpecified = 2
	};

	template<EDocType eDefValue = doctypeNotSpecified>
	class CDocType : public CSimpleType<EDocType, eDefValue>
	{
	public:
		CDocType() {}

		virtual EDocType FromString(CString &sValue)
		{
            if      ( _T("eMail")        == sValue ) this->m_eValue = doctypeEMail;
            else if ( _T("letter")       == sValue ) this->m_eValue = doctypeLetter;
            else if ( _T("notSpecified") == sValue ) this->m_eValue = doctypeNotSpecified;
            else                                     this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString  ToString  () const 
		{
            switch(this->m_eValue)
			{
			case doctypeEMail        : return _T("eMail");
			case doctypeLetter       : return _T("letter");
			case doctypeNotSpecified : return _T("notSpecified");
			default                  : return _T("notSpecified");
			}
		}

		SimpleType_FromString     (EDocType)
		SimpleType_Operator_Equal (CDocType)
	};

	//--------------------------------------------------------------------------------
	// DropCap 17.18.20 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EDropCap
	{
		dropcapNone   = 0,
		dropcapDrop   = 1,
		dropcapMargin = 2
	};

	template<EDropCap eDefValue = dropcapNone>
	class CDropCap : public CSimpleType<EDropCap, eDefValue>
	{
	public:
		CDropCap() {}

		virtual EDropCap FromString(CString &sValue)
		{
            if      ( _T("drop")   == sValue ) this->m_eValue = dropcapDrop;
            else if ( _T("margin") == sValue ) this->m_eValue = dropcapMargin;
            else if ( _T("none")   == sValue ) this->m_eValue = dropcapNone;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString  ToString  () const 
		{
            switch(this->m_eValue)
			{
			case dropcapDrop   : return _T("drop");
			case dropcapMargin : return _T("margin");
			case dropcapNone   : return _T("none");
			default            : return _T("none");
			}
		}

		SimpleType_FromString     (EDropCap)
		SimpleType_Operator_Equal (CDropCap)
	};


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

	template<EEdGrp eDefValue = edgrpEveryone>
	class CEdGrp : public CSimpleType<EEdGrp, eDefValue>
	{
	public:
		CEdGrp() {}

		virtual EEdGrp  FromString(CString &sValue)
		{
            if      ( _T("administrators") == sValue ) this->m_eValue = edgrpAdministrators;
            else if ( _T("contributors")   == sValue ) this->m_eValue = edgrpContributors;
            else if ( _T("current")        == sValue ) this->m_eValue = edgrpCurrent;
            else if ( _T("editors")        == sValue ) this->m_eValue = edgrpEditors;
            else if ( _T("everyone")       == sValue ) this->m_eValue = edgrpEveryone;
            else if ( _T("none")           == sValue ) this->m_eValue = edgrpNone;
            else if ( _T("owners")         == sValue ) this->m_eValue = edgrpOwners;
            else                                       this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case edgrpAdministrators : return _T("administrators");
			case edgrpContributors   : return _T("contributors");
			case edgrpCurrent        : return _T("current");
			case edgrpEditors        : return _T("editors");
			case edgrpEveryone       : return _T("everyone");
			case edgrpNone           : return _T("none");
			case edgrpOwners         : return _T("owners");
			default                  : return _T("everyone");
			}
		}

		SimpleType_FromString     (EEdGrp)
		SimpleType_Operator_Equal (CEdGrp)
	};


	//--------------------------------------------------------------------------------
	// EdnPos 17.18.22 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EEdnPos
	{
		ednposDocEnd  = 0,
		ednposSectEnd = 1
	};

	template<EEdnPos eDefValue = ednposSectEnd>
	class CEdnPos : public CSimpleType<EEdnPos, eDefValue>
	{
	public:
		CEdnPos() {}

		virtual EEdnPos FromString(CString &sValue)
		{
            if      ( _T("docEnd")  == sValue ) this->m_eValue = ednposDocEnd;
            else if ( _T("sectEnd") == sValue ) this->m_eValue = ednposSectEnd;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case ednposDocEnd  : return _T("docEnd");
			case ednposSectEnd : return _T("sectEnd");
			default            : return _T("sectEnd");
			}
		}

		SimpleType_FromString     (EEdnPos)
		SimpleType_Operator_Equal (CEdnPos)
	};


	//--------------------------------------------------------------------------------
	// EighthPointMeasure 17.18.23 (Part 1)
	//--------------------------------------------------------------------------------		

	template<int nDefValue = 0>
	class CEighthPointMeasure : CSimpleType<int, nDefValue>
	{
	public:
		CEighthPointMeasure() {}

		virtual int     FromString(CString &sValue)
		{
            this->m_eValue = _wtoi( sValue );

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;

            sResult.Format( _T("%d"), this->m_eValue);

			return sResult;
		}


		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CEighthPointMeasure)

		double ToPoints()
		{
            return this->m_eValue / 8.0;
		}
		double ToMM()
		{
            return Pt_To_Mm(this->m_eValue / 8.0);
		}
		double ToInches()
		{
            return this->m_eValue / 576.0;
		}

		int    FromPoints(double dValue)
		{
            this->m_eValue = dValue * 8;
            return this->m_eValue;
		}
		int    FromInches(double dValue)
		{
            this->m_eValue = dValue * 576;
            return this->m_eValue;
		}
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

	template<EEm eDefValue = emNone>
	class CEm : public CSimpleType<EEm, eDefValue>
	{
	public:
		CEm() {}

		virtual EEm     FromString(CString &sValue)
		{
            if      ( _T("circle")   == sValue ) this->m_eValue = emCircle;
            else if ( _T("comma")    == sValue ) this->m_eValue = emComma;
            else if ( _T("dot")      == sValue ) this->m_eValue = emDot;
            else if ( _T("none")     == sValue ) this->m_eValue = emNone;
            else if ( _T("underDot") == sValue ) this->m_eValue = emUnderDot;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case emCircle   : return _T("circle");
			case emComma    : return _T("comma");
			case emDot      : return _T("dot");
			case emNone     : return _T("none");
			case emUnderDot : return _T("underDot");
			default         : return _T("none");
			}
		}

		SimpleType_FromString     (EEm)
		SimpleType_Operator_Equal (CEm)
	};



	//--------------------------------------------------------------------------------
	// FFHelpTextVal 17.18.25 (Part 1)
	//--------------------------------------------------------------------------------		

	class CFFHelpTextVal
	{
	public:
		CFFHelpTextVal() {}

		CString GetValue() const
		{
			return m_sValue;
		}

		void    SetValue(CString &sValue)
		{
			if ( sValue.GetLength() <= 256 )
				m_sValue = sValue;
			else
				m_sValue = sValue.Mid( 0, 256 );
		}


		CString FromString(CString &sValue)
		{
			if ( sValue.GetLength() <= 256 )
				m_sValue = sValue;
			else
				m_sValue = sValue.Mid( 0, 256 );

			return m_sValue;
		}

		CString ToString  () const 
		{
			return m_sValue;
		}

		SimpleType_FromString2    (CString)
		SimpleType_Operator_Equal (CFFHelpTextVal)
	private:

		CString m_sValue;
	};

	//--------------------------------------------------------------------------------
	// FFName 17.18.26 (Part 1)
	//--------------------------------------------------------------------------------		

	class CFFName
	{
	public:
		CFFName() {}

		CString GetValue() const
		{
			return m_sValue;
		}

		void    SetValue(CString &sValue)
		{
			if ( sValue.GetLength() <= 65 )
				m_sValue = sValue;
			else
				m_sValue = sValue.Mid( 0, 65 );
		}


		CString FromString(CString &sValue)
		{
			if ( sValue.GetLength() <= 65 )
				m_sValue = sValue;
			else
				m_sValue = sValue.Mid( 0, 65 );

			return m_sValue;
		}

		CString ToString  () const 
		{
			return m_sValue;
		}

		SimpleType_FromString2    (CString)
		SimpleType_Operator_Equal (CFFName)
	private:

		CString m_sValue;
	};

	//--------------------------------------------------------------------------------
	// FFStatusTextVal 17.18.27 (Part 1)
	//--------------------------------------------------------------------------------		

	class CFFStatusTextVal
	{
	public:
		CFFStatusTextVal() {}

		CString GetValue() const
		{
			return m_sValue;
		}

		void    SetValue(CString &sValue)
		{
			if ( sValue.GetLength() <= 140 )
				m_sValue = sValue;
			else
				m_sValue = sValue.Mid( 0, 140 );
		}


		CString FromString(CString &sValue)
		{
			if ( sValue.GetLength() <= 140 )
				m_sValue = sValue;
			else
				m_sValue = sValue.Mid( 0, 140 );

			return m_sValue;
		}

		CString ToString  () const 
		{
			return m_sValue;
		}

		SimpleType_FromString2    (CString)
		SimpleType_Operator_Equal (CFFStatusTextVal)
	private:

		CString m_sValue;
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

	template<EFFTextType eDefValue = fftexttypeRegular>
	class CFFTextType : public CSimpleType<EFFTextType, eDefValue>
	{
	public:
		CFFTextType() {}

		virtual EFFTextType FromString(CString &sValue)
		{
            if      ( _T("calculated")  == sValue ) this->m_eValue = fftexttypeCalculated;
            else if ( _T("currentDate") == sValue ) this->m_eValue = fftexttypeCurrentDate;
            else if ( _T("currentTime") == sValue ) this->m_eValue = fftexttypeCurrentTime;
            else if ( _T("date")        == sValue ) this->m_eValue = fftexttypeDate;
            else if ( _T("number")      == sValue ) this->m_eValue = fftexttypeNumber;
            else if ( _T("regular")     == sValue ) this->m_eValue = fftexttypeRegular;
            else                                    this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case fftexttypeCalculated  : return _T("calculated");
			case fftexttypeCurrentDate : return _T("currentDate");
			case fftexttypeCurrentTime : return _T("currentTime");
			case fftexttypeDate        : return _T("date");
			case fftexttypeNumber      : return _T("number");
			case fftexttypeRegular     : return _T("regular");
			default                    : return _T("regular");
			}
		}

		SimpleType_FromString     (EFFTextType)
		SimpleType_Operator_Equal (CFFTextType)
	};



	//--------------------------------------------------------------------------------
	// FldCharType 17.18.29 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EFldCharType
	{
		fldchartypeBegin    = 0,
		fldchartypeSeparate = 1,
		fldchartypeEnd      = 2
	};

	template<EFldCharType eDefVal = fldchartypeBegin>
	class CFldCharType : public CSimpleType<EFldCharType, eDefVal>
	{
	public:
		CFldCharType() {}

		virtual EFldCharType FromString(CString &sValue)
		{
            if      ( _T("begin")    == sValue ) this->m_eValue = fldchartypeBegin;
            else if ( _T("end")      == sValue ) this->m_eValue = fldchartypeEnd;
            else if ( _T("separate") == sValue ) this->m_eValue = fldchartypeSeparate;
            else                                 this->m_eValue = eDefVal;

            return this->m_eValue;
		}

		virtual CString      ToString  () const 
		{
            switch(this->m_eValue)
			{
			case fldchartypeBegin    : return _T("begin");
			case fldchartypeEnd      : return _T("end");
			case fldchartypeSeparate : return _T("separate");
			default                  : return _T("begin");
			}
		}

		SimpleType_FromString     (EFldCharType)
		SimpleType_Operator_Equal (CFldCharType)
	};

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

	template<EFontFamily eDefValue = fontfamilyAuto>
	class CFontFamily : public CSimpleType<EFontFamily, eDefValue>
	{
	public:
		CFontFamily() {}

		virtual EFontFamily FromString(CString &sValue)
		{
            if      ( _T("auto")       == sValue ) this->m_eValue = fontfamilyAuto;
            else if ( _T("decorative") == sValue ) this->m_eValue = fontfamilyDecorative;
            else if ( _T("modern")     == sValue ) this->m_eValue = fontfamilyModern;
            else if ( _T("roman")      == sValue ) this->m_eValue = fontfamilyRoman;
            else if ( _T("script")     == sValue ) this->m_eValue = fontfamilyScript;
            else if ( _T("swiss")      == sValue ) this->m_eValue = fontfamilySwiss;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case fontfamilyAuto       : return _T("auto");
			case fontfamilyDecorative : return _T("decorative");
			case fontfamilyModern     : return _T("modern");
			case fontfamilyRoman      : return _T("roman");
			case fontfamilyScript     : return _T("script");
			case fontfamilySwiss      : return _T("swiss");
			default                   : return _T("auto");
			}
		}

		SimpleType_FromString     (EFontFamily)
		SimpleType_Operator_Equal (CFontFamily)
	};
	//--------------------------------------------------------------------------------
	// FrameLayout 17.18.31 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EFrameLayout
	{
		framelayoutCols = 0,
		framelayoutNone = 1,
		framelayoutRows = 2
	};

	template<EFrameLayout eDefValue = framelayoutNone>
	class CFrameLayout : public CSimpleType<EFrameLayout, eDefValue>
	{
	public:
		CFrameLayout() {}

		virtual EFrameLayout FromString(CString &sValue)
		{
            if      ( _T("cols") == sValue ) this->m_eValue = framelayoutCols;
            else if ( _T("none") == sValue ) this->m_eValue = framelayoutNone;
            else if ( _T("rows") == sValue ) this->m_eValue = framelayoutRows;
            else                             this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString      ToString  () const 
		{
            switch(this->m_eValue)
			{
			case framelayoutCols : return _T("cols");
			case framelayoutNone : return _T("none");
			case framelayoutRows : return _T("rows");
			default              : return _T("none");
			}
		}

		SimpleType_FromString     (EFrameLayout)
		SimpleType_Operator_Equal (CFrameLayout)
	};

	//--------------------------------------------------------------------------------
	// FrameScrollbar 17.18.32 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EFrameScrollbar
	{
		framescrollbarAuto = 0,
		framescrollbarOff  = 1,
		framescrollbarOn   = 2
	};

	template<EFrameScrollbar eDefValue = framescrollbarAuto>
	class CFrameScrollbar : public CSimpleType<EFrameScrollbar, eDefValue>
	{
	public:
		CFrameScrollbar() {}

		virtual EFrameScrollbar FromString(CString &sValue)
		{
            if      ( _T("auto") == sValue ) this->m_eValue = framescrollbarAuto;
            else if ( _T("off")  == sValue ) this->m_eValue = framescrollbarOff;
            else if ( _T("on")   == sValue ) this->m_eValue = framescrollbarOn;
            else                             this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString         ToString  () const 
		{
            switch(this->m_eValue)
			{
			case framescrollbarAuto : return _T("auto");
			case framescrollbarOff  : return _T("off");
			case framescrollbarOn   : return _T("on");
			default                 : return _T("auto");
			}
		}

		SimpleType_FromString     (EFrameScrollbar)
		SimpleType_Operator_Equal (CFrameScrollbar)
	};

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

	template<EFtnEdn eDefValue = ftnednNormal>
	class CFtnEdn : public CSimpleType<EFtnEdn, eDefValue>
	{
	public:
		CFtnEdn() {}

		virtual EFtnEdn FromString(CString &sValue)
		{
            if      ( _T("continuationNotice")    == sValue ) this->m_eValue = ftnednContinuationNotice;
            else if ( _T("continuationSeparator") == sValue ) this->m_eValue = ftnednContinuationSeparator;
            else if ( _T("normal")                == sValue ) this->m_eValue = ftnednNormal;
            else if ( _T("separator")             == sValue ) this->m_eValue = ftnednSeparator;
            else                                              this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case ftnednContinuationNotice    : return _T("continuationNotice");
			case ftnednContinuationSeparator : return _T("continuationSeparator");
			case ftnednNormal                : return _T("normal");
			case ftnednSeparator             : return _T("separator");
			default                          : return _T("normal");
			}
		}

		SimpleType_FromString     (EFtnEdn)
		SimpleType_Operator_Equal (CFtnEdn)
	};



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

	template<EFtnPos eDefValue = ftnposSectEnd>
	class CFtnPos : public CSimpleType<EFtnPos, eDefValue>
	{
	public:
		CFtnPos() {}

		virtual EFtnPos FromString(CString &sValue)
		{
            if      ( _T("beneathText") == sValue ) this->m_eValue = ftnposBeneathText;
            else if ( _T("docEnd")      == sValue ) this->m_eValue = ftnposDocEnd;
            else if ( _T("pageBottom")  == sValue ) this->m_eValue = ftnposPageBottom;
            else if ( _T("sectEnd")     == sValue ) this->m_eValue = ftnposSectEnd;
            else                                    this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case ftnposBeneathText : return _T("beneathText");
			case ftnposDocEnd      : return _T("docEnd");
			case ftnposPageBottom  : return _T("pageBottom");
			case ftnposSectEnd     : return _T("sectEnd");
			default                : return _T("sectEnd");
			}
		}

		SimpleType_FromString     (EFtnPos)
		SimpleType_Operator_Equal (CFtnPos)
	};


	//--------------------------------------------------------------------------------
	// HAnchor 17.18.35 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EHAnchor
	{
		hanchorMargin = 0,
		hanchorPage   = 1,
		hanchorText   = 2
	};

	template<EHAnchor eDefValue = hanchorText>
	class CHAnchor : public CSimpleType<EHAnchor, eDefValue>
	{
	public:
		CHAnchor() {}

		virtual EHAnchor FromString(CString &sValue)
		{
            if      ( _T("margin") == sValue ) this->m_eValue = hanchorMargin;
            else if ( _T("page")   == sValue ) this->m_eValue = hanchorPage;
            else if ( _T("text")   == sValue ) this->m_eValue = hanchorText;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString  ToString  () const 
		{
            switch(this->m_eValue)
			{
			case hanchorMargin : return _T("margin");
			case hanchorPage   : return _T("page");
			case hanchorText   : return _T("text");
			default            : return _T("text");
			}
		}

		SimpleType_FromString     (EHAnchor)
		SimpleType_Operator_Equal (CHAnchor)
	};

	//--------------------------------------------------------------------------------
	// HdrFtr 17.18.36 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EHdrFtr
	{
		hdrftrDefault = 0,
		hdrftrEven    = 1,
		hdrftrFirst   = 2
	};

	template<EHdrFtr eDefValue = hdrftrDefault>
	class CHdrFtr : public CSimpleType<EHdrFtr, eDefValue>
	{
	public:
		CHdrFtr() {}

		virtual EHdrFtr FromString(CString &sValue)
		{
            if      ( _T("default") == sValue ) this->m_eValue = hdrftrDefault;
            else if ( _T("even")    == sValue ) this->m_eValue = hdrftrEven;
            else if ( _T("first")   == sValue ) this->m_eValue = hdrftrFirst;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case hdrftrDefault : return _T("default");
			case hdrftrEven    : return _T("even");
			case hdrftrFirst   : return _T("first");
			default            : return _T("default");
			}
		}

		SimpleType_FromString     (EHdrFtr)
		SimpleType_Operator_Equal (CHdrFtr)
	};

	//--------------------------------------------------------------------------------
	// HeightRule 17.18.37 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EHeightRule
	{
		heightruleAtLeast = 0,
		heightruleAuto    = 1,
		heightruleExact   = 2
	};

	template<EHeightRule eDefValue = heightruleAuto>
	class CHeightRule : public CSimpleType<EHeightRule, eDefValue>
	{
	public:
		CHeightRule() {}

		virtual EHeightRule FromString(CString &sValue)
		{
            if      ( _T("atLeast") == sValue ) this->m_eValue = heightruleAtLeast;
            else if ( _T("auto")    == sValue ) this->m_eValue = heightruleAuto;
            else if ( _T("exact")   == sValue ) this->m_eValue = heightruleExact;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case heightruleAtLeast : return _T("atLeast");
			case heightruleAuto    : return _T("auto");
			case heightruleExact   : return _T("exact");
			default                : return _T("auto");
			}
		}

		SimpleType_FromString     (EHeightRule)
		SimpleType_Operator_Equal (CHeightRule)
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

	template<EHexColor eDefValue = hexcolorAuto>
	class CHexColor : public CSimpleType<EHexColor, eDefValue>
	{
	public:
		CHexColor() 
		{
			m_unR = 0;
			m_unG = 0;
			m_unB = 0;
		}
		CHexColor(unsigned char r, unsigned char g, unsigned char b) 
		{
            this->m_eValue = hexcolorRGB;
			m_unR = r;
			m_unG = g;
			m_unB = b;
		}

        void SetColorByString ();

        virtual EHexColor FromString(CString &sValue)
		{
			if ( _T("auto") == sValue || _T("none") == sValue )
				this->m_eValue = hexcolorAuto;
			else
			{
				//В документации не написано, что цвет может приходить строкой, но в реальных докуентах встречается и word это разруливает.
				//CHighlightColor<highlightcolorNone> oHighlightColor(sValue);
				CHighlightColor<> oHighlightColor(sValue);
				if(SimpleTypes::highlightcolorNone != oHighlightColor.GetValue())
				{
					this->m_eValue = hexcolorRGB;
					m_unR = oHighlightColor.Get_R();
					m_unG = oHighlightColor.Get_G();
					m_unB = oHighlightColor.Get_B();
				}
				else if ( 6 <= sValue.GetLength() )
				{
					this->m_eValue = hexcolorRGB;
					m_sValue = sValue.Mid( 0, 6 );
					Parse();
				}
				else if ( 3 == sValue.GetLength() )// a la #339 (Compo 3AP.docx)
				{
					this->m_eValue = hexcolorRGB;
					m_sValue = sValue;
					Parse3();
				}
				else   this->m_eValue = hexcolorAuto;//eDefValue;

			}

			return this->m_eValue;

		}

		virtual CString   ToString  () const 
		{
            switch(this->m_eValue)
			{
			case hexcolorAuto : return _T("auto");
			case hexcolorRGB  : 
			{
				CString sResult =_T("00");//alfa
				if(m_unR > 0x0f)
					sResult.AppendFormat(_T("%X"), m_unR);
				else
					sResult.AppendFormat(_T("0%X"), m_unR);
				if(m_unG > 0x0f)
					sResult.AppendFormat(_T("%X"), m_unG);
				else
					sResult.AppendFormat(_T("0%X"), m_unG);
				if(m_unB > 0x0f)
					sResult.AppendFormat(_T("%X"), m_unB);
				else
					sResult.AppendFormat(_T("0%X"), m_unB);
				return sResult;
			}
			default           : return _T("auto");
			}
		}

		SimpleType_FromString     (EHexColor)
		SimpleType_Operator_Equal (CHexColor)
		void Set_R(unsigned char R)
		{
			m_unR = R;
		}
		void Set_G(unsigned char G)
		{
			m_unG = G;
		}
		void Set_B(unsigned char B)
		{
			m_unB = B;
		}
		unsigned char Get_R() const
		{
			return m_unR;
		}
		unsigned char Get_G() const
		{
			return m_unG;
		}

		unsigned char Get_B() const
		{
			return m_unB;
		}
		unsigned char Get_A() const
		{
			return 255;
		}

	private:

		void Parse()
		{
			if ( m_sValue.GetLength() < 6 )
				return;

			m_unR = HexToInt( (int)m_sValue[1] ) + (unsigned char)(HexToInt( (int)m_sValue[0] ) << 4);
			m_unG = HexToInt( (int)m_sValue[3] ) + (unsigned char)(HexToInt( (int)m_sValue[2] ) << 4);
			m_unB = HexToInt( (int)m_sValue[5] ) + (unsigned char)(HexToInt( (int)m_sValue[4] ) << 4);
		}
		void Parse3()
		{
			if ( m_sValue.GetLength() < 3 )
				return;

			m_unR = HexToInt( (int)m_sValue[0] ) + (unsigned char)(HexToInt( (int)m_sValue[0]) << 4);
			m_unG = HexToInt( (int)m_sValue[1] ) + (unsigned char)(HexToInt( (int)m_sValue[1]) << 4);
			m_unB = HexToInt( (int)m_sValue[2] ) + (unsigned char)(HexToInt( (int)m_sValue[2]) << 4);
		}
		int	HexToInt(int nHex)
		{
			if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
			if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
			if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

			return 0;
		}

	private:

		CString       m_sValue;

		unsigned char m_unR;
		unsigned char m_unG;
		unsigned char m_unB;		
    };

	//--------------------------------------------------------------------------------
	// HexColorAuto 17.18.39 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EHexColorAuto
	{
		hexcolorautoAuto = 0
	};

	template<EHexColorAuto eDefValue = hexcolorAuto>
	class CHexColorAuto : public CSimpleType<EHexColorAuto, eDefValue>
	{
	public:
		CHexColorAuto() {}

		virtual EHexColorAuto FromString(CString &sValue)
		{
            this->m_eValue = hexcolorautoAuto;

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
			return _T("auto");
		}	
		SimpleType_FromString     (EHexColorAuto)
		SimpleType_Operator_Equal (CHexColorAuto)
	};
	//--------------------------------------------------------------------------------
	// HighlightColor 17.18.40 (Part 1)
	//--------------------------------------------------------------------------------		



	template<EHighlightColor eDefValue = highlightcolorNone>
	class CHighlightColor : public CSimpleType<EHighlightColor, eDefValue>
	{
	public:
		CHighlightColor() 
		{
			m_unR = 0;
			m_unG = 0;
			m_unB = 0;
			m_unA = 255;
		}
		CHighlightColor(const CHexColor<>& color) 
				: m_unR(color.Get_R()), m_unG(color.Get_G()), m_unB(color.Get_B()), m_unA(255) {}

		virtual EHighlightColor FromString(CString &sValue)
		{
			if      ( _T("black")       == sValue )
			{
                this->m_eValue = highlightcolorBlack;

				m_unR = 0x00;
				m_unG = 0x00;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( _T("blue")        == sValue )
			{
                this->m_eValue = highlightcolorBlue;
				m_unR = 0x00;
				m_unG = 0x00;
				m_unB = 0xFF;
				m_unA = 0xFF;
			}
			else if ( _T("cyan")        == sValue )
			{
                this->m_eValue = highlightcolorCyan;
				m_unR = 0x00;
				m_unG = 0xFF;
				m_unB = 0xFF;
				m_unA = 0xFF;
			}
			else if ( _T("darkBlue")    == sValue )
			{
                this->m_eValue = highlightcolorDarkBlue;
				m_unR = 0x00;
				m_unG = 0x00;
				m_unB = 0x8B;
				m_unA = 0xFF;
			}
			else if ( _T("darkCyan")    == sValue )
			{
                this->m_eValue = highlightcolorDarkCyan;
				m_unR = 0x00;
				m_unG = 0x8B;
				m_unB = 0x8B;
				m_unA = 0xFF;
			}
			else if ( _T("darkGray")    == sValue )
			{
                this->m_eValue = highlightcolorDarkGray;
				m_unR = 0xA9;
				m_unG = 0xA9;
				m_unB = 0xA9;
				m_unA = 0xFF;
			}
			else if ( _T("darkGreen")   == sValue )
			{
                this->m_eValue = highlightcolorDarkGreen;
				m_unR = 0x00;
				m_unG = 0x64;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( _T("darkMagenta") == sValue )
			{
                this->m_eValue = highlightcolorDarkMagenta;
				m_unR = 0x80;
				m_unG = 0x00;
				m_unB = 0x80;
				m_unA = 0xFF;
			}
			else if ( _T("darkRed")     == sValue )
			{
                this->m_eValue = highlightcolorDarkRed;
				m_unR = 0x8B;
				m_unG = 0x00;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( _T("darkYellow")  == sValue )
			{
                this->m_eValue = highlightcolorDarkYellow;
				m_unR = 0x80;
				m_unG = 0x80;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( _T("green")       == sValue )
			{
                this->m_eValue = highlightcolorGreen;
				m_unR = 0x00;
				m_unG = 0xFF;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( _T("lightGray")   == sValue )
			{
                this->m_eValue = highlightcolorLightGray;
				m_unR = 0xD3;
				m_unG = 0xD3;
				m_unB = 0xD3;
				m_unA = 0xFF;
			}
			else if ( _T("magenta")     == sValue )
			{
                this->m_eValue = highlightcolorMagenta;
				m_unR = 0xFF;
				m_unG = 0x00;
				m_unB = 0xFF;
				m_unA = 0xFF;
			}
			else if ( _T("none")        == sValue )
			{
                this->m_eValue = highlightcolorNone;
				m_unR = 0x00;
				m_unG = 0x00;
				m_unB = 0x00;
				m_unA = 0x00;
			}
			else if ( _T("red")         == sValue )
			{
                this->m_eValue = highlightcolorRed;
				m_unR = 0xFF;
				m_unG = 0x00;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( _T("white")       == sValue )
			{
                this->m_eValue = highlightcolorWhite;
				m_unR = 0xFF;
				m_unG = 0xFF;
				m_unB = 0xFF;
				m_unA = 0xFF;
			}
			else if ( _T("yellow")      == sValue )
			{
                this->m_eValue = highlightcolorYellow;
				m_unR = 0xFF;
				m_unG = 0xFF;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else
			{
                this->m_eValue = eDefValue;

				// Инициализируем цвет
                FromString( ToString() );
			}

            return this->m_eValue;
		}

		virtual CString         ToString  () const 
		{
            switch(this->m_eValue)
			{
			case highlightcolorBlack       : return _T("black");
			case highlightcolorBlue        : return _T("blue");
			case highlightcolorCyan        : return _T("cyan");
			case highlightcolorDarkBlue    : return _T("darkBlue");
			case highlightcolorDarkCyan    : return _T("darkCyan");
			case highlightcolorDarkGray    : return _T("darkGray");
			case highlightcolorDarkGreen   : return _T("darkGreen");
			case highlightcolorDarkMagenta : return _T("darkMagenta");
			case highlightcolorDarkRed     : return _T("darkRed");
			case highlightcolorDarkYellow  : return _T("darkYellow");
			case highlightcolorGreen       : return _T("green");
			case highlightcolorLightGray   : return _T("lightGray");
			case highlightcolorMagenta     : return _T("magenta");
			case highlightcolorNone        : return _T("none");
			case highlightcolorRed         : return _T("red");
			case highlightcolorWhite       : return _T("white");
			case highlightcolorYellow      : return _T("yellow");
			default                        : return _T("none");
			}
		}

		SimpleType_FromString     (EHighlightColor)
		SimpleType_Operator_Equal (CHighlightColor)
		unsigned char Get_R() const
		{
			return m_unR;
		}
		unsigned char Get_G() const
		{
			return m_unG;
		}

		unsigned char Get_B() const
		{
			return m_unB;
		}

		unsigned char Get_A() const
		{
			return m_unA;
		}

	private:

		unsigned char m_unR;
		unsigned char m_unG;
		unsigned char m_unB;
		unsigned char m_unA;

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

	template<EHint eDefValue = hintDefault>
	class CHint : public CSimpleType<EHint, eDefValue>
	{
	public:
		CHint() {}

		virtual EHint   FromString(CString &sValue)
		{
            if      ( _T("cs")       == sValue ) this->m_eValue = hintCs;
            else if ( _T("default")  == sValue ) this->m_eValue = hintDefault;
            else if ( _T("eastAsia") == sValue ) this->m_eValue = hintEastAsia;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case hintCs       : return _T("cs");
			case hintDefault  : return _T("default");
			case hintEastAsia : return _T("eastAsia");
			default           : return _T("default");
			}
		}

		SimpleType_FromString     (EHint)
		SimpleType_Operator_Equal (CHint)
	};



	//--------------------------------------------------------------------------------
	// HpsMeasure 17.18.42 (Part 1)
	//--------------------------------------------------------------------------------		

	class CHpsMeasure : public CUniversalMeasure
	{
	public:
		CHpsMeasure() {}

		virtual double  FromString(CString &sValue)
		{
			Parse(sValue, 2);

			m_dValue = fabs( m_dValue );

			return m_dValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;

			sResult.Format( _T("%fpt"), m_dValue);

			return sResult;
		}
		long ToHps() const
		{
			return (long)(2 * m_dValue);
		}

		virtual double FromPoints(double dValue)
		{
			m_dValue = fabs( dValue );
			return m_dValue;
		}
		virtual double FromInches(double dValue)
		{
			m_dValue = fabs( dValue * 72.0 );
			return m_dValue;
		}
		SimpleType_FromString          (double)
		SimpleType_Operator_Equal      (CHpsMeasure)
		UniversalMeasure_AdditionalOpearators(CHpsMeasure)
	};

	//--------------------------------------------------------------------------------
	// InfoTextType 17.18.43 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EInfoTextType
	{
		infotexttypeAutoText = 0,
		infotexttypeText     = 1
	};

	template<EInfoTextType eDefValue = infotexttypeText>
	class CInfoTextType : public CSimpleType<EInfoTextType, eDefValue>
	{
	public:
		CInfoTextType() {}

		virtual EInfoTextType FromString(CString &sValue)
		{
            if      ( _T("autoText") == sValue ) this->m_eValue = infotexttypeAutoText;
            else if ( _T("text")     == sValue ) this->m_eValue = infotexttypeText;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
            switch(this->m_eValue)
			{
			case infotexttypeAutoText : return _T("autoText");
			case infotexttypeText     : return _T("text");
			default                   : return _T("text");
			}
		}

		SimpleType_FromString     (EInfoTextType)
		SimpleType_Operator_Equal (CInfoTextType)
	};

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

	template<EJc eDefValue = jcLeft>
	class CJc : public CSimpleType<EJc, eDefValue>
	{
	public:
		CJc() {}

		virtual EJc     FromString(CString &sValue)
		{
            if      ( _T("both")           == sValue ) this->m_eValue = jcBoth;
            else if ( _T("center")         == sValue ) this->m_eValue = jcCenter;
            else if ( _T("distribute")     == sValue ) this->m_eValue = jcDistribute;
            else if ( _T("end")            == sValue ) this->m_eValue = jcEnd;
            else if ( _T("highKashida")    == sValue ) this->m_eValue = jcHighKashida;
            else if ( _T("lowKashida")     == sValue ) this->m_eValue = jcLowKashida;
            else if ( _T("mediumKashida")  == sValue ) this->m_eValue = jcMediumKashida;
            else if ( _T("numTab")         == sValue ) this->m_eValue = jcNumTab;
            else if ( _T("start")          == sValue ) this->m_eValue = jcStart;
            else if ( _T("thaiDistribute") == sValue ) this->m_eValue = jcThaiDistribute;
            else if ( _T("left")           == sValue ) this->m_eValue = jcLeft;
            else if ( _T("right")          == sValue ) this->m_eValue = jcRight;
            else                                       this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case jcBoth            : return _T("both");
			case jcCenter          : return _T("center");
			case jcDistribute      : return _T("distribute");
			case jcEnd             : return _T("end");
			case jcHighKashida     : return _T("highKashida");
			case jcLowKashida      : return _T("lowKashida");
			case jcMediumKashida   : return _T("mediumKashida");
			case jcNumTab          : return _T("numTab");
			case jcStart           : return _T("start");
			case jcThaiDistribute  : return _T("thaiDistribute");
			case jcLeft            : return _T("left");
			case jcRight           : return _T("right");
			default                : return _T("left");
			}
		}

		SimpleType_FromString     (EJc)
		SimpleType_Operator_Equal (CJc)
	};
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

	template<EJcTable eDefValue = jctableLeft>
	class CJcTable : public CSimpleType<EJcTable, eDefValue>
	{
	public:
		CJcTable() {}

		virtual EJcTable FromString(CString &sValue)
		{
            if      ( _T("center")         == sValue ) this->m_eValue = jctableCenter;
            else if ( _T("end")            == sValue ) this->m_eValue = jctableEnd;
            else if ( _T("start")          == sValue ) this->m_eValue = jctableStart;
            else if ( _T("left")           == sValue ) this->m_eValue = jctableLeft;
            else if ( _T("right")          == sValue ) this->m_eValue = jctableRight;
            else                                       this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString  ToString  () const 
		{
            switch(this->m_eValue)
			{
			case jctableCenter : return _T("center");
			case jctableEnd    : return _T("end");
			case jctableStart  : return _T("start");
			case jctableLeft   : return _T("left");
			case jctableRight  : return _T("right");
			default            : return _T("left");
			}
		}

		SimpleType_FromString     (EJcTable)
		SimpleType_Operator_Equal (CJcTable)
	};
	//--------------------------------------------------------------------------------
	// LevelSuffix 17.18.46 (Part 1)
	//--------------------------------------------------------------------------------		

	enum ELevelSuffix
	{
		levelsuffixNothing = 0,
		levelsuffixSpace   = 1,
		levelsuffixTab     = 2
	};

	template<ELevelSuffix eDefValue = levelsuffixNothing>
	class CLevelSuffix : public CSimpleType<ELevelSuffix, eDefValue>
	{
	public:
		CLevelSuffix() {}

		virtual ELevelSuffix FromString(CString &sValue)
		{
            if      ( _T("nothing") == sValue ) this->m_eValue = levelsuffixNothing;
            else if ( _T("space")   == sValue ) this->m_eValue = levelsuffixSpace;
            else if ( _T("tab")     == sValue ) this->m_eValue = levelsuffixTab;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString      ToString  () const 
		{
            switch(this->m_eValue)
			{
			case levelsuffixNothing : return _T("nothing");
			case levelsuffixSpace   : return _T("space");
			case levelsuffixTab     : return _T("tab");
			default                 : return _T("nothing");
			}
		}

		SimpleType_FromString     (ELevelSuffix)
		SimpleType_Operator_Equal (CLevelSuffix)
	};



	//--------------------------------------------------------------------------------
	// LineNumberRestart 17.18.47 (Part 1)
	//--------------------------------------------------------------------------------		

	enum ELineNumberRestart
	{
		linenumberrestartContinious = 0,
		linenumberrestartNewPage    = 1,
		linenumberrestartNewSection = 2
	};

	template<ELineNumberRestart eDefValue = linenumberrestartNewPage>
	class CLineNumberRestart : public CSimpleType<ELineNumberRestart, eDefValue>
	{
	public:
		CLineNumberRestart() {}

		virtual ELineNumberRestart FromString(CString &sValue)
		{
            if      ( _T("continuous") == sValue ) this->m_eValue = linenumberrestartContinious;
            else if ( _T("newPage")    == sValue ) this->m_eValue = linenumberrestartNewPage;
            else if ( _T("newSection") == sValue ) this->m_eValue = linenumberrestartNewSection;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case linenumberrestartContinious : return _T("continuous");
			case linenumberrestartNewPage    : return _T("newPage");
			case linenumberrestartNewSection : return _T("newSection");
			default                          : return _T("newPage");
			}
		}

		SimpleType_FromString     (ELineNumberRestart)
		SimpleType_Operator_Equal (CLineNumberRestart)
	};


	//--------------------------------------------------------------------------------
	// LineSpacingRule 17.18.48 (Part 1)
	//--------------------------------------------------------------------------------		

	enum ELineSpacingRule
	{
		linespacingruleAtLeast = 0,
		linespacingruleAuto    = 1,
		linespacingruleExact   = 2
	};

	template<ELineSpacingRule eDefValue = linespacingruleAuto>
	class CLineSpacingRule : public CSimpleType<ELineSpacingRule, eDefValue>
	{
	public:
		CLineSpacingRule() {}

		virtual ELineSpacingRule FromString(CString &sValue)
		{
            if      ( _T("atLeast") == sValue ) this->m_eValue = linespacingruleAtLeast;
            else if ( _T("auto")    == sValue ) this->m_eValue = linespacingruleAuto;
            else if ( _T("exact")   == sValue ) this->m_eValue = linespacingruleExact;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString          ToString  () const 
		{
            switch(this->m_eValue)
			{
			case linespacingruleAtLeast : return _T("atLeast");
			case linespacingruleAuto    : return _T("auto");
			case linespacingruleExact   : return _T("exact");
			default                     : return _T("auto");
			}
		}

		SimpleType_FromString     (ELineSpacingRule)
		SimpleType_Operator_Equal (CLineSpacingRule)
	};

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

	template<ELock eDefValue = lockUnlocked>
	class CLock : public CSimpleType<ELock, eDefValue>
	{
	public:
		CLock() {}

		virtual ELock   FromString(CString &sValue)
		{
            if      ( _T("contentLocked")    == sValue ) this->m_eValue = lockContentLocked;
            else if ( _T("sdtContentLocked") == sValue ) this->m_eValue = lockSdtContentLocked;
            else if ( _T("sdtLocked")        == sValue ) this->m_eValue = lockSdtLocked;
            else if ( _T("unlocked")         == sValue ) this->m_eValue = lockUnlocked;
            else                                         this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case lockContentLocked    : return _T("contentLocked");
			case lockSdtContentLocked : return _T("sdtContentLocked");
			case lockSdtLocked        : return _T("sdtLocked");
			case lockUnlocked         : return _T("unlocked");
			default                   : return _T("unlocked");
			}
		}

		SimpleType_FromString     (ELock)
		SimpleType_Operator_Equal (CLock)
	};


	//--------------------------------------------------------------------------------
	// LongHexNumber 17.18.50 (Part 1)
	//--------------------------------------------------------------------------------		
	template<int nDefValue = 0>
	class CLongHexNumber : public CSimpleType<int, nDefValue>
	{
	public:
		CLongHexNumber() {}

		virtual int     FromString(CString &sValue)
		{
			if ( !Parse(sValue) )
			{
                this->m_eValue = nDefValue;
			}

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;
            sResult.Format( _T("%08X"), this->m_eValue);

			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CLongHexNumber)
	private:

		bool Parse(CString &sValue)
		{
			if ( sValue.GetLength() < 8 )
				return false;

			bool bResult = true;

            this->m_eValue  = HexToInt( (int)sValue[7], bResult );
            this->m_eValue += HexToInt( (int)sValue[6], bResult ) <<  4;
            this->m_eValue += HexToInt( (int)sValue[5], bResult ) <<  8;
            this->m_eValue += HexToInt( (int)sValue[4], bResult ) << 12;
            this->m_eValue += HexToInt( (int)sValue[3], bResult ) << 16;
            this->m_eValue += HexToInt( (int)sValue[2], bResult ) << 20;
            this->m_eValue += HexToInt( (int)sValue[1], bResult ) << 24;
            this->m_eValue += HexToInt( (int)sValue[0], bResult ) << 28;

			return bResult;
		}

		int	HexToInt(int nHex, bool &bResult)
		{
			if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
			if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
			if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

			bResult = false;

			return 0;
		}

	};


	//--------------------------------------------------------------------------------
	// MacroName 17.18.51 (Part 1)
	//--------------------------------------------------------------------------------		

	class CMacroName
	{
	public:
		CMacroName() {}

		CString GetValue() const
		{
			return m_sValue;
		}

		void    SetValue(CString &sValue)
		{
			if ( sValue.GetLength() <= 33 )
				m_sValue = sValue;
			else
				m_sValue = sValue.Mid( 0, 33 );
		}


		CString FromString(CString &sValue)
		{
			if ( sValue.GetLength() <= 33 )
				m_sValue = sValue;
			else
				m_sValue = sValue.Mid( 0, 33 );

			return m_sValue;
		}

		CString ToString  () const 
		{
			return m_sValue;
		}

		SimpleType_FromString2    (CString)
		SimpleType_Operator_Equal (CMacroName)
	private:

		CString m_sValue;
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

	template<EMailMergeDataType eDefValue = mailmergeddatatypeTextFile>
	class CMailMergeDataType : public CSimpleType<EMailMergeDataType, eDefValue>
	{
	public:
		CMailMergeDataType() {}

		virtual EMailMergeDataType FromString(CString &sValue)
		{
            if      ( _T("native")      == sValue ) this->m_eValue = mailmergeddatatypeNative;
            else if ( _T("odbc")        == sValue ) this->m_eValue = mailmergeddatatypeODBC;
            else if ( _T("query")       == sValue ) this->m_eValue = mailmergeddatatypeQuery;
            else if ( _T("soap")        == sValue ) this->m_eValue = mailmergeddatatypeSoap;
            else if ( _T("spreadsheet") == sValue ) this->m_eValue = mailmergeddatatypeSpreadsheet;
            else if ( _T("textFile")    == sValue ) this->m_eValue = mailmergeddatatypeTextFile;
            else if ( _T("xQuery")      == sValue ) this->m_eValue = mailmergeddatatypeXQuery;
            else if ( _T("xmlFile")     == sValue ) this->m_eValue = mailmergeddatatypeXmlFile;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case mailmergeddatatypeNative      : return _T("native");
			case mailmergeddatatypeODBC        : return _T("odbc");
			case mailmergeddatatypeQuery       : return _T("query");
			case mailmergeddatatypeSoap        : return _T("soap");
			case mailmergeddatatypeSpreadsheet : return _T("spreadsheet");
			case mailmergeddatatypeTextFile    : return _T("textFile");
			case mailmergeddatatypeXQuery      : return _T("xQuery");
			case mailmergeddatatypeXmlFile     : return _T("xmlFile");
			default                            : return _T("textFile");
			}
		}

		SimpleType_FromString     (EMailMergeDataType)
		SimpleType_Operator_Equal (CMailMergeDataType)
	};



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

	template<EMailMergeDest eDefValue = mailmergeddestEmail>
	class CMailMergeDest : public CSimpleType<EMailMergeDest, eDefValue>
	{
	public:
		CMailMergeDest() {}

		virtual EMailMergeDest FromString(CString &sValue)
		{
            if      ( _T("email")       == sValue ) this->m_eValue = mailmergeddestEmail;
            else if ( _T("fax")         == sValue ) this->m_eValue = mailmergeddestFax;
            else if ( _T("newDocument") == sValue ) this->m_eValue = mailmergeddestNewDocument;
            else if ( _T("printer")     == sValue ) this->m_eValue = mailmergeddestPrinter;
            else                                    this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case mailmergeddestEmail       : return _T("email");
			case mailmergeddestFax         : return _T("fax");
			case mailmergeddestNewDocument : return _T("newDocument");
			case mailmergeddestPrinter     : return _T("printer");
			default                        : return _T("newDocument");
			}
		}

		SimpleType_FromString     (EMailMergeDest)
		SimpleType_Operator_Equal (CMailMergeDest)
	};



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

	template<EMailMergeDocType eDefValue = mailmergeddoctypeEmail>
	class CMailMergeDocType : public CSimpleType<EMailMergeDocType, eDefValue>
	{
	public:
		CMailMergeDocType() {}

		virtual EMailMergeDocType FromString(CString &sValue)
		{
            if      ( _T("catalog")       == sValue ) this->m_eValue = mailmergeddoctypeCatalog;
            else if ( _T("email")         == sValue ) this->m_eValue = mailmergeddoctypeEmail;
            else if ( _T("envelopes")     == sValue ) this->m_eValue = mailmergeddoctypeEnvelopes;
            else if ( _T("fax")           == sValue ) this->m_eValue = mailmergeddoctypeFax;
            else if ( _T("formLetters")   == sValue ) this->m_eValue = mailmergeddoctypeFormLetters;
            else if ( _T("mailingLabels") == sValue ) this->m_eValue = mailmergeddoctypeMailingLabels;
            else                                      this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case mailmergeddoctypeCatalog       : return _T("catalog");
			case mailmergeddoctypeEmail         : return _T("email");
			case mailmergeddoctypeEnvelopes     : return _T("envelopes");
			case mailmergeddoctypeFax           : return _T("fax");
			case mailmergeddoctypeFormLetters   : return _T("formLetters");
			case mailmergeddoctypeMailingLabels : return _T("mailingLabels");
			default                             : return _T("email");
			}
		}

		SimpleType_FromString     (EMailMergeDocType)
		SimpleType_Operator_Equal (CMailMergeDocType)
	};



	//--------------------------------------------------------------------------------
	// MailMergeOdsoFMDFieldType 17.18.55 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EMailMergeOdsoFMDFieldType
	{
		mailmergeodsofmdfieldtypeDbColumn = 0,
		mailmergeodsofmdfieldtypeNull     = 1
	};

	template<EMailMergeOdsoFMDFieldType eDefValue = mailmergeodsofmdfieldtypeNull>
	class CMailMergeOdsoFMDFieldType : public CSimpleType<EMailMergeOdsoFMDFieldType, eDefValue>
	{
	public:
		CMailMergeOdsoFMDFieldType() {}

		virtual EMailMergeOdsoFMDFieldType FromString(CString &sValue)
		{
            if      ( _T("dbColumn") == sValue ) this->m_eValue = mailmergeodsofmdfieldtypeDbColumn;
            else if ( _T("null")     == sValue ) this->m_eValue = mailmergeodsofmdfieldtypeNull;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString                    ToString  () const 
		{
            switch(this->m_eValue)
			{
			case mailmergeodsofmdfieldtypeDbColumn : return _T("dbColumn");
			case mailmergeodsofmdfieldtypeNull     : return _T("null");
			default                                : return _T("null");
			}
		}

		SimpleType_FromString     (EMailMergeOdsoFMDFieldType)
		SimpleType_Operator_Equal (CMailMergeOdsoFMDFieldType)
	};



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

	template<EMailMergeSourceType eDefValue = mailmergesourcetypeText>
	class CMailMergeSourceType : public CSimpleType<EMailMergeSourceType, eDefValue>
	{
	public:
		CMailMergeSourceType() {}

		virtual EMailMergeSourceType FromString(CString &sValue)
		{
            if      ( _T("addressBook") == sValue ) this->m_eValue = mailmergesourcetypeAddressBook;
            else if ( _T("database")    == sValue ) this->m_eValue = mailmergesourcetypeDatabase;
            else if ( _T("document1")   == sValue ) this->m_eValue = mailmergesourcetypeDocument1;
            else if ( _T("document2")   == sValue ) this->m_eValue = mailmergesourcetypeDocument2;
            else if ( _T("email")       == sValue ) this->m_eValue = mailmergesourcetypeEmail;
            else if ( _T("legacy")      == sValue ) this->m_eValue = mailmergesourcetypeLegacy;
            else if ( _T("master")      == sValue ) this->m_eValue = mailmergesourcetypeMaster;
            else if ( _T("native")      == sValue ) this->m_eValue = mailmergesourcetypeNative;
            else if ( _T("text")        == sValue ) this->m_eValue = mailmergesourcetypeText;
            else                                    this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString              ToString  () const 
		{
            switch(this->m_eValue)
			{
			case mailmergesourcetypeAddressBook : return _T("addressBook");
			case mailmergesourcetypeDatabase    : return _T("database");
			case mailmergesourcetypeDocument1   : return _T("document1");
			case mailmergesourcetypeDocument2   : return _T("document2");
			case mailmergesourcetypeEmail       : return _T("email");
			case mailmergesourcetypeLegacy      : return _T("legacy");
			case mailmergesourcetypeMaster      : return _T("master");
			case mailmergesourcetypeNative      : return _T("native");
			case mailmergesourcetypeText        : return _T("text");
			default                             : return _T("text");
			}
		}

		SimpleType_FromString     (EMailMergeSourceType)
		SimpleType_Operator_Equal (CMailMergeSourceType)
	};



	//--------------------------------------------------------------------------------
	// Merge 17.18.57 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EMerge
	{
		mergeContinue = 0,
		mergeRestart  = 1
	};

	template<EMerge eDefValue = mergeContinue>
	class CMerge : public CSimpleType<EMerge, eDefValue>
	{
	public:
		CMerge() {}

		virtual EMerge  FromString(CString &sValue)
		{
            if      ( _T("continue") == sValue ) this->m_eValue = mergeContinue;
            else if ( _T("restart")  == sValue ) this->m_eValue = mergeRestart;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case mergeContinue : return _T("continue");
			case mergeRestart  : return _T("restart");
			default            : return _T("restart");
			}
		}

		SimpleType_FromString     (EMerge)
		SimpleType_Operator_Equal (CMerge)
	};


	//--------------------------------------------------------------------------------
	// MultiLevelType 17.18.58 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EMultiLevelType
	{
		emultileveltypeHybridMultilevel = 0,
		emultileveltypeMultilevel       = 1,
		emultileveltypeSingleLevel      = 2
	};

	template<EMultiLevelType eDefValue = emultileveltypeSingleLevel>
	class CMultiLevelType : public CSimpleType<EMultiLevelType, eDefValue>
	{
	public:
		CMultiLevelType() {}

		virtual EMultiLevelType FromString(CString &sValue)
		{
            if      ( _T("hybridMultilevel") == sValue ) this->m_eValue = emultileveltypeHybridMultilevel;
            else if ( _T("multilevel")       == sValue ) this->m_eValue = emultileveltypeMultilevel;
            else if ( _T("singleLevel")      == sValue ) this->m_eValue = emultileveltypeSingleLevel;
            else                                         this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString         ToString  () const 
		{
            switch(this->m_eValue)
			{
			case emultileveltypeHybridMultilevel : return _T("hybridMultilevel");
			case emultileveltypeMultilevel       : return _T("multilevel");
			case emultileveltypeSingleLevel      : return _T("singleLevel");
			default                              : return _T("singleLevel");
			}
		}

		SimpleType_FromString     (EMultiLevelType)
		SimpleType_Operator_Equal (CMultiLevelType)
	};




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

	template<ENumberFormat eDefValue = numberformatNone>
	class CNumberFormat : public CSimpleType<ENumberFormat, eDefValue>
	{
	public:
		CNumberFormat() {}

		virtual ENumberFormat FromString(CString &sValue)
		{
            if      ( _T("aiueo")                        == sValue ) this->m_eValue = numberformatAiueo;
            else if ( _T("aiueoFullWidth")               == sValue ) this->m_eValue = numberformatAiueoFullWidth;
            else if ( _T("arabicAbjad")                  == sValue ) this->m_eValue = numberformatArabicAbjad;
            else if ( _T("arabicAlpha")                  == sValue ) this->m_eValue = numberformatArabicAlpha;
            else if ( _T("bahtText")                     == sValue ) this->m_eValue = numberformatBahtText;
            else if ( _T("bullet")                       == sValue ) this->m_eValue = numberformatBullet;
            else if ( _T("cardinalText")                 == sValue ) this->m_eValue = numberformatCardinalText;
            else if ( _T("chicago")                      == sValue ) this->m_eValue = numberformatChicago;
            else if ( _T("chineseCounting")              == sValue ) this->m_eValue = numberformatChineseCounting;
            else if ( _T("chineseCountingThousand")      == sValue ) this->m_eValue = numberformatChineseCountingThousand;
            else if ( _T("chineseLegalSimplified")       == sValue ) this->m_eValue = numberformatChineseLegalSimplified;
            else if ( _T("chosung")                      == sValue ) this->m_eValue = numberformatChosung;
            else if ( _T("custom")                       == sValue ) this->m_eValue = numberformatCustom;
            else if ( _T("decimal")                      == sValue ) this->m_eValue = numberformatDecimal;
            else if ( _T("decimalEnclosedCircle")        == sValue ) this->m_eValue = numberformatDecimalEnclosedCircle;
            else if ( _T("decimalEnclosedCircleChinese") == sValue ) this->m_eValue = numberformatDecimalEnclosedCircleChinese;
            else if ( _T("decimalEnclosedFullstop")      == sValue ) this->m_eValue = numberformatDecimalEnclosedFullstop;
            else if ( _T("decimalEnclosedParen")         == sValue ) this->m_eValue = numberformatDecimalEnclosedParen;
            else if ( _T("decimalFullWidth")             == sValue ) this->m_eValue = numberformatDecimalFullWidth;
            else if ( _T("decimalFullWidth2")            == sValue ) this->m_eValue = numberformatDecimalFullWidth2;
            else if ( _T("decimalHalfWidth")             == sValue ) this->m_eValue = numberformatDecimalHalfWidth;
            else if ( _T("decimalZero")                  == sValue ) this->m_eValue = numberformatDecimalZero;
            else if ( _T("dollarText")                   == sValue ) this->m_eValue = numberformatDollarText;
            else if ( _T("ganada")                       == sValue ) this->m_eValue = numberformatGanada;
            else if ( _T("hebrew1")                      == sValue ) this->m_eValue = numberformatHebrew1;
            else if ( _T("hebrew2")                      == sValue ) this->m_eValue = numberformatHebrew2;
            else if ( _T("hex")                          == sValue ) this->m_eValue = numberformatHex;
            else if ( _T("hindiConsonants")              == sValue ) this->m_eValue = numberformatHindiConsonants;
            else if ( _T("hindiCounting")                == sValue ) this->m_eValue = numberformatHindiCounting;
            else if ( _T("hindiNumbers")                 == sValue ) this->m_eValue = numberformatHindiNumbers;
            else if ( _T("hindiVowels")                  == sValue ) this->m_eValue = numberformatHindiVowels;
            else if ( _T("ideographDigital")             == sValue ) this->m_eValue = numberformatIdeographDigital;
            else if ( _T("ideographEnclosedCircle")      == sValue ) this->m_eValue = numberformatIdeographEnclosedCircle;
            else if ( _T("ideographLegalTraditional")    == sValue ) this->m_eValue = numberformatIdeographLegalTraditional;
            else if ( _T("ideographTraditional")         == sValue ) this->m_eValue = numberformatIdeographTraditional;
            else if ( _T("ideographZodiac")              == sValue ) this->m_eValue = numberformatIdeographZodiac;
            else if ( _T("ideographZodiacTraditional")   == sValue ) this->m_eValue = numberformatIdeographZodiacTraditional;
            else if ( _T("iroha")                        == sValue ) this->m_eValue = numberformatIroha;
            else if ( _T("irohaFullWidth")               == sValue ) this->m_eValue = numberformatIrohaFullWidth;
            else if ( _T("japaneseCounting")             == sValue ) this->m_eValue = numberformatJapaneseCounting;
            else if ( _T("japaneseDigitalTenThousand")   == sValue ) this->m_eValue = numberformatJapaneseDigitalTenThousand;
            else if ( _T("japaneseLegal")                == sValue ) this->m_eValue = numberformatJapaneseLegal;
            else if ( _T("koreanCounting")               == sValue ) this->m_eValue = numberformatKoreanCounting;
            else if ( _T("koreanDigital")                == sValue ) this->m_eValue = numberformatKoreanDigital;
            else if ( _T("koreanDigital2")               == sValue ) this->m_eValue = numberformatKoreanDigital2;
            else if ( _T("koreanLegal")                  == sValue ) this->m_eValue = numberformatKoreanLegal;
            else if ( _T("lowerLetter")                  == sValue ) this->m_eValue = numberformatLowerLetter;
            else if ( _T("lowerRoman")                   == sValue ) this->m_eValue = numberformatLowerRoman;
            else if ( _T("none")                         == sValue ) this->m_eValue = numberformatNone;
            else if ( _T("numberInDash")                 == sValue ) this->m_eValue = numberformatNumberInDash;
            else if ( _T("ordinal")                      == sValue ) this->m_eValue = numberformatOrdinal;
            else if ( _T("ordinalText")                  == sValue ) this->m_eValue = numberformatOrdinalText;
            else if ( _T("russianLower")                 == sValue ) this->m_eValue = numberformatRussianLower;
            else if ( _T("russianUpper")                 == sValue ) this->m_eValue = numberformatRussianUpper;
            else if ( _T("taiwaneseCounting")            == sValue ) this->m_eValue = numberformatTaiwaneseCounting;
            else if ( _T("taiwaneseCountingThousand")    == sValue ) this->m_eValue = numberformatTaiwaneseCountingThousand;
            else if ( _T("taiwaneseDigital")             == sValue ) this->m_eValue = numberformatTaiwaneseDigital;
            else if ( _T("thaiCounting")                 == sValue ) this->m_eValue = numberformatThaiCounting;
            else if ( _T("thaiLetters")                  == sValue ) this->m_eValue = numberformatThaiLetters;
            else if ( _T("thaiNumbers")                  == sValue ) this->m_eValue = numberformatThaiNumbers;
            else if ( _T("upperLetter")                  == sValue ) this->m_eValue = numberformatUpperLetter;
            else if ( _T("upperRoman")                   == sValue ) this->m_eValue = numberformatUpperRoman;
            else if ( _T("vietnameseCounting")           == sValue ) this->m_eValue = numberformatVietnameseCounting;
            else                                                     this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
            switch(this->m_eValue)
			{
			case numberformatAiueo                        : return _T("aiueo");
			case numberformatAiueoFullWidth               : return _T("aiueoFullWidth");
			case numberformatArabicAbjad                  : return _T("arabicAbjad");
			case numberformatArabicAlpha                  : return _T("arabicAlpha");
			case numberformatBahtText                     : return _T("bahtText");
			case numberformatBullet                       : return _T("bullet");
			case numberformatCardinalText                 : return _T("cardinalText");
			case numberformatChicago                      : return _T("chicago");
			case numberformatChineseCounting              : return _T("chineseCounting");
			case numberformatChineseCountingThousand      : return _T("chineseCountingThousand");
			case numberformatChineseLegalSimplified       : return _T("chineseLegalSimplified");
			case numberformatChosung                      : return _T("chosung");
			case numberformatCustom                       : return _T("custom");
			case numberformatDecimal                      : return _T("decimal");
			case numberformatDecimalEnclosedCircle        : return _T("decimalEnclosedCircle");
			case numberformatDecimalEnclosedCircleChinese : return _T("decimalEnclosedCircleChinese");
			case numberformatDecimalEnclosedFullstop      : return _T("decimalEnclosedFullstop");
			case numberformatDecimalEnclosedParen         : return _T("decimalEnclosedParen");
			case numberformatDecimalFullWidth             : return _T("decimalFullWidth");
			case numberformatDecimalFullWidth2            : return _T("decimalFullWidth2");
			case numberformatDecimalHalfWidth             : return _T("decimalHalfWidth");
			case numberformatDecimalZero                  : return _T("decimalZero");
			case numberformatDollarText                   : return _T("dollarText");
			case numberformatGanada                       : return _T("ganada");
			case numberformatHebrew1                      : return _T("hebrew1");
			case numberformatHebrew2                      : return _T("hebrew2");
			case numberformatHex                          : return _T("hex");
			case numberformatHindiConsonants              : return _T("hindiConsonants");
			case numberformatHindiCounting                : return _T("hindiCounting");
			case numberformatHindiNumbers                 : return _T("hindiNumbers");
			case numberformatHindiVowels                  : return _T("hindiVowels");
			case numberformatIdeographDigital             : return _T("ideographDigital");
			case numberformatIdeographEnclosedCircle      : return _T("ideographEnclosedCircle");
			case numberformatIdeographLegalTraditional    : return _T("ideographLegalTraditional");
			case numberformatIdeographTraditional         : return _T("ideographTraditional");
			case numberformatIdeographZodiac              : return _T("ideographZodiac");
			case numberformatIdeographZodiacTraditional   : return _T("ideographZodiacTraditional");
			case numberformatIroha                        : return _T("iroha");
			case numberformatIrohaFullWidth               : return _T("irohaFullWidth");
			case numberformatJapaneseCounting             : return _T("japaneseCounting");
			case numberformatJapaneseDigitalTenThousand   : return _T("japaneseDigitalTenThousand");
			case numberformatJapaneseLegal                : return _T("japaneseLegal");
			case numberformatKoreanCounting               : return _T("koreanCounting");
			case numberformatKoreanDigital                : return _T("koreanDigital");
			case numberformatKoreanDigital2               : return _T("koreanDigital2");
			case numberformatKoreanLegal                  : return _T("koreanLegal");
			case numberformatLowerLetter                  : return _T("lowerLetter");
			case numberformatLowerRoman                   : return _T("lowerRoman");
			case numberformatNone                         : return _T("none");
			case numberformatNumberInDash                 : return _T("numberInDash");
			case numberformatOrdinal                      : return _T("ordinal");
			case numberformatOrdinalText                  : return _T("ordinalText");
			case numberformatRussianLower                 : return _T("russianLower");
			case numberformatRussianUpper                 : return _T("russianUpper");
			case numberformatTaiwaneseCounting            : return _T("taiwaneseCounting");
			case numberformatTaiwaneseCountingThousand    : return _T("taiwaneseCountingThousand");
			case numberformatTaiwaneseDigital             : return _T("taiwaneseDigital");
			case numberformatThaiCounting                 : return _T("thaiCounting");
			case numberformatThaiLetters                  : return _T("thaiLetters");
			case numberformatThaiNumbers                  : return _T("thaiNumbers");
			case numberformatUpperLetter                  : return _T("upperLetter");
			case numberformatUpperRoman                   : return _T("upperRoman");
			case numberformatVietnameseCounting           : return _T("vietnameseCounting");
			default                                       : return _T("none");
			}
		}

		SimpleType_FromString     (ENumberFormat)
		SimpleType_Operator_Equal (CNumberFormat)
		ENumberFormat GetValue() const
		{
            return this->m_eValue;
		}
	};






	//--------------------------------------------------------------------------------
	// ObjectDrawAspect 17.18.60 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EObjectDrawAspect
	{
		objectdrawaspectContent = 0,
		objectdrawaspectIcon    = 1
	};

	template<EObjectDrawAspect eDefValue = objectdrawaspectContent>
	class CObjectDrawAspect : public CSimpleType<EObjectDrawAspect, eDefValue>
	{
	public:
		CObjectDrawAspect() {}

		virtual EObjectDrawAspect FromString(CString &sValue)
		{
            if      ( _T("content") == sValue ) this->m_eValue = objectdrawaspectContent;
            else if ( _T("icon")    == sValue ) this->m_eValue = objectdrawaspectIcon;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case objectdrawaspectContent : return _T("content");
			case objectdrawaspectIcon    : return _T("icon");
			default                      : return _T("content");
			}
		}

		SimpleType_FromString     (EObjectDrawAspect)
		SimpleType_Operator_Equal (CObjectDrawAspect)
	};


	//--------------------------------------------------------------------------------
	// ObjectUpdateMode 17.18.61 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EObjectUpdateMode
	{
		objectupdatemodeAlways = 0,
		objectupdatemodeOnCall = 1
	};

	template<EObjectUpdateMode eDefValue = objectupdatemodeAlways>
	class CObjectUpdateMode : public CSimpleType<EObjectUpdateMode, eDefValue>
	{
	public:
		CObjectUpdateMode() {}

		virtual EObjectUpdateMode FromString(CString &sValue)
		{
            if      ( _T("always") == sValue ) this->m_eValue = objectupdatemodeAlways;
            else if ( _T("onCall") == sValue ) this->m_eValue = objectupdatemodeOnCall;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case objectupdatemodeAlways : return _T("always");
			case objectupdatemodeOnCall : return _T("onCall");
			default                     : return _T("always");
			}
		}

		SimpleType_FromString     (EObjectUpdateMode)
		SimpleType_Operator_Equal (CObjectUpdateMode)
	};


	//--------------------------------------------------------------------------------
	// PageBorderDisplay 17.18.62 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EPageBorderDisplay
	{
		pageborderdisplayAllPages     = 0,
		pageborderdisplayFirstPage    = 1,
		pageborderdisplayNotFirstPage = 2
	};

	template<EPageBorderDisplay eDefValue = pageborderdisplayAllPages>
	class CPageBorderDisplay : public CSimpleType<EPageBorderDisplay, eDefValue>
	{
	public:
		CPageBorderDisplay() {}

		virtual EPageBorderDisplay FromString(CString &sValue)
		{
            if      ( _T("allPages")     == sValue ) this->m_eValue = pageborderdisplayAllPages;
            else if ( _T("firstPage")    == sValue ) this->m_eValue = pageborderdisplayFirstPage;
            else if ( _T("notFirstPage") == sValue ) this->m_eValue = pageborderdisplayNotFirstPage;
            else                                     this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case pageborderdisplayAllPages     : return _T("allPages");
			case pageborderdisplayFirstPage    : return _T("firstPage");
			case pageborderdisplayNotFirstPage : return _T("notFirstPage");
			default                            : return _T("allPages");
			}
		}

		SimpleType_FromString     (EPageBorderDisplay)
		SimpleType_Operator_Equal (CPageBorderDisplay)
	};




	//--------------------------------------------------------------------------------
	// PageBorderOffset 17.18.63 (Part1)
	//--------------------------------------------------------------------------------		

	enum EPageBorderOffset
	{
		pageborderoffsetPage = 0,
		pageborderoffsetText = 1
	};

	template<EPageBorderOffset eDefValue = pageborderoffsetPage>
	class CPageBorderOffset : public CSimpleType<EPageBorderOffset, eDefValue>
	{
	public:
		CPageBorderOffset() {}

		virtual EPageBorderOffset FromString(CString &sValue)
		{
            if      ( _T("page") == sValue ) this->m_eValue = pageborderoffsetPage;
            else if ( _T("text") == sValue ) this->m_eValue = pageborderoffsetText;
            else                             this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case pageborderoffsetPage : return _T("page");
			case pageborderoffsetText : return _T("text");
			default                   : return _T("page");
			}
		}

		SimpleType_FromString     (EPageBorderOffset)
		SimpleType_Operator_Equal (CPageBorderOffset)
	};




	//--------------------------------------------------------------------------------
	// PageBorderZOrder 17.18.64 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EPageBorderZOrder
	{
		pageborderzorderBack  = 0,
		pageborderzorderFront = 1
	};

	template<EPageBorderZOrder eDefValue = pageborderzorderBack>
	class CPageBorderZOrder : public CSimpleType<EPageBorderZOrder, eDefValue>
	{
	public:
		CPageBorderZOrder() {}

		virtual EPageBorderZOrder FromString(CString &sValue)
		{
            if      ( _T("back")  == sValue ) this->m_eValue = pageborderzorderBack;
            else if ( _T("front") == sValue ) this->m_eValue = pageborderzorderFront;
            else                             this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case pageborderzorderBack  : return _T("back");
			case pageborderzorderFront : return _T("front");
			default                    : return _T("back");
			}
		}

		SimpleType_FromString     (EPageBorderZOrder)
		SimpleType_Operator_Equal (CPageBorderZOrder)
	};


	//--------------------------------------------------------------------------------
	// PageOrientation 17.18.65 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EPageOrientation
	{
		pageorientLandscape = 0,
		pageorientPortrait  = 1
	};

	template<EPageOrientation eDefValue = pageorientPortrait>
	class CPageOrientation : public CSimpleType<EPageOrientation, eDefValue>
	{
	public:
		CPageOrientation() {}

		virtual EPageOrientation FromString(CString &sValue)
		{
            if      ( _T("landscape") == sValue ) this->m_eValue = pageorientLandscape;
            else if ( _T("portrait")  == sValue ) this->m_eValue = pageorientPortrait;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString          ToString  () const 
		{
            switch(this->m_eValue)
			{
			case pageorientLandscape : return _T("landscape");
			case pageorientPortrait  : return _T("portrait");
			default                  : return _T("portrait");
			}
		}

		SimpleType_FromString     (EPageOrientation)
		SimpleType_Operator_Equal (CPageOrientation)
	};


	//--------------------------------------------------------------------------------
	// Pitch 17.18.66 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EPitch
	{
		pitchDefault  = 0,
		pitchFixed    = 1,
		pitchVariable = 2
	};

	template<EPitch eDefValue = pitchDefault>
	class CPitch : public CSimpleType<EPitch, eDefValue>
	{
	public:
		CPitch() {}

		virtual EPitch  FromString(CString &sValue)
		{
            if      ( _T("default")  == sValue ) this->m_eValue = pitchDefault;
            else if ( _T("fixed")    == sValue ) this->m_eValue = pitchFixed;
            else if ( _T("variable") == sValue ) this->m_eValue = pitchVariable;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case pitchDefault  : return _T("default");
			case pitchFixed    : return _T("fixed");
			case pitchVariable : return _T("variable");
			default            : return _T("default");
			}
		}


		SimpleType_FromString     (EPitch)
		SimpleType_Operator_Equal (CPitch)
	};



	//--------------------------------------------------------------------------------
	// PixelsMeasure 17.18.67 (Part 1)
	//--------------------------------------------------------------------------------		

	template<int nDefValue = 0>
	class CPixelsMeasure : public CSimpleType<int, nDefValue>
	{
	public:
		CPixelsMeasure() {}

		virtual int     FromString(CString &sValue)
		{
            this->m_eValue = _wtoi( sValue );

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CPixelsMeasure)
	};

	//--------------------------------------------------------------------------------
	// PointMeasure 17.18.68 (Part 1)
	//--------------------------------------------------------------------------------		

	template<int nDefValue = 0>
	class CPointMeasure : public CSimpleType<int, nDefValue>
	{
	public:
		CPointMeasure() {}

		virtual int     FromString(CString &sValue)
		{
            this->m_eValue = _wtoi( sValue );

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CPointMeasure)
		double ToPoints() const
		{
            return (double)this->m_eValue;
		}
		double ToMM() const
		{
            return (double)Pt_To_Mm(this->m_eValue);
		}
		double ToInches() const
		{
            return this->m_eValue / 72.0;
		}

		void FromPoints(double dValue)
		{
            this->m_eValue = dValue;
		}

		void FromInches(double dValue)
		{
            this->m_eValue = dValue * 72.0;
		}

	};

	//--------------------------------------------------------------------------------
	// Proof 17.18.69 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EProof
	{
		proofClean = 0,
		proofDirty = 1
	};

	template<EProof eDefValue = proofDirty>
	class CProof : public CSimpleType<EProof, eDefValue>
	{
	public:
		CProof() {}

		virtual EProof  FromString(CString &sValue)
		{
            if      ( _T("clean") == sValue ) this->m_eValue = proofClean;
            else if ( _T("dirty") == sValue ) this->m_eValue = proofDirty;
            else                              this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case proofClean : return _T("clean");
			case proofDirty : return _T("dirty");
			default         : return _T("dirty");
			}
		}

		SimpleType_FromString     (EProof)
		SimpleType_Operator_Equal (CProof)
	};


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

	template<EProofErr eDefValue = prooferrSpellStart>
	class CProofErr : public CSimpleType<EProofErr, eDefValue>
	{
	public:
		CProofErr() {}

		virtual EProofErr FromString(CString &sValue)
		{
            if      ( _T("gramEnd")    == sValue ) this->m_eValue = prooferrGramEnd;
            else if ( _T("gramStart")  == sValue ) this->m_eValue = prooferrGramStart;
            else if ( _T("spellEnd")   == sValue ) this->m_eValue = prooferrSpellEnd;
            else if ( _T("spellStart") == sValue ) this->m_eValue = prooferrSpellStart;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString   ToString  () const 
		{
            switch(this->m_eValue)
			{
			case prooferrGramEnd    : return _T("gramEnd");
			case prooferrGramStart  : return _T("gramStart");
			case prooferrSpellEnd   : return _T("spellEnd");
			case prooferrSpellStart : return _T("spellStart");
			default                 : return _T("spellStart");
			}
		}

		SimpleType_FromString     (EProofErr)
		SimpleType_Operator_Equal (CProofErr)
	};



	//--------------------------------------------------------------------------------
	// PTabAlignment 17.18.71 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EPTabAlignment
	{
		ptabalignmentCenter = 0,
		ptabalignmentLeft   = 1,
		ptabalignmentRight  = 2
	};

	template<EPTabAlignment eDefValue = ptabalignmentLeft>
	class CPTabAlignment : public CSimpleType<EPTabAlignment, eDefValue>
	{
	public:
		CPTabAlignment() {}

		virtual EPTabAlignment FromString(CString &sValue)
		{
            if      ( _T("center") == sValue ) this->m_eValue = ptabalignmentCenter;
            else if ( _T("left")   == sValue ) this->m_eValue = ptabalignmentLeft;
            else if ( _T("right")  == sValue ) this->m_eValue = ptabalignmentRight;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case ptabalignmentCenter : return _T("center");
			case ptabalignmentLeft   : return _T("left");
			case ptabalignmentRight  : return _T("right");
			default                  : return _T("left");
			}
		}

		SimpleType_FromString     (EPTabAlignment)
		SimpleType_Operator_Equal (CPTabAlignment)
	};

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

	template<EPTabLeader eDefValue = ptableaderNone>
	class CPTabLeader : public CSimpleType<EPTabLeader, eDefValue>
	{
	public:
		CPTabLeader() {}

		virtual EPTabLeader FromString(CString &sValue)
		{
            if      ( _T("dot")        == sValue ) this->m_eValue = ptableaderDot;
            else if ( _T("hyphen")     == sValue ) this->m_eValue = ptableaderHyphen;
            else if ( _T("middleDot")  == sValue ) this->m_eValue = ptableaderMiddleDot;
            else if ( _T("none")       == sValue ) this->m_eValue = ptableaderNone;
            else if ( _T("underscore") == sValue ) this->m_eValue = ptableaderUnderscore;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case ptableaderDot        : return _T("dot");
			case ptableaderHyphen     : return _T("hyphen");
			case ptableaderMiddleDot  : return _T("middleDot");
			case ptableaderNone       : return _T("none");
			case ptableaderUnderscore : return _T("underscore");
			default                   : return _T("none");
			}
		}

		SimpleType_FromString     (EPTabLeader)
		SimpleType_Operator_Equal (CPTabLeader)
	};
	//--------------------------------------------------------------------------------
	// PTabRelativeTo 17.18.73 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EPTabRelativeTo
	{
		ptabrelativetoIndent = 0,
		ptabrelativetoMargin = 1
	};

	template<EPTabRelativeTo eDefValue = ptabrelativetoIndent>
	class CPTabRelativeTo : public CSimpleType<EPTabRelativeTo, eDefValue>
	{
	public:
		CPTabRelativeTo() {}

		virtual EPTabRelativeTo FromString(CString &sValue)
		{
            if      ( _T("indent") == sValue ) this->m_eValue = ptabrelativetoIndent;
            else if ( _T("margin") == sValue ) this->m_eValue = ptabrelativetoMargin;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString         ToString  () const 
		{
            switch(this->m_eValue)
			{
			case ptabrelativetoIndent : return _T("indent");
			case ptabrelativetoMargin : return _T("margin");
			default                   : return _T("indent");
			}
		}

		SimpleType_FromString     (EPTabRelativeTo)
		SimpleType_Operator_Equal (CPTabRelativeTo)
	};


	//--------------------------------------------------------------------------------
	// RestartNumber 17.18.74 (Part 1)
	//--------------------------------------------------------------------------------		

	enum ERestartNumber
	{
		restartnumberContinious = 0,
		restartnumberEachPage   = 1,
		restartnumberEachSect   = 2
	};

	template<ERestartNumber eDefValue = restartnumberContinious>
	class CRestartNumber : public CSimpleType<ERestartNumber, eDefValue>
	{
	public:
		CRestartNumber() {}

		virtual ERestartNumber FromString(CString &sValue)
		{
            if      ( _T("continuous") == sValue ) this->m_eValue = restartnumberContinious;
            else if ( _T("eachPage")   == sValue ) this->m_eValue = restartnumberEachPage;
            else if ( _T("eachSect")   == sValue ) this->m_eValue = restartnumberEachSect;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case restartnumberContinious : return _T("continuous");
			case restartnumberEachPage   : return _T("eachPage");
			case restartnumberEachSect   : return _T("eachSect");
			default                      : return _T("continuous");
			}
		}

		SimpleType_FromString     (ERestartNumber)
		SimpleType_Operator_Equal (CRestartNumber)
	};

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

	template<ERubyAlign eDefValue = erubyalignLeft>
	class CRubyAlign : public CSimpleType<ERubyAlign, eDefValue>
	{
	public:
		CRubyAlign() {}

		virtual ERubyAlign FromString(CString &sValue)
		{
            if      ( _T("center")           == sValue ) this->m_eValue = erubyalignCenter;
            else if ( _T("distributeLetter") == sValue ) this->m_eValue = erubyalignDistributeLetter;
            else if ( _T("distributeSpace")  == sValue ) this->m_eValue = erubyalignDistributeSpace;
            else if ( _T("left")             == sValue ) this->m_eValue = erubyalignLeft;
            else if ( _T("right")            == sValue ) this->m_eValue = erubyalignRight;
            else if ( _T("rightVertical")    == sValue ) this->m_eValue = erubyalignRightVertical;
            else                                         this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString    ToString  () const 
		{
            switch(this->m_eValue)
			{
			case erubyalignCenter           : return _T("center");
			case erubyalignDistributeLetter : return _T("distributeLetter");
			case erubyalignDistributeSpace  : return _T("distributeSpace");
			case erubyalignLeft             : return _T("left");
			case erubyalignRight            : return _T("right");
			case erubyalignRightVertical    : return _T("rightVertical");
			default                         : return _T("left");
			}
		}

		SimpleType_FromString     (ERubyAlign)
		SimpleType_Operator_Equal (CRubyAlign)
	};

	//--------------------------------------------------------------------------------
	// SdtDateMappingType 17.18.76 (Part 1)
	//--------------------------------------------------------------------------------		

	enum ESdtDateMappingType
	{
		sdtdatemappingtypeDate     = 0,
		sdtdatemappingtypeDateTime = 1,
		sdtdatemappingtypeText     = 2
	};

	template<ESdtDateMappingType eDefValue = sdtdatemappingtypeText>
	class CSdtDateMappingType : public CSimpleType<ESdtDateMappingType, eDefValue>
	{
	public:
		CSdtDateMappingType() {}

		virtual ESdtDateMappingType FromString(CString &sValue)
		{
            if      ( _T("date")     == sValue ) this->m_eValue = sdtdatemappingtypeDate;
            else if ( _T("dateTime") == sValue ) this->m_eValue = sdtdatemappingtypeDateTime;
            else if ( _T("text")     == sValue ) this->m_eValue = sdtdatemappingtypeText;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString             ToString  () const 
		{
            switch(this->m_eValue)
			{
			case sdtdatemappingtypeDate     : return _T("date");
			case sdtdatemappingtypeDateTime : return _T("dateTime");
			case sdtdatemappingtypeText     : return _T("text");
			default                         : return _T("text");
			}
		}

		SimpleType_FromString     (ESdtDateMappingType)
		SimpleType_Operator_Equal (CSdtDateMappingType)
	};

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

	template<ESectionMark eDefValue = sectionmarkNextPage>
	class CSectionMark : public CSimpleType<ESectionMark, eDefValue>
	{
	public:
		CSectionMark() {}

		virtual ESectionMark FromString(CString &sValue)
		{
            if      ( _T("continuous") == sValue ) this->m_eValue = sectionmarkContinious;
            else if ( _T("evenPage")   == sValue ) this->m_eValue = sectionmarkEvenPage;
            else if ( _T("nextColumn") == sValue ) this->m_eValue = sectionmarkNextColumn;
            else if ( _T("nextPage")   == sValue ) this->m_eValue = sectionmarkNextPage;
            else if ( _T("oddPage")    == sValue ) this->m_eValue = sectionmarkOddPage;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString      ToString  () const 
		{
            switch(this->m_eValue)
			{
			case sectionmarkContinious : return _T("continuous");
			case sectionmarkEvenPage   : return _T("evenPage");
			case sectionmarkNextColumn : return _T("nextColumn");
			case sectionmarkNextPage   : return _T("nextPage");
			case sectionmarkOddPage    : return _T("oddPage");
			default                    : return _T("nextPage");
			}
		}

		SimpleType_FromString     (ESectionMark)
		SimpleType_Operator_Equal (CSectionMark)
	};

	//--------------------------------------------------------------------------------
	// Shd 17.18.78 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EShd
	{
		shdClear                 =  0,
		shdDiagCross             =  1,
		shdDiagStripe            =  2,
		shdHorzCross             =  3,
		shdHorzStripe            =  4,
		shdNil                   =  5,
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

	template<EShd eDefValue = shdSolid>
	class CShd : public CSimpleType<EShd, eDefValue>
	{
	public:
		CShd() {}

		virtual EShd    FromString(CString &sValue)
		{
            if      ( _T("clear")                 == sValue ) this->m_eValue = shdClear;
            else if ( _T("diagCross")             == sValue ) this->m_eValue = shdDiagCross;
            else if ( _T("diagStripe")            == sValue ) this->m_eValue = shdDiagStripe;
            else if ( _T("horzCross")             == sValue ) this->m_eValue = shdHorzCross;
            else if ( _T("horzStripe")            == sValue ) this->m_eValue = shdHorzStripe;
            else if ( _T("nil")                   == sValue ) this->m_eValue = shdNil;
            else if ( _T("pct10")                 == sValue ) this->m_eValue = shdPct10;
            else if ( _T("pct12")                 == sValue ) this->m_eValue = shdPct12;
            else if ( _T("pct15")                 == sValue ) this->m_eValue = shdPct15;
            else if ( _T("pct20")                 == sValue ) this->m_eValue = shdPct20;
            else if ( _T("pct25")                 == sValue ) this->m_eValue = shdPct25;
            else if ( _T("pct30")                 == sValue ) this->m_eValue = shdPct30;
            else if ( _T("pct35")                 == sValue ) this->m_eValue = shdPct35;
            else if ( _T("pct37")                 == sValue ) this->m_eValue = shdPct37;
            else if ( _T("pct40")                 == sValue ) this->m_eValue = shdPct40;
            else if ( _T("pct45")                 == sValue ) this->m_eValue = shdPct45;
            else if ( _T("pct5")                  == sValue ) this->m_eValue = shdPct5;
            else if ( _T("pct50")                 == sValue ) this->m_eValue = shdPct50;
            else if ( _T("pct55")                 == sValue ) this->m_eValue = shdPct55;
            else if ( _T("pct60")                 == sValue ) this->m_eValue = shdPct60;
            else if ( _T("pct62")                 == sValue ) this->m_eValue = shdPct62;
            else if ( _T("pct65")                 == sValue ) this->m_eValue = shdPct65;
            else if ( _T("pct70")                 == sValue ) this->m_eValue = shdPct70;
            else if ( _T("pct75")                 == sValue ) this->m_eValue = shdPct75;
            else if ( _T("pct80")                 == sValue ) this->m_eValue = shdPct80;
            else if ( _T("pct85")                 == sValue ) this->m_eValue = shdPct85;
            else if ( _T("pct87")                 == sValue ) this->m_eValue = shdPct87;
            else if ( _T("pct90")                 == sValue ) this->m_eValue = shdPct90;
            else if ( _T("pct95")                 == sValue ) this->m_eValue = shdPct95;
            else if ( _T("reverseDiagStripe")     == sValue ) this->m_eValue = shdReverseDiagStripe;
            else if ( _T("solid")                 == sValue ) this->m_eValue = shdSolid;
            else if ( _T("thinDiagCross")         == sValue ) this->m_eValue = shdThinDiagCross;
            else if ( _T("thinDiagStripe")        == sValue ) this->m_eValue = shdThinDiagStripe;
            else if ( _T("thinHorzCross")         == sValue ) this->m_eValue = shdThinHorzCross;
            else if ( _T("thinHorzStripe")        == sValue ) this->m_eValue = shdThinHorzStripe;
            else if ( _T("thinReverseDiagStripe") == sValue ) this->m_eValue = shdThinReverseDiagStripe;
            else if ( _T("thinVertStripe")        == sValue ) this->m_eValue = shdThinVertStripe;
            else if ( _T("vertStripe")            == sValue ) this->m_eValue = shdVertStripe;
            else                                              this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case shdClear                 : return _T("clear");
			case shdDiagCross             : return _T("diagCross");
			case shdDiagStripe            : return _T("diagStripe");
			case shdHorzCross             : return _T("horzCross");
			case shdHorzStripe            : return _T("horzStripe");
			case shdNil                   : return _T("nil");
			case shdPct10                 : return _T("pct10");
			case shdPct12                 : return _T("pct12");
			case shdPct15                 : return _T("pct15");
			case shdPct20                 : return _T("pct20");
			case shdPct25                 : return _T("pct25");
			case shdPct30                 : return _T("pct30");
			case shdPct35                 : return _T("pct35");
			case shdPct37                 : return _T("pct37");
			case shdPct40                 : return _T("pct40");
			case shdPct45                 : return _T("pct45");
			case shdPct5                  : return _T("pct5");
			case shdPct50                 : return _T("pct50");
			case shdPct55                 : return _T("pct55");
			case shdPct60                 : return _T("pct60");
			case shdPct62                 : return _T("pct62");
			case shdPct65                 : return _T("pct65");
			case shdPct70                 : return _T("pct70");
			case shdPct75                 : return _T("pct75");
			case shdPct80                 : return _T("pct80");
			case shdPct85                 : return _T("pct85");
			case shdPct87                 : return _T("pct87");
			case shdPct90                 : return _T("pct90");
			case shdPct95                 : return _T("pct95");
			case shdReverseDiagStripe     : return _T("reverseDiagStripe");
			case shdSolid                 : return _T("solid");
			case shdThinDiagCross         : return _T("thinDiagCross");
			case shdThinDiagStripe        : return _T("thinDiagStripe");
			case shdThinHorzCross         : return _T("thinHorzCross");
			case shdThinHorzStripe        : return _T("thinHorzStripe");
			case shdThinReverseDiagStripe : return _T("thinReverseDiagStripe");
			case shdThinVertStripe        : return _T("thinVertStripe");
			case shdVertStripe            : return _T("vertStripe");
			default                       : return _T("solid");
			}
		}

		SimpleType_FromString     (EShd)
		SimpleType_Operator_Equal (CShd)
	};
	//--------------------------------------------------------------------------------
	// ShortHexNumber 17.18.79 (Part 1)
	//--------------------------------------------------------------------------------		
	template<int nDefValue = 0>
	class CShortHexNumber : public CSimpleType<int, nDefValue>
	{
	public:
		CShortHexNumber() {}

		virtual int     FromString(CString &sValue)
		{
			if ( !Parse(sValue) )
			{
                this->m_eValue = nDefValue;
			}

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;
            sResult.Format( _T("%04X"), this->m_eValue);

			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CShortHexNumber)
	private:

		bool Parse(CString &sValue)
		{
			int nLength = sValue.GetLength();

			bool bResult = true;
			if(nLength > 0)
			{
                this->m_eValue  = HexToInt( (int)sValue[nLength - 1], bResult );
				if(nLength > 1)
				{
                    this->m_eValue += HexToInt( (int)sValue[nLength - 2], bResult ) <<  4;
					if(nLength > 2)
					{
                        this->m_eValue += HexToInt( (int)sValue[nLength - 3], bResult ) <<  8;
						if(nLength > 3)
						{
                            this->m_eValue += HexToInt( (int)sValue[nLength - 4], bResult ) << 12;
						}
					}
				}
			}
			else
				bResult = false;

			return bResult;
		}

		int	HexToInt(int nHex, bool &bResult)
		{
			if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
			if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
			if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

			bResult = false;

			return 0;
		}

	};

	//--------------------------------------------------------------------------------
	// SignedHpsMeasure 17.18.80 (Part 1)
	//--------------------------------------------------------------------------------		

	class CSignedHpsMeasure : public CUniversalMeasure
	{
	public:
		CSignedHpsMeasure() {}

		virtual double  FromString(CString &sValue)
		{
			Parse(sValue, 2); 

			return m_dValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;

			sResult.Format( _T("%fpt"), m_dValue);

			return sResult;
		}

		SimpleType_FromString          (double)
		SimpleType_Operator_Equal      (CSignedHpsMeasure)
		UniversalMeasure_AdditionalOpearators(CSignedHpsMeasure)
	};

	//--------------------------------------------------------------------------------
	// SignedTwipsMeasure 17.18.81 (Part 1)
	//--------------------------------------------------------------------------------		

	class CSignedTwipsMeasure : public CUniversalMeasure
	{
	public:
		CSignedTwipsMeasure() {}

		virtual double  FromString(CString &sValue)
		{
			Parse(sValue, 20); 

			return m_dValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;

			if ( m_bUnit )
				sResult.Format( _T("%fpt"), m_dValue);
			else
				sResult.Format( _T("%d"), (int)(m_dValue * 20) );

			return sResult;
		}
		SimpleType_FromString          (double)
		SimpleType_Operator_Equal      (CSignedTwipsMeasure)
		UniversalMeasure_AdditionalOpearators(CSignedTwipsMeasure)
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

	template<EStyleSort eDefValue = stylesortName>
	class CStyleSort : public CSimpleType<EStyleSort, eDefValue>
	{
	public:
		CStyleSort() 
		{
			m_bNames = false;
		}

		virtual EStyleSort FromString(CString &sValue)
		{
			m_bNames = false;
            if      ( _T("basedOn")  == sValue || _T("0004") == sValue ) this->m_eValue = stylesortBasedOn;
            else if ( _T("default")  == sValue || _T("0002") == sValue ) this->m_eValue = stylesortDefault;
            else if ( _T("font")     == sValue || _T("0003") == sValue ) this->m_eValue = stylesortFont;
            else if ( _T("name")     == sValue || _T("0000") == sValue ) this->m_eValue = stylesortName;
            else if ( _T("priority") == sValue || _T("0001") == sValue ) this->m_eValue = stylesortPriority;
            else if ( _T("type")     == sValue || _T("0005") == sValue ) this->m_eValue = stylesortType;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString    ToString  () const 
		{
			if ( m_bNames )
			{
                switch(this->m_eValue)
				{
				case stylesortBasedOn  : return _T("basedOn");
				case stylesortDefault  : return _T("default");
				case stylesortFont     : return _T("font");
				case stylesortName     : return _T("name");
				case stylesortPriority : return _T("priority");
				case stylesortType     : return _T("type");
				default                : return _T("default");
				}
			}
			else
			{
                switch(this->m_eValue)
				{
				case stylesortBasedOn  : return _T("0004");
				case stylesortDefault  : return _T("0002");
				case stylesortFont     : return _T("0003");
				case stylesortName     : return _T("0000");
				case stylesortPriority : return _T("0001");
				case stylesortType     : return _T("0005");
				default                : return _T("0000");
				}
			}
		}
		SimpleType_FromString     (EStyleSort)
		SimpleType_Operator_Equal (CStyleSort)

		void WriteNames(bool bNames)
		{
			m_bNames = bNames;
		}

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

	template<EStyleType eDefValue = styletypeParagraph>
	class CStyleType : public CSimpleType<EStyleType, eDefValue>
	{
	public:
		CStyleType() {}

		virtual EStyleType FromString(CString &sValue)
		{
            if      ( _T("character") == sValue ) this->m_eValue = styletypeCharacter;
            else if ( _T("numbering") == sValue ) this->m_eValue = styletypeNumbering;
            else if ( _T("paragraph") == sValue ) this->m_eValue = styletypeParagraph;
            else if ( _T("table")     == sValue ) this->m_eValue = styletypeTable;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString    ToString  () const 
		{
            switch(this->m_eValue)
			{
			case styletypeCharacter : return _T("character");
			case styletypeNumbering : return _T("numbering");
			case styletypeParagraph : return _T("paragraph");
			case styletypeTable     : return _T("table");
			default                 : return _T("paragraph");
			}
		}

		SimpleType_FromString     (EStyleType)
		SimpleType_Operator_Equal (CStyleType)
	};



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

	template<ETabJc eDefValue = tabjcLeft> 
	class CTabJc : public CSimpleType<ETabJc, tabjcClear>
	{
	public:
		CTabJc() {}

		virtual ETabJc  FromString(CString &sValue)
		{
            if      ( _T("bar")     == sValue ) this->m_eValue = tabjcBar;
            else if ( _T("center")  == sValue ) this->m_eValue = tabjcCenter;
            else if ( _T("clear")   == sValue ) this->m_eValue = tabjcClear;
            else if ( _T("decimal") == sValue ) this->m_eValue = tabjcDecimal;
            else if ( _T("end")     == sValue ) this->m_eValue = tabjcEnd;
            else if ( _T("num")     == sValue ) this->m_eValue = tabjcNum;
            else if ( _T("start")   == sValue ) this->m_eValue = tabjcStart;
            else if ( _T("right")   == sValue ) this->m_eValue = tabjcRight;
            else if ( _T("left")    == sValue ) this->m_eValue = tabjcLeft;
            else                                this->m_eValue = tabjcClear;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			switch(m_eValue)
			{
			case tabjcBar     : return _T("bar");
			case tabjcCenter  : return _T("center");
			case tabjcClear   : return _T("clear");
			case tabjcDecimal : return _T("decimal");
			case tabjcEnd     : return _T("end");
			case tabjcNum     : return _T("num");
			case tabjcStart   : return _T("start");
			case tabjcRight   : return _T("right");
			case tabjcLeft    : return _T("left");
			default           : return _T("clear");
			}
		}

		SimpleType_FromString     (ETabJc)
		SimpleType_Operator_Equal (CTabJc)

		ETabJc GetValue() const
		{
            return this->m_eValue;
		}
	};


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

	template<ETabTlc eDefValue = tabtlcNone>
	class CTabTlc : public CSimpleType<ETabTlc, eDefValue>
	{
	public:
		CTabTlc() {}

		virtual ETabTlc FromString(CString &sValue)
		{
            if      ( _T("dot")        == sValue ) this->m_eValue = tabtlcDot;
            else if ( _T("heavy")      == sValue ) this->m_eValue = tabtlcHeavy;
            else if ( _T("hyphen")     == sValue ) this->m_eValue = tabtlcHyphen;
            else if ( _T("middleDot")  == sValue ) this->m_eValue = tabtlcMiddleDot;
            else if ( _T("none")       == sValue ) this->m_eValue = tabtlcNone;
            else if ( _T("underscore") == sValue ) this->m_eValue = tabtlcUnderscore;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case tabtlcDot        : return _T("dot");
			case tabtlcHeavy      : return _T("heavy");
			case tabtlcHyphen     : return _T("hyphen");
			case tabtlcMiddleDot  : return _T("middleDot");
			case tabtlcNone       : return _T("none");
			case tabtlcUnderscore : return _T("underscore");
			default                : return _T("none");
			}
		}

		SimpleType_FromString     (ETabTlc)
		SimpleType_Operator_Equal (CTabTlc)
	};

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

	template<ETargetScreenSz eDefValue = targetscreensz1280x1024>
	class CTargetScreenSz : public CSimpleType<ETargetScreenSz, eDefValue>
	{
	public:
		CTargetScreenSz() {}

		virtual ETargetScreenSz FromString(CString &sValue)
		{
            if      ( _T("1024x768")  == sValue ) this->m_eValue = targetscreensz1024x768;
            else if ( _T("1152x882")  == sValue ) this->m_eValue = targetscreensz1152x882;
            else if ( _T("1152x900")  == sValue ) this->m_eValue = targetscreensz1152x900;
            else if ( _T("1280x1024") == sValue ) this->m_eValue = targetscreensz1280x1024;
            else if ( _T("1600x1200") == sValue ) this->m_eValue = targetscreensz1600x1200;
            else if ( _T("1800x1440") == sValue ) this->m_eValue = targetscreensz1800x1440;
            else if ( _T("1920x1200") == sValue ) this->m_eValue = targetscreensz1920x1200;
            else if ( _T("544x376")   == sValue ) this->m_eValue = targetscreensz544x376;
            else if ( _T("640x480")   == sValue ) this->m_eValue = targetscreensz640x480;
            else if ( _T("720x512")   == sValue ) this->m_eValue = targetscreensz720x512;
            else if ( _T("800x600")   == sValue ) this->m_eValue = targetscreensz800x600;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString         ToString  () const 
		{
            switch(this->m_eValue)
			{
			case targetscreensz1024x768  : return _T("1024x768");
			case targetscreensz1152x882  : return _T("1152x882");
			case targetscreensz1152x900  : return _T("1152x900");
			case targetscreensz1280x1024 : return _T("1280x1024");
			case targetscreensz1600x1200 : return _T("1600x1200");
			case targetscreensz1800x1440 : return _T("1800x1440");
			case targetscreensz1920x1200 : return _T("1920x1200");
			case targetscreensz544x376   : return _T("544x376");
			case targetscreensz640x480   : return _T("640x480");
			case targetscreensz720x512   : return _T("720x512");
			case targetscreensz800x600   : return _T("800x600");
			default                      : return _T("1280x1024");
			}
		}

		SimpleType_FromString     (ETargetScreenSz)
		SimpleType_Operator_Equal (CTargetScreenSz)
	};



	//--------------------------------------------------------------------------------
	// TblLayoutType 17.18.87 (Part 1)
	//--------------------------------------------------------------------------------		

	enum ETblLayoutType
	{
		tbllayouttypeAutofit = 1,
		tbllayouttypeFixed   = 2
	};

	template<ETblLayoutType eDefValue = tbllayouttypeAutofit>
	class CTblLayoutType : public CSimpleType<ETblLayoutType, eDefValue>
	{
	public:
		CTblLayoutType() {}

		virtual ETblLayoutType FromString(CString &sValue)
		{
            if      ( _T("autofit") == sValue ) this->m_eValue = tbllayouttypeAutofit;
            else if ( _T("fixed")   == sValue ) this->m_eValue = tbllayouttypeFixed;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case tbllayouttypeAutofit : return _T("autofit");
			case tbllayouttypeFixed   : return _T("fixed");
			default                   : return _T("autofit");
			}
		}

		SimpleType_FromString     (ETblLayoutType)
		SimpleType_Operator_Equal (CTblLayoutType)
	};

	//--------------------------------------------------------------------------------
	// TblOverlap 17.18.88 (Part 1)
	//--------------------------------------------------------------------------------		

	enum ETblOverlap
	{
		tbloverlapNever   = 1,
		tbloverlapOverlap = 2
	};

	template<ETblOverlap eDefValue = tbloverlapOverlap>
	class CTblOverlap : public CSimpleType<ETblOverlap, eDefValue>
	{
	public:
		CTblOverlap() {}

		virtual ETblOverlap FromString(CString &sValue)
		{
            if      ( _T("never")   == sValue ) this->m_eValue = tbloverlapNever;
            else if ( _T("overlap") == sValue ) this->m_eValue = tbloverlapOverlap;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case tbloverlapNever   : return _T("never");
			case tbloverlapOverlap : return _T("overlap");
			default                : return _T("overlap");
			}
		}

		SimpleType_FromString     (ETblOverlap)
		SimpleType_Operator_Equal (CTblOverlap)
	};

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

	template<ETblStyleOverrideType eDefValue = tblstyleoverridetypeWholeTable>
	class CTblStyleOverrideType : public CSimpleType<ETblStyleOverrideType, eDefValue>
	{
	public:
		CTblStyleOverrideType() {}

		virtual ETblStyleOverrideType FromString(CString &sValue)
		{
            if      ( _T("band1Horz")  == sValue ) this->m_eValue = tblstyleoverridetypeBand1Horz;
            else if ( _T("band1Vert")  == sValue ) this->m_eValue = tblstyleoverridetypeBand1Vert;
            else if ( _T("band2Horz")  == sValue ) this->m_eValue = tblstyleoverridetypeBand2Horz;
            else if ( _T("band2Vert")  == sValue ) this->m_eValue = tblstyleoverridetypeBand2Vert;
            else if ( _T("firstCol")   == sValue ) this->m_eValue = tblstyleoverridetypeFirstCol;
            else if ( _T("firstRow")   == sValue ) this->m_eValue = tblstyleoverridetypeFirstRow;
            else if ( _T("lastCol")    == sValue ) this->m_eValue = tblstyleoverridetypeLastCol;
            else if ( _T("lastRow")    == sValue ) this->m_eValue = tblstyleoverridetypeLastRow;
            else if ( _T("neCell")     == sValue ) this->m_eValue = tblstyleoverridetypeNeCell;
            else if ( _T("nwCell")     == sValue ) this->m_eValue = tblstyleoverridetypeNwCell;
            else if ( _T("seCell")     == sValue ) this->m_eValue = tblstyleoverridetypeSeCell;
            else if ( _T("swCell")     == sValue ) this->m_eValue = tblstyleoverridetypeSwCell;
            else if ( _T("wholeTable") == sValue ) this->m_eValue = tblstyleoverridetypeWholeTable;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString               ToString  () const 
		{
            switch(this->m_eValue)
			{
			case tblstyleoverridetypeBand1Horz  : return _T("band1Horz");
			case tblstyleoverridetypeBand1Vert  : return _T("band1Vert");
			case tblstyleoverridetypeBand2Horz  : return _T("band2Horz");
			case tblstyleoverridetypeBand2Vert  : return _T("band2Vert");
			case tblstyleoverridetypeFirstCol   : return _T("firstCol");
			case tblstyleoverridetypeFirstRow   : return _T("firstRow");
			case tblstyleoverridetypeLastCol    : return _T("lastCol");
			case tblstyleoverridetypeLastRow    : return _T("lastRow");
			case tblstyleoverridetypeNeCell     : return _T("neCell");
			case tblstyleoverridetypeNwCell     : return _T("nwCell");
			case tblstyleoverridetypeSeCell     : return _T("seCell");
			case tblstyleoverridetypeSwCell     : return _T("swCell");
			case tblstyleoverridetypeWholeTable : return _T("wholeTable");
			default                             : return _T("wholeTable");
			}
		}

		SimpleType_FromString     (ETblStyleOverrideType)
		SimpleType_Operator_Equal (CTblStyleOverrideType)
	};




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

	template<ETblWidth eDefValue = tblwidthAuto>
	class CTblWidth : public CSimpleType<ETblWidth, eDefValue>
	{
	public:
		CTblWidth() {}

		virtual ETblWidth FromString(CString &sValue)
		{
            if      ( _T("auto") == sValue ) this->m_eValue = tblwidthAuto;
            else if ( _T("dxa")  == sValue ) this->m_eValue = tblwidthDxa;
            else if ( _T("nil")  == sValue ) this->m_eValue = tblwidthNil;
            else if ( _T("pct")  == sValue ) this->m_eValue = tblwidthPct;
            else                             this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString   ToString  () const 
		{
            switch(this->m_eValue)
			{
			case tblwidthAuto : return _T("auto");
			case tblwidthDxa  : return _T("dxa");
			case tblwidthNil  : return _T("nil");
			case tblwidthPct  : return _T("pct");
			default           : return _T("auto");
			}
		}

		SimpleType_FromString     (ETblWidth)
		SimpleType_Operator_Equal (CTblWidth)
	};


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

	template<ETextAlignment eDefValue = textalignAuto>
	class CTextAlignment : public CSimpleType<ETextAlignment, eDefValue>
	{
	public:
		CTextAlignment() {}

		virtual ETextAlignment FromString(CString &sValue)
		{
            if      ( _T("auto")     == sValue ) this->m_eValue = textalignAuto;
            else if ( _T("baseline") == sValue ) this->m_eValue = textalignBaseLine;
            else if ( _T("bottom")   == sValue ) this->m_eValue = textalignBottom;
            else if ( _T("center")   == sValue ) this->m_eValue = textalignCenter;
            else if ( _T("top")      == sValue ) this->m_eValue = textalignTop;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textalignAuto     : return _T("auto");
			case textalignBaseLine : return _T("baseline");
			case textalignBottom   : return _T("bottom");
			case textalignCenter   : return _T("center");
			case textalignTop      : return _T("top");
			default                : return _T("auto");
			}
		}

		SimpleType_FromString     (ETextAlignment)
		SimpleType_Operator_Equal (CTextAlignment)
	};

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

	template<ETextboxTightWrap eDefValue = textboxtightwrapNone>
	class CTextboxTightWrap : public CSimpleType<ETextboxTightWrap, eDefValue>
	{
	public:
		CTextboxTightWrap() {}

		virtual ETextboxTightWrap FromString(CString &sValue)
		{
            if      ( _T("allLines")         == sValue ) this->m_eValue = textboxtightwrapAllLines;
            else if ( _T("firstAndLastLine") == sValue ) this->m_eValue = textboxtightwrapFirstAndLastLine;
            else if ( _T("firstLineOnly")    == sValue ) this->m_eValue = textboxtightwrapFirstLineOnly;
            else if ( _T("lastLineOnly")     == sValue ) this->m_eValue = textboxtightwrapLastLineOnly;
            else if ( _T("none")             == sValue ) this->m_eValue = textboxtightwrapNone;
            else                                         this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textboxtightwrapAllLines         : return _T("allLines");
			case textboxtightwrapFirstAndLastLine : return _T("firstAndLastLine");
			case textboxtightwrapFirstLineOnly    : return _T("firstLineOnly");
			case textboxtightwrapLastLineOnly     : return _T("lastLineOnly");
			case textboxtightwrapNone             : return _T("none");
			default                               : return _T("none");
			}
		}

		SimpleType_FromString     (ETextboxTightWrap)
		SimpleType_Operator_Equal (CTextboxTightWrap)
	};
	//--------------------------------------------------------------------------------
	// TextDirection 17.18.93 (Part 1) + 9.10.6 (Part 4)
	//--------------------------------------------------------------------------------		

	enum ETextDirection
	{
		textdirectionLr   = 0,
		textdirectionLrV  = 1,
		textdirectionRl   = 2,
		textdirectionRlV  = 3,
		textdirectionTb   = 4,
		textdirectionTbV  = 5
	};

	template<ETextDirection eDefValue = textdirectionLr>
	class CTextDirection : public CSimpleType<ETextDirection, eDefValue>
	{
	public:
		CTextDirection() {}

		virtual ETextDirection FromString(CString &sValue)
		{
            if      ( _T("lr")  == sValue || _T("btLr")  == sValue ) this->m_eValue = textdirectionLr;
            else if ( _T("lrV") == sValue || _T("tbLrV") == sValue ) this->m_eValue = textdirectionLrV;
            else if ( _T("rl")  == sValue || _T("tbRl")  == sValue ) this->m_eValue = textdirectionRl;
            else if ( _T("rlV") == sValue || _T("tbRlV") == sValue ) this->m_eValue = textdirectionRlV;
            else if ( _T("tb")  == sValue || _T("lrTb")  == sValue ) this->m_eValue = textdirectionTb;
            else if ( _T("tbV") == sValue || _T("lrTbV") == sValue ) this->m_eValue = textdirectionTbV;
            else                                                     this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textdirectionLr  : return _T("lr");
			case textdirectionLrV : return _T("lrV");
			case textdirectionRl  : return _T("rl");
			case textdirectionRlV : return _T("rlV");
			case textdirectionTb  : return _T("tb");
			case textdirectionTbV : return _T("tbV");
			default               : return _T("lr");
			}
		}

		SimpleType_FromString     (ETextDirection)
		SimpleType_Operator_Equal (CTextDirection)
	};

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

	template<ETextEffect eDefValue = texteffectNone>
	class CTextEffect : public CSimpleType<ETextEffect, eDefValue>
	{
	public:
		CTextEffect() {}

		virtual ETextEffect FromString(CString &sValue)
		{
            if      ( _T("antsBlack")       == sValue ) this->m_eValue = texteffectAntsBlack;
            else if ( _T("antsRed")         == sValue ) this->m_eValue = texteffectAntsRed;
            else if ( _T("blinkBackground") == sValue ) this->m_eValue = texteffectBlinkBackground;
            else if ( _T("lights")          == sValue ) this->m_eValue = texteffectLights;
            else if ( _T("none")            == sValue ) this->m_eValue = texteffectNone;
            else if ( _T("shimmer")         == sValue ) this->m_eValue = texteffectShimmer;
            else if ( _T("sparkle")         == sValue ) this->m_eValue = texteffectSparkle;
            else                                        this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case texteffectAntsBlack       : return _T("antsBlack");
			case texteffectAntsRed         : return _T("antsRed");
			case texteffectBlinkBackground : return _T("blinkBackground");
			case texteffectLights          : return _T("lights");
			case texteffectNone            : return _T("none");
			case texteffectShimmer         : return _T("shimmer");
			case texteffectSparkle         : return _T("sparkle");
			default                        : return _T("none");
			}
		}

		SimpleType_FromString     (ETextEffect)
		SimpleType_Operator_Equal (CTextEffect)
	};


	//--------------------------------------------------------------------------------
	// TextScale 17.18.95 (Part 1)
	//--------------------------------------------------------------------------------		

	template<int nDefValue = 100>
	class CTextScale : public CSimpleType<int, nDefValue>
	{
	public:
		CTextScale() {}

		virtual void    SetValue(int nValue)
		{
            this->m_eValue = min( 600, max( 0, nValue ) );
		}

		virtual int     FromString(CString &sValue)
		{
            this->m_eValue = _wtoi( sValue );

            this->m_eValue = min( 600, max( 0, this->m_eValue ) );

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CTextScale)
	};

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

	template<ETheme eDefValue = themeMajorAscii>
	class CTheme : public CSimpleType<ETheme, eDefValue>
	{
	public:
		CTheme() {}

		virtual ETheme  FromString(CString &sValue)
		{
            if      ( _T("majorAscii")    == sValue ) this->m_eValue = themeMajorAscii;
            else if ( _T("majorBidi")     == sValue ) this->m_eValue = themeMajorBidi;
            else if ( _T("majorEastAsia") == sValue ) this->m_eValue = themeMajorEastAsia;
            else if ( _T("majorHAnsi")    == sValue ) this->m_eValue = themeMajorHAnsi;
            else if ( _T("minorAscii")    == sValue ) this->m_eValue = themeMinorAscii;
            else if ( _T("minorBidi")     == sValue ) this->m_eValue = themeMinorBidi;
            else if ( _T("minorEastAsia") == sValue ) this->m_eValue = themeMinorEastAsia;
            else if ( _T("minorHAnsi")    == sValue ) this->m_eValue = themeMinorHAnsi;
            else                                      this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case themeMajorAscii    : return _T("majorAscii");
			case themeMajorBidi     : return _T("majorBidi");
			case themeMajorEastAsia : return _T("majorEastAsia");
			case themeMajorHAnsi    : return _T("majorHAnsi");
			case themeMinorAscii    : return _T("minorAscii");
			case themeMinorBidi     : return _T("minorBidi");
			case themeMinorEastAsia : return _T("minorEastAsia");
			case themeMinorHAnsi    : return _T("minorHAnsi");
			default                 : return _T("majorAscii");
			}
		}

		SimpleType_FromString     (ETheme)
		SimpleType_Operator_Equal (CTheme)
	};

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

	template<EThemeColor eDefValue = themecolorNone>
	class CThemeColor : public CSimpleType<EThemeColor, eDefValue>
	{
	public:
		CThemeColor() {}

		virtual EThemeColor FromString(CString &sValue)
		{
            if      ( _T("accent1")           == sValue ) this->m_eValue = themecolorAccent1;
            else if ( _T("accent2")           == sValue ) this->m_eValue = themecolorAccent2;
            else if ( _T("accent3")           == sValue ) this->m_eValue = themecolorAccent3;
            else if ( _T("accent4")           == sValue ) this->m_eValue = themecolorAccent4;
            else if ( _T("accent5")           == sValue ) this->m_eValue = themecolorAccent5;
            else if ( _T("accent6")           == sValue ) this->m_eValue = themecolorAccent6;
            else if ( _T("background1")       == sValue ) this->m_eValue = themecolorBackground1;
            else if ( _T("background2")       == sValue ) this->m_eValue = themecolorBackground2;
            else if ( _T("dark1")             == sValue ) this->m_eValue = themecolorDark1;
            else if ( _T("dark2")             == sValue ) this->m_eValue = themecolorDark2;
            else if ( _T("followedHyperlink") == sValue ) this->m_eValue = themecolorFollowedHyperlink;
            else if ( _T("hyperlink")         == sValue ) this->m_eValue = themecolorHyperlink;
            else if ( _T("light1")            == sValue ) this->m_eValue = themecolorLight1;
            else if ( _T("light2")            == sValue ) this->m_eValue = themecolorLight2;
            else if ( _T("none")              == sValue ) this->m_eValue = themecolorNone;
            else if ( _T("text1")             == sValue ) this->m_eValue = themecolorText1;
            else if ( _T("text2")             == sValue ) this->m_eValue = themecolorText2;
            else                                          this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case themecolorAccent1           : return _T("accent1");
			case themecolorAccent2           : return _T("accent2");
			case themecolorAccent3           : return _T("accent3");
			case themecolorAccent4           : return _T("accent4");
			case themecolorAccent5           : return _T("accent5");
			case themecolorAccent6           : return _T("accent6");
			case themecolorBackground1       : return _T("background1");
			case themecolorBackground2       : return _T("background2");
			case themecolorDark1             : return _T("dark1");
			case themecolorDark2             : return _T("dark2");
			case themecolorFollowedHyperlink : return _T("followedHyperlink");
			case themecolorHyperlink         : return _T("hyperlink");
			case themecolorLight1            : return _T("light1");
			case themecolorLight2            : return _T("light2");
			case themecolorNone              : return _T("none");
			case themecolorText1             : return _T("text1");
			case themecolorText2             : return _T("text2");
			default                          : return _T("none");
			}
		}

		SimpleType_FromString     (EThemeColor)
		SimpleType_Operator_Equal (CThemeColor)
	};


	//--------------------------------------------------------------------------------
	// UcharHexNumber 17.18.98 (Part 1)
	//--------------------------------------------------------------------------------		
	template<unsigned char unDefValue = 0>
	class CUcharHexNumber : public CSimpleType<unsigned char, unDefValue>
	{
	public:
		CUcharHexNumber() {}

		virtual unsigned char FromString(CString &sValue)
		{
			if ( !Parse(sValue) )
			{
                this->m_eValue = unDefValue;
			}

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
			CString sResult;
            sResult.Format( _T("%02X"), this->m_eValue);

			return sResult;
		}

		SimpleType_FromString     (unsigned char)
		SimpleType_Operator_Equal (CUcharHexNumber)
	private:

		bool Parse(CString &sValue)
		{
			if ( sValue.GetLength() < 2 )
				return false;

			bool bResult = true;

            this->m_eValue  = (unsigned char)HexToInt( (int)sValue[1], bResult );
            this->m_eValue += (unsigned char)HexToInt( (int)sValue[0], bResult ) <<  4;

			return bResult;
		}

		int	HexToInt(int nHex, bool &bResult)
		{
			if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
			if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
			if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

			bResult = false;

			return 0;
		}

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

	template<EUnderline eDefValue = underlineNone>
	class CUnderline : public CSimpleType<EUnderline, eDefValue>
	{
	public:
		CUnderline() {}

		virtual EUnderline FromString(CString &sValue)
		{
            if      ( _T("dash")            == sValue ) this->m_eValue = underlineDash;
            else if ( _T("dashDotDotHeavy") == sValue ) this->m_eValue = underlineDashDotDotHeavy;
            else if ( _T("dashDotHeavy")    == sValue ) this->m_eValue = underlineDashDotHeavy;
            else if ( _T("dashedHeavy")     == sValue ) this->m_eValue = underlineDashedHeavy;
            else if ( _T("dashLong")        == sValue ) this->m_eValue = underlineDashLong;
            else if ( _T("dashLongHeavy")   == sValue ) this->m_eValue = underlineDashLongHeavy;
            else if ( _T("dotDash")         == sValue ) this->m_eValue = underlineDotDash;
            else if ( _T("dotDotDash")      == sValue ) this->m_eValue = underlineDotDotDash;
            else if ( _T("dotted")          == sValue ) this->m_eValue = underlineDotted;
            else if ( _T("dottedHeavy")     == sValue ) this->m_eValue = underlineDottedHeavy;
            else if ( _T("double")          == sValue ) this->m_eValue = underlineDouble;
            else if ( _T("none")            == sValue ) this->m_eValue = underlineNone;
            else if ( _T("single")          == sValue ) this->m_eValue = underlineSingle;
            else if ( _T("thick")           == sValue ) this->m_eValue = underlineThick;
            else if ( _T("wave")            == sValue ) this->m_eValue = underlineWave;
            else if ( _T("wavyDouble")      == sValue ) this->m_eValue = underlineWavyDouble;
            else if ( _T("wavyHeavy")       == sValue ) this->m_eValue = underlineWavyHeavy;
            else if ( _T("words")           == sValue ) this->m_eValue = underlineWords;
            else                                        this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString    ToString  () const 
		{
            switch(this->m_eValue)
			{
			case underlineDash            : return _T("dash");
			case underlineDashDotDotHeavy : return _T("dashDotDotHeavy");
			case underlineDashDotHeavy    : return _T("dashDotHeavy");
			case underlineDashedHeavy     : return _T("dashedHeavy");
			case underlineDashLong        : return _T("dashLong");
			case underlineDashLongHeavy   : return _T("dashLongHeavy");
			case underlineDotDash         : return _T("dotDash");
			case underlineDotDotDash      : return _T("dotDotDash");
			case underlineDotted          : return _T("dotted");
			case underlineDottedHeavy     : return _T("dottedHeavy");
			case underlineDouble          : return _T("double");
			case underlineNone            : return _T("none");
			case underlineSingle          : return _T("single");
			case underlineThick           : return _T("thick");
			case underlineWave            : return _T("wave");
			case underlineWavyDouble      : return _T("wavyDouble");
			case underlineWavyHeavy       : return _T("wavyHeavy");
			case underlineWords           : return _T("words");
			default                       : return _T("none");
			}
		}

		SimpleType_FromString     (EUnderline)
		SimpleType_Operator_Equal (CUnderline)
	};




	//--------------------------------------------------------------------------------
	// VAnchor 17.18.100 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EVAnchor
	{
		vanchorMargin = 0,
		vanchorPage   = 1,
		vanchorText   = 2
	};

	template<EVAnchor eDefValue = vanchorText>
	class CVAnchor : public CSimpleType<EVAnchor, eDefValue>
	{
	public:
		CVAnchor() {}

		virtual EVAnchor FromString(CString &sValue)
		{
            if      ( _T("margin") == sValue ) this->m_eValue = vanchorMargin;
            else if ( _T("page")   == sValue ) this->m_eValue = vanchorPage;
            else if ( _T("text")   == sValue ) this->m_eValue = vanchorText;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString  ToString  () const 
		{
            switch(this->m_eValue)
			{
			case vanchorMargin : return _T("margin");
			case vanchorPage   : return _T("page");
			case vanchorText   : return _T("text");
			default            : return _T("text");
			}
		}

		SimpleType_FromString     (EVAnchor)
		SimpleType_Operator_Equal (CVAnchor)
	};






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

	template<EVerticalJc eDefValue = verticaljcTop>
	class CVerticalJc : public CSimpleType<EVerticalJc, eDefValue>
	{
	public:
		CVerticalJc() {}

		virtual EVerticalJc FromString(CString &sValue)
		{
            if      ( _T("both")   == sValue ) this->m_eValue = verticaljcBoth;
            else if ( _T("bottom") == sValue ) this->m_eValue = verticaljcBottom;
            else if ( _T("center") == sValue ) this->m_eValue = verticaljcCenter;
            else if ( _T("top")    == sValue ) this->m_eValue = verticaljcTop;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString     ToString  () const 
		{
            switch(this->m_eValue)
			{
			case verticaljcBoth   : return _T("both");
			case verticaljcBottom : return _T("bottom");
			case verticaljcCenter : return _T("center");
			case verticaljcTop    : return _T("top");
			default               : return _T("top");
			}
		}

		SimpleType_FromString     (EVerticalJc)
		SimpleType_Operator_Equal (CVerticalJc)
	};





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

	template<EView eDefValue = viewNone>
	class CView : public CSimpleType<EView, eDefValue>
	{
	public:
		CView() {}

		virtual EView   FromString(CString &sValue)
		{
            if      ( _T("masterPages") == sValue ) this->m_eValue = viewMasterPages;
            else if ( _T("none")        == sValue ) this->m_eValue = viewNone;
            else if ( _T("normal")      == sValue ) this->m_eValue = viewNormal;
            else if ( _T("outline")     == sValue ) this->m_eValue = viewOutline;
            else if ( _T("print")       == sValue ) this->m_eValue = viewPrint;
            else if ( _T("web")         == sValue ) this->m_eValue = viewWeb;
            else                                    this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case viewMasterPages : return _T("masterPages");
			case viewNone        : return _T("none");
			case viewNormal      : return _T("normal");
			case viewOutline     : return _T("outline");
			case viewPrint       : return _T("print");
			case viewWeb         : return _T("web");
			default              : return _T("none");
			}
		}

		SimpleType_FromString     (EView)
		SimpleType_Operator_Equal (CView)
	};





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

	template<EWmlColorSchemeIndex eDefValue = wmlcolorschemeindexAccent1>
	class CWmlColorSchemeIndex : public CSimpleType<EWmlColorSchemeIndex, eDefValue>
	{
	public:
		CWmlColorSchemeIndex() {}

		virtual EWmlColorSchemeIndex FromString(CString &sValue)
		{
            if      ( _T("accent1")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent1;
            else if ( _T("accent2")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent2;
            else if ( _T("accent3")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent3;
            else if ( _T("accent4")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent4;
            else if ( _T("accent5")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent5;
            else if ( _T("accent6")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent6;
            else if ( _T("dark1")             == sValue ) this->m_eValue = wmlcolorschemeindexDark1;
            else if ( _T("dark2")             == sValue ) this->m_eValue = wmlcolorschemeindexDark2;
            else if ( _T("followedHyperlink") == sValue ) this->m_eValue = wmlcolorschemeindexFollowedHyperlink;
            else if ( _T("hyperlink")         == sValue ) this->m_eValue = wmlcolorschemeindexHyperlink;
            else if ( _T("light1")            == sValue ) this->m_eValue = wmlcolorschemeindexLight1;
            else if ( _T("light2")            == sValue ) this->m_eValue = wmlcolorschemeindexLight2;
            else                                          this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString              ToString  () const 
		{
            switch(this->m_eValue)
			{
			case wmlcolorschemeindexAccent1           : return _T("accent1");
			case wmlcolorschemeindexAccent2           : return _T("accent2");
			case wmlcolorschemeindexAccent3           : return _T("accent3");
			case wmlcolorschemeindexAccent4           : return _T("accent4");
			case wmlcolorschemeindexAccent5           : return _T("accent5");
			case wmlcolorschemeindexAccent6           : return _T("accent6");
			case wmlcolorschemeindexDark1             : return _T("dark1");
			case wmlcolorschemeindexDark2             : return _T("dark2");
			case wmlcolorschemeindexFollowedHyperlink : return _T("followedHyperlink");
			case wmlcolorschemeindexHyperlink         : return _T("hyperlink");
			case wmlcolorschemeindexLight1            : return _T("light1");
			case wmlcolorschemeindexLight2            : return _T("light2");
			default                                   : return _T("accent1");
			}
		}

		SimpleType_FromString     (EWmlColorSchemeIndex)
		SimpleType_Operator_Equal (CWmlColorSchemeIndex)
	};







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

	template<EWrap eDefValue = wrapAuto>
	class CWrap : public CSimpleType<EWrap, eDefValue>
	{
	public:
		CWrap() {}

		virtual EWrap   FromString(CString &sValue)
		{
            if      ( _T("around")    == sValue ) this->m_eValue = wrapAround;
            else if ( _T("auto")      == sValue ) this->m_eValue = wrapAuto;
            else if ( _T("none")      == sValue ) this->m_eValue = wrapNone;
            else if ( _T("notBeside") == sValue ) this->m_eValue = wrapNotBeside;
            else if ( _T("through")   == sValue ) this->m_eValue = wrapThrough;
            else if ( _T("tight")     == sValue ) this->m_eValue = wrapTight;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case wrapAround    : return _T("around");
			case wrapAuto      : return _T("auto");
			case wrapNone      : return _T("none");
			case wrapNotBeside : return _T("notBeside");
			case wrapThrough   : return _T("through");
			case wrapTight     : return _T("tight");
			default            : return _T("auto");
			}
		}

		SimpleType_FromString     (EWrap)
		SimpleType_Operator_Equal (CWrap)
	};


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

	template<EZoom eDefValue = zoomNone>
	class CZoom : public CSimpleType<EZoom, eDefValue>
	{
	public:
		CZoom() {}

		virtual EZoom   FromString(CString &sValue)
		{
            if      ( _T("bestFit")  == sValue ) this->m_eValue = zoomBestFit;
            else if ( _T("fullPage") == sValue ) this->m_eValue = zoomFullPage;
            else if ( _T("none")     == sValue ) this->m_eValue = zoomNone;
            else if ( _T("textFit")  == sValue ) this->m_eValue = zoomTextFit;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
            switch(this->m_eValue)
			{
			case zoomBestFit   : return _T("bestFit");
			case zoomFullPage : return _T("fullPage");
			case zoomNone     : return _T("none");
			case zoomTextFit  : return _T("textFit");
			default           : return _T("none");
			}
		}

		SimpleType_FromString     (EZoom)
		SimpleType_Operator_Equal (CZoom)
	};

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

	template<EFontCharset eDefValue = fontcharsetANSI>
	class CFontCharset : public CSimpleType<EFontCharset, eDefValue>
	{
	public:
		CFontCharset() {}

		virtual EFontCharset FromString(CString &sValue)
		{
            if      ( _T("00") == sValue ) this->m_eValue = fontcharsetANSI;
            else if ( _T("01") == sValue ) this->m_eValue = fontcharsetDefault;
            else if ( _T("02") == sValue ) this->m_eValue = fontcharsetSymbol;
            else if ( _T("4D") == sValue ) this->m_eValue = fontcharsetMacintosh;
            else if ( _T("80") == sValue ) this->m_eValue = fontcharsetShitJIS;
            else if ( _T("81") == sValue ) this->m_eValue = fontcharsetHangeul;
            else if ( _T("82") == sValue ) this->m_eValue = fontcharsetJohab;
            else if ( _T("86") == sValue ) this->m_eValue = fontcharsetGB2313;
            else if ( _T("88") == sValue ) this->m_eValue = fontcharsetChineseBig5;
            else if ( _T("A1") == sValue ) this->m_eValue = fontcharsetGreek;
            else if ( _T("A2") == sValue ) this->m_eValue = fontcharsetTurkish;
            else if ( _T("A3") == sValue ) this->m_eValue = fontcharsetVietnamese;
            else if ( _T("B1") == sValue ) this->m_eValue = fontcharsetHebrew;
            else if ( _T("B2") == sValue ) this->m_eValue = fontcharsetArabic;
            else if ( _T("BA") == sValue ) this->m_eValue = fontcharsetBaltic;
            else if ( _T("CC") == sValue ) this->m_eValue = fontcharsetRussian;
            else if ( _T("DE") == sValue ) this->m_eValue = fontcharsetThai;
            else if ( _T("EE") == sValue ) this->m_eValue = fontcharsetEastEurope;
            else if ( _T("FF") == sValue ) this->m_eValue = fontcharsetOEM;
            else                           this->m_eValue = fontcharsetANSI;

            return this->m_eValue;
		}

		virtual CString      ToString  () const 
		{
            switch(this->m_eValue)
			{
			case fontcharsetANSI        : return _T("00");
			case fontcharsetDefault     : return _T("01");
			case fontcharsetSymbol      : return _T("02");
			case fontcharsetMacintosh   : return _T("4D");
			case fontcharsetShitJIS     : return _T("80");
			case fontcharsetHangeul     : return _T("81");
			case fontcharsetJohab       : return _T("82");
			case fontcharsetGB2313      : return _T("86");
			case fontcharsetChineseBig5 : return _T("88");
			case fontcharsetGreek       : return _T("A1");
			case fontcharsetTurkish     : return _T("A2");
			case fontcharsetVietnamese  : return _T("A3");
			case fontcharsetHebrew      : return _T("B1");
			case fontcharsetArabic      : return _T("B2");
			case fontcharsetBaltic      : return _T("BA");
			case fontcharsetRussian     : return _T("CC");
			case fontcharsetThai        : return _T("DE");
			case fontcharsetEastEurope  : return _T("EE");
			case fontcharsetOEM         : return _T("FF");
			default                     : return _T("00");
			}
		}

		SimpleType_FromString     (EFontCharset)
		SimpleType_Operator_Equal (CFontCharset)
	};

	//--------------------------------------------------------------------------------
	// CCnf 9.10.7 (Part 4)
	//--------------------------------------------------------------------------------		

	template<unsigned short eDefValue = 0>
	class CCnf : public CSimpleType<unsigned short, eDefValue>
	{
	public:
		CCnf() {}

		virtual unsigned short FromString(CString &sValue)
		{
			Parse( sValue );     

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
			CString sResult;

			for( int nIndex = 0, nMult = 1; nIndex < 12; nIndex++, nMult <<= 1 )
			{
                if ( this->m_eValue & nMult )
					sResult += '1';
				else
					sResult += '0';
			}

			return sResult;
		}

		SimpleType_FromString     (unsigned short)
		SimpleType_Operator_Equal (CCnf)

		inline bool IsFirstRow       () const
		{
            return (bool)( (this->m_eValue & 1) != 0 );
		}
		inline bool IsLastRow        () const
		{
            return (bool)( (this->m_eValue & 2) != 0 );
		}
		inline bool IsFirstColumn    () const
		{
            return (bool)( (this->m_eValue & 4) != 0 );
		}
		inline bool IsLastColumn     () const
		{
            return (bool)( (this->m_eValue & 8) != 0 );
		}
		inline bool IsBand1Vertical  () const
		{
            return (bool)( (this->m_eValue & 16) != 0 );
		}
		inline bool IsBand2Vertical  () const
		{
            return (bool)( (this->m_eValue & 32) != 0 );
		}
		inline bool IsBand1Horizontal() const
		{
            return (bool)( (this->m_eValue & 64) != 0 );
		}
		inline bool IsBand2Horizontal() const
		{
            return (bool)( (this->m_eValue & 128) != 0 );
		}
		inline bool IsNeCell         () const
		{
            return (bool)( (this->m_eValue & 256) != 0 );
		}
		inline bool IsNwCell         () const
		{
            return (bool)( (this->m_eValue & 512) != 0 );
		}
		inline bool IsSeCell         () const
		{
            return (bool)( (this->m_eValue & 1024) != 0 );
		}
		inline bool IsSwCell         () const
		{
            return (bool)( (this->m_eValue & 2048) != 0 );
		}

	private:

		void Parse(CString &sValue)
		{
			int nLen = min( sValue.GetLength(), 12 );

			for ( int nIndex = 0, nMult = 1; nIndex < nLen; nIndex++, nMult <<= 1 )
			{
				wchar_t wsChar = sValue[nIndex];

				if ( '0' == wsChar )
				{
					// Ничего не делаем
				}
				else if ( '1' == wsChar )
				{
                    this->m_eValue ^= nMult;
				}
				else // Неправильный формат
					break;
			}
		}
	};


	//--------------------------------------------------------------------------------
	// XmlSpace
	//--------------------------------------------------------------------------------		

	enum EXmlSpace
	{
		xmlspaceDefault  = 0,
		xmlspacePreserve = 1
	};

	template<EXmlSpace eDefValue = xmlspaceDefault>
	class CXmlSpace : public CSimpleType<EXmlSpace, eDefValue>
	{
	public:
		CXmlSpace() {}

		virtual EXmlSpace FromString(CString &sValue)
		{
            if       ( _T("default")  == sValue ) this->m_eValue = xmlspaceDefault;
            else if  ( _T("preserve") == sValue ) this->m_eValue = xmlspacePreserve;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString   ToString  () const 
		{
            switch(this->m_eValue)
			{
			case xmlspaceDefault  : return _T("default");
			case xmlspacePreserve : return _T("preserve");
			default               : return _T("default");
			}
		}

		SimpleType_FromString     (EXmlSpace)
		SimpleType_Operator_Equal (CXmlSpace)
	};


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

	template<ECryptAlgoritmName eDefValue = cryptalgoritmnameUnknown>
	class CCryptAlgoritmName : public CSimpleType<ECryptAlgoritmName, eDefValue>
	{
	public:
		CCryptAlgoritmName() {}

		virtual ECryptAlgoritmName FromString(CString &sValue)
		{
            if       ( _T("MD2")        == sValue ) this->m_eValue = cryptalgoritmnameMD2;
            else if  ( _T("MD4")        == sValue ) this->m_eValue = cryptalgoritmnameMD4;
            else if  ( _T("MD5")        == sValue ) this->m_eValue = cryptalgoritmnameMD5;
            else if  ( _T("RIPEMD-128") == sValue ) this->m_eValue = cryptalgoritmnameRIPEMD128;
            else if  ( _T("RIPEMD-160") == sValue ) this->m_eValue = cryptalgoritmnameRIPEMD160;
            else if  ( _T("SHA-1")      == sValue ) this->m_eValue = cryptalgoritmnameSHA1;
            else if  ( _T("SHA-256")    == sValue ) this->m_eValue = cryptalgoritmnameSHA256;
            else if  ( _T("SHA-384")    == sValue ) this->m_eValue = cryptalgoritmnameSHA384;
            else if  ( _T("SHA-512")    == sValue ) this->m_eValue = cryptalgoritmnameSHA512;
            else if  ( _T("WHIRLPOOL")  == sValue ) this->m_eValue = cryptalgoritmnameWHIRLPOOL;
            else                                    this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case cryptalgoritmnameMD2       : return _T("MD2");
			case cryptalgoritmnameMD4       : return _T("MD4");
			case cryptalgoritmnameMD5       : return _T("MD5");
			case cryptalgoritmnameRIPEMD128 : return _T("RIPEMD-128");
			case cryptalgoritmnameRIPEMD160 : return _T("RIPEMD-160");
			case cryptalgoritmnameSHA1      : return _T("SHA-1");
			case cryptalgoritmnameSHA256    : return _T("SHA-256");
			case cryptalgoritmnameSHA384    : return _T("SHA-384");
			case cryptalgoritmnameSHA512    : return _T("SHA-512");
			case cryptalgoritmnameWHIRLPOOL : return _T("WHIRLPOOL");
			default                         : return _T("");
			}
		}

		SimpleType_FromString     (ECryptAlgoritmName)
		SimpleType_Operator_Equal (CCryptAlgoritmName)
	};


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

	template<EPitchFamily eDefValue = pitchfamilyDefUnk>
	class CPitchFamily : public CSimpleType<EPitchFamily, eDefValue>
	{
	public:
		CPitchFamily() {}

		virtual EPitchFamily FromString(CString &sValue)
		{
			CUcharHexNumber<> oHex = sValue;
			int nValue = oHex.GetValue();

			switch ( nValue )
			{
			case 0x00: case 0x01: case 0x02:
			case 0x10: case 0x11: case 0x12:
			case 0x20: case 0x21: case 0x22:
			case 0x30: case 0x31: case 0x32:
			case 0x40: case 0x41: case 0x42:
			case 0x50: case 0x51: case 0x52:
                this->m_eValue = (EPitchFamily)nValue; break;
			default:
                this->m_eValue = eDefValue; break;
			}

            return this->m_eValue;
		}

		virtual CString      ToString  () const 
		{
            switch(this->m_eValue)
			{
			case pitchfamilyDefUnk: return _T("00");
			case pitchfamilyFixUnk: return _T("01");
			case pitchfamilyVarUnk: return _T("02");
			case pitchfamilyDefRom: return _T("10");
			case pitchfamilyFixRom: return _T("11");
			case pitchfamilyVarRom: return _T("12");
			case pitchfamilyDefSwi: return _T("20");
			case pitchfamilyFixSwi: return _T("21");
			case pitchfamilyVarSwi: return _T("22");
			case pitchfamilyDefMod: return _T("30");
			case pitchfamilyFixMod: return _T("31");
			case pitchfamilyVarMod: return _T("32");
			case pitchfamilyDefScr: return _T("40");
			case pitchfamilyFixScr: return _T("41");
			case pitchfamilyVarScr: return _T("42");
			case pitchfamilyDefDec: return _T("50");
			case pitchfamilyFixDec: return _T("51");
			case pitchfamilyVarDec: return _T("52");
			default               : return _T("00");
			}
		}

		SimpleType_FromString     (EPitchFamily)
		SimpleType_Operator_Equal (CPitchFamily)

		unsigned char GetPitch()
		{
            int nValue = (int)this->m_eValue;
			return nValue & 0x0F;
		}
		unsigned char GetFamily()
		{
            int nValue = (int)this->m_eValue;
			return nValue >> 4;
		}
	};
	//--------------------------------------------------------------------------------
	// CryptAlgoritmName
	//--------------------------------------------------------------------------------		
	enum EDirVal
	{
		dirLtr    = 0,
		dirRtl    = 1
	};

	template<EDirVal eDefValue = dirLtr>
	class CDirVal : public CSimpleType<EDirVal, eDefValue>
	{
	public:
		CDirVal() {}

		virtual EDirVal FromString(CString &sValue)
		{
            if       ( _T("ltr")        == sValue ) this->m_eValue = dirLtr;
            else if  ( _T("rtl")        == sValue ) this->m_eValue = dirRtl;
            else                                    this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case cryptalgoritmnameMD2       : return _T("ltr");
			case cryptalgoritmnameMD4       : return _T("rtl");
			default                         : return _T("ltr");
			}
		}

		SimpleType_FromString     (EDirVal)
		SimpleType_Operator_Equal (CDirVal)
	};

} // SimpleTypes
