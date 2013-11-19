#pragma once
#ifndef ODT_CONTENT_TABLE_CELL_ITEM_BASE_INCLUDE_H_
#define ODT_CONTENT_TABLE_CELL_ITEM_BASE_INCLUDE_H_

#include "./../WritingElement.h"


namespace Odt
{
	namespace Content
	{
		class TableCellItemBase : public WritingElement
		{
		public:
			virtual const bool hasNote() const = 0;
			virtual const bool hasList() const = 0;

			virtual void setNumCol(const int num) = 0;
			virtual const int getNumCol() const = 0;

		public:
			int m_numCol;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_CELL_ITEM_BASE_INCLUDE_H_