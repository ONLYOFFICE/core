#pragma once
#ifndef ODT_SETTINGS_CONFIG_ITEM_STRING_INCLUDE_H_
#define ODT_SETTINGS_CONFIG_ITEM_STRING_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable.h"


namespace Odt
{
	namespace Settings
	{
		class ConfigItemString : public WritingElement
		{
		public:
			ConfigItemString();
			virtual ~ConfigItemString();
			explicit ConfigItemString(const XML::XNode& node);
			const ConfigItemString& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string							m_name;
			nullable<std::string>		m_value;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTINGS_CONFIG_ITEM_STRING_INCLUDE_H_