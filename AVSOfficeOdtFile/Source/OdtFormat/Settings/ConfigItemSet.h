#pragma once
#ifndef ODT_SETTINGS_CONFIG_ITEM_SET_INCLUDE_H_
#define ODT_SETTINGS_CONFIG_ITEM_SET_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include <string>
#include "nullable.h"
#include "ConfigItem.h"
#include "ConfigItemMapIndexed.h"


namespace Odt
{
	namespace Settings
	{
		class ConfigItemSet : public WritingElement
		{
		public:
			ConfigItemSet();
			virtual ~ConfigItemSet();
			explicit ConfigItemSet(const XML::XNode& node);
			const ConfigItemSet& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_name;
			std::vector<ConfigItem>					m_configItems;
			nullable<ConfigItemMapIndexed>	m_configItemMap;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTINGS_CONFIG_ITEM_SET_INCLUDE_H_