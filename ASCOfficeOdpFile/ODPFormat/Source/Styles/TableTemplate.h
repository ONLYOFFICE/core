#pragma once
#ifndef ODP_STYLES_TABLE_TEMPLATE_INCLUDE_H_
#define ODP_STYLES_TABLE_TEMPLATE_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include <string>
#include "property.h"

namespace Odp
{
	namespace Styles
	{
		class TableTemplate : public Odt::WritingElement
		{
		public:
			TableTemplate();
			virtual ~TableTemplate();
			explicit TableTemplate(const XML::XNode& node);
			const TableTemplate& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> StyleName;
			nullable_property<std::string> FirstRow;
			nullable_property<std::string> LastRow;
			nullable_property<std::string> FirstColumn;
			nullable_property<std::string> LastColumn;
			nullable_property<std::string> OddRows;
			nullable_property<std::string> OddColumns;
			nullable_property<std::string> Body;
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_TABLE_TEMPLATE_INCLUDE_H_