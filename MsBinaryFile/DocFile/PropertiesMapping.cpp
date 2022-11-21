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

#include "PropertiesMapping.h"
#include "RGBColor.h"

namespace DocFileFormat
{
    std::map<unsigned char, std::wstring>	  PropertiesMapping::brcTypeMap;
    NSCriticalSection::CRITICAL_SECTION_SMART PropertiesMapping::brcTypeMapLock;
	
	void PropertiesMapping::init()
	{
		brcTypeMapLock.Enter();
		if(brcTypeMap.size() == 0)
		{
            brcTypeMap.insert( std::make_pair( 0x00, std::wstring( L"none" ) ));
            brcTypeMap.insert( std::make_pair( 0x01, std::wstring( L"single" ) ));
            brcTypeMap.insert( std::make_pair( 0x02, std::wstring( L"thick" ) ));
            brcTypeMap.insert( std::make_pair( 0x03, std::wstring( L"double" ) ));
            brcTypeMap.insert( std::make_pair( 0x05, std::wstring( L"hairline" ) ));
            brcTypeMap.insert( std::make_pair( 0x06, std::wstring( L"dotted" ) ));
            brcTypeMap.insert( std::make_pair( 0x07, std::wstring( L"dashed" ) ));
            brcTypeMap.insert( std::make_pair( 0x08, std::wstring( L"dotDash" ) ));
            brcTypeMap.insert( std::make_pair( 0x09, std::wstring( L"dotDotDash" ) ));
            brcTypeMap.insert( std::make_pair( 0x0A, std::wstring( L"triple" ) ));
            brcTypeMap.insert( std::make_pair( 0x0B, std::wstring( L"thinThickSmallGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x0C, std::wstring( L"thickThinSmallGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x0D, std::wstring( L"thinThickThinSmallGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x0E, std::wstring( L"thinThickMediumGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x0F, std::wstring( L"thickThinMediumGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x10, std::wstring( L"thinThickThinMediumGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x11, std::wstring( L"thinThickLargeGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x12, std::wstring( L"thickThinLargeGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x13, std::wstring( L"thinThickThinLargeGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x14, std::wstring( L"wave" ) ));
            brcTypeMap.insert( std::make_pair( 0x15, std::wstring( L"doubleWave" ) ));
            brcTypeMap.insert( std::make_pair( 0x16, std::wstring( L"dashSmallGap" ) ));
            brcTypeMap.insert( std::make_pair( 0x17, std::wstring( L"dashDotStroked" ) ));
            brcTypeMap.insert( std::make_pair( 0x18, std::wstring( L"threeDEmboss" ) ));
            brcTypeMap.insert( std::make_pair( 0x19, std::wstring( L"threeDEngrave" ) ));
            brcTypeMap.insert( std::make_pair( 0x1A, std::wstring( L"outset" ) ));
            brcTypeMap.insert( std::make_pair( 0x1B, std::wstring( L"inset" ) ));
            brcTypeMap.insert( std::make_pair( 0x40, std::wstring( L"apples" ) ));
            brcTypeMap.insert( std::make_pair( 0x41, std::wstring( L"archedScallops" ) ));
            brcTypeMap.insert( std::make_pair( 0x42, std::wstring( L"babyPacifier" ) ));
            brcTypeMap.insert( std::make_pair( 0x43, std::wstring( L"babyRattle" ) ));
            brcTypeMap.insert( std::make_pair( 0x44, std::wstring( L"balloons3Colors" ) ));
            brcTypeMap.insert( std::make_pair( 0x45, std::wstring( L"balloonsHotAir" ) ));
            brcTypeMap.insert( std::make_pair( 0x46, std::wstring( L"basicBlackDashes" ) ));
            brcTypeMap.insert( std::make_pair( 0x47, std::wstring( L"basicBlackDots" ) ));
            brcTypeMap.insert( std::make_pair( 0x48, std::wstring( L"basicBlackSquares" ) ));
            brcTypeMap.insert( std::make_pair( 0x49, std::wstring( L"basicThinLines" ) ));
            brcTypeMap.insert( std::make_pair( 0x4A, std::wstring( L"basicWhiteDashes" ) ));
            brcTypeMap.insert( std::make_pair( 0x4B, std::wstring( L"basicWhiteDots" ) ));
            brcTypeMap.insert( std::make_pair( 0x4C, std::wstring( L"basicWhiteSquares" ) ));
            brcTypeMap.insert( std::make_pair( 0x4D, std::wstring( L"basicWideInline" ) ));
            brcTypeMap.insert( std::make_pair( 0x4E, std::wstring( L"basicWideMidline" ) ));
            brcTypeMap.insert( std::make_pair( 0x4F, std::wstring( L"basicWideOutline" ) ));
            brcTypeMap.insert( std::make_pair( 0x50, std::wstring( L"bats" ) ));
            brcTypeMap.insert( std::make_pair( 0x51, std::wstring( L"birds" ) ));
            brcTypeMap.insert( std::make_pair( 0x52, std::wstring( L"birdsFlight" ) ));
            brcTypeMap.insert( std::make_pair( 0x53, std::wstring( L"cabins" ) ));
            brcTypeMap.insert( std::make_pair( 0x54, std::wstring( L"cakeSlice" ) ));
            brcTypeMap.insert( std::make_pair( 0x55, std::wstring( L"candyCorn" ) ));
            brcTypeMap.insert( std::make_pair( 0x56, std::wstring( L"celticKnotwork" ) ));
            brcTypeMap.insert( std::make_pair( 0x57, std::wstring( L"certificateBanner" ) ));
            brcTypeMap.insert( std::make_pair( 0x58, std::wstring( L"chainLink" ) ));
            brcTypeMap.insert( std::make_pair( 0x59, std::wstring( L"champagneBottle" ) ));
            brcTypeMap.insert( std::make_pair( 0x5A, std::wstring( L"checkedBarBlack" ) ));
            brcTypeMap.insert( std::make_pair( 0x5B, std::wstring( L"checkedBarColor" ) ));
            brcTypeMap.insert( std::make_pair( 0x5C, std::wstring( L"checkered" ) ));
            brcTypeMap.insert( std::make_pair( 0x5D, std::wstring( L"christmasTree" ) ));
            brcTypeMap.insert( std::make_pair( 0x5E, std::wstring( L"circlesLines" ) ));
            brcTypeMap.insert( std::make_pair( 0x5F, std::wstring( L"circlesRectangles" ) ));
            brcTypeMap.insert( std::make_pair( 0x60, std::wstring( L"classicalWave" ) ));
            brcTypeMap.insert( std::make_pair( 0x61, std::wstring( L"clocks" ) ));
            brcTypeMap.insert( std::make_pair( 0x62, std::wstring( L"compass" ) ));
            brcTypeMap.insert( std::make_pair( 0x63, std::wstring( L"confetti" ) ));
            brcTypeMap.insert( std::make_pair( 0x64, std::wstring( L"confettiGrays" ) ));
            brcTypeMap.insert( std::make_pair( 0x65, std::wstring( L"confettiOutline" ) ));
            brcTypeMap.insert( std::make_pair( 0x66, std::wstring( L"confettiStreamers" ) ));
            brcTypeMap.insert( std::make_pair( 0x67, std::wstring( L"confettiWhite" ) ));
            brcTypeMap.insert( std::make_pair( 0x68, std::wstring( L"cornerTriangles" ) ));
            brcTypeMap.insert( std::make_pair( 0x69, std::wstring( L"couponCutoutDashes" ) ));
            brcTypeMap.insert( std::make_pair( 0x6A, std::wstring( L"couponCutoutDots" ) ));
            brcTypeMap.insert( std::make_pair( 0x6B, std::wstring( L"crazyMaze" ) ));
            brcTypeMap.insert( std::make_pair( 0x6C, std::wstring( L"creaturesButterfly" ) ));
            brcTypeMap.insert( std::make_pair( 0x6D, std::wstring( L"creaturesFish" ) ));
            brcTypeMap.insert( std::make_pair( 0x6E, std::wstring( L"creaturesInsects" ) ));
            brcTypeMap.insert( std::make_pair( 0x6F, std::wstring( L"creaturesLadyBug" ) ));
            brcTypeMap.insert( std::make_pair( 0x70, std::wstring( L"crossStitch" ) ));
            brcTypeMap.insert( std::make_pair( 0x71, std::wstring( L"cup" ) ));
            brcTypeMap.insert( std::make_pair( 0x72, std::wstring( L"decoArch" ) ));
            brcTypeMap.insert( std::make_pair( 0x73, std::wstring( L"decoArchColor" ) ));
            brcTypeMap.insert( std::make_pair( 0x74, std::wstring( L"decoBlocks" ) ));
            brcTypeMap.insert( std::make_pair( 0x75, std::wstring( L"diamondsGray" ) ));
            brcTypeMap.insert( std::make_pair( 0x76, std::wstring( L"doubleD" ) ));
            brcTypeMap.insert( std::make_pair( 0x77, std::wstring( L"doubleDiamonds" ) ));
            brcTypeMap.insert( std::make_pair( 0x78, std::wstring( L"earth1" ) ));
            brcTypeMap.insert( std::make_pair( 0x79, std::wstring( L"earth2" ) ));
            brcTypeMap.insert( std::make_pair( 0x7A, std::wstring( L"eclipsingSquares1" ) ));
            brcTypeMap.insert( std::make_pair( 0x7B, std::wstring( L"eclipsingSquares2" ) ));
            brcTypeMap.insert( std::make_pair( 0x7C, std::wstring( L"eggsBlack" ) ));
            brcTypeMap.insert( std::make_pair( 0x7D, std::wstring( L"fans" ) ));
            brcTypeMap.insert( std::make_pair( 0x7E, std::wstring( L"film" ) ));
            brcTypeMap.insert( std::make_pair( 0x7F, std::wstring( L"firecrackers" ) ));
            brcTypeMap.insert( std::make_pair( 0x80, std::wstring( L"flowersBlockPrint" ) ));
            brcTypeMap.insert( std::make_pair( 0x81, std::wstring( L"flowersDaisies" ) ));
            brcTypeMap.insert( std::make_pair( 0x82, std::wstring( L"flowersModern1" ) ));
            brcTypeMap.insert( std::make_pair( 0x83, std::wstring( L"flowersModern2" ) ));
            brcTypeMap.insert( std::make_pair( 0x84, std::wstring( L"flowersPansy" ) ));
            brcTypeMap.insert( std::make_pair( 0x85, std::wstring( L"flowersRedRose" ) ));
            brcTypeMap.insert( std::make_pair( 0x86, std::wstring( L"flowersRoses" ) ));
            brcTypeMap.insert( std::make_pair( 0x87, std::wstring( L"flowersTeacup" ) ));
            brcTypeMap.insert( std::make_pair( 0x88, std::wstring( L"flowersTiny" ) ));
            brcTypeMap.insert( std::make_pair( 0x89, std::wstring( L"gems" ) ));
            brcTypeMap.insert( std::make_pair( 0x8A, std::wstring( L"gingerbreadMan" ) ));
            brcTypeMap.insert( std::make_pair( 0x8B, std::wstring( L"gradient" ) ));
            brcTypeMap.insert( std::make_pair( 0x8C, std::wstring( L"handmade1" ) ));
            brcTypeMap.insert( std::make_pair( 0x8D, std::wstring( L"handmade2" ) ));
            brcTypeMap.insert( std::make_pair( 0x8E, std::wstring( L"heartBalloon" ) ));
            brcTypeMap.insert( std::make_pair( 0x8F, std::wstring( L"heartGray" ) ));
            brcTypeMap.insert( std::make_pair( 0x90, std::wstring( L"hearts" ) ));
            brcTypeMap.insert( std::make_pair( 0x91, std::wstring( L"heebieJeebies" ) ));
            brcTypeMap.insert( std::make_pair( 0x92, std::wstring( L"holly" ) ));
            brcTypeMap.insert( std::make_pair( 0x93, std::wstring( L"houseFunky" ) ));
            brcTypeMap.insert( std::make_pair( 0x94, std::wstring( L"hypnotic" ) ));
            brcTypeMap.insert( std::make_pair( 0x95, std::wstring( L"iceCreamCones" ) ));
            brcTypeMap.insert( std::make_pair( 0x96, std::wstring( L"lightBulb" ) ));
            brcTypeMap.insert( std::make_pair( 0x97, std::wstring( L"lightning1" ) ));
            brcTypeMap.insert( std::make_pair( 0x98, std::wstring( L"lightning2" ) ));
            brcTypeMap.insert( std::make_pair( 0x99, std::wstring( L"mapPins" ) ));
            brcTypeMap.insert( std::make_pair( 0x9A, std::wstring( L"mapleLeaf" ) ));
            brcTypeMap.insert( std::make_pair( 0x9B, std::wstring( L"mapleMuffins" ) ));
            brcTypeMap.insert( std::make_pair( 0x9C, std::wstring( L"marquee" ) ));
            brcTypeMap.insert( std::make_pair( 0x9D, std::wstring( L"marqueeToothed" ) ));
            brcTypeMap.insert( std::make_pair( 0x9E, std::wstring( L"moons" ) ));
            brcTypeMap.insert( std::make_pair( 0x9F, std::wstring( L"mosaic" ) ));
            brcTypeMap.insert( std::make_pair( 0xA0, std::wstring( L"musicNotes" ) ));
            brcTypeMap.insert( std::make_pair( 0xA1, std::wstring( L"northwest" ) ));
            brcTypeMap.insert( std::make_pair( 0xA2, std::wstring( L"ovals" ) ));
            brcTypeMap.insert( std::make_pair( 0xA3, std::wstring( L"packages" ) ));
            brcTypeMap.insert( std::make_pair( 0xA4, std::wstring( L"palmsBlack" ) ));
            brcTypeMap.insert( std::make_pair( 0xA5, std::wstring( L"palmsColor" ) ));
            brcTypeMap.insert( std::make_pair( 0xA6, std::wstring( L"paperClips" ) ));
            brcTypeMap.insert( std::make_pair( 0xA7, std::wstring( L"papyrus" ) ));
            brcTypeMap.insert( std::make_pair( 0xA8, std::wstring( L"partyFavor" ) ));
            brcTypeMap.insert( std::make_pair( 0xA9, std::wstring( L"partyGlass" ) ));
            brcTypeMap.insert( std::make_pair( 0xAA, std::wstring( L"pencils" ) ));
            brcTypeMap.insert( std::make_pair( 0xAB, std::wstring( L"people" ) ));
            brcTypeMap.insert( std::make_pair( 0xAC, std::wstring( L"peopleWaving" ) ));
            brcTypeMap.insert( std::make_pair( 0xAD, std::wstring( L"peopleHats" ) ));
            brcTypeMap.insert( std::make_pair( 0xAE, std::wstring( L"poinsettias" ) ));
            brcTypeMap.insert( std::make_pair( 0xAF, std::wstring( L"postageStamp" ) ));
            brcTypeMap.insert( std::make_pair( 0xB0, std::wstring( L"pumpkin1" ) ));
            brcTypeMap.insert( std::make_pair( 0xB1, std::wstring( L"pushPinNote2" ) ));
            brcTypeMap.insert( std::make_pair( 0xB2, std::wstring( L"pushPinNote1" ) ));
            brcTypeMap.insert( std::make_pair( 0xB3, std::wstring( L"pyramids" ) ));
            brcTypeMap.insert( std::make_pair( 0xB4, std::wstring( L"pyramidsAbove" ) ));
            brcTypeMap.insert( std::make_pair( 0xB5, std::wstring( L"quadrants" ) ));
            brcTypeMap.insert( std::make_pair( 0xB6, std::wstring( L"rings" ) ));
            brcTypeMap.insert( std::make_pair( 0xB7, std::wstring( L"safari" ) ));
            brcTypeMap.insert( std::make_pair( 0xB8, std::wstring( L"sawtooth" ) ));
            brcTypeMap.insert( std::make_pair( 0xB9, std::wstring( L"sawtoothGray" ) ));
            brcTypeMap.insert( std::make_pair( 0xBA, std::wstring( L"scaredCat" ) ));
            brcTypeMap.insert( std::make_pair( 0xBB, std::wstring( L"seattle" ) ));
            brcTypeMap.insert( std::make_pair( 0xBC, std::wstring( L"shadowedSquares" ) ));
            brcTypeMap.insert( std::make_pair( 0xBD, std::wstring( L"sharksTeeth" ) ));
            brcTypeMap.insert( std::make_pair( 0xBE, std::wstring( L"shorebirdTracks" ) ));
            brcTypeMap.insert( std::make_pair( 0xBF, std::wstring( L"skyrocket" ) ));
            brcTypeMap.insert( std::make_pair( 0xC0, std::wstring( L"snowflakeFancy" ) ));
            brcTypeMap.insert( std::make_pair( 0xC1, std::wstring( L"snowflakes" ) ));
            brcTypeMap.insert( std::make_pair( 0xC2, std::wstring( L"sombrero" ) ));
            brcTypeMap.insert( std::make_pair( 0xC3, std::wstring( L"southwest" ) ));
            brcTypeMap.insert( std::make_pair( 0xC4, std::wstring( L"stars" ) ));
            brcTypeMap.insert( std::make_pair( 0xC5, std::wstring( L"starsTop" ) ));
            brcTypeMap.insert( std::make_pair( 0xC6, std::wstring( L"stars3d" ) ));
            brcTypeMap.insert( std::make_pair( 0xC7, std::wstring( L"starsBlack" ) ));
            brcTypeMap.insert( std::make_pair( 0xC8, std::wstring( L"starsShadowed" ) ));
            brcTypeMap.insert( std::make_pair( 0xC9, std::wstring( L"sun" ) ));
            brcTypeMap.insert( std::make_pair( 0xCA, std::wstring( L"swirligig" ) ));
            brcTypeMap.insert( std::make_pair( 0xCB, std::wstring( L"tornPaper" ) ));
            brcTypeMap.insert( std::make_pair( 0xCC, std::wstring( L"tornPaperBlack" ) ));
            brcTypeMap.insert( std::make_pair( 0xCD, std::wstring( L"trees" ) ));
            brcTypeMap.insert( std::make_pair( 0xCE, std::wstring( L"triangleParty" ) ));
            brcTypeMap.insert( std::make_pair( 0xCF, std::wstring( L"triangles" ) ));
            brcTypeMap.insert( std::make_pair( 0xD0, std::wstring( L"tribal1" ) ));
            brcTypeMap.insert( std::make_pair( 0xD1, std::wstring( L"tribal2" ) ));
            brcTypeMap.insert( std::make_pair( 0xD2, std::wstring( L"tribal3" ) ));
            brcTypeMap.insert( std::make_pair( 0xD3, std::wstring( L"tribal4" ) ));
            brcTypeMap.insert( std::make_pair( 0xD4, std::wstring( L"tribal5" ) ));
            brcTypeMap.insert( std::make_pair( 0xD5, std::wstring( L"tribal6" ) ));
            brcTypeMap.insert( std::make_pair( 0xD6, std::wstring( L"twistedLines1" ) ));
            brcTypeMap.insert( std::make_pair( 0xD7, std::wstring( L"twistedLines2" ) ));
            brcTypeMap.insert( std::make_pair( 0xD8, std::wstring( L"vine" ) ));
            brcTypeMap.insert( std::make_pair( 0xD9, std::wstring( L"waveline" ) ));
            brcTypeMap.insert( std::make_pair( 0xDA, std::wstring( L"weavingAngles" ) ));
            brcTypeMap.insert( std::make_pair( 0xDB, std::wstring( L"weavingBraid" ) ));
            brcTypeMap.insert( std::make_pair( 0xDC, std::wstring( L"weavingRibbon" ) ));
            brcTypeMap.insert( std::make_pair( 0xDD, std::wstring( L"weavingStrips" ) ));
            brcTypeMap.insert( std::make_pair( 0xDE, std::wstring( L"whiteFlowers" ) ));
            brcTypeMap.insert( std::make_pair( 0xDF, std::wstring( L"woodwork" ) ));
            brcTypeMap.insert( std::make_pair( 0xE0, std::wstring( L"xIllusions" ) ));
            brcTypeMap.insert( std::make_pair( 0xE1, std::wstring( L"zanyTriangles" ) ));
            brcTypeMap.insert( std::make_pair( 0xE2, std::wstring( L"zigZag" ) ));
            brcTypeMap.insert( std::make_pair( 0xE3, std::wstring( L"zigZagStitch" ) ));
            brcTypeMap.insert( std::make_pair( 0xFF, std::wstring( L"none" ) ));
		}
		brcTypeMapLock.Leave();
	}
	PropertiesMapping::PropertiesMapping (XMLTools::CStringXmlWriter* pWriter) : m_pXmlWriter(pWriter)
	{	    
		init();
	}

	/*========================================================================================================*/

    void PropertiesMapping::appendFlagAttribute( XMLTools::XMLElement* node, const SinglePropertyModifier& sprm, const std::wstring & attributeName )
	{
        XMLTools::XMLAttribute att( attributeName, FormatUtils::IntToWideString( sprm.Arguments[0] ));

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

    void PropertiesMapping::appendFlagElement( XMLTools::XMLElement* node, const SinglePropertyModifier& sprm, const std::wstring & elementName, bool unique )
	{
        XMLTools::XMLElement ele( L"w", elementName );

		if ( sprm.Arguments[0] == 0 )
		{
            XMLTools::XMLAttribute val( L"w:val" , L"off" );
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

    void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement* node, const std::wstring & attributeName, const std::wstring & attributeValue )
	{
        XMLTools::XMLAttribute att( attributeName, attributeValue );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

    void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement* node, const std::wstring & attributeName, int attributeValue )
	{
        XMLTools::XMLAttribute att( attributeName, FormatUtils::IntToWideString( attributeValue ));

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

    void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement* node, const std::wstring & attributeName, short attributeValue )
	{
        XMLTools::XMLAttribute att( attributeName, FormatUtils::IntToWideString( attributeValue ));

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

    void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement* node, const std::wstring & attributeName, unsigned short attributeValue )
	{
        XMLTools::XMLAttribute att( attributeName, FormatUtils::IntToWideString( attributeValue ));

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

    void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement* node, const std::wstring & attributeName, unsigned char attributeValue )
	{
        XMLTools::XMLAttribute att( attributeName, FormatUtils::IntToWideString( attributeValue ));

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueElement( XMLTools::XMLElement* node, const std::wstring & elementName, const std::wstring & elementValue, bool unique )
	{
        XMLTools::XMLElement* ele = new XMLTools::XMLElement( L"w" , elementName );

		if(!elementValue.empty())
		{
            XMLTools::XMLAttribute* val = new XMLTools::XMLAttribute( L"w:val" );

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

    void PropertiesMapping::appendValueElement( XMLTools::XMLElement* node, const std::wstring & elementName, short elementValue, bool unique )
	{
        XMLTools::XMLElement* ele = new XMLTools::XMLElement( L"w" , elementName );

		std::wstring strValue = FormatUtils::IntToWideString( elementValue );

        if ( strValue != std::wstring( L""))
		{
            XMLTools::XMLAttribute* val = new XMLTools::XMLAttribute( L"w:val", strValue);
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

    void PropertiesMapping::appendValueElement( XMLTools::XMLElement* node, const std::wstring & elementName, unsigned short elementValue, bool unique )
	{
        XMLTools::XMLElement* ele = new XMLTools::XMLElement( L"w" , elementName );

		std::wstring strValue = FormatUtils::IntToWideString( elementValue );

        if ( strValue != std::wstring( L"" ))
		{
            XMLTools::XMLAttribute* val = new XMLTools::XMLAttribute( L"w:val", strValue);
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

    void PropertiesMapping::appendValueElement( XMLTools::XMLElement* node, const std::wstring & elementName, unsigned char elementValue, bool unique )
	{
        XMLTools::XMLElement* ele = new XMLTools::XMLElement( L"w", elementName );

		std::wstring strValue = FormatUtils::IntToWideString( elementValue );

        if ( strValue != std::wstring( L"" ))
		{
            XMLTools::XMLAttribute* val = new XMLTools::XMLAttribute( L"w:val", strValue);
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

    void PropertiesMapping::appendBorderAttributes( BorderCode* brc, XMLTools::XMLElement* border )
	{
        XMLTools::XMLAttribute val( L"w:val" );

		if ( brc->fNil )
		{
            val.SetValue( L"nil" );
			border->AppendAttribute( val );
		}
		else
		{
            val.SetValue( getBorderType( brc->brcType ));
			border->AppendAttribute( val );

            XMLTools::XMLAttribute sz( L"w:sz", FormatUtils::IntToWideString( brc->dptLineWidth ));
			border->AppendAttribute( sz );
           
            XMLTools::XMLAttribute space( L"w:space" , FormatUtils::IntToWideString( brc->dptSpace ));
			border->AppendAttribute( space );
			
			XMLTools::XMLAttribute color( L"w:color" );
            color.SetValue( RGBColor( brc->cv, RedFirst ).SixDigitHexCode);
			border->AppendAttribute( color );

			if ( brc->fShadow )
			{
                XMLTools::XMLAttribute shadow( L"w:shadow" );
                shadow.SetValue( L"1" );
				border->AppendAttribute( shadow );
			}
		}
	}

	/*========================================================================================================*/

    void PropertiesMapping::appendShading( XMLTools::XMLElement* parent, const ShadingDescriptor& desc )
	{
		std::wstring pattern = getShadingPattern( desc );
        if ( ( parent != NULL ) && ( desc.shadingSpecialValue == shadingSpecialValueNormal ))
		{
            XMLTools::XMLElement shd( L"w:shd" );

			//pattern
            XMLTools::XMLAttribute val( L"w:val" );
            val.SetValue( pattern);
			shd.AppendAttribute( val );

			if (pattern != L"nil")
			{
				//fill color
				XMLTools::XMLAttribute fill( L"w:fill" );

				if ( desc.shadingType == shadingTypeShd )
				{
					if ( desc.cvBackAuto )
					{
						fill.SetValue( L"auto" );
					}
					else
					{
						fill.SetValue( RGBColor( (int)desc.cvBack, RedLast ).SixDigitHexCode);
					}
				}
				else
				{
					fill.SetValue( FormatUtils::MapValueToWideString( desc.icoBack, &Global::ColorIdentifier[0][0], 17, 12 ));
				}

				shd.AppendAttribute( fill );

				//foreground color
				XMLTools::XMLAttribute color( L"w:color" );

				if ( desc.shadingType == shadingTypeShd )
				{
					if ( desc.cvForeAuto )
					{
						color.SetValue( L"auto" );
					}
					else
					{
						color.SetValue( RGBColor( (int)desc.cvFore, RedLast ).SixDigitHexCode);
					}
				}
				else
				{
					color.SetValue( FormatUtils::MapValueToWideString( desc.icoFore, &Global::ColorIdentifier[0][0], 17, 12 ));
				}

				shd.AppendAttribute( color );
			}
            parent->RemoveChildByName( L"w:shd" );
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
        std::wstring pattern = L"";

		switch ( shd.ipat )
		{
		case Automatic:
            pattern = L"clear";
			break;

		case Solid:
            pattern = L"solid";
			break;

		case Percent_5:
            pattern = L"pct5";
			break;

		case Percent_10:
            pattern = L"pct10";
			break;

		case Percent_20:
            pattern = L"pct20";
			break;

		case Percent_25:
            pattern = L"pct25";
			break;

		case Percent_30:
            pattern = L"pct30";
			break;

		case Percent_40:
            pattern = L"pct40";
			break;

		case Percent_50:
            pattern = L"pct50";
			break;

		case Percent_60:
            pattern = L"pct60";
			break;

		case Percent_70:
            pattern = L"pct70";
			break;

		case Percent_75:
            pattern = L"pct75";
			break;

		case Percent_80:
            pattern = L"pct80";
			break;

		case Percent_90:
            pattern = L"pct90";
			break;

		case DarkHorizontal:
            pattern = L"horzStripe";
			break;

		case DarkVertical:
            pattern = L"vertStripe";
			break;

		case DarkForwardDiagonal:
            pattern = L"reverseDiagStripe";
			break;

		case DarkBackwardDiagonal:
            pattern = L"diagStripe";
			break;

		case DarkCross:
            pattern = L"horzCross";
			break;

		case DarkDiagonalCross:
            pattern = L"diagCross";
			break;

		case Horizontal:
            pattern = L"thinHorzStripe";
			break;

		case Vertical:
            pattern = L"thinVertStripe";
			break;

		case ForwardDiagonal:
            pattern = L"thinReverseDiagStripe";
			break;

		case BackwardDiagonal:
            pattern = L"thinDiagStripe";
			break;

		case Cross:
            pattern = L"thinHorzCross";
			break;

		case DiagonalCross:
            pattern = L"thinDiagCross";
			break;

		case Percent_2_5:
            pattern = L"pct5";
			break;

		case Percent_7_5:
            pattern = L"pct10";
			break;

		case Percent_12_5:
            pattern = L"pct12";
			break;

		case Percent_15:
            pattern = L"pct15";
			break;

		case Percent_17_5:
            pattern = L"pct15";
			break;

		case Percent_22_5:
            pattern = L"pct20";
			break;

		case Percent_27_5:
            pattern = L"pct30";
			break;

		case Percent_32_5:
            pattern = L"pct35";
			break;

		case Percent_35:
            pattern = L"pct35";
			break;

		case Percent_37_5:
            pattern = L"pct37";
			break;

		case Percent_42_5:
            pattern = L"pct40";
			break;

		case Percent_45:
            pattern = L"pct45";
			break;

		case Percent_47_5:
            pattern = L"pct45";
			break;

		case Percent_52_5:
            pattern = L"pct50";
			break;

		case Percent_55:
            pattern = L"pct55";
			break;

		case Percent_57_5:
            pattern = L"pct55";
			break;

		case Percent_62_5:
            pattern = L"pct62";
			break;

		case Percent_65:
            pattern = L"pct65";
			break;

		case Percent_67_5:
            pattern = L"pct65";
			break;

		case Percent_72_5:
            pattern = L"pct70";
			break;

		case Percent_77_5:
            pattern = L"pct75";
			break;

		case Percent_82_5:
            pattern = L"pct80";
			break;

		case Percent_85:
            pattern = L"pct85";
			break;

		case Percent_87_5:
            pattern = L"pct87";
			break;

		case Percent_92_5:
            pattern = L"pct90";
			break;

		case Percent_95:
            pattern = L"pct95";
			break;

		case Percent_97_5:
            pattern = L"pct95";
			break;

		case Nil:
            pattern = L"nil";
			break;

		default:
            pattern = L"nil";
			break;
		}

		return pattern;
	}

	/*========================================================================================================*/

    void PropertiesMapping::appendDxaElement( XMLTools::XMLElement* node, const std::wstring & elementName, const std::wstring & elementValue, bool unique )
	{
        XMLTools::XMLElement ele( L"w", elementName );
        XMLTools::XMLAttribute val( L"w:w", elementValue );
		ele.AppendAttribute( val );
        XMLTools::XMLAttribute type( L"w:type", L"dxa" );
		ele.AppendAttribute( type );

		if ( unique )
		{
			node->RemoveChild( ele ); 
		}

		node->AppendChild( ele );
	}

	/*========================================================================================================*/

    void PropertiesMapping::addOrSetBorder( XMLTools::XMLElement* pBdr, const XMLTools::XMLElement* border )
	{
        if ( ( pBdr != NULL ) && ( border != NULL ))
		{
			//remove old border if it exist
            pBdr->RemoveChildByName( border->GetName());

			//add new
			pBdr->AppendChild( *border );
		}
	}
}
