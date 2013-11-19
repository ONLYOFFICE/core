
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "PageLayouts.h"
#include "Exception/log_runtime_error.h"
#include <algorithm>
#include <boost/bind.hpp>


namespace Odt
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


		PageLayout& PageLayouts::front()
		{
			if (m_items.empty())
				throw log_runtime_error("empty PageLayouts");
			return m_items.front();
		}


		const PageLayout& PageLayouts::front() const
		{
			if (m_items.empty())
				throw log_runtime_error("empty PageLayouts");
			return m_items.front();
		}


		const bool PageLayouts::exist(const std::string& name) const
		{
			return std::find_if(begin(), end(), boost::bind(&PageLayout::Name, _1) == name) != end();
		}


		PageLayout& PageLayouts::find(const std::string& name)
		{
			return *Odt::find_if(begin(), end(), boost::bind(&PageLayout::Name, _1) == name);
			//return *std::find_if(begin(), end(), boost::bind(&PageLayout::Name, _1) == name);
		}


		const PageLayout& PageLayouts::find(const std::string& name) const
		{
			return *Odt::find_if(begin(), end(), boost::bind(&PageLayout::Name, _1) == name);
			//return *std::find_if(begin(), end(), boost::bind(&PageLayout::Name, _1) == name);
		}

	} // namespace Styles
} // namespace Odt