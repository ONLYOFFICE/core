#pragma once

namespace DocFileFormat
{
    class IVisitable;

    class IMapping
	{
     public:
		virtual void Apply(IVisitable* visited) = 0;
        virtual ~IMapping()
        {
        }
	};
}
