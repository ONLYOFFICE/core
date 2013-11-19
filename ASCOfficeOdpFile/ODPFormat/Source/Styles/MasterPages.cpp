#include "MasterPages.h"
#include <algorithm>
#include <boost/bind.hpp>

namespace Odp
{
	namespace Styles
	{
		MasterPages::MasterPages()
		{
		}

		MasterPages::~MasterPages()
		{
		}

		MasterPages::MasterPages(const XML::XNode& node)
		{
			fromXML(node);
		}

		const MasterPages& MasterPages::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void MasterPages::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "master-page");
		}

		const bool MasterPages::exist(const std::string& name) const
		{
			return std::find_if(m_items.begin(), m_items.end(), boost::bind(&MasterPage::Name, _1) == name) != m_items.end();
		}

		const MasterPage& MasterPages::operator[](const std::string& name) const
		{
			return find(name);
		}

		MasterPage& MasterPages::operator[](const std::string& name)
		{
			return find(name);
		}

		const MasterPage& MasterPages::find(const std::string& name) const
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&MasterPage::Name, _1) == name);
		}
		
		MasterPage& MasterPages::find(const std::string& name)
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&MasterPage::Name, _1) == name);
		}
	} // namespace Styles
} // namespace Odp