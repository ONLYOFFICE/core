#pragma once
#ifndef ODT_SETTINGS_CONFIG_ITEM_INCLUDE_H_
#define ODT_SETTINGS_CONFIG_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include <boost/shared_ptr.hpp>


namespace Odt
{
	namespace Settings
	{
		class ConfigItem : public WritingElement
		{
		public:
			ConfigItem();
			virtual ~ConfigItem();
			explicit ConfigItem(const XML::XNode& node);
			const ConfigItem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			boost::shared_ptr<WritingElement> m_item;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTING_CONFIG_ITEM_INCLUDE_H_