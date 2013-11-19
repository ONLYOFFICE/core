
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ConfigItemShort.h"


namespace Odt
{
	namespace Settings
	{

		ConfigItemShort::ConfigItemShort()
		{
		}


		ConfigItemShort::~ConfigItemShort()
		{
		}


		ConfigItemShort::ConfigItemShort(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ConfigItemShort& ConfigItemShort::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ConfigItemShort::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name	= element.attribute("name").value();
			m_value = element.text();
		}


		const XML::XNode ConfigItemShort::toXML() const
		{
			return
				XML::XElement(ns.config + "config-item",
					XML::XAttribute(ns.config + "name", m_name) +
					XML::XAttribute(ns.config + "type", "short") +
					XML::XText(m_value)
				);
		}

	} // namespace Settings
} // namespace Odt