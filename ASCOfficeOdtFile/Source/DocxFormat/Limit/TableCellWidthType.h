#pragma once
#ifndef OOX_LIMIT_TABLE_CELL_WIDTH_TYPE_INCLUDE_H_
#define OOX_LIMIT_TABLE_CELL_WIDTH_TYPE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class TableCellWidthType : public setter::from<std::string>
		{
		public:
			TableCellWidthType();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_TABLE_CELL_WIDTH_TYPE_INCLUDE_H_