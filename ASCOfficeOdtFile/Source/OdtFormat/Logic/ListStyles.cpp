
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ListStyles.h"
#include <algorithm>
#include <boost/bind.hpp>


namespace Odt
{
	namespace Logic
	{

		ListStyles::ListStyles()
		{
		}


		ListStyles::~ListStyles()
		{
		}


		ListStyles::ListStyles(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ListStyles& ListStyles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ListStyles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "list-style");
		}


		const bool ListStyles::exist(const std::string& name) const
		{
			return std::find_if(begin(), end(), boost::bind(&ListStyle::Name, _1) == name) != end();
		}


		const ListStyle& ListStyles::operator[](const std::string& name) const
		{
			return find(name);
		}


		ListStyle& ListStyles::operator[](const std::string& name)
		{
			return find(name);
		}


		const ListStyle& ListStyles::find(const std::string& name) const
		{
			return *Odt::find_if(begin(), end(), boost::bind(&ListStyle::Name, _1) == name);
			//return *std::find_if(begin(), end(), boost::bind(&ListStyle::Name, _1) == name);
		}


		ListStyle& ListStyles::find(const std::string& name)
		{
			return *Odt::find_if(begin(), end(), boost::bind(&ListStyle::Name, _1) == name);
			//return *std::find_if(begin(), end(), boost::bind(&ListStyle::Name, _1) == name);
		}


		const int ListStyles::numStyleInVector(const std::string& name) const
		{
			for(const_iterator iter = begin(); iter != end(); ++iter)
			{
				if (iter->Name == name)
					return iter - begin() + 1;
			}
			return 0;
		}

	} // namespace Logic
} // namespace Odt