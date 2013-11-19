#pragma once

namespace DocFileFormat
{
	struct IVisitable;

	struct IMapping
	{
		virtual void Apply(IVisitable* visited) = 0;
		virtual ~IMapping() {};
	};
}