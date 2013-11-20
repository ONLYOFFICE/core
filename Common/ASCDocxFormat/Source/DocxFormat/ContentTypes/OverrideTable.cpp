
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "OverrideTable.h"


namespace OOX
{
	namespace ContentTypes
	{

		OverrideTable::OverrideTable()
		{
		}


		OverrideTable::~OverrideTable()
		{
		}


		OverrideTable::OverrideTable(const XML::XNode& node)
		{
			fromXML(node);
		}


		const OverrideTable& OverrideTable::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void OverrideTable::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "Override");
		}


		void OverrideTable::add(const std::string& type, const boost::filesystem::wpath& path)
		{
			m_items.push_back(Override(type, path));
		}

	} // namespace ContentTypes
} // namespace OOX