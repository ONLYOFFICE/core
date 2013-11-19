#pragma once

#include "IMapping.h"

namespace DocFileFormat
{
	struct IVisitable
	{
		virtual void Convert(IMapping* mapping)
		{
			mapping->Apply(this);
		}

		virtual ~IVisitable() {};
	};
}