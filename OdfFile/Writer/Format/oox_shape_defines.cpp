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


#include "Shapes/oox_shapeMaths.h"
#include "Shapes/oox_shapeSnipRoundRects.h"
#include "Shapes/oox_shapeCustoms.h"
#include "Shapes/oox_shapeStars.h"

#include "Shapes/oox_shapeAccentCallouts.h"
#include "Shapes/oox_shapeArrows.h"
#include "Shapes/oox_shapeCharts.h"
#include "Shapes/oox_shapePrimitives.h"
#include "Shapes/oox_shapeRibbons.h"
#include "Shapes/oox_shapeConnectors.h"
#include "Shapes/oox_shapeWordArt.h"
#include "Shapes/oox_shapeCurvedArrows.h"
#include "Shapes/oox_shapeCallouts.h"
#include "Shapes/oox_shapeBents.h"

#include "../../../OOXML/Common/SimpleTypes_Drawing.h"
#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore 
{

oox_shape::oox_shape()
{
	odf_type_name	= L"ooxml-non-primitive";
}

oox_shape_ptr oox_shape::create(int ooxPrstGeomType)
{
	switch (ooxPrstGeomType)
	{
	case SimpleTypes::shapetypeMathMultiply:		return boost::make_shared<oox_shape_mathMultiply>();
	case SimpleTypes::shapetypeMathEqual:			return boost::make_shared<oox_shape_mathEqual>();
	case SimpleTypes::shapetypeMathPlus:			return boost::make_shared<oox_shape_mathPlus>();
	case SimpleTypes::shapetypeMathNotEqual:		return boost::make_shared<oox_shape_mathNotEqual>();
	case SimpleTypes::shapetypeMathDivide:			return boost::make_shared<oox_shape_mathDivide>();
	case SimpleTypes::shapetypeMathMinus:			return boost::make_shared<oox_shape_mathMinus>();
	
	case SimpleTypes::shapetypeRoundRect:			return boost::make_shared<oox_shape_roundRect>();
	case SimpleTypes::shapetypeRound1Rect:			return boost::make_shared<oox_shape_round1Rect>();
	case SimpleTypes::shapetypeRound2DiagRect:		return boost::make_shared<oox_shape_round2DiagRect>();
	case SimpleTypes::shapetypeRound2SameRect:		return boost::make_shared<oox_shape_round2SameRect>();
	case SimpleTypes::shapetypeSnip1Rect:			return boost::make_shared<oox_shape_snip1Rect>();
	case SimpleTypes::shapetypeSnip2DiagRect:		return boost::make_shared<oox_shape_snip2DiagRect>();
	case SimpleTypes::shapetypeSnip2SameRect:		return boost::make_shared<oox_shape_snip2SameRect>();
	case SimpleTypes::shapetypeSnipRoundRect:		return boost::make_shared<oox_shape_snipRoundRect>();
	
	case SimpleTypes::shapetypeStar6:				return boost::make_shared<oox_shape_star6>();
	case SimpleTypes::shapetypeStar7:				return boost::make_shared<oox_shape_star7>();
	case SimpleTypes::shapetypeStar10:				return boost::make_shared<oox_shape_star10>();
	case SimpleTypes::shapetypeStar12:				return boost::make_shared<oox_shape_star12>();
	case SimpleTypes::shapetypeStar16:				return boost::make_shared<oox_shape_star16>();
	case SimpleTypes::shapetypeStar32:				return boost::make_shared<oox_shape_star32>();
	
	case SimpleTypes::shapetypeSun:					return boost::make_shared<oox_shape_sun>();
	case SimpleTypes::shapetypeMoon:				return boost::make_shared<oox_shape_moon>();
	case SimpleTypes::shapetypeLightningBolt:		return boost::make_shared<oox_shape_lightningBolt>();
	case SimpleTypes::shapetypeHeart:				return boost::make_shared<oox_shape_heart>();
	
	case SimpleTypes::shapetypeCircularArrow:		return boost::make_shared<oox_shape_CircularArrow>();
	case SimpleTypes::shapetypeCurvedLeftArrow:		return boost::make_shared<oox_shape_CurvedLeftArrow>();
	case SimpleTypes::shapetypeCurvedRightArrow:	return boost::make_shared<oox_shape_CurvedRightArrow>();
	case SimpleTypes::shapetypeCurvedDownArrow:		return boost::make_shared<oox_shape_CurvedDownArrow>();
	case SimpleTypes::shapetypeCurvedUpArrow:		return boost::make_shared<oox_shape_CurvedUpArrow>();
	case SimpleTypes::shapetypeLeftCircularArrow:	return boost::make_shared<oox_shape_LeftCircularArrow>();
	case SimpleTypes::shapetypeSwooshArrow:			return boost::make_shared<oox_shape_SwooshArrow>();
	case SimpleTypes::shapetypeLeftArrow:			return boost::make_shared<oox_shape_LeftArrow>();
	case SimpleTypes::shapetypeLeftRightArrow:		return boost::make_shared<oox_shape_LeftRightArrow>();
	case SimpleTypes::shapetypeLeftUpArrow:			return boost::make_shared<oox_shape_LeftUpArrow>();
	case SimpleTypes::shapetypeUpDownArrow:			return boost::make_shared<oox_shape_UpDownArrow>();
	case SimpleTypes::shapetypeRightArrow:			return boost::make_shared<oox_shape_RightArrow>();
	case SimpleTypes::shapetypeDownArrow:			return boost::make_shared<oox_shape_DownArrow>();
	case SimpleTypes::shapetypeUpArrow:				return boost::make_shared<oox_shape_UpArrow>();
	case SimpleTypes::shapetypeNotchedRightArrow:	return boost::make_shared<oox_shape_NotchedRightArrow>();
	case SimpleTypes::shapetypeQuadArrowCallout:	return boost::make_shared<oox_shape_QuadArrowCallout>();

	case SimpleTypes::shapetypeFunnel:				return boost::make_shared<oox_shape_Funnel>();
	case SimpleTypes::shapetypeGear6:				return boost::make_shared<oox_shape_Gear6>();
	case SimpleTypes::shapetypeGear9:				return boost::make_shared<oox_shape_Gear9>();
	case SimpleTypes::shapetypeBlockArc:			return boost::make_shared<oox_shape_BlockArc>();
	case SimpleTypes::shapetypeCorner:				return boost::make_shared<oox_shape_Corner>();
	case SimpleTypes::shapetypeChord:				return boost::make_shared<oox_shape_Chord>();
	case SimpleTypes::shapetypeChevron:				return boost::make_shared<oox_shape_Chevron>();
	case SimpleTypes::shapetypeHomePlate:			return boost::make_shared<oox_shape_HomePlate>();
	case SimpleTypes::shapetypeBevel:				return boost::make_shared<oox_shape_Bevel>();
	case SimpleTypes::shapetypeArc:					return boost::make_shared<oox_shape_Arc>();
	case SimpleTypes::shapetypeTrapezoid:			return boost::make_shared<oox_shape_Trapezoid>();
	case SimpleTypes::shapetypeTriangle:			return boost::make_shared<oox_shape_Triangle>();
	case SimpleTypes::shapetypeRtTriangle:			return boost::make_shared<oox_shape_RtTriangle>();
	case SimpleTypes::shapetypeParallelogram:		return boost::make_shared<oox_shape_Parallelogram>();
	case SimpleTypes::shapetypeDiamond:				return boost::make_shared<oox_shape_Diamond>();
	case SimpleTypes::shapetypePlus:				return boost::make_shared<oox_shape_Plus>();

	case SimpleTypes::shapetypePlaque:				return boost::make_shared<oox_shape_Plaque>();
	case SimpleTypes::shapetypeDoubleWave:			return boost::make_shared<oox_shape_DoubleWave>();
	case SimpleTypes::shapetypePieWedge:			return boost::make_shared<oox_shape_PieWedge>();
	case SimpleTypes::shapetypePie:					return boost::make_shared<oox_shape_Pie>();
	case SimpleTypes::shapetypeDonut:				return boost::make_shared<oox_shape_Donut>();	
	case SimpleTypes::shapetypeLeftRightRibbon:		return boost::make_shared<oox_shape_LeftRightRibbon>();	
	case SimpleTypes::shapetypeWave:				return boost::make_shared<oox_shape_Wave>();	
	case SimpleTypes::shapetypeFoldedCorner:		return boost::make_shared<oox_shape_FoldedCorner>();
	case SimpleTypes::shapetypeCan:					return boost::make_shared<oox_shape_Can>();

	case SimpleTypes::shapetypeFlowChartExtract:	return boost::make_shared<oox_shape_FlowChartExtract>();	

	case SimpleTypes::shapetypeLeftArrowCallout:	return boost::make_shared<oox_shape_LeftArrowCallout>();	
	case SimpleTypes::shapetypeRightArrowCallout:	return boost::make_shared<oox_shape_RightArrowCallout>();	
	case SimpleTypes::shapetypeUpArrowCallout:		return boost::make_shared<oox_shape_UpArrowCallout>();	
	case SimpleTypes::shapetypeDownArrowCallout:	return boost::make_shared<oox_shape_DownArrowCallout>();		

	case SimpleTypes::shapetypeBentUpArrow:			return boost::make_shared<oox_shape_BentUpArrow>();	
	case SimpleTypes::shapetypeLeftRightUpArrow:	return boost::make_shared<oox_shape_leftRightUpArrow>();	

	case SimpleTypes::shapetypeStraightConnector1:	return boost::make_shared<oox_shape_StraightConnector1>();
	case SimpleTypes::shapetypeFrame:				return boost::make_shared<oox_shape_Frame>();
	case SimpleTypes::shapetypePentagon:			return boost::make_shared<oox_shape_Pentagon>();
	case SimpleTypes::shapetypeOctagon:				return boost::make_shared<oox_shape_Octagon>();
	case SimpleTypes::shapetypeHexagon:				return boost::make_shared<oox_shape_Hexagon>();
	case SimpleTypes::shapetypeHeptagon:			return boost::make_shared<oox_shape_Heptagon>();
	case SimpleTypes::shapetypeDecagon:				return boost::make_shared<oox_shape_Decagon>();
	case SimpleTypes::shapetypeDodecagon:			return boost::make_shared<oox_shape_Dodecagon>();
	case SimpleTypes::shapetypeCube:				return boost::make_shared<oox_shape_Cube>();

	case SimpleTypes::shapetypeCallout1:			return boost::make_shared<oox_shape_Callout1>();
	case SimpleTypes::shapetypeCallout2:			return boost::make_shared<oox_shape_Callout2>();
//	case SimpleTypes::shapetypeCallout3:			return boost::make_shared<oox_shape_Callout3>();

	case SimpleTypes::shapetypeAccentCallout1:		return boost::make_shared<oox_shape_AccentCallout1>();
	case SimpleTypes::shapetypeAccentCallout2:		return boost::make_shared<oox_shape_AccentCallout2>();
	case SimpleTypes::shapetypeAccentCallout3:		return boost::make_shared<oox_shape_AccentCallout3>();

	case SimpleTypes::shapetypeBorderCallout1:		return boost::make_shared<oox_shape_BorderCallout1>();
	case SimpleTypes::shapetypeBorderCallout2:		return boost::make_shared<oox_shape_BorderCallout2>();
	case SimpleTypes::shapetypeBorderCallout3:		return boost::make_shared<oox_shape_BorderCallout3>();

	case SimpleTypes::shapetypeAccentBorderCallout1:	return boost::make_shared<oox_shape_AccentBorderCallout1>();
	case SimpleTypes::shapetypeAccentBorderCallout2:	return boost::make_shared<oox_shape_AccentBorderCallout2>();
	case SimpleTypes::shapetypeAccentBorderCallout3:	return boost::make_shared<oox_shape_AccentBorderCallout3>();

	case SimpleTypes::shapetypeCloudCallout:		return boost::make_shared<oox_shape_CloudCallout>();
	case SimpleTypes::shapetypeWedgeRectCallout:	return boost::make_shared<oox_shape_WedgeRectCallout>();
	case SimpleTypes::shapetypeWedgeRoundRectCallout:	return boost::make_shared<oox_shape_WedgeRoundRectCallout>();
	case SimpleTypes::shapetypeWedgeEllipseCallout:	return boost::make_shared<oox_shape_WedgeEllipseCallout>();

	case SimpleTypes::shapetypeRibbon:				return boost::make_shared<oox_shape_Ribbon>();
	case SimpleTypes::shapetypeRibbon2:				return boost::make_shared<oox_shape_Ribbon2>();
	case SimpleTypes::shapetypeEllipseRibbon:		return boost::make_shared<oox_shape_EllipseRibbon>();
	case SimpleTypes::shapetypeEllipseRibbon2:		return boost::make_shared<oox_shape_EllipseRibbon2>();

	case SimpleTypes::shapetypeRightBrace:			return boost::make_shared<oox_shape_RightBrace>();
	case SimpleTypes::shapetypeLeftBrace:			return boost::make_shared<oox_shape_LeftBrace>();
	case SimpleTypes::shapetypeRightBracket:		return boost::make_shared<oox_shape_RightBracket>();
	case SimpleTypes::shapetypeLeftBracket:			return boost::make_shared<oox_shape_LeftBracket>();
	case SimpleTypes::shapetypeBracePair:			return boost::make_shared<oox_shape_BracePair>();
	case SimpleTypes::shapetypeBracketPair:			return boost::make_shared<oox_shape_BracketPair>();
	//case (2001 + SimpleTypes::textshapetypeTextArchDown): 
	//case (2001 + SimpleTypes::textshapetypeTextArchDownPour): 
	//case (2001 + SimpleTypes::textshapetypeTextArchUp): 
	//case (2001 + SimpleTypes::textshapetypeTextArchUpPour): 
	//case (2001 + SimpleTypes::textshapetypeTextButton): 
	//case (2001 + SimpleTypes::textshapetypeTextButtonPour): 
	//case (2001 + SimpleTypes::textshapetypeTextCanDown): 
	//case (2001 + SimpleTypes::textshapetypeTextCanUp): 
	//case (2001 + SimpleTypes::textshapetypeTextCascadeDown): 
	//case (2001 + SimpleTypes::textshapetypeTextCascadeUp): 
	//case (2001 + SimpleTypes::textshapetypeTextChevron): 
	//case (2001 + SimpleTypes::textshapetypeTextChevronInverted): 
	//case (2001 + SimpleTypes::textshapetypeTextCircle): 
	case (2001 + SimpleTypes::textshapetypeTextCirclePour): return boost::make_shared<oox_shape_textCirclePour>();
	//case (2001 + SimpleTypes::textshapetypeTextCurveDown): 
	//case (2001 + SimpleTypes::textshapetypeTextCurveUp): 
	//case (2001 + SimpleTypes::textshapetypeTextDeflate): 
	//case (2001 + SimpleTypes::textshapetypeTextDeflateBottom): 
	//case (2001 + SimpleTypes::textshapetypeTextDeflateInflate): 
	//case (2001 + SimpleTypes::textshapetypeTextDeflateInflateDeflate): 
	//case (2001 + SimpleTypes::textshapetypeTextDeflateTop): 
	//case (2001 + SimpleTypes::textshapetypeTextDoubleWave1): 
	//case (2001 + SimpleTypes::textshapetypeTextFadeDown): 
	//case (2001 + SimpleTypes::textshapetypeTextFadeLeft): 
	//case (2001 + SimpleTypes::textshapetypeTextFadeRight): 
	//case (2001 + SimpleTypes::textshapetypeTextFadeUp): 
	//case (2001 + SimpleTypes::textshapetypeTextInflate): 
	//case (2001 + SimpleTypes::textshapetypeTextInflateBottom): 
	//case (2001 + SimpleTypes::textshapetypeTextInflateTop): 
	case (2001 + SimpleTypes::textshapetypeTextPlain): return boost::make_shared<oox_shape_textPlain>();
	//case (2001 + SimpleTypes::textshapetypeTextRingInside): 
	//case (2001 + SimpleTypes::textshapetypeTextRingOutside): 
	//case (2001 + SimpleTypes::textshapetypeTextSlantDown): 
	case (2001 + SimpleTypes::textshapetypeTextSlantUp): return boost::make_shared<oox_shape_textSlantUp>();
	//case (2001 + SimpleTypes::textshapetypeTextStop): 
	//case (2001 + SimpleTypes::textshapetypeTextTriangle): 
	//case (2001 + SimpleTypes::textshapetypeTextTriangleInverted): 
	//case (2001 + SimpleTypes::textshapetypeTextWave1): 
	//case (2001 + SimpleTypes::textshapetypeTextWave2): 
	//case (2001 + SimpleTypes::textshapetypeTextWave4): 

	default:
		if (ooxPrstGeomType > 2000) return boost::make_shared<oox_shape_textPlain>();
		else return oox_shape_ptr();
		
	}
}
void oox_shape::add(std::wstring name,std::wstring frmla)
{
    XmlUtils::replace_all(name, L"gd", L"f");
    _equation q = {name, frmla};
    equations.push_back(q);
}
}
