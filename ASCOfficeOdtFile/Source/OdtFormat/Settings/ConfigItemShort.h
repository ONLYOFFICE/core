#pragma once
#ifndef ODT_SETTINGS_CONFIG_ITEM_SHORT_INCLUDE_H_
#define ODT_SETTINGS_CONFIG_ITEM_SHORT_INCLUDE_H_

#include "./../WritingElement.h"


namespace Odt
{
	namespace Settings
	{
		class ConfigItemShort : public WritingElement
		{
		public:
			ConfigItemShort();
			virtual ~ConfigItemShort();
			explicit ConfigItemShort(const XML::XNode& node);
			const ConfigItemShort& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_name;
			int					m_value;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTINGS_CONFIG_ITEM_SHORT_INCLUDE_H_