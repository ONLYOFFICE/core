#pragma once

#include "../RtfDocument.h"
#include "../RtfProperty.h"
#include "OOXReaderBasic.h"

class OOXBorderReader
{
private:
	ComplexTypes::Word::CBorder			* m_ooxBorder;
	ComplexTypes::Word::CPageBorder		* m_ooxPageBorder;
public: 
	OOXBorderReader(ComplexTypes::Word::CBorder * ooxBorder)
	{
		m_ooxBorder = ooxBorder;
	}
	OOXBorderReader(ComplexTypes::Word::CPageBorder * ooxBorder)
	{
		m_ooxPageBorder = ooxBorder;
	}
	bool Parse( ReaderParameter oParam, RtfBorder& oOutputBorder )
	{	
		if (m_ooxBorder == NULL) return false;

		if (m_ooxBorder->m_oColor.IsInit())
		{
			RtfColor oColor(m_ooxBorder->m_oColor->Get_R(), m_ooxBorder->m_oColor->Get_G(), m_ooxBorder->m_oColor->Get_B());
			oOutputBorder.m_nColor = oParam.oRtf->m_oColorTable.AddItem( oColor );
		}
		if (m_ooxBorder->m_oSpace.IsInit())
		{
			oOutputBorder.m_nSpace = RtfUtility::pt2Twip(m_ooxBorder->m_oSpace->ToPoints());
		}
		if (m_ooxBorder->m_oSz.IsInit())
		{
			oOutputBorder.m_nWidth = RtfUtility::pt2Twip(m_ooxBorder->m_oSz->ToPoints());// = 5 * nSize / 2;//w:sz  1/8 twips (equivalent to 1/576th of an inch)
		}

		if (m_ooxBorder->m_oVal.IsInit())
		{
			switch(m_ooxBorder->m_oVal->GetValue())
			{
			//case SimpleTypes::bordervalueApples                : return _T("apples");
			//case SimpleTypes::bordervalueArchedScallops        : return _T("archedScallops");
			//case SimpleTypes::bordervalueBabyPacifier          : return _T("babyPacifier");
			//case SimpleTypes::bordervalueBabyRattle            : return _T("babyRattle");
			//case SimpleTypes::bordervalueBalloons3Colors       : return _T("balloons3Colors");
			//case SimpleTypes::bordervalueBalloonsHotAir        : return _T("balloonsHotAir");
			//case SimpleTypes::bordervalueBasicBlackDashes      : return _T("basicBlackDashes");
			//case SimpleTypes::bordervalueBasicBlackDots        : return _T("basicBlackDots");
			//case SimpleTypes::bordervalueBasicBlackSquares     : return _T("basicBlackSquares");
			//case SimpleTypes::bordervalueBasicThinLines        : return _T("basicThinLines");
			//case SimpleTypes::bordervalueBasicWhiteDashes      : return _T("basicWhiteDashes");
			//case SimpleTypes::bordervalueBasicWhiteDots        : return _T("basicWhiteDots");
			//case SimpleTypes::bordervalueBasicWhiteSquares     : return _T("basicWhiteSquares");
			//case SimpleTypes::bordervalueBasicWideInline       : return _T("basicWideInline");
			//case SimpleTypes::bordervalueBasicWideMidline      : return _T("basicWideMidline");
			//case SimpleTypes::bordervalueBasicWideOutline      : return _T("basicWideOutline");
			//case SimpleTypes::bordervalueBats                  : return _T("bats");
			//case SimpleTypes::bordervalueBirds                 : return _T("birds");
			//case SimpleTypes::bordervalueBirdsFlight           : return _T("birdsFlight");
			//case SimpleTypes::bordervalueCabins                : return _T("cabins");
			//case SimpleTypes::bordervalueCakeSlice             : return _T("cakeSlice");
			//case SimpleTypes::bordervalueCandyCorn             : return _T("candyCorn");
			//case SimpleTypes::bordervalueCelticKnotwork        : return _T("celticKnotwork");
			//case SimpleTypes::bordervalueCertificateBanner     : return _T("certificateBanner");
			//case SimpleTypes::bordervalueChainLink             : return _T("chainLink");
			//case SimpleTypes::bordervalueChampagneBottle       : return _T("champagneBottle");
			//case SimpleTypes::bordervalueCheckedBarBlack       : return _T("checkedBarBlack");
			//case SimpleTypes::bordervalueCheckedBarColor       : return _T("checkedBarColor");
			//case SimpleTypes::bordervalueCheckered             : return _T("checkered");
			//case SimpleTypes::bordervalueChristmasTree         : return _T("christmasTree");
			//case SimpleTypes::bordervalueCirclesLines          : return _T("circlesLines");
			//case SimpleTypes::bordervalueCirclesRectangles     : return _T("circlesRectangles");
			//case SimpleTypes::bordervalueClassicalWave         : return _T("classicalWave");
			//case SimpleTypes::bordervalueClocks                : return _T("clocks");
			//case SimpleTypes::bordervalueCompass               : return _T("compass");
			//case SimpleTypes::bordervalueConfetti              : return _T("confetti");
			//case SimpleTypes::bordervalueConfettiGrays         : return _T("confettiGrays");
			//case SimpleTypes::bordervalueConfettiOutline       : return _T("confettiOutline");
			//case SimpleTypes::bordervalueConfettiStreamers     : return _T("confettiStreamers");
			//case SimpleTypes::bordervalueConfettiWhite         : return _T("confettiWhite");
			//case SimpleTypes::bordervalueCornerTriangles       : return _T("cornerTriangles");
			//case SimpleTypes::bordervalueCouponCutoutDashes    : return _T("couponCutoutDashes");
			//case SimpleTypes::bordervalueCouponCutoutDots      : return _T("couponCutoutDots");
			//case SimpleTypes::bordervalueCrazyMaze             : return _T("crazyMaze");
			//case SimpleTypes::bordervalueCreaturesButterfly    : return _T("creaturesButterfly");
			//case SimpleTypes::bordervalueCreaturesFish         : return _T("creaturesFish");
			//case SimpleTypes::bordervalueCreaturesInsects      : return _T("creaturesInsects");
			//case SimpleTypes::bordervalueCreaturesLadyBug      : return _T("creaturesLadyBug");
			//case SimpleTypes::bordervalueCrossStitch           : return _T("crossStitch");
			//case SimpleTypes::bordervalueCup                   : return _T("cup");
			//case SimpleTypes::bordervalueCustom                : return _T("custom");
			case SimpleTypes::bordervalueDashDotStroked        : oOutputBorder.m_eType = RtfBorder::bt_brdrdashdotstr;	break;
			case SimpleTypes::bordervalueDashed                : oOutputBorder.m_eType = RtfBorder::bt_brdrdash;			break;
			case SimpleTypes::bordervalueDashSmallGap          : oOutputBorder.m_eType = RtfBorder::bt_brdrdashsm;		break;
			//case SimpleTypes::bordervalueDecoArch              : return _T("decoArch");
			//case SimpleTypes::bordervalueDecoArchColor         : return _T("decoArchColor");
			//case SimpleTypes::bordervalueDecoBlocks            : return _T("decoBlocks");
			//case SimpleTypes::bordervalueDiamondsGray          : return _T("diamondsGray");
			case SimpleTypes::bordervalueDotDash               : oOutputBorder.m_eType = RtfBorder::bt_brdrdashd;		break;
			case SimpleTypes::bordervalueDotDotDash            : oOutputBorder.m_eType = RtfBorder::bt_brdrdashdd;		break;
			case SimpleTypes::bordervalueDotted                : oOutputBorder.m_eType = RtfBorder::bt_brdrdot;			break;
			case SimpleTypes::bordervalueDouble                : oOutputBorder.m_eType = RtfBorder::bt_brdrdb;			break;
			//case SimpleTypes::bordervalueDoubleD               : return _T("doubleD");
			//case SimpleTypes::bordervalueDoubleDiamonds        : return _T("doubleDiamonds");
			case SimpleTypes::bordervalueDoubleWave            : oOutputBorder.m_eType = RtfBorder::bt_brdrwavydb;		break;
			//case SimpleTypes::bordervalueEarth1                : return _T("earth1");
			//case SimpleTypes::bordervalueEarth2                : return _T("earth2");
			//case SimpleTypes::bordervalueEarth3                : return _T("earth3");
			//case SimpleTypes::bordervalueEclipsingSquares1     : return _T("eclipsingSquares1");
			//case SimpleTypes::bordervalueEclipsingSquares2     : return _T("eclipsingSquares2");
			//case SimpleTypes::bordervalueEggsBlack             : return _T("eggsBlack");
			//case SimpleTypes::bordervalueFans                  : return _T("fans");
			//case SimpleTypes::bordervalueFilm                  : return _T("film");
			//case SimpleTypes::bordervalueFirecrackers          : return _T("firecrackers");
			//case SimpleTypes::bordervalueFlowersBlockPrint     : return _T("flowersBlockPrint");
			//case SimpleTypes::bordervalueFlowersDaisies        : return _T("flowersDaisies");
			//case SimpleTypes::bordervalueFlowersModern1        : return _T("flowersModern1");
			//case SimpleTypes::bordervalueFlowersModern2        : return _T("flowersModern2");
			//case SimpleTypes::bordervalueFlowersPansy          : return _T("flowersPansy");
			//case SimpleTypes::bordervalueFlowersRedRose        : return _T("flowersRedRose");
			//case SimpleTypes::bordervalueFlowersRoses          : return _T("flowersRoses");
			//case SimpleTypes::bordervalueFlowersTeacup         : return _T("flowersTeacup");
			//case SimpleTypes::bordervalueFlowersTiny           : return _T("flowersTiny");
			//case SimpleTypes::bordervalueGems                  : return _T("gems");
			//case SimpleTypes::bordervalueGingerbreadMan        : return _T("gingerbreadMan");
			//case SimpleTypes::bordervalueGradient              : return _T("gradient");
			//case SimpleTypes::bordervalueHandmade1             : return _T("handmade1");
			//case SimpleTypes::bordervalueHandmade2             : return _T("handmade2");
			//case SimpleTypes::bordervalueHeartBalloon          : return _T("heartBalloon");
			//case SimpleTypes::bordervalueHeartGray             : return _T("heartGray");
			//case SimpleTypes::bordervalueHearts                : return _T("hearts");
			//case SimpleTypes::bordervalueHeebieJeebies         : return _T("heebieJeebies");
			//case SimpleTypes::bordervalueHolly                 : return _T("holly");
			//case SimpleTypes::bordervalueHouseFunky            : return _T("houseFunky");
			//case SimpleTypes::bordervalueHypnotic              : return _T("hypnotic");
			//case SimpleTypes::bordervalueIceCreamCones         : return _T("iceCreamCones");
			case SimpleTypes::bordervalueInset                 : oOutputBorder.m_eType = RtfBorder::bt_brdrinset;		break;
			//case SimpleTypes::bordervalueLightBulb             : return _T("lightBulb");
			//case SimpleTypes::bordervalueLightning1            : return _T("lightning1");
			//case SimpleTypes::bordervalueLightning2            : return _T("lightning2");
			//case SimpleTypes::bordervalueMapleLeaf             : return _T("mapleLeaf");
			//case SimpleTypes::bordervalueMapleMuffins          : return _T("mapleMuffins");
			//case SimpleTypes::bordervalueMapPins               : return _T("mapPins");
			//case SimpleTypes::bordervalueMarquee               : return _T("marquee");
			//case SimpleTypes::bordervalueMarqueeToothed        : return _T("marqueeToothed");
			//case SimpleTypes::bordervalueMoons                 : return _T("moons");
			//case SimpleTypes::bordervalueMosaic                : return _T("mosaic");
			//case SimpleTypes::bordervalueMusicNotes            : return _T("musicNotes");
			case SimpleTypes::bordervalueNil                   : oOutputBorder.m_eType = RtfBorder::bt_brdrnone;			break;
			case SimpleTypes::bordervalueNone                  : oOutputBorder.m_eType = RtfBorder::bt_brdrnone;			break;
			//case SimpleTypes::bordervalueNorthwest             : return _T("northwest");
			case SimpleTypes::bordervalueOutset                : oOutputBorder.m_eType = RtfBorder::bt_brdroutset;		break;
			//case SimpleTypes::bordervalueOvals                 : return _T("ovals");
			//case SimpleTypes::bordervaluePackages              : return _T("packages");
			//case SimpleTypes::bordervaluePalmsBlack            : return _T("palmsBlack");
			//case SimpleTypes::bordervaluePalmsColor            : return _T("palmsColor");
			//case SimpleTypes::bordervaluePaperClips            : return _T("paperClips");
			//case SimpleTypes::bordervaluePapyrus               : return _T("papyrus");
			//case SimpleTypes::bordervaluePartyFavor            : return _T("partyFavor");
			//case SimpleTypes::bordervaluePartyGlass            : return _T("partyGlass");
			//case SimpleTypes::bordervaluePencils               : return _T("pencils");
			//case SimpleTypes::bordervaluePeople                : return _T("people");
			//case SimpleTypes::bordervaluePeopleHats            : return _T("peopleHats");
			//case SimpleTypes::bordervaluePeopleWaving          : return _T("peopleWaving");
			//case SimpleTypes::bordervaluePoinsettias           : return _T("poinsettias");
			//case SimpleTypes::bordervaluePostageStamp          : return _T("postageStamp");
			//case SimpleTypes::bordervaluePumpkin1              : return _T("pumpkin1");
			//case SimpleTypes::bordervaluePushPinNote1          : return _T("pushPinNote1");
			//case SimpleTypes::bordervaluePushPinNote2          : return _T("pushPinNote2");
			//case SimpleTypes::bordervaluePyramids              : return _T("pyramids");
			//case SimpleTypes::bordervaluePyramidsAbove         : return _T("pyramidsAbove");
			//case SimpleTypes::bordervalueQuadrants             : return _T("quadrants");
			//case SimpleTypes::bordervalueRings                 : return _T("rings");
			//case SimpleTypes::bordervalueSafari                : return _T("safari");
			//case SimpleTypes::bordervalueSawtooth              : return _T("sawtooth");
			//case SimpleTypes::bordervalueSawtoothGray          : return _T("sawtoothGray");
			//case SimpleTypes::bordervalueScaredCat             : return _T("scaredCat");
			//case SimpleTypes::bordervalueSeattle               : return _T("seattle");
			//case SimpleTypes::bordervalueShadowedSquares       : return _T("shadowedSquares");
			//case SimpleTypes::bordervalueShapes1               : return _T("shapes1");
			//case SimpleTypes::bordervalueShapes2               : return _T("shapes2");
			//case SimpleTypes::bordervalueSharksTeeth           : return _T("sharksTeeth");
			//case SimpleTypes::bordervalueShorebirdTracks       : return _T("shorebirdTracks");
			case SimpleTypes::bordervalueSingle                : oOutputBorder.m_eType = RtfBorder::bt_brdrs;			break;
			//case SimpleTypes::bordervalueSkyrocket             : return _T("skyrocket");
			//case SimpleTypes::bordervalueSnowflakeFancy        : return _T("snowflakeFancy");
			//case SimpleTypes::bordervalueSnowflakes            : return _T("snowflakes");
			//case SimpleTypes::bordervalueSombrero              : return _T("sombrero");
			//case SimpleTypes::bordervalueSouthwest             : return _T("southwest");
			//case SimpleTypes::bordervalueStars                 : return _T("stars");
			//case SimpleTypes::bordervalueStars3d               : return _T("stars3d");
			//case SimpleTypes::bordervalueStarsBlack            : return _T("starsBlack");
			//case SimpleTypes::bordervalueStarsShadowed         : return _T("starsShadowed");
			//case SimpleTypes::bordervalueStarsTop              : return _T("starsTop");
			//case SimpleTypes::bordervalueSun                   : return _T("sun");
			//case SimpleTypes::bordervalueSwirligig             : return _T("swirligig");
			case SimpleTypes::bordervalueThick                 : oOutputBorder.m_eType = RtfBorder::bt_brdrth;			break;
			case SimpleTypes::bordervalueThickThinLargeGap     : oOutputBorder.m_eType = RtfBorder::bt_brdrthtnlg;		break;
			case SimpleTypes::bordervalueThickThinMediumGap    : oOutputBorder.m_eType = RtfBorder::bt_brdrthtnmg;		break;
			case SimpleTypes::bordervalueThickThinSmallGap     : oOutputBorder.m_eType = RtfBorder::bt_brdrthtnsg;		break;
			case SimpleTypes::bordervalueThinThickLargeGap     : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthlg;		break;
			case SimpleTypes::bordervalueThinThickMediumGap    : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthmg;		break;
			case SimpleTypes::bordervalueThinThickSmallGap     : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthsg;		break;
			case SimpleTypes::bordervalueThinThickThinLargeGap : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthtnlg;	break;
			case SimpleTypes::bordervalueThinThickThinMediumGap: oOutputBorder.m_eType = RtfBorder::bt_brdrtnthtnmg;	break;
			case SimpleTypes::bordervalueThinThickThinSmallGap : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthtnsg;		break;
			case SimpleTypes::bordervalueThreeDEmboss          : oOutputBorder.m_eType = RtfBorder::bt_brdremboss;		break;
			case SimpleTypes::bordervalueThreeDEngrave         : oOutputBorder.m_eType = RtfBorder::bt_brdrengrave;		break;
			//case SimpleTypes::bordervalueTornPaper             : return _T("tornPaper");
			//case SimpleTypes::bordervalueTornPaperBlack        : return _T("tornPaperBlack");
			//case SimpleTypes::bordervalueTrees                 : return _T("trees");
			//case SimpleTypes::bordervalueTriangle1             : return _T("triangle1");
			//case SimpleTypes::bordervalueTriangle2             : return _T("triangle2");
			//case SimpleTypes::bordervalueTriangleCircle1       : return _T("triangleCircle1");
			//case SimpleTypes::bordervalueTriangleCircle2       : return _T("triangleCircle2");
			//case SimpleTypes::bordervalueTriangleParty         : return _T("triangleParty");
			//case SimpleTypes::bordervalueTriangles             : return _T("triangles");
			case SimpleTypes::bordervalueTriple                : oOutputBorder.m_eType = RtfBorder::bt_brdrtriple;		break;
			//case SimpleTypes::bordervalueTwistedLines1         : return _T("twistedLines1");
			//case SimpleTypes::bordervalueTwistedLines2         : return _T("twistedLines2");
			//case SimpleTypes::bordervalueVine                  : return _T("vine");
			case SimpleTypes::bordervalueWave                  : oOutputBorder.m_eType = RtfBorder::bt_brdrwavy;			break;
			//case SimpleTypes::bordervalueWaveline              : return _T("waveline");
			//case SimpleTypes::bordervalueWeavingAngles         : return _T("weavingAngles");
			//case SimpleTypes::bordervalueWeavingBraid          : return _T("weavingBraid");
			//case SimpleTypes::bordervalueWeavingRibbon         : return _T("weavingRibbon");
			//case SimpleTypes::bordervalueWeavingStrips         : return _T("weavingStrips");
			//case SimpleTypes::bordervalueWhiteFlowers          : return _T("whiteFlowers");
			//case SimpleTypes::bordervalueWoodwork              : return _T("woodwork");
			//case SimpleTypes::bordervalueXIllusions            : return _T("xIllusions");
			//case SimpleTypes::bordervalueZanyTriangles         : return _T("zanyTriangles");
			//case SimpleTypes::bordervalueZigZag                : return _T("zigZag");
			//case SimpleTypes::SimpleTypes::bordervalueZigZagStitch          : return _T("zigZagStitch");
				//oOutputBorder.m_eType = RtfBorder::bt_brdrsh;
				//oOutputBorder.m_eType = RtfBorder::bt_brdrhair;
			}
		}
		return true;
	}
};
