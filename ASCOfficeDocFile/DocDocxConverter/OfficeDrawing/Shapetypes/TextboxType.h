#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
	class TextboxType: public ShapeType
	{
	public:
		TextboxType () : ShapeType(msosptTextBox)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,l,21600r21600,l21600,xe");

		}

		virtual ~TextboxType()
		{

		}
	};
}