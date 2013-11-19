#pragma once
#ifndef ODT_SETTINGS_CONFIG_ITEM_BOOL_INCLUDE_H_
#define ODT_SETTINGS_CONFIG_ITEM_BOOL_INCLUDE_H_

#include "./../WritingElement.h"


namespace Odt
{
	namespace Settings
	{
		class ConfigItemBool : public WritingElement
		{
		public:
			ConfigItemBool();
			virtual ~ConfigItemBool();
			explicit ConfigItemBool(const XML::XNode& node);
			const ConfigItemBool& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_name;
			bool				m_value;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTINGS_CONFIG_ITEM_BOOL_INCLUDE_H_