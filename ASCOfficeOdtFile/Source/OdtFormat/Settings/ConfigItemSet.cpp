
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ConfigItemSet.h"


namespace Odt
{
	namespace Settings
	{

		ConfigItemSet::ConfigItemSet()
		{
		}


		ConfigItemSet::~ConfigItemSet()
		{
		}


		ConfigItemSet::ConfigItemSet(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ConfigItemSet& ConfigItemSet::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ConfigItemSet::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name = element.attribute("name").value();
			m_configItemMap = element.element("config-item-map-indexed");
			XML::Fill(m_configItems, element, "config-item");
		}


		const XML::XNode ConfigItemSet::toXML() const
		{
			return
				XML::XElement(ns.config + "config-item-set", 
					XML::XAttribute(ns.config + "name", m_name) +
					XML::Write(m_configItems) +
					XML::Write(m_configItemMap)
				);
		}

	} // namespace Settings
} // namespace Odt