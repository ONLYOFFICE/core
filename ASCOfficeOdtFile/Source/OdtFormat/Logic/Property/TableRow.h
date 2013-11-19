#pragma once
#ifndef ODT_LOGIC_PROPERTY_TABLE_ROW_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_TABLE_ROW_INCLUDE_H_

#include "./../../WritingElement.h"
#include "nullable_property.h"
#include <string>
#include "./../../Limit/KeepTogether.h"
#include "./../../Unit.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			class TableRow : public WritingElement
			{
			public:
				TableRow();
				virtual ~TableRow();
				explicit TableRow(const XML::XNode& node);
				const TableRow& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				const bool isSimple() const;

			public:
				nullable_property<UnitCm, setter::between<UnitCm, 0, 100> >	Height;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 100> >	MinHeight;
				nullable_property<std::string, Limit::KeepTogether>			KeepTogether;
				nullable_property<std::string, Limit::KeepTogether>			FoKeepTogether;

			public:
				static const TableRow merge(const TableRow& lhs, const TableRow& rhs);
			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTY_TABLE_ROW_INCLUDE_H_