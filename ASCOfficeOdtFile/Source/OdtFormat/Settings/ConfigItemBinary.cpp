
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ConfigItemBinary.h"


namespace Odt
{
	namespace Settings
	{

		ConfigItemBinary::ConfigItemBinary()
		{
		}


		ConfigItemBinary::~ConfigItemBinary()
		{
		}


		ConfigItemBinary::ConfigItemBinary(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ConfigItemBinary& ConfigItemBinary::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ConfigItemBinary::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name	= element.attribute("name").value();
			m_value = element.text();
		}


		const XML::XNode ConfigItemBinary::toXML() const
		{
			return 
				XML::XElement(ns.config + "config-item",
					XML::XAttribute(ns.config + "name", m_name) +
					XML::XAttribute(ns.config + "type", "base64Binary") +
					XML::XText(m_value)
				);
		}
	} // namespace Settings
} // namespace Odt