#pragma once
#ifndef ODT_SETTINGS_SETTINGS_INCLUDE_H_
#define ODT_SETTINGS_SETTINGS_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "ConfigItemSet.h"


namespace Odt
{
	namespace Settings
	{
		class  Settings : public WritingElement
		{
		public:
			Settings();
			virtual ~Settings();
			explicit Settings(const XML::XNode& node);
			const Settings& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<ConfigItemSet>	m_configItemSets;
		};
	} // namespace Settings
} // namespace Odt

#endif // ODT_SETTINGS_SETTINGS_INCLUDE_H_