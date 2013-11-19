#pragma once
#ifndef ODT_SETTINGS_CONFIG_ITEM_MAP_ENTRY_INCLUDE_H_
#define ODT_SETTINGS_CONFIG_ITEM_MAP_ENTRY_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "ConfigItem.h"


namespace Odt
{
	namespace Settings
	{
		class ConfigItemMapEntry : public WritingElement
		{
		public:
			ConfigItemMapEntry();
			virtual ~ConfigItemMapEntry();
			explicit ConfigItemMapEntry(const XML::XNode& node);
			const ConfigItemMapEntry& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<ConfigItem>		m_configItems;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTINGS_CONFIG_ITEM_MAP_ENTRY_INCLUDE_H_