
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ConfigItemInt.h"


namespace Odt
{
	namespace Settings
	{

		ConfigItemInt::ConfigItemInt()
		{
		}


		ConfigItemInt::~ConfigItemInt()
		{
		}


		ConfigItemInt::ConfigItemInt(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ConfigItemInt& ConfigItemInt::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ConfigItemInt::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name	= element.attribute("name").value();
		  m_value = element.text();
		}


		const XML::XNode ConfigItemInt::toXML() const
		{
			return 
				XML::XElement(ns.config + "config-item",
					XML::XAttribute(ns.config + "name", m_name) +
					XML::XAttribute(ns.config + "type", "int") +
					XML::XText(m_value)
				);
		}

	} // namespace Settings
} // namespace Odt