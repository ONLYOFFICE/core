#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
	class PentagonType : public ShapeType
	{
	public:
		PentagonType () : ShapeType(msosptPentagon)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
			Path				=	_T("m10800,l,8259,4200,21600r13200,l21600,8259xe");

			ConnectorLocations	=	_T( "10800,0;0,8259;4200,21600;10800,21600;17400,21600;21600,8259" );
			ConnectorAngles		=	_T( "270,180,90,90,90,0" );
			TextBoxRectangle	=	_T( "4200,5077,17400,21600" );
		}

		virtual ~PentagonType()
		{
		}
	};
}