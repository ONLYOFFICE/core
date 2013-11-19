#pragma once
#ifndef ODT_SETTINGS_CONFIG_ITEM_INT_INCLUDE_H_
#define ODT_SETTINGS_CONFIG_ITEM_INT_INCLUDE_H_

#include "./../WritingElement.h"


namespace Odt
{
	namespace Settings
	{
		class ConfigItemInt : public WritingElement
		{
		public:
			ConfigItemInt();
			virtual ~ConfigItemInt();
			explicit ConfigItemInt(const XML::XNode& node);
			const ConfigItemInt& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string	m_name;
			int					m_value;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTINGS_CONFIG_ITEM_INT_INCLUDE_H_