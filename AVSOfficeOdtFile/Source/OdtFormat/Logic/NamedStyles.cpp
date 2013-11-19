
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "NamedStyles.h"
#include <algorithm>
#include <boost/bind.hpp>


namespace Odt
{
	namespace Logic
	{

		NamedStyles::NamedStyles()
		{
		}


		NamedStyles::~NamedStyles()
		{
		}


		NamedStyles::NamedStyles(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NamedStyles& NamedStyles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NamedStyles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "style");
		}


		const bool NamedStyles::exist(const std::string& name) const
		{
			return std::find_if(begin(), end(), boost::bind(&NamedStyle::Name, _1) == name) != end();
		}


		const NamedStyle& NamedStyles::operator[](const std::string& name) const
		{
			return find(name);
		}


		NamedStyle& NamedStyles::operator[](const std::string& name)
		{
			return find(name);
		}


		const NamedStyle& NamedStyles::find(const std::string& name) const
		{
			return *Odt::find_if(begin(), end(), boost::bind(&NamedStyle::Name, _1) == name);
			//return *std::find_if(begin(), end(), boost::bind(&NamedStyle::Name, _1) == name);
		}


		NamedStyle& NamedStyles::find(const std::string& name)
		{
			return *Odt::find_if(begin(), end(), boost::bind(&NamedStyle::Name, _1) == name);
			//return *std::find_if(begin(), end(), boost::bind(&NamedStyle::Name, _1) == name);
		}

	} // namespace Logic
} // namespace Odt