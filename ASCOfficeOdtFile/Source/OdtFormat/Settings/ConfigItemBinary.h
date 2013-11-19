#pragma once
#ifndef ODT_SETTINGS_CONFIG_ITEM_BINARY_INCLUDE_H_
#define ODT_SETTINGS_CONFIG_ITEM_BINARY_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"


namespace Odt
{
	namespace Settings
	{
		class ConfigItemBinary : public WritingElement
		{
		public:
			ConfigItemBinary();
			virtual ~ConfigItemBinary();
			explicit ConfigItemBinary(const XML::XNode& node);
			const ConfigItemBinary& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string						m_name;
			nullable<std::string>	m_value;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTINGS_CONFIG_ITEM_BINARY_INCLUDE_H_