#include "stdafx.h"
#include "PropertiesMapping.h"

namespace DocFileFormat
{
	map<byte, wstring> PropertiesMapping::brcTypeMap;
	AVSOfficeCriticalSection PropertiesMapping::brcTypeMapLock;
	void PropertiesMapping::init()
	{
		brcTypeMapLock.Enter();
		if(brcTypeMap.size() == 0)
		{
			brcTypeMap.insert( make_pair( 0x00, wstring( _T( "none" ) ) ) );
			brcTypeMap.insert( make_pair( 0x01, wstring( _T( "single" ) ) ) );
			brcTypeMap.insert( make_pair( 0x02, wstring( _T( "thick" ) ) ) );
			brcTypeMap.insert( make_pair( 0x03, wstring( _T( "double" ) ) ) );
			brcTypeMap.insert( make_pair( 0x05, wstring( _T( "hairline" ) ) ) );
			brcTypeMap.insert( make_pair( 0x06, wstring( _T( "dotted" ) ) ) );
			brcTypeMap.insert( make_pair( 0x07, wstring( _T( "dashed" ) ) ) );
			brcTypeMap.insert( make_pair( 0x08, wstring( _T( "dotDash" ) ) ) );
			brcTypeMap.insert( make_pair( 0x09, wstring( _T( "dotDotDash" ) ) ) );
			brcTypeMap.insert( make_pair( 0x0A, wstring( _T( "triple" ) ) ) );
			brcTypeMap.insert( make_pair( 0x0B, wstring( _T( "thinThickSmallGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x0C, wstring( _T( "thickThinSmallGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x0D, wstring( _T( "thinThickThinSmallGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x0E, wstring( _T( "thinThickMediumGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x0F, wstring( _T( "thickThinMediumGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x10, wstring( _T( "thinThickThinMediumGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x11, wstring( _T( "thinThickLargeGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x12, wstring( _T( "thickThinLargeGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x13, wstring( _T( "thinThickThinLargeGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x14, wstring( _T( "wave" ) ) ) );
			brcTypeMap.insert( make_pair( 0x15, wstring( _T( "doubleWave" ) ) ) );
			brcTypeMap.insert( make_pair( 0x16, wstring( _T( "dashSmallGap" ) ) ) );
			brcTypeMap.insert( make_pair( 0x17, wstring( _T( "dashDotStroked" ) ) ) );
			brcTypeMap.insert( make_pair( 0x18, wstring( _T( "threeDEmboss" ) ) ) );
			brcTypeMap.insert( make_pair( 0x19, wstring( _T( "threeDEngrave" ) ) ) );
			brcTypeMap.insert( make_pair( 0x1A, wstring( _T( "outset" ) ) ) );
			brcTypeMap.insert( make_pair( 0x1B, wstring( _T( "inset" ) ) ) );
			brcTypeMap.insert( make_pair( 0x40, wstring( _T( "apples" ) ) ) );
			brcTypeMap.insert( make_pair( 0x41, wstring( _T( "archedScallops" ) ) ) );
			brcTypeMap.insert( make_pair( 0x42, wstring( _T( "babyPacifier" ) ) ) );
			brcTypeMap.insert( make_pair( 0x43, wstring( _T( "babyRattle" ) ) ) );
			brcTypeMap.insert( make_pair( 0x44, wstring( _T( "balloons3Colors" ) ) ) );
			brcTypeMap.insert( make_pair( 0x45, wstring( _T( "balloonsHotAir" ) ) ) );
			brcTypeMap.insert( make_pair( 0x46, wstring( _T( "basicBlackDashes" ) ) ) );
			brcTypeMap.insert( make_pair( 0x47, wstring( _T( "basicBlackDots" ) ) ) );
			brcTypeMap.insert( make_pair( 0x48, wstring( _T( "basicBlackSquares" ) ) ) );
			brcTypeMap.insert( make_pair( 0x49, wstring( _T( "basicThinLines" ) ) ) );
			brcTypeMap.insert( make_pair( 0x4A, wstring( _T( "basicWhiteDashes" ) ) ) );
			brcTypeMap.insert( make_pair( 0x4B, wstring( _T( "basicWhiteDots" ) ) ) );
			brcTypeMap.insert( make_pair( 0x4C, wstring( _T( "basicWhiteSquares" ) ) ) );
			brcTypeMap.insert( make_pair( 0x4D, wstring( _T( "basicWideInline" ) ) ) );
			brcTypeMap.insert( make_pair( 0x4E, wstring( _T( "basicWideMidline" ) ) ) );
			brcTypeMap.insert( make_pair( 0x4F, wstring( _T( "basicWideOutline" ) ) ) );
			brcTypeMap.insert( make_pair( 0x50, wstring( _T( "bats" ) ) ) );
			brcTypeMap.insert( make_pair( 0x51, wstring( _T( "birds" ) ) ) );
			brcTypeMap.insert( make_pair( 0x52, wstring( _T( "birdsFlight" ) ) ) );
			brcTypeMap.insert( make_pair( 0x53, wstring( _T( "cabins" ) ) ) );
			brcTypeMap.insert( make_pair( 0x54, wstring( _T( "cakeSlice" ) ) ) );
			brcTypeMap.insert( make_pair( 0x55, wstring( _T( "candyCorn" ) ) ) );
			brcTypeMap.insert( make_pair( 0x56, wstring( _T( "celticKnotwork" ) ) ) );
			brcTypeMap.insert( make_pair( 0x57, wstring( _T( "certificateBanner" ) ) ) );
			brcTypeMap.insert( make_pair( 0x58, wstring( _T( "chainLink" ) ) ) );
			brcTypeMap.insert( make_pair( 0x59, wstring( _T( "champagneBottle" ) ) ) );
			brcTypeMap.insert( make_pair( 0x5A, wstring( _T( "checkedBarBlack" ) ) ) );
			brcTypeMap.insert( make_pair( 0x5B, wstring( _T( "checkedBarColor" ) ) ) );
			brcTypeMap.insert( make_pair( 0x5C, wstring( _T( "checkered" ) ) ) );
			brcTypeMap.insert( make_pair( 0x5D, wstring( _T( "christmasTree" ) ) ) );
			brcTypeMap.insert( make_pair( 0x5E, wstring( _T( "circlesLines" ) ) ) );
			brcTypeMap.insert( make_pair( 0x5F, wstring( _T( "circlesRectangles" ) ) ) );
			brcTypeMap.insert( make_pair( 0x60, wstring( _T( "classicalWave" ) ) ) );
			brcTypeMap.insert( make_pair( 0x61, wstring( _T( "clocks" ) ) ) );
			brcTypeMap.insert( make_pair( 0x62, wstring( _T( "compass" ) ) ) );
			brcTypeMap.insert( make_pair( 0x63, wstring( _T( "confetti" ) ) ) );
			brcTypeMap.insert( make_pair( 0x64, wstring( _T( "confettiGrays" ) ) ) );
			brcTypeMap.insert( make_pair( 0x65, wstring( _T( "confettiOutline" ) ) ) );
			brcTypeMap.insert( make_pair( 0x66, wstring( _T( "confettiStreamers" ) ) ) );
			brcTypeMap.insert( make_pair( 0x67, wstring( _T( "confettiWhite" ) ) ) );
			brcTypeMap.insert( make_pair( 0x68, wstring( _T( "cornerTriangles" ) ) ) );
			brcTypeMap.insert( make_pair( 0x69, wstring( _T( "couponCutoutDashes" ) ) ) );
			brcTypeMap.insert( make_pair( 0x6A, wstring( _T( "couponCutoutDots" ) ) ) );
			brcTypeMap.insert( make_pair( 0x6B, wstring( _T( "crazyMaze" ) ) ) );
			brcTypeMap.insert( make_pair( 0x6C, wstring( _T( "creaturesButterfly" ) ) ) );
			brcTypeMap.insert( make_pair( 0x6D, wstring( _T( "creaturesFish" ) ) ) );
			brcTypeMap.insert( make_pair( 0x6E, wstring( _T( "creaturesInsects" ) ) ) );
			brcTypeMap.insert( make_pair( 0x6F, wstring( _T( "creaturesLadyBug" ) ) ) );
			brcTypeMap.insert( make_pair( 0x70, wstring( _T( "crossStitch" ) ) ) );
			brcTypeMap.insert( make_pair( 0x71, wstring( _T( "cup" ) ) ) );
			brcTypeMap.insert( make_pair( 0x72, wstring( _T( "decoArch" ) ) ) );
			brcTypeMap.insert( make_pair( 0x73, wstring( _T( "decoArchColor" ) ) ) );
			brcTypeMap.insert( make_pair( 0x74, wstring( _T( "decoBlocks" ) ) ) );
			brcTypeMap.insert( make_pair( 0x75, wstring( _T( "diamondsGray" ) ) ) );
			brcTypeMap.insert( make_pair( 0x76, wstring( _T( "doubleD" ) ) ) );
			brcTypeMap.insert( make_pair( 0x77, wstring( _T( "doubleDiamonds" ) ) ) );
			brcTypeMap.insert( make_pair( 0x78, wstring( _T( "earth1" ) ) ) );
			brcTypeMap.insert( make_pair( 0x79, wstring( _T( "earth2" ) ) ) );
			brcTypeMap.insert( make_pair( 0x7A, wstring( _T( "eclipsingSquares1" ) ) ) );
			brcTypeMap.insert( make_pair( 0x7B, wstring( _T( "eclipsingSquares2" ) ) ) );
			brcTypeMap.insert( make_pair( 0x7C, wstring( _T( "eggsBlack" ) ) ) );
			brcTypeMap.insert( make_pair( 0x7D, wstring( _T( "fans" ) ) ) );
			brcTypeMap.insert( make_pair( 0x7E, wstring( _T( "film" ) ) ) );
			brcTypeMap.insert( make_pair( 0x7F, wstring( _T( "firecrackers" ) ) ) );
			brcTypeMap.insert( make_pair( 0x80, wstring( _T( "flowersBlockPrint" ) ) ) );
			brcTypeMap.insert( make_pair( 0x81, wstring( _T( "flowersDaisies" ) ) ) );
			brcTypeMap.insert( make_pair( 0x82, wstring( _T( "flowersModern1" ) ) ) );
			brcTypeMap.insert( make_pair( 0x83, wstring( _T( "flowersModern2" ) ) ) );
			brcTypeMap.insert( make_pair( 0x84, wstring( _T( "flowersPansy" ) ) ) );
			brcTypeMap.insert( make_pair( 0x85, wstring( _T( "flowersRedRose" ) ) ) );
			brcTypeMap.insert( make_pair( 0x86, wstring( _T( "flowersRoses" ) ) ) );
			brcTypeMap.insert( make_pair( 0x87, wstring( _T( "flowersTeacup" ) ) ) );
			brcTypeMap.insert( make_pair( 0x88, wstring( _T( "flowersTiny" ) ) ) );
			brcTypeMap.insert( make_pair( 0x89, wstring( _T( "gems" ) ) ) );
			brcTypeMap.insert( make_pair( 0x8A, wstring( _T( "gingerbreadMan" ) ) ) );
			brcTypeMap.insert( make_pair( 0x8B, wstring( _T( "gradient" ) ) ) );
			brcTypeMap.insert( make_pair( 0x8C, wstring( _T( "handmade1" ) ) ) );
			brcTypeMap.insert( make_pair( 0x8D, wstring( _T( "handmade2" ) ) ) );
			brcTypeMap.insert( make_pair( 0x8E, wstring( _T( "heartBalloon" ) ) ) );
			brcTypeMap.insert( make_pair( 0x8F, wstring( _T( "heartGray" ) ) ) );
			brcTypeMap.insert( make_pair( 0x90, wstring( _T( "hearts" ) ) ) );
			brcTypeMap.insert( make_pair( 0x91, wstring( _T( "heebieJeebies" ) ) ) );
			brcTypeMap.insert( make_pair( 0x92, wstring( _T( "holly" ) ) ) );
			brcTypeMap.insert( make_pair( 0x93, wstring( _T( "houseFunky" ) ) ) );
			brcTypeMap.insert( make_pair( 0x94, wstring( _T( "hypnotic" ) ) ) );
			brcTypeMap.insert( make_pair( 0x95, wstring( _T( "iceCreamCones" ) ) ) );
			brcTypeMap.insert( make_pair( 0x96, wstring( _T( "lightBulb" ) ) ) );
			brcTypeMap.insert( make_pair( 0x97, wstring( _T( "lightning1" ) ) ) );
			brcTypeMap.insert( make_pair( 0x98, wstring( _T( "lightning2" ) ) ) );
			brcTypeMap.insert( make_pair( 0x99, wstring( _T( "mapPins" ) ) ) );
			brcTypeMap.insert( make_pair( 0x9A, wstring( _T( "mapleLeaf" ) ) ) );
			brcTypeMap.insert( make_pair( 0x9B, wstring( _T( "mapleMuffins" ) ) ) );
			brcTypeMap.insert( make_pair( 0x9C, wstring( _T( "marquee" ) ) ) );
			brcTypeMap.insert( make_pair( 0x9D, wstring( _T( "marqueeToothed" ) ) ) );
			brcTypeMap.insert( make_pair( 0x9E, wstring( _T( "moons" ) ) ) );
			brcTypeMap.insert( make_pair( 0x9F, wstring( _T( "mosaic" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA0, wstring( _T( "musicNotes" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA1, wstring( _T( "northwest" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA2, wstring( _T( "ovals" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA3, wstring( _T( "packages" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA4, wstring( _T( "palmsBlack" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA5, wstring( _T( "palmsColor" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA6, wstring( _T( "paperClips" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA7, wstring( _T( "papyrus" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA8, wstring( _T( "partyFavor" ) ) ) );
			brcTypeMap.insert( make_pair( 0xA9, wstring( _T( "partyGlass" ) ) ) );
			brcTypeMap.insert( make_pair( 0xAA, wstring( _T( "pencils" ) ) ) );
			brcTypeMap.insert( make_pair( 0xAB, wstring( _T( "people" ) ) ) );
			brcTypeMap.insert( make_pair( 0xAC, wstring( _T( "peopleWaving" ) ) ) );
			brcTypeMap.insert( make_pair( 0xAD, wstring( _T( "peopleHats" ) ) ) );
			brcTypeMap.insert( make_pair( 0xAE, wstring( _T( "poinsettias" ) ) ) );
			brcTypeMap.insert( make_pair( 0xAF, wstring( _T( "postageStamp" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB0, wstring( _T( "pumpkin1" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB1, wstring( _T( "pushPinNote2" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB2, wstring( _T( "pushPinNote1" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB3, wstring( _T( "pyramids" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB4, wstring( _T( "pyramidsAbove" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB5, wstring( _T( "quadrants" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB6, wstring( _T( "rings" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB7, wstring( _T( "safari" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB8, wstring( _T( "sawtooth" ) ) ) );
			brcTypeMap.insert( make_pair( 0xB9, wstring( _T( "sawtoothGray" ) ) ) );
			brcTypeMap.insert( make_pair( 0xBA, wstring( _T( "scaredCat" ) ) ) );
			brcTypeMap.insert( make_pair( 0xBB, wstring( _T( "seattle" ) ) ) );
			brcTypeMap.insert( make_pair( 0xBC, wstring( _T( "shadowedSquares" ) ) ) );
			brcTypeMap.insert( make_pair( 0xBD, wstring( _T( "sharksTeeth" ) ) ) );
			brcTypeMap.insert( make_pair( 0xBE, wstring( _T( "shorebirdTracks" ) ) ) );
			brcTypeMap.insert( make_pair( 0xBF, wstring( _T( "skyrocket" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC0, wstring( _T( "snowflakeFancy" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC1, wstring( _T( "snowflakes" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC2, wstring( _T( "sombrero" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC3, wstring( _T( "southwest" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC4, wstring( _T( "stars" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC5, wstring( _T( "starsTop" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC6, wstring( _T( "stars3d" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC7, wstring( _T( "starsBlack" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC8, wstring( _T( "starsShadowed" ) ) ) );
			brcTypeMap.insert( make_pair( 0xC9, wstring( _T( "sun" ) ) ) );
			brcTypeMap.insert( make_pair( 0xCA, wstring( _T( "swirligig" ) ) ) );
			brcTypeMap.insert( make_pair( 0xCB, wstring( _T( "tornPaper" ) ) ) );
			brcTypeMap.insert( make_pair( 0xCC, wstring( _T( "tornPaperBlack" ) ) ) );
			brcTypeMap.insert( make_pair( 0xCD, wstring( _T( "trees" ) ) ) );
			brcTypeMap.insert( make_pair( 0xCE, wstring( _T( "triangleParty" ) ) ) );
			brcTypeMap.insert( make_pair( 0xCF, wstring( _T( "triangles" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD0, wstring( _T( "tribal1" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD1, wstring( _T( "tribal2" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD2, wstring( _T( "tribal3" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD3, wstring( _T( "tribal4" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD4, wstring( _T( "tribal5" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD5, wstring( _T( "tribal6" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD6, wstring( _T( "twistedLines1" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD7, wstring( _T( "twistedLines2" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD8, wstring( _T( "vine" ) ) ) );
			brcTypeMap.insert( make_pair( 0xD9, wstring( _T( "waveline" ) ) ) );
			brcTypeMap.insert( make_pair( 0xDA, wstring( _T( "weavingAngles" ) ) ) );
			brcTypeMap.insert( make_pair( 0xDB, wstring( _T( "weavingBraid" ) ) ) );
			brcTypeMap.insert( make_pair( 0xDC, wstring( _T( "weavingRibbon" ) ) ) );
			brcTypeMap.insert( make_pair( 0xDD, wstring( _T( "weavingStrips" ) ) ) );
			brcTypeMap.insert( make_pair( 0xDE, wstring( _T( "whiteFlowers" ) ) ) );
			brcTypeMap.insert( make_pair( 0xDF, wstring( _T( "woodwork" ) ) ) );
			brcTypeMap.insert( make_pair( 0xE0, wstring( _T( "xIllusions" ) ) ) );
			brcTypeMap.insert( make_pair( 0xE1, wstring( _T( "zanyTriangles" ) ) ) );
			brcTypeMap.insert( make_pair( 0xE2, wstring( _T( "zigZag" ) ) ) );
			brcTypeMap.insert( make_pair( 0xE3, wstring( _T( "zigZagStitch" ) ) ) );
			brcTypeMap.insert( make_pair( 0xFF, wstring( _T( "none" ) ) ) );
		}
		brcTypeMapLock.Leave();
	}
	PropertiesMapping::PropertiesMapping (XmlUtils::CXmlWriter* pWriter) : m_pXmlWriter(pWriter)
	{	    
		init();
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendFlagAttribute( XMLTools::XMLElement<WCHAR>* node, const SinglePropertyModifier& sprm, const WCHAR* attributeName )
	{
		XMLTools::XMLAttribute<WCHAR> att( attributeName, FormatUtils::IntToWideString( sprm.Arguments[0] ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendFlagElement( XMLTools::XMLElement<WCHAR>* node, const SinglePropertyModifier& sprm, const WCHAR* elementName, bool unique )
	{
		XMLTools::XMLElement<WCHAR> ele( _T( "w" ), elementName );

		if ( sprm.Arguments[0] == 0 )
		{
			XMLTools::XMLAttribute<WCHAR> val( _T( "w:val" ), _T( "off" ) );
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

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, const WCHAR* attributeValue )
	{
		XMLTools::XMLAttribute<WCHAR> att( attributeName, attributeValue );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, int attributeValue )
	{
		XMLTools::XMLAttribute<WCHAR> att( attributeName, FormatUtils::IntToWideString( attributeValue ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, short attributeValue )
	{
		XMLTools::XMLAttribute<WCHAR> att( attributeName, FormatUtils::IntToWideString( attributeValue ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, unsigned short attributeValue )
	{
		XMLTools::XMLAttribute<WCHAR> att( attributeName, FormatUtils::IntToWideString( attributeValue ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, byte attributeValue )
	{
		XMLTools::XMLAttribute<WCHAR> att( attributeName, FormatUtils::IntToWideString( attributeValue ).c_str() );

		node->AppendAttribute( att );
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendValueElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, const WCHAR* elementValue, bool unique )
	{
		XMLTools::XMLElement<WCHAR>* ele = new XMLTools::XMLElement<WCHAR>( _T( "w" ), elementName );

		if( ( elementValue != NULL ) && ( wcscmp( elementValue, _T( "" ) ) != 0 ) )
		{
			XMLTools::XMLAttribute<WCHAR>* val = new XMLTools::XMLAttribute<WCHAR>( _T( "w:val" ) );

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

	void PropertiesMapping::appendValueElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, short elementValue, bool unique )
	{
		XMLTools::XMLElement<WCHAR>* ele = new XMLTools::XMLElement<WCHAR>( _T( "w" ), elementName );

		wstring strValue = FormatUtils::IntToWideString( elementValue );

		if ( strValue != wstring( _T( "" ) ) )
		{
			XMLTools::XMLAttribute<WCHAR>* val = new XMLTools::XMLAttribute<WCHAR>( _T( "w:val" ), strValue.c_str() );
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

	void PropertiesMapping::appendValueElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, unsigned short elementValue, bool unique )
	{
		XMLTools::XMLElement<WCHAR>* ele = new XMLTools::XMLElement<WCHAR>( _T( "w" ), elementName );

		wstring strValue = FormatUtils::IntToWideString( elementValue );

		if ( strValue != wstring( _T( "" ) ) )
		{
			XMLTools::XMLAttribute<WCHAR>* val = new XMLTools::XMLAttribute<WCHAR>( _T( "w:val" ), strValue.c_str() );
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

	void PropertiesMapping::appendValueElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, byte elementValue, bool unique )
	{
		XMLTools::XMLElement<WCHAR>* ele = new XMLTools::XMLElement<WCHAR>( _T( "w" ), elementName );

		wstring strValue = FormatUtils::IntToWideString( elementValue );

		if ( strValue != wstring( _T( "" ) ) )
		{
			XMLTools::XMLAttribute<WCHAR>* val = new XMLTools::XMLAttribute<WCHAR>( _T( "w:val" ), strValue.c_str() );
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

	void PropertiesMapping::appendBorderAttributes( BorderCode* brc, XMLTools::XMLElement<WCHAR>* border )
	{
		XMLTools::XMLAttribute<WCHAR> val( _T( "w:val" ) );

		if ( brc->fNil )
		{
			val.SetValue( _T( "nil" ) );
			border->AppendAttribute( val );
		}
		else
		{
			val.SetValue( getBorderType( brc->brcType ).c_str() );
			border->AppendAttribute( val );

			XMLTools::XMLAttribute<WCHAR> color( _T( "w:color" ) );
			color.SetValue( RGBColor( brc->cv, RedFirst ).SixDigitHexCode.c_str() );
			border->AppendAttribute( color );

			XMLTools::XMLAttribute<WCHAR> space( _T( "w:space" ), FormatUtils::IntToWideString( brc->dptSpace ).c_str() ); 
			border->AppendAttribute( space );

			XMLTools::XMLAttribute<WCHAR> sz( _T( "w:sz" ), FormatUtils::IntToWideString( brc->dptLineWidth ).c_str() ); 
			border->AppendAttribute( sz );

			if ( brc->fShadow )
			{
				XMLTools::XMLAttribute<WCHAR> shadow( _T( "w:shadow" ) );
				shadow.SetValue( _T( "1" ) );
				border->AppendAttribute( shadow );
			}
		}
	}

	/*========================================================================================================*/

	void PropertiesMapping::appendShading( XMLTools::XMLElement<WCHAR>* parent, const ShadingDescriptor& desc )
	{
		if ( ( parent != NULL ) && ( desc.shadingSpecialValue == shadingSpecialValueNormal ) )
		{
			XMLTools::XMLElement<WCHAR> shd( _T( "w:shd" ) );

			//fill color
			XMLTools::XMLAttribute<WCHAR> fill( _T( "w:fill" ) );

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
			XMLTools::XMLAttribute<WCHAR> color( _T( "w:color" ) );

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
			XMLTools::XMLAttribute<WCHAR> val( _T( "w:val" ) );
			val.SetValue( getShadingPattern( desc ).c_str() );
			shd.AppendAttribute( val );

			parent->RemoveChildByName( _T( "w:shd" ) );
			parent->AppendChild( shd );
		}
	}

	/*========================================================================================================*/

	wstring PropertiesMapping::getBorderType( byte type )
	{
		return this->brcTypeMap[type];
	}

	/*========================================================================================================*/

	wstring PropertiesMapping::getShadingPattern( const ShadingDescriptor& shd )
	{
		wstring pattern = _T( "" );

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

	void PropertiesMapping::appendDxaElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, const WCHAR* elementValue, bool unique )
	{
		XMLTools::XMLElement<WCHAR> ele( _T( "w" ), elementName );
		XMLTools::XMLAttribute<WCHAR> val( _T( "w:w" ), elementValue );
		ele.AppendAttribute( val );
		XMLTools::XMLAttribute<WCHAR> type( _T( "w:type" ), _T( "dxa" ) );
		ele.AppendAttribute( type );

		if ( unique )
		{
			node->RemoveChild( ele ); 
		}

		node->AppendChild( ele );
	}

	/*========================================================================================================*/

	void PropertiesMapping::addOrSetBorder( XMLTools::XMLElement<WCHAR>* pBdr, const XMLTools::XMLElement<WCHAR>* border )
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