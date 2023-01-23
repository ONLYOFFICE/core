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

#include "OOXBorderReader.h"

OOXBorderReader::OOXBorderReader(ComplexTypes::Word::CBorder * ooxBorder)
{
	m_ooxBorder		= ooxBorder;
	m_ooxPageBorder = NULL;
}
OOXBorderReader::OOXBorderReader(ComplexTypes::Word::CPageBorder * ooxBorder)
{
	m_ooxPageBorder = ooxBorder;
	m_ooxBorder		= NULL;
}
bool OOXBorderReader::Parse( ReaderParameter oParam, RtfBorder& oOutputBorder )
{
	if (m_ooxBorder == NULL && m_ooxPageBorder == NULL) return false;

	int type_border = -1;
	if (m_ooxPageBorder)
	{
		if ((m_ooxPageBorder->m_oColor.IsInit()) && (m_ooxPageBorder->m_oColor->GetValue() == SimpleTypes::hexcolorRGB))
		{
			RtfColor oColor(m_ooxPageBorder->m_oColor->Get_R(), m_ooxPageBorder->m_oColor->Get_G(), m_ooxPageBorder->m_oColor->Get_B());
			oOutputBorder.m_nColor = oParam.oRtf->m_oColorTable.AddItem( oColor );
		}
		if (m_ooxPageBorder->m_oSpace.IsInit())
		{
			oOutputBorder.m_nSpace = RtfUtility::pt2Twip(m_ooxPageBorder->m_oSpace->ToPoints());
		}
		if (m_ooxPageBorder->m_oSz.IsInit())
		{
			oOutputBorder.m_nWidth = RtfUtility::pt2Twip(m_ooxPageBorder->m_oSz->ToPoints());// = 5 * nSize / 2;//w:sz  1/8 twips (equivalent to 1/576th of an inch)
		}
		if (m_ooxPageBorder->m_oVal.IsInit())
			type_border = m_ooxPageBorder->m_oVal->GetValue();
	}

	if (m_ooxBorder)
	{
		if (m_ooxBorder->m_oVal.IsInit())
		{
			type_border = m_ooxBorder->m_oVal->GetValue();
		}
		//if (m_ooxBorder->m_oVal->GetValue() == SimpleTypes::bordervalueNone ) return true;
		//		// это отсутствие свойства а не бордера (Nil

		if ((m_ooxBorder->m_oColor.IsInit()) && (m_ooxBorder->m_oColor->GetValue() == SimpleTypes::hexcolorRGB))
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
	}
	switch(type_border)
	{
	//case SimpleTypes::bordervalueApples                : return L"apples";
	//case SimpleTypes::bordervalueArchedScallops        : return L"archedScallops";
	//case SimpleTypes::bordervalueBabyPacifier          : return L"babyPacifier";
	//case SimpleTypes::bordervalueBabyRattle            : return L"babyRattle";
	//case SimpleTypes::bordervalueBalloons3Colors       : return L"balloons3Colors";
	//case SimpleTypes::bordervalueBalloonsHotAir        : return L"balloonsHotAir";
	//case SimpleTypes::bordervalueBasicBlackDashes      : return L"basicBlackDashes";
	//case SimpleTypes::bordervalueBasicBlackDots        : return L"basicBlackDots";
	//case SimpleTypes::bordervalueBasicBlackSquares     : return L"basicBlackSquares";
	//case SimpleTypes::bordervalueBasicThinLines        : return L"basicThinLines";
	//case SimpleTypes::bordervalueBasicWhiteDashes      : return L"basicWhiteDashes";
	//case SimpleTypes::bordervalueBasicWhiteDots        : return L"basicWhiteDots";
	//case SimpleTypes::bordervalueBasicWhiteSquares     : return L"basicWhiteSquares";
	//case SimpleTypes::bordervalueBasicWideInline       : return L"basicWideInline";
	//case SimpleTypes::bordervalueBasicWideMidline      : return L"basicWideMidline";
	//case SimpleTypes::bordervalueBasicWideOutline      : return L"basicWideOutline";
	//case SimpleTypes::bordervalueBats                  : return L"bats";
	//case SimpleTypes::bordervalueBirds                 : return L"birds";
	//case SimpleTypes::bordervalueBirdsFlight           : return L"birdsFlight";
	//case SimpleTypes::bordervalueCabins                : return L"cabins";
	//case SimpleTypes::bordervalueCakeSlice             : return L"cakeSlice";
	//case SimpleTypes::bordervalueCandyCorn             : return L"candyCorn";
	//case SimpleTypes::bordervalueCelticKnotwork        : return L"celticKnotwork";
	//case SimpleTypes::bordervalueCertificateBanner     : return L"certificateBanner";
	//case SimpleTypes::bordervalueChainLink             : return L"chainLink";
	//case SimpleTypes::bordervalueChampagneBottle       : return L"champagneBottle";
	//case SimpleTypes::bordervalueCheckedBarBlack       : return L"checkedBarBlack";
	//case SimpleTypes::bordervalueCheckedBarColor       : return L"checkedBarColor";
	//case SimpleTypes::bordervalueCheckered             : return L"checkered";
	//case SimpleTypes::bordervalueChristmasTree         : return L"christmasTree";
	//case SimpleTypes::bordervalueCirclesLines          : return L"circlesLines";
	//case SimpleTypes::bordervalueCirclesRectangles     : return L"circlesRectangles";
	//case SimpleTypes::bordervalueClassicalWave         : return L"classicalWave";
	//case SimpleTypes::bordervalueClocks                : return L"clocks";
	//case SimpleTypes::bordervalueCompass               : return L"compass";
	//case SimpleTypes::bordervalueConfetti              : return L"confetti";
	//case SimpleTypes::bordervalueConfettiGrays         : return L"confettiGrays";
	//case SimpleTypes::bordervalueConfettiOutline       : return L"confettiOutline";
	//case SimpleTypes::bordervalueConfettiStreamers     : return L"confettiStreamers";
	//case SimpleTypes::bordervalueConfettiWhite         : return L"confettiWhite";
	//case SimpleTypes::bordervalueCornerTriangles       : return L"cornerTriangles";
	//case SimpleTypes::bordervalueCouponCutoutDashes    : return L"couponCutoutDashes";
	//case SimpleTypes::bordervalueCouponCutoutDots      : return L"couponCutoutDots";
	//case SimpleTypes::bordervalueCrazyMaze             : return L"crazyMaze";
	//case SimpleTypes::bordervalueCreaturesButterfly    : return L"creaturesButterfly";
	//case SimpleTypes::bordervalueCreaturesFish         : return L"creaturesFish";
	//case SimpleTypes::bordervalueCreaturesInsects      : return L"creaturesInsects";
	//case SimpleTypes::bordervalueCreaturesLadyBug      : return L"creaturesLadyBug";
	//case SimpleTypes::bordervalueCrossStitch           : return L"crossStitch";
	//case SimpleTypes::bordervalueCup                   : return L"cup";
	//case SimpleTypes::bordervalueCustom                : return L"custom";
	case SimpleTypes::bordervalueDashDotStroked        : oOutputBorder.m_eType = RtfBorder::bt_brdrdashdotstr;	break;
	case SimpleTypes::bordervalueDashed                : oOutputBorder.m_eType = RtfBorder::bt_brdrdash;		break;
	case SimpleTypes::bordervalueDashSmallGap          : oOutputBorder.m_eType = RtfBorder::bt_brdrdashsm;		break;
		//case SimpleTypes::bordervalueDecoArch              : return L"decoArch";
		//case SimpleTypes::bordervalueDecoArchColor         : return L"decoArchColor";
		//case SimpleTypes::bordervalueDecoBlocks            : return L"decoBlocks";
		//case SimpleTypes::bordervalueDiamondsGray          : return L"diamondsGray";
	case SimpleTypes::bordervalueDotDash               : oOutputBorder.m_eType = RtfBorder::bt_brdrdashd;		break;
	case SimpleTypes::bordervalueDotDotDash            : oOutputBorder.m_eType = RtfBorder::bt_brdrdashdd;		break;
	case SimpleTypes::bordervalueDotted                : oOutputBorder.m_eType = RtfBorder::bt_brdrdot;			break;
	case SimpleTypes::bordervalueDouble                : oOutputBorder.m_eType = RtfBorder::bt_brdrdb;			break;
		//case SimpleTypes::bordervalueDoubleD               : return L"doubleD";
		//case SimpleTypes::bordervalueDoubleDiamonds        : return L"doubleDiamonds";
	case SimpleTypes::bordervalueDoubleWave            : oOutputBorder.m_eType = RtfBorder::bt_brdrwavydb;		break;
		//case SimpleTypes::bordervalueEarth1                : return L"earth1";
		//case SimpleTypes::bordervalueEarth2                : return L"earth2";
		//case SimpleTypes::bordervalueEarth3                : return L"earth3";
		//case SimpleTypes::bordervalueEclipsingSquares1     : return L"eclipsingSquares1";
		//case SimpleTypes::bordervalueEclipsingSquares2     : return L"eclipsingSquares2";
		//case SimpleTypes::bordervalueEggsBlack             : return L"eggsBlack";
		//case SimpleTypes::bordervalueFans                  : return L"fans";
		//case SimpleTypes::bordervalueFilm                  : return L"film";
		//case SimpleTypes::bordervalueFirecrackers          : return L"firecrackers";
		//case SimpleTypes::bordervalueFlowersBlockPrint     : return L"flowersBlockPrint";
		//case SimpleTypes::bordervalueFlowersDaisies        : return L"flowersDaisies";
		//case SimpleTypes::bordervalueFlowersModern1        : return L"flowersModern1";
		//case SimpleTypes::bordervalueFlowersModern2        : return L"flowersModern2";
		//case SimpleTypes::bordervalueFlowersPansy          : return L"flowersPansy";
		//case SimpleTypes::bordervalueFlowersRedRose        : return L"flowersRedRose";
		//case SimpleTypes::bordervalueFlowersRoses          : return L"flowersRoses";
		//case SimpleTypes::bordervalueFlowersTeacup         : return L"flowersTeacup";
		//case SimpleTypes::bordervalueFlowersTiny           : return L"flowersTiny";
		//case SimpleTypes::bordervalueGems                  : return L"gems";
		//case SimpleTypes::bordervalueGingerbreadMan        : return L"gingerbreadMan";
		//case SimpleTypes::bordervalueGradient              : return L"gradient";
		//case SimpleTypes::bordervalueHandmade1             : return L"handmade1";
		//case SimpleTypes::bordervalueHandmade2             : return L"handmade2";
		//case SimpleTypes::bordervalueHeartBalloon          : return L"heartBalloon";
		//case SimpleTypes::bordervalueHeartGray             : return L"heartGray";
		//case SimpleTypes::bordervalueHearts                : return L"hearts";
		//case SimpleTypes::bordervalueHeebieJeebies         : return L"heebieJeebies";
		//case SimpleTypes::bordervalueHolly                 : return L"holly";
		//case SimpleTypes::bordervalueHouseFunky            : return L"houseFunky";
		//case SimpleTypes::bordervalueHypnotic              : return L"hypnotic";
		//case SimpleTypes::bordervalueIceCreamCones         : return L"iceCreamCones";
	case SimpleTypes::bordervalueInset                 : oOutputBorder.m_eType = RtfBorder::bt_brdrinset;		break;
		//case SimpleTypes::bordervalueLightBulb             : return L"lightBulb";
		//case SimpleTypes::bordervalueLightning1            : return L"lightning1";
		//case SimpleTypes::bordervalueLightning2            : return L"lightning2";
		//case SimpleTypes::bordervalueMapleLeaf             : return L"mapleLeaf";
		//case SimpleTypes::bordervalueMapleMuffins          : return L"mapleMuffins";
		//case SimpleTypes::bordervalueMapPins               : return L"mapPins";
		//case SimpleTypes::bordervalueMarquee               : return L"marquee";
		//case SimpleTypes::bordervalueMarqueeToothed        : return L"marqueeToothed";
		//case SimpleTypes::bordervalueMoons                 : return L"moons";
		//case SimpleTypes::bordervalueMosaic                : return L"mosaic";
		//case SimpleTypes::bordervalueMusicNotes            : return L"musicNotes";
	case SimpleTypes::bordervalueNil                   : oOutputBorder.m_eType = RtfBorder::bt_brdrnone;		break;
	case SimpleTypes::bordervalueNone                  : oOutputBorder.m_eType = RtfBorder::bt_brdrnone;		break;
		//case SimpleTypes::bordervalueNorthwest             : return L"northwest";
	case SimpleTypes::bordervalueOutset                : oOutputBorder.m_eType = RtfBorder::bt_brdroutset;		break;
		//case SimpleTypes::bordervalueOvals                 : return L"ovals";
		//case SimpleTypes::bordervaluePackages              : return L"packages";
		//case SimpleTypes::bordervaluePalmsBlack            : return L"palmsBlack";
		//case SimpleTypes::bordervaluePalmsColor            : return L"palmsColor";
		//case SimpleTypes::bordervaluePaperClips            : return L"paperClips";
		//case SimpleTypes::bordervaluePapyrus               : return L"papyrus";
		//case SimpleTypes::bordervaluePartyFavor            : return L"partyFavor";
		//case SimpleTypes::bordervaluePartyGlass            : return L"partyGlass";
		//case SimpleTypes::bordervaluePencils               : return L"pencils";
		//case SimpleTypes::bordervaluePeople                : return L"people";
		//case SimpleTypes::bordervaluePeopleHats            : return L"peopleHats";
		//case SimpleTypes::bordervaluePeopleWaving          : return L"peopleWaving";
		//case SimpleTypes::bordervaluePoinsettias           : return L"poinsettias";
		//case SimpleTypes::bordervaluePostageStamp          : return L"postageStamp";
		//case SimpleTypes::bordervaluePumpkin1              : return L"pumpkin1";
		//case SimpleTypes::bordervaluePushPinNote1          : return L"pushPinNote1";
		//case SimpleTypes::bordervaluePushPinNote2          : return L"pushPinNote2";
		//case SimpleTypes::bordervaluePyramids              : return L"pyramids";
		//case SimpleTypes::bordervaluePyramidsAbove         : return L"pyramidsAbove";
		//case SimpleTypes::bordervalueQuadrants             : return L"quadrants";
		//case SimpleTypes::bordervalueRings                 : return L"rings";
		//case SimpleTypes::bordervalueSafari                : return L"safari";
		//case SimpleTypes::bordervalueSawtooth              : return L"sawtooth";
		//case SimpleTypes::bordervalueSawtoothGray          : return L"sawtoothGray";
		//case SimpleTypes::bordervalueScaredCat             : return L"scaredCat";
		//case SimpleTypes::bordervalueSeattle               : return L"seattle";
		//case SimpleTypes::bordervalueShadowedSquares       : return L"shadowedSquares";
		//case SimpleTypes::bordervalueShapes1               : return L"shapes1";
		//case SimpleTypes::bordervalueShapes2               : return L"shapes2";
		//case SimpleTypes::bordervalueSharksTeeth           : return L"sharksTeeth";
		//case SimpleTypes::bordervalueShorebirdTracks       : return L"shorebirdTracks";
	case SimpleTypes::bordervalueSingle                : oOutputBorder.m_eType = RtfBorder::bt_brdrs;			break;
		//case SimpleTypes::bordervalueSkyrocket             : return L"skyrocket";
		//case SimpleTypes::bordervalueSnowflakeFancy        : return L"snowflakeFancy";
		//case SimpleTypes::bordervalueSnowflakes            : return L"snowflakes";
		//case SimpleTypes::bordervalueSombrero              : return L"sombrero";
		//case SimpleTypes::bordervalueSouthwest             : return L"southwest";
		//case SimpleTypes::bordervalueStars                 : return L"stars";
		//case SimpleTypes::bordervalueStars3d               : return L"stars3d";
		//case SimpleTypes::bordervalueStarsBlack            : return L"starsBlack";
		//case SimpleTypes::bordervalueStarsShadowed         : return L"starsShadowed";
		//case SimpleTypes::bordervalueStarsTop              : return L"starsTop";
		//case SimpleTypes::bordervalueSun                   : return L"sun";
		//case SimpleTypes::bordervalueSwirligig             : return L"swirligig";
	case SimpleTypes::bordervalueThick                 : oOutputBorder.m_eType = RtfBorder::bt_brdrth;			break;
	case SimpleTypes::bordervalueThickThinLargeGap     : oOutputBorder.m_eType = RtfBorder::bt_brdrthtnlg;		break;
	case SimpleTypes::bordervalueThickThinMediumGap    : oOutputBorder.m_eType = RtfBorder::bt_brdrthtnmg;		break;
	case SimpleTypes::bordervalueThickThinSmallGap     : oOutputBorder.m_eType = RtfBorder::bt_brdrthtnsg;		break;
	case SimpleTypes::bordervalueThinThickLargeGap     : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthlg;		break;
	case SimpleTypes::bordervalueThinThickMediumGap    : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthmg;		break;
	case SimpleTypes::bordervalueThinThickSmallGap     : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthsg;		break;
	case SimpleTypes::bordervalueThinThickThinLargeGap : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthtnlg;	break;
	case SimpleTypes::bordervalueThinThickThinMediumGap: oOutputBorder.m_eType = RtfBorder::bt_brdrtnthtnmg;	break;
	case SimpleTypes::bordervalueThinThickThinSmallGap : oOutputBorder.m_eType = RtfBorder::bt_brdrtnthtnsg;	break;
	case SimpleTypes::bordervalueThreeDEmboss          : oOutputBorder.m_eType = RtfBorder::bt_brdremboss;		break;
	case SimpleTypes::bordervalueThreeDEngrave         : oOutputBorder.m_eType = RtfBorder::bt_brdrengrave;		break;
		//case SimpleTypes::bordervalueTornPaper             : return L"tornPaper";
		//case SimpleTypes::bordervalueTornPaperBlack        : return L"tornPaperBlack";
		//case SimpleTypes::bordervalueTrees                 : return L"trees";
		//case SimpleTypes::bordervalueTriangle1             : return L"triangle1";
		//case SimpleTypes::bordervalueTriangle2             : return L"triangle2";
		//case SimpleTypes::bordervalueTriangleCircle1       : return L"triangleCircle1";
		//case SimpleTypes::bordervalueTriangleCircle2       : return L"triangleCircle2";
		//case SimpleTypes::bordervalueTriangleParty         : return L"triangleParty";
		//case SimpleTypes::bordervalueTriangles             : return L"triangles";
	case SimpleTypes::bordervalueTriple                : oOutputBorder.m_eType = RtfBorder::bt_brdrtriple;		break;
		//case SimpleTypes::bordervalueTwistedLines1         : return L"twistedLines1";
		//case SimpleTypes::bordervalueTwistedLines2         : return L"twistedLines2";
		//case SimpleTypes::bordervalueVine                  : return L"vine";
	case SimpleTypes::bordervalueWave                  : oOutputBorder.m_eType = RtfBorder::bt_brdrwavy;		break;
		//case SimpleTypes::bordervalueWaveline              : return L"waveline";
		//case SimpleTypes::bordervalueWeavingAngles         : return L"weavingAngles";
		//case SimpleTypes::bordervalueWeavingBraid          : return L"weavingBraid";
		//case SimpleTypes::bordervalueWeavingRibbon         : return L"weavingRibbon";
		//case SimpleTypes::bordervalueWeavingStrips         : return L"weavingStrips";
		//case SimpleTypes::bordervalueWhiteFlowers          : return L"whiteFlowers";
		//case SimpleTypes::bordervalueWoodwork              : return L"woodwork";
		//case SimpleTypes::bordervalueXIllusions            : return L"xIllusions";
		//case SimpleTypes::bordervalueZanyTriangles         : return L"zanyTriangles";
		//case SimpleTypes::bordervalueZigZag                : return L"zigZag";
		//case SimpleTypes::bordervalueZigZagStitch          : return L"zigZagStitch";

		//oOutputBorder.m_eType = RtfBorder::bt_brdrsh;
		//oOutputBorder.m_eType = RtfBorder::bt_brdrhair;
		//default												: oOutputBorder.m_eType = RtfBorder::bt_brdrs;			break;

	}

	if (type_border >=0 && oOutputBorder.m_eType == PROP_DEF)
		oOutputBorder.m_eType = RtfBorder::bt_brdrs;

	return true;
}
