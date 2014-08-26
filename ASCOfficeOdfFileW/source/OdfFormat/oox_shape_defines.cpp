#include "precompiled_cpodf.h"

#include "Shapes\oox_shapeMaths.h"
#include "Shapes\oox_shapeSnipRoundRects.h"
#include "Shapes\oox_shapeCustoms.h"
#include "Shapes\oox_shapeStars.h"

#include "Shapes\oox_shapeAccentCallouts.h"
#include "Shapes\oox_shapeArrows.h"
#include "Shapes\oox_shapeCharts.h"
#include "Shapes\oox_shapePrimitives.h"
#include "Shapes\oox_shapeRibbons.h"
#include "Shapes\oox_shapeConnectors.h"
#include "Shapes\oox_shapeWordArt.h"

#include "..\..\..\Common\DocxFormat\Source\Common\SimpleTypes_Drawing.h"

namespace cpdoccore 
{

oox_shape_ptr oox_shape::create(int ooxPrstGeomType)
{
	switch (ooxPrstGeomType)
	{
	case SimpleTypes::shapetypeMathMultiply:	return boost::make_shared<oox_shape_mathMultiply>();
	case SimpleTypes::shapetypeSun:				return boost::make_shared<oox_shape_sun>();
	case SimpleTypes::shapetypeRound1Rect:		return boost::make_shared<oox_shape_round1Rect>();
	case SimpleTypes::shapetypeRound2DiagRect:	return boost::make_shared<oox_shape_round2DiagRect>();
	case SimpleTypes::shapetypeRound2SameRect:	return boost::make_shared<oox_shape_round2SameRect>();
	case SimpleTypes::shapetypeSnip1Rect:		return boost::make_shared<oox_shape_snip1Rect>();
	case SimpleTypes::shapetypeSnip2DiagRect:	return boost::make_shared<oox_shape_snip2DiagRect>();
	case SimpleTypes::shapetypeSnip2SameRect:	return boost::make_shared<oox_shape_snip2SameRect>();
	case SimpleTypes::shapetypeSnipRoundRect:	return boost::make_shared<oox_shape_snipRoundRect>();
	case SimpleTypes::shapetypeStar6:			return boost::make_shared<oox_shape_star6>();
	case SimpleTypes::shapetypeStar7:			return boost::make_shared<oox_shape_star7>();
	case SimpleTypes::shapetypeStar10:			return boost::make_shared<oox_shape_star10>();
	case SimpleTypes::shapetypeStar12:			return boost::make_shared<oox_shape_star12>();
	case SimpleTypes::shapetypeStar16:			return boost::make_shared<oox_shape_star16>();
	case SimpleTypes::shapetypeStar32:			return boost::make_shared<oox_shape_star32>();

	case SimpleTypes::shapetypePlaque:			return boost::make_shared<oox_shape_Plaque>();

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
	//case SimpleTypes::shapetypeStraightConnector1: return boost::make_shared<oox_shape_straightConnector1>();

	default:
		if (ooxPrstGeomType > 2000) return boost::make_shared<oox_shape_textPlain>();
		else return boost::make_shared<oox_shape>();
		
	}
}
}