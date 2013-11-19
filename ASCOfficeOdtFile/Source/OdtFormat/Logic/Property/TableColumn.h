#pragma once
#ifndef ODT_LOGIC_PROPERTY_TABLE_COLUMN_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_TABLE_COLUMN_INCLUDE_H_

#include "./../../WritingElement.h"
#include "./../../Unit.h"
#include "nullable_property.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			class TableColumn : public WritingElement
			{
			public:
				TableColumn();
				virtual ~TableColumn();
				explicit TableColumn(const XML::XNode& node);
				const TableColumn& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				const bool isSimple() const;

			public:
				// TODO устаревшая
//				const TableColumn merge(const TableColumn& rhs) const;

			public:
				static const TableColumn merge(const TableColumn& lhs, const TableColumn& rhs);

			public:
				nullable_property<UnitCm, setter::between<UnitCm, 0, 100> >				Width;
				nullable_property<UnitMulti, setter::between<UnitMulti, 0, 100000 > >	RelWidth;
			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTY_TABLE_COLUMN_INCLUDE_H_