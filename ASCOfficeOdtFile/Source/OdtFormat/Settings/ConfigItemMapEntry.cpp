
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ConfigItemMapEntry.h"


namespace Odt
{
	namespace Settings
	{

		ConfigItemMapEntry::ConfigItemMapEntry()
		{
		}


		ConfigItemMapEntry::~ConfigItemMapEntry()
		{
		}


		ConfigItemMapEntry::ConfigItemMapEntry(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ConfigItemMapEntry& ConfigItemMapEntry::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ConfigItemMapEntry::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_configItems, element);
		}


		const XML::XNode ConfigItemMapEntry::toXML() const
		{
			return 
				XML::XElement(ns.config + "config-item-map-entry",
					XML::Write(m_configItems)
				);
		}

	} // namespace Settings
} // namespace Odt