
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ConfigItemMapIndexed.h"


namespace Odt
{
	namespace Settings
	{

		ConfigItemMapIndexed::ConfigItemMapIndexed()
		{
		}


		ConfigItemMapIndexed::~ConfigItemMapIndexed()
		{
		}


		ConfigItemMapIndexed::ConfigItemMapIndexed(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ConfigItemMapIndexed& ConfigItemMapIndexed::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ConfigItemMapIndexed::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name	= element.attribute("name").value();
			m_map		= element.element("config-item-map-entry");
		}


		const XML::XNode ConfigItemMapIndexed::toXML() const
		{
			return
				XML::XElement(ns.config + "config-item-map-indexed",
					XML::XAttribute(ns.config + "name", m_name) +
					XML::Write(m_map)
				);
		}

	} // namespace Settings
} // namespace Odt