#pragma once
#ifndef ODT_CONTENT_COVERED_TABLE_CELL_INCLUDE_H_
#define ODT_CONTENT_COVERED_TABLE_CELL_INCLUDE_H_

#include "TableCellItemBase.h"
#include <string>


namespace Odt
{
	namespace Content
	{
		class CoveredTableCell : public TableCellItemBase
		{
		public:
			CoveredTableCell();
			virtual ~CoveredTableCell();
			explicit CoveredTableCell(const XML::XNode& node);
			const CoveredTableCell& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool hasNote() const;
			const bool hasList() const;
		
		public:
			const int getNumCol() const;
			void setNumCol(const int num);

		public:
			property<int>		CountHMerge;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_COVERED_TABLE_CELL_INCLUDE_H_