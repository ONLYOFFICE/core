#include "PageLayouts.h"
#include <algorithm>
#include <boost/bind.hpp>

namespace Odp
{
	namespace Styles
	{
		PageLayouts::PageLayouts()
		{
		}

		PageLayouts::~PageLayouts()
		{
		}

		PageLayouts::PageLayouts(const XML::XNode& node)
		{
			fromXML(node);
		}

		const PageLayouts& PageLayouts::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void PageLayouts::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "page-layout");
		}

		const bool PageLayouts::exist(const std::string& name) const
		{
			return std::find_if(m_items.begin(), m_items.end(), boost::bind(&PageLayout::Name, _1) == name) != m_items.end();
		}

		const PageLayout& PageLayouts::operator[](const std::string& name) const
		{
			return find(name);
		}

		PageLayout& PageLayouts::operator[](const std::string& name)
		{
			return find(name);
		}

		const PageLayout& PageLayouts::find(const std::string& name) const
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&PageLayout::Name, _1) == name);
		}

		PageLayout& PageLayouts::find(const std::string& name)
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&PageLayout::Name, _1) == name);
		}
	} // namespace Styles
} // namespace Odp