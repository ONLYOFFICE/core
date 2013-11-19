
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ConfigItemBool.h"


namespace Odt
{
	namespace Settings
	{

		ConfigItemBool::ConfigItemBool()
		{
		}


		ConfigItemBool::~ConfigItemBool()
		{
		}


		ConfigItemBool::ConfigItemBool(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ConfigItemBool& ConfigItemBool::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ConfigItemBool::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name	= element.attribute("name").value();
		  m_value = element.text();
		}


		const XML::XNode ConfigItemBool::toXML() const
		{
			return 
				XML::XElement(ns.config + "config-item",
					XML::XAttribute(ns.config + "name", m_name) +
					XML::XAttribute(ns.config + "type", "boolean") +
					XML::XText(m_value)
				);
		}

	} // namespace Setting
} // namespace Odt