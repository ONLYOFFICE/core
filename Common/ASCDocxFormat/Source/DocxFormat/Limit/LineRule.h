#pragma once
#ifndef OOX_LIMIT_LINE_RULE_INCLUDE_H_
#define OOX_LIMIT_LINE_RULE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class LineRule : public setter::from<std::string>
		{
		public:
			LineRule();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_LINE_RULE_INCLUDE_H_