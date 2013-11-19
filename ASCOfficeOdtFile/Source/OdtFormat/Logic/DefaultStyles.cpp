
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DefaultStyles.h"
#include <algorithm>
#include <boost/bind.hpp>


namespace Odt
{
	namespace Logic
	{

		DefaultStyles::DefaultStyles()
		{
		}


		DefaultStyles::~DefaultStyles()
		{
		}


		DefaultStyles::DefaultStyles(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DefaultStyles& DefaultStyles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DefaultStyles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "default-style");
		}


		const bool DefaultStyles::exist(const std::string& family) const
		{
			return std::find_if(begin(), end(), boost::bind(&DefaultStyle::Family, _1) == family) != end();
		}

		
		const DefaultStyle& DefaultStyles::operator[](const std::string& family) const
		{
			return find(family);
		}
		
		
		DefaultStyle& DefaultStyles::operator[](const std::string& family)
		{
			return find(family);
		}


		const DefaultStyle& DefaultStyles::find(const std::string& family) const
		{
			return *Odt::find_if(begin(), end(), boost::bind(&DefaultStyle::Family, _1) == family);
			//return *std::find_if(begin(), end(), boost::bind(&DefaultStyle::Family, _1) == family);
		}

		
		DefaultStyle& DefaultStyles::find(const std::string& family)
		{
			return *Odt::find_if(begin(), end(), boost::bind(&DefaultStyle::Family, _1) == family);
			//return *std::find_if(begin(), end(), boost::bind(&DefaultStyle::Family, _1) == family);
		}

	} // namespace Logic
} // namespace Odt