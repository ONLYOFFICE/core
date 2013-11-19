#pragma once
#ifndef ODT_STYLES_COLUMNS_INCLUDE_H_
#define ODT_STYLES_COLUMNS_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "Column.h"
#include "ColumnSeparator.h"
#include "nullable_property.h"
#include "property.h"
#include "Utility.h"


namespace Odt
{
	namespace Styles
	{
		class Columns : public WritingElement
		{
		public:
			Columns();
			virtual ~Columns();
			explicit Columns(const XML::XNode& node);
			const Columns& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int>						Count;
			property<std::vector<Column> >		Items;
			nullable_property<UniversalUnit>	Gap;
			nullable_property<ColumnSeparator>	Separator;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_COLUMNS_INCLUDE_H_