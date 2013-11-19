#pragma once
#ifndef ODT_SETTINGS_CONFIG_ITEM_MAP_INDEXED_INCLUDE_H_
#define ODT_SETTINGS_CONFIG_ITEM_MAP_INDEXED_INCLUDE_H_

#include "./../WritingElement.h"
#include "ConfigItemMapEntry.h"
#include <string>


namespace Odt
{
	namespace Settings
	{
		class ConfigItemMapIndexed : public WritingElement
		{
		public:
			ConfigItemMapIndexed();
			virtual ~ConfigItemMapIndexed();
			explicit ConfigItemMapIndexed(const XML::XNode& node);
			const ConfigItemMapIndexed& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string					m_name;
			ConfigItemMapEntry	m_map;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTINGS_CONFIG_ITEM_MAP_INDEXED_INCLUDE_H_