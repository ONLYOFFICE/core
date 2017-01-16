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

#include "SimpleTypes_Base.h"
#include <algorithm>

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

        virtual EAnnotationVMerge FromString(std::wstring &sValue)
		{
            if       ( (L"cont") == sValue ) m_eValue = annotvmergeCont;
            else if  ( (L"rest") == sValue ) m_eValue = annotvmergeRest;
            else                              m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString() const
		{
            switch(m_eValue)
			{
			case annotvmergeCont : return (L"cont");
			case annotvmergeRest : return (L"rest");
			default              : return (L"cont");
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

        virtual EBorder FromString(std::wstring &sValue)
		{
			wchar_t wsFirstChar = 0;

            if ( sValue.length() > 0 )
				wsFirstChar = sValue[0];

			switch(wsFirstChar)
			{
			case 'a':
				{
                    if      ( (L"apples")                 == sValue ) m_eValue = bordervalueApples;
                    else if ( (L"archedScallops")         == sValue ) m_eValue = bordervalueArchedScallops;
                    else                                               m_eValue = eDefValue;
					break;
				}
			case 'b':
				{
                    if      ( (L"babyPacifier")           == sValue ) m_eValue = bordervalueBabyPacifier;
                    else if ( (L"babyRattle")             == sValue ) m_eValue = bordervalueBabyRattle;
                    else if ( (L"balloons3Colors")        == sValue ) m_eValue = bordervalueBalloons3Colors;
                    else if ( (L"balloonsHotAir")         == sValue ) m_eValue = bordervalueBalloonsHotAir;
                    else if ( (L"basicBlackDashes")       == sValue ) m_eValue = bordervalueBasicBlackDashes;
                    else if ( (L"basicBlackDots")         == sValue ) m_eValue = bordervalueBasicBlackDots;
                    else if ( (L"basicBlackSquares")      == sValue ) m_eValue = bordervalueBasicBlackSquares;
                    else if ( (L"basicThinLines")         == sValue ) m_eValue = bordervalueBasicThinLines;
                    else if ( (L"basicWhiteDashes")       == sValue ) m_eValue = bordervalueBasicWhiteDashes;
                    else if ( (L"basicWhiteDots")         == sValue ) m_eValue = bordervalueBasicWhiteDots;
                    else if ( (L"basicWhiteSquares")      == sValue ) m_eValue = bordervalueBasicWhiteSquares;
                    else if ( (L"basicWideInline")        == sValue ) m_eValue = bordervalueBasicWideInline;
                    else if ( (L"basicWideMidline")       == sValue ) m_eValue = bordervalueBasicWideMidline;
                    else if ( (L"basicWideOutline")       == sValue ) m_eValue = bordervalueBasicWideOutline;
                    else if ( (L"bats")                   == sValue ) m_eValue = bordervalueBats;
                    else if ( (L"birds")                  == sValue ) m_eValue = bordervalueBirds;
                    else if ( (L"birdsFlight")            == sValue ) m_eValue = bordervalueBirdsFlight;
                    else                                               m_eValue = eDefValue;
					break;
				}
			case 'c':
				{
                    if      ( (L"cabins")                 == sValue ) m_eValue = bordervalueCabins;
                    else if ( (L"cakeSlice")              == sValue ) m_eValue = bordervalueCakeSlice;
                    else if ( (L"candyCorn")              == sValue ) m_eValue = bordervalueCandyCorn;
                    else if ( (L"celticKnotwork")         == sValue ) m_eValue = bordervalueCelticKnotwork;
                    else if ( (L"certificateBanner")      == sValue ) m_eValue = bordervalueCertificateBanner;
                    else if ( (L"chainLink")              == sValue ) m_eValue = bordervalueChainLink;
                    else if ( (L"champagneBottle")        == sValue ) m_eValue = bordervalueChampagneBottle;
                    else if ( (L"checkedBarBlack")        == sValue ) m_eValue = bordervalueCheckedBarBlack;
                    else if ( (L"checkedBarColor")        == sValue ) m_eValue = bordervalueCheckedBarColor;
                    else if ( (L"checkered")              == sValue ) m_eValue = bordervalueCheckered;
                    else if ( (L"christmasTree")          == sValue ) m_eValue = bordervalueChristmasTree;
                    else if ( (L"circlesLines")           == sValue ) m_eValue = bordervalueCirclesLines;
                    else if ( (L"circlesRectangles")      == sValue ) m_eValue = bordervalueCirclesRectangles;
                    else if ( (L"classicalWave")          == sValue ) m_eValue = bordervalueClassicalWave;
                    else if ( (L"clocks")                 == sValue ) m_eValue = bordervalueClocks;
                    else if ( (L"compass")                == sValue ) m_eValue = bordervalueCompass;
                    else if ( (L"confetti")               == sValue ) m_eValue = bordervalueConfetti;
                    else if ( (L"confettiGrays")          == sValue ) m_eValue = bordervalueConfettiGrays;
                    else if ( (L"confettiOutline")        == sValue ) m_eValue = bordervalueConfettiOutline;
                    else if ( (L"confettiStreamers")      == sValue ) m_eValue = bordervalueConfettiStreamers;
                    else if ( (L"confettiWhite")          == sValue ) m_eValue = bordervalueConfettiWhite;
                    else if ( (L"cornerTriangles")        == sValue ) m_eValue = bordervalueCornerTriangles;
                    else if ( (L"couponCutoutDashes")     == sValue ) m_eValue = bordervalueCouponCutoutDashes;
                    else if ( (L"couponCutoutDots")       == sValue ) m_eValue = bordervalueCouponCutoutDots;
                    else if ( (L"crazyMaze")              == sValue ) m_eValue = bordervalueCrazyMaze;
                    else if ( (L"creaturesButterfly")     == sValue ) m_eValue = bordervalueCreaturesButterfly;
                    else if ( (L"creaturesFish")          == sValue ) m_eValue = bordervalueCreaturesFish;
                    else if ( (L"creaturesInsects")       == sValue ) m_eValue = bordervalueCreaturesInsects;
                    else if ( (L"creaturesLadyBug")       == sValue ) m_eValue = bordervalueCreaturesLadyBug;
                    else if ( (L"crossStitch")            == sValue ) m_eValue = bordervalueCrossStitch;
                    else if ( (L"cup")                    == sValue ) m_eValue = bordervalueCup;
                    else if ( (L"custom")                 == sValue ) m_eValue = bordervalueCustom;
                    else                                               m_eValue = eDefValue;
					break;
				}
			case 'd':
				{
                    if      ( (L"dashDotStroked")         == sValue ) m_eValue = bordervalueDashDotStroked;
                    else if ( (L"dashed")                 == sValue ) m_eValue = bordervalueDashed;
                    else if ( (L"dashSmallGap")           == sValue ) m_eValue = bordervalueDashSmallGap;
                    else if ( (L"decoArch")               == sValue ) m_eValue = bordervalueDecoArch;
                    else if ( (L"decoArchColor")          == sValue ) m_eValue = bordervalueDecoArchColor;
                    else if ( (L"decoBlocks")             == sValue ) m_eValue = bordervalueDecoBlocks;
                    else if ( (L"diamondsGray")           == sValue ) m_eValue = bordervalueDiamondsGray;
                    else if ( (L"dotDash")                == sValue ) m_eValue = bordervalueDotDash;
                    else if ( (L"dotDotDash")             == sValue ) m_eValue = bordervalueDotDotDash;
                    else if ( (L"dotted")                 == sValue ) m_eValue = bordervalueDotted;
                    else if ( (L"double")                 == sValue ) m_eValue = bordervalueDouble;
                    else if ( (L"doubleD")                == sValue ) m_eValue = bordervalueDoubleD;
                    else if ( (L"doubleDiamonds")         == sValue ) m_eValue = bordervalueDoubleDiamonds;
                    else if ( (L"doubleWave")             == sValue ) m_eValue = bordervalueDoubleWave;
                    else                                               m_eValue = eDefValue;
					break;
				}
			case 'e':
				{
                    if      ( (L"earth1")                 == sValue ) m_eValue = bordervalueEarth1;
                    else if ( (L"earth2")                 == sValue ) m_eValue = bordervalueEarth2;
                    else if ( (L"earth3")                 == sValue ) m_eValue = bordervalueEarth3;
                    else if ( (L"eclipsingSquares1")      == sValue ) m_eValue = bordervalueEclipsingSquares1;
                    else if ( (L"eclipsingSquares2")      == sValue ) m_eValue = bordervalueEclipsingSquares2;
                    else if ( (L"eggsBlack")              == sValue ) m_eValue = bordervalueEggsBlack;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'f':
				{
                    if      ( (L"fans")                   == sValue ) m_eValue = bordervalueFans;
                    else if ( (L"film")                   == sValue ) m_eValue = bordervalueFilm;
                    else if ( (L"firecrackers")           == sValue ) m_eValue = bordervalueFirecrackers;
                    else if ( (L"flowersBlockPrint")      == sValue ) m_eValue = bordervalueFlowersBlockPrint;
                    else if ( (L"flowersDaisies")         == sValue ) m_eValue = bordervalueFlowersDaisies;
                    else if ( (L"flowersModern1")         == sValue ) m_eValue = bordervalueFlowersModern1;
                    else if ( (L"flowersModern2")         == sValue ) m_eValue = bordervalueFlowersModern2;
                    else if ( (L"flowersPansy")           == sValue ) m_eValue = bordervalueFlowersPansy;
                    else if ( (L"flowersRedRose")         == sValue ) m_eValue = bordervalueFlowersRedRose;
                    else if ( (L"flowersRoses")           == sValue ) m_eValue = bordervalueFlowersRoses;
                    else if ( (L"flowersTeacup")          == sValue ) m_eValue = bordervalueFlowersTeacup;
                    else if ( (L"flowersTiny")            == sValue ) m_eValue = bordervalueFlowersTiny;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'g':
				{
                    if      ( (L"gems")                   == sValue ) m_eValue = bordervalueGems;
                    else if ( (L"gingerbreadMan")         == sValue ) m_eValue = bordervalueGingerbreadMan;
                    else if ( (L"gradient")               == sValue ) m_eValue = bordervalueGradient;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'h':
				{
                    if      ( (L"handmade1")              == sValue ) m_eValue = bordervalueHandmade1;
                    else if ( (L"handmade2")              == sValue ) m_eValue = bordervalueHandmade2;
                    else if ( (L"heartBalloon")           == sValue ) m_eValue = bordervalueHeartBalloon;
                    else if ( (L"heartGray")              == sValue ) m_eValue = bordervalueHeartGray;
                    else if ( (L"hearts")                 == sValue ) m_eValue = bordervalueHearts;
                    else if ( (L"heebieJeebies")          == sValue ) m_eValue = bordervalueHeebieJeebies;
                    else if ( (L"holly")                  == sValue ) m_eValue = bordervalueHolly;
                    else if ( (L"houseFunky")             == sValue ) m_eValue = bordervalueHouseFunky;
                    else if ( (L"hypnotic")               == sValue ) m_eValue = bordervalueHypnotic;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'i':
				{
                    if      ( (L"iceCreamCones")          == sValue ) m_eValue = bordervalueIceCreamCones;
                    else if ( (L"inset")                  == sValue ) m_eValue = bordervalueInset;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'l':
				{
                    if      ( (L"lightBulb")              == sValue ) m_eValue = bordervalueLightBulb;
                    else if ( (L"lightning1")             == sValue ) m_eValue = bordervalueLightning1;
                    else if ( (L"lightning2")             == sValue ) m_eValue = bordervalueLightning2;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'm':
				{
                    if      ( (L"mapleLeaf")              == sValue ) m_eValue = bordervalueMapleLeaf;
                    else if ( (L"mapleMuffins")           == sValue ) m_eValue = bordervalueMapleMuffins;
                    else if ( (L"mapPins")                == sValue ) m_eValue = bordervalueMapPins;
                    else if ( (L"marquee")                == sValue ) m_eValue = bordervalueMarquee;
                    else if ( (L"marqueeToothed")         == sValue ) m_eValue = bordervalueMarqueeToothed;
                    else if ( (L"moons")                  == sValue ) m_eValue = bordervalueMoons;
                    else if ( (L"mosaic")                 == sValue ) m_eValue = bordervalueMosaic;
                    else if ( (L"musicNotes")             == sValue ) m_eValue = bordervalueMusicNotes;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'n':
				{
                    if      ( (L"nil")                    == sValue ) m_eValue = bordervalueNil;
                    else if ( (L"none")                   == sValue ) m_eValue = bordervalueNone;
                    else if ( (L"northwest")              == sValue ) m_eValue = bordervalueNorthwest;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'o':
				{
                    if      ( (L"outset")                 == sValue ) m_eValue = bordervalueOutset;
                    else if ( (L"ovals")                  == sValue ) m_eValue = bordervalueOvals;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'p':
				{
                    if      ( (L"packages")               == sValue ) m_eValue = bordervaluePackages;
                    else if ( (L"palmsBlack")             == sValue ) m_eValue = bordervaluePalmsBlack;
                    else if ( (L"palmsColor")             == sValue ) m_eValue = bordervaluePalmsColor;
                    else if ( (L"paperClips")             == sValue ) m_eValue = bordervaluePaperClips;
                    else if ( (L"papyrus")                == sValue ) m_eValue = bordervaluePapyrus;
                    else if ( (L"partyFavor")             == sValue ) m_eValue = bordervaluePartyFavor;
                    else if ( (L"partyGlass")             == sValue ) m_eValue = bordervaluePartyGlass;
                    else if ( (L"pencils")                == sValue ) m_eValue = bordervaluePencils;
                    else if ( (L"people")                 == sValue ) m_eValue = bordervaluePeople;
                    else if ( (L"peopleHats")             == sValue ) m_eValue = bordervaluePeopleHats;
                    else if ( (L"peopleWaving")           == sValue ) m_eValue = bordervaluePeopleWaving;
                    else if ( (L"poinsettias")            == sValue ) m_eValue = bordervaluePoinsettias;
                    else if ( (L"postageStamp")           == sValue ) m_eValue = bordervaluePostageStamp;
                    else if ( (L"pumpkin1")               == sValue ) m_eValue = bordervaluePumpkin1;
                    else if ( (L"pushPinNote1")           == sValue ) m_eValue = bordervaluePushPinNote1;
                    else if ( (L"pushPinNote2")           == sValue ) m_eValue = bordervaluePushPinNote2;
                    else if ( (L"pyramids")               == sValue ) m_eValue = bordervaluePyramids;
                    else if ( (L"pyramidsAbove")          == sValue ) m_eValue = bordervaluePyramidsAbove;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'q':
				{
                    if      ( (L"quadrants")              == sValue ) m_eValue = bordervalueQuadrants;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'r':
				{
                    if      ( (L"rings")                  == sValue ) m_eValue = bordervalueRings;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 's':
				{
                    if      ( (L"safari")                 == sValue ) m_eValue = bordervalueSafari;
                    else if ( (L"sawtooth")               == sValue ) m_eValue = bordervalueSawtooth;
                    else if ( (L"sawtoothGray")           == sValue ) m_eValue = bordervalueSawtoothGray;
                    else if ( (L"scaredCat")              == sValue ) m_eValue = bordervalueScaredCat;
                    else if ( (L"seattle")                == sValue ) m_eValue = bordervalueSeattle;
                    else if ( (L"shadowedSquares")        == sValue ) m_eValue = bordervalueShadowedSquares;
                    else if ( (L"shapes1")                == sValue ) m_eValue = bordervalueShapes1;
                    else if ( (L"shapes2")                == sValue ) m_eValue = bordervalueShapes2;
                    else if ( (L"sharksTeeth")            == sValue ) m_eValue = bordervalueSharksTeeth;
                    else if ( (L"shorebirdTracks")        == sValue ) m_eValue = bordervalueShorebirdTracks;
                    else if ( (L"single")                 == sValue ) m_eValue = bordervalueSingle;
                    else if ( (L"skyrocket")              == sValue ) m_eValue = bordervalueSkyrocket;
                    else if ( (L"snowflakeFancy")         == sValue ) m_eValue = bordervalueSnowflakeFancy;
                    else if ( (L"snowflakes")             == sValue ) m_eValue = bordervalueSnowflakes;
                    else if ( (L"sombrero")               == sValue ) m_eValue = bordervalueSombrero;
                    else if ( (L"southwest")              == sValue ) m_eValue = bordervalueSouthwest;
                    else if ( (L"stars")                  == sValue ) m_eValue = bordervalueStars;
                    else if ( (L"stars3d")                == sValue ) m_eValue = bordervalueStars3d;
                    else if ( (L"starsBlack")             == sValue ) m_eValue = bordervalueStarsBlack;
                    else if ( (L"starsShadowed")          == sValue ) m_eValue = bordervalueStarsShadowed;
                    else if ( (L"starsTop")               == sValue ) m_eValue = bordervalueStarsTop;
                    else if ( (L"sun")                    == sValue ) m_eValue = bordervalueSun;
                    else if ( (L"swirligig")              == sValue ) m_eValue = bordervalueSwirligig;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 't':
				{
                    if      ( (L"thick")                  == sValue ) m_eValue = bordervalueThick;
                    else if ( (L"thickThinLargeGap")      == sValue ) m_eValue = bordervalueThickThinLargeGap;
                    else if ( (L"thickThinMediumGap")     == sValue ) m_eValue = bordervalueThickThinMediumGap;
                    else if ( (L"thickThinSmallGap")      == sValue ) m_eValue = bordervalueThickThinSmallGap;
                    else if ( (L"thinThickLargeGap")      == sValue ) m_eValue = bordervalueThinThickLargeGap;
                    else if ( (L"thinThickMediumGap")     == sValue ) m_eValue = bordervalueThinThickMediumGap;
                    else if ( (L"thinThickSmallGap")      == sValue ) m_eValue = bordervalueThinThickSmallGap;
                    else if ( (L"thinThickThinLargeGap")  == sValue ) m_eValue = bordervalueThinThickThinLargeGap;
                    else if ( (L"thinThickThinMediumGap") == sValue ) m_eValue = bordervalueThinThickThinMediumGap;
                    else if ( (L"thinThickThinSmallGap")  == sValue ) m_eValue = bordervalueThinThickThinSmallGap;
                    else if ( (L"threeDEmboss")           == sValue ) m_eValue = bordervalueThreeDEmboss;
                    else if ( (L"threeDEngrave")          == sValue ) m_eValue = bordervalueThreeDEngrave;
                    else if ( (L"tornPaper")              == sValue ) m_eValue = bordervalueTornPaper;
                    else if ( (L"tornPaperBlack")         == sValue ) m_eValue = bordervalueTornPaperBlack;
                    else if ( (L"trees")                  == sValue ) m_eValue = bordervalueTrees;
                    else if ( (L"triangle1")              == sValue ) m_eValue = bordervalueTriangle1;
                    else if ( (L"triangle2")              == sValue ) m_eValue = bordervalueTriangle2;
                    else if ( (L"triangleCircle1")        == sValue ) m_eValue = bordervalueTriangleCircle1;
                    else if ( (L"triangleCircle2")        == sValue ) m_eValue = bordervalueTriangleCircle2;
                    else if ( (L"triangleParty")          == sValue ) m_eValue = bordervalueTriangleParty;
                    else if ( (L"triangles")              == sValue ) m_eValue = bordervalueTriangles;
                    else if ( (L"triple")                 == sValue ) m_eValue = bordervalueTriple;
                    else if ( (L"twistedLines1")          == sValue ) m_eValue = bordervalueTwistedLines1;
                    else if ( (L"twistedLines2")          == sValue ) m_eValue = bordervalueTwistedLines2;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'v':
				{
                    if      ( (L"vine")                   == sValue ) m_eValue = bordervalueVine;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'w':
				{
                    if      ( (L"wave")                   == sValue ) m_eValue = bordervalueWave;
                    else if ( (L"waveline")               == sValue ) m_eValue = bordervalueWaveline;
                    else if ( (L"weavingAngles")          == sValue ) m_eValue = bordervalueWeavingAngles;
                    else if ( (L"weavingBraid")           == sValue ) m_eValue = bordervalueWeavingBraid;
                    else if ( (L"weavingRibbon")          == sValue ) m_eValue = bordervalueWeavingRibbon;
                    else if ( (L"weavingStrips")          == sValue ) m_eValue = bordervalueWeavingStrips;
                    else if ( (L"whiteFlowers")           == sValue ) m_eValue = bordervalueWhiteFlowers;
                    else if ( (L"woodwork")               == sValue ) m_eValue = bordervalueWoodwork;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'x':
				{
                    if      ( (L"xIllusions")             == sValue ) m_eValue = bordervalueXIllusions;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			case 'z':
				{
                    if      ( (L"zanyTriangles")          == sValue ) m_eValue = bordervalueZanyTriangles;
                    else if ( (L"zigZag")                 == sValue ) m_eValue = bordervalueZigZag;
                    else if ( (L"zigZagStitch")           == sValue ) m_eValue = bordervalueZigZagStitch;
                    else                                               m_eValue = eDefValue;
					break;					
				}
			default:
				{
                    m_eValue = eDefValue;
					break;
				}
			}

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case bordervalueApples                : return (L"apples");
			case bordervalueArchedScallops        : return (L"archedScallops");
			case bordervalueBabyPacifier          : return (L"babyPacifier");
			case bordervalueBabyRattle            : return (L"babyRattle");
			case bordervalueBalloons3Colors       : return (L"balloons3Colors");
			case bordervalueBalloonsHotAir        : return (L"balloonsHotAir");
			case bordervalueBasicBlackDashes      : return (L"basicBlackDashes");
			case bordervalueBasicBlackDots        : return (L"basicBlackDots");
			case bordervalueBasicBlackSquares     : return (L"basicBlackSquares");
			case bordervalueBasicThinLines        : return (L"basicThinLines");
			case bordervalueBasicWhiteDashes      : return (L"basicWhiteDashes");
			case bordervalueBasicWhiteDots        : return (L"basicWhiteDots");
			case bordervalueBasicWhiteSquares     : return (L"basicWhiteSquares");
			case bordervalueBasicWideInline       : return (L"basicWideInline");
			case bordervalueBasicWideMidline      : return (L"basicWideMidline");
			case bordervalueBasicWideOutline      : return (L"basicWideOutline");
			case bordervalueBats                  : return (L"bats");
			case bordervalueBirds                 : return (L"birds");
			case bordervalueBirdsFlight           : return (L"birdsFlight");
			case bordervalueCabins                : return (L"cabins");
			case bordervalueCakeSlice             : return (L"cakeSlice");
			case bordervalueCandyCorn             : return (L"candyCorn");
			case bordervalueCelticKnotwork        : return (L"celticKnotwork");
			case bordervalueCertificateBanner     : return (L"certificateBanner");
			case bordervalueChainLink             : return (L"chainLink");
			case bordervalueChampagneBottle       : return (L"champagneBottle");
			case bordervalueCheckedBarBlack       : return (L"checkedBarBlack");
			case bordervalueCheckedBarColor       : return (L"checkedBarColor");
			case bordervalueCheckered             : return (L"checkered");
			case bordervalueChristmasTree         : return (L"christmasTree");
			case bordervalueCirclesLines          : return (L"circlesLines");
			case bordervalueCirclesRectangles     : return (L"circlesRectangles");
			case bordervalueClassicalWave         : return (L"classicalWave");
			case bordervalueClocks                : return (L"clocks");
			case bordervalueCompass               : return (L"compass");
			case bordervalueConfetti              : return (L"confetti");
			case bordervalueConfettiGrays         : return (L"confettiGrays");
			case bordervalueConfettiOutline       : return (L"confettiOutline");
			case bordervalueConfettiStreamers     : return (L"confettiStreamers");
			case bordervalueConfettiWhite         : return (L"confettiWhite");
			case bordervalueCornerTriangles       : return (L"cornerTriangles");
			case bordervalueCouponCutoutDashes    : return (L"couponCutoutDashes");
			case bordervalueCouponCutoutDots      : return (L"couponCutoutDots");
			case bordervalueCrazyMaze             : return (L"crazyMaze");
			case bordervalueCreaturesButterfly    : return (L"creaturesButterfly");
			case bordervalueCreaturesFish         : return (L"creaturesFish");
			case bordervalueCreaturesInsects      : return (L"creaturesInsects");
			case bordervalueCreaturesLadyBug      : return (L"creaturesLadyBug");
			case bordervalueCrossStitch           : return (L"crossStitch");
			case bordervalueCup                   : return (L"cup");
			case bordervalueCustom                : return (L"custom");
			case bordervalueDashDotStroked        : return (L"dashDotStroked");
			case bordervalueDashed                : return (L"dashed");
			case bordervalueDashSmallGap          : return (L"dashSmallGap");
			case bordervalueDecoArch              : return (L"decoArch");
			case bordervalueDecoArchColor         : return (L"decoArchColor");
			case bordervalueDecoBlocks            : return (L"decoBlocks");
			case bordervalueDiamondsGray          : return (L"diamondsGray");
			case bordervalueDotDash               : return (L"dotDash");
			case bordervalueDotDotDash            : return (L"dotDotDash");
			case bordervalueDotted                : return (L"dotted");
			case bordervalueDouble                : return (L"double");
			case bordervalueDoubleD               : return (L"doubleD");
			case bordervalueDoubleDiamonds        : return (L"doubleDiamonds");
			case bordervalueDoubleWave            : return (L"doubleWave");
			case bordervalueEarth1                : return (L"earth1");
			case bordervalueEarth2                : return (L"earth2");
			case bordervalueEarth3                : return (L"earth3");
			case bordervalueEclipsingSquares1     : return (L"eclipsingSquares1");
			case bordervalueEclipsingSquares2     : return (L"eclipsingSquares2");
			case bordervalueEggsBlack             : return (L"eggsBlack");
			case bordervalueFans                  : return (L"fans");
			case bordervalueFilm                  : return (L"film");
			case bordervalueFirecrackers          : return (L"firecrackers");
			case bordervalueFlowersBlockPrint     : return (L"flowersBlockPrint");
			case bordervalueFlowersDaisies        : return (L"flowersDaisies");
			case bordervalueFlowersModern1        : return (L"flowersModern1");
			case bordervalueFlowersModern2        : return (L"flowersModern2");
			case bordervalueFlowersPansy          : return (L"flowersPansy");
			case bordervalueFlowersRedRose        : return (L"flowersRedRose");
			case bordervalueFlowersRoses          : return (L"flowersRoses");
			case bordervalueFlowersTeacup         : return (L"flowersTeacup");
			case bordervalueFlowersTiny           : return (L"flowersTiny");
			case bordervalueGems                  : return (L"gems");
			case bordervalueGingerbreadMan        : return (L"gingerbreadMan");
			case bordervalueGradient              : return (L"gradient");
			case bordervalueHandmade1             : return (L"handmade1");
			case bordervalueHandmade2             : return (L"handmade2");
			case bordervalueHeartBalloon          : return (L"heartBalloon");
			case bordervalueHeartGray             : return (L"heartGray");
			case bordervalueHearts                : return (L"hearts");
			case bordervalueHeebieJeebies         : return (L"heebieJeebies");
			case bordervalueHolly                 : return (L"holly");
			case bordervalueHouseFunky            : return (L"houseFunky");
			case bordervalueHypnotic              : return (L"hypnotic");
			case bordervalueIceCreamCones         : return (L"iceCreamCones");
			case bordervalueInset                 : return (L"inset");
			case bordervalueLightBulb             : return (L"lightBulb");
			case bordervalueLightning1            : return (L"lightning1");
			case bordervalueLightning2            : return (L"lightning2");
			case bordervalueMapleLeaf             : return (L"mapleLeaf");
			case bordervalueMapleMuffins          : return (L"mapleMuffins");
			case bordervalueMapPins               : return (L"mapPins");
			case bordervalueMarquee               : return (L"marquee");
			case bordervalueMarqueeToothed        : return (L"marqueeToothed");
			case bordervalueMoons                 : return (L"moons");
			case bordervalueMosaic                : return (L"mosaic");
			case bordervalueMusicNotes            : return (L"musicNotes");
			case bordervalueNil                   : return (L"nil");
			case bordervalueNone                  : return (L"none");
			case bordervalueNorthwest             : return (L"northwest");
			case bordervalueOutset                : return (L"outset");
			case bordervalueOvals                 : return (L"ovals");
			case bordervaluePackages              : return (L"packages");
			case bordervaluePalmsBlack            : return (L"palmsBlack");
			case bordervaluePalmsColor            : return (L"palmsColor");
			case bordervaluePaperClips            : return (L"paperClips");
			case bordervaluePapyrus               : return (L"papyrus");
			case bordervaluePartyFavor            : return (L"partyFavor");
			case bordervaluePartyGlass            : return (L"partyGlass");
			case bordervaluePencils               : return (L"pencils");
			case bordervaluePeople                : return (L"people");
			case bordervaluePeopleHats            : return (L"peopleHats");
			case bordervaluePeopleWaving          : return (L"peopleWaving");
			case bordervaluePoinsettias           : return (L"poinsettias");
			case bordervaluePostageStamp          : return (L"postageStamp");
			case bordervaluePumpkin1              : return (L"pumpkin1");
			case bordervaluePushPinNote1          : return (L"pushPinNote1");
			case bordervaluePushPinNote2          : return (L"pushPinNote2");
			case bordervaluePyramids              : return (L"pyramids");
			case bordervaluePyramidsAbove         : return (L"pyramidsAbove");
			case bordervalueQuadrants             : return (L"quadrants");
			case bordervalueRings                 : return (L"rings");
			case bordervalueSafari                : return (L"safari");
			case bordervalueSawtooth              : return (L"sawtooth");
			case bordervalueSawtoothGray          : return (L"sawtoothGray");
			case bordervalueScaredCat             : return (L"scaredCat");
			case bordervalueSeattle               : return (L"seattle");
			case bordervalueShadowedSquares       : return (L"shadowedSquares");
			case bordervalueShapes1               : return (L"shapes1");
			case bordervalueShapes2               : return (L"shapes2");
			case bordervalueSharksTeeth           : return (L"sharksTeeth");
			case bordervalueShorebirdTracks       : return (L"shorebirdTracks");
			case bordervalueSingle                : return (L"single");
			case bordervalueSkyrocket             : return (L"skyrocket");
			case bordervalueSnowflakeFancy        : return (L"snowflakeFancy");
			case bordervalueSnowflakes            : return (L"snowflakes");
			case bordervalueSombrero              : return (L"sombrero");
			case bordervalueSouthwest             : return (L"southwest");
			case bordervalueStars                 : return (L"stars");
			case bordervalueStars3d               : return (L"stars3d");
			case bordervalueStarsBlack            : return (L"starsBlack");
			case bordervalueStarsShadowed         : return (L"starsShadowed");
			case bordervalueStarsTop              : return (L"starsTop");
			case bordervalueSun                   : return (L"sun");
			case bordervalueSwirligig             : return (L"swirligig");
			case bordervalueThick                 : return (L"thick");
			case bordervalueThickThinLargeGap     : return (L"thickThinLargeGap");
			case bordervalueThickThinMediumGap    : return (L"thickThinMediumGap");
			case bordervalueThickThinSmallGap     : return (L"thickThinSmallGap");
			case bordervalueThinThickLargeGap     : return (L"thinThickLargeGap");
			case bordervalueThinThickMediumGap    : return (L"thinThickMediumGap");
			case bordervalueThinThickSmallGap     : return (L"thinThickSmallGap");
			case bordervalueThinThickThinLargeGap : return (L"thinThickThinLargeGap");
			case bordervalueThinThickThinMediumGap: return (L"thinThickThinMediumGap");
			case bordervalueThinThickThinSmallGap : return (L"thinThickThinSmallGap");
			case bordervalueThreeDEmboss          : return (L"threeDEmboss");
			case bordervalueThreeDEngrave         : return (L"threeDEngrave");
			case bordervalueTornPaper             : return (L"tornPaper");
			case bordervalueTornPaperBlack        : return (L"tornPaperBlack");
			case bordervalueTrees                 : return (L"trees");
			case bordervalueTriangle1             : return (L"triangle1");
			case bordervalueTriangle2             : return (L"triangle2");
			case bordervalueTriangleCircle1       : return (L"triangleCircle1");
			case bordervalueTriangleCircle2       : return (L"triangleCircle2");
			case bordervalueTriangleParty         : return (L"triangleParty");
			case bordervalueTriangles             : return (L"triangles");
			case bordervalueTriple                : return (L"triple");
			case bordervalueTwistedLines1         : return (L"twistedLines1");
			case bordervalueTwistedLines2         : return (L"twistedLines2");
			case bordervalueVine                  : return (L"vine");
			case bordervalueWave                  : return (L"wave");
			case bordervalueWaveline              : return (L"waveline");
			case bordervalueWeavingAngles         : return (L"weavingAngles");
			case bordervalueWeavingBraid          : return (L"weavingBraid");
			case bordervalueWeavingRibbon         : return (L"weavingRibbon");
			case bordervalueWeavingStrips         : return (L"weavingStrips");
			case bordervalueWhiteFlowers          : return (L"whiteFlowers");
			case bordervalueWoodwork              : return (L"woodwork");
			case bordervalueXIllusions            : return (L"xIllusions");
			case bordervalueZanyTriangles         : return (L"zanyTriangles");
			case bordervalueZigZag                : return (L"zigZag");
			case bordervalueZigZagStitch          : return (L"zigZagStitch");

			default                               : return (L"none");
			}
		}




		SimpleType_FromString     (EBorder)
		SimpleType_Operator_Equal (CBorder)

		bool IsArtBorder () const
		{
            return ((int)m_eValue > 100);
		}
		bool IsLineBorder() const
		{
            return ( (int)m_eValue <= 25 && (int)m_eValue >= 1 );
		}
		bool IsNoBorder  () const
		{
            return ( (int)m_eValue <= 0 );
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

        virtual EBrClear FromString(std::wstring &sValue)
		{
            if      ( (L"all")   == sValue ) m_eValue = brclearAll;
            else if ( (L"left")  == sValue ) m_eValue = brclearLeft;
            else if ( (L"none")  == sValue ) m_eValue = brclearNone;
            else if ( (L"right") == sValue ) m_eValue = brclearRight;
            else                              m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring  ToString  () const
		{
            switch(m_eValue)
			{
			case brclearAll   : return (L"all");
			case brclearLeft  : return (L"left");
			case brclearNone  : return (L"none");
			case brclearRight : return (L"right");
			default           : return (L"all");
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

        virtual EBrType FromString(std::wstring &sValue)
		{
            if      ( (L"column")       == sValue ) m_eValue = brtypeColumn;
            else if ( (L"page")         == sValue ) m_eValue = brtypePage;
            else if ( (L"textWrapping") == sValue ) m_eValue = brtypeTextWrapping;
            else                                     m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case brtypeColumn       : return (L"column");
			case brtypePage         : return (L"page");
			case brtypeTextWrapping : return (L"textWrapping");
			default                 : return (L"page");
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

        virtual ECaptionPos FromString(std::wstring &sValue)
		{
            if      ( (L"above") == sValue ) m_eValue = captionposAbove;
            else if ( (L"below") == sValue ) m_eValue = captionposBelow;
            else if ( (L"left")  == sValue ) m_eValue = captionposLeft;
            else if ( (L"right") == sValue ) m_eValue = captionposRight;
            else                              m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case captionposAbove : return (L"above");
			case captionposBelow : return (L"below");
			case captionposLeft  : return (L"left");
			case captionposRight : return (L"right");
			default              : return (L"above");
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

        virtual EChapterSep FromString(std::wstring &sValue)
		{
            if      ( (L"colon")  == sValue ) m_eValue = chaptersepColon;
            else if ( (L"emDash") == sValue ) m_eValue = chaptersepEmDash;
            else if ( (L"enDash") == sValue ) m_eValue = chaptersepEnDash;
            else if ( (L"hyphen") == sValue ) m_eValue = chaptersepHyphen;
            else if ( (L"period") == sValue ) m_eValue = chaptersepPeriod;
            else                               m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case chaptersepColon  : return (L"colon");
			case chaptersepEmDash : return (L"emDash");
			case chaptersepEnDash : return (L"enDash");
			case chaptersepHyphen : return (L"hyphen");
			case chaptersepPeriod : return (L"period");
			default               : return (L"colon");
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

        virtual ECharacterSpacing FromString(std::wstring &sValue)
		{
            if      ( (L"compressPunctuation")                == sValue ) m_eValue = charspacingCompressPunctuation;
            else if ( (L"compressPunctuationAndJapaneseKana") == sValue ) m_eValue = charspacingCompressPunctuationAndJapaneseKana;
            else if ( (L"doNotCompress")                      == sValue ) m_eValue = charspacingDoNotCompress;
            else                                                           m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(m_eValue)
			{
			case charspacingCompressPunctuation                : return (L"compressPunctuation");
			case charspacingCompressPunctuationAndJapaneseKana : return (L"compressPunctuationAndJapaneseKana");
			case charspacingDoNotCompress                      : return (L"doNotCompress");
			default                                            : return (L"doNotCompress");
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

        virtual ECombineBrackets FromString(std::wstring &sValue)
		{
            if      ( (L"angle")  == sValue ) m_eValue = combinebracketsAngle;
            else if ( (L"curly")  == sValue ) m_eValue = combinebracketsCurly;
            else if ( (L"none")   == sValue ) m_eValue = combinebracketsNone;
            else if ( (L"round")  == sValue ) m_eValue = combinebracketsRound;
            else if ( (L"square") == sValue ) m_eValue = combinebracketsSquare;
            else                               m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring          ToString  () const
		{
            switch(m_eValue)
			{
			case combinebracketsAngle  : return (L"angle");
			case combinebracketsCurly  : return (L"curly");
			case combinebracketsNone   : return (L"none");
			case combinebracketsRound  : return (L"round");
			case combinebracketsSquare : return (L"square");
			default                    : return (L"none");
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

        std::wstring GetValue() const
		{
			return m_sValue;
		}

        void    SetValue(std::wstring &sValue)
		{
			m_sValue = sValue;
		}


        std::wstring FromString(std::wstring &sValue)
		{
			m_sValue = sValue;

			return m_sValue;
		}

        std::wstring ToString  () const
		{
			return m_sValue;
		}

        SimpleType_FromString2    (std::wstring)
		SimpleType_Operator_Equal (CDateTime)

	private:

        std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// DecimalNumber 17.18.10 (Part 1)
	//--------------------------------------------------------------------------------		

	template<int nDefValue = 0>
	class CDecimalNumber : public CSimpleType<int, nDefValue>
	{
	public:
		CDecimalNumber() {}

        virtual int FromString(std::wstring &sValue)
		{
            try
            {
				m_eValue =  _wtoi( sValue.c_str() );
                return m_eValue;
            }
            catch(...)
            {
            }

            try
            {
				m_eValue = static_cast<int>(_wtoi64(sValue.c_str()));
            }
            catch(...)
            {
                m_eValue = 0;
            }

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            return std::to_wstring( m_eValue);
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

		int GetValueIntegerPercent() const
		{
			if(m_bTrailingPercentSign)
			{
				return (int)((m_dValue / 2) * 100);
			}
			else
			{
				return (int)m_dValue;
			}
		}

		void   SetValue(double &dValue)
		{
			m_dValue = dValue;
		}

        double  FromString(std::wstring &sValue, double dDefValue = 0)
		{
            if ( sValue.length() <= 0 )
			{
				m_dValue = dDefValue;
				return m_dValue;
			}

            int nLen = sValue.length();
            wchar_t wsLastChar = sValue[nLen - 1 ];
			if ( wsLastChar == '%' )
			{
				nLen--;
				m_bTrailingPercentSign = true;
			}
			else
				m_bTrailingPercentSign = false;

            m_dValue = _wtof( sValue.substr(0, nLen).c_str() );

			return m_dValue;
		}

        std::wstring ToString  () const
		{
            std::wstring sResult;

			if ( !m_bTrailingPercentSign )
                sResult = std::to_wstring( (int)m_dValue);
			else
				sResult = std::to_wstring(m_dValue) + L"%";

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

        virtual EDirection FromString(std::wstring &sValue)
		{
            if      ( (L"ltr")  == sValue ) m_eValue = directionLTR;
            else if ( (L"rtl")  == sValue ) m_eValue = directionRTL;
            else                             m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring    ToString  () const
		{
            switch(m_eValue)
			{
			case directionLTR : return (L"ltr");
			case directionRTL : return (L"rtl");
			default           : return (L"ltr");
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

        virtual EDisplacedByCustomXml FromString(std::wstring &sValue)
		{
            if      ( (L"next")  == sValue ) m_eValue = displacedbycustomxmlNext;
            else if ( (L"prev")  == sValue ) m_eValue = displacedbycustomxmlPrev;
            else                              m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring          ToString  () const
		{
            switch(m_eValue)
			{
			case displacedbycustomxmlNext : return (L"next");
			case displacedbycustomxmlPrev : return (L"prev");
			default                       : return (L"next");
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

        virtual EDocGrid FromString(std::wstring &sValue)
		{
            if      ( (L"default")       == sValue ) m_eValue = docgridDefault;
            else if ( (L"lines")         == sValue ) m_eValue = docgridLines;
            else if ( (L"linesAndChars") == sValue ) m_eValue = docgridLinesAndChars;
            else if ( (L"snapToChars")   == sValue ) m_eValue = docgridSnapToChars;
            else                                      m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring  ToString  () const
		{
            switch(m_eValue)
			{
			case docgridDefault       : return (L"default");
			case docgridLines         : return (L"lines");
			case docgridLinesAndChars : return (L"linesAndChars");
			case docgridSnapToChars   : return (L"snapToChars");
			default                   : return (L"default");
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

        virtual EDocPartBehavior FromString(std::wstring &sValue)
		{
            if      ( (L"content") == sValue ) m_eValue = docpartbehaviorContent;
            else if ( (L"p")       == sValue ) m_eValue = docpartbehaviorP;
            else if ( (L"pg")      == sValue ) m_eValue = docpartbehaviorPg;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring          ToString  () const
		{
            switch(m_eValue)
			{
			case docpartbehaviorContent : return (L"content");
			case docpartbehaviorP       : return (L"p");
			case docpartbehaviorPg      : return (L"pg");
			default                     : return (L"content");
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

        virtual EDocPartGallery FromString(std::wstring &sValue)
		{
            if      ( (L"any")               == sValue ) m_eValue = docpartgalleryAny;
            else if ( (L"autoTxt")           == sValue ) m_eValue = docpartgalleryAutoTxt;
            else if ( (L"bib")               == sValue ) m_eValue = docpartgalleryBib;
            else if ( (L"coverPg")           == sValue ) m_eValue = docpartgalleryCoverPg;
            else if ( (L"custAutoTxt")       == sValue ) m_eValue = docpartgalleryCustAutoTxt;
            else if ( (L"custBib")           == sValue ) m_eValue = docpartgalleryCustBib;
            else if ( (L"custCoverPg")       == sValue ) m_eValue = docpartgalleryCustCoverPg;
            else if ( (L"custEq")            == sValue ) m_eValue = docpartgalleryCustEq;
            else if ( (L"custFtrs")          == sValue ) m_eValue = docpartgalleryCustFtrs;
            else if ( (L"custHdrs")          == sValue ) m_eValue = docpartgalleryCustHdrs;
            else if ( (L"custom1")           == sValue ) m_eValue = docpartgalleryCustom1;
            else if ( (L"custom2")           == sValue ) m_eValue = docpartgalleryCustom2;
            else if ( (L"custom3")           == sValue ) m_eValue = docpartgalleryCustom3;
            else if ( (L"custom4")           == sValue ) m_eValue = docpartgalleryCustom4;
            else if ( (L"custom5")           == sValue ) m_eValue = docpartgalleryCustom5;
            else if ( (L"custPgNum")         == sValue ) m_eValue = docpartgalleryCustPgNum;
            else if ( (L"custPgNumB")        == sValue ) m_eValue = docpartgalleryCustPgNumB;
            else if ( (L"custPgNumMargins")  == sValue ) m_eValue = docpartgalleryCustPgNumMargins;
            else if ( (L"custPgNumT")        == sValue ) m_eValue = docpartgalleryCustPgNumT;
            else if ( (L"custQuickParts")    == sValue ) m_eValue = docpartgalleryCustQuickParts;
            else if ( (L"custTblOfContents") == sValue ) m_eValue = docpartgalleryCustTblOfContents;
            else if ( (L"custTbls")          == sValue ) m_eValue = docpartgalleryCustTbls;
            else if ( (L"custTxtBox")        == sValue ) m_eValue = docpartgalleryCustTxtBox;
            else if ( (L"custWatermarks")    == sValue ) m_eValue = docpartgalleryCustWatermarks;
            else if ( (L"default")           == sValue ) m_eValue = docpartgalleryDefault;
            else if ( (L"docParts")          == sValue ) m_eValue = docpartgalleryDocParts;
            else if ( (L"eq")                == sValue ) m_eValue = docpartgalleryEq;
            else if ( (L"ftrs")              == sValue ) m_eValue = docpartgalleryFtrs;
            else if ( (L"hdrs")              == sValue ) m_eValue = docpartgalleryHdrs;
            else if ( (L"pgNum")             == sValue ) m_eValue = docpartgalleryPgNum;
            else if ( (L"pgNumB")            == sValue ) m_eValue = docpartgalleryPgNumB;
            else if ( (L"pgNumMargins")      == sValue ) m_eValue = docpartgalleryPgNumMargins;
            else if ( (L"pgNumT")            == sValue ) m_eValue = docpartgalleryPgNumT;
            else if ( (L"placeholder")       == sValue ) m_eValue = docpartgalleryPlaceHolder;
            else if ( (L"tblOfContents")     == sValue ) m_eValue = docpartgalleryTblOfContents;
            else if ( (L"tbls")              == sValue ) m_eValue = docpartgalleryTbls;
            else if ( (L"txtBox")            == sValue ) m_eValue = docpartgalleryTxtBox;
            else if ( (L"watermarks")        == sValue ) m_eValue = docpartgalleryWatermarks;
            else                                          m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring         ToString  () const
		{
            switch(m_eValue)
			{
			case docpartgalleryAny               : return (L"any");
			case docpartgalleryAutoTxt           : return (L"autoTxt");
			case docpartgalleryBib               : return (L"bib");
			case docpartgalleryCoverPg           : return (L"coverPg");
			case docpartgalleryCustAutoTxt       : return (L"custAutoTxt");
			case docpartgalleryCustBib           : return (L"custBib");
			case docpartgalleryCustCoverPg       : return (L"custCoverPg");
			case docpartgalleryCustEq            : return (L"custEq");
			case docpartgalleryCustFtrs          : return (L"custFtrs");
			case docpartgalleryCustHdrs          : return (L"custHdrs");
			case docpartgalleryCustom1           : return (L"custom1");
			case docpartgalleryCustom2           : return (L"custom2");
			case docpartgalleryCustom3           : return (L"custom3");
			case docpartgalleryCustom4           : return (L"custom4");
			case docpartgalleryCustom5           : return (L"custom5");
			case docpartgalleryCustPgNum         : return (L"custPgNum");
			case docpartgalleryCustPgNumB        : return (L"custPgNumB");
			case docpartgalleryCustPgNumMargins  : return (L"custPgNumMargins");
			case docpartgalleryCustPgNumT        : return (L"custPgNumT");
			case docpartgalleryCustQuickParts    : return (L"custQuickParts");
			case docpartgalleryCustTblOfContents : return (L"custTblOfContents");
			case docpartgalleryCustTbls          : return (L"custTbls");
			case docpartgalleryCustTxtBox        : return (L"custTxtBox");
			case docpartgalleryCustWatermarks    : return (L"custWatermarks");
			case docpartgalleryDefault           : return (L"default");
			case docpartgalleryDocParts          : return (L"docParts");
			case docpartgalleryEq                : return (L"eq");
			case docpartgalleryFtrs              : return (L"ftrs");
			case docpartgalleryHdrs              : return (L"hdrs");
			case docpartgalleryPgNum             : return (L"pgNum");
			case docpartgalleryPgNumB            : return (L"pgNumB");
			case docpartgalleryPgNumMargins      : return (L"pgNumMargins");
			case docpartgalleryPgNumT            : return (L"pgNumT");
			case docpartgalleryPlaceHolder       : return (L"placeholder");
			case docpartgalleryTblOfContents     : return (L"tblOfContents");
			case docpartgalleryTbls              : return (L"tbls");
			case docpartgalleryTxtBox            : return (L"txtBox");
			case docpartgalleryWatermarks        : return (L"watermarks");
			default                              : return (L"default");
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

        virtual EDocPartType FromString(std::wstring &sValue)
		{
            if      ( (L"autoExp")  == sValue ) m_eValue = docparttypeAutoExp;
            else if ( (L"bbPlcHdr") == sValue ) m_eValue = docparttypeBBPlcHdr;
            else if ( (L"formFld")  == sValue ) m_eValue = docparttypeFormFld;
            else if ( (L"none")     == sValue ) m_eValue = docparttypeNone;
            else if ( (L"normal")   == sValue ) m_eValue = docparttypeNormal;
            else if ( (L"speller")  == sValue ) m_eValue = docparttypeSpeller;
            else if ( (L"toolbar")  == sValue ) m_eValue = docparttypeToolbar;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring      ToString  () const
		{
            switch(m_eValue)
			{
			case docparttypeAutoExp  : return (L"autoExp");
			case docparttypeBBPlcHdr : return (L"bbPlcHdr");
			case docparttypeFormFld  : return (L"formFld");
			case docparttypeNone     : return (L"none");
			case docparttypeNormal   : return (L"normal");
			case docparttypeSpeller  : return (L"speller");
			case docparttypeToolbar  : return (L"toolbar");
			default                  : return (L"none");
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

        virtual EDocProtect FromString(std::wstring &sValue)
		{
            if      ( (L"comments")       == sValue ) m_eValue = docprotectComments;
            else if ( (L"forms")          == sValue ) m_eValue = docprotectForms;
            else if ( (L"none")           == sValue ) m_eValue = docprotectNone;
            else if ( (L"readOnly")       == sValue ) m_eValue = docprotectReadOnly;
            else if ( (L"trackedChanges" )== sValue ) m_eValue = docprotectTrackedChanges;
            else                                       m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case docprotectComments       : return (L"comments");
			case docprotectForms          : return (L"forms");
			case docprotectNone           : return (L"none");
			case docprotectReadOnly       : return (L"readOnly");
			case docprotectTrackedChanges : return (L"trackedChanges");
			default                       : return (L"none");
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

        virtual EDocType FromString(std::wstring &sValue)
		{
            if      ( (L"eMail")        == sValue ) m_eValue = doctypeEMail;
            else if ( (L"letter")       == sValue ) m_eValue = doctypeLetter;
            else if ( (L"notSpecified") == sValue ) m_eValue = doctypeNotSpecified;
            else                                     m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring  ToString  () const
		{
            switch(m_eValue)
			{
			case doctypeEMail        : return (L"eMail");
			case doctypeLetter       : return (L"letter");
			case doctypeNotSpecified : return (L"notSpecified");
			default                  : return (L"notSpecified");
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

        virtual EDropCap FromString(std::wstring &sValue)
		{
            if      ( (L"drop")   == sValue ) m_eValue = dropcapDrop;
            else if ( (L"margin") == sValue ) m_eValue = dropcapMargin;
            else if ( (L"none")   == sValue ) m_eValue = dropcapNone;
            else                               m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring  ToString  () const
		{
            switch(m_eValue)
			{
			case dropcapDrop   : return (L"drop");
			case dropcapMargin : return (L"margin");
			case dropcapNone   : return (L"none");
			default            : return (L"none");
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

        virtual EEdGrp  FromString(std::wstring &sValue)
		{
            if      ( (L"administrators") == sValue ) m_eValue = edgrpAdministrators;
            else if ( (L"contributors")   == sValue ) m_eValue = edgrpContributors;
            else if ( (L"current")        == sValue ) m_eValue = edgrpCurrent;
            else if ( (L"editors")        == sValue ) m_eValue = edgrpEditors;
            else if ( (L"everyone")       == sValue ) m_eValue = edgrpEveryone;
            else if ( (L"none")           == sValue ) m_eValue = edgrpNone;
            else if ( (L"owners")         == sValue ) m_eValue = edgrpOwners;
            else                                       m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case edgrpAdministrators : return (L"administrators");
			case edgrpContributors   : return (L"contributors");
			case edgrpCurrent        : return (L"current");
			case edgrpEditors        : return (L"editors");
			case edgrpEveryone       : return (L"everyone");
			case edgrpNone           : return (L"none");
			case edgrpOwners         : return (L"owners");
			default                  : return (L"everyone");
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

        virtual EEdnPos FromString(std::wstring &sValue)
		{
            if      ( (L"docEnd")  == sValue ) m_eValue = ednposDocEnd;
            else if ( (L"sectEnd") == sValue ) m_eValue = ednposSectEnd;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case ednposDocEnd  : return (L"docEnd");
			case ednposSectEnd : return (L"sectEnd");
			default            : return (L"sectEnd");
			}
		}

		SimpleType_FromString     (EEdnPos)
		SimpleType_Operator_Equal (CEdnPos)
	};


	//--------------------------------------------------------------------------------
	// EighthPointMeasure 17.18.23 (Part 1)
	//--------------------------------------------------------------------------------		

	template<int nDefValue = 0>
	class CEighthPointMeasure : public CSimpleType<int, nDefValue>
	{
	public:
		CEighthPointMeasure() {}

        virtual int     FromString(std::wstring &sValue)
		{
            if (sValue == (L"auto"))
            {

            }
            else
            {
				m_eValue = sValue.empty() ? 0 : _wtoi( sValue.c_str() );
            }

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            std::wstring sResult = std::to_wstring( m_eValue);

			return sResult;
		}


		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CEighthPointMeasure)

		double ToPoints()
		{
            return m_eValue / 8.0;
		}
		double ToMM()
		{
            return Pt_To_Mm(m_eValue / 8.0);
		}
		double ToInches()
		{
            return m_eValue / 576.0;
		}

		int    FromPoints(double dValue)
		{
            m_eValue = dValue * 8;
            return m_eValue;
		}
		int    FromInches(double dValue)
		{
            m_eValue = dValue * 576;
            return m_eValue;
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

        virtual EEm     FromString(std::wstring &sValue)
		{
            if      ( (L"circle")   == sValue ) m_eValue = emCircle;
            else if ( (L"comma")    == sValue ) m_eValue = emComma;
            else if ( (L"dot")      == sValue ) m_eValue = emDot;
            else if ( (L"none")     == sValue ) m_eValue = emNone;
            else if ( (L"underDot") == sValue ) m_eValue = emUnderDot;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case emCircle   : return (L"circle");
			case emComma    : return (L"comma");
			case emDot      : return (L"dot");
			case emNone     : return (L"none");
			case emUnderDot : return (L"underDot");
			default         : return (L"none");
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

        std::wstring GetValue() const
		{
			return m_sValue;
		}

        void    SetValue(std::wstring &sValue)
		{
            if ( sValue.length() <= 256 )
				m_sValue = sValue;
			else
                m_sValue = sValue.substr( 0, 256 );
		}


        std::wstring FromString(std::wstring &sValue)
		{
            if ( sValue.length() <= 256 )
				m_sValue = sValue;
			else
                m_sValue = sValue.substr( 0, 256 );

			return m_sValue;
		}

        std::wstring ToString  () const
		{
			return m_sValue;
		}

        SimpleType_FromString2    (std::wstring)
		SimpleType_Operator_Equal (CFFHelpTextVal)
	private:

        std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// FFName 17.18.26 (Part 1)
	//--------------------------------------------------------------------------------		

	class CFFName
	{
	public:
		CFFName() {}

        std::wstring GetValue() const
		{
			return m_sValue;
		}

        void    SetValue(std::wstring &sValue)
		{
            if ( sValue.length() <= 65 )
				m_sValue = sValue;
			else
                m_sValue = sValue.substr( 0, 65 );
		}


        std::wstring FromString(std::wstring &sValue)
		{
            if ( sValue.length() <= 65 )
				m_sValue = sValue;
			else
                m_sValue = sValue.substr( 0, 65 );

			return m_sValue;
		}

        std::wstring ToString  () const
		{
			return m_sValue;
		}

        SimpleType_FromString2    (std::wstring)
		SimpleType_Operator_Equal (CFFName)
	private:

        std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// FFStatusTextVal 17.18.27 (Part 1)
	//--------------------------------------------------------------------------------		

	class CFFStatusTextVal
	{
	public:
		CFFStatusTextVal() {}

        std::wstring GetValue() const
		{
			return m_sValue;
		}

        void    SetValue(std::wstring &sValue)
		{
            if ( sValue.length() <= 140 )
				m_sValue = sValue;
			else
                m_sValue = sValue.substr( 0, 140 );
		}


        std::wstring FromString(std::wstring &sValue)
		{
            if ( sValue.length() <= 140 )
				m_sValue = sValue;
			else
                m_sValue = sValue.substr( 0, 140 );

			return m_sValue;
		}

        std::wstring ToString  () const
		{
			return m_sValue;
		}

        SimpleType_FromString2    (std::wstring)
		SimpleType_Operator_Equal (CFFStatusTextVal)
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

	template<EFFTextType eDefValue = fftexttypeRegular>
	class CFFTextType : public CSimpleType<EFFTextType, eDefValue>
	{
	public:
		CFFTextType() {}

        virtual EFFTextType FromString(std::wstring &sValue)
		{
            if      ( (L"calculated")  == sValue ) m_eValue = fftexttypeCalculated;
            else if ( (L"currentDate") == sValue ) m_eValue = fftexttypeCurrentDate;
            else if ( (L"currentTime") == sValue ) m_eValue = fftexttypeCurrentTime;
            else if ( (L"date")        == sValue ) m_eValue = fftexttypeDate;
            else if ( (L"number")      == sValue ) m_eValue = fftexttypeNumber;
            else if ( (L"regular")     == sValue ) m_eValue = fftexttypeRegular;
            else                                    m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case fftexttypeCalculated  : return (L"calculated");
			case fftexttypeCurrentDate : return (L"currentDate");
			case fftexttypeCurrentTime : return (L"currentTime");
			case fftexttypeDate        : return (L"date");
			case fftexttypeNumber      : return (L"number");
			case fftexttypeRegular     : return (L"regular");
			default                    : return (L"regular");
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

        virtual EFldCharType FromString(std::wstring &sValue)
		{
            if      ( (L"begin")    == sValue ) m_eValue = fldchartypeBegin;
            else if ( (L"end")      == sValue ) m_eValue = fldchartypeEnd;
            else if ( (L"separate") == sValue ) m_eValue = fldchartypeSeparate;
            else                                 m_eValue = eDefVal;

            return m_eValue;
		}

        virtual std::wstring      ToString  () const
		{
            switch(m_eValue)
			{
			case fldchartypeBegin    : return (L"begin");
			case fldchartypeEnd      : return (L"end");
			case fldchartypeSeparate : return (L"separate");
			default                  : return (L"begin");
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

        virtual EFontFamily FromString(std::wstring &sValue)
		{
            if      ( (L"auto")       == sValue ) m_eValue = fontfamilyAuto;
            else if ( (L"decorative") == sValue ) m_eValue = fontfamilyDecorative;
            else if ( (L"modern")     == sValue ) m_eValue = fontfamilyModern;
            else if ( (L"roman")      == sValue ) m_eValue = fontfamilyRoman;
            else if ( (L"script")     == sValue ) m_eValue = fontfamilyScript;
            else if ( (L"swiss")      == sValue ) m_eValue = fontfamilySwiss;
            else                                   m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case fontfamilyAuto       : return (L"auto");
			case fontfamilyDecorative : return (L"decorative");
			case fontfamilyModern     : return (L"modern");
			case fontfamilyRoman      : return (L"roman");
			case fontfamilyScript     : return (L"script");
			case fontfamilySwiss      : return (L"swiss");
			default                   : return (L"auto");
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

        virtual EFrameLayout FromString(std::wstring &sValue)
		{
            if      ( (L"cols") == sValue ) m_eValue = framelayoutCols;
            else if ( (L"none") == sValue ) m_eValue = framelayoutNone;
            else if ( (L"rows") == sValue ) m_eValue = framelayoutRows;
            else                             m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring      ToString  () const
		{
            switch(m_eValue)
			{
			case framelayoutCols : return (L"cols");
			case framelayoutNone : return (L"none");
			case framelayoutRows : return (L"rows");
			default              : return (L"none");
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

        virtual EFrameScrollbar FromString(std::wstring &sValue)
		{
            if      ( (L"auto") == sValue ) m_eValue = framescrollbarAuto;
            else if ( (L"off")  == sValue ) m_eValue = framescrollbarOff;
            else if ( (L"on")   == sValue ) m_eValue = framescrollbarOn;
            else                             m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring         ToString  () const
		{
            switch(m_eValue)
			{
			case framescrollbarAuto : return (L"auto");
			case framescrollbarOff  : return (L"off");
			case framescrollbarOn   : return (L"on");
			default                 : return (L"auto");
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

        virtual EFtnEdn FromString(std::wstring &sValue)
		{
            if      ( (L"continuationNotice")    == sValue ) m_eValue = ftnednContinuationNotice;
            else if ( (L"continuationSeparator") == sValue ) m_eValue = ftnednContinuationSeparator;
            else if ( (L"normal")                == sValue ) m_eValue = ftnednNormal;
            else if ( (L"separator")             == sValue ) m_eValue = ftnednSeparator;
            else                                              m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case ftnednContinuationNotice    : return (L"continuationNotice");
			case ftnednContinuationSeparator : return (L"continuationSeparator");
			case ftnednNormal                : return (L"normal");
			case ftnednSeparator             : return (L"separator");
			default                          : return (L"normal");
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

        virtual EFtnPos FromString(std::wstring &sValue)
		{
            if      ( (L"beneathText") == sValue ) m_eValue = ftnposBeneathText;
            else if ( (L"docEnd")      == sValue ) m_eValue = ftnposDocEnd;
            else if ( (L"pageBottom")  == sValue ) m_eValue = ftnposPageBottom;
            else if ( (L"sectEnd")     == sValue ) m_eValue = ftnposSectEnd;
            else                                    m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case ftnposBeneathText : return (L"beneathText");
			case ftnposDocEnd      : return (L"docEnd");
			case ftnposPageBottom  : return (L"pageBottom");
			case ftnposSectEnd     : return (L"sectEnd");
			default                : return (L"sectEnd");
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

        virtual EHAnchor FromString(std::wstring &sValue)
		{
            if      ( (L"margin") == sValue ) m_eValue = hanchorMargin;
            else if ( (L"page")   == sValue ) m_eValue = hanchorPage;
            else if ( (L"text")   == sValue ) m_eValue = hanchorText;
            else                               m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring  ToString  () const
		{
            switch(m_eValue)
			{
			case hanchorMargin : return (L"margin");
			case hanchorPage   : return (L"page");
			case hanchorText   : return (L"text");
			default            : return (L"text");
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

        virtual EHdrFtr FromString(std::wstring &sValue)
		{
            if      ( (L"default") == sValue ) m_eValue = hdrftrDefault;
            else if ( (L"even")    == sValue ) m_eValue = hdrftrEven;
            else if ( (L"first")   == sValue ) m_eValue = hdrftrFirst;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case hdrftrDefault : return (L"default");
			case hdrftrEven    : return (L"even");
			case hdrftrFirst   : return (L"first");
			default            : return (L"default");
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

        virtual EHeightRule FromString(std::wstring &sValue)
		{
            if      ( (L"atLeast") == sValue ) m_eValue = heightruleAtLeast;
            else if ( (L"auto")    == sValue ) m_eValue = heightruleAuto;
            else if ( (L"exact")   == sValue ) m_eValue = heightruleExact;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case heightruleAtLeast : return (L"atLeast");
			case heightruleAuto    : return (L"auto");
			case heightruleExact   : return (L"exact");
			default                : return (L"auto");
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
            m_eValue = hexcolorRGB;
			m_unR = r;
			m_unG = g;
			m_unB = b;
		}

        void SetColorByString ();

        virtual EHexColor FromString(std::wstring &sValue);

        virtual std::wstring   ToString  () const
		{
            switch(m_eValue)
			{
				case hexcolorRGB  : 
				{
					std::wstringstream sstream;
					sstream << L"00" << boost::wformat( L"%02x%02x%02x" ) % m_unR % m_unG % m_unB;
					
					return sstream.str();
				}
				case hexcolorAuto : 
				default :
					return (L"auto");
			}
		}
        virtual std::wstring   ToStringNoAlpha  () const
		{
			switch(m_eValue)
			{
				case hexcolorRGB  : 
				{
					std::wstringstream sstream;
					sstream << boost::wformat( L"%02x%02x%02x" ) % m_unR % m_unG % m_unB;
					
					return sstream.str();
				}
				case hexcolorAuto : 
				default :
					return (L"auto");			}
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
            if ( m_sValue.length() < 6 )
				return;

			m_unR = HexToInt( (int)m_sValue[1] ) + (unsigned char)(HexToInt( (int)m_sValue[0] ) << 4);
			m_unG = HexToInt( (int)m_sValue[3] ) + (unsigned char)(HexToInt( (int)m_sValue[2] ) << 4);
			m_unB = HexToInt( (int)m_sValue[5] ) + (unsigned char)(HexToInt( (int)m_sValue[4] ) << 4);
		}
		void Parse3()
		{
            if ( m_sValue.length() < 3 )
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

        std::wstring       m_sValue;

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

	template<EHexColorAuto eDefValue = hexcolorautoAuto>
	class CHexColorAuto : public CSimpleType<EHexColorAuto, eDefValue>
	{
	public:
		CHexColorAuto() {}

        virtual EHexColorAuto FromString(std::wstring &sValue)
		{
            m_eValue = hexcolorautoAuto;

            return m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
			return (L"auto");
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

        virtual EHighlightColor FromString(std::wstring &sValue)
		{
			if      ( (L"black")       == sValue )
			{
                m_eValue = highlightcolorBlack;

				m_unR = 0x00;
				m_unG = 0x00;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( (L"blue")        == sValue )
			{
                m_eValue = highlightcolorBlue;
				m_unR = 0x00;
				m_unG = 0x00;
				m_unB = 0xFF;
				m_unA = 0xFF;
			}
			else if ( (L"cyan")        == sValue )
			{
                m_eValue = highlightcolorCyan;
				m_unR = 0x00;
				m_unG = 0xFF;
				m_unB = 0xFF;
				m_unA = 0xFF;
			}
			else if ( (L"darkBlue")    == sValue )
			{
                m_eValue = highlightcolorDarkBlue;
				m_unR = 0x00;
				m_unG = 0x00;
				m_unB = 0x8B;
				m_unA = 0xFF;
			}
			else if ( (L"darkCyan")    == sValue )
			{
                m_eValue = highlightcolorDarkCyan;
				m_unR = 0x00;
				m_unG = 0x8B;
				m_unB = 0x8B;
				m_unA = 0xFF;
			}
			else if ( (L"darkGray")    == sValue )
			{
                m_eValue = highlightcolorDarkGray;
				m_unR = 0xA9;
				m_unG = 0xA9;
				m_unB = 0xA9;
				m_unA = 0xFF;
			}
			else if ( (L"darkGreen")   == sValue )
			{
                m_eValue = highlightcolorDarkGreen;
				m_unR = 0x00;
				m_unG = 0x64;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( (L"darkMagenta") == sValue )
			{
                m_eValue = highlightcolorDarkMagenta;
				m_unR = 0x80;
				m_unG = 0x00;
				m_unB = 0x80;
				m_unA = 0xFF;
			}
			else if ( (L"darkRed")     == sValue )
			{
                m_eValue = highlightcolorDarkRed;
				m_unR = 0x8B;
				m_unG = 0x00;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( (L"darkYellow")  == sValue )
			{
                m_eValue = highlightcolorDarkYellow;
				m_unR = 0x80;
				m_unG = 0x80;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( (L"green")       == sValue )
			{
                m_eValue = highlightcolorGreen;
				m_unR = 0x00;
				m_unG = 0xFF;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( (L"lightGray")   == sValue )
			{
                m_eValue = highlightcolorLightGray;
				m_unR = 0xD3;
				m_unG = 0xD3;
				m_unB = 0xD3;
				m_unA = 0xFF;
			}
			else if ( (L"magenta")     == sValue )
			{
                m_eValue = highlightcolorMagenta;
				m_unR = 0xFF;
				m_unG = 0x00;
				m_unB = 0xFF;
				m_unA = 0xFF;
			}
			else if ( (L"none")        == sValue )
			{
                m_eValue = highlightcolorNone;
				m_unR = 0x00;
				m_unG = 0x00;
				m_unB = 0x00;
				m_unA = 0x00;
			}
			else if ( (L"red")         == sValue )
			{
                m_eValue = highlightcolorRed;
				m_unR = 0xFF;
				m_unG = 0x00;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else if ( (L"white")       == sValue )
			{
                m_eValue = highlightcolorWhite;
				m_unR = 0xFF;
				m_unG = 0xFF;
				m_unB = 0xFF;
				m_unA = 0xFF;
			}
			else if ( (L"yellow")      == sValue )
			{
                m_eValue = highlightcolorYellow;
				m_unR = 0xFF;
				m_unG = 0xFF;
				m_unB = 0x00;
				m_unA = 0xFF;
			}
			else
			{
                m_eValue = eDefValue;

				// Инициализируем цвет
                FromString( ToString() );
			}

            return m_eValue;
		}

        virtual std::wstring         ToString  () const
		{
            switch(m_eValue)
			{
			case highlightcolorBlack       : return (L"black");
			case highlightcolorBlue        : return (L"blue");
			case highlightcolorCyan        : return (L"cyan");
			case highlightcolorDarkBlue    : return (L"darkBlue");
			case highlightcolorDarkCyan    : return (L"darkCyan");
			case highlightcolorDarkGray    : return (L"darkGray");
			case highlightcolorDarkGreen   : return (L"darkGreen");
			case highlightcolorDarkMagenta : return (L"darkMagenta");
			case highlightcolorDarkRed     : return (L"darkRed");
			case highlightcolorDarkYellow  : return (L"darkYellow");
			case highlightcolorGreen       : return (L"green");
			case highlightcolorLightGray   : return (L"lightGray");
			case highlightcolorMagenta     : return (L"magenta");
			case highlightcolorNone        : return (L"none");
			case highlightcolorRed         : return (L"red");
			case highlightcolorWhite       : return (L"white");
			case highlightcolorYellow      : return (L"yellow");
			default                        : return (L"none");
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

        virtual EHint   FromString(std::wstring &sValue)
		{
            if      ( (L"cs")       == sValue ) m_eValue = hintCs;
            else if ( (L"default")  == sValue ) m_eValue = hintDefault;
            else if ( (L"eastAsia") == sValue ) m_eValue = hintEastAsia;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case hintCs       : return (L"cs");
			case hintDefault  : return (L"default");
			case hintEastAsia : return (L"eastAsia");
			default           : return (L"default");
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

        virtual double  FromString(std::wstring &sValue)
		{
			Parse(sValue, 2);

			m_dValue = fabs( m_dValue );

			return m_dValue;
		}

        virtual std::wstring ToString  () const
		{
            std::wstring sResult;

			if (m_bUnit)
			{
				sResult = std::to_wstring( m_dValue ) + L"pt";
			}
			else
			{
				sResult = std::to_wstring( (int)(m_dValue * 2) );
			}

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

        virtual EInfoTextType FromString(std::wstring &sValue)
		{
            if      ( (L"autoText") == sValue ) m_eValue = infotexttypeAutoText;
            else if ( (L"text")     == sValue ) m_eValue = infotexttypeText;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(m_eValue)
			{
			case infotexttypeAutoText : return (L"autoText");
			case infotexttypeText     : return (L"text");
			default                   : return (L"text");
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

        virtual EJc     FromString(std::wstring &sValue)
		{
            if      ( (L"both")           == sValue ) m_eValue = jcBoth;
            else if ( (L"center")         == sValue ) m_eValue = jcCenter;
            else if ( (L"distribute")     == sValue ) m_eValue = jcDistribute;
            else if ( (L"end")            == sValue ) m_eValue = jcEnd;
            else if ( (L"highKashida")    == sValue ) m_eValue = jcHighKashida;
            else if ( (L"lowKashida")     == sValue ) m_eValue = jcLowKashida;
            else if ( (L"mediumKashida")  == sValue ) m_eValue = jcMediumKashida;
            else if ( (L"numTab")         == sValue ) m_eValue = jcNumTab;
            else if ( (L"start")          == sValue ) m_eValue = jcStart;
            else if ( (L"thaiDistribute") == sValue ) m_eValue = jcThaiDistribute;
            else if ( (L"left")           == sValue ) m_eValue = jcLeft;
            else if ( (L"right")          == sValue ) m_eValue = jcRight;
            else                                       m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case jcBoth            : return (L"both");
			case jcCenter          : return (L"center");
			case jcDistribute      : return (L"distribute");
			case jcEnd             : return (L"end");
			case jcHighKashida     : return (L"highKashida");
			case jcLowKashida      : return (L"lowKashida");
			case jcMediumKashida   : return (L"mediumKashida");
			case jcNumTab          : return (L"numTab");
			case jcStart           : return (L"start");
			case jcThaiDistribute  : return (L"thaiDistribute");
			case jcLeft            : return (L"left");
			case jcRight           : return (L"right");
			default                : return (L"left");
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

        virtual EJcTable FromString(std::wstring &sValue)
		{
            if      ( (L"center")         == sValue ) m_eValue = jctableCenter;
            else if ( (L"end")            == sValue ) m_eValue = jctableEnd;
            else if ( (L"start")          == sValue ) m_eValue = jctableStart;
            else if ( (L"left")           == sValue ) m_eValue = jctableLeft;
            else if ( (L"right")          == sValue ) m_eValue = jctableRight;
            else                                       m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring  ToString  () const
		{
            switch(m_eValue)
			{
			case jctableCenter : return (L"center");
			case jctableEnd    : return (L"end");
			case jctableStart  : return (L"start");
			case jctableLeft   : return (L"left");
			case jctableRight  : return (L"right");
			default            : return (L"left");
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

        virtual ELevelSuffix FromString(std::wstring &sValue)
		{
            if      ( (L"nothing") == sValue ) m_eValue = levelsuffixNothing;
            else if ( (L"space")   == sValue ) m_eValue = levelsuffixSpace;
            else if ( (L"tab")     == sValue ) m_eValue = levelsuffixTab;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring      ToString  () const
		{
            switch(m_eValue)
			{
			case levelsuffixNothing : return (L"nothing");
			case levelsuffixSpace   : return (L"space");
			case levelsuffixTab     : return (L"tab");
			default                 : return (L"nothing");
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

        virtual ELineNumberRestart FromString(std::wstring &sValue)
		{
            if      ( (L"continuous") == sValue ) m_eValue = linenumberrestartContinious;
            else if ( (L"newPage")    == sValue ) m_eValue = linenumberrestartNewPage;
            else if ( (L"newSection") == sValue ) m_eValue = linenumberrestartNewSection;
            else                                   m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(m_eValue)
			{
			case linenumberrestartContinious : return (L"continuous");
			case linenumberrestartNewPage    : return (L"newPage");
			case linenumberrestartNewSection : return (L"newSection");
			default                          : return (L"newPage");
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

        virtual ELineSpacingRule FromString(std::wstring &sValue)
		{
            if      ( (L"atLeast") == sValue ) m_eValue = linespacingruleAtLeast;
            else if ( (L"auto")    == sValue ) m_eValue = linespacingruleAuto;
            else if ( (L"exact")   == sValue ) m_eValue = linespacingruleExact;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring          ToString  () const
		{
            switch(m_eValue)
			{
			case linespacingruleAtLeast : return (L"atLeast");
			case linespacingruleAuto    : return (L"auto");
			case linespacingruleExact   : return (L"exact");
			default                     : return (L"auto");
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

        virtual ELock   FromString(std::wstring &sValue)
		{
            if      ( (L"contentLocked")    == sValue ) m_eValue = lockContentLocked;
            else if ( (L"sdtContentLocked") == sValue ) m_eValue = lockSdtContentLocked;
            else if ( (L"sdtLocked")        == sValue ) m_eValue = lockSdtLocked;
            else if ( (L"unlocked")         == sValue ) m_eValue = lockUnlocked;
            else                                         m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case lockContentLocked    : return (L"contentLocked");
			case lockSdtContentLocked : return (L"sdtContentLocked");
			case lockSdtLocked        : return (L"sdtLocked");
			case lockUnlocked         : return (L"unlocked");
			default                   : return (L"unlocked");
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

        virtual int     FromString(std::wstring &sValue)
		{
			if ( !Parse(sValue) )
			{
                m_eValue = nDefValue;
			}

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
			return XmlUtils::IntToString(m_eValue, L"%08X");
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CLongHexNumber)
	private:

        bool Parse(std::wstring &sValue)
		{
            if ( sValue.length() < 8 )
				return false;

			bool bResult = true;

            m_eValue  = HexToInt( (int)sValue[7], bResult );
            m_eValue += HexToInt( (int)sValue[6], bResult ) <<  4;
            m_eValue += HexToInt( (int)sValue[5], bResult ) <<  8;
            m_eValue += HexToInt( (int)sValue[4], bResult ) << 12;
            m_eValue += HexToInt( (int)sValue[3], bResult ) << 16;
            m_eValue += HexToInt( (int)sValue[2], bResult ) << 20;
            m_eValue += HexToInt( (int)sValue[1], bResult ) << 24;
            m_eValue += HexToInt( (int)sValue[0], bResult ) << 28;

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

        std::wstring GetValue() const
		{
			return m_sValue;
		}

        void    SetValue(std::wstring &sValue)
		{
            if ( sValue.length() <= 33 )
				m_sValue = sValue;
			else
                m_sValue = sValue.substr( 0, 33 );
		}


        std::wstring FromString(std::wstring &sValue)
		{
            if ( sValue.length() <= 33 )
				m_sValue = sValue;
			else
                m_sValue = sValue.substr( 0, 33 );

			return m_sValue;
		}

        std::wstring ToString  () const
		{
			return m_sValue;
		}

        SimpleType_FromString2    (std::wstring)
		SimpleType_Operator_Equal (CMacroName)
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

	template<EMailMergeDataType eDefValue = mailmergeddatatypeTextFile>
	class CMailMergeDataType : public CSimpleType<EMailMergeDataType, eDefValue>
	{
	public:
		CMailMergeDataType() {}

        virtual EMailMergeDataType FromString(std::wstring &sValue)
		{
            if      ( (L"native")      == sValue ) m_eValue = mailmergeddatatypeNative;
            else if ( (L"odbc")        == sValue ) m_eValue = mailmergeddatatypeODBC;
            else if ( (L"query")       == sValue ) m_eValue = mailmergeddatatypeQuery;
            else if ( (L"soap")        == sValue ) m_eValue = mailmergeddatatypeSoap;
            else if ( (L"spreadsheet") == sValue ) m_eValue = mailmergeddatatypeSpreadsheet;
            else if ( (L"textFile")    == sValue ) m_eValue = mailmergeddatatypeTextFile;
            else if ( (L"xQuery")      == sValue ) m_eValue = mailmergeddatatypeXQuery;
            else if ( (L"xmlFile")     == sValue ) m_eValue = mailmergeddatatypeXmlFile;
            else                                   m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(m_eValue)
			{
			case mailmergeddatatypeNative      : return (L"native");
			case mailmergeddatatypeODBC        : return (L"odbc");
			case mailmergeddatatypeQuery       : return (L"query");
			case mailmergeddatatypeSoap        : return (L"soap");
			case mailmergeddatatypeSpreadsheet : return (L"spreadsheet");
			case mailmergeddatatypeTextFile    : return (L"textFile");
			case mailmergeddatatypeXQuery      : return (L"xQuery");
			case mailmergeddatatypeXmlFile     : return (L"xmlFile");
			default                            : return (L"textFile");
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

        virtual EMailMergeDest FromString(std::wstring &sValue)
		{
            if      ( (L"email")       == sValue ) m_eValue = mailmergeddestEmail;
            else if ( (L"fax")         == sValue ) m_eValue = mailmergeddestFax;
            else if ( (L"newDocument") == sValue ) m_eValue = mailmergeddestNewDocument;
            else if ( (L"printer")     == sValue ) m_eValue = mailmergeddestPrinter;
            else                                    m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(m_eValue)
			{
			case mailmergeddestEmail       : return (L"email");
			case mailmergeddestFax         : return (L"fax");
			case mailmergeddestNewDocument : return (L"newDocument");
			case mailmergeddestPrinter     : return (L"printer");
			default                        : return (L"newDocument");
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

        virtual EMailMergeDocType FromString(std::wstring &sValue)
		{
            if      ( (L"catalog")       == sValue ) m_eValue = mailmergeddoctypeCatalog;
            else if ( (L"email")         == sValue ) m_eValue = mailmergeddoctypeEmail;
            else if ( (L"envelopes")     == sValue ) m_eValue = mailmergeddoctypeEnvelopes;
            else if ( (L"fax")           == sValue ) m_eValue = mailmergeddoctypeFax;
            else if ( (L"formLetters")   == sValue ) m_eValue = mailmergeddoctypeFormLetters;
            else if ( (L"mailingLabels") == sValue ) m_eValue = mailmergeddoctypeMailingLabels;
            else                                      m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(m_eValue)
			{
			case mailmergeddoctypeCatalog       : return (L"catalog");
			case mailmergeddoctypeEmail         : return (L"email");
			case mailmergeddoctypeEnvelopes     : return (L"envelopes");
			case mailmergeddoctypeFax           : return (L"fax");
			case mailmergeddoctypeFormLetters   : return (L"formLetters");
			case mailmergeddoctypeMailingLabels : return (L"mailingLabels");
			default                             : return (L"email");
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

        virtual EMailMergeOdsoFMDFieldType FromString(std::wstring &sValue)
		{
            if      ( (L"dbColumn") == sValue ) m_eValue = mailmergeodsofmdfieldtypeDbColumn;
            else if ( (L"null")     == sValue ) m_eValue = mailmergeodsofmdfieldtypeNull;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring                    ToString  () const
		{
            switch(m_eValue)
			{
			case mailmergeodsofmdfieldtypeDbColumn : return (L"dbColumn");
			case mailmergeodsofmdfieldtypeNull     : return (L"null");
			default                                : return (L"null");
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

        virtual EMailMergeSourceType FromString(std::wstring &sValue)
		{
            if      ( (L"addressBook") == sValue ) m_eValue = mailmergesourcetypeAddressBook;
            else if ( (L"database")    == sValue ) m_eValue = mailmergesourcetypeDatabase;
            else if ( (L"document1")   == sValue ) m_eValue = mailmergesourcetypeDocument1;
            else if ( (L"document2")   == sValue ) m_eValue = mailmergesourcetypeDocument2;
            else if ( (L"email")       == sValue ) m_eValue = mailmergesourcetypeEmail;
            else if ( (L"legacy")      == sValue ) m_eValue = mailmergesourcetypeLegacy;
            else if ( (L"master")      == sValue ) m_eValue = mailmergesourcetypeMaster;
            else if ( (L"native")      == sValue ) m_eValue = mailmergesourcetypeNative;
            else if ( (L"text")        == sValue ) m_eValue = mailmergesourcetypeText;
            else                                    m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring              ToString  () const
		{
            switch(m_eValue)
			{
			case mailmergesourcetypeAddressBook : return (L"addressBook");
			case mailmergesourcetypeDatabase    : return (L"database");
			case mailmergesourcetypeDocument1   : return (L"document1");
			case mailmergesourcetypeDocument2   : return (L"document2");
			case mailmergesourcetypeEmail       : return (L"email");
			case mailmergesourcetypeLegacy      : return (L"legacy");
			case mailmergesourcetypeMaster      : return (L"master");
			case mailmergesourcetypeNative      : return (L"native");
			case mailmergesourcetypeText        : return (L"text");
			default                             : return (L"text");
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

        virtual EMerge  FromString(std::wstring &sValue)
		{
            if      ( (L"continue") == sValue ) m_eValue = mergeContinue;
            else if ( (L"restart")  == sValue ) m_eValue = mergeRestart;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case mergeContinue : return (L"continue");
			case mergeRestart  : return (L"restart");
			default            : return (L"restart");
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

        virtual EMultiLevelType FromString(std::wstring &sValue)
		{
            if      ( (L"hybridMultilevel") == sValue ) m_eValue = emultileveltypeHybridMultilevel;
            else if ( (L"multilevel")       == sValue ) m_eValue = emultileveltypeMultilevel;
            else if ( (L"singleLevel")      == sValue ) m_eValue = emultileveltypeSingleLevel;
            else                                         m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring         ToString  () const
		{
            switch(m_eValue)
			{
			case emultileveltypeHybridMultilevel : return (L"hybridMultilevel");
			case emultileveltypeMultilevel       : return (L"multilevel");
			case emultileveltypeSingleLevel      : return (L"singleLevel");
			default                              : return (L"singleLevel");
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

        virtual ENumberFormat FromString(std::wstring &sValue)
		{
            if      ( (L"aiueo")                        == sValue ) m_eValue = numberformatAiueo;
            else if ( (L"aiueoFullWidth")               == sValue ) m_eValue = numberformatAiueoFullWidth;
            else if ( (L"arabicAbjad")                  == sValue ) m_eValue = numberformatArabicAbjad;
            else if ( (L"arabicAlpha")                  == sValue ) m_eValue = numberformatArabicAlpha;
            else if ( (L"bahtText")                     == sValue ) m_eValue = numberformatBahtText;
            else if ( (L"bullet")                       == sValue ) m_eValue = numberformatBullet;
            else if ( (L"cardinalText")                 == sValue ) m_eValue = numberformatCardinalText;
            else if ( (L"chicago")                      == sValue ) m_eValue = numberformatChicago;
            else if ( (L"chineseCounting")              == sValue ) m_eValue = numberformatChineseCounting;
            else if ( (L"chineseCountingThousand")      == sValue ) m_eValue = numberformatChineseCountingThousand;
            else if ( (L"chineseLegalSimplified")       == sValue ) m_eValue = numberformatChineseLegalSimplified;
            else if ( (L"chosung")                      == sValue ) m_eValue = numberformatChosung;
            else if ( (L"custom")                       == sValue ) m_eValue = numberformatCustom;
            else if ( (L"decimal")                      == sValue ) m_eValue = numberformatDecimal;
            else if ( (L"decimalEnclosedCircle")        == sValue ) m_eValue = numberformatDecimalEnclosedCircle;
            else if ( (L"decimalEnclosedCircleChinese") == sValue ) m_eValue = numberformatDecimalEnclosedCircleChinese;
            else if ( (L"decimalEnclosedFullstop")      == sValue ) m_eValue = numberformatDecimalEnclosedFullstop;
            else if ( (L"decimalEnclosedParen")         == sValue ) m_eValue = numberformatDecimalEnclosedParen;
            else if ( (L"decimalFullWidth")             == sValue ) m_eValue = numberformatDecimalFullWidth;
            else if ( (L"decimalFullWidth2")            == sValue ) m_eValue = numberformatDecimalFullWidth2;
            else if ( (L"decimalHalfWidth")             == sValue ) m_eValue = numberformatDecimalHalfWidth;
            else if ( (L"decimalZero")                  == sValue ) m_eValue = numberformatDecimalZero;
            else if ( (L"dollarText")                   == sValue ) m_eValue = numberformatDollarText;
            else if ( (L"ganada")                       == sValue ) m_eValue = numberformatGanada;
            else if ( (L"hebrew1")                      == sValue ) m_eValue = numberformatHebrew1;
            else if ( (L"hebrew2")                      == sValue ) m_eValue = numberformatHebrew2;
            else if ( (L"hex")                          == sValue ) m_eValue = numberformatHex;
            else if ( (L"hindiConsonants")              == sValue ) m_eValue = numberformatHindiConsonants;
            else if ( (L"hindiCounting")                == sValue ) m_eValue = numberformatHindiCounting;
            else if ( (L"hindiNumbers")                 == sValue ) m_eValue = numberformatHindiNumbers;
            else if ( (L"hindiVowels")                  == sValue ) m_eValue = numberformatHindiVowels;
            else if ( (L"ideographDigital")             == sValue ) m_eValue = numberformatIdeographDigital;
            else if ( (L"ideographEnclosedCircle")      == sValue ) m_eValue = numberformatIdeographEnclosedCircle;
            else if ( (L"ideographLegalTraditional")    == sValue ) m_eValue = numberformatIdeographLegalTraditional;
            else if ( (L"ideographTraditional")         == sValue ) m_eValue = numberformatIdeographTraditional;
            else if ( (L"ideographZodiac")              == sValue ) m_eValue = numberformatIdeographZodiac;
            else if ( (L"ideographZodiacTraditional")   == sValue ) m_eValue = numberformatIdeographZodiacTraditional;
            else if ( (L"iroha")                        == sValue ) m_eValue = numberformatIroha;
            else if ( (L"irohaFullWidth")               == sValue ) m_eValue = numberformatIrohaFullWidth;
            else if ( (L"japaneseCounting")             == sValue ) m_eValue = numberformatJapaneseCounting;
            else if ( (L"japaneseDigitalTenThousand")   == sValue ) m_eValue = numberformatJapaneseDigitalTenThousand;
            else if ( (L"japaneseLegal")                == sValue ) m_eValue = numberformatJapaneseLegal;
            else if ( (L"koreanCounting")               == sValue ) m_eValue = numberformatKoreanCounting;
            else if ( (L"koreanDigital")                == sValue ) m_eValue = numberformatKoreanDigital;
            else if ( (L"koreanDigital2")               == sValue ) m_eValue = numberformatKoreanDigital2;
            else if ( (L"koreanLegal")                  == sValue ) m_eValue = numberformatKoreanLegal;
            else if ( (L"lowerLetter")                  == sValue ) m_eValue = numberformatLowerLetter;
            else if ( (L"lowerRoman")                   == sValue ) m_eValue = numberformatLowerRoman;
            else if ( (L"none")                         == sValue ) m_eValue = numberformatNone;
            else if ( (L"numberInDash")                 == sValue ) m_eValue = numberformatNumberInDash;
            else if ( (L"ordinal")                      == sValue ) m_eValue = numberformatOrdinal;
            else if ( (L"ordinalText")                  == sValue ) m_eValue = numberformatOrdinalText;
            else if ( (L"russianLower")                 == sValue ) m_eValue = numberformatRussianLower;
            else if ( (L"russianUpper")                 == sValue ) m_eValue = numberformatRussianUpper;
            else if ( (L"taiwaneseCounting")            == sValue ) m_eValue = numberformatTaiwaneseCounting;
            else if ( (L"taiwaneseCountingThousand")    == sValue ) m_eValue = numberformatTaiwaneseCountingThousand;
            else if ( (L"taiwaneseDigital")             == sValue ) m_eValue = numberformatTaiwaneseDigital;
            else if ( (L"thaiCounting")                 == sValue ) m_eValue = numberformatThaiCounting;
            else if ( (L"thaiLetters")                  == sValue ) m_eValue = numberformatThaiLetters;
            else if ( (L"thaiNumbers")                  == sValue ) m_eValue = numberformatThaiNumbers;
            else if ( (L"upperLetter")                  == sValue ) m_eValue = numberformatUpperLetter;
            else if ( (L"upperRoman")                   == sValue ) m_eValue = numberformatUpperRoman;
            else if ( (L"vietnameseCounting")           == sValue ) m_eValue = numberformatVietnameseCounting;
            else                                                     m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(m_eValue)
			{
			case numberformatAiueo                        : return (L"aiueo");
			case numberformatAiueoFullWidth               : return (L"aiueoFullWidth");
			case numberformatArabicAbjad                  : return (L"arabicAbjad");
			case numberformatArabicAlpha                  : return (L"arabicAlpha");
			case numberformatBahtText                     : return (L"bahtText");
			case numberformatBullet                       : return (L"bullet");
			case numberformatCardinalText                 : return (L"cardinalText");
			case numberformatChicago                      : return (L"chicago");
			case numberformatChineseCounting              : return (L"chineseCounting");
			case numberformatChineseCountingThousand      : return (L"chineseCountingThousand");
			case numberformatChineseLegalSimplified       : return (L"chineseLegalSimplified");
			case numberformatChosung                      : return (L"chosung");
			case numberformatCustom                       : return (L"custom");
			case numberformatDecimal                      : return (L"decimal");
			case numberformatDecimalEnclosedCircle        : return (L"decimalEnclosedCircle");
			case numberformatDecimalEnclosedCircleChinese : return (L"decimalEnclosedCircleChinese");
			case numberformatDecimalEnclosedFullstop      : return (L"decimalEnclosedFullstop");
			case numberformatDecimalEnclosedParen         : return (L"decimalEnclosedParen");
			case numberformatDecimalFullWidth             : return (L"decimalFullWidth");
			case numberformatDecimalFullWidth2            : return (L"decimalFullWidth2");
			case numberformatDecimalHalfWidth             : return (L"decimalHalfWidth");
			case numberformatDecimalZero                  : return (L"decimalZero");
			case numberformatDollarText                   : return (L"dollarText");
			case numberformatGanada                       : return (L"ganada");
			case numberformatHebrew1                      : return (L"hebrew1");
			case numberformatHebrew2                      : return (L"hebrew2");
			case numberformatHex                          : return (L"hex");
			case numberformatHindiConsonants              : return (L"hindiConsonants");
			case numberformatHindiCounting                : return (L"hindiCounting");
			case numberformatHindiNumbers                 : return (L"hindiNumbers");
			case numberformatHindiVowels                  : return (L"hindiVowels");
			case numberformatIdeographDigital             : return (L"ideographDigital");
			case numberformatIdeographEnclosedCircle      : return (L"ideographEnclosedCircle");
			case numberformatIdeographLegalTraditional    : return (L"ideographLegalTraditional");
			case numberformatIdeographTraditional         : return (L"ideographTraditional");
			case numberformatIdeographZodiac              : return (L"ideographZodiac");
			case numberformatIdeographZodiacTraditional   : return (L"ideographZodiacTraditional");
			case numberformatIroha                        : return (L"iroha");
			case numberformatIrohaFullWidth               : return (L"irohaFullWidth");
			case numberformatJapaneseCounting             : return (L"japaneseCounting");
			case numberformatJapaneseDigitalTenThousand   : return (L"japaneseDigitalTenThousand");
			case numberformatJapaneseLegal                : return (L"japaneseLegal");
			case numberformatKoreanCounting               : return (L"koreanCounting");
			case numberformatKoreanDigital                : return (L"koreanDigital");
			case numberformatKoreanDigital2               : return (L"koreanDigital2");
			case numberformatKoreanLegal                  : return (L"koreanLegal");
			case numberformatLowerLetter                  : return (L"lowerLetter");
			case numberformatLowerRoman                   : return (L"lowerRoman");
			case numberformatNone                         : return (L"none");
			case numberformatNumberInDash                 : return (L"numberInDash");
			case numberformatOrdinal                      : return (L"ordinal");
			case numberformatOrdinalText                  : return (L"ordinalText");
			case numberformatRussianLower                 : return (L"russianLower");
			case numberformatRussianUpper                 : return (L"russianUpper");
			case numberformatTaiwaneseCounting            : return (L"taiwaneseCounting");
			case numberformatTaiwaneseCountingThousand    : return (L"taiwaneseCountingThousand");
			case numberformatTaiwaneseDigital             : return (L"taiwaneseDigital");
			case numberformatThaiCounting                 : return (L"thaiCounting");
			case numberformatThaiLetters                  : return (L"thaiLetters");
			case numberformatThaiNumbers                  : return (L"thaiNumbers");
			case numberformatUpperLetter                  : return (L"upperLetter");
			case numberformatUpperRoman                   : return (L"upperRoman");
			case numberformatVietnameseCounting           : return (L"vietnameseCounting");
			default                                       : return (L"none");
			}
		}

		SimpleType_FromString     (ENumberFormat)
		SimpleType_Operator_Equal (CNumberFormat)
		ENumberFormat GetValue() const
		{
            return m_eValue;
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

        virtual EObjectDrawAspect FromString(std::wstring &sValue)
		{
            if      ( (L"content") == sValue ) m_eValue = objectdrawaspectContent;
            else if ( (L"icon")    == sValue ) m_eValue = objectdrawaspectIcon;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(m_eValue)
			{
			case objectdrawaspectContent : return (L"content");
			case objectdrawaspectIcon    : return (L"icon");
			default                      : return (L"content");
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

        virtual EObjectUpdateMode FromString(std::wstring &sValue)
		{
            if      ( (L"always") == sValue ) m_eValue = objectupdatemodeAlways;
            else if ( (L"onCall") == sValue ) m_eValue = objectupdatemodeOnCall;
            else                               m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(m_eValue)
			{
			case objectupdatemodeAlways : return (L"always");
			case objectupdatemodeOnCall : return (L"onCall");
			default                     : return (L"always");
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

        virtual EPageBorderDisplay FromString(std::wstring &sValue)
		{
            if      ( (L"allPages")     == sValue ) m_eValue = pageborderdisplayAllPages;
            else if ( (L"firstPage")    == sValue ) m_eValue = pageborderdisplayFirstPage;
            else if ( (L"notFirstPage") == sValue ) m_eValue = pageborderdisplayNotFirstPage;
            else                                     m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(m_eValue)
			{
			case pageborderdisplayAllPages     : return (L"allPages");
			case pageborderdisplayFirstPage    : return (L"firstPage");
			case pageborderdisplayNotFirstPage : return (L"notFirstPage");
			default                            : return (L"allPages");
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

        virtual EPageBorderOffset FromString(std::wstring &sValue)
		{
            if      ( (L"page") == sValue ) m_eValue = pageborderoffsetPage;
            else if ( (L"text") == sValue ) m_eValue = pageborderoffsetText;
            else                             m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(m_eValue)
			{
			case pageborderoffsetPage : return (L"page");
			case pageborderoffsetText : return (L"text");
			default                   : return (L"page");
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

        virtual EPageBorderZOrder FromString(std::wstring &sValue)
		{
            if      ( (L"back")  == sValue ) m_eValue = pageborderzorderBack;
            else if ( (L"front") == sValue ) m_eValue = pageborderzorderFront;
            else                             m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(m_eValue)
			{
			case pageborderzorderBack  : return (L"back");
			case pageborderzorderFront : return (L"front");
			default                    : return (L"back");
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

        virtual EPageOrientation FromString(std::wstring &sValue)
		{
            if      ( (L"landscape") == sValue ) m_eValue = pageorientLandscape;
            else if ( (L"portrait")  == sValue ) m_eValue = pageorientPortrait;
            else                                  m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring          ToString  () const
		{
            switch(m_eValue)
			{
			case pageorientLandscape : return (L"landscape");
			case pageorientPortrait  : return (L"portrait");
			default                  : return (L"portrait");
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

        virtual EPitch  FromString(std::wstring &sValue)
		{
            if      ( (L"default")  == sValue ) m_eValue = pitchDefault;
            else if ( (L"fixed")    == sValue ) m_eValue = pitchFixed;
            else if ( (L"variable") == sValue ) m_eValue = pitchVariable;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case pitchDefault  : return (L"default");
			case pitchFixed    : return (L"fixed");
			case pitchVariable : return (L"variable");
			default            : return (L"default");
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

        virtual int     FromString(std::wstring &sValue)
		{
            m_eValue = _wtoi( sValue.c_str() );

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            std::wstring sResult = std::to_wstring( m_eValue);

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

        virtual int     FromString(std::wstring &sValue)
		{
			m_eValue = _wtoi( sValue.c_str() );

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            std::wstring sResult = std::to_wstring( m_eValue);

			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CPointMeasure)
		double ToPoints() const
		{
            return (double)m_eValue;
		}
		double ToMM() const
		{
            return (double)Pt_To_Mm(m_eValue);
		}
		double ToInches() const
		{
            return m_eValue / 72.0;
		}

		void FromPoints(double dValue)
		{
            m_eValue = dValue;
		}

		void FromInches(double dValue)
		{
            m_eValue = dValue * 72.0;
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

        virtual EProof  FromString(std::wstring &sValue)
		{
            if      ( (L"clean") == sValue ) m_eValue = proofClean;
            else if ( (L"dirty") == sValue ) m_eValue = proofDirty;
            else                              m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case proofClean : return (L"clean");
			case proofDirty : return (L"dirty");
			default         : return (L"dirty");
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

        virtual EProofErr FromString(std::wstring &sValue)
		{
            if      ( (L"gramEnd")    == sValue ) m_eValue = prooferrGramEnd;
            else if ( (L"gramStart")  == sValue ) m_eValue = prooferrGramStart;
            else if ( (L"spellEnd")   == sValue ) m_eValue = prooferrSpellEnd;
            else if ( (L"spellStart") == sValue ) m_eValue = prooferrSpellStart;
            else                                   m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring   ToString  () const
		{
            switch(m_eValue)
			{
			case prooferrGramEnd    : return (L"gramEnd");
			case prooferrGramStart  : return (L"gramStart");
			case prooferrSpellEnd   : return (L"spellEnd");
			case prooferrSpellStart : return (L"spellStart");
			default                 : return (L"spellStart");
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

        virtual EPTabAlignment FromString(std::wstring &sValue)
		{
            if      ( (L"center") == sValue ) m_eValue = ptabalignmentCenter;
            else if ( (L"left")   == sValue ) m_eValue = ptabalignmentLeft;
            else if ( (L"right")  == sValue ) m_eValue = ptabalignmentRight;
            else                               m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(m_eValue)
			{
			case ptabalignmentCenter : return (L"center");
			case ptabalignmentLeft   : return (L"left");
			case ptabalignmentRight  : return (L"right");
			default                  : return (L"left");
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

        virtual EPTabLeader FromString(std::wstring &sValue)
		{
            if      ( (L"dot")        == sValue ) m_eValue = ptableaderDot;
            else if ( (L"hyphen")     == sValue ) m_eValue = ptableaderHyphen;
            else if ( (L"middleDot")  == sValue ) m_eValue = ptableaderMiddleDot;
            else if ( (L"none")       == sValue ) m_eValue = ptableaderNone;
            else if ( (L"underscore") == sValue ) m_eValue = ptableaderUnderscore;
            else                                   m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case ptableaderDot        : return (L"dot");
			case ptableaderHyphen     : return (L"hyphen");
			case ptableaderMiddleDot  : return (L"middleDot");
			case ptableaderNone       : return (L"none");
			case ptableaderUnderscore : return (L"underscore");
			default                   : return (L"none");
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

        virtual EPTabRelativeTo FromString(std::wstring &sValue)
		{
            if      ( (L"indent") == sValue ) m_eValue = ptabrelativetoIndent;
            else if ( (L"margin") == sValue ) m_eValue = ptabrelativetoMargin;
            else                               m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring         ToString  () const
		{
            switch(m_eValue)
			{
			case ptabrelativetoIndent : return (L"indent");
			case ptabrelativetoMargin : return (L"margin");
			default                   : return (L"indent");
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
		restartnumberEachSect   = 1,
		restartnumberEachPage   = 2
	};

	template<ERestartNumber eDefValue = restartnumberContinious>
	class CRestartNumber : public CSimpleType<ERestartNumber, eDefValue>
	{
	public:
		CRestartNumber() {}

        virtual ERestartNumber FromString(std::wstring &sValue)
		{
            if      ( (L"continuous") == sValue ) m_eValue = restartnumberContinious;
            else if ( (L"eachPage")   == sValue ) m_eValue = restartnumberEachPage;
            else if ( (L"eachSect")   == sValue ) m_eValue = restartnumberEachSect;
            else                                   m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(m_eValue)
			{
			case restartnumberContinious : return (L"continuous");
			case restartnumberEachPage   : return (L"eachPage");
			case restartnumberEachSect   : return (L"eachSect");
			default                      : return (L"continuous");
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

        virtual ERubyAlign FromString(std::wstring &sValue)
		{
            if      ( (L"center")           == sValue ) m_eValue = erubyalignCenter;
            else if ( (L"distributeLetter") == sValue ) m_eValue = erubyalignDistributeLetter;
            else if ( (L"distributeSpace")  == sValue ) m_eValue = erubyalignDistributeSpace;
            else if ( (L"left")             == sValue ) m_eValue = erubyalignLeft;
            else if ( (L"right")            == sValue ) m_eValue = erubyalignRight;
            else if ( (L"rightVertical")    == sValue ) m_eValue = erubyalignRightVertical;
            else                                         m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring    ToString  () const
		{
            switch(m_eValue)
			{
			case erubyalignCenter           : return (L"center");
			case erubyalignDistributeLetter : return (L"distributeLetter");
			case erubyalignDistributeSpace  : return (L"distributeSpace");
			case erubyalignLeft             : return (L"left");
			case erubyalignRight            : return (L"right");
			case erubyalignRightVertical    : return (L"rightVertical");
			default                         : return (L"left");
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

        virtual ESdtDateMappingType FromString(std::wstring &sValue)
		{
            if      ( (L"date")     == sValue ) m_eValue = sdtdatemappingtypeDate;
            else if ( (L"dateTime") == sValue ) m_eValue = sdtdatemappingtypeDateTime;
            else if ( (L"text")     == sValue ) m_eValue = sdtdatemappingtypeText;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring             ToString  () const
		{
            switch(m_eValue)
			{
			case sdtdatemappingtypeDate     : return (L"date");
			case sdtdatemappingtypeDateTime : return (L"dateTime");
			case sdtdatemappingtypeText     : return (L"text");
			default                         : return (L"text");
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

        virtual ESectionMark FromString(std::wstring &sValue)
		{
            if      ( (L"continuous") == sValue ) m_eValue = sectionmarkContinious;
            else if ( (L"evenPage")   == sValue ) m_eValue = sectionmarkEvenPage;
            else if ( (L"nextColumn") == sValue ) m_eValue = sectionmarkNextColumn;
            else if ( (L"nextPage")   == sValue ) m_eValue = sectionmarkNextPage;
            else if ( (L"oddPage")    == sValue ) m_eValue = sectionmarkOddPage;
            else                                   m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring      ToString  () const
		{
            switch(m_eValue)
			{
			case sectionmarkContinious : return (L"continuous");
			case sectionmarkEvenPage   : return (L"evenPage");
			case sectionmarkNextColumn : return (L"nextColumn");
			case sectionmarkNextPage   : return (L"nextPage");
			case sectionmarkOddPage    : return (L"oddPage");
			default                    : return (L"nextPage");
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

        virtual EShd    FromString(std::wstring &sValue)
		{
            if      ( (L"clear")                 == sValue ) m_eValue = shdClear;
            else if ( (L"diagCross")             == sValue ) m_eValue = shdDiagCross;
            else if ( (L"diagStripe")            == sValue ) m_eValue = shdDiagStripe;
            else if ( (L"horzCross")             == sValue ) m_eValue = shdHorzCross;
            else if ( (L"horzStripe")            == sValue ) m_eValue = shdHorzStripe;
            else if ( (L"nil")                   == sValue ) m_eValue = shdNil;
            else if ( (L"pct10")                 == sValue ) m_eValue = shdPct10;
            else if ( (L"pct12")                 == sValue ) m_eValue = shdPct12;
            else if ( (L"pct15")                 == sValue ) m_eValue = shdPct15;
            else if ( (L"pct20")                 == sValue ) m_eValue = shdPct20;
            else if ( (L"pct25")                 == sValue ) m_eValue = shdPct25;
            else if ( (L"pct30")                 == sValue ) m_eValue = shdPct30;
            else if ( (L"pct35")                 == sValue ) m_eValue = shdPct35;
            else if ( (L"pct37")                 == sValue ) m_eValue = shdPct37;
            else if ( (L"pct40")                 == sValue ) m_eValue = shdPct40;
            else if ( (L"pct45")                 == sValue ) m_eValue = shdPct45;
            else if ( (L"pct5")                  == sValue ) m_eValue = shdPct5;
            else if ( (L"pct50")                 == sValue ) m_eValue = shdPct50;
            else if ( (L"pct55")                 == sValue ) m_eValue = shdPct55;
            else if ( (L"pct60")                 == sValue ) m_eValue = shdPct60;
            else if ( (L"pct62")                 == sValue ) m_eValue = shdPct62;
            else if ( (L"pct65")                 == sValue ) m_eValue = shdPct65;
            else if ( (L"pct70")                 == sValue ) m_eValue = shdPct70;
            else if ( (L"pct75")                 == sValue ) m_eValue = shdPct75;
            else if ( (L"pct80")                 == sValue ) m_eValue = shdPct80;
            else if ( (L"pct85")                 == sValue ) m_eValue = shdPct85;
            else if ( (L"pct87")                 == sValue ) m_eValue = shdPct87;
            else if ( (L"pct90")                 == sValue ) m_eValue = shdPct90;
            else if ( (L"pct95")                 == sValue ) m_eValue = shdPct95;
            else if ( (L"reverseDiagStripe")     == sValue ) m_eValue = shdReverseDiagStripe;
            else if ( (L"solid")                 == sValue ) m_eValue = shdSolid;
            else if ( (L"thinDiagCross")         == sValue ) m_eValue = shdThinDiagCross;
            else if ( (L"thinDiagStripe")        == sValue ) m_eValue = shdThinDiagStripe;
            else if ( (L"thinHorzCross")         == sValue ) m_eValue = shdThinHorzCross;
            else if ( (L"thinHorzStripe")        == sValue ) m_eValue = shdThinHorzStripe;
            else if ( (L"thinReverseDiagStripe") == sValue ) m_eValue = shdThinReverseDiagStripe;
            else if ( (L"thinVertStripe")        == sValue ) m_eValue = shdThinVertStripe;
            else if ( (L"vertStripe")            == sValue ) m_eValue = shdVertStripe;
            else                                              m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case shdClear                 : return (L"clear");
			case shdDiagCross             : return (L"diagCross");
			case shdDiagStripe            : return (L"diagStripe");
			case shdHorzCross             : return (L"horzCross");
			case shdHorzStripe            : return (L"horzStripe");
			case shdNil                   : return (L"nil");
			case shdPct10                 : return (L"pct10");
			case shdPct12                 : return (L"pct12");
			case shdPct15                 : return (L"pct15");
			case shdPct20                 : return (L"pct20");
			case shdPct25                 : return (L"pct25");
			case shdPct30                 : return (L"pct30");
			case shdPct35                 : return (L"pct35");
			case shdPct37                 : return (L"pct37");
			case shdPct40                 : return (L"pct40");
			case shdPct45                 : return (L"pct45");
			case shdPct5                  : return (L"pct5");
			case shdPct50                 : return (L"pct50");
			case shdPct55                 : return (L"pct55");
			case shdPct60                 : return (L"pct60");
			case shdPct62                 : return (L"pct62");
			case shdPct65                 : return (L"pct65");
			case shdPct70                 : return (L"pct70");
			case shdPct75                 : return (L"pct75");
			case shdPct80                 : return (L"pct80");
			case shdPct85                 : return (L"pct85");
			case shdPct87                 : return (L"pct87");
			case shdPct90                 : return (L"pct90");
			case shdPct95                 : return (L"pct95");
			case shdReverseDiagStripe     : return (L"reverseDiagStripe");
			case shdSolid                 : return (L"solid");
			case shdThinDiagCross         : return (L"thinDiagCross");
			case shdThinDiagStripe        : return (L"thinDiagStripe");
			case shdThinHorzCross         : return (L"thinHorzCross");
			case shdThinHorzStripe        : return (L"thinHorzStripe");
			case shdThinReverseDiagStripe : return (L"thinReverseDiagStripe");
			case shdThinVertStripe        : return (L"thinVertStripe");
			case shdVertStripe            : return (L"vertStripe");
			default                       : return (L"solid");
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

        virtual int     FromString(std::wstring &sValue)
		{
			if ( !Parse(sValue) )
			{
                m_eValue = nDefValue;
			}

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
			return XmlUtils::IntToString( m_eValue, L"%04X"); 
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CShortHexNumber)
	private:

        bool Parse(std::wstring &sValue)
		{
            int nLength = sValue.length();

			bool bResult = true;
			if(nLength > 0)
			{
                m_eValue  = HexToInt( (int)sValue[nLength - 1], bResult );
				if(nLength > 1)
				{
                    m_eValue += HexToInt( (int)sValue[nLength - 2], bResult ) <<  4;
					if(nLength > 2)
					{
                        m_eValue += HexToInt( (int)sValue[nLength - 3], bResult ) <<  8;
						if(nLength > 3)
						{
                            m_eValue += HexToInt( (int)sValue[nLength - 4], bResult ) << 12;
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

        virtual double  FromString(std::wstring &sValue)
		{
			Parse(sValue, 2); 

			return m_dValue;
		}

        virtual std::wstring ToString  () const
		{
			return std::to_wstring(m_dValue) + L"pt";
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

        virtual double  FromString(std::wstring &sValue)
		{
			Parse(sValue, 20); 

			return m_dValue;
		}

        virtual std::wstring ToString  () const
		{
            std::wstring sResult;

			if ( m_bUnit )
				sResult = std::to_wstring( m_dValue ) + L"pt";
			else
                sResult = std::to_wstring( (int)(m_dValue * 20) );

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

        virtual EStyleSort FromString(std::wstring &sValue)
		{
			m_bNames = false;
            if      ( (L"basedOn")  == sValue || (L"0004") == sValue ) m_eValue = stylesortBasedOn;
            else if ( (L"default")  == sValue || (L"0002") == sValue ) m_eValue = stylesortDefault;
            else if ( (L"font")     == sValue || (L"0003") == sValue ) m_eValue = stylesortFont;
            else if ( (L"name")     == sValue || (L"0000") == sValue ) m_eValue = stylesortName;
            else if ( (L"priority") == sValue || (L"0001") == sValue ) m_eValue = stylesortPriority;
            else if ( (L"type")     == sValue || (L"0005") == sValue ) m_eValue = stylesortType;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring    ToString  () const
		{
			if ( m_bNames )
			{
                switch(m_eValue)
				{
				case stylesortBasedOn  : return (L"basedOn");
				case stylesortDefault  : return (L"default");
				case stylesortFont     : return (L"font");
				case stylesortName     : return (L"name");
				case stylesortPriority : return (L"priority");
				case stylesortType     : return (L"type");
				default                : return (L"default");
				}
			}
			else
			{
                switch(m_eValue)
				{
				case stylesortBasedOn  : return (L"0004");
				case stylesortDefault  : return (L"0002");
				case stylesortFont     : return (L"0003");
				case stylesortName     : return (L"0000");
				case stylesortPriority : return (L"0001");
				case stylesortType     : return (L"0005");
				default                : return (L"0000");
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

        virtual EStyleType FromString(std::wstring &sValue)
		{
            if      ( (L"character") == sValue ) m_eValue = styletypeCharacter;
            else if ( (L"numbering") == sValue ) m_eValue = styletypeNumbering;
            else if ( (L"paragraph") == sValue ) m_eValue = styletypeParagraph;
            else if ( (L"table")     == sValue ) m_eValue = styletypeTable;
            else                                  m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring    ToString  () const
		{
            switch(m_eValue)
			{
			case styletypeCharacter : return (L"character");
			case styletypeNumbering : return (L"numbering");
			case styletypeParagraph : return (L"paragraph");
			case styletypeTable     : return (L"table");
			default                 : return (L"paragraph");
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

        virtual ETabJc  FromString(std::wstring &sValue)
		{
            if      ( (L"bar")     == sValue ) m_eValue = tabjcBar;
            else if ( (L"center")  == sValue ) m_eValue = tabjcCenter;
            else if ( (L"clear")   == sValue ) m_eValue = tabjcClear;
            else if ( (L"decimal") == sValue ) m_eValue = tabjcDecimal;
            else if ( (L"end")     == sValue ) m_eValue = tabjcEnd;
            else if ( (L"num")     == sValue ) m_eValue = tabjcNum;
            else if ( (L"start")   == sValue ) m_eValue = tabjcStart;
            else if ( (L"right")   == sValue ) m_eValue = tabjcRight;
            else if ( (L"left")    == sValue ) m_eValue = tabjcLeft;
            else                                m_eValue = tabjcClear;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
			switch(m_eValue)
			{
			case tabjcBar     : return (L"bar");
			case tabjcCenter  : return (L"center");
			case tabjcClear   : return (L"clear");
			case tabjcDecimal : return (L"decimal");
			case tabjcEnd     : return (L"end");
			case tabjcNum     : return (L"num");
			case tabjcStart   : return (L"start");
			case tabjcRight   : return (L"right");
			case tabjcLeft    : return (L"left");
			default           : return (L"clear");
			}
		}

		SimpleType_FromString     (ETabJc)
		SimpleType_Operator_Equal (CTabJc)

		ETabJc GetValue() const
		{
            return m_eValue;
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

        virtual ETabTlc FromString(std::wstring &sValue)
		{
            if      ( (L"dot")        == sValue ) m_eValue = tabtlcDot;
            else if ( (L"heavy")      == sValue ) m_eValue = tabtlcHeavy;
            else if ( (L"hyphen")     == sValue ) m_eValue = tabtlcHyphen;
            else if ( (L"middleDot")  == sValue ) m_eValue = tabtlcMiddleDot;
            else if ( (L"none")       == sValue ) m_eValue = tabtlcNone;
            else if ( (L"underscore") == sValue ) m_eValue = tabtlcUnderscore;
            else                                   m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case tabtlcDot        : return (L"dot");
			case tabtlcHeavy      : return (L"heavy");
			case tabtlcHyphen     : return (L"hyphen");
			case tabtlcMiddleDot  : return (L"middleDot");
			case tabtlcNone       : return (L"none");
			case tabtlcUnderscore : return (L"underscore");
			default                : return (L"none");
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

        virtual ETargetScreenSz FromString(std::wstring &sValue)
		{
            if      ( (L"1024x768")  == sValue ) m_eValue = targetscreensz1024x768;
            else if ( (L"1152x882")  == sValue ) m_eValue = targetscreensz1152x882;
            else if ( (L"1152x900")  == sValue ) m_eValue = targetscreensz1152x900;
            else if ( (L"1280x1024") == sValue ) m_eValue = targetscreensz1280x1024;
            else if ( (L"1600x1200") == sValue ) m_eValue = targetscreensz1600x1200;
            else if ( (L"1800x1440") == sValue ) m_eValue = targetscreensz1800x1440;
            else if ( (L"1920x1200") == sValue ) m_eValue = targetscreensz1920x1200;
            else if ( (L"544x376")   == sValue ) m_eValue = targetscreensz544x376;
            else if ( (L"640x480")   == sValue ) m_eValue = targetscreensz640x480;
            else if ( (L"720x512")   == sValue ) m_eValue = targetscreensz720x512;
            else if ( (L"800x600")   == sValue ) m_eValue = targetscreensz800x600;
            else                                  m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring         ToString  () const
		{
            switch(m_eValue)
			{
			case targetscreensz1024x768  : return (L"1024x768");
			case targetscreensz1152x882  : return (L"1152x882");
			case targetscreensz1152x900  : return (L"1152x900");
			case targetscreensz1280x1024 : return (L"1280x1024");
			case targetscreensz1600x1200 : return (L"1600x1200");
			case targetscreensz1800x1440 : return (L"1800x1440");
			case targetscreensz1920x1200 : return (L"1920x1200");
			case targetscreensz544x376   : return (L"544x376");
			case targetscreensz640x480   : return (L"640x480");
			case targetscreensz720x512   : return (L"720x512");
			case targetscreensz800x600   : return (L"800x600");
			default                      : return (L"1280x1024");
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

        virtual ETblLayoutType FromString(std::wstring &sValue)
		{
            if      ( (L"autofit") == sValue ) m_eValue = tbllayouttypeAutofit;
            else if ( (L"fixed")   == sValue ) m_eValue = tbllayouttypeFixed;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(m_eValue)
			{
			case tbllayouttypeAutofit : return (L"autofit");
			case tbllayouttypeFixed   : return (L"fixed");
			default                   : return (L"autofit");
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

        virtual ETblOverlap FromString(std::wstring &sValue)
		{
            if      ( (L"never")   == sValue ) m_eValue = tbloverlapNever;
            else if ( (L"overlap") == sValue ) m_eValue = tbloverlapOverlap;
            else                                m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case tbloverlapNever   : return (L"never");
			case tbloverlapOverlap : return (L"overlap");
			default                : return (L"overlap");
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

        virtual ETblStyleOverrideType FromString(std::wstring &sValue)
		{
            if      ( (L"band1Horz")  == sValue ) m_eValue = tblstyleoverridetypeBand1Horz;
            else if ( (L"band1Vert")  == sValue ) m_eValue = tblstyleoverridetypeBand1Vert;
            else if ( (L"band2Horz")  == sValue ) m_eValue = tblstyleoverridetypeBand2Horz;
            else if ( (L"band2Vert")  == sValue ) m_eValue = tblstyleoverridetypeBand2Vert;
            else if ( (L"firstCol")   == sValue ) m_eValue = tblstyleoverridetypeFirstCol;
            else if ( (L"firstRow")   == sValue ) m_eValue = tblstyleoverridetypeFirstRow;
            else if ( (L"lastCol")    == sValue ) m_eValue = tblstyleoverridetypeLastCol;
            else if ( (L"lastRow")    == sValue ) m_eValue = tblstyleoverridetypeLastRow;
            else if ( (L"neCell")     == sValue ) m_eValue = tblstyleoverridetypeNeCell;
            else if ( (L"nwCell")     == sValue ) m_eValue = tblstyleoverridetypeNwCell;
            else if ( (L"seCell")     == sValue ) m_eValue = tblstyleoverridetypeSeCell;
            else if ( (L"swCell")     == sValue ) m_eValue = tblstyleoverridetypeSwCell;
            else if ( (L"wholeTable") == sValue ) m_eValue = tblstyleoverridetypeWholeTable;
            else                                   m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring               ToString  () const
		{
            switch(m_eValue)
			{
			case tblstyleoverridetypeBand1Horz  : return (L"band1Horz");
			case tblstyleoverridetypeBand1Vert  : return (L"band1Vert");
			case tblstyleoverridetypeBand2Horz  : return (L"band2Horz");
			case tblstyleoverridetypeBand2Vert  : return (L"band2Vert");
			case tblstyleoverridetypeFirstCol   : return (L"firstCol");
			case tblstyleoverridetypeFirstRow   : return (L"firstRow");
			case tblstyleoverridetypeLastCol    : return (L"lastCol");
			case tblstyleoverridetypeLastRow    : return (L"lastRow");
			case tblstyleoverridetypeNeCell     : return (L"neCell");
			case tblstyleoverridetypeNwCell     : return (L"nwCell");
			case tblstyleoverridetypeSeCell     : return (L"seCell");
			case tblstyleoverridetypeSwCell     : return (L"swCell");
			case tblstyleoverridetypeWholeTable : return (L"wholeTable");
			default                             : return (L"wholeTable");
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

        virtual ETblWidth FromString(std::wstring &sValue)
		{
            if      ( (L"auto") == sValue ) m_eValue = tblwidthAuto;
            else if ( (L"dxa")  == sValue ) m_eValue = tblwidthDxa;
            else if ( (L"nil")  == sValue ) m_eValue = tblwidthNil;
            else if ( (L"pct")  == sValue ) m_eValue = tblwidthPct;
            else                             m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring   ToString  () const
		{
            switch(m_eValue)
			{
			case tblwidthAuto : return (L"auto");
			case tblwidthDxa  : return (L"dxa");
			case tblwidthNil  : return (L"nil");
			case tblwidthPct  : return (L"pct");
			default           : return (L"auto");
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

        virtual ETextAlignment FromString(std::wstring &sValue)
		{
            if      ( (L"auto")     == sValue ) m_eValue = textalignAuto;
            else if ( (L"baseline") == sValue ) m_eValue = textalignBaseLine;
            else if ( (L"bottom")   == sValue ) m_eValue = textalignBottom;
            else if ( (L"center")   == sValue ) m_eValue = textalignCenter;
            else if ( (L"top")      == sValue ) m_eValue = textalignTop;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(m_eValue)
			{
			case textalignAuto     : return (L"auto");
			case textalignBaseLine : return (L"baseline");
			case textalignBottom   : return (L"bottom");
			case textalignCenter   : return (L"center");
			case textalignTop      : return (L"top");
			default                : return (L"auto");
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

        virtual ETextboxTightWrap FromString(std::wstring &sValue)
		{
            if      ( (L"allLines")         == sValue ) m_eValue = textboxtightwrapAllLines;
            else if ( (L"firstAndLastLine") == sValue ) m_eValue = textboxtightwrapFirstAndLastLine;
            else if ( (L"firstLineOnly")    == sValue ) m_eValue = textboxtightwrapFirstLineOnly;
            else if ( (L"lastLineOnly")     == sValue ) m_eValue = textboxtightwrapLastLineOnly;
            else if ( (L"none")             == sValue ) m_eValue = textboxtightwrapNone;
            else                                         m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(m_eValue)
			{
			case textboxtightwrapAllLines         : return (L"allLines");
			case textboxtightwrapFirstAndLastLine : return (L"firstAndLastLine");
			case textboxtightwrapFirstLineOnly    : return (L"firstLineOnly");
			case textboxtightwrapLastLineOnly     : return (L"lastLineOnly");
			case textboxtightwrapNone             : return (L"none");
			default                               : return (L"none");
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
		textdirectionTb   = 0,
		textdirectionRl   = 1,
		textdirectionLr   = 2,
		textdirectionTbV  = 3,
		textdirectionRlV  = 4,
		textdirectionLrV  = 5
	};

	template<ETextDirection eDefValue = textdirectionTb>
	class CTextDirection : public CSimpleType<ETextDirection, eDefValue>
	{
	public:
		CTextDirection() {}

        virtual ETextDirection FromString(std::wstring &sValue)
		{
            if      ( (L"lr")  == sValue || (L"btLr")  == sValue ) m_eValue = textdirectionLr;
            else if ( (L"lrV") == sValue || (L"tbLrV") == sValue ) m_eValue = textdirectionLrV;
            else if ( (L"rl")  == sValue || (L"tbRl")  == sValue ) m_eValue = textdirectionRl;
            else if ( (L"rlV") == sValue || (L"tbRlV") == sValue ) m_eValue = textdirectionRlV;
            else if ( (L"tb")  == sValue || (L"lrTb")  == sValue ) m_eValue = textdirectionTb;
            else if ( (L"tbV") == sValue || (L"lrTbV") == sValue ) m_eValue = textdirectionTbV;
            else                                                     m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(m_eValue)
			{
			case textdirectionLr  : return (L"btLr");
			case textdirectionLrV : return (L"tbLrV");
			case textdirectionRl  : return (L"tbRl");
			case textdirectionRlV : return (L"tbRlV");
			case textdirectionTb  : return (L"lrTb");
			case textdirectionTbV : return (L"lrTbV");
			default               : return (L"lrTb");
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

        virtual ETextEffect FromString(std::wstring &sValue)
		{
            if      ( (L"antsBlack")       == sValue ) m_eValue = texteffectAntsBlack;
            else if ( (L"antsRed")         == sValue ) m_eValue = texteffectAntsRed;
            else if ( (L"blinkBackground") == sValue ) m_eValue = texteffectBlinkBackground;
            else if ( (L"lights")          == sValue ) m_eValue = texteffectLights;
            else if ( (L"none")            == sValue ) m_eValue = texteffectNone;
            else if ( (L"shimmer")         == sValue ) m_eValue = texteffectShimmer;
            else if ( (L"sparkle")         == sValue ) m_eValue = texteffectSparkle;
            else                                        m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case texteffectAntsBlack       : return (L"antsBlack");
			case texteffectAntsRed         : return (L"antsRed");
			case texteffectBlinkBackground : return (L"blinkBackground");
			case texteffectLights          : return (L"lights");
			case texteffectNone            : return (L"none");
			case texteffectShimmer         : return (L"shimmer");
			case texteffectSparkle         : return (L"sparkle");
			default                        : return (L"none");
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
            m_eValue = (std::min)( 600, (std::max)( 0, nValue ) );
		}

        virtual int     FromString(std::wstring &sValue)
		{
            m_eValue = _wtoi( sValue.c_str() );

            m_eValue = (std::min)( 600, (std::max)( 0, m_eValue ) );

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            std::wstring sResult = std::to_wstring( m_eValue);

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

        virtual ETheme  FromString(std::wstring &sValue)
		{
            if      ( (L"majorAscii")    == sValue ) m_eValue = themeMajorAscii;
            else if ( (L"majorBidi")     == sValue ) m_eValue = themeMajorBidi;
            else if ( (L"majorEastAsia") == sValue ) m_eValue = themeMajorEastAsia;
            else if ( (L"majorHAnsi")    == sValue ) m_eValue = themeMajorHAnsi;
            else if ( (L"minorAscii")    == sValue ) m_eValue = themeMinorAscii;
            else if ( (L"minorBidi")     == sValue ) m_eValue = themeMinorBidi;
            else if ( (L"minorEastAsia") == sValue ) m_eValue = themeMinorEastAsia;
            else if ( (L"minorHAnsi")    == sValue ) m_eValue = themeMinorHAnsi;
            else                                      m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case themeMajorAscii    : return (L"majorAscii");
			case themeMajorBidi     : return (L"majorBidi");
			case themeMajorEastAsia : return (L"majorEastAsia");
			case themeMajorHAnsi    : return (L"majorHAnsi");
			case themeMinorAscii    : return (L"minorAscii");
			case themeMinorBidi     : return (L"minorBidi");
			case themeMinorEastAsia : return (L"minorEastAsia");
			case themeMinorHAnsi    : return (L"minorHAnsi");
			default                 : return (L"majorAscii");
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

        virtual EThemeColor FromString(std::wstring &sValue)
		{
            if      ( (L"accent1")           == sValue ) m_eValue = themecolorAccent1;
            else if ( (L"accent2")           == sValue ) m_eValue = themecolorAccent2;
            else if ( (L"accent3")           == sValue ) m_eValue = themecolorAccent3;
            else if ( (L"accent4")           == sValue ) m_eValue = themecolorAccent4;
            else if ( (L"accent5")           == sValue ) m_eValue = themecolorAccent5;
            else if ( (L"accent6")           == sValue ) m_eValue = themecolorAccent6;
            else if ( (L"background1")       == sValue ) m_eValue = themecolorBackground1;
            else if ( (L"background2")       == sValue ) m_eValue = themecolorBackground2;
            else if ( (L"dark1")             == sValue ) m_eValue = themecolorDark1;
            else if ( (L"dark2")             == sValue ) m_eValue = themecolorDark2;
            else if ( (L"followedHyperlink") == sValue ) m_eValue = themecolorFollowedHyperlink;
            else if ( (L"hyperlink")         == sValue ) m_eValue = themecolorHyperlink;
            else if ( (L"light1")            == sValue ) m_eValue = themecolorLight1;
            else if ( (L"light2")            == sValue ) m_eValue = themecolorLight2;
            else if ( (L"none")              == sValue ) m_eValue = themecolorNone;
            else if ( (L"text1")             == sValue ) m_eValue = themecolorText1;
            else if ( (L"text2")             == sValue ) m_eValue = themecolorText2;
            else                                          m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case themecolorAccent1           : return (L"accent1");
			case themecolorAccent2           : return (L"accent2");
			case themecolorAccent3           : return (L"accent3");
			case themecolorAccent4           : return (L"accent4");
			case themecolorAccent5           : return (L"accent5");
			case themecolorAccent6           : return (L"accent6");
			case themecolorBackground1       : return (L"background1");
			case themecolorBackground2       : return (L"background2");
			case themecolorDark1             : return (L"dark1");
			case themecolorDark2             : return (L"dark2");
			case themecolorFollowedHyperlink : return (L"followedHyperlink");
			case themecolorHyperlink         : return (L"hyperlink");
			case themecolorLight1            : return (L"light1");
			case themecolorLight2            : return (L"light2");
			case themecolorNone              : return (L"none");
			case themecolorText1             : return (L"text1");
			case themecolorText2             : return (L"text2");
			default                          : return (L"none");
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

        virtual unsigned char FromString(std::wstring &sValue)
		{
			if ( !Parse(sValue) )
			{
                m_eValue = unDefValue;
			}

            return m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
			return XmlUtils::IntToString(m_eValue, L"%02X");
		}

		SimpleType_FromString     (unsigned char)
		SimpleType_Operator_Equal (CUcharHexNumber)
	private:

        bool Parse(std::wstring &sValue)
		{
            if ( sValue.length() < 2 )
				return false;

			bool bResult = true;

            m_eValue  = (unsigned char)HexToInt( (int)sValue[1], bResult );
            m_eValue += (unsigned char)HexToInt( (int)sValue[0], bResult ) <<  4;

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

        virtual EUnderline FromString(std::wstring &sValue)
		{
            if      ( (L"dash")            == sValue ) m_eValue = underlineDash;
            else if ( (L"dashDotDotHeavy") == sValue ) m_eValue = underlineDashDotDotHeavy;
            else if ( (L"dashDotHeavy")    == sValue ) m_eValue = underlineDashDotHeavy;
            else if ( (L"dashedHeavy")     == sValue ) m_eValue = underlineDashedHeavy;
            else if ( (L"dashLong")        == sValue ) m_eValue = underlineDashLong;
            else if ( (L"dashLongHeavy")   == sValue ) m_eValue = underlineDashLongHeavy;
            else if ( (L"dotDash")         == sValue ) m_eValue = underlineDotDash;
            else if ( (L"dotDotDash")      == sValue ) m_eValue = underlineDotDotDash;
            else if ( (L"dotted")          == sValue ) m_eValue = underlineDotted;
            else if ( (L"dottedHeavy")     == sValue ) m_eValue = underlineDottedHeavy;
            else if ( (L"double")          == sValue ) m_eValue = underlineDouble;
            else if ( (L"none")            == sValue ) m_eValue = underlineNone;
            else if ( (L"single")          == sValue ) m_eValue = underlineSingle;
            else if ( (L"thick")           == sValue ) m_eValue = underlineThick;
            else if ( (L"wave")            == sValue ) m_eValue = underlineWave;
            else if ( (L"wavyDouble")      == sValue ) m_eValue = underlineWavyDouble;
            else if ( (L"wavyHeavy")       == sValue ) m_eValue = underlineWavyHeavy;
            else if ( (L"words")           == sValue ) m_eValue = underlineWords;
            else                                        m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring    ToString  () const
		{
            switch(m_eValue)
			{
			case underlineDash            : return (L"dash");
			case underlineDashDotDotHeavy : return (L"dashDotDotHeavy");
			case underlineDashDotHeavy    : return (L"dashDotHeavy");
			case underlineDashedHeavy     : return (L"dashedHeavy");
			case underlineDashLong        : return (L"dashLong");
			case underlineDashLongHeavy   : return (L"dashLongHeavy");
			case underlineDotDash         : return (L"dotDash");
			case underlineDotDotDash      : return (L"dotDotDash");
			case underlineDotted          : return (L"dotted");
			case underlineDottedHeavy     : return (L"dottedHeavy");
			case underlineDouble          : return (L"double");
			case underlineNone            : return (L"none");
			case underlineSingle          : return (L"single");
			case underlineThick           : return (L"thick");
			case underlineWave            : return (L"wave");
			case underlineWavyDouble      : return (L"wavyDouble");
			case underlineWavyHeavy       : return (L"wavyHeavy");
			case underlineWords           : return (L"words");
			default                       : return (L"none");
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

        virtual EVAnchor FromString(std::wstring &sValue)
		{
            if      ( (L"margin") == sValue ) m_eValue = vanchorMargin;
            else if ( (L"page")   == sValue ) m_eValue = vanchorPage;
            else if ( (L"text")   == sValue ) m_eValue = vanchorText;
            else                               m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring  ToString  () const
		{
            switch(m_eValue)
			{
			case vanchorMargin : return (L"margin");
			case vanchorPage   : return (L"page");
			case vanchorText   : return (L"text");
			default            : return (L"text");
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

        virtual EVerticalJc FromString(std::wstring &sValue)
		{
            if      ( (L"both")   == sValue ) m_eValue = verticaljcBoth;
            else if ( (L"bottom") == sValue ) m_eValue = verticaljcBottom;
            else if ( (L"center") == sValue ) m_eValue = verticaljcCenter;
            else if ( (L"top")    == sValue ) m_eValue = verticaljcTop;
            else                               m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring     ToString  () const
		{
            switch(m_eValue)
			{
			case verticaljcBoth   : return (L"both");
			case verticaljcBottom : return (L"bottom");
			case verticaljcCenter : return (L"center");
			case verticaljcTop    : return (L"top");
			default               : return (L"top");
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

        virtual EView   FromString(std::wstring &sValue)
		{
            if      ( (L"masterPages") == sValue ) m_eValue = viewMasterPages;
            else if ( (L"none")        == sValue ) m_eValue = viewNone;
            else if ( (L"normal")      == sValue ) m_eValue = viewNormal;
            else if ( (L"outline")     == sValue ) m_eValue = viewOutline;
            else if ( (L"print")       == sValue ) m_eValue = viewPrint;
            else if ( (L"web")         == sValue ) m_eValue = viewWeb;
            else                                    m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case viewMasterPages : return (L"masterPages");
			case viewNone        : return (L"none");
			case viewNormal      : return (L"normal");
			case viewOutline     : return (L"outline");
			case viewPrint       : return (L"print");
			case viewWeb         : return (L"web");
			default              : return (L"none");
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

        virtual EWmlColorSchemeIndex FromString(std::wstring &sValue)
		{
            if      ( (L"accent1")           == sValue ) m_eValue = wmlcolorschemeindexAccent1;
            else if ( (L"accent2")           == sValue ) m_eValue = wmlcolorschemeindexAccent2;
            else if ( (L"accent3")           == sValue ) m_eValue = wmlcolorschemeindexAccent3;
            else if ( (L"accent4")           == sValue ) m_eValue = wmlcolorschemeindexAccent4;
            else if ( (L"accent5")           == sValue ) m_eValue = wmlcolorschemeindexAccent5;
            else if ( (L"accent6")           == sValue ) m_eValue = wmlcolorschemeindexAccent6;
            else if ( (L"dark1")             == sValue ) m_eValue = wmlcolorschemeindexDark1;
            else if ( (L"dark2")             == sValue ) m_eValue = wmlcolorschemeindexDark2;
            else if ( (L"followedHyperlink") == sValue ) m_eValue = wmlcolorschemeindexFollowedHyperlink;
            else if ( (L"hyperlink")         == sValue ) m_eValue = wmlcolorschemeindexHyperlink;
            else if ( (L"light1")            == sValue ) m_eValue = wmlcolorschemeindexLight1;
            else if ( (L"light2")            == sValue ) m_eValue = wmlcolorschemeindexLight2;
            else                                          m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring              ToString  () const
		{
            switch(m_eValue)
			{
			case wmlcolorschemeindexAccent1           : return (L"accent1");
			case wmlcolorschemeindexAccent2           : return (L"accent2");
			case wmlcolorschemeindexAccent3           : return (L"accent3");
			case wmlcolorschemeindexAccent4           : return (L"accent4");
			case wmlcolorschemeindexAccent5           : return (L"accent5");
			case wmlcolorschemeindexAccent6           : return (L"accent6");
			case wmlcolorschemeindexDark1             : return (L"dark1");
			case wmlcolorschemeindexDark2             : return (L"dark2");
			case wmlcolorschemeindexFollowedHyperlink : return (L"followedHyperlink");
			case wmlcolorschemeindexHyperlink         : return (L"hyperlink");
			case wmlcolorschemeindexLight1            : return (L"light1");
			case wmlcolorschemeindexLight2            : return (L"light2");
			default                                   : return (L"accent1");
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

        virtual EWrap   FromString(std::wstring &sValue)
		{
            if      ( (L"around")    == sValue ) m_eValue = wrapAround;
            else if ( (L"auto")      == sValue ) m_eValue = wrapAuto;
            else if ( (L"none")      == sValue ) m_eValue = wrapNone;
            else if ( (L"notBeside") == sValue ) m_eValue = wrapNotBeside;
            else if ( (L"through")   == sValue ) m_eValue = wrapThrough;
            else if ( (L"tight")     == sValue ) m_eValue = wrapTight;
            else                                  m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case wrapAround    : return (L"around");
			case wrapAuto      : return (L"auto");
			case wrapNone      : return (L"none");
			case wrapNotBeside : return (L"notBeside");
			case wrapThrough   : return (L"through");
			case wrapTight     : return (L"tight");
			default            : return (L"auto");
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

        virtual EZoom   FromString(std::wstring &sValue)
		{
            if      ( (L"bestFit")  == sValue ) m_eValue = zoomBestFit;
            else if ( (L"fullPage") == sValue ) m_eValue = zoomFullPage;
            else if ( (L"none")     == sValue ) m_eValue = zoomNone;
            else if ( (L"textFit")  == sValue ) m_eValue = zoomTextFit;
            else                                 m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(m_eValue)
			{
			case zoomBestFit   : return (L"bestFit");
			case zoomFullPage : return (L"fullPage");
			case zoomNone     : return (L"none");
			case zoomTextFit  : return (L"textFit");
			default           : return (L"none");
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

        virtual EFontCharset FromString(std::wstring &sValue)
		{
            if      ( (L"00") == sValue ) m_eValue = fontcharsetANSI;
            else if ( (L"01") == sValue ) m_eValue = fontcharsetDefault;
            else if ( (L"02") == sValue ) m_eValue = fontcharsetSymbol;
            else if ( (L"4D") == sValue ) m_eValue = fontcharsetMacintosh;
            else if ( (L"80") == sValue ) m_eValue = fontcharsetShitJIS;
            else if ( (L"81") == sValue ) m_eValue = fontcharsetHangeul;
            else if ( (L"82") == sValue ) m_eValue = fontcharsetJohab;
            else if ( (L"86") == sValue ) m_eValue = fontcharsetGB2313;
            else if ( (L"88") == sValue ) m_eValue = fontcharsetChineseBig5;
            else if ( (L"A1") == sValue ) m_eValue = fontcharsetGreek;
            else if ( (L"A2") == sValue ) m_eValue = fontcharsetTurkish;
            else if ( (L"A3") == sValue ) m_eValue = fontcharsetVietnamese;
            else if ( (L"B1") == sValue ) m_eValue = fontcharsetHebrew;
            else if ( (L"B2") == sValue ) m_eValue = fontcharsetArabic;
            else if ( (L"BA") == sValue ) m_eValue = fontcharsetBaltic;
            else if ( (L"CC") == sValue ) m_eValue = fontcharsetRussian;
            else if ( (L"DE") == sValue ) m_eValue = fontcharsetThai;
            else if ( (L"EE") == sValue ) m_eValue = fontcharsetEastEurope;
            else if ( (L"FF") == sValue ) m_eValue = fontcharsetOEM;
            else                           m_eValue = fontcharsetANSI;

            return m_eValue;
		}

        virtual std::wstring      ToString  () const
		{
            switch(m_eValue)
			{
			case fontcharsetANSI        : return (L"00");
			case fontcharsetDefault     : return (L"01");
			case fontcharsetSymbol      : return (L"02");
			case fontcharsetMacintosh   : return (L"4D");
			case fontcharsetShitJIS     : return (L"80");
			case fontcharsetHangeul     : return (L"81");
			case fontcharsetJohab       : return (L"82");
			case fontcharsetGB2313      : return (L"86");
			case fontcharsetChineseBig5 : return (L"88");
			case fontcharsetGreek       : return (L"A1");
			case fontcharsetTurkish     : return (L"A2");
			case fontcharsetVietnamese  : return (L"A3");
			case fontcharsetHebrew      : return (L"B1");
			case fontcharsetArabic      : return (L"B2");
			case fontcharsetBaltic      : return (L"BA");
			case fontcharsetRussian     : return (L"CC");
			case fontcharsetThai        : return (L"DE");
			case fontcharsetEastEurope  : return (L"EE");
			case fontcharsetOEM         : return (L"FF");
			default                     : return (L"00");
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

        virtual unsigned short FromString(std::wstring &sValue)
		{
			Parse( sValue );     

            return m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            std::wstring sResult;

			for( int nIndex = 0, nMult = 1; nIndex < 12; nIndex++, nMult <<= 1 )
			{
                if ( m_eValue & nMult )
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
            return (bool)( (m_eValue & 1) != 0 );
		}
		inline bool IsLastRow        () const
		{
            return (bool)( (m_eValue & 2) != 0 );
		}
		inline bool IsFirstColumn    () const
		{
            return (bool)( (m_eValue & 4) != 0 );
		}
		inline bool IsLastColumn     () const
		{
            return (bool)( (m_eValue & 8) != 0 );
		}
		inline bool IsBand1Vertical  () const
		{
            return (bool)( (m_eValue & 16) != 0 );
		}
		inline bool IsBand2Vertical  () const
		{
            return (bool)( (m_eValue & 32) != 0 );
		}
		inline bool IsBand1Horizontal() const
		{
            return (bool)( (m_eValue & 64) != 0 );
		}
		inline bool IsBand2Horizontal() const
		{
            return (bool)( (m_eValue & 128) != 0 );
		}
		inline bool IsNeCell         () const
		{
            return (bool)( (m_eValue & 256) != 0 );
		}
		inline bool IsNwCell         () const
		{
            return (bool)( (m_eValue & 512) != 0 );
		}
		inline bool IsSeCell         () const
		{
            return (bool)( (m_eValue & 1024) != 0 );
		}
		inline bool IsSwCell         () const
		{
            return (bool)( (m_eValue & 2048) != 0 );
		}

	private:

        void Parse(std::wstring &sValue)
		{
            int nLen = (std::min)( (int)sValue.length(), 12 );

			for ( int nIndex = 0, nMult = 1; nIndex < nLen; nIndex++, nMult <<= 1 )
			{
				wchar_t wsChar = sValue[nIndex];

				if ( '0' == wsChar )
				{
					// Ничего не делаем
				}
				else if ( '1' == wsChar )
				{
                    m_eValue ^= nMult;
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

        virtual EXmlSpace FromString(std::wstring &sValue)
		{
            if       ( (L"default")  == sValue ) m_eValue = xmlspaceDefault;
            else if  ( (L"preserve") == sValue ) m_eValue = xmlspacePreserve;
            else                                  m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring   ToString  () const
		{
            switch(m_eValue)
			{
			case xmlspaceDefault  : return (L"default");
			case xmlspacePreserve : return (L"preserve");
			default               : return (L"default");
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

        virtual ECryptAlgoritmName FromString(std::wstring &sValue)
		{
            if       ( (L"MD2")        == sValue ) m_eValue = cryptalgoritmnameMD2;
            else if  ( (L"MD4")        == sValue ) m_eValue = cryptalgoritmnameMD4;
            else if  ( (L"MD5")        == sValue ) m_eValue = cryptalgoritmnameMD5;
            else if  ( (L"RIPEMD-128") == sValue ) m_eValue = cryptalgoritmnameRIPEMD128;
            else if  ( (L"RIPEMD-160") == sValue ) m_eValue = cryptalgoritmnameRIPEMD160;
            else if  ( (L"SHA-1")      == sValue ) m_eValue = cryptalgoritmnameSHA1;
            else if  ( (L"SHA-256")    == sValue ) m_eValue = cryptalgoritmnameSHA256;
            else if  ( (L"SHA-384")    == sValue ) m_eValue = cryptalgoritmnameSHA384;
            else if  ( (L"SHA-512")    == sValue ) m_eValue = cryptalgoritmnameSHA512;
            else if  ( (L"WHIRLPOOL")  == sValue ) m_eValue = cryptalgoritmnameWHIRLPOOL;
            else                                    m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(m_eValue)
			{
			case cryptalgoritmnameMD2       : return (L"MD2");
			case cryptalgoritmnameMD4       : return (L"MD4");
			case cryptalgoritmnameMD5       : return (L"MD5");
			case cryptalgoritmnameRIPEMD128 : return (L"RIPEMD-128");
			case cryptalgoritmnameRIPEMD160 : return (L"RIPEMD-160");
			case cryptalgoritmnameSHA1      : return (L"SHA-1");
			case cryptalgoritmnameSHA256    : return (L"SHA-256");
			case cryptalgoritmnameSHA384    : return (L"SHA-384");
			case cryptalgoritmnameSHA512    : return (L"SHA-512");
			case cryptalgoritmnameWHIRLPOOL : return (L"WHIRLPOOL");
			default                         : return (L"");
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

        virtual EPitchFamily FromString(std::wstring &sValue)
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
                m_eValue = (EPitchFamily)nValue; break;
			default:
                m_eValue = eDefValue; break;
			}

            return m_eValue;
		}

        virtual std::wstring      ToString  () const
		{
            switch(m_eValue)
			{
			case pitchfamilyDefUnk: return (L"00");
			case pitchfamilyFixUnk: return (L"01");
			case pitchfamilyVarUnk: return (L"02");
			case pitchfamilyDefRom: return (L"10");
			case pitchfamilyFixRom: return (L"11");
			case pitchfamilyVarRom: return (L"12");
			case pitchfamilyDefSwi: return (L"20");
			case pitchfamilyFixSwi: return (L"21");
			case pitchfamilyVarSwi: return (L"22");
			case pitchfamilyDefMod: return (L"30");
			case pitchfamilyFixMod: return (L"31");
			case pitchfamilyVarMod: return (L"32");
			case pitchfamilyDefScr: return (L"40");
			case pitchfamilyFixScr: return (L"41");
			case pitchfamilyVarScr: return (L"42");
			case pitchfamilyDefDec: return (L"50");
			case pitchfamilyFixDec: return (L"51");
			case pitchfamilyVarDec: return (L"52");
			default               : return (L"00");
			}
		}

		SimpleType_FromString     (EPitchFamily)
		SimpleType_Operator_Equal (CPitchFamily)

		unsigned char GetPitch()
		{
            int nValue = (int)m_eValue;
			return nValue & 0x0F;
		}
		unsigned char GetFamily()
		{
            int nValue = (int)m_eValue;
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

        virtual EDirVal FromString(std::wstring &sValue)
		{
            if       ( (L"ltr")        == sValue ) m_eValue = dirLtr;
            else if  ( (L"rtl")        == sValue ) m_eValue = dirRtl;
            else                                    m_eValue = eDefValue;

            return m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(m_eValue)
			{
			case cryptalgoritmnameMD2       : return (L"ltr");
			case cryptalgoritmnameMD4       : return (L"rtl");
			default                         : return (L"ltr");
			}
		}

		SimpleType_FromString     (EDirVal)
		SimpleType_Operator_Equal (CDirVal)
	};

} // SimpleTypes
