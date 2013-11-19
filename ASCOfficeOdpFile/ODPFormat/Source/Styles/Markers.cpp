#include "Markers.h"
#include <algorithm>
#include <boost/bind.hpp>

namespace Odp
{
	namespace Styles
	{
		Markers::Markers()
		{
		}

		Markers::~Markers()
		{
		}

		Markers::Markers(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Markers& Markers::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Markers::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "marker");
		}

		const bool Markers::exist(const std::string& name) const
		{
			return std::find_if(m_items.begin(), m_items.end(), boost::bind(&Marker::Name, _1) == name) != m_items.end();
		}

		const Marker& Markers::operator[](const std::string& name) const
		{
			return find(name);
		}

		Marker& Markers::operator[](const std::string& name)
		{
			return find(name);
		}

		const Marker& Markers::find(const std::string& name) const
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&Marker::Name, _1) == name);
		}
		
		Marker& Markers::find(const std::string& name)
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&Marker::Name, _1) == name);
		}
	} // namespace Styles
} // namespace Odp