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

	//case SimpleTypes::shapetypeStraightConnector1: return boost::make_shared<oox_shape_straightConnector1>();

	default:
		return boost::make_shared<oox_shape>();
		
	}
}
}