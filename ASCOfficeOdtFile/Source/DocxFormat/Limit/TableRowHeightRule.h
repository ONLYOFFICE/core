#pragma once
#ifndef OOX_LIMIT_TABLE_ROW_HEIGHT_RULE_INCLUDE_H_
#define OOX_LIMIT_TABLE_ROW_HEIGHT_RULE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class TableRowHeightRule : public setter::from<std::string>
		{
		public:
			TableRowHeightRule();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_TABLE_ROW_HEIGHT_RULE_INCLUDE_H_