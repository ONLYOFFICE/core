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
#include "SimpleTypes_Word.h"
#include "SimpleTypes_Drawing.h"

#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"
#include "../Base/Unit.h"

namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// AnnotationVMerge 17.18.1 (Part 1)
	//--------------------------------------------------------------------------------

	EAnnotationVMerge CAnnotationVMerge::FromString(const std::wstring &sValue)
	{
		if       ( (L"cont") == sValue ) this->m_eValue = annotvmergeCont;
		else if  ( (L"rest") == sValue ) this->m_eValue = annotvmergeRest;
		else                              this->m_eValue = annotvmergeCont;

		return this->m_eValue;
	}

	std::wstring CAnnotationVMerge::ToString() const
	{
		switch(this->m_eValue)
		{
		case annotvmergeCont : return (L"cont");
		case annotvmergeRest : return (L"rest");
		default              : return (L"cont");
		}
	}

	//--------------------------------------------------------------------------------
	// Border 17.18.2 (Part 1)
	//--------------------------------------------------------------------------------

	EBorder CBorder::FromString(const std::wstring &sValue)
	{
		wchar_t wsFirstChar = 0;

		if ( sValue.length() > 0 )
			wsFirstChar = sValue[0];

		EBorder eDefValue = bordervalueNone;

		switch(wsFirstChar)
		{
		case 'a':
		{
			if      ( (L"apples")                 == sValue ) this->m_eValue = bordervalueApples;
			else if ( (L"archedScallops")         == sValue ) this->m_eValue = bordervalueArchedScallops;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'b':
		{
			if      ( (L"babyPacifier")           == sValue ) this->m_eValue = bordervalueBabyPacifier;
			else if ( (L"babyRattle")             == sValue ) this->m_eValue = bordervalueBabyRattle;
			else if ( (L"balloons3Colors")        == sValue ) this->m_eValue = bordervalueBalloons3Colors;
			else if ( (L"balloonsHotAir")         == sValue ) this->m_eValue = bordervalueBalloonsHotAir;
			else if ( (L"basicBlackDashes")       == sValue ) this->m_eValue = bordervalueBasicBlackDashes;
			else if ( (L"basicBlackDots")         == sValue ) this->m_eValue = bordervalueBasicBlackDots;
			else if ( (L"basicBlackSquares")      == sValue ) this->m_eValue = bordervalueBasicBlackSquares;
			else if ( (L"basicThinLines")         == sValue ) this->m_eValue = bordervalueBasicThinLines;
			else if ( (L"basicWhiteDashes")       == sValue ) this->m_eValue = bordervalueBasicWhiteDashes;
			else if ( (L"basicWhiteDots")         == sValue ) this->m_eValue = bordervalueBasicWhiteDots;
			else if ( (L"basicWhiteSquares")      == sValue ) this->m_eValue = bordervalueBasicWhiteSquares;
			else if ( (L"basicWideInline")        == sValue ) this->m_eValue = bordervalueBasicWideInline;
			else if ( (L"basicWideMidline")       == sValue ) this->m_eValue = bordervalueBasicWideMidline;
			else if ( (L"basicWideOutline")       == sValue ) this->m_eValue = bordervalueBasicWideOutline;
			else if ( (L"bats")                   == sValue ) this->m_eValue = bordervalueBats;
			else if ( (L"birds")                  == sValue ) this->m_eValue = bordervalueBirds;
			else if ( (L"birdsFlight")            == sValue ) this->m_eValue = bordervalueBirdsFlight;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'c':
		{
			if      ( (L"cabins")                 == sValue ) this->m_eValue = bordervalueCabins;
			else if ( (L"cakeSlice")              == sValue ) this->m_eValue = bordervalueCakeSlice;
			else if ( (L"candyCorn")              == sValue ) this->m_eValue = bordervalueCandyCorn;
			else if ( (L"celticKnotwork")         == sValue ) this->m_eValue = bordervalueCelticKnotwork;
			else if ( (L"certificateBanner")      == sValue ) this->m_eValue = bordervalueCertificateBanner;
			else if ( (L"chainLink")              == sValue ) this->m_eValue = bordervalueChainLink;
			else if ( (L"champagneBottle")        == sValue ) this->m_eValue = bordervalueChampagneBottle;
			else if ( (L"checkedBarBlack")        == sValue ) this->m_eValue = bordervalueCheckedBarBlack;
			else if ( (L"checkedBarColor")        == sValue ) this->m_eValue = bordervalueCheckedBarColor;
			else if ( (L"checkered")              == sValue ) this->m_eValue = bordervalueCheckered;
			else if ( (L"christmasTree")          == sValue ) this->m_eValue = bordervalueChristmasTree;
			else if ( (L"circlesLines")           == sValue ) this->m_eValue = bordervalueCirclesLines;
			else if ( (L"circlesRectangles")      == sValue ) this->m_eValue = bordervalueCirclesRectangles;
			else if ( (L"classicalWave")          == sValue ) this->m_eValue = bordervalueClassicalWave;
			else if ( (L"clocks")                 == sValue ) this->m_eValue = bordervalueClocks;
			else if ( (L"compass")                == sValue ) this->m_eValue = bordervalueCompass;
			else if ( (L"confetti")               == sValue ) this->m_eValue = bordervalueConfetti;
			else if ( (L"confettiGrays")          == sValue ) this->m_eValue = bordervalueConfettiGrays;
			else if ( (L"confettiOutline")        == sValue ) this->m_eValue = bordervalueConfettiOutline;
			else if ( (L"confettiStreamers")      == sValue ) this->m_eValue = bordervalueConfettiStreamers;
			else if ( (L"confettiWhite")          == sValue ) this->m_eValue = bordervalueConfettiWhite;
			else if ( (L"cornerTriangles")        == sValue ) this->m_eValue = bordervalueCornerTriangles;
			else if ( (L"couponCutoutDashes")     == sValue ) this->m_eValue = bordervalueCouponCutoutDashes;
			else if ( (L"couponCutoutDots")       == sValue ) this->m_eValue = bordervalueCouponCutoutDots;
			else if ( (L"crazyMaze")              == sValue ) this->m_eValue = bordervalueCrazyMaze;
			else if ( (L"creaturesButterfly")     == sValue ) this->m_eValue = bordervalueCreaturesButterfly;
			else if ( (L"creaturesFish")          == sValue ) this->m_eValue = bordervalueCreaturesFish;
			else if ( (L"creaturesInsects")       == sValue ) this->m_eValue = bordervalueCreaturesInsects;
			else if ( (L"creaturesLadyBug")       == sValue ) this->m_eValue = bordervalueCreaturesLadyBug;
			else if ( (L"crossStitch")            == sValue ) this->m_eValue = bordervalueCrossStitch;
			else if ( (L"cup")                    == sValue ) this->m_eValue = bordervalueCup;
			else if ( (L"custom")                 == sValue ) this->m_eValue = bordervalueCustom;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'd':
		{
			if      ( (L"dashDotStroked")         == sValue ) this->m_eValue = bordervalueDashDotStroked;
			else if ( (L"dashed")                 == sValue ) this->m_eValue = bordervalueDashed;
			else if ( (L"dashSmallGap")           == sValue ) this->m_eValue = bordervalueDashSmallGap;
			else if ( (L"decoArch")               == sValue ) this->m_eValue = bordervalueDecoArch;
			else if ( (L"decoArchColor")          == sValue ) this->m_eValue = bordervalueDecoArchColor;
			else if ( (L"decoBlocks")             == sValue ) this->m_eValue = bordervalueDecoBlocks;
			else if ( (L"diamondsGray")           == sValue ) this->m_eValue = bordervalueDiamondsGray;
			else if ( (L"dotDash")                == sValue ) this->m_eValue = bordervalueDotDash;
			else if ( (L"dotDotDash")             == sValue ) this->m_eValue = bordervalueDotDotDash;
			else if ( (L"dotted")                 == sValue ) this->m_eValue = bordervalueDotted;
			else if ( (L"double")                 == sValue ) this->m_eValue = bordervalueDouble;
			else if ( (L"doubleD")                == sValue ) this->m_eValue = bordervalueDoubleD;
			else if ( (L"doubleDiamonds")         == sValue ) this->m_eValue = bordervalueDoubleDiamonds;
			else if ( (L"doubleWave")             == sValue ) this->m_eValue = bordervalueDoubleWave;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'e':
		{
			if      ( (L"earth1")                 == sValue ) this->m_eValue = bordervalueEarth1;
			else if ( (L"earth2")                 == sValue ) this->m_eValue = bordervalueEarth2;
			else if ( (L"earth3")                 == sValue ) this->m_eValue = bordervalueEarth3;
			else if ( (L"eclipsingSquares1")      == sValue ) this->m_eValue = bordervalueEclipsingSquares1;
			else if ( (L"eclipsingSquares2")      == sValue ) this->m_eValue = bordervalueEclipsingSquares2;
			else if ( (L"eggsBlack")              == sValue ) this->m_eValue = bordervalueEggsBlack;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'f':
		{
			if      ( (L"fans")                   == sValue ) this->m_eValue = bordervalueFans;
			else if ( (L"film")                   == sValue ) this->m_eValue = bordervalueFilm;
			else if ( (L"firecrackers")           == sValue ) this->m_eValue = bordervalueFirecrackers;
			else if ( (L"flowersBlockPrint")      == sValue ) this->m_eValue = bordervalueFlowersBlockPrint;
			else if ( (L"flowersDaisies")         == sValue ) this->m_eValue = bordervalueFlowersDaisies;
			else if ( (L"flowersModern1")         == sValue ) this->m_eValue = bordervalueFlowersModern1;
			else if ( (L"flowersModern2")         == sValue ) this->m_eValue = bordervalueFlowersModern2;
			else if ( (L"flowersPansy")           == sValue ) this->m_eValue = bordervalueFlowersPansy;
			else if ( (L"flowersRedRose")         == sValue ) this->m_eValue = bordervalueFlowersRedRose;
			else if ( (L"flowersRoses")           == sValue ) this->m_eValue = bordervalueFlowersRoses;
			else if ( (L"flowersTeacup")          == sValue ) this->m_eValue = bordervalueFlowersTeacup;
			else if ( (L"flowersTiny")            == sValue ) this->m_eValue = bordervalueFlowersTiny;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'g':
		{
			if      ( (L"gems")                   == sValue ) this->m_eValue = bordervalueGems;
			else if ( (L"gingerbreadMan")         == sValue ) this->m_eValue = bordervalueGingerbreadMan;
			else if ( (L"gradient")               == sValue ) this->m_eValue = bordervalueGradient;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'h':
		{
			if      ( (L"handmade1")              == sValue ) this->m_eValue = bordervalueHandmade1;
			else if ( (L"handmade2")              == sValue ) this->m_eValue = bordervalueHandmade2;
			else if ( (L"heartBalloon")           == sValue ) this->m_eValue = bordervalueHeartBalloon;
			else if ( (L"heartGray")              == sValue ) this->m_eValue = bordervalueHeartGray;
			else if ( (L"hearts")                 == sValue ) this->m_eValue = bordervalueHearts;
			else if ( (L"heebieJeebies")          == sValue ) this->m_eValue = bordervalueHeebieJeebies;
			else if ( (L"holly")                  == sValue ) this->m_eValue = bordervalueHolly;
			else if ( (L"houseFunky")             == sValue ) this->m_eValue = bordervalueHouseFunky;
			else if ( (L"hypnotic")               == sValue ) this->m_eValue = bordervalueHypnotic;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'i':
		{
			if      ( (L"iceCreamCones")          == sValue ) this->m_eValue = bordervalueIceCreamCones;
			else if ( (L"inset")                  == sValue ) this->m_eValue = bordervalueInset;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'l':
		{
			if      ( (L"lightBulb")              == sValue ) this->m_eValue = bordervalueLightBulb;
			else if ( (L"lightning1")             == sValue ) this->m_eValue = bordervalueLightning1;
			else if ( (L"lightning2")             == sValue ) this->m_eValue = bordervalueLightning2;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'm':
		{
			if      ( (L"mapleLeaf")              == sValue ) this->m_eValue = bordervalueMapleLeaf;
			else if ( (L"mapleMuffins")           == sValue ) this->m_eValue = bordervalueMapleMuffins;
			else if ( (L"mapPins")                == sValue ) this->m_eValue = bordervalueMapPins;
			else if ( (L"marquee")                == sValue ) this->m_eValue = bordervalueMarquee;
			else if ( (L"marqueeToothed")         == sValue ) this->m_eValue = bordervalueMarqueeToothed;
			else if ( (L"moons")                  == sValue ) this->m_eValue = bordervalueMoons;
			else if ( (L"mosaic")                 == sValue ) this->m_eValue = bordervalueMosaic;
			else if ( (L"musicNotes")             == sValue ) this->m_eValue = bordervalueMusicNotes;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'n':
		{
			if      ( (L"nil")                    == sValue ) this->m_eValue = bordervalueNil;
			else if ( (L"none")                   == sValue ) this->m_eValue = bordervalueNone;
			else if ( (L"northwest")              == sValue ) this->m_eValue = bordervalueNorthwest;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'o':
		{
			if      ( (L"outset")                 == sValue ) this->m_eValue = bordervalueOutset;
			else if ( (L"ovals")                  == sValue ) this->m_eValue = bordervalueOvals;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'p':
		{
			if      ( (L"packages")               == sValue ) this->m_eValue = bordervaluePackages;
			else if ( (L"palmsBlack")             == sValue ) this->m_eValue = bordervaluePalmsBlack;
			else if ( (L"palmsColor")             == sValue ) this->m_eValue = bordervaluePalmsColor;
			else if ( (L"paperClips")             == sValue ) this->m_eValue = bordervaluePaperClips;
			else if ( (L"papyrus")                == sValue ) this->m_eValue = bordervaluePapyrus;
			else if ( (L"partyFavor")             == sValue ) this->m_eValue = bordervaluePartyFavor;
			else if ( (L"partyGlass")             == sValue ) this->m_eValue = bordervaluePartyGlass;
			else if ( (L"pencils")                == sValue ) this->m_eValue = bordervaluePencils;
			else if ( (L"people")                 == sValue ) this->m_eValue = bordervaluePeople;
			else if ( (L"peopleHats")             == sValue ) this->m_eValue = bordervaluePeopleHats;
			else if ( (L"peopleWaving")           == sValue ) this->m_eValue = bordervaluePeopleWaving;
			else if ( (L"poinsettias")            == sValue ) this->m_eValue = bordervaluePoinsettias;
			else if ( (L"postageStamp")           == sValue ) this->m_eValue = bordervaluePostageStamp;
			else if ( (L"pumpkin1")               == sValue ) this->m_eValue = bordervaluePumpkin1;
			else if ( (L"pushPinNote1")           == sValue ) this->m_eValue = bordervaluePushPinNote1;
			else if ( (L"pushPinNote2")           == sValue ) this->m_eValue = bordervaluePushPinNote2;
			else if ( (L"pyramids")               == sValue ) this->m_eValue = bordervaluePyramids;
			else if ( (L"pyramidsAbove")          == sValue ) this->m_eValue = bordervaluePyramidsAbove;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'q':
		{
			if      ( (L"quadrants")              == sValue ) this->m_eValue = bordervalueQuadrants;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'r':
		{
			if      ( (L"rings")                  == sValue ) this->m_eValue = bordervalueRings;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 's':
		{
			if      ( (L"safari")                 == sValue ) this->m_eValue = bordervalueSafari;
			else if ( (L"sawtooth")               == sValue ) this->m_eValue = bordervalueSawtooth;
			else if ( (L"sawtoothGray")           == sValue ) this->m_eValue = bordervalueSawtoothGray;
			else if ( (L"scaredCat")              == sValue ) this->m_eValue = bordervalueScaredCat;
			else if ( (L"seattle")                == sValue ) this->m_eValue = bordervalueSeattle;
			else if ( (L"shadowedSquares")        == sValue ) this->m_eValue = bordervalueShadowedSquares;
			else if ( (L"shapes1")                == sValue ) this->m_eValue = bordervalueShapes1;
			else if ( (L"shapes2")                == sValue ) this->m_eValue = bordervalueShapes2;
			else if ( (L"sharksTeeth")            == sValue ) this->m_eValue = bordervalueSharksTeeth;
			else if ( (L"shorebirdTracks")        == sValue ) this->m_eValue = bordervalueShorebirdTracks;
			else if ( (L"single")                 == sValue ) this->m_eValue = bordervalueSingle;
			else if ( (L"skyrocket")              == sValue ) this->m_eValue = bordervalueSkyrocket;
			else if ( (L"snowflakeFancy")         == sValue ) this->m_eValue = bordervalueSnowflakeFancy;
			else if ( (L"snowflakes")             == sValue ) this->m_eValue = bordervalueSnowflakes;
			else if ( (L"sombrero")               == sValue ) this->m_eValue = bordervalueSombrero;
			else if ( (L"southwest")              == sValue ) this->m_eValue = bordervalueSouthwest;
			else if ( (L"stars")                  == sValue ) this->m_eValue = bordervalueStars;
			else if ( (L"stars3d")                == sValue ) this->m_eValue = bordervalueStars3d;
			else if ( (L"starsBlack")             == sValue ) this->m_eValue = bordervalueStarsBlack;
			else if ( (L"starsShadowed")          == sValue ) this->m_eValue = bordervalueStarsShadowed;
			else if ( (L"starsTop")               == sValue ) this->m_eValue = bordervalueStarsTop;
			else if ( (L"sun")                    == sValue ) this->m_eValue = bordervalueSun;
			else if ( (L"swirligig")              == sValue ) this->m_eValue = bordervalueSwirligig;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 't':
		{
			if      ( (L"thick")                  == sValue ) this->m_eValue = bordervalueThick;
			else if ( (L"thickThinLargeGap")      == sValue ) this->m_eValue = bordervalueThickThinLargeGap;
			else if ( (L"thickThinMediumGap")     == sValue ) this->m_eValue = bordervalueThickThinMediumGap;
			else if ( (L"thickThinSmallGap")      == sValue ) this->m_eValue = bordervalueThickThinSmallGap;
			else if ( (L"thinThickLargeGap")      == sValue ) this->m_eValue = bordervalueThinThickLargeGap;
			else if ( (L"thinThickMediumGap")     == sValue ) this->m_eValue = bordervalueThinThickMediumGap;
			else if ( (L"thinThickSmallGap")      == sValue ) this->m_eValue = bordervalueThinThickSmallGap;
			else if ( (L"thinThickThinLargeGap")  == sValue ) this->m_eValue = bordervalueThinThickThinLargeGap;
			else if ( (L"thinThickThinMediumGap") == sValue ) this->m_eValue = bordervalueThinThickThinMediumGap;
			else if ( (L"thinThickThinSmallGap")  == sValue ) this->m_eValue = bordervalueThinThickThinSmallGap;
			else if ( (L"threeDEmboss")           == sValue ) this->m_eValue = bordervalueThreeDEmboss;
			else if ( (L"threeDEngrave")          == sValue ) this->m_eValue = bordervalueThreeDEngrave;
			else if ( (L"tornPaper")              == sValue ) this->m_eValue = bordervalueTornPaper;
			else if ( (L"tornPaperBlack")         == sValue ) this->m_eValue = bordervalueTornPaperBlack;
			else if ( (L"trees")                  == sValue ) this->m_eValue = bordervalueTrees;
			else if ( (L"triangle1")              == sValue ) this->m_eValue = bordervalueTriangle1;
			else if ( (L"triangle2")              == sValue ) this->m_eValue = bordervalueTriangle2;
			else if ( (L"triangleCircle1")        == sValue ) this->m_eValue = bordervalueTriangleCircle1;
			else if ( (L"triangleCircle2")        == sValue ) this->m_eValue = bordervalueTriangleCircle2;
			else if ( (L"triangleParty")          == sValue ) this->m_eValue = bordervalueTriangleParty;
			else if ( (L"triangles")              == sValue ) this->m_eValue = bordervalueTriangles;
			else if ( (L"triple")                 == sValue ) this->m_eValue = bordervalueTriple;
			else if ( (L"twistedLines1")          == sValue ) this->m_eValue = bordervalueTwistedLines1;
			else if ( (L"twistedLines2")          == sValue ) this->m_eValue = bordervalueTwistedLines2;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'v':
		{
			if      ( (L"vine")                   == sValue ) this->m_eValue = bordervalueVine;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'w':
		{
			if      ( (L"wave")                   == sValue ) this->m_eValue = bordervalueWave;
			else if ( (L"waveline")               == sValue ) this->m_eValue = bordervalueWaveline;
			else if ( (L"weavingAngles")          == sValue ) this->m_eValue = bordervalueWeavingAngles;
			else if ( (L"weavingBraid")           == sValue ) this->m_eValue = bordervalueWeavingBraid;
			else if ( (L"weavingRibbon")          == sValue ) this->m_eValue = bordervalueWeavingRibbon;
			else if ( (L"weavingStrips")          == sValue ) this->m_eValue = bordervalueWeavingStrips;
			else if ( (L"whiteFlowers")           == sValue ) this->m_eValue = bordervalueWhiteFlowers;
			else if ( (L"woodwork")               == sValue ) this->m_eValue = bordervalueWoodwork;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'x':
		{
			if      ( (L"xIllusions")             == sValue ) this->m_eValue = bordervalueXIllusions;
			else                                               this->m_eValue = eDefValue;
			break;
		}
		case 'z':
		{
			if      ( (L"zanyTriangles")          == sValue ) this->m_eValue = bordervalueZanyTriangles;
			else if ( (L"zigZag")                 == sValue ) this->m_eValue = bordervalueZigZag;
			else if ( (L"zigZagStitch")           == sValue ) this->m_eValue = bordervalueZigZagStitch;
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

	std::wstring CBorder::ToString  () const
	{
		switch(this->m_eValue)
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

	bool CBorder::IsArtBorder () const
	{
		return ((int)this->m_eValue > 100);
	}

	bool CBorder::IsLineBorder() const
	{
		return ( (int)this->m_eValue <= 25 && (int)this->m_eValue >= 1 );
	}

	bool CBorder::IsNoBorder  () const
	{
		return ( (int)this->m_eValue <= 0 );
	}

	//--------------------------------------------------------------------------------
	// BrClear 17.18.3 (Part 1)
	//--------------------------------------------------------------------------------

	EBrClear CBrClear::FromString(const std::wstring &sValue)
	{
		if      ( (L"all")   == sValue ) this->m_eValue = brclearAll;
		else if ( (L"left")  == sValue ) this->m_eValue = brclearLeft;
		else if ( (L"none")  == sValue ) this->m_eValue = brclearNone;
		else if ( (L"right") == sValue ) this->m_eValue = brclearRight;
		else                              this->m_eValue = brclearAll;

		return this->m_eValue;
	}

	std::wstring CBrClear::ToString  () const
	{
		switch(this->m_eValue)
		{
		case brclearAll   : return (L"all");
		case brclearLeft  : return (L"left");
		case brclearNone  : return (L"none");
		case brclearRight : return (L"right");
		default           : return (L"all");
		}
	}

	//--------------------------------------------------------------------------------
	// BrType 17.18.4 (Part 1)
	//--------------------------------------------------------------------------------


	EBrType CBrType::FromString(const std::wstring &sValue)
	{
		if      ( (L"column")       == sValue ) this->m_eValue = brtypeColumn;
		else if ( (L"page")         == sValue ) this->m_eValue = brtypePage;
		else if ( (L"textWrapping") == sValue ) this->m_eValue = brtypeTextWrapping;
		else                                     this->m_eValue = brtypePage;

		return this->m_eValue;
	}

	std::wstring CBrType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case brtypeColumn       : return (L"column");
		case brtypePage         : return (L"page");
		case brtypeTextWrapping : return (L"textWrapping");
		default                 : return (L"page");
		}
	}

	//--------------------------------------------------------------------------------
	// CaptionPos 17.18.5 (Part 1)
	//--------------------------------------------------------------------------------

	ECaptionPos CCaptionPos::FromString(const std::wstring &sValue)
	{
		if      ( (L"above") == sValue ) this->m_eValue = captionposAbove;
		else if ( (L"below") == sValue ) this->m_eValue = captionposBelow;
		else if ( (L"left")  == sValue ) this->m_eValue = captionposLeft;
		else if ( (L"right") == sValue ) this->m_eValue = captionposRight;
		else                              this->m_eValue = captionposAbove;

		return this->m_eValue;
	}

	std::wstring CCaptionPos::ToString  () const
	{
		switch(this->m_eValue)
		{
		case captionposAbove : return (L"above");
		case captionposBelow : return (L"below");
		case captionposLeft  : return (L"left");
		case captionposRight : return (L"right");
		default              : return (L"above");
		}
	}

	//--------------------------------------------------------------------------------
	// ChapterSep 17.18.6 (Part 1)
	//--------------------------------------------------------------------------------

	EChapterSep CChapterSep::FromString(const std::wstring &sValue)
	{
		if      ( (L"colon")  == sValue ) this->m_eValue = chaptersepColon;
		else if ( (L"emDash") == sValue ) this->m_eValue = chaptersepEmDash;
		else if ( (L"enDash") == sValue ) this->m_eValue = chaptersepEnDash;
		else if ( (L"hyphen") == sValue ) this->m_eValue = chaptersepHyphen;
		else if ( (L"period") == sValue ) this->m_eValue = chaptersepPeriod;
		else                               this->m_eValue = chaptersepColon;

		return this->m_eValue;
	}

	std::wstring CChapterSep::ToString  () const
	{
		switch(this->m_eValue)
		{
		case chaptersepColon  : return (L"colon");
		case chaptersepEmDash : return (L"emDash");
		case chaptersepEnDash : return (L"enDash");
		case chaptersepHyphen : return (L"hyphen");
		case chaptersepPeriod : return (L"period");
		default               : return (L"colon");
		}
	}

	//--------------------------------------------------------------------------------
	// CharacterSpacing 17.18.7 (Part 1)
	//--------------------------------------------------------------------------------

	ECharacterSpacing CCharacterSpacing::FromString(const std::wstring &sValue)
	{
		if      ( (L"compressPunctuation")                == sValue ) this->m_eValue = charspacingCompressPunctuation;
		else if ( (L"compressPunctuationAndJapaneseKana") == sValue ) this->m_eValue = charspacingCompressPunctuationAndJapaneseKana;
		else if ( (L"doNotCompress")                      == sValue ) this->m_eValue = charspacingDoNotCompress;
		else                                                           this->m_eValue = charspacingDoNotCompress;

		return this->m_eValue;
	}

	std::wstring CCharacterSpacing::ToString  () const
	{
		switch(this->m_eValue)
		{
		case charspacingCompressPunctuation                : return (L"compressPunctuation");
		case charspacingCompressPunctuationAndJapaneseKana : return (L"compressPunctuationAndJapaneseKana");
		case charspacingDoNotCompress                      : return (L"doNotCompress");
		default                                            : return (L"doNotCompress");
		}
	}

	//--------------------------------------------------------------------------------
	// CombineBrackets 17.18.8 (Part 1)
	//--------------------------------------------------------------------------------


	ECombineBrackets CCombineBrackets::FromString(const std::wstring &sValue)
	{
		if      ( (L"angle")  == sValue ) this->m_eValue = combinebracketsAngle;
		else if ( (L"curly")  == sValue ) this->m_eValue = combinebracketsCurly;
		else if ( (L"none")   == sValue ) this->m_eValue = combinebracketsNone;
		else if ( (L"round")  == sValue ) this->m_eValue = combinebracketsRound;
		else if ( (L"square") == sValue ) this->m_eValue = combinebracketsSquare;
		else                               this->m_eValue = combinebracketsNone;

		return this->m_eValue;
	}

	std::wstring CCombineBrackets::ToString  () const
	{
		switch(this->m_eValue)
		{
		case combinebracketsAngle  : return (L"angle");
		case combinebracketsCurly  : return (L"curly");
		case combinebracketsNone   : return (L"none");
		case combinebracketsRound  : return (L"round");
		case combinebracketsSquare : return (L"square");
		default                    : return (L"none");
		}
	}

	//--------------------------------------------------------------------------------
	// DateTime 17.18.9 (Part 1)
	//--------------------------------------------------------------------------------

	CDateTime::CDateTime() {}

	std::wstring CDateTime::GetValue() const
	{
		return m_sValue;
	}

	void CDateTime::SetValue(const std::wstring &sValue)
	{
		m_sValue = sValue;
	}

	std::wstring CDateTime::FromString(const std::wstring &sValue)
	{
		m_sValue = sValue;

		return m_sValue;
	}

	std::wstring CDateTime::ToString  () const
	{
		return m_sValue;
	}

	//--------------------------------------------------------------------------------
	// DecimalNumberOrPercent 17.18.11 (Part 1)
	//--------------------------------------------------------------------------------

	CDecimalNumberOrPercent::CDecimalNumberOrPercent()
	{
		m_dValue = 0;
		m_bTrailingPercentSign = false;
	}

	double CDecimalNumberOrPercent::GetValue() const
	{
		return m_dValue;
	}

	int CDecimalNumberOrPercent::GetValueIntegerPercent() const
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

	void CDecimalNumberOrPercent::SetValue(double dValue)
	{
		m_dValue = dValue;
	}

	double CDecimalNumberOrPercent::FromString(const std::wstring &sValue, double dDefValue)
	{
		if ( sValue.empty() )
		{
			m_dValue = dDefValue;
			return m_dValue;
		}

		int nLen = (int)sValue.length();
		wchar_t wsLastChar = sValue[nLen - 1 ];
		if ( wsLastChar == '%' )
		{
			nLen--;
			m_bTrailingPercentSign = true;
		}
		else
			m_bTrailingPercentSign = false;

		m_dValue = XmlUtils::GetDouble(sValue.substr(0, nLen));

		return m_dValue;
	}

	std::wstring CDecimalNumberOrPercent::ToString  () const
	{
		std::wstring sResult;

		if ( !m_bTrailingPercentSign )
			sResult = std::to_wstring( (int)m_dValue);
		else
			sResult = boost::lexical_cast<std::wstring>(m_dValue) + L"%";

		return sResult;
	}

	bool CDecimalNumberOrPercent::IsPercent() const
	{
		return m_bTrailingPercentSign;
	}

	void CDecimalNumberOrPercent::SetPercent(bool bPercent)
	{
		m_bTrailingPercentSign = bPercent;
	}

	//--------------------------------------------------------------------------------
	// Direction 17.18.12 (Part 1)
	//--------------------------------------------------------------------------------

	EDirection CDirection::FromString(const std::wstring &sValue)
	{
		if      ( (L"ltr")  == sValue ) this->m_eValue = directionLTR;
		else if ( (L"rtl")  == sValue ) this->m_eValue = directionRTL;
		else                             this->m_eValue = directionLTR;

		return this->m_eValue;
	}

	std::wstring CDirection::ToString  () const
	{
		switch(this->m_eValue)
		{
		case directionLTR : return (L"ltr");
		case directionRTL : return (L"rtl");
		default           : return (L"ltr");
		}
	}

	//--------------------------------------------------------------------------------
	// DisplacedByCustomXml 17.18.13 (Part 1)
	//--------------------------------------------------------------------------------

	EDisplacedByCustomXml CDisplacedByCustomXml::FromString(const std::wstring &sValue)
	{
		if      ( (L"next")  == sValue ) this->m_eValue = displacedbycustomxmlNext;
		else if ( (L"prev")  == sValue ) this->m_eValue = displacedbycustomxmlPrev;
		else                             this->m_eValue = displacedbycustomxmlNext;

		return this->m_eValue;
	}

	std::wstring CDisplacedByCustomXml::ToString  () const
	{
		switch(this->m_eValue)
		{
		case displacedbycustomxmlNext : return (L"next");
		case displacedbycustomxmlPrev : return (L"prev");
		default                       : return (L"next");
		}
	}

	//--------------------------------------------------------------------------------
	// DocGrid 17.18.14 (Part 1)
	//--------------------------------------------------------------------------------

	EDocGrid CDocGrid::FromString(const std::wstring &sValue)
	{
		if      ( (L"default")       == sValue ) this->m_eValue = docgridDefault;
		else if ( (L"lines")         == sValue ) this->m_eValue = docgridLines;
		else if ( (L"linesAndChars") == sValue ) this->m_eValue = docgridLinesAndChars;
		else if ( (L"snapToChars")   == sValue ) this->m_eValue = docgridSnapToChars;
		else                                      this->m_eValue = docgridDefault;

		return this->m_eValue;
	}

	std::wstring  CDocGrid::ToString  () const
	{
		switch(this->m_eValue)
		{
		case docgridDefault       : return (L"default");
		case docgridLines         : return (L"lines");
		case docgridLinesAndChars : return (L"linesAndChars");
		case docgridSnapToChars   : return (L"snapToChars");
		default                   : return (L"default");
		}
	}

	//--------------------------------------------------------------------------------
	// DocPartBehavior 17.18.15 (Part 1)
	//--------------------------------------------------------------------------------

	EDocPartBehavior CDocPartBehavior::FromString(const std::wstring &sValue)
	{
		if      ( (L"content") == sValue ) this->m_eValue = docpartbehaviorContent;
		else if ( (L"p")       == sValue ) this->m_eValue = docpartbehaviorP;
		else if ( (L"pg")      == sValue ) this->m_eValue = docpartbehaviorPg;
		else                                this->m_eValue = docpartbehaviorContent;

		return this->m_eValue;
	}

	std::wstring CDocPartBehavior::ToString  () const
	{
		switch(this->m_eValue)
		{
		case docpartbehaviorContent : return (L"content");
		case docpartbehaviorP       : return (L"p");
		case docpartbehaviorPg      : return (L"pg");
		default                     : return (L"content");
		}
	}

	//--------------------------------------------------------------------------------
	// DocPartGallery 17.18.16 (Part 1)
	//--------------------------------------------------------------------------------

	EDocPartGallery CDocPartGallery::FromString(const std::wstring &sValue)
	{
		if      ( (L"any")               == sValue ) this->m_eValue = docpartgalleryAny;
		else if ( (L"autoTxt")           == sValue ) this->m_eValue = docpartgalleryAutoTxt;
		else if ( (L"bib")               == sValue ) this->m_eValue = docpartgalleryBib;
		else if ( (L"coverPg")           == sValue ) this->m_eValue = docpartgalleryCoverPg;
		else if ( (L"custAutoTxt")       == sValue ) this->m_eValue = docpartgalleryCustAutoTxt;
		else if ( (L"custBib")           == sValue ) this->m_eValue = docpartgalleryCustBib;
		else if ( (L"custCoverPg")       == sValue ) this->m_eValue = docpartgalleryCustCoverPg;
		else if ( (L"custEq")            == sValue ) this->m_eValue = docpartgalleryCustEq;
		else if ( (L"custFtrs")          == sValue ) this->m_eValue = docpartgalleryCustFtrs;
		else if ( (L"custHdrs")          == sValue ) this->m_eValue = docpartgalleryCustHdrs;
		else if ( (L"custom1")           == sValue ) this->m_eValue = docpartgalleryCustom1;
		else if ( (L"custom2")           == sValue ) this->m_eValue = docpartgalleryCustom2;
		else if ( (L"custom3")           == sValue ) this->m_eValue = docpartgalleryCustom3;
		else if ( (L"custom4")           == sValue ) this->m_eValue = docpartgalleryCustom4;
		else if ( (L"custom5")           == sValue ) this->m_eValue = docpartgalleryCustom5;
		else if ( (L"custPgNum")         == sValue ) this->m_eValue = docpartgalleryCustPgNum;
		else if ( (L"custPgNumB")        == sValue ) this->m_eValue = docpartgalleryCustPgNumB;
		else if ( (L"custPgNumMargins")  == sValue ) this->m_eValue = docpartgalleryCustPgNumMargins;
		else if ( (L"custPgNumT")        == sValue ) this->m_eValue = docpartgalleryCustPgNumT;
		else if ( (L"custQuickParts")    == sValue ) this->m_eValue = docpartgalleryCustQuickParts;
		else if ( (L"custTblOfContents") == sValue ) this->m_eValue = docpartgalleryCustTblOfContents;
		else if ( (L"custTbls")          == sValue ) this->m_eValue = docpartgalleryCustTbls;
		else if ( (L"custTxtBox")        == sValue ) this->m_eValue = docpartgalleryCustTxtBox;
		else if ( (L"custWatermarks")    == sValue ) this->m_eValue = docpartgalleryCustWatermarks;
		else if ( (L"default")           == sValue ) this->m_eValue = docpartgalleryDefault;
		else if ( (L"docParts")          == sValue ) this->m_eValue = docpartgalleryDocParts;
		else if ( (L"eq")                == sValue ) this->m_eValue = docpartgalleryEq;
		else if ( (L"ftrs")              == sValue ) this->m_eValue = docpartgalleryFtrs;
		else if ( (L"hdrs")              == sValue ) this->m_eValue = docpartgalleryHdrs;
		else if ( (L"pgNum")             == sValue ) this->m_eValue = docpartgalleryPgNum;
		else if ( (L"pgNumB")            == sValue ) this->m_eValue = docpartgalleryPgNumB;
		else if ( (L"pgNumMargins")      == sValue ) this->m_eValue = docpartgalleryPgNumMargins;
		else if ( (L"pgNumT")            == sValue ) this->m_eValue = docpartgalleryPgNumT;
		else if ( (L"placeholder")       == sValue ) this->m_eValue = docpartgalleryPlaceHolder;
		else if ( (L"tblOfContents")     == sValue ) this->m_eValue = docpartgalleryTblOfContents;
		else if ( (L"tbls")              == sValue ) this->m_eValue = docpartgalleryTbls;
		else if ( (L"txtBox")            == sValue ) this->m_eValue = docpartgalleryTxtBox;
		else if ( (L"watermarks")        == sValue ) this->m_eValue = docpartgalleryWatermarks;
		else                                          this->m_eValue = docpartgalleryDefault;

		return this->m_eValue;
	}

	std::wstring CDocPartGallery::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// DocPartType 17.18.17 (Part 1)
	//--------------------------------------------------------------------------------

	EDocPartType CDocPartType::FromString(const std::wstring &sValue)
	{
		if      ( (L"autoExp")  == sValue ) this->m_eValue = docparttypeAutoExp;
		else if ( (L"bbPlcHdr") == sValue ) this->m_eValue = docparttypeBBPlcHdr;
		else if ( (L"formFld")  == sValue ) this->m_eValue = docparttypeFormFld;
		else if ( (L"none")     == sValue ) this->m_eValue = docparttypeNone;
		else if ( (L"normal")   == sValue ) this->m_eValue = docparttypeNormal;
		else if ( (L"speller")  == sValue ) this->m_eValue = docparttypeSpeller;
		else if ( (L"toolbar")  == sValue ) this->m_eValue = docparttypeToolbar;
		else                                 this->m_eValue = docparttypeNone;

		return this->m_eValue;
	}

	std::wstring CDocPartType::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// DocProtect 17.18.18 (Part 1)
	//--------------------------------------------------------------------------------

	EDocProtect CDocProtect::FromString(const std::wstring &sValue)
	{
		if      ( (L"comments")       == sValue ) this->m_eValue = docprotectComments;
		else if ( (L"forms")          == sValue ) this->m_eValue = docprotectForms;
		else if ( (L"none")           == sValue ) this->m_eValue = docprotectNone;
		else if ( (L"readOnly")       == sValue ) this->m_eValue = docprotectReadOnly;
		else if ( (L"trackedChanges" )== sValue ) this->m_eValue = docprotectTrackedChanges;
		else                                       this->m_eValue = docprotectNone;

		return this->m_eValue;
	}

	std::wstring CDocProtect::ToString  () const
	{
		switch(this->m_eValue)
		{
		case docprotectComments       : return (L"comments");
		case docprotectForms          : return (L"forms");
		case docprotectNone           : return (L"none");
		case docprotectReadOnly       : return (L"readOnly");
		case docprotectTrackedChanges : return (L"trackedChanges");
		default                       : return (L"none");
		}
	}

	//--------------------------------------------------------------------------------
	// DocType 17.18.19 (Part 1)
	//--------------------------------------------------------------------------------

	EDocType CDocType::FromString(const std::wstring &sValue)
	{
		if      ( (L"eMail")        == sValue ) this->m_eValue = doctypeEMail;
		else if ( (L"letter")       == sValue ) this->m_eValue = doctypeLetter;
		else if ( (L"notSpecified") == sValue ) this->m_eValue = doctypeNotSpecified;
		else                                     this->m_eValue = doctypeNotSpecified;

		return this->m_eValue;
	}

	std::wstring CDocType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case doctypeEMail        : return (L"eMail");
		case doctypeLetter       : return (L"letter");
		case doctypeNotSpecified : return (L"notSpecified");
		default                  : return (L"notSpecified");
		}
	}

	//--------------------------------------------------------------------------------
	// DropCap 17.18.20 (Part 1)
	//--------------------------------------------------------------------------------

	EDropCap CDropCap::FromString(const std::wstring &sValue)
	{
		if      ( (L"drop")   == sValue ) this->m_eValue = dropcapDrop;
		else if ( (L"margin") == sValue ) this->m_eValue = dropcapMargin;
		else if ( (L"none")   == sValue ) this->m_eValue = dropcapNone;
		else                               this->m_eValue = dropcapNone;

		return this->m_eValue;
	}

	std::wstring CDropCap::ToString  () const
	{
		switch(this->m_eValue)
		{
		case dropcapDrop   : return (L"drop");
		case dropcapMargin : return (L"margin");
		case dropcapNone   : return (L"none");
		default            : return (L"none");
		}
	}

	//--------------------------------------------------------------------------------
	// EdGrp 17.18.21 (Part 1)
	//--------------------------------------------------------------------------------

	EEdGrp CEdGrp::FromString(const std::wstring &sValue)
	{
		if      ( (L"administrators") == sValue ) this->m_eValue = edgrpAdministrators;
		else if ( (L"contributors")   == sValue ) this->m_eValue = edgrpContributors;
		else if ( (L"current")        == sValue ) this->m_eValue = edgrpCurrent;
		else if ( (L"editors")        == sValue ) this->m_eValue = edgrpEditors;
		else if ( (L"everyone")       == sValue ) this->m_eValue = edgrpEveryone;
		else if ( (L"none")           == sValue ) this->m_eValue = edgrpNone;
		else if ( (L"owners")         == sValue ) this->m_eValue = edgrpOwners;
		else                                       this->m_eValue = edgrpEveryone;

		return this->m_eValue;
	}

	std::wstring CEdGrp::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// EdnPos 17.18.22 (Part 1)
	//--------------------------------------------------------------------------------

	EEdnPos CEdnPos::FromString(const std::wstring &sValue)
	{
		if      ( (L"docEnd")  == sValue ) this->m_eValue = ednposDocEnd;
		else if ( (L"sectEnd") == sValue ) this->m_eValue = ednposSectEnd;
		else                                this->m_eValue = ednposSectEnd;

		return this->m_eValue;
	}

	std::wstring CEdnPos::ToString  () const
	{
		switch(this->m_eValue)
		{
		case ednposDocEnd  : return (L"docEnd");
		case ednposSectEnd : return (L"sectEnd");
		default            : return (L"sectEnd");
		}
	}

	//--------------------------------------------------------------------------------
	// EighthPointMeasure 17.18.23 (Part 1)
	//--------------------------------------------------------------------------------

	int CEighthPointMeasure::FromString(const std::wstring &sValue)
	{
		if (sValue == (L"auto"))
		{

		}
		else
		{
			this->m_eValue = sValue.empty() ? 0 : XmlUtils::GetInteger(sValue);
		}

		return this->m_eValue;
	}

	std::wstring CEighthPointMeasure::ToString  () const
	{
		std::wstring sResult = std::to_wstring( this->m_eValue);

		return sResult;
	}

	double CEighthPointMeasure::ToPoints()
	{
		return this->m_eValue / 8.0;
	}

	double CEighthPointMeasure::ToMM()
	{
		return Pt_To_Mm(this->m_eValue / 8.0);
	}

	double CEighthPointMeasure::ToInches()
	{
		return this->m_eValue / 576.0;
	}

	int CEighthPointMeasure::FromPoints(double dValue)
	{
		this->m_eValue = (int)(dValue * 8);
		return this->m_eValue;
	}

	int CEighthPointMeasure::FromInches(double dValue)
	{
		this->m_eValue = (int)(dValue * 576);
		return this->m_eValue;
	}

	//--------------------------------------------------------------------------------
	// Em 17.18.24 (Part 1)
	//--------------------------------------------------------------------------------

	EEm CEm::FromString(const std::wstring &sValue)
	{
		if      ( (L"circle")   == sValue ) this->m_eValue = emCircle;
		else if ( (L"comma")    == sValue ) this->m_eValue = emComma;
		else if ( (L"dot")      == sValue ) this->m_eValue = emDot;
		else if ( (L"none")     == sValue ) this->m_eValue = emNone;
		else if ( (L"underDot") == sValue ) this->m_eValue = emUnderDot;
		else                                 this->m_eValue = emNone;

		return this->m_eValue;
	}

	std::wstring CEm::ToString  () const
	{
		switch(this->m_eValue)
		{
		case emCircle   : return (L"circle");
		case emComma    : return (L"comma");
		case emDot      : return (L"dot");
		case emNone     : return (L"none");
		case emUnderDot : return (L"underDot");
		default         : return (L"none");
		}
	}

	//--------------------------------------------------------------------------------
	// FFHelpTextVal 17.18.25 (Part 1)
	//--------------------------------------------------------------------------------

	CFFHelpTextVal::CFFHelpTextVal() {}

	std::wstring CFFHelpTextVal::GetValue() const
	{
		return m_sValue;
	}

	void CFFHelpTextVal::SetValue(std::wstring &sValue)
	{
		if ( sValue.length() <= 256 )
			m_sValue = sValue;
		else
			m_sValue = sValue.substr( 0, 256 );
	}

	std::wstring CFFHelpTextVal::FromString(const std::wstring &sValue)
	{
		if ( sValue.length() <= 256 )
			m_sValue = sValue;
		else
			m_sValue = sValue.substr( 0, 256 );

		return m_sValue;
	}

	std::wstring CFFHelpTextVal::ToString  () const
	{
		return m_sValue;
	}
	std::wstring CFFHelpTextVal::ToXmlString() const
	{
		return XmlUtils::EncodeXmlString(m_sValue);
	}

	//--------------------------------------------------------------------------------
	// FFName 17.18.26 (Part 1)
	//--------------------------------------------------------------------------------

	CFFName::CFFName() {}

	std::wstring CFFName::GetValue() const
	{
		return m_sValue;
	}

	void CFFName::SetValue(std::wstring &sValue)
	{
		if ( sValue.length() <= 65 )
			m_sValue = sValue;
		else
			m_sValue = sValue.substr( 0, 65 );
	}

	std::wstring CFFName::FromString(const std::wstring &sValue)
	{
		if ( sValue.length() <= 65 )
			m_sValue = sValue;
		else
			m_sValue = sValue.substr( 0, 65 );

		return m_sValue;
	}

	std::wstring CFFName::ToString  () const
	{
		return m_sValue;
	}
	std::wstring CFFName::ToXmlString() const
	{
		return XmlUtils::EncodeXmlString(m_sValue);
	}

	//--------------------------------------------------------------------------------
	// FFStatusTextVal 17.18.27 (Part 1)
	//--------------------------------------------------------------------------------

	CFFStatusTextVal::CFFStatusTextVal() {}

	std::wstring CFFStatusTextVal::GetValue() const
	{
		return m_sValue;
	}

	void CFFStatusTextVal::SetValue(std::wstring &sValue)
	{
		if ( sValue.length() <= 140 )
			m_sValue = sValue;
		else
			m_sValue = sValue.substr( 0, 140 );
	}

	std::wstring CFFStatusTextVal::FromString(const std::wstring &sValue)
	{
		if ( sValue.length() <= 140 )
			m_sValue = sValue;
		else
			m_sValue = sValue.substr( 0, 140 );

		return m_sValue;
	}

	std::wstring CFFStatusTextVal::ToString () const
	{
		return m_sValue;
	}
	std::wstring CFFStatusTextVal::ToXmlString() const
	{
		return XmlUtils::EncodeXmlString(m_sValue);
	}

	//--------------------------------------------------------------------------------
	// FFTextType 17.18.28 (Part 1)
	//--------------------------------------------------------------------------------

	EFFTextType CFFTextType::FromString(const std::wstring &sValue)
	{
		if      ( (L"calculated")  == sValue ) this->m_eValue = fftexttypeCalculated;
		else if ( (L"currentDate") == sValue ) this->m_eValue = fftexttypeCurrentDate;
		else if ( (L"currentTime") == sValue ) this->m_eValue = fftexttypeCurrentTime;
		else if ( (L"date")        == sValue ) this->m_eValue = fftexttypeDate;
		else if ( (L"number")      == sValue ) this->m_eValue = fftexttypeNumber;
		else if ( (L"regular")     == sValue ) this->m_eValue = fftexttypeRegular;
		else                                    this->m_eValue = fftexttypeRegular;

		return this->m_eValue;
	}

	std::wstring CFFTextType::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// FldCharType 17.18.29 (Part 1)
	//--------------------------------------------------------------------------------

	EFldCharType CFldCharType::FromString(const std::wstring &sValue)
	{
		if      ( (L"begin")    == sValue ) this->m_eValue = fldchartypeBegin;
		else if ( (L"end")      == sValue ) this->m_eValue = fldchartypeEnd;
		else if ( (L"separate") == sValue ) this->m_eValue = fldchartypeSeparate;
		else                                 this->m_eValue = fldchartypeBegin;

		return this->m_eValue;
	}

	std::wstring CFldCharType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case fldchartypeBegin    : return (L"begin");
		case fldchartypeEnd      : return (L"end");
		case fldchartypeSeparate : return (L"separate");
		default                  : return (L"begin");
		}
	}

	//--------------------------------------------------------------------------------
	// FontFamily 17.18.30 (Part 1)
	//--------------------------------------------------------------------------------

	EFontFamily CFontFamily::FromString(const std::wstring &sValue)
	{
		if      ( (L"auto")       == sValue ) this->m_eValue = fontfamilyAuto;
		else if ( (L"decorative") == sValue ) this->m_eValue = fontfamilyDecorative;
		else if ( (L"modern")     == sValue ) this->m_eValue = fontfamilyModern;
		else if ( (L"roman")      == sValue ) this->m_eValue = fontfamilyRoman;
		else if ( (L"script")     == sValue ) this->m_eValue = fontfamilyScript;
		else if ( (L"swiss")      == sValue ) this->m_eValue = fontfamilySwiss;
		else                                   this->m_eValue = fontfamilyAuto;

		return this->m_eValue;
	}

	std::wstring CFontFamily::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// FrameLayout 17.18.31 (Part 1)
	//--------------------------------------------------------------------------------

	EFrameLayout CFrameLayout::FromString(const std::wstring &sValue)
	{
		if      ( (L"cols") == sValue ) this->m_eValue = framelayoutCols;
		else if ( (L"none") == sValue ) this->m_eValue = framelayoutNone;
		else if ( (L"rows") == sValue ) this->m_eValue = framelayoutRows;
		else                             this->m_eValue = framelayoutNone;

		return this->m_eValue;
	}

	std::wstring CFrameLayout::ToString  () const
	{
		switch(this->m_eValue)
		{
		case framelayoutCols : return (L"cols");
		case framelayoutNone : return (L"none");
		case framelayoutRows : return (L"rows");
		default              : return (L"none");
		}
	}

	//--------------------------------------------------------------------------------
	// FrameScrollbar 17.18.32 (Part 1)
	//--------------------------------------------------------------------------------

	EFrameScrollbar CFrameScrollbar::FromString(const std::wstring &sValue)
	{
		if      ( (L"auto") == sValue ) this->m_eValue = framescrollbarAuto;
		else if ( (L"off")  == sValue ) this->m_eValue = framescrollbarOff;
		else if ( (L"on")   == sValue ) this->m_eValue = framescrollbarOn;
		else                             this->m_eValue = framescrollbarAuto;

		return this->m_eValue;
	}

	std::wstring CFrameScrollbar::ToString  () const
	{
		switch(this->m_eValue)
		{
		case framescrollbarAuto : return (L"auto");
		case framescrollbarOff  : return (L"off");
		case framescrollbarOn   : return (L"on");
		default                 : return (L"auto");
		}
	}

	//--------------------------------------------------------------------------------
	// FtnEdn 17.18.33 (Part 1)
	//--------------------------------------------------------------------------------

	EFtnEdn CFtnEdn::FromString(const std::wstring &sValue)
	{
		if      (L"continuationNotice"    == sValue ) this->m_eValue = ftnednContinuationNotice;
		else if (L"continuationSeparator" == sValue ||
				 L"continuation-separator" == sValue) this->m_eValue = ftnednContinuationSeparator;
		else if (L"normal"                == sValue ) this->m_eValue = ftnednNormal;
		else if (L"separator"             == sValue ) this->m_eValue = ftnednSeparator;
		else                                          this->m_eValue = ftnednNormal;

		return this->m_eValue;
	}

	std::wstring CFtnEdn::ToString  () const
	{
		switch(this->m_eValue)
		{
		case ftnednContinuationNotice    : return (L"continuationNotice");
		case ftnednContinuationSeparator : return (L"continuationSeparator");
		case ftnednNormal                : return (L"normal");
		case ftnednSeparator             : return (L"separator");
		default                          : return (L"normal");
		}
	}

	//--------------------------------------------------------------------------------
	// FtnPos 17.18.34 (Part 1)
	//--------------------------------------------------------------------------------

	EFtnPos CFtnPos::FromString(const std::wstring &sValue)
	{
		if      ( (L"beneathText") == sValue ) this->m_eValue = ftnposBeneathText;
		else if ( (L"docEnd")      == sValue ) this->m_eValue = ftnposDocEnd;
		else if ( (L"pageBottom")  == sValue ) this->m_eValue = ftnposPageBottom;
		else if ( (L"sectEnd")     == sValue ) this->m_eValue = ftnposSectEnd;
		else                                    this->m_eValue = ftnposSectEnd;

		return this->m_eValue;
	}

	std::wstring CFtnPos::ToString  () const
	{
		switch(this->m_eValue)
		{
		case ftnposBeneathText : return (L"beneathText");
		case ftnposDocEnd      : return (L"docEnd");
		case ftnposPageBottom  : return (L"pageBottom");
		case ftnposSectEnd     : return (L"sectEnd");
		default                : return (L"sectEnd");
		}
	}

	//--------------------------------------------------------------------------------
	// HAnchor 17.18.35 (Part 1)
	//--------------------------------------------------------------------------------

	EHAnchor CHAnchor::FromString(const std::wstring &sValue)
	{
		if      ( (L"margin") == sValue ) this->m_eValue = hanchorMargin;
		else if ( (L"page")   == sValue ) this->m_eValue = hanchorPage;
		else if ( (L"text")   == sValue ) this->m_eValue = hanchorText;
		else                               this->m_eValue = hanchorText;

		return this->m_eValue;
	}

	std::wstring CHAnchor::ToString  () const
	{
		switch(this->m_eValue)
		{
		case hanchorMargin : return (L"margin");
		case hanchorPage   : return (L"page");
		case hanchorText   : return (L"text");
		default            : return (L"text");
		}
	}

	//--------------------------------------------------------------------------------
	// HdrFtr 17.18.36 (Part 1)
	//--------------------------------------------------------------------------------

	EHdrFtr CHdrFtr::FromString(const std::wstring &sValue)
	{
		if      ( L"default" == sValue ||
				  L"odd"	 == sValue ) this->m_eValue = hdrftrDefault;
		else if ( L"even"    == sValue ) this->m_eValue = hdrftrEven;
		else if ( L"first"   == sValue ) this->m_eValue = hdrftrFirst;
		else							 this->m_eValue = hdrftrDefault;

		return this->m_eValue;
	}

	std::wstring CHdrFtr::ToString() const
	{
		switch(this->m_eValue)
		{
		case hdrftrDefault : return L"default";
		case hdrftrEven    : return L"even";
		case hdrftrFirst   : return L"first";
		default            : return L"default";
		}
	}

	//--------------------------------------------------------------------------------
	// HeightRule 17.18.37 (Part 1)
	//--------------------------------------------------------------------------------

	EHeightRule CHeightRule::FromString(const std::wstring &sValue)
	{
		if      ( (L"atLeast") == sValue ) this->m_eValue = heightruleAtLeast;
		else if ( (L"auto")    == sValue ) this->m_eValue = heightruleAuto;
		else if ( (L"exact")   == sValue ) this->m_eValue = heightruleExact;
		else                                this->m_eValue = heightruleAuto;

		return this->m_eValue;
	}

	std::wstring CHeightRule::ToString  () const
	{
		switch(this->m_eValue)
		{
		case heightruleAtLeast : return (L"atLeast");
		case heightruleAuto    : return (L"auto");
		case heightruleExact   : return (L"exact");
		default                : return (L"auto");
		}
	}

	//--------------------------------------------------------------------------------
	// TextFormFormatType
	//--------------------------------------------------------------------------------

	ETextFormFormatType CTextFormFormatType::FromString(const std::wstring &sValue)
	{
		if (L"none" == sValue) this->m_eValue = textFormFormatTypeNone;
		else if (L"digit" == sValue) this->m_eValue = textFormFormatTypeDigit;
		else if (L"letter" == sValue) this->m_eValue = textFormFormatTypeLetter;
		else if (L"mask" == sValue) this->m_eValue = textFormFormatTypeMask;
		else if (L"regExp" == sValue) this->m_eValue = textFormFormatTypeRegExp;
		else this->m_eValue = textFormFormatTypeNone;

		return this->m_eValue;
	}

	std::wstring CTextFormFormatType::ToString() const
	{
		switch (this->m_eValue)
		{
		case textFormFormatTypeNone: return L"none";
		case textFormFormatTypeDigit: return L"digit";
		case textFormFormatTypeLetter: return L"letter";
		case textFormFormatTypeMask: return L"mask";
		case textFormFormatTypeRegExp: return L"regExp";
		default: return (L"none");
		}
	}

	EComplexFormType CComplexFormType::FromString(const std::wstring &sValue)
	{
		if (L"custom" == sValue || L"none" == sValue) this->m_eValue = complexFormTypeCustom;
		else if (L"telephone" == sValue || L"phone" == sValue) this->m_eValue = complexFormTypeTelephone;
		else if (L"email" == sValue) this->m_eValue = complexFormTypeEmail;
		else this->m_eValue = complexFormTypeCustom;

		return this->m_eValue;
	}

	std::wstring CComplexFormType::ToString() const
	{
		switch (this->m_eValue)
		{
		case complexFormTypeCustom: return L"custom";
		case complexFormTypeTelephone: return L"telephone";
		case complexFormTypeEmail: return L"email";
		default: return (L"custom");
		}
	}

	bool CComplexFormType::IsDefaultValue() const
	{
		return (this->m_eValue == complexFormTypeCustom);
	}

	///

	CHexColor::CHexColor(unsigned char r, unsigned char g, unsigned char b)
	{
		this->m_eValue = hexcolorRGB;
		m_unR = r;
		m_unG = g;
		m_unB = b;
	}

	int	CHexColor::HexToInt(int nHex)
	{
		if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
		if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
		if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

		return 0;
	}

	void CHexColor::Parse()
	{
		if ( m_sValue.length() < 6 )
			return;

		m_unR = HexToInt( (int)m_sValue[1] ) + (unsigned char)(HexToInt( (int)m_sValue[0] ) << 4);
		m_unG = HexToInt( (int)m_sValue[3] ) + (unsigned char)(HexToInt( (int)m_sValue[2] ) << 4);
		m_unB = HexToInt( (int)m_sValue[5] ) + (unsigned char)(HexToInt( (int)m_sValue[4] ) << 4);
	}

	void CHexColor::Parse3()
	{
		if ( m_sValue.length() < 3 )
			return;

		m_unR = HexToInt( (int)m_sValue[0] ) + (unsigned char)(HexToInt( (int)m_sValue[0]) << 4);
		m_unG = HexToInt( (int)m_sValue[1] ) + (unsigned char)(HexToInt( (int)m_sValue[1]) << 4);
		m_unB = HexToInt( (int)m_sValue[2] ) + (unsigned char)(HexToInt( (int)m_sValue[2]) << 4);
	}

	EHexColor CHexColor::FromString(const std::wstring &sValueSrc)
	{
		if ( L"auto" == sValueSrc || L"none" == sValueSrc )
			this->m_eValue = hexcolorAuto;
		else
		{
			std::wstring sValue = sValueSrc;
			size_t split = sValue.find(L" ["); //index ala #ff9 [43]
			if (std::wstring::npos != split)
			{
				sValue = sValue.substr(0, split);
			}
			size_t s0 = sValue.find(L"#"); // ala #ff9
			if (0 == s0)
			{
				sValue = sValue.substr(1);
			}

			//В документации не написано, что цвет может приходить строкой, но в реальных документах встречается и word это разруливает.
			CPresetColorVal oPresetColorVal;
			if(oPresetColorVal.FromStringIgnoreCase(sValue))
			{
				this->m_eValue = hexcolorRGB;
				m_unR = oPresetColorVal.Get_R();
				m_unG = oPresetColorVal.Get_G();
				m_unB = oPresetColorVal.Get_B();
			}
			else if ( 6 <= sValue.length() )
			{
				this->m_eValue = hexcolorRGB;
				m_sValue = sValue.substr( 0, 6 );
				Parse();
			}
			else if ( 3 == sValue.length() )// a la #339 (Compo 3AP.docx)
			{
				this->m_eValue = hexcolorRGB;
				m_sValue = sValue;
				Parse3();
			}
			else   this->m_eValue = EHexColor::hexcolorAuto;//eDefValue;

		}

		return this->m_eValue;
	}

	/*
	template<>
	EHexColor CHexColor<EHexColor::hexcolorRGB>::FromString(const std::wstring &sValue)
	{
		if ( _T("auto") == sValue || _T("none") == sValue )
			this->m_eValue = hexcolorAuto;
		else
		{
			//В документации не написано, что цвет может приходить строкой, но в реальных докуентах встречается и word это разруливает.
			//CHighlightColor<highlightcolorNone> oHighlightColor(sValue);
			CPresetColorVal<> oPresetColorVal;
			if(oPresetColorVal.FromStringIgnoreCase(sValue))
			{
				this->m_eValue = hexcolorRGB;
				m_unR = oPresetColorVal.Get_R();
				m_unG = oPresetColorVal.Get_G();
				m_unB = oPresetColorVal.Get_B();
			}
			else if ( 6 <= sValue.length() )
			{
				this->m_eValue = hexcolorRGB;
				m_sValue = sValue.substr( 0, 6 );
				Parse();
			}
			else if ( 3 == sValue.length() )// a la #339 (Compo 3AP.docx)
			{
				this->m_eValue = hexcolorRGB;
				m_sValue = sValue;
				Parse3();
			}
			else   this->m_eValue = EHexColor::hexcolorAuto;//eDefValue;

		}

		return this->m_eValue;
	}
	*/

	std::wstring CHexColor::ToString  () const
	{
		switch(this->m_eValue)
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

	std::wstring CHexColor::ToStringNoAlpha  () const
	{
		switch(this->m_eValue)
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

	void CHexColor::Set_R(unsigned char R)
	{
		m_unR = R;
	}

	void CHexColor::Set_G(unsigned char G)
	{
		m_unG = G;
	}

	void CHexColor::Set_B(unsigned char B)
	{
		m_unB = B;
	}

	unsigned char CHexColor::Get_R() const
	{
		return m_unR;
	}

	unsigned char CHexColor::Get_G() const
	{
		return m_unG;
	}

	unsigned char CHexColor::Get_B() const
	{
		return m_unB;
	}

	unsigned char CHexColor::Get_A() const
	{
		return 255;
	}

	//--------------------------------------------------------------------------------
	// HexColorAuto 17.18.39 (Part 1)
	//--------------------------------------------------------------------------------

	EHexColorAuto CHexColorAuto::FromString(const std::wstring &sValue)
	{
		this->m_eValue = hexcolorautoAuto;

		return this->m_eValue;
	}

	std::wstring CHexColorAuto::ToString  () const
	{
		return (L"auto");
	}

	//--------------------------------------------------------------------------------
	// HighlightColor 17.18.40 (Part 1)
	//--------------------------------------------------------------------------------

	CHighlightColor::CHighlightColor(const CHexColor& color)
		: m_unR(color.Get_R()), m_unG(color.Get_G()), m_unB(color.Get_B()), m_unA(255) {}

	std::wstring CHighlightColor::ToString() const
	{
		switch(this->m_eValue)
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

	EHighlightColor CHighlightColor::FromString(const std::wstring &sValue)
	{
		if      ( (L"black")       == sValue )
		{
			this->m_eValue = highlightcolorBlack;

			m_unR = 0x00;
			m_unG = 0x00;
			m_unB = 0x00;
			m_unA = 0xFF;
		}
		else if ( (L"blue")        == sValue )
		{
			this->m_eValue = highlightcolorBlue;
			m_unR = 0x00;
			m_unG = 0x00;
			m_unB = 0xFF;
			m_unA = 0xFF;
		}
		else if ( (L"cyan")        == sValue )
		{
			this->m_eValue = highlightcolorCyan;
			m_unR = 0x00;
			m_unG = 0xFF;
			m_unB = 0xFF;
			m_unA = 0xFF;
		}
		else if ( (L"darkBlue")    == sValue )
		{
			this->m_eValue = highlightcolorDarkBlue;
			m_unR = 0x00;
			m_unG = 0x00;
			m_unB = 0x8B;
			m_unA = 0xFF;
		}
		else if ( (L"darkCyan")    == sValue )
		{
			this->m_eValue = highlightcolorDarkCyan;
			m_unR = 0x00;
			m_unG = 0x8B;
			m_unB = 0x8B;
			m_unA = 0xFF;
		}
		else if ( (L"darkGray")    == sValue )
		{
			this->m_eValue = highlightcolorDarkGray;
			m_unR = 0xA9;
			m_unG = 0xA9;
			m_unB = 0xA9;
			m_unA = 0xFF;
		}
		else if ( (L"darkGreen")   == sValue )
		{
			this->m_eValue = highlightcolorDarkGreen;
			m_unR = 0x00;
			m_unG = 0x64;
			m_unB = 0x00;
			m_unA = 0xFF;
		}
		else if ( (L"darkMagenta") == sValue )
		{
			this->m_eValue = highlightcolorDarkMagenta;
			m_unR = 0x80;
			m_unG = 0x00;
			m_unB = 0x80;
			m_unA = 0xFF;
		}
		else if ( (L"darkRed")     == sValue )
		{
			this->m_eValue = highlightcolorDarkRed;
			m_unR = 0x8B;
			m_unG = 0x00;
			m_unB = 0x00;
			m_unA = 0xFF;
		}
		else if ( (L"darkYellow")  == sValue )
		{
			this->m_eValue = highlightcolorDarkYellow;
			m_unR = 0x80;
			m_unG = 0x80;
			m_unB = 0x00;
			m_unA = 0xFF;
		}
		else if ( (L"green")       == sValue )
		{
			this->m_eValue = highlightcolorGreen;
			m_unR = 0x00;
			m_unG = 0xFF;
			m_unB = 0x00;
			m_unA = 0xFF;
		}
		else if ( (L"lightGray")   == sValue )
		{
			this->m_eValue = highlightcolorLightGray;
			m_unR = 0xD3;
			m_unG = 0xD3;
			m_unB = 0xD3;
			m_unA = 0xFF;
		}
		else if ( (L"magenta")     == sValue )
		{
			this->m_eValue = highlightcolorMagenta;
			m_unR = 0xFF;
			m_unG = 0x00;
			m_unB = 0xFF;
			m_unA = 0xFF;
		}
		else if ( (L"none")        == sValue )
		{
			this->m_eValue = highlightcolorNone;
			m_unR = 0x00;
			m_unG = 0x00;
			m_unB = 0x00;
			m_unA = 0x00;
		}
		else if ( (L"red")         == sValue )
		{
			this->m_eValue = highlightcolorRed;
			m_unR = 0xFF;
			m_unG = 0x00;
			m_unB = 0x00;
			m_unA = 0xFF;
		}
		else if ( (L"white")       == sValue )
		{
			this->m_eValue = highlightcolorWhite;
			m_unR = 0xFF;
			m_unG = 0xFF;
			m_unB = 0xFF;
			m_unA = 0xFF;
		}
		else if ( (L"yellow")      == sValue )
		{
			this->m_eValue = highlightcolorYellow;
			m_unR = 0xFF;
			m_unG = 0xFF;
			m_unB = 0x00;
			m_unA = 0xFF;
		}
		else
		{
			this->m_eValue = highlightcolorNone;

			// Инициализируем цвет
			FromString( ToString() );
		}

		return this->m_eValue;
	}

	unsigned char CHighlightColor::Get_R() const
	{
		return m_unR;
	}

	unsigned char CHighlightColor::Get_G() const
	{
		return m_unG;
	}

	unsigned char CHighlightColor::Get_B() const
	{
		return m_unB;
	}

	unsigned char CHighlightColor::Get_A() const
	{
		return m_unA;
	}

	//--------------------------------------------------------------------------------
	// Hint 17.18.41 (Part 1)
	//--------------------------------------------------------------------------------

	EHint CHint::FromString(const std::wstring &sValue)
	{
		if      ( (L"cs")       == sValue ) this->m_eValue = hintCs;
		else if ( (L"default")  == sValue ) this->m_eValue = hintDefault;
		else if ( (L"eastAsia") == sValue ) this->m_eValue = hintEastAsia;
		else                                this->m_eValue = hintDefault;

		return this->m_eValue;
	}

	std::wstring CHint::ToString  () const
	{
		switch(this->m_eValue)
		{
		case hintCs       : return (L"cs");
		case hintDefault  : return (L"default");
		case hintEastAsia : return (L"eastAsia");
		default           : return (L"default");
		}
	}

	//--------------------------------------------------------------------------------
	// HpsMeasure 17.18.42 (Part 1)
	//--------------------------------------------------------------------------------

	CHpsMeasure::CHpsMeasure() {}

	double  CHpsMeasure::FromString(const std::wstring &sValue)
	{
		Parse(sValue, 2);

		m_dValue = fabs( m_dValue );

		return m_dValue;
	}
	void CHpsMeasure::SetValue(double dValue)
	{
		m_bUnit = false;
		m_dValue = dValue;
	}
	std::wstring CHpsMeasure::ToString  () const
	{
		std::wstring sResult;

		if (m_bUnit)
		{
			sResult = boost::lexical_cast<std::wstring>( m_dValue ) + L"pt";
		}
		else
		{
			sResult = std::to_wstring( (int)(m_dValue * 2) );
		}

		return sResult;
	}
	long CHpsMeasure::ToHps() const
	{
		return (long)(2 * m_dValue);
	}

	double CHpsMeasure::FromPoints(double dValue)
	{
		m_dValue = fabs( dValue );
		return m_dValue;
	}
	double CHpsMeasure::FromInches(double dValue)
	{
		m_dValue = fabs( dValue * 72.0 );
		return m_dValue;
	}
	double CHpsMeasure::FromHps(double dValue)
	{
		m_dValue = dValue / 2;
		return m_dValue;
	}

	//--------------------------------------------------------------------------------
	// InfoTextType 17.18.43 (Part 1)
	//--------------------------------------------------------------------------------

	EInfoTextType CInfoTextType::FromString(const std::wstring &sValue)
	{
		if      ( (L"autoText") == sValue ) this->m_eValue = infotexttypeAutoText;
		else if ( (L"text")     == sValue ) this->m_eValue = infotexttypeText;
		else                                 this->m_eValue = infotexttypeText;

		return this->m_eValue;
	}

	std::wstring CInfoTextType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case infotexttypeAutoText : return (L"autoText");
		case infotexttypeText     : return (L"text");
		default                   : return (L"text");
		}
	}

	//--------------------------------------------------------------------------------
	// Jc 17.18.44 (Part 1) + 9.10.2 (Part 4)
	//--------------------------------------------------------------------------------

	EJc CJc::FromString(const std::wstring &sValue)
	{
		if      ( (L"both")           == sValue ) this->m_eValue = jcBoth;
		else if ( (L"center")         == sValue ) this->m_eValue = jcCenter;
		else if ( (L"distribute")     == sValue ) this->m_eValue = jcDistribute;
		else if ( (L"end")            == sValue ) this->m_eValue = jcEnd;
		else if ( (L"highKashida")    == sValue ) this->m_eValue = jcHighKashida;
		else if ( (L"lowKashida")     == sValue ) this->m_eValue = jcLowKashida;
		else if ( (L"mediumKashida")  == sValue ) this->m_eValue = jcMediumKashida;
		else if ( (L"numTab")         == sValue ) this->m_eValue = jcNumTab;
		else if ( (L"start")          == sValue ) this->m_eValue = jcStart;
		else if ( (L"thaiDistribute") == sValue ) this->m_eValue = jcThaiDistribute;
		else if ( (L"left")           == sValue ) this->m_eValue = jcLeft;
		else if ( (L"right")          == sValue ) this->m_eValue = jcRight;
		else                                       this->m_eValue = jcLeft;

		return this->m_eValue;
	}

	std::wstring CJc::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// JcTable 17.18.45 (Part 1) + 9.10.3 (Part 4)
	//--------------------------------------------------------------------------------

	EJcTable CJcTable::FromString(const std::wstring &sValue)
	{
		if      ( (L"center")         == sValue ) this->m_eValue = jctableCenter;
		else if ( (L"end")            == sValue ) this->m_eValue = jctableEnd;
		else if ( (L"start")          == sValue ) this->m_eValue = jctableStart;
		else if ( (L"left")           == sValue ) this->m_eValue = jctableLeft;
		else if ( (L"right")          == sValue ) this->m_eValue = jctableRight;
		else                                       this->m_eValue = jctableLeft;

		return this->m_eValue;
	}

	std::wstring CJcTable::ToString  () const
	{
		switch(this->m_eValue)
		{
		case jctableCenter : return (L"center");
		case jctableEnd    : return (L"end");
		case jctableStart  : return (L"start");
		case jctableLeft   : return (L"left");
		case jctableRight  : return (L"right");
		default            : return (L"left");
		}
	}

	//--------------------------------------------------------------------------------
	// LevelSuffix 17.18.46 (Part 1)
	//--------------------------------------------------------------------------------

	ELevelSuffix CLevelSuffix::FromString(const std::wstring &sValue)
	{
		if      ( (L"nothing") == sValue ) this->m_eValue = levelsuffixNothing;
		else if ( (L"space")   == sValue ) this->m_eValue = levelsuffixSpace;
		else if ( (L"tab")     == sValue ) this->m_eValue = levelsuffixTab;
		else                                this->m_eValue = levelsuffixNothing;

		return this->m_eValue;
	}

	std::wstring CLevelSuffix::ToString  () const
	{
		switch(this->m_eValue)
		{
		case levelsuffixNothing : return (L"nothing");
		case levelsuffixSpace   : return (L"space");
		case levelsuffixTab     : return (L"tab");
		default                 : return (L"nothing");
		}
	}

	//--------------------------------------------------------------------------------
	// LineNumberRestart 17.18.47 (Part 1)
	//--------------------------------------------------------------------------------

	ELineNumberRestart CLineNumberRestart::FromString(const std::wstring &sValue)
	{
		if      ( (L"continuous") == sValue ) this->m_eValue = linenumberrestartContinious;
		else if ( (L"newPage")    == sValue ) this->m_eValue = linenumberrestartNewPage;
		else if ( (L"newSection") == sValue ) this->m_eValue = linenumberrestartNewSection;
		else                                   this->m_eValue = linenumberrestartNewPage;

		return this->m_eValue;
	}

	std::wstring CLineNumberRestart::ToString  () const
	{
		switch(this->m_eValue)
		{
		case linenumberrestartContinious : return (L"continuous");
		case linenumberrestartNewPage    : return (L"newPage");
		case linenumberrestartNewSection : return (L"newSection");
		default                          : return (L"newPage");
		}
	}

	//--------------------------------------------------------------------------------
	// LineSpacingRule 17.18.48 (Part 1)
	//--------------------------------------------------------------------------------

	ELineSpacingRule CLineSpacingRule::FromString(const std::wstring &sValue)
	{
		if      ( (L"atLeast") == sValue ) this->m_eValue = linespacingruleAtLeast;
		else if ( (L"auto")    == sValue ) this->m_eValue = linespacingruleAuto;
		else if ( (L"exact")   == sValue ) this->m_eValue = linespacingruleExact;
		else                                this->m_eValue = linespacingruleAuto;

		return this->m_eValue;
	}

	std::wstring CLineSpacingRule::ToString  () const
	{
		switch(this->m_eValue)
		{
		case linespacingruleAtLeast : return (L"atLeast");
		case linespacingruleAuto    : return (L"auto");
		case linespacingruleExact   : return (L"exact");
		default                     : return (L"auto");
		}
	}

	//--------------------------------------------------------------------------------
	// Lock 17.18.49 (Part 1)
	//--------------------------------------------------------------------------------

	ELock CLock::FromString(const std::wstring &sValue)
	{
		if      ( (L"contentLocked")    == sValue ) this->m_eValue = lockContentLocked;
		else if ( (L"sdtContentLocked") == sValue ) this->m_eValue = lockSdtContentLocked;
		else if ( (L"sdtLocked")        == sValue ) this->m_eValue = lockSdtLocked;
		else if ( (L"unlocked")         == sValue ) this->m_eValue = lockUnlocked;
		else                                         this->m_eValue = lockUnlocked;

		return this->m_eValue;
	}

	std::wstring CLock::ToString  () const
	{
		switch(this->m_eValue)
		{
		case lockContentLocked    : return (L"contentLocked");
		case lockSdtContentLocked : return (L"sdtContentLocked");
		case lockSdtLocked        : return (L"sdtLocked");
		case lockUnlocked         : return (L"unlocked");
		default                   : return (L"unlocked");
		}
	}

	//--------------------------------------------------------------------------------
	// LongHexNumber 17.18.50 (Part 1)
	//--------------------------------------------------------------------------------

	int	CLongHexNumber::HexToInt(int nHex, bool &bResult)
	{
		if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
		if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
		if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

		bResult = false;

		return 0;
	}

	bool CLongHexNumber::Parse(const std::wstring &sValue)
	{
		if ( sValue.length() < 8 )
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

	unsigned int CLongHexNumber::FromString(const std::wstring &sValue)
	{
		if ( !Parse(sValue) )
		{
			this->m_eValue = 0;
		}

		return this->m_eValue;
	}

	std::wstring CLongHexNumber::ToString  () const
	{
		return XmlUtils::ToString(this->m_eValue, L"%08X");
	}

	//--------------------------------------------------------------------------------
	// MacroName 17.18.51 (Part 1)
	//--------------------------------------------------------------------------------

	CMacroName::CMacroName() {}

	std::wstring CMacroName::GetValue() const
	{
		return m_sValue;
	}

	void CMacroName::SetValue(std::wstring &sValue)
	{
		if ( sValue.length() <= 33 )
			m_sValue = sValue;
		else
			m_sValue = sValue.substr( 0, 33 );
	}

	std::wstring CMacroName::FromString(const std::wstring &sValue)
	{
		if ( sValue.length() <= 33 )
			m_sValue = sValue;
		else
			m_sValue = sValue.substr( 0, 33 );

		return m_sValue;
	}

	std::wstring CMacroName::ToString  () const
	{
		return m_sValue;
	}

	//--------------------------------------------------------------------------------
	// MailMergeDataType 17.18.52 (Part 1)
	//--------------------------------------------------------------------------------

	EMailMergeDataType CMailMergeDataType::FromString(const std::wstring &sValue)
	{
		if      ( (L"native")      == sValue ) this->m_eValue = mailmergeddatatypeNative;
		else if ( (L"odbc")        == sValue ) this->m_eValue = mailmergeddatatypeODBC;
		else if ( (L"query")       == sValue ) this->m_eValue = mailmergeddatatypeQuery;
		else if ( (L"soap")        == sValue ) this->m_eValue = mailmergeddatatypeSoap;
		else if ( (L"spreadsheet") == sValue ) this->m_eValue = mailmergeddatatypeSpreadsheet;
		else if ( (L"textFile")    == sValue ) this->m_eValue = mailmergeddatatypeTextFile;
		else if ( (L"xQuery")      == sValue ) this->m_eValue = mailmergeddatatypeXQuery;
		else if ( (L"xmlFile")     == sValue ) this->m_eValue = mailmergeddatatypeXmlFile;
		else                                   this->m_eValue = mailmergeddatatypeTextFile;

		return this->m_eValue;
	}

	std::wstring CMailMergeDataType::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// MailMergeDest 17.18.53 (Part 1)
	//--------------------------------------------------------------------------------

	EMailMergeDest CMailMergeDest::FromString(const std::wstring &sValue)
	{
		if      ( (L"email")       == sValue ) this->m_eValue = mailmergeddestEmail;
		else if ( (L"fax")         == sValue ) this->m_eValue = mailmergeddestFax;
		else if ( (L"newDocument") == sValue ) this->m_eValue = mailmergeddestNewDocument;
		else if ( (L"printer")     == sValue ) this->m_eValue = mailmergeddestPrinter;
		else                                    this->m_eValue = mailmergeddestEmail;

		return this->m_eValue;
	}

	std::wstring CMailMergeDest::ToString  () const
	{
		switch(this->m_eValue)
		{
		case mailmergeddestEmail       : return (L"email");
		case mailmergeddestFax         : return (L"fax");
		case mailmergeddestNewDocument : return (L"newDocument");
		case mailmergeddestPrinter     : return (L"printer");
		default                        : return (L"newDocument");
		}
	}

	//--------------------------------------------------------------------------------
	// MailMergeDocType 17.18.54 (Part 1)
	//--------------------------------------------------------------------------------

	EMailMergeDocType CMailMergeDocType::FromString(const std::wstring &sValue)
	{
		if      ( (L"catalog")       == sValue ) this->m_eValue = mailmergeddoctypeCatalog;
		else if ( (L"email")         == sValue ) this->m_eValue = mailmergeddoctypeEmail;
		else if ( (L"envelopes")     == sValue ) this->m_eValue = mailmergeddoctypeEnvelopes;
		else if ( (L"fax")           == sValue ) this->m_eValue = mailmergeddoctypeFax;
		else if ( (L"formLetters")   == sValue ) this->m_eValue = mailmergeddoctypeFormLetters;
		else if ( (L"mailingLabels") == sValue ) this->m_eValue = mailmergeddoctypeMailingLabels;
		else                                      this->m_eValue = mailmergeddoctypeEmail;

		return this->m_eValue;
	}

	std::wstring CMailMergeDocType::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// MailMergeOdsoFMDFieldType 17.18.55 (Part 1)
	//--------------------------------------------------------------------------------

	EMailMergeOdsoFMDFieldType CMailMergeOdsoFMDFieldType::FromString(const std::wstring &sValue)
	{
		if      ( (L"dbColumn") == sValue ) this->m_eValue = mailmergeodsofmdfieldtypeDbColumn;
		else if ( (L"null")     == sValue ) this->m_eValue = mailmergeodsofmdfieldtypeNull;
		else                                 this->m_eValue = mailmergeodsofmdfieldtypeNull;

		return this->m_eValue;
	}

	std::wstring CMailMergeOdsoFMDFieldType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case mailmergeodsofmdfieldtypeDbColumn : return (L"dbColumn");
		case mailmergeodsofmdfieldtypeNull     : return (L"null");
		default                                : return (L"null");
		}
	}

	//--------------------------------------------------------------------------------
	// MailMergeSourceType 17.18.56 (Part 1)
	//--------------------------------------------------------------------------------

	EMailMergeSourceType CMailMergeSourceType::FromString(const std::wstring &sValue)
	{
		if      ( (L"addressBook") == sValue ) this->m_eValue = mailmergesourcetypeAddressBook;
		else if ( (L"database")    == sValue ) this->m_eValue = mailmergesourcetypeDatabase;
		else if ( (L"document1")   == sValue ) this->m_eValue = mailmergesourcetypeDocument1;
		else if ( (L"document2")   == sValue ) this->m_eValue = mailmergesourcetypeDocument2;
		else if ( (L"email")       == sValue ) this->m_eValue = mailmergesourcetypeEmail;
		else if ( (L"legacy")      == sValue ) this->m_eValue = mailmergesourcetypeLegacy;
		else if ( (L"master")      == sValue ) this->m_eValue = mailmergesourcetypeMaster;
		else if ( (L"native")      == sValue ) this->m_eValue = mailmergesourcetypeNative;
		else if ( (L"text")        == sValue ) this->m_eValue = mailmergesourcetypeText;
		else                                    this->m_eValue = mailmergesourcetypeText;

		return this->m_eValue;
	}

	std::wstring CMailMergeSourceType::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// Merge 17.18.57 (Part 1)
	//--------------------------------------------------------------------------------

	EMerge CMerge::FromString(const std::wstring &sValue)
	{
		if      ( (L"continue") == sValue ) this->m_eValue = mergeContinue;
		else if ( (L"restart")  == sValue ) this->m_eValue = mergeRestart;
		else                                 this->m_eValue = mergeContinue;

		return this->m_eValue;
	}

	std::wstring CMerge::ToString  () const
	{
		switch(this->m_eValue)
		{
		case mergeContinue : return (L"continue");
		case mergeRestart  : return (L"restart");
		default            : return (L"restart");
		}
	}

	//--------------------------------------------------------------------------------
	// MultiLevelType 17.18.58 (Part 1)
	//--------------------------------------------------------------------------------

	EMultiLevelType CMultiLevelType::FromString(const std::wstring &sValue)
	{
		if      (	L"hybridMultilevel" == sValue ||
					L"HybridMultilevel" == sValue)	this->m_eValue = emultileveltypeHybridMultilevel;
		else if (	L"multilevel"       == sValue ||
					L"Multilevel"       == sValue )	this->m_eValue = emultileveltypeMultilevel;
		else if (	L"singleLevel"      == sValue ||
					L"SingleLevel"      == sValue)	this->m_eValue = emultileveltypeSingleLevel;
		else										this->m_eValue = emultileveltypeSingleLevel;

		return this->m_eValue;
	}

	std::wstring CMultiLevelType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case emultileveltypeHybridMultilevel : return (L"hybridMultilevel");
		case emultileveltypeMultilevel       : return (L"multilevel");
		case emultileveltypeSingleLevel      : return (L"singleLevel");
		default                              : return (L"singleLevel");
		}
	}

	//--------------------------------------------------------------------------------
	// NumberFormat 17.18.59 (Part 1)
	//--------------------------------------------------------------------------------

	ENumberFormat CNumberFormat::FromString(const std::wstring &sValue)
	{
		if      ( (L"aiueo")                        == sValue ) this->m_eValue = numberformatAiueo;
		else if ( (L"aiueoFullWidth")               == sValue ) this->m_eValue = numberformatAiueoFullWidth;
		else if ( (L"arabicAbjad")                  == sValue ) this->m_eValue = numberformatArabicAbjad;
		else if ( (L"arabicAlpha")                  == sValue ) this->m_eValue = numberformatArabicAlpha;
		else if ( (L"bahtText")                     == sValue ) this->m_eValue = numberformatBahtText;
		else if ( (L"bullet")                       == sValue ) this->m_eValue = numberformatBullet;
		else if ( (L"cardinalText")                 == sValue ) this->m_eValue = numberformatCardinalText;
		else if ( (L"chicago")                      == sValue ) this->m_eValue = numberformatChicago;
		else if ( (L"chineseCounting")              == sValue ) this->m_eValue = numberformatChineseCounting;
		else if ( (L"chineseCountingThousand")      == sValue ) this->m_eValue = numberformatChineseCountingThousand;
		else if ( (L"chineseLegalSimplified")       == sValue ) this->m_eValue = numberformatChineseLegalSimplified;
		else if ( (L"chosung")                      == sValue ) this->m_eValue = numberformatChosung;
		else if ( (L"custom")                       == sValue ) this->m_eValue = numberformatCustom;
		else if ( (L"decimal")                      == sValue ) this->m_eValue = numberformatDecimal;
		else if ( (L"decimalEnclosedCircle")        == sValue ) this->m_eValue = numberformatDecimalEnclosedCircle;
		else if ( (L"decimalEnclosedCircleChinese") == sValue ) this->m_eValue = numberformatDecimalEnclosedCircleChinese;
		else if ( (L"decimalEnclosedFullstop")      == sValue ) this->m_eValue = numberformatDecimalEnclosedFullstop;
		else if ( (L"decimalEnclosedParen")         == sValue ) this->m_eValue = numberformatDecimalEnclosedParen;
		else if ( (L"decimalFullWidth")             == sValue ) this->m_eValue = numberformatDecimalFullWidth;
		else if ( (L"decimalFullWidth2")            == sValue ) this->m_eValue = numberformatDecimalFullWidth2;
		else if ( (L"decimalHalfWidth")             == sValue ) this->m_eValue = numberformatDecimalHalfWidth;
		else if ( (L"decimalZero")                  == sValue ) this->m_eValue = numberformatDecimalZero;
		else if ( (L"dollarText")                   == sValue ) this->m_eValue = numberformatDollarText;
		else if ( (L"ganada")                       == sValue ) this->m_eValue = numberformatGanada;
		else if ( (L"hebrew1")                      == sValue ) this->m_eValue = numberformatHebrew1;
		else if ( (L"hebrew2")                      == sValue ) this->m_eValue = numberformatHebrew2;
		else if ( (L"hex")                          == sValue ) this->m_eValue = numberformatHex;
		else if ( (L"hindiConsonants")              == sValue ) this->m_eValue = numberformatHindiConsonants;
		else if ( (L"hindiCounting")                == sValue ) this->m_eValue = numberformatHindiCounting;
		else if ( (L"hindiNumbers")                 == sValue ) this->m_eValue = numberformatHindiNumbers;
		else if ( (L"hindiVowels")                  == sValue ) this->m_eValue = numberformatHindiVowels;
		else if ( (L"ideographDigital")             == sValue ) this->m_eValue = numberformatIdeographDigital;
		else if ( (L"ideographEnclosedCircle")      == sValue ) this->m_eValue = numberformatIdeographEnclosedCircle;
		else if ( (L"ideographLegalTraditional")    == sValue ) this->m_eValue = numberformatIdeographLegalTraditional;
		else if ( (L"ideographTraditional")         == sValue ) this->m_eValue = numberformatIdeographTraditional;
		else if ( (L"ideographZodiac")              == sValue ) this->m_eValue = numberformatIdeographZodiac;
		else if ( (L"ideographZodiacTraditional")   == sValue ) this->m_eValue = numberformatIdeographZodiacTraditional;
		else if ( (L"iroha")                        == sValue ) this->m_eValue = numberformatIroha;
		else if ( (L"irohaFullWidth")               == sValue ) this->m_eValue = numberformatIrohaFullWidth;
		else if ( (L"japaneseCounting")             == sValue ) this->m_eValue = numberformatJapaneseCounting;
		else if ( (L"japaneseDigitalTenThousand")   == sValue ) this->m_eValue = numberformatJapaneseDigitalTenThousand;
		else if ( (L"japaneseLegal")                == sValue ) this->m_eValue = numberformatJapaneseLegal;
		else if ( (L"koreanCounting")               == sValue ) this->m_eValue = numberformatKoreanCounting;
		else if ( (L"koreanDigital")                == sValue ) this->m_eValue = numberformatKoreanDigital;
		else if ( (L"koreanDigital2")               == sValue ) this->m_eValue = numberformatKoreanDigital2;
		else if ( (L"koreanLegal")                  == sValue ) this->m_eValue = numberformatKoreanLegal;
		else if ( (L"lowerLetter")                  == sValue ) this->m_eValue = numberformatLowerLetter;
		else if ( (L"lowerRoman")                   == sValue ) this->m_eValue = numberformatLowerRoman;
		else if ( (L"none")                         == sValue ) this->m_eValue = numberformatNone;
		else if ( (L"numberInDash")                 == sValue ) this->m_eValue = numberformatNumberInDash;
		else if ( (L"ordinal")                      == sValue ) this->m_eValue = numberformatOrdinal;
		else if ( (L"ordinalText")                  == sValue ) this->m_eValue = numberformatOrdinalText;
		else if ( (L"russianLower")                 == sValue ) this->m_eValue = numberformatRussianLower;
		else if ( (L"russianUpper")                 == sValue ) this->m_eValue = numberformatRussianUpper;
		else if ( (L"taiwaneseCounting")            == sValue ) this->m_eValue = numberformatTaiwaneseCounting;
		else if ( (L"taiwaneseCountingThousand")    == sValue ) this->m_eValue = numberformatTaiwaneseCountingThousand;
		else if ( (L"taiwaneseDigital")             == sValue ) this->m_eValue = numberformatTaiwaneseDigital;
		else if ( (L"thaiCounting")                 == sValue ) this->m_eValue = numberformatThaiCounting;
		else if ( (L"thaiLetters")                  == sValue ) this->m_eValue = numberformatThaiLetters;
		else if ( (L"thaiNumbers")                  == sValue ) this->m_eValue = numberformatThaiNumbers;
		else if ( (L"upperLetter")                  == sValue ) this->m_eValue = numberformatUpperLetter;
		else if ( (L"upperRoman")                   == sValue ) this->m_eValue = numberformatUpperRoman;
		else if ( (L"vietnameseCounting")           == sValue ) this->m_eValue = numberformatVietnameseCounting;
		else                                                     this->m_eValue = numberformatNone;

		return this->m_eValue;
	}

	std::wstring CNumberFormat::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// ObjectDrawAspect 17.18.60 (Part 1)
	//--------------------------------------------------------------------------------

	EObjectDrawAspect CObjectDrawAspect::FromString(const std::wstring &sValue)
	{
		if      ( (L"content") == sValue ) this->m_eValue = objectdrawaspectContent;
		else if ( (L"icon")    == sValue ) this->m_eValue = objectdrawaspectIcon;
		else                                this->m_eValue = objectdrawaspectContent;

		return this->m_eValue;
	}

	std::wstring CObjectDrawAspect::ToString  () const
	{
		switch(this->m_eValue)
		{
		case objectdrawaspectContent : return (L"content");
		case objectdrawaspectIcon    : return (L"icon");
		default                      : return (L"content");
		}
	}

	//--------------------------------------------------------------------------------
	// ObjectUpdateMode 17.18.61 (Part 1)
	//--------------------------------------------------------------------------------

	EObjectUpdateMode CObjectUpdateMode::FromString(const std::wstring &sValue)
	{
		if      ( (L"always") == sValue ) this->m_eValue = objectupdatemodeAlways;
		else if ( (L"onCall") == sValue ) this->m_eValue = objectupdatemodeOnCall;
		else                               this->m_eValue = objectupdatemodeAlways;

		return this->m_eValue;
	}

	std::wstring CObjectUpdateMode::ToString  () const
	{
		switch(this->m_eValue)
		{
		case objectupdatemodeAlways : return (L"always");
		case objectupdatemodeOnCall : return (L"onCall");
		default                     : return (L"always");
		}
	}

	//--------------------------------------------------------------------------------
	// PageBorderDisplay 17.18.62 (Part 1)
	//--------------------------------------------------------------------------------

	EPageBorderDisplay CPageBorderDisplay::FromString(const std::wstring &sValue)
	{
		if      ( (L"allPages")     == sValue ) this->m_eValue = pageborderdisplayAllPages;
		else if ( (L"firstPage")    == sValue ) this->m_eValue = pageborderdisplayFirstPage;
		else if ( (L"notFirstPage") == sValue ) this->m_eValue = pageborderdisplayNotFirstPage;
		else                                     this->m_eValue = pageborderdisplayAllPages;

		return this->m_eValue;
	}

	std::wstring CPageBorderDisplay::ToString  () const
	{
		switch(this->m_eValue)
		{
		case pageborderdisplayAllPages     : return (L"allPages");
		case pageborderdisplayFirstPage    : return (L"firstPage");
		case pageborderdisplayNotFirstPage : return (L"notFirstPage");
		default                            : return (L"allPages");
		}
	}

	//--------------------------------------------------------------------------------
	// PageBorderOffset 17.18.63 (Part1)
	//--------------------------------------------------------------------------------

	EPageBorderOffset CPageBorderOffset::FromString(const std::wstring &sValue)
	{
		if      ( (L"page") == sValue ) this->m_eValue = pageborderoffsetPage;
		else if ( (L"text") == sValue ) this->m_eValue = pageborderoffsetText;
		else                             this->m_eValue = pageborderoffsetPage;

		return this->m_eValue;
	}

	std::wstring CPageBorderOffset::ToString  () const
	{
		switch(this->m_eValue)
		{
		case pageborderoffsetPage : return (L"page");
		case pageborderoffsetText : return (L"text");
		default                   : return (L"page");
		}
	}

	//--------------------------------------------------------------------------------
	// PageBorderZOrder 17.18.64 (Part 1)
	//--------------------------------------------------------------------------------

	EPageBorderZOrder CPageBorderZOrder::FromString(const std::wstring &sValue)
	{
		if      ( (L"back")  == sValue ) this->m_eValue = pageborderzorderBack;
		else if ( (L"front") == sValue ) this->m_eValue = pageborderzorderFront;
		else                             this->m_eValue = pageborderzorderBack;

		return this->m_eValue;
	}

	std::wstring CPageBorderZOrder::ToString  () const
	{
		switch(this->m_eValue)
		{
		case pageborderzorderBack  : return (L"back");
		case pageborderzorderFront : return (L"front");
		default                    : return (L"back");
		}
	}

	//--------------------------------------------------------------------------------
	// PageOrientation 17.18.65 (Part 1)
	//--------------------------------------------------------------------------------

	EPageOrientation CPageOrientation::FromString(const std::wstring &sValue)
	{
		if      ( (L"landscape") == sValue ) this->m_eValue = pageorientLandscape;
		else if ( (L"portrait")  == sValue ) this->m_eValue = pageorientPortrait;
		else                                  this->m_eValue = pageorientPortrait;

		return this->m_eValue;
	}

	std::wstring CPageOrientation::ToString  () const
	{
		switch(this->m_eValue)
		{
		case pageorientLandscape : return (L"landscape");
		case pageorientPortrait  : return (L"portrait");
		default                  : return (L"portrait");
		}
	}

	//--------------------------------------------------------------------------------
	// Pitch 17.18.66 (Part 1)
	//--------------------------------------------------------------------------------

	EPitch CPitch::FromString(const std::wstring &sValue)
	{
		if      ( (L"default")  == sValue ) this->m_eValue = pitchDefault;
		else if ( (L"fixed")    == sValue ) this->m_eValue = pitchFixed;
		else if ( (L"variable") == sValue ) this->m_eValue = pitchVariable;
		else                                 this->m_eValue = pitchDefault;

		return this->m_eValue;
	}

	std::wstring CPitch::ToString  () const
	{
		switch(this->m_eValue)
		{
		case pitchDefault  : return (L"default");
		case pitchFixed    : return (L"fixed");
		case pitchVariable : return (L"variable");
		default            : return (L"default");
		}
	}

	//--------------------------------------------------------------------------------
	// PixelsMeasure 17.18.67 (Part 1)
	//--------------------------------------------------------------------------------

	int CPixelsMeasure::FromString(const std::wstring &sValue)
	{
		this->m_eValue = XmlUtils::GetInteger(sValue);

		return this->m_eValue;
	}

	std::wstring CPixelsMeasure::ToString  () const
	{
		std::wstring sResult = std::to_wstring( this->m_eValue);

		return sResult;
	}

	//--------------------------------------------------------------------------------
	// PointMeasure 17.18.68 (Part 1)
	//--------------------------------------------------------------------------------

	int CPointMeasure::FromString(const std::wstring &sValue)
	{
		this->m_eValue = XmlUtils::GetInteger(sValue);

		return this->m_eValue;
	}

	std::wstring CPointMeasure::ToString  () const
	{
		std::wstring sResult = std::to_wstring( this->m_eValue);

		return sResult;
	}

	double CPointMeasure::ToPoints() const
	{
		return (double)this->m_eValue;
	}

	double CPointMeasure::ToMM() const
	{
		return (double)Pt_To_Mm(this->m_eValue);
	}

	double CPointMeasure::ToInches() const
	{
		return this->m_eValue / 72.0;
	}


	void CPointMeasure::FromPoints(double dValue)
	{
		this->m_eValue = (int)dValue;
	}

	void CPointMeasure::FromInches(double dValue)
	{
		this->m_eValue = (int)(dValue * 72.0);
	}

	//--------------------------------------------------------------------------------
	// Proof 17.18.69 (Part 1)
	//--------------------------------------------------------------------------------

	EProof CProof::FromString(const std::wstring &sValue)
	{
		if      ( (L"clean") == sValue ) this->m_eValue = proofClean;
		else if ( (L"dirty") == sValue ) this->m_eValue = proofDirty;
		else                              this->m_eValue = proofDirty;

		return this->m_eValue;
	}

	std::wstring CProof::ToString  () const
	{
		switch(this->m_eValue)
		{
		case proofClean : return (L"clean");
		case proofDirty : return (L"dirty");
		default         : return (L"dirty");
		}
	}

	//--------------------------------------------------------------------------------
	// ProofErr 17.18.70 (Part 1)
	//--------------------------------------------------------------------------------

	EProofErr CProofErr::FromString(const std::wstring &sValue)
	{
		if      ( (L"gramEnd")    == sValue ) this->m_eValue = prooferrGramEnd;
		else if ( (L"gramStart")  == sValue ) this->m_eValue = prooferrGramStart;
		else if ( (L"spellEnd")   == sValue ) this->m_eValue = prooferrSpellEnd;
		else if ( (L"spellStart") == sValue ) this->m_eValue = prooferrSpellStart;
		else                                   this->m_eValue = prooferrSpellStart;

		return this->m_eValue;
	}

	std::wstring CProofErr::ToString  () const
	{
		switch(this->m_eValue)
		{
		case prooferrGramEnd    : return (L"gramEnd");
		case prooferrGramStart  : return (L"gramStart");
		case prooferrSpellEnd   : return (L"spellEnd");
		case prooferrSpellStart : return (L"spellStart");
		default                 : return (L"spellStart");
		}
	}

	//--------------------------------------------------------------------------------
	// PTabAlignment 17.18.71 (Part 1)
	//--------------------------------------------------------------------------------

	EPTabAlignment CPTabAlignment::FromString(const std::wstring &sValue)
	{
		if      ( (L"center") == sValue ) this->m_eValue = ptabalignmentCenter;
		else if ( (L"left")   == sValue ) this->m_eValue = ptabalignmentLeft;
		else if ( (L"right")  == sValue ) this->m_eValue = ptabalignmentRight;
		else                               this->m_eValue = ptabalignmentLeft;

		return this->m_eValue;
	}

	std::wstring CPTabAlignment::ToString  () const
	{
		switch(this->m_eValue)
		{
		case ptabalignmentCenter : return (L"center");
		case ptabalignmentLeft   : return (L"left");
		case ptabalignmentRight  : return (L"right");
		default                  : return (L"left");
		}
	}

	//--------------------------------------------------------------------------------
	// PTabLeader 17.18.72 (Part 1)
	//--------------------------------------------------------------------------------

	EPTabLeader CPTabLeader::FromString(const std::wstring &sValue)
	{
		if      ( (L"dot")        == sValue ) this->m_eValue = ptableaderDot;
		else if ( (L"hyphen")     == sValue ) this->m_eValue = ptableaderHyphen;
		else if ( (L"middleDot")  == sValue ) this->m_eValue = ptableaderMiddleDot;
		else if ( (L"none")       == sValue ) this->m_eValue = ptableaderNone;
		else if ( (L"underscore") == sValue ) this->m_eValue = ptableaderUnderscore;
		else                                   this->m_eValue = ptableaderNone;

		return this->m_eValue;
	}

	std::wstring CPTabLeader::ToString  () const
	{
		switch(this->m_eValue)
		{
		case ptableaderDot        : return (L"dot");
		case ptableaderHyphen     : return (L"hyphen");
		case ptableaderMiddleDot  : return (L"middleDot");
		case ptableaderNone       : return (L"none");
		case ptableaderUnderscore : return (L"underscore");
		default                   : return (L"none");
		}
	}

	//--------------------------------------------------------------------------------
	// PTabRelativeTo 17.18.73 (Part 1)
	//--------------------------------------------------------------------------------

	EPTabRelativeTo CPTabRelativeTo::FromString(const std::wstring &sValue)
	{
		if      ( (L"indent") == sValue ) this->m_eValue = ptabrelativetoIndent;
		else if ( (L"margin") == sValue ) this->m_eValue = ptabrelativetoMargin;
		else                               this->m_eValue = ptabrelativetoIndent;

		return this->m_eValue;
	}

	std::wstring CPTabRelativeTo::ToString  () const
	{
		switch(this->m_eValue)
		{
		case ptabrelativetoIndent : return (L"indent");
		case ptabrelativetoMargin : return (L"margin");
		default                   : return (L"indent");
		}
	}

	//--------------------------------------------------------------------------------
	// RestartNumber 17.18.74 (Part 1)
	//--------------------------------------------------------------------------------

	ERestartNumber CRestartNumber::FromString(const std::wstring &sValue)
	{
		if      ( (L"continuous") == sValue ) this->m_eValue = restartnumberContinious;
		else if ( (L"eachPage")   == sValue ) this->m_eValue = restartnumberEachPage;
		else if ( (L"eachSect")   == sValue ) this->m_eValue = restartnumberEachSect;
		else                                   this->m_eValue = restartnumberContinious;

		return this->m_eValue;
	}

	std::wstring CRestartNumber::ToString  () const
	{
		switch(this->m_eValue)
		{
		case restartnumberContinious : return (L"continuous");
		case restartnumberEachPage   : return (L"eachPage");
		case restartnumberEachSect   : return (L"eachSect");
		default                      : return (L"continuous");
		}
	}

	//--------------------------------------------------------------------------------
	// RubyAlign 17.18.75 (Part 1)
	//--------------------------------------------------------------------------------

	ERubyAlign CRubyAlign::FromString(const std::wstring &sValue)
	{
		if      ( (L"center")           == sValue ) this->m_eValue = erubyalignCenter;
		else if ( (L"distributeLetter") == sValue ) this->m_eValue = erubyalignDistributeLetter;
		else if ( (L"distributeSpace")  == sValue ) this->m_eValue = erubyalignDistributeSpace;
		else if ( (L"left")             == sValue ) this->m_eValue = erubyalignLeft;
		else if ( (L"right")            == sValue ) this->m_eValue = erubyalignRight;
		else if ( (L"rightVertical")    == sValue ) this->m_eValue = erubyalignRightVertical;
		else                                         this->m_eValue = erubyalignLeft;

		return this->m_eValue;
	}

	std::wstring CRubyAlign::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// SdtDateMappingType 17.18.76 (Part 1)
	//--------------------------------------------------------------------------------

	ESdtDateMappingType CSdtDateMappingType::FromString(const std::wstring &sValue)
	{
		if      ( (L"date")     == sValue ) this->m_eValue = sdtdatemappingtypeDate;
		else if ( (L"dateTime") == sValue ) this->m_eValue = sdtdatemappingtypeDateTime;
		else if ( (L"text")     == sValue ) this->m_eValue = sdtdatemappingtypeText;
		else                                 this->m_eValue = sdtdatemappingtypeText;

		return this->m_eValue;
	}

	std::wstring CSdtDateMappingType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case sdtdatemappingtypeDate     : return (L"date");
		case sdtdatemappingtypeDateTime : return (L"dateTime");
		case sdtdatemappingtypeText     : return (L"text");
		default                         : return (L"text");
		}
	}

	//--------------------------------------------------------------------------------
	// SectionMark 17.18.77 (Part 1)
	//--------------------------------------------------------------------------------

	ESectionMark CSectionMark::FromString(const std::wstring &sValue)
	{
		if      ( (L"continuous") == sValue ) this->m_eValue = sectionmarkContinious;
		else if ( (L"evenPage")   == sValue ) this->m_eValue = sectionmarkEvenPage;
		else if ( (L"nextColumn") == sValue ) this->m_eValue = sectionmarkNextColumn;
		else if ( (L"nextPage")   == sValue ) this->m_eValue = sectionmarkNextPage;
		else if ( (L"oddPage")    == sValue ) this->m_eValue = sectionmarkOddPage;
		else                                   this->m_eValue = sectionmarkNextPage;

		return this->m_eValue;
	}

	std::wstring CSectionMark::ToString  () const
	{
		switch(this->m_eValue)
		{
		case sectionmarkContinious : return (L"continuous");
		case sectionmarkEvenPage   : return (L"evenPage");
		case sectionmarkNextColumn : return (L"nextColumn");
		case sectionmarkNextPage   : return (L"nextPage");
		case sectionmarkOddPage    : return (L"oddPage");
		default                    : return (L"nextPage");
		}
	}

	//--------------------------------------------------------------------------------
	// Shd 17.18.78 (Part 1)
	//--------------------------------------------------------------------------------

	EShd CShd::FromString(const std::wstring &sValue)
	{
		if      ( (L"clear")                 == sValue ) this->m_eValue = shdClear;
		else if ( (L"diagCross")             == sValue ) this->m_eValue = shdDiagCross;
		else if ( (L"diagStripe")            == sValue ) this->m_eValue = shdDiagStripe;
		else if ( (L"horzCross")             == sValue ) this->m_eValue = shdHorzCross;
		else if ( (L"horzStripe")            == sValue ) this->m_eValue = shdHorzStripe;
		else if ( (L"nil")                   == sValue ) this->m_eValue = shdNil;
		else if ( (L"pct10")                 == sValue ) this->m_eValue = shdPct10;
		else if ( (L"pct12")                 == sValue ) this->m_eValue = shdPct12;
		else if ( (L"pct15")                 == sValue ) this->m_eValue = shdPct15;
		else if ( (L"pct20")                 == sValue ) this->m_eValue = shdPct20;
		else if ( (L"pct25")                 == sValue ) this->m_eValue = shdPct25;
		else if ( (L"pct30")                 == sValue ) this->m_eValue = shdPct30;
		else if ( (L"pct35")                 == sValue ) this->m_eValue = shdPct35;
		else if ( (L"pct37")                 == sValue ) this->m_eValue = shdPct37;
		else if ( (L"pct40")                 == sValue ) this->m_eValue = shdPct40;
		else if ( (L"pct45")                 == sValue ) this->m_eValue = shdPct45;
		else if ( (L"pct5")                  == sValue ) this->m_eValue = shdPct5;
		else if ( (L"pct50")                 == sValue ) this->m_eValue = shdPct50;
		else if ( (L"pct55")                 == sValue ) this->m_eValue = shdPct55;
		else if ( (L"pct60")                 == sValue ) this->m_eValue = shdPct60;
		else if ( (L"pct62")                 == sValue ) this->m_eValue = shdPct62;
		else if ( (L"pct65")                 == sValue ) this->m_eValue = shdPct65;
		else if ( (L"pct70")                 == sValue ) this->m_eValue = shdPct70;
		else if ( (L"pct75")                 == sValue ) this->m_eValue = shdPct75;
		else if ( (L"pct80")                 == sValue ) this->m_eValue = shdPct80;
		else if ( (L"pct85")                 == sValue ) this->m_eValue = shdPct85;
		else if ( (L"pct87")                 == sValue ) this->m_eValue = shdPct87;
		else if ( (L"pct90")                 == sValue ) this->m_eValue = shdPct90;
		else if ( (L"pct95")                 == sValue ) this->m_eValue = shdPct95;
		else if ( (L"reverseDiagStripe")     == sValue ) this->m_eValue = shdReverseDiagStripe;
		else if ( (L"solid")                 == sValue ) this->m_eValue = shdSolid;
		else if ( (L"thinDiagCross")         == sValue ) this->m_eValue = shdThinDiagCross;
		else if ( (L"thinDiagStripe")        == sValue ) this->m_eValue = shdThinDiagStripe;
		else if ( (L"thinHorzCross")         == sValue ) this->m_eValue = shdThinHorzCross;
		else if ( (L"thinHorzStripe")        == sValue ) this->m_eValue = shdThinHorzStripe;
		else if ( (L"thinReverseDiagStripe") == sValue ) this->m_eValue = shdThinReverseDiagStripe;
		else if ( (L"thinVertStripe")        == sValue ) this->m_eValue = shdThinVertStripe;
		else if ( (L"vertStripe")            == sValue ) this->m_eValue = shdVertStripe;
		else                                              this->m_eValue = shdSolid;

		return this->m_eValue;
	}

	std::wstring CShd::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// ShortHexNumber 17.18.79 (Part 1)
	//--------------------------------------------------------------------------------

	int	CShortHexNumber::HexToInt(int nHex, bool &bResult)
	{
		if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
		if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
		if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

		bResult = false;

		return 0;
	}

	bool CShortHexNumber::Parse(const std::wstring &sValue)
	{
		size_t nLength = sValue.length();

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

	int CShortHexNumber::FromString(const std::wstring &sValue)
	{
		if ( !Parse(sValue) )
		{
			this->m_eValue = 0;
		}

		return this->m_eValue;
	}

	std::wstring CShortHexNumber::ToString  () const
	{
		return XmlUtils::ToString( this->m_eValue, L"%04X");
	}

	//--------------------------------------------------------------------------------
	// SignedHpsMeasure 17.18.80 (Part 1)
	//--------------------------------------------------------------------------------

	CSignedHpsMeasure::CSignedHpsMeasure() {}

	double CSignedHpsMeasure::FromString(const std::wstring &sValue)
	{
		Parse(sValue, 2);

		return m_dValue;
	}
	void CSignedHpsMeasure::SetValue(double dValue)
	{
		m_bUnit = false;
		m_dValue = dValue;
	}
	std::wstring CSignedHpsMeasure::ToString () const
	{
		std::wstring sResult;

		if (m_bUnit)
			sResult = boost::lexical_cast<std::wstring>(m_dValue) + L"pt";
		else
			sResult = std::to_wstring((int)(m_dValue * 2));

		return sResult;
	}

	//--------------------------------------------------------------------------------
	// SignedTwipsMeasure 17.18.81 (Part 1)
	//--------------------------------------------------------------------------------

	CSignedTwipsMeasure::CSignedTwipsMeasure() {}
	void CSignedTwipsMeasure::SetValue(double dValue)
	{
		m_bUnit = false;
		m_dValue = FromTwips(dValue);
	}
	double CSignedTwipsMeasure::FromString(const std::wstring &sValue)
	{
		Parse(sValue, 20);

		return m_dValue;
	}

	std::wstring CSignedTwipsMeasure::ToString  () const
	{
		std::wstring sResult;

		if ( m_bUnit )
			sResult = boost::lexical_cast<std::wstring>( m_dValue ) + L"pt";
		else
			sResult = std::to_wstring( (int)(m_dValue * 20) );

		return sResult;
	}

	//--------------------------------------------------------------------------------
	// StyleSort 17.18.82 (Part 1) + 9.10.4 (Part 4)
	//--------------------------------------------------------------------------------

	EStyleSort CStyleSort::FromString(const std::wstring &sValue)
	{
		m_bNames = false;
		if      ( (L"basedOn")  == sValue || (L"0004") == sValue ) this->m_eValue = stylesortBasedOn;
		else if ( (L"default")  == sValue || (L"0002") == sValue ) this->m_eValue = stylesortDefault;
		else if ( (L"font")     == sValue || (L"0003") == sValue ) this->m_eValue = stylesortFont;
		else if ( (L"name")     == sValue || (L"0000") == sValue ) this->m_eValue = stylesortName;
		else if ( (L"priority") == sValue || (L"0001") == sValue ) this->m_eValue = stylesortPriority;
		else if ( (L"type")     == sValue || (L"0005") == sValue ) this->m_eValue = stylesortType;
		else                                 this->m_eValue = stylesortName;

		return this->m_eValue;
	}

	std::wstring CStyleSort::ToString  () const
	{
		if ( m_bNames )
		{
			switch(this->m_eValue)
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
			switch(this->m_eValue)
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

	void CStyleSort::WriteNames(bool bNames)
	{
		m_bNames = bNames;
	}

	//--------------------------------------------------------------------------------
	// StyleType 17.18.83 (Part 1)
	//--------------------------------------------------------------------------------

	EStyleType CStyleType::FromString(const std::wstring &sValue)
	{
		if      ( (L"character") == sValue ) this->m_eValue = styletypeCharacter;
		else if ( (L"numbering") == sValue ) this->m_eValue = styletypeNumbering;
		else if ( (L"paragraph") == sValue ) this->m_eValue = styletypeParagraph;
		else if ( (L"table")     == sValue ) this->m_eValue = styletypeTable;
		else if ( (L"list")		 == sValue)  this->m_eValue = styletypeNumbering;
		else                                  this->m_eValue = styletypeParagraph;

		return this->m_eValue;
	}

	std::wstring CStyleType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case styletypeCharacter : return (L"character");
		case styletypeNumbering : return (L"numbering");
		case styletypeParagraph : return (L"paragraph");
		case styletypeTable     : return (L"table");
		default                 : return (L"paragraph");
		}
	}

	//--------------------------------------------------------------------------------
	// TabJc 17.18.84 (Part 1) + 9.10.5 (Part 4)
	//--------------------------------------------------------------------------------

	ETabJc CTabJc::FromString(const std::wstring &sValue)
	{
		if      ( (L"bar")     == sValue ) this->m_eValue = tabjcBar;
		else if ( (L"center")  == sValue ) this->m_eValue = tabjcCenter;
		else if ( (L"clear")   == sValue ) this->m_eValue = tabjcClear;
		else if ( (L"decimal") == sValue ) this->m_eValue = tabjcDecimal;
		else if ( (L"end")     == sValue ) this->m_eValue = tabjcEnd;
		else if ( (L"num")     == sValue ) this->m_eValue = tabjcNum;
		else if ( (L"start")   == sValue ) this->m_eValue = tabjcStart;
		else if ( (L"right")   == sValue ) this->m_eValue = tabjcRight;
		else if ( (L"left")    == sValue ) this->m_eValue = tabjcLeft;
		else                                this->m_eValue = tabjcClear;

		return this->m_eValue;
	}

	std::wstring CTabJc::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// TabTlc 17.18.85 (Part 1)
	//--------------------------------------------------------------------------------

	ETabTlc CTabTlc::FromString(const std::wstring &sValue)
	{
		if      ( (L"dot")        == sValue ) this->m_eValue = tabtlcDot;
		else if ( (L"heavy")      == sValue ) this->m_eValue = tabtlcHeavy;
		else if ( (L"hyphen")     == sValue ) this->m_eValue = tabtlcHyphen;
		else if ( (L"middleDot")  == sValue ) this->m_eValue = tabtlcMiddleDot;
		else if ( (L"none")       == sValue ) this->m_eValue = tabtlcNone;
		else if ( (L"underscore") == sValue ) this->m_eValue = tabtlcUnderscore;
		else                                   this->m_eValue = tabtlcNone;

		return this->m_eValue;
	}

	std::wstring CTabTlc::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// TargetScreenSz 17.18.86 (Part 1)
	//--------------------------------------------------------------------------------

	ETargetScreenSz CTargetScreenSz::FromString(const std::wstring &sValue)
	{
		if      ( (L"1024x768")  == sValue ) this->m_eValue = targetscreensz1024x768;
		else if ( (L"1152x882")  == sValue ) this->m_eValue = targetscreensz1152x882;
		else if ( (L"1152x900")  == sValue ) this->m_eValue = targetscreensz1152x900;
		else if ( (L"1280x1024") == sValue ) this->m_eValue = targetscreensz1280x1024;
		else if ( (L"1600x1200") == sValue ) this->m_eValue = targetscreensz1600x1200;
		else if ( (L"1800x1440") == sValue ) this->m_eValue = targetscreensz1800x1440;
		else if ( (L"1920x1200") == sValue ) this->m_eValue = targetscreensz1920x1200;
		else if ( (L"544x376")   == sValue ) this->m_eValue = targetscreensz544x376;
		else if ( (L"640x480")   == sValue ) this->m_eValue = targetscreensz640x480;
		else if ( (L"720x512")   == sValue ) this->m_eValue = targetscreensz720x512;
		else if ( (L"800x600")   == sValue ) this->m_eValue = targetscreensz800x600;
		else                                  this->m_eValue = targetscreensz1280x1024;

		return this->m_eValue;
	}

	std::wstring CTargetScreenSz::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// TblLayoutType 17.18.87 (Part 1)
	//--------------------------------------------------------------------------------

	ETblLayoutType CTblLayoutType::FromString(const std::wstring &sValue)
	{
		if      ( (L"autofit") == sValue ) this->m_eValue = tbllayouttypeAutofit;
		else if ( (L"fixed")   == sValue ) this->m_eValue = tbllayouttypeFixed;
		else                                this->m_eValue = tbllayouttypeAutofit;

		return this->m_eValue;
	}

	std::wstring CTblLayoutType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case tbllayouttypeAutofit : return (L"autofit");
		case tbllayouttypeFixed   : return (L"fixed");
		default                   : return (L"autofit");
		}
	}

	//--------------------------------------------------------------------------------
	// TblOverlap 17.18.88 (Part 1)
	//--------------------------------------------------------------------------------

	ETblOverlap CTblOverlap::FromString(const std::wstring &sValue)
	{
		if      ( (L"never")   == sValue ) this->m_eValue = tbloverlapNever;
		else if ( (L"overlap") == sValue ) this->m_eValue = tbloverlapOverlap;
		else                                this->m_eValue = tbloverlapOverlap;

		return this->m_eValue;
	}

	std::wstring CTblOverlap::ToString  () const
	{
		switch(this->m_eValue)
		{
		case tbloverlapNever   : return (L"never");
		case tbloverlapOverlap : return (L"overlap");
		default                : return (L"overlap");
		}
	}

	//--------------------------------------------------------------------------------
	// TblStyleOverrideType 17.18.89 (Part 1)
	//--------------------------------------------------------------------------------

	ETblStyleOverrideType CTblStyleOverrideType::FromString(const std::wstring &sValue)
	{
		if      ( (L"band1Horz")  == sValue ) this->m_eValue = tblstyleoverridetypeBand1Horz;
		else if ( (L"band1Vert")  == sValue ) this->m_eValue = tblstyleoverridetypeBand1Vert;
		else if ( (L"band2Horz")  == sValue ) this->m_eValue = tblstyleoverridetypeBand2Horz;
		else if ( (L"band2Vert")  == sValue ) this->m_eValue = tblstyleoverridetypeBand2Vert;
		else if ( (L"firstCol")   == sValue ) this->m_eValue = tblstyleoverridetypeFirstCol;
		else if ( (L"firstRow")   == sValue ) this->m_eValue = tblstyleoverridetypeFirstRow;
		else if ( (L"lastCol")    == sValue ) this->m_eValue = tblstyleoverridetypeLastCol;
		else if ( (L"lastRow")    == sValue ) this->m_eValue = tblstyleoverridetypeLastRow;
		else if ( (L"neCell")     == sValue ) this->m_eValue = tblstyleoverridetypeNeCell;
		else if ( (L"nwCell")     == sValue ) this->m_eValue = tblstyleoverridetypeNwCell;
		else if ( (L"seCell")     == sValue ) this->m_eValue = tblstyleoverridetypeSeCell;
		else if ( (L"swCell")     == sValue ) this->m_eValue = tblstyleoverridetypeSwCell;
		else if ( (L"wholeTable") == sValue ) this->m_eValue = tblstyleoverridetypeWholeTable;
		else                                   this->m_eValue = tblstyleoverridetypeWholeTable;

		return this->m_eValue;
	}

	std::wstring CTblStyleOverrideType::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// TblWidth 17.18.90 (Part 1)
	//--------------------------------------------------------------------------------

	ETblWidth CTblWidth::FromString(const std::wstring &sValue)
	{
		if      ( (L"auto") == sValue ) this->m_eValue = tblwidthAuto;
		else if ( (L"dxa")  == sValue ) this->m_eValue = tblwidthDxa;
		else if ( (L"nil")  == sValue ) this->m_eValue = tblwidthNil;
		else if ( (L"pct")  == sValue ) this->m_eValue = tblwidthPct;
		else                             this->m_eValue = tblwidthAuto;

		return this->m_eValue;
	}

	std::wstring CTblWidth::ToString  () const
	{
		switch(this->m_eValue)
		{
		case tblwidthAuto : return (L"auto");
		case tblwidthDxa  : return (L"dxa");
		case tblwidthNil  : return (L"nil");
		case tblwidthPct  : return (L"pct");
		default           : return (L"auto");
		}
	}

	//--------------------------------------------------------------------------------
	// TextAlignment 17.18.91 (Part 1)
	//--------------------------------------------------------------------------------

	ETextAlignment CTextAlignment::FromString(const std::wstring &sValue)
	{
		if      ( (L"auto")     == sValue ) this->m_eValue = textalignAuto;
		else if ( (L"baseline") == sValue ) this->m_eValue = textalignBaseLine;
		else if ( (L"bottom")   == sValue ) this->m_eValue = textalignBottom;
		else if ( (L"center")   == sValue ) this->m_eValue = textalignCenter;
		else if ( (L"top")      == sValue ) this->m_eValue = textalignTop;
		else                                 this->m_eValue = textalignAuto;

		return this->m_eValue;
	}

	std::wstring CTextAlignment::ToString  () const
	{
		switch(this->m_eValue)
		{
		case textalignAuto     : return (L"auto");
		case textalignBaseLine : return (L"baseline");
		case textalignBottom   : return (L"bottom");
		case textalignCenter   : return (L"center");
		case textalignTop      : return (L"top");
		default                : return (L"auto");
		}
	}

	//--------------------------------------------------------------------------------
	// TextboxTightWrap 17.18.92 (Part 1)
	//--------------------------------------------------------------------------------

	ETextboxTightWrap CTextboxTightWrap::FromString(const std::wstring &sValue)
	{
		if      ( (L"allLines")         == sValue ) this->m_eValue = textboxtightwrapAllLines;
		else if ( (L"firstAndLastLine") == sValue ) this->m_eValue = textboxtightwrapFirstAndLastLine;
		else if ( (L"firstLineOnly")    == sValue ) this->m_eValue = textboxtightwrapFirstLineOnly;
		else if ( (L"lastLineOnly")     == sValue ) this->m_eValue = textboxtightwrapLastLineOnly;
		else if ( (L"none")             == sValue ) this->m_eValue = textboxtightwrapNone;
		else                                         this->m_eValue = textboxtightwrapNone;

		return this->m_eValue;
	}

	std::wstring CTextboxTightWrap::ToString  () const
	{
		switch(this->m_eValue)
		{
		case textboxtightwrapAllLines         : return (L"allLines");
		case textboxtightwrapFirstAndLastLine : return (L"firstAndLastLine");
		case textboxtightwrapFirstLineOnly    : return (L"firstLineOnly");
		case textboxtightwrapLastLineOnly     : return (L"lastLineOnly");
		case textboxtightwrapNone             : return (L"none");
		default                               : return (L"none");
		}
	}

	//--------------------------------------------------------------------------------
	// TextDirection 17.18.93 (Part 1) + 9.10.6 (Part 4)
	//--------------------------------------------------------------------------------

	ETextDirection CTextDirection::FromString(const std::wstring &sValue)
	{
		if      ( (L"lr")  == sValue || (L"btLr")  == sValue || (L"bt-lr") == sValue)
			this->m_eValue = textdirectionLr;
		else if ( (L"lrV") == sValue || (L"tbLrV") == sValue )
			this->m_eValue = textdirectionLrV;
		else if ( (L"rl")  == sValue || (L"tbRl")  == sValue || (L"tb-rl") == sValue)
			this->m_eValue = textdirectionRl;
		else if ( (L"rlV") == sValue || (L"tbRlV") == sValue || (L"tb-rl-v") == sValue)
			this->m_eValue = textdirectionRlV;
		else if ( (L"tb")  == sValue || (L"lrTb")  == sValue)
			this->m_eValue = textdirectionTb;
		else if ( (L"tbV") == sValue || (L"lrTbV") == sValue || (L"lr-tb-v") == sValue)
			this->m_eValue = textdirectionTbV;
		//tb-rl
		else
			this->m_eValue = textdirectionTb;

		return this->m_eValue;
	}

	std::wstring CTextDirection::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// TextEffect 17.18.94 (Part 1)
	//--------------------------------------------------------------------------------

	ETextEffect CTextEffect::FromString(const std::wstring &sValue)
	{
		if      ( (L"antsBlack")       == sValue ) this->m_eValue = texteffectAntsBlack;
		else if ( (L"antsRed")         == sValue ) this->m_eValue = texteffectAntsRed;
		else if ( (L"blinkBackground") == sValue ) this->m_eValue = texteffectBlinkBackground;
		else if ( (L"lights")          == sValue ) this->m_eValue = texteffectLights;
		else if ( (L"none")            == sValue ) this->m_eValue = texteffectNone;
		else if ( (L"shimmer")         == sValue ) this->m_eValue = texteffectShimmer;
		else if ( (L"sparkle")         == sValue ) this->m_eValue = texteffectSparkle;
		else                                        this->m_eValue = texteffectNone;

		return this->m_eValue;
	}

	std::wstring CTextEffect::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// TextScale 17.18.95 (Part 1)
	//--------------------------------------------------------------------------------

	void CTextScale::SetValue(int nValue)
	{
		this->m_eValue = (std::min)( 600, (std::max)( 0, nValue ) );
	}

	int CTextScale::FromString(const std::wstring &sValue)
	{
		this->m_eValue = XmlUtils::GetInteger(sValue);

		this->m_eValue = (std::min)( 600, (std::max)( 0, this->m_eValue ) );

		return this->m_eValue;
	}

	std::wstring CTextScale::ToString  () const
	{
		std::wstring sResult = std::to_wstring( this->m_eValue);

		return sResult;
	}

	//--------------------------------------------------------------------------------
	// Theme 17.18.96 (Part 1)
	//--------------------------------------------------------------------------------

	ETheme CTheme::FromString(const std::wstring &sValue)
	{
		if      ( (L"majorAscii")    == sValue ) this->m_eValue = themeMajorAscii;
		else if ( (L"majorBidi")     == sValue ) this->m_eValue = themeMajorBidi;
		else if ( (L"majorEastAsia") == sValue ) this->m_eValue = themeMajorEastAsia;
		else if ( (L"majorHAnsi")    == sValue ) this->m_eValue = themeMajorHAnsi;
		else if ( (L"minorAscii")    == sValue ) this->m_eValue = themeMinorAscii;
		else if ( (L"minorBidi")     == sValue ) this->m_eValue = themeMinorBidi;
		else if ( (L"minorEastAsia") == sValue ) this->m_eValue = themeMinorEastAsia;
		else if ( (L"minorHAnsi")    == sValue ) this->m_eValue = themeMinorHAnsi;
		else                                      this->m_eValue = themeMajorAscii;

		return this->m_eValue;
	}

	std::wstring CTheme::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// ThemeColor 17.18.97 (Part 1)
	//--------------------------------------------------------------------------------

	EThemeColor CThemeColor::FromString(const std::wstring &sValue)
	{
		if      ( (L"accent1")           == sValue ) this->m_eValue = themecolorAccent1;
		else if ( (L"accent2")           == sValue ) this->m_eValue = themecolorAccent2;
		else if ( (L"accent3")           == sValue ) this->m_eValue = themecolorAccent3;
		else if ( (L"accent4")           == sValue ) this->m_eValue = themecolorAccent4;
		else if ( (L"accent5")           == sValue ) this->m_eValue = themecolorAccent5;
		else if ( (L"accent6")           == sValue ) this->m_eValue = themecolorAccent6;
		else if ( (L"background1")       == sValue ) this->m_eValue = themecolorBackground1;
		else if ( (L"background2")       == sValue ) this->m_eValue = themecolorBackground2;
		else if ( (L"dark1")             == sValue ) this->m_eValue = themecolorDark1;
		else if ( (L"dark2")             == sValue ) this->m_eValue = themecolorDark2;
		else if ( (L"followedHyperlink") == sValue ) this->m_eValue = themecolorFollowedHyperlink;
		else if ( (L"hyperlink")         == sValue ) this->m_eValue = themecolorHyperlink;
		else if ( (L"light1")            == sValue ) this->m_eValue = themecolorLight1;
		else if ( (L"light2")            == sValue ) this->m_eValue = themecolorLight2;
		else if ( (L"none")              == sValue ) this->m_eValue = themecolorNone;
		else if ( (L"text1")             == sValue ) this->m_eValue = themecolorText1;
		else if ( (L"text2")             == sValue ) this->m_eValue = themecolorText2;
		else                                          this->m_eValue = themecolorNone;

		return this->m_eValue;
	}

	std::wstring CThemeColor::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// UcharHexNumber 17.18.98 (Part 1)
	//--------------------------------------------------------------------------------

	int	CUcharHexNumber::HexToInt(int nHex, bool &bResult)
	{
		if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
		if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
		if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

		bResult = false;

		return 0;
	}

	bool CUcharHexNumber::Parse(const std::wstring &sValue)
	{
		if ( sValue.length() < 2 )
			return false;

		bool bResult = true;

		this->m_eValue  = (unsigned char)HexToInt( (int)sValue[1], bResult );
		this->m_eValue += (unsigned char)HexToInt( (int)sValue[0], bResult ) <<  4;

		return bResult;
	}

	unsigned char CUcharHexNumber::FromString(const std::wstring &sValue)
	{
		if ( !Parse(sValue) )
		{
			this->m_eValue = 0;
		}

		return this->m_eValue;
	}

	std::wstring CUcharHexNumber::ToString  () const
	{
		return XmlUtils::ToString(this->m_eValue, L"%02X");
	}

	//--------------------------------------------------------------------------------
	// Underline 17.18.99 (Part 1) 20.1.10.82
	//--------------------------------------------------------------------------------

	EUnderline CUnderline::FromString(const std::wstring &sValue)
	{
		if      ( (L"dash")            == sValue ) this->m_eValue = underlineDash;
		else if ( (L"dashDotDotHeavy") == sValue ) this->m_eValue = underlineDashDotDotHeavy;
		else if ( (L"dashDotHeavy")    == sValue ) this->m_eValue = underlineDashDotHeavy;
		else if ( (L"dashedHeavy")     == sValue ) this->m_eValue = underlineDashedHeavy;
		else if ( (L"dashLong")        == sValue ) this->m_eValue = underlineDashLong;
		else if ( (L"dashLongHeavy")   == sValue ) this->m_eValue = underlineDashLongHeavy;
		else if ( (L"dotDash")         == sValue ) this->m_eValue = underlineDotDash;
		else if ( (L"dotDotDash")      == sValue ) this->m_eValue = underlineDotDotDash;
		else if ( (L"dotted")          == sValue ) this->m_eValue = underlineDotted;
		else if ( (L"dottedHeavy")     == sValue ) this->m_eValue = underlineDottedHeavy;
		else if ( (L"double")          == sValue ) this->m_eValue = underlineDouble;
		else if ( (L"none")            == sValue ) this->m_eValue = underlineNone;
		else if ( (L"single")          == sValue ) this->m_eValue = underlineSingle;
		else if ( (L"thick")           == sValue ) this->m_eValue = underlineThick;
		else if ( (L"wave")            == sValue ) this->m_eValue = underlineWave;
		else if ( (L"wavyDouble")      == sValue ) this->m_eValue = underlineWavyDouble;
		else if ( (L"wavyHeavy")       == sValue ) this->m_eValue = underlineWavyHeavy;
		else if ( (L"words")           == sValue ) this->m_eValue = underlineWords;
		else                                        this->m_eValue = underlineNone;

		return this->m_eValue;
	}

	std::wstring CUnderline::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// VAnchor 17.18.100 (Part 1)
	//--------------------------------------------------------------------------------

	EVAnchor CVAnchor::FromString(const std::wstring &sValue)
	{
		if      ( (L"margin") == sValue ) this->m_eValue = vanchorMargin;
		else if ( (L"page")   == sValue ) this->m_eValue = vanchorPage;
		else if ( (L"text")   == sValue ) this->m_eValue = vanchorText;
		else                               this->m_eValue = vanchorText;

		return this->m_eValue;
	}

	std::wstring CVAnchor::ToString  () const
	{
		switch(this->m_eValue)
		{
		case vanchorMargin : return (L"margin");
		case vanchorPage   : return (L"page");
		case vanchorText   : return (L"text");
		default            : return (L"text");
		}
	}

	//--------------------------------------------------------------------------------
	// VerticalJc 17.18.101 (Part 1)
	//--------------------------------------------------------------------------------

	EVerticalJc CVerticalJc::FromString(const std::wstring &sValue)
	{
		if      ( (L"both")   == sValue ) this->m_eValue = verticaljcBoth;
		else if ( (L"bottom") == sValue ) this->m_eValue = verticaljcBottom;
		else if ( (L"center") == sValue ) this->m_eValue = verticaljcCenter;
		else if ( (L"top")    == sValue ) this->m_eValue = verticaljcTop;
		else                               this->m_eValue = verticaljcTop;

		return this->m_eValue;
	}

	std::wstring CVerticalJc::ToString  () const
	{
		switch(this->m_eValue)
		{
		case verticaljcBoth   : return (L"both");
		case verticaljcBottom : return (L"bottom");
		case verticaljcCenter : return (L"center");
		case verticaljcTop    : return (L"top");
		default               : return (L"top");
		}
	}

	//--------------------------------------------------------------------------------
	// View 17.18.102 (Part 1)
	//--------------------------------------------------------------------------------

	EView CView::FromString(const std::wstring &sValue)
	{
		if      ( (L"masterPages") == sValue ) this->m_eValue = viewMasterPages;
		else if ( (L"none")        == sValue ) this->m_eValue = viewNone;
		else if ( (L"normal")      == sValue ) this->m_eValue = viewNormal;
		else if ( (L"outline")     == sValue ) this->m_eValue = viewOutline;
		else if ( (L"print")       == sValue ) this->m_eValue = viewPrint;
		else if ( (L"web")         == sValue ) this->m_eValue = viewWeb;
		else                                    this->m_eValue = viewNone;

		return this->m_eValue;
	}

	std::wstring CView::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// WmlColorSchemeIndex 17.18.103 (Part 1)
	//--------------------------------------------------------------------------------

	EWmlColorSchemeIndex CWmlColorSchemeIndex::FromString(const std::wstring &sValue)
	{
		if      ( (L"accent1")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent1;
		else if ( (L"accent2")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent2;
		else if ( (L"accent3")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent3;
		else if ( (L"accent4")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent4;
		else if ( (L"accent5")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent5;
		else if ( (L"accent6")           == sValue ) this->m_eValue = wmlcolorschemeindexAccent6;
		else if ( (L"dark1")             == sValue ) this->m_eValue = wmlcolorschemeindexDark1;
		else if ( (L"dark2")             == sValue ) this->m_eValue = wmlcolorschemeindexDark2;
		else if ( (L"followedHyperlink") == sValue ) this->m_eValue = wmlcolorschemeindexFollowedHyperlink;
		else if ( (L"hyperlink")         == sValue ) this->m_eValue = wmlcolorschemeindexHyperlink;
		else if ( (L"light1")            == sValue ) this->m_eValue = wmlcolorschemeindexLight1;
		else if ( (L"light2")            == sValue ) this->m_eValue = wmlcolorschemeindexLight2;
		else                                          this->m_eValue = wmlcolorschemeindexAccent1;

		return this->m_eValue;
	}

	std::wstring CWmlColorSchemeIndex::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// Wrap 17.18.104 (Part 1)
	//--------------------------------------------------------------------------------

	EWrap CWrap::FromString(const std::wstring &sValue)
	{
		if      ( (L"around")    == sValue ) this->m_eValue = wrapAround;
		else if ( (L"auto")      == sValue ) this->m_eValue = wrapAuto;
		else if ( (L"none")      == sValue ) this->m_eValue = wrapNone;
		else if ( (L"notBeside") == sValue ) this->m_eValue = wrapNotBeside;
		else if ( (L"through")   == sValue ) this->m_eValue = wrapThrough;
		else if ( (L"tight")     == sValue ) this->m_eValue = wrapTight;
		else                                  this->m_eValue = wrapAuto;

		return this->m_eValue;
	}

	std::wstring CWrap::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// Zoom 17.18.105 (Part 1)
	//--------------------------------------------------------------------------------

	EZoom CZoom::FromString(const std::wstring &sValue)
	{
		if      ( (L"bestFit")  == sValue ) this->m_eValue = zoomBestFit;
		else if ( (L"fullPage") == sValue ) this->m_eValue = zoomFullPage;
		else if ( (L"none")     == sValue ) this->m_eValue = zoomNone;
		else if ( (L"textFit")  == sValue ) this->m_eValue = zoomTextFit;
		else                                 this->m_eValue = zoomNone;

		return this->m_eValue;
	}

	std::wstring CZoom::ToString  () const
	{
		switch(this->m_eValue)
		{
		case zoomBestFit   : return (L"bestFit");
		case zoomFullPage : return (L"fullPage");
		case zoomNone     : return (L"none");
		case zoomTextFit  : return (L"textFit");
		default           : return (L"none");
		}
	}

}; // SimpleTypes



// Дополнительные типы
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// CFontCharset 9.4.1.1 (Part 4)
	//--------------------------------------------------------------------------------

	EFontCharset CFontCharset::FromString(const std::wstring &sValue)
	{
		if      ( (L"00") == sValue ) this->m_eValue = fontcharsetANSI;
		else if ( (L"01") == sValue ) this->m_eValue = fontcharsetDefault;
		else if ( (L"02") == sValue ) this->m_eValue = fontcharsetSymbol;
		else if ( (L"4D") == sValue ) this->m_eValue = fontcharsetMacintosh;
		else if ( (L"80") == sValue ) this->m_eValue = fontcharsetShitJIS;
		else if ( (L"81") == sValue ) this->m_eValue = fontcharsetHangeul;
		else if ( (L"82") == sValue ) this->m_eValue = fontcharsetJohab;
		else if ( (L"86") == sValue ) this->m_eValue = fontcharsetGB2313;
		else if ( (L"88") == sValue ) this->m_eValue = fontcharsetChineseBig5;
		else if ( (L"A1") == sValue ) this->m_eValue = fontcharsetGreek;
		else if ( (L"A2") == sValue ) this->m_eValue = fontcharsetTurkish;
		else if ( (L"A3") == sValue ) this->m_eValue = fontcharsetVietnamese;
		else if ( (L"B1") == sValue ) this->m_eValue = fontcharsetHebrew;
		else if ( (L"B2") == sValue ) this->m_eValue = fontcharsetArabic;
		else if ( (L"BA") == sValue ) this->m_eValue = fontcharsetBaltic;
		else if ( (L"CC") == sValue ) this->m_eValue = fontcharsetRussian;
		else if ( (L"DE") == sValue ) this->m_eValue = fontcharsetThai;
		else if ( (L"EE") == sValue ) this->m_eValue = fontcharsetEastEurope;
		else if ( (L"FF") == sValue ) this->m_eValue = fontcharsetOEM;
		else                           this->m_eValue = fontcharsetANSI;

		return this->m_eValue;
	}

	std::wstring CFontCharset::ToString  () const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// CCnf 9.10.7 (Part 4)
	//--------------------------------------------------------------------------------

	void CCnf::Parse(const std::wstring &sValue)
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
				this->m_eValue ^= nMult;
			}
			else // Неправильный формат
				break;
		}
	}

	unsigned short CCnf::FromString(const std::wstring &sValue)
	{
		Parse( sValue );

		return this->m_eValue;
	}

	std::wstring CCnf::ToString  () const
	{
		std::wstring sResult;

		for( int nIndex = 0, nMult = 1; nIndex < 12; nIndex++, nMult <<= 1 )
		{
			if ( this->m_eValue & nMult )
				sResult += '1';
			else
				sResult += '0';
		}

		return sResult;
	}

	bool CCnf::IsFirstRow       () const
	{
		return (bool)( (this->m_eValue & 1) != 0 );
	}

	bool CCnf::IsLastRow        () const
	{
		return (bool)( (this->m_eValue & 2) != 0 );
	}

	bool CCnf::IsFirstColumn    () const
	{
		return (bool)( (this->m_eValue & 4) != 0 );
	}

	bool CCnf::IsLastColumn     () const
	{
		return (bool)( (this->m_eValue & 8) != 0 );
	}

	bool CCnf::IsBand1Vertical  () const
	{
		return (bool)( (this->m_eValue & 16) != 0 );
	}

	bool CCnf::IsBand2Vertical  () const
	{
		return (bool)( (this->m_eValue & 32) != 0 );
	}

	bool CCnf::IsBand1Horizontal() const
	{
		return (bool)( (this->m_eValue & 64) != 0 );
	}

	bool CCnf::IsBand2Horizontal() const
	{
		return (bool)( (this->m_eValue & 128) != 0 );
	}

	bool CCnf::IsNeCell         () const
	{
		return (bool)( (this->m_eValue & 256) != 0 );
	}

	bool CCnf::IsNwCell         () const
	{
		return (bool)( (this->m_eValue & 512) != 0 );
	}

	bool CCnf::IsSeCell         () const
	{
		return (bool)( (this->m_eValue & 1024) != 0 );
	}

	bool CCnf::IsSwCell         () const
	{
		return (bool)( (this->m_eValue & 2048) != 0 );
	}

	//--------------------------------------------------------------------------------
	// XmlSpace
	//--------------------------------------------------------------------------------

	EXmlSpace CXmlSpace::FromString(const std::wstring &sValue)
	{
		if       ( (L"default")  == sValue ) this->m_eValue = xmlspaceDefault;
		else if  ( (L"preserve") == sValue ) this->m_eValue = xmlspacePreserve;
		else                                  this->m_eValue = xmlspaceDefault;

		return this->m_eValue;
	}

	EXmlSpace CXmlSpace::FromStringA(const char* pValue)
	{
		if       ( strcmp("default", pValue)  == 0 ) this->m_eValue = xmlspaceDefault;
		else if  ( strcmp("preserve", pValue) == 0 ) this->m_eValue = xmlspacePreserve;
		else                                  this->m_eValue = xmlspaceDefault;

		return this->m_eValue;
	}

	std::wstring CXmlSpace::ToString  () const
	{
		switch(this->m_eValue)
		{
		case xmlspaceDefault  : return (L"default");
		case xmlspacePreserve : return (L"preserve");
		default               : return (L"default");
		}
	}

	//---------------------------------------------------------------------------------------------

	ECryptAlgClass CCryptAlgClass::FromString(const std::wstring &sValue)
	{
		if (L"custom" == sValue) this->m_eValue = AlgClassCustom;
		else if (L"hash" == sValue) this->m_eValue = AlgClassHash;
		else this->m_eValue = AlgClassCustom;

		return this->m_eValue;
	}

	ECryptAlgClass CCryptAlgClass::FromStringA(const char* pValue)
	{
		if (strcmp("custom", pValue) == 0) this->m_eValue = AlgClassCustom;
		else if (strcmp("hash", pValue) == 0) this->m_eValue = AlgClassHash;
		else this->m_eValue = AlgClassCustom;

		return this->m_eValue;
	}

	std::wstring CCryptAlgClass::ToString() const
	{
		switch (this->m_eValue)
		{
		case AlgClassCustom: return (L"custom");
		case AlgClassHash: return (L"hash");
		default: return (L"custom");
		}
	}

	//---------------------------------------------------------------------------------------------

	ECryptAlgType CCryptAlgType::FromString(const std::wstring &sValue)
	{
		if (L"custom" == sValue) this->m_eValue = AlgTypeCustom;
		else if (L"typeAny" == sValue) this->m_eValue = AlgTypeTypeAny;
		else this->m_eValue = AlgTypeCustom;

		return this->m_eValue;
	}

	ECryptAlgType CCryptAlgType::FromStringA(const char* pValue)
	{
		if (strcmp("custom", pValue) == 0) this->m_eValue = AlgTypeCustom;
		else if (strcmp("typeAny", pValue) == 0) this->m_eValue = AlgTypeTypeAny;
		else this->m_eValue = AlgTypeCustom;

		return this->m_eValue;
	}

	std::wstring CCryptAlgType::ToString() const
	{
		switch (this->m_eValue)
		{
		case AlgTypeCustom: return (L"custom");
		case AlgTypeTypeAny: return (L"typeAny");
		default: return (L"custom");
		}
	}

	//---------------------------------------------------------------------------------------------

	ECryptProv CCryptProv::FromString(const std::wstring &sValue)
	{
		if (L"custom" == sValue) this->m_eValue = CryptProvCustom;
		else if (L"rsaAES" == sValue) this->m_eValue = CryptProvRsaAES;
		else if (L"rsaFull" == sValue) this->m_eValue = CryptProvRsaFull;
		else this->m_eValue = CryptProvCustom;

		return this->m_eValue;
	}

	ECryptProv CCryptProv::FromStringA(const char* pValue)
	{
		if (strcmp("custom", pValue) == 0) this->m_eValue = CryptProvCustom;
		else if (strcmp("rsaAES", pValue) == 0) this->m_eValue = CryptProvRsaAES;
		else if (strcmp("rsaFull", pValue) == 0) this->m_eValue = CryptProvRsaFull;
		else this->m_eValue = CryptProvCustom;

		return this->m_eValue;
	}

	std::wstring CCryptProv::ToString() const
	{
		switch (this->m_eValue)
		{
		case CryptProvCustom: return (L"custom");
		case CryptProvRsaAES: return (L"rsaAES");
		case CryptProvRsaFull: return (L"rsaFull");
		default: return (L"custom");
		}
	}

	//--------------------------------------------------------------------------------
	// CryptAlgoritmName
	//--------------------------------------------------------------------------------

	ECryptAlgoritmName CCryptAlgoritmName::FromString(const std::wstring &sValue)
	{
		if       ( L"MD2"        == sValue || L"1"	== sValue ) this->m_eValue = cryptalgoritmnameMD2;
		else if  ( L"MD4"        == sValue || L"2"	== sValue ) this->m_eValue = cryptalgoritmnameMD4;
		else if  ( L"MD5"        == sValue || L"3"	== sValue ) this->m_eValue = cryptalgoritmnameMD5;
		else if  ( L"RIPEMD-128" == sValue || L"6"	== sValue ) this->m_eValue = cryptalgoritmnameRIPEMD128;
		else if  ( L"RIPEMD-160" == sValue || L"7"	== sValue ) this->m_eValue = cryptalgoritmnameRIPEMD160;
		else if  ( L"SHA-1"      == sValue || L"4"	== sValue ) this->m_eValue = cryptalgoritmnameSHA1;
		else if  ( L"SHA-256"    == sValue || L"12"	== sValue ) this->m_eValue = cryptalgoritmnameSHA256;
		else if  ( L"SHA-384"    == sValue || L"13"	== sValue ) this->m_eValue = cryptalgoritmnameSHA384;
		else if  ( L"SHA-512"    == sValue || L"14"	== sValue ) this->m_eValue = cryptalgoritmnameSHA512;
		else if  ( L"WHIRLPOOL"  == sValue ) this->m_eValue = cryptalgoritmnameWHIRLPOOL;
		else
			this->m_eValue = cryptalgoritmnameUnknown;

		return this->m_eValue;
	}

	std::wstring CCryptAlgoritmName::ToString() const
	{
		switch(this->m_eValue)
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

	//--------------------------------------------------------------------------------
	// PitchFamily
	//--------------------------------------------------------------------------------

	EPitchFamily CPitchFamily::FromString(const std::wstring &sValue)
	{
		CUcharHexNumber oHex = sValue;
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
			this->m_eValue = pitchfamilyDefUnk; break;
		}

		return this->m_eValue;
	}

	std::wstring CPitchFamily::ToString  () const
	{
		switch(this->m_eValue)
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

	unsigned char CPitchFamily::GetPitch()
	{
		int nValue = (int)this->m_eValue;
		return nValue & 0x0F;
	}

	unsigned char CPitchFamily::GetFamily()
	{
		int nValue = (int)this->m_eValue;
		return nValue >> 4;
	}

	//--------------------------------------------------------------------------------
	// DirVal
	//--------------------------------------------------------------------------------

	EDirVal CDirVal::FromString(const std::wstring &sValue)
	{
		if       ( (L"ltr") == sValue ) this->m_eValue = dirLtr;
		else if  ( (L"rtl") == sValue ) this->m_eValue = dirRtl;
		else							this->m_eValue = dirLtr;

		return this->m_eValue;
	}

	std::wstring CDirVal::ToString  () const
	{
		switch(this->m_eValue)
		{
		case dirLtr       : return (L"ltr");
		case dirRtl       : return (L"rtl");
		default           : return (L"ltr");
		}
	}

	ELigatures CLigatures::FromString(const std::wstring &sValue)
	{
		if (L"none" == sValue)									this->m_eValue = ligatureNone;
		else if (L"standard" == sValue)							this->m_eValue = ligatureStandard;
		else if (L"contextual" == sValue)						this->m_eValue = ligatureContextual;
		else if (L"historical" == sValue)						this->m_eValue = ligatureHistorical;
		else if (L"discretional" == sValue)						this->m_eValue = ligatureDiscretional;
		else if (L"standardContextual" == sValue)				this->m_eValue = ligatureStandardContextual;
		else if (L"standardHistorical" == sValue)				this->m_eValue = ligatureStandardHistorical;
		else if (L"contextualHistorical" == sValue)				this->m_eValue = ligatureContextualHistorical;
		else if (L"standardDiscretional" == sValue)				this->m_eValue = ligatureStandardDiscretional;
		else if (L"contextualDiscretional" == sValue)			this->m_eValue = ligatureContextualDiscretional;
		else if (L"historicalDiscretional" == sValue)			this->m_eValue = ligatureHistoricalDiscretional;
		else if (L"standardContextualHistorical" == sValue)		this->m_eValue = ligatureStandardContextualHistorical;
		else if (L"standardContextualDiscretional" == sValue)	this->m_eValue = ligatureStandardContextualDiscretional;
		else if (L"standardHistoricalDiscretional" == sValue)	this->m_eValue = ligatureStandardHistoricalDiscretional;
		else if (L"contextualHistoricalDiscretional" == sValue)	this->m_eValue = ligatureContextualHistoricalDiscretional;
		else if (L"all" == sValue)								this->m_eValue = ligatureAll;
		return this->m_eValue;
	}


	std::wstring CLigatures::ToString() const
	{
		switch (this->m_eValue)
		{
		case ligatureNone: return (L"none");
		case ligatureStandard: return (L"standard");
		case ligatureContextual: return (L"contextual");
		case ligatureHistorical: return (L"historical");
		case ligatureDiscretional: return (L"discretional");
		case ligatureStandardContextual: return (L"standardContextual");
		case ligatureStandardHistorical: return (L"standardHistorical");
		case ligatureContextualHistorical: return (L"contextualHistorical");
		case ligatureStandardDiscretional: return (L"standardDiscretional");
		case ligatureContextualDiscretional: return (L"contextualDiscretional");
		case ligatureHistoricalDiscretional: return (L"historicalDiscretional");
		case ligatureStandardContextualHistorical: return (L"standardContextualHistorical");
		case ligatureStandardContextualDiscretional: return (L"standardContextualDiscretional");
		case ligatureStandardHistoricalDiscretional: return (L"standardHistoricalDiscretional");
		case ligatureContextualHistoricalDiscretional: return (L"contextualHistoricalDiscretional");
		case ligatureAll: return (L"all");
		default: return (L"standard");
		}
	}

	ENumForm CNumForm::FromString(const std::wstring &sValue)
	{
		if (L"default" == sValue)		this->m_eValue = numFormDefault;
		else if (L"lining" == sValue)	this->m_eValue = numFormLining;
		else if (L"oldStyle" == sValue) this->m_eValue = numFormOldStyle;
		else							this->m_eValue = numFormDefault;

		return this->m_eValue;
	}

	std::wstring CNumForm::ToString() const
	{
		switch (this->m_eValue)
		{
		case numFormLining:		return L"lining";
		case numFormOldStyle:	return L"oldStyle";
		case numFormDefault:
		default:				return L"default";
		}
	}

	ENumSpacing CNumSpacing::FromString(const std::wstring &sValue)
	{
		if (L"default" == sValue) this->m_eValue = numSpacingDefault;
		else if (L"proportional" == sValue) this->m_eValue = numSpacingProportional;
		else if (L"tabular" == sValue) this->m_eValue = numSpacingTabular;
		else							this->m_eValue = numSpacingDefault;

		return this->m_eValue;
	}

	std::wstring CNumSpacing::ToString() const
	{
		switch (this->m_eValue)
		{
		case numSpacingProportional:	return L"proportional";
		case numSpacingTabular:			return L"tabular";
		case numSpacingDefault:
		default:						return L"default";
		}
	}

} // SimpleTypes
