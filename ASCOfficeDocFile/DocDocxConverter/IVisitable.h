#pragma once

#include "IMapping.h"

namespace DocFileFormat
{
    class IVisitable
	{
     public:
		virtual void Convert(IMapping* mapping)
		{
			mapping->Apply(this);
		}

        virtual ~IVisitable()
        {
        }
	};
}
