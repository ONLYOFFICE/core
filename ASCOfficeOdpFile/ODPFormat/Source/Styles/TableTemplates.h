#pragma once
#ifndef ODP_STYLES_TABLE_TEMPLATES_INCLUDE_H_
#define ODP_STYLES_TABLE_TEMPLATES_INCLUDE_H_

#include "OdtFormat/WritingVector.h"
#include "TableTemplate.h"

namespace Odp
{
	namespace Styles
	{
		class TableTemplates : public Odt::WritingVector<TableTemplate>
		{
		public:
			TableTemplates();
			virtual ~TableTemplates();
			explicit TableTemplates(const XML::XNode& node);
			const TableTemplates& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& name) const;
			const TableTemplate& operator[](const std::string& name) const;
			TableTemplate& operator[](const std::string& name);
			const TableTemplate& find(const std::string& name) const;
			TableTemplate& find(const std::string& name);
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_TABLE_TEMPLATES_INCLUDE_H_