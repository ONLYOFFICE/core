#include "TableTemplates.h"
#include <algorithm>
#include <boost/bind.hpp>

namespace Odp
{
	namespace Styles
	{
		TableTemplates::TableTemplates()
		{
		}

		TableTemplates::~TableTemplates()
		{
		}

		TableTemplates::TableTemplates(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableTemplates& TableTemplates::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableTemplates::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "table-template");
		}

		const bool TableTemplates::exist(const std::string& name) const
		{
			return std::find_if(m_items.begin(), m_items.end(), boost::bind(&TableTemplate::StyleName, _1) == name) != m_items.end();
		}

		const TableTemplate& TableTemplates::operator[](const std::string& name) const
		{
			return find(name);
		}

		TableTemplate& TableTemplates::operator[](const std::string& name)
		{
			return find(name);
		}

		const TableTemplate& TableTemplates::find(const std::string& name) const
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&TableTemplate::StyleName, _1) == name);
		}
		
		TableTemplate& TableTemplates::find(const std::string& name)
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&TableTemplate::StyleName, _1) == name);
		}
	} // namespace Styles
} // namespace Odp