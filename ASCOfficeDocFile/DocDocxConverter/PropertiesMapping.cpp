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

#include "PropertiesMapping.h"

namespace DocFileFormat
{
	std::map<unsigned char, std::wstring>	PropertiesMapping::brcTypeMap;
	ASCOfficeCriticalSection				PropertiesMapping::brcTypeMapLock;
	
	void PropertiesMapping::init()
	{
		brcTypeMapLock.Enter();
		if(brcTypeMap.size() == 0)
		{
			brcTypeMap.insert( std::make_pair( 0x00, std::wstring( _T( "none" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x01, std::wstring( _T( "single" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x02, std::wstring( _T( "thick" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x03, std::wstring( _T( "double" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x05, std::wstring( _T( "hairline" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x06, std::wstring( _T( "dotted" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x07, std::wstring( _T( "dashed" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x08, std::wstring( _T( "dotDash" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x09, std::wstring( _T( "dotDotDash" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x0A, std::wstring( _T( "triple" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x0B, std::wstring( _T( "thinThickSmallGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x0C, std::wstring( _T( "thickThinSmallGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x0D, std::wstring( _T( "thinThickThinSmallGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x0E, std::wstring( _T( "thinThickMediumGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x0F, std::wstring( _T( "thickThinMediumGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x10, std::wstring( _T( "thinThickThinMediumGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x11, std::wstring( _T( "thinThickLargeGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x12, std::wstring( _T( "thickThinLargeGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x13, std::wstring( _T( "thinThickThinLargeGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x14, std::wstring( _T( "wave" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x15, std::wstring( _T( "doubleWave" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x16, std::wstring( _T( "dashSmallGap" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x17, std::wstring( _T( "dashDotStroked" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x18, std::wstring( _T( "threeDEmboss" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x19, std::wstring( _T( "threeDEngrave" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x1A, std::wstring( _T( "outset" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x1B, std::wstring( _T( "inset" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x40, std::wstring( _T( "apples" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x41, std::wstring( _T( "archedScallops" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x42, std::wstring( _T( "babyPacifier" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x43, std::wstring( _T( "babyRattle" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x44, std::wstring( _T( "balloons3Colors" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x45, std::wstring( _T( "balloonsHotAir" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x46, std::wstring( _T( "basicBlackDashes" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x47, std::wstring( _T( "basicBlackDots" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x48, std::wstring( _T( "basicBlackSquares" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x49, std::wstring( _T( "basicThinLines" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x4A, std::wstring( _T( "basicWhiteDashes" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x4B, std::wstring( _T( "basicWhiteDots" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x4C, std::wstring( _T( "basicWhiteSquares" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x4D, std::wstring( _T( "basicWideInline" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x4E, std::wstring( _T( "basicWideMidline" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x4F, std::wstring( _T( "basicWideOutline" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x50, std::wstring( _T( "bats" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x51, std::wstring( _T( "birds" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x52, std::wstring( _T( "birdsFlight" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x53, std::wstring( _T( "cabins" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x54, std::wstring( _T( "cakeSlice" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x55, std::wstring( _T( "candyCorn" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x56, std::wstring( _T( "celticKnotwork" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x57, std::wstring( _T( "certificateBanner" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x58, std::wstring( _T( "chainLink" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x59, std::wstring( _T( "champagneBottle" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x5A, std::wstring( _T( "checkedBarBlack" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x5B, std::wstring( _T( "checkedBarColor" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x5C, std::wstring( _T( "checkered" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x5D, std::wstring( _T( "christmasTree" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x5E, std::wstring( _T( "circlesLines" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x5F, std::wstring( _T( "circlesRectangles" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x60, std::wstring( _T( "classicalWave" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x61, std::wstring( _T( "clocks" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x62, std::wstring( _T( "compass" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x63, std::wstring( _T( "confetti" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x64, std::wstring( _T( "confettiGrays" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x65, std::wstring( _T( "confettiOutline" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x66, std::wstring( _T( "confettiStreamers" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x67, std::wstring( _T( "confettiWhite" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x68, std::wstring( _T( "cornerTriangles" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x69, std::wstring( _T( "couponCutoutDashes" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x6A, std::wstring( _T( "couponCutoutDots" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x6B, std::wstring( _T( "crazyMaze" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x6C, std::wstring( _T( "creaturesButterfly" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x6D, std::wstring( _T( "creaturesFish" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x6E, std::wstring( _T( "creaturesInsects" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x6F, std::wstring( _T( "creaturesLadyBug" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x70, std::wstring( _T( "crossStitch" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x71, std::wstring( _T( "cup" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x72, std::wstring( _T( "decoArch" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x73, std::wstring( _T( "decoArchColor" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x74, std::wstring( _T( "decoBlocks" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x75, std::wstring( _T( "diamondsGray" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x76, std::wstring( _T( "doubleD" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x77, std::wstring( _T( "doubleDiamonds" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x78, std::wstring( _T( "earth1" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x79, std::wstring( _T( "earth2" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x7A, std::wstring( _T( "eclipsingSquares1" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x7B, std::wstring( _T( "eclipsingSquares2" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x7C, std::wstring( _T( "eggsBlack" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x7D, std::wstring( _T( "fans" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x7E, std::wstring( _T( "film" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x7F, std::wstring( _T( "firecrackers" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x80, std::wstring( _T( "flowersBlockPrint" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x81, std::wstring( _T( "flowersDaisies" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x82, std::wstring( _T( "flowersModern1" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x83, std::wstring( _T( "flowersModern2" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x84, std::wstring( _T( "flowersPansy" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x85, std::wstring( _T( "flowersRedRose" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x86, std::wstring( _T( "flowersRoses" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x87, std::wstring( _T( "flowersTeacup" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x88, std::wstring( _T( "flowersTiny" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x89, std::wstring( _T( "gems" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x8A, std::wstring( _T( "gingerbreadMan" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x8B, std::wstring( _T( "gradient" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x8C, std::wstring( _T( "handmade1" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x8D, std::wstring( _T( "handmade2" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x8E, std::wstring( _T( "heartBalloon" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x8F, std::wstring( _T( "heartGray" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x90, std::wstring( _T( "hearts" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x91, std::wstring( _T( "heebieJeebies" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x92, std::wstring( _T( "holly" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x93, std::wstring( _T( "houseFunky" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x94, std::wstring( _T( "hypnotic" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x95, std::wstring( _T( "iceCreamCones" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x96, std::wstring( _T( "lightBulb" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x97, std::wstring( _T( "lightning1" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x98, std::wstring( _T( "lightning2" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x99, std::wstring( _T( "mapPins" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x9A, std::wstring( _T( "mapleLeaf" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x9B, std::wstring( _T( "mapleMuffins" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x9C, std::wstring( _T( "marquee" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x9D, std::wstring( _T( "marqueeToothed" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x9E, std::wstring( _T( "moons" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0x9F, std::wstring( _T( "mosaic" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA0, std::wstring( _T( "musicNotes" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA1, std::wstring( _T( "northwest" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA2, std::wstring( _T( "ovals" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA3, std::wstring( _T( "packages" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA4, std::wstring( _T( "palmsBlack" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA5, std::wstring( _T( "palmsColor" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA6, std::wstring( _T( "paperClips" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA7, std::wstring( _T( "papyrus" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA8, std::wstring( _T( "partyFavor" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xA9, std::wstring( _T( "partyGlass" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xAA, std::wstring( _T( "pencils" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xAB, std::wstring( _T( "people" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xAC, std::wstring( _T( "peopleWaving" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xAD, std::wstring( _T( "peopleHats" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xAE, std::wstring( _T( "poinsettias" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xAF, std::wstring( _T( "postageStamp" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB0, std::wstring( _T( "pumpkin1" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB1, std::wstring( _T( "pushPinNote2" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB2, std::wstring( _T( "pushPinNote1" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB3, std::wstring( _T( "pyramids" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB4, std::wstring( _T( "pyramidsAbove" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB5, std::wstring( _T( "quadrants" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB6, std::wstring( _T( "rings" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB7, std::wstring( _T( "safari" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB8, std::wstring( _T( "sawtooth" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xB9, std::wstring( _T( "sawtoothGray" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xBA, std::wstring( _T( "scaredCat" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xBB, std::wstring( _T( "seattle" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xBC, std::wstring( _T( "shadowedSquares" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xBD, std::wstring( _T( "sharksTeeth" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xBE, std::wstring( _T( "shorebirdTracks" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xBF, std::wstring( _T( "skyrocket" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC0, std::wstring( _T( "snowflakeFancy" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC1, std::wstring( _T( "snowflakes" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC2, std::wstring( _T( "sombrero" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC3, std::wstring( _T( "southwest" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC4, std::wstring( _T( "stars" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC5, std::wstring( _T( "starsTop" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC6, std::wstring( _T( "stars3d" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC7, std::wstring( _T( "starsBlack" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC8, std::wstring( _T( "starsShadowed" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xC9, std::wstring( _T( "sun" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xCA, std::wstring( _T( "swirligig" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xCB, std::wstring( _T( "tornPaper" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xCC, std::wstring( _T( "tornPaperBlack" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xCD, std::wstring( _T( "trees" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xCE, std::wstring( _T( "triangleParty" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xCF, std::wstring( _T( "triangles" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD0, std::wstring( _T( "tribal1" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD1, std::wstring( _T( "tribal2" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD2, std::wstring( _T( "tribal3" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD3, std::wstring( _T( "tribal4" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD4, std::wstring( _T( "tribal5" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD5, std::wstring( _T( "tribal6" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD6, std::wstring( _T( "twistedLines1" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD7, std::wstring( _T( "twistedLines2" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD8, std::wstring( _T( "vine" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xD9, std::wstring( _T( "waveline" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xDA, std::wstring( _T( "weavingAngles" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xDB, std::wstring( _T( "weavingBraid" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xDC, std::wstring( _T( "weavingRibbon" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xDD, std::wstring( _T( "weavingStrips" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xDE, std::wstring( _T( "whiteFlowers" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xDF, std::wstring( _T( "woodwork" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xE0, std::wstring( _T( "xIllusions" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xE1, std::wstring( _T( "zanyTriangles" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xE2, std::wstring( _T( "zigZag" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xE3, std::wstring( _T( "zigZagStitch" ) ) ) );
			brcTypeMap.insert( std::make_pair( 0xFF, std::wstring( _T( "none" ) ) ) );
		}
		brcTypeMapLock.Leave();
	}
	PropertiesMapping::PropertiesMapping (XMLTools::CStringXmlWriter* pWriter) : m_pXmlWriter(pWriter)
	{	    
		init();
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendFlagAttribute( XMLTools::XMLElement<wchar_t>* node, const SinglePropertyModifier& sprm, const wchar_t* attributeName )
	{
		XMLTools::XMLAttribute<wchar_t> att( attributeName, FormatUtils::IntToWideString( sprm.Arguments[0] ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendFlagElement( XMLTools::XMLElement<wchar_t>* node, const SinglePropertyModifier& sprm, const wchar_t* elementName, bool unique )
	{
		XMLTools::XMLElement<wchar_t> ele( _T( "w" ), elementName );

		if ( sprm.Arguments[0] == 0 )
		{
			XMLTools::XMLAttribute<wchar_t> val( _T( "w:val" ), _T( "off" ) );
			ele.AppendAttribute( val );
		}

		// !!!TODO!!!
		if ( unique )
		{
			node->RemoveChild( ele );    
		}

		node->AppendChild( ele );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, const wchar_t* attributeValue )
	{
		XMLTools::XMLAttribute<wchar_t> att( attributeName, attributeValue );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, int attributeValue )
	{
		XMLTools::XMLAttribute<wchar_t> att( attributeName, FormatUtils::IntToWideString( attributeValue ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, short attributeValue )
	{
		XMLTools::XMLAttribute<wchar_t> att( attributeName, FormatUtils::IntToWideString( attributeValue ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, unsigned short attributeValue )
	{
		XMLTools::XMLAttribute<wchar_t> att( attributeName, FormatUtils::IntToWideString( attributeValue ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, unsigned char attributeValue )
	{
		XMLTools::XMLAttribute<wchar_t> att( attributeName, FormatUtils::IntToWideString( attributeValue ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, const wchar_t* elementValue, bool unique )
	{
		XMLTools::XMLElement<wchar_t>* ele = new XMLTools::XMLElement<wchar_t>( _T( "w" ), elementName );

		if( ( elementValue != NULL ) && ( wcscmp( elementValue, _T( "" ) ) != 0 ) )
		{
			XMLTools::XMLAttribute<wchar_t>* val = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ) );

			val->SetValue( elementValue );

			ele->AppendAttribute( *val );

			RELEASEOBJECT( val );
		}

		// !!!TODO!!!
		if ( unique )
		{
			node->RemoveChild( *ele );
		}

		node->AppendChild( *ele );

		RELEASEOBJECT( ele );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, short elementValue, bool unique )
	{
		XMLTools::XMLElement<wchar_t>* ele = new XMLTools::XMLElement<wchar_t>( _T( "w" ), elementName );

		std::wstring strValue = FormatUtils::IntToWideString( elementValue );

		if ( strValue != std::wstring( _T( "" ) ) )
		{
			XMLTools::XMLAttribute<wchar_t>* val = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ), strValue.c_str() );
			ele->AppendAttribute( *val );
			RELEASEOBJECT( val );
		}

		// !!!TODO!!!
		if ( unique )
		{
			node->RemoveChild( *ele );
		}

		node->AppendChild( *ele );

		RELEASEOBJECT( ele );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, unsigned short elementValue, bool unique )
	{
		XMLTools::XMLElement<wchar_t>* ele = new XMLTools::XMLElement<wchar_t>( _T( "w" ), elementName );

		std::wstring strValue = FormatUtils::IntToWideString( elementValue );

		if ( strValue != std::wstring( _T( "" ) ) )
		{
			XMLTools::XMLAttribute<wchar_t>* val = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ), strValue.c_str() );
			ele->AppendAttribute( *val );
			RELEASEOBJECT( val );
		}

		// !!!TODO!!!
		if ( unique )
		{
			node->RemoveChild( *ele );
		}

		node->AppendChild( *ele );

		RELEASEOBJECT( ele );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, unsigned char elementValue, bool unique )
	{
		XMLTools::XMLElement<wchar_t>* ele = new XMLTools::XMLElement<wchar_t>( _T( "w" ), elementName );

		std::wstring strValue = FormatUtils::IntToWideString( elementValue );

		if ( strValue != std::wstring( _T( "" ) ) )
		{
			XMLTools::XMLAttribute<wchar_t>* val = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ), strValue.c_str() );
			ele->AppendAttribute( *val );
			RELEASEOBJECT( val );
		}

		// !!!TODO!!!
		if ( unique )
		{
			node->RemoveChild( *ele );
		}

		node->AppendChild( *ele );

		RELEASEOBJECT( ele );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendBorderAttributes( BorderCode* brc, XMLTools::XMLElement<wchar_t>* border )
	{
		XMLTools::XMLAttribute<wchar_t> val( _T( "w:val" ) );

		if ( brc->fNil )
		{
			val.SetValue( _T( "nil" ) );
			border->AppendAttribute( val );
		}
		else
		{
			val.SetValue( getBorderType( brc->brcType ).c_str() );
			border->AppendAttribute( val );

			XMLTools::XMLAttribute<wchar_t> color( _T( "w:color" ) );
			color.SetValue( RGBColor( brc->cv, RedFirst ).SixDigitHexCode.c_str() );
			border->AppendAttribute( color );

			XMLTools::XMLAttribute<wchar_t> space( _T( "w:space" ), FormatUtils::IntToWideString( brc->dptSpace ).c_str() ); 
			border->AppendAttribute( space );

			XMLTools::XMLAttribute<wchar_t> sz( _T( "w:sz" ), FormatUtils::IntToWideString( brc->dptLineWidth ).c_str() ); 
			border->AppendAttribute( sz );

			if ( brc->fShadow )
			{
				XMLTools::XMLAttribute<wchar_t> shadow( _T( "w:shadow" ) );
				shadow.SetValue( _T( "1" ) );
				border->AppendAttribute( shadow );
			}
		}
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendShading( XMLTools::XMLElement<wchar_t>* parent, const ShadingDescriptor& desc )
	{
		if ( ( parent != NULL ) && ( desc.shadingSpecialValue == shadingSpecialValueNormal ) )
		{
			XMLTools::XMLElement<wchar_t> shd( _T( "w:shd" ) );

			//fill color
			XMLTools::XMLAttribute<wchar_t> fill( _T( "w:fill" ) );

			if ( desc.shadingType == shadingTypeShd )
			{
				if ( desc.cvBackAuto )
				{
					fill.SetValue( _T( "auto" ) );
				}
				else
				{
					fill.SetValue( RGBColor( (int)desc.cvBack, RedLast ).SixDigitHexCode.c_str() );
				}
			}
			else
			{
				fill.SetValue( FormatUtils::MapValueToWideString( desc.icoBack, &Global::ColorIdentifier[0][0], 17, 12 ).c_str() ); 
			}

			shd.AppendAttribute( fill );

			//foreground color
			XMLTools::XMLAttribute<wchar_t> color( _T( "w:color" ) );

			if ( desc.shadingType == shadingTypeShd )
			{
				if ( desc.cvForeAuto )
				{
					color.SetValue( _T( "auto" ) );
				}
				else
				{
					color.SetValue( RGBColor( (int)desc.cvFore, RedLast ).SixDigitHexCode.c_str() );
				}
			}
			else
			{
				color.SetValue( FormatUtils::MapValueToWideString( desc.icoFore, &Global::ColorIdentifier[0][0], 17, 12 ).c_str() );
			}

			shd.AppendAttribute( color );

			//pattern
			XMLTools::XMLAttribute<wchar_t> val( _T( "w:val" ) );
			val.SetValue( getShadingPattern( desc ).c_str() );
			shd.AppendAttribute( val );

			parent->RemoveChildByName( _T( "w:shd" ) );
			parent->AppendChild( shd );
		}
	}

	/*========================================================================================================*/

	std::wstring PropertiesMapping::getBorderType( unsigned char type )
	{
		return this->brcTypeMap[type];
	}

	/*========================================================================================================*/

	std::wstring PropertiesMapping::getShadingPattern( const ShadingDescriptor& shd )
	{
		std::wstring pattern = _T( "" );

		switch ( shd.ipat )
		{
		case Automatic:
			pattern = _T( "clear" );
			break;

		case Solid:
			pattern = _T( "solid" );
			break;

		case Percent_5:
			pattern = _T( "pct5" );
			break;

		case Percent_10:
			pattern = _T( "pct10" );
			break;

		case Percent_20:
			pattern = _T( "pct20" );
			break;

		case Percent_25:
			pattern = _T( "pct25" );
			break;

		case Percent_30:
			pattern = _T( "pct30" );
			break;

		case Percent_40:
			pattern = _T( "pct40" );
			break;

		case Percent_50:
			pattern = _T( "pct50" );
			break;

		case Percent_60:
			pattern = _T( "pct60" );
			break;

		case Percent_70:
			pattern = _T( "pct70" );
			break;

		case Percent_75:
			pattern = _T( "pct75" );
			break;

		case Percent_80:
			pattern = _T( "pct80" );
			break;

		case Percent_90:
			pattern = _T( "pct90" );
			break;

		case DarkHorizontal:
			pattern = _T( "horzStripe" );
			break;

		case DarkVertical:
			pattern = _T( "vertStripe" );
			break;

		case DarkForwardDiagonal:
			pattern = _T( "reverseDiagStripe" );
			break;

		case DarkBackwardDiagonal:
			pattern = _T( "diagStripe" );
			break;

		case DarkCross:
			pattern = _T( "horzCross" );
			break;

		case DarkDiagonalCross:
			pattern = _T( "diagCross" );
			break;

		case Horizontal:
			pattern = _T( "thinHorzStripe" );
			break;

		case Vertical:
			pattern = _T( "thinVertStripe" );
			break;

		case ForwardDiagonal:
			pattern = _T( "thinReverseDiagStripe" );
			break;

		case BackwardDiagonal:
			pattern = _T( "thinDiagStripe" );
			break;

		case Cross:
			pattern = _T( "thinHorzCross" );
			break;

		case DiagonalCross:
			pattern = _T( "thinDiagCross" );
			break;

		case Percent_2_5:
			pattern = _T( "pct5" );
			break;

		case Percent_7_5:
			pattern = _T( "pct10" );
			break;

		case Percent_12_5:
			pattern = _T( "pct12" );
			break;

		case Percent_15:
			pattern = _T( "pct15" );
			break;

		case Percent_17_5:
			pattern = _T( "pct15" );
			break;

		case Percent_22_5:
			pattern = _T( "pct20" );
			break;

		case Percent_27_5:
			pattern = _T( "pct30" );
			break;

		case Percent_32_5:
			pattern = _T( "pct35" );
			break;

		case Percent_35:
			pattern = _T( "pct35" );
			break;

		case Percent_37_5:
			pattern = _T( "pct37" );
			break;

		case Percent_42_5:
			pattern = _T( "pct40" );
			break;

		case Percent_45:
			pattern = _T( "pct45" );
			break;

		case Percent_47_5:
			pattern = _T( "pct45" );
			break;

		case Percent_52_5:
			pattern = _T( "pct50" );
			break;

		case Percent_55:
			pattern = _T( "pct55" );
			break;

		case Percent_57_5:
			pattern = _T( "pct55" );
			break;

		case Percent_62_5:
			pattern = _T( "pct62" );
			break;

		case Percent_65:
			pattern = _T( "pct65" );
			break;

		case Percent_67_5:
			pattern = _T( "pct65" );
			break;

		case Percent_72_5:
			pattern = _T( "pct70" );
			break;

		case Percent_77_5:
			pattern = _T( "pct75" );
			break;

		case Percent_82_5:
			pattern = _T( "pct80" );
			break;

		case Percent_85:
			pattern = _T( "pct85" );
			break;

		case Percent_87_5:
			pattern = _T( "pct87" );
			break;

		case Percent_92_5:
			pattern = _T( "pct90" );
			break;

		case Percent_95:
			pattern = _T( "pct95" );
			break;

		case Percent_97_5:
			pattern = _T( "pct95" );
			break;

		case Nil:
			pattern = _T( "nil" );
			break;

		default:
			pattern = _T( "nil" );
			break;
		}

		return pattern;
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendDxaElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, const wchar_t* elementValue, bool unique )
	{
		XMLTools::XMLElement<wchar_t> ele( _T( "w" ), elementName );
		XMLTools::XMLAttribute<wchar_t> val( _T( "w:w" ), elementValue );
		ele.AppendAttribute( val );
		XMLTools::XMLAttribute<wchar_t> type( _T( "w:type" ), _T( "dxa" ) );
		ele.AppendAttribute( type );

		if ( unique )
		{
			node->RemoveChild( ele ); 
		}

		node->AppendChild( ele );
	}

	/*========================================================================================================*/

	void PropertiesMapping::addOrSetBorder( XMLTools::XMLElement<wchar_t>* pBdr, const XMLTools::XMLElement<wchar_t>* border )
	{
		if ( ( pBdr != NULL ) && ( border != NULL ) )
		{
			//remove old border if it exist
			pBdr->RemoveChildByName( border->GetName() );

			//add new
			pBdr->AppendChild( *border );
		}
	}
}