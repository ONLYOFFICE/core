#pragma once

#include "RectangleType.h"

namespace DocFileFormat
{
	class RoundedRectangleType: public RectangleType
	{
	public:
		RoundedRectangleType() : RectangleType()
		{
			TypeCode			=	msosptRoundRectangle;
			Joins				=	round;
			AdjustmentValues	=	_T( "5400" );
		}

		virtual ~RoundedRectangleType()
		{

		}
	};
}