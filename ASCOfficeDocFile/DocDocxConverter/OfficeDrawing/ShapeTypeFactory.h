#pragma once

#include "ShapeType.h"

namespace DocFileFormat
{
	struct ShapeTypeFactory
	{
		static ShapeType* NewShapeType (MSOSPT TypeCode);
	};
}