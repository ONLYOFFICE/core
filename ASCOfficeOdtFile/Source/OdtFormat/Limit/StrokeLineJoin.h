#pragma once
#ifndef ODT_LIMIT_STROKE_LINE_JOIN_INCLUDE_H_
#define ODT_LIMIT_STROKE_LINE_JOIN_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class StrokeLineJoin : public setter::from<std::string>
		{
		public:
			StrokeLineJoin();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_STROKE_LINE_JOIN_INCLUDE_H_