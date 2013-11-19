
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ConfigItemString.h"


namespace Odt
{
	namespace Settings
	{

		ConfigItemString::ConfigItemString()
		{
		}


		ConfigItemString::~ConfigItemString()
		{
		}


		ConfigItemString::ConfigItemString(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ConfigItemString& ConfigItemString::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}
	

		void ConfigItemString::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name	= element.attribute("name").value();
			m_value = element.text();
		}


		const XML::XNode ConfigItemString::toXML() const
		{
			return
				XML::XElement(ns.config + "config-item",
					XML::XAttribute(ns.config + "name", m_name) +
					XML::XAttribute(ns.config + "type", "string") +
					XML::XText(m_value)
				);
		}

	} // namespace Settings
} // namespace Odt