#pragma once
#ifndef ODT_CONTENT_TABLE_COLUMN_INCLUDE_H_
#define ODT_CONTENT_TABLE_COLUMN_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"


namespace Odt
{
	namespace Content
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
			property<std::string>												Style;
			nullable_property<int, setter::only_positive<int> >	Repeated;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_COLUMN_INCLUDE_H_